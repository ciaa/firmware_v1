/*
  vtobjectlinearbargraph_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTLINEARBARGRAPH_C_H
#define VTOBJECTLINEARBARGRAPH_C_H

#include <IsoAgLib/isoaglib_config.h>


#ifdef USE_VTOBJECT_linearbargraph

#include "vtobject_c.h"
#include "vtclientconnection_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

class vtObjectLinearBarGraph_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectLinearBarGraph_s* vtObjectLinearBarGraphSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  { vtObject_c::init ((iVtObject_s*) vtObjectLinearBarGraphSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); }

  iVtObjectLinearBarGraph_s* get_vtObjectLinearBarGraph_a() { return (iVtObjectLinearBarGraph_s *)&(get_vtObject_a()); }

  vtObjectLinearBarGraph_c();

  uint32_t fitTerminal() const;

  // //////////////////////////////////
  // All special Attribute-Set methods
  void setWidth(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLinearBarGraph_a(), width) : 0, sizeof(iVtObjectLinearBarGraph_s), 1, newValue, b_enableReplaceOfCmd);
  }
  void setHeight(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLinearBarGraph_a(), height) : 0, sizeof(iVtObjectLinearBarGraph_s), 2, newValue, b_enableReplaceOfCmd);
  }
  void setColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLinearBarGraph_a(), colour) : 0, sizeof(iVtObjectLinearBarGraph_s), 3, newValue, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newValue, this, IsoAgLib::Colour), b_enableReplaceOfCmd);
  }
  void setTargetLineColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLinearBarGraph_a(), targetLineColour) : 0, sizeof(iVtObjectLinearBarGraph_s), 4, newValue, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newValue, this, IsoAgLib::TargetLineColour), b_enableReplaceOfCmd);
  }
  void setOptions(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLinearBarGraph_a(), options) : 0, sizeof(iVtObjectLinearBarGraph_s), 5, newValue, newValue, b_enableReplaceOfCmd);
  }
  void setNumberOfTicks(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLinearBarGraph_a(), numberOfTicks) : 0, sizeof(iVtObjectLinearBarGraph_s), 6, newValue, newValue, b_enableReplaceOfCmd);
  }
  void setMinValue(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLinearBarGraph_a(), minValue) : 0, sizeof(iVtObjectLinearBarGraph_s), 7, newValue, b_enableReplaceOfCmd);
  }
  void setMaxValue(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLinearBarGraph_a(), maxValue) : 0, sizeof(iVtObjectLinearBarGraph_s), 8, newValue, b_enableReplaceOfCmd);
  }
  void setVariableReference(IsoAgLib::iVtObject_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValuePSetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLinearBarGraph_a(), variableReference) : 0, sizeof(iVtObjectLinearBarGraph_s), 9, newValue, b_enableReplaceOfCmd);
  }

  void setValue(uint16_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=true);

  void setTargetValueVariableReference(IsoAgLib::iVtObject_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValuePSetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLinearBarGraph_a(), targetValueVariableReference) : 0, sizeof(iVtObjectLinearBarGraph_s), 10, newValue, b_enableReplaceOfCmd);
  }
  void setTargetValue(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLinearBarGraph_a(), targetValue) : 0, sizeof(iVtObjectLinearBarGraph_s), 11, newValue, b_enableReplaceOfCmd);
  }

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 18; }
  */

  uint16_t updateWidth(bool b_SendRequest=false);
  uint16_t updateHeight(bool b_SendRequest=false);
  uint8_t updateColour(bool b_SendRequest=false);
  uint8_t updateTargetLineColour(bool b_SendRequest=false);
  uint8_t updateOptions(bool b_SendRequest=false);
  uint8_t updateNumberOfTicks(bool b_SendRequest=false);
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
