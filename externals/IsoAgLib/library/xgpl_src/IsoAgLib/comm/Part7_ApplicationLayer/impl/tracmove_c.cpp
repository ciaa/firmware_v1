/*
  tracmove_c.cpp: stores, updates and delivers all moving data
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

#include "tracmove_c.h"
#include <IsoAgLib/comm/impl/isobus_c.h>
#if defined(USE_BASE) || defined(USE_TRACTOR_GENERAL)
  #include <IsoAgLib/comm/Part7_ApplicationLayer/impl/tracgeneral_c.h>
#endif
#include <IsoAgLib/util/impl/util_funcs.h>


#if ( (defined USE_BASE || defined USE_TIME_GPS) && defined ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
  #include <IsoAgLib/comm/Part7_ApplicationLayer/impl/timeposgps_c.h>
#endif
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgn_c.h>


#define TIMEOUT_SPEED_LOST 3000
#define MISSING_SPEED         0

namespace __IsoAgLib { // Begin Namespace __IsoAglib

  /** C-style function, to get access to the unique TracMove_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  TracMove_c &getTracMoveInstance( uint8_t aui8_instance )
  { // if > 1 singleton instance is used, no static reference can be used
    MACRO_MULTITON_GET_INSTANCE_BODY(TracMove_c, PRT_INSTANCE_CNT, aui8_instance);
  }

  bool TracMove_c::config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask)
  {
    //store old mode to decide to register or unregister to request for pgn
    IsoAgLib::IdentMode_t t_oldMode = getMode();

    //call config for handling which is base data independent
    //if something went wrong leave function before something is configured
    if ( ! BaseCommon_c::config_base ( apc_ident, at_identMode, aui16_suppressMask ) ) return false;

    ///Set time Period for Scheduler_c
    if (at_identMode == IsoAgLib::IdentModeTractor)
    { // SEND data with short period
      mt_task.setPeriod( 100, false );
    }
    else
    { // check with long period for timeout after loss of sending node
      mt_task.setPeriod( 1000, false );
    }

    // un-/register to PGN
    if (t_oldMode == at_identMode)
      ; // no change, still the same mode
    else if (at_identMode == IsoAgLib::IdentModeTractor) {
      // a change from Implement mode to Tractor mode occured
      // create FilterBox_c for REQUEST_PGN_MSG_PGN, LANGUAGE_PGN
      RegisterPgn_s s_register = getRegisterPgn();
      if (canSendGroundBasedSpeedDist())
        s_register(GROUND_BASED_SPEED_DIST_PGN);
      if (canSendWheelBasedSpeedDist())
        s_register(WHEEL_BASED_SPEED_DIST_PGN);
      if (canSendSelectedSpeed())
        s_register(SELECTED_SPEED_PGN);
      if (canSendEngineSpeed())
        s_register(ELECTRONIC_ENGINE_CONTROLLER_1_PGN);
    } else {
      // a change from Tractor mode to Implement mode occured
      // unregister from request for pgn, because in implement mode no request should be answered
      UnregisterPgn_s s_unregister = getUnregisterPgn();
      if (canSendGroundBasedSpeedDist())
        s_unregister(GROUND_BASED_SPEED_DIST_PGN);
      if (canSendWheelBasedSpeedDist())
        s_unregister(WHEEL_BASED_SPEED_DIST_PGN);
      if (canSendSelectedSpeed())
        s_unregister(SELECTED_SPEED_PGN);
      if (canSendEngineSpeed())
        s_unregister(ELECTRONIC_ENGINE_CONTROLLER_1_PGN);
    }

    // set distance value to NO_VAL codes
    mui32_distReal = mui32_distTheor = NO_VAL_32;

    // set the member msg value vars to NO_VAL codes
    mi32_speedReal = mi32_speedTheor = NO_VAL_32S;

    mt_operatorDirectionReversed = IsoAgLib::IsoNotAvailableReversed;
    mt_startStopState = IsoAgLib::IsoNotAvailable;

    mui32_selectedDistance = 0xFFFFFFFF;
    mi32_selectedSpeed = NO_VAL_32S;
    mt_selectedDirection = mt_directionReal = mt_directionTheor = IsoAgLib::IsoNotAvailableDirection;
    mt_selectedSpeedSource = IsoAgLib::IsoNotAvailableSpeed;
    mt_selectedSpeedLimitStatus = IsoAgLib::IsoNotAvailableLimit;

    mt_speedSource = IsoAgLib::NoSpeed;
    mt_distDirecSource = IsoAgLib::NoDistDirec;
    // actually the 3000 seem to be a dummy value, but not 100% sure on this!
    mui32_lastUpdateTimeSpeedSelected = 3000;
    mui32_lastUpdateTimeSpeedReal = 3000;
    mui32_lastUpdateTimeSpeedTheor = 3000;
    mui32_lastUpdateTimeDistDirec = 3000;

    return true;
  };

  /** is looking for a valid speed value
      @return true if speed is valid otherwise false
    */
  bool TracMove_c::isSelectedSpeedUsable() const
  {
    if ( isSelectedSpeedMissing() ) return false;
    else if ( isSelectedSpeedErroneous() ) return false;
    else return true;
  }

  /** is looking for a valid speed value
      @return true if speed is valid otherwise false
    */
  bool TracMove_c::isRealSpeedUsable() const
  {
    if ( isRealSpeedMissing() ) return false;
    else if ( isRealSpeedErroneous() ) return false;
    else return true;
  }

  /** is looking for a valid speed value
      @return true if speed is valid otherwise false
    */
  bool TracMove_c::isTheorSpeedUsable() const
  {
    if ( isTheorSpeedMissing() ) return false;
    else if ( isTheorSpeedErroneous() ) return false;
    else return true;
  }

  /** check if filter boxes shall be created and create them */
  void TracMove_c::checkCreateReceiveFilter()
  {
    if ( ! checkFilterCreated() )
    { // check if needed receive filters for ISO are active
      setFilterCreated();

      IsoBus_c &c_can = getIsoBusInstance4Comm();
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (GROUND_BASED_SPEED_DIST_PGN<<8) ), 8 );
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (WHEEL_BASED_SPEED_DIST_PGN<<8) ), 8 );
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (SELECTED_SPEED_PGN<<8) ), 8 );
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (ELECTRONIC_ENGINE_CONTROLLER_1_PGN<<8) ), 8 );
    }
  }

  /** process a ISO11783 base information PGN
      @pre  sender of message is existent in monitor list
      @see  CanPkgExt_c::resolveSendingInformation()
    */
  void TracMove_c::processMsg( const CanPkg_c& arc_data )
  {
    CanPkgExt_c pkg( arc_data, getMultitonInst() );
    if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
      return;

    IsoName_c const& rcc_tempISOName = pkg.getISONameForSA();

    #if defined(USE_BASE) || defined(USE_TRACTOR_GENERAL)
    TracGeneral_c& c_tracgeneral = getTracGeneralInstance4Comm();
    #endif

    const int32_t ci32_now = pkg.time();

    switch (pkg.isoPgn() /*& 0x3FFFF*/) // don't need to &0x3FFFF as this is the whole PGN...
    {
      case GROUND_BASED_SPEED_DIST_PGN:
      case WHEEL_BASED_SPEED_DIST_PGN:
        // only take values, if i am not the regular sender
        // and if actual sender isn't in conflict to previous sender
        if ( ( checkParseReceived( rcc_tempISOName ) ) )/*|| noch keine normale geschw empfangen */
        { // sender is allowed to send
          int32_t i32_tempSpeed = pkg.getUint16Data(0);
          bool b_usableSpeed = true;
          uint32_t ui32_tempDist = pkg.getUint32Data( 2 );

          // handle special values
          switch (pkg.getUint16Data(0))
          {
            case NO_VAL_16: // missing signal --> match to NO_VAL_32S
              i32_tempSpeed = NO_VAL_32S;
              b_usableSpeed = false;
              break;
            case ERROR_VAL_16: // erroneous signal --> match to ERROR_VAL_32S
              i32_tempSpeed = ERROR_VAL_32S;
              b_usableSpeed = false;
              break;
            default: // handle too high value as erroneous signal
              if ( i32_tempSpeed > 0xFAFF ) {
                i32_tempSpeed = ERROR_VAL_32S;
                b_usableSpeed = false;
              }
              break;
          }

          // if distance value is outside valid area, the special error state flag-value ERROR_VAL_32 should be used
          // but if NO_VAL_32 indicates missing information, this information should NOT be overwritten
          if ( ( ui32_tempDist > 0xFAFFFFFF ) && ( ui32_tempDist != NO_VAL_32 ) ) ui32_tempDist = ERROR_VAL_32;

          if ( ( (pkg.getUint8Data( 7 ) & 3 ) == IsoAgLib::IsoReverse) && (b_usableSpeed) )
            //if direction is in error state this has no effect to the speed;
            //if direction is not available it is assumed that the speed is positive
            i32_tempSpeed *= -1; //driving reverse;


          const uint32_t testTimeOutdatedSpeed = (pkg.time() - mui32_lastUpdateTimeSpeedSelected);
          const uint32_t testTimeOutdatedDist  = (pkg.time() - mui32_lastUpdateTimeDistDirec);
          if (pkg.isoPgn() == GROUND_BASED_SPEED_DIST_PGN)
          {
            mui32_lastUpdateTimeSpeedReal = pkg.time();
            setSpeedReal(i32_tempSpeed);
            setDistReal( ui32_tempDist );
            mt_directionReal = IsoAgLib::IsoDirectionFlag_t(pkg.getUint8Data(7) & 0x3 );

            //decide if ground based speed is actually the best available speed
            if ( ( b_usableSpeed ) &&
                 ( ( mt_speedSource <= IsoAgLib::GroundBasedSpeed )
                || ( (testTimeOutdatedSpeed >= getTimeOut()) && (testTimeOutdatedSpeed < (getTimeOut()+1000u)) )
                 )
               )
            { // speed information is usable and the current selected speed is at least not better or outdated
              updateSpeed(IsoAgLib::GroundBasedSpeed, pkg.time() );
            }

            //if ground based dist and direction is actually the best available
            if ( ( mui32_distReal <= 0xFAFFFFFF ) &&
                 ( ( mt_distDirecSource <= IsoAgLib::GroundBasedDistDirec )
                || ( testTimeOutdatedDist >= getTimeOut() && testTimeOutdatedDist < (getTimeOut()+1000u) )
                 )
               )
            { // distance information is usable and the current selected distance is at least not better or outdated
              updateDistanceDirection(IsoAgLib::GroundBasedDistDirec);
            }

          }
          else
          {
            mui32_lastUpdateTimeSpeedTheor = pkg.time();
            setSpeedTheor(i32_tempSpeed);
            setDistTheor( ui32_tempDist );
            #if defined(USE_BASE) || defined(USE_TRACTOR_GENERAL)
            // additionally scan for key switch and maximum power time
            c_tracgeneral.setKeySwitch(IsoAgLib::IsoActiveFlag_t( ( pkg.getUint8Data( 7 ) >> 2 ) & 0x3 ));
            c_tracgeneral.setMaxPowerTime(pkg.getUint8Data( 6 ) );
            #endif
            mt_operatorDirectionReversed = IsoAgLib::IsoOperatorDirectionFlag_t( ( pkg.getUint8Data(7) >> 6) & 0x3);
            mt_startStopState = IsoAgLib::IsoActiveFlag_t( ( pkg.getUint8Data(7) >> 4) & 0x3);
            mt_directionTheor = IsoAgLib::IsoDirectionFlag_t(pkg.getUint8Data(7)       & 0x3 );
            if ( ( b_usableSpeed ) &&
                 ( ( mt_speedSource <= IsoAgLib::WheelBasedSpeed )
                || ( testTimeOutdatedSpeed >= getTimeOut() && testTimeOutdatedSpeed < (getTimeOut()+1000u) )
                 )
               )
            { // speed information is usable and the current selected speed is at least not better or outdated
              updateSpeed(IsoAgLib::WheelBasedSpeed, pkg.time() );
            }
            if ( ( mui32_distTheor <= 0xFAFFFFFF ) &&
                 ( ( mt_distDirecSource <= IsoAgLib::WheelBasedDistDirec )
                || ( testTimeOutdatedDist >= getTimeOut() && testTimeOutdatedDist < (getTimeOut()+1000u) )
                 )
               )
            { // distance information is usable and the current selected distance is at least not better or outdated
              updateDistanceDirection(IsoAgLib::WheelBasedDistDirec);
            }
          }
          setUpdateTime( ci32_now );
          setSelectedDataSourceISOName( rcc_tempISOName );
        }
        else
        { // there is a sender conflict
          IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracMultipleSender, getMultitonInst() );
          return;
        }
        break;
      case SELECTED_SPEED_PGN:
        {
          // only take values, if i am not the regular sender
          // and if actual sender isn't in conflict to previous sender

          // first check whether the received message indicates a valid speed source - and ignore the message totally,
          // if speedSource == IsoNotAvailableSpeed
          const IsoAgLib::IsoSpeedSourceFlag_t t_testSpeedSource = IsoAgLib::IsoSpeedSourceFlag_t( ( (pkg.getUint8Data(7) >> 2) & 0x7) );

          if( t_testSpeedSource == IsoAgLib::IsoNotAvailableSpeed )
          {
            // Not a valid speed source.  Ignore the message totally even if from a valid Source Address
          }
          else
          {
            if ( checkParseReceived( rcc_tempISOName ) )
            {
                mt_selectedSpeedLimitStatus = IsoAgLib::IsoLimitFlag_t( ( (pkg.getUint8Data(7) >> 5) & 0x7) );
                mt_selectedDirection        = IsoAgLib::IsoDirectionFlag_t( pkg.getUint8Data(7) & 0x3);

                setSelectedDataSourceISOName(rcc_tempISOName);
                setUpdateTime( ci32_now );

                if (pkg.getUint16Data(0) <= 0xFAFF) //valid selected speed?
                {
                  mi32_selectedSpeed = pkg.getUint16Data(0);
                  mt_selectedSpeedSource =  t_testSpeedSource;
                  updateSpeed(IsoAgLib::SelectedSpeed, pkg.time());
                  if (mt_selectedDirection == IsoAgLib::IsoReverse)
                    mi32_selectedSpeed *= -1; //driving reverse
                }
                else //fall back to ground based speed
                  mt_speedSource = IsoAgLib::GroundBasedSpeed;

                if (pkg.getUint32Data(2) <= 0xFAFFFFFF) //valid selected distance?
                {
                  mui32_selectedDistance = pkg.getUint32Data(2);
                  mt_distDirecSource = IsoAgLib::SelectedDistDirec;
                  mt_selectedDirection = IsoAgLib::IsoDirectionFlag_t(   ( (pkg.getUint8Data(7) >> 0) & 0x3) );
                  updateDistanceDirection(IsoAgLib::SelectedDistDirec);
                } else //fall back to ground based direction and distance
                  mt_distDirecSource = IsoAgLib::GroundBasedDistDirec;

            } else
            { // there is a sender conflict
              IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracMultipleSender, getMultitonInst() );
              return;
            }
          }
        }
        break;
      case ELECTRONIC_ENGINE_CONTROLLER_1_PGN:
        mui16_engineSpeed = pkg.getUint16Data(3);
        break;
    }
  }

  // actually means updateSELECTEDSpeed !!!
  void TracMove_c::updateSpeed(IsoAgLib::SpeedSource_t t_speedSrc, int32_t ai_time )
  {
    #if ( (defined USE_BASE || defined USE_TIME_GPS) && defined ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
    TimePosGps_c& c_timeposgps = getTimePosGpsInstance4Comm();
    #endif
    switch(t_speedSrc)
    {
      case IsoAgLib::SelectedSpeed:
        mt_speedSource = IsoAgLib::SelectedSpeed; //nothing more to do because variables are already set
        break;
      #if ( (defined USE_BASE || defined USE_TIME_GPS) && defined ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
      case IsoAgLib::GpsBasedSpeed:
        mt_speedSource = IsoAgLib::GpsBasedSpeed;
        mt_selectedSpeedSource = IsoAgLib::IsoNavigationBasedSpeed;
        mi32_selectedSpeed = c_timeposgps.getGpsSpeedCmSec()*10; // Selected Speed is in mm/s
        break;
      #endif
      case IsoAgLib::GroundBasedSpeed:
        mt_speedSource = IsoAgLib::GroundBasedSpeed;
        mt_selectedSpeedSource = IsoAgLib::IsoGroundBasedSpeed;
        mi32_selectedSpeed = mi32_speedReal;
        break;
      case IsoAgLib::WheelBasedSpeed:
        mt_speedSource = IsoAgLib::WheelBasedSpeed;
        mt_selectedSpeedSource = IsoAgLib::IsoWheelBasedSpeed;
        mi32_selectedSpeed = mi32_speedTheor;
        break;
      default:
        mt_speedSource = IsoAgLib::NoSpeed;
        mt_selectedSpeedSource = IsoAgLib::IsoNotAvailableSpeed;
        break;
    }
    mui32_lastUpdateTimeSpeedSelected = ai_time;
  }

  void TracMove_c::updateDistanceDirection(IsoAgLib::DistanceDirectionSource_t t_distanceSrc)
  {
    //select the direction of the tractor and distance in reference to the selected speed source
    switch(t_distanceSrc)
    {
      case IsoAgLib::SelectedDistDirec:
        mt_distDirecSource = IsoAgLib::SelectedDistDirec; //nothing more to do because variables are already set
        break;
      case IsoAgLib::GroundBasedDistDirec:
        mt_distDirecSource = IsoAgLib::GroundBasedDistDirec;
        mui32_selectedDistance = mui32_distReal;
        mt_selectedDirection = mt_directionReal;
        break;
      case IsoAgLib::WheelBasedDistDirec:
        mt_distDirecSource = IsoAgLib::WheelBasedDistDirec;
        mui32_selectedDistance = mui32_distTheor;
        mt_selectedDirection = mt_directionTheor;
        break;
      default:
        mt_distDirecSource = IsoAgLib::NoDistDirec;
        break;
    }
    /// update time -> System Time caused by call of interface class
    mui32_lastUpdateTimeDistDirec = System_c::getTime();
  }

  /** Detect stop of Speed update from tractor
      @see  BaseCommon_c::timeEvent()
    */
  void TracMove_c::timeEventImplMode()
  {
    const int32_t ci32_now = System_c::getTime();
    // checking for timeout of speed update
    if ( ( (ci32_now - mui32_lastUpdateTimeSpeedSelected)  >= TIMEOUT_SPEED_LOST || getSelectedDataSourceISONameConst().isUnspecified()  )
      && ( !isSelectedSpeedMissing() ) )
    { // TECU stoppped its Speed and doesn't send speed updates - as defined by ISO 11783
      // --> switch value of selected speed to ZERO
      setSelectedSpeed( NO_VAL_32S );
    }
    if ( ( (ci32_now - mui32_lastUpdateTimeSpeedReal)  >= TIMEOUT_SPEED_LOST || getSelectedDataSourceISONameConst().isUnspecified()  )
      && ( !isRealSpeedMissing() ) )
    { // TECU stoppped its Speed and doesn't send speed updates - as defined by ISO 11783
      // --> switch value of selected speed to ZERO
      setSpeedReal( NO_VAL_32S );
    }
    if ( ( (ci32_now - mui32_lastUpdateTimeSpeedTheor)  >= TIMEOUT_SPEED_LOST || getSelectedDataSourceISONameConst().isUnspecified()  )
      && ( !isTheorSpeedMissing() ) )
    { // TECU stoppped its Speed and doesn't send speed updates - as defined by ISO 11783
      // --> switch value of selected speed to ZERO
      setSpeedTheor( NO_VAL_32S );
    }
  }

  /** send a ISO11783 moving information PGN.
    * this is only called when sending ident is configured and it has already claimed an address
      @pre  function is only called in tractor mode
      @see  BaseCommon_c::timeEvent()
    */
  void TracMove_c::timeEventTracMode( )
  { ///Timeperiod of 100ms is set in ::config
    sendMovingTracMode();
  }

  void TracMove_c::prepareSending( CanPkgExt_c& pkg )
  {
    pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
    pkg.setIsoPri(3);
    pkg.setLen(8);

    setSelectedDataSourceISOName( getIdentItem()->isoName());
  }

  TracMove_c::SendMessage_e TracMove_c::sendGroundBasedSpeedDist()
  {
    if (!canSendGroundBasedSpeedDist())
      return MessageNotSent;
    CanPkgExt_c pkg;
    prepareSending( pkg );
    pkg.setIsoPgn(GROUND_BASED_SPEED_DIST_PGN);
    pkg.setUint16Data( 0, __IsoAgLib::abs(mi32_speedReal));
    pkg.setUint32Data(2, mui32_distReal);

    uint8_t b_val8 = 0xFC; // set reserved bits to 1
    b_val8 |= mt_directionReal;
    pkg.setUint8Data(7, b_val8);
    //reserved fields
    pkg.setUint8Data(6, 0xFF);

    // CanIo_c::operator<< retreives the information with the help of CanPkg_c::getData
    // then it sends the data
    getIsoBusInstance4Comm() << pkg;
    return MessageSent;
  }

  TracMove_c::SendMessage_e TracMove_c::sendWheelBasedSpeedDist()
  {
    if (!canSendWheelBasedSpeedDist())
      return MessageNotSent;
    CanPkgExt_c pkg;
    prepareSending( pkg );
    pkg.setIsoPgn(WHEEL_BASED_SPEED_DIST_PGN);
    pkg.setUint16Data(0, __IsoAgLib::abs(mi32_speedTheor));
    pkg.setUint32Data(2, mui32_distTheor);

    uint8_t b_val8 = 0;
    //pkg.setUint8Data(7, b_val8);
#if defined(USE_BASE) || defined(USE_TRACTOR_GENERAL)
    TracGeneral_c& c_tracgeneral = getTracGeneralInstance4Comm();
    // additionally scan for key switch and maximum power time
    pkg.setUint8Data(6, c_tracgeneral.maxPowerTime() );
    b_val8 |= (c_tracgeneral.keySwitch() << 2);
#endif
    b_val8 |= (mt_operatorDirectionReversed << 6);
    b_val8 |= (mt_startStopState << 4);
    b_val8 |= mt_directionTheor;
    pkg.setUint8Data(7, b_val8);

    getIsoBusInstance4Comm() << pkg;
    return MessageSent;
  }

  TracMove_c::SendMessage_e TracMove_c::sendSelectedSpeed()
  {
    if (!canSendSelectedSpeed())
      return MessageNotSent;
    CanPkgExt_c pkg;
    prepareSending( pkg );
    pkg.setIsoPgn(SELECTED_SPEED_PGN);
    uint8_t ui8_temp = 0;

    pkg.setUint16Data(0, __IsoAgLib::abs(mi32_selectedSpeed));
    pkg.setUint32Data(2, mui32_selectedDistance);
    ui8_temp |= (mt_selectedSpeedLimitStatus << 5);
    ui8_temp |= (mt_selectedSpeedSource      << 2);
    ui8_temp |= (mt_selectedDirection        << 0);
    pkg.setUint8Data(7, ui8_temp);
    //reserved fields
    pkg.setUint8Data(6, 0xFF);

    getIsoBusInstance4Comm() << pkg;
    return MessageSent;
  }

  TracMove_c::SendMessage_e TracMove_c::sendEngineSpeed()
  {
    if (!canSendEngineSpeed())
      return MessageNotSent;

    CanPkgExt_c pkg;
    prepareSending( pkg );
    pkg.setIsoPgn(ELECTRONIC_ENGINE_CONTROLLER_1_PGN);

    // TODO unimplemented Engine Torque mode
    pkg.setUint16Data(0, 0xFF);
    // TODO Driver's demand engine - Percentage torque
    pkg.setUint8Data(1, 0xFF);
    // TODO Actual Engine - Percentage torque
    pkg.setUint8Data(2, 0xFF);
    // Engine speed
    pkg.setUint16Data(3, mui16_engineSpeed);
    // Source adresse of controlling deice for engine.
    pkg.setUint8Data(5, 0xFF);
    // Engine starter mode
    pkg.setUint8Data(6, 0xFF);
    // Engine demand
    pkg.setUint8Data(7, 0xFF);

    getIsoBusInstance4Comm() << pkg;
    return MessageSent;
  }

  /** send moving data with ground&theor speed&dist
      @pre  function is only called in tractor mode and only from timeEventTracMode
      @see  CanIo_c::operator<<
    */
  void TracMove_c::sendMovingTracMode( )
  {
    (void)sendGroundBasedSpeedDist();
    (void)sendWheelBasedSpeedDist();
    (void)sendSelectedSpeed();
    (void)sendEngineSpeed();
  }


bool TracMove_c::processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t )
{
  // check if we are allowed to send a request for pgn
  if ( ! BaseCommon_c::check4ReqForPgn(aui32_pgn, apc_isoItemSender, apc_isoItemReceiver) )
    return false;

  bool b_processed = true;
  switch (aui32_pgn) {
  default:
    b_processed = false;
    break;
  case GROUND_BASED_SPEED_DIST_PGN:
    sendGroundBasedSpeedDist();
    break;
  case WHEEL_BASED_SPEED_DIST_PGN:
    sendWheelBasedSpeedDist();
    break;
  case SELECTED_SPEED_PGN:
    sendSelectedSpeed();
    break;
  case ELECTRONIC_ENGINE_CONTROLLER_1_PGN:
    sendEngineSpeed();
    break;
  }
  return b_processed;
}

} // End Namespace __IsoAglib
