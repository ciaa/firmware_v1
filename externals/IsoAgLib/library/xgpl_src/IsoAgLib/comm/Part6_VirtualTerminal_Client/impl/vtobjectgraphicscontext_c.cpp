/*
  vtobjectgraphicscontext_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectgraphicscontext_c.h"

#include "../ivtobjectpicturegraphic_c.h"
#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectlineattributes_c.h"
#include "../ivtobjectfillattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"
#include <IsoAgLib/util/convert.h>
#include <IsoAgLib/util/iassert.h>

// Make sure not such macro is used.
#undef MACRO_vtObjectTypeA
#undef MACRO_vtObjectTypeS


namespace __IsoAgLib {

int16_t
vtObjectGraphicsContext_c::stream(uint8_t* destMemory, uint16_t maxBytes, objRange_t sourceOffset)
{
  iVtObjectGraphicsContext_s* pc_vtOGC_a = get_vtObjectGraphicsContext_a();

  if (sourceOffset == 0) { // dump out constant sized stuff
    // Check precondition
    // (Not allways because we have no exception handling for 16-bit systems implemented).
    isoaglib_assert(maxBytes >= mi_totalSize);
    (void)maxBytes;

    uint8_t* p = destMemory;
    number2LittleEndianString( uint16_t(vtObject_a->ID), p ); p += sizeof(uint16_t);
    number2LittleEndianString( uint8_t(e_objectType), p ); p += sizeof(uint8_t);
    number2LittleEndianString( uint16_t(pc_vtOGC_a->viewportWidth), p ); p += sizeof(uint16_t);
    number2LittleEndianString( uint16_t(pc_vtOGC_a->viewportHeight), p ); p += sizeof(uint16_t);
    number2LittleEndianString( uint16_t(pc_vtOGC_a->viewportX), p ); p += sizeof(uint16_t);
    number2LittleEndianString( uint16_t(pc_vtOGC_a->viewportY), p ); p += sizeof(uint16_t);
    number2LittleEndianString( uint16_t(pc_vtOGC_a->canvasWidth), p ); p += sizeof(uint16_t);
    number2LittleEndianString( uint16_t(pc_vtOGC_a->canvasHeight), p ); p += sizeof(uint16_t);
    number2LittleEndianString( uint8_t(pc_vtOGC_a->viewportZoom), p ); p += sizeof(uint8_t);

    number2LittleEndianString( uint16_t(pc_vtOGC_a->cursorX), p ); p += sizeof(uint16_t);
    number2LittleEndianString( uint16_t(pc_vtOGC_a->cursorY), p ); p += sizeof(uint16_t);
    *(p++) = getVtClientInstance4Comm().getClientByID(s_properties.clientId).getUserClippedColor(
                                    pc_vtOGC_a->foregroundColour, this, IsoAgLib::Colour);
    *(p++) = getVtClientInstance4Comm().getClientByID(s_properties.clientId).getUserClippedColor(
                                    pc_vtOGC_a->backgroundColour, this, IsoAgLib::BackgroundColour);

    if (pc_vtOGC_a->fontAttributes)
      number2LittleEndianString( uint16_t(pc_vtOGC_a->fontAttributes->getID()), p );
    else
      number2LittleEndianString( uint16_t(0xFFFF), p );
    p += sizeof(uint16_t);

    if (pc_vtOGC_a->lineAttributes)
      number2LittleEndianString( uint16_t(pc_vtOGC_a->lineAttributes->getID()), p );
    else
      number2LittleEndianString( uint16_t(0xFFFF), p );
    p += sizeof(uint16_t);

    if (pc_vtOGC_a->fillAttributes)
      number2LittleEndianString( uint16_t(pc_vtOGC_a->fillAttributes->getID()), p );
    else
      number2LittleEndianString( uint16_t(0xFFFF), p );
    p += sizeof(uint16_t);

    number2LittleEndianString( uint8_t(pc_vtOGC_a->format), p ); p += sizeof(uint8_t);
    number2LittleEndianString( uint8_t(pc_vtOGC_a->options), p ); p += sizeof(uint8_t);
    *(p++) = getVtClientInstance4Comm().getClientByID(s_properties.clientId).getUserClippedColor(
                                    pc_vtOGC_a->transparencyColour, this, IsoAgLib::TransparencyColour );

    // Check postcondition
    isoaglib_assert((destMemory + mi_totalSize) == p);

    return mi_totalSize;
  }
  return 0;
}

vtObjectGraphicsContext_c::vtObjectGraphicsContext_c() {}

void
vtObjectGraphicsContext_c::setGraphicsCursor( int16_t ai16_x, int16_t ai16_y,
                                              bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorX),
      sizeof(iVtObjectGraphicsContext_s), ai16_x );
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorY),
      sizeof(iVtObjectGraphicsContext_s), ai16_y );
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandSetGraphicsCursor(
              this, ai16_x, ai16_y, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::setForegroundColour( uint8_t newValue,
                                                bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue8 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), foregroundColour),
      sizeof(iVtObjectGraphicsContext_s), newValue);
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandSetForegroundColour(
              this, newValue, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::setBackgroundColour( uint8_t newValue,
                                                bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue8 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), backgroundColour),
      sizeof(iVtObjectGraphicsContext_s), newValue);
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandSetBackgroundColour(
              this, newValue, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::setLineAttributes( const IsoAgLib::iVtObjectLineAttributes_c* const newLineAttributes,
                                              bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValueP (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), lineAttributes),
      sizeof(iVtObjectGraphicsContext_s), newLineAttributes);
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandSetGCLineAttributes(
              this, newLineAttributes, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::setFillAttributes( const IsoAgLib::iVtObjectFillAttributes_c* const newFillAttributes,
                                              bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValueP (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), fillAttributes),
      sizeof(iVtObjectGraphicsContext_s), newFillAttributes);
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandSetGCFillAttributes(
              this, newFillAttributes, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::setFontAttributes( const IsoAgLib::iVtObjectFontAttributes_c* const newFontAttributes,
                                              bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValueP (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), fontAttributes),
      sizeof(iVtObjectGraphicsContext_s), newFontAttributes);
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandSetGCFontAttributes(
              this, newFontAttributes, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::eraseRectangle( int16_t ai16_x, int16_t ai16_y,
                                           bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorX),
      sizeof(iVtObjectGraphicsContext_s), ai16_x );
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorY),
      sizeof(iVtObjectGraphicsContext_s), ai16_y );
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandEraseRectangle(
              this, ai16_x, ai16_y, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::drawPoint( bool /*b_updateObject*/, bool b_enableReplaceOfCmd)
{
  // No change of object => b_updateObject ignored.

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandDrawPoint(
              this, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::drawLine( int16_t ai16_x, int16_t ai16_y,
                                     bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorX),
      sizeof(iVtObjectGraphicsContext_s), ai16_x );
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorY),
      sizeof(iVtObjectGraphicsContext_s), ai16_y );
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandDrawLine(
              this, ai16_x, ai16_y, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::drawRectangle( int16_t ai16_x, int16_t ai16_y,
                                          bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorX),
      sizeof(iVtObjectGraphicsContext_s), ai16_x );
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorY),
      sizeof(iVtObjectGraphicsContext_s), ai16_y );
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandDrawRectangle(
              this, ai16_x, ai16_y, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::drawClosedEllipse( int16_t ai16_x, int16_t ai16_y,
                                              bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorX),
      sizeof(iVtObjectGraphicsContext_s), ai16_x );
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorY),
      sizeof(iVtObjectGraphicsContext_s), ai16_y );
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandDrawClosedEllipse(
              this, ai16_x, ai16_y, b_enableReplaceOfCmd);
}

