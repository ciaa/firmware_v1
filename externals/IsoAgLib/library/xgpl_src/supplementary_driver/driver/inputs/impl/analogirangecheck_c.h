/*
  analogirangecheck_c.h - header file for AnalogIRangeCheck_c

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ANALOGIRANGECHECK_C_H
#define ANALOGIRANGECHECK_C_H

#include "analogi_c.h"


namespace __IsoAgLib {
/**
  Extend AnalogI_c by range check for input signal
  -> store [minValid..maxValid] and answer state dependent
  @see AnalogI_c
  @see CounterI_c
  @see DigitalI_c
  @see InputBase_c
  @short AnalogIRangeCheck_c serves an easy interface to sensor signals with linear transformation.
  @author Dipl.-Inform. Achim Spangler
  */
class AnalogIRangeCheck_c : public AnalogI_c
{
public:
  /**
    internal called constructor which creates a new input channel,initialize the hardware and configures conversion calculation
    @param ab_channel default-argument for the hardware channel of the input
    @param ren_analogType default-argument for choosing voltage(default) or current as input type
    @param ab_useMean default-argument for setting the calculation of mean value on true (false as default)
    @param ab_fastAdc default-argument for setting fast ADC method (false as default)
    @param aui16_minValid default min allowed value (min limit of range)
    @param aui16_maxValid default max allowed value (max limit of range)
  */
  AnalogIRangeCheck_c(uint8_t ab_channel = 0xFF, IsoAgLib::iInput_c::analogType_t ren_analogType = IsoAgLib::iInput_c::voltage, bool ab_useMean = false,
             bool ab_fastAdc = false, uint16_t aui16_minValid = 0, uint16_t aui16_maxValid = 0xFFFF );

  /**
    internal called constructor which creates a new input channel,initialize the hardware and configures conversion calculation
    @param ab_channel default-argument for the hardware channel of the input
    @param ren_analogType default-argument for choosing voltage(default) or current as input type
    @param ab_useMean default-argument for setting the calculation of mean value on true (false as default)
    @param ab_fastAdc default-argument for setting fast ADC method (false as default)
    @param aui16_minValid default min allowed value (min limit of range)
    @param aui16_maxValid default max allowed value (max limit of range)
  */
  void init(uint8_t ab_channel, IsoAgLib::iInput_c::analogType_t ren_analogType = IsoAgLib::iInput_c::voltage, bool ab_useMean = false,
             bool ab_fastAdc = false, uint16_t aui16_minValid = 0, uint16_t aui16_maxValid = 0xFFFF);

  virtual ~AnalogIRangeCheck_c();

  /** get validate val
    @param rb_tooLow  reference to bool value which is set dependent on ( value < minLimit )
    @param rb_tooHigh reference to bool value which is set dependent on ( value > maxLimit )
    @return input value
  */
  int16_t validatedVal( bool &rb_tooLow, bool &rb_tooHigh ) const;

  /** check if input value is valid */
  bool good( void ) const;

  /** check if input value is in error state */
  bool error( void ) const;

  /** check if input value is out of range */
  bool checkRangeError( void ) const;

  /** check if value is too high */
  bool checkTooHigh( void ) const;

  /** check if value is too low */
  bool checkTooLow( void ) const;

  /* set range
    @param aui16_minValid default min allowed value (min limit of range)
    @param aui16_maxValid default max allowed value (max limit of range)
  */
  void setRange( uint16_t aui16_minValid, uint16_t aui16_maxValid );

  /** deliver total valid range in mV */
  uint16_t getRange( void ) const { return ( ui16_maxValid - ui16_minValid ); }

  /** deliver min Limit */
  uint16_t getMinValid( void ) const { return ui16_minValid; }

  /** deliver max Limit */
  uint16_t getMaxValid( void ) const { return ui16_maxValid; }

protected:
  /**
    HIDDEN! copy constructor for AnalogIRangeCheck_c
    NEVER copy a AnalogIRangeCheck_c around!!!!
    ONLY copy pointers to the wanted instance!!!
    ==> the copy constructor is defined as private, so that compiler
        detects this fault, and shows you this WARNING!!
    @param acrc_src source
  */
  AnalogIRangeCheck_c(const AnalogIRangeCheck_c&);

  /**
    HIDDEN! assignment for AnalogIRangeCheck_c
    NEVER assign a AnalogIRangeCheck_c to another instance!!!!
    ==> the asignment is defined as private, so that compiler
        detects this fault, and shows you this WARNING!!
    <!--@param acrc_src source-->
  */
  AnalogIRangeCheck_c& operator=(const AnalogIRangeCheck_c&);

private:
  uint16_t ui16_minValid;
  uint16_t ui16_maxValid;
};

} // __IsoAgLib

#endif
