/*
  timeposgps_c.h: working on GPS data and Calendar; stores, updates
    and delivers all information from CanCustomer_c derived for CAN
    sending and receiving interaction; from BaseCommon_c derived for
    interaction with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TIMEPOSGPS_C_H
#define TIMEPOSGPS_C_H

#include <IsoAgLib/comm/Part3_DataLink/impl/multisendeventhandler_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgnhandler_c.h>
#include <IsoAgLib/comm/Part7_ApplicationLayer/impl/basecommon_c.h>

#if defined(ENABLE_NMEA_2000_MULTI_PACKET)
#include <IsoAgLib/comm/Part3_DataLink/imultisendstreamer_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisend_c.h>
#endif

#include <ctime>


namespace __IsoAgLib {

#if defined(ENABLE_NMEA_2000_MULTI_PACKET)

class MultiSendPkg_c;
class Nmea2000SendStreamer_c : public IsoAgLib::iMultiSendStreamer_c
{
  public:
    Nmea2000SendStreamer_c() : ui16_currentSendPosition(0), ui16_storedSendPosition(0) {};
    virtual ~Nmea2000SendStreamer_c()  {};
    /** place next data to send direct into send buffer of pointed
      stream send package - MultiSend_c will send this
      buffer afterwards
    */
    virtual void setDataNextStreamPart (MultiSendPkg_c* mspData, uint8_t bytes);

#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
    /** place next data to send direct into send buffer of pointed
      stream send package - MultiSend_c will send this
      buffer afterwards
   */
    virtual void setDataNextFastPacketStreamPart (MultiSendPkg_c* mspData, uint8_t bytes, uint8_t aui8_offset = 0 );
#endif

    /** set cache for data source to stream start */
    virtual void resetDataNextStreamPart();

    /** save current send position in data source - neeed for resend on send problem */
    virtual void saveDataNextStreamPart ();

    /** reactivate previously stored data source position - used for resend on problem */
    virtual void restoreDataNextStreamPart ();

    /** calculate the size of the data source */
    virtual uint32_t getStreamSize ();

    /** get the first byte, which is the COMMAND-byte and should be given back CONST! */
    virtual uint8_t getFirstByte ();

    void reset() { ui16_currentSendPosition = ui16_storedSendPosition = 0; vec_data.clear();};

    /** public send buffer */
    STL_NAMESPACE::vector<uint8_t> vec_data;
  private:
    uint16_t ui16_currentSendPosition;
    uint16_t ui16_storedSendPosition;
};
#endif // END of ENABLE_NMEA_2000_MULTI_PACKET


/** Class definition to be used with event processing of GPS messages et al. **/
class MsgEventHandler_c
{
public:
  MsgEventHandler_c() {}
  virtual ~MsgEventHandler_c() {}
  virtual void handleMsgEvent (uint32_t aui32_pgn) = 0;
};

class TimePosGps_c;

/** working on GPS data and Calendar;
  stores, updates  and delivers all base data informations;
  Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
  Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
  Derive from SINGLETON to create a Singleton which manages one global accessible singleton
  per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
  */

class TimePosGps_c : public BaseCommon_c
{
  MACRO_MULTITON_CONTRIBUTION();
public:
  // Public methods
  /* ********************************************* */
  /** \name Management Functions for class TimePosGps_c  */
  /*@{*/

  /** functions with actions, which must be performed periodically
      -> called periodically by Scheduler_c
      ==> sends base msg if configured in the needed rates
      possible errors:
        * dependant error in CanIo_c on CAN send problems
      @see CanPkg_c::getData
      @see CanPkgExt_c::getData
      @see CanIo_c::operator<<
    */
  virtual void timeEvent();

  // from BaseCommon_c
  virtual void init_specialized();

