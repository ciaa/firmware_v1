/*
  outputs_target_extensions.cpp: source for PC specific extensions for
    the HAL for outputs

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "outputs_target_extensions.h"

#include <IsoAgLib/hal/pc/errcodes.h>

#include <cstdio>


namespace __HAL {


void
setMainRelais( bool on )
{
  printf("Outputs: MainRelais set to %d\n", on);
}


int16_t
setPwmFreq(uint8_t bOutputGroup, uint32_t dwFrequency)
{
  printf("Outputs: Channel %i: PwmFreq set to %i.\n", bOutputGroup, dwFrequency);
  return HAL_NO_ERR;
}


int16_t
setDigout( uint8_t bOutputNo, uint16_t wPWMValue )
{
  printf("Outputs: Channel %i: DigOut set to %i.\n", bOutputNo, wPWMValue);
  return HAL_NO_ERR;
}

} // __HAL
