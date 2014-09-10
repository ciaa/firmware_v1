/*
  system_c.h: module for a interfacing the system hardware

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef SYSTEM_H
#define SYSTEM_H

#include <IsoAgLib/isoaglib_config.h>

#include <IsoAgLib/hal/hal_system.h>
#include <IsoAgLib/util/impl/singleton.h>

namespace __IsoAgLib {

  /**
    Layer class to encapsulate the hardware specific details.
    All system interaction have to be done via this class.
    @author Dipl.-Inform. Martin Wodok
    @author Dipl.-Inform. Achim Spangler
  */
  class System_c
  {
    public:
      /** Initialize the system hardware. */
      void init();

      /** Shutdown system hardware */
      void close();

      /** init the hardware watchdog */
      static void initWatchdog( void* config) {
        HAL::initWatchdog( config );
      }

      /** init the hardware watchdog */
      static void closeWatchdog() {
        HAL::closeWatchdog();
      }

      /** trigger the watchdog */
      static void triggerWatchdog() { HAL::triggerWatchdog(); }

      /** deliver lasted time from start of system in msec.
        (use BIOS function)
        @return running time in [msec.]
      */
      static int32_t getTime() { return HAL::getTime(); }

      /** get the main power voltage
        @return voltage of power [mV]
      */
      static int16_t getBatteryVoltage() { return HAL::getAdcUbat(); }

      /** get the voltage of the external reference 8.5Volt for work of external sensors
        @return voltage at external reference [mV]
      */
      static int16_t getExternalSensorPowerVoltage() { return HAL::getAdc_u85(); }

      /** @return true -> ECU's On/Off-Switch reports system being "On".  */
      static bool switchedOn() { return HAL::getOn_offSwitch(); }

      /**
        deliver the serial nr of the device into uint8_t[6] array
        - f.e. to calculated individual wait berfore address claim
        (uses BIOS function)
        @param snrDat pointer to 6 uint8_t array, where the serial no of the device is stored
      */
      static inline void serialNo( uint8_t *const snrDat ) { HAL::getSnr( snrDat ); }

    private:
      System_c() {}
      friend System_c &getSystemInstance();
  };

  System_c& getSystemInstance();

}
#endif
