/*
  inputs_target_extensions.cpp: some extensions to the used BIOS;
    for adaptations to the needed BIOS functions which can't be
    implemented in inline functions in bios_namespace_xx.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "inputs_target_extensions.h"
#include <IsoAgLib/hal/c2c/config.h>
#include <IsoAgLib/util/impl/util_funcs.h>

#include <cstdlib>
#include <cstring>
#include <string.h>


namespace __HAL {
extern "C" {
  /** include the BIOS specific header with the part for CAN into __HAL */
  #include <commercial_BIOS/bios_c2c/c2c10osy.h>
}


/** prescaler value for digin input channels DIN1 */
static uint8_t _b_prescale_1_Index;
/**
  array of pointers to uint32_t arrays with counter
  values; avoid memory waste, if only some channels are
  used for counting
*/
static uint32_t _pulDiginCounter;
static uint16_t _prevCounter;

/**
  structure to store last and actual trigger timestamp
  (with MOD(0xFFFF) )
*/
typedef struct
{
  uint16_t uiPeriod;
  uint16_t uiAct;
} t_triggerNode;

/**
  if the given timebase is longer than the timebase powered
  by standard BIOS, the timestamp of last two trigger events
  should be stored for calculating of longer periods
*/
static t_triggerNode _pt_diginTriggerTime;

/**
  pointer to triger node of diginput which caused interrupt
*/
t_triggerNode *_put_temp;
/**
  configured timebase for counter channels;
  if time between two events is longer than given
  timebase, getCounterPeriod_us answers 0;
  important for timebases which are not supported by standard BIOS
*/
static uint16_t _puiDiginTimebase;

typedef void (*counterIrqFunction)();
static uint16_t _wIrqTime;

void counterIrqFlex()
{
  _pulDiginCounter++;
  // the config function of input must guarantee, that
  // vectors memory are allocated
  _put_temp = &_pt_diginTriggerTime;
  _wIrqTime = (get_time() & 0xFFFF);
  if (_wIrqTime > _put_temp->uiAct) _put_temp->uiPeriod = _wIrqTime - _put_temp->uiAct;
  else
  {
    if (_wIrqTime == _put_temp->uiAct) _put_temp->uiPeriod = 1;
    else _put_temp->uiPeriod = _wIrqTime - (0xFFFF - _put_temp->uiAct);
  }
  _put_temp->uiAct = _wIrqTime;
}

void counterIrq_0(){counterIrqFlex();}

int16_t init_counter(uint16_t aui16_timebase, bool ab_risingEdge)
{
  int32_t i32_prescale = aui16_timebase;
  uint8_t b_codeEdge = (ab_risingEdge)?RISING_EDGE:FALLING_EDGE;

//  int32_t ui16_prescale = ((aui16_timebase * getCpuFreq() * 1000) / 65534);
  uint8_t ui8_pow;
  int16_t i16_errorState;
  i32_prescale *= (get_cpu_freq() * 1000);
  i32_prescale /= 65534;
  /* configure init channel */
//  init_digin(b_codeEdge, irqFuncArr);
  (void) init_digin(b_codeEdge, NULL);

  /* standard BIOS supports two prescaler parts */
  for (ui8_pow = 9; ui8_pow > 1; ui8_pow--)
  { /* the prescaler must be configured by (2 << pow) values */
    if ((i32_prescale > (2 << ui8_pow)) || (ui8_pow == 2))
    { /* only change prescaler if longer than before */
      if (_b_prescale_1_Index < (ui8_pow - 2))
        _b_prescale_1_Index = (ui8_pow - 2);
      break;
    }
  }
  /* set prescaler */
  i16_errorState = set_digin_prescaler(_b_prescale_1_Index);

  /* clear counter value
   */
  _pulDiginCounter = 0;

  /* clear given timebase */
  _puiDiginTimebase = 0;

  if (i32_prescale > 1024)
  { /* standard BIOS frequency and period methods doesn�t fit for
     * the wanted timebase -> use extension functions -> allocate needed vars
     */
    CNAMESPACE::memset(&_pt_diginTriggerTime, 0, sizeof(t_triggerNode));
  }

  return i16_errorState;
}

