/*
  ivtobjectlinearbargraph_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTLINEARBARGRAPH_C_H
#define IVTOBJECTLINEARBARGRAPH_C_H

#include "impl/vtobjectlinearbargraph_c.h"


#ifdef USE_VTOBJECT_linearbargraph

namespace IsoAgLib {

class iVtObjectLinearBarGraph_c : public __IsoAgLib::vtObjectLinearBarGraph_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_LINEAR_BAR_GRAPH; }

  void init(const iVtObjectLinearBarGraph_s* vtObjectLinearBarGraphSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectLinearBarGraph_c::init (vtObjectLinearBarGraphSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectLinearBarGraph_s& get_vtObjectLinearBarGraph_a() { return *vtObjectLinearBarGraph_c::get_vtObjectLinearBarGraph_a(); }

  void setValue(uint16_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=true) {
    vtObjectLinearBarGraph_c::setValue (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setWidth(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setWidth (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setHeight(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setHeight (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setTargetLineColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setTargetLineColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setOptions(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setOptions (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setNumberOfTicks(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setNumberOfTicks (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setMinValue(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setMinValue (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setMaxValue(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setMaxValue (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setVariableReference(iVtObject_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setVariableReference (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setTargetValueVariableReference(iVtObject_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setTargetValueVariableReference (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setTargetValue(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setTargetValue (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLinearBarGraph_c::setSize(newWidth, newHeight, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectLinearBarGraph_c::updateObjectType(); }
  */

  uint16_t updateWidth(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::updateWidth(b_SendRequest);
  }

  uint16_t updateHeight(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::updateHeight(b_SendRequest);
  }

  uint8_t updateColour(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::updateColour(b_SendRequest);
  }

  uint8_t updateTargetLineColour(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::updateTargetLineColour(b_SendRequest);
  }

  uint8_t updateOptions(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::updateOptions(b_SendRequest);
  }

  uint8_t updateNumberOfTicks(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::updateNumberOfTicks(b_SendRequest);
  }

  uint16_t updateMinValue(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::updateMinValue(b_SendRequest);
  }

  uint16_t updateMaxValue(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::updateMaxValue(b_SendRequest);
  }

  uint16_t updateVariableReference(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::updateVariableReference(b_SendRequest);
  }

  uint16_t updateTargetValueVariableReference(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::updateTargetValueVariableReference(b_SendRequest);
  }

  uint16_t updateTargetValue(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::updateTargetValue(b_SendRequest);
  }

  /** that attribute is in parentheses in the spec, so commented out here
  uint16_t updateValue(bool b_SendRequest=false) {
    return vtObjectLinearBarGraph_c::getValue(b_SendRequest);
  }
  */
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