  /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
      @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      @return true -> configuration was successfull
    */
  virtual bool config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask = 0 );

  virtual ~TimePosGps_c() {}

  /** process a message request for PGN
      @param aui32_pgn PGN
      @param apc_isoItemSender pointer to source address
      @param apc_isoItemReceiver pointer to destination address
    */
  bool processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t ai_time );

  /** force a request for pgn for time/date information */
  bool sendRequestUpdateTimeDate();

#ifdef USE_J1939_VEHICLE_PGNS
  void sendVehiclePosDirSpd() const;
#endif

  /** config the Base_c object after init -> set pointer to isoName and
      config send/receive of different base msg types
      @param apc_isoName pointer to the ISOName variable of the responsible member instance (pointer enables automatic value update if var val is changed)
      @param at_identModeGps either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      @return true -> configuration was successfull
    */
  bool configGps( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identModeGps);

  /** return if you currently are in gps mode*/
  bool checkModeGps(IsoAgLib::IdentMode_t at_identModeGps) const {return (mt_identModeGps == at_identModeGps);}

  bool checkParseReceivedGps(const IsoName_c& acrc_currentSender) const;

  /** send ISO11783 calendar PGN */
  void sendCalendar();

  /** Retrieve the last update time of the specified information type*/
  int32_t lastedTimeSinceUpdateGps() const;

  /** Retrieve the time of last update */
  int32_t lastUpdateTimeGps() const;


  /** Retrieve the last update time of the specified information type*/
  int32_t lastedTimeSinceUpdateDirection() const;

  /** Retrieve the time of last update */
  int32_t lastUpdateTimeDirection() const;


  /** return a sender which sends commands as a tractor */
  IsoName_c& getSenderISONameGps() {return mc_sendGpsISOName;}

  /** return a sender which sends commands as a tractor */
  const IsoName_c& getSenderISONameGpsConst() const {return mc_sendGpsISOName;}

  /* ********************************************* */
  /** \name MultiReceive functions for TimePosGps_c  */
  /*@{*/
  #if defined(ENABLE_NMEA_2000_MULTI_PACKET)
public:
  virtual bool reactOnStreamStart(const ReceiveStreamIdentifier_c& ac_ident,
                                  uint32_t aui32_totalLen);


  virtual bool processPartStreamDataChunk(Stream_c& apc_stream,
                                          bool ab_isFirstChunk,
                                          bool ab_isLastChunkAndACKd);

  virtual void reactOnAbort (Stream_c& apc_stream);

private:
  bool reactOnLastChunk (const ReceiveStreamIdentifier_c& ac_ident, Stream_c& apc_stream);
  #endif // END of ENABLE_NMEA_2000_MULTI_PACKET

