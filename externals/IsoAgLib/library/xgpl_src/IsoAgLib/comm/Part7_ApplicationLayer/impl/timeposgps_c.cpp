/*
  timeposgps_c.cpp: working on Base Data Msg Type NMEA 2000 GPS and
    Calendar; stores, updates and delivers all base data informations
    from CanCustomer_c derived for CAN sending and receiving
    interaction; from BaseCommon_c derived for interaction with other
    IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "timeposgps_c.h"

#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multireceive_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/stream_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/sendstream_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisendpkg_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgn_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#include <IsoAgLib/comm/Part7_ApplicationLayer/itracmove_c.h>
#include <IsoAgLib/util/iutil_funcs.h>
#include <IsoAgLib/util/iliberr_c.h>
#include <IsoAgLib/util/impl/util_funcs.h>

#ifdef WINCE
#include <time_CE.h>
#endif

#if DEBUG_NMEA
  #ifdef SYSTEM_PC
    #include <iostream>
  #else
    #include <supplementary_driver/driver/rs232/irs232io_c.h>
  #endif
#endif

#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#pragma warning( disable : 4355 )
#endif

const float gcf_rapidUpdateFilter = 0.15f;  // 15% new, 85%old to filter the update time.


#ifdef ENABLE_NMEA_2000_MULTI_PACKET
// Off-class/namespace c-style helper functions
void getDegree10Minus7FromStream( __IsoAgLib::Stream_c& rc_stream, int32_t& ri32_result )
{
  #if HAL_SIZEOF_INT == 4
  // use 64 bit variable
  int64_t i64_temp;
  IsoAgLib::convertIstream( rc_stream, i64_temp );
  double d_temp = double(i64_temp);
  ri32_result = int32_t( d_temp * 1.0e-9 );
  #else
  // only take higher 4 bytes
  int32_t i32_temp;

  // ignore the result of the following call
  IsoAgLib::convertIstream( rc_stream, i32_temp );
  // only take this part
  IsoAgLib::convertIstream( rc_stream, i32_temp );
  // NMEA sends with 1.0e-16, while normally 1.0e-7 is enough -> mult with 1.0e-9
  double d_temp = ( double(i32_temp) * 4294967296.0 * 1.0e-9 );
  ri32_result = int32_t( d_temp );
  #endif
}

void getAltitude10Minus2FromStream( __IsoAgLib::Stream_c& rc_stream, int32_t& ri32_result )
{
  #if HAL_SIZEOF_INT == 4
  // use 64 bit variable
  int64_t i64_temp;
  IsoAgLib::convertIstream( rc_stream, i64_temp );
  double d_temp = double(i64_temp);
  // NMEA sends with 1.0e-6, while normally 1.0e-2 is enough -> mult with 1.0e-4
  ri32_result = int32_t( d_temp * 1.0e-4 );
  #else
  // only take higher 4 bytes
  int32_t i32_temp;

  // ignore the result of the following call
  IsoAgLib::convertIstream( rc_stream, i32_temp );
  // only take this part
  IsoAgLib::convertIstream( rc_stream, i32_temp );
  // NMEA sends with 1.0e-6, while normally 1.0e-2 is enough -> mult with 1.0e-4
  double d_temp = ( double(i32_temp) * 4294967296.0 * 1.0e-4 );
  ri32_result = int32_t( d_temp );
  #endif
}
#endif // END of ENABLE_NMEA_2000_MULTI_PACKET

namespace __IsoAgLib {

  #if defined(ENABLE_NMEA_2000_MULTI_PACKET)
  /** place next data to send direct into send buffer of pointed
    stream send package - MultiSend_c will send this
    buffer afterwards
  */
  void Nmea2000SendStreamer_c::setDataNextStreamPart (MultiSendPkg_c* mspData, uint8_t bytes)
  {
    mspData->setDataPart (vec_data, ui16_currentSendPosition, bytes);
    ui16_currentSendPosition += bytes;
  }

#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  /** place next data to send direct into send buffer of pointed
      stream send package - MultiSend_c will send this
      buffer afterwards
  */
  void Nmea2000SendStreamer_c::setDataNextFastPacketStreamPart (MultiSendPkg_c* mspData, uint8_t bytes, uint8_t aui8_offset )
  {
    mspData->setFastPacketDataPart (vec_data, ui16_currentSendPosition, bytes, aui8_offset );
    ui16_currentSendPosition += bytes;
  }
