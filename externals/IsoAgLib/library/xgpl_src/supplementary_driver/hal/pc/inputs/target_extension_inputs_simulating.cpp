/*
  inputs_target_extensions.cpp:
    source for PC specific extensions for the HAL for inputs

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "inputs_target_extensions.h"
#include "inputs.h"

#include <IsoAgLib/hal/pc/system/system.h>

#include <stdlib.h>
#include <cstring>
#include <cstdio>

#ifdef _MSC_VER
#pragma warning( disable : 4996 )
#endif

using namespace std; // simple version to avoid problems with using CNAMESPACE


namespace __HAL {

struct DigitalInputLine_s
{
  int32_t val;
  int32_t freq;
};

static FILE* sensorAnalogInput[ANALOG_INPUT_MAX+1];
static FILE* sensorDigitalInput[DIGITAL_INPUT_MAX+1];
static bool sensorAnalogInputOpen[] = { false,false,false,false,false,false,false,false };
static bool sensorDigitalInputOpen[] = { false,false,false,false,false,false,false,false,
                                            false,false,false,false,false,false,false,false };

/// Analog
static int32_t lastSensorAnalogInputTime[ANALOG_INPUT_MAX+1];
static int32_t lastSensorAnalogInputVal[ANALOG_INPUT_MAX+1];

static int32_t next_sensorAnalogInputTime[ANALOG_INPUT_MAX+1];
static int32_t next_sensorAnalogInputVal[ANALOG_INPUT_MAX+1];

/// Digital
static int32_t lastSensorDigitalInputTime[DIGITAL_INPUT_MAX+1];
static DigitalInputLine_s lastSensorDigitalInputVal[DIGITAL_INPUT_MAX+1];

static int32_t next_sensorDigitalInputTime[DIGITAL_INPUT_MAX+1];
static DigitalInputLine_s next_sensorDigitalInputVal[DIGITAL_INPUT_MAX+1];


/* *************************************** */
/* ****** Sensor_c I BIOS functions  ******* */
/* *************************************** */
typedef void (*counterIrqFunction)(...);
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
static uint16_t _wIrqTime;

void counterIrqFlex(uint8_t ab_row, uint8_t ab_col)
{
  (_pulDiginCounter[ab_row][ab_col])++;
  // the config function of input must guarantee, that
  // vectors memory are allocated
  if (_pt_diginTriggerTime[ab_row] != NULL)
  {
    _put_temp = &(_pt_diginTriggerTime[ab_row][ab_col]);
    _wIrqTime = (getTime() & 0xFFFF);
    if (_wIrqTime > _put_temp->uiAct) _put_temp->uiPeriod = _wIrqTime - _put_temp->uiAct;
    else
    {
      if (_wIrqTime == _put_temp->uiAct) _put_temp->uiPeriod = 1;
      else _put_temp->uiPeriod = _wIrqTime - (0xFFFF - _put_temp->uiAct);
    }
    _put_temp->uiAct = _wIrqTime;
  }
}


void counterIrq_0(...){counterIrqFlex(0,0);}
void counterIrq_1(...){counterIrqFlex(0,1);}
void counterIrq_2(...){counterIrqFlex(0,2);}
void counterIrq_3(...){counterIrqFlex(0,3);}
void counterIrq_4(...){counterIrqFlex(1,0);}
void counterIrq_5(...){counterIrqFlex(1,1);}
void counterIrq_6(...){counterIrqFlex(1,2);}
void counterIrq_7(...){counterIrqFlex(1,3);}
void counterIrq_8(...){counterIrqFlex(2,0);}
void counterIrq_9(...){counterIrqFlex(2,1);}
void counterIrq_10(...){counterIrqFlex(2,2);}
void counterIrq_11(...){counterIrqFlex(2,3);}
void counterIrq_12(...){counterIrqFlex(3,0);}
void counterIrq_13(...){counterIrqFlex(3,1);}
void counterIrq_14(...){counterIrqFlex(3,2);}
void counterIrq_15(...){counterIrqFlex(3,3);}


