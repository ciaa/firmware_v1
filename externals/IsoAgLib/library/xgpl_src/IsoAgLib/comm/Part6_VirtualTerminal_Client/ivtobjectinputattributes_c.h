/*
  ivtobjectinputattributes_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTINPUTATTRIBUTES_C_H
#define IVTOBJECTINPUTATTRIBUTES_C_H

#include "impl/vtobjectinputattributes_c.h"


namespace IsoAgLib {

class iVtObjectInputAttributes_c : public __IsoAgLib::vtObjectInputAttributes_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_INPUT_ATTRIBUTES; }

  void init(const iVtObjectInputAttributes_s* vtObjectInputAttributesSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectInputAttributes_c::init (vtObjectInputAttributesSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectInputAttributes_s& get_vtObjectInputAttributes_a() { return *vtObjectInputAttributes_c::get_vtObjectInputAttributes_a(); }

  const char* getString(){
      return vtObjectInputAttributes_c::getString();}

  void setValidationStringCopy(const char* newValidationString, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputAttributes_c::setValidationStringCopy (newValidationString, b_updateObject, b_enableReplaceOfCmd);
  }

  void setValidationStringRef(const char* newValidationString, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectInputAttributes_c::setValidationStringRef (newValidationString, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** these attributes are in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectInputAttributes_c::updateObjectType(); }
  
  uint8_t updateValidationType(bool b_SendRequest=false) {
    return vtObjectInputAttributes_c::updateValidationType(b_SendRequest);
  }
  */
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif
