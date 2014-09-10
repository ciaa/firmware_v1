/*
  vtobjectmeter_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTMETER_C_H
#define VTOBJECTMETER_C_H

#include <IsoAgLib/isoaglib_config.h>


#ifdef USE_VTOBJECT_meter

#include "vtobject_c.h"
#include "vtclient_c.h"
#include "vtclientconnection_c.h"


namespace __IsoAgLib {

class vtObjectMeter_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectMeter_s* vtObjectMeterSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  { vtObject_c::init ((iVtObject_s*) vtObjectMeterSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); }

  iVtObjectMeter_s* get_vtObjectMeter_a() { return (iVtObjectMeter_s *)&(get_vtObject_a()); }

  vtObjectMeter_c();

  uint32_t fitTerminal() const;

  void setWidth(uint16_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectMeter_a(), width) : 0, sizeof(iVtObjectMeter_s), 1 /* "Width" */, newValue, b_enableReplaceOfCmd);
  }

  void setNeedleColour(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectMeter_a(), needleColour) : 0, sizeof(iVtObjectMeter_s), 2 /* "Needle Colour" */, newValue, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newValue, this, IsoAgLib::NeedleColour), b_enableReplaceOfCmd);
  }

  void setBorderColour(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectMeter_a(), borderColour) : 0, sizeof(iVtObjectMeter_s), 3 /* "BorderColour" */, newValue, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newValue, this, IsoAgLib::BorderColour), b_enableReplaceOfCmd);
  }

  void setArcAndTickColour(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectMeter_a(), arcAndTickColour) : 0, sizeof(iVtObjectMeter_s), 4 /* "Arc and Tick Colour" */, newValue, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newValue, this, IsoAgLib::ArcAndTickColour), b_enableReplaceOfCmd);
  }

  void setOptions(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectMeter_a(), options) : 0, sizeof(iVtObjectMeter_s), 5 /* "Options" */, newValue, newValue, b_enableReplaceOfCmd);
  }

  void setNumberOfTicks(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectMeter_a(), numberOfTicks) : 0, sizeof(iVtObjectMeter_s), 6 /* "# of Ticks" */, newValue, newValue, b_enableReplaceOfCmd);
  }

  void setStartAngle(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectMeter_a(), startAngle) : 0, sizeof(iVtObjectMeter_s), 7 /* "Start Angle" */, newValue, newValue, b_enableReplaceOfCmd);
  }

  void setEndAngle(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectMeter_a(), endAngle) : 0, sizeof(iVtObjectMeter_s), 8 /* "End Angle" */, newValue, newValue, b_enableReplaceOfCmd);
  }

  void setMin(uint16_t newMin, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectMeter_a(), minValue) : 0, sizeof(iVtObjectMeter_s), 9 /* "Min value" */, newMin, b_enableReplaceOfCmd);
  }

  void setMax(uint16_t newMax, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectMeter_a(), maxValue) : 0, sizeof(iVtObjectMeter_s), 10 /* "Max value" */, newMax, b_enableReplaceOfCmd);
  }

  void setVariableReference(IsoAgLib::iVtObject_c* newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    saveValuePSetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectMeter_a(), variableReference) : 0, sizeof(iVtObjectMeter_s), 11 /* "Variable Reference" */, newValue, b_enableReplaceOfCmd);
  }

  void setValue(uint16_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=true);

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 17; }
  */

  uint16_t updateWidth(bool b_SendRequest=false);
  uint8_t updateNeedleColour(bool b_SendRequest=false);
  uint8_t updateBorderColour(bool b_SendRequest=false);
  uint8_t updateArcAndTickColour(bool b_SendRequest=false);
  uint8_t updateOptions(bool b_SendRequest=false);
  uint8_t updateNumberOfTicks(bool b_SendRequest=false);
  uint8_t updateStartAngle(bool b_SendRequest=false);
  uint8_t updateEndAngle(bool b_SendRequest=false);
  uint16_t updateMinValue(bool b_SendRequest=false);
  uint16_t updateMaxValue(bool b_SendRequest=false);
  uint16_t updateVariableReference(bool b_SendRequest=false);

  /** that attribute is in parentheses in the spec, so commented out here
  uint16_t updateValue(bool b_SendRequest=false);
  */

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif
};

} // __IsoAgLib

#endif

#endif
