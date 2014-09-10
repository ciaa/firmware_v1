/*
  vtobjectstringvariable_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "../ivtobjectstringvariable_c.h"
#include "vtobjectstringvariable_c.h"

#ifdef USE_VTOBJECT_stringvariable

#include "vtclient_c.h"

namespace IsoAgLib {
  // implement here a normal constructor, as the compiler dislikes inlining of that simple constructor
  // direct in scope of iVtObjectStringVariable_c
  iVtObjectStringVariable_c::iVtObjectStringVariable_c() : vtObjectStringVariable_c() {}
}


namespace __IsoAgLib {

int16_t
vtObjectStringVariable_c::stream(uint8_t* destMemory,
                                 uint16_t maxBytes,
                                 objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectStringVariable_a
#define MACRO_vtObjectTypeS iVtObjectStringVariable_s
  MACRO_streamLocalVars;

  if (sourceOffset == 0) { // dump out constant sized stuff
    destMemory [0] = vtObject_a->ID & 0xFF;
    destMemory [1] = vtObject_a->ID >> 8;
    destMemory [2] = 22; // Object Type = Integer Variable
    destMemory [3] = vtObjectStringVariable_a->length & 0xFF;
    destMemory [4] = vtObjectStringVariable_a->length >> 8;
    curBytes += 5;
    sourceOffset += 5;
  }

  while ((sourceOffset >= 5U) && (sourceOffset < (5U+vtObjectStringVariable_a->length)) && ((curBytes+1) <= maxBytes)) {
    destMemory [curBytes] = vtObjectStringVariable_a->value [sourceOffset-5];
    curBytes++;
    sourceOffset++;
  }

  return curBytes;
}


vtObjectStringVariable_c::vtObjectStringVariable_c() {}


uint32_t
vtObjectStringVariable_c::fitTerminal() const
{
  MACRO_localVars;
  return 5+vtObjectStringVariable_a->length;
}

#ifdef USE_VT_UNICODE_SUPPORT
// Operation : setValueCopyUTF8
//! @param newValue:
//! @param b_updateObject:
void
vtObjectStringVariable_c::setValueCopyUTF8 (const char* newValue, uint8_t aui8_fontType, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  const uint16_t cui16_strLen = (uint16_t)CNAMESPACE::strlen (newValue);
  char* pc_iso8859 = new char [cui16_strLen+1];

  convertStringUnicodeTo8859 (newValue, cui16_strLen, pc_iso8859, aui8_fontType);

  setValueCopy (pc_iso8859, b_updateObject, b_enableReplaceOfCmd);
  delete[] pc_iso8859;
}


void
vtObjectStringVariable_c::setValueCopyUTF16 (const char* newValue, uint16_t length, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  int copyLen = ( length <= get_vtObjectStringVariable_a()->length) ? length : get_vtObjectStringVariable_a()->length;

  // UTF-16 string must be a multiple of 2 bytes long.
  if( copyLen % 2 )
   copyLen--;

  if (b_updateObject) {
    // check if not already RAM string buffer?
    if (!(s_properties.flags & FLAG_STRING_IN_RAM)) {
      s_properties.flags |= FLAG_STRING_IN_RAM;
      // create new String buffer with same length as original one, as the size can't be changed !!
      char *newStringBuffer = new char [get_vtObjectStringVariable_a()->length+1];
      saveValueP (MACRO_getStructOffset(get_vtObjectStringVariable_a(), value), sizeof(iVtObjectStringVariable_s), (IsoAgLib::iVtObject_c*) newStringBuffer);
    }
    char *dest = get_vtObjectStringVariable_a()->value;
    const char *src = newValue;

    int i=0; for (; i<copyLen; i++) *dest++ = *src++;
    spacePadBomUTF16( dest, copyLen, get_vtObjectStringVariable_a()->length );
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeStringValueUTF16 (getID(), newValue, copyLen, b_enableReplaceOfCmd);

}
#endif


void
vtObjectStringVariable_c::setValueCopy(const char* newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    // check if not already RAM string buffer?
    if (!(s_properties.flags & FLAG_STRING_IN_RAM)) {
      s_properties.flags |= FLAG_STRING_IN_RAM;
      // create new String buffer with same length as original one, as the size can't be changed !!
      char *newStringBuffer = new char [get_vtObjectStringVariable_a()->length+1];
      saveValueP (MACRO_getStructOffset(get_vtObjectStringVariable_a(), value), sizeof(iVtObjectStringVariable_s), (IsoAgLib::iVtObject_c*) newStringBuffer);
    }
    char *dest = get_vtObjectStringVariable_a()->value;
    const char *src = newValue;
    int copyLen = (CNAMESPACE::strlen (newValue) <= get_vtObjectStringVariable_a()->length) ? CNAMESPACE::strlen (newValue) : get_vtObjectStringVariable_a()->length;
    int i=0; for (; i<copyLen; i++) *dest++ = *src++;
    for (; i<get_vtObjectStringVariable_a()->length; i++) *dest++ = ' ';
    *dest = 0x00; // 0-termiante!
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeStringValue (this, newValue, get_vtObjectStringVariable_a()->length, b_enableReplaceOfCmd);
}


void
vtObjectStringVariable_c::setValueRef(const char* newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    // delete RAM_String first, before we lose the pointer!
    if (s_properties.flags & FLAG_STRING_IN_RAM) {
      delete (get_vtObjectStringVariable_a()->value);
      s_properties.flags &= ~FLAG_STRING_IN_RAM;
    }

    saveValueP (MACRO_getStructOffset(get_vtObjectStringVariable_a(), value), sizeof(iVtObjectStringVariable_s), (IsoAgLib::iVtObject_c*) newValue);
  }

  setStringToStream( newValue ); // use MultiSendStreamer with mpc_stringToStream set!
  uint16_t ui16_tempLen = 0;
  if (newValue != NULL ) ui16_tempLen = (CNAMESPACE::strlen (newValue) <= get_vtObjectStringVariable_a()->length) ? CNAMESPACE::strlen (newValue) : get_vtObjectStringVariable_a()->length;
  setStrLenToSend( ui16_tempLen );
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeStringValue (this, b_enableReplaceOfCmd);
}


const char*
vtObjectStringVariable_c::getString()
{
  return get_vtObjectStringVariable_a()->value;
}

} // __IsoAgLib

#endif
