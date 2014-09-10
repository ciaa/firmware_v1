/*
  vtobjectkey_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectkey_c.h"

#ifdef USE_VTOBJECT_key
#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectKey_c::stream(uint8_t* destMemory,
                      uint16_t maxBytes,
                      objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectKey_a
#define MACRO_vtObjectTypeS iVtObjectKey_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 5; // Object Type = Key
      destMemory [3] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectKey_a->backgroundColour, this, IsoAgLib::BackgroundColour);
      destMemory [4] = vtObjectKey_a->keyCode;
      destMemory [5] = vtObjectKey_a->numberOfObjectsToFollow;
      destMemory [6] = vtObjectKey_a->numberOfMacrosToFollow;
      sourceOffset += 7;
      curBytes += 7;
    }

    MACRO_streamObjectXYcenteredInSoftKey(7);
    MACRO_streamEventMacro(7U+vtObjectKey_a->numberOfObjectsToFollow*6U);
    return curBytes;
}



vtObjectKey_c::vtObjectKey_c() {}


uint32_t
vtObjectKey_c::fitTerminal() const
{
  MACRO_localVars;
  return 7+vtObjectKey_a->numberOfObjectsToFollow*6+vtObjectKey_a->numberOfMacrosToFollow*2;
}

bool
vtObjectKey_c::moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildLocation (apc_childObject, dx, dy, b_updateObject, vtObjectKey_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectKey_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectKey_a(), objectsToFollow), sizeof(iVtObjectKey_s), b_enableReplaceOfCmd);
}

bool
vtObjectKey_c::setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t x, int16_t y, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildPosition (apc_childObject, x, y, b_updateObject, vtObjectKey_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectKey_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectKey_a(), objectsToFollow), sizeof(iVtObjectKey_s), b_enableReplaceOfCmd);
}


void
vtObjectKey_c::setOriginSKM(bool /*b_SKM*/)
{
  MACRO_localVars;
  for (int i=0; i<vtObjectKey_a->numberOfObjectsToFollow; i++) {
    vtObjectKey_a->objectsToFollow[i].vtObject->setOriginSKM (true);
  }
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint8_t
vtObjectKey_c::updateBackgroundColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectKey_a(), backgroundColour), sizeof(iVtObjectKey_s), 1);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectKey_a(), backgroundColour), sizeof(iVtObjectKey_s));
}

uint8_t
vtObjectKey_c::updateKeyCode(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectKey_a(), keyCode), sizeof(iVtObjectKey_s), 2);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectKey_a(), keyCode), sizeof(iVtObjectKey_s));
}

void
vtObjectKey_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue8(MACRO_getStructOffset(get_vtObjectKey_a(), backgroundColour), sizeof(iVtObjectKey_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 2: saveValue8(MACRO_getStructOffset(get_vtObjectKey_a(), keyCode), sizeof(iVtObjectKey_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    default: break;
  }
}
#endif

} // __IsoAgLib

#endif
