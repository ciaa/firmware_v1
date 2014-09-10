/*
  ivtobjectoutputnumber_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTOUTPUTNUMBER_C_H
#define IVTOBJECTOUTPUTNUMBER_C_H

#include "impl/vtobjectoutputnumber_c.h"


#ifdef USE_VTOBJECT_outputnumber

#include "ivtobjectfontattributes_c.h"

namespace IsoAgLib {

class iVtObjectOutputNumber_c : public __IsoAgLib::vtObjectOutputNumber_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_OUTPUT_NUMBER; }

  void init(const iVtObjectOutputNumber_s* vtObjectOutputNumberSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectOutputNumber_c::init (vtObjectOutputNumberSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectOutputNumber_s& get_vtObjectOutputNumber_a() { return *vtObjectOutputNumber_c::get_vtObjectOutputNumber_a(); }

  void setValue(uint32_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=true) {
    vtObjectOutputNumber_c::setValue (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setWidth(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setWidth (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setHeight(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setHeight (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setBackgroundColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setBackgroundColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setFontAttributes(iVtObjectFontAttributes_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setFontAttributes (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setOptions(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setOptions (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setVariableReference(iVtObjectNumberVariable_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setVariableReference (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setOffset(int32_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setOffset (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setScale(float newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setScale (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setNumberOfDecimals(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setNumberOfDecimals (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setFormat(bool newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setFormat (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setHorizontalJustification(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setHorizontalJustification (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectOutputNumber_c::setSize(newWidth, newHeight, b_updateObject, b_enableReplaceOfCmd);
  }


#ifdef USE_ISO_TERMINAL_GETATTRIBUTES  
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectOutputNumber_c::updateObjectType(); }
  */
  
  uint16_t updateWidth(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::updateWidth(b_SendRequest);
  }

  uint16_t updateHeight(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::updateHeight(b_SendRequest);
  }

  uint8_t updateBackgroundColour(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::updateBackgroundColour(b_SendRequest);
  }

  uint16_t updateFontAttributes(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::updateFontAttributes(b_SendRequest);
  }

  uint8_t updateOptions(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::updateOptions(b_SendRequest);
  }

  uint16_t updateVariableReference(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::updateVariableReference(b_SendRequest);
  }

  int32_t updateOffset(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::updateOffset(b_SendRequest);
  }

  float updateScale(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::updateScale(b_SendRequest);
  }

  uint8_t updateNumberOfDecimals(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::updateNumberOfDecimals(b_SendRequest);
  }

  uint8_t updateFormat(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::updateFormat(b_SendRequest);
  }

  uint8_t updateJustification(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::updateJustification(b_SendRequest);
  }

  /** that attribute is in parentheses in the spec, so commented out here
  uint32_t updateValue(bool b_SendRequest=false) {
    return vtObjectOutputNumber_c::getValue(b_SendRequest);
  }
  */
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