#endif

  /** set cache for data source to stream start */
  void Nmea2000SendStreamer_c::resetDataNextStreamPart()
  {
    ui16_currentSendPosition = 0;
  }

  /** save current send position in data source - neeed for resend on send problem */
  void Nmea2000SendStreamer_c::saveDataNextStreamPart ()
  {
    ui16_storedSendPosition = ui16_currentSendPosition;
  }

  /** reactivate previously stored data source position - used for resend on problem */
  void Nmea2000SendStreamer_c::restoreDataNextStreamPart ()
  {
    ui16_currentSendPosition = ui16_storedSendPosition;
  }

  /** calculate the size of the data source */
  uint32_t Nmea2000SendStreamer_c::getStreamSize ()
  {
    return vec_data.size();
  }

  /** get the first byte, which is the COMMAND-byte and should be given back CONST! */
  uint8_t Nmea2000SendStreamer_c::getFirstByte ()
  {
    return *(vec_data.begin());
  }
  #endif // END of ENABLE_NMEA_2000_MULTI_PACKET


  /** HIDDEN constructor for a TimePosGps_c object instance which can optional
      set the configuration for send/receive for base msg type NMEA 2000 GPS and calendar
      NEVER instantiate a variable of type TimePosGps_c within application
      only access TimePosGps_c via getTimePosGpsInstance()
      or getTimePosGpsInstance (protocolInstanceNr) in case more than one ISO11783 BUS is used for IsoAgLib
   */
  TimePosGps_c::TimePosGps_c()
  : mt_multiSendEventHandler(*this)
  , mf_rapidUpdateRateFilter(0.0f)
  , mi32_rapidUpdateRateMs(0)
  , mi32_altitudeCm(0x7FFFFFFF)
  , mc_sendGpsISOName()
  , mpc_identGps(NULL)
  , mt_identModeGps( IsoAgLib::IdentModeImplement )
  , mvec_msgEventHandlers()
  {}


  /** functions with actions, which must be performed periodically
      -> called periodically by Scheduler_c
      ==> sends base msg if configured in the needed rates
      possible errors:
          * dependant error in CanIo_c on CAN send problems
      @see CanPkg_c::getData
      @see CanPkgExt_c::getData
      @see CanIo_c::operator<<
      @return true -> all planned activities performed in allowed time
    */
  void TimePosGps_c::timeEvent()
  {
    checkCreateReceiveFilter();

    const int32_t ci32_now = System_c::getTime();

    // check for different base data types whether the previously
    if ( ( checkMode(IsoAgLib::IdentModeImplement)      )
      && ( getSelectedDataSourceISOName().isSpecified() )
      && ( (lastedTimeSinceUpdate() >= getTimeOut( )) || (yearUtc() == 0) ) // yearUtc means ERROR and NOT year0/1900/1970/whatever...
       )
    { // the previously sending node didn't send the information for 3 seconds -> give other items a chance
      getSelectedDataSourceISOName().setUnspecified();

      /// Set date to NO_VAL, i.e. 01.01.0000
      bit_calendar.year = 0;
      bit_calendar.month = 1;
      bit_calendar.day = 1;
      /// There's no NO_VAL for time yet, so set it to 00:00:00
      bit_calendar.hour = 0;
      bit_calendar.minute = 0;
      bit_calendar.second = 0;
      /** @todo SOON-259 maybe later also reset those values here, too... */
      //bit_calendar.msec = 0;
      //bit_calendar.timezoneMinuteOffset = 0;
      //bit_calendar.timezoneHourOffsetMinus24 = 24;
    }

    if ( ( checkModeGps(IsoAgLib::IdentModeImplement) )
      && ( mc_sendGpsISOName.isSpecified() ) )
    { // we are receiving GPS data -> check whether the old sender is still active
      // and we were already receiving sometimes in past GPS data
      bool b_noPosition = false;
      if (
          ( (ci32_now - mi32_lastIsoPositionSimple) >= getTimeOut( ) )
          #ifdef ENABLE_NMEA_2000_MULTI_PACKET
          &&
          ( (ci32_now - mi32_lastIsoPositionStream) >= getTimeOut( ) )
          #endif
        )
      { // the previously sending node didn't send POSITION information for 3 seconds -> give other items a chance
        mi32_latitudeDegree10Minus7 = mi32_longitudeDegree10Minus7 = 0x7FFFFFFF;
        mt_gnssMethod = IsoAgLib::IsoNoGps;
        #ifdef ENABLE_NMEA_2000_MULTI_PACKET
        mt_gnssType = IsoAgLib::IsoGnssGps;
        mui8_satelliteCnt = 0;
        mi32_altitudeCm = 0;
        #endif
        b_noPosition = true;
        mf_rapidUpdateRateFilter = 0.0f;
        mi32_rapidUpdateRateMs = 0;
      }
      if ( (ci32_now - mi32_lastIsoDirection) >= getTimeOut( ) )
      { // the previously sending node didn't send the information for 3 seconds -> give other items a chance
        mui16_speedOverGroundCmSec = mui16_courseOverGroundRad10Minus4 = 0xFFFF;

        if (b_noPosition)
        { // neither Pos nor Dir are specified, so kick the sender!
          mc_sendGpsISOName.setUnspecified();
        }
        /** @todo ON REQUEST-259: Maybe make it 2 GpsIsoNames: One for Position and one for Direction!
        * Then we don't have to wait for both to be silent in order to kick the mc_sendGpsISOName.
        * Naming:       Gps for Position
        *         Direction for Direction */
      }
      if ( mf_rapidUpdateRateFilter != 0.0f )
      {
        mi32_rapidUpdateRateMs = ((int32_t(mf_rapidUpdateRateFilter) + 50) / 100) * 100;   // round to the nearest 100ms
      }
    }


    if ( ( getIdentItem() && getIdentItem()->isClaimedAddress() && checkMode(IsoAgLib::IdentModeTractor ) )
      || ( mpc_identGps && mpc_identGps->isClaimedAddress() && checkModeGps( IsoAgLib::IdentModeTractor ) ) )
    { // there is at least something configured for send where the time sending or GPS sending is activated
      timeEventTracMode();
    }
  }

  void TimePosGps_c::checkCreateReceiveFilter( )
  {
    if ( !checkFilterCreated() )
    {
      setFilterCreated();

      IsoBus_c &c_can = getIsoBusInstance4Comm();
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (TIME_DATE_PGN<<8) ), 8 );
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (NMEA_GPS_POSITION_RAPID_UPDATE_PGN<<8) ), 8 );
#ifdef USE_J1939_VEHICLE_PGNS
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (VEHICLE_POSITION_PGN<<8) ), 8 );
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (VEHICLE_DIRECTION_SPEED_PGN<<8) ), 8 );
#endif
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (NMEA_GPS_COG_SOG_RAPID_UPDATE_PGN<<8) ), 8 );

    }
  }


  void
  TimePosGps_c::init_specialized()
  {
    mpc_identGps = NULL;
    // set the GPS mode always to non-sending
    configGps( NULL, IsoAgLib::IdentModeImplement );

    // 01.01.1970 00:00:00
    struct CNAMESPACE::tm t_testTime = {0, 0, 0, 1, 0, 70, 0 , 0 ,-1
                            #if defined(__USE_BSD) || defined(__GNU_LIBRARY__) || defined(__GLIBC__) || defined(__GLIBC_MINOR__)
                            , 0, NULL
                            #endif
                          };

    mt_tzOffset = - MACRO_ISOAGLIB_MKTIME(&t_testTime);
    if (1 == mt_tzOffset)
    { // mktime returned -1 => error
      mt_tzOffset = 0;
    }
  }

  bool TimePosGps_c::config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask )
  {
    //store old mode to decide to register or unregister to request for pgn
    IsoAgLib::IdentMode_t t_oldMode = getMode();

    //call config for handling which is base data independent
    //if something went wrong leave function before something is configured
    if ( ! BaseCommon_c::config_base ( apc_ident, at_identMode, aui16_suppressMask ) ) return false;


    if ( checkMode( IsoAgLib::IdentModeTractor ) || checkModeGps( IsoAgLib::IdentModeTractor ) )
    { // we are in sending state for at least one type
      ///setTimePeriod for Scheduler_c 100ms is minimal periode in GPSmodule up to now
      mt_task.setPeriod( 100, false );
    }
    else
    { // we are only in receiving mode for all types
      mt_task.setPeriod( 1000, false );
    }


    if (t_oldMode == IsoAgLib::IdentModeImplement && at_identMode == IsoAgLib::IdentModeTractor)
    {  // a change from Implement mode to Tractor mode occured
      getRegisterPgn()(TIME_DATE_PGN); // request for time and date
    }

    if (t_oldMode == IsoAgLib::IdentModeTractor && at_identMode == IsoAgLib::IdentModeImplement)
    {  // a change from Tractor mode to Implement mode occured
      // unregister from request for pgn, because in implement mode no request should be answered
      getUnregisterPgn()(TIME_DATE_PGN);
    }

    /// Set date to NO_VAL, i.e. 01.01.0000
    bit_calendar.year = 0;
    bit_calendar.month = 1;
    bit_calendar.day = 1;
    /// There's no NO_VAL for time yet, so set it to 00:00:00
    bit_calendar.hour = 0;
    bit_calendar.minute = 0;
    bit_calendar.second = 0;
    // additionally set the other field, too...
    bit_calendar.msec = 0;
    bit_calendar.timezoneMinuteOffset = 0;
    bit_calendar.timezoneHourOffsetMinus24 = 24;

    mi32_lastCalendarSet = 0;

    mt_cachedLocalSeconds1970AtLastSet = 0;

    return true;
  };

  /** force a request for pgn for time/date information */
  bool TimePosGps_c::sendRequestUpdateTimeDate()
  {
    if ( checkMode(IsoAgLib::IdentModeImplement) )
      return BaseCommon_c::sendPgnRequest(TIME_DATE_PGN);
    else
      return false;
  }


  bool TimePosGps_c::configGps( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identModeGps)
  {
    isoaglib_assert( ! ( ( at_identModeGps == IsoAgLib::IdentModeTractor ) && ( NULL == apc_ident ) ) );

    //set configure values
    mi32_lastIsoPositionSimple = 0;
    mi32_lastIsoDirection = 0;
#ifdef ENABLE_NMEA_2000_MULTI_PACKET
    mi32_lastIsoPositionStream = 0;
    mt_multiSendSuccessState = SendStream_c::SendSuccess;
    mi32_altitudeCm = 0x7FFFFFFF;

    mui8_satelliteCnt = 0;
#endif // END of ENABLE_NMEA_2000_MULTI_PACKET

    mpc_identGps = apc_ident;
    mt_identModeGps = at_identModeGps;

    mi32_latitudeDegree10Minus7 = mi32_longitudeDegree10Minus7 = 0x7FFFFFFF;
    mui16_speedOverGroundCmSec = mui16_courseOverGroundRad10Minus4 = 0xFFFF;

    if ( at_identModeGps == IsoAgLib::IdentModeTractor )
    { // GPS send from now on
      // because wer are in tractor mode the apc_isoName cannot be NULL
      mc_sendGpsISOName = apc_ident->isoName();
      #ifdef ENABLE_NMEA_2000_MULTI_PACKET
      // also remove any previously registered MultiReceive connections
      getMultiReceiveInstance4Comm().deregisterClient( *this );
      mc_nmea2000Streamer.reset();
      mc_nmea2000Streamer.vec_data.resize(0);
      #endif // END of ENABLE_NMEA_2000_MULTI_PACKET
    }
    else
    { // IdentModeImplement
      mc_sendGpsISOName.setUnspecified();
      // register Broadcast-TP/FP receive of NMEA 2000 data
      // make sure that the needed multi receive are registered
      #ifdef ENABLE_NMEA_2000_MULTI_PACKET
      getMultiReceiveInstance4Comm().registerClientIso (*this, __IsoAgLib::IsoName_c::IsoNameUnspecified(), NMEA_GPS_POSITION_DATA_PGN,  0x3FFFFLU, true, false);
      getMultiReceiveInstance4Comm().registerClientIso (*this, __IsoAgLib::IsoName_c::IsoNameUnspecified(), NMEA_GPS_DIRECTION_DATA_PGN, 0x3FFFFLU, true, false);

      #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
      getMultiReceiveInstance4Comm().registerClientNmea (*this, __IsoAgLib::IsoName_c::IsoNameUnspecified(), NMEA_GPS_POSITION_DATA_PGN,  0x3FFFFLU, true, false);
      getMultiReceiveInstance4Comm().registerClientNmea (*this, __IsoAgLib::IsoName_c::IsoNameUnspecified(), NMEA_GPS_DIRECTION_DATA_PGN, 0x3FFFFLU, true, false);
      #endif
      mc_nmea2000Streamer.vec_data.reserve(51); // GNSS Position Data with TWO reference stations
      #endif // END of ENABLE_NMEA_2000_MULTI_PACKET
    }

    if ( checkMode( IsoAgLib::IdentModeTractor ) || checkModeGps( IsoAgLib::IdentModeTractor ) )
    { // we are in sending state for at least one type
      ///setTimePeriod for Scheduler_c 100ms is minimal periode in GPSmodul up to now
      mt_task.setPeriod( 100, false );
    }
    else
    { // we are only in receiving mode for all types
      mt_task.setPeriod( 1000, false );
    }

    return true;
  }

  /** Retrieve the last update time of the specified information type
    */
  int32_t TimePosGps_c::lastedTimeSinceUpdateGps() const
  {
    const int32_t ci32_now = System_c::getTime();
    #ifdef ENABLE_NMEA_2000_MULTI_PACKET
    if ( mi32_lastIsoPositionStream > mi32_lastIsoPositionSimple ) return ( ci32_now - mi32_lastIsoPositionStream);
    else return ( ci32_now - mi32_lastIsoPositionSimple);
    #else
    return ( ci32_now - mi32_lastIsoPositionSimple);
    #endif
  }
  /** Retrieve the time of last update */
  int32_t TimePosGps_c::lastUpdateTimeGps() const
  {
    #ifdef ENABLE_NMEA_2000_MULTI_PACKET
    if ( mi32_lastIsoPositionStream > mi32_lastIsoPositionSimple ) return mi32_lastIsoPositionStream;
    else return mi32_lastIsoPositionSimple;
    #else
    return mi32_lastIsoPositionSimple;
    #endif
  }

  /** Retrieve the last update time of the specified information type
    */
  int32_t TimePosGps_c::lastedTimeSinceUpdateDirection() const
  {
    const int32_t ci32_now = System_c::getTime();
    return ( ci32_now - mi32_lastIsoDirection);
  }
  /** Retrieve the time of last update */
  int32_t TimePosGps_c::lastUpdateTimeDirection() const
  {
    return mi32_lastIsoDirection;
  }

  /** C-style function, to get access to the unique Base_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  TimePosGps_c &getTimePosGpsInstance( uint8_t aui8_instance )
  {
    MACRO_MULTITON_GET_INSTANCE_BODY(TimePosGps_c, PRT_INSTANCE_CNT, aui8_instance);
  }

  /** check if a received GPS message should be parsed */
  bool TimePosGps_c::checkParseReceivedGps(const IsoName_c& acrc_currentSender) const
  {
    return ( checkModeGps(IsoAgLib::IdentModeImplement) // I'm not the sender
             && ( // one of the following conditions must be true
                     (mc_sendGpsISOName == acrc_currentSender) // actual sender equivalent to last
                  || (mc_sendGpsISOName.isUnspecified()      ) // last sender has not correctly claimed address member
                )
           );
  }


  void TimePosGps_c::processMsg( const CanPkg_c& arc_data )
  {
    CanPkgExt_c pkg( arc_data, getMultitonInst() );
    if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
      return;

    IsoName_c const& rcc_tempISOName = pkg.getISONameForSA();

    const int32_t ci32_now = pkg.time();

    switch (pkg.isoPgn() /*& 0x3FFFF*/) // don't need to &, we're interested in the whole PGN.
    {
      case TIME_DATE_PGN:
        // time - date
        // only take values, if i am not the regular sender
        // and if actual sender isn't in conflict to previous sender
        if ( checkParseReceived( rcc_tempISOName ) )
        { // sender is allowed to send
          // store new calendar setting
          if ( mc_sendGpsISOName.isUnspecified()  )
          { // neither this item nor another item is sending GPS data -> this is the best time source
            setCalendarUtc(
            (pkg.getUint8Data(5) + 1985), pkg.getUint8Data(3), ((pkg.getUint8Data(4)+3) / 4), (pkg.getUint8Data(2)),
            (pkg.getUint8Data(1)), (pkg.getUint8Data(0) / 4));
          }
          else
          { // only fetch the date, as this information might not be defined by GPS
            setDateUtc((pkg.getUint8Data(5) + 1985), pkg.getUint8Data(3), ((pkg.getUint8Data(4)+3) / 4));
          }
          setTimeOut( TIMEOUT_SENDING_NODE_NMEA );
          // take local timezone offset in all cases
          bit_calendar.timezoneMinuteOffset = pkg.getUint8Data(6);
          bit_calendar.timezoneHourOffsetMinus24 = pkg.getUint8Data(7);
          // set last time
          setUpdateTime(ci32_now);
          setSelectedDataSourceISOName (rcc_tempISOName);
        }
        else
        { // there is a sender conflict
          IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracMultipleSender, getMultitonInst() );
        }
        return;

      case NMEA_GPS_POSITION_RAPID_UPDATE_PGN:
#ifdef USE_J1939_VEHICLE_PGNS
      case VEHICLE_POSITION_PGN:
#endif
        if ( checkParseReceivedGps( rcc_tempISOName ) )
        { // sender is allowed to send
#ifdef USE_J1939_VEHICLE_PGNS
          if (pkg.isoPgn() == VEHICLE_POSITION_PGN)
          {
            mi32_latitudeDegree10Minus7  = pkg.getUint32Data( 0 ) - 2100000000; // 210 / 0.0000001
            mi32_longitudeDegree10Minus7 = pkg.getUint32Data( 4 ) - 2100000000; // 210 / 0.0000001

            setTimeOut( TIMEOUT_SENDING_NODE_J1939 );
          }
          else
#endif
          {
            mi32_latitudeDegree10Minus7  = pkg.getInt32Data( 0 );
            mi32_longitudeDegree10Minus7 = pkg.getInt32Data( 4 );

            setTimeOut( TIMEOUT_SENDING_NODE_NMEA );
          }
          mi32_lastIsoPositionSimple = ci32_now;
          mc_sendGpsISOName = rcc_tempISOName;

          if (pkg.isoPgn() == NMEA_GPS_POSITION_RAPID_UPDATE_PGN)
          {
            // give an offset to the millisecond time of day based on a filtered message rate
            const int32_t ci32_updateDelta = ci32_now - mi32_lastMillisecondUpdate;
            if ( (mi32_lastMillisecondUpdate != 0) && (ci32_updateDelta < 1000) )
            { // (don't process the first time or following lost communications)
              if ( mf_rapidUpdateRateFilter == 0.0f )
              { // preload the filter value with the current period
                mf_rapidUpdateRateFilter = float(ci32_updateDelta);
                mi32_rapidUpdateRateMs = ((ci32_updateDelta + 50) / 100) * 100;   // round to the nearest 100ms
              }
              else
              {
                mf_rapidUpdateRateFilter = (mf_rapidUpdateRateFilter * (1.0f-gcf_rapidUpdateFilter)) +
                                          ((ci32_now - mi32_lastMillisecondUpdate) * gcf_rapidUpdateFilter);
              }
            }
            else
            { // reset the filter for the rate
              mf_rapidUpdateRateFilter = 0.0f;
            }
          }
          // else: nothing to do for VEHICLE_POSITION
/** @todo ON REQUEST-259: Should we allow NMEA_GPS_POSITION_RAPID_UPDATE_PGN at all without ENABLE_NMEA_2000_MULTI_PACKET?? See below! */
#ifdef ENABLE_NMEA_2000_MULTI_PACKET
          // if it has been at least 1/2 an update time since the GNSS message, add the time
          // otherwise use the same time that came from the GNSS message
          if ( (ci32_now - mi32_lastIsoPositionStream) > (mi32_rapidUpdateRateMs / 2) )
          {
            mui32_timeMillisecondOfDay += mi32_rapidUpdateRateMs;
            mi32_lastMillisecondUpdate = ci32_now;
          }
#endif
          if (getGnssMode() == IsoAgLib::IsoNoGps)
          { /// @todo ON REQUEST-259: Allow Rapid Update without Complete Position TP/FP before? Is is just an update or can it be standalone?
              /// for now, allow it as standalone and set GpsMethod simply to IsoGnssNull as we don't have reception info...
            mt_gnssMethod = IsoAgLib::IsoGnssNull; // was IsoGnssFix before. Actually, noone knows what to set here ;-)
            #ifdef ENABLE_NMEA_2000_MULTI_PACKET
            mt_gnssType = IsoAgLib::IsoGnssGps;
            #endif
          }

          notifyOnEvent (pkg.isoPgn());
        }
        else
        { // there is a sender conflict
          IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracMultipleSender, getMultitonInst() );
        }
        return;

