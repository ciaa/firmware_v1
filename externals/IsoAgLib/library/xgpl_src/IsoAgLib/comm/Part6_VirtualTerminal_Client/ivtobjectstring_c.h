/*
  ivtobjectstring.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTSTRING_H
#define IVTOBJECTSTRING_H

#include "impl/vtobjectstring_c.h"


namespace IsoAgLib {

class iVtObjectString_c : public __IsoAgLib::vtObjectString_c
{
public:
  iVtObjectString_c();
  ~iVtObjectString_c();

  virtual const char* getString() = 0;
};

} // IsoAgLib

#endif
