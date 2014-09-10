/*
  vtobjectcontainer_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTCONTAINER_C_H
#define VTOBJECTCONTAINER_C_H

#include <IsoAgLib/isoaglib_config.h>


#ifdef USE_VTOBJECT_container

#include "vtobject_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

class vtObjectContainer_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectContainer_s* vtObjectContainer_sROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) { vtObject_c::init ((iVtObject_s*) vtObjectContainer_sROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);}

  iVtObjectContainer_s* get_vtObjectContainer_a() { return (iVtObjectContainer_s *)&(get_vtObject_a()); }

  vtObjectContainer_c();

  uint32_t fitTerminal() const;

  void setOriginSKM(bool b_SKM);

  void setOriginBTN(IsoAgLib::iVtObjectButton_c* /*p_btn*/);

  void setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  bool moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);
  bool setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** these attributes are in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 3; }

  uint16_t updateWidth(bool b_SendRequest=false);

  uint16_t updateHeight(bool b_SendRequest=false);

  uint8_t updateHidden(bool b_SendRequest=false);
  */

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif
protected:
  void hideShow(uint8_t b_hideOrShow, bool b_updateObject= false, bool b_enableReplaceOfCmd=false);
};

} // __IsoAgLib

#endif

#endif
