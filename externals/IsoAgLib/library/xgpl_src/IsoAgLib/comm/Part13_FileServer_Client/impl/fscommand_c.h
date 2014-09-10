/*
  fscommand_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef FSCOMMAND_C_H
#define FSCOMMAND_C_H

#include "fsserverinstance_c.h"

#include <IsoAgLib/comm/Part3_DataLink/impl/multisend_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multireceive_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisendeventhandler_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/identitem_c.h>
#include <IsoAgLib/scheduler/impl/schedulertask_c.h>
#include <IsoAgLib/driver/can/impl/cancustomer_c.h>


namespace __IsoAgLib
{
// forward declarations
class FsClientServerCommunication_c;

/**
  * Class responsible to send and receive commands to and from a fileserver. It also takes care about the commands used by
  * the fileserver as they can differ according to the version of the standard implemented by the fileserver.
  */
class FsCommand_c : CanCustomer_c
{
  public:
    FsCommand_c(FsClientServerCommunication_c &FSCSComm, FsServerInstance_c &fileServerInst);
    ~FsCommand_c();

    CLASS_SCHEDULER_TASK_PROXY(FsCommand_c)

  class MultiSendEventHandlerProxy_c : public MultiSendEventHandler_c {
  public:
    typedef FsCommand_c Owner_t;

    MultiSendEventHandlerProxy_c(Owner_t &owner) : mrt_owner(owner) {}    ~MultiSendEventHandlerProxy_c() {}

  private:
    void reactOnStateChange(const SendStream_c& sendStream)
    {
       mrt_owner.reactOnStateChange(sendStream);
    }

    // IsoRequestPgnHandlerProxy_c shall not be copyable. Otherwise
    // the reference to the containing object would become invalid.
    MultiSendEventHandlerProxy_c(MultiSendEventHandlerProxy_c const &);
    MultiSendEventHandlerProxy_c &operator=(MultiSendEventHandlerProxy_c const &);

    Owner_t &mrt_owner;
  }; // MultiSendEventHandlerProxy_c

  void reactOnStateChange(const SendStream_c& sendStream);

  private:
    // forbid copy construction
    FsCommand_c( const FsCommand_c& );

    /** function used to send a request. depending on the size of the request single of multi packet send is selected. **/
    enum RequestType_en { RequestInitial, RequestRetry };
    void sendRequest (RequestType_en);

    /** decodes a given attribute and saves the decoded values in the corresponding member variables **/
    void decodeAttributes(uint8_t attributes);

    /** functions to decode fileserver repsonses. **/
    void decodeGetCurrentDirectoryResponse();
    void decodeOpenFileResponse( const CanPkg_c& );
    void decodeSeekFileResponse( const CanPkg_c& );
    void decodeSeekFileResponse();
    void decodeReadFileResponse();
    void decodeReadDirectoryResponse();

    /** clean up when done **/
    void clearDirectoryList();
    void doCleanUp();

    /** internal read file use for read file and read directory **/
    IsoAgLib::iFsCommandErrors readFile(uint8_t fileHandle, uint16_t count, bool b_reportHiddenFiles);

    /** which command are we sending? **/
    enum commandtype_en
    {
      en_noCommand = 0x00,
      en_requestProperties = 0x01,
      en_getCurrentDirectory = 0x10,
      en_changeCurrentDirectory = 0x11,
      en_openFile = 0x20,
      en_seekFile = 0x21,
      en_readFile = 0x22,
      en_writeFile = 0x23,
      en_closeFile = 0x24,
      en_moveFile = 0x30,
      en_deleteFile = 0x31,
      en_getFileAttributes = 0x32,
      en_setFileAttributes = 0x33,
      en_getFileDateTime = 0x34,
      en_initializeVolume = 0x40
    };
    commandtype_en en_lastCommand;

  public:

    /** is the command busy, meaning waiting for a response? **/
    bool isBusy() { return !m_receivedResponse; }

    /** time event function. If no response received, resend request periodically. **/
    void timeEvent(void);

