/*
  vtobjectellipse_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectellipse_c.h"

#ifdef USE_VTOBJECT_ellipse
#include "../ivtobjectlineattributes_c.h"
#include "../ivtobjectfillattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectEllipse_c::stream(uint8_t* destMemory,
                          uint16_t maxBytes,
                          objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectEllipse_a
#define MACRO_vtObjectTypeS iVtObjectEllipse_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    MACRO_scaleSKLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 15; // Object Type = Ellipse
      destMemory [3] = vtObjectEllipse_a->lineAttributes->getID() & 0xFF;
      destMemory [4] = vtObjectEllipse_a->lineAttributes->getID() >> 8;
      if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) {
        destMemory [5] = (((uint32_t) vtObjectEllipse_a->width*factorM)/factorD) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectEllipse_a->width*factorM)/factorD) >> 8;
        destMemory [7] = (((uint32_t) vtObjectEllipse_a->height*factorM)/factorD) & 0xFF;
        destMemory [8] = (((uint32_t) vtObjectEllipse_a->height*factorM)/factorD) >> 8;
      } else {
        destMemory [5] = (((uint32_t) vtObjectEllipse_a->width*vtDimension)/opDimension) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectEllipse_a->width*vtDimension)/opDimension) >> 8;
        destMemory [7] = (((uint32_t) vtObjectEllipse_a->height*vtDimension)/opDimension) & 0xFF;
        destMemory [8] = (((uint32_t) vtObjectEllipse_a->height*vtDimension)/opDimension) >> 8;

      }
      destMemory [9] = vtObjectEllipse_a->ellipseType;
      destMemory [10] = vtObjectEllipse_a->startAngle;
      destMemory [11] = vtObjectEllipse_a->endAngle;

      if (vtObjectEllipse_a->fillAttributes != NULL)
      {
        destMemory [12] = vtObjectEllipse_a->fillAttributes->getID() & 0xFF;
        destMemory [13] = vtObjectEllipse_a->fillAttributes->getID() >> 8;
      } else {
        destMemory [12] = 0xFF;
        destMemory [13] = 0xFF;
      }

      destMemory [14] = vtObjectEllipse_a->numberOfMacrosToFollow;
      sourceOffset += 15;
      curBytes += 15;
    }

    MACRO_streamEventMacro(15);
    return curBytes;
}


vtObjectEllipse_c::vtObjectEllipse_c() {}


uint32_t
vtObjectEllipse_c::fitTerminal() const
{
  MACRO_localVars;
  return 15+vtObjectEllipse_a->numberOfMacrosToFollow*2;
}


void
vtObjectEllipse_c::setOriginSKM(bool b_SKM)
{
  MACRO_localVars;
  if (b_SKM) {
    s_properties.flags |= FLAG_ORIGIN_SKM;
    vtObjectEllipse_a->lineAttributes->setOriginSKM (b_SKM);
  }
}

void
vtObjectEllipse_c::setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectEllipse_a(), width),  sizeof(iVtObjectEllipse_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectEllipse_a(), height), sizeof(iVtObjectEllipse_s), newHeight);
  }
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSize(
    this, newWidth, newHeight, b_enableReplaceOfCmd );
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectEllipse_c::updateLineAttributes(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectEllipse_a(), lineAttributes), sizeof(iVtObjectEllipse_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectEllipse_a(), lineAttributes), sizeof(iVtObjectEllipse_s));
}

uint16_t
vtObjectEllipse_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectEllipse_a(), lineAttributes), sizeof(iVtObjectEllipse_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectEllipse_a(), lineAttributes), sizeof(iVtObjectEllipse_s));
}

uint16_t
vtObjectEllipse_c::updateHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectEllipse_a(), height), sizeof(iVtObjectEllipse_s), 3);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectEllipse_a(), height), sizeof(iVtObjectEllipse_s));
}

uint8_t
vtObjectEllipse_c::updateEllipseType(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectEllipse_a(), ellipseType), sizeof(iVtObjectEllipse_s), 4);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectEllipse_a(), ellipseType), sizeof(iVtObjectEllipse_s));
}

uint8_t
vtObjectEllipse_c::updateStartAngle(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectEllipse_a(), startAngle), sizeof(iVtObjectEllipse_s), 5);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectEllipse_a(), startAngle), sizeof(iVtObjectEllipse_s));
}

uint8_t
vtObjectEllipse_c::updateEndAngle(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectEllipse_a(), endAngle), sizeof(iVtObjectEllipse_s), 6);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectEllipse_a(), endAngle), sizeof(iVtObjectEllipse_s));
}

uint16_t
vtObjectEllipse_c::updateFillAttributes(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectEllipse_a(), fillAttributes), sizeof(iVtObjectEllipse_s), 7);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectEllipse_a(), fillAttributes), sizeof(iVtObjectEllipse_s));
}

void
vtObjectEllipse_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectEllipse_a(), lineAttributes), sizeof(iVtObjectEllipse_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectEllipse_a(), lineAttributes), sizeof(iVtObjectEllipse_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue16(MACRO_getStructOffset(get_vtObjectEllipse_a(), height), sizeof(iVtObjectEllipse_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 4: saveValue8(MACRO_getStructOffset(get_vtObjectEllipse_a(), ellipseType), sizeof(iVtObjectEllipse_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 5: saveValue8(MACRO_getStructOffset(get_vtObjectEllipse_a(), startAngle), sizeof(iVtObjectEllipse_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 6: saveValue8(MACRO_getStructOffset(get_vtObjectEllipse_a(), endAngle), sizeof(iVtObjectEllipse_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 7: saveValue16(MACRO_getStructOffset(get_vtObjectEllipse_a(), fillAttributes), sizeof(iVtObjectEllipse_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    default: break;
  }
}
#endif

} // __IsoAgLib

#endif
