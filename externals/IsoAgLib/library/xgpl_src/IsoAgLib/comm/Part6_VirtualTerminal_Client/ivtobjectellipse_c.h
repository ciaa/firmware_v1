/*
  ivtobjectellipse_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTELLIPSE_C_H
#define IVTOBJECTELLIPSE_C_H

#include "impl/vtobjectellipse_c.h"


#ifdef USE_VTOBJECT_ellipse

namespace IsoAgLib {

class iVtObjectEllipse_c : public __IsoAgLib::vtObjectEllipse_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_ELLIPSE; }

  void init(const iVtObjectEllipse_s* vtObjectellipseSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectEllipse_c::init (vtObjectellipseSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectEllipse_s& get_vtObjectEllipse_a() { return *vtObjectEllipse_c::get_vtObjectEllipse_a(); }

  void setLineAttributes(iVtObjectLineAttributes_c* newLineAttributes, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectEllipse_c::setLineAttributes (newLineAttributes, b_updateObject, b_enableReplaceOfCmd);
  }

  void setWidth(uint16_t newWidth, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectEllipse_c::setWidth (newWidth, b_updateObject, b_enableReplaceOfCmd);
  }

  void setHeight(uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectEllipse_c::setHeight (newHeight, b_updateObject, b_enableReplaceOfCmd);
  }

  void setEllipseType(uint8_t newEllipseType, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectEllipse_c::setEllipseType (newEllipseType, b_updateObject, b_enableReplaceOfCmd);
  }

  void setStartAngle(uint8_t newStartAngle, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectEllipse_c::setStartAngle (newStartAngle, b_updateObject, b_enableReplaceOfCmd);
  }

  void setEndAngle(uint8_t newEndAngle, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectEllipse_c::setEndAngle (newEndAngle, b_updateObject, b_enableReplaceOfCmd);
  }

  void setFillAttributes(iVtObjectFillAttributes_c* newFillAttributes, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectEllipse_c::setFillAttributes (newFillAttributes, b_updateObject, b_enableReplaceOfCmd);
  }

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectEllipse_c::setSize(newWidth, newHeight, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectEllipse_c::updateObjectType(); }
  */
  
  uint16_t updateLineAttributes(bool b_SendRequest=false) {
    return vtObjectEllipse_c::updateLineAttributes(b_SendRequest);
  }

  uint16_t updateWidth(bool b_SendRequest=false) {
    return vtObjectEllipse_c::updateWidth(b_SendRequest);
  }

  uint16_t updateHeight(bool b_SendRequest=false) {
    return vtObjectEllipse_c::updateHeight(b_SendRequest);
  }

  uint8_t updateEllipseType(bool b_SendRequest=false) {
    return vtObjectEllipse_c::updateEllipseType(b_SendRequest);
  }

  uint8_t updateStartAngle(bool b_SendRequest=false) {
    return vtObjectEllipse_c::updateStartAngle(b_SendRequest);
  }

  uint8_t updateEndAngle(bool b_SendRequest=false) {
    return vtObjectEllipse_c::updateEndAngle(b_SendRequest);
  }

  uint16_t updateFillAttributes(bool b_SendRequest=false) {
    return vtObjectEllipse_c::updateFillAttributes(b_SendRequest);
  }
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
