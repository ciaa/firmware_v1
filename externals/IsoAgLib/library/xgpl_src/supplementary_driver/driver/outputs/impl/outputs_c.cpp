/*
  outputs_c.cpp:
    implementation for the outputs management object

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "outputs_c.h"

#include <IsoAgLib/util/iliberr_c.h>
#include <IsoAgLib/driver/system/impl/system_c.h>
#include <supplementary_driver/hal/hal_outputs.h>


namespace __IsoAgLib {

Outputs_c &
getOutputsInstance()
{
  MACRO_SINGLETON_GET_INSTANCE_BODY(Outputs_c);
}


void
Outputs_c::setMainRelais( bool ab_active )
{
  HAL::setMainRelais( ab_active );
}


} // __IsoAgLib
