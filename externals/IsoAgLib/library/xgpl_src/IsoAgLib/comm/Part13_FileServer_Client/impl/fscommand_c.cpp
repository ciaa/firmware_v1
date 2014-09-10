/*
  fscommand_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <cstring>

// own
#include "fscommand_c.h"
#include "fsclientservercommunication_c.h"

// ISOAgLib
#include <IsoAgLib/comm/Part13_FileServer_Client/impl/fsmanager_c.h>
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/util/iassert.h>

// debug
#if DEBUG_FILESERVER
  #ifdef SYSTEM_PC
    #include <iostream>
  #else
    #include <supplementary_driver/driver/rs232/impl/rs232io_c.h>
  #endif
  #include <IsoAgLib/util/impl/util_funcs.h>
#endif

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif

namespace __IsoAgLib
{

const int gci_NR_REQUEST_ATTEMPTS=5;
const int gci_REQUEST_REPEAT_TIME=6500;
/* TODO may need to create a timeout based on number of bytes being read */
const int gci_REQUEST_REPEAT_BUSY_TIME=24000;

uint8_t FsCommand_c::m_maintenanceMsgBuf[8] = {0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

FsCommand_c::FsCommand_c(FsClientServerCommunication_c &FSCSComm, FsServerInstance_c &fileServerInst)
  : m_schedulerTask(*this, 100, true)
  , m_multiSendEventHandler(*this)
  , m_FSCSComm( FSCSComm )
  , mc_fileserver( fileServerInst )
  , m_tan( 0 )
  , m_nrOpenFiles( 0 )
  , m_keepConnectionOpen( false )
  , m_lastAliveSentTime( -1 )
  , m_packetLength(0)
  , m_multireceiveMsgBuf( NULL )
  , m_multireceiveMsgBufAllocSize( 0 )
  , m_receivedResponse( true )
  , m_lastrequestAttemptTime( -1 )
  , m_requestAttempts( 0 ) // value will be reset when needed
  , m_sendSuccessNotify( __IsoAgLib::SendStream_c::SendSuccess )
  , m_errorCode( 0 )
  , m_currentDirectory( NULL )
  , m_currentDirectoryAllocSize( 0 )
  , m_fileHandle( 0 )
  , m_fileName( NULL )
  , m_fileNameAllocSize( 0 )
  , m_attrCaseSensitive( false )
  , m_attrRemovable( false )
  , m_attrLongFilenames( false )
  , m_attrIsDirectory( false )
  , m_attrIsVolume( false )
  , m_attrHidden( false )
  , m_attrReadOnly( false )
  , m_positionMode( 0 )
  , m_offset( 0 )
  , m_position( 0 )
  , m_count( 0 )
  , m_data( NULL )
  , m_dataAllocSize( 0 )
  , m_dirData()
  , m_readDirectory( false )
  , m_receiveFilterCreated( false )
  , m_initialQueryStarted( false )
  , m_initializingFileserver( mc_fileserver.isBeingInitialized() )
  , m_waitForMultiSendFinish( false )
  , m_retryMultiPacketSend( false)
{
  getSchedulerInstance().registerTask( m_schedulerTask, 0 );

#if DEBUG_FILESERVER
  INTERNAL_DEBUG_DEVICE << "FsCommand created!" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
}


FsCommand_c::~FsCommand_c()
{
  doCleanUp();

  if (m_receiveFilterCreated)
  {
    // Multi-Packet
    getMultiReceiveInstance4Comm().deregisterClient(*this, m_FSCSComm.getClientIdentItem().getIsoItem()->isoName(), FS_TO_CLIENT_PGN, 0x3FFFFLU, &getFileserver().getIsoName());
  }

  // Scheduler
  getSchedulerInstance().deregisterTask(m_schedulerTask);

#if DEBUG_FILESERVER
  INTERNAL_DEBUG_DEVICE << "FsCommand destroyed!" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
}


void
FsCommand_c::timeEvent(void)
{
  if (!m_FSCSComm.getClientIdentItem().isClaimedAddress())
    return;

  if (!m_receiveFilterCreated)
  {
    // Multi-Packet (completely SA->DA specific!)
    getMultiReceiveInstance4Comm().registerClientIso (*this, m_FSCSComm.getClientIdentItem().getIsoItem()->isoName(), FS_TO_CLIENT_PGN, 0x3FFFFLU, false, false, &getFileserver().getIsoName());

    m_receiveFilterCreated = true;
  }

  if (m_waitForMultiSendFinish)
  { // Wait for multisend to finish...
    // If we couldn't start yet
    if (m_retryMultiPacketSend)
    { // re-try...
      sendMultiPacketTry();
    }
    else
    { // was started, so check progress
      switch (m_sendSuccessNotify)
      {
        case SendStream_c::Running:
          /* just wait */
          break;

        case SendStream_c::SendSuccess:
          m_waitForMultiSendFinish = false;
          // NOW we have really sent out the request
          // and can wait for the answer/timeout!
          m_lastrequestAttemptTime = HAL::getTime();
          break;

        case SendStream_c::SendAborted:
          // retry (currently) unlimited
          sendMultiPacketTry();
          break;
      }
    }
  }

  /// First of all, check if we are a command for
  /// A) Querying the FS's Properties/Volumes
  /// or
  /// B) normal application commands
  if (m_initializingFileserver)
  { // A)
    // do request the properties here, as we
    // have the IdentItem claimed definitely here!
    if (!m_initialQueryStarted)
    {
      m_initialQueryStarted = true;
      requestProperties();
    }
    // else: nothing to do if init sequence was started
  }
  // else B): nop right now.

  int32_t i32_requestRepeatTime = gci_REQUEST_REPEAT_TIME;
  if ( getFileserver().getBusy() )
    i32_requestRepeatTime = gci_REQUEST_REPEAT_BUSY_TIME;

  // Check for time-out
  if ( !m_receivedResponse
    && (m_lastrequestAttemptTime >= 0) // command sent out completely (important if TP/ETP)
    && ((HAL::getTime() - m_lastrequestAttemptTime) > i32_requestRepeatTime) )
  { // Time-out
    if (m_requestAttempts < gci_NR_REQUEST_ATTEMPTS)
    { // Retry
      sendRequest (RequestRetry);
    }
    else
    { // Give up
      if (m_initializingFileserver)
      {
        getFileserver().setState (FsServerInstance_c::unusable);
        // nothing more to do, this instance needs to be removed
        // (will be done in timeEvent) because the FS failed.
        return;
      }
      else
      {
        switch (en_lastCommand)
        {
          case en_noCommand:
            isoaglib_assert (!"Internal error. Shouldn't be in en_noCommand state when giveing up with the current command.");
            break;
          case en_getCurrentDirectory:
            m_FSCSComm.getCurrentDirectoryResponse(IsoAgLib::fsFileserverNotResponding, (uint8_t *)NULL);
            break;
          case en_changeCurrentDirectory:
            m_FSCSComm.changeCurrentDirectoryResponse(IsoAgLib::fsFileserverNotResponding, (uint8_t *)NULL);
            break;
          case en_openFile:
            m_FSCSComm.openFileResponse(IsoAgLib::fsFileserverNotResponding, 0, false, false, false, false, false, false, false);
            break;
          case en_seekFile:
            m_FSCSComm.seekFileResponse(IsoAgLib::fsFileserverNotResponding, 0);
            break;
          case en_readFile:
            if (m_readDirectory)
            {
              clearDirectoryList();

              m_FSCSComm.readDirectoryResponse(IsoAgLib::fsFileserverNotResponding, m_dirData);
            }
            else
              m_FSCSComm.readFileResponse(IsoAgLib::fsFileserverNotResponding, 0, (uint8_t *)NULL);
            break;
          case en_writeFile:
            m_FSCSComm.writeFileResponse(IsoAgLib::fsFileserverNotResponding, 0);
            break;
          case en_closeFile:
            m_FSCSComm.closeFileResponse(IsoAgLib::fsFileserverNotResponding);
            break;
          case en_moveFile:
            m_FSCSComm.moveFileResponse(IsoAgLib::fsFileserverNotResponding);
            break;
          case en_deleteFile:
            m_FSCSComm.deleteFileResponse(IsoAgLib::fsFileserverNotResponding);
            break;
          case en_getFileAttributes:
            m_FSCSComm.getFileAttributesResponse(IsoAgLib::fsFileserverNotResponding, false, false, false, false, false, false, false);
            break;
          case en_setFileAttributes:
            m_FSCSComm.setFileAttributesResponse(IsoAgLib::fsFileserverNotResponding);
            break;
          case en_getFileDateTime:
            m_FSCSComm.getFileDateTimeResponse(IsoAgLib::fsFileserverNotResponding, 0, 0, 0, 0, 0, 0);
            break;
          case en_initializeVolume:
            m_FSCSComm.initializeVolumeResponse(IsoAgLib::fsFileserverNotResponding, false, false, false, false, false, false, false);
            break;
          default:
  #if DEBUG_FILESERVER
            INTERNAL_DEBUG_DEVICE << "Repetition of command not defined!" << INTERNAL_DEBUG_DEVICE_ENDL;
  #endif
            break;
        }

        m_receivedResponse = true;
        en_lastCommand = en_noCommand;
        m_offset = 0;
      }
    }
  }

  // do we have open files or request is active? -> send maintenance messages.
  if ( (m_nrOpenFiles > 0) || !m_receivedResponse || m_keepConnectionOpen )
  { // Do Alive Sending
    if ( (m_lastAliveSentTime == -1) || ( (HAL::getTime () - (uint32_t)m_lastAliveSentTime) > 2000 ) )
    {
      CanPkgExt_c canpkgext;
      canpkgext.setExtCanPkg8(0x07, 0x00, CLIENT_TO_FS_PGN >> 8,
                              getFileserver().getIsoItem().nr(),
                              m_FSCSComm.getClientIdentItem().getIsoItem()->nr(),
                              m_maintenanceMsgBuf[0], m_maintenanceMsgBuf[1], m_maintenanceMsgBuf[2],
                              m_maintenanceMsgBuf[3], m_maintenanceMsgBuf[4], m_maintenanceMsgBuf[5],
                              m_maintenanceMsgBuf[6], m_maintenanceMsgBuf[7]);

      getIsoBusInstance4Comm() << canpkgext;
      m_lastAliveSentTime = HAL::getTime();
    }
  }
  else
  { // No Alive Sending
    if (m_lastAliveSentTime != -1)
    { // there was Alive Sending before
      doCleanUp();
      // reset timestamp so that at next timeEvent the Alive is
      // being sent out and not delayed until lastAliveSent+2000
      // and the doCleanUp() is only done once.
      m_lastAliveSentTime = -1;
    }
  }
}


bool
FsCommand_c::processPartStreamDataChunk (Stream_c& stream, bool isFirstChunk, bool isLastChunkAndACKd)
{
  if (stream.getStreamInvalid())
    return false; // don't keep the stream, we've "processed" it, so remove it (if it's the last chunk!)

  if (isFirstChunk)
  {
    m_offset = 0;
    uint8_t tan = stream.get();
    if (tan != m_tan)
    {
      stream.setStreamInvalid();
#if DEBUG_FILESERVER
      INTERNAL_DEBUG_DEVICE << "TAN does not match expected one!" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
      return false; // return value won't be interpreted on first chunk, so don't care...
    }
  }
  // else: further chunks - no checks...

  uint16_t ui16_notParsedSize
    = stream.getNotParsedSize();

  if ( (uint32_t)(m_offset + ui16_notParsedSize) > m_multireceiveMsgBufAllocSize )
  {
    stream.setStreamInvalid();
#if DEBUG_FILESERVER
    INTERNAL_DEBUG_DEVICE << "More data than allocated! " << m_multireceiveMsgBufAllocSize << " " << uint32_t(m_offset + ui16_notParsedSize) << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
    return false; // don't keep the stream, we've processed it right now, so remove it
  }

  for (uint8_t i = 0; i < ui16_notParsedSize; ++i)
    m_multireceiveMsgBuf[i + m_offset] = stream.get();

  m_offset += ui16_notParsedSize;

  if (isLastChunkAndACKd)
  {
    m_receivedResponse = true;
    switch ( stream.getFirstByte() )
    {
      case en_getCurrentDirectory:
        decodeGetCurrentDirectoryResponse();
        m_FSCSComm.getCurrentDirectoryResponse(IsoAgLib::iFsError(m_errorCode), m_currentDirectory);
        m_offset = 0;
        en_lastCommand = en_noCommand;

        // GetCurDir response MAY indicate a ready FsCSC
        // so notify the FsCSC (which will catch double notifies)
        m_FSCSComm.notifyOnFsReady();

        return false; // don't keep the stream, we've processed it right now, so remove it

      case en_readFile:
        en_lastCommand = en_noCommand;
        if ( m_readDirectory )
        {
          decodeReadDirectoryResponse();
          if (m_initializingFileserver)
          {
            closeFile(m_fileHandle);
          }
          else
          {
            m_FSCSComm.readDirectoryResponse(IsoAgLib::iFsError(m_errorCode), m_dirData);
          }
        }
        else
        {
          decodeReadFileResponse();
          m_FSCSComm.readFileResponse(IsoAgLib::iFsError(m_errorCode), m_count, m_data);
        }
        m_offset = 0;
        return false; // don't keep the stream, we've processed it right now, so remove it
      default:
        return false; // don't keep the stream, we've processed it right now, so remove it
    }
  }
  return false; // don't keep the stream, we've processed it right now, so remove it
}


void
FsCommand_c::reactOnAbort (Stream_c& )
{
  // nothing to be done, FS will hopefully send a retry.
  // if not, we will retry after a timeout, too.
}


bool
FsCommand_c::reactOnStreamStart(const ReceiveStreamIdentifier_c& /*streamIdent*/, uint32_t totalLen)
{
  const uint32_t newSize = totalLen - 1; // -1 for "FirstByte" that's already read.

  if (newSize > m_multireceiveMsgBufAllocSize)
  {
    if ( m_multireceiveMsgBuf != NULL )
      delete [] m_multireceiveMsgBuf;

    m_multireceiveMsgBufAllocSize = newSize;
    m_multireceiveMsgBuf = new uint8_t[m_multireceiveMsgBufAllocSize];
  }
  return true;
}


void
FsCommand_c::processMsgIso( const CanPkgExt_c& pkg )
{
  if( pkg.getMonitorItemForSA() != &getFileserver().getIsoItem() )
    return;

  if( pkg.getMonitorItemForDA() != m_FSCSComm.getClientIdentItem().getIsoItem() )
    return;

  if (pkg.getUint8Data(1) != m_tan && pkg.getUint8Data(0) != en_requestProperties)
  {
#if DEBUG_FILESERVER
      INTERNAL_DEBUG_DEVICE << "TAN does not match expected one!" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
    return;
  }

  switch (pkg.getUint8Data(0))
  {
    case en_requestProperties:
      if (m_initializingFileserver)
      {
        const FsServerInstance_c::FsVersion_en ce_versionNumber =
          FsServerInstance_c::FsVersion_en(pkg.getUint8Data(1));
        const uint8_t cui8_maxSimOpenFiles = pkg.getUint8Data(2);
        const uint8_t cui8_fsCapabilities = pkg.getUint8Data(3);
        // Check if that FS is okay for us to operate...
        if (ce_versionNumber >= FsServerInstance_c::FsVersionFDIS) // currently only don't support DIS fileservers...
        { // supported FileServer.
          getFileserver().setFsProperties (ce_versionNumber, cui8_maxSimOpenFiles, cui8_fsCapabilities);
          m_receivedResponse = true;
          ++m_tan;
          openFile((uint8_t *)"\\\\", false, false, false, true, false, true);
          return;
        }
        else
        { // Unsupported old version of the FileServer.
          getFileserver().setState (FsServerInstance_c::unusable);
        }
      }
      // else: not initializing, ignore such a message
      return;

    case en_changeCurrentDirectory:
      m_receivedResponse = true;
      ++m_tan;
      m_FSCSComm.changeCurrentDirectoryResponse(IsoAgLib::iFsError(pkg.getUint8Data(2)), m_fileName);
      en_lastCommand = en_noCommand;
      return;

    case en_openFile:
      decodeOpenFileResponse( pkg );

      if (m_errorCode == IsoAgLib::fsSuccess)
        ++m_nrOpenFiles;

      if (m_initializingFileserver)
      {
        seekFile(m_fileHandle, 2, 0);
        return;
      }

      m_FSCSComm.openFileResponse(IsoAgLib::iFsError(m_errorCode), m_fileHandle, m_attrCaseSensitive, m_attrRemovable, m_attrLongFilenames, m_attrIsDirectory,  m_attrIsVolume, m_attrHidden, m_attrReadOnly);
      en_lastCommand = en_noCommand;
      return;

    case en_seekFile:
      // in case of "isBeingInitialized" two Seek commands are being executed!
      //init case get volumes or real external seek file?
      if (m_initializingFileserver && m_positionMode == 0)
      {
        m_receivedResponse = true;
        ++m_tan;
	      if (m_position == 0)
        {
#if DEBUG_FILESERVER
	        INTERNAL_DEBUG_DEVICE << "exploratory seek failed: using read length of 16\n" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
	        readDirectory(m_fileHandle, 16, true);
   	    }
     	  else
          readDirectory(m_fileHandle, m_position, true);

        return;
      }

      decodeSeekFileResponse( pkg );

      //init case get volumes or real external seek file?
      if (m_initializingFileserver && m_positionMode == 2)
      {
        seekFile(m_fileHandle, 0, 0);
      }
      else
      {
        m_FSCSComm.seekFileResponse(IsoAgLib::iFsError(m_errorCode), m_position);
        en_lastCommand = en_noCommand;
      }

      return;

    case en_readFile:
      if (6 > m_multireceiveMsgBufAllocSize)
      {
        if ( m_multireceiveMsgBuf != NULL )
          delete [] m_multireceiveMsgBuf;

        m_multireceiveMsgBufAllocSize = 6;
        m_multireceiveMsgBuf = new uint8_t[m_multireceiveMsgBufAllocSize];
      }

      for (uint8_t i = 0; i < 6; ++i)
        m_multireceiveMsgBuf[i] = pkg.getUint8Data(i + 2);

      en_lastCommand = en_noCommand;
      if ( m_readDirectory )
      {
        decodeReadDirectoryResponse();
        if (m_initializingFileserver)
        {
          closeFile(m_fileHandle);
        }
        else
        {
          m_FSCSComm.readDirectoryResponse(IsoAgLib::iFsError(m_errorCode), m_dirData);
        }
      }
      else
      {
        decodeReadFileResponse();
        m_FSCSComm.readFileResponse(IsoAgLib::iFsError(m_errorCode), m_count, m_data);
      }
      return;

    case en_writeFile:
      m_receivedResponse = true;
      ++m_tan;
      m_FSCSComm.writeFileResponse(IsoAgLib::iFsError(pkg.getUint8Data(2)), (pkg.getUint8Data(3) | pkg.getUint8Data(4) << 0x08));
      en_lastCommand = en_noCommand;
      return;

    case en_closeFile:
      m_errorCode = pkg.getUint8Data(2);
      m_receivedResponse = true;
      ++m_tan;

      if ( (m_errorCode == IsoAgLib::fsSuccess) && (m_nrOpenFiles > 0) )
        --m_nrOpenFiles;

      //init case get volumes or real external seek file?
      if (m_initializingFileserver)
      {
        getFileserver().setVolumes(m_dirData);
        getFileserver().setState (FsServerInstance_c::usable);
        return;
      }

      m_FSCSComm.closeFileResponse(IsoAgLib::iFsError(m_errorCode));
      en_lastCommand = en_noCommand;
      return;

    case en_moveFile:
      m_receivedResponse = true;
      ++m_tan;
      m_FSCSComm.moveFileResponse(IsoAgLib::iFsError(pkg.getUint8Data(2)));
      en_lastCommand = en_noCommand;
      return;

    case en_deleteFile:
      ++m_tan;
      m_receivedResponse = true;
      en_lastCommand = en_noCommand;
      m_FSCSComm.deleteFileResponse(IsoAgLib::iFsError(pkg.getUint8Data(2)));
      return;

    case en_getFileAttributes:
      ++m_tan;
      m_receivedResponse = true;
      decodeAttributes(pkg.getUint8Data(3));
      m_FSCSComm.getFileAttributesResponse(IsoAgLib::iFsError(pkg.getUint8Data(2)), m_attrCaseSensitive, m_attrRemovable, m_attrLongFilenames, m_attrIsDirectory,  m_attrIsVolume, m_attrHidden, m_attrReadOnly);
      en_lastCommand = en_noCommand;
      return;

    case en_setFileAttributes:
      ++m_tan;
      m_receivedResponse = true;
      m_FSCSComm.setFileAttributesResponse(IsoAgLib::iFsError(pkg.getUint8Data(2)));
      en_lastCommand = en_noCommand;
      return;

    case en_getFileDateTime:
      {
        m_receivedResponse = true;
        ++m_tan;

        uint16_t date = pkg.getUint8Data(3) | (pkg.getUint8Data(4) << 8);
        uint16_t time = pkg.getUint8Data(5) | (pkg.getUint8Data(6) << 8);

        m_FSCSComm.getFileDateTimeResponse(IsoAgLib::iFsError(pkg.getUint8Data(2)), (uint16_t)(1980 + ((date >> 9) & 0x7F)), (date >> 5) & 0xF, (date) & 0x1F, (time >> 11) & 0x1F, (time >> 5) & 0x3F, 2 * ((time) & 0x1F));
        en_lastCommand = en_noCommand;
        return;
      }

    case en_initializeVolume:
      m_receivedResponse = true;
      ++m_tan;

      decodeAttributes(pkg.getUint8Data(3));

      m_FSCSComm.initializeVolumeResponse(IsoAgLib::iFsError(pkg.getUint8Data(2)), m_attrCaseSensitive, m_attrRemovable, m_attrLongFilenames, m_attrIsDirectory,  m_attrIsVolume, m_attrHidden, m_attrReadOnly);
      en_lastCommand = en_noCommand;
      return;
    default:
#if DEBUG_FILESERVER
      INTERNAL_DEBUG_DEVICE << "got message with content (decimal): " << (uint32_t)pkg.getUint8Data(0) << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
      return;
  }
}


void
FsCommand_c::sendRequest (RequestType_en requestType)
{
  switch (requestType)
  {
    case RequestInitial:
      m_requestAttempts = 1;
      break;

    case RequestRetry:
      ++m_requestAttempts;
      break;

    default:
      isoaglib_assert (!"INTERNAL-FAILURE: Wrong RequestType. Shouldn't happen!");
  }

#if DEBUG_FILESERVER
  INTERNAL_DEBUG_DEVICE << "Sending Request " << int(m_sendMsgBuf[0]) << " -> Fileserver [Try No. " << int(m_requestAttempts) << "]." << INTERNAL_DEBUG_DEVICE_ENDL;
#endif

  if (m_packetLength <= 8)
  {
    sendSinglePacket();
    m_lastrequestAttemptTime = HAL::getTime();
  }
  else
  {
    sendMultiPacketTry();
    m_waitForMultiSendFinish = true;
    // -1: Don't check timeout, wait for completion of MultiPacket first...
    m_lastrequestAttemptTime = -1;
  }

  m_receivedResponse = false;
}


void
FsCommand_c::sendSinglePacket()
{
  CanPkgExt_c isoPkg;
  isoPkg.setExtCanPkg8(
    0x07,
    0x00,
    CLIENT_TO_FS_PGN >> 8,
    getFileserver().getIsoItem().nr(),
    m_FSCSComm.getClientIdentItem().getIsoItem()->nr(),
    m_sendMsgBuf[0], m_sendMsgBuf[1], m_sendMsgBuf[2], m_sendMsgBuf[3],
    m_sendMsgBuf[4], m_sendMsgBuf[5], m_sendMsgBuf[6], m_sendMsgBuf[7]);

  getIsoBusInstance4Comm() << isoPkg;
}


void
FsCommand_c::sendMultiPacketTry()
{
  m_retryMultiPacketSend =
    !getMultiSendInstance4Comm().sendIsoTarget(
      m_FSCSComm.getClientIdentItem().getIsoItem()->isoName(),
      getFileserver().getIsoName(),
      m_sendMsgBuf,
      m_packetLength,
      CLIENT_TO_FS_PGN,
      &m_multiSendEventHandler);
}


void
FsCommand_c::requestProperties()
{
  en_lastCommand = en_requestProperties;

  m_sendMsgBuf[0] = en_requestProperties;
  m_sendMsgBuf[1] = 0xFF;
  m_sendMsgBuf[2] = 0xFF;
  m_sendMsgBuf[3] = 0xFF;
  m_sendMsgBuf[4] = 0xFF;
  m_sendMsgBuf[5] = 0xFF;
  m_sendMsgBuf[6] = 0xFF;
  m_sendMsgBuf[7] = 0xFF;

  m_packetLength = 8;

  sendRequest (RequestInitial);
}


void
FsCommand_c::setKeepConnectionOpen( bool keepOpen, bool forceClose )
{
  m_keepConnectionOpen = keepOpen;
  if ( !keepOpen && forceClose )
  {
    /* connection is being force closed, clear out number of open files */
    m_nrOpenFiles = 0;
  }
}


IsoAgLib::iFsCommandErrors
FsCommand_c::getCurrentDirectory()
{
  en_lastCommand = en_getCurrentDirectory;

  m_sendMsgBuf[0] = en_getCurrentDirectory;
  m_sendMsgBuf[1] = m_tan;
  m_sendMsgBuf[2] = 0xFF;
  m_sendMsgBuf[3] = 0xFF;
  m_sendMsgBuf[4] = 0xFF;
  m_sendMsgBuf[5] = 0xFF;
  m_sendMsgBuf[6] = 0xFF;
  m_sendMsgBuf[7] = 0xFF;

  m_packetLength = 8;

  sendRequest (RequestInitial);
  return IsoAgLib::fsCommandNoError;
}


IsoAgLib::iFsCommandErrors
FsCommand_c::changeCurrentDirectory(uint8_t *newDirectory)
{
  en_lastCommand = en_changeCurrentDirectory;

  uint16_t ui16_length = CNAMESPACE::strlen((const char*)newDirectory);
  uint8_t ui8_bufferPosition = 0;
  if ((ui16_length + 1) > m_fileNameAllocSize)
  {
    if (m_fileName != NULL)
      delete [] m_fileName;
    m_fileNameAllocSize = ui16_length + 1;
    m_fileName = new uint8_t[m_fileNameAllocSize];
  }
  m_fileName[ui16_length] = 0;

  for (uint16_t i = 0; i < ui16_length; ++i)
    m_fileName[i] = newDirectory[i];

  m_sendMsgBuf[ui8_bufferPosition++] = en_changeCurrentDirectory;
  m_sendMsgBuf[ui8_bufferPosition++] = m_tan;
  m_sendMsgBuf[ui8_bufferPosition++] = static_cast<uint8_t>(ui16_length);
  m_sendMsgBuf[ui8_bufferPosition++] = static_cast<uint8_t>(ui16_length >> 8);

  for (uint16_t i = 0; i < ui16_length; ++i)
    m_sendMsgBuf[ui8_bufferPosition + i] = m_fileName[i];

  m_packetLength = ui8_bufferPosition + ui16_length;

  sendRequest (RequestInitial);

  return IsoAgLib::fsCommandNoError;
}


IsoAgLib::iFsCommandErrors
FsCommand_c::openFile(uint8_t *fileName, bool openExclusive, bool openForAppend, bool createNewFile, bool openForReading, bool openForWriting, bool openDirectory)
{
  if (en_lastCommand == en_noCommand)
    en_lastCommand = en_openFile;

  uint16_t ui16_length = CNAMESPACE::strlen((const char*)fileName);
  if ((ui16_length + 1) > m_fileNameAllocSize)
  {
    if (m_fileName != NULL)
      delete [] m_fileName;
    m_fileNameAllocSize = ui16_length + 1;
    m_fileName = new uint8_t[m_fileNameAllocSize];
  }
  m_fileName[ui16_length] = 0;
  uint8_t ui8_bufferPosition = 0;

  m_sendMsgBuf[ui8_bufferPosition++] = en_openFile;
  m_sendMsgBuf[ui8_bufferPosition++] = m_tan;

  uint8_t openFlags = 0x0;

  if (openExclusive)
  {
    openFlags = openFlags | (0x1 << 4);
  }
  if (openForAppend)
  {
    openFlags = openFlags | (0x1 << 3);
  }
  if (createNewFile)
  {
    openFlags = openFlags | (0x1 << 2);
  }

  if (openDirectory)
  {
    openFlags = openFlags | 0x3;
  }
  else if (openForReading && openForWriting)
  {
    openFlags = openFlags | 0x2;
  }
  else if (openForReading)
  {
  }
  else if (openForWriting)
  {
    openFlags = openFlags | 0x1;
  }
  else
  {
    return IsoAgLib::fsCommandWrongFlag;
  }

  m_sendMsgBuf[ui8_bufferPosition++] = openFlags;
  m_sendMsgBuf[ui8_bufferPosition++] = (0xFFu & ui16_length);
  m_sendMsgBuf[ui8_bufferPosition++] = ui16_length >> 8;

  for (uint16_t i = 0; (i < ui16_length) || (5 + i < 8); ++i)
  {
    if (i < ui16_length)
    {
      m_fileName[i] = fileName[i];
      m_sendMsgBuf[ui8_bufferPosition + i] = m_fileName[i];
    }
    else
      m_sendMsgBuf[ui8_bufferPosition + i] = 0xff;
  }

  m_packetLength = ui8_bufferPosition + ui16_length;

  sendRequest (RequestInitial);

  return IsoAgLib::fsCommandNoError;
}


IsoAgLib::iFsCommandErrors
FsCommand_c::seekFile(uint8_t fileHandle, uint8_t positionMode, int32_t offset)
{
  if (en_lastCommand == en_noCommand)
    en_lastCommand = en_seekFile;

  m_fileHandle = fileHandle;
  m_positionMode = positionMode;
  m_offset = offset;

  m_sendMsgBuf[0] = en_seekFile;
  m_sendMsgBuf[1] = m_tan;
  m_sendMsgBuf[2] = m_fileHandle;
  m_sendMsgBuf[3] = m_positionMode;
  m_sendMsgBuf[4] = m_offset;
  m_sendMsgBuf[5] = m_offset >> 0x08;
  m_sendMsgBuf[6] = m_offset >> 0x10;
  m_sendMsgBuf[7] = m_offset >> 0x18;

  m_packetLength = 8;

  sendRequest (RequestInitial);

  return IsoAgLib::fsCommandNoError;
}


IsoAgLib::iFsCommandErrors
FsCommand_c::readFile(uint8_t fileHandle, uint16_t count)
{
  m_readDirectory = false;
  return readFile(fileHandle, count, false);
}


IsoAgLib::iFsCommandErrors
FsCommand_c::readDirectory(uint8_t fileHandle, uint16_t count, bool reportHiddenFiles)
{
  m_readDirectory = true;
  return readFile(fileHandle, count, reportHiddenFiles);
}


IsoAgLib::iFsCommandErrors
FsCommand_c::writeFile(uint8_t fileHandle, uint16_t count, uint8_t *data)
{
  en_lastCommand = en_writeFile;

  m_fileHandle = fileHandle;

  m_sendMsgBuf[0] = en_writeFile;
  m_sendMsgBuf[1] = m_tan;
  m_sendMsgBuf[2] = m_fileHandle;
  m_sendMsgBuf[3] = static_cast<uint8_t>(count);
  m_sendMsgBuf[4] = static_cast<uint8_t>(count >> 8);

  for (uint16_t ui16_writeDataSz = 0; ui16_writeDataSz < count; ++ui16_writeDataSz)
  {
    m_sendMsgBuf[ui16_writeDataSz + 5] = data[ui16_writeDataSz];
  }

  m_packetLength = count + 5;

  sendRequest (RequestInitial);

  return IsoAgLib::fsCommandNoError;
}


IsoAgLib::iFsCommandErrors
FsCommand_c::closeFile(uint8_t fileHandle)
{
  en_lastCommand = en_closeFile;

  m_fileHandle = fileHandle;

  m_sendMsgBuf[0] = en_closeFile;
  m_sendMsgBuf[1] = m_tan;
  m_sendMsgBuf[2] = m_fileHandle;
  m_sendMsgBuf[3] = 0xFF;
  m_sendMsgBuf[4] = 0xFF;
  m_sendMsgBuf[5] = 0xFF;
  m_sendMsgBuf[6] = 0xFF;
  m_sendMsgBuf[7] = 0xFF;

  m_packetLength = 8;

  sendRequest (RequestInitial);
  return IsoAgLib::fsCommandNoError;
}


IsoAgLib::iFsCommandErrors
FsCommand_c::moveFile(uint8_t *sourceName, uint8_t *destName, bool recursive, bool force, bool copy)
{
  uint8_t ui8_bufferPosition = 0;

  en_lastCommand = en_moveFile;

  m_sendMsgBuf[ui8_bufferPosition++] = en_moveFile;
  m_sendMsgBuf[ui8_bufferPosition++] = m_tan;

  uint8_t fileHandleMode = 0x00;

  if (recursive)
    fileHandleMode = fileHandleMode | 0x04;
  if (force)
    fileHandleMode = fileHandleMode | 0x02;
  if (copy)
    fileHandleMode = fileHandleMode | 0x01;

  m_sendMsgBuf[ui8_bufferPosition++] = fileHandleMode;

  uint16_t ui16_srcLength = CNAMESPACE::strlen((const char *)sourceName);
  uint16_t ui16_destLength = CNAMESPACE::strlen((const char *)destName);

  m_sendMsgBuf[ui8_bufferPosition++] = (0xFFu & ui16_srcLength);
  m_sendMsgBuf[ui8_bufferPosition++] = ui16_srcLength >> 0x08;
  m_sendMsgBuf[ui8_bufferPosition++] = (0xFFu & ui16_destLength);
  m_sendMsgBuf[ui8_bufferPosition++] = ui16_destLength >> 0x08;

  for (uint16_t ui16_iSrc = 0; ui16_iSrc < ui16_srcLength; ++ui16_iSrc)
    m_sendMsgBuf[ui8_bufferPosition + ui16_iSrc] = sourceName[ui16_iSrc];

  for (uint16_t ui16_iDst = 0; ui16_iDst < ui16_destLength; ++ui16_iDst)
    m_sendMsgBuf[ui8_bufferPosition + ui16_srcLength + ui16_iDst] = destName[ui16_iDst];

  m_packetLength = ui8_bufferPosition + ui16_srcLength + ui16_destLength;

  sendRequest (RequestInitial);
  return IsoAgLib::fsCommandNoError;
}


IsoAgLib::iFsCommandErrors
FsCommand_c::deleteFile (uint8_t *sourceName, bool recursive, bool force)
{
  uint8_t ui8_bufferPosition = 0;

  en_lastCommand = en_deleteFile;

  m_sendMsgBuf[ui8_bufferPosition++] = en_deleteFile;
  m_sendMsgBuf[ui8_bufferPosition++] = m_tan;

  uint8_t fileHandleMode = 0x00;

  if (recursive)
    fileHandleMode = fileHandleMode | 0x04;
  if (force)
    fileHandleMode = fileHandleMode | 0x02;

  m_sendMsgBuf[ui8_bufferPosition++] = fileHandleMode;

  uint16_t ui16_srcLength = CNAMESPACE::strlen((const char *)sourceName);

  m_sendMsgBuf[ui8_bufferPosition++] = (0xFFu & ui16_srcLength);
  m_sendMsgBuf[ui8_bufferPosition++] = ui16_srcLength >> 8;

  for (uint16_t ui16_iSrc = 0; ui16_iSrc < ui16_srcLength; ++ui16_iSrc)
    m_sendMsgBuf[ui8_bufferPosition + ui16_iSrc] = sourceName[ui16_iSrc];

  m_packetLength = ui8_bufferPosition + ui16_srcLength;

  sendRequest (RequestInitial);
  return IsoAgLib::fsCommandNoError;
}

IsoAgLib::iFsCommandErrors
FsCommand_c::getFileAttributes(uint8_t *sourceName)
{
  en_lastCommand = en_getFileAttributes;

  m_sendMsgBuf[0] = en_getFileAttributes;
  m_sendMsgBuf[1] = m_tan;

  uint16_t ui16_srcLength = CNAMESPACE::strlen((const char *)sourceName);

  m_sendMsgBuf[2] = static_cast<uint8_t>(ui16_srcLength);
  m_sendMsgBuf[3] = static_cast<uint8_t>(ui16_srcLength >> 8);

  for (uint16_t ui16_iSrc = 0; ui16_iSrc < ui16_srcLength; ++ui16_iSrc)
    m_sendMsgBuf[4 + ui16_iSrc] = sourceName[ui16_iSrc];

  m_packetLength = 4 + ui16_srcLength;

  sendRequest (RequestInitial);
  return IsoAgLib::fsCommandNoError;
}

IsoAgLib::iFsCommandErrors
FsCommand_c::setFileAttributes(uint8_t *sourceName, uint8_t hiddenAttr, uint8_t readOnlyAttr)
{
  en_lastCommand = en_setFileAttributes;

  m_sendMsgBuf[0] = en_setFileAttributes;
  m_sendMsgBuf[1] = m_tan;

  m_sendMsgBuf[2] = 0xF0 | hiddenAttr << 2 | readOnlyAttr;

  uint16_t ui16_srcLength = CNAMESPACE::strlen((const char *)sourceName);

  m_sendMsgBuf[3] = static_cast<uint8_t>(ui16_srcLength);
  m_sendMsgBuf[4] = static_cast<uint8_t>(ui16_srcLength >> 8);

  for (uint16_t ui16_iSrc = 0; ui16_iSrc < ui16_srcLength; ++ui16_iSrc)
    m_sendMsgBuf[5 + ui16_iSrc] = sourceName[ui16_iSrc];

  m_packetLength = 5 + ui16_srcLength;

  sendRequest (RequestInitial);
  return IsoAgLib::fsCommandNoError;
}


IsoAgLib::iFsCommandErrors
FsCommand_c::getFileDateTime(uint8_t *sourceName)
{
  en_lastCommand = en_getFileDateTime;

  m_sendMsgBuf[0] = en_getFileDateTime;
  m_sendMsgBuf[1] = m_tan;

  uint16_t ui16_srcLength = CNAMESPACE::strlen((const char *)sourceName);

  m_sendMsgBuf[2] = static_cast<uint8_t>(ui16_srcLength);
  m_sendMsgBuf[3] = static_cast<uint8_t>(ui16_srcLength >> 8);

  for (uint16_t ui16_iSrc = 0; ui16_iSrc < ui16_srcLength; ++ui16_iSrc)
    m_sendMsgBuf[4 + ui16_iSrc] = sourceName[ui16_iSrc];

  m_packetLength = 4 + ui16_srcLength;

  sendRequest (RequestInitial);
  return IsoAgLib::fsCommandNoError;
}


IsoAgLib::iFsCommandErrors FsCommand_c::initializeVolume(
#if DEBUG_FILESERVER
  uint8_t *pathName, uint32_t space, bool createVolumeUsingSpace, bool createNewVolume
#else
  uint8_t * /*pathName*/, uint32_t /*space*/, bool /*createVolumeUsingSpace*/, bool /*createNewVolume*/
#endif
)
{
  en_lastCommand = en_initializeVolume;

//TODO implement this one...

#if DEBUG_FILESERVER
  INTERNAL_DEBUG_DEVICE << "NOT IMPLEMENTED initializeVolume pathname: " << pathName << " space: " << space << " attributes: " << createVolumeUsingSpace << " " << createNewVolume << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
  return IsoAgLib::fsCommandNoError;
}


void
FsCommand_c::decodeAttributes(uint8_t attributes)
{
  m_attrCaseSensitive = ((attributes & 0x80) != 0);
  m_attrRemovable = ((attributes & 0x40) == 0);
  m_attrLongFilenames = ((attributes & 0x20) != 0);
  m_attrIsDirectory = ((attributes & 0x10) != 0);
  m_attrIsVolume = ((attributes & 0x8) != 0);
  m_attrHidden = ((attributes & 0x2) != 0);
  m_attrReadOnly = ((attributes & 0x1) != 0);
}


void
FsCommand_c::decodeGetCurrentDirectoryResponse()
{
  uint16_t ui16_length;
  uint8_t *pui8_receivePointer;

  m_errorCode = m_multireceiveMsgBuf[0];

  ui16_length = m_multireceiveMsgBuf[9] | (m_multireceiveMsgBuf[10] << 8);
  pui8_receivePointer = &m_multireceiveMsgBuf[11];

  m_receivedResponse = true;
  ++m_tan;
  if ((ui16_length + 1) > m_currentDirectoryAllocSize)
  {
    if (m_currentDirectory != NULL)
      delete [] m_currentDirectory;
    m_currentDirectoryAllocSize = ui16_length + 1;
    m_currentDirectory = new uint8_t[m_currentDirectoryAllocSize];
  }
  m_currentDirectory[ui16_length] = 0;

  for (uint16_t i = 0; i < ui16_length; ++i)
  {
    m_currentDirectory[i] = *pui8_receivePointer;
    ++pui8_receivePointer;
  }
}


void
FsCommand_c::decodeOpenFileResponse( const CanPkg_c& arc_data )
{
  m_receivedResponse = true;
  ++m_tan;
  m_errorCode = arc_data.getUint8Data(2);
  m_fileHandle = arc_data.getUint8Data(3);
  decodeAttributes(arc_data.getUint8Data(4));
}


void
FsCommand_c::decodeSeekFileResponse( const CanPkg_c& arc_data )
{
  m_receivedResponse = true;
  ++m_tan;
  m_errorCode = arc_data.getUint8Data(2);

  m_position = static_cast<uint32_t>(arc_data.getUint8Data(4)) | (static_cast<uint32_t>(arc_data.getUint8Data(5)) << 0x08) | (static_cast<uint32_t>(arc_data.getUint8Data(6)) << 0x10) | (static_cast<uint32_t>(arc_data.getUint8Data(7)) << 0x18);
}


void
FsCommand_c::decodeReadFileResponse()
{
  m_receivedResponse = true;
  ++m_tan;

  m_errorCode = m_multireceiveMsgBuf[0];
  m_count = m_multireceiveMsgBuf[1] | (m_multireceiveMsgBuf[2] << 0x08);

  if (m_count > m_dataAllocSize)
  {
    if (m_data != NULL)
      delete [] m_data;
    m_dataAllocSize = m_count;
    m_data = new uint8_t[m_dataAllocSize];
  }

  if ( ((uint32_t)m_count + 3) > m_multireceiveMsgBufAllocSize )
  {
    m_count = m_multireceiveMsgBufAllocSize - 3;
#if DEBUG_FILESERVER
    INTERNAL_DEBUG_DEVICE << "message size larger then allocated buffer " << m_multireceiveMsgBufAllocSize << " " << uint32_t(m_count + 3) << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
  }

  for (uint16_t i = 0; i < m_count; ++i)
  {
    m_data[i] = m_multireceiveMsgBuf[i + 3];
  }
}


void
FsCommand_c::decodeReadDirectoryResponse()
{
  m_receivedResponse = true;
  ++m_tan;
  IsoAgLib::iFsDirectoryPtr ps_tmpDir;

  m_errorCode = m_multireceiveMsgBuf[0];

  clearDirectoryList();

  m_offset = 3;

  for (uint16_t ui16_nrEntries = 0; ui16_nrEntries < m_count; ++ui16_nrEntries)
  {
    if ((uint32_t)m_offset >= m_multireceiveMsgBufAllocSize)
      break;

    ps_tmpDir = new IsoAgLib::iFsDirectory();

    ps_tmpDir->pui8_filename = new uint8_t[m_multireceiveMsgBuf[m_offset] + 1];
    ps_tmpDir->pui8_filename[m_multireceiveMsgBuf[m_offset]] = 0;

    for (uint8_t ui8_nameLength = 0; ui8_nameLength < m_multireceiveMsgBuf[m_offset]; ++ui8_nameLength)
      ps_tmpDir->pui8_filename[ui8_nameLength] = m_multireceiveMsgBuf[ui8_nameLength + m_offset + 1];

    m_offset += (m_multireceiveMsgBuf[m_offset] + 1);
    if ((uint32_t)m_offset >= m_multireceiveMsgBufAllocSize)
      break;

    decodeAttributes(m_multireceiveMsgBuf[m_offset]);

    ps_tmpDir->b_caseSensitive = m_attrCaseSensitive;
    ps_tmpDir->b_removable = m_attrRemovable;
    ps_tmpDir->b_longFilenames = m_attrLongFilenames;
    ps_tmpDir->b_isDirectory = m_attrIsDirectory;
    ps_tmpDir->b_isVolume = m_attrIsVolume;
    ps_tmpDir->b_hidden = m_attrHidden;
    ps_tmpDir->b_readOnly = m_attrReadOnly;

    ++m_offset;
    if ((uint32_t)(m_offset + 1) >= m_multireceiveMsgBufAllocSize)
      break;

    ps_tmpDir->ui16_date = m_multireceiveMsgBuf[m_offset] | (m_multireceiveMsgBuf[m_offset + 1] << 0x08);

    m_offset += 2;
    if ((uint32_t)(m_offset + 1) >= m_multireceiveMsgBufAllocSize)
      break;

    ps_tmpDir->ui16_time = m_multireceiveMsgBuf[m_offset] | (m_multireceiveMsgBuf[m_offset + 1] << 0x08);

    m_offset += 2;
    if ((uint32_t)(m_offset + 3) >= m_multireceiveMsgBufAllocSize)
      break;

    ps_tmpDir->ui32_size = static_cast<uint32_t>(m_multireceiveMsgBuf[m_offset]) | (static_cast<uint32_t>(m_multireceiveMsgBuf[m_offset + 1]) << 0x08) | (static_cast<uint32_t>(m_multireceiveMsgBuf[m_offset + 2]) << 0x10) | (static_cast<uint32_t>(m_multireceiveMsgBuf[m_offset + 3]) << 0x18);

    m_offset += 4;

    m_dirData.push_back(ps_tmpDir);
  }
}


void
FsCommand_c::clearDirectoryList()
{
  IsoAgLib::iFsDirectoryPtr ps_tmpDir;
  while (!m_dirData.empty())
  {
    ps_tmpDir = m_dirData.back();
    m_dirData.pop_back();
    if (ps_tmpDir->pui8_filename != NULL)
      delete [] ps_tmpDir->pui8_filename;
    delete ps_tmpDir;
  }
}


void
FsCommand_c::doCleanUp()
{
  if (m_multireceiveMsgBuf != NULL)
  {
    delete [] m_multireceiveMsgBuf;
    m_multireceiveMsgBuf = NULL;
  }
  m_multireceiveMsgBufAllocSize = 0;

  if (m_currentDirectory != NULL)
  {
    delete [] m_currentDirectory;
    m_currentDirectory = NULL;
  }
  m_currentDirectoryAllocSize = 0;

  if (m_fileName != NULL)
  {
    delete [] m_fileName;
    m_fileName = NULL;
  }
  m_fileNameAllocSize = 0;

  if (m_data != NULL)
  {
    delete [] m_data;
    m_data = NULL;
  }
  m_dataAllocSize = 0;

  clearDirectoryList();
}


IsoAgLib::iFsCommandErrors
FsCommand_c::readFile(uint8_t fileHandle, uint16_t count, bool reportHiddenFiles)
{
  if (en_lastCommand == en_noCommand)
    en_lastCommand = en_readFile;

  m_fileHandle = fileHandle;
  m_count = count;
  uint8_t ui8_bufferPosition = 0;

  m_sendMsgBuf[ui8_bufferPosition++] = en_readFile;
  m_sendMsgBuf[ui8_bufferPosition++] = m_tan;
  m_sendMsgBuf[ui8_bufferPosition++] = m_fileHandle;
  m_sendMsgBuf[ui8_bufferPosition++] = static_cast<uint8_t>(m_count);
  m_sendMsgBuf[ui8_bufferPosition++] = static_cast<uint8_t>(m_count >> 8);

  m_sendMsgBuf[ui8_bufferPosition++] = reportHiddenFiles;

  while (ui8_bufferPosition < 8)
    m_sendMsgBuf[ui8_bufferPosition++] = 0xFF;

  m_packetLength = ui8_bufferPosition;

  sendRequest (RequestInitial);

  return IsoAgLib::fsCommandNoError;
}

void
FsCommand_c::reactOnStateChange(const SendStream_c& sendStream)
{
  m_sendSuccessNotify = sendStream.getSendSuccess();
}

} // __IsoAgLib
