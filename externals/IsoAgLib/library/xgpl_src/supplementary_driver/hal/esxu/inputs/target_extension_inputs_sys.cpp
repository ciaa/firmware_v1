/*
  inputs_target_extensions.cpp:
    some extensions to the used BIOS; for adaptations to the needed
    BIOS functions which can't be implemented in inline functions in
    bios_namespace_xx.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "inputs_target_extensions.h"
#include <IsoAgLib/util/impl/util_funcs.h>

using namespace std; // simple version to avoid problems with using CNAMESPACE


namespace __HAL {

#define RPM_PRESCALER_MIN	RPM_PRESCALER_8
#define RPM_PRESCALER_MAX	RPM_PRESCALER_1024

/** prescaler value for digin input channels DIN9 & DIN10 */
static uint8_t _b_prescale_1_Index = RPM_PRESCALER_MIN;

/**
  array of pointers to uint32_t arrays with counter
  values; avoid memory waste, if only some channels are
  used for counting
*/
#define SENSOR_ARRAY_SIZE	10
static uint32_t _pulDiginCounter[SENSOR_ARRAY_SIZE];
static uint16_t _prevCounter[SENSOR_ARRAY_SIZE];

int16_t init_counter(uint8_t ab_channel, uint16_t aui16_timebase, bool ab_activHigh, bool ab_risingEdge)
{
  int16_t i16_errorState;

  uint8_t b_codeActiv = (ab_activHigh)?HIGH_ACTIVE:LOW_ACTIVE;
  uint8_t b_codeEdge = (ab_risingEdge)?RISING_EDGE:FALLING_EDGE;

  uint32_t ui32_prescale = ( ((uint32_t)aui16_timebase) * get_cpu_freq() * 1000) / 65534;
  /* configure init channel */
  int16_t ret = init_digin(ab_channel, b_codeEdge, b_codeActiv, NULL);

// NOTE: On ESXu, The prescaler applies to both RPM inputs DIN9 and DIN10
// So, try to find the best prescaler for both inputs based on the maximum time per pulse in ms given by aui16_timebase
// The best (highest precision) prescaler is RPM_PRESCALER_MIN
  for( uint8_t ui8_pow = RPM_PRESCALER_MAX; ; ui8_pow-- )
  { /* the prescaler must be configured by (2 << pow) values */
    if ((ui32_prescale > (1 << (ui8_pow+2))) || (ui8_pow == RPM_PRESCALER_MIN))
    { /* only change prescaler if longer than before */
      if (_b_prescale_1_Index < ui8_pow )
        _b_prescale_1_Index = ui8_pow;
      break;
    }
  }
  /* set prescaler */
  i16_errorState = set_digin_prescaler(_b_prescale_1_Index);

  /* clear counter value
   */
  _pulDiginCounter[ab_channel] = 0;

  // Really need to call this for each input when the prescaler changes
  uint16_t wTime = (uint32_t)aui16_timebase * 1000 * get_cpu_freq() >> (_b_prescale_1_Index+3);
  uint16_t numPulsesToAvg = 1;
  int16_t configretval = config_digin_freq( ab_channel, wTime, numPulsesToAvg );

  return i16_errorState;
}

uint32_t getCounter(uint8_t ab_channel)
{
  uint16_t ui16_result = 0xFFFF, ui16_counter;

  int16_t retval = get_digin_period(ab_channel, &ui16_result, &ui16_counter);

  _pulDiginCounter[ab_channel] += ( ui16_counter - _prevCounter[ab_channel] );
  _prevCounter[ab_channel] = ui16_counter;

  return _pulDiginCounter[ab_channel];
}

int16_t resetCounter(uint8_t ab_channel)
{
  uint16_t ui16_result = 0xFFFF, ui16_counter;
  int16_t retval = get_digin_period(ab_channel, &ui16_result, &ui16_counter);

  _pulDiginCounter[ab_channel] = 0;
  _prevCounter[ab_channel] = ui16_counter;

  return C_NO_ERR;
}

uint32_t getCounterPeriod_us(uint8_t ab_channel)
{
  uint16_t ui16_result = 0xFFFF, ui16_counter;
  uint32_t ui32_result = 0xFFFFFFFFUL;

  int16_t retval = get_digin_period(ab_channel, &ui16_result, &ui16_counter);

  _pulDiginCounter[ab_channel] += ( ui16_counter - _prevCounter[ab_channel] );
  _prevCounter[ab_channel] = ui16_counter;
  // use util helper function to avoid overflow
  ui32_result	= __IsoAgLib::mul1Div1Mul2Div2((2 << (_b_prescale_1_Index + 2)), get_cpu_freq(), ui16_result, 1 );

  return ui32_result;
}

uint32_t getCounterFrequency(uint8_t ab_channel)
{
  word ui16_result = 0;

  int16_t retval = get_digin_freq((byte)ab_channel, &ui16_result);

  return ui16_result;
}

uint16_t getCounterLastSignalAge(uint8_t ab_channel)
{
  return 0;
}

} // __HAL
