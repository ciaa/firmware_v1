/*
  outputs.h: definition of HAL namesapce layer for ESXu

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _HAL_ESXu_OUTPUTS_H_
#define _HAL_ESXu_OUTPUTS_H_

namespace __HAL {
  extern "C" {
    /** include the BIOS specific header into __HAL */
    #include <commercial_BIOS/bios_esxu/mos10osy.h>
  }
}

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/esxu/typedef.h>
#include <IsoAgLib/hal/esxu/errcodes.h>


/* ******************************************************** */
/**
 * \name Basic BIOS limit constants
 */
/*@{*/
#define DIGITAL_OUTPUT_MIN OUT1
#define DIGITAL_OUTPUT_MAX OUT6
/*@}*/

namespace __HAL {
  /**
    deliver channel number for checking/requesting of
    ADC value at pwm output
    for call of __HAL:: functions (differences mostly caused
    by multiplexers)
    @param channel number from interval [0..maxNo]
    @return according channel number for __HAL call
  */
  inline uint8_t getPwmoutAdcCheckNr(uint8_t ab_channel)
    { return GET_OUT_1-ab_channel; }

  /**
    deliver channel number for checking/requesting of
    current output through given PWM output
    @param channel number from interval [0..maxNo]
    @return according channel number for __HAL call
  */
  inline uint8_t getPwmCurrentCheckNr(uint8_t ab_channel)
    {return GET_I_PWM_1-ab_channel;}
}

namespace HAL
{
  /* **************************** */
  /** \name Output BIOS functions */
/*@{*/

  inline void setMainRelais(bool bitState)
  { __HAL::set_relais(bitState); }

  // ESXu BIOS lets PWM channels OUT1, OUT2 configure individual PWM FREQ
  inline int16_t setPwmFreq(uint8_t bOutput, uint32_t dwFrequency)
  { return __HAL::set_pwm_freq(bOutput, dwFrequency); }

  inline uint16_t getMaxPwmDigout(uint8_t aui8_channel)
  {
    __HAL::tOutput tOutputstatus;
    int16_t retval = __HAL::get_digout_status(aui8_channel,&tOutputstatus);

    return tOutputstatus.wMaxOutput;
  }

  inline int16_t setDigout(uint8_t aui8_channel, uint16_t wPWMValue)
  { return __HAL::set_digout(aui8_channel, wPWMValue); }

  inline int16_t setDigoutMask(uint16_t wOutputMask, uint16_t wDigitalValue)
  { return __HAL::set_digout_mask(wOutputMask, wDigitalValue); }

  inline int16_t getDigoutCurrent( uint8_t aui8_channel )
  { return (aui8_channel <= OUT4) ? __HAL::get_adc( __HAL::getPwmCurrentCheckNr(aui8_channel) ) : -1; }

  int16_t getDigoutDiagnose(uint8_t aui8_channel, uint16_t aui16_minCurrent, uint16_t aui16_maxCurrent);
  int16_t getDigoutAdc( uint8_t aui8_channel );

  /*@}*/
}
#endif
