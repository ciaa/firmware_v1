/*
  ifsclientservercommunication_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IFSCLIENTSERVERCOMMUNICATION_C_H
#define IFSCLIENTSERVERCOMMUNICATION_C_H

#include "impl/fsclientservercommunication_c.h"
#include "ifsserverinstance_c.h"

// Begin Namespace IsoAgLib
namespace IsoAgLib {

/**
  * iFsClientServerCommunication_c interface class for communication between a fileserver client and server.
  * After a fileserver client registered to the FsManager_c fileserver manager, it gets a reference to a
  * iFsClientServerCommunication_c object that is handling the communication between a fileserver client and
  * it's selected fileserver. A client has to call the corresponding methods of iFsClientServerCommunication_c
  * to interact with a fileserver.
  */
class iFsClientServerCommunication_c : private __IsoAgLib::FsClientServerCommunication_c
{
  private:
    /** only declared, never defined -> not constructable */
    iFsClientServerCommunication_c();

    friend class __IsoAgLib::FsClientServerCommunication_c;

  public :

    /**
      * Request a connection to a fileserver. Once this method is called, the iFsClientServerCommunication_c starts requesting
      * fileserver-client specific information (e.g. current directory of the fileserver). Once this information is received,
      * the client is notified about the success and that the fileserver is from then on ready for communication.
      * @param rc_FileServer the fileserver to connect to.
      */
    void requestFsConnection(iFsServerInstance_c &rc_FileServer)
  {__IsoAgLib::FsClientServerCommunication_c::requestFsConnection(static_cast<__IsoAgLib::FsServerInstance_c&>(rc_FileServer)); }

    /**
      * get the used fileservers current directory. as iFsClientServerCommunication_c saves the current directory information
      * no XXXResponse method is used to the response, but the infromation is returned by the information.
      * @return the fileservers current directory.
      */
    uint8_t *getCurrentDirectory() { return __IsoAgLib::FsClientServerCommunication_c::getCurrentDirectory(); }

    /**
      * did the iFsClientServerCommunication_c receive all requested information from the requested fileserver and is
      * the fileserver therefore ready`
      * @return true if the fileserver is ready, false else.
      */
    bool finishedRegistering() { return __IsoAgLib::FsClientServerCommunication_c::finishedRegistering(); }

    /**
      * get the fileservers volumes.
      * @return fileservers volume information saved as iFsDirList
      */
    iFsDirList getFsVolumes() { return __IsoAgLib::FsClientServerCommunication_c::getFsVolumes(); }

    /// FileServer access functions

