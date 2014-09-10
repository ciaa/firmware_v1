/*
  itracguidancecommand_c.h: working on guidance command data; stores,
    updates and delivers all guidance data informations from
    CanCustomer_c derived for CAN sending and receiving interaction;
    from SchedulerTask_c derived for interaction with other IsoAgLib
    objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISOAGLIBITRACGUIDANCECOMMAND_C_H
#define ISOAGLIBITRACGUIDANCECOMMAND_C_H

#include <IsoAgLib/comm/Part5_NetworkManagement/iidentitem_c.h>
#include "ibasetypes.h"
#include "impl/tracguidancecommand_c.h"

namespace IsoAgLib {

/** stores, updates  and delivers guidance command data information;
    Derive from BaseCommon_c some fundamental funktionality for all base data
    Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
    Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
    Derive from SINGLETON to create a Singleton which manages one global accessible singleton
    per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
  */
class iTracGuidanceCommand_c :
private __IsoAgLib::TracGuidanceCommand_c {
 public:
  /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
      @param at_identMode set mode to either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      @return true -> configuration was successfull
    */
  bool config ( const iIdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode = IsoAgLib::IdentModeImplement, uint16_t aui16_suppressMask = 0 )
  { return TracGuidanceCommand_c::config_base ( apc_ident, at_identMode, aui16_suppressMask ); }

  /* ******************************************* */
  /** \name Set Values for periodic send on BUS  */
  /*@{*/
  /** set desired course curvature over ground that a machine's steering system is required to achieve
      @param  i16_val  desired course curvature over ground
    */
  void setCurvatureCmd(int16_t i16_val) {TracGuidanceCommand_c::setCurvatureCmd( i16_val );}

  /** set whether the guidance system is attempting to control steering with this command
      @param  t_val  is guidance system attempting to control steering
    */
  void setCurvatureCmdStatus(IsoAgLib::IsoSteerFlag_t t_val) {TracGuidanceCommand_c::setCurvatureCmdStatus( t_val );}

  /** get currently selected curvature commander source address
      @param  ui8_sourceAddress  source address of curvature commander
      */
  void setCommander(uint8_t ui8_sourceAddress) {TracGuidanceCommand_c::setCommander( ui8_sourceAddress );}
  /*@}*/

  /* ****************************************************** */
  /** \name Retrieve Values which are sent from other ECUs  */
  /*@{*/
  /** set desired course curvature over ground that a machine's steering system is required to achieve
      @return  desired course curvature over ground
    */
  int16_t curvatureCmd() {return TracGuidanceCommand_c::curvatureCmd();}

  /** set whether the guidance system is attempting to control steering with this command
      @return  is guidance system attempting to control steering
    */
  IsoAgLib::IsoSteerFlag_t curvatureCmdStatus() {return TracGuidanceCommand_c::curvatureCmdStatus();}

  /** get currently selected curvature commander source address
      @return  source address of curvature commander
      */
  uint8_t getCommander() {return TracGuidanceCommand_c::getCommander();}
  /*@}*/

 private:
  /** allow getITracGuidanceInstance() access to shielded traccert class.
    otherwise __IsoAgLib::getTracGuidanceInstance() wouldn't be accepted by compiler
  */
  #if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
  friend iTracGuidanceCommand_c& getITracGuidanceCommandInstance(uint8_t aui8_instance);
  #else
  friend iTracGuidanceCommand_c& getITracGuidanceCommandInstance(void);
  #endif

};

#if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
/** C-style function, to get access to the unique iTracGuidance_c singleton instance
  * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
  */
inline iTracGuidanceCommand_c& getITracGuidanceCommandInstance(uint8_t aui8_instance = 0)
{ return static_cast<iTracGuidanceCommand_c&>(__IsoAgLib::getTracGuidanceCommandInstance(aui8_instance));}
#else
/** C-style function, to get access to the unique iTracGuidance_c singleton instance */
inline iTracGuidanceCommand_c& getITracGuidanceCommandInstance(void)
{ return static_cast<iTracGuidanceCommand_c&>(__IsoAgLib::getTracGuidanceCommandInstance());}
#endif

} // end of namespace IsoAgLib

#endif
