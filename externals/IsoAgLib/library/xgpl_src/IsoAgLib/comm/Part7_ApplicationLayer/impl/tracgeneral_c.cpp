/*
  tracgeneral_c.cpp: stores, updates and delivers all base data
    informations from CanCustomer_c derived for CAN sending and
    receiving interaction; from SchedulerTask_c derived for
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
#include "tracgeneral_c.h"
#include "tracpto_c.h"

namespace {
  int32_t const sci32_timeoutMaintenancePgn = 2000;
}


namespace __IsoAgLib { // Begin Namespace __IsoAgLib

  /** C-style function, to get access to the unique TracGeneral_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  TracGeneral_c &getTracGeneralInstance( uint8_t aui8_instance )
  { // if > 1 singleton instance is used, no static reference can be used
    MACRO_MULTITON_GET_INSTANCE_BODY(TracGeneral_c, PRT_INSTANCE_CNT, aui8_instance);
  }

  TracGeneral_c::TracGeneral_c()
    : mb_languageTecuReceived (false)
    , mb_languagePgnChanged (false)
  {
    mp8ui8_languageTecu[0] = 0; // OK
    mp8ui8_languageTecu[1] = 0;  //OK
    mp8ui8_languageTecu[2] = 0x0F; //OK
    mp8ui8_languageTecu[3] = 0; //OK
    mp8ui8_languageTecu[4] = 0; //OK
    mp8ui8_languageTecu[5] = 0; //OK
    mp8ui8_languageTecu[6] = 0xFF; //OK
    mp8ui8_languageTecu[7] = 0xFF; //OK
  }


  bool TracGeneral_c::config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask)
  { // set configure values
    //store old mode to decide to register or unregister to request for pgn
    IsoAgLib::IdentMode_t t_oldMode = getMode();

    //call config for handling which is base data independent
    //if something went wrong leave function before something is configured
    if ( ! BaseCommon_c::config_base ( apc_ident, at_identMode, aui16_suppressMask) ) return false;


    ///Set time Period for Scheduler_c
    if (at_identMode == IsoAgLib::IdentModeTractor)
    { // SEND data with short period
      mt_task.setPeriod( 100, false );
    }
    else
    { // check with long period for timeout after loss of sending node
      mt_task.setPeriod( 1000, false );
    }

    // set the member base msg value vars to NO_VAL codes
    setHitchRear(NO_VAL_8);
    setHitchFront(NO_VAL_8);
    mi16_engine = NO_VAL_16S;

    // un-/register to PGN
    if (t_oldMode == at_identMode)
      ; // no change, still the same mode
    else if (at_identMode == IsoAgLib::IdentModeTractor) {
      // a change from Implement mode to Tractor mode occured
      RegisterPgn_s s_register = getRegisterPgn();
      if (canSendLanguage())
        s_register(LANGUAGE_PGN);
      if (canSendFrontHitchState())
        s_register(FRONT_HITCH_STATE_PGN);
      if (canSendRearHitchState())
        s_register(REAR_HITCH_STATE_PGN);
    } else {
      // a change from Tractor mode to Implement mode occured
      UnregisterPgn_s s_unregister = getUnregisterPgn();
      if (canSendLanguage())
        s_unregister(LANGUAGE_PGN);
      if (canSendFrontHitchState())
        s_unregister(FRONT_HITCH_STATE_PGN);
      if (canSendRearHitchState())
        s_unregister(REAR_HITCH_STATE_PGN);
    }

    mi32_lastIsoPositionSimple = 0;

    mt_keySwitch = IsoAgLib::IsoNotAvailable; // mark as not available
    mui8_maxPowerTime = mui8_frontLinkForce = mui8_rearLinkForce = NO_VAL_8;
    mui16_frontDraft = mui16_rearDraft = NO_VAL_16;
    mi32_lastMaintainPowerRequest = -1;
    mb_maintainEcuPower = mb_maintainActuatorPower = false;
    mt_implState.i32_lastMaintainPowerRequest = -1;
    mt_implState.b_maintainEcuPower = mt_implState.b_maintainActuatorPower = false;
    mt_implState.inPark =      IsoAgLib::IsoNotAvailablePark;
    mt_implState.inTransport = IsoAgLib::IsoNotAvailableTransport;
    mt_implState.inWork =      IsoAgLib::IsoNotAvailableWork;
    mt_frontHitchPosLimitStatus = mt_rearHitchPosLimitStatus = IsoAgLib::IsoNotAvailableLimit;

    return true;
  };

  void TracGeneral_c::checkCreateReceiveFilter( )
  {
    if ( !checkFilterCreated() )
    {
      setFilterCreated();

      IsoBus_c &c_can = getIsoBusInstance4Comm();
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (FRONT_HITCH_STATE_PGN<<8) ), 8 );
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (REAR_HITCH_STATE_PGN<<8) ), 8 );
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (MAINTAIN_POWER_REQUEST_PGN<<8) ), 8 );
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (LANGUAGE_PGN<<8) ), 8 );
    }
  }

  /** send a ISO11783 general base information PGN.
    * this is only called when sending ident is configured and it has already claimed an address
    * Timeperiod of 100ms is set in config()
      @pre  function is only called in tractor mode
      @see  BaseCommon_c::timeEvent()
    */
  void TracGeneral_c::timeEventTracMode( )
  { ///Timeperiod of 100ms is set in ::config
    sendMessage();

    updateMaintainPowerRequest();

    if (mb_languagePgnChanged)
    {
      sendLanguage();
      mb_languagePgnChanged = false;
    }
  }

  /** process a ISO11783 base information PGN
      @pre  sender of message is existent in monitor list
      @see  CANPkgExt_c::resolveSendingInformation()
    */
  void TracGeneral_c::processMsg( const CanPkg_c& arc_data )
  {
    CanPkgExt_c pkg( arc_data, getMultitonInst() );
    if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
      return;

    IsoName_c const& rcc_tempISOName = pkg.getISONameForSA();

    switch (pkg.isoPgn() /* & 0x3FFFF */) // don't need to &0x3FFFF, as this is the whole PGN...
    {
      case FRONT_HITCH_STATE_PGN:
      case REAR_HITCH_STATE_PGN:
        // only take values, if i am not the regular sender
        // and if actual sender isn't in conflict to previous sender
        if ( checkParseReceived( rcc_tempISOName ) )
        { // sender is allowed to send
          uint8_t ui8_tempHitch = (( pkg.getUint8Data( 0 ) * 4) / 10 );
          if ( (ui8_tempHitch != ERROR_VAL_8)
              && (ui8_tempHitch != NO_VAL_8) ) {
            switch ( (pkg.getUint8Data( 1 ) >> 6) & 3 ) {
            case 0: // not in work
              break;
            case 1: // in work
              ui8_tempHitch |= 0x80;
              break;
            case 2: // Error
              ui8_tempHitch = ERROR_VAL_8;
              break;
            case 3: // Not available
              ui8_tempHitch = NO_VAL_8;
              break;
            }
          }
          if (pkg.isoPgn() == FRONT_HITCH_STATE_PGN)
          { // front hitch
            setHitchFront(ui8_tempHitch);
            mui8_frontLinkForce = pkg.getUint8Data( 2 );
            mui16_frontDraft = static_cast<uint16_t>(pkg.getUint8Data( 3 ) ) + (static_cast<uint16_t>(pkg.getUint8Data( 4 ) ) << 8);
            mt_frontHitchPosLimitStatus = IsoAgLib::IsoLimitFlag_t( ( pkg.getUint8Data(1) >> 3 ) & 3 );
          }
          else
          { // back hitch
            setHitchRear(ui8_tempHitch);
            mui8_rearLinkForce = pkg.getUint8Data( 2 );
            mui16_rearDraft = static_cast<uint16_t>(pkg.getUint8Data( 3 ) ) + (static_cast<uint16_t>(pkg.getUint8Data( 4 )) << 8);
            mt_rearHitchPosLimitStatus = IsoAgLib::IsoLimitFlag_t( ( pkg.getUint8Data(1) >> 3 ) & 3 );
          }
          setSelectedDataSourceISOName (rcc_tempISOName);
          //set update time
          setUpdateTime( pkg.time() );
        }
        else
        { // there is a sender conflict
          IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracMultipleSender, getMultitonInst() );
        }
        break;

      case MAINTAIN_POWER_REQUEST_PGN: // maintain power request
      {
        indicatedStateImpl_t &rt_indicateData = mmap_indicatedState[pkg.getISONameForSA()];

        rt_indicateData.b_maintainEcuPower =      ( ( (pkg.getUint8Data( 0 ) >> 6) & 3) == 1);
        rt_indicateData.b_maintainActuatorPower = ( ( (pkg.getUint8Data( 0 ) >> 4) & 3) == 1);

        rt_indicateData.inTransport = ( (pkg.getUint8Data( 1 ) >> 6) & 3 );
        rt_indicateData.inPark =      ( (pkg.getUint8Data( 1 ) >> 4) & 3 );
        rt_indicateData.inWork =      ( (pkg.getUint8Data( 1 ) >> 2) & 3 );

        rt_indicateData.i32_lastMaintainPowerRequest = pkg.time();
        mi32_lastMaintainPowerRequest = pkg.time();

        updateMaintainPowerRequest();
        break;
      }

      case LANGUAGE_PGN:
        if (pkg.getISONameForSA().getEcuType() == IsoName_c::ecuTypeTractorECU)
        { // only from nodes that indicate "TractorECU"
          mb_languageTecuReceived = true;
          for (int i=0; i<8; i++)
          {
            mp8ui8_languageTecu[i] = pkg.getUint8Data(i);
          }
        }
        break;
    }
  }

  bool TracGeneral_c::processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t )
  {
    // check if we are allowed to send a request for pgn
    if ( ! BaseCommon_c::check4ReqForPgn(aui32_pgn, apc_isoItemSender, apc_isoItemReceiver) )
      return false;

    bool b_processed = true;
    switch (aui32_pgn) {
    default:
      b_processed = false;
      break;
    case LANGUAGE_PGN:
      // call TracGeneral_c function to send language of Tractor-ECU
      // sendLanguage checks if this item (identified by ISOName)
      // is configured to send language
      sendLanguage();
      break;
    case FRONT_HITCH_STATE_PGN:
      b_processed = HitchStateSent == sendFrontHitchState();
      break;
    case REAR_HITCH_STATE_PGN:
      b_processed = HitchStateSent == sendRearHitchState();
      break;
    }
    return b_processed;
  }

  /** send front hitch and rear hitch data msg
      @see  CanIo_c::operator<<
    */
  void TracGeneral_c::sendMessage()
  {
    (void)sendFrontHitchState();
    (void)sendRearHitchState();
  }

  TracGeneral_c::SendHitchState_e TracGeneral_c::prepareSendingHitchState( CanPkgExt_c& pkg)
  {
    //check for isoName and if address claim has yet occured, because this function can also bo called
    //independent from timeEvent() function
    if( ( ! getIdentItem() ) || ( ! getIdentItem()->getIsoItem() ) )
      return HitchStateNotSent;

    pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
    pkg.setIsoPri(3);
    pkg.setLen(8);

    setSelectedDataSourceISOName( getIdentItem()->isoName()  );
    return HitchStateSent;
  }

  TracGeneral_c::SendHitchState_e TracGeneral_c::sendFrontHitchState()
  {
    if ( !canSendFrontHitchState() )
      return HitchStateNotSent;

    CanPkgExt_c pkg;
    SendHitchState_e const e_sending = prepareSendingHitchState( pkg );
    if (HitchStateNotSent == e_sending)
      return e_sending;
    pkg.setIsoPgn(FRONT_HITCH_STATE_PGN);

    uint8_t ui8_temp = 0x7;  /* Pre-load the reserved bits */
    ui8_temp |= mt_frontHitchPosLimitStatus << 3;
    switch (hitchFront()) {
    case ERROR_VAL_8:
      pkg.setUint8Data(0, hitchFront());
      ui8_temp |= ( IsoAgLib::IsoError << 6 );
      pkg.setUint8Data(1, ui8_temp);
      break;
    case NO_VAL_8:
      pkg.setUint8Data(0, hitchFront());
      ui8_temp |= ( IsoAgLib::IsoNotAvailable << 6 );
      pkg.setUint8Data(1, ui8_temp);
      break;
    default:
      pkg.setUint8Data(0, ((hitchFront() & 0x7F)*10/4));
      if ((hitchFront() & 0x80) != 0)
      {
        ui8_temp |= IsoAgLib::IsoActive << 6;
        pkg.setUint8Data(1, ui8_temp ); // work
      }
      else
      {
        ui8_temp |= IsoAgLib::IsoInactive << 6;
        pkg.setUint8Data(1, ui8_temp);
      }
      break;
    }
    pkg.setUint8Data(2, mui8_frontLinkForce);
    pkg.setUint8Data(3, mui16_frontDraft& 0xFF);
    pkg.setUint8Data(4, (mui16_frontDraft >> 8) );

    /* Reserved Bytes */
    pkg.setUint8Data(5, 0xFF );
    pkg.setUint16Data(6, 0xFFFF );

    // CanIo_c::operator<< retreives the information with the help of CanPkg_c::getData
    // then it sends the data
    getIsoBusInstance4Comm() << pkg;
    return HitchStateSent;
  }

  TracGeneral_c::SendHitchState_e TracGeneral_c::sendRearHitchState()
  {
    if ( !canSendRearHitchState() )
      return HitchStateNotSent;

    CanPkgExt_c pkg;
    SendHitchState_e const e_sending = prepareSendingHitchState( pkg );
    if (HitchStateNotSent == e_sending)
      return e_sending;
    pkg.setIsoPgn(REAR_HITCH_STATE_PGN);
    uint8_t ui8_temp = 0x7;  /* Pre-load the reserved bits */
    ui8_temp = mt_rearHitchPosLimitStatus << 3;
    switch (hitchRear()) {
    case ERROR_VAL_8:
      pkg.setUint8Data(0, hitchRear());
      ui8_temp |= ( IsoAgLib::IsoError << 6 );
      pkg.setUint8Data(1, ui8_temp);
      break;
    case NO_VAL_8:
      pkg.setUint8Data(0, hitchRear());
      ui8_temp |= ( IsoAgLib::IsoNotAvailable << 6 );
      pkg.setUint8Data(1, ui8_temp);
      break;
    default:
      pkg.setUint8Data(0, ((hitchRear() & 0x7F)*10/4));
      if ((hitchRear() & 0x80) != 0)
      {
        ui8_temp |= IsoAgLib::IsoActive << 6;
        pkg.setUint8Data(1, ui8_temp ); // work
      }
      else
      {
        ui8_temp |= IsoAgLib::IsoInactive << 6;
        pkg.setUint8Data(1, ui8_temp);
      }
      break;
    }
    pkg.setUint8Data(2, mui8_rearLinkForce);
    pkg.setUint8Data(3, (mui16_rearDraft& 0xFF) );
    pkg.setUint8Data(4, mui16_rearDraft >> 8);

    /* Reserved Bytes */
    pkg.setUint8Data(5, 0xFF );
    pkg.setUint16Data(6, 0xFFFF );

    // CanIo_c::operator<< retreives the information with the help of CanPkg_c::getData
    // then it sends the data
    getIsoBusInstance4Comm() << pkg;
    return HitchStateSent;
  }

  /** send iso language data msg
      @see  TracGeneral_c::processMsgRequestPGN
      @see  CanIo_c::operator<<
    */
  TracGeneral_c::SendLanguage_e TracGeneral_c::sendLanguage()
  {
    //language is only send in tractor mode
    if ( checkMode(IsoAgLib::IdentModeImplement) )
      return LanguageNotSent;

    if ( !canSendLanguage() )
      return LanguageNotSent;

    // we only send the TECU language settings
    // those will be overwritten by the VT-language
    /*if (  !mb_languageVtReceived
          || ( getISOName() == NULL ) // shouldn't be NULL as we're in tractor-mode
          || ( getISOName()->isUnspecified()  )
          || !getIsoMonitorInstance4Comm().existIsoMemberISOName(*getISOName(), true)
      )
    { //if VT has up to now not send the language command there is no sense to send it
      return LanguageNotSent;
    }
    data().setISONameForSA( *getISOName() );
    data().setIsoPri(6);
    data().setLen(8);

    setSelectedDataSourceISOName( *getISOName() );
    IsoBus_c& c_can = getIsoBusInstance4Comm();
    data().setIsoPgn(LANGUAGE_PGN);
    //Bytes 1,2: language command
    data().setUint16Data(0, (mp8ui8_languageVt[0] | (mp8ui8_languageVt[1] << 8)) );
    //Byte 3: number format; Bit 1-4: reserved, Bit 5,6: time format, Bit 7,8: decimal symbol
    data().setUint8Data(2, mp8ui8_languageVt[2]);
    //Byte 4: date format
    data().setUint8Data(3, mp8ui8_languageVt[3]);
    //Byte 5: units of measure; Bit 1,2: mass units, Bit 3,4: volume units, Bit 5,6: area units, Bit 7,8: distance units
    data().setUint8Data(4, mp8ui8_languageVt[4]);
    //Byte 6: units of measure; Bit 1,2: units system; Bit 3,4; force units, Bit 5,6: pressure units, Bit 7,8: temperature units
    data().setUint8Data(5, mp8ui8_languageVt[5]);
    //Bytes 7,8: reserved
    data().setUint8Data(6, 0xFF);
    data().setUint8Data(7, 0xFF);

    c_can << data();*/

    if( ( ( ! getIdentItem() ) || ( ! getIdentItem()->isClaimedAddress() ) ) )
    { //if VT has up to now not send the language command there is no sense to send it
      return LanguageNotSent;
    }
    CanPkgExt_c pkg;
    pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
    pkg.setIsoPri(6);
    pkg.setLen(8);

    setSelectedDataSourceISOName( getIdentItem()->isoName() );
    IsoBus_c& c_can = getIsoBusInstance4Comm();
    pkg.setIsoPgn(LANGUAGE_PGN);
    //Bytes 1,2: language command
    pkg.setUint16Data(0, (mp8ui8_languageTecu[0] | (mp8ui8_languageTecu[1] << 8)) );
    //Byte 3: number format; Bit 1-4: reserved, Bit 5,6: time format, Bit 7,8: decimal symbol
    pkg.setUint8Data(2, mp8ui8_languageTecu[2]);
    //Byte 4: date format
    pkg.setUint8Data(3, mp8ui8_languageTecu[3]);
    //Byte 5: units of measure; Bit 1,2: mass units, Bit 3,4: volume units, Bit 5,6: area units, Bit 7,8: distance units
    pkg.setUint8Data(4, mp8ui8_languageTecu[4]);
    //Byte 6: units of measure; Bit 1,2: units system; Bit 3,4; force units, Bit 5,6: pressure units, Bit 7,8: temperature units
    pkg.setUint8Data(5, mp8ui8_languageTecu[5]);
    //Bytes 7,8: reserved
    pkg.setUint8Data(6, 0xFF);
    pkg.setUint8Data(7, 0xFF);

    c_can << pkg;
    return LanguageSent;
  }

 /** force maintain power from tractor
     @see  CanIo_c::operator<<
     @param at_ecuPower IsoActive -> maintain ECU power
     @param at_actuatorPower IsoActive-> maintain actuator power
   */
  void TracGeneral_c::forceMaintainPower( IsoAgLib::IsoActiveFlag_t at_ecuPower, IsoAgLib::IsoActiveFlag_t at_actuatorPower )
  {
    if ( (mui16_suppressMask & MAINTAIN_POWER_REQUEST_PGN_DISABLE_MASK) != 0)
      return;

    isoaglib_assert( getIdentItem() );
    if ( ! getIdentItem()->isClaimedAddress() )
      return;

    CanPkgExt_c pkg;
    pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
    pkg.setIsoPri(6);
    pkg.setIsoPgn(MAINTAIN_POWER_REQUEST_PGN);
    pkg.setUint8Data(0, ( at_ecuPower      << 6) |
                        ( at_actuatorPower << 4) );
    pkg.setUint8Data(1, ( mt_implState.inTransport << 6) |
                        ( mt_implState.inPark      << 4) |
                        ( mt_implState.inWork      << 2) );
    pkg.setUint16Data(2, 0xFFFFU);
    pkg.setUint32Data(4, 0xFFFFFFFFUL);
    pkg.setLen(8);
    getIsoBusInstance4Comm() << pkg;
  }

  /** force a request for pgn for language information */
  bool TracGeneral_c::sendRequestUpdateLanguage()
  {
    if ( checkMode(IsoAgLib::IdentModeImplement) )
      return BaseCommon_c::sendPgnRequest(LANGUAGE_PGN);
    else
      return false;
  }

  /** clean map of maintain power data, set Maintain Power value */
  void TracGeneral_c::updateMaintainPowerRequest()
  {
    mb_maintainEcuPower = false;
    mb_maintainActuatorPower = false;

    const int32_t i32_now = HAL::getTime();
    for (STL_NAMESPACE::map< IsoName_c, indicatedStateImpl_t >::iterator iter = mmap_indicatedState.begin();
         iter != mmap_indicatedState.end();
        )
    {
      if ((iter->second.i32_lastMaintainPowerRequest + sci32_timeoutMaintenancePgn) < i32_now)
        mmap_indicatedState.erase(iter++);
      else
      {
        mb_maintainEcuPower |= iter->second.b_maintainEcuPower;
        mb_maintainActuatorPower |= iter->second.b_maintainActuatorPower;
        ++iter;
      }
    }
  }

  /** check if timeout reached since last receive time of maintain power request */
  bool
  TracGeneral_c::timeOutMaintainPowerRequest() const
  {
    return (mmap_indicatedState.empty());
  }

  /**
    * set the ISO key switch state of the tractor
    * @pre valid IsoName_c instance should exist
    * @param at_val IsoActive -> key switch ON
    */
  void TracGeneral_c::setKeySwitch(IsoAgLib::IsoActiveFlag_t at_val)
  {
    mt_keySwitch = at_val;

    if (checkMode(IsoAgLib::IdentModeTractor))
    {

      if (IsoAgLib::IsoActive != at_val)
      {
        // IsoInactive, IsoError and IsoNotAvailable
        indicatedStateImpl_t &rt_indicateData = mmap_indicatedState[getIdentItem()->isoName()];
        rt_indicateData.b_maintainEcuPower = true;
        rt_indicateData.b_maintainActuatorPower = true;
        rt_indicateData.i32_lastMaintainPowerRequest = mi32_lastMaintainPowerRequest = HAL::getTime();
      }
      else
      {
        // IsoActive
        mmap_indicatedState.clear();
      }

      updateMaintainPowerRequest();
    }
  }

