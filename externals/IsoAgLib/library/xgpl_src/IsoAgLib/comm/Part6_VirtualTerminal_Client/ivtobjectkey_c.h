/*
  ivtobjectkey_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTKEY_C_H
#define IVTOBJECTKEY_C_H

#include "impl/vtobjectkey_c.h"


#ifdef USE_VTOBJECT_key

namespace IsoAgLib {

class iVtObjectKey_c : public __IsoAgLib::vtObjectKey_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_KEY; }

  void init(const iVtObjectKey_s* vtObjectKeySROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectKey_c::init (vtObjectKeySROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectKey_s& get_vtObjectKey_a() { return *vtObjectKey_c::get_vtObjectKey_a(); }

  void setBackgroundColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectKey_c::setBackgroundColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  //! @param newValue: Range 1-255, KeyCode 0 is reserved for KeyCode:"ESC"
  void setKeyCode(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectKey_c::setKeyCode (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  bool moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject=false) {
    return vtObjectKey_c::moveChildLocation(apc_childObject, dx, dy, b_updateObject);
  }

  bool setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    return vtObjectKey_c::setChildPosition(apc_childObject, dx, dy, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const {
  return vtObjectKey_c::updateObjectType();
  }
   */

  uint8_t updateBackgroundColour(bool b_SendRequest=false) {
    return vtObjectKey_c::updateBackgroundColour(b_SendRequest);
  }

  uint8_t updateKeyCode(bool b_SendRequest=false) {
    return vtObjectKey_c::updateKeyCode(b_SendRequest);
  }
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