//! You need to supply valid pointers for api16_x/api16_y - no checks are done in here...
void
vtObjectGraphicsContext_c::drawPolygon( uint16_t cnt, const int16_t* api16_x, const int16_t* api16_y,
                                        bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorX),
      sizeof(iVtObjectGraphicsContext_s), api16_x[cnt-1] );
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorY),
      sizeof(iVtObjectGraphicsContext_s), api16_y[cnt-1] );
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandDrawPolygon(
              this, cnt, api16_x, api16_y, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::drawText( uint8_t type, uint8_t cnt, const char* apc_string,
                                     bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    uint16_t ui16_widthHeight = get_vtObjectGraphicsContext_a()->fontAttributes->getScaledWidthHeight();
    uint16_t ui16_totalHeight = ui16_widthHeight & 0xFF;
    uint16_t ui16_totalWidth = uint16_t(ui16_widthHeight >> 8) * uint16_t(cnt);

    int16_t i16_x = get_vtObjectGraphicsContext_a()->cursorX - 1 + ui16_totalWidth;
    int16_t i16_y = get_vtObjectGraphicsContext_a()->cursorY - 1 + ui16_totalHeight;

    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorX),
      sizeof(iVtObjectGraphicsContext_s), i16_x );
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorY),
      sizeof(iVtObjectGraphicsContext_s), i16_y );
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandDrawText(
              this, type, cnt, apc_string, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::panViewport( int16_t ai16_x, int16_t ai16_y,
                                        bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportX),
      sizeof(iVtObjectGraphicsContext_s), ai16_x );
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportY),
      sizeof(iVtObjectGraphicsContext_s), ai16_y );
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandPanViewport(
              this, ai16_x, ai16_y, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::zoomViewport( int8_t newValue,
                                         bool b_updateObject, bool b_enableReplaceOfCmd)
{
  // Check precondition as defined in ISO
  if ((-32 > newValue) || (newValue > 32)) { return; }

  if (b_updateObject) {
    saveSignedValue8 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportZoom),
      sizeof(iVtObjectGraphicsContext_s), newValue);
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandZoomViewport(
              this, newValue, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::panAndZoomViewport( int16_t ai16_x, int16_t ai16_y, int8_t newValue,
                                               bool b_updateObject, bool b_enableReplaceOfCmd)
{
  // Check precondition as defined in ISO
  if ((-32 > newValue) || (newValue > 32)) { return; }

  if (b_updateObject) {
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportX),
      sizeof(iVtObjectGraphicsContext_s), ai16_x );
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportY),
      sizeof(iVtObjectGraphicsContext_s), ai16_y );
    saveSignedValue8 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportZoom),
      sizeof(iVtObjectGraphicsContext_s), newValue);
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandPanAndZoomViewport(
              this, ai16_x, ai16_y, newValue, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::changeViewportSize( uint16_t newWidth, uint16_t newHeight,
                                               bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportWidth),
      sizeof(iVtObjectGraphicsContext_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportHeight),
      sizeof(iVtObjectGraphicsContext_s), newHeight);
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandChangeViewportSize(
              this, newWidth, newHeight, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::drawVtObject( const iVtObject_c* const newVtObject,
                                         bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    //! @todo ON REQUEST Revision4 The object size is not know and thus the cursor is not moved for b_updateObject case.
    uint16_t ui16_totalHeight = 1;
    uint16_t ui16_totalWidth = 1;

    int16_t i16_x = get_vtObjectGraphicsContext_a()->cursorX - 1 + ui16_totalWidth;
    int16_t i16_y = get_vtObjectGraphicsContext_a()->cursorY - 1 + ui16_totalHeight;

    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorX),
      sizeof(iVtObjectGraphicsContext_s), i16_x );
    saveSignedValue16 (MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorY),
      sizeof(iVtObjectGraphicsContext_s), i16_y );
  }

  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandDrawVtObject(
              this, newVtObject, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::copyCanvas2PictureGraphic( const IsoAgLib::iVtObjectPictureGraphic_c* const pc_iVtObjectPictureGraphic,
                                                      bool /*b_updateObject*/, bool b_enableReplaceOfCmd)
{
  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandCopyCanvas2PictureGraphic(
              this, pc_iVtObjectPictureGraphic, b_enableReplaceOfCmd);
}

void
vtObjectGraphicsContext_c::copyViewport2PictureGraphic( const IsoAgLib::iVtObjectPictureGraphic_c* const pc_iVtObjectPictureGraphic,
                                                        bool /*b_updateObject*/, bool b_enableReplaceOfCmd)
{
  getVtClientInstance4Comm().getClientByID(s_properties.clientId).sendCommandCopyViewport2PictureGraphic(
              this, pc_iVtObjectPictureGraphic, b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectGraphicsContext_c::updateViewportWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportWidth), sizeof(iVtObjectGraphicsContext_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportWidth), sizeof(iVtObjectGraphicsContext_s));
}

