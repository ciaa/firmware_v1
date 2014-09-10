/*
  fsclientservercommunication_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef FSCLIETSERVERCOMMUNICATION_C_H
#define FSCLIETSERVERCOMMUNICATION_C_H

// ISOAgLib
#include <IsoAgLib/scheduler/ischedulertask_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/identitem_c.h>

// own
#include "fsserverinstance_c.h"
#include "fscommand_c.h"
#include "../ifsclient_c.h"


namespace IsoAgLib {
  class iFsClientServerCommunication_c;
}

// Begin Namespace __IsoAgLib
namespace __IsoAgLib {

/**
  * class FsClientServerCommunication_c, managing the communication betweent a fileserver client and
  * a fileserver. The CAN-BUS communication is done by the FsClientServerCommunication_c's FsResponse_c.
  */
class FsClientServerCommunication_c
{
  public:
    void processMsgIso( const CanPkgExt_c& pkg );

    /** getFileserver()
      * @pre There's a Fileserver/FsCommand set. (pc_commandHandler != NULL)
      *      (This is done when the Application is connecting to a FileServer)
      */
    FsServerInstance_c &getFileserver() { return pc_commandHandler->getFileserver(); }

    /**
      * Get method to request the current directory of the used fileserver.
      * @return the used fileserver's current directory.
      */
    uint8_t *getCurrentDirectory() { return pui8_currentDirectory; }

    /**
      * Indicates if the requested fileserver connection has been registered with success. The registration process is
      * considered to have been successfull, if the current directory and all volumes of the fileserver have been set.
      * @return true if filerserver registered with success, false else.
      */
    bool finishedRegistering() { return mb_finishedRegistering; }

    /**
      * Method used to get the used fileserver's existing volumes.
      * @pre This instance must be connected to a FileServer!
      * @return IsoAgLib::iFsDirList the volumes of the used fileserver.
      */
    IsoAgLib::iFsDirList getFsVolumes() { return pc_commandHandler->getFileserver().getFsVolumes(); }

    /** constructor to init client-server communication without fileserver*/
    FsClientServerCommunication_c(IdentItem_c &rc_identItem, IsoAgLib::iFsClient_c &rc_fsClient, const IsoAgLib::iFsWhitelistList &v_fsWhitelist);

    /** explicit conversion to reference of interface class type*/
    IsoAgLib::iFsClientServerCommunication_c* toInterfacePointer();

    /**
      * returns the client's IdentItem_c as const IdentItem_c
      */
    const IdentItem_c &getClientIdentItem() { return c_identItem; }

    /**
      * returns the iFsClient_c implementation used for this fileserver - client communication.
      */
    const IsoAgLib::iFsClient_c &getFsClient() { return c_fsClient; }

    /**
      * Method called by FsManager_c to notify FsClientServerCommunication_c on usable fileserver,
      * meaning the the properties/volumes have been queried already.
      * Information is passed directly to iFsClient_c implementation.
      */
    void notifyOnUsableFileServer (FsServerInstance_c &rc_fsServerInstance);

    /**
      * Method called by FsManager_c to notify all FsClientServerCommunication_c that
      * some fileserver has gone offline. Information is passed to the iFsClient_c
      * implementation as well.
      */
    void notifyOnOfflineFileServer (FsServerInstance_c &rc_fsServerInstance);

    /**
      * Method called to request conncetion to specified fileserver
      * @param rc_FileServer The fileserver for the requested connection.
      */
    void requestFsConnection (FsServerInstance_c &rc_FileServer);

    /// FileServer access functions as defined in iFsClientServerCommunication_c
    IsoAgLib::iFsCommandErrors getFileServerProperties();

    IsoAgLib::iFsCommandErrors changeCurrentDirectory(uint8_t *pui8_newDirectory);

    IsoAgLib::iFsCommandErrors openFile(uint8_t *pui8_fileName, bool b_openExclusive, bool b_openForAppend, bool b_createNewFile, bool b_openForReading, bool b_openForWriting, bool b_openDirectory);
    IsoAgLib::iFsCommandErrors seekFile(uint8_t ui8_fileHandle, uint8_t ui8_possitionMode, int32_t i32_offset);
    IsoAgLib::iFsCommandErrors readFile(uint8_t ui8_fileHandle, uint16_t ui16_count);
    IsoAgLib::iFsCommandErrors readDirectory(uint8_t ui8_fileHandle, uint16_t ui16_count, bool b_reportHiddenFiles);
    IsoAgLib::iFsCommandErrors writeFile(uint8_t ui8_fileHandle, uint16_t ui16_count, uint8_t *pui8_data);
    IsoAgLib::iFsCommandErrors closeFile(uint8_t ui8_fileHandle);

    IsoAgLib::iFsCommandErrors moveFile(uint8_t *pui8_sourceName, uint8_t *pui8_destName, bool b_recursive, bool b_force, bool b_copy);
    IsoAgLib::iFsCommandErrors deleteFile(uint8_t *pui8_fileName, bool b_recursive, bool b_force);
    IsoAgLib::iFsCommandErrors getFileAttributes(uint8_t *pui8_fileName);
    IsoAgLib::iFsCommandErrors setFileAttributes(uint8_t *pui8_fileName, uint8_t ui8_hiddenAtt, uint8_t ui8_readOnlyAtt);
    IsoAgLib::iFsCommandErrors getFileDateTime(uint8_t *pui8_fileName);

