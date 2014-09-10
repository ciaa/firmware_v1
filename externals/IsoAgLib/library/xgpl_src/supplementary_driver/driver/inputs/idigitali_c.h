/*
  idigital_i.h - interface header file for DigitalI_c

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef IDIGITALI_C_H
#define IDIGITALI_C_H

#include "impl/digitali_c.h"


namespace IsoAgLib {

/**Interface class for Analog input objects
  @author Dipl.-Inform. Achim Spangler
  */
class iDigitalI_c : private __IsoAgLib::DigitalI_c {
public:
  /**
    internal called constructor for a new digital input channel which performs configuration of hardware
    @param ab_channel default-argument for setting hardware channel for this input
    @param ren_onoff default-argument for setting whether 1 should be returned on High(Default: OnHigh) or Low signal
    @param ab_static default-argument for setting if hardware input should be gathered static (default false with no static)
    @param apc_handler optional pointer to handler class, which can be called, if an HAL irq event occurs
  */
  iDigitalI_c(uint8_t ab_channel = 0xFF, IsoAgLib::iInput_c::onoff_t ren_onoff = IsoAgLib::iInput_c::OnHigh,
      bool ab_static = false, iInputEventHandler* apc_handler = NULL )
    : DigitalI_c(ab_channel, ren_onoff, ab_static, apc_handler)
  {}

  /**
    internal called constructor for a new digital input channel which performs configuration of hardware
    @param ab_channel default-argument for setting hardware channel for this input
    @param ren_onoff default-argument for setting whether 1 should be returned on High(Default: OnHigh) or Low signal
    @param ab_static default-argument for setting if hardware input should be gathered static (default false with no static)
    @param apc_handler optional pointer to handler class, which can be called, if an HAL irq event occurs
  */
  void init(uint8_t ab_channel, IsoAgLib::iInput_c::onoff_t ren_onoff = IsoAgLib::iInput_c::OnHigh,
              bool ab_static = false, iInputEventHandler* apc_handler = NULL )
    { DigitalI_c::init(ab_channel, ren_onoff, ab_static, apc_handler);}

  virtual ~iDigitalI_c(){}

  /** change detection mode of activity to OnHigh */
  void setOnHigh( void ) { DigitalI_c::setOnHigh(); }

  /** change detection mode of activity to OnLow */
  void setOnLow( void ) { DigitalI_c::setOnLow(); }

  /**
    check for the input value
    @return 1 for (High signal AND ren_onoff==OnHigh)(Default!) or (Low signal AND ren_onoff==OnLow); otherwise 0
  */
  uint16_t val()const{ return DigitalI_c::val(); }

  /**
    check for the input value (uses BIOS function)
    @return true for (High signal AND ren_onoff==OnHigh)(Default!) or (Low signal AND ren_onoff==OnLow); otherwise false
  */
  bool active()const { return DigitalI_c::active(); }

  /**
    deliver the channel number of the output object
    @return number to use for BIOS access to this channel
  */
  uint8_t channelNr() const { return DigitalI_c::channelNr(); }

private:
// Private attributes
  /**
    HIDDEN! copy constructor for iDigitalI_c
    NEVER copy a iDigitalI_c around!!!!
    ONLY copy pointers to the wanted instance!!!
    ==> the copy constructor is defined as private, so that compiler
        detects this fault, and shows you this WARNING!!
    <!--@param acrc_src source-->
  */
  iDigitalI_c(const iDigitalI_c& /*acrc_src*/);

  /**
    HIDDEN! assignment for iDigitalI_c
    NEVER assign a iDigitalI_c to another instance!!!!
    ==> the asignment is defined as private, so that compiler
        detects this fault, and shows you this WARNING!!
    <!--@param acrc_src source-->
  */
  iDigitalI_c& operator=(const iDigitalI_c& /*acrc_src*/);
};

} // IsoAgLib

#endif