    /** send the current command out via 8 Byte CAN */
    void sendSinglePacket();

    /** send the current command out via TP/ETP.
        In case it coulnd't be sent out right now
        set "mb_retryMultiPacketSend=true" so the timeEvent()
        will retry until it could be started... */
    void sendMultiPacketTry();

    void processMsgIso( const CanPkgExt_c& pkg );

    /**
      * Method called by FsClientServerCommunciation_c. After the response of get current directory, the fileserver is considered to
      * be ready for use.
      */
    IsoAgLib::iFsCommandErrors getCurrentDirectory();
    /**
      * Method to change the fileserver's current directory. The response-method returns the new current directory to the
      * FsClientServerCommunication_c, as this information is saved there.
      * @param newDirectory the new current directory of the fileserver.
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors changeCurrentDirectory(uint8_t *newDirectory);

    /**
      * Method to open a file and request a filehandle for the opened file.
      * @param pui8_fileName the name of the file to be openend.
      * @param openExclusive open file with exclusive access?
      * @param openForAppend open writeable file for append at the end, or allow random access.
      * @param createNewFile create not existing file?
      * @param openForReading open for reading?
      * @param openForWriting open for writing?
      * @param openDirectory open directory?
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors openFile(uint8_t *fileName, bool openExclusive, bool openForAppend, bool createNewFile, bool openForReading, bool openForWriting, bool openDirectory);
    /**
      * Change the position pointer of a file.
      * @param fileHandle filehandle of the file to be seeked in.
      * @param positionMode position mode of the file, 0 from beginning of file, 1 from current position, 2 from end.
      * @param offset offset from position given in positionMode
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors seekFile(uint8_t fileHandle, uint8_t positionMode, int32_t offset);
    /**
      * read content of a file
      * @param fileHandle filehandle of the file to be read.
      * @param count number of bytes to be read.
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors readFile(uint8_t fileHandle, uint16_t count);
    /**
      * read content of a directory
      * @param fileHandle directoryhandle of the directory to be read.
      * @param count number of entries to be read.
      * @param reportHiddenFiles shall hidden files of a directory be reported?
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors readDirectory(uint8_t fileHandle, uint16_t count, bool reportHiddenFiles);
    /**
      * write data to a file
      * @param fileHandle filehandle of the file to be written.
      * @param count number of bytes to be written.
      * @param data data that shall be written
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors writeFile(uint8_t fileHandle, uint16_t count, uint8_t *data);
    /**
      * close a file
      * @param fileHandle filehandle of the file to be closed.
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors closeFile(uint8_t fileHandle);

    /**
      * move/copy a file.
      * @param sourceName source name of the file to be moved/copied
      * @param destName destination for the file
      * @param recursive shall a directory be moved/copied recoursivly?
      * @param force override existing/protected files?
      * @param copy shall the file be moved or copied?
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors moveFile(uint8_t *sourceName, uint8_t *destName, bool recursive, bool force, bool copy);
    /**
      * delete a file.
      * @param fileName name of the file to be deleted.
      * @param recursive shall content of a directory be deleted as well?
      * @param force shall write-protected files be deleted?
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors deleteFile(uint8_t *fileName, bool recursive, bool force);
    /**
      * get Attributes of a file.
      * @param fileName name of the requested file.
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors getFileAttributes(uint8_t *fileName);
    /**
      * change attributed for a file.
      * @param fileName name of the file that's attributes have to be changed.
      * @param hiddenAttr change hidden attribute command: 0 clear, 1 set abd 3 leave as is.
      * @param readOnlyAttr change read only attribute command: 0 clear, 1 set abd 3 leave as is.
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors setFileAttributes(uint8_t *fileName, uint8_t hiddenAttr, uint8_t readOnlyAttr);
    /**
      * get date and time of last change of file.
      * @param fileName name of the reqeusted file.
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors getFileDateTime(uint8_t *fileName);

    /**
      * initialize volume
      * @param pathName pathname for the directory.
      * @param space space to be used.
      * @param createVolumeUsingSpace does all space have to be used?
      * @param createNewVolume shall a new volum be created?
      * @return 0 if request was sent without problems, else an errorcode is returned.
      */
    IsoAgLib::iFsCommandErrors initializeVolume(uint8_t *pathName, uint32_t space, bool createVolumeUsingSpace, bool createNewVolume);

