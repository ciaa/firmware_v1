/*
  vtobjectpicturegraphic_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTPICTUREGRAPHIC_C_H
#define VTOBJECTPICTUREGRAPHIC_C_H

#include "vtobject_c.h"
#include "vtclient_c.h"
#include "vtclientconnection_c.h"


namespace __IsoAgLib {

class vtObjectPictureGraphic_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectPictureGraphic_s* vtObjectPictureGraphicSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  { vtObject_c::init ((iVtObject_s*) vtObjectPictureGraphicSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); }

  inline iVtObjectPictureGraphic_s* get_vtObjectPictureGraphic_a() { return (iVtObjectPictureGraphic_s *)&(get_vtObject_a()); }

  vtObjectPictureGraphic_c();

  uint32_t fitTerminal() const;

  // //////////////////////////////////
  // All special Attribute-Set methods
  void setWidth(uint16_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue16SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), width) : 0, sizeof(iVtObjectPictureGraphic_s), 1, newValue, b_enableReplaceOfCmd);
  }
  void setOptions(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), options) : 0, sizeof(iVtObjectPictureGraphic_s), 2, newValue, newValue, b_enableReplaceOfCmd);
  }
  void setTransparencyColour(uint8_t newValue, bool b_updateObject=false, bool b_enableReplaceOfCmd=false) {
    saveValue8SetAttribute ((b_updateObject) ? MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), transparencyColour) : 0, sizeof(iVtObjectPictureGraphic_s), 3, newValue, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newValue, this, IsoAgLib::TransparencyColour), b_enableReplaceOfCmd);
  }

  /// The following modification functions will only take affect on updating the object pool!
  /// USE THEM WITH CARE!!!
  void setRawData0 (HUGE_MEM uint8_t* newValue, uint32_t aui32_size, bool ab_rle, uint16_t aui16_actWidth=0xFFFF, uint16_t aui16_actHeight=0xFFFF, uint16_t aui16_width=0xFFFF)
  { // normally it would be enough to just use saveValueP once, because the ram-struct is then created... but anyway...
    saveValueP (MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), rawData0),                sizeof(iVtObjectPictureGraphic_s), (IsoAgLib::iVtObject_c*)newValue);
    saveValue32(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), numberOfBytesInRawData0), sizeof(iVtObjectPictureGraphic_s), aui32_size);
    saveValue8 (MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), options),                 sizeof(iVtObjectPictureGraphic_s), ab_rle ? (get_vtObjectPictureGraphic_a()->options |  (1<<2))
                                                                                                                                          : (get_vtObjectPictureGraphic_a()->options & ~(1<<2)) );
    if (aui16_actWidth != 0xFFFF) saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualWidth),  sizeof(iVtObjectPictureGraphic_s), aui16_actWidth);
    if (aui16_actHeight!= 0xFFFF) saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualHeight), sizeof(iVtObjectPictureGraphic_s), aui16_actHeight);
    if (aui16_width    != 0xFFFF) saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), width),        sizeof(iVtObjectPictureGraphic_s), aui16_width);
  }

  void setRawData1 (HUGE_MEM uint8_t* newValue, uint32_t aui32_size, bool ab_rle, uint16_t aui16_actWidth=0xFFFF, uint16_t aui16_actHeight=0xFFFF, uint16_t aui16_width=0xFFFF)
  { // normally it would be enough to just use saveValueP once, because the ram-struct is then created... but anyway...
    saveValueP (MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), rawData1),                sizeof(iVtObjectPictureGraphic_s), (IsoAgLib::iVtObject_c*)newValue);
    saveValue32(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), numberOfBytesInRawData1), sizeof(iVtObjectPictureGraphic_s), aui32_size);
    saveValue8 (MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), options),                 sizeof(iVtObjectPictureGraphic_s), ab_rle ? (get_vtObjectPictureGraphic_a()->options |  (1<<3))
                                                                                                                                          : (get_vtObjectPictureGraphic_a()->options & ~(1<<3)) );
    if (aui16_actWidth != 0xFFFF) saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualWidth),  sizeof(iVtObjectPictureGraphic_s), aui16_actWidth);
    if (aui16_actHeight!= 0xFFFF) saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualHeight), sizeof(iVtObjectPictureGraphic_s), aui16_actHeight);
    if (aui16_width    != 0xFFFF) saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), width),        sizeof(iVtObjectPictureGraphic_s), aui16_width);
  }

  void setRawData2 (HUGE_MEM uint8_t* newValue, uint32_t aui32_size, bool ab_rle, uint16_t aui16_actWidth=0xFFFF, uint16_t aui16_actHeight=0xFFFF, uint16_t aui16_width=0xFFFF)
  { // normally it would be enough to just use saveValueP once, because the ram-struct is then created... but anyway...
    saveValueP (MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), rawData2),                sizeof(iVtObjectPictureGraphic_s), (IsoAgLib::iVtObject_c*)newValue);
    saveValue32(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), numberOfBytesInRawData2), sizeof(iVtObjectPictureGraphic_s), aui32_size);
    saveValue8 (MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), options),                 sizeof(iVtObjectPictureGraphic_s), ab_rle ? (get_vtObjectPictureGraphic_a()->options |  (1<<4))
                                                                                                                                          : (get_vtObjectPictureGraphic_a()->options & ~(1<<4)) );
    if (aui16_actWidth != 0xFFFF) saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualWidth),  sizeof(iVtObjectPictureGraphic_s), aui16_actWidth);
    if (aui16_actHeight!= 0xFFFF) saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualHeight), sizeof(iVtObjectPictureGraphic_s), aui16_actHeight);
    if (aui16_width    != 0xFFFF) saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), width),        sizeof(iVtObjectPictureGraphic_s), aui16_width);
  }
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 20; }
  */

  uint16_t updateWidth(bool b_SendRequest=false);
  uint8_t updateOptions(bool b_SendRequest=false);
  uint8_t updateTransparencyColour(bool b_SendRequest=false);
  /** these attributes are in parentheses in the spec, so commented out here
  uint16_t updateActualWidth(bool b_SendRequest=false);
  uint16_t updateActualHeight(bool b_SendRequest=false);
  uint8_t updateFormat(bool b_SendRequest=false);
  */

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif
};

} //__IsoAgLib

#endif
