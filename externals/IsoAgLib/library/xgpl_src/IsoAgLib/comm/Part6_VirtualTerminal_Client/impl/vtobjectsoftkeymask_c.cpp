/*
  vtobjectsoftkeymask_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectsoftkeymask_c.h"
#include "../ivtobjectmacro_c.h"

#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectSoftKeyMask_c::stream(uint8_t* destMemory,
                              uint16_t maxBytes,
                              objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectSoftKeyMask_a
#define MACRO_vtObjectTypeS iVtObjectSoftKeyMask_s
  MACRO_streamLocalVars;

  if (__IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId)
      .getVtServerInst().getVtCapabilities()->skVirtual < vtObjectSoftKeyMask_a->numberOfObjectsToFollow)
  { // can't upload this SKM because it has more Keys than virtually supported
    return 0;
  }

  if (sourceOffset == 0) { // dump out constant sized stuff
    destMemory [0] = vtObject_a->ID & 0xFF;
    destMemory [1] = vtObject_a->ID >> 8;
    destMemory [2] = 4; // Object Type = Soft Key Mask
    destMemory [3] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectSoftKeyMask_a->backgroundColour, this, IsoAgLib::BackgroundColour);
    destMemory [4] = vtObjectSoftKeyMask_a->numberOfObjectsToFollow;
    destMemory [5] = vtObjectSoftKeyMask_a->numberOfMacrosToFollow;

    sourceOffset += 6;
    curBytes += 6;
  }

  MACRO_streamObject(6);
  MACRO_streamEventMacro(6+vtObjectSoftKeyMask_a->numberOfObjectsToFollow*2U);
  return curBytes;
}


vtObjectSoftKeyMask_c::vtObjectSoftKeyMask_c()
{
}


uint32_t
vtObjectSoftKeyMask_c::fitTerminal() const
{
  MACRO_localVars;

  if (__IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId)
      .getVtServerInst().getVtCapabilities()->skVirtual < vtObjectSoftKeyMask_a->numberOfObjectsToFollow)
  { // can't upload this SKM because it has more Keys than virtually supported
    return 0;
  }
  else
  { // okay, enough virtual SKs supported, so upload this SKM
    return 6+vtObjectSoftKeyMask_a->numberOfObjectsToFollow*2+vtObjectSoftKeyMask_a->numberOfMacrosToFollow*2;
  }
}


void
vtObjectSoftKeyMask_c::setOriginSKM(bool /*b_SKM*/)
{
  MACRO_localVars;
  for (int i=0; i<vtObjectSoftKeyMask_a->numberOfObjectsToFollow; i++) {
    vtObjectSoftKeyMask_a->objectsToFollow[i].vtObject->setOriginSKM (true);
  }
}
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint8_t
vtObjectSoftKeyMask_c::updateBackgroundColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectSoftKeyMask_a(), backgroundColour), sizeof(iVtObjectSoftKeyMask_s), 1);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectSoftKeyMask_a(), backgroundColour), sizeof(iVtObjectSoftKeyMask_s));
}

void
vtObjectSoftKeyMask_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  if (attrID == 1)
    saveValue8(MACRO_getStructOffset(get_vtObjectSoftKeyMask_a(), backgroundColour), sizeof(iVtObjectSoftKeyMask_s), convertLittleEndianStringUi8(pui8_attributeValue));
}
#endif

} // __IsoAgLib
