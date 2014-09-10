/*
  vtobjectoutputlist_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectoutputlist_c.h"

#ifdef USE_VTOBJECT_outputlist
#include "vtclient_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"


namespace __IsoAgLib {

int16_t
vtObjectOutputList_c::stream(uint8_t* destMemory,
                            uint16_t maxBytes,
                            objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectOutputList_a
#define MACRO_vtObjectTypeS iVtObjectOutputList_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    MACRO_scaleSKLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 37; // Object Type = Output List
      if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) {
        destMemory [3] = (((uint32_t) vtObjectOutputList_a->width*factorM)/factorD) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectOutputList_a->width*factorM)/factorD) >> 8;
        destMemory [5] = (((uint32_t) vtObjectOutputList_a->height*factorM)/factorD) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectOutputList_a->height*factorM)/factorD) >> 8;
      } else {
        destMemory [3] = (((uint32_t) vtObjectOutputList_a->width*vtDimension)/opDimension) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectOutputList_a->width*vtDimension)/opDimension) >> 8;
        destMemory [5] = (((uint32_t) vtObjectOutputList_a->height*vtDimension)/opDimension) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectOutputList_a->height*vtDimension)/opDimension) >> 8;
      }
      if (vtObjectOutputList_a->variableReference != NULL) {
        destMemory [7] = vtObjectOutputList_a->variableReference->getID() & 0xFF;
        destMemory [8] = vtObjectOutputList_a->variableReference->getID() >> 8;
      } else {
        destMemory [7] = 0xFF;
        destMemory [8] = 0xFF;
      }
      destMemory [9] = vtObjectOutputList_a->value;

      destMemory [10] = vtObjectOutputList_a->numberOfObjectsToFollow;
      destMemory [11] = vtObjectOutputList_a->numberOfMacrosToFollow;
      sourceOffset += 12;
      curBytes += 12;
    }

    MACRO_streamObject(12);
    MACRO_streamEventMacro(12U+vtObjectOutputList_a->numberOfObjectsToFollow*2U);
    return curBytes;
}


vtObjectOutputList_c::vtObjectOutputList_c() {}


IsoAgLib::iVtObject_c*
vtObjectOutputList_c::getListItem(uint8_t xth)
{
  return ((iVtObjectOutputList_s *) vtObject_a)->objectsToFollow[xth].vtObject;
}


uint8_t
vtObjectOutputList_c::getNumberOfListItems()
{
  return ((iVtObjectOutputList_s *) vtObject_a)->numberOfObjectsToFollow;
}


uint32_t
    vtObjectOutputList_c::fitTerminal() const
{
  MACRO_localVars;
  return 12+vtObjectOutputList_a->numberOfObjectsToFollow*2+vtObjectOutputList_a->numberOfMacrosToFollow*2;
}


void
vtObjectOutputList_c::setValue(uint8_t newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (get_vtObjectOutputList_a()->variableReference == NULL) {
    if (b_updateObject) saveValue8 (MACRO_getStructOffset(get_vtObjectOutputList_a(), value), sizeof(iVtObjectOutputList_s), newValue);

    __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeNumericValue (this, newValue, 0x00, 0x00, 0x00, b_enableReplaceOfCmd);
  }
}


void
vtObjectOutputList_c::setItem(uint8_t aui8_index, IsoAgLib::iVtObject_c* apc_object, bool b_enableReplaceOfCmd)
{
  uint8_t hi, lo;
  if (apc_object == NULL) {
    lo = 0xFF;
    hi = 0xFF;
  } else {
    lo = apc_object->getID() & 0xFF;
    hi = apc_object->getID() >> 8;
  }
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommand (177 /* Command: Command --- Parameter: Change List Item */,
                                                   this->getID() & 0xFF,
                                                   this->getID() >> 8,
                                                   aui8_index,
                                                   lo,
                                                   hi,
                                                   0xFF,
                                                   0xFF,
                                                   b_enableReplaceOfCmd
                                                   );
}

void
vtObjectOutputList_c::setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectOutputList_a(), width),  sizeof(iVtObjectOutputList_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectOutputList_a(), height), sizeof(iVtObjectOutputList_s), newHeight);
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSize (this, newWidth, newHeight, b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectOutputList_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectOutputList_a(), width), sizeof(iVtObjectOutputList_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectOutputList_a(), width), sizeof(iVtObjectOutputList_s));
}

uint16_t
vtObjectOutputList_c::updateHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectOutputList_a(), height), sizeof(iVtObjectOutputList_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectOutputList_a(), height), sizeof(iVtObjectOutputList_s));
}

uint16_t
vtObjectOutputList_c::updateVariableReference(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectOutputList_a(), variableReference), sizeof(iVtObjectOutputList_s), 3);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectOutputList_a(), variableReference), sizeof(iVtObjectOutputList_s));
}

/** these attributes are in parentheses in the spec, so commented out here
uint8_t
vtObjectOutputList_c::updateValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectOutputList_a(), value), sizeof(iVtObjectOutputList_s), 4);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectOutputList_a(), value), sizeof(iVtObjectOutputList_s));
}

uint8_t
vtObjectOutputList_c::updateOptions(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectOutputList_a(), enabled), sizeof(iVtObjectOutputList_s), 5);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectOutputList_a(), enabled), sizeof(iVtObjectOutputList_s));
}
*/

void
vtObjectOutputList_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectOutputList_a(), width), sizeof(iVtObjectOutputList_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectOutputList_a(), height), sizeof(iVtObjectOutputList_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue16(MACRO_getStructOffset(get_vtObjectOutputList_a(), variableReference), sizeof(iVtObjectOutputList_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    /** these attributes are in parentheses in the spec, so commented out here
    case 4: saveValue8(MACRO_getStructOffset(get_vtObjectOutputList_a(), value), sizeof(iVtObjectOutputList_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 5: saveValue8(MACRO_getStructOffset(get_vtObjectOutputList_a(), enabled), sizeof(iVtObjectOutputList_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    */
    default: break;
  }
}
#endif

} // __IsoAgLib

#endif
