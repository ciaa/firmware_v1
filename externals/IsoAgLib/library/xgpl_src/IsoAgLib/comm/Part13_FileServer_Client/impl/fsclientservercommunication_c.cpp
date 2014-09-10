/*
  fsclientservercommunication_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

// own
#include "fsclientservercommunication_c.h"
#include "fsserverinstance_c.h"
#include "fsmanager_c.h"

// ISOAgLib
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multireceive_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/streamlinear_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoitem_c.h>
#include <IsoAgLib/util/impl/util_funcs.h>

// own interfaces
#include "../ifsclientservercommunication_c.h"
#include "../ifsstructs.h"

// STL
#include <vector>


// Begin Namespace __IsoAgLib
namespace __IsoAgLib {

/** constructor to init client-server communication without fileserver */
FsClientServerCommunication_c::FsClientServerCommunication_c(
    IdentItem_c &rc_identItem,
    IsoAgLib::iFsClient_c &rc_fsClient,
    const IsoAgLib::iFsWhitelistList &v_inFsWhitelist
)
  : v_fsWhitelist( v_inFsWhitelist )
  , c_fsClient(rc_fsClient)
  , c_identItem( rc_identItem )
  , pui8_currentDirectory( NULL )
  , pc_commandHandler( NULL )
  , mb_finishedRegistering( false )
{
}

/** explicit conversion to reference of interface class type */
IsoAgLib::iFsClientServerCommunication_c*
FsClientServerCommunication_c::toInterfacePointer()
{
  return static_cast<IsoAgLib::iFsClientServerCommunication_c*>(this);
}

void
FsClientServerCommunication_c::processMsgIso( const CanPkgExt_c& pkg )
{
  if( pc_commandHandler == NULL )
    return;

  pc_commandHandler->processMsgIso( pkg );
}

void
FsClientServerCommunication_c::notifyOnUsableFileServer(FsServerInstance_c &rc_fsServerInstance)
{
  for (IsoAgLib::iFsWhitelistList::iterator it_whitelist = v_fsWhitelist.begin();
       it_whitelist != v_fsWhitelist.end();
       ++it_whitelist)
  { /// @todo #854 Properly compare removable request with volumes-list. Therefor enhance volume-list with volume-flags.
    if ( // (*it_whitelist).b_requestRemoveableMedium == rc_fsServerInstance.getSupportsMultiVolumes() &&
        ((*it_whitelist).i32_requestedManufacturer == rc_fsServerInstance.getIsoName().manufCode() ||
         (*it_whitelist).i32_requestedManufacturer == -1) &&
        (*it_whitelist).ui8_minSimultaniouslyOpenFiles <= rc_fsServerInstance.getMaxNrOpenFiles())
    {
      c_fsClient.finishedConnectingToFileserver (*rc_fsServerInstance.toInterfacePointer());
      break;
    }
  }
}


void
FsClientServerCommunication_c::notifyOnOfflineFileServer (FsServerInstance_c &rc_fsServerInstance)
{
  // check if we are connected to any fileserver at all
  if (pc_commandHandler == NULL)
  { // not connected, so offline going fileserver are not of interest
    return;
  }

  // check if this loss affects us...
  if (&getFileserver() != &rc_fsServerInstance)
    return;

  // notify the Application on lost FS
  c_fsClient.notifyOnOfflineFileServer (*(getFileserver().toInterfacePointer()));

  // now delete the FsCommand (incl. the reference to the connected FS)
  delete pc_commandHandler;
  pc_commandHandler = NULL;
}


void
FsClientServerCommunication_c::notifyOnFsReady()
{
  // only when this command is for initializing the FsCSC,
  if (!mb_finishedRegistering)
  { // call the API
    c_fsClient.fileserverReady();
    mb_finishedRegistering = true;
  }
}


