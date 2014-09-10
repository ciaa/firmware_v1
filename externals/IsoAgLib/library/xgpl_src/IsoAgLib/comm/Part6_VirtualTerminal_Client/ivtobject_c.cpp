/*
  ivtobject_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "ivtobject_c.h"

#include "impl/vttypes.h"

// Begin Namespace IsoAgLib
namespace IsoAgLib {
// Operation : vtObject_c
//iVtObject_c::vtObject_c()
//: flags( 0 ), ui16_bitmaskAttributeAlreadyRequested  (0)
//{
//}

// Operation : setOriginSKM
//! @param b_SKM: true if this object is included in a softkeymask.
void
iVtObject_c::setOriginSKM(bool b_SKM)
{
  if (b_SKM) s_properties.flags |= FLAG_ORIGIN_SKM;
}

/// Operation : setOriginBTN
/// @param p_btn: false if this object is not included in a button.
void iVtObject_c::setOriginBTN(iVtObjectButton_c* p_btn)
{
  if (p_btn) p_parentButtonObject = p_btn;
}

} // end of namespace IsoAgLib
