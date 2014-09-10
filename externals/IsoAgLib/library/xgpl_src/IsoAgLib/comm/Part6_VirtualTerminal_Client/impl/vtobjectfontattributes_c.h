/*
  vtobjectfontattributes_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTFONTATTRIBUTES_C_H
#define VTOBJECTFONTATTRIBUTES_C_H

#include "vtobject_c.h"
#include "vtclient_c.h"
#include "vtclientconnection_c.h"


namespace __IsoAgLib {

class vtObjectFontAttributes_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectFontAttributes_s* vtObjectFontAttributesSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  { vtObject_c::init ((iVtObject_s*) vtObjectFontAttributesSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);}

  iVtObjectFontAttributes_s* get_vtObjectFontAttributes_a() { return (iVtObjectFontAttributes_s *)&(get_vtObject_a()); }

  vtObjectFontAttributes_c();

  virtual ~vtObjectFontAttributes_c();

  uint32_t fitTerminal() const;

  uint16_t getScaledWidthHeight();

  // //////////////////////////////////
  // All special Attribute-Set methods
  void setFontColour(uint8_t newValue,  bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontColour) :
        0, sizeof(iVtObjectFontAttributes_s), 1, newValue, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newValue, this, IsoAgLib::FontColour), b_enableReplaceOfCmd);
  }

  void setFontSize(uint8_t newValue,  bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontSize) : 0, sizeof(iVtObjectFontAttributes_s), 2, newValue, newValue, b_enableReplaceOfCmd);
  }

  void setFontType(uint8_t newValue,  bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontType) : 0, sizeof(iVtObjectFontAttributes_s), 3, newValue, newValue, b_enableReplaceOfCmd);
  }

  void setFontStyle(uint8_t newValue,  bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontStyle) : 0, sizeof(iVtObjectFontAttributes_s), 4, newValue, newValue, b_enableReplaceOfCmd);
  }

  void setFontAttributes(uint8_t newFontColour, uint8_t newFontSize, uint8_t newFontType, uint8_t newFontStyle, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 23; }
  */

  uint8_t updateFontColour(bool b_SendRequest=false);

  uint8_t updateFontSize(bool b_SendRequest=false);

  uint8_t updateFontType(bool b_SendRequest=false);

  uint8_t updateFontStyle(bool b_SendRequest=false);

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif

private:

  static uint8_t marr_font2PixelDimensionTableW [15];
  static uint8_t marr_font2PixelDimensionTableH [15];

  //! Calculate font size and cache in mui8_fontSizeScaled
  void calcScaledFontDimension() const;

  //! Chached font size calculated by calcScaledFontDimension()
  mutable uint8_t mui8_fontSizeScaled;

};

} // __IsoAgLib

#endif

