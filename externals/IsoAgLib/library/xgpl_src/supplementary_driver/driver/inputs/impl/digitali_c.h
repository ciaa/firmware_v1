/*
  digitali_c.h:
    header file for Digital_I_c, an object for digital sensor input

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef DIGITAL_I_H
#define DIGITAL_I_H

#include "inputbase_c.h"


namespace __IsoAgLib {
/**
  Input object for simple digital input;
  has option for setting ON state to HI or LO and
  can request static input values
  @see InputBase_c
  @author Dipl.-Inform. Achim Spangler
  */
class DigitalI_c : public InputBase_c {
public:
  /**
    internal called constructor for a new digital input channel which performs configuration of hardware
    @param ab_channel default-argument for setting hardware channel for this input
    @param ren_onoff default-argument for setting whether 1 should be returned on High(Default: OnHigh) or Low signal
    @param ab_static default-argument for setting if hardware input should be gathered static (default false with no static)
    @param apc_handler optional pointer to handler class, which can be called, if an HAL irq event occurs
  */
  DigitalI_c(uint8_t ab_channel = 0xFF, IsoAgLib::iInput_c::onoff_t ren_onoff = IsoAgLib::iInput_c::OnHigh,
              bool ab_static = false, IsoAgLib::iInputEventHandler* apc_handler = NULL );

  ~DigitalI_c() {}

  /**
    internal called constructor for a new digital input channel which performs configuration of hardware
    @param ab_channel default-argument for setting hardware channel for this input
    @param ren_onoff default-argument for setting whether 1 should be returned on High(Default: OnHigh) or Low signal
    @param ab_static default-argument for setting if hardware input should be gathered static (default false with no static)
    @param apc_handler optional pointer to handler class, which can be called, if an HAL irq event occurs
  */
  void init(uint8_t ab_channel, IsoAgLib::iInput_c::onoff_t ren_onoff = IsoAgLib::iInput_c::OnHigh,
              bool ab_static = false, IsoAgLib::iInputEventHandler* apc_handler = NULL );

  /** change detection mode of activity to OnHigh */
  void setOnHigh( void );

  /** change detection mode of activity to OnLow */
  void setOnLow( void );

  /**
    check for the input value
    @return 1 for (High signal AND ren_onoff==OnHigh)(Default!) or (Low signal AND ren_onoff==OnLow); otherwise 0
  */
  uint16_t val() const;

  /**
    check for the input value
    @return true for (High signal AND ren_onoff==OnHigh)(Default!) or (Low signal AND ren_onoff==OnLow); otherwise false
  */
  bool active() const;

  /** handler function which is called from HAL */
  static void handleHalIrqEvent( uint8_t aui8_channel );

private:
  // unimplemented, not copyable
  DigitalI_c(const DigitalI_c&);
  DigitalI_c& operator=(const DigitalI_c&);

  /** true if the digit input should be gathered static */
  bool b_static;

  /**
    mapping type for sensor HI/LO -> ON/OFF
    @see t_onoff
  */
  IsoAgLib::iInput_c::onoff_t en_onoff;

  /** array of pointers to handler for IRQ events */
  #ifndef __TSW_CPP__
  static IsoAgLib::iInputEventHandler* ppc_handler[16];
  #endif
};

} // __IsoAgLib

#endif
