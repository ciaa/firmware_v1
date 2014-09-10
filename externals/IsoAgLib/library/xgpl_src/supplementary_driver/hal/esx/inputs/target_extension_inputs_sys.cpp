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
#include <IsoAgLib/hal/esx/config.h>
#include <IsoAgLib/util/impl/util_funcs.h>

#include <cstdlib>

/** acivate the following define, if the signal period shall be
    calculated from get_digin_freq() as the BIOS function
		get_digin_period() is very unprecise as long as the
		chosen timebase is not 80 times larger than the signal period.
		--> the better precision is worth while the more calculation afford
    NOTE: Currently hard-set in here by default.
	*/
#define CONFIG_HAL_ESX_CALC_PERDIOD_FROM_FREQ

namespace __HAL {
extern "C" {
  /** include the BIOS specific header with the part for CAN into __HAL */
  #include <commercial_BIOS/bios_esx/Xos20esx.h>
}


using namespace std; // simple version to avoid problems with using CNAMESPACE


/** prescaler value for digin input channels 1_4 */
static uint8_t _b_prescale_1_4Index;

/** prescaler value for digin input channels 5_16 */
static uint8_t _b_prescale_5_16Index;
/**
  array of pointers to uint32_t arrays with counter
  values; avoid memory waste, if only some channels are
  used for counting
*/
static uint32_t *_pulDiginCounter[4];

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
static t_triggerNode *_pt_diginTriggerTime[4];

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
static uint16_t *_puiDiginTimebase[4];

typedef void (*counterIrqFunction)();
static uint16_t _wIrqTime;

void counterIrqFlex(uint8_t ab_row, uint8_t ab_col)
{
  (_pulDiginCounter[ab_row][ab_col])++;
  // the config function of input must guarantee, that
  // vectors memory are allocated
  if (_pt_diginTriggerTime[ab_row] != NULL)
  {
    _put_temp = &(_pt_diginTriggerTime[ab_row][ab_col]);
    _wIrqTime = (get_time() & 0xFFFF);
    if (_wIrqTime > _put_temp->uiAct) _put_temp->uiPeriod = _wIrqTime - _put_temp->uiAct;
    else if (_wIrqTime == _put_temp->uiAct) _put_temp->uiPeriod = 1;
		else _put_temp->uiPeriod = _wIrqTime - (0xFFFF - _put_temp->uiAct);

    _put_temp->uiAct = _wIrqTime;
  }
}

void counterIrq_0(){counterIrqFlex(0,0);}
void counterIrq_1(){counterIrqFlex(0,1);}
void counterIrq_2(){counterIrqFlex(0,2);}
void counterIrq_3(){counterIrqFlex(0,3);}
void counterIrq_4(){counterIrqFlex(1,0);}
void counterIrq_5(){counterIrqFlex(1,1);}
void counterIrq_6(){counterIrqFlex(1,2);}
void counterIrq_7(){counterIrqFlex(1,3);}
void counterIrq_8(){counterIrqFlex(2,0);}
void counterIrq_9(){counterIrqFlex(2,1);}
void counterIrq_10(){counterIrqFlex(2,2);}
void counterIrq_11(){counterIrqFlex(2,3);}
void counterIrq_12(){counterIrqFlex(3,0);}
void counterIrq_13(){counterIrqFlex(3,1);}
void counterIrq_14(){counterIrqFlex(3,2);}
void counterIrq_15(){counterIrqFlex(3,3);}




static counterIrqFunction irqFuncArr[16] =
{&counterIrq_0, &counterIrq_1, &counterIrq_2, &counterIrq_3, &counterIrq_4, &counterIrq_5,
 &counterIrq_6, &counterIrq_7, &counterIrq_8, &counterIrq_9, &counterIrq_10, &counterIrq_11,
 &counterIrq_12, &counterIrq_13, &counterIrq_14, &counterIrq_15};

/**
  helper function to get max period between counted events, so that
  BIOS/OS internal functions for freq,period detection can be used
  */
uint16_t get_max_bios_timebase( void )
{
  static uint16_t sui16_limit = (1024 * 65534 / (get_cpu_freq() * 1000));
  return sui16_limit;
}

int16_t init_counter(uint8_t ab_channel, uint16_t aui16_timebase, bool ab_activHigh, bool ab_risingEdge)
{
  int32_t i32_prescale = aui16_timebase;
  uint8_t b_codeActiv = (ab_activHigh)?HIGH_ACTIV:LOW_ACTIV;
  uint8_t b_codeEdge = (ab_risingEdge)?RISING_EDGE:FALLING_EDGE;

  uint8_t ui8_pow;
  int16_t i16_errorState = 0;
  /* check if ab_channel is allowed and exit function with RANGE error if not correct */
  if (ab_channel > 15) return C_RANGE;


  /* store given timebase in according 4-group */
  if (_puiDiginTimebase[(ab_channel / 4)] == NULL)
  {
    _puiDiginTimebase[(ab_channel / 4)] = (uint16_t*)malloc(4*sizeof(uint16_t));
    /* check if allocated properly and init */
    if (_puiDiginTimebase[(ab_channel / 4)] == NULL) i16_errorState |= C_OVERFLOW;
    else
    {
      CNAMESPACE::memset(_puiDiginTimebase[(ab_channel / 4)], 0, sizeof(uint16_t) * 4);
      _puiDiginTimebase[(ab_channel / 4)][(ab_channel % 4)] = aui16_timebase;
    }
  }

  i32_prescale *= (get_cpu_freq() * 1000);
  i32_prescale /= 65534;
  if (i32_prescale <= 1024)
  { // use BIOS for period/freq detection
    init_digin(ab_channel, b_codeEdge, b_codeActiv, NULL );
    uint16_t ui16_tempPrescaleInd = (ab_channel < 5)?_b_prescale_1_4Index:_b_prescale_5_16Index;
    for (ui8_pow = 9; ui8_pow > 1; ui8_pow--)
    { /* the prescaler must be configured by (2 << pow) values */
      if ((i32_prescale > (2 << ui8_pow)) || (ui8_pow == 2))
      { /* only change prescaler if longer than before */
        if (ui16_tempPrescaleInd < (ui8_pow - 2)) ui16_tempPrescaleInd = (ui8_pow - 2);
        break;
      }
    }

    if (ab_channel < 5)
    { /* set prescaler */
      _b_prescale_1_4Index = ui16_tempPrescaleInd;
      i16_errorState = set_digin_prescaler(RPM_IN_1_4, _b_prescale_1_4Index);
    }
    else
    { /* same for other prescaler part */
      _b_prescale_5_16Index = ui16_tempPrescaleInd;
      i16_errorState = set_digin_prescaler(RPM_IN_5_16, _b_prescale_5_16Index);
    }
  }
  else
  { // the max time between impulses that shall be gathere for
    // period detection is larger than supported by BIOS
    // ==> use IsoAgLib IRQ based detection
    init_digin(ab_channel, b_codeEdge, b_codeActiv, irqFuncArr[ab_channel]);

    /* create var for counter value -> this vars are managed in 4-groups
     *  of int32_t -> avoid memory waste
     */
    if (_pulDiginCounter[(ab_channel / 4)] == NULL)
    { /* according 4-group of uint32_t isn't created -> allocate */
      _pulDiginCounter[(ab_channel / 4)] = (uint32_t*)malloc(4*sizeof(uint32_t));
      /* check if allocated properly and init */
      if (_pulDiginCounter[(ab_channel / 4)] == NULL) i16_errorState |= C_OVERFLOW;
      else CNAMESPACE::memset(_pulDiginCounter[(ab_channel / 4)], 0, sizeof(uint32_t)*4);
    }

    // create corresponding array elements for last trigger time
    if (_pt_diginTriggerTime[(ab_channel / 4)] == NULL)
    {  /* according 4-group of t_triggerNode isn't created -> allocate */
      _pt_diginTriggerTime[(ab_channel / 4)] = (t_triggerNode*)malloc(4*sizeof(t_triggerNode));
      if (_pt_diginTriggerTime[(ab_channel / 4)] == NULL) i16_errorState |= C_OVERFLOW;
      else CNAMESPACE::memset(_pt_diginTriggerTime[(ab_channel / 4)], 0, sizeof(t_triggerNode) * 4);
    }
  }

  return i16_errorState;
}

uint32_t getCounter(uint8_t ab_channel)
{
  if ((ab_channel < 16) && (_pulDiginCounter[(ab_channel / 4)] != NULL))
  { /* ab_channel is allowed and var array is allocated */
    return _pulDiginCounter[(ab_channel / 4)][(ab_channel % 4)];
  }
  else
  { // this can happen if counter is not yet initialized
    // or if wanted timebase allows use of BIOS for
    // period and freq detection
    return 0;
  }
}

int16_t resetCounter(uint8_t ab_channel)
{
  /* check if ab_channel is allowed and var array is allocated */
  if ((ab_channel < 16) && (_pulDiginCounter[(ab_channel / 4)] != NULL))
  {
    _pulDiginCounter[(ab_channel / 4)][(ab_channel % 4)] = 0;
    return C_NO_ERR;
  }
  else
  { // this can happen if counter is not yet initialized
    // or if wanted timebase allows use of BIOS for
    // period and freq detection
    return C_RANGE;
  }
}

uint32_t getCounterPeriod_us(uint8_t ab_channel)
{
  #ifndef CONFIG_HAL_ESX_CALC_PERDIOD_FROM_FREQ
  uint16_t ui16_counter;
  #endif
  uint16_t ui16_timebase, ui16_result = 0xFFFF;
  uint32_t ui32_result = 0xFFFFFFFFUL;
  /* check if ab_channel is allowed and var array is allocated */
  if (ab_channel > 15) return 0xFFFF;
  if (_puiDiginTimebase[(ab_channel / 4)] != NULL)
  {
    ui16_timebase = _puiDiginTimebase[(ab_channel / 4)][(ab_channel % 4)];
    if (ui16_timebase == 0) ui32_result = 0xFFFF;
    else if ( ui16_timebase <= get_max_bios_timebase() )
    { /* use standard BIOS method because timebase is short enough */
      #ifdef CONFIG_HAL_ESX_CALC_PERDIOD_FROM_FREQ
      get_digin_freq(ab_channel, &ui16_result);
      // result of get_digin_freq has scaling [100mHz] -> use 10000/result to get period [usec]
      ui32_result = ( ( 1000000UL * 10 ) / ui16_result );
      #else
      get_digin_period(ab_channel, &ui16_result, &ui16_counter);
      const uint16_t cui_usePrescaleInd = (ab_channel < 5)?_b_prescale_1_4Index:_b_prescale_5_16Index;

      // use util helper function to avoid overflow
      ui32_result = __IsoAgLib::mul1Div1Mul2Div2((2 << (cui_usePrescaleInd + 2)), get_cpu_freq(), ui16_result, 1 );
      #endif
    }
    else
    { /* use extension method */
      if (_pt_diginTriggerTime[(ab_channel / 4)] != NULL)
      { /* vars are accessible */
        if (getCounterLastSignalAge(ab_channel) < ui16_timebase)
        { // handle overflow between uiLast and uiAct
          ui32_result = _pt_diginTriggerTime[(ab_channel / 4)][(ab_channel % 4)].uiPeriod * 1000;
          if (ui32_result == 0) ui32_result = 1;
        }
      }
    }
  }
  return ui32_result;
}

uint32_t getCounterFrequency(uint8_t ab_channel)
{
  uint16_t ui16_timebase, ui16_result = 0;
  uint16_t ui16_lastSignalAge = 0;
  uint16_t ui16_lastPeriod;
  /* check if ab_channel is allowed and var array is allocated */
  if ((ab_channel < 16) && (_puiDiginTimebase[(ab_channel / 4)] != NULL))
  {
    ui16_timebase = _puiDiginTimebase[(ab_channel / 4)][(ab_channel % 4)];
    if (ui16_timebase == 0) ui16_result = 0;
    else if ( ui16_timebase <= get_max_bios_timebase() )
    { /* use standard BIOS method because timebase is short enough */
      get_digin_freq(ab_channel, &ui16_result);
    }
    else
    { /* use extension method */
      if (_pt_diginTriggerTime[(ab_channel / 4)] != NULL)
      { /* vars are accessible */
        ui16_lastSignalAge = getCounterLastSignalAge(ab_channel);
        if (ui16_lastSignalAge < ui16_timebase)
        { // handle overflow between uiLast and uiAct
          ui16_lastPeriod = _pt_diginTriggerTime[(ab_channel / 4)][(ab_channel % 4)].uiPeriod;
          if (ui16_lastPeriod == 0) ui16_result = 0;
          else
          {
            if ((ui16_lastPeriod * 2) < ui16_lastSignalAge)
              ui16_lastPeriod = ((int32_t)ui16_lastPeriod + (int32_t)ui16_lastSignalAge)/(int32_t)2;
            ui16_result = (10000 / ui16_lastPeriod);
          }
        }
      }
    }
  }
  return ui16_result;
}

uint16_t getCounterLastSignalAge(uint8_t ab_channel)
{
  uint16_t uiResult = 0xFFFF, uiTime = (get_time() & 0xFFFF);
  uint16_t ui16_period, ui16_actTime;
  /* check if ab_channel is allowed and var array is allocated */
  if ((ab_channel < 16) && (_pt_diginTriggerTime[(ab_channel / 4)] != NULL))
  {
    ui16_period = _pt_diginTriggerTime[(ab_channel / 4)][(ab_channel % 4)].uiPeriod;
    ui16_actTime = _pt_diginTriggerTime[(ab_channel / 4)][(ab_channel % 4)].uiAct;
    if (ui16_period <= 0xFFFE)
    { // both values are valid and not resetted
      uiResult = (uiTime >= ui16_actTime)?(uiTime - ui16_actTime): (uiTime + (0xFFFF - ui16_actTime));
      // if timebase is exceeded -> reset *puiAct and *puiLast
      if (_puiDiginTimebase[(ab_channel / 4)][(ab_channel % 4)] < uiResult)
        _pt_diginTriggerTime[(ab_channel / 4)][(ab_channel % 4)].uiPeriod = 0xFFFF; // if both are equal answers in future time age 0xFFFF
    }
  }
  return uiResult;
}

} // __HAL
