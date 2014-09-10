/*
  vtobjectinputnumber_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectinputnumber_c.h"

#ifdef USE_VTOBJECT_inputnumber
#include <IsoAgLib/util/impl/util_funcs.h>
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectInputNumber_c::stream(uint8_t* destMemory,
                              uint16_t maxBytes,
                              objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectInputNumber_a
#define MACRO_vtObjectTypeS iVtObjectInputNumber_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 9; // Object Type = Input Number
      destMemory [3] = (((uint32_t) vtObjectInputNumber_a->width*vtDimension)/opDimension) & 0xFF;
      destMemory [4] = (((uint32_t) vtObjectInputNumber_a->width*vtDimension)/opDimension) >> 8;
      destMemory [5] = (((uint32_t) vtObjectInputNumber_a->height*vtDimension)/opDimension) & 0xFF;
      destMemory [6] = (((uint32_t) vtObjectInputNumber_a->height*vtDimension)/opDimension) >> 8;
      destMemory [7] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectInputNumber_a->backgroundColour, this, IsoAgLib::BackgroundColour);
      destMemory [8] = vtObjectInputNumber_a->fontAttributes->getID() & 0xFF;
      destMemory [9] = vtObjectInputNumber_a->fontAttributes->getID() >> 8;
      destMemory [10] = vtObjectInputNumber_a->options;
      if (vtObjectInputNumber_a->variableReference != NULL) {
          destMemory [11] = vtObjectInputNumber_a->variableReference->getID() & 0xFF;
          destMemory [12] = vtObjectInputNumber_a->variableReference->getID() >> 8;
      } else {
          destMemory [11] = 0xFF;
          destMemory [12] = 0xFF;
      }
      destMemory [13] = (vtObjectInputNumber_a->value) & 0xFF;
      destMemory [14] = (vtObjectInputNumber_a->value >> 8) & 0xFF;
      destMemory [15] = (vtObjectInputNumber_a->value >> 16) & 0xFF;
      destMemory [16] = (vtObjectInputNumber_a->value >> 24) & 0xFF;

      destMemory [17] = (vtObjectInputNumber_a->minValue) & 0xFF;
      destMemory [18] = (vtObjectInputNumber_a->minValue >> 8) & 0xFF;
      destMemory [19] = (vtObjectInputNumber_a->minValue >> 16) & 0xFF;
      destMemory [20] = (vtObjectInputNumber_a->minValue >> 24) & 0xFF;

      destMemory [21] = (vtObjectInputNumber_a->maxValue) & 0xFF;
      destMemory [22] = (vtObjectInputNumber_a->maxValue >> 8) & 0xFF;
      destMemory [23] = (vtObjectInputNumber_a->maxValue >> 16) & 0xFF;
      destMemory [24] = (vtObjectInputNumber_a->maxValue >> 24) & 0xFF;

      destMemory [25] = (vtObjectInputNumber_a->offset) & 0xFF;
      destMemory [26] = (vtObjectInputNumber_a->offset >> 8) & 0xFF;
      destMemory [27] = (vtObjectInputNumber_a->offset >> 16) & 0xFF;
      destMemory [28] = (vtObjectInputNumber_a->offset >> 24) & 0xFF;

      __IsoAgLib::floatVar2LittleEndianStream (&vtObjectInputNumber_a->scale, &destMemory[29]);

      destMemory [33] = vtObjectInputNumber_a->numberOfDecimals;
      destMemory [34] = vtObjectInputNumber_a->format;
      destMemory [35] = vtObjectInputNumber_a->horizontalJustification;
      destMemory [36] = vtObjectInputNumber_a->secondOptionsByte;
      destMemory [37] = vtObjectInputNumber_a->numberOfMacrosToFollow;
      sourceOffset += 38;
      curBytes += 38;
    }

    MACRO_streamEventMacro(38);
    return curBytes;
}


vtObjectInputNumber_c::vtObjectInputNumber_c() {}


uint32_t
vtObjectInputNumber_c::fitTerminal() const
{
  MACRO_localVars;
  return 38+vtObjectInputNumber_a->numberOfMacrosToFollow*2;
}


void
vtObjectInputNumber_c::updateEnable(uint8_t aui8_enOrDis)
{
  saveValue8 (MACRO_getStructOffset(get_vtObjectInputNumber_a(), secondOptionsByte), sizeof(iVtObjectInputNumber_s), aui8_enOrDis);
}


void
vtObjectInputNumber_c::setValue(uint32_t newValue,
                                bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (get_vtObjectInputNumber_a()->variableReference == NULL) {
    if (b_updateObject) saveValue32 (MACRO_getStructOffset(get_vtObjectInputNumber_a(), value),  sizeof(iVtObjectInputNumber_s), newValue);

    __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeNumericValue (this, newValue & 0xFF, (newValue >> 8) & 0xFF, (newValue >> 16) & 0xFF, newValue >> 24, b_enableReplaceOfCmd);
  }
}

void
vtObjectInputNumber_c::setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectInputNumber_a(), width),  sizeof(iVtObjectInputNumber_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectInputNumber_a(), height), sizeof(iVtObjectInputNumber_s), newHeight);
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSize (this, newWidth, newHeight, b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectInputNumber_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), width), sizeof(iVtObjectInputNumber_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectInputNumber_a(), width), sizeof(iVtObjectInputNumber_s));
}

uint16_t
vtObjectInputNumber_c::updateHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), height), sizeof(iVtObjectInputNumber_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectInputNumber_a(), height), sizeof(iVtObjectInputNumber_s));
}

uint8_t
vtObjectInputNumber_c::updateBackgroundColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), backgroundColour), sizeof(iVtObjectInputNumber_s), 3);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), backgroundColour), sizeof(iVtObjectInputNumber_s));
}

uint16_t
vtObjectInputNumber_c::updateFontAttributes(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), fontAttributes), sizeof(iVtObjectInputNumber_s), 4);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectInputNumber_a(), fontAttributes), sizeof(iVtObjectInputNumber_s));
}

uint8_t
vtObjectInputNumber_c::updateOptions(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), options), sizeof(iVtObjectInputNumber_s), 5);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), options), sizeof(iVtObjectInputNumber_s));
}

uint16_t
vtObjectInputNumber_c::updateVariableReference(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), variableReference), sizeof(iVtObjectInputNumber_s), 6);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectInputNumber_a(), variableReference), sizeof(iVtObjectInputNumber_s));
}

uint32_t
vtObjectInputNumber_c::updateMinValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue32GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), minValue), sizeof(iVtObjectInputNumber_s), 7);
  else
    return getValue32(MACRO_getStructOffset(get_vtObjectInputNumber_a(), minValue), sizeof(iVtObjectInputNumber_s));
}

uint32_t
vtObjectInputNumber_c::updateMaxValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue32GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), maxValue), sizeof(iVtObjectInputNumber_s), 8);
  else
    return getValue32(MACRO_getStructOffset(get_vtObjectInputNumber_a(), maxValue), sizeof(iVtObjectInputNumber_s));
}

int32_t
vtObjectInputNumber_c::updateOffset(bool b_SendRequest)
{
  if (b_SendRequest)
    return getSignedValue32GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), offset), sizeof(iVtObjectInputNumber_s), 9);
  else
    return getSignedValue32(MACRO_getStructOffset(get_vtObjectInputNumber_a(), offset), sizeof(iVtObjectInputNumber_s));
}

float
vtObjectInputNumber_c::updateScale(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValueFloatGetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), scale), sizeof(iVtObjectInputNumber_s), 10);
  else
    return getValueFloat(MACRO_getStructOffset(get_vtObjectInputNumber_a(), scale), sizeof(iVtObjectInputNumber_s));
}

uint8_t
vtObjectInputNumber_c::updateNumberOfDecimals(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), numberOfDecimals), sizeof(iVtObjectInputNumber_s), 11);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), numberOfDecimals), sizeof(iVtObjectInputNumber_s));
}

uint8_t
vtObjectInputNumber_c::updateFormat(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), format), sizeof(iVtObjectInputNumber_s), 12);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), format), sizeof(iVtObjectInputNumber_s));
}

uint8_t
vtObjectInputNumber_c::updateJustification(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), horizontalJustification), sizeof(iVtObjectInputNumber_s), 13);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), horizontalJustification), sizeof(iVtObjectInputNumber_s));
}

/** these attributes are in parentheses in the spec, so commented out here
uint32_t
vtObjectInputNumber_c::updateValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue32GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), value), sizeof(iVtObjectInputNumber_s), 14);
  else
    return getValue32(MACRO_getStructOffset(get_vtObjectInputNumber_a(), value), sizeof(iVtObjectInputNumber_s));
}

uint8_t
vtObjectInputNumber_c::update2ndOptionsByte(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectInputNumber_a(), secondOptionsByte), sizeof(iVtObjectInputNumber_s), 15);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), secondOptionsByte), sizeof(iVtObjectInputNumber_s));
}
*/

