/*
  outputs.h:
    definition of Hardware Abstraction Layer for output functions for
    debug/simulation system on PC

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _HAL_PC_OUTPUTS_H_
#define _HAL_PC_OUTPUTS_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/pc/typedef.h>
#include <IsoAgLib/hal/pc/errcodes.h>
#include "outputs_target_extensions.h"

#include <cstdio>


/* ******************************************************** */
/**
 * \name Basic BIOS limit constants
 */
/*@{*/
#ifndef DIGITAL_OUTPUT_MIN
#define DIGITAL_OUTPUT_MIN 0
#endif
#ifndef DIGITAL_OUTPUT_MAX
#define DIGITAL_OUTPUT_MAX 11
#endif
/*@}*/

namespace HAL
{
  /* **************************** */
  /** \name Output BIOS functions */
  /*@{*/

  inline void setMainRelais( bool bitState )
  { __HAL::setMainRelais(bitState); }

  inline int16_t setPwmFreq(uint8_t bOutputGroup, uint32_t dwFrequency)
  { return __HAL::setPwmFreq(bOutputGroup, dwFrequency); }

  inline uint16_t getMaxPwmDigout(uint8_t /* aui8_channel */ )
  { return 0xFFFF; }

  inline int16_t setDigout(uint8_t bOutputNo, uint16_t wPWMValue)
    {return __HAL::setDigout(bOutputNo, wPWMValue);};

  inline int16_t getDigoutCurrent( uint8_t aui8_channel )
  {
    if ( aui8_channel < 5 ) return 0;
    else return -1;
  }

  inline int16_t getDigoutDiagnose(uint8_t /* aui8_channel */, uint16_t /* aui16_minCurrent */, uint16_t /* aui16_maxCurrent */)
  { return HAL_NO_ERR; }

  inline int16_t getDigoutAdc( uint8_t /* aui8_channel */ )
  { return 1000; }
  
  /*@}*/
  
} // HAL

#endif
