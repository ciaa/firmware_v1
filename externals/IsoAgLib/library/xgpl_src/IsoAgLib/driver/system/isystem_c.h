/*
  isystem_c.h: module for a interfacing the system hardware

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "impl/system_c.h"

namespace IsoAgLib {

/**
  Layer class to encapsulate the hardware specific details.
  All system interaction have to be done via this class.
  @author Dipl.-Inform. Martin Wodok
  @author Dipl.-Inform. Achim Spangler
*/
class iSystem_c : private __IsoAgLib::System_c {
public:
  /** Initialize the system hardware */
  void init() { System_c::init(); }

  /** Shutdown system hardware */
  void close() { System_c::close(); }

  /** init the hardware watchdog */
  static void initWatchdog( void* config ) { System_c::initWatchdog( config ); }

  /** init the hardware watchdog */
  static void closeWatchdog() { System_c::closeWatchdog(); }

  /** trigger the watchdog */
  static void triggerWatchdog() { System_c::triggerWatchdog(); }

  /**
    deliver lasted time from start of system in msec.
    @return running time in [msec.]
  */
  static int32_t getTime() { return System_c::getTime(); }

  /** @return true -> ECU's On/Off-Switch reports system being "On" */
  static bool switchedOn() { return System_c::switchedOn(); }

  /**
    get the main power voltage
    @return voltage of power [mV]
  */
  static int16_t getBatteryVoltage() {return System_c::getBatteryVoltage(); }
  /**
    get the voltage of the external reference 8.5Volt for work of external sensors
    @return voltage at external reference [mV]
  */
  static int16_t getExternalSensorPowerVoltage()
  { return System_c::getExternalSensorPowerVoltage(); }

  /**
    deliver the serial nr of the device into uint8_t[6] array
    - f.e. to calculated individual wait berfore address claim
    @param snrDat pointer to 6 uint8_t array, where the serial no of the device is stored
  */
  static void serialNo( uint8_t *const snrDat) { System_c::serialNo(snrDat); }

private:
  friend iSystem_c& getIsystemInstance( void );

};

inline iSystem_c& getIsystemInstance( void ) { return static_cast<iSystem_c&>( __IsoAgLib::getSystemInstance()) ; }

}

#endif