public:
  /// Setter functions /////////////////////////////////////////////////////////////////////////////////
  /** set the calendar value
    @param ai16_year value to store as year
    @param ab_month value to store as month
    @param ab_day value to store as day
    @param ab_hour value to store as hour
    @param ab_minute value to store as minute
    @param ab_second value to store as second
   */
  void setCalendarUtc(int16_t ai16_year, uint8_t ab_month, uint8_t ab_day, uint8_t ab_hour, uint8_t ab_minute, uint8_t ab_second, uint16_t aui16_msec = 0);

  /** set the calendar value as local time ( take local time offsets into account )
    @param ai16_year value to store as year
    @param ab_month value to store as month
    @param ab_day value to store as day
    @param ab_hour value to store as hour
    @param ab_minute value to store as minute
    @param ab_second value to store as second
    */
  void setCalendarLocal(int16_t ai16_year, uint8_t ab_month, uint8_t ab_day, uint8_t ab_hour, uint8_t ab_minute, uint8_t ab_second, uint16_t aui16_msec = 0);

  /** set the date in local timezone */
  void setDateLocal(int16_t ai16_year, uint8_t ab_month, uint8_t ab_day);

  /** set the date in UTC timezone */
  void setDateUtc(int16_t ai16_year, uint8_t ab_month, uint8_t ab_day);

  /** set the time in local timezone */
  void setTimeLocal(uint8_t ab_hour, uint8_t ab_minute, uint8_t ab_second, uint16_t aui16_msec = 0);

  /** set the time in UTC timezone */
  void setTimeUtc(uint8_t ab_hour, uint8_t ab_minute, uint8_t ab_second, uint16_t aui16_msec = 0, bool b_updateDate = false);

  /** set the actual calendar year value */
  void setYearUtc(uint16_t aui16_year){mi32_lastCalendarSet = System_c::getTime();bit_calendar.year = aui16_year; mt_cachedLocalSeconds1970AtLastSet = 0;};

  /** set raw GPS Latitude [degree] with scaling 10.0e-7 */
  void setGpsLatitudeDegree10Minus7( int32_t ai32_newVal ) { mi32_latitudeDegree10Minus7 = ai32_newVal; }

  /** set raw GPS Longitude [degree] with scaling 10.0e-7 */
  void setGpsLongitudeDegree10Minus7( int32_t ai32_newVal ) { mi32_longitudeDegree10Minus7 = ai32_newVal; }

  /** deliver GPS Speed Over Ground as [cm/s] */
  void setGpsSpeedCmSec( uint16_t aui16_newVal ) { mui16_speedOverGroundCmSec = aui16_newVal; }
  /** deliver GPS Course Over Ground [1x10E-4rad] */
  void setGpsCourseRad10Minus4( uint16_t aui16_newVal ) { mui16_courseOverGroundRad10Minus4 = aui16_newVal; };

  /** set GPS altitude - [cm] */
  void setGpsAltitudeCm( int32_t ai32_newVal ) { mi32_altitudeCm = ai32_newVal; }

  #if defined(ENABLE_NMEA_2000_MULTI_PACKET)
  /** set the GPS time in UTC timezone.
   *  When no remote system is sending the 11783-7 PGN with date & time, the new UTC time is also set with
   *  setTimeUtc().
   *  In case another system is sending TIME_DATE_PGN, this time could be out-of-sync with GPS time.
   *  To avoid a jumping back and forth Non-GPS UTC time, those two UTC time sources are then not to be synced.
   */
  void setTimeUtcGps(uint8_t ab_hour, uint8_t ab_minute, uint8_t ab_second, uint16_t aui16_msec = 0);

  /** set the calendar hour value
   *  When no remote system is sending the 11783-7 PGN with date & time, the new UTC time is also set with
   *  setTimeUtc().
   *  In case another system is sending TIME_DATE_PGN, this time could be out-of-sync with GPS time.
   *  To avoid a jumping back and forth Non-GPS UTC time, those two UTC time sources are then not to be synced.
   *  @param ab_hour actual calendar hour value
   */
  void setHourUtcGps(uint8_t ab_hour);

  /** set the calendar minute value
   *  When no remote system is sending the 11783-7 PGN with date & time, the new UTC time is also set with
   *  setTimeUtc().
   *  In case another system is sending TIME_DATE_PGN, this time could be out-of-sync with GPS time.
   *  To avoid a jumping back and forth Non-GPS UTC time, those two UTC time sources are then not to be synced.
   *  @param ab_minute actual calendar minute value
   */
  void setMinuteUtcGps(uint8_t ab_minute);

  /** set the calendar second value
   *  When no remote system is sending the 11783-7 PGN with date & time, the new UTC time is also set with
   *  setTimeUtc().
   *  In case another system is sending TIME_DATE_PGN, this time could be out-of-sync with GPS time.
   *  To avoid a jumping back and forth Non-GPS UTC time, those two UTC time sources are then not to be synced.
   *  @param ab_second actual calendar second value
   */
  void setSecondUtcGps(uint8_t ab_second);

  /** set the calendar millisecond value
   *  When no remote system is sending the 11783-7 PGN with date & time, the new UTC time is also set with
   *  setTimeUtc().
   *  In case another system is sending TIME_DATE_PGN, this time could be out-of-sync with GPS time.
   *  To avoid a jumping back and forth Non-GPS UTC time, those two UTC time sources are then not to be synced.
   *  @param ab_millisecond actual calendar second value
   */
  void setMillisecondUtcGps(uint16_t aui16_millisecond);

  /** set GPS receive qualitiy */
  void setGnssMode( IsoAgLib::IsoGnssMethod_t at_newVal );

  /** set GNSS type ( e.g. GPS, GLONASS or SBAS ) */
  void setGnssType( IsoAgLib::IsoGnssType_t at_newVal )   { if(at_newVal<=IsoAgLib::IsoGnssTypeMAX) mt_gnssType = at_newVal;}

  /** set number of received satellites */
  void setSatelliteCnt( uint8_t aui8_newVal ) { mui8_satelliteCnt = aui8_newVal;}

  /** set HDOP with scaling [1x10E-2] */
  void setHdop10Minus2( uint16_t aui16_newVal ) { mi16_hdop = aui16_newVal;}

  /** set PDOP with scaling [1x10E-2] */
  void setPdop10Minus2( uint16_t aui16_newVal ) { mi16_pdop = aui16_newVal;}
  #endif // END of ENABLE_NMEA_2000_MULTI_PACKET

  /** set the calendar month value
    @param ab_month actual calendar month value
    */
  void setMonthUtc(uint8_t ab_month){mi32_lastCalendarSet = System_c::getTime();bit_calendar.month = ab_month; mt_cachedLocalSeconds1970AtLastSet = 0;}

  /** set the calendar day value
    @param ab_day actual calendar day value
    */
  void setDayUtc(uint8_t ab_day){mi32_lastCalendarSet = System_c::getTime();bit_calendar.day = ab_day; mt_cachedLocalSeconds1970AtLastSet = 0;}

  /** set the calendar hour value
    @param ab_hour actual calendar hour value
    */
  void setHourUtc(uint8_t ab_hour){mi32_lastCalendarSet = System_c::getTime();bit_calendar.hour = ab_hour; mt_cachedLocalSeconds1970AtLastSet = 0;}

  /** set the calendar minute value
    @param ab_minute actual calendar minute value
    */
  void setMinuteUtc(uint8_t ab_minute){mi32_lastCalendarSet = System_c::getTime();bit_calendar.minute = ab_minute; mt_cachedLocalSeconds1970AtLastSet = 0;}

  /** set the calendar second value
    @param ab_second actual calendar second value
    */
  void setSecond(uint8_t ab_second){mi32_lastCalendarSet = System_c::getTime();bit_calendar.second = ab_second; mt_cachedLocalSeconds1970AtLastSet = 0;}

  /** set the calendar millisecond value
    @param aui16_millisecond actual calendar second value
    */
  void setMillisecond(uint16_t aui16_millisecond){mi32_lastCalendarSet = System_c::getTime();bit_calendar.msec = aui16_millisecond; mt_cachedLocalSeconds1970AtLastSet = 0;}

  /** set the local time to UTC time offsets */
  void setLocalTimeOffsets( int16_t ai16_hourOffset, uint16_t aui16_minuteOffset )
  {bit_calendar.timezoneHourOffsetMinus24 = (ai16_hourOffset+24); bit_calendar.timezoneMinuteOffset = aui16_minuteOffset;}


  /// Getter functions
  /** check if a calendar information was received since init */
  bool isCalendarReceived() const;

  /** check if a calendar's DATE information was received with valid value since init */
  bool isCalendarDateValid() const;

  /** check if a calendar's TIME information was received with valid value since init */
  /// @todo SOON-259: ... bool isCalendarTimeValid() const;

  /** get the calendar year value
      @return actual calendar year value
    */
  int16_t yearLocal();

  /** get the calendar year value
    @return actual calendar year value
    */
  int16_t yearUtc();

  /** get the calendar month value
    @return actual calendar month value
    */
  uint8_t monthLocal();

  /** get the calendar month value
    @return actual calendar month value
    */
  uint8_t monthUtc();

  /** get the calendar day value
    @return actual calendar day value
    */
  uint8_t dayLocal();

  /** get the calendar day value
    @return actual calendar day value
    */
  uint8_t dayUtc();

  /** get the calendar hour value
    @return actual calendar hour value
    */
  uint8_t hourLocal();

  /** get the calendar hour value
    @return actual calendar hour value
    */
  uint8_t hourUtc();

  /** get the calendar minute value
    @return actual calendar minute value
    */
  uint8_t minuteLocal();

  /** get the calendar minute value
    @return actual calendar minute value
    */
  uint8_t minuteUtc();

  /** get the calendar second value
    @return actual calendar second value
    */
  uint8_t second();

  /** get the calendar millisecond value
      @return actual calendar second value
    */
  uint16_t millisecond() const {return ((bit_calendar.msec + calendarSetAge())%1000);}

  /** deliver UTC time in struct tm instead of separate calls to minuteUtc(), hourUtc(), ... to reduce load
    @return struct tm
  */
  struct CNAMESPACE::tm* currentUtcTm();



  /** *** Position ** **/

  /** deliver raw GPS Latitude [degree] with scaling 10.0e-7 */
  int32_t getGpsLatitudeDegree10Minus7( void ) const { return mi32_latitudeDegree10Minus7; }

  /** deliver raw GPS Longitude [degree] with scaling 10.0e-7 */
  int32_t getGpsLongitudeDegree10Minus7( void ) const { return mi32_longitudeDegree10Minus7; }

  /** check if an NMEA2000 position signal was received */
  bool isPositionReceived() const;

  /** check if we have valid Position data, so the information should be sent out in timeEventTracMode */
  bool isPositionSimpleToSend() const
    { return ( (mi32_latitudeDegree10Minus7  >= ( -90*10000000)) && (mi32_latitudeDegree10Minus7  <= ( 90*10000000))
            && (mi32_longitudeDegree10Minus7 >= (-180*10000000)) && (mi32_longitudeDegree10Minus7 <= (180*10000000))); }

  /** deliver Minute GPS Latitude */
  float getGpsLatitudeMinute( void ) const { return ( mi32_latitudeDegree10Minus7 * 6.0e-4f  ); }

  /** deliver Minute GPS Longitude */
  float getGpsLongitudeMinute( void ) const { return ( mi32_longitudeDegree10Minus7 * 6.0e-4f ); }

  /** deliver Degree GPS Latitude */
  float getGpsLatitudeDegree( void ) const { return ( float(mi32_latitudeDegree10Minus7) * 1.0e-7f  ); }

  /** deliver Degree GPS Longitude */
  float getGpsLongitudeDegree( void ) const { return ( float(mi32_longitudeDegree10Minus7) * 1.0e-7f ); }

  
  /** *** Direction *** **/

  /** deliver GPS Speed Over Ground as [cm/s] */
  uint16_t getGpsSpeedCmSec( void ) const { return mui16_speedOverGroundCmSec; }
  /** deliver GPS Course Over Ground [1x10E-4rad] */
  uint16_t getGpsCourseRad10Minus4( void ) const { return mui16_courseOverGroundRad10Minus4; }

  /** check if an NMEA2000 direction signal was received */
  bool isDirectionReceived() const;

  /** check if we have valid Direction data, so the information should be sent out in timeEventTracMode */
  bool isDirectionToSend() const
    /// @todo SOON-259: check for the REAL max, 62855 is a little bigger than 62831 or alike that could be calculated. but anyway...
    { return ( (mui16_courseOverGroundRad10Minus4 <= 62855)
            && (mui16_speedOverGroundCmSec <= 65532)); }

  /** deliver GPS receive qualitiy - also needed to see if we have valid GPS-positioning!!! */
  IsoAgLib::IsoGnssMethod_t getGnssMode( void ) const { return mt_gnssMethod; }

  bool isPositionStreamToSend() const
  { return isPositionSimpleToSend() && (mi32_altitudeCm != 0x7FFFFFFF); }

  /** deliver GPS altitude - [cm] */
  int32_t getGpsAltitudeCm( void ) const { return mi32_altitudeCm; }

  #ifdef ENABLE_NMEA_2000_MULTI_PACKET

  /** get the GPS UTC hour value
    @return actual GPS UTC hour value
   */
  uint8_t hourUtcGps() const {return bit_gpsTime.hour;}

  /** get the GPS UTC minute value
      @return actual GPS UTC minute value
   */
  uint8_t minuteUtcGps() const {return bit_gpsTime.minute;}

  /** get the GPS UTC second value
      @return actual GPS UTC second value
   */
  uint8_t secondUtcGps() const {return bit_gpsTime.second;}

  /** get the GPS UTC millisecond value
      @return actual GPS UTC millisecond value
   */
  uint16_t millisecondUtcGps() const {return bit_gpsTime.msec;}

  /** get the millisecond time of day for the last received position */
  uint32_t millisecondTimeOfDay() const { return mui32_timeMillisecondOfDay; }

  /** deliver GPS Geodial Separation - [cm] */
  int32_t getGpsGeoidalSeparationCm ( void ) const { return mi32_geoidalSeparationCm; }

  /** simply check for some sort of Differential signal */
  bool hasDifferentialPosition() const { return ( ( mt_gnssMethod > IsoAgLib::IsoGnssFix ) && ( mt_gnssMethod < IsoAgLib::IsoDrEstimated ) );}

  /** deliver GNSS type ( e.g. GPS, GLONASS or SBAS ) */
  IsoAgLib::IsoGnssType_t getGnssType(void) const { return mt_gnssType; }

  /** deliver number of received satellites */
  uint8_t satelliteCnt() const { return mui8_satelliteCnt; }

  /** deliver HDOP with scaling [1x10E-2] */
  int16_t hdop10Minus2() const { return mi16_hdop; }

  /** PDOP with scaling [1x10E-2] */
  int16_t pdop10Minus2() const { return mi16_pdop; }

  /** deliver age of last gps-position-update in milliseconds
      please use in conjunction with isPositionReceived() first,
      as else the uint16_t would wrap the result around into 0..x areas
      which would indicate you a not too old position you would use! */
  uint16_t getGpsPositionUpdateAge( void ) const
  { if ( mi32_lastIsoPositionStream > mi32_lastIsoPositionSimple) return (System_c::getTime() - mi32_lastIsoPositionStream);
    else                                                        return (System_c::getTime() - mi32_lastIsoPositionSimple);}