    IsoAgLib::iFsCommandErrors initializeVolume(uint8_t *pui8_pathName, uint32_t ui32_space, bool b_createVolumeUsingSpace, bool b_createNewVolume);

    IsoAgLib::iFsCommandErrors setKeepConnectionOpen( bool b_keepOpen, bool b_forceClose=false );
    bool getKeepConnectionOpen();
    /// FileServer access functions END

    /// FileServer access response functions as defined in iFsClient_c

    void getFileServerPropertiesResponse(uint8_t ui8_versionNumber, uint8_t ui8_maxNumberOpenFiles, bool b_fsMultiVolumes);

    void getCurrentDirectoryResponse(IsoAgLib::iFsError ui8_errorCode, uint8_t *piu8_currentDirectory);
    void changeCurrentDirectoryResponse(IsoAgLib::iFsError ui8_errorCode, uint8_t *piu8_newCurrentDirectory);

    void openFileResponse(IsoAgLib::iFsError ui8_errorCode,
                          uint8_t ui8_fileHandle,
                          bool b_caseSensitive,
                          bool b_removable,
                          bool b_longFilenames,
                          bool b_directory,
                          bool b_volume,
                          bool b_hidden,
                          bool b_readOnly)
    { c_fsClient.openFileResponse(ui8_errorCode, ui8_fileHandle, b_caseSensitive, b_removable, b_longFilenames, b_directory,  b_volume, b_hidden, b_readOnly); }
    void seekFileResponse(IsoAgLib::iFsError ui8_errorCode, uint32_t ui32_position)
    { c_fsClient.seekFileResponse(ui8_errorCode, ui32_position); }
    void readFileResponse(IsoAgLib::iFsError ui8_errorCode, uint16_t ui16_dataLength, uint8_t *pui8_data)
    { c_fsClient.readFileResponse(ui8_errorCode, ui16_dataLength, pui8_data); }
    void readDirectoryResponse(IsoAgLib::iFsError ui8_errorCode, IsoAgLib::iFsDirList v_directories)
    { c_fsClient.readDirectoryResponse(ui8_errorCode, v_directories); }
    void writeFileResponse(IsoAgLib::iFsError ui8_errorCode, uint16_t ui16_dataWritten)
    { c_fsClient.writeFileResponse(ui8_errorCode, ui16_dataWritten); }
    void closeFileResponse(IsoAgLib::iFsError ui8_errorCode)
    { c_fsClient.closeFileResponse(ui8_errorCode); }

    void moveFileResponse(IsoAgLib::iFsError ui8_errorCode);
    void deleteFileResponse(IsoAgLib::iFsError ui8_errorCode);
    void getFileAttributesResponse(IsoAgLib::iFsError ui8_errorCode,
                                   bool b_caseSensitive,
                                   bool b_removable,
                                   bool b_longFilenames,
                                   bool b_directory,
                                   bool b_volume,
                                   bool b_hidden,
                                   bool b_readOnly);
    void setFileAttributesResponse(IsoAgLib::iFsError ui8_errorCode);
    void getFileDateTimeResponse(IsoAgLib::iFsError ui8_errorCode,
                                 uint16_t ui16_fileYear,
                                 uint8_t ui8_fileMonth,
                                 uint8_t ui8_fileDay,
                                 uint8_t ui8_fileHour,
                                 uint8_t ui8_fileMinute,
                                 uint8_t ui8_fileSecond);

    void initializeVolumeResponse(IsoAgLib::iFsError ui8_errorCode,
                                   bool b_caseSensitive,
                                   bool b_removable,
                                   bool b_longFilenames,
                                   bool b_directory,
                                   bool b_volume,
                                   bool b_hidden,
                                   bool b_readOnly);
    /// FileServer access response functions END

    void notifyOnFsReady();

  private:
    /**
      * The whitelist of fileserver-properties that are desired for this client server communication.
      * Only fileservers of that type will be forwarded to the iFsClient (c_fsClient).
      */
    IsoAgLib::iFsWhitelistList v_fsWhitelist;

    /** The fileserver client for the client-server communication */
    IsoAgLib::iFsClient_c &c_fsClient;

    /**
      * The fileserver client's IdentItem_c.
      */
    IdentItem_c &c_identItem;

    /**
      * The current directory of the used fileserver.
      */
    uint8_t *pui8_currentDirectory;

    /**
      * The pointer to the FsRequest_c object used for the CAN-BUS communication between the fileserver client and fileserver.
      */
    FsCommand_c *pc_commandHandler;

    /**
      * Flag indication if the registration process to a fileserver has been done successfully. This means that the current
      * directory as well as the existing volumes of the fileserver have been requested successfully.
      */
    bool mb_finishedRegistering;

 private:
  /** not copyable : copy constructor is only declared, never defined */
  FsClientServerCommunication_c(const FsClientServerCommunication_c&);
  /** not copyable : copy operator is only declared, never defined */
  FsClientServerCommunication_c& operator=(const FsClientServerCommunication_c&); 
};


} // __IsoAgLib

#endif
