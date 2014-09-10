/*
  ianalogirangecheck_c.h - interface header file for iAnalogIRangeCheck_c

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef IANALOGIRANGECHECK_C_H
#define IANALOGIRANGECHECK_C_H

#include "impl/analogirangecheck_c.h"


namespace IsoAgLib {

/**Interface class for Analog input objects
  *@author Dipl.-Inform. Achim Spangler
  */
class iAnalogIRangeCheck_c : private __IsoAgLib::AnalogIRangeCheck_c
{
private:
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
  iAnalogIRangeCheck_c(uint8_t ab_channel = 0xFF, IsoAgLib::iInput_c::analogType_t ren_analogType = IsoAgLib::iInput_c::voltage, bool ab_useMean = false, bool ab_fastAdc = false,
    uint16_t aui16_minValid = 0, uint16_t aui16_maxValid = 0xFFFF )
    : AnalogIRangeCheck_c(ab_channel, ren_analogType, ab_useMean, ab_fastAdc, aui16_minValid, aui16_maxValid ) {}

  /**
    internal called constructor which creates a new input channel,initialize the hardware and configures conversion calculation
    @param ab_channel default-argument for the hardware channel of the input
    @param ren_analogType default-argument for choosing voltage(default) or current as input type
    @param ab_useMean default-argument for setting the calculation of mean value on true (false as default)
    @param ab_fastAdc default-argument for setting fast ADC method (false as default)
    @param aui16_minValid default min allowed value (min limit of range)
    @param aui16_maxValid default max allowed value (max limit of range)
  */
  void init(uint8_t ab_channel, IsoAgLib::iInput_c::analogType_t ren_analogType = IsoAgLib::iInput_c::voltage, bool ab_useMean = false, bool ab_fastAdc = false,
    uint16_t aui16_minValid = 0, uint16_t aui16_maxValid = 0xFFFF )
    { AnalogIRangeCheck_c::init(ab_channel, ren_analogType, ab_useMean, ab_fastAdc, aui16_minValid, aui16_maxValid );}

  virtual ~iAnalogIRangeCheck_c() {}

  /**
    get the actual input value with the configured linear conversion (use the configured ADC method)
    @return input value: A) Volt [mV], or B) Ampere [mA]
  */
  uint16_t val() const {return AnalogIRangeCheck_c::val();}

  /** get validate val
    @param rb_tooLow  reference to bool value which is set dependent on ( value < minLimit )
    @param rb_tooHigh reference to bool value which is set dependent on ( value > maxLimit )
    @return input value
  */
  int16_t validatedVal( bool &rb_tooLow, bool &rb_tooHigh ) const
   { return AnalogIRangeCheck_c::validatedVal( rb_tooLow, rb_tooHigh ); }

  /**
    check if value is greater than 0
    @return true if input value is different from 0, otherwise 0
  */
  bool active() const {return AnalogIRangeCheck_c::active();}

  /**
    configure fast ADC gathering
    @param ab_useFast default-argument for setting fast ADC (true as default)
  */
  void setFastAdc(bool ab_useFast=true){AnalogIRangeCheck_c::setFastAdc(ab_useFast);}

  /**
    deliver the channel number of the output object
    @return number to use for BIOS access to this channel
  */
  uint8_t channelNr() const { return AnalogIRangeCheck_c::channelNr();}

  /** check if input value is valid */
  bool good( void ) const { return AnalogIRangeCheck_c::good(); }

  /** check if input value is in error state */
  bool error( void ) const { return AnalogIRangeCheck_c::error(); }

  /** check if input value is out of range */
  bool checkRangeError( void ) const { return AnalogIRangeCheck_c::checkRangeError(); }

  /** check if value is too high */
  bool checkTooHigh( void ) const { return AnalogIRangeCheck_c::checkTooHigh(); }

  /** check if value is too low */
  bool checkTooLow( void ) const { return AnalogIRangeCheck_c::checkTooLow(); }

  /* set range
    @param aui16_minValid default min allowed value (min limit of range)
    @param aui16_maxValid default max allowed value (max limit of range)
  */
  void setRange( uint16_t aui16_minValid, uint16_t aui16_maxValid )
    { AnalogIRangeCheck_c::setRange( aui16_minValid, aui16_maxValid ); }

  /** deliver total valid range in mV */
  uint16_t getRange( void ) const { return AnalogIRangeCheck_c::getRange(); }

  /** deliver min Limit */
  uint16_t getMinValid( void ) const { return AnalogIRangeCheck_c::getMinValid(); }

  /** deliver max Limit */
  uint16_t getMaxValid( void ) const { return AnalogIRangeCheck_c::getMaxValid(); }

private:
  /**
    HIDDEN! copy constructor for AnalogIRangeCheck_c
    NEVER copy a AnalogIRangeCheck_c around!!!!
    ONLY copy pointers to the wanted instance!!!
    ==> the copy constructor is defined as private, so that compiler
        detects this fault, and shows you this WARNING!!
    <!--@param acrc_src source-->
  */
  iAnalogIRangeCheck_c(const iAnalogIRangeCheck_c &);

  /**
    HIDDEN! assignment for AnalogIRangeCheck_c
    NEVER assign a AnalogIRangeCheck_c to another instance!!!!
    ==> the asignment is defined as private, so that compiler
        detects this fault, and shows you this WARNING!!
    <!--@param acrc_src source-->
  */
  iAnalogIRangeCheck_c& operator=(const iAnalogIRangeCheck_c &);
};

} // IsoAgLib

#endif
