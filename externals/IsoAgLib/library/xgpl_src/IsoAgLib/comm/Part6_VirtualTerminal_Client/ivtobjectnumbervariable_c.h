/*
  ivtobjectnumbervariable_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTNUMBERVARIABLE_C_H
#define IVTOBJECTNUMBERVARIABLE_C_H

#include "impl/vtobjectnumbervariable_c.h"


#ifdef USE_VTOBJECT_numbervariable

namespace IsoAgLib {

class iVtObjectNumberVariable_c : public __IsoAgLib::vtObjectNumberVariable_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_NUMBER_VARIABLE; }

  void init(const iVtObjectNumberVariable_s* vtObjectNumberVariableSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectNumberVariable_c::init (vtObjectNumberVariableSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectNumberVariable_s& get_vtObjectNumberVariable_a() { return *vtObjectNumberVariable_c::get_vtObjectNumberVariable_a(); }

  void setValue(uint32_t newValue, bool b_updateObject= false, bool b_enableReplaceOfCmd=true) {
    vtObjectNumberVariable_c::setValue (newValue, b_updateObject, b_enableReplaceOfCmd); }


#ifdef USE_ISO_TERMINAL_GETATTRIBUTES  
  // ///////////////////////// getter for attributes
  /** these attributes are in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectNumberVariable_c::updateObjectType(); }
  
  uint32_t updateValue(bool b_SendRequest=false) {
    return vtObjectNumberVariable_c::getValue(b_SendRequest);
  }
  */
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