    /**
      * Get the actual fileservers properties. Properties are returned by a call to the clients XXXResponse method.
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t getFileServerProperties() { return __IsoAgLib::FsClientServerCommunication_c::getFileServerProperties(); }

    /**
      * change the fileservers current directory.
      * @param pui8_newDirectory the new current directory
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t changeCurrentDirectory(uint8_t *pui8_newDirectory)
    { return __IsoAgLib::FsClientServerCommunication_c::changeCurrentDirectory(pui8_newDirectory); }

    /**
      * open a file and request a filehandle for this file
      * @param pui8_fileName the file that has to be opened.
      * @param b_openExclusive open file for exclusive access
      * @param b_openForAppend open file to append data at the end of the file or for random access
      * @param b_createNewFile create a new file if not exist?
      * @param b_openForReading open for reading?
      * @param b_openForWriting open for writing?
      * @param b_openDirectory open directory?
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t openFile(uint8_t *pui8_fileName, bool b_openExclusive, bool b_openForAppend, bool b_createNewFile, bool b_openForReading, bool b_openForWriting, bool b_openDirectory)
    { return __IsoAgLib::FsClientServerCommunication_c::openFile(pui8_fileName, b_openExclusive, b_openForAppend, b_createNewFile, b_openForReading, b_openForWriting, b_openDirectory); }
    /**
      * change positon pointer of a file.
      * @param ui8_fileHandle filehandle of the desired file
      * @param ui8_possitionMode possition mode of the funciton. 0 = from beginning, 1 = from current possition, 2 = from end
      * @param i32_offset offset in relation to ui8_possitionMode
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t seekFile(uint8_t ui8_fileHandle, uint8_t ui8_possitionMode, int32_t i32_offset)
    { return __IsoAgLib::FsClientServerCommunication_c::seekFile(ui8_fileHandle, ui8_possitionMode, i32_offset); }

    /**
      * read content of a file
      * @param ui8_fileHandle filehandle of the desired file
      * @param ui16_count number of bytes that shall be read
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t readFile(uint8_t ui8_fileHandle, uint16_t ui16_count)
    { return __IsoAgLib::FsClientServerCommunication_c::readFile(ui8_fileHandle, ui16_count); }

    /**
      * read content of a file
      * @param ui8_fileHandle filehandle of the desired file
      * @param ui16_count number of bytes that shall be read
      * @param b_reportHiddenFiles report hidden files?
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t readDirectory(uint8_t ui8_fileHandle, uint16_t ui16_count, bool b_reportHiddenFiles)
    { return __IsoAgLib::FsClientServerCommunication_c::readDirectory(ui8_fileHandle, ui16_count, b_reportHiddenFiles); }

    /**
      * write data to a file
      * @param ui8_fileHandle filehandle of the desired file
      * @param ui16_count number of bytes that shall be written
      * @param pui8_data bytes that shall be written
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t writeFile(uint8_t ui8_fileHandle, uint16_t ui16_count, uint8_t *pui8_data)
    { return __IsoAgLib::FsClientServerCommunication_c::writeFile(ui8_fileHandle, ui16_count, pui8_data); }

    /**
      * close a file
      * @param ui8_fileHandle filehandle of the desired file
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t closeFile(uint8_t ui8_fileHandle)
    { return __IsoAgLib::FsClientServerCommunication_c::closeFile(ui8_fileHandle); }


    /**
      * move/copy a file
      * @param pui8_sourceName source name of the file that has to be moved/copied
      * @param pui8_destName destination name for move/copy operation
      * @param b_recursive shall the content of a file be moved/copied recursively
      * @param b_force shall proteced files be read/moved/overridden?
      * @param b_copy shall the file be copied?
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t moveFile(uint8_t *pui8_sourceName, uint8_t *pui8_destName, bool b_recursive, bool b_force, bool b_copy)
    { return __IsoAgLib::FsClientServerCommunication_c::moveFile(pui8_sourceName, pui8_destName, b_recursive, b_force, b_copy); }
    /**
      * delete a file
      * @param pui8_fileName the name of the file that shall be deleted
      * @param b_recursive delete content of a directory as well?
      * @param b_force shall protected files be deleted?
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t deleteFile(uint8_t *pui8_fileName, bool b_recursive, bool b_force)
    { return __IsoAgLib::FsClientServerCommunication_c::deleteFile(pui8_fileName, b_recursive, b_force); }

    /**
      * get a file's attributes
      * @param pui8_fileName the file's name
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t getFileAttributes(uint8_t *pui8_fileName)
    { return __IsoAgLib::FsClientServerCommunication_c::getFileAttributes(pui8_fileName); }

    /**
      * change a file's attributes.
      * @param pui8_fileName the file's name
      * @param ui8_hiddenAtt change hidden attribute command: 0 clear, 1 set abd 3 leave as is.
      * @param ui8_readOnlyAtt change read only attribute command: 0 clear, 1 set abd 3 leave as is.
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t setFileAttributes(uint8_t *pui8_fileName, uint8_t ui8_hiddenAtt, uint8_t ui8_readOnlyAtt)
    { return __IsoAgLib::FsClientServerCommunication_c::setFileAttributes(pui8_fileName, ui8_hiddenAtt, ui8_readOnlyAtt); }
    /**
      * get date and time of last change of file.
      * @param pui8_fileName name of the reqeusted file.
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t getFileDateTime(uint8_t *pui8_fileName)
    { return __IsoAgLib::FsClientServerCommunication_c::getFileDateTime(pui8_fileName); }


    /**
      * initialize volume
      * @param pui8_pathName pathname for the directory.
      * @param ui32_space space to be used.
      * @param b_createVolumeUsingSpace does all space have to be used?
      * @param b_createNewVolume shall a new volum be created?
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    uint8_t initializeVolume(uint8_t *pui8_pathName, uint32_t ui32_space, bool b_createVolumeUsingSpace, bool b_createNewVolume)
    { return __IsoAgLib::FsClientServerCommunication_c::initializeVolume(pui8_pathName, ui32_space, b_createVolumeUsingSpace, b_createNewVolume); }


    /**
      * set connection state to keep open even after all files have been closed
      * @param b_keepOpen connection state.
      * @param b_forceClose force the connection to close even if files are open.
      */
    uint8_t setKeepConnectionOpen( bool b_keepOpen, bool b_forceClose=false )
    { return __IsoAgLib::FsClientServerCommunication_c::setKeepConnectionOpen(b_keepOpen, b_forceClose); }

    /**
      * get conneciton state
      * @return true if the keep connection open is current set else false is returned.
      */
    bool getKeepConnectionOpen()
    { return __IsoAgLib::FsClientServerCommunication_c::getKeepConnectionOpen(); }

    /// FileServer access functions END
};

//End namespace
}

#endif
