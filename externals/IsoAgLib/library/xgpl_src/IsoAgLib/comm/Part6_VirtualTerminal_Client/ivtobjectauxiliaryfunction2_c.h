/*
  ivtobjectauxiliaryfunction_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTAUXILIARYFUNCTION2_C_H
#define IVTOBJECTAUXILIARYFUNCTION2_C_H

#include "impl/vtobjectauxiliaryfunction2_c.h"


#ifdef USE_VTOBJECT_auxiliaryfunction2

namespace IsoAgLib {

class iVtObjectAuxiliaryFunction2_c : public __IsoAgLib::vtObjectAuxiliaryFunction2_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_AUXILIARY_FUNCTION_2; }

  ~iVtObjectAuxiliaryFunction2_c(){};

  void init(const iVtObjectAuxiliaryFunction2_s* vtObjectAuxiliaryFunction2SROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) { vtObject_c::init ((iVtObject_s*) vtObjectAuxiliaryFunction2SROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); };

  const iVtObjectAuxiliaryFunction2_s& get_vtObjectAuxiliaryFunction2_a() { return *vtObjectAuxiliaryFunction2_c::get_vtObjectAuxiliaryFunction2_a(); };

  bool moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject=false) {
    return vtObjectAuxiliaryFunction2_c::moveChildLocation(apc_childObject, dx, dy, b_updateObject);
  }

  bool setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    return vtObjectAuxiliaryFunction2_c::setChildPosition(apc_childObject, dx, dy, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectAuxiliaryFunction2_c::updateObjectType(); }
  */
#endif

  //! @return true, if AUX2 input is assigned
  bool isAssigned() const
  { return __IsoAgLib::vtObjectAuxiliaryFunction2_c::isAssigned(); }

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
