/*
  vtobjectworkingset_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTWORKINGSET_C_H
#define VTOBJECTWORKINGSET_C_H

#include "vtobject_c.h"
#include "../ivtobjectmask_c.h"


namespace __IsoAgLib {

class vtObjectWorkingSet_c : public vtObject_c
{
public:
  int16_t stream(uint8_t* destMemory,
                 uint16_t maxBytes,
                 objRange_t sourceOffset);

  void init(const iVtObjectWorkingSet_s* vtObjectWorkingSetSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) { vtObject_c::init ((iVtObject_s*) vtObjectWorkingSetSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA); };

  iVtObjectWorkingSet_s* get_vtObjectWorkingSet_a() { return (iVtObjectWorkingSet_s *)&(get_vtObject_a()); }

  vtObjectWorkingSet_c();

  uint32_t fitTerminal() const;

  void changeActiveMask(IsoAgLib::iVtObjectMask_c* apc_vtObjectMask, bool b_updateObject= false, bool b_enableReplaceOfCmd=false);

  void changeBackgroundColour(uint8_t newColour, bool b_updateObject= false, bool b_enableReplaceOfCmd=false);

  void setOriginSKM(bool b_SKM);

  bool moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);
  bool setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  bool controlAudioDevice (uint8_t aui8_repetitions, uint16_t aui16_frequency, uint16_t aui16_onTime, uint16_t aui16_offTime);

  bool setAudioVolume (uint8_t aui8_volume);

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** these attributes are in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 0; }
  uint8_t updateBackgroundColour(bool b_SendRequest=false);
  uint8_t updateSelectable(bool b_SendRequest=false);
  uint16_t updateActiveMask(bool b_SendRequest=false);
  */

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif
};

} // __IsoAgLib

#endif
