/*
  itracgeneral_c.h: working on Base Data Msg Type NMEA 200 GPS and
    Calendar; stores, updates and delivers all base data informations
    from CanCustomer_c derived for CAN sending and receiving
    interaction; from SchedulerTask_c derived for interaction with
    other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ITRACGENERAL_C_H
#define ITRACGENERAL_C_H

/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */
#include <IsoAgLib/comm/Part5_NetworkManagement/iidentitem_c.h>
#include "ibasetypes.h"
#include "impl/tracgeneral_c.h"

// Begin Namespace IsoAgLib
namespace IsoAgLib {

  /** stores, updates  and delivers all base data informations;
    Derive from BaseCommon_c some fundamental funktionality for all base data
    Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
    Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
    Derive from SINGLETON to create a Singleton which manages one global accessible singleton
    per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
  */
  class iTracGeneral_c : private __IsoAgLib::TracGeneral_c {
  public:

  /**
      @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
      @param at_identMode set mode to either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      @return true -> configuration was successfull
    */
  bool config ( const iIdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode = IsoAgLib::IdentModeImplement, uint16_t aui16_suppressMask = 0)
  { return TracGeneral_c::config_base( apc_ident, at_identMode, aui16_suppressMask ); }


  /* ******************************************* */
  /** \name Set Values for periodic send on BUS  */
  /*@{*/
  /** set rear hitch */
  void setHitchRear(uint8_t ab_val) { TracGeneral_c::setHitchRear(ab_val);}

  /** set front hitch */
  void setHitchFront(uint8_t ab_val) { TracGeneral_c::setHitchFront(ab_val);}

  /** set front hitch draft */
  void setHitchFrontDraft(int32_t ai32_val) { TracGeneral_c::setHitchFrontDraft(ai32_val);}

  /** set rear hitch draft */
  void setHitchRearDraft(int32_t ai32_val) { TracGeneral_c::setHitchRearDraft(ai32_val);}

  /** set front hitch nominal link force */
  void setHitchFrontLowerLinkForce(int16_t ai16_val) { TracGeneral_c::setHitchFrontLowerLinkForce(ai16_val);}

  /** set rear hitch nominal link force */
  void setHitchRearLowerLinkForce(int16_t ai16_val) { TracGeneral_c::setHitchRearLowerLinkForce(ai16_val);}

  /** set the ISO key switch state of the tractor */
  void setKeySwitch(IsoAgLib::IsoActiveFlag_t at_val) { TracGeneral_c::setKeySwitch(at_val);}

  /** set the maximum power time of the tractor in [min] */
  void setMaxPowerTime(uint8_t aui8_val) { TracGeneral_c::setMaxPowerTime(aui8_val);}

  /** force maintain power from tractor */
  void forceMaintainPower(IsoActiveFlag_t at_ecuPower, IsoActiveFlag_t at_actuatorPower)
  { TracGeneral_c::forceMaintainPower(at_ecuPower, at_actuatorPower);}

  /** set present limit status of the front hitch position
        @param at_val  limit status of the front hitch position
      */
  void setFrontHitchPosLimitStatus(const IsoAgLib::IsoLimitFlag_t at_val) {TracGeneral_c::setFrontHitchPosLimitStatus(at_val);}

  /** set present limit status of the rear hitch position
      @param at_val  limit status of the rear hitch position
    */
  void setRearHitchPosLimitStatus(const IsoAgLib::IsoLimitFlag_t at_val) {TracGeneral_c::setRearHitchPosLimitStatus(at_val);}

  /** set state of implement in transport state
      * @param at_val state of implement in transport state
      */
  void setMaintainPowerForImplInTransport(IsoAgLib::IsoImplTransportFlag_t at_val) { TracGeneral_c::setMaintainPowerForImplInTransport(at_val); }

    /** set state of implement in park state
      * @param at_val state of implement in park state
      */
  void setMaintainPowerForImplInPark(IsoAgLib::IsoImplParkFlag_t at_val) { TracGeneral_c::setMaintainPowerForImplInPark(at_val);}

    /** set state of implement in work
      * @param at_val state of implement in work state
      */
  void setMaintainPowerForImplInWork(IsoAgLib::IsoImplWorkFlag_t at_val) { TracGeneral_c::setMaintainPowerForImplInWork(at_val);}
  /*@}*/
  //Update language values.
  void updateLanguage(char aac_language[2]) { TracGeneral_c::updateLanguage(aac_language);}
  void updateDecimalSymbol(uint8_t aui8_decimalSymbol) { TracGeneral_c::updateDecimalSymbol(aui8_decimalSymbol);}
  void updateDateFormat(uint8_t aui8_dateFormat) { TracGeneral_c::updateDateFormat(aui8_dateFormat);}
  void updateTimeFormat(uint8_t aui8_timeFormat) { TracGeneral_c::updateTimeFormat(aui8_timeFormat);}
  void updateDistanceUnit(uint8_t aui8_distanceUnit) { TracGeneral_c::updateDistanceUnit(aui8_distanceUnit);}
  void updateAreaUnit(uint8_t aui8_areaUnit) { TracGeneral_c::updateAreaUnit(aui8_areaUnit);}
  void updateVolumeUnit(uint8_t aui8_volumeUnit) { TracGeneral_c::updateVolumeUnit(aui8_volumeUnit);}
  void updateMassUnit(uint8_t aui8_massUnit) { TracGeneral_c::updateMassUnit(aui8_massUnit);}
  void updateTemperatureUnit(uint8_t aui8_temperatureUnit) { TracGeneral_c::updateTemperatureUnit(aui8_temperatureUnit);}
  void updatePressureUnit(uint8_t aui8_pressureUnit) { TracGeneral_c::updatePressureUnit(aui8_pressureUnit);}
  void updateForceUnit(uint8_t aui8_forceUnit) { TracGeneral_c::updateForceUnit(aui8_forceUnit);}
  void updateUnitSystem(uint8_t aui8_unitSystem) { TracGeneral_c::updateUnitSystem(aui8_unitSystem);}

