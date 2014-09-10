/*
  vtobjectrectangle_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectrectangle_c.h"

#ifdef USE_VTOBJECT_rectangle
#include "../ivtobjectlineattributes_c.h"
#include "../ivtobjectfillattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectRectangle_c::stream(uint8_t* destMemory,
                            uint16_t maxBytes,
                            objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectRectangle_a
#define MACRO_vtObjectTypeS iVtObjectRectangle_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    MACRO_scaleSKLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 14; // Object Type = Rectangle
      destMemory [3] = vtObjectRectangle_a->lineAttributes->getID() & 0xFF;
      destMemory [4] = vtObjectRectangle_a->lineAttributes->getID() >> 8;
      if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) {
        destMemory [5] = (((uint32_t) vtObjectRectangle_a->width*factorM)/factorD) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectRectangle_a->width*factorM)/factorD) >> 8;
        destMemory [7] = (((uint32_t) vtObjectRectangle_a->height*factorM)/factorD) & 0xFF;
        destMemory [8] = (((uint32_t) vtObjectRectangle_a->height*factorM)/factorD) >> 8;
      } else {
        destMemory [5] = (((uint32_t) vtObjectRectangle_a->width*vtDimension)/opDimension) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectRectangle_a->width*vtDimension)/opDimension) >> 8;
        destMemory [7] = (((uint32_t) vtObjectRectangle_a->height*vtDimension)/opDimension) & 0xFF;
        destMemory [8] = (((uint32_t) vtObjectRectangle_a->height*vtDimension)/opDimension) >> 8;

    }
      destMemory [9] = vtObjectRectangle_a->lineSuppression;

    if (vtObjectRectangle_a->fillAttributes != NULL)
    {
        destMemory [10] = vtObjectRectangle_a->fillAttributes->getID() & 0xFF;
        destMemory [11] = vtObjectRectangle_a->fillAttributes->getID() >> 8;
      } else {
        destMemory [10] = 0xFF;
        destMemory [11] = 0xFF;
      }

      destMemory [12] = vtObjectRectangle_a->numberOfMacrosToFollow;
      sourceOffset += 13;
      curBytes += 13;
    }

    MACRO_streamEventMacro(13);
    return curBytes;
}


vtObjectRectangle_c::vtObjectRectangle_c() {}


uint32_t
vtObjectRectangle_c::fitTerminal() const
{
  MACRO_localVars;
  return 13+vtObjectRectangle_a->numberOfMacrosToFollow*2;
}


void
vtObjectRectangle_c::setOriginSKM(bool b_SKM)
{
  MACRO_localVars;
  if (b_SKM) {
    s_properties.flags |= FLAG_ORIGIN_SKM;
    vtObjectRectangle_a->lineAttributes->setOriginSKM (b_SKM);
  }
}

void
vtObjectRectangle_c::setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectRectangle_a(), width),  sizeof(iVtObjectRectangle_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectRectangle_a(), height), sizeof(iVtObjectRectangle_s), newHeight);
  }
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSize (this, newWidth, newHeight, b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectRectangle_c::updateLineAttributes(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectRectangle_a(), lineAttributes), sizeof(iVtObjectRectangle_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectRectangle_a(), lineAttributes), sizeof(iVtObjectRectangle_s));
}

uint16_t
vtObjectRectangle_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectRectangle_a(), width), sizeof(iVtObjectRectangle_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectRectangle_a(), width), sizeof(iVtObjectRectangle_s));
}

uint16_t
vtObjectRectangle_c::updateHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectRectangle_a(), height), sizeof(iVtObjectRectangle_s), 3);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectRectangle_a(), height), sizeof(iVtObjectRectangle_s));
}

uint8_t
vtObjectRectangle_c::updateLineSuppression(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectRectangle_a(), lineSuppression), sizeof(iVtObjectRectangle_s), 4);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectRectangle_a(), lineSuppression), sizeof(iVtObjectRectangle_s));
}

uint16_t
vtObjectRectangle_c::updateFillAttributes(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectRectangle_a(), fillAttributes), sizeof(iVtObjectRectangle_s), 5);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectRectangle_a(), fillAttributes), sizeof(iVtObjectRectangle_s));
}

void
vtObjectRectangle_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectRectangle_a(), lineAttributes), sizeof(iVtObjectRectangle_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectRectangle_a(), width), sizeof(iVtObjectRectangle_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue16(MACRO_getStructOffset(get_vtObjectRectangle_a(), height), sizeof(iVtObjectRectangle_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 4: saveValue8(MACRO_getStructOffset(get_vtObjectRectangle_a(), lineSuppression), sizeof(iVtObjectRectangle_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 5: saveValue16(MACRO_getStructOffset(get_vtObjectRectangle_a(), fillAttributes), sizeof(iVtObjectRectangle_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    default: break;
  }
}
#endif

} // __IsoAgLib

#endif
