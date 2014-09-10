/*
  itracguidance_c.h: working on guidance data; stores, updates and
    delivers all guidance data informations from CanCustomer_c derived
    for CAN sending and receiving interaction; from SchedulerTask_c
    derived for interaction with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ITRACGUIDANCE_C_H
#define ITRACGUIDANCE_C_H

/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */
#include <IsoAgLib/comm/Part5_NetworkManagement/iidentitem_c.h>
#include "ibasetypes.h"
#include "impl/tracguidance_c.h"


// Begin Namespace IsoAgLib
namespace IsoAgLib {

  /** stores, updates  and delivers certification data information;
      Derive from BaseCommon_c some fundamental funktionality for all base data
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */
  class iTracGuidance_c :
  private __IsoAgLib::TracGuidance_c {
    public:

  /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
      @param at_identMode set mode to either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      @return true -> configuration was successfull
    */
  bool config ( const iIdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode = IsoAgLib::IdentModeImplement, uint16_t aui16_suppressMask = 0)
  { return TracGuidance_c::config_base ( apc_ident, at_identMode, aui16_suppressMask ); }

  /* ******************************************* */
  /** \name Set Values for periodic send on BUS  */
  /*@{*/
  /** set machine steering system's estimate of the curvature of the current turn
      @param i16_val  estimated value of curvature of current turn
    */
  void setEstCurvature(const int16_t i16_val) {TracGuidance_c::setEstCurvature(i16_val);}
  /** set machine steering system request to the automatic guidance system to change curvature command status
      @param t_val  value of curvature command status
    */
  void setRequestResetCmdStatus(const IsoAgLib::IsoResetFlag_t t_val) {TracGuidance_c::setRequestResetCmdStatus(t_val);}
  /** set machine steering system indication if manual steering is in correct position for guidance to function
      @param t_val  state of manual steering position
    */
  void setSteeringInputPosStatus(const IsoAgLib::IsoSteerPosFlag_t t_val) {TracGuidance_c::setSteeringInputPosStatus(t_val);}
  /** set machine steering system indication that it is free from faults that would preclude guidance system operation
      @param t_val  status of machine steering system
    */
  void setSteeringSystemReadiness( const IsoAgLib::IsoSteerReadinessFlag_t t_val) {TracGuidance_c::setSteeringSystemReadiness(t_val);}
  /** set state of lockout switch that allows operators to disable automatic steering system functions
      @param t_val  status of lockout switch
    */
  void setMechanicalSystemLogout(const IsoAgLib::IsoActiveFlag_t t_val) {TracGuidance_c::setMechanicalSystemLogout(t_val);}
  /*@}*/

  /* ****************************************************** */
  /** \name Retrieve Values which are sent from other ECUs  */
  /*@{*/
  /** get machine steering system's estimate of the curvature of the current turn
      @return  estimated value of curvature of current turn
    */
  int16_t estCurvature() const { return TracGuidance_c::estCurvature();}
  /** get machine steering system request to the automatic guidance system to change curvature command status
      @return  request reset command status
    */
  IsoAgLib::IsoResetFlag_t requestResetCmdStatus() const { return TracGuidance_c::requestResetCmdStatus();}
  /** get machine steering system indication if manual steering is in correct position for guidance to function
      @return  state of manual steering position
    */
  IsoAgLib::IsoSteerPosFlag_t steeringInputPosStatus() const { return TracGuidance_c::steeringInputPosStatus();}
  /** get machine steering system indication that it is free from faults that would preclude guidance system operation
      @return  status of machine steering system
    */
  IsoAgLib::IsoSteerReadinessFlag_t steeringSystemReadiness() const { return TracGuidance_c::steeringSystemReadiness();}
  /** get state of lockout switch that allows operators to disable automatic steering system functions
      @return  status of lockout switch
    */
  IsoAgLib::IsoActiveFlag_t mechanicalSystemLogout() const { return TracGuidance_c::mechanicalSystemLogout();}
  /*@}*/
  private:
    /** allow getITracGuidanceInstance() access to shielded traccert class.
      otherwise __IsoAgLib::getTracGuidanceInstance() wouldn't be accepted by compiler
    */
    #if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
    friend iTracGuidance_c& getITracGuidanceInstance(uint8_t aui8_instance);
    #else
    friend iTracGuidance_c& getITracGuidanceInstance(void);
    #endif

  };

  #if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
  /** C-style function, to get access to the unique iTracGuidance_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  inline iTracGuidance_c& getITracGuidanceInstance(uint8_t aui8_instance = 0)
  { return static_cast<iTracGuidance_c&>(__IsoAgLib::getTracGuidanceInstance(aui8_instance));}
  #else
  /** C-style function, to get access to the unique iTracGuidance_c singleton instance */
  inline iTracGuidance_c& getITracGuidanceInstance(void)
  { return static_cast<iTracGuidance_c&>(__IsoAgLib::getTracGuidanceInstance());}
  #endif
}
#endif