static counterIrqFunction irqFuncArr[16] =
{&counterIrq_0, &counterIrq_1, &counterIrq_2, &counterIrq_3, &counterIrq_4, &counterIrq_5,
 &counterIrq_6, &counterIrq_7, &counterIrq_8, &counterIrq_9, &counterIrq_10, &counterIrq_11,
 &counterIrq_12, &counterIrq_13, &counterIrq_14, &counterIrq_15};


void
readNewLineSimulatedDigitalValues (uint8_t bInputNumber)
{
  lastSensorDigitalInputTime[bInputNumber] = next_sensorDigitalInputTime[bInputNumber];
  lastSensorDigitalInputVal[bInputNumber] = next_sensorDigitalInputVal[bInputNumber];
  char zeile[100]; zeile[100-1] = 0x00;
  if( fgets(zeile, 99, sensorDigitalInput[bInputNumber]) != NULL )
  { // read next line
    // default the next values to the last ones, in case not all columns are specified in the read line.
    next_sensorDigitalInputVal[bInputNumber] = lastSensorDigitalInputVal[bInputNumber];
    sscanf(zeile, "%u %u %u\n", &(next_sensorDigitalInputTime[bInputNumber]), &(next_sensorDigitalInputVal[bInputNumber].val), &(next_sensorDigitalInputVal[bInputNumber].freq));
  }
}

void
checkForNewSimulatedDigitalValues (uint8_t bInputNumber)
{
  if ( getTime() >= next_sensorDigitalInputTime[bInputNumber] )
  { // save next_XX to last_XX
    readNewLineSimulatedDigitalValues (bInputNumber);
  }
}


int16_t
init_digin (uint8_t bInput, uint8_t bMode, uint8_t bAktivhighlow, void (*pfFunctionName)(...))
{
  (void)bMode;
  (void)bAktivhighlow;

  // The following seems to have made problems under Visual C++
  // It shouldn't anymore with the lastest Visual Studio versions,
  // but for the case that there is a problem, the #if is kept for
  // informational reasons commented out.
  irqFuncArr[bInput] = pfFunctionName;
  //#endif
  if ( sensorDigitalInputOpen[bInput] )
    fclose(sensorDigitalInput[ bInput]);
  
  char name[100], zeile[100];

#ifdef WIN32
  sprintf(name, "..\\..\\..\\simulated_io\\digitalInput_%hu", bInput );
#else
  sprintf(name, "../../../simulated_io/digitalInput_%hu", bInput );
#endif
  sensorDigitalInput[ bInput] = fopen(name, "r");
  // BEGIN: Added by M.Wodok 6.12.04
  if (sensorDigitalInput[ bInput] == NULL) {
    // try again in current directory...
    sprintf(name, "digitalInput_%hu", bInput );
    sensorDigitalInput[ bInput] = fopen(name, "r");
  }
  if ( sensorDigitalInput[ bInput] == NULL) {
    // still not open
    sensorDigitalInputOpen[bInput] = false;
    return HAL_CONFIG_ERR;
  }
  // END: Added by M.Wodok 6.12.04

  sensorDigitalInputOpen[bInput] = true;
  char* dontcare = fgets(zeile, 99, sensorDigitalInput[bInput]);
  (void)dontcare;
  sscanf(zeile, "%u %u %u\n", &(next_sensorDigitalInputTime[bInput]), &(next_sensorDigitalInputVal[bInput].val), &(next_sensorDigitalInputVal[bInput].freq));

  readNewLineSimulatedDigitalValues (bInput);

  return HAL_NO_ERR;
}

int16_t
getDiginOnoff(uint8_t bInputNumber)
{
  if ( ! sensorDigitalInputOpen[bInputNumber] ) return HAL_CONFIG_ERR;
  checkForNewSimulatedDigitalValues (bInputNumber);
  return (lastSensorDigitalInputVal[bInputNumber].val > 0);
}

/// @todo HAL RETURN VALUES (SENSORS): return type would be int16_t, but returned are either HAL_CONFIG_ERR or bool.

int16_t
getDiginOnoffStatic(uint8_t bInputNumber)
{
  if ( ! sensorDigitalInputOpen[bInputNumber] ) return HAL_CONFIG_ERR;
  checkForNewSimulatedDigitalValues (bInputNumber);
  return (lastSensorDigitalInputVal[bInputNumber].val > 0);
}


