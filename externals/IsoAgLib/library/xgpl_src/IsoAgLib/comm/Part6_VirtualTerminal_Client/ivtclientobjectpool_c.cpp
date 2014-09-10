/*
  ivtclientobjectpool_c.cpp: Class for managing an object pool with
    softkey/button activation event handling

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "ivtclientobjectpool_c.h"
#include <IsoAgLib/util/iassert.h>

namespace
{

uint8_t const carrui8_normalized[] = {
  16, 231, 34, 37, 124, 127, 142, 188, 145, 21, 46, 51, 196, 201, 226, 19 };

inline uint8_t normalizeDepth2Color(uint8_t ui8_color) {
  return ui8_color < 16 ? carrui8_normalized[ui8_color] : ui8_color;
}

}


namespace IsoAgLib
{

uint8_t iVtClientObjectPool_c::convertColourDefault(
    uint8_t colorValue,
    uint8_t colorDepth,
    IsoAgLib::iVtObject_c * /*obj*/,
    IsoAgLib::e_vtColour /*whichColour*/)
{
  unsigned const cu_fittingDepth =
    colorValue < 2 ? 0 :
    colorValue < 16 ? 1 :
    2;

  if (colorDepth >= cu_fittingDepth)
    return colorValue; // don't convert

  uint8_t const colorValue1 = (1 == cu_fittingDepth) ?
    normalizeDepth2Color(colorValue) : colorValue;

  isoaglib_assert(15 < colorValue1);
  if (colorValue1 < 232) {
    unsigned const cu_r0 = colorValue1 - 16u;
    unsigned const cu_z0 = cu_r0 % 6u;
    unsigned const cu_r1 = cu_r0 / 6u;
    unsigned const cu_z1 = cu_r1 % 6u;
    unsigned const cu_r2 = cu_r1 / 6u;
    unsigned const cu_z2 = cu_r2 % 6u;
    return 7u < cu_z0 + cu_z1 + cu_z2 ? 1 : 0;
  } else {
    // Convert proprietary colors to black:
    return 0;
  }
}

}
