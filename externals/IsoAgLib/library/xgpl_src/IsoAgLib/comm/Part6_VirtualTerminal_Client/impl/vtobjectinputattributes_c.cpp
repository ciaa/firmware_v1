/*
  vtobjectinputattributes_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectinputattributes_c.h"

#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectInputAttributes_c::stream(uint8_t* destMemory,
                                  uint16_t maxBytes,
                                  objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectInputAttributes_a
#define MACRO_vtObjectTypeS iVtObjectInputAttributes_s
    MACRO_streamLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 26; // Object Type = Input Attributes
      destMemory [3] = vtObjectInputAttributes_a->validationType;

      destMemory [4] = vtObjectInputAttributes_a->length;
      sourceOffset += 5;
      curBytes += 5;
    }

    while ((sourceOffset >= 5U) && (sourceOffset < (5U+vtObjectInputAttributes_a->length)) && ((curBytes+1) <= maxBytes))
    {
      if (vtObjectInputAttributes_a->validationString == NULL)
          destMemory [curBytes] = 0x00;
      else
          destMemory [curBytes] = vtObjectInputAttributes_a->validationString [sourceOffset-5];

      curBytes++;
      sourceOffset++;
    }

    if ((sourceOffset == (5U + vtObjectInputAttributes_a->length)) && ((curBytes+1) <= maxBytes))
    {
      destMemory [curBytes] = vtObjectInputAttributes_a->numberOfMacrosToFollow;
      curBytes++;
      sourceOffset++;
    }
    MACRO_streamEventMacro(6U+vtObjectInputAttributes_a->length);
    return curBytes;
}


vtObjectInputAttributes_c::vtObjectInputAttributes_c() {}


uint32_t
vtObjectInputAttributes_c::fitTerminal() const
{
  MACRO_localVars;
  return 6+vtObjectInputAttributes_a->length+vtObjectInputAttributes_a->numberOfMacrosToFollow*2;
}


void
vtObjectInputAttributes_c::setValidationStringCopy(const char* newValidationString, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    // check if not already RAM string buffer?
    if (!(s_properties.flags & FLAG_STRING_IN_RAM)) {
      s_properties.flags |= FLAG_STRING_IN_RAM;
      // create new String buffer with same length as original one, as the size can't be changed !!
      char *newStringBuffer = new char [get_vtObjectInputAttributes_a()->length+1];
      saveValueP (MACRO_getStructOffset(get_vtObjectInputAttributes_a(), validationString), sizeof(iVtObjectInputAttributes_s), (IsoAgLib::iVtObject_c*) newStringBuffer);
    }
    char *dest = get_vtObjectInputAttributes_a()->validationString;
    const char *src = newValidationString;
    int copyLen = (CNAMESPACE::strlen (newValidationString) <= get_vtObjectInputAttributes_a()->length) ? CNAMESPACE::strlen (newValidationString) : get_vtObjectInputAttributes_a()->length;
    int i=0; for (; i<copyLen; i++) *dest++ = *src++;
    for (; i<get_vtObjectInputAttributes_a()->length; i++) *dest++ = ' ';
    *dest = 0x00; // 0-termiante!
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeStringValue (this, newValidationString, get_vtObjectInputAttributes_a()->length, b_enableReplaceOfCmd);
}


void
vtObjectInputAttributes_c::setValidationStringRef(const char* newValidationString, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    // delete RAM_String first, before we lose the pointer!
    if (s_properties.flags & FLAG_STRING_IN_RAM) {
      delete (get_vtObjectInputAttributes_a()->validationString);
      s_properties.flags &= ~FLAG_STRING_IN_RAM;
    }

    saveValueP (MACRO_getStructOffset(get_vtObjectInputAttributes_a(), validationString), sizeof(iVtObjectInputAttributes_s), (IsoAgLib::iVtObject_c*) newValidationString);
  }

  setStringToStream( newValidationString ); // use MultiSendStreamer with mpc_stringToStream set!
  const uint16_t ui16_tempLen = (CNAMESPACE::strlen (newValidationString) <= get_vtObjectInputAttributes_a()->length) ? CNAMESPACE::strlen (newValidationString) : get_vtObjectInputAttributes_a()->length;
  setStrLenToSend( ui16_tempLen );
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeStringValue (this, b_enableReplaceOfCmd);
}



const char*
vtObjectInputAttributes_c::getString()
{
  return get_vtObjectInputAttributes_a()->validationString;
}
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
/** that attribute is in parentheses in the spec, so commented out here
uint8_t
vtObjectInputAttributes_c::updateValidationType(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputAttributes_a(), validationType), sizeof(iVtObjectInputAttributes_s), 1);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputAttributes_a(), validationType), sizeof(iVtObjectInputAttributes_s));
}
*/

void
vtObjectInputAttributes_c::saveReceivedAttribute(uint8_t /*attrID*/, uint8_t* /*pui8_attributeValue*/)
{
  /** that attribute is in parentheses in the spec, so commented out here
  if (attrID == 1)
    saveValue8(MACRO_getStructOffset(get_vtObjectInputAttributes_a(), validationType), sizeof(iVtObjectInputAttributes_s), convertLittleEndianStringUi8(pui8_attributeValue));
  */
}
#endif

} // __IsoAgLib