int16_t
setDiginPrescaler(uint8_t bGroup, uint8_t bMode)
{
  (void)bGroup;
  (void)bMode;
  return HAL_NO_ERR;
}


int16_t
getDiginFreq (uint8_t bInputNumber, uint32_t *pwFrequency)
{
  if ( ! sensorDigitalInputOpen[bInputNumber] ) return HAL_CONFIG_ERR;
  checkForNewSimulatedDigitalValues (bInputNumber);
  *pwFrequency = lastSensorDigitalInputVal[bInputNumber].freq;
  return HAL_NO_ERR;
}

/* evaluation of analog channels */
int16_t
getAdc(uint8_t bKanalnummer)
{
  if ( bKanalnummer > ANALOG_INPUT_MAX ) return 1000;
  else if ( getTime() >= next_sensorAnalogInputTime[bKanalnummer] )
  { // save next_XX to last_XX
    lastSensorAnalogInputTime[bKanalnummer] = next_sensorAnalogInputTime[bKanalnummer];
    lastSensorAnalogInputVal[bKanalnummer] = next_sensorAnalogInputVal[bKanalnummer];
    char zeile[100]; zeile[100-1] = 0x00;
    if( fgets(zeile, 99, sensorAnalogInput[bKanalnummer]) != NULL )
    { // read next line
      sscanf(zeile, "%u %u\n", &(next_sensorAnalogInputTime[bKanalnummer]), &(next_sensorAnalogInputVal[bKanalnummer]));
    }
  }
  return lastSensorAnalogInputVal[bKanalnummer];
}

/* initialisation of analog inputs */
int16_t
init_analogin (uint8_t bNumber, uint8_t bType)
{
  (void)bType;

  if ( sensorAnalogInputOpen[bNumber] ) fclose(sensorAnalogInput[bNumber]);
  char name[100], zeile[100];

#ifdef WIN32
  sprintf(name, "..\\..\\..\\simulated_io\\analogInput_%hu", bNumber );
#else
  sprintf(name, "../../../simulated_io/analogInput_%hu", bNumber );
#endif
  sensorAnalogInput[ bNumber] = fopen(name, "r");
  // BEGIN: Added by M.Wodok 6.12.04
  if (sensorAnalogInput[ bNumber] == NULL) {
    // try again in current directory...
    sprintf(name, "analogInput_%hu", bNumber );
    sensorAnalogInput[ bNumber] = fopen(name, "r");
    // check if file can't be opened in current directory
    if (sensorAnalogInput[ bNumber] == NULL) {
      // still not open
      sensorAnalogInputOpen[bNumber] = false;
      return HAL_CONFIG_ERR;
    }
  }
  // END: Added by M.Wodok 6.12.04
  sensorAnalogInputOpen[bNumber] = true;
  char *dontcare = fgets(zeile, 99, sensorAnalogInput[bNumber]);
  (void)dontcare;
  sscanf(zeile, "%u %u\n", &(lastSensorAnalogInputTime[bNumber]), &(lastSensorAnalogInputVal[bNumber]));
  if( fgets(zeile, 99, sensorAnalogInput[bNumber]) != NULL )
  { // read next line
    sscanf(zeile, "%u %u\n", &(next_sensorAnalogInputTime[bNumber]), &(next_sensorAnalogInputVal[bNumber]));
  }
  return HAL_NO_ERR;
}

/* switching between fast and slow input sampling */
void setFastAnalogin (boolean bMode)
{
  (void)bMode;
}

/* evaluation of the mean value of an analog input */
int16_t
getAnaloginMean(uint8_t bInput)
{
  if ( getTime() >= next_sensorAnalogInputTime[bInput] )
  { // save next_XX to last_XX
    lastSensorAnalogInputTime[bInput] = next_sensorAnalogInputTime[bInput];
    lastSensorAnalogInputVal[bInput] = next_sensorAnalogInputVal[bInput];
    char zeile[100]; zeile[100-1] = 0x00;
    if ( (sensorAnalogInput[bInput] != NULL) && (fgets(zeile, 99, sensorAnalogInput[bInput]) != NULL) )
    { // read next line
      sscanf(zeile, "%u %u\n", &(next_sensorAnalogInputTime[bInput]), &(next_sensorAnalogInputVal[bInput]));
    }
  }
  return lastSensorAnalogInputVal[bInput];
}
/* *************************************** */
/* *** Special Extension for Counters  *** */
/* *************************************** */


