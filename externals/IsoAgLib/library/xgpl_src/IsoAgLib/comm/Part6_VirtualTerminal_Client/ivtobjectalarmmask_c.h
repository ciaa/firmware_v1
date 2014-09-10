/*
  ivtobjectalarmmask_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTALARMMASK_C_H
#define IVTOBJECTALARMMASK_C_H

#include "impl/vtobjectalarmmask_c.h"

#ifdef USE_VTOBJECT_alarmmask

#include "ivtobjectsoftkeymask_c.h"


namespace IsoAgLib {

class iVtObjectAlarmMask_c : public __IsoAgLib::vtObjectAlarmMask_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_ALARM_MASK; }

  void init(const iVtObjectAlarmMask_s* vtObjectAlarmMaskSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectAlarmMask_c::init (vtObjectAlarmMaskSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }
  ~iVtObjectAlarmMask_c(){}

  const iVtObjectAlarmMask_s& get_vtObjectAlarmMask_a() { return *vtObjectAlarmMask_c::get_vtObjectAlarmMask_a(); }

  void setBackgroundColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectAlarmMask_c::setBackgroundColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setSoftKeyMask(iVtObjectSoftKeyMask_c* newSoftKeyMask, bool b_updateObject= false, bool b_enableReplaceOfCmd=false) {
    vtObjectAlarmMask_c::setSoftKeyMask (newSoftKeyMask, b_updateObject, b_enableReplaceOfCmd);
  }

  void setPriority(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectAlarmMask_c::setPriority (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setAcousticSignal(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectAlarmMask_c::setAcousticSignal (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  bool moveChildLocation(iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject=false) {
    return vtObjectAlarmMask_c::moveChildLocation (apc_childObject, dx, dy, b_updateObject);
  }

  bool setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    return vtObjectAlarmMask_c::setChildPosition(apc_childObject, dx, dy, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const {
    return vtObjectAlarmMask_c::updateObjectType();
  }
   */

  uint8_t updateBackgroundColour(bool b_SendRequest=false) {
    return vtObjectAlarmMask_c::updateBackgroundColour(b_SendRequest);
  }

  uint16_t updateSoftKeyMask(bool b_SendRequest=false) {
    return vtObjectAlarmMask_c::updateSoftKeyMask(b_SendRequest);
  }

  uint8_t updatePriority(bool b_SendRequest=false) {
    return vtObjectAlarmMask_c::updatePriority(b_SendRequest);
  }

  uint8_t updateAcousticSignal(bool b_SendRequest=false) {
    return vtObjectAlarmMask_c::updateAcousticSignal(b_SendRequest);
  }
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
