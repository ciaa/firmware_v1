/*
  ivtobjectmacro_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECTMACRO_C_H
#define IVTOBJECTMACRO_C_H

#include "impl/vtobjectmacro_c.h"


namespace IsoAgLib {

class iVtObjectMacro_c : public __IsoAgLib::vtObjectMacro_c
{
public:
  static uint16_t objectType() { return VT_OBJECT_TYPE_MACRO; }

  void init(const iVtObjectMacro_s* vtObjectMacroSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObjectMacro_c::init (vtObjectMacroSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  const iVtObjectMacro_s& get_vtObjectMacro_a() { return *vtObjectMacro_c::get_vtObjectMacro_a(); }

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES  
  /** these attributes are in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return vtObjectMacro_c::updateObjectType(); }
  */
#endif

  virtual uint16_t getObjectType() const { return objectType(); }
};

} // IsoAgLib

#endif