void
TracGeneral_c::updateLanguage(char aac_language[2])
{
  mp8ui8_languageTecu[0] = aac_language[0];
  mp8ui8_languageTecu[1] = aac_language[1];
  mb_languagePgnChanged = true;
}

void
TracGeneral_c::updateDecimalSymbol(uint8_t aui8_decimalSymbol)
{
  mp8ui8_languageTecu[2] = ((aui8_decimalSymbol & 0x3) << 6) | (mp8ui8_languageTecu[2] & 0x3F);
  mb_languagePgnChanged = true;
}

void TracGeneral_c::updateDateFormat(uint8_t aui8_dateFormat)
{
  mp8ui8_languageTecu[3] = aui8_dateFormat;
  mb_languagePgnChanged = true;
}

void
TracGeneral_c::updateTimeFormat(uint8_t aui8_timeFormat)
{
  mp8ui8_languageTecu[2] = ((aui8_timeFormat & 0x3) << 4) | (mp8ui8_languageTecu[2] & 0xCF);
  mb_languagePgnChanged = true;
}

void
TracGeneral_c::updateDistanceUnit(uint8_t aui8_distanceUnit)
{
  mp8ui8_languageTecu[4] = ((aui8_distanceUnit & 0x3) << 6) | (mp8ui8_languageTecu[4] & 0x3F);
  mb_languagePgnChanged = true;
}

