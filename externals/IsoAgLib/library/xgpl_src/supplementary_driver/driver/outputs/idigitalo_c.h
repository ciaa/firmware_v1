/*
  idigital_o.h:
    header file for iDigital_O_c, an object for digital output

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IDIGITAL_O_H
#define IDIGITAL_O_H

#include "impl/digitalo_c.h"


namespace IsoAgLib {

/**
  Output object for simple digital output;
  @see Outputs_c
  @see OutputBase_c
  @author Dipl.-Inform. Achim Spangler
*/
class iDigitalO_c : private __IsoAgLib::DigitalO_c {
public:
  /** enum for error states of digital output
    * values: noDoutErr, dout_openErr, dout_shortcutErr,
    *         dout_overvoltErr, dout_untervoltErr
    */
  typedef enum { noDoutErr, dout_openErr, dout_shortcutErr } dout_err_t;

  iDigitalO_c(uint8_t aui8_channel)
    : DigitalO_c(aui8_channel)
  {}

  iDigitalO_c()
    : DigitalO_c()
  {}

  virtual ~iDigitalO_c() {}

  /** Channel-management */
  void setChannel(uint8_t aui8_channel) { DigitalO_c::setChannel( aui8_channel ); }
  uint8_t channelNr() const { return DigitalO_c::channelNr(); }

  /**
    set the output PWM frequency
    @param aui32_val value to use as PWM frequency in [mHz]
  */
  void setFreq(uint32_t aui32_val){ DigitalO_c::setFreq(aui32_val); }

  /**
    set the output PWM value
    @param aui16_val value to set for the output channel
  */
  void set(uint16_t aui16_val) { DigitalO_c::set(aui16_val); }

  /**
    switch PWM to total OFF or ON (simple switch function)
    @param ab_state
  */
  void set(bool ab_state) { DigitalO_c::set(ab_state); }

  /**
    deliver actual set value
    @return last set value [0..0xffff]
  */
  uint16_t get()const{ return DigitalO_c::get(); }

  /**
    * set the allowed current limits for active state
    * @param aui16_minActiveCurrent minimal allowed current in active state
    * @param aui16_maxActiveCurrent maximum allowed current in active state
    */
  void setActiveCurrentLimits( uint16_t aui16_minActiveCurrent, uint16_t aui16_maxActiveCurrent )
  { DigitalO_c::setActiveCurrentLimits(aui16_minActiveCurrent,aui16_maxActiveCurrent); }

  /** deliver the actual current of the digital output
    * @return current in [mA] ( if this digital out instance doesn't support current measurement, -1 is returned )
    */
  inline int16_t getDigoutCurrent( void ) const { return DigitalO_c::getDigoutCurrent();}

  /** check if this digital output channel is in FAIL condition
    * @return true -> channel is in fail condition
    */
  bool operator!( void ) const { return DigitalO_c::operator!();}

  /** check if this digital output channel is in GOOD condition
    * @return true -> channel is in GOOD condition
    */
  bool good( void ) const { return DigitalO_c::good();}

  /** deliver detailed error state information for this Digital Output
    * This function evaluates the current where possible, otherwise it evaluates
    * the measured voltage at the output. The latter interpretation can go wrong
    * if the PWM setting is >0 but has a very low value, so that even under normal
    * conditions the voltage with connected consuming device is lower than to open
    * connector state at low level.
    * @return dout_err_t [noDoutErr|dout_openErr|dout_shortcutErr]
    */
  iDigitalO_c::dout_err_t getState( void ) const { return iDigitalO_c::dout_err_t((uint16_t)DigitalO_c::getState());}

  /** deliver the measure voltage at the PWM output.
    Use this for application specific state evaluation for cases, where the standard
    getDigoutDiagnose function can go wrong.
    @return voltage at PWM output [mV]
  */
  int16_t getDigoutAdc( void ) const { return DigitalO_c::getDigoutAdc(); }

  /** deliver the max allowed PWM -> setting this value results in max output
    @return max allowed PWM for the current PWM frequency setting
  */
  uint16_t getMaxOutputPwmFreq() const { return DigitalO_c::getMaxOutputPwmFreq(); }

private:
  iDigitalO_c(const iDigitalO_c& /*acrc_src*/); // intentionally private and unimplemented
  iDigitalO_c& operator=(const iDigitalO_c& /*acrc_src*/); // intentionally private and unimplemented

  friend class iOutputs_c;
};

}
#endif
