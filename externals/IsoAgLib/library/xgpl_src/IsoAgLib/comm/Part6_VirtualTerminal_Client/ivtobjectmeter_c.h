/*
  ivtobjectmeter_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTMETER_C_H
#define IVTOBJECTMETER_C_H

#include "impl/vtobjectmeter_c.h"


#ifdef USE_VTOBJECT_meter

namespace IsoAgLib {

class iVtObjectMeter_c : public __IsoAgLib::vtObjectMeter_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_METER; }

  void init(const iVtObjectMeter_s* vtObjectMeterSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectMeter_c::init (vtObjectMeterSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectMeter_s& get_vtObjectMeter_a() { return *vtObjectMeter_c::get_vtObjectMeter_a(); }

  void setWidth(uint16_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectMeter_c::setWidth (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setNeedleColour(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectMeter_c::setNeedleColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setBorderColour(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectMeter_c::setBorderColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setArcAndTickColour(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectMeter_c::setArcAndTickColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setOptions(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectMeter_c::setOptions (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setNumberOfTicks(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectMeter_c::setNumberOfTicks (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setStartAngle(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectMeter_c::setStartAngle (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setEndAngle(uint8_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectMeter_c::setEndAngle (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setMin(uint16_t newMin, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectMeter_c::setMin (newMin, b_updateObject, b_enableReplaceOfCmd);
  }

  void setMax(uint16_t newMax, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectMeter_c::setMax (newMax, b_updateObject, b_enableReplaceOfCmd);
  }

  void setValue(uint16_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=true) {
    vtObjectMeter_c::setValue (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setVariableReference(iVtObject_c* newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectMeter_c::setVariableReference (newValue, b_updateObject, b_enableReplaceOfCmd);
  }
  
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectMeter_c::updateObjectType(); }
  */
  
  uint16_t updateWidth(bool b_SendRequest=false) {
    return vtObjectMeter_c::updateWidth(b_SendRequest);
  }

  uint8_t updateNeedleColour(bool b_SendRequest=false) {
    return vtObjectMeter_c::updateNeedleColour(b_SendRequest);
  }

  uint8_t updateBorderColour(bool b_SendRequest=false) {
    return vtObjectMeter_c::updateBorderColour(b_SendRequest);
  }

  uint8_t updateArcAndTickColour(bool b_SendRequest=false) {
    return vtObjectMeter_c::updateArcAndTickColour(b_SendRequest);
  }

  uint8_t updateOptions(bool b_SendRequest=false) {
    return vtObjectMeter_c::updateOptions(b_SendRequest);
  }

  uint8_t updateNumberOfTicks(bool b_SendRequest=false) {
    return vtObjectMeter_c::updateNumberOfTicks(b_SendRequest);
  }

  uint8_t updateStartAngle(bool b_SendRequest=false) {
    return vtObjectMeter_c::updateStartAngle(b_SendRequest);
  }

  uint8_t updateEndAngle(bool b_SendRequest=false) {
    return vtObjectMeter_c::updateEndAngle(b_SendRequest);
  }

  uint16_t updateMinValue(bool b_SendRequest=false) {
    return vtObjectMeter_c::updateMinValue(b_SendRequest);
  }

  uint16_t updateMaxValue(bool b_SendRequest=false) {
    return vtObjectMeter_c::updateMaxValue(b_SendRequest);
  }

  uint16_t updateVariableReference(bool b_SendRequest=false) {
    return vtObjectMeter_c::updateVariableReference(b_SendRequest);
  }

  /** that attribute is in parentheses in the spec, so commented out here
  uint16_t updateValue(bool b_SendRequest=false) {
    return vtObjectMeter_c::getValue(b_SendRequest);
  }
  */
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
