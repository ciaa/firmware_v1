/*
  ivtobjectpolygon_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTPOLYGON_C_H
#define IVTOBJECTPOLYGON_C_H

#include "impl/vtobjectpolygon_c.h"


#ifdef USE_VTOBJECT_polygon

namespace IsoAgLib {

class iVtObjectPolygon_c : public __IsoAgLib::vtObjectPolygon_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_POLYGON; }

  void init(const iVtObjectPolygon_s* vtObjectPolygonSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectPolygon_c::init (vtObjectPolygonSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectPolygon_s& get_vtObjectPolygon_a() { return *vtObjectPolygon_c::get_vtObjectPolygon_a(); }

  void setWidth(uint16_t newWidth, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectPolygon_c::setWidth (newWidth, b_updateObject, b_enableReplaceOfCmd);
  }

  void setHeight(uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectPolygon_c::setHeight (newHeight, b_updateObject, b_enableReplaceOfCmd);
  }

  void setLineAttributes(IsoAgLib::iVtObjectLineAttributes_c* newLineAttributes, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectPolygon_c::setLineAttributes (newLineAttributes, b_updateObject, b_enableReplaceOfCmd);
  }

  void setFillAttributes(IsoAgLib::iVtObjectFillAttributes_c* newFillAttributes, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectPolygon_c::setFillAttributes (newFillAttributes, b_updateObject, b_enableReplaceOfCmd);
  }

  void setPolygonType(uint8_t newPolygonType, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectPolygon_c::setPolygonType (newPolygonType, b_updateObject, b_enableReplaceOfCmd);
  }

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectPolygon_c::setSize(newWidth, newHeight, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectPolygon_c::updateObjectType(); }
  */

  uint16_t updateWidth(bool b_SendRequest=false) {
    return vtObjectPolygon_c::updateWidth(b_SendRequest);
  }

  uint16_t updateHeight(bool b_SendRequest=false) {
    return vtObjectPolygon_c::updateHeight(b_SendRequest);
  }

  uint16_t updateLineAttributes(bool b_SendRequest=false) {
    return vtObjectPolygon_c::updateLineAttributes(b_SendRequest);
  }

  uint16_t updateFillAttributes(bool b_SendRequest=false) {
    return vtObjectPolygon_c::updateFillAttributes(b_SendRequest);
  }

  uint8_t updatePolygonType(bool b_SendRequest=false) {
    return vtObjectPolygon_c::updatePolygonType(b_SendRequest);
  }
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLibb

#endif

#endif
