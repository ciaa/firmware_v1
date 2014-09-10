/*
  vtobjectinputlist_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "vtobjectinputlist_c.h"

#ifdef USE_VTOBJECT_inputlist
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectInputList_c::stream(uint8_t* destMemory,
                            uint16_t maxBytes,
                            objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectInputList_a
#define MACRO_vtObjectTypeS iVtObjectInputList_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 10; // Object Type = Input List
      destMemory [3] = (((uint32_t) vtObjectInputList_a->width*vtDimension)/opDimension) & 0xFF;
      destMemory [4] = (((uint32_t) vtObjectInputList_a->width*vtDimension)/opDimension) >> 8;
      destMemory [5] = (((uint32_t) vtObjectInputList_a->height*vtDimension)/opDimension) & 0xFF;
      destMemory [6] = (((uint32_t) vtObjectInputList_a->height*vtDimension)/opDimension) >> 8;
      if (vtObjectInputList_a->variableReference != NULL) {
        destMemory [7] = vtObjectInputList_a->variableReference->getID() & 0xFF;
        destMemory [8] = vtObjectInputList_a->variableReference->getID() >> 8;
      } else {
        destMemory [7] = 0xFF;
        destMemory [8] = 0xFF;
      }
      destMemory [9] = vtObjectInputList_a->value;

      destMemory [10] = vtObjectInputList_a->numberOfObjectsToFollow;
      destMemory [11] = vtObjectInputList_a->enabled;
      destMemory [12] = vtObjectInputList_a->numberOfMacrosToFollow;
      sourceOffset += 13;
      curBytes += 13;
    }

    MACRO_streamObject(13);
    MACRO_streamEventMacro(13U+vtObjectInputList_a->numberOfObjectsToFollow*2U);
    return curBytes;
}


vtObjectInputList_c::vtObjectInputList_c() {}


IsoAgLib::iVtObject_c*
vtObjectInputList_c::getListItem(uint8_t xth)
{
  return ((iVtObjectInputList_s *) vtObject_a)->objectsToFollow[xth].vtObject;
}


uint8_t
vtObjectInputList_c::getNumberOfListItems()
{
    return ((iVtObjectInputList_s *) vtObject_a)->numberOfObjectsToFollow;
}


uint32_t
vtObjectInputList_c::fitTerminal() const
{
  MACRO_localVars;
  return 13+vtObjectInputList_a->numberOfObjectsToFollow*2+vtObjectInputList_a->numberOfMacrosToFollow*2;
}


void
vtObjectInputList_c::updateEnable(uint8_t aui8_enOrDis)
{
  saveValue8 (MACRO_getStructOffset(get_vtObjectInputList_a(), enabled), sizeof(iVtObjectInputList_s), aui8_enOrDis);
}


void
vtObjectInputList_c::setValue(uint8_t newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (get_vtObjectInputList_a()->variableReference == NULL) {
    if (b_updateObject) saveValue8 (MACRO_getStructOffset(get_vtObjectInputList_a(), value), sizeof(iVtObjectInputList_s), newValue);

    __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeNumericValue (this, newValue, 0x00, 0x00, 0x00, b_enableReplaceOfCmd);
  }
}


void
vtObjectInputList_c::setItem(uint8_t aui8_index, IsoAgLib::iVtObject_c* apc_object, bool b_enableReplaceOfCmd)
{
  uint8_t hi, lo;
  if (apc_object == NULL) {
    lo = 0xFF;
    hi = 0xFF;
  } else {
    lo = apc_object->getID() & 0xFF;
    hi = apc_object->getID() >> 8;
  }
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommand(
    177 /* Command: Command --- Parameter: Change List Item */,
    this->getID() & 0xFF, this->getID() >> 8, aui8_index, lo, hi, 0xFF, 0xFF, b_enableReplaceOfCmd );
}

void
vtObjectInputList_c::setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectInputList_a(), width),  sizeof(iVtObjectInputList_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectInputList_a(), height), sizeof(iVtObjectInputList_s), newHeight);
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSize (this, newWidth, newHeight, b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectInputList_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectInputList_a(), width), sizeof(iVtObjectInputList_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectInputList_a(), width), sizeof(iVtObjectInputList_s));
}

uint16_t
vtObjectInputList_c::updateHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectInputList_a(), height), sizeof(iVtObjectInputList_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectInputList_a(), height), sizeof(iVtObjectInputList_s));
}

uint16_t
vtObjectInputList_c::updateVariableReference(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectInputList_a(), variableReference), sizeof(iVtObjectInputList_s), 3);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectInputList_a(), variableReference), sizeof(iVtObjectInputList_s));
}

/** these attributes are in parentheses in the spec, so commented out here
uint8_t
vtObjectInputList_c::updateValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputList_a(), value), sizeof(iVtObjectInputList_s), 4);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputList_a(), value), sizeof(iVtObjectInputList_s));
}

uint8_t
vtObjectInputList_c::updateOptions(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputList_a(), enabled), sizeof(iVtObjectInputList_s), 5);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputList_a(), enabled), sizeof(iVtObjectInputList_s));
}
*/

void
vtObjectInputList_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectInputList_a(), width), sizeof(iVtObjectInputList_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectInputList_a(), height), sizeof(iVtObjectInputList_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue16(MACRO_getStructOffset(get_vtObjectInputList_a(), variableReference), sizeof(iVtObjectInputList_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    /** these attributes are in parentheses in the spec, so commented out here
    case 4: saveValue8(MACRO_getStructOffset(get_vtObjectInputList_a(), value), sizeof(iVtObjectInputList_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 5: saveValue8(MACRO_getStructOffset(get_vtObjectInputList_a(), enabled), sizeof(iVtObjectInputList_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    */
    default: break;
  }
}
#endif

} // __IsoAgLib

#endif
