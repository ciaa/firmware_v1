/*
  target_extension_can_client_msq.cpp:
    CAN-Server client based on message-queues

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <cstring>
#include <cstdio>
#include <cctype>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>

#include <set>

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/driver/can/impl/canpkg_c.h>
#include <IsoAgLib/hal/generic_utils/can/canfifo_c.h>
#include <IsoAgLib/hal/hal_can.h>
#include <IsoAgLib/hal/pc/system/system.h>
#include <IsoAgLib/util/iassert.h>

#include "can_server_interface.h"


namespace __HAL {
  static msqData_s msqDataClient = { -1, -1, -1, -1, -1, -1 };

#ifdef USE_MUTUAL_EXCLUSION
  static int breakWaitPipeFd[2] = { -1, -1 };
#endif

  struct idFilter_s {
    bool operator<(const struct idFilter_s& r ) const {
      return idx < r.idx;
    }
    unsigned idx;
    struct {
      bool xtd;
      uint32_t mask;
      uint32_t filter;
    } filter;
  };
  static std::set<idFilter_s> filterIdx[HAL_CAN_MAX_BUS_NR];

  bool canStartDriver() {
    transferBuf_s s_transferBuf;

    int16_t rc = createMsqs( msqDataClient );

    if ( rc != HAL_NO_ERR )
      return false;

    s_transferBuf.i32_mtypePid = msqDataClient.i32_pid;
    s_transferBuf.ui16_command = COMMAND_REGISTER;
    s_transferBuf.s_startTimeClock.t_clock = getStartupTime();

    rc = send_command( &s_transferBuf, &msqDataClient );

    if ( rc == HAL_NO_ERR ) {

      if ( s_transferBuf.s_acknowledge.i32_dataContent != ACKNOWLEDGE_DATA_CONTENT_PIPE_ID ) {
        return false;
      }

#ifdef USE_MUTUAL_EXCLUSION
      if( pipe2( breakWaitPipeFd, O_NONBLOCK ) != 0 ) {
        perror("pipe");
      }
#endif

      char pipe_name[255];
      sprintf( pipe_name, "%s%d", PIPE_PATH, s_transferBuf.s_acknowledge.i32_data );

      int fd = open( pipe_name, O_NONBLOCK | O_RDONLY );
      if( fd == -1 ) {
        return false;
      } else {
        msqDataClient.i32_pipeHandle = fd;
      }
    }

    return true;
  }


  bool canStopDriver() {
    transferBuf_s s_transferBuf;

    if ( msqDataClient.i32_pipeHandle )
      close( msqDataClient.i32_pipeHandle );

    s_transferBuf.i32_mtypePid = msqDataClient.i32_pid;
    s_transferBuf.ui16_command = COMMAND_DEREGISTER;

#ifdef USE_MUTUAL_EXCLUSION
    ( void )close(breakWaitPipeFd[0]);
    ( void )close(breakWaitPipeFd[1]);
#endif

    return ( HAL_NO_ERR == send_command( &s_transferBuf, &msqDataClient ) );
  }


  void canEnqueue( unsigned channel, const __HAL::can_data& data ) {
    static __IsoAgLib::CanPkg_c pkg;
    pkg.setIdent( data.i32_ident, data.b_xtd ? __IsoAgLib::Ident_c::ExtendedIdent : __IsoAgLib::Ident_c::StandardIdent );
    pkg.setLen( data.b_dlc );
    const int32_t now = getTime();
    pkg.setTime( now < data.i32_time ? now : data.i32_time );
    memcpy( pkg.getUint8DataPointer(), data.pb_data, data.b_dlc );
    HAL::CanFifos_c::get( channel ).push( pkg );
  }


}

namespace HAL {

  bool canInit( unsigned channel, unsigned baudrate ) {
    bool r = true;

    __HAL::transferBuf_s b;
    b.i32_mtypePid = __HAL::msqDataClient.i32_pid;
    b.ui16_command = COMMAND_INIT;
    b.s_init.ui8_bus = channel;
    b.s_init.ui16_wGlobMask = 0x0;
    b.s_init.ui32_dwGlobMask = 0x0;
    b.s_init.ui32_dwGlobMaskLastmsg = 0x0;
    b.s_init.ui16_wBitrate = baudrate;
    r &= ( HAL_NO_ERR == __HAL::send_command( &b, &__HAL::msqDataClient ) );

    b.ui16_command = COMMAND_CONFIG;
    b.s_config.ui8_bus = channel;
    b.s_config.ui8_obj = 0;
    b.s_config.ui8_bXtd = 0;
    b.s_config.ui32_mask = 0;
    b.s_config.ui8_bMsgType = TX;
    b.s_config.ui32_dwId = 0x0;
    b.s_config.ui16_wNumberMsgs = 0x20;
    r &= ( HAL_NO_ERR == __HAL::send_command( &b, &__HAL::msqDataClient ) );
    isoaglib_assert( r );

    return r;
  }


  bool canClose( unsigned channel ) {
    __HAL::transferBuf_s s_transferBuf;

    s_transferBuf.i32_mtypePid = __HAL::msqDataClient.i32_pid;
    s_transferBuf.ui16_command = COMMAND_CLOSE;
    s_transferBuf.s_init.ui8_bus = channel;
    return( __HAL::send_command( &s_transferBuf, &__HAL::msqDataClient ) == HAL_NO_ERR );
  }


  bool canState( unsigned /* channel */, canState_t& state ) {
    state = e_canNoError;
    return true;
  }


  bool canTxSend( unsigned channel, const __IsoAgLib::CanPkg_c& msg ) {

    __HAL::msqWrite_s msqWriteBuf;
    memset( &msqWriteBuf, 0, sizeof( __HAL::msqWrite_s ) );

    __HAL::transferBuf_s s_transferBuf;

    msqWriteBuf.i32_mtypePrioAnd1 = __HAL::assembleWrite_mtype( true );
    msqWriteBuf.ui16_pid = __HAL::msqDataClient.i32_pid;
    msqWriteBuf.ui8_bus = channel;
    msqWriteBuf.ui8_obj = 0;
    msqWriteBuf.s_canMsg.ui32_id = msg.ident();
    msqWriteBuf.s_canMsg.i32_len = msg.getLen();
    msqWriteBuf.s_canMsg.i32_msgType = ( msg.identType() == __IsoAgLib::Ident_c::ExtendedIdent ) ? 1 : 0;
    memcpy( &( msqWriteBuf.s_canMsg.ui8_data ), msg.getUint8DataConstPointer(), msg.getLen() );

    if ( msgsnd( __HAL::msqDataClient.i32_wrHandle, &msqWriteBuf, sizeof( __HAL::msqWrite_s ) - sizeof( long ), 0 ) == -1 ) {
      perror( "msgsnd" );
      return false;
    }

    if( msgrcv( __HAL::msqDataClient.i32_cmdAckHandle, &s_transferBuf, sizeof( __HAL::transferBuf_s ) - sizeof( long ), __HAL::msqDataClient.i32_pid, 0 ) == -1 )
      return false;

    if ( s_transferBuf.ui16_command == COMMAND_ACKNOWLEDGE )
      return true;

    return false;
  }


  void canRxPoll( unsigned channel ) {
    __HAL::msqRead_s msqReadBuf;
    while( msgrcv( __HAL::msqDataClient.i32_rdHandle,
                &msqReadBuf,
                sizeof( __HAL::msqRead_s ) - sizeof( long ),
                __HAL::assembleRead_mtype( __HAL::msqDataClient.i32_pid, channel, COMMON_MSGOBJ_IN_QUEUE ), IPC_NOWAIT ) > 0 ) {
      canEnqueue( channel, msqReadBuf.s_canData );
    }
  }


  bool canRxWait( unsigned timeout_ms ) {
    int16_t rc;
    fd_set rfds;
    struct timeval s_timeout;
    static uint8_t ui8_buf[16];

    FD_ZERO( &rfds );
    FD_SET( __HAL::msqDataClient.i32_pipeHandle, &rfds );
#ifdef USE_MUTUAL_EXCLUSION
    FD_SET( __HAL::breakWaitPipeFd[0], &rfds );
#endif

    s_timeout.tv_sec = 0;
    s_timeout.tv_usec = timeout_ms * 1000;

    rc = select( __HAL::msqDataClient.i32_pipeHandle+1, &rfds, NULL, NULL, &s_timeout );

#ifdef USE_MUTUAL_EXCLUSION
    if( FD_ISSET( __HAL::breakWaitPipeFd[0], &rfds ) ) {
      static char buff[256];
      while( 0 < read( __HAL::breakWaitPipeFd[0], buff, 256 ) ) { }
    }
#endif

    const bool r = ( rc > 0 );

    if( r > 0 && FD_ISSET( __HAL::msqDataClient.i32_pipeHandle, &rfds ) > 0 ) {
      for ( rc = read( __HAL::msqDataClient.i32_pipeHandle, &ui8_buf, 16 ); rc == 16 ;
            rc = read( __HAL::msqDataClient.i32_pipeHandle, &ui8_buf, 16 ) ) {}
    }

    return r;
  }