uint16_t
vtObjectGraphicsContext_c::updateViewportHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportHeight), sizeof(iVtObjectGraphicsContext_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportHeight), sizeof(iVtObjectGraphicsContext_s));
}

uint16_t
vtObjectGraphicsContext_c::updateViewportXPos(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportX), sizeof(iVtObjectGraphicsContext_s), 3);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportX), sizeof(iVtObjectGraphicsContext_s));
}

uint16_t
vtObjectGraphicsContext_c::updateViewportYPos(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportY), sizeof(iVtObjectGraphicsContext_s), 4);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportY), sizeof(iVtObjectGraphicsContext_s));
}

uint8_t
vtObjectGraphicsContext_c::updateViewPortZoom(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportZoom), sizeof(iVtObjectGraphicsContext_s), 5);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportZoom), sizeof(iVtObjectGraphicsContext_s));
}

uint16_t
vtObjectGraphicsContext_c::updateCursorXPos(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorX), sizeof(iVtObjectGraphicsContext_s), 6);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorY), sizeof(iVtObjectGraphicsContext_s));
}

uint16_t
vtObjectGraphicsContext_c::updateCursorYPos(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportY), sizeof(iVtObjectGraphicsContext_s), 7);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportY), sizeof(iVtObjectGraphicsContext_s));
}

