/*
  targetExtensions.cc: source for C2C specific extensions for the HAL
    for central system

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/** \file c2c/system/system_target_extensions.cpp
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
  const int16_t i16_result = open_c2c( &t_biosextSysdata );

  switch( i16_result )
  {
    case C_DEFAULT:
    case C_NO_ERR:
      system_is_opened = true;
      break;
    default:
      abort();
  }
}

void closeSystem()
{
  system_is_opened = false;
}

bool isSystemOpened()
{
  return system_is_opened;
}

void initWatchdog( void* config )
{
  byte bTime = WD_OFF;
  if( config ) {
    bTime = *( static_cast<byte*>( config ) );
  }
  if( C_NO_ERR != config_wd(bTime) )
    abort();
}

} // end namespace __HAL
