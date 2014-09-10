/*
  ifsclient_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IFSCLIENT_C_H
#define IFSCLIENT_C_H

/* FileServer interfaces includes */
#include "ifsserverinstance_c.h"
#include "ifsstructs.h"

namespace IsoAgLib
{

/**
  * Interface class used for communication with a fileserver on the ISO-BUS. A application that wants to use the services
  * of a fileserver has to implement the methods given in this interface and register to the FsManager_c. As soon as fileserver
  * are on the bus, the FsManager reports thoser fileserver to the implementing class. It can then decide if it wants to
  * communicate to this fileserver. If it wants to communcate to the fileserver it has to register to this filerserver.
  * the fsclientservercommunication starts to request the fileservers additional properties (current directory) and notifies the
  * implementing class once all requested values have been reported.
  *
  * Requests are always done using the FsClientServerCommunication_c object for the implementing class. Responses are received
  * directly by the implementing class via the Response methods.
  */
class iFsClient_c
{
  public:

    /** virtual destructor */
    virtual ~iFsClient_c() {}

    /**
      * virtual function called once a new fileserver has been initialized. implementing class has to decide what to do with that
      * information.
      */
    virtual void finishedConnectingToFileserver(iFsServerInstance_c &/*rc_fileServer*/) = 0;
    /**
      * when registering to a fileserver, the fsclientservercommunication starts to request additional information on the fileserver.
      * once this inforamtion has been received the client is notified using this function.
      */
    virtual void fileserverReady() = 0;
    /**
      * If the request for information on the fileserver was not sucessful,this function is called with an error code.
      */
    virtual void fileserverError(iFsError /*ui8_errorCode*/) = 0;

    /**
      * Method called once the client's fileserver has gone offline.
      */
    virtual void notifyOnOfflineFileServer(iFsServerInstance_c &/*rc_fsServerInstance*/) = 0;

    /**
      * methods that are called once the requested information has been received.
      */

    /**
      * The registered fileserver's properties as defined in the ISO 11873 standard.
      */
    virtual void getFileServerPropertiesResponse(uint8_t /*ui8_versionNumber*/, uint8_t /*ui8_maxNumberOpenFiles*/, bool /*b_fsMultiVolumes*/) = 0;

    /**
      * After the call for a current directory change, this function is used to receive the successstatus of the command.
      */
    virtual void changeCurrentDirectoryResponse(iFsError /*ui8_errorCode*/) = 0;
    /**
      * After the call to open a file, this function is used to receive the successstatus of the command and the additional
      * information on the opened file. IMPORTANT: The filehandle is used for all further file access!
      */

    virtual void openFileResponse(iFsError /*ui8_errorCode*/,
                          uint8_t /*ui8_fileHandle*/,
                          bool /*b_caseSensitive*/,
                          bool /*b_removable*/,
                          bool /*b_longFilenames*/,
                          bool /*b_directory*/,
                          bool /*b_volume*/,
                          bool /*b_hidden*/,
                          bool /*b_readOnly*/) = 0;
    /**
      * After the call to seek in a file, this function is used to receive the successstatus of the command and the new possition
      * of the filepointer of the file.
      */

    virtual void seekFileResponse(iFsError /*ui8_errorCode*/, uint32_t /*ui32_position*/) = 0;
    /**
      * After the call to read a file, this function is used to receive the successstatus of the command and the read file content.
      * IMPORTANT: the content of th file has to be copied to the applications memory as it can be changed by the library.
      */

    virtual void readFileResponse(iFsError /*ui8_errorCode*/, uint16_t /*ui16_dataLength*/, uint8_t * /*pui8_data*/) = 0;
    /**
      * After the call to read a file, this function is used to receive the successstatus of the command if the file was a directory.
      * The content of the file is returned.
      */
    virtual void readDirectoryResponse(iFsError /*ui8_errorCode*/, iFsDirList /*v_directories*/) = 0;
    /**
      * After the call to write data to a file, this function is used to receive the successstatus of the command and the number of
      * written data.
      */
    virtual void writeFileResponse(iFsError /*ui8_errorCode*/, uint16_t /*ui16_dataWritten*/) = 0;
    /**
      * After the call to close a file, this function is used to receive the successstatus of the command.
      */
    virtual void closeFileResponse(iFsError /*ui8_errorCode*/) = 0;

    /**
      * After the call to move a file, this function is used to receive the successstatus of the command.
      */
    virtual void moveFileResponse(iFsError /*ui8_errorCode*/) = 0;
    /**
      * After the call to delete a file, this function is used to receive the successstatus of the command.
      */
    virtual void deleteFileResponse(iFsError /*ui8_errorCode*/) = 0;
    /**
      * After the call to get a file's attributes, this function is used to receive the successstatus of the command. In addition
      * the files attributes are returned.
      */
    virtual void getFileAttributesResponse(iFsError /*ui8_errorCode*/,
                                   bool /*b_caseSensitive*/,
                                   bool /*b_removable*/,
                                   bool /*b_longFilenames*/,
                                   bool /*b_directory*/,
                                   bool /*b_volume*/,
                                   bool /*b_hidden*/,
                                   bool /*b_readOnly*/) = 0;
    /**
      * After the call to set a files attributes, this function is used to receive the successstatus of the command.
      */
    virtual void setFileAttributesResponse(iFsError /*ui8_errorCode*/) = 0;
    /**
      * After the call to get a files date and time, this function is used to receive the successstatus of the command and the files
      * date and time.
      */
    virtual void getFileDateTimeResponse(iFsError /*ui8_errorCode*/,
                                 uint16_t /*ui16_fileYear*/,
                                 uint8_t /*ui8_fileMonth*/,
                                 uint8_t /*ui8_fileDay*/,
                                 uint8_t /*ui8_fileHour*/,
                                 uint8_t /*ui8_fileMinute*/,
                                 uint8_t /*ui8_fileSecond*/) = 0;

    /**
      * After the call to initialize a volume, this function is used to receive the successstatus of the command and the volumes
      * properties.
      */
    virtual void initializeVolumeResponse(iFsError /*ui8_errorCode*/,
                                   bool /*b_caseSensitive*/,
                                   bool /*b_removable*/,
                                   bool /*b_longFilenames*/,
                                   bool /*b_directory*/,
                                   bool /*b_volume*/,
                                   bool /*b_hidden*/,
                                   bool /*b_readOnly*/) = 0;

    /// FileServer access response functions END
};

}

#endif
