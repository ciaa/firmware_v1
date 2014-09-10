/*
  commandhandler_c.cpp: class for sending all the commands.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "commandhandler_c.h"
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/canpkgext_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtobject_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtobjectstring_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtobjectpicturegraphic_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/impl/vtclientconnection_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/impl/vtserverinstance_c.h>
#include <supplementary_driver/driver/datastreams/volatilememory_c.h>


namespace __IsoAgLib {

SendUpload_c CommandHandler_c::msc_tempSendUpload;

/// The following timeouts are ISOAgLib-proprietary values.
/// In 11783-6 there are no real timeouts specified, yet
/// ISOAgLib has to react on the case of a non-responding VT
/// (either at Upload, Load or normal command)
#ifdef ENABLE_VTCLIENT_RETRY
static const int32_t s_timeOutNormalCommand   = 3000; // instead of 10000 due to 3 retries!!!
#else
static const int32_t s_timeOutNormalCommand   = 10000;
#endif
static const int32_t s_timeOutVersionLabel    = 60000;
static const int32_t s_timeOutEndOfObjectPool = 60000;

static const uint8_t scui8_cmdCompareTableMin = 0x92;
static const uint8_t scui8_cmdCompareTableMax = 0xBD;

/// this table is used to identify if a command can override an earlier command of same function
/// 1<<databyte to indicate which databytes to compare to decide if command is replaced or not
/// (1<<0) means DO NOT OVERRIDE THESE COMMANDS AT ALL
static const uint8_t scpui8_cmdCompareTable[(scui8_cmdCompareTableMax-scui8_cmdCompareTableMin)+1] = {
/* 0x92 */ (1<<0) , //NEVER OVERRIDE THIS COMMAND
/* 0x93 */ 0 , //invalid command
/* 0x94 */ 0 , //invalid command
/* 0x95 */ 0 , //invalid command
/* 0x96 */ 0 , //invalid command
/* 0x97 */ 0 , //invalid command
/* 0x98 */ 0 , //invalid command
/* 0x99 */ 0 , //invalid command
/* 0x9A */ 0 , //invalid command
/* 0x9B */ 0 , //invalid command
/* 0x9C */ 0 , //invalid command
/* 0x9D */ 0 , //invalid command
/* 0x9E */ 0 , //invalid command
/* 0x9F */ 0 , //invalid command
/* 0xA0 */ (1<<1) | (1<<2) ,
/* 0xA1 */ (1<<1) | (1<<2) ,
/* 0xA2 */ (1<<1) | (1<<2) ,
/* 0xA3 */ (1<<0) , //NEVER OVERRIDE THIS COMMAND (Control Audio Device)
/* 0xA4 */ (1<<0) , //NEVER OVERRIDE THIS COMMAND (Set Audio Volume)
/* 0xA5 */ (1<<0) , //NEVER OVERRIDE THIS COMMAND (Change Child Location), as it's relative!!!
/* 0xA6 */ (1<<1) | (1<<2) ,
/* 0xA7 */ (1<<1) | (1<<2) ,
/* 0xA8 */ (1<<1) | (1<<2) , // Change Numeric Value (all has been done for THIS ONE originally ;-)
/* 0xA9 */ (1<<1) | (1<<2) ,
/* 0xAA */ (1<<1) | (1<<2) ,
/* 0xAB */ (1<<1) | (1<<2) ,
/* 0xAC */ (1<<1) | (1<<2) ,
/* 0xAD */ (1<<0) , // changed. was "(1<<1) | (1<<2) ," before, but we shouldn't change earlier to a datamask as it's probably not setup correctly at this time.  // (Change Active Mask)
/* 0xAE */ (1<<1) | (1<<2) | (1<<3) ,
/* 0xAF */ (1<<1) | (1<<2) | (1<<3) ,
/* 0xB0 */ (1<<1) | (1<<2) ,
/* 0xB1 */ (1<<1) | (1<<2) | (1<<3) ,
/* 0xB2 */ (1<<0) , //NEVER OVERRIDE THIS COMMAND (Delete Object Pool)
/* 0xB3 */ (1<<1) | (1<<2) ,
/* 0xB4 */ (1<<1) | (1<<2) | (1<<3) | (1<<4), // (Change Child Position)
/* 0xB5 */ 0 , //invalid command
/* 0xB6 */ 0 , //invalid command
/* 0xB7 */ 0 , //invalid command
/* 0xB8 */ (1<<0) ,  //NEVER OVERRIDE THIS COMMAND (Graphics Context)
/* 0xB9 */ (1<<0) , // changed. was "(1<<1) | (1<<2) | (1<<3) | (1<<4)," before, but I guess no overriding should take place in request commands // (Get Attribute Value)
/* 0xBA */ 0, //invalid command
/* 0xBB */ 0, //invalid command
/* 0xBC */ 0, //invalid command
/* 0xBD */ (1<<0) //NEVER OVERRIDE THIS COMMAND (Lock/Unlock Mask)
};