    /**
      * Starts the initialisation process for a fileserver.
      */
    void requestProperties();

    /**
      * returns the fileserver of the FsCommand_c.
      */
    FsServerInstance_c &getFileserver() { return mc_fileserver; }

    /**
      * set connection state to keep open even after all files have been closed
      * @param keepOpen connection state.
      * @param forceClose force the connection to close even if files are open.
      */
    void setKeepConnectionOpen( bool keepOpen, bool forceClose=false );
    /**
      * get connection state
      * @return true if the keep connection open is current set else false is returned.
      */
    bool getKeepConnectionOpen() const { return m_keepConnectionOpen; }

    /** MultiReceiveClient functions BEGIN **/
    bool processPartStreamDataChunk (Stream_c& stream, bool isFirstChunk, bool isLastChunkAndACKd);
    void reactOnAbort(Stream_c& stream);
    bool reactOnStreamStart(const ReceiveStreamIdentifier_c& streamIdent, uint32_t totalLen);
    /** MultiReceiveClient functions END **/

    int getMultitonInst() const { return 0; }

  private:
    // member variable instead of multiple inheritance
    SchedulerTaskProxy_c m_schedulerTask;
    MultiSendEventHandlerProxy_c m_multiSendEventHandler;
    FsClientServerCommunication_c &m_FSCSComm;
    FsServerInstance_c &mc_fileserver;

    uint8_t m_tan;

    /** open file maintenance message **/
    uint8_t m_nrOpenFiles;
    bool m_keepConnectionOpen;
    int32_t m_lastAliveSentTime;
    static uint8_t m_maintenanceMsgBuf[8];

    uint8_t m_packetLength;
    uint8_t m_sendMsgBuf[256];
    uint8_t *m_multireceiveMsgBuf;
    uint32_t m_multireceiveMsgBufAllocSize;

    bool m_receivedResponse;
    int32_t m_lastrequestAttemptTime;
    uint8_t m_requestAttempts;

    SendStream_c::sendSuccess_t m_sendSuccessNotify;

    /** receive information **/
    uint8_t m_errorCode;

    /** currentDirectory information **/
    uint8_t *m_currentDirectory;
    uint16_t m_currentDirectoryAllocSize;

    /** file handle information **/
    uint8_t m_fileHandle;

    /** open file information **/
    uint8_t *m_fileName;
    uint16_t m_fileNameAllocSize;

    /** open file or directory flags */
    bool m_attrCaseSensitive;
    bool m_attrRemovable;
    bool m_attrLongFilenames;
    bool m_attrIsDirectory;
    bool m_attrIsVolume;
    bool m_attrHidden;
    bool m_attrReadOnly;

    /** seek file information **/
    uint8_t m_positionMode;
    int32_t m_offset;
    uint32_t m_position;

    /** read file information */
    uint16_t m_count;
    uint8_t *m_data;
    uint16_t m_dataAllocSize;
    IsoAgLib::iFsDirList m_dirData;
    bool m_readDirectory;


    /** filter information **/
    bool m_receiveFilterCreated;

    /// FS Initial Query Part (Properties/Volumes)
    bool m_initialQueryStarted;

    /// Is this instance for Initializing the FsInstance_c
    /// (call callbacks and do initialization procedure then)
    /// or is it for normal Application-triggered operation?
    bool m_initializingFileserver;

    /// Indicate if a MultiSend was started and it
    /// needs to be waited for the Success/Abort.
    bool m_waitForMultiSendFinish;

    /// If the MultiPacketSend couldn't be started
    /// it needs to be retried in the timeEvent()
    bool m_retryMultiPacketSend;
};

} // __IsoAgLib

#endif
