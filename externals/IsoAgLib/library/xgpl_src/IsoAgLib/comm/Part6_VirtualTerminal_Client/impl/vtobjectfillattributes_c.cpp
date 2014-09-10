/*
  vtobjectfillattributes_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "vtobjectfillattributes_c.h"

#include "../ivtobjectpicturegraphic_c.h"
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectFillAttributes_c::stream(uint8_t* destMemory,
                                 uint16_t maxBytes,
                                 objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectFillAttributes_a
#define MACRO_vtObjectTypeS iVtObjectFillAttributes_s
    MACRO_streamLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 25; // Object Type = Fill Attributes
      destMemory [3] = vtObjectFillAttributes_a->fillType;
      destMemory [4] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectFillAttributes_a->fillColour, this, IsoAgLib::FillColour);
      if (vtObjectFillAttributes_a->fillPatternObject != NULL)
      {
        destMemory [5] = vtObjectFillAttributes_a->fillPatternObject->getID() & 0xFF;
        destMemory [6] = vtObjectFillAttributes_a->fillPatternObject->getID() >> 8;
      }
      else
      {
        destMemory [5] = 0xFF;
        destMemory [6] = 0xFF;
      }
      destMemory [7] = vtObjectFillAttributes_a->numberOfMacrosToFollow;
      sourceOffset += 8;
      curBytes += 8;
    }

    MACRO_streamEventMacro(8);
    return curBytes;
}


vtObjectFillAttributes_c::vtObjectFillAttributes_c() {}


uint32_t
vtObjectFillAttributes_c::fitTerminal() const
{
  MACRO_localVars;
  return 8+vtObjectFillAttributes_a->numberOfMacrosToFollow*2;
}

void
vtObjectFillAttributes_c::setFillAttributes(uint8_t newFillType, uint8_t newFillColour, IsoAgLib::iVtObjectPictureGraphic_c* newFillPattern, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue8 (MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillType),   sizeof(iVtObjectFillAttributes_s), newFillType);
    saveValue8 (MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillColour), sizeof(iVtObjectFillAttributes_s), __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newFillColour, this, IsoAgLib::FillColour));
    saveValueP (MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillPatternObject), sizeof(iVtObjectFillAttributes_s), newFillPattern);
  }
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeFillAttributes (this, newFillType, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor(
    newFillColour, this, IsoAgLib::FillColour), newFillPattern, b_enableReplaceOfCmd );
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint8_t
vtObjectFillAttributes_c::updateFillType(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillType), sizeof(iVtObjectFillAttributes_s), 1);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillType), sizeof(iVtObjectFillAttributes_s));
}

uint8_t
vtObjectFillAttributes_c::updateFillColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillColour), sizeof(iVtObjectFillAttributes_s), 2);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillColour), sizeof(iVtObjectFillAttributes_s));
}

uint16_t
vtObjectFillAttributes_c::updateFillPattern(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillPatternObject), sizeof(iVtObjectFillAttributes_s), 3);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillPatternObject), sizeof(iVtObjectFillAttributes_s));
}

void
vtObjectFillAttributes_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue8(MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillType), sizeof(iVtObjectFillAttributes_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 2: saveValue8(MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillColour), sizeof(iVtObjectFillAttributes_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 3: saveValue16(MACRO_getStructOffset(get_vtObjectFillAttributes_a(), fillPatternObject), sizeof(iVtObjectFillAttributes_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    default: break;
  }
}
#endif
} // __IsoAgLib