uint32_t getCounter()
{
  uint16_t ui16_result = 0xFFFF, ui16_counter;

  get_digin_period(&ui16_result, &ui16_counter);
  _pulDiginCounter += ( ui16_counter - _prevCounter );
  _prevCounter = ui16_counter;

  return _pulDiginCounter;
}

int16_t resetCounter()
{
  uint16_t ui16_result = 0xFFFF, ui16_counter;
  get_digin_period(&ui16_result, &ui16_counter);
  _pulDiginCounter = 0;
  _prevCounter = ui16_counter;

  return C_NO_ERR;
}

uint32_t getCounterPeriod_us()
{
  uint16_t ui16_result = 0xFFFF, ui16_counter;
  uint32_t ui32_result = 0xFFFFFFFFUL;

  get_digin_period(&ui16_result, &ui16_counter);
  _pulDiginCounter += ( ui16_counter - _prevCounter );
  _prevCounter = ui16_counter;

  // use util helper function to avoid overflow
  ui32_result	= __IsoAgLib::mul1Div1Mul2Div2((2 << (_b_prescale_1_Index + 2)), get_cpu_freq(), ui16_result, 1 );
#if 0
  uint16_t ui16_timebase = _puiDiginTimebase;
  if (ui16_timebase == 0) ui16_result = 0xFFFF;
  else if (ui16_timebase < (1024 * 65534 / (get_cpu_freq() * 1000)))
  { /* use standard BIOS method because timebase is short enough */
    get_digin_period(&ui16_result, &ui16_counter);

    // use util helper function to avoid overflow
    ui32_result = __IsoAgLib::mul1Div1Mul2Div2((2 << (_b_prescale_1_Index + 2)), get_cpu_freq(), ui16_result, 1 );
  }
  else
  { /* use extension method */
    /* vars are accessible */
    if (getCounterLastSignalAge() < ui16_timebase)
    { // handle overflow between uiLast and uiAct
      ui32_result = _pt_diginTriggerTime.uiPeriod * 1000;
      if (ui32_result == 0) ui32_result = 1;
    }
  }
#endif
  return ui32_result;
}

uint32_t getCounterFrequency()
{
  uint16_t ui16_result = 0;

#if 0
  uint16_t ui16_timebase = _puiDiginTimebase;
#endif

  get_digin_freq(&ui16_result);

#if 0
  if (ui16_timebase == 0) ui16_result = 0;
  else if (ui16_timebase < (1024 * 65534L / (get_cpu_freq() * 1000)))
  { /* use standard BIOS method because timebase is short enough */
    get_digin_freq(&ui16_result);
  }
  else
  { /* use extension method */
    /* vars are accessible */
    uint16_t ui16_lastSignalAge = getCounterLastSignalAge();
    if (ui16_lastSignalAge < ui16_timebase)
    { // handle overflow between uiLast and uiAct
      uint16_t ui16_lastPeriod = _pt_diginTriggerTime.uiPeriod;
      if (ui16_lastPeriod == 0)
        ui16_result = 0;
      else
      {
        if ((ui16_lastPeriod * 2) < ui16_lastSignalAge) ui16_lastPeriod = ((int32_t)ui16_lastPeriod + (int32_t)ui16_lastSignalAge)/(int32_t)2;
        ui16_result = (10000 / ui16_lastPeriod);
      }
    }
  }
#endif
  return ui16_result;
}

uint16_t getCounterLastSignalAge()
{
  uint16_t uiResult = 0xFFFF, uiTime = (get_time() & 0xFFFF);
  uint16_t ui16_period, ui16_actTime;
  ui16_period = _pt_diginTriggerTime.uiPeriod;
  ui16_actTime = _pt_diginTriggerTime.uiAct;
  if (ui16_period <= 0xFFFE)
  { // both values are valid and not resetted
    uiResult = (uiTime >= ui16_actTime)?(uiTime - ui16_actTime): (uiTime + (0xFFFF - ui16_actTime));
    // if timebase is exceeded -> reset *puiAct and *puiLast
    if (_puiDiginTimebase < uiResult)
      _pt_diginTriggerTime.uiPeriod = 0xFFFF; // if both are equal answers in future time age 0xFFFF
  }
  return uiResult;
}

} // __HAL
