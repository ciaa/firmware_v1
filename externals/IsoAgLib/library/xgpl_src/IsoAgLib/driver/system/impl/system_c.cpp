/*
  system_c.cpp: module for a interfacing the system hardware

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "system_c.h"

#include <IsoAgLib/util/iliberr_c.h>

namespace __IsoAgLib {

System_c& getSystemInstance()
{
  MACRO_SINGLETON_GET_INSTANCE_BODY(System_c);
}

void
System_c::init()
{
  isoaglib_assert( ! HAL::isSystemOpened() );
  HAL::openSystem();
  HAL::powerHold( true );
}

void
System_c::close()
{
  isoaglib_assert( HAL::isSystemOpened() );
  HAL::powerHold( false );
  HAL::closeSystem();
}

}
