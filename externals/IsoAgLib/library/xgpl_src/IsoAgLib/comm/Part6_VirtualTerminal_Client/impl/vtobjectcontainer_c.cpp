/*
  vtobjectcontainer_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectcontainer_c.h"
#include "../ivtobjectcontainer_c.h"

#ifdef USE_VTOBJECT_container
#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"

namespace IsoAgLib {
  // implement here a normal constructor/destructor, as the compiler dislikes inlining of that simple
  // constructor/destructor direct in scope of iVtObjectContainer_c
  iVtObjectContainer_c::iVtObjectContainer_c() {}
  iVtObjectContainer_c::~iVtObjectContainer_c() {}
}


namespace __IsoAgLib {

int16_t
vtObjectContainer_c::stream(uint8_t* destMemory,
                            uint16_t maxBytes,
                            objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectContainer_a
#define MACRO_vtObjectTypeS iVtObjectContainer_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    MACRO_scaleSKLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 3; // Object Type = Container
      if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) {
        destMemory [3] = (((uint32_t) vtObjectContainer_a->width*factorM)/factorD) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectContainer_a->width*factorM)/factorD) >> 8;
        destMemory [5] = (((uint32_t) vtObjectContainer_a->height*factorM)/factorD) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectContainer_a->height*factorM)/factorD) >> 8;
      } else {
        destMemory [3] = (((uint32_t) vtObjectContainer_a->width*vtDimension)/opDimension) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectContainer_a->width*vtDimension)/opDimension) >> 8;
        destMemory [5] = (((uint32_t) vtObjectContainer_a->height*vtDimension)/opDimension) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectContainer_a->height*vtDimension)/opDimension) >> 8;
      }
      destMemory [7] = vtObjectContainer_a->hidden;
      destMemory [8] = vtObjectContainer_a->numberOfObjectsToFollow;
      destMemory [9] = vtObjectContainer_a->numberOfMacrosToFollow;

      sourceOffset += 10;
      curBytes += 10;
    }

    MACRO_streamObjectXY(10);
    MACRO_streamEventMacro(10U+vtObjectContainer_a->numberOfObjectsToFollow*6U);
    return curBytes;
}


vtObjectContainer_c::vtObjectContainer_c() {}


uint32_t
vtObjectContainer_c::fitTerminal() const
{
  MACRO_localVars;
  return 10+vtObjectContainer_a->numberOfObjectsToFollow*6+vtObjectContainer_a->numberOfMacrosToFollow*2;
}


void
vtObjectContainer_c::hideShow(uint8_t b_hideOrShow, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) saveValue8 (MACRO_getStructOffset(get_vtObjectContainer_a(), hidden), sizeof(iVtObjectContainer_s), (!b_hideOrShow)&0x01);

   __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandHideShow(
     this, b_hideOrShow, b_enableReplaceOfCmd );
}

void
vtObjectContainer_c::setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectContainer_a(), width),  sizeof(iVtObjectContainer_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectContainer_a(), height), sizeof(iVtObjectContainer_s), newHeight);
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSize(
    this, newWidth, newHeight, b_enableReplaceOfCmd );
}

bool
vtObjectContainer_c::moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject, bool b_enableReplaceOfCmd)
{

  MACRO_localVars;
  return genericChangeChildLocation (apc_childObject, dx, dy, b_updateObject, vtObjectContainer_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectContainer_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectContainer_a(), objectsToFollow), sizeof(iVtObjectContainer_s), b_enableReplaceOfCmd);
}

bool
vtObjectContainer_c::setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t x, int16_t y, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildPosition (apc_childObject, x, y, b_updateObject, vtObjectContainer_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectContainer_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectContainer_a(), objectsToFollow), sizeof(iVtObjectContainer_s), b_enableReplaceOfCmd);
}

void
vtObjectContainer_c::setOriginSKM(bool b_SKM)
{
  MACRO_localVars;
  if (b_SKM) {
    s_properties.flags |= FLAG_ORIGIN_SKM;
    for (int i=0; i<vtObjectContainer_a->numberOfObjectsToFollow; i++) {
      vtObjectContainer_a->objectsToFollow[i].vtObject->setOriginSKM (b_SKM);
    }
  }
}

void
vtObjectContainer_c::setOriginBTN(IsoAgLib::iVtObjectButton_c* p_btn)
{
  MACRO_localVars;
  if (p_btn) p_parentButtonObject = p_btn;
  for (int i=0; i<vtObjectContainer_a->numberOfObjectsToFollow; i++) {
    vtObjectContainer_a->objectsToFollow[i].vtObject->setOriginBTN (p_btn);
  }
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
/** these attributes are in parentheses in the spec, so commented out here
uint16_t
vtObjectContainer_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectContainer_a(), width), sizeof(iVtObjectContainer_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectContainer_a(), width), sizeof(iVtObjectContainer_s));
}

uint16_t
vtObjectContainer_c::updateHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectContainer_a(), height), sizeof(iVtObjectContainer_s), 2);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectContainer_a(), height), sizeof(iVtObjectContainer_s));
}

uint8_t
vtObjectContainer_c::updateHidden(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectContainer_a(), hidden), sizeof(iVtObjectContainer_s), 3);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectContainer_a(), hidden), sizeof(iVtObjectContainer_s));
}
*/

void
vtObjectContainer_c::saveReceivedAttribute(uint8_t attrID, uint8_t* /*pui8_attributeValue*/)
{
  switch (attrID)
  {
    /** these attributes are in parentheses in the spec, so commented out here
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectContainer_a(), width), sizeof(iVtObjectContainer_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectContainer_a(), height), sizeof(iVtObjectContainer_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue8(MACRO_getStructOffset(get_vtObjectContainer_a(), hidden), sizeof(iVtObjectContainer_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    */
    default: break;
  }
}
#endif
} // __IsoAgLib
#endif
