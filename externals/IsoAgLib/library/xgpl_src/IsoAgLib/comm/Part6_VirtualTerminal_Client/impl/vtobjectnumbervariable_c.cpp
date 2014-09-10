/*
  vtobjectnumbervariable_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectnumbervariable_c.h"

#ifdef USE_VTOBJECT_numbervariable 
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectNumberVariable_c::stream(uint8_t* destMemory,
                                 uint16_t /*maxBytes*/,
                                 objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectNumberVariable_a
#define MACRO_vtObjectTypeS iVtObjectNumberVariable_s
    MACRO_localVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 21; // Object Type = Number Variable

      destMemory [3] = (vtObjectNumberVariable_a->value) & 0xFF;
      destMemory [4] = (vtObjectNumberVariable_a->value >> 8) & 0xFF;
      destMemory [5] = (vtObjectNumberVariable_a->value >> 16) & 0xFF;
      destMemory [6] = (vtObjectNumberVariable_a->value >> 24) & 0xFF;
      return 7;
    }
    return 0;
}


vtObjectNumberVariable_c::vtObjectNumberVariable_c() {}


uint32_t
vtObjectNumberVariable_c::fitTerminal() const
{
  // 3 byte header and 4 byte attributes.
  return (3+4);
}


void
vtObjectNumberVariable_c::setValue(uint32_t newValue,
                                   bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) saveValue32 (MACRO_getStructOffset(get_vtObjectNumberVariable_a(), value), sizeof(iVtObjectNumberVariable_s), newValue);

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeNumericValue (this, newValue & 0xFF, (newValue >> 8) & 0xFF, (newValue >> 16) & 0xFF, newValue >> 24, b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
/** that attribute is in parentheses in the spec, so commented out here
uint32_t
vtObjectNumberVariable_c::updateValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue32GetAttribute(MACRO_getStructOffset(get_vtObjectNumberVariable_a(), value), sizeof(iVtObjectNumberVariable_s), 1);
  else
    return getValue32(MACRO_getStructOffset(get_vtObjectNumberVariable_a(), value), sizeof(iVtObjectNumberVariable_s));
}
*/

void
vtObjectNumberVariable_c::saveReceivedAttribute(uint8_t /*attrID*/, uint8_t* /*pui8_attributeValue*/)
{
  /** that attribute is in parentheses in the spec, so commented out here
  if (attrID == 1)
    saveValue32(MACRO_getStructOffset(get_vtObjectNumberVariable_a(), value), sizeof(iVtObjectNumberVariable_s), convertLittleEndianStringUi32(pui8_attributeValue));
  */
}
#endif

} // __IsoAgLib

#endif
