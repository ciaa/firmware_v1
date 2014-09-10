/*
  volatilememory_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "volatilememory_c.h"



// //////////////////////////////// +X2C Operation 6851 : operator >>
//! Parameter:
//! @param ui8_data:
StreamInput_c&
VolatileMemory_c::operator >>(uint8_t& ui8_data)
{
  ui8_data = *p_volatileMemory++;
  return *this;
}



const VolatileMemory_c&
VolatileMemory_c::operator = (const VolatileMemory_c& acrc_src)
{
  p_volatileMemory = acrc_src.p_volatileMemory;
  return acrc_src;
}


// //////////////////////////////// +X2C Operation 6853 : eof
bool
VolatileMemory_c::eof() const
{
  return false;
}

