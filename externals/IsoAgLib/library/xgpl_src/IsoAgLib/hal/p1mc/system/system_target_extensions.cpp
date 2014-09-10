/*
  system_target_extensions.cpp: source for P1MC specific extensions
    for the HAL for central system

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

 /** \file IsoAgLib/hal/p1mc/system/system_target_extensions.ccp
 * A module targetExtensions should be used
 * for all methods, which can't be simply
 * mapped from ECU standard BIOS to the needs of
 * IsoAgLib by mostly renaming and reordering of functions, parameters
 * and types in <i>\<target\>/\<device\>/\<device\>.h</i> .*/
 /* ********************************************************** */

#include "../typedef.h"
#include "system_target_extensions.h"
#include "../config.h"

#include "datatypes.h"
#include "idapp.h"
#include "datacontainer.h"

extern "C"
{
  extern U32 os_gulGetTimeMs(void);
}

namespace __HAL {

void openSystem()
{
  // set repetition time for maintask to 1 ms
  OS.ExecTimeOut = 1;
}
void closeSystem() {}

int32_t getTime(void)
{
  return os_gulGetTimeMs();
}

int16_t  getSnr(uint8_t *snrDat)
{
  return -1;
}

void start_task_timer ( void )
{

}

bool getOn_offSwitch() {
  return true;
}

void powerHold( bool )
{

}

int16_t __getAdcUbat()
{
  return -1;
}

int16_t getAdc_u85()
{
  return -1;
}

} // end namespace __HAL
