/*
  commandhandler_c.h: class for sending all the commands.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <IsoAgLib/isoaglib_config.h>
#include "sendupload_c.h"

#include <list>

namespace IsoAgLib { class iVtObject_c; }
namespace IsoAgLib { class iVtObjectString_c; }
namespace IsoAgLib { class iVtObjectPictureGraphic_c; }


namespace __IsoAgLib {

class VtClientConnection_c;


class CommandHandler_c : public MultiSendEventHandler_c
{
  // not copyable
  CommandHandler_c &operator=( const CommandHandler_c & );
  CommandHandler_c( const CommandHandler_c & );

  enum UploadCommandState_t {
    UploadCommandIdle,
    UploadCommandWithAwaitingResponse,
    UploadCommandPartialPoolUpdate // for e.g. user/language reasons
  };

public:
  CommandHandler_c( VtClientConnection_c& );

  void processMsgVtToEcuActivations( const CanPkgExt_c& pkg );
  void processMsgVtToEcuActivations( Stream_c &stream );
  void processMsgVtToEcuResponses( const CanPkgExt_c& pkg );

  bool sendCommand (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, uint8_t byte9, bool b_enableReplaceOfCmd=true);
  bool sendCommand (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, bool b_enableReplaceOfCmd=true, IsoAgLib::iVtObject_c** rppc_vtObjects=NULL, uint16_t aui16_numObjects=0);
  bool sendCommand (uint8_t* apui8_buffer, uint32_t ui32_size);

  bool sendCommandHideShow               (IsoAgLib::iVtObject_c*, uint8_t b_hideOrShow, bool b_enableReplaceOfCmd);
  bool sendCommandChangeNumericValue     (IsoAgLib::iVtObject_c*, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeAttribute        (IsoAgLib::iVtObject_c*, uint8_t attrId, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeSoftKeyMask      (IsoAgLib::iVtObject_c*, uint8_t maskType, uint16_t newSoftKeyMaskID, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeStringValue      (IsoAgLib::iVtObject_c*, const char* apc_newValue, uint16_t overrideSendLength, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeActiveMask       (IsoAgLib::iVtObject_c*, IsoAgLib::iVtObject_c* apc_mask, bool b_enableReplaceOfCmd=true );
  bool sendCommandChangeStringValue      (IsoAgLib::iVtObjectString_c*, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeChildPosition    (IsoAgLib::iVtObject_c*, IsoAgLib::iVtObject_c* apc_childObject, int16_t x, int16_t y, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeChildLocation    (IsoAgLib::iVtObject_c*, IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeBackgroundColour (IsoAgLib::iVtObject_c*, uint8_t newColour,  bool b_enableReplaceOfCmd=true);
//bool sendCommandChangePriority         (IsoAgLib::iVtObject_c*, int8_t newPriority, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeSize             (IsoAgLib::iVtObject_c*, uint16_t newWidth, uint16_t newHeight, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeEndPoint         (IsoAgLib::iVtObject_c*, uint16_t newWidth, uint16_t newHeight, uint8_t newLineDirection, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeFillAttributes   (IsoAgLib::iVtObject_c*, uint8_t newFillType, uint8_t newFillColour, IsoAgLib::iVtObjectPictureGraphic_c* newFillPatternObject, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeFontAttributes   (IsoAgLib::iVtObject_c*, uint8_t newFontColour, uint8_t newFontSize, uint8_t newFontType, uint8_t newFontStyle, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeLineAttributes   (IsoAgLib::iVtObject_c*, uint8_t newLineColour, uint8_t newLineWidth, uint16_t newLineArt, bool b_enableReplaceOfCmd=true);
  bool sendCommandLockUnlockMask         (IsoAgLib::iVtObject_c*, bool b_lockMask, uint16_t ui16_lockTimeOut, bool b_enableReplaceOfCmd = true);

  bool sendCommandHideShow               (uint16_t aui16_objectUid, uint8_t b_hideOrShow, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeNumericValue     (uint16_t aui16_objectUid, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeAttribute        (uint16_t aui16_objectUid, uint8_t attrId, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeSoftKeyMask      (uint16_t aui16_objectUid, uint8_t maskType, uint16_t newSoftKeyMaskID, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeActiveMask       (uint16_t aui16_objectUid, uint16_t maskId, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeStringValue      (uint16_t aui16_objectUid, const char* apc_newValue, uint16_t overrideSendLength, bool b_enableReplaceOfCmd=true);
#ifdef USE_VT_UNICODE_SUPPORT
  bool sendCommandChangeStringValueUTF16 (uint16_t aui16_objectUid, const char* apc_newValue, uint16_t overrideSendLength, bool b_enableReplaceOfCmd=true);
#endif
  bool sendCommandChangeChildPosition    (uint16_t aui16_objectUid, uint16_t aui16_childObjectUid, int16_t x, int16_t y, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeChildLocation    (uint16_t aui16_objectUid, uint16_t aui16_childObjectUid, int16_t dx, int16_t dy, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeBackgroundColour (uint16_t aui16_objectUid, uint8_t newColour,  bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeSize             (uint16_t aui16_objectUid, uint16_t newWidth, uint16_t newHeight, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeFillAttributes   (uint16_t aui16_objectUid, uint8_t newFillType, uint8_t newFillColour, IsoAgLib::iVtObjectPictureGraphic_c* newFillPatternObject, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeFontAttributes   (uint16_t aui16_objectUid, uint8_t newFontColour, uint8_t newFontSize, uint8_t newFontType, uint8_t newFontStyle, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeLineAttributes   (uint16_t aui16_objectUid, uint8_t newLineColour, uint8_t newLineWidth, uint16_t newLineArt, bool b_enableReplaceOfCmd=true);

#ifdef USE_ISO_TERMINAL_GRAPHICCONTEXT
  bool sendCommandSetGraphicsCursor           (IsoAgLib::iVtObject_c*, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd=true);
  bool sendCommandSetForegroundColour         (IsoAgLib::iVtObject_c*, uint8_t newValue, bool b_enableReplaceOfCmd=true);
  bool sendCommandSetBackgroundColour         (IsoAgLib::iVtObject_c*, uint8_t newValue, bool b_enableReplaceOfCmd=true);
  bool sendCommandSetGCLineAttributes         (IsoAgLib::iVtObject_c*, const IsoAgLib::iVtObjectLineAttributes_c* const newLineAttributes, bool b_enableReplaceOfCmd=true);
  bool sendCommandSetGCFillAttributes         (IsoAgLib::iVtObject_c*, const IsoAgLib::iVtObjectFillAttributes_c* const newFillAttributes, bool b_enableReplaceOfCmd=true);
  bool sendCommandSetGCFontAttributes         (IsoAgLib::iVtObject_c*, const IsoAgLib::iVtObjectFontAttributes_c* const newFontAttributes, bool b_enableReplaceOfCmd=true);
  bool sendCommandEraseRectangle              (IsoAgLib::iVtObject_c*, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd=true);
  bool sendCommandDrawPoint                   (IsoAgLib::iVtObject_c*, bool  b_enableReplaceOfCmd=true);
  bool sendCommandDrawLine                    (IsoAgLib::iVtObject_c*, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd=true);
  bool sendCommandDrawRectangle               (IsoAgLib::iVtObject_c*, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd=true);
  bool sendCommandDrawClosedEllipse           (IsoAgLib::iVtObject_c*, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd=true);
  bool sendCommandDrawPolygon                 (IsoAgLib::iVtObject_c*, uint16_t ui16_numOfPoints, const int16_t* api16_x, const int16_t* api16_y, bool b_enableReplaceOfCmd=true);
  bool sendCommandDrawText                    (IsoAgLib::iVtObject_c*, uint8_t ui8_textType, uint8_t ui8_numOfCharacters, const char *apc_newValue, bool b_enableReplaceOfCmd=true);
  bool sendCommandPanViewport                 (IsoAgLib::iVtObject_c*, int16_t ai16_x, int16_t ai16_y, bool b_enableReplaceOfCmd=true);
  bool sendCommandZoomViewport                (IsoAgLib::iVtObject_c*, int8_t newValue, bool b_enableReplaceOfCmd=true);
  bool sendCommandPanAndZoomViewport          (IsoAgLib::iVtObject_c*, int16_t ai16_x, int16_t ai16_y, int8_t newValue, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeViewportSize          (IsoAgLib::iVtObject_c*, uint16_t newWidth, uint16_t newHeight, bool b_enableReplaceOfCmd=true);
  bool sendCommandDrawVtObject                (IsoAgLib::iVtObject_c*, const IsoAgLib::iVtObject_c* const pc_VtObject, bool b_enableReplaceOfCmd=true);
  bool sendCommandCopyCanvas2PictureGraphic   (IsoAgLib::iVtObject_c*, const IsoAgLib::iVtObjectPictureGraphic_c* const pc_VtObjectPictureGraphic, bool b_enableReplaceOfCmd=true);
  bool sendCommandCopyViewport2PictureGraphic (IsoAgLib::iVtObject_c*, const IsoAgLib::iVtObjectPictureGraphic_c* const pc_VtObjectPictureGraphic, bool b_enableReplaceOfCmd=true);
#endif
#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  bool sendCommandGetAttributeValue           (IsoAgLib::iVtObject_c*, const uint8_t cui8_attrID, bool b_enableReplaceOfCmd=true);
#endif

  bool sendCommandControlAudioDevice (uint8_t aui8_repetitions, uint16_t aui16_frequency, uint16_t aui16_onTime, uint16_t aui16_offTime);
  bool sendCommandSetAudioVolume (uint8_t aui8_volume);
  bool sendCommandEsc (bool b_enableReplaceOfCmd=true);
  bool sendCommandDeleteObjectPool();
  bool sendCommandUpdateLanguagePool();
  bool sendCommandUpdateObjectPool (IsoAgLib::iVtObject_c** rppc_vtObjects, uint16_t aui16_numObjects);

  bool sendNonVolatileDeleteVersion( const char* versionLabel7chars );

  bool queueOrReplace (SendUpload_c& ar_sendUpload, bool b_enableReplaceOfCmd=true);
  unsigned getQueueSize() const { return mq_sendUpload.size(); }

  void doStop();
  bool tryToStart();
  void finishUploadCommand();
  uint8_t timeEventCommandTimeoutCheck() const;
  
  void sendCommandsToBus( bool commandsToBus ) { mb_commandsToBus = commandsToBus; }
  void enableSameCommandCheck() { mb_checkSameCommand = true; }
  void disableSameCommandCheck() { mb_checkSameCommand = false; }

private:
  void dumpQueue();
  void finalizeCommand( unsigned errByte, const uint8_t *dataBytes );

  // MultiSendEventHandler_c
  virtual void reactOnStateChange( const SendStream_c& sendStream );

private:
  VtClientConnection_c &m_connection;

  UploadCommandState_t men_uploadCommandState;

  uint8_t mui8_commandParameter; // this is kinda used as a cache only, because it's a four-case if-else to get the first byte!
  int32_t mi32_commandTimestamp;
  int32_t mi32_commandTimeout;
#ifdef WORKAROUND_PREMATURE_TP_RESPONSES
  uint8_t m_queuedResponseErrByte; // 1-8 ==> command queued, 0 ==> no cmd queued.
  uint8_t m_queuedResponsePkg[ 8 ];
#endif

  #ifdef OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED
  STL_NAMESPACE::list<SendUpload_c,MALLOC_TEMPLATE(SendUpload_c) >  mq_sendUpload;
  #else
  STL_NAMESPACE::list<SendUpload_c>  mq_sendUpload;
  #endif
  static SendUpload_c msc_tempSendUpload;

  bool mb_checkSameCommand;
  bool mb_commandsToBus;
};


inline
CommandHandler_c::CommandHandler_c( VtClientConnection_c &connection )
  : m_connection( connection )
  , men_uploadCommandState( UploadCommandIdle )
  , mui8_commandParameter( 0 ) // this is kinda used as a cache only, because it's a four-case if-else to get the first byte!
  , mi32_commandTimestamp( -1 ) // no check initially
  , mi32_commandTimeout( 0 ) // will be set when needed
#ifdef WORKAROUND_PREMATURE_TP_RESPONSES
  , m_queuedResponseErrByte( 0 )
//, m_queuedResponsePkg
#endif
  , mq_sendUpload()
  , mb_checkSameCommand( true )
  , mb_commandsToBus( true )
{
}


} // __IsoAgLib

#endif
