/*
  vtobjectmeter_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectmeter_c.h"

#ifdef USE_VTOBJECT_meter
#include "vtclient_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"


namespace __IsoAgLib {

int16_t
vtObjectMeter_c::stream(uint8_t* destMemory,
                        uint16_t maxBytes,
                        objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectMeter_a
#define MACRO_vtObjectTypeS iVtObjectMeter_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    MACRO_scaleSKLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 17; // Object Type = Meter
      if (s_properties.flags & FLAG_ORIGIN_SKM) { // no need to check for p_parentButtonObject as this object can't be nested in a button!
        destMemory [3] = (((uint32_t) vtObjectMeter_a->width*factorM)/factorD) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectMeter_a->width*factorM)/factorD) >> 8;
      } else {
        destMemory [3] = (((uint32_t) vtObjectMeter_a->width*vtDimension)/opDimension) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectMeter_a->width*vtDimension)/opDimension) >> 8;
      }
      destMemory [5] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectMeter_a->needleColour, this, IsoAgLib::NeedleColour);
      destMemory [6] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectMeter_a->borderColour, this, IsoAgLib::BorderColour);
      destMemory [7] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectMeter_a->arcAndTickColour, this, IsoAgLib::ArcAndTickColour);
      destMemory [8] = vtObjectMeter_a->options;
      destMemory [9] = vtObjectMeter_a->numberOfTicks;
      destMemory [10] = vtObjectMeter_a->startAngle;
      destMemory [11] = vtObjectMeter_a->endAngle;
      destMemory [12] = vtObjectMeter_a->minValue & 0xFF;
      destMemory [13] = vtObjectMeter_a->minValue >> 8;
      destMemory [14] = vtObjectMeter_a->maxValue & 0xFF;
      destMemory [15] = vtObjectMeter_a->maxValue >> 8;
      if (vtObjectMeter_a->variableReference != NULL) {
        destMemory [16] = vtObjectMeter_a->variableReference->getID() & 0xFF;
        destMemory [17] = vtObjectMeter_a->variableReference->getID() >> 8;
      } else {
        destMemory [16] = 0xFF;
        destMemory [17] = 0xFF;
      }
      destMemory [18] = vtObjectMeter_a->value & 0xFF;
      destMemory [19] = vtObjectMeter_a->value >> 8;

      destMemory [20] = vtObjectMeter_a->numberOfMacrosToFollow;
      sourceOffset += 21;
      curBytes += 21;
    }

    MACRO_streamEventMacro(21);
    return curBytes;
}


vtObjectMeter_c::vtObjectMeter_c() {}


uint32_t
vtObjectMeter_c::fitTerminal() const
{
  MACRO_localVars;
  return 21+vtObjectMeter_a->numberOfMacrosToFollow*2;
}


void
vtObjectMeter_c::setValue(uint16_t newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (get_vtObjectMeter_a()->variableReference == NULL) {
    if (b_updateObject) saveValue16 (MACRO_getStructOffset(get_vtObjectMeter_a(), value), sizeof(iVtObjectMeter_s), newValue);

    __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeNumericValue (this, newValue & 0xFF, (newValue >> 8) & 0xFF, 0x00, 0x00, b_enableReplaceOfCmd);
  }
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectMeter_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), width), sizeof(iVtObjectMeter_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectMeter_a(), width), sizeof(iVtObjectMeter_s));
}

uint8_t
vtObjectMeter_c::updateNeedleColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), needleColour), sizeof(iVtObjectMeter_s), 2);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), needleColour), sizeof(iVtObjectMeter_s));
}

uint8_t
vtObjectMeter_c::updateBorderColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), borderColour), sizeof(iVtObjectMeter_s), 3);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), borderColour), sizeof(iVtObjectMeter_s));
}

uint8_t
vtObjectMeter_c::updateArcAndTickColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), arcAndTickColour), sizeof(iVtObjectMeter_s), 4);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), arcAndTickColour), sizeof(iVtObjectMeter_s));
}

uint8_t
vtObjectMeter_c::updateOptions(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), options), sizeof(iVtObjectMeter_s), 5);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), options), sizeof(iVtObjectMeter_s));
}

uint8_t
vtObjectMeter_c::updateNumberOfTicks(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), numberOfTicks), sizeof(iVtObjectMeter_s), 6);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), numberOfTicks), sizeof(iVtObjectMeter_s));
}

uint8_t
vtObjectMeter_c::updateStartAngle(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), startAngle), sizeof(iVtObjectMeter_s), 7);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), startAngle), sizeof(iVtObjectMeter_s));
}

uint8_t
vtObjectMeter_c::updateEndAngle(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), endAngle), sizeof(iVtObjectMeter_s), 8);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), endAngle), sizeof(iVtObjectMeter_s));
}

uint16_t
vtObjectMeter_c::updateMinValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), minValue), sizeof(iVtObjectMeter_s), 9);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectMeter_a(), minValue), sizeof(iVtObjectMeter_s));
}

uint16_t
vtObjectMeter_c::updateMaxValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), maxValue), sizeof(iVtObjectMeter_s), 10);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectMeter_a(), maxValue), sizeof(iVtObjectMeter_s));
}

uint16_t
vtObjectMeter_c::updateVariableReference(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), variableReference), sizeof(iVtObjectMeter_s), 11);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectMeter_a(), variableReference), sizeof(iVtObjectMeter_s));
}

/** that attribute is in parentheses in the spec, so commented out here
uint16_t
vtObjectMeter_c::updateValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectMeter_a(), value), sizeof(iVtObjectMeter_s), 12);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectMeter_a(), value), sizeof(iVtObjectMeter_s));
}
*/

void
vtObjectMeter_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectMeter_a(), width), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), needleColour), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 3: saveValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), borderColour), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 4: saveValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), arcAndTickColour), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 5: saveValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), options), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 6: saveValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), numberOfTicks), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 7: saveValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), startAngle), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 8: saveValue8(MACRO_getStructOffset(get_vtObjectMeter_a(), endAngle), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 9: saveValue16(MACRO_getStructOffset(get_vtObjectMeter_a(), minValue), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 10: saveValue16(MACRO_getStructOffset(get_vtObjectMeter_a(), maxValue), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 11: saveValue16(MACRO_getStructOffset(get_vtObjectMeter_a(), variableReference), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    /** that attribute is in parentheses in the spec, so commented out here
    case 12: saveValue16(MACRO_getStructOffset(get_vtObjectMeter_a(), value), sizeof(iVtObjectMeter_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    */
    default: break;
  }
}
#endif

} // __IsoAgLib

#endif
