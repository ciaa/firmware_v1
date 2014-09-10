/*
  inputs.h:
    definition of Hardware Abstraction Layer for input functions for
    debug/simulation system on PC

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _HAL_PC_INPUTS_H_
#define _HAL_PC_INPUTS_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/pc/typedef.h>
#include <IsoAgLib/hal/pc/errcodes.h>
#include "inputs_target_extensions.h"

#include <cstdio>

/* ******************************************************** */
/**
 * \name Basic BIOS limit constants
 */
/*@{*/
#ifndef ANALOG_INPUT_MIN
#define ANALOG_INPUT_MIN 0
#endif
#ifndef ANALOG_INPUT_MAX
#define ANALOG_INPUT_MAX 7
#endif

#ifndef DIGITAL_INPUT_MIN
#define DIGITAL_INPUT_MIN 0
#endif
#ifndef DIGITAL_INPUT_MAX
#define DIGITAL_INPUT_MAX 15
#endif

#ifndef COUNTER_INPUT_MIN
#define COUNTER_INPUT_MIN 0
#endif
#ifndef COUNTER_INPUT_MAX
#define COUNTER_INPUT_MAX 15
#endif


/// set maximal voltage input for analog input set to voltage in [mV]
#ifndef MAX_ANALOG_MV
#define MAX_ANALOG_MV 8500
#endif
///set max digital BIOS function return value corresponding to MAX_ANALOG_MV
#ifndef MAX_ANALOG_MV_DIGIT
#define MAX_ANALOG_MV_DIGIT 907
#endif
/// set maximal current input for analog input set [mA]
#ifndef MAX_ANALOG_MA
#define MAX_ANALOG_MA 20
#endif
///set max digital BIOS function return value corresponding to MAX_ANALOG_MA
#ifndef MAX_ANALOG_MA_DIGIT
#define MAX_ANALOG_MA_DIGIT 833
#endif
/*@}*/

namespace HAL
{
  /* *************************** */
  /** \name Input BIOS functions */
  /*@{*/

  inline int16_t  init_analoginVolt(uint8_t bNumber)
  { return __HAL::init_analogin(bNumber, VOLTAGE_IN); }

  inline int16_t  init_analoginCurrent(uint8_t bNumber)
  { return __HAL::init_analogin(bNumber, CURRENT_IN); }

  inline int16_t  init_digin(uint8_t bInput,uint8_t bMode,uint8_t bAktivhighlow,void (*pfFunctionName)(...))
  { return __HAL::init_digin(bInput, bMode, bAktivhighlow, pfFunctionName); }

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
  { __HAL::setFastAnalogin(bMode); }

  inline int16_t  getAdcVoltage(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::getAdc(__HAL::getAnaloginCheckNr(ab_channel));
    if ( i16_temp == HAL_RANGE_ERR )
      return HAL_RANGE_ERR;
    // use 1:1 value mappgin for PC test
    // return ((i16_temp * 9) + ((i16_temp * 39)/100));};
    return i16_temp;
  }

  inline int16_t  getAdcMeanVoltage(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::getAnaloginMean(ab_channel);
    if ( i16_temp == HAL_RANGE_ERR )
      return HAL_RANGE_ERR;
    // use 1:1 value mappgin for PC test
    // return ((i16_temp * 9) + ((i16_temp * 39)/100));};
    return i16_temp;
  }

  inline int16_t  getAdcCurrent(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::getAdc(__HAL::getAnaloginCheckNr(ab_channel));
    if ( i16_temp == HAL_RANGE_ERR )
      return HAL_RANGE_ERR;
    // use 1:1 value mappgin for PC test
    // return ((i16_temp * 24) + ((i16_temp * 41)/100));};
    return i16_temp;
  }

  inline int16_t  getAdcMeanCurrent(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::getAnaloginMean(ab_channel);
    if ( i16_temp == HAL_RANGE_ERR )
      return HAL_RANGE_ERR;
    // use 1:1 value mappgin for PC test
    // return ((i16_temp * 24) + ((i16_temp * 41)/100));};
    return i16_temp;
  }

  inline int16_t  getDiginDiagnoseAdc(uint8_t ab_channel)
  {
    int16_t i16_temp = __HAL::getAdc(__HAL::getDiginAdcCheckNr(ab_channel));
    if ( i16_temp == HAL_RANGE_ERR )
      return HAL_RANGE_ERR;
    // use 1:1 value mappgin for PC test
    // return ((i16_temp * 8) + ((i16_temp * 43)/100));};
    return i16_temp;
  }

  inline bool  getAdcVirtMass( void )
  { return (__HAL::getAdc(GET_M_VIRT) < 440 ); }

  inline int16_t  getAdcTemp( void )
  {
    int16_t i16_temp = __HAL::getAdc(GET_D_T);
    return ((100*(i16_temp - 273))/(-83));
  }

  inline int16_t  getAdcThreshold( void )
  {
    int16_t i16_temp = __HAL::getAdc(GET_U_THRESHOLD);
    return ((i16_temp * 4) + ((i16_temp * 94)/100));
  }

  inline int16_t  getAdcPic( void )
  {
    int16_t i16_temp = __HAL::getAdc(GET_U_PIC);
    return ((i16_temp * 4) + ((i16_temp * 94)/100));
  }

  inline int16_t  getAdcCurrentSum( void )
  {
    int16_t i16_temp = __HAL::getAdc(GET_SUM_I);
    return ((i16_temp * 24) + ((i16_temp * 41)/100));
  }

  inline int16_t  getDiginOnoff(uint8_t bInputNumber)
  { return __HAL::getDiginOnoff(bInputNumber); }

  inline int16_t  getDiginOnoffStatic(uint8_t bInputNumber)
  { return __HAL::getDiginOnoffStatic(bInputNumber); }

  inline uint8_t getUbatAdcCheckNr()
  { return GET_U_C; }

  inline uint8_t getU85AdcCheckNr()
  { return GET_U_EXT_8_5_V; }

  /*@}*/

} // HAL

#endif
