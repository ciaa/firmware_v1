/*
  inputbase_c.h - header file for InputBase_c

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef INPUTBASE_C_H
#define INPUTBASE_C_H

#include <supplementary_driver/hal/hal_inputs.h>
#include <supplementary_driver/driver/inputs/iinput_c.h>


namespace __IsoAgLib {

/**
  Base Class for input channels, holds channel number/type
  @author Dipl.-Inform. Achim Spangler
  */
class InputBase_c {
public:
  /**
    @param aui8_channelNr hardware channel of this input object
    @param ren_inputType input type, which is needed because of the relation
           of config number and gathering number which vary dependent on input type
  */
  inline InputBase_c(uint8_t aui8_channelNr, IsoAgLib::iInput_c::inputType_t ren_inputType = IsoAgLib::iInput_c::undef_input);

  /**
    @param aui8_channelNr hardware channel of this input object
    @param ren_inputType input type, which is needed because of the relation
           of config number and gathering number which vary dependent on input type
  */
  inline void init(uint8_t aui8_channelNr, IsoAgLib::iInput_c::inputType_t ren_inputType = IsoAgLib::iInput_c::undef_input);

  virtual ~InputBase_c() {}

  /**
    deliver the channel number of the output object
    @return number to use for BIOS access to this channel
  */
  uint8_t channelNr() const { return ui8_channelNr;}

protected:
  uint8_t ui8_channelNr;

private:
  IsoAgLib::iInput_c::inputType_t en_inputType;
};


inline
InputBase_c::InputBase_c( uint8_t aui8_channelNr, IsoAgLib::iInput_c::inputType_t ren_inputType )
  : ui8_channelNr( aui8_channelNr )
  , en_inputType( ren_inputType )
{ 
}


inline
void
InputBase_c::init( uint8_t aui8_channelNr, IsoAgLib::iInput_c::inputType_t ren_inputType )
{
  ui8_channelNr = aui8_channelNr;
  en_inputType = ren_inputType;
}



} // __IsoAgLib

#endif