/** prescaler value for digin input channels 1_4 */
static uint8_t _b_prescale_1_4Index;
/** prescaler value for digin input channels 5_16 */
static uint8_t _b_prescale_5_16Index;


/**
  configured timebase for counter channels;
  if time between two events is longer than given
  timebase, getCounterPeriod_us answers 0;
  important for timebases which are not supported by standard BIOS
*/
static uint16_t *_puiDiginTimebase[4];



int16_t init_counter(uint8_t ab_channel, uint16_t aui16_timebase, boolean ab_activHigh, boolean ab_risingEdge)
{
  int32_t i32_prescale = aui16_timebase;
  uint8_t b_codeActiv = (ab_activHigh)?HIGH_ACTIV:LOW_ACTIV;
  uint8_t b_codeEdge = (ab_risingEdge)?RISING_EDGE:FALLING_EDGE;

//  int32_t ui16_prescale = ((aui16_timebase * getCpuFreq() * 1000) / 65534);
//  uint8_t ui8_prescaleIndex
  uint8_t b_pow;
  int16_t i16_errorState;
  i32_prescale *= (getCpuFreq() * 1000);
  i32_prescale /= 65534;
  /* check if ab_channel is allowed and exit function with RANGE error if not correct */
  if (ab_channel > 15) return HAL_RANGE_ERR;
  /* configure init channel */
  init_digin(ab_channel, b_codeEdge, b_codeActiv, irqFuncArr[ab_channel]);
  if (ab_channel < 5)
  { /* standard BIOS supports two prescaler parts */
//    ui8_prescaleIndex = _b_prescale_1_4Index;
    for (b_pow = 9; b_pow > 1; b_pow--)
    { /* the prescaler must be configured by (2 << pow) values */
      if ((i32_prescale > (2 << b_pow)) || (b_pow == 2))
      { /* only change prescaler if longer than before */
        if (_b_prescale_1_4Index < (b_pow - 2))
          _b_prescale_1_4Index = (b_pow - 2);
        break;
      }
    }
    /* set prescaler */
    i16_errorState = setDiginPrescaler(RPM_IN_1_4, _b_prescale_1_4Index);
  }
  else
  { /* same for other prescaler part */
//    ui8_prescaleIndex = _b_prescale_5_16Index;
    for (b_pow = 9; b_pow > 1; b_pow--)
    {
      if ((i32_prescale > (2 << b_pow)) || (b_pow == 2))
      {
        if (_b_prescale_5_16Index < (b_pow - 2))
          _b_prescale_5_16Index = (b_pow - 2);
        break;
      }
    }
    i16_errorState = setDiginPrescaler(RPM_IN_5_16, _b_prescale_5_16Index);
  }

  /* create var for counter value -> this vars are managed in 4-groups
   *  of int32_t -> avoid memory waste
   */
  if (_pulDiginCounter[(ab_channel / 4)] == NULL)
  { /* according 4-group of uint32_t isn�t created -> allocate */
    _pulDiginCounter[(ab_channel / 4)] = (uint32_t*) malloc(4*sizeof(uint32_t));
    /* check if allocated properly and init */
    if (_pulDiginCounter[(ab_channel / 4)] == NULL) i16_errorState |= HAL_OVERFLOW_ERR;
    else memset(_pulDiginCounter[(ab_channel / 4)], 0, sizeof(uint32_t)*4);
  }

  /* store given timebase in according 4-group */
  if (_puiDiginTimebase[(ab_channel / 4)] == NULL)
  {
    _puiDiginTimebase[(ab_channel / 4)] = (uint16_t*) malloc(4*sizeof(uint16_t));
    /* check if allocated properly and init */
    if (_puiDiginTimebase[(ab_channel / 4)] == NULL) i16_errorState |= HAL_OVERFLOW_ERR;
    else
    {
      memset(_puiDiginTimebase[(ab_channel / 4)], 0, sizeof(uint16_t) * 4);
      _puiDiginTimebase[(ab_channel / 4)][(ab_channel % 4)] = aui16_timebase;
    }
  }

  if (i32_prescale > 1024)
  { /* standard BIOS frequency and period methods doesn�t fir for
     * the wanted timebase -> use extension functions -> allocate needed vars
     */
    if (_pt_diginTriggerTime[(ab_channel / 4)] == NULL)
    {  /* according 4-group of t_triggerNode isn�t created -> allocate */
      _pt_diginTriggerTime[(ab_channel / 4)] = (t_triggerNode*) malloc(4*sizeof(t_triggerNode));
      if (_pt_diginTriggerTime[(ab_channel / 4)] == NULL) i16_errorState |= HAL_OVERFLOW_ERR;
      else memset(_pt_diginTriggerTime[(ab_channel / 4)], 0, sizeof(t_triggerNode) * 4);
    }
  }

  return i16_errorState;
}

