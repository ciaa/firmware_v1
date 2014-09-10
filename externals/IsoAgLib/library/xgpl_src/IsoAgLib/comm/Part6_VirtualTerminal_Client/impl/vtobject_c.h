/*
  vtobject_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECT_C_H
#define VTOBJECT_C_H

#include "../ivtobject_c.h"
#include "vttypes.h"
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoname_c.h>
#include <IsoAgLib/util/iassert.h>


namespace __IsoAgLib {

class vtObject_c : public IsoAgLib::iVtObject_c
{
public:
  vtObject_c();
  virtual ~vtObject_c();

  virtual int16_t stream(uint8_t* destMemory,
                         uint16_t maxBytes,
                         objRange_t sourceOffset) = 0;

  virtual uint32_t fitTerminal() const = 0;
 
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  virtual void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue) = 0;
#endif

  void init (iVtObject_s* aps_vtObject_a MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  {
    // typical double init is caught in objectpool-class's init-call!
    vtObject_a = aps_vtObject_a;
    s_properties.flags &= ~FLAG_IN_RAM;
    s_properties.flags &= ~FLAG_STRING_IN_RAM;
    s_properties.flags &= ~FLAG_OBJECTS2FOLLOW_IN_RAM;
    // NOTE: If objects were modified using b_updateObject==true,
    // then these modifications are GONE and result in a MEMORY LEAK
    // This is subject to be correctly fixed in some later version, if needed!

    MULTITON_INST_INIT_CALL
  }

  //! Internal checker function
  bool isOmittedFromUpload() const;

protected:
  iVtObject_s& get_vtObject_a()
  {
    isoaglib_header_assert(vtObject_a);
    return *vtObject_a;
  }

  void setAttribute      (uint8_t attrID, uint32_t newValue, bool b_enableReplaceOfCmd=true);
  void setAttributeFloat (uint8_t attrID, float newValue, bool b_enableReplaceOfCmd=true);

  #ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  void getAttribute      (uint8_t attrID, bool b_enableReplaceOfCmd=true);
  #endif

  void createRamStructIfNotYet (uint16_t ui16_structLen);

  void saveValue8     (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_newValue);
  void saveValue16    (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint16_t ui16_newValue);
  void saveValue32    (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint32_t ui32_newValue);
  void saveSignedValue8  (uint16_t ui16_structOffset, uint16_t ui16_structLen, int8_t i8_newValue);
  void saveSignedValue16 (uint16_t ui16_structOffset, uint16_t ui16_structLen, int16_t i16_newValue);
  void saveSignedValue32 (uint16_t ui16_structOffset, uint16_t ui16_structLen, int32_t i32_newValue);
  void saveValueFloat (uint16_t ui16_structOffset, uint16_t ui16_structLen, float f_newValue);
  void saveValueP     (uint16_t ui16_structOffset, uint16_t ui16_structLen, const IsoAgLib::iVtObject_c* const p_newValue);
  void saveValueISOName (uint16_t ui16_structOffset, uint16_t ui16_structLen, const IsoAgLib::iIsoName_c& ar_newIsoName);
  void saveValue8SetAttribute     (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, uint8_t ui8_newValue, uint8_t newValueSend, bool b_enableReplaceOfCmd);
  void saveValue16SetAttribute    (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, uint16_t ui16_newValue, bool b_enableReplaceOfCmd);
  void saveValue32SetAttribute    (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, uint32_t ui32_newValue, bool b_enableReplaceOfCmd);
  void saveValueFloatSetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, float f_newValue, bool b_enableReplaceOfCmd);
  void saveValuePSetAttribute     (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, IsoAgLib::iVtObject_c* p_newValue, bool b_enableReplaceOfCmd);
  void saveValueBool  (const uint16_t ui16_structOffset, const uint16_t ui16_structLen, bool b_newValue);

  uint8_t getValue8   (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded=false);
  uint16_t getValue16 (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded=false);
  uint32_t getValue32 (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded=false);
  int8_t getSignedValue8   (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded=false);
  int16_t getSignedValue16 (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded=false);
  int32_t getSignedValue32 (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded=false);
  float getValueFloat (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded=false);
  #ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  uint8_t getValue8GetAttribute  (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_createRamStructIfNeeded=false);
  uint16_t getValue16GetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_createRamStructIfNeeded=false);
  uint32_t getValue32GetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_createRamStructIfNeeded=false);
  int8_t getSignedValue8GetAttribute  (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_createRamStructIfNeeded=false);
  int16_t getSignedValue16GetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_createRamStructIfNeeded=false);
  int32_t getSignedValue32GetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_createRamStructIfNeeded=false);
  float getValueFloatGetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_enableReplaceOfCmd=false);
  #endif

  bool genericChangeChildLocation (IsoAgLib::iVtObject_c* childObject, int16_t dx, int16_t dy, bool b_updateObject, uint8_t numObjectsToFollow, IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow, uint16_t ui16_structOffset, uint16_t ui16_structLen,bool b_enableReplaceOfCmd);
  bool genericChangeChildPosition (IsoAgLib::iVtObject_c* childObject, int16_t dx, int16_t dy, bool b_updateObject, uint8_t numObjectsToFollow, IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow, uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_enableReplaceOfCmd);

  // ATTENTION: Should only be used by Input-Objects of course!
  bool able (uint8_t enOrDis, bool b_updateObject= false, bool b_enableReplaceOfCmd=true);
  bool select(uint8_t selectOrActivate);

private:
  friend class vtLayoutManager_c;
  bool genericChangeChildLocationPosition (bool ab_isLocation, IsoAgLib::iVtObject_c* childObject, int16_t dx, int16_t dy, bool b_updateObject, uint8_t numObjectsToFollow, IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow, uint16_t ui16_structOffset, uint16_t ui16_structLen);

  virtual void updateEnable(uint8_t /*aui8_enOrDis*/) {}
};


inline
bool
vtObject_c::isOmittedFromUpload() const
{
  return (s_properties.flags & FLAG_OMIT_OBJECT) != 0;
}


} // __IsoAgLib

#endif
