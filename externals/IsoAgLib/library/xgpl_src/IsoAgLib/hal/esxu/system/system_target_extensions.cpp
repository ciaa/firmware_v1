/*
  targetExtensions.cc: source for ESXu specific extensions for the HAL
    for central system

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/** \file esxu/system/system_target_extensions.cpp
 * A module targetExtensions should be used
 * for all methods, which can't be simply 
 * mapped from ECU standard BIOS to the needs of
 * IsoAgLib by mostly renaming and reordering of functions, parameters
 * and types in <i>\<target\>/\<device\>/\<device\>.h</i> .
 * ********************************************************** */
#include <stdlib.h>

#include "system.h"

namespace __HAL {

static bool system_is_opened = false;

void openSystem()
{
  tSystem t_biosextSysdata;
  switch( open_micro( &t_biosextSysdata ) )
  {
    case C_NO_ERR:
      system_is_opened = true;
      break;
    default:
      abort();
  }

}

void closeSystem()
{
  if( C_NO_ERR != close_micro() )
    abort();

  // close micro performs a CPU reset - so
  // the following is just for convenience
  system_is_opened = false;
}

bool isSystemOpened( void )
{
  return system_is_opened;
}

void initWatchdog( void* config ) 
{
  tWDConfig t_watchdogConf = {
      WD_MAX_TIME,
      UD_MIN,
      UD_MAX
  };

  if( config ) {
    t_watchdogConf = *static_cast<tWDConfig*>( config );

  }

  const int16_t retval = config_wd( &t_watchdogConf );

  if( retval != C_NO_ERR ) {
    abort();
  }
}

} // __HAL
