/*
  inputs.h: definition of Hardware Abstraction Layer for input
    functions for ESXu

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _HAL_ESXu_INPUTS_H_
#define _HAL_ESXu_INPUTS_H_

#include <IsoAgLib/isoaglib_config.h>
#include "inputs_target_extensions.h"

/* ******************************************************** */
/**
 * \name Basic BIOS limit constants
 */
/*@{*/
#define ANALOG_INPUT_MIN AIN1
#define ANALOG_INPUT_MAX AIN8
#define DIGITAL_INPUT_MIN DIN1
#define DIGITAL_INPUT_MAX DIN10
#define COUNTER_INPUT_MIN DIN1
#define COUNTER_INPUT_MAX DIN10

/// set maximal voltage input for analog input set to voltage in [mV]
#define MAX_ANALOG_MV 30000
///set max digital BIOS function return value corresponding to MAX_ANALOG_MV
#define MAX_ANALOG_MV_DIGIT 1000
/// set maximal current input for analog input set [mA]
#define MAX_ANALOG_MA 25
///set max digital BIOS function return value corresponding to MAX_ANALOG_MA
#define MAX_ANALOG_MA_DIGIT 1000
/*@}*/

namespace __HAL {
  inline uint8_t getAnaloginCheckNr(uint8_t channel)
  { return (GET_A_IN_1 - channel); }

  inline uint8_t getDiginAdcCheckNr(uint8_t ab_channel)
  { return getAnaloginCheckNr( ab_channel );}
}

namespace HAL
{
  /* *************************** */
  /** \name Input BIOS functions */
/*@{*/

  inline int16_t  init_analoginVolt(uint8_t bNumber)
  { return __HAL::init_analogin(bNumber, VOLTAGE_IN); }

  inline int16_t  init_analoginCurrent(uint8_t bNumber)
  { return __HAL::init_analogin(bNumber, CURRENT_IN); }

  inline int16_t  init_digin(uint8_t ab_channel,uint8_t bMode,uint8_t bAktivhighlow,void (*pfFunctionName)())
  { return __HAL::init_digin(ab_channel, bMode, bAktivhighlow, pfFunctionName); }

  inline int16_t init_counter(uint8_t ab_channel, uint16_t aui16_timebase, bool ab_activHigh, bool ab_risingEdge)
  { return __HAL::init_counter(ab_channel, aui16_timebase, ab_activHigh, ab_risingEdge); }

  inline uint32_t getCounter(uint8_t ab_channel)
  { return __HAL::getCounter(ab_channel); }

  inline int16_t resetCounter(uint8_t ab_channel)
  { return __HAL::resetCounter(ab_channel); }

  inline uint32_t getCounterPeriod_us(uint8_t ab_channel)
  { return __HAL::getCounterPeriod_us(ab_channel); }

  inline uint32_t getCounterFrequency(uint8_t ab_channel)
  { return __HAL::getCounterFrequency(ab_channel); }

  inline uint32_t getCounterLastSignalAge(uint8_t ab_channel)
  { return __HAL::getCounterLastSignalAge(ab_channel); }

  inline void setFastAnalogin(bool bMode)
  { /* seems to be not supported here */ }

  inline int16_t  getAdcVoltage(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::get_adc(__HAL::getAnaloginCheckNr(ab_channel));

    if ( i16_temp == C_RANGE )
      return C_RANGE;
    return (i16_temp * 10);
  }

  inline int16_t  getAdcMeanVoltage(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::get_adc_mean(__HAL::getAnaloginCheckNr(ab_channel));

    if ( i16_temp == C_RANGE )
      return C_RANGE;
    return (i16_temp * 10);
  }

  inline int16_t  getAdcCurrent(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::get_adc(__HAL::getDiginAdcCheckNr(ab_channel));

    if ( i16_temp == C_RANGE )
      return C_RANGE;
    return (i16_temp * 25);
  }

  inline int16_t  getAdcMeanCurrent(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::get_adc_mean(__HAL::getDiginAdcCheckNr(ab_channel));

    if ( i16_temp == C_RANGE )
      return C_RANGE;
    return (i16_temp * 25);
  }

  inline int16_t  getDiginDiagnoseAdc(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::get_adc(__HAL::getDiginAdcCheckNr(ab_channel));

    if ( i16_temp == C_RANGE )
      return C_RANGE;
    return (i16_temp * 10);
  }

  inline int16_t  getAdcTemp( void )
  {
    __HAL::t_Sys_AnalogData t_Sys_AnalogData;
    __HAL::get_system_analogdata(&t_Sys_AnalogData);

    return t_Sys_AnalogData.iTemperatur;
  }

  inline int16_t  getDiginOnoff(uint8_t ab_channelNumber)
  { return __HAL::get_digin_onoff(ab_channelNumber); }

  inline int16_t  getDiginOnoffStatic(uint8_t ab_channelNumber)
  { return __HAL::get_digin_onoff_static(ab_channelNumber); }

} // HAL

#endif