uint32_t getCounter(uint8_t ab_channel)
{
  /* check if ab_channel is allowed and var array is allocated */
  if ((ab_channel < 16) && (_pulDiginCounter[(ab_channel / 4)] != NULL))
  {
    return _pulDiginCounter[(ab_channel / 4)][(ab_channel % 4)];
  }
  else return 0;
}

int16_t resetCounter(uint8_t ab_channel)
{
  /* check if ab_channel is allowed and var array is allocated */
  if ((ab_channel < 16) && (_pulDiginCounter[(ab_channel / 4)] != NULL))
  {
    _pulDiginCounter[(ab_channel / 4)][(ab_channel % 4)] = 0;
    return HAL_NO_ERR;
  }
  else return HAL_RANGE_ERR;
}

uint32_t getCounterPeriod_us(uint8_t ab_channel)
{
  uint16_t ui16_timebase;
  uint32_t ui32_result = 0xFFFFFFFFUL;
  /* check if ab_channel is allowed and var array is allocated */
  if (ab_channel > 15) return 0xFFFFFFFFUL;
  if (_puiDiginTimebase[(ab_channel / 4)] != NULL)
  {
    ui16_timebase = _puiDiginTimebase[(ab_channel / 4)][(ab_channel % 4)];
    if (ui16_timebase == 0) ui32_result = 0xFFFFFFFFUL;
    else if (ui16_timebase < (1024 * 65534 / (getCpuFreq() * 1000)))
    { /* use standard BIOS method because timebase is short enough */
      ui32_result = ((getTime()%10000 +ab_channel)) / 3;

      if (ab_channel < 5)
      {
        ui32_result = __IsoAgLib::mul1Div1Mul2Div2((2 << (_b_prescale_1_4Index + 2)), getCpuFreq(), ui32_result, 1 );
      }
      else
      {
        ui32_result = __IsoAgLib::mul1Div1Mul2Div2((2 << (_b_prescale_5_16Index + 2)), getCpuFreq(), ui32_result, 1 );
      }
    }
    else
    { /* use extension method */
      if (_pt_diginTriggerTime[(ab_channel / 4)] != NULL)
      { /* vars are accessible */
        if (getCounterLastSignalAge(ab_channel) < ui16_timebase)
        { // handle overflow between uiLast and uiAct
          ui32_result = _pt_diginTriggerTime[(ab_channel / 4)][(ab_channel % 4)].uiPeriod;
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
    else if (ui16_timebase < (1024 * 65534 / (getCpuFreq() * 1000)))
    { /* use standard BIOS method because timebase is short enough */
      uint32_t ui32_result = 0;
      getDiginFreq(ab_channel, &ui32_result);
      ui16_result = (uint16_t)ui32_result; // for now okay.
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
            if ((ui16_lastPeriod * 2) < ui16_lastSignalAge) ui16_lastPeriod = ((int32_t)ui16_lastPeriod + (int32_t)ui16_lastSignalAge)/(int32_t)2;
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
  uint16_t uiResult = 0xFFFF, uiTime = (getTime() & 0xFFFF);
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