#else
  /** deliver age of last gps-position-update in milliseconds
      please use in conjunction with isPositionReceived() first,
      as else the uint16_t would wrap the result around into 0..x areas
      which would indicate you a not too old position you would use! */
  uint16_t getGpsPositionUpdateAge( void ) const
  { return (System_c::getTime() - mi32_lastIsoPositionSimple); }
#endif

  /** deliver age of last gps-direction-update in milliseconds
      please use in conjunction with isPositionReceived() first,
      as else the uint16_t would wrap the result around into 0..x areas
      which would indicate you a not too old position you would use! */
  uint16_t getGpsDirectionUpdateAge( void ) const
  { return (System_c::getTime() - mi32_lastIsoDirection); }

  /** register an event handler that gets called for any incoming PGN.
      Please look into the implementation to see for which PGNs it is
      actually called.
      Note: Double registration will be allowed, whereas deregistration
            will remove all occurances. */
  void registerMsgEventHandler (MsgEventHandler_c &arc_msgEventHandler)
  { mvec_msgEventHandlers.push_back (&arc_msgEventHandler); }

  /** deregister all event handlers matching the parameter
      @param arc_msgEventHandler Reference to an implementation of the
                                 handler class of type MsgEventHandler_c */
  void deregisterMsgEventHandler (MsgEventHandler_c &arc_msgEventHandler);

