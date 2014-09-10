/*
  outputbase_c.h - header file for OutputBase_c

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef OUTPUTBASE_C_H
#define OUTPUTBASE_C_H

#include <supplementary_driver/hal/hal_outputs.h>


namespace __IsoAgLib {

/**
  Base Class for Outputs_c output channels;
  holds information for channel number
  @see DigitalO_c
  @see Outputs_c
  */
class OutputBase_c {
public:
  OutputBase_c (uint8_t a_channelNr)
    : ui8_channelNr( a_channelNr )
  {}

  virtual ~OutputBase_c()
  {}

  void setChannel(uint8_t aui8_channelNr) { ui8_channelNr = aui8_channelNr; }
  uint8_t channelNr() const { return ui8_channelNr; }

private:
  uint8_t ui8_channelNr;
};

} // __IsoAgLib

#endif

