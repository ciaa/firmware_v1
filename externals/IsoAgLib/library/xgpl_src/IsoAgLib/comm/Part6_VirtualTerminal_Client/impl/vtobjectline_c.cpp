/*
  vtobjectline_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectline_c.h"

#ifdef USE_VTOBJECT_line
#include "../ivtobjectlineattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectLine_c::stream(uint8_t* destMemory,
                       uint16_t maxBytes,
                       objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectLine_a
#define MACRO_vtObjectTypeS iVtObjectLine_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    MACRO_scaleSKLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 13; // Object Type = Line
      destMemory [3] = vtObjectLine_a->lineAttributes->getID() & 0xFF;
      destMemory [4] = vtObjectLine_a->lineAttributes->getID() >> 8;
      if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) {
        destMemory [5] = (((uint32_t) vtObjectLine_a->width*factorM)/factorD) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectLine_a->width*factorM)/factorD) >> 8;
        destMemory [7] = (((uint32_t) vtObjectLine_a->height*factorM)/factorD) & 0xFF;
        destMemory [8] = (((uint32_t) vtObjectLine_a->height*factorM)/factorD) >> 8;
      } else {
        destMemory [5] = (((uint32_t) vtObjectLine_a->width*vtDimension)/opDimension) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectLine_a->width*vtDimension)/opDimension) >> 8;
        destMemory [7] = (((uint32_t) vtObjectLine_a->height*vtDimension)/opDimension) & 0xFF;
        destMemory [8] = (((uint32_t) vtObjectLine_a->height*vtDimension)/opDimension) >> 8;
      }
      destMemory [9] = vtObjectLine_a->lineDirection;
      destMemory [10] = vtObjectLine_a->numberOfMacrosToFollow;
      sourceOffset += 11;
      curBytes += 11;
    }

    MACRO_streamEventMacro(11);
    return curBytes;
}


vtObjectLine_c::vtObjectLine_c() {}


uint32_t
vtObjectLine_c::fitTerminal() const
{
  MACRO_localVars;
  return 11+vtObjectLine_a->numberOfMacrosToFollow*2;
}


void
vtObjectLine_c::setOriginSKM(bool b_SKM)
{
  MACRO_localVars;
  if (b_SKM) {
    s_properties.flags |= FLAG_ORIGIN_SKM;
    vtObjectLine_a->lineAttributes->setOriginSKM (b_SKM);
  }
}

void
vtObjectLine_c::setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectLine_a(), width),  sizeof(iVtObjectLine_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectLine_a(), height), sizeof(iVtObjectLine_s), newHeight);
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSize (this, newWidth, newHeight, b_enableReplaceOfCmd);
}

void
vtObjectLine_c::setEndPoint (uint16_t newWidth, uint16_t newHeight, uint8_t newLineDirection, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectLine_a(), width), sizeof(iVtObjectLine_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectLine_a(), height), sizeof(iVtObjectLine_s), newHeight);
    saveValue8  (MACRO_getStructOffset(get_vtObjectLine_a(), lineDirection), sizeof(iVtObjectLine_s), newLineDirection);
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeEndPoint (this, newWidth, newHeight, newLineDirection, b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectLine_c::updateLineAttributes(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectLine_a(), lineAttributes), sizeof(iVtObjectLine_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectLine_a(), lineAttributes), sizeof(iVtObjectLine_s));
}

uint16_t
vtObjectLine_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectLine_a(), width), sizeof(iVtObjectLine_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectLine_a(), width), sizeof(iVtObjectLine_s));
}

uint16_t
vtObjectLine_c::updateHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectLine_a(), height), sizeof(iVtObjectLine_s), 3);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectLine_a(), height), sizeof(iVtObjectLine_s));
}

uint8_t
vtObjectLine_c::updateLineDirection(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectLine_a(), lineDirection), sizeof(iVtObjectLine_s), 4);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectLine_a(), lineDirection), sizeof(iVtObjectLine_s));
}

void
vtObjectLine_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectLine_a(), lineAttributes), sizeof(iVtObjectLine_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectLine_a(), width), sizeof(iVtObjectLine_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue16(MACRO_getStructOffset(get_vtObjectLine_a(), height), sizeof(iVtObjectLine_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 4: saveValue8(MACRO_getStructOffset(get_vtObjectLine_a(), lineDirection), sizeof(iVtObjectLine_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    default: break;
  }
}
#endif

} // __IsoAgLib

#endif
