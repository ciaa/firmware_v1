/*
  system_target_extensions.cpp: source for ESX specific extensions for
    the HAL for central system

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/** \file esx/system/system_target_extensions.cpp
 * A module targetExtensions should be used
 * for all methods, which can't be simply
 * mapped from ECU standard BIOS to the needs of
 * IsoAgLib by mostly renaming and reordering of functions, parameters
 * and types in <i>\<target\>/\<device\>/\<device\>.h</i> .
 * ********************************************************** */
#include <stdlib.h>

#include "system_target_extensions.h"
#include "../config.h"


namespace __HAL {
extern "C" {
  /** include the BIOS specific header with the part for CAN into __HAL */
  #include <commercial_BIOS/bios_esx/Xos20esx.h>

  #ifdef _INIT_BABYBOARD_
  #include <commercial_BIOS/bios_esx/Module/xma20.h>
  #endif

/** task function which is called each 50msec to trigger the BIOS WD
    if the App has called wdTriggern(void) max cui16_maxTaskTriggeredWdResets
		rounds ago
*/
void taskTriggerWatchdog();
}
/** initialise static tSystem to complete zero, so that a call of
  * openSystem can be reliable detected
  */
static tSystem t_biosextSysdata =
  {0,0,0,0,0,0,0,
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
  0,0,0,0,0};


void openSystem()
{
  const int16_t i16_result = open_esx(&t_biosextSysdata);


  #ifdef _INIT_BABYBOARD_
   /************initialization of Babyboard at Position 1*********/
  BA_init(POSITION_1, VARIANT_MIX );
  BA_set_pwm_freq(POSITION_1, PIN_1, 500);
  BA_set_pwm_freq(POSITION_1, PIN_2, 500);
  BA_set_pwm_freq(POSITION_1, PIN_3, 500);
  BA_set_pwm_freq(POSITION_1, PIN_4, 500);
  #endif

  switch( i16_result )
  {
    case C_CONFIG:
    case C_NO_ERR:
      return;
    default:
      abort();
  }
}

void closeSystem()
{
  if( C_NO_ERR != close_esx() )
    abort();
}

bool isSystemOpened( void )
{
  return ( ( t_biosextSysdata.bCPU_freq != 0 )
    && ( t_biosextSysdata.wRAMSize != 0 )
    && ( t_biosextSysdata.wROMSize != 0 )
    && ( t_biosextSysdata.bEESize != 0 ) );
}

/** max amount of task triggered WD resets */
static const uint16_t cui16_maxTaskTriggeredWdResets = 40;

/** counter for task triggered WD events
	  --> if the WD trigger task is activated it checks for this counter
		  ==> on < cui16_maxTaskTriggeredWdResets -> trigger BIOS wd
			==> else -> do nothing so that WD resets the system
*/
static uint16_t sui16_watchdogCounter = 0;

/** task function which is called each 50msec to trigger the BIOS WD
    if the App has called wdTriggern(void) max cui16_maxTaskTriggeredWdResets
		rounds ago
*/
void taskTriggerWatchdog()
{
	if ( sui16_watchdogCounter < cui16_maxTaskTriggeredWdResets )
	{ // fine - last WD call by app was not too long ago
		wd_triggern();
		sui16_watchdogCounter++;
	}
}

/** trigger the watchdog */
void triggerWatchdog()
{
	sui16_watchdogCounter = 0;
}

/** flag to control if WD-Task is started already */
static bool sb_isWdTriggerTaskRunning = false;

void initWatchdog( void* config )
{
  tWDConfig t_watchdogConf = {
      WD_MAX_TIME,
      WD_MIN_TIME,
      UD_MAX,
      UD_MIN,
      CONFIG_RELAIS,
      CONFIG_RESET
  };

  if( config ) {
    t_watchdogConf = *static_cast<tWDConfig*>( config );
  }

  switch( config_wd( &t_watchdogConf ) ) {
    case DATA_CHANGED:
    case C_CHECKSUM:
      if( C_NO_ERR != wd_reset() ) {
        abort();
      }
  }

  if ( ! sb_isWdTriggerTaskRunning )
  {
    init_task_call( TASKLEVEL_2A, 5, 5, taskTriggerWatchdog );
    start_task_timer( 20 );
    sb_isWdTriggerTaskRunning = true;
  }
}

} // end namespace __HAL
