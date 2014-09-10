/*
  iinput_c.h:
    header file for common input enums.

  (C) Copyright 2012 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IINPUT_C_H
#define IINPUT_C_H

#include <IsoAgLib/isoaglib_config.h>


namespace IsoAgLib {

/** virtual base class for classes in applications, which want
  * to be called on a sensor related HAL IRQ event
  * a special application class must overload the appropriate
  * event handler function for the specific type;
  * additionally, the contructor of the DigitalI_c, AnalogI_c
  * or CounterI_c instance must be called with a pointer to the
  * handling object, which must be derived from this class
  */
class iInputEventHandler {
 public:
  /** function to handle a DigitalI_c event from HAL
    * @param aui8_channel channel of the input object, which received the IRQ
    *        from HAL
    */
  virtual void handleDigitalEvent( uint8_t aui8_channel );
  virtual ~iInputEventHandler() {};
};


class iInput_c
{
public:
  /** enum type input type of analog: voltage or current (values equivalent to BIOS) */
  enum analogType_t {voltage = 0, current = 1};

  /** enum type for state of an sensor input */
  enum state_t {good = 0, error = 1};

  /** enum type for mapping input HI/LO to on/off */
  enum onoff_t {OnHigh = 1, OnLow = 0};

  /** enum type for the different input type object: at the moment digital and analog */
  enum inputType_t {undef_input = 0, digital = 1, analog = 2, counter = 3};
};

}

#endif
