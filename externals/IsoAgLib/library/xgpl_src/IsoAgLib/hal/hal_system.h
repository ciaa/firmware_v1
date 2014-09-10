/*
  hal_system.h: include dependent on used target (defined in
    IsoAgLib/isoaglib_config.h) the suitable HAL specific header for
    central system functions

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/* ************************************************************ */
/** \file IsoAgLib/hal/hal_system.h
  * include dependent on used target (defined in
	  IsoAgLib/isoaglib_config.h) the suitable HAL
		specific header for central system functions.
*/
/* ************************************************************ */
#ifndef _HAL_INDEPENDENT_SYSTEM_H_
#define _HAL_INDEPENDENT_SYSTEM_H_

#include <IsoAgLib/isoaglib_config.h>
#include "hal_config.h"

namespace HAL
{
  void openSystem();
  void closeSystem();

  bool isSystemOpened();

  void initWatchdog( void* config );
  void closeWatchdog();

  void triggerWatchdog();

  int32_t getTime(); // in [ms]

  int16_t getSnr(uint8_t *snrDat);
  int32_t getSerialNr(int16_t* pi16_errCode = NULL);

  /**
    check if D+/OnOffSwitch is active
    @return true if D+ is switched on
  */
  bool getOn_offSwitch();

  /**
    get the main power voltage
    @return voltage of power [mV]
  */
  int16_t getAdcUbat();

  /**
    get the voltage of the external reference 8.5Volt for work of external sensors
    @return voltage at external reference [mV]
  */
  int16_t getAdc_u85();

  /** allow the HAL to power down the system (or not) */
  void powerHold( bool ab_on );

  /**
    Sleep and yield CPU for other tasks.
    Especially important for Multitasking environments
    as Windows/Linux running with CAN-Server for example.
    NOTE: The sleep can also be shorter depending on the
          operating system, even no-sleep on
          single-tasking systems!
   */    
  void sleep_max_ms( uint32_t ms );
}


/// INCLUDE CONCRETE SYSTEM-HAL-IMPLEMENTATION
/// ==========================================

#ifdef HAL_PATH_ISOAGLIB_SYSTEM
#  define _hal_system_header_ <HAL_PATH_ISOAGLIB_SYSTEM/system.h>
#  include _hal_system_header_
#else
#  error "No HAL_PATH_ISOAGLIB_SYSTEM set. This is a mandatory module!"
#endif


#endif
