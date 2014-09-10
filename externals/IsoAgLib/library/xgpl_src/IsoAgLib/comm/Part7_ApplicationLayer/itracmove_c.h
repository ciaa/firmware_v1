/*
  itracmove_c.h: stores, updates and delivers all moving data
    information from CanCustomer_c derived for CAN sending and
    receiving interaction; from SchedulerTask_c derived for
    interaction with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ITRACMOVE_C_H
#define ITRACMOVE_C_H

/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */
#include <IsoAgLib/comm/Part5_NetworkManagement/iidentitem_c.h>
#include "ibasetypes.h"
#include "impl/tracmove_c.h"

// Begin Namespace IsoAgLib
namespace IsoAgLib {

  /** stores, updates  and delivers all moving data information;
      Derive from BaseCommon_c some fundamental funktionality for all base data
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */
  class iTracMove_c : private __IsoAgLib::TracMove_c {
  public:
  /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
      @param at_identMode set mode to either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      @return true -> configuration was successfull
    */
  bool config ( const iIdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode = IsoAgLib::IdentModeImplement, uint16_t aui16_suppressMask = 0)
  { return TracMove_c::config_base( apc_ident, at_identMode, aui16_suppressMask ); }

#if 0
  /** update selected speed with actually best available speed
      @param at_speedSource  from which source is the speed available
    */
  void updateSpeed(IsoAgLib::SpeedSource_t at_speedSource) {TracMove_c::updateSpeed(at_speedSource);}
#endif

  /** update distance and direction with the actually best available distance and direction
      @param t_distanceSrc  from which source is the distance and direction available
    */
  void updateDistanceDirection(IsoAgLib::DistanceDirectionSource_t t_distanceSrc) {TracMove_c::updateDistanceDirection(t_distanceSrc);}

  /* ******************************************* */
  /** \name Set Values for periodic send on BUS  */
  /*@{*/
  /** set the value of real speed (measured by radar)
      @param ai32_val value to store as real radar measured speed
    */
  void setSpeedReal(const int32_t& ai32_val)
  {
    TracMove_c::setSpeedReal(ai32_val);
  }
  /** set the value of theoretical speed (calculated from gear)
      @param ai32_val value to store as theoretical gear calculated speed
    */
  void setSpeedTheor(const int32_t& ai32_val)
  {
    TracMove_c::setSpeedTheor(ai32_val);
  }
  /** set the real (radar measured) driven distance with int32_t val
      @param rreflVal value to store as real radar measured distance
    */
  void setDistReal(const uint32_t& rreflVal) {TracMove_c::setDistReal(rreflVal);}
  /** set the theoretical (gear calculated) driven distance with int32_t val
      @param rreflVal value to store as theoretical (gear calculated) driven distance
    */
  void setDistTheor(const uint32_t& rreflVal) {TracMove_c::setDistTheor(rreflVal);}

  /** set measured signal indicating either forward or reverse as the theoretical (gear calculated) direction of travel
      @return  direction of travel
    */
  void setDirectionTheor(IsoAgLib::IsoDirectionFlag_t t_val) {TracMove_c::setDirectionTheor(t_val);}

  /** set measured signal indicating either forward or reverse as the real (radar measured) direction of travel
      @return  direction of travel
    */
  void setDirectionReal(IsoAgLib::IsoDirectionFlag_t t_val) {TracMove_c::setDirectionReal(t_val);}

  /** set parameter which indicates whetcher the reported direction is reversed from the perspective of the operator
      @param at_val  indicates direction (IsoInactive = not reversed; IsoActive = reversed)
    */
  void setOperatorDirectionReversed(const IsoAgLib::IsoOperatorDirectionFlag_t at_val)
  { TracMove_c::setOperatorDirectionReversed( at_val );}

  /** set actual distance traveled by the machine based on the value of selected machine speed
      @param ui32_val  actual distance traveled
    */
  void setSelectedDistance(const uint32_t& ui32_val) {TracMove_c::setSelectedDistance(ui32_val);}

  /** set indicated current direction of travel of the machine
      @param t_val  current direction of travel
    */
  void setSelectedDirection(const IsoAgLib::IsoDirectionFlag_t t_val) {TracMove_c::setSelectedDirection(t_val);}

  /** set current value of the speed as determined from a number of sources by the machine
      @param i32_val  current value of speed
    */
  void setSelectedSpeed(const int32_t& i32_val) {TracMove_c::setSelectedSpeed(i32_val);}

  /** set present limit status of selected speed
      @param t_val  limit status
    */
  void setSelectedSpeedLimitStatus(const IsoAgLib::IsoLimitFlag_t t_val)
  {TracMove_c::setSelectedSpeedLimitStatus(t_val);}

  /** set indicated speed source that is currently being reported in the machine speed parameter
      @param t_val  speed source that is currently being reported
    */
  void setSelectedSpeedSource(const IsoAgLib::IsoSpeedSourceFlag_t t_val)
  {TracMove_c::setSelectedSpeedSource(t_val);}
  /*@}*/

  /**
    * Has to be set as has to be sent to bus -> 1 bit is 0.125 rpm
    */
  void setEngineSpeed(const uint16_t aui16_engineSpeed) { TracMove_c::setEngineSpeed(aui16_engineSpeed); }

