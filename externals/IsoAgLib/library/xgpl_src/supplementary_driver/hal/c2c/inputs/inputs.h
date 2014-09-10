/*
  inputs.h: definition of Hardware Abstraction Layer for input
    functions for C2C

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _HAL_C2C_INPUTS_H_
#define _HAL_C2C_INPUTS_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/c2c/typedef.h>
#include <IsoAgLib/hal/c2c/errcodes.h>
#include "inputs_target_extensions.h"


/* ******************************************************** */
/**
 * \name Basic BIOS limit constants
 */
/*@{*/
#define ANALOG_INPUT_MIN 0
#define ANALOG_INPUT_MAX 3
#define DIGITAL_INPUT_MIN 0
#define DIGITAL_INPUT_MAX 0

#define COUNTER_INPUT_MIN 0
#define COUNTER_INPUT_MAX 0

/// set maximal voltage input for analog input set to voltage in [mV]
#define MAX_ANALOG_MV 8500
///set max digital BIOS function return value corresponding to MAX_ANALOG_MV
#define MAX_ANALOG_MV_DIGIT 907
/// set maximal current input for analog input set [mA]
#define MAX_ANALOG_MA 20
///set max digital BIOS function return value corresponding to MAX_ANALOG_MA
#define MAX_ANALOG_MA_DIGIT 833

///set the max voltage that can be read off the ADC [V].  There are 2 variants of C2Cs
///Those with single CAN (that we have) are setup to be 0-30 V
///Those with dual CAN (that we have)  are setup to be 0-10 V
// Override this in your config_xxx.h file if you use single CAN C2Cs with 0-30 V range.
#ifndef CONFIG_HAL_C2C_MAX_ADC_V
#  define CONFIG_HAL_C2C_MAX_ADC_V 10
#endif

/*@}*/

namespace __HAL {
  extern "C" {
    /** include the BIOS specific header into __HAL */
    #include <commercial_BIOS/bios_c2c/c2c10osy.h>
  }
  /**
    deliver channel number for checking/requesting of analog input
    for call of __HAL:: functions (differences mostly caused
    by multiplexers)
    @param channel number from interval [0..maxNo] == [0..3]
    @return according channel number for __HAL call
  */
  inline uint8_t getAnaloginCheckNr(uint8_t ab_channel)
    {return (GET_A_IN_1 + ab_channel);};
}


namespace HAL
{
  inline int16_t init_analoginVolt(uint8_t bNumber)
  { return __HAL::init_analogin(bNumber, /* bitPullDownResistor: */ false); }

  inline int16_t init_analoginCurrent(uint8_t bNumber)
  { return HAL_CONFIG_ERR; }

  /* The C2C has only 1 channel (DIN1), so ab_channel must be set to DIN1 otherwise C_RANGE is returned */
  /* The C2C also does not support bAktivhighlow and so bAktivhighlow is ignored */
  inline int16_t init_digin(uint8_t ab_channel,uint8_t bMode,uint8_t bAktivhighlow,void (*pfFunctionName)())
  {
    int16_t ret;
    if( ab_channel == DIN1 )
      ret = __HAL::init_digin(bMode, pfFunctionName);
    else
      ret = C_RANGE;
    return ret;
  }

  inline int16_t init_counter(uint8_t ab_channel, uint16_t aui16_timebase, bool ab_activHigh, bool ab_risingEdge)
  { return __HAL::init_counter(aui16_timebase, ab_risingEdge); }

  inline uint32_t getCounter(uint8_t ab_channel)
  { return (ab_channel == DIN1) ? __HAL::getCounter() : 0; }

  inline int16_t resetCounter(uint8_t ab_channel)
  { return (ab_channel == DIN1) ? __HAL::resetCounter() : C_RANGE; }

  inline uint32_t getCounterPeriod_us(uint8_t ab_channel)
  { return (ab_channel == DIN1) ? __HAL::getCounterPeriod_us() : 0xFFFFFFFFUL; }

  inline uint32_t getCounterFrequency(uint8_t ab_channel)
  { return (ab_channel == DIN1) ? __HAL::getCounterFrequency() : 0; }

  inline uint32_t getCounterLastSignalAge(uint8_t ab_channel)
  { return (ab_channel == DIN1) ? __HAL::getCounterLastSignalAge() : 0xFFFF; }


  inline void setFastAnalogin(bool bMode)
  { /** !!! the C2C doesn't provide setFastAnalogin !! */ }

  inline int16_t getAdcVoltage(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::get_adc(__HAL::getAnaloginCheckNr(ab_channel));
    if ( i16_temp == C_RANGE )
      return C_RANGE;
    return (i16_temp * CONFIG_HAL_C2C_MAX_ADC_V);
  }

  inline int16_t getAdcMeanVoltage(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::get_adc_mean(__HAL::getAnaloginCheckNr(ab_channel));
    if ( i16_temp == C_RANGE ) return C_RANGE;
    return (i16_temp * CONFIG_HAL_C2C_MAX_ADC_V);
  }

  inline int16_t getAdcCurrent(uint8_t ab_channel)
  { return C_RANGE; }

  inline int16_t getAdcMeanCurrent(uint8_t ab_channel)
  { return C_RANGE; }

  inline int16_t getDiginDiagnoseAdc(uint8_t ab_channel)
  { return C_RANGE; }

  inline int16_t getAdcTemp()
  { return __HAL::get_boardtemperature(); }

  inline int16_t getDiginOnoff(uint8_t ab_channelNumber)
  { return __HAL::get_digin_hilo(); }

  inline int16_t getDiginOnoffStatic(uint8_t ab_channelNumber)
  { return __HAL::get_digin_hilo_static(); }

}

#endif
