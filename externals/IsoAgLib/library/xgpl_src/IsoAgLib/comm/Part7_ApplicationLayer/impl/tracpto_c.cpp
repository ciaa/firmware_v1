/*
  tracpto_c.cpp: working on PTO data; stores, updates and delivers all
    base data informations from CanCustomer_c derived for CAN sending
    and receiving interaction; from BaseCommon_c derived for
    interaction with other IsoAgLib objects

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
#include <IsoAgLib/util/iliberr_c.h>
#include "tracpto_c.h"
#include "tracgeneral_c.h"


namespace __IsoAgLib { // Begin Namespace __IsoAgLib

  /** C-style function, to get access to the unique TracPTO_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  TracPTO_c &getTracPtoInstance(uint8_t aui8_instance)
  {
    MACRO_MULTITON_GET_INSTANCE_BODY(TracPTO_c, PRT_INSTANCE_CNT, aui8_instance);
  }

  /**
      @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
      @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      @return true -> configuration was successfull
   */
  bool TracPTO_c::config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask)
  {
    //store old mode to decide to register or unregister from request for pgn
    //and set Periode for Scheduler_c
    IsoAgLib::IdentMode_t t_oldMode = getMode();

    //call config for handling which is base data independent
    //if something went wrong leave function before something is configured
    if ( ! BaseCommon_c::config_base ( apc_ident, at_identMode, aui16_suppressMask) ) return false;

    ///Set time Period for Scheduler_c
    if (at_identMode == IsoAgLib::IdentModeTractor) mt_task.setPeriod( 100, false );
    else  mt_task.setPeriod( CONFIG_TIMEOUT_TRACTOR_DATA, false );

    // set the member base msg value vars to NO_VAL codes
    mt_ptoFront.ui16_pto8DigitPerRpm = mt_ptoRear.ui16_pto8DigitPerRpm = NO_VAL_16;
    // set the timestamps to 0
    mt_ptoFront.i32_lastPto = mt_ptoRear.i32_lastPto = 0;

    // un-/register to PGN
    if (t_oldMode == at_identMode)
      ; // no change, still the same mode
    else if (at_identMode == IsoAgLib::IdentModeTractor) {
      // a change from Implement mode to Tractor mode occured
      RegisterPgn_s s_register = getRegisterPgn();
      s_register(FRONT_PTO_STATE_PGN);
      s_register(REAR_PTO_STATE_PGN);
    } else {
      // a change from Tractor mode to Implement mode occured
      UnregisterPgn_s s_unregister = getUnregisterPgn();
      s_unregister(FRONT_PTO_STATE_PGN);
      s_unregister(REAR_PTO_STATE_PGN);
    }

    mt_ptoFront.t_ptoEngaged = mt_ptoRear.t_ptoEngaged
    = mt_ptoFront.t_pto1000 = mt_ptoRear.t_pto1000
    = mt_ptoFront.t_ptoEconomy = mt_ptoRear.t_ptoEconomy = IsoAgLib::IsoNotAvailable; // mark as not available
    mt_ptoFront.ui16_ptoSetPoint8DigitPerRpm = 0;
    mt_ptoRear.ui16_ptoSetPoint8DigitPerRpm = 0;
    mt_ptoFront.t_ptoEngagementReqStatus = mt_ptoRear.t_ptoEngagementReqStatus
    = mt_ptoFront.t_ptoModeReqStatus = mt_ptoRear.t_ptoModeReqStatus
    = mt_ptoFront.t_ptoEconomyModeReqStatus = mt_ptoRear.t_ptoEconomyModeReqStatus = IsoAgLib::IsoNotAvailableReq;
    mt_ptoFront.t_ptoShaftSpeedLimitStatus = mt_ptoRear.t_ptoShaftSpeedLimitStatus = IsoAgLib::IsoNotAvailableLimit;

    return true;
  };


  void TracPTO_c::checkCreateReceiveFilter( )
  {
    if ( !checkFilterCreated() )
    {
      setFilterCreated();

      IsoBus_c &c_can = getIsoBusInstance4Comm();
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (FRONT_PTO_STATE_PGN<<8) ), 8 );
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (REAR_PTO_STATE_PGN<<8) ), 8 );

    }
  }

  bool TracPTO_c::processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t )
  {
    // check if we are allowed to send a request for pgn
    if ( ! BaseCommon_c::check4ReqForPgn(aui32_pgn, apc_isoItemSender, apc_isoItemReceiver) ) return false;

    // call TracPto_c function to send pto informtation
    // isoSendMessage checks if this item (identified by ISOName)
    // is configured to send pto information
    if ( aui32_pgn == FRONT_PTO_STATE_PGN  && (IsoAgLib::IsoActiveFlag_t)(mt_ptoFront.t_ptoEngaged) != IsoAgLib::IsoActive)
    {
      sendMessage(sendFrontPto);
    }
    if ( aui32_pgn == REAR_PTO_STATE_PGN && (IsoAgLib::IsoActiveFlag_t)mt_ptoRear.t_ptoEngaged != IsoAgLib::IsoActive)
    {
      sendMessage(sendRearPto);
    }
    return true;
  }


  void TracPTO_c::processMsg( const CanPkg_c& arc_data )
  {
    CanPkgExt_c pkg( arc_data, getMultitonInst() );
    if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
      return;

    IsoName_c const& rcc_tempISOName = pkg.getISONameForSA();

    if (((pkg.isoPgn() /*& 0x3FFFF*/) == FRONT_PTO_STATE_PGN) || ((pkg.isoPgn() /*& 0x3FFFF*/) == REAR_PTO_STATE_PGN))
    {
      const int32_t ci32_now = pkg.time();
      // only take values, if i am not the regular sender
      // and if actual sender isn't in conflict to previous sender
      if ( checkParseReceived( rcc_tempISOName ) )
      { // sender is allowed to send
        PtoData_t* pt_ptoData = NULL;
        if (pkg.isoPgn() == FRONT_PTO_STATE_PGN)
        { // front PTO
          pt_ptoData = &mt_ptoFront;
        }
        else
        { // rear PTO
          pt_ptoData = &mt_ptoRear;
        }

        pt_ptoData->i32_lastPto = ci32_now;
        pt_ptoData->ui16_pto8DigitPerRpm         = pkg.getUint16Data(0);
        pt_ptoData->ui16_ptoSetPoint8DigitPerRpm = pkg.getUint16Data(2);
        pt_ptoData->t_ptoEngaged = IsoAgLib::IsoActiveFlag_t(          (    pkg.getUint8Data(4) >> 6) & 3 );
        pt_ptoData->t_pto1000    = IsoAgLib::IsoActiveFlag_t(          (    pkg.getUint8Data(4) >> 4) & 3 );
        pt_ptoData->t_ptoEconomy = IsoAgLib::IsoActiveFlag_t(          (    pkg.getUint8Data(4) >> 2) & 3 );
        pt_ptoData->t_ptoEngagementReqStatus   = IsoAgLib::IsoReqFlag_t(    pkg.getUint8Data(4)       & 3 );
        pt_ptoData->t_ptoModeReqStatus         = IsoAgLib::IsoReqFlag_t(   (pkg.getUint8Data(5) >> 6) & 3 );
        pt_ptoData->t_ptoEconomyModeReqStatus  = IsoAgLib::IsoReqFlag_t(   (pkg.getUint8Data(5) >> 4) & 3 );
        pt_ptoData->t_ptoShaftSpeedLimitStatus = IsoAgLib::IsoLimitFlag_t( (pkg.getUint8Data(5) >> 1) & 0x7 );

        // set last time
        setSelectedDataSourceISOName (rcc_tempISOName);
        // update time
        pt_ptoData->i32_lastPto = pkg.time();
        // must be set because this is needed in basecommon_c
        setUpdateTime( pt_ptoData->i32_lastPto );

        //msg from Tractor received do tell Scheduler_c next call not until  3000ms
        mt_task.setNextTriggerTime( pkg.time() + CONFIG_TIMEOUT_TRACTOR_DATA );
      }

      else
      { // there is a sender conflict
        IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracMultipleSender, getMultitonInst() );
      }
    }
  }

  /** send a ISO11783 base information PGN.
    * this is only called when sending ident is configured and it has already claimed an address
      @pre  function is only called in tractor mode
      @see  BaseCommon_c::timeEvent()
    */
  void TracPTO_c::timeEventTracMode( )
  {
     ///Timeperiod of 100ms is set in ::config
    if ( mt_ptoFront.t_ptoEngaged == IsoAgLib::IsoActive  )
    { // it's time to send tractor PTO information and the FRONT PTO is engaged
      sendMessage(sendFrontPto);
    }

    ///Timeperiod of 100ms is set in ::config
    if ( mt_ptoRear.t_ptoEngaged == IsoAgLib::IsoActive  )
    { // it's time to send tractor PTO information and the REAR PTO is engaged
      sendMessage(sendRearPto);
    }
  }

  /** Detect stop of PTO update from tractor -> indication for stopped PTO */
  void TracPTO_c::timeEventImplMode()
  {
    const int32_t ci32_now = System_c::getTime();
    // check for different pto data types whether the previously
    // sending node stopped sending -> other nodes can now step in
    if ( ( (ci32_now - mt_ptoFront.i32_lastPto)  >= CONFIG_TIMEOUT_TRACTOR_DATA
           || getSelectedDataSourceISOName().isUnspecified()
         )
         && ( (IsoAgLib::IsoActiveFlag_t)(mt_ptoFront.ui16_pto8DigitPerRpm) != 0 && (IsoAgLib::IsoActiveFlag_t)(mt_ptoFront.t_ptoEngaged) != IsoAgLib::IsoInactive)
       )
    { // TECU stoppped its PTO and doesn'T send PTO updates - as defined by ISO 11783
      // --> switch values to ZERO
      mt_ptoFront.ui16_pto8DigitPerRpm = 0;
      mt_ptoFront.t_ptoEngaged = IsoAgLib::IsoInactive;
    }
    if ( ( ( ci32_now - mt_ptoRear.i32_lastPto ) >= CONFIG_TIMEOUT_TRACTOR_DATA
           || (getSelectedDataSourceISOName().isUnspecified() )
         )
         && ((IsoAgLib::IsoActiveFlag_t)(mt_ptoRear.ui16_pto8DigitPerRpm) != 0 && (IsoAgLib::IsoActiveFlag_t)(mt_ptoRear.t_ptoEngaged) != IsoAgLib::IsoInactive )
       )
    { // TECU stoppped its PTO and doesn'T send PTO updates - as defined by ISO 11783
      // --> switch values to ZERO
      mt_ptoRear.ui16_pto8DigitPerRpm = 0;
      mt_ptoRear.t_ptoEngaged = IsoAgLib::IsoInactive;
    }
  }

  /** send pto data message
      @param t_sendptodata  send pto front or pto rear
      @see  TracCert_c::processMsgRequestPGN
      @see  CanIo_c::operator<<
    */
  void TracPTO_c::sendMessage(SendPtoData_t t_sendptodata)
  {
    if( ( ! getIdentItem() ) || ( ! getIdentItem()->isClaimedAddress() ) )
      return;

    IsoBus_c& c_can = getIsoBusInstance4Comm();

    CanPkgExt_c pkg;
    pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
    pkg.setIsoPri(3);
    pkg.setLen(8);

    setSelectedDataSourceISOName( getIdentItem()->isoName() );

    PtoData_t* pt_ptoData = NULL;
    if (t_sendptodata == sendFrontPto)
    {
      pkg.setIsoPgn(FRONT_PTO_STATE_PGN);
      pt_ptoData = &mt_ptoFront;
    }
    else
    { //sendRearPto
      pkg.setIsoPgn(REAR_PTO_STATE_PGN);
      pt_ptoData = &mt_ptoRear;
    }

    uint8_t ui8_val;
    pkg.setUint16Data(0, pt_ptoData->ui16_pto8DigitPerRpm);
    pkg.setUint16Data(2, pt_ptoData->ui16_ptoSetPoint8DigitPerRpm);
    ui8_val =  (pt_ptoData->t_ptoEngaged             << 6);
    ui8_val |= (pt_ptoData->t_pto1000                << 4);
    ui8_val |= (pt_ptoData->t_ptoEconomy             << 2);
    ui8_val |= (pt_ptoData->t_ptoEngagementReqStatus << 0);
    pkg.setUint8Data(4, ui8_val);
    ui8_val = 0;
    ui8_val |= (pt_ptoData->t_ptoModeReqStatus         << 6);
    ui8_val |= (pt_ptoData->t_ptoEconomyModeReqStatus  << 4);
    ui8_val |= (pt_ptoData->t_ptoShaftSpeedLimitStatus << 1);
    pkg.setUint8Data(5, ui8_val);
    //reserved fields
    pkg.setUint16Data(6, 0xFFFF);

    c_can << pkg;

    // update time
    pt_ptoData->i32_lastPto = System_c::getTime();
  }

  /** force a request for pgn for front pto state */
  bool TracPTO_c::sendRequestUpdateFront()
  {
    if ( checkMode(IsoAgLib::IdentModeImplement) )
      return BaseCommon_c::sendPgnRequest(FRONT_PTO_STATE_PGN);
    else
      return false;
  }
  /** force a request for pgn for rear pto state */
  bool TracPTO_c::sendRequestUpdateRear()
  {
    if ( checkMode(IsoAgLib::IdentModeImplement) )
      return BaseCommon_c::sendPgnRequest(REAR_PTO_STATE_PGN);
    else
      return false;
  }



} // End Namespace __IsoAgLib
