/*
  hal_inputs.h: include dependent on used target (defined in
    IsoAgLib/isoaglib_config.h) the suitable HAL specific header for
    data input

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _HAL_INDEPENDENT_INPUTS_H_
#define _HAL_INDEPENDENT_INPUTS_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/hal_config.h>


namespace HAL
{
  /**
    initialize one of the [0..7] analog input channels to VOLTAGE input
    @param bNumber number of the analog input channel
    @return error state (C_NO_ERR == o.k.)
  */
  int16_t init_analoginVolt(uint8_t bNumber);

  /**
    initialize one of the [0..7] analog input channels to CURRENT input
    @param bNumber number of the analog input channel
    @return error state (C_NO_ERR == o.k.)
  */
  int16_t init_analoginCurrent(uint8_t bNumber);

  /**
    initialize one of the [0..15] digital input channels
    @param ab_channel number of the digital input channel
    @param bMode input mode {DIGIN, RISING_EDGE, FALLING_EDGE, BOTH_EDGE}
    @param bAktivhighlow {HIGH_ACTIV, LOW_ACTIV}
    @param pfFunctionName adress of function which is called on input events (NULL -> none)
    @return error state (C_NO_ERR == o.k.)
  */
  int16_t init_digin(uint8_t ab_channel,uint8_t bMode,uint8_t bAktivhighlow,void (*pfFunctionName)(IRQ_FUNC_PAR));

  /**
    init counter for trigger events on digital inoput;
    rising edges are counted;
    @param ab_channel input channel to use [0..15]
    @param aui16_timebase timebase to calculate periods, frequency
                       should be at least longer than longest
                       awaited signal period [msec.]
    @param ab_activHigh true -> counter input is configured fo ACTIV_HIGH; else ACTIV_LOW
    @param ab_risingEdge true -> counter triggers on rising edge; else on falling edge
    @return C_NO_ERR if no error occured
  */
  int16_t init_counter(uint8_t ab_channel, uint16_t aui16_timebase, bool ab_activHigh, bool ab_risingEdge);

  /**
    get counter value of an digital counter input
    @param ab_channel channel of counter [0..15]
    @return counter events since init or last reset
  */
  uint32_t getCounter(uint8_t ab_channel);

  /**
    reset the given counter
    @param ab_channel channel of counter [0..15]
    @return C_NO_ERR ; C_RANGE if counter for ab_channel isn't configured properly
  */
  int16_t resetCounter(uint8_t ab_channel);

  /**
    get period of counter channel
    @param ab_channel channel of counter [0..15]
    @return time between last two signals in microseconds
            or 0xFFFFFFFF if time is longer than initially given timebase
  */
  uint32_t getCounterPeriod_us(uint8_t ab_channel);

  /**
    get frequency of counter channel
    @param ab_channel channel of counter [0..15]
    @return frequency calculated from time between last two signals
            or 0 if time is longer than initially given timebase
  */
  uint32_t getCounterFrequency(uint8_t ab_channel);

  /**
   get time since last signal
   @param ab_channel channel of counter
   @return time since last signal [msec.]
  */
  uint32_t getCounterLastSignalAge(uint8_t ab_channel);

  /**
    set fast ADC mode ON or OFF
    (important: only the 8 analog inputs are measured during this time,
      --> the other system voltages aren't measured with active fast ADC)
    @param bMode set fast ADC to ON or OFF
  */
  void setFastAnalogin(bool bMode);

  /**
    get the measured voltage value of a channel in [mV]
    @param ab_channel measured channel
    @return voltage [0..8500] [mV] or C_RANGE on wrong input channel number
  */
  int16_t getAdcVoltage(uint8_t ab_channel);

  /**
    get the MEDIUM of measured voltage value of a channel in [mV]
    @param ab_channel measured channel
    @return voltage [0..8500] [mV] or C_RANGE on wrong input channel number
  */
  int16_t getAdcMeanVoltage(uint8_t ab_channel);

  /**
    get the measured current value of a channel in [uA]
    @param ab_channel measured channel
    @return current [4000..20000] [uA] or C_RANGE on wrong input channel number
  */
  int16_t getAdcCurrent(uint8_t ab_channel);

  /**
    get the MEDIUM of measured current value of a channel in [uA]
    @param ab_channel measured channel
    @return current [4000..20000] [uA] or C_RANGE on wrong input channel number
  */
  int16_t getAdcMeanCurrent(uint8_t ab_channel);

  /**
    get the diagnose ADC value from specified DIGITAL INPUT channel
    @param ab_channel channel number [0..15]
    @return ADC diagnose voltage [ or C_RANGE on wrong input channel number
  */
  int16_t getDiginDiagnoseAdc(uint8_t ab_channel);

  /**
    get the connection state of case mass
    @return true == case is connected to ground
  */
  bool getAdcVirtMass();

  /**
    get the temperature
    @return temperature in degree [-40..85]
  */
  int16_t getAdcTemp();

  /**
    get the threshold value
    @return threshold voltage [mV]
  */
  int16_t getAdcThreshold();

  /**
    get the voltage at the PIC
    @return PIC voltage [mV]
  */
  int16_t getAdcPic();

  /**
    get the complete sum of all currents
    @return sum of all currents [mA]
  */
  int16_t getAdcCurrentSum();

  /**
    deliver state of digital input based on Activ-High/Low setting
    (evalutation of input signals independent from switching type)
    @param ab_channelNumber input channel number [DIN1..DIN16]
    @return ON, OFF or C_RANGE
  */
  int16_t getDiginOnoff(uint8_t ab_channelNumber);

  /**
    deliver debounced state of digital input based on Activ-High/Low setting
    (evalutation of input signals independent from switching type)
    @param ab_channelNumber input channel number [DIN1..DIN16]
    @return ON, OFF or C_RANGE
  */
  int16_t getDiginOnoffStatic(uint8_t ab_channelNumber);
} // HAL


/// INCLUDE CONCRETE SYSTEM-HAL-IMPLEMENTATION
/// ==========================================

#ifdef HAL_PATH_SUPPLEMENTARY_INPUTS
#  define _hal_inputs_header_ <HAL_PATH_SUPPLEMENTARY_INPUTS/inputs.h>
#  include _hal_inputs_header_
#endif


#endif