#ifdef USE_J1939_VEHICLE_PGNS
      case VEHICLE_DIRECTION_SPEED_PGN:
        if ( checkParseReceivedGps( rcc_tempISOName ) )
        { // sender is allowed to send
          // Here we get degrees as fraction of 128, and have to change to rad 10^-4
          // -> / 128 * MATH_PI / 180 * 10000
#define MATH_PI 3.14159265
          mui16_courseOverGroundRad10Minus4 = static_cast<uint16_t>( static_cast<double>(pkg.getUint16Data( 0 ) ) * MATH_PI * 125.0 / 288.0 );
          // [256 one bit is 1/256 km/h] [* 1000 * 100 / 60 / 60 -> we get km/h but want cm/sec]
          mui16_speedOverGroundCmSec        = static_cast<uint16_t>( static_cast<double>( pkg.getUint16Data( 2 ) * 125 ) / static_cast<double>( 128 * 9 ) );
          // we are getting speed from here as well:
          mi32_altitudeCm = static_cast<int32_t>( ( static_cast<double>( pkg.getUint16Data( 6 ) ) * 0.125 - 2500.0 ) * 100.0 );
          // always update values to know if the information is there or not!

          // set last time (also always, because if the sender's sending it's sending so we can't send!!
          mi32_lastIsoDirection = ci32_now;
          mc_sendGpsISOName = rcc_tempISOName;

          setTimeOut(TIMEOUT_SENDING_NODE_J1939);

          /// @todo ON REQUEST-259: check for the REAL max, 62855 is a little bigger than 62831 or alike that could be calculated. but anyway...
          if ( (mui16_courseOverGroundRad10Minus4 <= (62855))
            && (mui16_speedOverGroundCmSec        <= (65532))
             )
          {
#if defined (USE_TRACTOR_MOVE) || defined (USE_BASE)
            getTracMoveInstance4Comm().updateSpeed(IsoAgLib::GpsBasedSpeed, pkg.time() );
#endif
            notifyOnEvent (VEHICLE_DIRECTION_SPEED_PGN);
          }
          // else: Regard this as NO (valid) COG/SOG, so it's just like nothing meaningful got received!
        }
        else
        { // there is a sender conflict
          IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracMultipleSender, getMultitonInst() );
        }
        return;
