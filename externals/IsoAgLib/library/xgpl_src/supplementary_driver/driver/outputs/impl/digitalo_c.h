/*
  digitalo_c.h
    header file for DigitalO_c, an object for digital output

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef DIGITAL_O_H
#define DIGITAL_O_H

#include "outputbase_c.h"


// Begin Namespace __IsoAgLib
namespace __IsoAgLib {
/**
  Output object for simple digital output;
  @see Outputs_c
  @see OutputBase_c
  @author Dipl.-Inform. Achim Spangler
*/
class DigitalO_c : public OutputBase_c  {
public:
  /** enum for error states of digital output */
  typedef enum { noDoutErr, dout_openErr, dout_shortcutErr } dout_err_t;

  DigitalO_c(uint8_t aui8_channel);
  DigitalO_c();
  virtual ~DigitalO_c();

  /** compare this channel to an uint8_t value representing a dout channel
    * this function is used by the base Singleton to find a specific item
    */
  bool operator==( uint8_t aui8_channel ) const { return ( aui8_channel == channelNr() );}

  /**
    set the output PWM frequency
    (uses BIOS function)

    possible errors:
       * Err_c::range wrong output channel number or wrong frequency
    @param aui32_val value to use as PWM frequency in [mHz]
  */
  void setFreq(uint32_t aui32_val);

  /**
    set the output PWM value
    (uses BIOS function)

    possible errors:
        * Err_c::range wrong output channel number
    @param aui16_val value to set for the output channel [0..0xffff]
  */
  void set(uint16_t aui16_val);

  /**
    switch PWM to total OFF or ON (simple switch function)
    (uses BIOS function)

    possible errors:
       * Err_c::range wrong output channel number
    @param ab_state
  */
  void set(bool ab_state);

  /**
    deliver actual set value
    @return last set value [0..0xffff]
  */
  uint16_t get()const{return ui16_value;}

  /**
    * set the allowed current limits for active state
    * @param aui16_minActiveCurrent minimal allowed current in active state
    * @param aui16_maxActiveCurrent maximum allowed current in active state
    */
  void setActiveCurrentLimits( uint16_t aui16_minActiveCurrent, uint16_t aui16_maxActiveCurrent )
  {ui16_minAllowedCurrent = aui16_minActiveCurrent; ui16_maxAllowedCurrent = aui16_maxActiveCurrent;}

  /** deliver the actual current of the digital output
    * @return current in [mA] ( if this digital out instance doesn't support current measurement, -1 is returned )
    */
  inline int16_t getDigoutCurrent( void ) const { return HAL::getDigoutCurrent( channelNr() );}

  /** check if this digital output channel is in FAIL condition
    * @return true -> channel is in fail condition
    */
  bool operator!( void ) const { return (!good());}

  /** check if this digital output channel is in GOOD condition
    * @return true -> channel is in GOOD condition
    */
  bool good( void ) const;

  /** deliver detailed error state information for this Digital Output
    * This function evaluates the current where possible, otherwise it evaluates
    * the measured voltage at the output. The latter interpretation can go wrong
    * if the PWM setting is >0 but has a very low value, so that even under normal
    * conditions the voltage with connected consuming device is lower than to open
    * connector state at low level.
    * @return dout_err_t [noDoutErr|dout_openErr|dout_shortcutErr]
    */
  dout_err_t getState( void ) const;

  /** deliver the measure voltage at the PWM output.
    Use this for application specific state evaluation for cases, where the standard
    getDigoutDiagnose function can go wrong.
    @return voltage at PWM output [mV]
  */
  int16_t getDigoutAdc( void ) const { return HAL::getDigoutAdc( channelNr() );}

  /** deliver the max allowed PWM -> setting this value results in max output
    @return max allowed PWM for the current PWM frequency setting
  */
  uint16_t getMaxOutputPwmFreq() const
  { return ui16_maxOutputPwmFreq; }

private:
  DigitalO_c(const DigitalO_c&); // intentionally private and unimplemented
  DigitalO_c& operator=(const DigitalO_c&); // intentionally private and unimplemented

  uint16_t ui16_value;

  /** minimal allowed current in active state */
  uint16_t ui16_minAllowedCurrent;

  /** maximum allowed current in active state */
  uint16_t ui16_maxAllowedCurrent;

  /** max allowed PWM freq -> setting this value results in max output */
  uint16_t ui16_maxOutputPwmFreq;
};

} // __IsoAgLib
#endif