private:
  class MultiSendEventHandlerProxy_c : public MultiSendEventHandler_c {
  public:
    typedef TimePosGps_c Owner_t;

    MultiSendEventHandlerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}

    ~MultiSendEventHandlerProxy_c() {}

  private:
    void reactOnStateChange(const SendStream_c& sendStream)
    {
       mrt_owner.reactOnStateChange(sendStream);
    }

    // IsoRequestPgnHandlerProxy_c shall not be copyable. Otherwise
    // the reference to the containing object would become invalid.
    MultiSendEventHandlerProxy_c(MultiSendEventHandlerProxy_c const &);

    MultiSendEventHandlerProxy_c &operator=(MultiSendEventHandlerProxy_c const &);

    Owner_t &mrt_owner;
  }; // MultiSendEventHandlerProxy_c
  MultiSendEventHandlerProxy_c mt_multiSendEventHandler;

  void reactOnStateChange(const SendStream_c& sendStream);

private:
  // Private methods

  /** HIDDEN constructor for a TimePosGps_c object instance which can optional
    set the configuration for send/receive for base msg type NMEA 2000 GPS
    and calendar
    NEVER instantiate a variable of type TimePosGps_c within application
    only access TimePosGps_c via getTimePosGpsInstance() or getTimePosGpsInstance( int riLbsBusNr ) in case more than one BUS is used for IsoAgLib
    */
  TimePosGps_c();

  /** deliver time between now and last calendar set in [msec]
    @return msec since last calendar set
    */
  int32_t calendarSetAge() const {return (System_c::getTime() - mi32_lastCalendarSet);};

  const struct CNAMESPACE::tm* Utc2LocalTime();

  /** check if filter boxes shall be created - create only filters based
      on active local idents which has already claimed an address
      --> avoid to much Filter Boxes
    */
  virtual void checkCreateReceiveFilter( );

  /** send a ISO11783 base information PGN.
    * this is only called when sending ident is configured and it has already claimed an address
      @pre  function is only called in tractor mode
      @see  BaseCommon_c::timeEvent()
    */
  virtual void timeEventTracMode();

  /** send position rapid update message */
  void sendPositionRapidUpdate( void );

  /** process a ISO11783 base information PGN
      @pre  sender of message is existent in monitor list
      @see  CanPkgExt_c::resolveSendingInformation()
    */
  void processMsg( const CanPkg_c& arc_data );

  /** Calls all the registered handlers with the given PGN,
      so they can get the current values via the normal getters. */
  void notifyOnEvent (uint32_t aui32_pgn);

  /** send direction as detailed stream */
  void isoSendDirection( void );

