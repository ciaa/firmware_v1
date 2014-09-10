/*
  vtobjectalarmmask_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectalarmmask_c.h"

#ifdef USE_VTOBJECT_alarmmask
#include "vtclient_c.h"
#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectmacro_c.h"


namespace __IsoAgLib {

int16_t
vtObjectAlarmMask_c::stream(uint8_t* destMemory,
                            uint16_t maxBytes,
                            objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectAlarmMask_a
#define MACRO_vtObjectTypeS iVtObjectAlarmMask_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    int32_t factorM=1; // should never happen
    int32_t factorD=1; // should never happen

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObjectAlarmMask_a->ID & 0xFF;
      destMemory [1] = vtObjectAlarmMask_a->ID >> 8;
      destMemory [2] = 2; // Object Type = Alarm Mask
      destMemory [3] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectAlarmMask_a->backgroundColour, this, IsoAgLib::BackgroundColour);
      if (vtObjectAlarmMask_a->softKeyMask != NULL) {
          destMemory [4] = vtObjectAlarmMask_a->softKeyMask->getID() & 0xFF;
          destMemory [5] = vtObjectAlarmMask_a->softKeyMask->getID() >> 8;
      } else {
          destMemory [4] = 0xFF;
          destMemory [5] = 0xFF;
      }
      destMemory [6] = vtObjectAlarmMask_a->priority;
      destMemory [7] = vtObjectAlarmMask_a->acousticSignal;
      destMemory [8] = vtObjectAlarmMask_a->numberOfObjectsToFollow;
      destMemory [9] = vtObjectAlarmMask_a->numberOfMacrosToFollow;
      sourceOffset += 10;
      curBytes += 10;
    }

    MACRO_streamObjectXY(10);
    MACRO_streamEventMacro(10U+vtObjectAlarmMask_a->numberOfObjectsToFollow*6U);
    return curBytes;
}


vtObjectAlarmMask_c::vtObjectAlarmMask_c() {}


uint32_t
vtObjectAlarmMask_c::fitTerminal() const
{
  MACRO_localVars;
  return 10+vtObjectAlarmMask_a->numberOfObjectsToFollow*6+vtObjectAlarmMask_a->numberOfMacrosToFollow*2;
}


void
vtObjectAlarmMask_c::setSoftKeyMask(IsoAgLib::iVtObjectSoftKeyMask_c* newSoftKeyMask,
                                    bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) saveValueP (MACRO_getStructOffset(get_vtObjectAlarmMask_a(), softKeyMask), sizeof(iVtObjectAlarmMask_s), newSoftKeyMask);

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSoftKeyMask(
    this, 2 /* "Type: Alarm Mask" */, (newSoftKeyMask == NULL) ? 0xFFFF : newSoftKeyMask->getID(), b_enableReplaceOfCmd );
}

/// No cmdReplacing here, as it's a relative command!!
bool
vtObjectAlarmMask_c::moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject)
{
  MACRO_localVars;
  return genericChangeChildLocation (apc_childObject, dx, dy, b_updateObject, vtObjectAlarmMask_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectAlarmMask_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectAlarmMask_a(), objectsToFollow), sizeof(iVtObjectAlarmMask_s), false);
}

bool
vtObjectAlarmMask_c::setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t x, int16_t y, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildPosition (apc_childObject, x, y, b_updateObject, vtObjectAlarmMask_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectAlarmMask_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectAlarmMask_a(), objectsToFollow), sizeof(iVtObjectAlarmMask_s), b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint8_t
vtObjectAlarmMask_c::updateBackgroundColour (bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), backgroundColour), sizeof(iVtObjectAlarmMask_s), 1);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), backgroundColour), sizeof(iVtObjectAlarmMask_s));
}

uint16_t
vtObjectAlarmMask_c::updateSoftKeyMask(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), softKeyMask), sizeof(iVtObjectAlarmMask_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), softKeyMask), sizeof(iVtObjectAlarmMask_s));
}

uint8_t
vtObjectAlarmMask_c::updatePriority (bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), priority), sizeof(iVtObjectAlarmMask_s), 3);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), priority), sizeof(iVtObjectAlarmMask_s));
}

uint8_t
vtObjectAlarmMask_c::updateAcousticSignal(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), acousticSignal), sizeof(iVtObjectAlarmMask_s), 4);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), acousticSignal), sizeof(iVtObjectAlarmMask_s));
}

void
vtObjectAlarmMask_c::saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue8(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), backgroundColour), sizeof(iVtObjectAlarmMask_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), softKeyMask), sizeof(iVtObjectAlarmMask_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue8(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), priority), sizeof(iVtObjectAlarmMask_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 4: saveValue8(MACRO_getStructOffset(get_vtObjectAlarmMask_a(), acousticSignal), sizeof(iVtObjectAlarmMask_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    default: break;
  }
}
#endif
} // end of namespace __IsoAgLib
#endif