bool
CommandHandler_c::sendCommand (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, uint8_t byte9, bool b_enableReplaceOfCmd)
{
  msc_tempSendUpload.set (byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9);
  return queueOrReplace (msc_tempSendUpload, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommand (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, bool b_enableReplaceOfCmd, IsoAgLib::iVtObject_c** rppc_vtObjects, uint16_t aui16_numObjects)
{
  msc_tempSendUpload.set (byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, rppc_vtObjects, aui16_numObjects);
  return queueOrReplace (msc_tempSendUpload, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommand (uint8_t* apui8_buffer, uint32_t ui32_size)
{
  msc_tempSendUpload.set (apui8_buffer, ui32_size);
  return queueOrReplace (msc_tempSendUpload, false);
}


bool
CommandHandler_c::sendCommandChangeStringValue (IsoAgLib::iVtObjectString_c* apc_objectString, bool b_enableReplaceOfCmd)
{
  msc_tempSendUpload.set (apc_objectString);
  return queueOrReplace (msc_tempSendUpload, b_enableReplaceOfCmd);
}


#if 0
// currently not used because it's done via Change Attribute command
bool
CommandHandler_c::sendCommandChangePriority (IsoAgLib::iVtObject_c* apc_object, int8_t newPriority, bool b_enableReplaceOfCmd)
{
  isoaglib_assert( newPriority < 3 );
  return sendCommand (176 /* Command: Command --- Parameter: Change Priority */,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      newPriority, 0xFF, 0xFF, 0xFF, 0xFF, b_enableReplaceOfCmd);
}
#endif 


bool
CommandHandler_c::sendCommandChangeEndPoint (IsoAgLib::iVtObject_c* apc_object,uint16_t newWidth, uint16_t newHeight, uint8_t newLineAttributes, bool b_enableReplaceOfCmd)
{
  return sendCommand (169 /* Command: Command --- Parameter: Change Size */,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      newWidth & 0xFF, newWidth >> 8,
                      newHeight & 0xFF, newHeight >> 8,
                      newLineAttributes, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandControlAudioDevice (uint8_t aui8_repetitions, uint16_t aui16_frequency, uint16_t aui16_onTime, uint16_t aui16_offTime)
{
  return sendCommand (163 /* Command: Command --- Parameter: Control Audio Device */,
                      aui8_repetitions,
                      aui16_frequency & 0xFF, aui16_frequency >> 8,
                      aui16_onTime & 0xFF, aui16_onTime >> 8,
                      aui16_offTime & 0xFF, aui16_offTime >> 8,
                      false); // don't care for enable-same command stuff
}


bool
CommandHandler_c::sendCommandSetAudioVolume (uint8_t aui8_volume)
{
  return sendCommand (164 /* Command: Command --- Parameter: Set Audio Volume */,
                      aui8_volume, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, false); // don't care for enableReplaceOfCommand parameter actually
}


bool
CommandHandler_c::sendCommandDeleteObjectPool()
{
  return sendCommand (178 /* Command: Command --- Parameter: Delete Object Pool */,
                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, true); // don't care for enableReplaceOfCommand parameter actually
}


bool
CommandHandler_c::sendCommandUpdateLanguagePool()
{
  /// Enqueue a fake command which will trigger the language object pool update to be multi-sent out. using 0x11 here, as this is the command then and won't be used
  return sendCommand (0x11 /* Command: Object Pool Transfer --- Parameter: Object Pool Transfer */,
                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, false) // replaces COULD happen if user-triggered sequences are there.
      && sendCommand (0x12 /* Command: Object Pool Transfer --- Parameter: Object Pool Ready */,
                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, false); // replaces COULD happen if user-triggered sequences are there.
}


bool
CommandHandler_c::sendCommandUpdateObjectPool (IsoAgLib::iVtObject_c** rppc_vtObjects, uint16_t aui16_numObjects)
{
  /// Enqueue a fake command which will trigger the language object pool update to be multi-sent out. using 0x11 here, as this is the command then and won't be used
  return sendCommand (0x11 /* Command: Object Pool Transfer --- Parameter: Object Pool Transfer */,
                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, false, rppc_vtObjects, aui16_numObjects) // replaces COULD happen if user-triggered sequences are there.
      && sendCommand (0x12 /* Command: Object Pool Transfer --- Parameter: Object Pool Ready */,
                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, false); // replaces COULD happen if user-triggered sequences are there.
}


bool
CommandHandler_c::sendCommandChangeNumericValue (uint16_t aui16_objectUid, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd)
{
  return sendCommand (168 /* Command: Command --- Parameter: Change Numeric Value */,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8,
                      0xFF, byte1, byte2, byte3, byte4, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandChangeAttribute (uint16_t aui16_objectUid, uint8_t attrId, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd)
{
  return sendCommand (175 /* Command: Command --- Parameter: Change Attribute */,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8,
                      attrId, byte1, byte2, byte3, byte4, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandChangeSoftKeyMask (uint16_t aui16_objectUid, uint8_t maskType, uint16_t newSoftKeyMask, bool b_enableReplaceOfCmd)
{
  return sendCommand (174 /* Command: Command --- Parameter: Change Soft Key Mask */,
                      maskType,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8,
                      newSoftKeyMask & 0xFF, newSoftKeyMask >> 8,
                      0xFF, 0xFF, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandChangeActiveMask (uint16_t aui16_objectUid, uint16_t maskId, bool b_enableReplaceOfCmd )
{
  return sendCommand (173 /* Command: Command --- Parameter: Change Active Mask */,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8,
                      maskId & 0xFF, maskId >> 8, 0xFF, 0xFF, 0xFF, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandChangeStringValue (uint16_t aui16_objectUid, const char* apc_newValue, uint16_t overrideSendLength, bool b_enableReplaceOfCmd)
{
  msc_tempSendUpload.set (aui16_objectUid, apc_newValue, overrideSendLength);
  return queueOrReplace (msc_tempSendUpload, b_enableReplaceOfCmd);
}


#ifdef USE_VT_UNICODE_SUPPORT
bool
CommandHandler_c::sendCommandChangeStringValueUTF16 (uint16_t aui16_objectUid, const char* apc_newValue, uint16_t overrideSendLength, bool b_enableReplaceOfCmd)
{
  msc_tempSendUpload.set (aui16_objectUid, apc_newValue, overrideSendLength, true);
  return queueOrReplace (msc_tempSendUpload, b_enableReplaceOfCmd);
}
#endif


bool
CommandHandler_c::sendCommandChangeChildPosition (uint16_t aui16_objectUid, uint16_t aui16_childObjectUid, int16_t x, int16_t y, bool b_enableReplaceOfCmd)
{
  return sendCommand (180 /* Command: Command --- Parameter: Change Child Position */,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8,
                      aui16_childObjectUid & 0xFF, aui16_childObjectUid >> 8,
                      x & 0xFF, x >> 8, y & 0xFF, y >> 8, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandChangeChildLocation (uint16_t aui16_objectUid, uint16_t aui16_childObjectUid, int16_t dx, int16_t dy, bool b_enableReplaceOfCmd)
{
  return sendCommand (165 /* Command: Command --- Parameter: Change Child Location */,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8,
                      aui16_childObjectUid & 0xFF, aui16_childObjectUid >> 8,
                      dx+127, dy+127, 0xFF, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandChangeBackgroundColour (uint16_t aui16_objectUid, uint8_t newColour, bool b_enableReplaceOfCmd)
{
  return sendCommand (167 /* Command: Command --- Parameter: Change Background Color */,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8,
                      newColour, 0xFF, 0xFF, 0xFF, 0xFF, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandChangeSize (uint16_t aui16_objectUid,uint16_t newWidth, uint16_t newHeight, bool b_enableReplaceOfCmd)
{
  return sendCommand (166 /* Command: Command --- Parameter: Change Size */,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8,
                      newWidth & 0xFF, newWidth >> 8,
                      newHeight & 0xFF, newHeight >> 8,
                      0xFF, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandChangeFillAttributes (uint16_t aui16_objectUid, uint8_t newFillType, uint8_t newFillColour, IsoAgLib::iVtObjectPictureGraphic_c* newFillPatternObject, bool b_enableReplaceOfCmd)
{
  return sendCommand (172 /* Command: Command --- Parameter: Change FillAttributes */,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8,
                      newFillType, newFillColour,
                      (newFillType == 3) ? newFillPatternObject->getID() & 0xFF : 0xFF,
                      (newFillType == 3) ? newFillPatternObject->getID() >> 8 : 0xFF,
                      0xFF, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandChangeFontAttributes (uint16_t aui16_objectUid, uint8_t newFontColour, uint8_t newFontSize, uint8_t newFontType, uint8_t newFontStyle, bool b_enableReplaceOfCmd)
{
  return sendCommand (170 /* Command: Command --- Parameter: Change FontAttributes */,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8,
                      newFontColour, newFontSize, newFontType, newFontStyle, 0xFF,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandChangeLineAttributes (uint16_t aui16_objectUid, uint8_t newLineColour, uint8_t newLineWidth, uint16_t newLineArt, bool b_enableReplaceOfCmd)
{
  return sendCommand (171 /* Command: Command --- Parameter: Change LineAttributes */,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8,
                      newLineColour, newLineWidth, newLineArt & 0xFF, newLineArt >> 8, 0xFF,
                      b_enableReplaceOfCmd);
}


#ifdef USE_ISO_TERMINAL_GRAPHICCONTEXT
bool
CommandHandler_c::sendCommandSetGraphicsCursor(
  IsoAgLib::iVtObject_c* apc_object, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd)
{
  uint16_t x=convert_n::castUI( ai16_x );
  uint16_t y=convert_n::castUI( ai16_y );
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_setGraphicsCursorCmdID,
                      x & 0xFF, x >> 8, y & 0xFF, y >> 8,
                      b_enableReplaceOfCmd );
}


bool
CommandHandler_c::sendCommandSetForegroundColour(
  IsoAgLib::iVtObject_c* apc_object, uint8_t newValue, bool b_enableReplaceOfCmd)
{
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_setForegroundColourCmdID,
                      newValue, 0xFF, 0xFF, 0xFF,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandSetBackgroundColour(
  IsoAgLib::iVtObject_c* apc_object, uint8_t newValue, bool b_enableReplaceOfCmd)
{
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_setBackgroundColourCmdID,
                      newValue, 0xFF, 0xFF, 0xFF,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandSetGCLineAttributes(
  IsoAgLib::iVtObject_c* apc_object, const IsoAgLib::iVtObjectLineAttributes_c* const newLineAttributes, bool b_enableReplaceOfCmd)
{
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_setLineAttributeCmdID,
                      newLineAttributes->getID() & 0xFF, newLineAttributes->getID() >> 8, 0xFF, 0xFF,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandSetGCFillAttributes(
  IsoAgLib::iVtObject_c* apc_object, const IsoAgLib::iVtObjectFillAttributes_c* const newFillAttributes, bool b_enableReplaceOfCmd)
{
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_setFillAttributeCmdID,
                      newFillAttributes->getID() & 0xFF, newFillAttributes->getID() >> 8, 0xFF, 0xFF,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandSetGCFontAttributes(
  IsoAgLib::iVtObject_c* apc_object, const IsoAgLib::iVtObjectFontAttributes_c* const newFontAttributes, bool b_enableReplaceOfCmd)
{
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_setFontAttributeCmdID,
                      newFontAttributes->getID() & 0xFF, newFontAttributes->getID() >> 8, 0xFF, 0xFF,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandEraseRectangle(
  IsoAgLib::iVtObject_c* apc_object, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd)
{
  uint16_t x=convert_n::castUI( ai16_x );
  uint16_t y=convert_n::castUI( ai16_y );
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_eraseRectangleCmdID,
                      x & 0xFF, x >> 8, y & 0xFF, y >> 8,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandDrawPoint(
  IsoAgLib::iVtObject_c* apc_object, bool  b_enableReplaceOfCmd)
{
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_drawPointCmdID,
                      0xFF, 0xFF, 0xFF, 0xFF,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandDrawLine(
  IsoAgLib::iVtObject_c* apc_object, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd)
{
  uint16_t x=convert_n::castUI( ai16_x );
  uint16_t y=convert_n::castUI( ai16_y );
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_drawLineCmdID,
                      x & 0xFF, x >> 8, y & 0xFF, y >> 8,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandDrawRectangle(
  IsoAgLib::iVtObject_c* apc_object, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd)
{
  uint16_t x=convert_n::castUI( ai16_x );
  uint16_t y=convert_n::castUI( ai16_y );
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_drawRectangleCmdID,
                      x & 0xFF, x >> 8, y & 0xFF, y >> 8,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandDrawClosedEllipse(
  IsoAgLib::iVtObject_c* apc_object, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd)
{
  uint16_t x=convert_n::castUI( ai16_x );
  uint16_t y=convert_n::castUI( ai16_y );
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_drawClosedEllipseCmdID,
                      x & 0xFF, x >> 8, y & 0xFF, y >> 8,
                      b_enableReplaceOfCmd);
}


/// @todo OPTIMIZATION Revision4 Better struct for array of x/y pairs!
bool
CommandHandler_c::sendCommandDrawPolygon(
  IsoAgLib::iVtObject_c* apc_object, uint16_t ui16_numOfPoints, const int16_t* api16_x, const int16_t* api16_y, bool b_enableReplaceOfCmd)
{
  // Prevent from derefernzing NULL pointer.
  if ((0 == api16_x) || (0 == api16_y)) { ui16_numOfPoints = 0; }

  // Check if valid polgon (at least one point)
  if (0 == ui16_numOfPoints) { return false; }

  // Trivial case (like draw line) without TP.
  if (ui16_numOfPoints == 1) {
    uint16_t x = convert_n::castUI( *api16_x );
    uint16_t y = convert_n::castUI( *api16_y );
    return sendCommand( vtObjectGraphicsContext_c::e_commandID,
                        apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                        vtObjectGraphicsContext_c::e_drawPolygonCmdID,
                        x & 0xFF, x >> 8, y & 0xFF, y >> 8,
                        b_enableReplaceOfCmd );
  }

  // send a polygon with more than one point
  uint16_t ui16_bufferSize = 4+(ui16_numOfPoints*4);
  uint8_t *pui8_buffer = new uint8_t[ ui16_bufferSize ];
  pui8_buffer[0] = vtObjectGraphicsContext_c::e_commandID;
  pui8_buffer[1] = apc_object->getID() & 0xFF;
  pui8_buffer[2] = apc_object->getID() >> 8;
  pui8_buffer[3] = vtObjectGraphicsContext_c::e_drawPolygonCmdID;

  // add all points from the list to the buffer
  uint16_t ui16_index = 4;		/* first 4 bytes are set */
  for ( uint16_t ui16_currentPoint = 0;
        ui16_currentPoint < ui16_numOfPoints;
        ui16_currentPoint++ )
  {
    uint16_t x = convert_n::castUI( api16_x[ui16_currentPoint] );
    pui8_buffer[ui16_index]   = x & 0xFF;
    pui8_buffer[ui16_index+1] = x >> 8;
    uint16_t y = convert_n::castUI( api16_y[ui16_currentPoint] );
    pui8_buffer[ui16_index+2] = y & 0xFF;
    pui8_buffer[ui16_index+3] = y >> 8;
    ui16_index+=4;
  }

  // Send buffer as ISOBUS command.
  msc_tempSendUpload.set (pui8_buffer, ui16_bufferSize );
  delete[] pui8_buffer;
  return queueOrReplace (msc_tempSendUpload, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandDrawText(
  IsoAgLib::iVtObject_c* apc_object, uint8_t ui8_textType, uint8_t ui8_numOfCharacters, const char *apc_newValue, bool b_enableReplaceOfCmd)
{
  // Non TP case
  if (ui8_numOfCharacters <= 2) {
    uint8_t a = (ui8_numOfCharacters >= 1) ? apc_newValue[0] : 0xFF;
    uint8_t b = (ui8_numOfCharacters >= 2) ? apc_newValue[1] : 0xFF;
    return sendCommand( vtObjectGraphicsContext_c::e_commandID,
                        apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                        vtObjectGraphicsContext_c::e_drawTextCmdID,
                        ui8_textType, ui8_numOfCharacters, a, b,
                        b_enableReplaceOfCmd );
  }

  uint8_t *pui8_buffer = new uint8_t[6+ui8_numOfCharacters];
  pui8_buffer[0] = vtObjectGraphicsContext_c::e_commandID;
  pui8_buffer[1] = apc_object->getID() & 0xFF;
  pui8_buffer[2] = apc_object->getID() >> 8;
  pui8_buffer[3] = vtObjectGraphicsContext_c::e_drawTextCmdID;
  pui8_buffer[4] = ui8_textType;
  pui8_buffer[5] = ui8_numOfCharacters;

  for (uint8_t ui8_index = 0; ui8_index < ui8_numOfCharacters; ui8_index++)
    pui8_buffer[6+ui8_index] = apc_newValue [ui8_index];

  msc_tempSendUpload.set (pui8_buffer, (6+ui8_numOfCharacters));
  delete[] pui8_buffer;

  return queueOrReplace (msc_tempSendUpload, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandPanViewport(
  IsoAgLib::iVtObject_c* apc_object, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd)
{
  uint16_t x = convert_n::castUI( ai16_x );
  uint16_t y = convert_n::castUI( ai16_y );
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_panViewportCmdID,
                      x & 0xFF, x >> 8, y & 0xFF, y >> 8,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandZoomViewport(
  IsoAgLib::iVtObject_c* apc_object, int8_t newValue, bool b_enableReplaceOfCmd)
{
  uint8_t zoom = convert_n::castUI( newValue );
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_zoomViewportCmdID,
                      zoom, 0xFF, 0xFF, 0xFF,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandPanAndZoomViewport(
  IsoAgLib::iVtObject_c* apc_object, int16_t ai16_x, int16_t ai16_y, int8_t newValue, bool b_enableReplaceOfCmd)
{
  uint16_t x = convert_n::castUI( ai16_x );
  uint16_t y = convert_n::castUI( ai16_y );
  uint8_t zoom = convert_n::castUI( newValue );
  uint8_t pui8_buffer[9];
  pui8_buffer[0] = vtObjectGraphicsContext_c::e_commandID;
  pui8_buffer[1] = apc_object->getID() & 0xFF;
  pui8_buffer[2] = apc_object->getID() >> 8;
  pui8_buffer[3] = vtObjectGraphicsContext_c::e_panAndZoomViewportCmdID;
  pui8_buffer[4] = x & 0xFF;
  pui8_buffer[5] = x >> 8;
  pui8_buffer[6] = y & 0xFF;
  pui8_buffer[7] = y >> 8;
  pui8_buffer[8] = zoom;

  // Send buffer as ISOBUS command.
  msc_tempSendUpload.set( pui8_buffer, sizeof(pui8_buffer)/sizeof(*pui8_buffer) );
  return queueOrReplace (msc_tempSendUpload, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandChangeViewportSize(
IsoAgLib::iVtObject_c* apc_object, uint16_t newWidth, uint16_t newHeight, bool b_enableReplaceOfCmd)
{
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_changeViewportSizeCmdID,
                      newWidth & 0xFF, newWidth >> 8, newHeight & 0xFF, newHeight >> 8,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandDrawVtObject(
  IsoAgLib::iVtObject_c* apc_object, const IsoAgLib::iVtObject_c* const pc_VtObject, bool b_enableReplaceOfCmd)
{
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_drawVTObjectCmdID,
                      pc_VtObject->getID() & 0xFF, pc_VtObject->getID() >> 8, 0xFF, 0xFF,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandCopyCanvas2PictureGraphic(
  IsoAgLib::iVtObject_c* apc_object, const IsoAgLib::iVtObjectPictureGraphic_c* const pc_VtObject, bool b_enableReplaceOfCmd)
{
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_copyCanvasToPictureGraphicCmdID,
                      pc_VtObject->getID() & 0xFF, pc_VtObject->getID() >> 8, 0xFF, 0xFF,
                      b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandCopyViewport2PictureGraphic(
  IsoAgLib::iVtObject_c* apc_object, const IsoAgLib::iVtObjectPictureGraphic_c* const pc_VtObject, bool b_enableReplaceOfCmd)
{
  return sendCommand (vtObjectGraphicsContext_c::e_commandID,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      vtObjectGraphicsContext_c::e_copyViewportToPictureGraphicCmdID,
                      pc_VtObject->getID() & 0xFF, pc_VtObject->getID() >> 8, 0xFF, 0xFF,
                      b_enableReplaceOfCmd);
}
// ########## END Graphics Context ##########
#endif


#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
bool
CommandHandler_c::sendCommandGetAttributeValue( IsoAgLib::iVtObject_c* apc_object, const uint8_t cui8_attrID, bool b_enableReplaceOfCmd)
{
  return sendCommand (185 /* Command: Get Technical Data --- Parameter: Get Attribute Value */,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8,
                      cui8_attrID, 0xFF, 0xFF, 0xFF, 0xFF, b_enableReplaceOfCmd);
}
#endif


bool
CommandHandler_c::sendCommandLockUnlockMask( IsoAgLib::iVtObject_c* apc_object, bool b_lockMask, uint16_t ui16_lockTimeOut, bool b_enableReplaceOfCmd)
{
  return sendCommand (189 /* Command: Command --- Parameter: Lock/Undlock Mask */,
                      b_lockMask,
                      apc_object->getID() & 0xFF, apc_object->getID() >> 8, /* object id of the data mask to lock */
                      ui16_lockTimeOut & 0xFF, ui16_lockTimeOut >> 8, /* lock timeout on ms or zero for no timeout */
                      0xFF, 0xFF, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandHideShow (uint16_t aui16_objectUid, uint8_t b_hideOrShow, bool b_enableReplaceOfCmd)
{
  return sendCommand (160 /* Command: Command --- Parameter: Hide/Show Object */,
                      aui16_objectUid & 0xFF, aui16_objectUid >> 8, b_hideOrShow,
                      0xFF, 0xFF, 0xFF, 0xFF, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendCommandEsc( bool b_enableReplaceOfCmd )
{
  return sendCommand (146 /* Command: Command --- Parameter: ESC */,
                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, b_enableReplaceOfCmd);
}


bool
CommandHandler_c::sendNonVolatileDeleteVersion( const char* versionLabel7chars )
{
  return sendCommand (210 /* Non-Volatile Memory Operation Delete Version */,
                      static_cast<uint8_t>( versionLabel7chars[0] ), static_cast<uint8_t>( versionLabel7chars[1] ),
                      static_cast<uint8_t>( versionLabel7chars[2] ), static_cast<uint8_t>( versionLabel7chars[3] ),
                      static_cast<uint8_t>( versionLabel7chars[4] ), static_cast<uint8_t>( versionLabel7chars[5] ),
                      static_cast<uint8_t>( versionLabel7chars[6] ), false );
}


bool
CommandHandler_c::queueOrReplace (SendUpload_c& ar_sendUpload, bool b_enableReplaceOfCmd)
{
  if( !mb_commandsToBus )
    return false;

#if DEBUG_VTCOMM
  if( !m_connection.poolSuccessfullyUploaded() )
    INTERNAL_DEBUG_DEVICE << "--NOT ENQUEUED - POOL NO YET COMPLETELY UPLOADED!--" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif

  if( !m_connection.poolSuccessfullyUploaded() )
    return false;

  SendUpload_c* p_queue = NULL;
  uint8_t i = 0;
#ifdef OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED
  STL_NAMESPACE::list<SendUpload_c,MALLOC_TEMPLATE(SendUpload_c) >::iterator i_sendUpload;
#else
  STL_NAMESPACE::list<SendUpload_c>::iterator i_sendUpload;
#endif
  if (mb_checkSameCommand && b_enableReplaceOfCmd && !mq_sendUpload.empty())
  { //get first equal command in queue
    i_sendUpload = mq_sendUpload.begin();
    if ( men_uploadCommandState != UploadCommandIdle )
    { // the first item in the queue is currently in upload process - so do NOT use this for replacement, as the next action
      // after receive of the awaited ACK is simple erase of the first command
      ++i_sendUpload;
    }
    for (; (p_queue == NULL) && (i_sendUpload != mq_sendUpload.end()); i_sendUpload++)
    { //first check if multisendstreamer is used!
      /* four cases:
      1. both use buffer
      2. both use mssObjectString
      3. mss is queued and could be replaced by buffer
      4. buffer is queued and could be replaced by mssObjectString
        */
      if ((i_sendUpload->mssObjectString == NULL) && (ar_sendUpload.mssObjectString == NULL))
      {
        if (i_sendUpload->vec_uploadBuffer[0] == ar_sendUpload.vec_uploadBuffer[0])
        {
          uint8_t ui8_offset = (ar_sendUpload.vec_uploadBuffer[0]);
          if ( (ui8_offset<scui8_cmdCompareTableMin) || (ui8_offset > scui8_cmdCompareTableMax))
          { // only 0x12 is possible, but no need to override, it shouldn't occur anyway!
            if ((ui8_offset == 0x11) || (ui8_offset == 0x12) ||
                ((ui8_offset >= 0x60) && (ui8_offset <= 0x7F)) ) /// no checking for Proprietary commands (we don't need the replace-feature here!)
              break;

            isoaglib_assert( !"Shouldn't reach here. Check which command it was!" );
            return false;
          }
          // get bitmask for the corresponding command
          uint8_t ui8_bitmask = scpui8_cmdCompareTable [ui8_offset-scui8_cmdCompareTableMin];
          if (!(ui8_bitmask & (1<<0)))
          { // go Check for overwrite...
            for (i=1;i<=7;i++)
            {
              if (((ui8_bitmask & 1<<i) !=0) && !(i_sendUpload->vec_uploadBuffer[i] == ar_sendUpload.vec_uploadBuffer[i]))
                break;
            }
            if (!(i<=7))
            { // loop ran through, all to-compare-bytes matched!
              p_queue = &*i_sendUpload; // so overwrite this SendUpload_c with the new value one
            }
          }
        }
      }
      if ((i_sendUpload->mssObjectString != NULL) && (ar_sendUpload.mssObjectString != NULL))
      {
        if ((*i_sendUpload).mssObjectString->getStreamer()->getFirstByte() == ar_sendUpload.mssObjectString->getStreamer()->getFirstByte())
        {
          if ((*i_sendUpload).mssObjectString->getStreamer()->getID() == ar_sendUpload.mssObjectString->getStreamer()->getID())
            p_queue = &*i_sendUpload;
        }
      }
      if ((i_sendUpload->mssObjectString != NULL) && (ar_sendUpload.mssObjectString == NULL))
      {
        if ((*i_sendUpload).mssObjectString->getStreamer()->getFirstByte() == ar_sendUpload.vec_uploadBuffer[0])
        {
          if ((*i_sendUpload).mssObjectString->getStreamer()->getID() == (ar_sendUpload.vec_uploadBuffer[1] | (ar_sendUpload.vec_uploadBuffer[2]<<8)))
            p_queue = &*i_sendUpload;
        }
      }
      if ((i_sendUpload->mssObjectString == NULL) && (ar_sendUpload.mssObjectString != NULL))
      {
        if ((*i_sendUpload).vec_uploadBuffer[0] == ar_sendUpload.mssObjectString->getStreamer()->getFirstByte())
        {
          if (((*i_sendUpload).vec_uploadBuffer[1] | (*i_sendUpload).vec_uploadBuffer[2]<<8) == ar_sendUpload.mssObjectString->getStreamer()->getID())
            p_queue = &*i_sendUpload;
        }
      }
    } // for
  }

  if( p_queue == NULL )
  {
    const bool filledFromEmpty = mq_sendUpload.empty();

    mq_sendUpload.push_back (ar_sendUpload);

    // call after push(_back), so it's already available at call-time!
    if( filledFromEmpty )
      m_connection.notifyOnCommandQueueFilledFromEmpty();
  }
  else
    *p_queue = ar_sendUpload; // overloaded "operator="

  return true;
}


void
CommandHandler_c::dumpQueue()
{
#if DEBUG_VTCOMM
#ifdef OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED
  STL_NAMESPACE::list<SendUpload_c,MALLOC_TEMPLATE(SendUpload_c) >::iterator i_sendUpload;
#else
  STL_NAMESPACE::list<SendUpload_c>::iterator i_sendUpload;
#endif

  for (i_sendUpload = mq_sendUpload.begin(); i_sendUpload != mq_sendUpload.end(); ++i_sendUpload)
  {
    if (i_sendUpload->mssObjectString == NULL)
    {
      for (uint8_t i=0; i<=7; i++)
      {
        INTERNAL_DEBUG_DEVICE << " " << (uint16_t)(i_sendUpload->vec_uploadBuffer[i]);
      }
    }
    else
    {
      MultiSendPkg_c msp;
      int i_strSize = i_sendUpload->mssObjectString->getStreamer()->getStreamSize();
      for (int i=0; i < i_strSize; i+=7) {
        i_sendUpload->mssObjectString->getStreamer()->setDataNextStreamPart (&msp, (unsigned char) ((i_strSize - i) > 7 ? 7 : (i_strSize-i)));
        for (uint8_t j=1; j<=7; j++)
        {
          INTERNAL_DEBUG_DEVICE << " " << (uint16_t)(msp[j]);
        }
      }
    }
  }
  INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
}

bool
CommandHandler_c::tryToStart()
{
  if( mq_sendUpload.empty() )
    return false;

  men_uploadCommandState = UploadCommandWithAwaitingResponse;
  SendUpload_c &actSend = mq_sendUpload.front();

#ifdef WORKAROUND_PREMATURE_TP_RESPONSES
  m_queuedResponseErrByte = 0; // no response stored.
#endif

   /// Use Multi or Single CAN-Pkgs?
  //////////////////////////////////

  if ((actSend.mssObjectString == NULL) && (actSend.vec_uploadBuffer.size() < 9))
  { /// Fits into a single CAN-Pkg!
    if (actSend.vec_uploadBuffer[0] == 0x11)
    { /// Handle special case of LanguageUpdate / UserPoolUpdate
      if (actSend.ppc_vtObjects)
      { /// User triggered Partial Pool Update
        m_connection.uploadPoolState().initObjectPoolUploadingPhases(
          UploadPoolState_c::UploadPoolTypeUserPoolUpdate,
          actSend.ppc_vtObjects,
          actSend.ui16_numObjects );
      }
      else
      { /// Language Pool Update
        m_connection.uploadPoolState().initObjectPoolUploadingPhases(
          UploadPoolState_c::UploadPoolTypeLanguageUpdate );
      }
      m_connection.uploadPoolState().startCurrentUploadPhase();

      men_uploadCommandState = UploadCommandPartialPoolUpdate; // There's NO response for command 0x11! And there may be multiple parts!
      mi32_commandTimestamp = -1;
      mi32_commandTimeout = -1;
      // There's no timeout (hence no valid "mui8_commandParameter")
      return true;
    }
    else
    { /// normal 8 byte package
      mi32_commandTimestamp = HAL::getTime();
      mui8_commandParameter = actSend.vec_uploadBuffer [0];

      // Shouldn't be less than 8, else we're messin around with vec_uploadBuffer!
      m_connection.sendMessage(
        actSend.vec_uploadBuffer [0], actSend.vec_uploadBuffer [1], actSend.vec_uploadBuffer [2], actSend.vec_uploadBuffer [3],
        actSend.vec_uploadBuffer [4], actSend.vec_uploadBuffer [5], actSend.vec_uploadBuffer [6], actSend.vec_uploadBuffer [7]);
    }
  }
  else if ((actSend.mssObjectString != NULL) && (actSend.mssObjectString->getStreamer()->getStreamSize() < 9))
  { /// Fits into a single CAN-Pkg!
    mi32_commandTimestamp = HAL::getTime();
    mui8_commandParameter = actSend.mssObjectString->getStreamer()->getFirstByte();

    uint8_t ui8_len = actSend.mssObjectString->getStreamer()->getStreamSize();

    uint8_t data[ 8 ];
    actSend.mssObjectString->getStreamer()->set5ByteCommandHeader( data );
    int i=5;
    for (; i < ui8_len; ++i) data[ i ] = actSend.mssObjectString->getStreamer()->getStringToStream() [i-5];
    for (; i < 8;       ++i) data[ i ] = 0xFF; // pad unused bytes with "0xFF", so CAN-Pkg is of size 8!
    
    m_connection.sendMessage(
      data[ 0 ], data[ 1 ], data[ 2 ], data[ 3 ], data[ 4 ], data[ 5 ], data[ 6 ], data[ 7 ] );   
  }
  else if (actSend.mssObjectString == NULL)
  { /// Use multi CAN-Pkgs [(E)TP], doesn't fit into a single CAN-Pkg!
    mi32_commandTimestamp = -1; // will get set on SendSuccess
    mui8_commandParameter = actSend.vec_uploadBuffer [0];

    ( void )getMultiSendInstance( m_connection.getMultitonInst() ).sendIsoTarget(
      m_connection.getIdentItem().isoName(),
      m_connection.getVtServerInst().getIsoName(),
      &actSend.vec_uploadBuffer.front(),
      actSend.vec_uploadBuffer.size(), ECU_TO_VT_PGN, this );
  }
  else
  {
    mi32_commandTimestamp = -1; // will get set on SendSuccess
    mui8_commandParameter = actSend.mssObjectString->getStreamer()->getFirstByte();

    ( void )getMultiSendInstance( m_connection.getMultitonInst() ).sendIsoTarget(
      m_connection.getIdentItem().isoName(),
      m_connection.getVtServerInst().getIsoName(),
      (IsoAgLib::iMultiSendStreamer_c*)actSend.mssObjectString->getStreamer(),
      ECU_TO_VT_PGN, this );
  }

#if DEBUG_VTCOMM
  INTERNAL_DEBUG_DEVICE << "Waiting for response to " << unsigned( mui8_commandParameter ) << "... " << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
  mi32_commandTimeout = ((mui8_commandParameter >= 0xD0) && (mui8_commandParameter <= 0xD2))
                          ? s_timeOutVersionLabel
                          : (mui8_commandParameter == 0x12) ? s_timeOutEndOfObjectPool
                            /* default: sending Annex F. */ : s_timeOutNormalCommand;

  return true;
}


// will be called either from
// - finished pool-update-(E)TPs...
// - incoming response
void
CommandHandler_c::finishUploadCommand()
{
  isoaglib_assert( !mq_sendUpload.empty() );
  isoaglib_assert( men_uploadCommandState != UploadCommandIdle );

  mq_sendUpload.pop_front();

  #if DEBUG_VTCOMM
  INTERNAL_DEBUG_DEVICE
    << "Dequeued (after success, timeout, whatever..): " 
    << ( mq_sendUpload.size()+1 ) << " -> " << mq_sendUpload.size() << "."
    << INTERNAL_DEBUG_DEVICE_ENDL;
  #endif

  mi32_commandTimestamp = -1;
#ifdef WORKAROUND_PREMATURE_TP_RESPONSES
  m_queuedResponseErrByte = 0; // actually not used in UploadCommandIdle, just as the timestamp above!
#endif
  men_uploadCommandState = UploadCommandIdle;
  m_connection.notifyOnFinishedCommand( !mq_sendUpload.empty() );
}


void
CommandHandler_c::reactOnStateChange( const SendStream_c& sendStream )
{
  if( !m_connection.isVtActive() )
    return;

  switch( sendStream.getSendSuccess() )
  {
  case __IsoAgLib::SendStream_c::Running:
    break;

  case __IsoAgLib::SendStream_c::SendAborted:
    {
      const bool started = tryToStart();
      isoaglib_assert( started ); ( void )started;
    } break;

  case __IsoAgLib::SendStream_c::SendSuccess:
    mi32_commandTimestamp = HAL::getTime();
#ifdef WORKAROUND_PREMATURE_TP_RESPONSES
    if( m_queuedResponseErrByte > 0 )
      finalizeCommand( m_queuedResponseErrByte, m_queuedResponsePkg );
#endif
    // now two things can be detected:
    // 1) time-out
    // 2) incoming response triggering the next command
    break;
  }
}


void
CommandHandler_c::processMsgVtToEcuActivations( const CanPkgExt_c& pkg )
{
  IsoAgLib::iVtClientObjectPool_c& pool = m_connection.getPool();

  switch( pkg.getUint8Data( 0 ) )
  {
  case 0x00: // Command: "Control Element Function", parameter "Soft Key"
  case 0x01: // Command: "Control Element Function", parameter "Button"
    pool.eventKeyCode(
        pkg.getUint8Data( 1 ) /* key activation code (pressed, released, held) */,
        pkg.getUint8Data( 2 ) | (pkg.getUint8Data( 3 ) << 8) /* objID of key object */,
        pkg.getUint8Data( 4 ) | (pkg.getUint8Data( 5 ) << 8) /* objID of visible mask */,
        pkg.getUint8Data( 6 ) /* key code */,
        (pkg.getUint8Data( 0 ) != 0)/* 0 for sk, 1 for button -- matches wasButton? boolean */ );
    break;

  case 0x02: // Command: "Control Element Function", parameter "Pointing Event"
    pool.eventPointingEvent(
        pkg.getUint8Data( 1 ) | (pkg.getUint8Data( 2 ) << 8) /* X position in pixels */,
        pkg.getUint8Data( 3 ) | (pkg.getUint8Data( 4 ) << 8) /* Y position in pixels */);
    break;

  case 0x03: // Command: "VT Select Input Object"
    pool.eventVtSelectInputObject(
        uint16_t(pkg.getUint8Data( 1 )) | (uint16_t(pkg.getUint8Data( 2 )) << 8) /* objID */,
        pkg.getUint8Data( 3 ),
        pkg.getUint8Data( 4 ));
    break;

  case 0x04: // Command: "Control Element Function", parameter "VT ESC"
    pool.eventVtESC( (pkg.getUint8Data( 3 ) != 0x0)
      ? 0xFFFF
      : ( uint16_t(pkg.getUint8Data( 1 )) | (uint16_t(pkg.getUint8Data( 2 )) << 8) ) );
    break;

  case 0x05: // Command: "Control Element Function", parameter "VT Change Numeric Value"
    pool.eventNumericValue(
        uint16_t(pkg.getUint8Data( 1 )) | (uint16_t(pkg.getUint8Data( 2 )) << 8) /* objID */,
        pkg.getUint8Data( 4 ) /* 1 byte value */,
        (uint32_t(pkg.getUint8Data( 4 ))      ) | (uint32_t(pkg.getUint8Data( 5 )) << 8) | 
        (uint32_t(pkg.getUint8Data( 6 )) << 16) | (uint32_t(pkg.getUint8Data( 7 )) << 24) /* 4 byte value */);
    break;

  case 0x08:  // Command: "Control Element Function", parameter "VT Input String Value"
    if (pkg.getUint8Data( 3 ) <= 4) //within a 8 byte long cmd can be only a 4 char long string
    {
      VolatileMemory_c c_vmString (pkg.getUint8DataConstPointer( 4 ));
      pool.eventStringValue(
          uint16_t(pkg.getUint8Data( 1 )) | (uint16_t(pkg.getUint8Data( 2 )) << 8) /* objID */,
          pkg.getUint8Data( 3 ) /* total number of bytes */, c_vmString,
          pkg.getUint8Data( 3 ) /* total number of bytes */, true, true);
    }
    break;

  // Version 4 feature that was "Display Activation" in some draft
  // but is now "VT On User-Layout Hide/Show message"
  case 0x09:  // Command: "Control Element Function", parameter "VT On User-Layout Hide/Show message"
    // Command: "Control Element Function"
    // Parameter "VT On User-Layout Hide/Show message Response"
    // @todo Version 4 Handling of that message needs to be redone when adding Version 4 properly.
    m_connection.sendMessage(
      pkg.getUint8Data( 0 ), pkg.getUint8Data( 1 ), pkg.getUint8Data( 2 ), pkg.getUint8Data( 3 ),
      pkg.getUint8Data( 4 ), pkg.getUint8Data( 5 ), pkg.getUint8Data( 6 ), pkg.getUint8Data( 7 ) );
    break;

  /***************************************/
  /*** ### AUX Assignment Messages ### ***/
  case 0x20: // Command: "Auxiliary Control type 1", parameter "Auxiliary Assignment"
    { 
      if( !m_connection.uploadPoolState().activeAuxO() )
        break;

      /** @todo SOON-258 If we can't assign because WE don't know this SA, should we anyway answer the assignment?
       * for now we don't answer if we can't take the assignment - VTs have to handle this anyway...
       * Update on 22.11.2007: Should be okay so far, as written, VT has to handle, and we can't NACK the assignment! */
      bool const cb_assignmentOkay = m_connection.storeAuxAssignment( pkg );

      if( cb_assignmentOkay )
      { // respond if it was a valid assignment...
        m_connection.sendMessage(
          pkg.getUint8Data( 0 ), pkg.getUint8Data( 1 ), pkg.getUint8Data( 2 ), pkg.getUint8Data( 3 ),
          pkg.getUint8Data( 4 ), pkg.getUint8Data( 5 ), pkg.getUint8Data( 6 ), pkg.getUint8Data( 7 ) );
      }
    } break;

  case 0x25: // Command: "Auxiliary Control type 2", parameter "Input Status Enable"
    {
      if( !m_connection.uploadPoolState().activeAuxN() )
        break;

      const uint16_t ui16_inputObjId = (pkg.getUint8Data( 1 ) | (pkg.getUint8Data( 2 ) << 8));
      const bool b_objFound = m_connection.aux2Inputs().setInputStateEnabledInObjects(ui16_inputObjId, ( 0 != pkg.getUint8Data( 3 ) ));

      m_connection.sendMessage(
        pkg.getUint8Data( 0 ), pkg.getUint8Data( 1 ), pkg.getUint8Data( 2 ), pkg.getUint8Data( 3 ),
        !b_objFound, 0xFF, 0xFF, 0xFF );
    } break;
  }
}

void
CommandHandler_c::processMsgVtToEcuActivations( Stream_c &stream )
{
  const uint8_t command = stream.getFirstByte();

  switch( command )
  {
    case 0x08:
    {
      const uint16_t inputStringId = uint16_t( stream.getNextNotParsed() ) | ( uint16_t( stream.getNextNotParsed() ) << 8 );
      const unsigned inputStringLength = stream.getNextNotParsed();

      const uint16_t ui16_totalstreamsize = stream.getByteTotalSize();
      if( ui16_totalstreamsize >= (inputStringLength + 4) )
      { /** @todo SOON-258 "if (ui16_totalstreamsize > (mui8_inputStringLength + 4)) registerErronousVtMessage("VT Input String Activation CAN-Message too long.");
	      This is/was a problem of the John Deere GS2 VT and needs to be registered for any VT.
	      It will be fixed in the GS2 in 2008, but for now we have relaxed the checking and put this comment in here.
	    */
        // no on-the-fly parsing anymore
        m_connection.getPool().eventStringValue( inputStringId, inputStringLength, stream, stream.getNotParsedSize(), true, true );
      }
    } break;

    case 0x24: // Command: "Auxiliary Control", parameter "Auxiliary Assignment type 2"
    {
      if( !m_connection.uploadPoolState().activeAuxN() )
        break;

      uint16_t ui16_functionObjId = 0xFFFF;
      const uint8_t errorCode = m_connection.storeAux2Assignment( stream, ui16_functionObjId );

      m_connection.sendMessage( 0x24, ui16_functionObjId & 0xFF, ui16_functionObjId >> 8, errorCode, 0xFF, 0xFF, 0xFF, 0xFF );
    } break;

    default:
      if( command >= 0x60 && command <= 0x7F )
        m_connection.getPool().eventProprietaryCommand(
          m_connection.getVtServerInst().getIsoName().toConstIisoName_c(),
          command,
          stream );
      break;
  }
}

void
CommandHandler_c::processMsgVtToEcuResponses( const CanPkgExt_c& pkg )
{
  unsigned errByte = 0; // from 1-8, or 0 for NO errorHandling, as NO user command (was intern command like C0/C2/C3/C7/etc.)
  bool needRestart = false;
  bool checkForVtSwitch = false;

  IsoAgLib::iVtClientObjectPool_c& pool = m_connection.getPool();

  switch( pkg.getUint8Data( 0 ) )
  {
  case 0x12: // Command: "End of Object Pool Transfer", parameter "Object Pool Ready Response"
    needRestart = m_connection.uploadPoolState().handleEndOfObjectPoolResponseOnLanguageUpdate( pkg.getUint8Data( 1 ) == 0 );
    errByte = 2;
    break;

  case 0x22: // Command: "Auxiliary Control type 2", parameter "preferred assignment"
    errByte = 2;
    break;

  case 0xA3: // Command: "Command", parameter "Control Audio Device Response"
  case 0xA4: // Command: "Command", parameter "Set Audio Volume Response"
    errByte = 2;
    break;

  case 0xB2: // Command: "Command", parameter "Delete Object Pool Response"
    errByte = 2;
    checkForVtSwitch = true;
    break;

  case 0xA6: // Command: "Command", parameter "Change Size Response"
  case 0xA8: // Command: "Command", parameter "Change Numeric Value Response"
  case 0xA9: // Command: "Command", parameter "Change End Point Response"
  case 0xAA: // Command: "Command", parameter "Change Font Attributes Response"
  case 0xAB: // Command: "Command", parameter "Change Line Attributes Response"
  case 0xAC: // Command: "Command", parameter "Change Fill Attributes Response"
  case 0xAD: // Command: "Command", parameter "Change Active Mask Response"
  case 0x92: // Command: "Command", parameter "ESC Response"
    errByte = 4;
    break;

  case 0xA0: // Command: "Command", parameter "Hide/Show Object Response" (Container)
  case 0xA1: // Command: "Command", parameter "Enable/Disable Object Response" (Input Object)
  case 0xA2: // Command: "Command", parameter "Select Input Object Response"
  case 0xA7: // Command: "Command", parameter "Change Background Colour Response"
  case 0xAF: // Command: "Command", parameter "Change Attribute Response"
  case 0xB0: // Command: "Command", parameter "Change Priority Response"
  case 0xB8: // Command: "Command", parameter "Graphics Context Command"
    errByte = 5;
    break;

  case 0xA5: // Command: "Command", parameter "Change Child Location Response"
  case 0xAE: // Command: "Command", parameter "Change Soft Key Mask Response"
  case 0xB3: // Command: "Command", parameter "Change String Value Response"
  case 0xB4: // Command: "Command", parameter "Change Child Position Response"
    errByte = 6;
    break;

  case 0xB1: // Command: "Command", parameter "Change List Item Response"
    errByte = 7;
    break;

  case 0xB9: // Command: "Get Technical Data", parameter "Get Attribute Value Response"
    errByte = 7;
    #ifdef USE_ISO_TERMINAL_GETATTRIBUTES
    #warning Currently not supported!!
    // client requested any attribute value for an object in the pool -> create ram struct if not yet existing
    if ((pkg.getUint8Data( 1 ) == 0xFF) && (pkg.getUint8Data( 2 ) == 0xFF)) // object id is set to 0xFFFF to indicate error response
    {
      /// what to do if attribute value request returns error response???
    }
    else
    {
      // first store object ID for later use
      uint16_t ui16_objID = pkg.getUint8Data( 1 ) | (pkg.getUint8Data( 2 ) << 8);

      /// search for suitable iVtObject in all object lists of the client (pointer array to all fix and language dependent iVtObjects)

      uint8_t ui8_arrIndex = 0;
      bool b_objectFound = false;

      // first check if first item is the requested one -> working is the first item list no matter what objectID it has
      if (ui16_objID == mrc_pool.getIVtObjects()[ui8_arrIndex][0]->getID())
        mrc_pool.eventAttributeValue(
            mrc_pool.getIVtObjects()[ui8_arrIndex][0],
            pkg.getUint8Data( 3 ),
            (uint8_t *)pkg.getUint8DataConstPointer( 4 ) ); // bad cast, actually the interface should be changed!
      else
      {
        // if last item of the list was reached or the requested object was found
        while (mrc_pool.getIVtObjects()[ui8_arrIndex] != NULL)
        {
          uint16_t ui16_arrBegin = 1;
          uint16_t ui16_arrMiddle;
          uint16_t ui16_arrEnd;

          // first item in list contains all fix objects of the pool (when language changes, these object stay the same)
          if (ui8_arrIndex == 0)
            ui16_arrEnd = mrc_pool.getNumObjects() - 1;
          else
            ui16_arrEnd = mrc_pool.getNumObjectsLang() - 1;

          // if object is among these we can leave the while-loop
          if ((ui16_objID < mrc_pool.getIVtObjects()[ui8_arrIndex][ui16_arrBegin]->getID())
           || (ui16_objID > mrc_pool.getIVtObjects()[ui8_arrIndex][ui16_arrEnd]->getID())) // range check
          {
            ui8_arrIndex++;
            continue; // try next object list, the requested object could not be found in the current list
          }

          while (ui16_arrBegin <= ui16_arrEnd)
          {
            ui16_arrMiddle = ui16_arrBegin + ((ui16_arrEnd - ui16_arrBegin) / 2);

            if (mrc_pool.getIVtObjects()[ui8_arrIndex][ui16_arrMiddle]->getID() == ui16_objID) // objID found?
            {
              b_objectFound = true;
              mrc_pool.eventAttributeValue(
                  mrc_pool.getIVtObjects()[ui8_arrIndex][ui16_arrMiddle],
                  pkg.getUint8Data( 3 ),
                  (uint8_t *)pkg.getUint8DataConstPointer( 4 ) ); // bad cast, actually the interface should be changed!
              break;
            }
            else
            {
              if (mrc_pool.getIVtObjects()[ui8_arrIndex][ui16_arrMiddle]->getID() > ui16_objID)
                ui16_arrEnd = ui16_arrMiddle - 1;
              else
                ui16_arrBegin = ui16_arrMiddle + 1;
            }
          }

          if ((ui8_arrIndex == 0) && b_objectFound) // an object is either a fix object or a language dependent object (at least once in any language dependent list)
            break;

          ui8_arrIndex++;
        }
      }
    }
    #endif
    break;

  case 0xBD: // Command: "Command", parameter "Lock/Unlock Mask Response"
    errByte = 3;
    break;

  case 0xD0: // Command: "Non Volatile Memory", parameter "Store Version Response"
    // in case StoreVersion was triggered by the Application in normal operation
    errByte = 6;
    break;

  case 0xD2: // Command: "Non Volatile Memory", parameter "Delete Version Response"
    errByte = 6;
    break;

  default: // handle proprietary messages from VT
    if(    pkg.getUint8Data( 0 ) >= 0x60
        && pkg.getUint8Data( 0 ) <= 0x7F )
    {
      errByte = pool.eventProprietaryCommand(
        m_connection.getVtServerInst().getIsoName().toConstIisoName_c(),
        pkg.getUint8DataConstPointer() );
    }
    break;
  }

  // Was it some command that requires queue-deletion & error processing?
  if( errByte != 0 )
    finalizeCommand( errByte, pkg.getUint8DataConstPointer( 0 ) );

  if( needRestart )
    m_connection.restart();

  if( checkForVtSwitch )
    m_connection.restartWithNextVt();
}


// it's safe to answer 0x00 as "no timeout", as we're never sending a "0x00"-command to the VT!
uint8_t
CommandHandler_c::timeEventCommandTimeoutCheck() const
{
  // nothing to check (yet)
  if( mi32_commandTimestamp < 0 )
    return 0x00;

  // Waiting for an answer now... Did it time out?
  return( HAL::getTime() > ( mi32_commandTimestamp + mi32_commandTimeout ) )
    ? mui8_commandParameter
    : 0x00;
}


void
CommandHandler_c::finalizeCommand( unsigned errByte, const uint8_t *dataBytes )
{
  const uint8_t cmd = dataBytes[ 0 ];
  if( mi32_commandTimestamp > 0 ) // command completely transferred!!
  { /// Our command was successfully sent & responded to, so remove it from the queue
    if( mui8_commandParameter == cmd )
    { /* okay, right response for our current command! */
#if DEBUG_VTCOMM
      INTERNAL_DEBUG_DEVICE << "Got awaited response for " << unsigned( mui8_commandParameter ) << "... " << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
      // special treatment for Get Attribute Value command -> error byte is also being used as value byte for successful response
      const uint8_t ui8_uploadCommandError = ((mui8_commandParameter == 0xB9) && ((uint16_t(dataBytes[ 2-1 ]) | uint16_t(dataBytes[ 3-1 ])<<8) != 0xFFFF))
                                               ? 0 : dataBytes[ errByte-1 ];

      m_connection.getPool().eventCommandResponse( ui8_uploadCommandError, dataBytes ); // pass "ui8_uploadCommandError" in case it's only important if it's an error or not. get Cmd and all databytes from "arc_data.name()"
      finishUploadCommand();
    }
  }
  else
  { /// Our command is still running, so do NOT remove from the queue
    /// To early (in case of TP) or unsolicited response from VT!!
#ifdef WORKAROUND_PREMATURE_TP_RESPONSES
    if( mui8_commandParameter == cmd )
    { /* okay, right (but premature) response for our current command! */
      m_queuedResponseErrByte = errByte;
      for( int i=0; i<8; ++i )
        m_queuedResponsePkg[ i ] = dataBytes[ i ];
    }
#endif
  }
}


void
CommandHandler_c::doStop()
{
  mq_sendUpload.clear();

  men_uploadCommandState = UploadCommandIdle;
}


bool CommandHandler_c::sendCommandChangeNumericValue (IsoAgLib::iVtObject_c* apc_object, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd)
{ return sendCommandChangeNumericValue(apc_object->getID(), byte1, byte2, byte3, byte4, b_enableReplaceOfCmd); }

bool CommandHandler_c::sendCommandChangeAttribute    (IsoAgLib::iVtObject_c* apc_object, uint8_t attrId, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd)
{ return sendCommandChangeAttribute(apc_object->getID(), attrId, byte1, byte2, byte3, byte4, b_enableReplaceOfCmd); }

bool CommandHandler_c::sendCommandChangeSoftKeyMask  (IsoAgLib::iVtObject_c* apc_object, uint8_t maskType, uint16_t newSoftKeyMaskID, bool b_enableReplaceOfCmd)
{ return sendCommandChangeSoftKeyMask(apc_object->getID(), maskType, newSoftKeyMaskID, b_enableReplaceOfCmd); }

bool CommandHandler_c::sendCommandChangeStringValue (IsoAgLib::iVtObject_c* apc_object, const char* apc_newValue, uint16_t overrideSendLength, bool b_enableReplaceOfCmd)
{ return sendCommandChangeStringValue(apc_object->getID(), apc_newValue, overrideSendLength, b_enableReplaceOfCmd); }

bool CommandHandler_c::sendCommandChangeActiveMask (IsoAgLib::iVtObject_c* apc_object, IsoAgLib::iVtObject_c* apc_mask, bool b_enableReplaceOfCmd)
{ return sendCommandChangeActiveMask( apc_object->getID(), apc_mask->getID(), b_enableReplaceOfCmd ); }

bool CommandHandler_c::sendCommandChangeChildPosition (IsoAgLib::iVtObject_c* apc_object, IsoAgLib::iVtObject_c* apc_childObject, int16_t x, int16_t y, bool b_enableReplaceOfCmd)
{ return sendCommandChangeChildPosition(apc_object->getID(), apc_childObject->getID(), x, y, b_enableReplaceOfCmd); }

bool CommandHandler_c::sendCommandChangeChildLocation (IsoAgLib::iVtObject_c* apc_object, IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_enableReplaceOfCmd)
{ return sendCommandChangeChildLocation(apc_object->getID(), apc_childObject->getID(), dx, dy, b_enableReplaceOfCmd); }

bool CommandHandler_c::sendCommandChangeBackgroundColour (IsoAgLib::iVtObject_c* apc_object, uint8_t newColour,  bool b_enableReplaceOfCmd)
{ return sendCommandChangeBackgroundColour(apc_object->getID(), newColour, b_enableReplaceOfCmd); }

bool CommandHandler_c::sendCommandChangeSize (IsoAgLib::iVtObject_c* apc_object, uint16_t newWidth, uint16_t newHeight, bool b_enableReplaceOfCmd)
{ return sendCommandChangeSize(apc_object->getID(), newWidth, newHeight, b_enableReplaceOfCmd); }

bool CommandHandler_c::sendCommandChangeFillAttributes (IsoAgLib::iVtObject_c* apc_object, uint8_t newFillType, uint8_t newFillColour, IsoAgLib::iVtObjectPictureGraphic_c* newFillPatternObject, bool b_enableReplaceOfCmd)
{ return sendCommandChangeFillAttributes(apc_object->getID(), newFillType, newFillColour, newFillPatternObject, b_enableReplaceOfCmd); }

bool CommandHandler_c::sendCommandChangeFontAttributes (IsoAgLib::iVtObject_c* apc_object, uint8_t newFontColour, uint8_t newFontSize, uint8_t newFontType, uint8_t newFontStyle, bool b_enableReplaceOfCmd)
{ return sendCommandChangeFontAttributes( apc_object->getID(), newFontColour, newFontSize, newFontType, newFontStyle, b_enableReplaceOfCmd); }

bool CommandHandler_c::sendCommandChangeLineAttributes (IsoAgLib::iVtObject_c* apc_object, uint8_t newLineColour, uint8_t newLineWidth, uint16_t newLineArt, bool b_enableReplaceOfCmd)
{ return sendCommandChangeLineAttributes( apc_object->getID(), newLineColour, newLineWidth, newLineArt, b_enableReplaceOfCmd); }

bool CommandHandler_c::sendCommandHideShow( IsoAgLib::iVtObject_c* apc_object, uint8_t b_hideOrShow, bool b_enableReplaceOfCmd)
{ return sendCommandHideShow(apc_object->getID(), b_hideOrShow, b_enableReplaceOfCmd); }


} // __IsoAgLib
