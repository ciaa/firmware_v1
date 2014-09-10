/*
  vtobjectarchedbargraph_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTARCHEDBARGRAPH_C_H
#define VTOBJECTARCHEDBARGRAPH_C_H

#include <IsoAgLib/isoaglib_config.h>


#ifdef USE_VTOBJECT_archedbargraph

#include "vtobject_c.h"
#include "vtclientconnection_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

class vtObjectArchedBarGraph_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectArchedBarGraph_s* vtObjectArchedBarGraphSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  { vtObject_c::init ((iVtObject_s*) vtObjectArchedBarGraphSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); }

  iVtObjectArchedBarGraph_s* get_vtObjectArchedBarGraph_a() { return (iVtObjectArchedBarGraph_s *)&(get_vtObject_a()); }

  vtObjectArchedBarGraph_c();
  ~vtObjectArchedBarGraph_c() {}

  uint32_t fitTerminal() const;

  void setValue(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=true);

  void setWidth(uint16_t newWidth, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), width) : 0, sizeof(iVtObjectArchedBarGraph_s), 1 /* "Width" */, newWidth, b_enableReplaceOfCmd);
  }

  void setHeight(uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), height) : 0, sizeof(iVtObjectArchedBarGraph_s), 2 /* "Height" */, newHeight, b_enableReplaceOfCmd);
  }

  void setColour(uint8_t newColour, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), colour) : 0, sizeof(iVtObjectArchedBarGraph_s), 3 /* "Colour" */, newColour, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newColour, this, IsoAgLib::Colour), b_enableReplaceOfCmd);
  }

  void setTargetLineColour(uint8_t newTargetLineColour, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetLineColour) : 0, sizeof(iVtObjectArchedBarGraph_s), 4 /* "Target Line Colour" */, newTargetLineColour, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newTargetLineColour, this, IsoAgLib::TargetLineColour), b_enableReplaceOfCmd);
  }

  void setOptions(uint8_t newOptions, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), options) : 0, sizeof(iVtObjectArchedBarGraph_s), 5 /* "Options" */, newOptions, newOptions, b_enableReplaceOfCmd);
  }

  void setStartAngle(uint8_t newStartAngle, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), startAngle) : 0, sizeof(iVtObjectArchedBarGraph_s), 6 /* "Start Angle" */, newStartAngle, newStartAngle, b_enableReplaceOfCmd);
  }

  void setEndAngle(uint8_t newEndAngle, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), endAngle) : 0, sizeof(iVtObjectArchedBarGraph_s), 7 /* "End Angle" */, newEndAngle, newEndAngle, b_enableReplaceOfCmd);
  }

  void setBarGraphWidth(uint16_t newBarGraphWidth, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), barGraphWidth) : 0, sizeof(iVtObjectArchedBarGraph_s), 8 /* "Bar Graph Width" */, newBarGraphWidth, b_enableReplaceOfCmd);
  }

  void setMinValue(uint16_t newMin, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), minValue) : 0, sizeof(iVtObjectArchedBarGraph_s), 9 /* "Min value" */, newMin, b_enableReplaceOfCmd);
  }

  void setMaxValue(uint16_t newMax, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), maxValue) : 0, sizeof(iVtObjectArchedBarGraph_s), 10 /* "Max value" */, newMax, b_enableReplaceOfCmd);
  }

  void setVariableReference(IsoAgLib::iVtObject_c* newVariableReference, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValuePSetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), variableReference) : 0, sizeof(iVtObjectArchedBarGraph_s), 11 /* "Variable Reference" */, newVariableReference, b_enableReplaceOfCmd);
  }

  void setTargetValueVariableReference(IsoAgLib::iVtObject_c* newTargetValueVariableReference, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValuePSetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetValueVariableReference) : 0, sizeof(iVtObjectArchedBarGraph_s), 12 /* "Target Value Variable Reference" */, newTargetValueVariableReference, b_enableReplaceOfCmd);
  }

  void setTargetValue(uint16_t newTargetValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectArchedBarGraph_a(), targetValue) : 0, sizeof(iVtObjectArchedBarGraph_s), 13 /* "Target Value" */, newTargetValue, b_enableReplaceOfCmd);
  }

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 19; }
  */

  uint16_t updateWidth(bool b_SendRequest=false);
  uint16_t updateHeight(bool b_SendRequest=false);
  uint8_t updateColour(bool b_SendRequest=false);
  uint8_t updateTargetLineColour(bool b_SendRequest=false);
  uint8_t updateOptions(bool b_SendRequest=false);
  uint8_t updateStartAngle(bool b_SendRequest=false);
  uint8_t updateEndAngle(bool b_SendRequest=false);
  uint16_t updateBarGraphWidth(bool b_SendRequest=false);
  uint16_t updateMinValue(bool b_SendRequest=false);
  uint16_t updateMaxValue(bool b_SendRequest=false);
  uint16_t updateVariableReference(bool b_SendRequest=false);
  uint16_t updateTargetValueVariableReference(bool b_SendRequest=false);
  uint16_t updateTargetValue(bool b_SendRequest=false);

  /** that attribute is in parentheses in the spec, so commented out here
  uint16_t updateValue(bool b_SendRequest=false);
  */

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif
};

} // __IsoAgLib

#endif

#endif
