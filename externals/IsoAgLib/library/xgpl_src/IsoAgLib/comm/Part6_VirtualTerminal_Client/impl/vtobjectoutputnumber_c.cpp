/*
  vtobjectoutputnumber_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectoutputnumber_c.h"

#ifdef USE_VTOBJECT_outputnumber
#include <IsoAgLib/util/impl/util_funcs.h>
#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectOutputNumber_c::stream(uint8_t* destMemory,
                               uint16_t maxBytes,
                               objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectOutputNumber_a
#define MACRO_vtObjectTypeS iVtObjectOutputNumber_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    MACRO_scaleSKLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 12; // Object Type = Output Number
      if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) {
        destMemory [3] = (((uint32_t) vtObjectOutputNumber_a->width*factorM)/factorD) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectOutputNumber_a->width*factorM)/factorD) >> 8;
        destMemory [5] = (((uint32_t) vtObjectOutputNumber_a->height*factorM)/factorD) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectOutputNumber_a->height*factorM)/factorD) >> 8;
      } else {
        destMemory [3] = (((uint32_t) vtObjectOutputNumber_a->width*vtDimension)/opDimension) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectOutputNumber_a->width*vtDimension)/opDimension) >> 8;
        destMemory [5] = (((uint32_t) vtObjectOutputNumber_a->height*vtDimension)/opDimension) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectOutputNumber_a->height*vtDimension)/opDimension) >> 8;
      }
      destMemory [7] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectOutputNumber_a->backgroundColour, this, IsoAgLib::BackgroundColour);
      destMemory [8] = vtObjectOutputNumber_a->fontAttributes->getID() & 0xFF;
      destMemory [9] = vtObjectOutputNumber_a->fontAttributes->getID() >> 8;
      destMemory [10] = vtObjectOutputNumber_a->options;
      if (vtObjectOutputNumber_a->variableReference != NULL) {
        destMemory [11] = vtObjectOutputNumber_a->variableReference->getID() & 0xFF;
        destMemory [12] = vtObjectOutputNumber_a->variableReference->getID() >> 8;
      } else {
        destMemory [11] = 0xFF;
        destMemory [12] = 0xFF;
      }
      destMemory [13] = (vtObjectOutputNumber_a->value) & 0xFF;
      destMemory [14] = (vtObjectOutputNumber_a->value >> 8) & 0xFF;
      destMemory [15] = (vtObjectOutputNumber_a->value >> 16) & 0xFF;
      destMemory [16] = (vtObjectOutputNumber_a->value >> 24) & 0xFF;

      uint32_t offset = *((uint32_t*)&(vtObjectOutputNumber_a->offset));
      destMemory [17] = (offset) & 0xFF;
      destMemory [18] = (offset >> 8) & 0xFF;
      destMemory [19] = (offset >> 16) & 0xFF;
      destMemory [20] = (offset >> 24) & 0xFF;

      __IsoAgLib::floatVar2LittleEndianStream (&vtObjectOutputNumber_a->scale, &destMemory[21]);

      destMemory [25] = vtObjectOutputNumber_a->numberOfDecimals;
      destMemory [26] = vtObjectOutputNumber_a->format;
      destMemory [27] = vtObjectOutputNumber_a->horizontalJustification;
      destMemory [28] = vtObjectOutputNumber_a->numberOfMacrosToFollow;
      sourceOffset += 29;
      curBytes += 29;
    }

    MACRO_streamEventMacro(29);
    return curBytes;
}


vtObjectOutputNumber_c::vtObjectOutputNumber_c() {}


uint32_t
vtObjectOutputNumber_c::fitTerminal() const
{
  MACRO_localVars;
  return 29+vtObjectOutputNumber_a->numberOfMacrosToFollow*2;
}


void
vtObjectOutputNumber_c::setValue(uint32_t newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (get_vtObjectOutputNumber_a()->variableReference == NULL) {
    // Save Value if requested
    if (b_updateObject) saveValue32 (MACRO_getStructOffset(get_vtObjectOutputNumber_a(), value), sizeof(iVtObjectOutputNumber_s), newValue);

    // Send Value update
    __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeNumericValue (this, newValue & 0xFF, (newValue >> 8) & 0xFF, (newValue >> 16) & 0xFF, newValue >> 24, b_enableReplaceOfCmd);
  }
}


void
vtObjectOutputNumber_c::setOriginSKM(bool b_SKM)
{
  MACRO_localVars;
  if (b_SKM) {
    s_properties.flags |= FLAG_ORIGIN_SKM;
    vtObjectOutputNumber_a->fontAttributes->setOriginSKM (b_SKM);
  }
}

void
vtObjectOutputNumber_c::setOriginBTN(IsoAgLib::iVtObjectButton_c* p_btn)
{
  MACRO_localVars;
  if (p_btn)
  {
    p_parentButtonObject = p_btn;
    vtObjectOutputNumber_a->fontAttributes->setOriginBTN (p_btn);
  }
}

void
vtObjectOutputNumber_c::setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectOutputNumber_a(), width),  sizeof(iVtObjectOutputNumber_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectOutputNumber_a(), height), sizeof(iVtObjectOutputNumber_s), newHeight);
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSize (this, newWidth, newHeight, b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectOutputNumber_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), width), sizeof(iVtObjectOutputNumber_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), width), sizeof(iVtObjectOutputNumber_s));
}

uint16_t
vtObjectOutputNumber_c::updateHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), height), sizeof(iVtObjectOutputNumber_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), height), sizeof(iVtObjectOutputNumber_s));
}

uint8_t
vtObjectOutputNumber_c::updateBackgroundColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), backgroundColour), sizeof(iVtObjectOutputNumber_s), 3);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), backgroundColour), sizeof(iVtObjectOutputNumber_s));
}

uint16_t
vtObjectOutputNumber_c::updateFontAttributes(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), fontAttributes), sizeof(iVtObjectOutputNumber_s), 4);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), fontAttributes), sizeof(iVtObjectOutputNumber_s));
}

uint8_t
vtObjectOutputNumber_c::updateOptions(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), options), sizeof(iVtObjectOutputNumber_s), 5);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), options), sizeof(iVtObjectOutputNumber_s));
}

uint16_t
vtObjectOutputNumber_c::updateVariableReference(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), variableReference), sizeof(iVtObjectOutputNumber_s), 6);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), variableReference), sizeof(iVtObjectOutputNumber_s));
}

int32_t
vtObjectOutputNumber_c::updateOffset(bool b_SendRequest)
{
  if (b_SendRequest)
    return getSignedValue32GetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), offset), sizeof(iVtObjectOutputNumber_s), 7);
  else
    return getSignedValue32(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), offset), sizeof(iVtObjectOutputNumber_s));
}

float
vtObjectOutputNumber_c::updateScale(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValueFloatGetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), scale), sizeof(iVtObjectOutputNumber_s), 8);
  else
    return getValueFloat(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), scale), sizeof(iVtObjectOutputNumber_s));
}

uint8_t
vtObjectOutputNumber_c::updateNumberOfDecimals(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), numberOfDecimals), sizeof(iVtObjectOutputNumber_s), 9);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), numberOfDecimals), sizeof(iVtObjectOutputNumber_s));
}

uint8_t
vtObjectOutputNumber_c::updateFormat(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), format), sizeof(iVtObjectOutputNumber_s), 10);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), format), sizeof(iVtObjectOutputNumber_s));
}

uint8_t
vtObjectOutputNumber_c::updateJustification(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), horizontalJustification), sizeof(iVtObjectOutputNumber_s), 11);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), horizontalJustification), sizeof(iVtObjectOutputNumber_s));
}

/** that attribute is in parentheses in the spec, so commented out here
uint32_t
vtObjectOutputNumber_c::updateValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue32GetAttribute(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), value), sizeof(iVtObjectOutputNumber_s), 12);
  else
    return getValue32(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), value), sizeof(iVtObjectOutputNumber_s));
}
*/

void
vtObjectOutputNumber_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), width), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), height), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue8(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), backgroundColour), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 4: saveValue16(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), fontAttributes), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 5: saveValue8(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), options), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 6: saveValue16(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), variableReference), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 7: saveSignedValue32(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), offset), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringI32(pui8_attributeValue)); break;
    case 8: saveValueFloat(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), scale), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringFloat(pui8_attributeValue)); break;
    case 9: saveValue8(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), numberOfDecimals), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 10: saveValue8(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), format), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 11: saveValue8(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), horizontalJustification), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    /** that attribute is in parentheses in the spec, so commented out here
    case 12: saveValue32(MACRO_getStructOffset(get_vtObjectOutputNumber_a(), value), sizeof(iVtObjectOutputNumber_s), convertLittleEndianStringUi32(pui8_attributeValue)); break;
    */
    default: break;
  }
}
#endif

} // __IsoAgLib

#endif
