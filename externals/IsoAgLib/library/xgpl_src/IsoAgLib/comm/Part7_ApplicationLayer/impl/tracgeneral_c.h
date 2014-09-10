/*
  tracgeneral_c.h: stores, updates and delivers all base data
    informations from CanCustomer_c derived for CAN sending and
    receiving interaction; from BaseCommon_c derived for interaction
    with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TRACGENERAL_C_H
#define TRACGENERAL_C_H

#include <map>

#include <IsoAgLib/comm/Part7_ApplicationLayer/impl/basecommon_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgnhandler_c.h>

#include <ctime>

// Begin Namespace __IsoAgLib
namespace __IsoAgLib {

#define MAINTAIN_POWER_REQUEST_PGN_DISABLE_MASK        0x0001LU
#define LANGUAGE_PGN_DISABLE_MASK                      0x0002LU
#define REAR_HITCH_STATE_PGN_DISABLE_MASK              0x0004LU
#define FRONT_HITCH_STATE_PGN_DISABLE_MASK             0x0008LU

typedef struct
{
  /** last time of maintain power request [ms] */
  int32_t i32_lastMaintainPowerRequest;

  /** state whether maintenance of ECU power was requested */
  bool b_maintainEcuPower : 1;
  /** state whether maintenance of actuator power was requested */
  bool b_maintainActuatorPower : 1;

  /** indicates the transport state of an implement connected to a tractor or power unit */
  unsigned int inTransport: 2;
  /** indicates the state of an implement where it may be disconnected from a tractor or power unit */
  unsigned int inPark: 2;
  /** indicates that an implement is connected to a tractor or power unit and is ready for work */
  unsigned int inWork: 2;

} indicatedStateImpl_t;

/** stores, updates  and delivers all base data informations;
    Derive from BaseCommon_c some fundamental funktionality for all base data
    Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
    Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
    Derive from SINGLETON to create a Singleton which manages one global accessible singleton
    per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
  */
class TracGeneral_c : public BaseCommon_c
{
  MACRO_MULTITON_CONTRIBUTION();
public:
  /* ********************************************* */
  /** \name Management Functions for class TracGeneral_c  */
  /*@{*/

