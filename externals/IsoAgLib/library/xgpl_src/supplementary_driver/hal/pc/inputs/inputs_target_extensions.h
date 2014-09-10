/*
  inputs_target_extensions.h:
    header for PC specific extensions for the HAL for inputs

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _PC_INPUTS_TARGET_EXTENSIONS_H_
#define _PC_INPUTS_TARGET_EXTENSIONS_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/pc/typedef.h>


namespace __HAL {

/* ******************************************************** */
/**
 * \name Specific Functions
 */
/* ******************************************************** */
/*@{*/
#define DIGIN                  0   /* = 0; digital inputg*/
#define RISING_EDGE            1   /* = 1; interrupt on rising edge*/
#define FALLING_EDGE           2   /* = 2; interrupt on falling edge*/
#define BOTH_EDGE              3   /* = 3; interrupt on both edges*/
#define HIGH_ACTIV             1   /* = 1; input for aktiv high signals */
#define LOW_ACTIV              0   /* = 0; input for aktiv low signals */
#define  GET_U_C               35        /* UC (Boardspannung)   */
#define  GET_U_EXT_8_5_V       15        /* U 8,5 V EXT */
#define RPM_IN_1_4             0   /* = 0; group of inputs 1 to 4 */
#define RPM_IN_5_16            1   /* = 1; group of inputs 5 to 16 */
#define  VOLTAGE_IN               0       /* Define to configure the input as voltage input */
#define  CURRENT_IN               1       /* Define to configure the input as current input */
#define  GET_M_VIRT            16        /* Virtuelle Masse */
#define  GET_D_T               17        /* Temperaturaenderung DT */
#define  GET_U_THRESHOLD       80        /* U 2,3 V   */
#define  GET_U_PIC             32        /* Spannungsversg. PIC, 5V   */
#define  GET_SUM_I             1         /* Messung Summenstrom       */

int16_t  init_digin(uint8_t bInput,uint8_t bMode,uint8_t bAktivhighlow,void (*pfFunctionName)(...));
int16_t  getDiginOnoff(uint8_t bInputNumber);
int16_t  getDiginOnoffStatic(uint8_t bInputNumber);
int16_t  setDiginPrescaler(uint8_t bGroup, uint8_t bMode);
int16_t  getDiginFreq(uint8_t bInput, uint32_t *pwFrequency);


int16_t  getAdc(uint8_t bKanalnummer);              /* evaluation of analog channels */
int16_t  init_analogin(uint8_t bNumber, uint8_t bType); /* initialisation of analog inputs */
void setFastAnalogin(boolean bMode);        /* switching between fast and slow input sampling */
int16_t  getAnaloginMean(uint8_t bInput);          /* evaluation of the mean value of an analog input */

inline uint8_t getAnaloginCheckNr(uint8_t ab_channel)
{ return (ab_channel); } // for simulation, directly map the channels

inline uint8_t getDiginAdcCheckNr(uint8_t ab_channel)
{ return (ab_channel); } // for simulation, directly map the channels

int16_t init_counter(uint8_t ab_channel, uint16_t aui16_timebase, boolean ab_activHigh, boolean ab_risingEdge);
uint32_t getCounter(uint8_t ab_channel);
int16_t resetCounter(uint8_t ab_channel);
uint32_t getCounterPeriod_us(uint8_t ab_channel);
uint32_t getCounterFrequency(uint8_t ab_channel);
uint16_t getCounterLastSignalAge(uint8_t ab_channel);

/*@}*/

} // __HAL

#endif
