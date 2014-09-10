/*
  tracptosetpoint_c.cpp: working on pto set point data; stores,
    updates and delivers all pto set point data informations from
    CanCustomer_c derived for CAN sending and receiving interaction;
    from SchedulerTask_c derived for interaction with other IsoAgLib
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
#if defined(USE_BASE) || defined(USE_TRACTOR_PTO)
  #include "tracpto_c.h"
#endif
#include "tracptosetpoint_c.h"


namespace __IsoAgLib { // Begin Namespace __IsoAgLib

  /** C-style function, to get access to the unique TracPTOSetPoint_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an m_index must be given to select the wanted BUS
    */
  TracPTOSetPoint_c &getTracPtoSetPointInstance(uint8_t aui8_instance)
  {
    MACRO_MULTITON_GET_INSTANCE_BODY(TracPTOSetPoint_c, PRT_INSTANCE_CNT, aui8_instance);
  }

  bool TracPTOSetPoint_c::config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask )
  {
    //call config for handling which is base data independent
    //if something went wrong leave function before something is configured
    if ( ! BaseCommon_c::config_base ( apc_ident, at_identMode, aui16_suppressMask) ) return false;

    ///Set time Period for Scheduler_c --> periodic action only if in IMPLEMENT MODE!!
    if (at_identMode == IsoAgLib::IdentModeImplement) mt_task.setPeriod( 100, false );
    else  mt_task.setPeriod( 1000, false );

    //set default values
    mui8_frontHitchPosCmd     = 0;
    mui8_rearHitchPosCmd      = 0;
    mui16_frontPtoSetPointCmd = 0;
    mui16_rearPtoSetPointCmd  = 0;
    mt_frontPtoEngagement =  IsoAgLib::IsoNotAvailable;
    mt_rearPtoEngagement =   IsoAgLib::IsoNotAvailable;
    mt_frontPtoMode =        IsoAgLib::IsoNotAvailable;
    mt_rearPtoMode =         IsoAgLib::IsoNotAvailable;
    mt_frontPtoEconomyMode = IsoAgLib::IsoNotAvailable;
    mt_rearPtoEconomyMode =  IsoAgLib::IsoNotAvailable;

    return true;
  };

  /** check if filter boxes shall be created - create only ISO filters based
      on active local idents which has already claimed an address
      --> avoid to much Filter Boxes
    */
  void TracPTOSetPoint_c::checkCreateReceiveFilter( )
  {
    IsoBus_c &c_can = getIsoBusInstance4Comm();

    if( !checkFilterCreated() )
    { // check if needed receive filters for ISO are active
      setFilterCreated();

      c_can.insertFilter(*this, IsoAgLib::iMaskFilter_c( 0x3FFFFLU << 8, HITCH_PTO_COMMANDS << 8), 8);
    }
  }

  /** process a ISO11783 base information PGN
      @pre  sender of message is existent in monitor list
      @see  CanPkgExt_c::resolveSendingInformation()
    */
  void TracPTOSetPoint_c::processMsg( const CanPkg_c& arc_data )
  {
    CanPkgExt_c pkg( arc_data, getMultitonInst() );
    if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
      return;

    switch (pkg.isoPgn() /*& 0x3FFFF*/) // don't need to &, as we're interested in the whole PGN
    {
      case HITCH_PTO_COMMANDS:
        if ( checkMode(IsoAgLib::IdentModeTractor) )
        {
          mui8_frontHitchPosCmd =     pkg.getUint8Data(0);
          mui8_rearHitchPosCmd =      pkg.getUint8Data(1);
          mui16_frontPtoSetPointCmd = pkg.getUint16Data(2);
          mui16_rearPtoSetPointCmd =  pkg.getUint16Data(4);
          mt_frontPtoEngagement =  IsoAgLib::IsoActiveFlag_t( (pkg.getUint8Data(6) >> 6) & 3);
          mt_rearPtoEngagement =   IsoAgLib::IsoActiveFlag_t( (pkg.getUint8Data(6) >> 4) & 3);
          mt_frontPtoMode =        IsoAgLib::IsoActiveFlag_t( (pkg.getUint8Data(7) >> 6) & 3);
          mt_rearPtoMode =         IsoAgLib::IsoActiveFlag_t( (pkg.getUint8Data(7) >> 4) & 3);
          mt_frontPtoEconomyMode = IsoAgLib::IsoActiveFlag_t( (pkg.getUint8Data(7) >> 2) & 3);
          mt_rearPtoEconomyMode =  IsoAgLib::IsoActiveFlag_t(  pkg.getUint8Data(7)       & 3);

          // we do not need to set setUpdateTime() in basecommon_c because time relevant
          // verifications are now done by the scheduler
        }
        break;
    }
  }

  /** send a ISO11783 pto information PGN.
    * this is only called when sending ident is configured and it has already claimed an address
      @pre  function is only called in implement mode
      @see  BaseCommon_c::timeEvent()
    */
  void TracPTOSetPoint_c::timeEventImplMode( )
  {
    /// check if we are in implement mode and have a pointer to the sending isoname
    /// stored Pto information sending ISO member has claimed address
    sendPtoMsg();
  }

  /** send hitch and pto command
      @see  CanIo_c::operator<<
    */
  void TracPTOSetPoint_c::sendPtoMsg()
  {
    if( ( ! getIdentItem() ) || ( ! getIdentItem()->isClaimedAddress() ) )
      return;

    #if defined(USE_BASE) || defined(USE_TRACTOR_PTO)
    TracPTO_c& c_tracpto = getTracPtoInstance4Comm();
    //do not send message if no tractor is available to process the command
    if ( ! c_tracpto.getSelectedDataSourceISONameConst().isSpecified() ) return;
    #endif

    IsoBus_c& c_can = getIsoBusInstance4Comm();

    CanPkgExt_c pkg;
    pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
    pkg.setIsoPri(3);
    pkg.setLen(8);
    pkg.setIsoPgn(HITCH_PTO_COMMANDS);

    pkg.setUint8Data(0, mui8_frontHitchPosCmd);
    pkg.setUint8Data(1, mui8_rearHitchPosCmd);
    pkg.setUint16Data(2, mui16_frontPtoSetPointCmd);
    pkg.setUint16Data(4, mui16_rearPtoSetPointCmd);
    uint8_t ui8_val6 = 0;
    ui8_val6 |= (mt_frontPtoEngagement  << 6);
    ui8_val6 |= (mt_rearPtoEngagement   << 4);
    pkg.setUint8Data(6, ui8_val6);
    uint8_t ui8_val7 = 0;
    ui8_val7 |= (mt_frontPtoMode        << 6);
    ui8_val7 |= (mt_rearPtoMode         << 4);
    ui8_val7 |= (mt_frontPtoEconomyMode << 2);
    ui8_val7 |= (mt_frontPtoEconomyMode     );
    pkg.setUint8Data(7, ui8_val7);

    c_can << pkg;

    // we do not need to set setUpdateTime() in basecommon_c because time relevant
    // settings are now done by the scheduler
  }


/** dummy implementation
    @todo SOON-824: add answering of requestPGN in case this object is configured for sending of these information
           - verify this also for the other TracFoo classes
  */
bool TracPTOSetPoint_c::processMsgRequestPGN (uint32_t /*aui32_pgn*/, IsoItem_c* /*apc_isoItemSender*/, IsoItem_c* /*apc_isoItemReceiver*/, int32_t )
{
  return false;
}

} // End Namespace __IsoAgLib
