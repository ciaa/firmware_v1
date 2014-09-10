/*
  tracmovesetpoint_c.cpp: working on Base Data Msg Type 1; stores,
    updates and delivers all base data informations from CanCustomer_c
    derived for CAN sending and receiving interaction; from
    SchedulerTask_c derived for interaction with other IsoAgLib
    objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#if defined(USE_BASE) || defined(USE_TRACTOR_MOVE)
  #include "tracmove_c.h"
#endif
#include "tracmovesetpoint_c.h"


namespace __IsoAgLib { // Begin Namespace __IsoAglib

  /** C-style function, to get access to the unique TracMoveSetPoint_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an m_index must be given to select the wanted BUS
    */
  TracMoveSetPoint_c &getTracMoveSetPointInstance( uint8_t aui8_instance )
  { // if > 1 singleton instance is used, no static reference can be used
    MACRO_MULTITON_GET_INSTANCE_BODY(TracMoveSetPoint_c, PRT_INSTANCE_CNT, aui8_instance);
  }


  bool TracMoveSetPoint_c::config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask )
  {
    //call config for handling which is base data independent
    //if something went wrong leave function before something is configured
    if ( !BaseCommon_c::config_base ( apc_ident, at_identMode, aui16_suppressMask) ) return false;

    ///Set time Period for Scheduler_c --> periodic action only if in IMPLEMENT MODE!!
    if (at_identMode == IsoAgLib::IdentModeImplement) mt_task.setPeriod( 100, false );
    else  mt_task.setPeriod( 1000, false );

    // set configure values
    mi16_selectedSpeedSetPointLimit = 0;
    mi16_selectedSpeedSetPointCmd = 0;
    mt_selectedDirectionCmd = IsoAgLib::IsoNotAvailableDirection;

    return true;
  }

  /** check if filter boxes shall be created - create only ISO filters based
      on active local idents which has already claimed an address
      --> avoid to much Filter Boxes
    */
  void TracMoveSetPoint_c::checkCreateReceiveFilter()
  {
    IsoBus_c &c_can = getIsoBusInstance4Comm();

    if (!checkFilterCreated())
    { // check if needed receive filters for ISO are active
      setFilterCreated();

      c_can.insertFilter(*this, IsoAgLib::iMaskFilter_c( 0x3FFFFLU << 8, SELECTED_SPEED_CMD << 8), 8);
    }
  }

  void TracMoveSetPoint_c::processMsg( const CanPkg_c& arc_data )
  {
    CanPkgExt_c pkg( arc_data, getMultitonInst() );
    if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
      return;

    switch (pkg.isoPgn() /*& 0x3FFFF*/) // don't need to &, we're interested in the whole PGN
    {
      case SELECTED_SPEED_CMD:
        if ( checkMode(IsoAgLib::IdentModeTractor) )
        {
          mi16_selectedSpeedSetPointCmd =   pkg.getUint16Data(0);
          mi16_selectedSpeedSetPointLimit = pkg.getUint16Data(2);
          mt_selectedDirectionCmd = IsoAgLib::IsoDirectionFlag_t( (pkg.getUint8Data(7) & 0x3) );
        }
        break;
    }
  }

  /** send a ISO11783 base information PGN.
    * this is only called when sending ident is configured and it has already claimed an address
      @pre  function is only called in implement mode
      @see  BaseCommon_c::timeEvent()
      @see  CanIo_c::operator<<
  */
  void TracMoveSetPoint_c::timeEventImplMode( )
  {
    #if defined(USE_BASE) || defined(USE_TRACTOR_MOVE)
    TracMove_c& c_tracmove = getTracMoveInstance4Comm();
    #endif

    if ( 1
      #if defined(USE_BASE) || defined(USE_TRACTOR_MOVE)
        && c_tracmove.getSelectedDataSourceISONameConst().isSpecified()
      #endif
       )
    {
      if( ( ! getIdentItem() ) || ( ! getIdentItem()->isClaimedAddress() ) ) {
        return;
      }

      IsoBus_c& c_can = getIsoBusInstance4Comm();

      CanPkgExt_c pkg;
      pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
      pkg.setIsoPri(3);
      pkg.setLen(8);

      pkg.setIsoPgn(SELECTED_SPEED_CMD);
      uint8_t ui8_temp = 0;
      pkg.setUint16Data(0, mi16_selectedSpeedSetPointCmd);
      pkg.setUint16Data(2, mi16_selectedSpeedSetPointLimit);
      ui8_temp |= mt_selectedDirectionCmd;
      pkg.setUint8Data(7, ui8_temp);
      //reserved fields
      pkg.setUint16Data(4, 0xFF);
      pkg.setUint8Data( 6, 0xFF);

      c_can << pkg;
    }
  }


/** dummy implementation
    @todo SOON-824: add answering of requestPGN in case this object is configured for sending of these information
           - verify this also for the other TracFoo classes
  */
bool TracMoveSetPoint_c::processMsgRequestPGN (uint32_t /*aui32_pgn*/, IsoItem_c* /*apc_isoItemSender*/, IsoItem_c* /*apc_isoItemReceiver*/, int32_t )
{
  return false;
}

} // End Namespace __IsoAglib
