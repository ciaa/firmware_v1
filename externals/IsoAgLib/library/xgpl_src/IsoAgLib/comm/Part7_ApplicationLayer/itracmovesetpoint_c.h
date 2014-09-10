/*
  itracmove_c.h: working on Base Data Msg Type NMEA 200 GPS and
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
#ifndef ITRACMOVESETPOINT_C_H
#define ITRACMOVESETPOINT_C_H

/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */
#include <IsoAgLib/comm/Part5_NetworkManagement/iidentitem_c.h>
#include "ibasetypes.h"
#include "impl/tracmovesetpoint_c.h"


// Begin Namespace IsoAgLib
namespace IsoAgLib {

  /** working on Base Data Msg Type 1;
      stores, updates  and delivers all base data informations;
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */
  class iTracMoveSetPoint_c : private __IsoAgLib::TracMoveSetPoint_c {
  public:

  /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
      @param at_identMode set mode to either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      @return true -> configuration was successfull
    */
    bool config ( const iIdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode = IsoAgLib::IdentModeImplement, uint16_t aui16_suppressMask = 0 )
    { return TracMoveSetPoint_c::config_base ( apc_ident, at_identMode, aui16_suppressMask ); }

    /** set commanded set point value of the machine speed as measured by the selected source
        @param ui16_val  set point value of the machine speed
      */
    void setSelectedSpeedSetPointCmd(const uint16_t ui16_val)
    {TracMoveSetPoint_c::setSelectedSpeedSetPointCmd(ui16_val);}
    /** communicate maximum allowed speed to the tractor
        @param ui16_val  maximum allowed speed
      */
    void setSelectedSpeedSetPointLimit(const uint16_t ui16_val)
    {TracMoveSetPoint_c::setSelectedSpeedSetPointLimit(ui16_val);}
    /** set commanded direction of the machine
        @param t_val  commanded direction of travel
      */
    void setSelectedDirectionCmd(const IsoAgLib::IsoDirectionFlag_t t_val) {TracMoveSetPoint_c::setSelectedDirectionCmd(t_val);}

    /** get commanded set point value of the machine speed as measured by the selected source
        @return  set point value of the machine speed
      */
    uint16_t selectedSpeedSetPointCmd() const {return TracMoveSetPoint_c::selectedSpeedSetPointCmd();}
    /** get communicated maximum allowed speed to the tractor
        @return  maximum allowed speed
      */
    uint16_t selectedSpeedSetPointLimit() const {return TracMoveSetPoint_c::selectedSpeedSetPointLimit();}
    /** get commanded direction of the machine
        @return  commanded direction of travel
      */
    IsoAgLib::IsoDirectionFlag_t selectedDirectionCmd() const {return TracMoveSetPoint_c::selectedDirectionCmd();}

  private:
    /** allow getITracMoveInstance() access to shielded tracgeneral class.
    otherwise __IsoAgLib::getTracMoveInstance4Comm() wouldn't be accepted by compiler
    */
    #if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
    friend iTracMoveSetPoint_c& getITracMoveSetPointInstance(uint8_t aui8_instance);
    #else
    friend iTracMoveSetPoint_c& getITracMoveSetPointInstance(void);
    #endif

  };

  #if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
  /** C-style function, to get access to the unique TracMove_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  inline iTracMoveSetPoint_c& getITracMoveSetPointInstance(uint8_t aui8_instance = 0)
  { return static_cast<iTracMoveSetPoint_c&>(__IsoAgLib::getTracMoveSetPointInstance(aui8_instance));};
  #else
  /** C-style function, to get access to the unique TracMove_c singleton instance */
  inline iTracMoveSetPoint_c& getITracMoveSetPointInstance(void)
  { return static_cast<iTracMoveSetPoint_c&>(__IsoAgLib::getTracMoveSetPointInstance());};
  #endif
}
#endif
