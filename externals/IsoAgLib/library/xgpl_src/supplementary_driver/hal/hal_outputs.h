/*
  hal_outputs.h: include dependent on used target (defined in
    IsoAgLib/isoaglib_config.h) the suitable HAL specific header for
    output control

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _HAL_INDEPENDENT_OUTPUTS_H_
#define _HAL_INDEPENDENT_OUTPUTS_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/hal_config.h>


namespace HAL
{
  /**
    switch main relais on or off
    @param bitState true -> Main Relais ON
  */
  void setMainRelais( bool bitState );

  /**
    define the frequency of the pwm signal
    @param bOutput PWM output channel [0..11] ([0..15] with babyboard)
        (4 sets for ESX equivalent freq for channels [4..11])
    @param dwFrequency PWM frequency in mHz [5x10^3..4,29x10^9]
    @return error state (C_NO_ERR == o.k.; C_RANGE == wrong channel OR frequency)
  */
  int16_t setPwmFreq(uint8_t bOutput, uint32_t dwFrequency);

  /**
    retrieve maximal PWM frequency -> setting to this value results in maximal output
    @param aui8_channel channel number of output [0..11] ([0..15] with babyboard)
    @return max possible PWM value
  */
  uint16_t getMaxPwmDigout(uint8_t aui8_channel);

  /**
    set pwm value 0 ... 100 %
    @param aui8_channel channel number of output [0..11] ([0..15] with babyboard)
    @param wPWMValue Value to set; depends on configured PWM freq; [0..0xFFFF]
    @return error state (C_NO_ERR == o.k.; C_RANGE == wrong channel)
  */
  int16_t setDigout(uint8_t aui8_channel, uint16_t wPWMValue);

  /** deliver the actual current of the digital output
    * @param aui8_channel channel to check [0..11] ([0..15] with babyboard)
    * @return current in [mA] ( if specified channel doesn't support current measurement, -1 is returned )
    */
  int16_t getDigoutCurrent( uint8_t aui8_channel );

  /** deliver the state of a digital output
		* This function evaluates the current where possible, otherwise it evaluates
		* the measured voltage at the output. The latter interpretation can go wrong
		* if the PWM setting is >0 but has a very low value, so that even under normal
		* conditions the voltage with connected consuming device is lower than to open
		* connector state at low level.
    * @param aui8_channel channel to check [0..11] ([0..15] with babyboard)
    * @param aui16_minCurrent minimal allowed current in [mA]
    * @param aui16_maxCurrent maximum allowed current in [mA]
    * @return HAL_NO_ERR, HAL_DIGOUT_OPEN, HAL_DIGOUT_SHORTCUT, HAL_DIGOUT_OVERTEMP,
              HAL_DIGOUT_UNDERVOLT, HAL_DIGOUT_OVERVOLT
    */
  int16_t getDigoutDiagnose(uint8_t aui8_channel, uint16_t aui16_minCurrent, uint16_t aui16_maxCurrent);
  
  /**
    deliver the measure voltage at the PWM output.
    Use this for application specific state evaluation for cases, where the standard
    getDigoutDiagnose function can go wrong.
    @return voltage at PWM channel [mV]
  */
  int16_t getDigoutAdc( uint8_t aui8_channel );
} // HAL


/// INCLUDE CONCRETE SYSTEM-HAL-IMPLEMENTATION
/// ==========================================

#ifdef HAL_PATH_SUPPLEMENTARY_OUTPUTS
#  define _hal_outputs_header_ <HAL_PATH_SUPPLEMENTARY_OUTPUTS/outputs.h>
#  include _hal_outputs_header_
#endif


#endif
