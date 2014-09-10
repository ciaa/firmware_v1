/*
  ivtobjectinputboolean_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTINPUTBOOLEAN_C_H
#define IVTOBJECTINPUTBOOLEAN_C_H

#include "impl/vtobjectinputboolean_c.h"


#ifdef USE_VTOBJECT_inputboolean

namespace IsoAgLib {

class iVtObjectInputBoolean_c : public __IsoAgLib::vtObjectInputBoolean_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_INPUT_BOOLEAN; }

  void init(const iVtObjectInputBoolean_s* vtObjectInputBooleanSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectInputBoolean_c::init (vtObjectInputBooleanSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectInputBoolean_s& get_vtObjectInputBoolean_a() { return *vtObjectInputBoolean_c::get_vtObjectInputBoolean_a(); }

  void setValue(bool newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=true) {
    vtObjectInputBoolean_c::setValue (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setBackgroundColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputBoolean_c::setBackgroundColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setWidth(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputBoolean_c::setWidth (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setForegroundColour(iVtObject_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputBoolean_c::setForegroundColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setVariableReference(iVtObject_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputBoolean_c::setVariableReference (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  bool enable(bool b_updateObject= false, bool b_enableReplaceOfCmd=false) { return vtObject_c::able (1, b_updateObject, b_enableReplaceOfCmd); }
  bool disable(bool b_updateObject= false, bool b_enableReplaceOfCmd=false) { return vtObject_c::able (0, b_updateObject, b_enableReplaceOfCmd); }

  bool select(uint8_t selectOrActivate) { return vtObject_c::select(selectOrActivate); }

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES  
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectInputBoolean_c::updateObjectType(); }
  */
  
  uint8_t updateBackgroundColour(bool b_SendRequest=false) {
    return vtObjectInputBoolean_c::updateBackgroundColour(b_SendRequest);
  }
  
  uint16_t updateWidth(bool b_SendRequest=false) {
    return vtObjectInputBoolean_c::updateWidth(b_SendRequest);
  }

  uint16_t updateForegroundColour(bool b_SendRequest=false) {
    return vtObjectInputBoolean_c::updateForegroundColour(b_SendRequest);
  }

  uint16_t updateVariableReference(bool b_SendRequest=false) {
    return vtObjectInputBoolean_c::updateVariableReference(b_SendRequest);
  }

  /** these attributes are in parentheses in the spec, so commented out here
  uint8_t updateValue(bool b_SendRequest=false) {
    return vtObjectInputBoolean_c::getValue(b_SendRequest);
  }

  uint8_t updateEnabled(bool b_SendRequest=false) {
    return vtObjectInputBoolean_c::updateEnabled(b_SendRequest);
  }
  */
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