void
FsClientServerCommunication_c::requestFsConnection(FsServerInstance_c &rc_fileServer)
{
  // catch double call!
  if (pc_commandHandler)
    return;

  // "connect" to the given FsServerInstance_c
  pc_commandHandler = new FsCommand_c(*this, rc_fileServer);

  /// (currently only Get Current Directory is performed)
  pc_commandHandler->getCurrentDirectory();
}


/// /// Explicit FS disconnect
/// /// @todo On request (if explicit disconnecting is requested) - need to make sure what happens afterwards
/// /// (automatic connection to a new one/the same one? etc.)
/// FsClientServerCommunication_c::disconnectFsConnection()
/// {
///   [...]
///   mb_finishedRegistering = false;
///   [...]
/// }


/// FileServer access functions
    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::changeCurrentDirectory(uint8_t *pui8_newDirectory)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->changeCurrentDirectory(pui8_newDirectory);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::openFile(uint8_t *pui8_fileName, bool b_openExclusive, bool b_openForAppend, bool b_createNewFile, bool b_openForReading, bool b_openForWriting, bool b_openDirectory)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->openFile(pui8_fileName, b_openExclusive, b_openForAppend, b_createNewFile, b_openForReading, b_openForWriting, b_openDirectory);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::seekFile(uint8_t ui8_fileHandle, uint8_t ui8_possitionMode, int32_t i32_offset)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->seekFile(ui8_fileHandle, ui8_possitionMode, i32_offset);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::readFile(uint8_t ui8_fileHandle, uint16_t ui16_count)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->readFile(ui8_fileHandle, ui16_count);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::readDirectory(uint8_t ui8_fileHandle, uint16_t ui16_count, bool b_reportHiddenFiles)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->readDirectory(ui8_fileHandle, ui16_count, b_reportHiddenFiles);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::writeFile(uint8_t ui8_fileHandle, uint16_t ui16_count, uint8_t *pui8_data)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->writeFile(ui8_fileHandle, ui16_count, pui8_data);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::closeFile(uint8_t ui8_fileHandle)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->closeFile(ui8_fileHandle);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::moveFile(uint8_t *pui8_sourceName, uint8_t *pui8_destName, bool b_recursive, bool b_force, bool b_copy)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->moveFile(pui8_sourceName, pui8_destName, b_recursive, b_force, b_copy);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::deleteFile(uint8_t *pui8_fileName, bool b_recursive, bool b_force)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->deleteFile(pui8_fileName, b_recursive, b_force);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::getFileAttributes(uint8_t *pui8_fileName)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->getFileAttributes(pui8_fileName);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::setFileAttributes(uint8_t *pui8_fileName, uint8_t ui8_hiddenAtt, uint8_t ui8_readOnlyAtt)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->setFileAttributes(pui8_fileName, ui8_hiddenAtt, ui8_readOnlyAtt);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::getFileDateTime(uint8_t *pui8_fileName)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->getFileDateTime(pui8_fileName);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::initializeVolume(uint8_t *pui8_pathName, uint32_t ui32_space, bool b_createVolumeUsingSpace, bool b_createNewVolume)
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      if (pc_commandHandler->isBusy())
        return IsoAgLib::fsCommandBusy;
      else
        return pc_commandHandler->initializeVolume(pui8_pathName, ui32_space, b_createVolumeUsingSpace, b_createNewVolume);
    }

    IsoAgLib::iFsCommandErrors
    FsClientServerCommunication_c::setKeepConnectionOpen( bool b_keepOpen, bool b_forceClose )
    {
      if (!pc_commandHandler)
        return IsoAgLib::fsCommandNotPressent;
      else
      {
        pc_commandHandler->setKeepConnectionOpen(b_keepOpen, b_forceClose);
        return IsoAgLib::fsCommandNoError;
      }
    }

    bool
    FsClientServerCommunication_c::getKeepConnectionOpen()
    {
      if (!pc_commandHandler)
        return false;
      else
        return pc_commandHandler->getKeepConnectionOpen();
    }

