/*
  ivtobjectdatamask_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTDATAMASK_C_H
#define IVTOBJECTDATAMASK_C_H

#include "impl/vtobjectdatamask_c.h"


namespace IsoAgLib {

class iVtObjectDataMask_c : public __IsoAgLib::vtObjectDataMask_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_DATA_MASK; }

  void init(const iVtObjectDataMask_s* vtObjectDataMaskSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectDataMask_c::init (vtObjectDataMaskSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectDataMask_s& get_vtObjectDataMask_a() { return *vtObjectDataMask_c::get_vtObjectDataMask_a(); }

  void setSoftKeyMask(iVtObjectSoftKeyMask_c* newSoftKeyMask, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectDataMask_c::setSoftKeyMask (newSoftKeyMask, b_updateObject, b_enableReplaceOfCmd);
  }

  void setBackgroundColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectDataMask_c::setBackgroundColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  bool moveChildLocation(iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject=false) {
    return vtObjectDataMask_c::moveChildLocation (apc_childObject, dx, dy, b_updateObject);
  }

  bool setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    return vtObjectDataMask_c::setChildPosition(apc_childObject, dx, dy, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const {
    return vtObjectDataMask_c::updateObjectType();
  }
  */

  uint8_t updateBackgroundColour(bool b_SendRequest=false) {
    return vtObjectDataMask_c::updateBackgroundColour(b_SendRequest);
  }

  uint16_t updateSoftKeyMask(bool b_SendRequest=false) {
    return vtObjectDataMask_c::updateSoftKeyMask(b_SendRequest);
  }
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif
