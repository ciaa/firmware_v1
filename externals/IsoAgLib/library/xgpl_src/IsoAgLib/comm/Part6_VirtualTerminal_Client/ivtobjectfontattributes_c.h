/*
  ivtobjectfontattributes_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTFONTATTRIBUTES_C_H
#define IVTOBJECTFONTATTRIBUTES_C_H

#include "impl/vtobjectfontattributes_c.h"


namespace IsoAgLib {

class iVtObjectFontAttributes_c : public __IsoAgLib::vtObjectFontAttributes_c
{
public:
  iVtObjectFontAttributes_c();

  static uint16_t objectType() { return VT_OBJECT_TYPE_FONT_ATTRIBUTES; }

  virtual ~iVtObjectFontAttributes_c();

  void init(const iVtObjectFontAttributes_s* vtObjectFontAttributesSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectFontAttributes_c::init (vtObjectFontAttributesSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  };

  const iVtObjectFontAttributes_s& get_vtObjectFontAttributes_a() { return *vtObjectFontAttributes_c::get_vtObjectFontAttributes_a(); };

  uint16_t getScaledWidthHeight();

  void setFontColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectFontAttributes_c::setFontColour (newValue, b_updateObject, b_enableReplaceOfCmd);
  };

  void setFontSize(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectFontAttributes_c::setFontSize (newValue, b_updateObject, b_enableReplaceOfCmd);
  };

  void setFontType(uint8_t newValue,  bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectFontAttributes_c::setFontType(newValue, b_updateObject, b_enableReplaceOfCmd);
  };

  void setFontStyle(uint8_t newValue,  bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    vtObjectFontAttributes_c::setFontStyle(newValue, b_updateObject, b_enableReplaceOfCmd);
  };

  void setFontAttributes(uint8_t newFontColour, uint8_t newFontSize, uint8_t newFontType, uint8_t newFontStyle, bool b_updateObject=false, bool b_enableReplaceOfCmd=false){
    vtObjectFontAttributes_c::setFontAttributes(newFontColour, newFontSize, newFontType, newFontStyle, b_updateObject, b_enableReplaceOfCmd);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectFontAttributes_c::updateObjectType(); }
  */

  uint8_t updateFontColour(bool b_SendRequest=false) {
    return vtObjectFontAttributes_c::updateFontColour(b_SendRequest);
  }

  uint8_t updateFontSize(bool b_SendRequest=false) {
    return vtObjectFontAttributes_c::updateFontSize(b_SendRequest);
  }

  uint8_t updateFontType(bool b_SendRequest=false) {
    return vtObjectFontAttributes_c::updateFontType(b_SendRequest);
  }

  uint8_t updateFontStyle(bool b_SendRequest=false) {
    return vtObjectFontAttributes_c::updateFontStyle(b_SendRequest);
  }
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif
