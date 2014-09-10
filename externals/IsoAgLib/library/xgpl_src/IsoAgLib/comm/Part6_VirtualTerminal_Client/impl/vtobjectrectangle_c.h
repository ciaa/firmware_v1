/*
  vtobjectrectangle_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTRECTANGLE_C_H
#define VTOBJECTRECTANGLE_C_H

#include <IsoAgLib/isoaglib_config.h>


#ifdef USE_VTOBJECT_rectangle

#include "vtobject_c.h"


namespace __IsoAgLib {

class vtObjectRectangle_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectRectangle_s* vtObjectRectangleSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  { vtObject_c::init ((iVtObject_s*) vtObjectRectangleSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); }

  iVtObjectRectangle_s* get_vtObjectRectangle_a() { return (iVtObjectRectangle_s *)&(get_vtObject_a()); }

  vtObjectRectangle_c();

  uint32_t fitTerminal() const;

  void setLineAttributes(IsoAgLib::iVtObjectLineAttributes_c* newLineAttributes, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValuePSetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectRectangle_a(), lineAttributes) : 0, sizeof(iVtObjectRectangle_s), 1 /* "Line Attribute" */, (IsoAgLib::iVtObject_c*) newLineAttributes, b_enableReplaceOfCmd);
  }

  void setWidth(uint16_t newWidth, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectRectangle_a(), width) : 0, sizeof(iVtObjectRectangle_s), 2 /* "Width" */, newWidth, b_enableReplaceOfCmd);
  }

  void setHeight(uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectRectangle_a(), height) : 0, sizeof(iVtObjectRectangle_s), 3 /* "Height" */, newHeight, b_enableReplaceOfCmd);
  }

  void setLineSuppression(uint8_t newLineSupressionValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectRectangle_a(), lineSuppression) : 0, sizeof(iVtObjectRectangle_s), 4 /* "Line Suppression" */, newLineSupressionValue, newLineSupressionValue, b_enableReplaceOfCmd);
  }

  void setFillAttributes(IsoAgLib::iVtObjectFillAttributes_c* newFillAttributes, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValuePSetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectRectangle_a(), fillAttributes) : 0, sizeof(iVtObjectRectangle_s), 5 /* "Fill Attributes" */, (IsoAgLib::iVtObject_c*) newFillAttributes, b_enableReplaceOfCmd);
  }

  void setOriginSKM(bool b_SKM);

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 14; }
  */

  uint16_t updateLineAttributes(bool b_SendRequest=false);
  uint16_t updateWidth(bool b_SendRequest=false);
  uint16_t updateHeight(bool b_SendRequest=false);
  uint8_t updateLineSuppression(bool b_SendRequest=false);
  uint16_t updateFillAttributes(bool b_SendRequest=false);

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif
};

} // __IsoAgLib

#endif

#endif