  /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
      @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      @return true -> configuration was successfull
   */
  virtual bool config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask = 0);

  virtual ~TracGeneral_c() {}

  bool processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t );

  /** force a request for pgn for language information */
  bool sendRequestUpdateLanguage();

  /*@}*/

  /* ******************************************* */
  /** \name Set Values for periodic send on BUS  */
  /*@{*/
  /**
  set rear hitch
  @param ab_val uint8_t value to store as position of rear hitch [%] highest bit is used to indicate active/inactive.
   */
  void setHitchRear(uint8_t ab_val)
  { mui8_hitchRear = ab_val;}

  /**
  set front hitch
  @param ab_val uint8_t value to store as position of front hitch [%] highest bit is used to indicate active/inactive.
   */
  void setHitchFront(uint8_t ab_val)
  { mui8_hitchFront = ab_val;}

  /** set front hitch draft
    * @return front hitch draft [-320.000N;322.550N]; 1N/bit
    */
  void setHitchFrontDraft(int32_t ai32_val) { mui16_frontDraft = ((ai32_val + 320000) / 10);}

  /** set rear hitch draft
    * @return rear hitch draft [-320.000N;322.550N]; 1N/bit
    */
  void setHitchRearDraft(int32_t ai32_val) { mui16_rearDraft = ((ai32_val + 320000) / 10);}

  /** set front hitch nominal link force
    * @return front nominal link force [-100%;100%]; 1 promille per bit
    */
  void setHitchFrontLowerLinkForce(int16_t ai16_val) { mui8_frontLinkForce = ((ai16_val + 1000) / 8);}

  /** set rear hitch nominal link force
    * @return rear nominal link force [-100%;100%]; 1 promille per bit
    */
  void setHitchRearLowerLinkForce(int16_t ai16_val) { mui8_rearLinkForce = ((ai16_val + 1000) / 8);}

  /**
    * set the ISO key switch state of the tractor
    * @param at_val IsoActive -> key switch ON
    */
  void setKeySwitch(IsoAgLib::IsoActiveFlag_t at_val);

  /** set the maximum power time of the tractor in [min]
    * @return maximum power time of the tractor in [min]
    */
  void setMaxPowerTime(uint8_t aui8_val) { mui8_maxPowerTime = aui8_val;}

  /** set state of implement in transport state
    * @param at_val state of implement in transport state
    */
  void setMaintainPowerForImplInTransport(IsoAgLib::IsoImplTransportFlag_t at_val) { mt_implState.inTransport = at_val; }

  /** set state of implement in park state
    * @param at_val state of implement in park state
    */
  void setMaintainPowerForImplInPark(IsoAgLib::IsoImplParkFlag_t at_val) { mt_implState.inPark = at_val;}

  /** set state of implement in work
    * @param at_val state of implement in work state
    */
  void setMaintainPowerForImplInWork(IsoAgLib::IsoImplWorkFlag_t at_val) { mt_implState.inWork = at_val;}

  /** set present limit status of the front hitch position
      @param at_val  limit status of the front hitch position
    */
  void setFrontHitchPosLimitStatus(const IsoAgLib::IsoLimitFlag_t at_val) {mt_frontHitchPosLimitStatus = at_val;}

  /** set present limit status of the rear hitch position
      @param at_val  limit status of the rear hitch position
    */
  void setRearHitchPosLimitStatus(const IsoAgLib::IsoLimitFlag_t at_val) {mt_rearHitchPosLimitStatus = at_val;}
  /*@}*/
  //Update language values.
  void updateLanguage(char aac_language[2]);
  void updateDecimalSymbol(uint8_t aui8_decimalSymbol);
  void updateDateFormat(uint8_t aui8_dateFormat);
  void updateTimeFormat(uint8_t aui8_timeFormat);
  void updateDistanceUnit(uint8_t aui8_distanceUnit);
  void updateAreaUnit(uint8_t aui8_areaUnit);
  void updateVolumeUnit(uint8_t aui8_volumeUnit);
  void updateMassUnit(uint8_t aui8_massUnit);
  void updateTemperatureUnit(uint8_t aui8_temperatureUnit);
  void updatePressureUnit(uint8_t aui8_pressureUnit);
  void updateForceUnit(uint8_t aui8_forceUnit);
  void updateUnitSystem(uint8_t aui8_unitSystem);

  /* ****************************************************** */
  /** \name Retrieve Values which are sent from other ECUs  */
  /*@{*/
  /** get rear hitch
      @return actual position of rear hitch [%]
    */
  uint8_t hitchRear() const {return mui8_hitchRear;}

  /** get front hitch
      @return actual position of front hitch [%]
    */
  uint8_t hitchFront() const {return mui8_hitchFront;}

  /** deliver front hitch draft
    * @return front hitch draft [-320.000N;322.550N]; 1N/bit
    */
  int32_t hitchFrontDraft() const { return static_cast<int32_t>( (mui16_frontDraft * 10) - 320000 );}

  /** deliver rear hitch draft
    * @return rear hitch draft [-320.000N;322.550N]; 1N/bit
    */
  int32_t hitchRearDraft() const { return static_cast<int32_t>( (mui16_rearDraft * 10) - 320000 );}

  /** deliver front hitch nominal link force
    * @return front nominal link force [-100%;100%]; 1 promille per bit
    */
  int16_t hitchFrontLowerLinkForce() const { return ((static_cast<int16_t>(mui8_frontLinkForce) * 8) - 1000);}

  /** deliver rear hitch nominal link force
    * @return rear nominal link force [-100%;100%]; 1 promille per bit
    */
  int16_t hitchRearLowerLinkForce() const { return ((static_cast<int16_t>(mui8_rearLinkForce) * 8) - 1000);}

  /** deliver the ISO key switch state of the tractor
    * @return IsoActive -> key switch ON
    */
  IsoAgLib::IsoActiveFlag_t keySwitch() const { return mt_keySwitch;}

  /** deliver the maximum power time of the tractor in [min]
    * @return maximum power time of the tractor in [min]
    */
  uint8_t maxPowerTime() const { return mui8_maxPowerTime;}

  /** deliver last receive time of maintain power request
    * @return time in [ms] since system start -> comparable to system time
    */
  int32_t lastMaintainPowerRequest() const { return mi32_lastMaintainPowerRequest;}

  /** check if timeout reached since last receive time of maintain power request */
  bool timeOutMaintainPowerRequest() const;

  /** get present limit status of the front hitch position
      @return  limit status of front hitch position
    */
  IsoAgLib::IsoLimitFlag_t frontHitchPosLimitStatus()const {return mt_frontHitchPosLimitStatus;}

  /** get present limit status of the rear hitch position
      @return  limit status of rear hitch position
    */
  IsoAgLib::IsoLimitFlag_t rearHitchPosLimitStatus()const {return mt_rearHitchPosLimitStatus;}

  /** check whether maintenance of ECU power was requested */
  bool maintainEcuPower() const { return mb_maintainEcuPower;}

  /** check whether maintenance of actuator power was requested */
  bool maintainActuatorPower() const { return mb_maintainActuatorPower;}

  /** check whether maintenance of power
    * for implement in transport state was requested */
  IsoAgLib::IsoImplTransportFlag_t maintainPowerForImplInTransport() const { return IsoAgLib::IsoImplTransportFlag_t(mt_implState.inTransport); }

  /** check whether maintenance of power
    * for implement in park state was requested */
  IsoAgLib::IsoImplParkFlag_t maintainPowerForImplInPark() const { return IsoAgLib::IsoImplParkFlag_t(mt_implState.inPark); }

  /** check whether maintenance of power
    * for implement in work state was requested */
  IsoAgLib::IsoImplWorkFlag_t maintainPowerForImplInWork() const { return IsoAgLib::IsoImplWorkFlag_t(mt_implState.inWork); }

  bool isTecuLanguageReceived() const { return mb_languageTecuReceived; }

  const uint8_t* getTecuLanguage() const { return mp8ui8_languageTecu; }

  enum SendLanguage_e { LanguageSent, LanguageNotSent };

  /** send iso language data msg
      @see  TracGeneral_c::processMsgRequestPGN
      @see  CanIo_c::operator<<
    */
  enum SendLanguage_e sendLanguage();

  /** Client function: force maintain power from tractor (sends once per call)
      @param at_ecuPower IsoActive -> maintain ECU power
      @param at_actuatorPower IsoActive-> maintain actuator power
    */
  void forceMaintainPower( IsoAgLib::IsoActiveFlag_t at_ecuPower, IsoAgLib::IsoActiveFlag_t at_actuatorPower );
  /*@}*/

