/*
  ivtobjectinputnumber_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTINPUTNUMBER_C_H
#define IVTOBJECTINPUTNUMBER_C_H

#include "impl/vtobjectinputnumber_c.h"


#ifdef USE_VTOBJECT_inputnumber

namespace IsoAgLib {

class iVtObjectInputNumber_c : public __IsoAgLib::vtObjectInputNumber_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_INPUT_NUMBER; }

  void init(const iVtObjectInputNumber_s* vtObjectInputNumberSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectInputNumber_c::init (vtObjectInputNumberSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectInputNumber_s& get_vtObjectInputNumber_a() { return *vtObjectInputNumber_c::get_vtObjectInputNumber_a(); }

  void setValue(uint32_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=true) {
    vtObjectInputNumber_c::setValue (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setWidth(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setWidth (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setHeight(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setHeight (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setBackgroundColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setBackgroundColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setFontAttributes(iVtObjectFontAttributes_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setFontAttributes (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setOptions(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setOptions (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setVariableReference(iVtObjectNumberVariable_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setVariableReference (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setMinValue(uint32_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setMinValue (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setMaxValue(uint32_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setMaxValue (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setOffset(int32_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setOffset (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setScale(float newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setScale (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setNumberOfDecimals(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setNumberOfDecimals (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setFormat(bool newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setFormat (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setHorizontalJustification(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputNumber_c::setHorizontalJustification (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false){
    vtObjectInputNumber_c::setSize(newWidth, newHeight, b_updateObject, b_enableReplaceOfCmd);
  }

  bool enable(bool b_updateObject= false, bool b_enableReplaceOfCmd=false) { return vtObject_c::able (1, b_updateObject, b_enableReplaceOfCmd); }
  bool disable(bool b_updateObject= false, bool b_enableReplaceOfCmd=false) { return vtObject_c::able (0, b_updateObject, b_enableReplaceOfCmd); }

  bool select(uint8_t selectOrActivate) { return vtObject_c::select(selectOrActivate); }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectInputNumber_c::updateObjectType(); }
  */
  
  uint16_t updateWidth(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateWidth(b_SendRequest);
  }
  
  uint16_t updateHeight(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateHeight(b_SendRequest);
  }

  uint8_t updateBackgroundColour(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateBackgroundColour(b_SendRequest);
  }

  uint16_t updateFontAttributes(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateFontAttributes(b_SendRequest);
  }

  uint8_t updateOptions(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateOptions(b_SendRequest);
  }

  uint16_t updateVariableReference(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateVariableReference(b_SendRequest);
  }

  uint32_t updateMinValue(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateMinValue(b_SendRequest);
  }

  uint32_t updateMaxValue(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateMaxValue(b_SendRequest);
  }

  int32_t updateOffset(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateOffset(b_SendRequest);
  }

  float updateScale(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateScale(b_SendRequest);
  }

  uint8_t updateNumberOfDecimals(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateNumberOfDecimals(b_SendRequest);
  }

  uint8_t updateFormat(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateFormat(b_SendRequest);
  }

  uint8_t updateJustification(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::updateJustification(b_SendRequest);
  }

  /** these attributes are in parentheses in the spec, so commented out here
  uint32_t updateValue(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::getValue(b_SendRequest);
  }

  uint8_t update2ndOptionsByte(bool b_SendRequest=false) {
    return vtObjectInputNumber_c::update2ndOptionsByte(b_SendRequest);
  }
  */
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
