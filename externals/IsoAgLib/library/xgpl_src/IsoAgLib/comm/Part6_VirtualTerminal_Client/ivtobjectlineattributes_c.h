/*
  ivtobjectlineattributes_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTLINEATTRIBUTES_C_H
#define IVTOBJECTLINEATTRIBUTES_C_H

#include "impl/vtobjectlineattributes_c.h"


namespace IsoAgLib {

class iVtObjectLineAttributes_c : public __IsoAgLib::vtObjectLineAttributes_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_LINE_ATTRIBUTES; }

  void init(const iVtObjectLineAttributes_s* vtObjectLineAttributesSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectLineAttributes_c::init (vtObjectLineAttributesSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectLineAttributes_s& get_vtObjectLineAttributes_a() { return *vtObjectLineAttributes_c::get_vtObjectLineAttributes_a (); }

  void setLineColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLineAttributes_c::setLineColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setLineWidth(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLineAttributes_c::setLineWidth (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setLineArt(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLineAttributes_c::setLineArt (newValue, b_updateObject, b_enableReplaceOfCmd);
  }

  void setLineAttributes(uint8_t newLineColour, uint8_t newLineWidth, uint16_t newLineArt, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectLineAttributes_c::setLineAttributes (newLineColour, newLineWidth, newLineArt, b_updateObject, b_enableReplaceOfCmd);
  }

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectLineAttributes_c::updateObjectType(); }
  */

  uint8_t updateLineColour(bool b_SendRequest=false) {
    return vtObjectLineAttributes_c::updateLineColour(b_SendRequest);
  }

  uint8_t updateLineWidth(bool b_SendRequest=false) {
    return vtObjectLineAttributes_c::updateLineWidth(b_SendRequest);
  }

  uint16_t updateLineArt(bool b_SendRequest=false) {
    return vtObjectLineAttributes_c::updateLineArt(b_SendRequest);
  }
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif
