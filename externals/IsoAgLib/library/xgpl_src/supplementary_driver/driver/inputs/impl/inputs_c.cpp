/*
  inputs_c.cpp - implmentation for the inputs management

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "inputs_c.h"
#include <IsoAgLib/util/impl/singleton.h>



namespace __IsoAgLib {


Inputs_c &
getInputsInstance()
{
  MACRO_SINGLETON_GET_INSTANCE_BODY(Inputs_c);
}


} // __IsoAgLib