uint8_t vtObjectGraphicsContext_c::updateForegroundColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), foregroundColour), sizeof(iVtObjectGraphicsContext_s), 8);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), foregroundColour), sizeof(iVtObjectGraphicsContext_s));
}

uint8_t
vtObjectGraphicsContext_c::updateBackgroundColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), backgroundColour), sizeof(iVtObjectGraphicsContext_s), 9);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), backgroundColour), sizeof(iVtObjectGraphicsContext_s));
}

uint16_t
vtObjectGraphicsContext_c::updateFontAttributes(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), fontAttributes), sizeof(iVtObjectGraphicsContext_s), 10);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), fontAttributes), sizeof(iVtObjectGraphicsContext_s));
}

uint16_t
vtObjectGraphicsContext_c::updateLineAttributes(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), lineAttributes), sizeof(iVtObjectGraphicsContext_s), 11);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), lineAttributes), sizeof(iVtObjectGraphicsContext_s));
}

uint16_t
vtObjectGraphicsContext_c::updateFillAttributes(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), fillAttributes), sizeof(iVtObjectGraphicsContext_s), 12);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), fillAttributes), sizeof(iVtObjectGraphicsContext_s));
}

uint8_t
vtObjectGraphicsContext_c::updateFormat(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), format), sizeof(iVtObjectGraphicsContext_s), 13);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), format), sizeof(iVtObjectGraphicsContext_s));
}

uint8_t
vtObjectGraphicsContext_c::updateOptions(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), options), sizeof(iVtObjectGraphicsContext_s), 14);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), options), sizeof(iVtObjectGraphicsContext_s));
}

uint8_t
vtObjectGraphicsContext_c::updateTransparencyColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), transparencyColour), sizeof(iVtObjectGraphicsContext_s), 15);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), transparencyColour), sizeof(iVtObjectGraphicsContext_s));
}

void
vtObjectGraphicsContext_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportWidth), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportHeight), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportX), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 4: saveValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportY), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 5: saveValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportZoom), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 6: saveValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), cursorX), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 7: saveValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), viewportY), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 8: saveValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), foregroundColour), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 9: saveValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), backgroundColour), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 10: saveValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), fontAttributes), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 11: saveValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), lineAttributes), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 12: saveValue16(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), fillAttributes), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 13: saveValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), format), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 14: saveValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), options), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 15: saveValue8(MACRO_getStructOffset(get_vtObjectGraphicsContext_a(), transparencyColour), sizeof(iVtObjectGraphicsContext_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    default: break;
  }
}

#endif

} // __IsoAgLib