#endif

      case NMEA_GPS_COG_SOG_RAPID_UPDATE_PGN:
        if ( checkParseReceivedGps( rcc_tempISOName ) )
        { // sender is allowed to send
          mui8_directionSequenceID          = pkg.getUint8Data ( 0 );
          mui8_courseOverGroundReference    = pkg.getUint8Data ( 1 ) & 0x03;
          mui16_courseOverGroundRad10Minus4 = pkg.getUint16Data( 2 );
          mui16_speedOverGroundCmSec        = pkg.getUint16Data( 4 );
          // always update values to know if the information is there or not!

          setTimeOut(TIMEOUT_SENDING_NODE_NMEA);

          // set last time (also always, because if the sender's sending it's sending so we can't send!!
          mi32_lastIsoDirection = ci32_now;
          mc_sendGpsISOName = rcc_tempISOName;

          /// @todo ON REQUEST-259: check for the REAL max, 62855 is a little bigger than 62831 or alike that could be calculated. but anyway...
          if ( (mui16_courseOverGroundRad10Minus4 <= (62855))
            && (mui16_speedOverGroundCmSec        <= (65532))
             )
          {
#if defined (USE_TRACTOR_MOVE) || defined (USE_BASE)
            getTracMoveInstance4Comm().updateSpeed(IsoAgLib::GpsBasedSpeed, pkg.time() );
#endif
            notifyOnEvent (NMEA_GPS_COG_SOG_RAPID_UPDATE_PGN);
          }
          // else: Regard this as NO (valid) COG/SOG, so it's just like nothing meaningful got received!
        }
        else
        { // there is a sender conflict
          IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracMultipleSender, getMultitonInst() );
        }
        return;
    }
  }

  bool TimePosGps_c::processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t )
  {
    // check if we are allowed to send a request for pgn
    if ( ! BaseCommon_c::check4ReqForPgn(aui32_pgn, apc_isoItemSender, apc_isoItemReceiver) ) return false;

    // call TimePosGps_c function to send time/date
    // isoSendCalendar checks if this item (identified by ISOName)
    // is configured to send time/date
    sendCalendar();
    return true;
  };

  /** check if an NMEA2000 position signal was received - it does NOT indicate that this is an up2date signal */
  /** @todo ON REQUEST-259: improve with isPositionStreamReceived(), so we know that e.g. Altitude is there, too... */
  bool TimePosGps_c::isPositionReceived() const
  {
    if ( (mi32_latitudeDegree10Minus7  >= ( -90*10000000)) && (mi32_latitudeDegree10Minus7  <= ( 90*10000000))
      && (mi32_longitudeDegree10Minus7 >= (-180*10000000)) && (mi32_longitudeDegree10Minus7 <= (180*10000000))
      && (mt_gnssMethod != IsoAgLib::IsoNoGps)
      ) /// @todo ON REQUEST-259: improve the checking on what's valid!
    { // yep, valid GPS information
      return true;
    }
    else
    { // there is no valid GPS information
      return false;
    }
  }

  /** check if an NMEA2000 direction signal was received - it does NOT indicate that this is an up2date signal */
  bool TimePosGps_c::isDirectionReceived() const
  {
    /// @todo ON REQUEST-259: check for the REAL max, 62855 is a little bigger than 62831 or alike that could be calculated. but anyway...
    if ( (mui16_courseOverGroundRad10Minus4 <= 62855)
      && (mui16_speedOverGroundCmSec        <= 65532)
       )
    { // yep, valid GPS-direction information
      return true;
    }
    else
    { // there is no valid GPS-direction information
      return false;
    }
  }


  #ifdef ENABLE_NMEA_2000_MULTI_PACKET
  /** set the GPS time in UTC timezone.
  *  When no remote system is sending the 11783-7 PGN with date & time, the new UTC time is also set with
  *  setTimeUtc().
  *  In case another system is sending TIME_DATE_PGN, this time could be out-of-sync with GPS time.
  *  To avoid a jumping back and forth Non-GPS UTC time, those two UTC time sources are then not to be synced.
  */
  void TimePosGps_c::setTimeUtcGps(uint8_t ab_hour, uint8_t ab_minute, uint8_t ab_second, uint16_t aui16_msec )
  {
    bit_gpsTime.hour = ab_hour;
    bit_gpsTime.minute = ab_minute;
    bit_gpsTime.second = ab_second;
    bit_gpsTime.msec = aui16_msec;
    if ( getSelectedDataSourceISOName().isUnspecified() )
    { // no active source for the GPS independent time / date is specified -> so update the other UTC time also from GPS
      setTimeUtc( ab_hour, ab_minute, ab_second, aui16_msec );
    }
  }


  /** set the calendar hour value
  *  When no remote system is sending the 11783-7 PGN with date & time, the new UTC time is also set with
  *  setTimeUtc().
  *  In case another system is sending TIME_DATE_PGN, this time could be out-of-sync with GPS time.
  *  To avoid a jumping back and forth Non-GPS UTC time, those two UTC time sources are then not to be synced.
  *  @param ab_hour actual calendar hour value
  */
  void TimePosGps_c::setHourUtcGps(uint8_t ab_hour)
  {
    bit_gpsTime.hour = ab_hour;
    if ( getSelectedDataSourceISOName().isUnspecified() )
    { // no active source for the GPS independent time / date is specified -> so update the other UTC time also from GPS
      setHourUtc( ab_hour);
    }
  }


  /** set the calendar minute value
  *  When no remote system is sending the 11783-7 PGN with date & time, the new UTC time is also set with
  *  setTimeUtc().
  *  In case another system is sending TIME_DATE_PGN, this time could be out-of-sync with GPS time.
  *  To avoid a jumping back and forth Non-GPS UTC time, those two UTC time sources are then not to be synced.
  *  @param ab_minute actual calendar minute value
  */
  void TimePosGps_c::setMinuteUtcGps(uint8_t ab_minute)
  {
    bit_gpsTime.minute = ab_minute;
    if ( getSelectedDataSourceISOName().isUnspecified() )
    { // no active source for the GPS independent time / date is specified -> so update the other UTC time also from GPS
      setMinuteUtc( ab_minute );
    }
  }


  /** set the calendar second value
  *  When no remote system is sending the 11783-7 PGN with date & time, the new UTC time is also set with
  *  setTimeUtc().
  *  In case another system is sending TIME_DATE_PGN, this time could be out-of-sync with GPS time.
  *  To avoid a jumping back and forth Non-GPS UTC time, those two UTC time sources are then not to be synced.
  *  @param ab_second actual calendar second value
  */
  void TimePosGps_c::setSecondUtcGps(uint8_t ab_second)
  {
    bit_gpsTime.second = ab_second;
    if ( getSelectedDataSourceISOName().isUnspecified() )
    { // no active source for the GPS independent time / date is specified -> so update the other UTC time also from GPS
      setSecond( ab_second );
    }
  }


  /** set the calendar millisecond value
  *  When no remote system is sending the 11783-7 PGN with date & time, the new UTC time is also set with
  *  setTimeUtc().
  *  In case another system is sending TIME_DATE_PGN, this time could be out-of-sync with GPS time.
  *  To avoid a jumping back and forth Non-GPS UTC time, those two UTC time sources are then not to be synced.
  *  @param ab_millisecond actual calendar second value
  */
  void TimePosGps_c::setMillisecondUtcGps(uint16_t aui16_millisecond)
  {
    bit_gpsTime.msec = aui16_millisecond;
    if ( getSelectedDataSourceISOName().isUnspecified() )
    { // no active source for the GPS independent time / date is specified -> so update the other UTC time also from GPS
      setMillisecond( aui16_millisecond );
    }
  }

  /** deliver GPS receive qualitiy */
  void TimePosGps_c::setGnssMode( IsoAgLib::IsoGnssMethod_t at_newVal )
  {
    if(at_newVal<=IsoAgLib::IsoGnssMethodMAX) mt_gnssMethod = at_newVal;
/**
 * not using data-mode anymore as we changed from PGN 130577 to 129026
 *
    // set the upper 4 bits to unused as the Set/COG/Heading Ref is not used
    ui8_dataModeAndHeadingReference = 0xF0;
    switch ( at_newVal )
    {
      case IsoAgLib::IsoDgnssFix:
      case IsoAgLib::IsoGnssPrecise:
      case IsoAgLib::IsoRtkFixedInteger:
      case IsoAgLib::IsoRtkFloat:
        ui8_dataModeAndHeadingReference |= 0x1; // Differential enhanced mode
        break;
      case IsoAgLib::IsoDrEstimated:
        ui8_dataModeAndHeadingReference |= 0x2; // Estimated Mode
        break;
      case IsoAgLib::IsoGnssSimulated:
        ui8_dataModeAndHeadingReference |= 0x3; // Simulated Mode
        break;
      case IsoAgLib::IsoGnssManual:
        ui8_dataModeAndHeadingReference |= 0x4; // Manual Mode
        break;
      case IsoAgLib::IsoGnssError:
        ui8_dataModeAndHeadingReference |= 0xE; // Error
        break;
      case IsoAgLib::IsoGnssNull:
        ui8_dataModeAndHeadingReference |= 0xF; // Error
        break;
      default:
        // let the lower 4 bits at value "0" as currently initialized --> Autonomous Mode
        break;
    }
*/
  }


  bool TimePosGps_c::reactOnStreamStart (const ReceiveStreamIdentifier_c& ac_ident,
                                         uint32_t /*aui32_totalLen */)
  {
    if ( ( ( ac_ident.getPgn() == NMEA_GPS_POSITION_DATA_PGN  )
        || ( ac_ident.getPgn() == NMEA_GPS_DIRECTION_DATA_PGN ) )
        &&  checkModeGps(IsoAgLib::IdentModeImplement) )
    { // this a NMEA multi stream of interest where we don't send it
      return true;
    }
    else
    { // this is not of interest for us
      return false;
    }
  }


  bool TimePosGps_c::processPartStreamDataChunk (Stream_c& arc_stream,
                                          bool /*ab_isFirstChunk*/,
                                          bool ab_isLastChunkAndACKd)
  {
    const ReceiveStreamIdentifier_c& ac_ident = arc_stream.getIdent();

    // >>>Last Chunk<<< Processing
    if (ab_isLastChunkAndACKd)
    {  /** let reactOnLastChunk decide whether the pool should be kept in memory */
      return reactOnLastChunk(ac_ident, arc_stream);
    }
    // default - don't keep it
    return false;
  }

  void TimePosGps_c::reactOnAbort (Stream_c& /*arc_stream*/)
  { // as we don't perform on-the-fly parse, there is nothing special to do
  }

  //! Parameter:
  //! @param ac_ident:
  //! @param apc_stream:
  bool TimePosGps_c::reactOnLastChunk (const ReceiveStreamIdentifier_c& ac_ident,
                                       Stream_c& rc_stream)
  { // see if it's a pool upload, string upload or whatsoever! (First byte is already read by MultiReceive!)
    IsoName_c const& rcc_tempISOName = rc_stream.getIdent().getSaIsoName();

    // check if we want to process the information
    if (
        ( checkModeGps(IsoAgLib::IdentModeTractor) ) // I'm the sender
        ||
        ( // one of the following conditions must be true
         (mc_sendGpsISOName != rcc_tempISOName) // actual sender different to last
          &&
         (mc_sendGpsISOName.isSpecified() ) // last sender has correctly claimed address member
        )
       )
    { // DO NOT take this message, as this might be a falsly double source
      return false;
    }

    // set last time and ISOName information
    mc_sendGpsISOName = rcc_tempISOName;
    uint8_t ui8_tempValue;

    switch ( ac_ident.getPgn() )
    {
      case NMEA_GPS_POSITION_DATA_PGN: // 0x01F805LU -> 129029
      {
        setTimeOut( TIMEOUT_SENDING_NODE_NMEA );
        mi32_lastIsoPositionStream = rc_stream.getStartTime();
        // fetch sequence number from Byte1
        IsoAgLib::convertIstream( rc_stream, mui8_positionSequenceID );
        // --> continue with Byte2 ...
        uint16_t ui16_daysSince1970;
        uint32_t ui32_milliseconds;
        // read 2-bytes of Generic date as days since 1-1-1970 ( UNIX date )
        IsoAgLib::convertIstream( rc_stream, ui16_daysSince1970 );
        IsoAgLib::convertIstream( rc_stream, ui32_milliseconds );
        // NMEA NMEA_GPS_POSITON_DATA_PGN sends with 0.1 msec
        ui32_milliseconds /= 10;
        mui32_timeMillisecondOfDay = ui32_milliseconds;
        mi32_lastMillisecondUpdate = mi32_lastIsoPositionStream;

        const CNAMESPACE::time_t t_tempUnixTime = ( CNAMESPACE::time_t(ui16_daysSince1970) * CNAMESPACE::time_t(60L * 60L * 24L) ) + (ui32_milliseconds/1000);
#ifdef WINCE
        time_t_ce ceTime = (time_t_ce)t_tempUnixTime;
        tm* UtcNow = gmtime_ce(&ceTime);
#else
        CNAMESPACE::tm* UtcNow = CNAMESPACE::gmtime( &t_tempUnixTime );
#endif
        if ( UtcNow != NULL )
        {
          if ( checkMode(IsoAgLib::IdentModeTractor) || getSelectedDataSourceISOName().isUnspecified())
          { // update the normal UTC time from GPS time, as we are either sending the calendar + time PGN _or_
            // there is currently no other active sender of this PGN --> other getter functions of this application should get
            // the GPS time as notmal UTC time
            setCalendarUtc((UtcNow->tm_year+1900), UtcNow->tm_mon + 1, UtcNow->tm_mday,
                            UtcNow->tm_hour, UtcNow->tm_min, UtcNow->tm_sec, (ui32_milliseconds%1000));
          }
          // now set also the pure GPS time
          setTimeUtcGps(UtcNow->tm_hour, UtcNow->tm_min, UtcNow->tm_sec, (ui32_milliseconds%1000));

          if (getSelectedDataSourceISOName().isSpecified())
          { // set also UTC time (in this case setTimeUtcGps() doesn't update the UTC time)
            setTimeUtc(UtcNow->tm_hour, UtcNow->tm_min, UtcNow->tm_sec, (ui32_milliseconds%1000));
          }
        }
        // now read Latitude --> convert into double [degree]
        getDegree10Minus7FromStream( rc_stream, mi32_latitudeDegree10Minus7 );
        // now read Longitude --> convert into double [degree]
        getDegree10Minus7FromStream( rc_stream, mi32_longitudeDegree10Minus7 );
        // now read Altitude --> convert into double [meter]
        getAltitude10Minus2FromStream( rc_stream, mi32_altitudeCm );
        // now fetch Quality - gps-mode
        rc_stream >> ui8_tempValue;
        if ( ( ui8_tempValue >> 4  ) <= IsoAgLib::IsoGnssMethodMAX ) mt_gnssMethod = IsoAgLib::IsoGnssMethod_t(ui8_tempValue >> 4 );
        if ( ( ui8_tempValue & 0xF ) <= IsoAgLib::IsoGnssTypeMAX   ) mt_gnssType   = IsoAgLib::IsoGnssType_t(ui8_tempValue & 0xF );
        // GNSS Integrity
        rc_stream >> mui8_integrity;
        mui8_integrity &= 0x3; // mask reserved bits out
        // now fetch the number of satelites
        rc_stream >> mui8_satelliteCnt;
        // now fetch HDOP from raw uint16_t to float [1.0*1.0e-2)
        IsoAgLib::convertIstream( rc_stream, mi16_hdop );
        // now fetch PDOP from raw uint16_t to float [1.0*1.0e-2)
        IsoAgLib::convertIstream( rc_stream, mi16_pdop );
        // Geodial Separation
        IsoAgLib::convertIstream( rc_stream, mi32_geoidalSeparationCm );
        // number of reference stations
        IsoAgLib::convertIstream( rc_stream, mui8_noRefStations );
        // now read the type and age of all following reference stations
        for ( unsigned int ind = 0; ((ind < mui8_noRefStations) &&(!rc_stream.eof())); ind++ )
        { // push new items at the end or simply update the corresponding value
          if ( mvec_refStationTypeAndStation.size() < (ind+1) )
            mvec_refStationTypeAndStation.push_back(__IsoAgLib::convertIstreamUi16( rc_stream ) );
          else
            IsoAgLib::convertIstream( rc_stream, mvec_refStationTypeAndStation[ind] );
          if ( mvec_refStationDifferentialAge10Msec.size() < (ind+1) )
            mvec_refStationDifferentialAge10Msec.push_back(__IsoAgLib::convertIstreamUi16( rc_stream ) );
          else
            IsoAgLib::convertIstream( rc_stream, mvec_refStationDifferentialAge10Msec[ind] );
        }
        notifyOnEvent (NMEA_GPS_POSITION_DATA_PGN);
        #if DEBUG_NMEA
        INTERNAL_DEBUG_DEVICE << "process NMEA_GPS_POSITON_DATA_PGN Lat: " << mi32_latitudeDegree10Minus7
          << ", Lon: " << mi32_longitudeDegree10Minus7 << ", Alt: " << mi32_altitudeCm
          << ", TotalSize: " << rc_stream.getByteTotalSize() << ", resceived: " << rc_stream.getByteAlreadyReceived()
          << INTERNAL_DEBUG_DEVICE_ENDL;
        #endif
      }
      break;

      case NMEA_GPS_DIRECTION_DATA_PGN: // 0x01FE11LU - 130577 with Heading and Speed
        setTimeOut( TIMEOUT_SENDING_NODE_NMEA );
        uint8_t ui8_dummy;
        uint16_t ui16_newCOG, ui16_newSOG;
        IsoAgLib::convertIstream( rc_stream, ui8_dummy ); //ui8_dataModeAndHeadingReference ); //ui8_dataModeAndHeadingReferenceDUMMY &= 0x3F;
        IsoAgLib::convertIstream( rc_stream, mui8_directionSequenceID );
        IsoAgLib::convertIstream( rc_stream, ui16_newCOG );
        IsoAgLib::convertIstream( rc_stream, ui16_newSOG );
      //IsoAgLib::convertIstream( rc_stream, ui16_dummy ); //ui16_headingRad10Minus4 );
      //IsoAgLib::convertIstream( rc_stream, ui16_dummy ); //ui16_speedCmSec );
      //IsoAgLib::convertIstream( rc_stream, ui16_dummy ); //ui16_flowDirectionRad10Minus4 );
      //IsoAgLib::convertIstream( rc_stream, ui16_dummy ); //ui16_driftSpeedCmSec );
        /** @todo ON REQUEST-259: Do we need those valus above?? */

        /// @todo ON REQUEST-259: check for the REAL max, 62855 is a little bigger than 62831 or alike that could be calculated. but anyway...
        if ( (ui16_newCOG <= (62855))
          && (ui16_newSOG <= (65532))
           )
        {
          mui16_courseOverGroundRad10Minus4 = ui16_newCOG;
          mui16_speedOverGroundCmSec = ui16_newSOG;
          mi32_lastIsoDirection = rc_stream.getStartTime();
#if defined (USE_TRACTOR_MOVE) || defined (USE_BASE)
          getTracMoveInstance4Comm().updateSpeed(IsoAgLib::GpsBasedSpeed, mi32_lastIsoDirection );
#endif
          notifyOnEvent (NMEA_GPS_DIRECTION_DATA_PGN);
        }

        #if DEBUG_NMEA
        INTERNAL_DEBUG_DEVICE << "process NMEA_GPS_DIRECTION_DATA_PGN: CourseOverGround: " << mui16_courseOverGroundRad10Minus4
          << ", SpeedOverGround [cm/sec]: " << mui16_speedOverGroundCmSec
          << INTERNAL_DEBUG_DEVICE_ENDL;
        #endif
        break;
    }
    return false;
  }
  #endif // END ENABLE_NMEA_2000_MULTI_PACKET

  /** send a ISO11783 base information PGN.
    * this is only called when sending ident is configured and it has already claimed an address
      @pre  function is only called in tractor mode
      @see  BaseCommon_c::timeEvent()
    */
  void TimePosGps_c::timeEventTracMode()
  {
    const int32_t ci32_now = System_c::getTime();

    // time/date is only sent on request

    if ( checkModeGps(IsoAgLib::IdentModeTractor) )
    { // mpc_identGps must be != NULL, because we are in tractor mode
      if ( isPositionSimpleToSend() && ((ci32_now - mi32_lastIsoPositionSimple) >= 100) )
      {
        sendPositionRapidUpdate();
      }

      if ( isDirectionToSend() && ((ci32_now - mi32_lastIsoDirection) >= 250) )
      {
        isoSendDirection();
      }

      #ifdef ENABLE_NMEA_2000_MULTI_PACKET
      if ( ( isPositionStreamToSend() )
        && ( ( ci32_now - mi32_lastIsoPositionStream ) >= 1000 )
        && ( mt_multiSendSuccessState != SendStream_c::Running  ) )
      {
        isoSendPositionStream();
      }
      #endif // END of ENABLE_NMEA_2000_MULTI_PACKET
    }
  }

  /** send position rapid update message */
  void TimePosGps_c::sendPositionRapidUpdate( void )
  {
    CanPkgExt_c pkg;
    pkg.setIsoPri(2);
    pkg.setLen(8);
    pkg.setMonitorItemForSA( mpc_identGps->getIsoItem() );

    pkg.setIsoPgn(NMEA_GPS_POSITION_RAPID_UPDATE_PGN);
    pkg.setInt32Data(0, mi32_latitudeDegree10Minus7 );
    pkg.setInt32Data(4, mi32_longitudeDegree10Minus7);

    getIsoBusInstance4Comm() << pkg;

    // update time
    mi32_lastIsoPositionSimple = System_c::getTime();
  }

