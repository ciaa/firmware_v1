/*
  vtobjectlineattributes_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTLINEATTRIBUTES_C_H
#define VTOBJECTLINEATTRIBUTES_C_H

#include "vtobject_c.h"
#include "vtclient_c.h"
#include "vtclientconnection_c.h"


namespace __IsoAgLib {

class vtObjectLineAttributes_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectLineAttributes_s* vtObjectLineAttributesSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  { vtObject_c::init ((iVtObject_s*) vtObjectLineAttributesSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); }

  iVtObjectLineAttributes_s* get_vtObjectLineAttributes_a() { return (iVtObjectLineAttributes_s *)&(get_vtObject_a()); }

  vtObjectLineAttributes_c();

  uint32_t fitTerminal() const;

  // //////////////////////////////////
  // All special Attribute-Set methods
  void setLineColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineColour) : 0, sizeof(iVtObjectLineAttributes_s), 1, newValue, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newValue, this, IsoAgLib::LineColour), b_enableReplaceOfCmd);
  }

  void setLineWidth(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineWidth) : 0, sizeof(iVtObjectLineAttributes_s), 2, newValue, newValue, b_enableReplaceOfCmd);
  }

  void setLineArt(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineArt) : 0, sizeof(iVtObjectLineAttributes_s), 3, newValue, b_enableReplaceOfCmd);
  }

  void setLineAttributes(uint8_t newLineColour, uint8_t newLineWidth, uint16_t newLineArt, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 24; }
  */

  uint8_t updateLineColour(bool b_SendRequest=false);
  uint8_t updateLineWidth(bool b_SendRequest=false);
  uint16_t updateLineArt(bool b_SendRequest=false);

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif
};

} // __IsoAgLib

#endif