void
vtObjectInputNumber_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectInputNumber_a(), width), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectInputNumber_a(), height), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), backgroundColour), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 4: saveValue16(MACRO_getStructOffset(get_vtObjectInputNumber_a(), fontAttributes), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 5: saveValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), options), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 6: saveValue16(MACRO_getStructOffset(get_vtObjectInputNumber_a(), variableReference), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 7: saveValue32(MACRO_getStructOffset(get_vtObjectInputNumber_a(), minValue), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi32(pui8_attributeValue)); break;
    case 8: saveValue32(MACRO_getStructOffset(get_vtObjectInputNumber_a(), maxValue), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi32(pui8_attributeValue)); break;
    case 9: saveSignedValue32(MACRO_getStructOffset(get_vtObjectInputNumber_a(), offset), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringI32(pui8_attributeValue)); break;
    case 10: saveValueFloat(MACRO_getStructOffset(get_vtObjectInputNumber_a(), scale), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringFloat(pui8_attributeValue)); break;
    case 11: saveValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), numberOfDecimals), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 12: saveValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), format), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 13: saveValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), horizontalJustification), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    /** these attributes are in parentheses in the spec, so commented out here
    case 14: saveValue32(MACRO_getStructOffset(get_vtObjectInputNumber_a(), value), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi32(pui8_attributeValue)); break;
    case 15: saveValue8(MACRO_getStructOffset(get_vtObjectInputNumber_a(), secondOptionsByte), sizeof(iVtObjectInputNumber_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    */
    default: break;
  }
}
#endif

} // __IsoAgLib

#endif
