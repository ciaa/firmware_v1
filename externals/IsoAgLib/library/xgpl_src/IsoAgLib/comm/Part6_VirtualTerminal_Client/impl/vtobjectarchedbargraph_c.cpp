/*
  vtobjectarchedbargraph_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectarchedbargraph_c.h"

#ifdef USE_VTOBJECT_archedbargraph
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectArchedBarGraph_c::stream(uint8_t* destMemory,
                                 uint16_t maxBytes,
                                 objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectArchedBarGraph_a
#define MACRO_vtObjectTypeS iVtObjectArchedBarGraph_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    MACRO_scaleSKLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 19; // Object Type = Arched Bar Graph
      if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) {
        destMemory [3] = (((uint32_t) vtObjectArchedBarGraph_a->width*factorM)/factorD) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectArchedBarGraph_a->width*factorM)/factorD) >> 8;
        destMemory [5] = (((uint32_t) vtObjectArchedBarGraph_a->height*factorM)/factorD) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectArchedBarGraph_a->height*factorM)/factorD) >> 8;
      } else {
        destMemory [3] = (((uint32_t) vtObjectArchedBarGraph_a->width*vtDimension)/opDimension) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectArchedBarGraph_a->width*vtDimension)/opDimension) >> 8;
        destMemory [5] = (((uint32_t) vtObjectArchedBarGraph_a->height*vtDimension)/opDimension) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectArchedBarGraph_a->height*vtDimension)/opDimension) >> 8;
      }
      destMemory [7] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectArchedBarGraph_a->colour, this, IsoAgLib::Colour);
      destMemory [8] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectArchedBarGraph_a->targetLineColour, this, IsoAgLib::TargetLineColour);
      destMemory [9] = vtObjectArchedBarGraph_a->options;
      destMemory [10] = vtObjectArchedBarGraph_a->startAngle;
      destMemory [11] = vtObjectArchedBarGraph_a->endAngle;
      if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject)
      {
        destMemory [12] = (((uint32_t) vtObjectArchedBarGraph_a->barGraphWidth*factorM)/factorD) & 0xFF;
        destMemory [13] = (((uint32_t) vtObjectArchedBarGraph_a->barGraphWidth*factorM)/factorD) >> 8;
      }
      else
      {
        destMemory [12] = (((uint32_t) vtObjectArchedBarGraph_a->barGraphWidth*vtDimension)/opDimension) & 0xFF;
        destMemory [13] = (((uint32_t) vtObjectArchedBarGraph_a->barGraphWidth*vtDimension)/opDimension) >> 8;
      }
      if ( (vtObjectArchedBarGraph_a->barGraphWidth > 0) &&
           (destMemory [12] == 0) && (destMemory [13] == 0))
      { // if the arch had a linewidth > 0, then don't allow downscaling to let it become 0!
        destMemory [12] = 0x01;
      //destMemory [13] = 0x00; // is already set to 0 as checked in the if-statement!
      }
      destMemory [14] = vtObjectArchedBarGraph_a->minValue & 0xFF;
      destMemory [15] = vtObjectArchedBarGraph_a->minValue >> 8;
      destMemory [16] = vtObjectArchedBarGraph_a->maxValue & 0xFF;
      destMemory [17] = vtObjectArchedBarGraph_a->maxValue >> 8;
      if (vtObjectArchedBarGraph_a->variableReference != NULL) {
        destMemory [18] = vtObjectArchedBarGraph_a->variableReference->getID() & 0xFF;
        destMemory [19] = vtObjectArchedBarGraph_a->variableReference->getID() >> 8;
      } else {
        destMemory [18] = 0xFF;
        destMemory [19] = 0xFF;
      }
      destMemory [20] = vtObjectArchedBarGraph_a->value & 0xFF;
      destMemory [21] = vtObjectArchedBarGraph_a->value >> 8;
      if (vtObjectArchedBarGraph_a->targetValueVariableReference != NULL) {
        destMemory [22] = vtObjectArchedBarGraph_a->targetValueVariableReference->getID() & 0xFF;
        destMemory [23] = vtObjectArchedBarGraph_a->targetValueVariableReference->getID() >> 8;
      } else {
        destMemory [22] = 0xFF;
        destMemory [23] = 0xFF;
      }
      destMemory [24] = vtObjectArchedBarGraph_a->targetValue & 0xFF;
      destMemory [25] = vtObjectArchedBarGraph_a->targetValue >> 8;
      destMemory [26] = vtObjectArchedBarGraph_a->numberOfMacrosToFollow;
      sourceOffset += 27;
      curBytes += 27;
    }

    MACRO_streamEventMacro(27);
    return curBytes;
}


vtObjectArchedBarGraph_c::vtObjectArchedBarGraph_c() {}


uint32_t
vtObjectArchedBarGraph_c::fitTerminal() const
{
  MACRO_localVars;
  return 27+vtObjectArchedBarGraph_a->numberOfMacrosToFollow*2;
}


void
vtObjectArchedBarGraph_c::setValue(uint16_t newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (get_vtObjectArchedBarGraph_a()->variableReference == NULL) {
    if (b_updateObject) saveValue16 (MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), value), sizeof(iVtObjectArchedBarGraph_s), newValue);

    __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeNumericValue (this, newValue & 0xFF, (newValue >> 8) & 0xFF, 0x00, 0x00, b_enableReplaceOfCmd);
  }
}

void
vtObjectArchedBarGraph_c::setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), width),  sizeof(iVtObjectArchedBarGraph_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), height), sizeof(iVtObjectArchedBarGraph_s), newHeight);
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSize (this, newWidth, newHeight, b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectArchedBarGraph_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), width), sizeof(iVtObjectArchedBarGraph_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), width), sizeof(iVtObjectArchedBarGraph_s));
}

uint16_t
vtObjectArchedBarGraph_c::updateHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), height), sizeof(iVtObjectArchedBarGraph_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), height), sizeof(iVtObjectArchedBarGraph_s));
}

uint8_t
vtObjectArchedBarGraph_c::updateColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), colour), sizeof(iVtObjectArchedBarGraph_s), 3);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), colour), sizeof(iVtObjectArchedBarGraph_s));
}

uint8_t
vtObjectArchedBarGraph_c::updateTargetLineColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetLineColour), sizeof(iVtObjectArchedBarGraph_s), 4);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetLineColour), sizeof(iVtObjectArchedBarGraph_s));
}

uint8_t
vtObjectArchedBarGraph_c::updateOptions(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), options), sizeof(iVtObjectArchedBarGraph_s), 5);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), options), sizeof(iVtObjectArchedBarGraph_s));
}

uint8_t
vtObjectArchedBarGraph_c::updateStartAngle(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), startAngle), sizeof(iVtObjectArchedBarGraph_s), 6);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), startAngle), sizeof(iVtObjectArchedBarGraph_s));
}

uint8_t
vtObjectArchedBarGraph_c::updateEndAngle(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), endAngle), sizeof(iVtObjectArchedBarGraph_s), 7);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), endAngle), sizeof(iVtObjectArchedBarGraph_s));
}

uint16_t
vtObjectArchedBarGraph_c::updateBarGraphWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), barGraphWidth), sizeof(iVtObjectArchedBarGraph_s), 8);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), barGraphWidth), sizeof(iVtObjectArchedBarGraph_s));
}

uint16_t
vtObjectArchedBarGraph_c::updateMinValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), minValue), sizeof(iVtObjectArchedBarGraph_s), 9);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), minValue), sizeof(iVtObjectArchedBarGraph_s));
}

uint16_t
vtObjectArchedBarGraph_c::updateMaxValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), maxValue), sizeof(iVtObjectArchedBarGraph_s), 10);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), maxValue), sizeof(iVtObjectArchedBarGraph_s));
}

uint16_t
vtObjectArchedBarGraph_c::updateVariableReference(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), variableReference), sizeof(iVtObjectArchedBarGraph_s), 11);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), variableReference), sizeof(iVtObjectArchedBarGraph_s));
}

uint16_t
vtObjectArchedBarGraph_c::updateTargetValueVariableReference(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetValueVariableReference), sizeof(iVtObjectArchedBarGraph_s), 12);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetValueVariableReference), sizeof(iVtObjectArchedBarGraph_s));
}

uint16_t
vtObjectArchedBarGraph_c::updateTargetValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetValue), sizeof(iVtObjectArchedBarGraph_s), 13);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetValue), sizeof(iVtObjectArchedBarGraph_s));
}

/** that attribute is in parentheses in the spec, so commented out here
uint16_t
vtObjectArchedBarGraph_c::updateValue(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), value), sizeof(iVtObjectArchedBarGraph_s), 14);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), value), sizeof(iVtObjectArchedBarGraph_s));
}
*/

void
vtObjectArchedBarGraph_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), width), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), height), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue8(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), colour), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 4: saveValue8(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetLineColour), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 5: saveValue8(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), options), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 6: saveValue8(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), startAngle), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 7: saveValue8(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), endAngle), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 8: saveValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), barGraphWidth), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 9: saveValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), minValue), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 10: saveValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), maxValue), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 11: saveValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), variableReference), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 12: saveValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetValueVariableReference), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 13: saveValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetValue), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    /** that attribute is in parentheses in the spec, so commented out here
    case 14: saveValue16(MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), value), sizeof(iVtObjectArchedBarGraph_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    */
    default: break;
  }
}
#endif
} // __IsoAgLib
#endif