private:
  // Private methods
  /** HIDDEN constructor for a TracGeneral_c object instance which can optional
      set the configuration for send/receive for general base msg
      NEVER instantiate a variable of type TracGeneral_c within application
      only access TracGeneral_c via getTracGeneralInstance() or getTracGeneralInstance( int riLbsBusNr )
      in case more than one BUS is used for IsoAgLib
    */
  TracGeneral_c();

  /** check if filter boxes shall be created - create only filters based
      on active local idents which has already claimed an address
      --> avoid to much Filter Boxes
    */
  virtual void checkCreateReceiveFilter();

  /** send a ISO11783 general base information PGN.
    * this is only called when sending ident is configured and it has already claimed an address
      @pre  function is only called in tractor mode
      @see  BaseCommon_c::timeEvent()
    */
  virtual void timeEventTracMode();

  /** process a ISO11783 base information PGN
      @pre  sender of message is existent in monitor list
      @see  CanPkgExt_c::resolveSendingInformation()
    */
  void processMsg( const CanPkg_c& arc_data );

  /** send front hitch and rear hitch data msg
      @see  CanIo_c::operator<<
    */
  void sendMessage();

  /** update map of maintain power data, update maintain power boolean */
  void updateMaintainPowerRequest();

  bool canSendFrontHitchState();

  enum SendHitchState_e { HitchStateSent, HitchStateNotSent };

  /** Send front hitch date message. */
  SendHitchState_e sendFrontHitchState();

  bool canSendRearHitchState();

  /** Send rear hitch date message. */
  SendHitchState_e sendRearHitchState();

  /** Prepare sending any hitch date message. */
  SendHitchState_e prepareSendingHitchState( CanPkgExt_c& pkg );

  bool canSendLanguage();

private:
  // Private attributes
  /** engine speed */
  int16_t mi16_engine;

  /** front hitch data */
  uint8_t mui8_hitchFront;

  /** rear hitch data */
  uint8_t mui8_hitchRear;


  /** TECU language information */
  uint8_t mp8ui8_languageTecu[8];

  /** TECU language reception information */
  bool mb_languageTecuReceived;


  /// General
  /** last time of ISO GPS msg [msec] */
  int32_t mi32_lastIsoPositionSimple;

  /** key switch state */
  IsoAgLib::IsoActiveFlag_t mt_keySwitch;

  /** maximum time of tractor power in [min] */
  uint8_t mui8_maxPowerTime;

  /** front hitch draft [-320.000;322.550N], res: 10N/bit, offset: -320.000N  */
  uint16_t mui16_frontDraft;

  /** rear hitch draft [-320.000;322.550N], res: 10N/bit, offset: -320.000N  */
  uint16_t mui16_rearDraft;

  /** front nominal lower link force [-100%;100%], res: 0.8%/bit, offset: -100% */
  uint8_t mui8_frontLinkForce;

  /** rear nominal lower link force [-100%;100%], res: 0.8%/bit, offset: -100% */
  uint8_t mui8_rearLinkForce;

  /** set reported the tractor ECU's present limit status of the front hitch position */
  IsoAgLib::IsoLimitFlag_t mt_frontHitchPosLimitStatus;

  /** set reported the tractor ECU's present limit status of the rear hitch position */
  IsoAgLib::IsoLimitFlag_t mt_rearHitchPosLimitStatus;

  /** last time of maintain power request [ms] */
  int32_t mi32_lastMaintainPowerRequest;

  /** state whether maintenance of ECU power was requested */
  bool mb_maintainEcuPower;

  /** state whether maintenance of actuator power was requested */
  bool mb_maintainActuatorPower;

  /** stores for each requesting implement (sourceAddress) a bit field with all indicated state*/
  STL_NAMESPACE::map< IsoName_c, indicatedStateImpl_t > mmap_indicatedState; // access mmap_data[isoName].

  /** bit field with indicated state for tractor*/
  indicatedStateImpl_t mt_implState;

  /** should a new language pgn changed command be sent? */
  bool mb_languagePgnChanged;
  friend TracGeneral_c &getTracGeneralInstance(uint8_t aui8_instance);
};

inline bool TracGeneral_c::canSendFrontHitchState() {
  return 0 == (FRONT_HITCH_STATE_PGN_DISABLE_MASK & mui16_suppressMask);
}

inline bool TracGeneral_c::canSendRearHitchState() {
  return 0 == (REAR_HITCH_STATE_PGN_DISABLE_MASK & mui16_suppressMask);
}

inline bool TracGeneral_c::canSendLanguage() {
  return 0 == (LANGUAGE_PGN_DISABLE_MASK & mui16_suppressMask);
}

/** C-style function, to get access to the unique TracGeneral_c singleton instance
 * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
 */
TracGeneral_c &getTracGeneralInstance(uint8_t aui8_instance = 0);
} // End Namespace __IsoAgLib
#endif