#ifdef USE_J1939_VEHICLE_PGNS
  void TimePosGps_c::sendVehiclePosDirSpd() const
  {
    CanPkgExt_c pkg;
    pkg.setIsoPri(2);
    pkg.setLen(8);
    pkg.setMonitorItemForSA( mpc_identGps->getIsoItem() );

    pkg.setIsoPgn(VEHICLE_POSITION_PGN);
    pkg.setInt32Data(0, mi32_latitudeDegree10Minus7  + 2100000000 ); // 210 / 0.0000001
    pkg.setInt32Data(4, mi32_longitudeDegree10Minus7 + 2100000000 ); // 210 / 0.0000001);

    getIsoBusInstance4Comm() << pkg;

    pkg.setIsoPgn(VEHICLE_DIRECTION_SPEED_PGN);
    pkg.setUint16Data(0, static_cast<uint16_t>( double( mui16_courseOverGroundRad10Minus4 ) * 288.0 / 125.0 / MATH_PI ) );
    pkg.setUint16Data(2, static_cast<uint16_t>( double( mui16_speedOverGroundCmSec ) * static_cast<double>( 128 * 9 ) / 125.0 ) );
    pkg.setUint16Data(4, 0xFFFF ); // dunno what to send here for N/A
    pkg.setUint16Data(6, static_cast<uint16_t>( ( ( double( mi32_altitudeCm ) / 100.0 ) + 2500.0 ) / 0.125 ) );

    getIsoBusInstance4Comm() << pkg;
  }
