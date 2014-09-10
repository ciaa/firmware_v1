/*
  ivtobjectline_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTLINE_C_H
#define IVTOBJECTLINE_C_H

#include "impl/vtobjectline_c.h"


#ifdef USE_VTOBJECT_line

namespace IsoAgLib {

class iVtObjectLine_c : public __IsoAgLib::vtObjectLine_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_LINE; }

  void init(const iVtObjectLine_s* vtObjectLineSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectLine_c::init (vtObjectLineSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectLine_s& get_vtObjectLine_a() { return *vtObjectLine_c::get_vtObjectLine_a(); }

  void setLineAttributes(iVtObjectLineAttributes_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLine_c::setLineAttributes(newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setWidth (int32_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLine_c::setWidth(newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setHeight (int32_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLine_c::setHeight(newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setLineDirection (uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLine_c::setLineDirection(newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setEndPoint (uint16_t newWidth, uint16_t newHeight, uint8_t newLineDirection, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLine_c::setEndPoint(newWidth, newHeight, newLineDirection, b_updateObject, b_enableReplaceOfCmd);
  }

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLine_c::setSize(newWidth, newHeight, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectLine_c::updateObjectType(); }
  */

  uint16_t updateLineAttributes(bool b_SendRequest=false) {
    return vtObjectLine_c::updateLineAttributes(b_SendRequest);
  }

  uint16_t updateWidth(bool b_SendRequest=false) {
    return vtObjectLine_c::updateWidth(b_SendRequest);
  }

  uint16_t updateHeight(bool b_SendRequest=false) {
    return vtObjectLine_c::updateHeight(b_SendRequest);
  }

  uint8_t updateLineDirection(bool b_SendRequest=false) {
    return vtObjectLine_c::updateLineDirection(b_SendRequest);
  }
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif

#endif