  /* ****************************************************** */
  /** \name Retrieve Values which are sent from other ECUs  */
  /*@{*/
    /** get rear hitch */
  uint8_t hitchRear() const { return TracGeneral_c::hitchRear();}

  /** get front hitch */
  uint8_t hitchFront() const { return TracGeneral_c::hitchFront();}

  /** deliver front hitch draft */
  int32_t hitchFrontDraft() const { return TracGeneral_c::hitchFrontDraft();}

  /** deliver rear hitch draft */
  int32_t hitchRearDraft() const { return TracGeneral_c::hitchRearDraft();}

  /** deliver front hitch nominal link force */
  int16_t hitchFrontLowerLinkForce() const { return TracGeneral_c::hitchFrontLowerLinkForce();}

  /** deliver rear hitch nominal link force */
  int16_t hitchRearLowerLinkForce() const { return TracGeneral_c::hitchRearLowerLinkForce();}

  /** deliver the ISO key switch state of the tractor */
  IsoAgLib::IsoActiveFlag_t keySwitch() const { return TracGeneral_c:: keySwitch();}

  /** deliver the maximum power time of the tractor in [min] */
  uint8_t maxPowerTime() const { return TracGeneral_c::maxPowerTime();}

  /** deliver last receive time of maintain power request */
  int32_t lastMaintainPowerRequest() const { return TracGeneral_c::lastMaintainPowerRequest();}

  /** check if timeout reached since last receive time of maintain power request */
  bool timeOutMaintainPowerRequest() const { return TracGeneral_c::timeOutMaintainPowerRequest();}

  /** check whether maintenance of ECU power was requested */
  bool maintainEcuPower() const { return TracGeneral_c::maintainEcuPower();}

  /** check whether maintenance of actuator power was requested */
  bool maintainActuatorPower() const { return TracGeneral_c::maintainActuatorPower();}

  /** check whether maintenance of power
    * for implement in transport state was requested */
  IsoAgLib::IsoImplTransportFlag_t maintainPowerForImplInTransport() const { return TracGeneral_c::maintainPowerForImplInTransport();}

  /** check whether maintenance of power
    * for implement in park state was requested */
  IsoAgLib::IsoImplParkFlag_t maintainPowerForImplInPark() const { return TracGeneral_c::maintainPowerForImplInPark();}

  /** check whether maintenance of power
    * for implement in work state was requested */
  IsoAgLib::IsoImplWorkFlag_t maintainPowerForImplInWork() const { return TracGeneral_c::maintainPowerForImplInWork();}

  /** get present limit status of the front hitch position
    @return  limit status of front hitch position
  */
  IsoAgLib::IsoLimitFlag_t frontHitchPosLimitStatus()const {return TracGeneral_c::frontHitchPosLimitStatus();}

  /** get present limit status of the rear hitch position
      @return  limit status of rear hitch position
    */
  IsoAgLib::IsoLimitFlag_t rearHitchPosLimitStatus()const {return TracGeneral_c::rearHitchPosLimitStatus();}

  bool isTecuLanguageReceived() const { return TracGeneral_c::isTecuLanguageReceived();}

  const uint8_t* getTecuLanguage() const { return TracGeneral_c::getTecuLanguage();}

  /** send iso language data msg*/
  void sendLanguage() { (void)TracGeneral_c::sendLanguage();}

  /** force a request for pgn for language information */
  bool sendRequestUpdateLanguage() { return TracGeneral_c::sendRequestUpdateLanguage(); }
  /*@}*/

  private:
    /** allow getITracGeneralInstance() access to shielded tracgeneral class.
      otherwise __IsoAgLib::getTracGeneralInstance() wouldn't be accepted by compiler
    */
    #if defined( PRT_INSTANCE_CNT ) && ( PRT_INSTANCE_CNT > 1 )
    friend iTracGeneral_c& getITracGeneralInstance( uint8_t aui8_instance );
    #else
    friend iTracGeneral_c& getITracGeneralInstance( void );
    #endif

  };
  #if defined( PRT_INSTANCE_CNT ) && ( PRT_INSTANCE_CNT > 1 )
  /** C-style function, to get access to the unique Base_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  inline iTracGeneral_c& getITracGeneralInstance( uint8_t aui8_instance = 0 )
  { return static_cast<iTracGeneral_c&>(__IsoAgLib::getTracGeneralInstance(aui8_instance));}
  #else
  /** C-style function, to get access to the unique Base_c singleton instance */
  inline iTracGeneral_c& getITracGeneralInstance( void )
  { return static_cast<iTracGeneral_c&>(__IsoAgLib::getTracGeneralInstance());}
  #endif
}
#endif
