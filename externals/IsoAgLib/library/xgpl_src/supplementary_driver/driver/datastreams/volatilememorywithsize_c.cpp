/*
  volatilememorywithsize_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "volatilememorywithsize_c.h"



// //////////////////////////////// +X2C Operation 6851 : operator >>
//! Parameter:
//! @param ui8_data:
StreamInput_c&
VolatileMemoryWithSize_c::operator >>(uint8_t& ui8_data)
{
	if (mi32_available > 0)
	{
		ui8_data = *mpcui8_data++;
		mi32_available --;
	}
	else
		ui8_data = scui8_valueWhenEOF;
	

  return *this;
}



const VolatileMemoryWithSize_c&
VolatileMemoryWithSize_c::operator = (const VolatileMemoryWithSize_c& rrefc_src)
{
  mpcui8_data = rrefc_src.mpcui8_data;
  mi32_available = rrefc_src.mi32_available;
  return rrefc_src;
}


// //////////////////////////////// +X2C Operation 6853 : eof
bool
VolatileMemoryWithSize_c::eof() const
{
  return !(mi32_available > 0);
}