#endif

/* send COG and SOG as 250ms rapid update, but with single packet only */
/*
 * not using anymore as we changed from PGN 130577 to 129026
 *
  void TimePosGps_c::isoSendDirectionStream( void )
  {
    const int32_t ci32_now = getLastRetriggerTime();
    // set data in Nmea2000SendStreamer_c
    mc_nmea2000Streamer.reset();
    STL_NAMESPACE::vector<uint8_t>& writeRef = mc_nmea2000Streamer.vec_data;
    // use helper function to transfer value to the byte vector
    number2LittleEndianString( uint8_t (ui8_dataModeAndHeadingReference|0xC0), writeRef );     /// NOT there in the RAPID UPDATE one
    number2LittleEndianString( mui8_directionSequenceID, writeRef );
    number2LittleEndianString( mui16_courseOverGroundRad10Minus4, writeRef );
    number2LittleEndianString( mui16_speedOverGroundCmSec, writeRef );

    number2LittleEndianString( ui16_headingRad10Minus4, writeRef );           /// not init'ed  /// NOT there in the RAPID UPDATE one
    number2LittleEndianString( ui16_speedCmSec, writeRef );                   /// not init'ed  /// NOT there in the RAPID UPDATE one

    number2LittleEndianString( ui16_flowDirectionRad10Minus4, writeRef );     /// not init'ed   /// NOT there in the RAPID UPDATE one
    number2LittleEndianString( ui16_driftSpeedCmSec, writeRef );              /// not init'ed  /// NOT there in the RAPID UPDATE one

    //now trigger sending
    #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
    if ( getMultiSendInstance4Comm().sendIsoFastPacket(mc_sendGpsISOName, &mc_nmea2000Streamer, NMEA_GPS_DIRECTION_DATA_PGN, mt_multiSendSuccessState) )
    #else
    if ( getMultiSendInstance4Comm().sendIsoBroadcast(mc_sendGpsISOName, &mc_nmea2000Streamer, NMEA_GPS_DIRECTION_DATA_PGN, mt_multiSendSuccessState) )
    #endif
    { // update time
      mi32_lastIsoDirectionStream = ci32_now;
      mui8_directionSequenceID++;
    }
  }
*/

void TimePosGps_c::isoSendDirection( void )
{
  CanPkgExt_c pkg;
  // little pre-Setup
  mui8_directionSequenceID = 0xFF; // not using tied-together-packaging right now
  mui8_courseOverGroundReference = 0; // for now, we only send true NORTH

  pkg.setIsoPri(2);
  pkg.setLen(8);
  pkg.setMonitorItemForSA( mpc_identGps->getIsoItem() );

  pkg.setIsoPgn (NMEA_GPS_COG_SOG_RAPID_UPDATE_PGN);
  pkg.setUint8Data (0, mui8_directionSequenceID );
  pkg.setUint8Data (1, mui8_courseOverGroundReference);
  pkg.setUint16Data(2, mui16_courseOverGroundRad10Minus4 );
  pkg.setUint16Data(4, mui16_speedOverGroundCmSec );
  pkg.setUint16Data(6, 0xFFFF );

  // CanIo_c::operator<< retreives the information with the help of CanPkg_c::getData
  // then it sends the data
  getIsoBusInstance4Comm() << pkg;

  // update time
  mi32_lastIsoDirection = System_c::getTime();
}


