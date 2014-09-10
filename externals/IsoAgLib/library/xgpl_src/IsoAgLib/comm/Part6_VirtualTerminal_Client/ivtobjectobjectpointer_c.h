/*
  ivtobjectobjectpointer_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTOBJECTPOINTER_C_H
#define IVTOBJECTOBJECTPOINTER_C_H

#include "impl/vtobjectobjectpointer_c.h"


#ifdef USE_VTOBJECT_objectpointer

namespace IsoAgLib {

class iVtObjectObjectPointer_c : public __IsoAgLib::vtObjectObjectPointer_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_OBJECT_POINTER; }

  void init(const iVtObjectObjectPointer_s* vtObjectObjectPointerSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectObjectPointer_c::init (vtObjectObjectPointerSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectObjectPointer_s& get_vtObjectObjectPointer_a() { return *vtObjectObjectPointer_c::get_vtObjectObjectPointer_a(); }

  void setValue(const iVtObject_c* apc_newObject, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectObjectPointer_c::setValue (apc_newObject, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** these attributes are in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectInputAttributes_c::updateObjectType(); }
  
  uint16_t updateValue(bool b_SendRequest=false) {
    return vtObjectInputAttributes_c::getValue(b_SendRequest);
  }
  */
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
