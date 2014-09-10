/*
  ivtobjectauxiliaryinput2_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTAUXILIARYINPUT2_C_H
#define IVTOBJECTAUXILIARYINPUT2_C_H

#include "impl/vtobjectauxiliaryinput2_c.h"


#ifdef USE_VTOBJECT_auxiliaryinput2

namespace IsoAgLib {

class iVtObjectAuxiliaryInput2_c : public __IsoAgLib::vtObjectAuxiliaryInput2_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_AUXILIARY_INPUT_2; }

  typedef  __IsoAgLib::vtObjectAuxiliaryInput2_c::StateForLearnMode_en StateForLearnMode_en;

  void init(const iVtObjectAuxiliaryInput2_s* vtObjectAuxiliaryInput2SROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) { vtObject_c::init ((iVtObject_s*) vtObjectAuxiliaryInput2SROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); }

  const iVtObjectAuxiliaryInput2_s& get_vtObjectAuxiliaryInput2_a() { return *vtObjectAuxiliaryInput2_c::get_vtObjectAuxiliaryInput2_a(); }

  bool moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject=false) {
    return vtObjectAuxiliaryInput2_c::moveChildLocation(apc_childObject, dx, dy, b_updateObject);
  }

  bool setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    return vtObjectAuxiliaryInput2_c::setChildPosition(apc_childObject, dx, dy, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectAuxiliaryInput2_c::updateObjectType(); }
  */
#endif

  /**
    set new value and trigger AUX2 input state message
    @paramter aen_stateForLearnMode:
       - default value: the active state is calculated from aui16_value1 != 0
       - StateForLearnMode_SetActive: set state to activated
       - StateForLearnMode_SetInactive: set state to not activated
    @return true if range of value matches the expected range for this function type
  */
  bool setValue(uint16_t aui16_value1, uint16_t aui16_value2 = 0, StateForLearnMode_en aen_stateForLearnMode = StateForLearnMode_CalculateInternally)
  {
    return vtObjectAuxiliaryInput2_c::setValue(aui16_value1, aui16_value2, aen_stateForLearnMode);
  }

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