#if defined(ENABLE_NMEA_2000_MULTI_PACKET)
  void setDegree10Minus7ToStream( const int32_t& ri32_src, STL_NAMESPACE::vector<uint8_t>& writeRef )
  {
    #if HAL_SIZEOF_INT == 4
    // use 64 bit variable
    const double d_temp = double(ri32_src) * 1.0e+9;
    int64_t i64_temp = int64_t(d_temp);
    number2LittleEndianString( i64_temp, writeRef );
    #else
    // only take higher 4 bytes ( write the lower four byte with 0xFF in case the value is negative )
    int32_t i32_temp = (ri32_src >= 0)?0:-1;
    number2LittleEndianString( i32_temp, writeRef );
    i32_temp = int32_t(double(ri32_src) * 1.0e+9 / 4294967296.0);
    number2LittleEndianString( i32_temp, writeRef );
    #endif
  }

  void setAltitude10Minus2ToStream( const int32_t& ri32_result, STL_NAMESPACE::vector<uint8_t>& writeRef )
  {
    #if HAL_SIZEOF_INT == 4
    // use 64 bit variable
    // NMEA sends with 1.0e-6, while normally 1.0e-2 is enough -> mult with 1.0e-4
    const double d_temp = double(ri32_result) * 1.0e+4;
    const int64_t i64_temp = int64_t(d_temp);
    number2LittleEndianString( i64_temp, writeRef );
    #else
    // only take higher 4 bytes
    int32_t i32_temp = 0;
    number2LittleEndianString( i32_temp, writeRef );
    // NMEA sends with 1.0e-6, while normally 1.0e-2 is enough -> mult with 1.0e-4
    i32_temp = int32_t(double(ri32_result) * 1.0e+4 / 4294967296.0);
    number2LittleEndianString( i32_temp, writeRef );
    #endif
  }

  /** send position as detailed stream */
  void TimePosGps_c::isoSendPositionStream( void )
  {
    // little pre-Setup
    mui8_positionSequenceID = 0xFF; // not using tied-together-packaging right now

    const int32_t ci32_now = System_c::getTime();
    // set data in Nmea2000SendStreamer_c
    mc_nmea2000Streamer.reset();
    STL_NAMESPACE::vector<uint8_t>& writeRef = mc_nmea2000Streamer.vec_data;
    // use helper function to transfer value to the byte vector
    number2LittleEndianString( mui8_positionSequenceID, writeRef );

    uint16_t ui16_daysSince1970 = 0; // standard value (or 0xFFFF?), if UTC date is not set (= 01.01.1900)

    const struct CNAMESPACE::tm* p_tm = currentUtcTm();

    if (p_tm)
    { // testTime is only used for calculation of ui16_daysSince1970 => use time 12:00:00 to avoid daylight setting influence
      struct CNAMESPACE::tm testTime = {0, 0, 12, p_tm->tm_mday, p_tm->tm_mon, p_tm->tm_year, 0,0,-1
                            #if defined(__USE_BSD) || defined(__GNU_LIBRARY__) || defined(__GLIBC__) || defined(__GLIBC_MINOR__)
                            , 0, NULL
                            #endif
                            };
      // compensate mktime() time zone influence:
      const CNAMESPACE::time_t secondsSince1970 = MACRO_ISOAGLIB_MKTIME( &testTime ) + mt_tzOffset;
      // calculate the days
      ui16_daysSince1970 = static_cast<uint16_t>(secondsSince1970 / ( 60L * 60L *24L ));
    }
    const uint32_t ui32_milliseconds = ( ( ( ( ( ( hourUtcGps() * 60 ) + minuteUtcGps() ) * 60 ) + secondUtcGps() ) * 1000 ) + millisecondUtcGps()) * 10;
    // write Position Date as Days since 1.1.1970
    number2LittleEndianString( ui16_daysSince1970, writeRef );
    // write Position Time as Milliseconds*10 per day
    number2LittleEndianString( ui32_milliseconds, writeRef );
    // write Latitude as uint64_t value
    setDegree10Minus7ToStream( mi32_latitudeDegree10Minus7, writeRef );
    // write Longitude as uint64_t value
    setDegree10Minus7ToStream( mi32_longitudeDegree10Minus7, writeRef );
    // write Altitude as uint64_t value
    setAltitude10Minus2ToStream( mi32_altitudeCm, writeRef );

    // write type and method
    const uint8_t cu8_tempTypeMethod = ( mt_gnssType | ( mt_gnssMethod << 4 ) );
    number2LittleEndianString( cu8_tempTypeMethod, writeRef );
    // write integrity ( set unused highest 6 bits to "1" )
    number2LittleEndianString( uint8_t(mui8_integrity| 0xFC), writeRef );
    // write #satelites
    number2LittleEndianString( mui8_satelliteCnt, writeRef );
    // write HDOP
    number2LittleEndianString( mi16_hdop, writeRef );
    // write PDOP
    number2LittleEndianString( mi16_pdop, writeRef );
    // write geodial separation
    number2LittleEndianString( mi32_geoidalSeparationCm, writeRef );

    // write number of reference stations
    uint8_t ui8_limit = mui8_noRefStations;
    if ( mvec_refStationTypeAndStation.size() < ui8_limit ) ui8_limit = mvec_refStationTypeAndStation.size();
    if ( mvec_refStationDifferentialAge10Msec.size() < ui8_limit ) ui8_limit = mvec_refStationDifferentialAge10Msec.size();

    number2LittleEndianString( ui8_limit, writeRef );

    for ( unsigned int ind = 0; ind < ui8_limit; ind++ )
    {
      number2LittleEndianString( mvec_refStationTypeAndStation[ind], writeRef );
      number2LittleEndianString( mvec_refStationDifferentialAge10Msec[ind], writeRef );
    }

    //now trigger sending
    #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
    if ( getMultiSendInstance4Comm().sendIsoFastPacketBroadcast(mc_sendGpsISOName, &mc_nmea2000Streamer, NMEA_GPS_POSITION_DATA_PGN, &mt_multiSendEventHandler) )
    #else
    if ( getMultiSendInstance4Comm().sendIsoBroadcast(mc_sendGpsISOName, &mc_nmea2000Streamer, NMEA_GPS_POSITION_DATA_PGN, &mt_multiSendEventHandler) )
    #endif
    { // update time
      mi32_lastIsoPositionStream = ci32_now;
   }
  }
  #endif // END OF ENABLE_NMEA_2000_MULTI_PACKET

  /** send ISO11783 calendar PGN
      @param acrc_isoName ISOName code off calling item which wants to send
      possible errors:
          * dependant error in CanIo_c on CAN send problems
      @see CanPkg_c::getData
      @see CanPkgExt_c::getData
      @see CanIo_c::operator<<
    */
  void TimePosGps_c::sendCalendar()
  {
    if( ( ! getIdentItem() ) || ( ! getIdentItem()->isClaimedAddress() ) )
      return;

    CanPkgExt_c pkg;
    pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
    pkg.setIsoPri(6);
    pkg.setLen(8);

    if ( ( getSelectedDataSourceISOName() == getIdentItem()->isoName() ) )
    { // this item (identified by ISOName is configured to send
      if ( checkMode(IsoAgLib::IdentModeTractor) )
        setSelectedDataSourceISOName( getIdentItem()->isoName() );

      pkg.setIsoPgn(TIME_DATE_PGN);

      const struct CNAMESPACE::tm* p_tm = currentUtcTm();
      if (NULL != p_tm)
      {
        pkg.setUint8Data(0, (p_tm->tm_sec * 4) );
        pkg.setUint8Data(1, p_tm->tm_min);
        pkg.setUint8Data(2, p_tm->tm_hour);
        pkg.setUint8Data(3, p_tm->tm_mon + 1);
        pkg.setUint8Data(4, p_tm->tm_mday * 4 + (p_tm->tm_hour/6) - 3);
        pkg.setUint8Data(5, p_tm->tm_year + 1900 - 1985);
      }
      else
      {
        pkg.setUint16Data( 0, 0U );
        pkg.setUint8Data(2, 0U);
        pkg.setUint8Data(3, 1U);
        pkg.setUint8Data(4, 1U);
        pkg.setUint8Data(5, 0U);
      }
      pkg.setUint8Data(6, bit_calendar.timezoneMinuteOffset );
      pkg.setUint8Data(7, bit_calendar.timezoneHourOffsetMinus24 );

      // CanIo_c::operator<< retreives the information with the help of CanPkg_c::getData
      // then it sends the data
      getIsoBusInstance4Comm() << pkg;

      // update time
      setUpdateTime( System_c::getTime() );
    }
  }

  /** check if a calendar information was received since init
   */
  bool TimePosGps_c::isCalendarReceived() const
  {
    return !( ( bit_calendar.day == 1 ) && ( bit_calendar.month == 1 ) && ( bit_calendar.year == 0 ) // year 0 (absolute) indicates NO_VAL!
        && ( bit_calendar.second == 0 ) && ( bit_calendar.minute == 0 ) && ( bit_calendar.hour == 0 ) );
  }

  /** check if a calendar's DATE information that was received is VALID */
  bool TimePosGps_c::isCalendarDateValid() const
  { // check if default data from init is still in vars
    return ( (bit_calendar.day > 0) && (bit_calendar.month > 0) && (bit_calendar.year > 0) );
  }

  /** check if a calendar's TIME information that was received is VALID
   * @todo ON REQUEST-259: Add this check if it's ensured everywhere that NO_VAL times
   *       don't screw around (with some mktime() or whatever functions...)
  bool TimePosGps_c::isCalendarTimeValid() const
   */

  /**
  set the calendar value
  @param ai16_year value to store as year
  @param ab_month value to store as month
  @param ab_day value to store as day
  @param ab_hour value to store as hour
  @param ab_minute value to store as minute
  @param ab_second value to store as second
  */
  void TimePosGps_c::setCalendarUtc(int16_t ai16_year, uint8_t ab_month, uint8_t ab_day, uint8_t ab_hour, uint8_t ab_minute, uint8_t ab_second, uint16_t aui16_msec)
  {
    mi32_lastCalendarSet = System_c::getTime();
    mt_cachedLocalSeconds1970AtLastSet = 0;

    bit_calendar.year   = ai16_year;
    bit_calendar.month  = ab_month;
    bit_calendar.day    = ab_day;
    bit_calendar.hour   = ab_hour;
    bit_calendar.minute = ab_minute;
    bit_calendar.second = ab_second;
    bit_calendar.msec   = aui16_msec;
  };

  /**
  set the calendar value as local time ( take local time offsets into account )
  @param ai16_year value to store as year
  @param ab_month value to store as month
  @param ab_day value to store as day
  @param ab_hour value to store as hour
  @param ab_minute value to store as minute
  @param ab_second value to store as second
  */
  void TimePosGps_c::setCalendarLocal(int16_t ai16_year, uint8_t ab_month, uint8_t ab_day, uint8_t ab_hour, uint8_t ab_minute, uint8_t ab_second, uint16_t aui16_msec )
  {
    mi32_lastCalendarSet = System_c::getTime();
    mt_cachedLocalSeconds1970AtLastSet = 0;
    struct CNAMESPACE::tm testTime = { ab_second, int(ab_minute)-int(bit_calendar.timezoneMinuteOffset), (int(ab_hour)-(int(bit_calendar.timezoneHourOffsetMinus24)-24)),
                            ab_day,(ab_month-1),(ai16_year-1900),0,0,-1
                            #if defined(__USE_BSD) || defined(__GNU_LIBRARY__) || defined(__GLIBC__) || defined(__GLIBC_MINOR__)
                            , 0, NULL
                            #endif
                            };
    // argument of mktime is interpreted as local time (system time zone influence!)
    const CNAMESPACE::time_t middle = MACRO_ISOAGLIB_MKTIME( &testTime );
    // compensate system time zone setting: call localtime() and not gmtime()
    const struct CNAMESPACE::tm* normalizedTime = MACRO_ISOAGLIB_LOCALTIME( &middle );

    bit_calendar.year   = normalizedTime->tm_year+1900;
    bit_calendar.month  = (normalizedTime->tm_mon+1);
    bit_calendar.day    = normalizedTime->tm_mday;
    bit_calendar.hour   = normalizedTime->tm_hour;
    bit_calendar.minute = normalizedTime->tm_min;
    bit_calendar.second = normalizedTime->tm_sec;
    bit_calendar.msec   = aui16_msec;
  };

  /** set the date in local timezone */
  void TimePosGps_c::setDateLocal(int16_t ai16_year, uint8_t ab_month, uint8_t ab_day)
  {
    mi32_lastCalendarSet = System_c::getTime();
    mt_cachedLocalSeconds1970AtLastSet = 0;

    /** @todo ON REQUEST-259: calender time consists of UTC time and local date? */
    struct CNAMESPACE::tm testTime = { bit_calendar.second, bit_calendar.minute, bit_calendar.hour,
                            ab_day,(ab_month-1),(ai16_year-1900),0,0,-1
                            #if defined(__USE_BSD) || defined(__GNU_LIBRARY__) || defined(__GLIBC__) || defined(__GLIBC_MINOR__)
                            , 0, NULL
                            #endif
                          };

    const CNAMESPACE::time_t middle = MACRO_ISOAGLIB_MKTIME( &testTime );
    const struct CNAMESPACE::tm* normalizedTime = MACRO_ISOAGLIB_LOCALTIME( &middle );

    bit_calendar.year   = normalizedTime->tm_year+1900;
    bit_calendar.month  = (normalizedTime->tm_mon+1);
    bit_calendar.day    = normalizedTime->tm_mday;
  }

  /** set the date in UTC timezone */
  void TimePosGps_c::setDateUtc(int16_t ai16_year, uint8_t ab_month, uint8_t ab_day)
  {
    mi32_lastCalendarSet = System_c::getTime();
    mt_cachedLocalSeconds1970AtLastSet = 0;
    bit_calendar.year   = ai16_year;
    bit_calendar.month  = ab_month;
    bit_calendar.day    = ab_day;
  }

  /** set the time in local timezone */
  void TimePosGps_c::setTimeLocal(uint8_t ab_hour, uint8_t ab_minute, uint8_t ab_second, uint16_t aui16_msec)
  {
    mi32_lastCalendarSet = System_c::getTime();
    mt_cachedLocalSeconds1970AtLastSet = 0;

    struct CNAMESPACE::tm testTime = { ab_second, int(ab_minute)-int(bit_calendar.timezoneMinuteOffset), (int(ab_hour)-(int(bit_calendar.timezoneHourOffsetMinus24)-24)),
                            bit_calendar.day,(bit_calendar.month-1),
                            (bit_calendar.year == 0) ? 70 : (bit_calendar.year-1900), // in case bit_calendar.year is not yet set: use 1970
                            0,0,-1
                            #if defined(__USE_BSD) || defined(__GNU_LIBRARY__) || defined(__GLIBC__) || defined(__GLIBC_MINOR__)
                            , 0, NULL
                            #endif
                          };

    // argument of mktime is interpreted as local time (system time zone influence!)
    const CNAMESPACE::time_t middle = MACRO_ISOAGLIB_MKTIME( &testTime );
    // compensate system time zone setting: call localtime() and not gmtime()
    const struct CNAMESPACE::tm* normalizedTime = MACRO_ISOAGLIB_LOCALTIME( &middle );

    bit_calendar.hour   = normalizedTime->tm_hour;
    bit_calendar.minute = normalizedTime->tm_min;
    bit_calendar.second = normalizedTime->tm_sec;
    bit_calendar.msec   = aui16_msec;
  }

  /** set the time in UTC timezone */
  void TimePosGps_c::setTimeUtc(uint8_t ab_hour, uint8_t ab_minute, uint8_t ab_second, uint16_t aui16_msec, bool b_updateDate)
  {
    if (b_updateDate)
    {
      struct CNAMESPACE::tm* p_tm = currentUtcTm();
      if (p_tm != NULL)
      {
        bit_calendar.year   = p_tm->tm_year + 1900;
        bit_calendar.month  = p_tm->tm_mon +1;
        bit_calendar.day    = p_tm->tm_mday;
      }
      // else: can't update date.
    }

    mi32_lastCalendarSet = System_c::getTime();
    mt_cachedLocalSeconds1970AtLastSet = 0;
    bit_calendar.hour   = ab_hour;
    bit_calendar.minute = ab_minute;
    bit_calendar.second = ab_second;
    bit_calendar.msec   = aui16_msec;
  }

  const struct CNAMESPACE::tm* TimePosGps_c::Utc2LocalTime()
  {
    if (0 == mt_cachedLocalSeconds1970AtLastSet)
    {
      // set mt_cachedLocalSeconds1970AtLastSet
      currentUtcTm();
    }

    CNAMESPACE::time_t t_secondsSince1970Local = mt_cachedLocalSeconds1970AtLastSet + calendarSetAge()/1000L
                                     + (bit_calendar.timezoneHourOffsetMinus24 - 24L) * 60L * 60L  // negative offsets => increased local time
                                     + bit_calendar.timezoneMinuteOffset * 60L;

    for (;;){
      // compensate system time zone setting: call localtime() and not gmtime()
      struct CNAMESPACE::tm *p_ret = MACRO_ISOAGLIB_LOCALTIME( &t_secondsSince1970Local );
      if (p_ret)
        return p_ret;
      // non-negative, because otherwise localtime(..) would return NULL!
      t_secondsSince1970Local = 0;
    }
  }

  /**
  get the calendar year value
  @return actual calendar year value
  */
  int16_t TimePosGps_c::yearLocal()
  {
    return Utc2LocalTime()->tm_year+1900;
  }

  /**
  get the calendar month value
  @return actual calendar month value
  */
  uint8_t TimePosGps_c::monthLocal()
  { // month is delivered with range [0..11] -> add 1
    return Utc2LocalTime()->tm_mon+1;
  }

  /**
  get the calendar day value
  @return actual calendar day value
  */
  uint8_t TimePosGps_c::dayLocal()
  {
    return Utc2LocalTime()->tm_mday;
  }

  /**
  get the calendar hour value
  @return actual calendar hour value
  */
  uint8_t TimePosGps_c::hourLocal()
  {
    return Utc2LocalTime()->tm_hour;
  }

  /**
  get the calendar minute value
  @return actual calendar minute value
  */
  uint8_t TimePosGps_c::minuteLocal()
  {
    return Utc2LocalTime()->tm_min;
  };

  int16_t TimePosGps_c::yearUtc()
  {
    const struct CNAMESPACE::tm* p_tm = currentUtcTm();
    return (NULL != p_tm) ? p_tm->tm_year + 1900 : 0;
  }
  uint8_t TimePosGps_c::monthUtc()
  {
    const struct CNAMESPACE::tm* p_tm = currentUtcTm();
    return (NULL != p_tm) ? p_tm->tm_mon + 1 : 0;
  }
  uint8_t TimePosGps_c::dayUtc()
  {
    const struct CNAMESPACE::tm* p_tm = currentUtcTm();
    return (NULL != p_tm) ? p_tm->tm_mday : 0;
  }
  uint8_t TimePosGps_c::hourUtc()
  {
    const struct CNAMESPACE::tm* p_tm = currentUtcTm();
    return (NULL != p_tm) ? p_tm->tm_hour : 0;
  }
  uint8_t TimePosGps_c::minuteUtc()
  {
    const struct CNAMESPACE::tm* p_tm = currentUtcTm();
    return (NULL != p_tm) ? p_tm->tm_min : 0;
  }
  uint8_t TimePosGps_c::second()
  {
    const struct CNAMESPACE::tm* p_tm = currentUtcTm();
    return (NULL != p_tm) ? p_tm->tm_sec : 0;
  }

  struct CNAMESPACE::tm* TimePosGps_c::currentUtcTm()
  {
    if ( 0 == mt_cachedLocalSeconds1970AtLastSet)
    { // recalculate seconds from bit_calendar struct
      // compensate system time zone setting (part 1)
      if (bit_calendar.year != 0)
      {
        struct CNAMESPACE::tm testTime = { bit_calendar.second, bit_calendar.minute, bit_calendar.hour,
                               bit_calendar.day, bit_calendar.month-1,
                               bit_calendar.year-1900,
                               0,0,-1
                               #if defined(__USE_BSD) || defined(__GNU_LIBRARY__) || defined(__GLIBC__) || defined(__GLIBC_MINOR__)
                               , 0, NULL
                               #endif
                             };
        mt_cachedLocalSeconds1970AtLastSet = MACRO_ISOAGLIB_MKTIME( &testTime );
        if ((CNAMESPACE::time_t)-1 == mt_cachedLocalSeconds1970AtLastSet)
        { // this shouldn't happen anymore, but in case it does, reset the cachedSeconds to 0
          // because -1 will make localtime(..) return NULL!
          mt_cachedLocalSeconds1970AtLastSet = 0;
          return NULL;
        }
      }
      // in case bit_calendar.year is not yet set: keep mt_cachedLocalSeconds1970AtLastSet 0.
    }

    const CNAMESPACE::time_t t_secondsSince1970 = mt_cachedLocalSeconds1970AtLastSet + calendarSetAge()/1000;

    // compensate system time zone setting (part 2)
    return MACRO_ISOAGLIB_LOCALTIME( &t_secondsSince1970 );
  }

void
TimePosGps_c::notifyOnEvent(uint32_t aui32_pgn)
{
  STL_NAMESPACE::vector<MsgEventHandler_c*>::iterator iter_end = mvec_msgEventHandlers.end();
  for (STL_NAMESPACE::vector<MsgEventHandler_c*>::iterator iter = mvec_msgEventHandlers.begin(); iter != iter_end; ++iter)
  { // call handler for each entry
    (*iter)->handleMsgEvent (aui32_pgn);
  }
}


void
TimePosGps_c::deregisterMsgEventHandler (MsgEventHandler_c &arc_msgEventHandler)
{
  STL_NAMESPACE::vector<MsgEventHandler_c*>::iterator iter_end = mvec_msgEventHandlers.end();
  for (STL_NAMESPACE::vector<MsgEventHandler_c*>::iterator iter = mvec_msgEventHandlers.begin(); iter != iter_end;)
  {
    if ((*iter) == &arc_msgEventHandler)
    { // remove entry
      iter = mvec_msgEventHandlers.erase (iter);
    }
    else
    { // keep entry
      ++iter;
    }
  }
}

void
TimePosGps_c::reactOnStateChange(const SendStream_c& sendStream)
{
#ifdef ENABLE_NMEA_2000_MULTI_PACKET
  mt_multiSendSuccessState = sendStream.getSendSuccess();
#else
  (void)sendStream;
#endif
}


} // namespace __IsoAgLib