#if defined(ENABLE_NMEA_2000_MULTI_PACKET)
  /** send position as detailed stream */
  void isoSendPositionStream( void );
#endif // END ENABLE_NMEA_2000_MULTI_PACKET



private:
  // Private attributes
    /** last timestamp where calendar was set
    -> use this to calculate new time
    */
  int32_t mi32_lastCalendarSet;

      /** last timestamp where calendar was set
    -> use this to calculate new time
    */
  CNAMESPACE::time_t mt_cachedLocalSeconds1970AtLastSet;

  CNAMESPACE::time_t mt_tzOffset;

  /** bit_calendar */
  struct {
    uint16_t year : 12;
    uint16_t month : 4;
    uint16_t day : 6;
    uint16_t hour : 6;
    uint16_t minute : 6;
    uint16_t second : 6;
    uint16_t msec   : 10;
    uint16_t timezoneMinuteOffset : 6;
    uint16_t timezoneHourOffsetMinus24 : 6;
  } bit_calendar;

  /** raw GPS latitude [degree] ; Lat_Min < 0 --> South */
  int32_t mi32_latitudeDegree10Minus7;

  /** raw GPS longitude [degree]; Long_Min < 0 --> West */
  int32_t mi32_longitudeDegree10Minus7;

  /** raw millisecond time of day from the GPS message */
  uint32_t mui32_timeMillisecondOfDay;
  int32_t  mi32_lastMillisecondUpdate;  // time stamp of the update of this variable
  float    mf_rapidUpdateRateFilter;
  int32_t  mi32_rapidUpdateRateMs;


  /// General
  /** last time of ISO GPS msg [msec] Position */
  int32_t mi32_lastIsoPositionSimple;

  /** last time of ISO GPS msg [msec] Direction */
  int32_t mi32_lastIsoDirection;

  /** course over ground reference */
  uint8_t mui8_courseOverGroundReference;

  /** sequence ID of direction data */
  uint8_t mui8_directionSequenceID;

  /** course over ground */
  uint16_t mui16_courseOverGroundRad10Minus4;

  /** speed over ground */
  uint16_t mui16_speedOverGroundCmSec;

