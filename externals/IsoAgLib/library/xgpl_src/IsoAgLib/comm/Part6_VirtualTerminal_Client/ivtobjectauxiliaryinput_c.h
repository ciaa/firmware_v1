/*
  ivtobjectauxiliaryinput_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTAUXILIARYINPUT_C_H
#define IVTOBJECTAUXILIARYINPUT_C_H

#include "impl/vtobjectauxiliaryinput_c.h"


#ifdef USE_VTOBJECT_auxiliaryinput

namespace IsoAgLib {

class iVtObjectAuxiliaryInput_c : public __IsoAgLib::vtObjectAuxiliaryInput_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_AUXILIARY_INPUT_1; }

  void init(const iVtObjectAuxiliaryInput_s* vtObjectAuxiliaryInputSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) { vtObject_c::init ((iVtObject_s*) vtObjectAuxiliaryInputSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); }

  const iVtObjectAuxiliaryInput_s& get_vtObjectAuxiliaryInput_a() { return *vtObjectAuxiliaryInput_c::get_vtObjectAuxiliaryInput_a(); }

  bool moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject=false) {
    return vtObjectAuxiliaryInput_c::moveChildLocation(apc_childObject, dx, dy, b_updateObject);
  }

  bool setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    return vtObjectAuxiliaryInput_c::setChildPosition(apc_childObject, dx, dy, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectAuxiliaryInput_c::updateObjectType(); }
  */
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