#ifdef USE_MUTUAL_EXCLUSION
  void canRxWaitBreak() {
    if( write( __HAL::breakWaitPipeFd[1], "\0", 1 ) != 1 ) {
      perror("write");
    }
  }
#endif


  int canTxQueueFree( unsigned /* channel */ ) {
    return -1; // unsupported
  }


  void defineRxFilter( unsigned channel, bool xtd, uint32_t filter, uint32_t mask ) {

    unsigned idx = 1;
    for( std::set<__HAL::idFilter_s>::const_iterator i = __HAL::filterIdx[channel].begin(); i != __HAL::filterIdx[channel].end(); ++i ) {
      if( ( i->idx - idx ) > 0 )
        break;
      idx++;
    }

    __HAL::transferBuf_s b;
    b.i32_mtypePid = __HAL::msqDataClient.i32_pid;
    b.ui16_command = COMMAND_CONFIG;
    b.s_config.ui8_bus = channel;
    b.s_config.ui8_obj = idx;
    b.s_config.ui8_bXtd = xtd ? 1 : 0;
    b.s_config.ui32_mask = mask;
    b.s_config.ui8_bMsgType = RX;
    b.s_config.ui32_dwId = filter;
    b.s_config.ui16_wNumberMsgs = 0x20;
    const bool r = ( HAL_NO_ERR == __HAL::send_command( &b, &__HAL::msqDataClient ) );

    isoaglib_assert( r ); (void)r;
    struct __HAL::idFilter_s f;
    f.idx = idx;
    f.filter.xtd = xtd;
    f.filter.mask = mask;
    f.filter.filter = filter;
    __HAL::filterIdx[channel].insert( f );
  }

  void deleteRxFilter( unsigned channel, bool xtd, uint32_t filter, uint32_t mask ) {
    for( std::set<__HAL::idFilter_s>::const_iterator i = __HAL::filterIdx[channel].begin(); i != __HAL::filterIdx[channel].end(); ++i ) {

      if( ( i->filter.xtd == xtd ) &&
          ( i->filter.mask == mask ) &&
          ( i->filter.filter == filter ) ) {
        __HAL::transferBuf_s b;
        b.i32_mtypePid = __HAL::msqDataClient.i32_pid;
        b.ui16_command = COMMAND_CLOSEOBJ;
        b.s_config.ui8_bus = channel;
        b.s_config.ui8_obj = i->idx;
        const bool r = ( HAL_NO_ERR == __HAL::send_command( &b, &__HAL::msqDataClient ) );
        isoaglib_assert( r ); (void)r;
        __HAL::filterIdx[channel].erase( i );
        break;
      }
    }
  }


} // __HAL