/** not using anymore as we changed from PGN 130577 to 129026
  // data mode and Set/COG/Heading Ref. of Direction Data PGN 130577
  uint8_t ui8_dataModeAndHeadingReference;

  // GPS heading [1x10E-4rad]
  uint16_t ui16_headingRad10Minus4;

  // GPS speed - [cm/s]
  uint16_t ui16_speedCmSec;

  // flow direction
  uint16_t ui16_flowDirectionRad10Minus4;

  // drift speed
  uint16_t ui16_driftSpeedCmSec;
*/


  /** GNSS Method and Quality - not only in ENABLE_NMEA_2000_MULTI_PACKET as we need to know if GPS inf. is valid! */
  IsoAgLib::IsoGnssMethod_t mt_gnssMethod;

  /** GPS altitude - [cm] */
  int32_t mi32_altitudeCm;

#ifdef ENABLE_NMEA_2000_MULTI_PACKET
  /** GPS time in UTC */
  struct {
    uint16_t hour : 6;
    uint16_t minute : 6;
    uint16_t second : 6;
    uint16_t msec   : 10;
  } bit_gpsTime;

  /** last time of ISO GPS msg [msec] */
  int32_t mi32_lastIsoPositionStream;

  /** GNSS Type */
  IsoAgLib::IsoGnssType_t mt_gnssType;

  /** number of received satellites */
  uint8_t mui8_satelliteCnt;

  /** HDOP with scaling [1x10E-2] */
  int16_t mi16_hdop;

  /** PDOP with scaling [1x10E-2] */
  int16_t mi16_pdop;

  /** integrity of GPS signal */
  uint8_t mui8_integrity;

  /** sequence ID of GPS string */
  uint8_t mui8_positionSequenceID;
  int32_t mi32_geoidalSeparationCm;
  uint8_t mui8_noRefStations;
  STL_NAMESPACE::vector<uint16_t> mvec_refStationTypeAndStation;
  STL_NAMESPACE::vector<uint16_t> mvec_refStationDifferentialAge10Msec;

  /** buffer class for sending data streams */
  Nmea2000SendStreamer_c mc_nmea2000Streamer;

  /** sending success state */
  SendStream_c::sendSuccess_t mt_multiSendSuccessState;

  #endif // END of ENABLE_NMEA_2000_MULTI_PACKET
  /** ISOName of GPS data sender */
  IsoName_c mc_sendGpsISOName;

  /** isoName which act as sender of a msg: either responses on behalf of implement or commands as tractor.
      This pointer is set in config function
    */
  const IdentItem_c* mpc_identGps;
  IsoAgLib::IdentMode_t  mt_identModeGps;

  STL_NAMESPACE::vector<MsgEventHandler_c*> mvec_msgEventHandlers;

  friend TimePosGps_c &getTimePosGpsInstance( uint8_t aui8_instance );
};

  /** C-style function, to get access to the unique Base_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  TimePosGps_c &getTimePosGpsInstance( uint8_t aui8_instance = 0 );
} // END of Namespace __IsoAgLib

#endif // END of TIMEPOSGPS_C_H
