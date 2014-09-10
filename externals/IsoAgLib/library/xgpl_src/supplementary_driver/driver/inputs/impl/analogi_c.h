/*
  analogi_c.h - header file for AnalogI_c

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ANALOGI_C_H
#define ANALOGI_C_H

#include "inputbase_c.h"


namespace __IsoAgLib {
/**
  Input object for analog sensors;
  has options like automatic calculation of linear
  y = (mMult/mDiv*x+t) for easy evaluating the sensor values;
  can use fast or slow ADC method by config setting
  @short AnalogI_c serves an easy interface to sensor signals with linear transformation.
  @author Dipl.-Inform. Achim Spangler
  */
class AnalogI_c : public InputBase_c
{
public:
  /**
    internal called constructor which creates a new input channel,initialize the hardware and configures conversion calculation
    @param ab_channel default-argument for the hardware channel of the input
    @param ren_analogType default-argument for choosing voltage(default) or current as input type
    @param ab_useMean default-argument for setting the calculation of mean value on true (false as default)
    @param ab_fastAdc default-argument for setting fast ADC method (false as default)
  */
  AnalogI_c(uint8_t ab_channel = 0xFF, IsoAgLib::iInput_c::analogType_t ren_analogType = IsoAgLib::iInput_c::voltage, bool ab_useMean = false,
             bool ab_fastAdc = false);

  ~AnalogI_c() {}

  /**
    internal called constructor which creates a new input channel,initialize the hardware and configures conversion calculation
    @param ab_channel default-argument for the hardware channel of the input
    @param ren_analogType default-argument for choosing voltage(default) or current as input type
    @param ab_useMean default-argument for setting the calculation of mean value on true (false as default)
    @param ab_fastAdc default-argument for setting fast ADC method (false as default)
  */
  void init(uint8_t ab_channel, IsoAgLib::iInput_c::analogType_t ren_analogType = IsoAgLib::iInput_c::voltage, bool ab_useMean = false,
             bool ab_fastAdc = false);

  /**
    get the actual sensor value with the configured linear conversion (use the configured ADC method)
    @return sensor value: A) Volt [mV], or B) Ampere [mA]
  */
  uint16_t val() const;

  /**
    check if value is greater than 0
    @return true if sensor value is different from 0, otherwise 0
  */
  bool active() const;

  /**
    configure fast ADC gathering
    @param ab_useFast default-argument for setting fast ADC (true as default)
  */
  void setFastAdc(bool ab_useFast=true);

protected:
  // unimplemented, not copyabled
  AnalogI_c(const AnalogI_c&);
  AnalogI_c& operator=(const AnalogI_c&);

private: // Private attributes
  /**
    input type: voltage or current
    @see __IsoAgLib::t_analogType
  */
  IsoAgLib::iInput_c::analogType_t en_analogType;

  /** if true a mean of the sensor values is retreived */
  bool b_useMean;

  /** if true the fast analog input method is used */
  bool b_fastAdc;
};

} // __IsoAgLib

#endif
