/*
  vtobjectinputboolean_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectinputboolean_c.h"

#ifdef USE_VTOBJECT_inputboolean
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectInputBoolean_c::stream(uint8_t* destMemory,
                               uint16_t maxBytes,
                               objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectInputBoolean_a
#define MACRO_vtObjectTypeS iVtObjectInputBoolean_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 7; // Object Type = Input Boolean
      destMemory [3] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectInputBoolean_a->backgroundColour, this, IsoAgLib::BackgroundColour);
      destMemory [4] = (((uint32_t) vtObjectInputBoolean_a->width*vtDimension)/opDimension) & 0xFF;
      destMemory [5] = (((uint32_t) vtObjectInputBoolean_a->width*vtDimension)/opDimension) >> 8;
      destMemory [6] = vtObjectInputBoolean_a->foregroundColour->getID() & 0xFF;
      destMemory [7] = vtObjectInputBoolean_a->foregroundColour->getID() >> 8;
      if (vtObjectInputBoolean_a->variableReference != NULL) {
        destMemory [8] = vtObjectInputBoolean_a->variableReference->getID() & 0xFF;
        destMemory [9] = vtObjectInputBoolean_a->variableReference->getID() >> 8;
      } else {
        destMemory [8] = 0xFF;
        destMemory [9] = 0xFF;
      }
      destMemory [10] = vtObjectInputBoolean_a->value;
      destMemory [11] = vtObjectInputBoolean_a->enabled;
      destMemory [12] = vtObjectInputBoolean_a->numberOfMacrosToFollow;
      sourceOffset += 13;
      curBytes += 13;
    }

    MACRO_streamEventMacro(13);
    return curBytes;
}



vtObjectInputBoolean_c::vtObjectInputBoolean_c() {}


uint32_t
vtObjectInputBoolean_c::fitTerminal() const
{
  MACRO_localVars;
  return 13+vtObjectInputBoolean_a->numberOfMacrosToFollow*2;
}


void
vtObjectInputBoolean_c::updateEnable(uint8_t aui8_enOrDis)
{
  saveValue8 (MACRO_getStructOffset(get_vtObjectInputBoolean_a(), enabled), sizeof(iVtObjectInputBoolean_s), aui8_enOrDis);
}


void
vtObjectInputBoolean_c::setValue(bool newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (get_vtObjectInputBoolean_a()->variableReference == NULL) {
    if (b_updateObject) saveValue8 (MACRO_getStructOffset(get_vtObjectInputBoolean_a(), value),  sizeof(iVtObjectInputBoolean_s), newValue);
    __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeNumericValue (this, newValue?1:0, 0x00, 0x00, 0x00, b_enableReplaceOfCmd);
  }
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint8_t
vtObjectInputBoolean_c::updateBackgroundColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), backgroundColour), sizeof(iVtObjectInputBoolean_s), 1);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), backgroundColour), sizeof(iVtObjectInputBoolean_s));
}

uint16_t
vtObjectInputBoolean_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), width), sizeof(iVtObjectInputBoolean_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), width), sizeof(iVtObjectInputBoolean_s));
}

uint16_t
vtObjectInputBoolean_c::updateForegroundColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), foregroundColour), sizeof(iVtObjectInputBoolean_s), 3);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), foregroundColour), sizeof(iVtObjectInputBoolean_s));
}

uint16_t
vtObjectInputBoolean_c::updateVariableReference(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), variableReference), sizeof(iVtObjectInputBoolean_s), 4);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), variableReference), sizeof(iVtObjectInputBoolean_s));
}

/** these attributes are in parentheses in the spec, so commented out here
uint8_t
vtObjectInputBoolean_c::updateValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), value), sizeof(iVtObjectInputBoolean_s), 5);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), value), sizeof(iVtObjectInputBoolean_s));
}

uint8_t
vtObjectInputBoolean_c::updateEnabled(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), enabled), sizeof(iVtObjectInputBoolean_s), 6);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), enabled), sizeof(iVtObjectInputBoolean_s));
}
*/

void
vtObjectInputBoolean_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue8(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), backgroundColour), sizeof(iVtObjectInputBoolean_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), width), sizeof(iVtObjectInputBoolean_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue16(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), foregroundColour), sizeof(iVtObjectInputBoolean_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 4: saveValue16(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), variableReference), sizeof(iVtObjectInputBoolean_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    /** these attributes are in parentheses in the spec, so commented out here
    case 5: saveValue8(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), value), sizeof(iVtObjectInputBoolean_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 6: saveValue8(MACRO_getStructOffset(get_vtObjectInputBoolean_a(), enabled), sizeof(iVtObjectInputBoolean_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    */
    default: break;
  }
}
#endif

} // __IsoAgLib

#endif
