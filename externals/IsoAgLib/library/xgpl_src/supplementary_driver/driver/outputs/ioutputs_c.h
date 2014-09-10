/*
  ioutputs.h - header for the outputs management object

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IOUTPUTS_C_H
#define IOUTPUTS_C_H

#include "impl/outputs_c.h"
#include "idigitalo_c.h"


namespace IsoAgLib {

/**
  Hardware dependent object for hardware independent controling of outputs.
  @see iDigital_O
  @author Dipl.-Inform. Achim Spangler
  */
class iOutputs_c : private __IsoAgLib::Outputs_c {
public:
  void init() { Outputs_c::init(); }
  void close() { Outputs_c::close(); }

  /** control the relay which is responsible for activation of the PWM output */
  void setMainRelais( bool ab_active ) { Outputs_c::setMainRelais( ab_active ); }

private:
  /** allow getIoutputsInstance() access to shielded base class.
      otherwise __IsoAgLib::getOutputsInstance() wouldn't be accepted by compiler
    */
  friend iOutputs_c& getIoutputsInstance( void );

  /** private constructor which prevents direct instantiation in user application
    * NEVER define instance of Outputs_c within application
    */
  iOutputs_c();
  ~iOutputs_c();
};

/** C-style function, to get access to the unique iOutputs_c singleton instance */
inline iOutputs_c& getIoutputsInstance( void ) { return static_cast<iOutputs_c&>(__IsoAgLib::getOutputsInstance());};

}

#endif
