/*
  outputs_c.h - header for the outputs management object

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef OUTPUTS_C_H
#define OUTPUTS_C_H

#include <IsoAgLib/util/impl/singleton.h>
#include "digitalo_c.h"


namespace __IsoAgLib {

/**
  Hardware dependent object for hardware independent getting of output data;
  */
class Outputs_c
{
public:
  void init() {}
  void close() {}

  /** control the relay which is responsible for activation of the PWM output */
  void setMainRelais( bool ab_active ); 

private:
  // for singleton only
  Outputs_c() {}
  ~Outputs_c() {}

private:
  friend Outputs_c &getOutputsInstance();
};


Outputs_c &
getOutputsInstance();

}

#endif