void
TracGeneral_c::updateAreaUnit(uint8_t aui8_areaUnit)
{
  mp8ui8_languageTecu[4] = ((aui8_areaUnit & 0x3) << 4) | (mp8ui8_languageTecu[4] & 0xCF);
  mb_languagePgnChanged = true;
}

void
TracGeneral_c::updateVolumeUnit(uint8_t aui8_volumeUnit)
{
  mp8ui8_languageTecu[4] = ((aui8_volumeUnit & 0x3) << 2) | (mp8ui8_languageTecu[4] & 0xF3);
  mb_languagePgnChanged = true;
}

void
TracGeneral_c::updateMassUnit(uint8_t aui8_massUnit)
{
  mp8ui8_languageTecu[4] = (aui8_massUnit & 0x3) | (mp8ui8_languageTecu[4] & 0xFC);
  mb_languagePgnChanged = true;
}

void
TracGeneral_c::updateTemperatureUnit(uint8_t aui8_temperatureUnit)
{
  mp8ui8_languageTecu[5] = ((aui8_temperatureUnit & 0x3) << 6) | (mp8ui8_languageTecu[5] & 0x3F);
  mb_languagePgnChanged = true;
}

void
TracGeneral_c::updatePressureUnit(uint8_t aui8_pressureUnit)
{
  mp8ui8_languageTecu[5] = ((aui8_pressureUnit & 0x3) << 4) | (mp8ui8_languageTecu[5] & 0xCF);
  mb_languagePgnChanged = true;
}

void
TracGeneral_c::updateForceUnit(uint8_t aui8_forceUnit)
{
  mp8ui8_languageTecu[5] = ((aui8_forceUnit & 0x3) << 2) | (mp8ui8_languageTecu[5] & 0xF3);
  mb_languagePgnChanged = true;
}

void
TracGeneral_c::updateUnitSystem(uint8_t aui8_unitSystem)
{
  mp8ui8_languageTecu[5] = (aui8_unitSystem & 0x3) | (mp8ui8_languageTecu[5] & 0xFC);
  mb_languagePgnChanged = true;
}



} // End Namespace __IsoAgLib
