/*
  vtobjectgraphicscontext_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTGRAPHICSCONTEXT_C_H
#define VTOBJECTGRAPHICSCONTEXT_C_H

#include <IsoAgLib/isoaglib_config.h>

#include "vtobject_c.h"
#include "vtclient_c.h"

namespace __IsoAgLib {
//! VT client object for graphics context.
class vtObjectGraphicsContext_c : public vtObject_c
{
public:
  //! ISO related IDs
  enum ID_t {
     //! Object type ID as defined by ISO App.B
     e_objectType               = 36
     //! Command ID as defined by ISO App.F
    ,e_commandID                = 0xB8  // dec. 184
     //! Graphics context sub command ID as in ISO App.F
    ,e_setGraphicsCursorCmdID   = 0
    ,e_setForegroundColourCmdID = 1
    ,e_setBackgroundColourCmdID = 2
    ,e_setLineAttributeCmdID    = 3
    ,e_setFillAttributeCmdID    = 4
    ,e_setFontAttributeCmdID    = 5
    ,e_eraseRectangleCmdID      = 6
    ,e_drawPointCmdID           = 7
    ,e_drawLineCmdID            = 8
    ,e_drawRectangleCmdID       = 9
    ,e_drawClosedEllipseCmdID   = 10
    ,e_drawPolygonCmdID         = 11
    ,e_drawTextCmdID            = 12
    ,e_panViewportCmdID         = 13
    ,e_zoomViewportCmdID        = 14
    ,e_panAndZoomViewportCmdID  = 15
    ,e_changeViewportSizeCmdID  = 16
    ,e_drawVTObjectCmdID        = 17
    ,e_copyCanvasToPictureGraphicCmdID   = 18
    ,e_copyViewportToPictureGraphicCmdID = 19
  };

  //! @param destMemory
  //! @param maxBytes Maximum bytes that can be streamed out without upload-buffer overrun.
  //! @param sourceOffset
  int16_t stream(uint8_t* destMemory, uint16_t maxBytes, objRange_t sourceOffset);

  /// Operation: init
  /// @param vtObjectGraphicsContextSROM
  /// @param b_initPointer
  void init(const iVtObjectGraphicsContext_s* vtObjectGraphicsContextSROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA) {
    vtObject_c::init ((iVtObject_s*) vtObjectGraphicsContextSROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);
  }

  //  Operation: get_vtObjectGraphicsContext_a
  inline iVtObjectGraphicsContext_s* get_vtObjectGraphicsContext_a( void ) {
    return (iVtObjectGraphicsContext_s *)&(get_vtObject_a());
  }

  //  Operation: vtObjectGraphicsContext_c
  vtObjectGraphicsContext_c( void );

  //! Give total size of object including header and attributes.
  uint32_t fitTerminal( void ) const { return mi_totalSize; }

  // //////////////////////////////////
  // All special Attribute-Set methods
  void setGraphicsCursor(int16_t ai16_x, int16_t ai16_y,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void setForegroundColour(uint8_t newValue,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void setBackgroundColour(uint8_t newValue,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void setLineAttributes(const IsoAgLib::iVtObjectLineAttributes_c* const newLineAttributes,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void setFillAttributes(const IsoAgLib::iVtObjectFillAttributes_c* const newFillAttributes,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void setFontAttributes(const IsoAgLib::iVtObjectFontAttributes_c* const newFontAttributes,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void eraseRectangle(int16_t ai16_x, int16_t ai16_y,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void drawPoint(
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void drawLine(int16_t ai16_x, int16_t ai16_y,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void drawRectangle(int16_t ai16_x, int16_t ai16_y,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void drawClosedEllipse(int16_t ai16_x, int16_t ai16_y,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void drawPolygon(uint16_t cnt, const int16_t* api16_x, const int16_t* api16_y,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void drawText(uint8_t type, uint8_t cnt, const char* apc_string,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void panViewport(int16_t ai16_x, int16_t ai16_y,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void zoomViewport(int8_t newValue,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void panAndZoomViewport(int16_t ai16_x, int16_t ai16_y, int8_t newValue,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void changeViewportSize(uint16_t newWidth, uint16_t newHeight,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void drawVtObject(const IsoAgLib::iVtObject_c* const p_VtObject,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void copyCanvas2PictureGraphic(const IsoAgLib::iVtObjectPictureGraphic_c* const pc_VtObjectPictureGraphic,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

  void copyViewport2PictureGraphic(const IsoAgLib::iVtObjectPictureGraphic_c* const pc_VtObjectPictureGraphic,
       bool b_updateObject=false, bool b_enableReplaceOfCmd=false);

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  // ///////////////////////// getter for attributes
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 36; }
  */

  uint16_t updateViewportWidth(bool b_SendRequest=false);

  uint16_t updateViewportHeight(bool b_SendRequest=false);

  uint16_t updateViewportXPos(bool b_SendRequest=false);

  uint16_t updateViewportYPos(bool b_SendRequest=false);

  uint8_t updateViewPortZoom(bool b_SendRequest=false);

  uint16_t updateCursorXPos(bool b_SendRequest=false);

  uint16_t updateCursorYPos(bool b_SendRequest=false);

  uint8_t updateForegroundColour(bool b_SendRequest=false);

  uint8_t updateBackgroundColour(bool b_SendRequest=false);

  uint16_t updateFontAttributes(bool b_SendRequest=false);

  uint16_t updateLineAttributes(bool b_SendRequest=false);

  uint16_t updateFillAttributes(bool b_SendRequest=false);

  uint8_t updateFormat(bool b_SendRequest=false);

  uint8_t updateOptions(bool b_SendRequest=false);

  uint8_t updateTransparencyColour(bool b_SendRequest=false);

  void saveReceivedAttribute (uint8_t attrID, uint8_t* pui8_attributeValue);
#endif
private:
  //! Total size of Graphics Context attributes.
  static const unsigned mi_attributesSize = (
    sizeof(uint16_t) +
    sizeof(uint16_t) +
    sizeof(int16_t)*2 +
    sizeof(uint16_t) +
    sizeof(uint16_t) +
    sizeof(int8_t) +
    sizeof(int16_t)*2 +
    sizeof(uint8_t) +
    sizeof(uint8_t) +
    sizeof(uint16_t) +
    sizeof(uint16_t) +
    sizeof(uint16_t) +
    sizeof(uint8_t) +
    sizeof(uint8_t) +
    sizeof(uint8_t) );

  //! Total size of Graphics Context object including header
  static const unsigned mi_totalSize = 3+mi_attributesSize;
};

} // __IsoAgLib

#endif
