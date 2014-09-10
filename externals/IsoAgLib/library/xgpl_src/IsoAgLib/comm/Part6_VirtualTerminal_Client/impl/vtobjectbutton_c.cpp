/*
  vtobjectbutton_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectbutton_c.h"

#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"

namespace IsoAgLib {
  // implement here a normal functions, as the compiler dislikes inlining of that simple functions
  // direct in scope of iVtObjectButton_s
  const iVtObjectButton_c::iVtObjectButton_s& iVtObjectButton_c::get_vtObjectButton_a()
  {
    return *vtObjectButton_c::get_vtObjectButton_a();
  }

}


namespace __IsoAgLib {

int16_t
vtObjectButton_c::stream(uint8_t* destMemory,
                         uint16_t maxBytes,
                         objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectButton_a
#define MACRO_vtObjectTypeS iVtObjectButton_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 6; // Object Type = Button
      destMemory [3] = (((uint32_t) vtObjectButton_a->width*vtDimension)/opDimension) & 0xFF;
      destMemory [4] = (((uint32_t) vtObjectButton_a->width*vtDimension)/opDimension) >> 8;
      destMemory [5] = (((uint32_t) vtObjectButton_a->height*vtDimension)/opDimension) & 0xFF;
      destMemory [6] = (((uint32_t) vtObjectButton_a->height*vtDimension)/opDimension) >> 8;
      destMemory [7] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectButton_a->backgroundColour, this, IsoAgLib::BackgroundColour);
      destMemory [8] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectButton_a->borderColour, this, IsoAgLib::BorderColour);
      destMemory [9] = vtObjectButton_a->keyCode;
      destMemory [10] = vtObjectButton_a->options;
      destMemory [11] = vtObjectButton_a->numberOfObjectsToFollow;
      destMemory [12] = vtObjectButton_a->numberOfMacrosToFollow;
      sourceOffset += 13;
      curBytes += 13;
    }

    MACRO_streamObjectXYcenteredInButton(13);
    MACRO_streamEventMacro(13U+vtObjectButton_a->numberOfObjectsToFollow*6U);
    return curBytes;
}


vtObjectButton_c::vtObjectButton_c() {}


uint32_t
vtObjectButton_c::fitTerminal() const
{
  MACRO_localVars;
  return 13+vtObjectButton_a->numberOfObjectsToFollow*6+vtObjectButton_a->numberOfMacrosToFollow*2;
}

void
vtObjectButton_c::setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectButton_a(), width),  sizeof(iVtObjectButton_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectButton_a(), height), sizeof(iVtObjectButton_s), newHeight);
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSize(
    this, newWidth, newHeight, b_enableReplaceOfCmd );
}

bool
vtObjectButton_c::moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildLocation (apc_childObject, dx, dy, b_updateObject, vtObjectButton_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectButton_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectButton_a(), objectsToFollow), sizeof(iVtObjectButton_s), b_enableReplaceOfCmd);
}

bool
vtObjectButton_c::setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t x, int16_t y, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildPosition (apc_childObject, x, y, b_updateObject, vtObjectButton_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectButton_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectButton_a(), objectsToFollow), sizeof(iVtObjectButton_s), b_enableReplaceOfCmd);
}

void
vtObjectButton_c::setOriginBTN(IsoAgLib::iVtObjectButton_c* /*p_btn*/)
{
  MACRO_localVars;
  p_parentButtonObject = (IsoAgLib::iVtObjectButton_c*)this;
  for (int i=0; i<vtObjectButton_a->numberOfObjectsToFollow; i++) {
    vtObjectButton_a->objectsToFollow[i].vtObject->setOriginBTN (p_parentButtonObject);
  }
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectButton_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectButton_a(), width), sizeof(iVtObjectButton_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectButton_a(), width), sizeof(iVtObjectButton_s));
}

uint16_t
vtObjectButton_c::updateHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectButton_a(), height), sizeof(iVtObjectButton_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectButton_a(), height), sizeof(iVtObjectButton_s));
}

uint8_t
vtObjectButton_c::updateBackgroundColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectButton_a(), backgroundColour), sizeof(iVtObjectButton_s), 3);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectButton_a(), backgroundColour), sizeof(iVtObjectButton_s));
}

uint8_t
vtObjectButton_c::updateBorderColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectButton_a(), borderColour), sizeof(iVtObjectButton_s), 4);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectButton_a(), borderColour), sizeof(iVtObjectButton_s));
}

uint8_t
vtObjectButton_c::updateKeyCode(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectButton_a(), keyCode), sizeof(iVtObjectButton_s), 5);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectButton_a(), keyCode), sizeof(iVtObjectButton_s));
}

uint8_t
vtObjectButton_c::updateOptions(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectButton_a(), options), sizeof(iVtObjectButton_s), 6);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectButton_a(), options), sizeof(iVtObjectButton_s));
}

void
vtObjectButton_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectButton_a(), width), sizeof(iVtObjectButton_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectButton_a(), height), sizeof(iVtObjectButton_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue8(MACRO_getStructOffset(get_vtObjectButton_a(), backgroundColour), sizeof(iVtObjectButton_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 4: saveValue8(MACRO_getStructOffset(get_vtObjectButton_a(), borderColour), sizeof(iVtObjectButton_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 5: saveValue8(MACRO_getStructOffset(get_vtObjectButton_a(), keyCode), sizeof(iVtObjectButton_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 6: saveValue8(MACRO_getStructOffset(get_vtObjectButton_a(), options), sizeof(iVtObjectButton_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    default: break;
  }
}
#endif
} // __IsoAgLib
