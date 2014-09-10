/*
  ivtobjectcontainer_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTCONTAINER_C_H
#define IVTOBJECTCONTAINER_C_H

#include "impl/vtobjectcontainer_c.h"


#ifdef USE_VTOBJECT_container

namespace IsoAgLib {

class iVtObjectContainer_c : public __IsoAgLib::vtObjectContainer_c
{
public:
  iVtObjectContainer_c();
  ~iVtObjectContainer_c();

  static uint16_t objectType() { return VT_OBJECT_TYPE_CONTAINER; }

  void init(const iVtObjectContainer_s* vtObjectContainer_sROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectContainer_c::init (vtObjectContainer_sROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectContainer_s& get_vtObjectContainer_a() { return *vtObjectContainer_c::get_vtObjectContainer_a(); }

  void hide(bool b_updateObject= false, bool b_enableReplaceOfCmd=false) { vtObjectContainer_c::hideShow (0, b_updateObject, b_enableReplaceOfCmd); }
  void show(bool b_updateObject= false, bool b_enableReplaceOfCmd=false) { vtObjectContainer_c::hideShow (1, b_updateObject, b_enableReplaceOfCmd); }

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectContainer_c::setSize(newWidth, newHeight, b_updateObject, b_enableReplaceOfCmd);
  }

  bool moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject=false) {
    return vtObjectContainer_c::moveChildLocation(apc_childObject, dx, dy, b_updateObject);
  }

  bool setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    return vtObjectContainer_c::setChildPosition(apc_childObject, dx, dy, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** these attributes are in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const {
    return vtObjectContainer_c::updateObjectType();
  }


  uint16_t updateWidth(bool b_SendRequest=false) {
    return vtObjectContainer_c::updateWidth(b_SendRequest);
  }

  uint16_t updateHeight(bool b_SendRequest=false) {
    return vtObjectContainer_c::updateHeight(b_SendRequest);
  }

  uint8_t updateHidden(bool b_SendRequest=false) {
    return vtObjectContainer_c::updateHidden(b_SendRequest);
  }
  */
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
