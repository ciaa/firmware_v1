/*
  vtobjectobjectpointer_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTOBJECTPOINTER_C_H
#define VTOBJECTOBJECTPOINTER_C_H

#include <IsoAgLib/isoaglib_config.h>

#ifdef USE_VTOBJECT_objectpointer
#include "vtobject_c.h"
#include "vttypes.h"
#include "vtclientconnection_c.h"


namespace __IsoAgLib {

class vtObjectObjectPointer_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectObjectPointer_s* vtObjectObjectPointerSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  { vtObject_c::init ((iVtObject_s*) vtObjectObjectPointerSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);}

  iVtObjectObjectPointer_s* get_vtObjectObjectPointer_a() { return (iVtObjectObjectPointer_s *)&(get_vtObject_a()); }

  vtObjectObjectPointer_c();

  uint32_t fitTerminal() const;

  void setValue(const IsoAgLib::iVtObject_c* apc_newObject, bool b_updateObject= false, bool b_enableReplaceOfCmd=false);

  void setOriginSKM(bool b_SKM);

  void setOriginBTN(IsoAgLib::iVtObjectButton_c* /*p_btn*/);
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** these attributes are in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 27; }

  uint16_t updateValue(bool b_SendRequest=false);
  */

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif
};

} // __IsoAgLib

#endif

#endif