/// FileServer access functions END



    /// FileServer access response functions
    void
    FsClientServerCommunication_c::getCurrentDirectoryResponse(IsoAgLib::iFsError ui8_errorCode, uint8_t *piu8_newCurrentDirectory)
    {
      if (!ui8_errorCode)
      {
        uint16_t ui16_length = CNAMESPACE::strlen((char *)piu8_newCurrentDirectory);
        pui8_currentDirectory = new uint8_t[ui16_length + 1];
        pui8_currentDirectory[ui16_length] = 0;

        for (uint16_t i = 0; i < ui16_length; ++i)
        {
          pui8_currentDirectory[i] = piu8_newCurrentDirectory[i];
        }
      }
    }

    void
    FsClientServerCommunication_c::changeCurrentDirectoryResponse(IsoAgLib::iFsError ui8_errorCode, uint8_t *piu8_newCurrentDirectory)
    {
      if (!ui8_errorCode)
      {
        uint16_t ui16_length = CNAMESPACE::strlen((char *)piu8_newCurrentDirectory);
        pui8_currentDirectory = new uint8_t[ui16_length + 1];
        pui8_currentDirectory[ui16_length] = 0;

        for (uint16_t i = 0; i < ui16_length; ++i)
        {
          pui8_currentDirectory[i] = piu8_newCurrentDirectory[i];
        }
      }

      c_fsClient.changeCurrentDirectoryResponse(ui8_errorCode);
    }

    void
    FsClientServerCommunication_c::moveFileResponse(IsoAgLib::iFsError ui8_errorCode)
    {
      c_fsClient.moveFileResponse(ui8_errorCode);
    }

    void
    FsClientServerCommunication_c::deleteFileResponse(IsoAgLib::iFsError ui8_errorCode)
    {
      c_fsClient.deleteFileResponse(ui8_errorCode);
    }

    void
    FsClientServerCommunication_c::getFileAttributesResponse(IsoAgLib::iFsError ui8_errorCode,
                                   bool b_caseSensitive,
                                   bool b_removable,
                                   bool b_longFilenames,
                                   bool b_directory,
                                   bool b_volume,
                                   bool b_hidden,
                                   bool b_readOnly)
    {
      c_fsClient.getFileAttributesResponse(ui8_errorCode, b_caseSensitive, b_removable, b_longFilenames, b_directory, b_volume, b_hidden, b_readOnly);
    }

    void
    FsClientServerCommunication_c::setFileAttributesResponse(IsoAgLib::iFsError ui8_errorCode)
    {
      c_fsClient.setFileAttributesResponse(ui8_errorCode);
    }

    void
    FsClientServerCommunication_c::getFileDateTimeResponse(IsoAgLib::iFsError ui8_errorCode,
                                 uint16_t ui16_fileYear,
                                 uint8_t ui8_fileMonth,
                                 uint8_t ui8_fileDay,
                                 uint8_t ui8_fileHour,
                                 uint8_t ui8_fileMinute,
                                 uint8_t ui8_fileSecond)
    {
      c_fsClient.getFileDateTimeResponse(ui8_errorCode, ui16_fileYear, ui8_fileMonth, ui8_fileDay, ui8_fileHour, ui8_fileMinute, ui8_fileSecond);
    }

    void
    FsClientServerCommunication_c::initializeVolumeResponse(IsoAgLib::iFsError ui8_errorCode,
                                   bool b_caseSensitive,
                                   bool b_removable,
                                   bool b_longFilenames,
                                   bool b_directory,
                                   bool b_volume,
                                   bool b_hidden,
                                   bool b_readOnly)
    {
      c_fsClient.initializeVolumeResponse(ui8_errorCode, b_caseSensitive, b_removable, b_longFilenames, b_directory, b_volume, b_hidden, b_readOnly);
    }

    /// FileServer access response functions END

} // __IsoAgLib