  /* ****************************************************** */
  /** \name Retrieve Values which are sent from other ECUs  */
  /*@{*/
  /** get the value of real speed (measured by radar)
      @return actual radar measured speed value
    */
  int32_t speedReal() const {return TracMove_c::speedReal();}

  /** is looking for a valid speed value
      @return true if speed is valid otherwise false */
  bool isRealSpeedUsable() const { return TracMove_c::isRealSpeedUsable();}

  /** is looking for a missing speed value
      @return true if speed is missing otherwise false */
  bool isRealSpeedMissing() const { return  TracMove_c::isRealSpeedMissing();}

  /** is looking for a erroneous speed value
      @return true if speed is erroneous otherwise false */
  bool isRealSpeedErroneous() const { return TracMove_c::isRealSpeedErroneous();}

  /** get the value of theoretical speed (calculated from gear)
      @return theoretical gear calculated speed value
    */
  int32_t speedTheor() const {return TracMove_c::speedTheor();}

  /** is looking for a valid speed value
      @return true if speed is valid otherwise false */
  bool isTheorSpeedUsable() const { return TracMove_c::isTheorSpeedUsable();}

  /** is looking for a missing speed value
      @return true if speed is missing otherwise false */
  bool isTheorSpeedMissing() const { return  TracMove_c::isTheorSpeedMissing();}

  /** is looking for a erroneous speed value
      @return true if speed is erroneous otherwise false */
  bool isTheorSpeedErroneous() const { return TracMove_c::isTheorSpeedErroneous();}

  /** get the real driven distance with int32_t val
      @return actual radar measured driven distance value
    */
  uint32_t distReal() const {return TracMove_c::distReal();}

  /** get the real driven distance with int32_t val
      @return actual gear calculated driven distance value
    */
  uint32_t distTheor() const {return TracMove_c::distTheor();}

  /** get measured signal indicating either forward or reverse as the theoretical (gear calculated) direction of travel
      @return  direction of travel
    */
  IsoAgLib::IsoDirectionFlag_t directionTheor() {return TracMove_c::directionTheor();}

  /** get measured signal indicating either forward or reverse as the real (radar measured) direction of travel
      @return  direction of travel
    */
  IsoAgLib::IsoDirectionFlag_t directionReal() {return TracMove_c::directionReal();}

  /** get parameter which indicates whetcher the reported direction is reversed from the perspective of the operator
      @return indicates direction (IsoInactive = not reversed; IsoActive = reversed)
    */
  IsoAgLib::IsoOperatorDirectionFlag_t operatorDirectionReversed()const { return TracMove_c::operatorDirectionReversed();}

  /** get actual distance traveled by the machine based on the value of selected machine speed
      @return  actual distance traveled
    */
  uint32_t selectedDistance() const {return TracMove_c::selectedDistance();}

  /** get current direction of travel of the machine
      @return  current direction of travel
    */
  IsoAgLib::IsoDirectionFlag_t selectedDirection() const {return TracMove_c::selectedDirection();}

  /** get current value of the speed as determined from a number of sources by the machine
      @return  current value of speed
    */
  int32_t selectedSpeed() { return TracMove_c::selectedSpeed();}

  /** is looking for a valid speed value
      @return true if speed is valid otherwise false */
  bool isSelectedSpeedUsable() const { return TracMove_c::isSelectedSpeedUsable();}

  /** is looking for a missing speed value
      @return true if speed is missing otherwise false */
  bool isSelectedSpeedMissing() const { return  TracMove_c::isSelectedSpeedMissing();}

  /** is looking for a erroneous speed value
      @return true if speed is erroneous otherwise false */
  bool isSelectedSpeedErroneous() const { return TracMove_c::isSelectedSpeedErroneous();}

  /** get present limit status of selected speed
      @return  limit status
    */
  IsoAgLib::IsoLimitFlag_t selectedSpeedLimitStatus() const {return TracMove_c::selectedSpeedLimitStatus();}

  /** get speed source that is currently being reported in the machine speed parameter
      @return  speed source that is currently being reported
    */
  IsoAgLib::IsoSpeedSourceFlag_t selectedSpeedSource() const {return TracMove_c::selectedSpeedSource();}

  /** get the value of engine speed
      @return engine speed -> 1 bit is 0.125 rpm
    */
  uint16_t engineSpeed() const { return TracMove_c::engineSpeed();}

  private:
    /** allow getITracMoveInstance() access to shielded tracmove class.
      otherwise __IsoAgLib::getTracMoveInstance() wouldn't be accepted by compiler
    */
    #if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
    friend iTracMove_c& getITracMoveInstance(uint8_t aui8_instance);
    #else
    friend iTracMove_c& getITracMoveInstance(void);
    #endif

  };

  #if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
  /** C-style function, to get access to the unique TracMove_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  inline iTracMove_c& getITracMoveInstance(uint8_t aui8_instance = 0)
  { return static_cast<iTracMove_c&>(__IsoAgLib::getTracMoveInstance(aui8_instance));}
  #else
  /** C-style function, to get access to the unique TracMove_c singleton instance */
  inline iTracMove_c& getITracMoveInstance(void)
  { return static_cast<iTracMove_c&>(__IsoAgLib::getTracMoveInstance());}
  #endif
}
#endif
