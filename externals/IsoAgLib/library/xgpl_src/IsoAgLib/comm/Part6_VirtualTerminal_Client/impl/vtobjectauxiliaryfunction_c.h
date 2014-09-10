/*
  vtobjectauxiliaryfunction_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTAUXILIARYFUNCTION_C_H
#define VTOBJECTAUXILIARYFUNCTION_C_H

#include <IsoAgLib/isoaglib_config.h>


#ifdef USE_VTOBJECT_auxiliaryfunction

#include "vtobject_c.h"
#include "vttypes.h"

namespace __IsoAgLib {

class vtObjectAuxiliaryFunction_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectAuxiliaryFunction_s* vtObjectAuxiliaryFunctionSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  {vtObject_c::init((iVtObject_s *)vtObjectAuxiliaryFunctionSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);}

  iVtObjectAuxiliaryFunction_s* get_vtObjectAuxiliaryFunction_a() {return (iVtObjectAuxiliaryFunction_s *)&(get_vtObject_a());}

  vtObjectAuxiliaryFunction_c();
  ~vtObjectAuxiliaryFunction_c() {}

  uint32_t fitTerminal() const;

  void setOriginSKM(bool b_SKM);

  bool moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);
  bool setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 29; }
  */

  void saveReceivedAttribute (uint8_t /*attrID*/, uint8_t* /*pui8_attributeValue*/) {};
#endif
};

} // __IsoAgLib

#endif

#endif
