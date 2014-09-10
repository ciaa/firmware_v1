/*
  actor_target_extensions.h:
    header for PC specific extensions for the HAL for actor

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef _HAL_PC_OUTPUTS_TARGET_EXTENSIONS_H_
#define _HAL_PC_OUTPUTS_TARGET_EXTENSIONS_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/pc/typedef.h>

namespace __HAL {

  int16_t  setPwmFreq(uint8_t bOutputGroup, uint32_t dwFrequency);  /* define the frequency of the pwm signal */
  int16_t  setDigout(uint8_t bOutputNo, uint16_t wPWMValue);          /* set pwm value 0 ... 100 %*/

  void setMainRelais( bool on );

/*@}*/

} // __HAL

#endif
