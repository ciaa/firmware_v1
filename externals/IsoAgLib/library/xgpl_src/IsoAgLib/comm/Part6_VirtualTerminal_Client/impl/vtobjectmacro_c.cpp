/*
  vtobjectmacro_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectmacro_c.h"

#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectMacro_c::stream(uint8_t* destMemory,
                        uint16_t maxBytes,
                        objRange_t sourceOffset)
{ // ~X2C
#define MACRO_vtObjectTypeA vtObjectMacro_a
#define MACRO_vtObjectTypeS iVtObjectMacro_s
    MACRO_streamLocalVars;

    uint16_t numBytesToFollow = vtObjectMacro_a->commandsToFollow ? vtObjectMacro_a->numBytesToFollow : 0;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 28; // Object Type = Macro
      destMemory [3] = numBytesToFollow & 0xFF;
      destMemory [4] = numBytesToFollow >> 8;
      curBytes += 5;
      sourceOffset += 5;
    }

    while ((sourceOffset >= 5U) && (sourceOffset < (5U+numBytesToFollow)) && ((curBytes+1) <= maxBytes))
    {
      destMemory [curBytes] = vtObjectMacro_a->commandsToFollow [sourceOffset-5];
      curBytes++;
      sourceOffset++;
    }
    return curBytes;
}


vtObjectMacro_c::vtObjectMacro_c() {}


uint32_t
vtObjectMacro_c::fitTerminal() const
{
  MACRO_localVars;
  return 5 + (vtObjectMacro_a->commandsToFollow ? vtObjectMacro_a->numBytesToFollow : 0);
}

} // __IsoAgLib
