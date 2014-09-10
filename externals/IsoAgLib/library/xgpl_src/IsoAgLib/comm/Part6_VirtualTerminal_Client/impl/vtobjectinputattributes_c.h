/*
  vtobjectinputattributes_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTINPUTATTRIBUTES_C_H
#define VTOBJECTINPUTATTRIBUTES_C_H

#include "../ivtobjectstring_c.h"
#include "vtclientconnection_c.h"


namespace __IsoAgLib {

class vtObjectInputAttributes_c : public IsoAgLib::iVtObjectString_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectInputAttributes_s* vtObjectInputAttributesSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  { vtObject_c::init ((iVtObject_s*) vtObjectInputAttributesSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); }

  iVtObjectInputAttributes_s* get_vtObjectInputAttributes_a() { return (iVtObjectInputAttributes_s *)&(get_vtObject_a()); }

  vtObjectInputAttributes_c();

  uint32_t fitTerminal() const;

  const char* getString();

  void setValidationStringCopy(const char* newValidationString, bool b_updateObject= false, bool b_enableReplaceOfCmd=false);
  void setValidationStringRef(const char* newValidationString, bool b_updateObject= false, bool b_enableReplaceOfCmd=false);

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** these attributes are in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 26; }

  uint8_t updateValidationType(bool b_SendRequest=false);
  */

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif
};

} // __IsoAgLib

#endif
