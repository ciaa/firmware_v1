/*
  vtobjectworkingset_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "vtobjectworkingset_c.h"

#include "vtclient_c.h"
#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"


namespace __IsoAgLib {

int16_t
vtObjectWorkingSet_c::stream(uint8_t* destMemory,
                             uint16_t maxBytes,
                             objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectWorkingSet_a
#define MACRO_vtObjectTypeS iVtObjectWorkingSet_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
        destMemory [0] = vtObjectWorkingSet_a->ID & 0xFF;
        destMemory [1] = vtObjectWorkingSet_a->ID >> 8;
        destMemory [2] = 0; // Object Type = Working Set
        destMemory [3] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectWorkingSet_a->backgroundColour, this, IsoAgLib::BackgroundColour);
        destMemory [4] = vtObjectWorkingSet_a->selectable;
        if (vtObjectWorkingSet_a->activeMask != NULL) {
            destMemory [5] = vtObjectWorkingSet_a->activeMask->getID() & 0xFF;
            destMemory [6] = vtObjectWorkingSet_a->activeMask->getID() >> 8;
        } else {
            destMemory [5] = 0; // using 0x00 here as 0xFFFF is NOT allowed
            destMemory [6] = 0; // using 0x00 here as 0xFFFF is NOT allowed
        }
        destMemory [7] = vtObjectWorkingSet_a->numberOfObjectsToFollow;
        destMemory [8] = vtObjectWorkingSet_a->numberOfMacrosToFollow;
        destMemory [9] = vtObjectWorkingSet_a->numberOfLanguagesToFollow;

        sourceOffset += 10;
        curBytes += 10;
    }

    MACRO_streamObjectXYcenteredInSoftKey(10);
    uint16_t tempOffset = 10+vtObjectWorkingSet_a->numberOfObjectsToFollow*6;
    MACRO_streamEventMacro(tempOffset);
    tempOffset = tempOffset + vtObjectWorkingSet_a->numberOfMacrosToFollow*2;
    MACRO_streamLanguages(tempOffset);

    return curBytes;
}

vtObjectWorkingSet_c::vtObjectWorkingSet_c() {}


uint32_t
vtObjectWorkingSet_c::fitTerminal() const
{
  MACRO_localVars;
  return 10+vtObjectWorkingSet_a->numberOfObjectsToFollow*6+vtObjectWorkingSet_a->numberOfMacrosToFollow*2+vtObjectWorkingSet_a->numberOfLanguagesToFollow*2;
}


void
vtObjectWorkingSet_c::changeActiveMask(IsoAgLib::iVtObjectMask_c* apc_vtObjectMask, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) saveValueP (MACRO_getStructOffset(get_vtObjectWorkingSet_a(), activeMask), sizeof(iVtObjectWorkingSet_s), apc_vtObjectMask);

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeActiveMask( this, apc_vtObjectMask, b_enableReplaceOfCmd);
}

void
vtObjectWorkingSet_c::changeBackgroundColour(uint8_t newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) saveValue8 (MACRO_getStructOffset(get_vtObjectWorkingSet_a(), backgroundColour), sizeof(iVtObjectWorkingSet_s), newValue);

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeBackgroundColour (this, newValue, b_enableReplaceOfCmd);
}

bool
vtObjectWorkingSet_c::moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildLocation (apc_childObject, dx, dy, b_updateObject, vtObjectWorkingSet_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectWorkingSet_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectWorkingSet_a(), objectsToFollow), sizeof(iVtObjectWorkingSet_s), b_enableReplaceOfCmd);
}

bool
vtObjectWorkingSet_c::setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t x, int16_t y, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildPosition (apc_childObject, x, y, b_updateObject, vtObjectWorkingSet_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectWorkingSet_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectWorkingSet_a(), objectsToFollow), sizeof(iVtObjectWorkingSet_s), b_enableReplaceOfCmd);
}

bool
vtObjectWorkingSet_c::controlAudioDevice (uint8_t aui8_repetitions, uint16_t aui16_frequency, uint16_t aui16_onTime, uint16_t aui16_offTime)
{
  return __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandControlAudioDevice (aui8_repetitions, aui16_frequency, aui16_onTime, aui16_offTime);
}

bool
vtObjectWorkingSet_c::setAudioVolume (uint8_t aui8_volume)
{
  return __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandSetAudioVolume (aui8_volume);
}


void
vtObjectWorkingSet_c::setOriginSKM(bool /*b_SKM*/)
{
  MACRO_localVars;
  s_properties.flags |= FLAG_ORIGIN_SKM; // WS Descriptor has to fit inside a SoftKey!
  for (int i=0; i<vtObjectWorkingSet_a->numberOfObjectsToFollow; i++) {
    vtObjectWorkingSet_a->objectsToFollow[i].vtObject->setOriginSKM (true);
  }
}
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
/** these attributes are in parentheses in the spec, so commented out here
uint8_t
vtObjectWorkingSet_c::updateBackgroundColour (bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectWorkingSet_a(), backgroundColour), sizeof(iVtObjectWorkingSet_s), 1);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectWorkingSet_a(), backgroundColour), sizeof(iVtObjectWorkingSet_s));
}

uint8_t
vtObjectWorkingSet_c::updateSelectable(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectWorkingSet_a(), selectable), sizeof(iVtObjectWorkingSet_s), 2);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectWorkingSet_a(), selectable), sizeof(iVtObjectWorkingSet_s));
}

uint16_t
vtObjectWorkingSet_c::updateActiveMask(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectWorkingSet_a(), activeMask), sizeof(iVtObjectWorkingSet_s), 3);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectWorkingSet_a(), activeMask), sizeof(iVtObjectWorkingSet_s));
}
*/

void
vtObjectWorkingSet_c::saveReceivedAttribute (uint8_t attrID, uint8_t* /*pui8_attributeValue*/)
{
  switch (attrID)
  {
    /** these attributes are in parentheses in the spec, so commented out here
    case 1: saveValue8(MACRO_getStructOffset(get_vtObjectWorkingSet_a(), backgroundColour), sizeof(iVtObjectWorkingSet_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 2: saveValue8(MACRO_getStructOffset(get_vtObjectWorkingSet_a(), selectable), sizeof(iVtObjectWorkingSet_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 3: saveValue16(MACRO_getStructOffset(get_vtObjectWorkingSet_a(), activeMask), sizeof(iVtObjectWorkingSet_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    */
    default: break;
  }
}
#endif

} // __IsoAgLib
