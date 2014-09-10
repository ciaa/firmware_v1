/*
  tracguidance_c.cpp: stores, updates and delivers all guidance data
    information from CanCustomer_c derived for CAN sending and
    receiving interaction; from BaseCommon_c derived for interaction
    with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgn_c.h>
#include "tracguidancecommand_c.h"
#include "tracguidance_c.h"


namespace __IsoAgLib { // Begin Namespace __IsoAglib

  /** C-style function, to get access to the unique TracGuidance_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an m_index must be given to select the wanted BUS
    */
  TracGuidance_c &getTracGuidanceInstance( uint8_t aui8_instance )
  { // if > 1 singleton instance is used, no static reference can be used
    MACRO_MULTITON_GET_INSTANCE_BODY(TracGuidance_c, PRT_INSTANCE_CNT, aui8_instance);
  }

  bool TracGuidance_c::config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask)
  {
    IsoAgLib::IdentMode_t const ct_oldMode = getMode();
    //call config for handling which is base data independent
    //if something went wrong leave function before something is configured
    if ( ! BaseCommon_c::config_base ( apc_ident, at_identMode, aui16_suppressMask) ) return false;

    ///Set time Period for Scheduler_c
    if (at_identMode == IsoAgLib::IdentModeTractor) mt_task.setPeriod( 100, false );
    else  mt_task.setPeriod( CONFIG_TIMEOUT_TRACTOR_DATA, false );

    mui16_estCurvature = 32128;
    mt_requestResetCmdStatus = IsoAgLib::IsoNotAvailableReset ;
    mt_steeringInputPosStatus = IsoAgLib::IsoNotAvailableSteerPos ;
    mt_steeringSystemReadiness = IsoAgLib::IsoNotAvailableSteerReadiness ;
    mt_mechanicalSystemLogout = IsoAgLib::IsoNotAvailable ;

    // un-/register to PGN
    if (ct_oldMode == at_identMode)
      ; // no change, still the same mode
    else if (at_identMode == IsoAgLib::IdentModeTractor)
      // a change from Implement mode to Tractor mode occured
      getRegisterPgn()(GUIDANCE_MACHINE_STATUS);
    else
      // a change from Tractor mode to Implement mode occured
      getUnregisterPgn()(GUIDANCE_MACHINE_STATUS);

    return true;
  }

  /** check if filter boxes shall be created - create only ISO filters based
      on active local idents which has already claimed an address
      --> avoid to much Filter Boxes
    */
  void TracGuidance_c::checkCreateReceiveFilter( )
  {
    if( !checkFilterCreated() )
    {
      setFilterCreated();

      IsoBus_c &c_can = getIsoBusInstance4Comm();
      c_can.insertFilter(*this, IsoAgLib::iMaskFilter_c( 0x3FF00LU << 8, GUIDANCE_MACHINE_STATUS << 8), 8 );
    }
  }

  /** send a ISO11783 guidance information PGN.
    * this is only called when sending ident is configured and it has already claimed an address
      @pre  function is only called in tractor mode
      @see  BaseCommon_c::timeEvent()
    */
  void TracGuidance_c::timeEventTracMode( )
  { ///Timeperiod of 100ms is set in ::config
    isoSendMessageTracMode();
  }

  void TracGuidance_c::processMsg( const CanPkg_c& arc_data )
  {
    CanPkgExt_c pkg( arc_data, getMultitonInst() );
    if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
      return;

    if( ( pkg.isoPgn() & 0x3FF00LU ) ==  GUIDANCE_MACHINE_STATUS )
    {
      IsoName_c const& rcc_tempISOName = pkg.getISONameForSA();
      if ( checkParseReceived( rcc_tempISOName ) )
      {
        mui16_estCurvature = pkg.getUint16Data(0);
        mt_requestResetCmdStatus = IsoAgLib::IsoResetFlag_t           ( (pkg.getUint16Data(2) >> 6) & 0x3);
        mt_steeringInputPosStatus = IsoAgLib::IsoSteerPosFlag_t       ( (pkg.getUint8Data(2) >> 4) & 0x3);
        mt_steeringSystemReadiness = IsoAgLib::IsoSteerReadinessFlag_t( (pkg.getUint8Data(2) >> 2) & 0x3);
        mt_mechanicalSystemLogout = IsoAgLib::IsoActiveFlag_t         ( (pkg.getUint8Data(2) >> 0) & 0x3);

        setSelectedDataSourceISOName (rcc_tempISOName);
        setUpdateTime( pkg.time() );
      }
      else
      { // there is a sender conflict
        IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracMultipleSender, getMultitonInst() );
      }
    }
  }

  /** send guidance data
      @see  CanIo_c::operator<<
    */
  void TracGuidance_c::isoSendMessageTracMode( )
  {
    setSelectedDataSourceISOName( getIdentItem()->isoName() );

    CanPkgExt_c pkg;
    pkg.setIsoPri(3);
    pkg.setIsoPgn(GUIDANCE_MACHINE_STATUS);
    pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
    pkg.setMonitorItemForDA( getIsoMonitorInstance4Comm().item( getTracGuidanceCommandInstance4Comm().getCommander() ) );
    pkg.setLen(8);
    pkg.setUint16Data(0, mui16_estCurvature);
    uint8_t ui8_temp = 0;
    ui8_temp |= (mt_requestResetCmdStatus   << 6);
    ui8_temp |= (mt_steeringInputPosStatus  << 4);
    ui8_temp |= (mt_steeringSystemReadiness << 2);
    ui8_temp |= (mt_mechanicalSystemLogout  << 0);
    pkg.setUint8Data(2, ui8_temp);
    pkg.setUint32Data(3, 0xFFFFFFFFUL);
    pkg.setUint8Data(7, 0xFF);

    getIsoBusInstance4Comm() << pkg;
  }

bool TracGuidance_c::processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t )
{
  bool const b_allowed = BaseCommon_c::check4ReqForPgn(aui32_pgn, apc_isoItemSender, apc_isoItemReceiver);
  if (!b_allowed)
    return false;
  if (GUIDANCE_MACHINE_STATUS != aui32_pgn)
    return false;
  isoSendMessageTracMode();
  return true;
}

}// End Namespace __IsoAglib
