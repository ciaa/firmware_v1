/*
  vtobjectobjectpointer_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectobjectpointer_c.h"

#ifdef USE_VTOBJECT_objectpointer
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectObjectPointer_c::stream(uint8_t* destMemory,
                                uint16_t /*maxBytes*/,
                                objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectObjectPointer_a
#define MACRO_vtObjectTypeS iVtObjectObjectPointer_s
    MACRO_localVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 27; // Object Type = Object Pointer
      if (vtObjectObjectPointer_a->value != NULL) {
        destMemory [3] = vtObjectObjectPointer_a->value->getID() & 0xFF;
        destMemory [4] = vtObjectObjectPointer_a->value->getID() >> 8;
      } else {
        destMemory [3] = 0xFF;
        destMemory [4] = 0xFF;
      }
      return 5;
    }
    return 0;
}


vtObjectObjectPointer_c::vtObjectObjectPointer_c() {}


uint32_t
vtObjectObjectPointer_c::fitTerminal() const
{
  return 5;
}


void
vtObjectObjectPointer_c::setValue(const IsoAgLib::iVtObject_c* apc_newObject, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) saveValueP (MACRO_getStructOffset(get_vtObjectObjectPointer_a(), value),  sizeof(iVtObjectObjectPointer_s), apc_newObject);

  if (apc_newObject != NULL) __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeNumericValue (this, apc_newObject->getID() & 0xFF, (apc_newObject->getID() >> 8) & 0xFF, 0x00, 0x00, b_enableReplaceOfCmd);
  else                       __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeNumericValue (this, 0xFF, 0xFF, 0x00, 0x00, b_enableReplaceOfCmd);
}

void
vtObjectObjectPointer_c::setOriginSKM(bool b_SKM)
{
  MACRO_localVars;
  if (vtObjectObjectPointer_a->value != NULL) {
    vtObjectObjectPointer_a->value->setOriginSKM (b_SKM);
  }
}

void
vtObjectObjectPointer_c::setOriginBTN(IsoAgLib::iVtObjectButton_c* p_btn)
{
  MACRO_localVars;
  if (p_btn) p_parentButtonObject = p_btn;
  if (vtObjectObjectPointer_a->value != NULL) {
    vtObjectObjectPointer_a->value->setOriginBTN (p_btn);
  }
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
/** that attribute is in parentheses in the spec, so commented out here
uint16_t
vtObjectObjectPointer_c::updateValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectObjectPointer_a(), value), sizeof(iVtObjectObjectPointer_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectObjectPointer_a(), value), sizeof(iVtObjectObjectPointer_s));
}
*/

void
vtObjectObjectPointer_c::saveReceivedAttribute(uint8_t /*attrID*/, uint8_t* /*pui8_attributeValue*/)
{
  /** that attribute is in parentheses in the spec, so commented out here
  case 1: saveValue16(MACRO_getStructOffset(get_vtObjectObjectPointer_a(), value), sizeof(iVtObjectObjectPointer_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
  default: break;
  */
}
#endif

} // __IsoAgLib

#endif
