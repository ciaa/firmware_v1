/*
  vtobjectline_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTLINE_C_H
#define VTOBJECTLINE_C_H

#include <IsoAgLib/isoaglib_config.h>


#ifdef USE_VTOBJECT_line

#include "vtobject_c.h"


namespace __IsoAgLib {

class vtObjectLine_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectLine_s* vtObjectLineSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  { vtObject_c::init ((iVtObject_s*) vtObjectLineSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); }

  iVtObjectLine_s* get_vtObjectLine_a() { return (iVtObjectLine_s *)&(get_vtObject_a()); }

  vtObjectLine_c();

  uint32_t fitTerminal() const;

  void setOriginSKM(bool b_SKM);

  // //////////////////////////////////
  // All special Attribute-Set methods
  void setLineAttributes(IsoAgLib::iVtObjectLineAttributes_c* newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValuePSetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLine_a(), lineAttributes) : 0, sizeof(iVtObjectLine_s), 1, (IsoAgLib::iVtObject_c*) newValue, b_enableReplaceOfCmd);
  }

  void setWidth (uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLine_a(), width) : 0, sizeof(iVtObjectLine_s), 2, newValue, b_enableReplaceOfCmd);
  }

  void setHeight (uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLine_a(), height) : 0, sizeof(iVtObjectLine_s), 3, newValue, b_enableReplaceOfCmd);
  }

  void setLineDirection (uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLine_a(), lineDirection) : 0, sizeof(iVtObjectLine_s), 4, newValue, newValue, b_enableReplaceOfCmd);
  }

  void setEndPoint (uint16_t newWidth, uint16_t newHeight, uint8_t newLineDirection, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 13; }
  */
  uint16_t updateLineAttributes(bool b_SendRequest=false);
  uint16_t updateWidth(bool b_SendRequest=false);
  uint16_t updateHeight(bool b_SendRequest=false);
  uint8_t updateLineDirection(bool b_SendRequest=false);

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif
};

} // __IsoAgLib

#endif

#endif
