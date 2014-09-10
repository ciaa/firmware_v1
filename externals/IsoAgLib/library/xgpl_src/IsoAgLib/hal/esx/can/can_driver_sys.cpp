/*
  can_driver_sys.cpp: CAN interface between BIOS/OS and IsoAgLib to
    concentrate CAN handling abstraction within one module

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/


#include "../typedef.h"
#include "../config.h"
#include "../errcodes.h"

#include <IsoAgLib/driver/can/impl/ident_c.h>
#include <IsoAgLib/driver/can/impl/canpkg_c.h>
#include <IsoAgLib/driver/system/impl/system_c.h>
#include <IsoAgLib/hal/generic_utils/can/canfifo_c.h>
#include <IsoAgLib/hal/generic_utils/can/canutils.h>
#include <IsoAgLib/hal/hal_can.h>

namespace __HAL {

  extern "C" {
#include <commercial_BIOS/bios_esx/xos20esx.h>
  }

  static const int16_t configTxBufferSize = 20;
  static const int16_t configRxBufferSize = 20;
  static const uint8_t configTxMsgObjNum = 1;
  static const uint8_t configRxStdMsgObjNum = 2;
  static const uint8_t configRxExtMsgObjNum = 3;

  static tCanObjConfig configData;
  static tSend sendData;


  extern "C" {
    __HAL::tCanMsgReg HUGE_MEM * rxIrq( byte bBus, byte bOjekt, __HAL::tCanMsgReg HUGE_MEM *a_reg ) {

      const __HAL::byte& cfg = static_cast<__HAL::byte>( a_reg->tCfg_D0.b[0] );
      const bool ext = ( cfg & 0x4 ) != 0;
      const unsigned len = ( cfg & 0xF0 ) >> 4;
      uint32_t id = ( uint32_t( ( uint16_t( a_reg->tArbit.b[0] ) << 8 ) | uint16_t( a_reg->tArbit.b[1]  ) ) << 16 ) |
                    ( uint32_t( ( uint16_t( a_reg->tArbit.b[2] ) << 8 ) | uint16_t( a_reg->tArbit.b[3]  ) ) );
      id >>= ext ? 3 : 21;

      __IsoAgLib::CanPkg_c p( id, ext, len, HAL::getTime() );

      p.setUint8Data( 0, a_reg->tCfg_D0.b[1]);
      p.setUint8Data( 1, a_reg->tD1_D4.b[0]);
      p.setUint8Data( 2, a_reg->tD1_D4.b[1]);
      p.setUint8Data( 3, a_reg->tD1_D4.b[2]);
      p.setUint8Data( 4, a_reg->tD1_D4.b[3]);
      p.setUint8Data( 5, a_reg->tD5_D7.b[0]);
      p.setUint8Data( 6, a_reg->tD5_D7.b[1]);
      p.setUint8Data( 7, a_reg->tD5_D7.b[2]);

      HAL::CanFifos_c::get( bBus ).push( p );

#ifdef USE_CAN_MEASURE_BUSLOAD
      HAL::canBusLoads[ bBus ].updateCanBusLoad( ( ( ( cfg & 0xF0 ) >> 4 ) + ( ext ? 4 : 2 ) ) );
#endif

      return 0;
    }
  }


  bool canConfigMsgObj( unsigned channel, unsigned msgObj, const __IsoAgLib::Ident_c& arc_ident, bool tx ) {

    configData.dwId = arc_ident.ident();
    configData.bXtd = ( arc_ident.identType() == __IsoAgLib::Ident_c::ExtendedIdent ) ? TRUE : FALSE;
    configData.wPause = 0;

    if ( tx ) {
      configData.bMsgType = TX;
      configData.bTimeStamped = FALSE;
      configData.pfIrqFunction = 0;
      configData.wNumberMsgs = configTxBufferSize;
    } else {
      configData.bMsgType = RX;
      configData.bTimeStamped = TRUE;
      configData.pfIrqFunction = rxIrq;
      configData.wNumberMsgs = configRxBufferSize;
    }

    return ( C_NO_ERR == config_can_obj( channel, msgObj, &configData ) );
  }
}


namespace HAL {

  bool canInit( unsigned channel, unsigned baudrate ) {
    if( C_NO_ERR != __HAL::init_can( channel, 0x0, 0x0, 0x0, baudrate ) )
      return false;

    if( ! __HAL::canConfigMsgObj( channel, __HAL::configTxMsgObjNum, __IsoAgLib::Ident_c( 0, __IsoAgLib::Ident_c::ExtendedIdent ), true ) )
      return false;

    if( ! __HAL::canConfigMsgObj( channel, __HAL::configRxStdMsgObjNum, __IsoAgLib::Ident_c( 0, __IsoAgLib::Ident_c::StandardIdent ), false ) )
      return false;

    if( ! __HAL::canConfigMsgObj( channel, __HAL::configRxExtMsgObjNum, __IsoAgLib::Ident_c( 0, __IsoAgLib::Ident_c::ExtendedIdent ), false ) )
      return false;

#ifdef USE_CAN_MEASURE_BUSLOAD
    HAL::canBusLoads[ channel ].init();
#endif

    return true;
  }


  bool canClose( unsigned channel ) {
    return ( C_NO_ERR == __HAL::close_can( channel ) );
  }


  bool canState( unsigned channel, canState_t& state ) {
    state = e_canNoError;
    return true;
  }


  bool canTxSend( unsigned channel, const __IsoAgLib::CanPkg_c& msg ) {
    msg.getData( __HAL::sendData.dwId, __HAL::sendData.bXtd, __HAL::sendData.bDlc, __HAL::sendData.abData );
    return ( C_NO_ERR == __HAL::send_can_msg( channel, __HAL::configTxMsgObjNum, &__HAL::sendData ) );
  }


  void canRxPoll( unsigned ) {
    /* nothing here - irq driven... */
  }


  bool canRxWait( unsigned timeout_ms ) {

    const int32_t endTime_ms = __IsoAgLib::System_c::getTime() + timeout_ms;

    while ( __IsoAgLib::System_c::getTime() < endTime_ms ) {
      for ( unsigned int c = 0; c < ( HAL_CAN_MAX_BUS_NR + 1 ); ++c ) {
        if ( ! HAL::CanFifos_c::get( c ).empty() ) {
          return true;
        }
      }
    }
    return false;
  }


  int canTxQueueFree( unsigned channel ) {
    const int r = __HAL::get_can_msg_buf_count( channel, __HAL::configTxMsgObjNum );
    if( r >= 0 ) {
      return ( __HAL::configTxBufferSize - r );
    }
    return -1;
  }

  void defineRxFilter( unsigned, bool, uint32_t, uint32_t ) {}
  void deleteRxFilter( unsigned, bool, uint32_t, uint32_t ) {}

}


// eof
