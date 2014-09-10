/*
  actor_target_extensions_hal_simulator.cpp:
    source for HAL simulator for Actors (Outputs) This file was based
    on target_extension_actor_HALSimulator.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <IsoAgLib/isoaglib_config.h>

#include "outputs_target_extensions.h"

#include <IsoAgLib/util/iliberr_c.h>
#include <IsoAgLib/hal/pc/errcodes.h>

#include <IsoAgLib/hal/hal_system.h>


namespace __HAL {

void
setMainRelais( bool on )
{
  halSimulator().eventMainRelais( on );
}


int16_t
setPwmFreq(uint8_t bOutputGroup, uint32_t dwFrequency)
{
  if( bOutputGroup == 0xFF )
    return HAL_RANGE_ERR;

  // Callback to HAL Event Handler in the application to update the screen (or whatever)
  halSimulator().eventSetPwmFreq( bOutputGroup, dwFrequency );
  return HAL_NO_ERR;
}


int16_t
setDigout(uint8_t bOutputNo, uint16_t wPWMValue )
{
  if( bOutputNo == 0xFF )
    return HAL_RANGE_ERR;

  // Callback to HAL Event Handler in the application to update the screen (or whatever)
  halSimulator().eventSetDigout( bOutputNo, wPWMValue );
  return HAL_NO_ERR;
}


}
