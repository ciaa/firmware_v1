/*
  tcclientconnection_c.h: class for managing a connection

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TCCLIENTCONNECTION_H
#define TCCLIENTCONNECTION_H

#include <IsoAgLib/driver/can/impl/cancustomer_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisendeventhandler_c.h>
#include <IsoAgLib/comm/Part3_DataLink/imultisendstreamer_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisend_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/procdata/measureprog_c.h>

#include <map>

namespace IsoAgLib {
  class iDevicePool_c;
  class iTcClientConnection_c;
  class iTcClientConnectionStateHandler_c;
}

namespace __IsoAgLib {
  class IdentItem_c;
  class TcClient_c;
  class MultiSendPkg_c;
  class ProcessPkg_c;
  class ProcData_c;
  class DevicePool_c;
  class ServerInstance_c;


  class TcClientConnection_c
#ifdef HAL_USE_SPECIFIC_FILTERS
      : public CanCustomer_c
#endif
  {
// @todo Convert to static const...
#define DEF_TimeOut_GetVersion            5000
#define DEF_TimeOut_OPTransfer            20000
#define DEF_TimeOut_EndOfDevicePool       10000
#define DEF_WaitFor_Reupload              5000
#define DEF_TimeOut_ChangeDesignatorValue 1500
#define DEF_TimeOut_NormalCommand         1500

    public:
      enum DevPoolState_t {
        PoolStateInit = 0,
        PoolStatePresetting,
        PoolStateNotPresent,
        PoolStateStale,
        PoolStateInvalid,
        PoolStateUploaded,
        PoolStateActive,
        PoolStateError
      };

      enum UploadPoolState_t {
        DDOPRegistered,
        DDOPCannotBeUploaded
      };


      class StateHandler_c {
        public:
          virtual void _eventDefaultLoggingStarted( TcClientConnection_c& ecu ) = 0;
          virtual void _eventTaskStarted( TcClientConnection_c& ecu ) = 0;
          virtual void _eventTaskStopped( TcClientConnection_c& ecu ) = 0;
      };


      class ByteStreamBuffer_c {
        public:
          ByteStreamBuffer_c() {reset();}
          void setBuffer( uint8_t* b ) {
            m_buffer = b;
          }
          uint8_t* getBuffer() {
            return m_buffer;
          }

          void setEnd( uint32_t e ) {
            m_offset = e;
          }
          uint32_t getEnd() const {
            return m_offset;
          }

          void setSize( uint32_t s ) {
            m_size = s;
          }
          uint32_t getSize() const {
            return m_size;
          }

          uint8_t& operator[]( uint32_t p ) {
            return m_buffer[ p ];
          }
          void reset() {
            setBuffer( 0 );
            setEnd( 0 );
            setSize( 0 );
          }
          void format( uint8_t val );
          void format( uint16_t val );
          void format( uint32_t val );
          void format( const uint8_t* bp, size_t len );
          void format( const char* str );
          void format( int32_t val );
          void format( float val );
        private:
          uint32_t m_offset;
          uint8_t* m_buffer;
          uint32_t m_size;

          void push_back( uint8_t b ) {
            isoaglib_header_assert( ( m_offset + 1 ) <= getSize() );
            m_buffer[ m_offset++ ] = b;
          }
      };


      TcClientConnection_c(
        const IdentItem_c& identItem,
        TcClient_c& tcClient,
        StateHandler_c& sh,
        ServerInstance_c& server,
        DevicePool_c& pool );

      virtual ~TcClientConnection_c();

      const IdentItem_c& getIdentItem() const {
        return m_identItem;
      }
      TcClient_c& getTcClient() const {
        return *m_tcClient;
      }

      const IsoName_c& getServerName() const {
        return m_serverName;
      }

      const ServerInstance_c& getServer() const {
        return m_server;
      }

      const IsoItem_c& getIsoItem() const;

      void processProcMsg( const ProcessPkg_c& );

      void sendNack( int16_t ddi, int16_t element, IsoAgLib::ProcData::NackResponse_t errorcodes ) const;
      void sendProcMsg( uint16_t ddi, uint16_t element, int32_t pdValue ) const;

      bool sendCommandChangeDesignator( uint16_t /* objID */, const char* /* newString */, uint8_t /* length */ ) {
        return false; /* @todo: send msg */
      }

      void eventTaskStarted();
      void eventTaskStopped();
      void stopRunningMeasurement();

    private:
// @todo Send this?
#if 0
      void requestVersion() { doCommand( procCmdPar_RequestVersionMsg, DEF_TimeOut_GetVersion ); }
#endif
      int32_t requestPoolTransfer( ByteStreamBuffer_c pool );

      void eventStructureLabelResponse( uint8_t result, const STL_NAMESPACE::vector<uint8_t>& label );
      void eventLocalizationLabelResponse( uint8_t result, const STL_NAMESPACE::vector<uint8_t>& label );
      void eventPoolUploadResponse( uint8_t result );
      void eventPoolActivateResponse( uint8_t result );
      void eventPoolDeleteResponse( uint8_t result );

      void setDevPoolState( DevPoolState_t newState ) {
        m_devPoolState = newState;
      }
      DevPoolState_t getDevPoolState() const {
        return m_devPoolState;
      }

      UploadPoolState_t getUploadPoolState() {
        return m_uploadPoolState;
      }

      void timeEvent();
      void timeEventDevicePool();
      void startUpload();
#ifdef HAL_USE_SPECIFIC_FILTERS
      virtual void processMsg( const CanPkg_c& data );
#endif

      void processMsgTc( const ProcessPkg_c& );

      void processMeasurementMsg( const ProcessPkg_c& );
      void processRequestMsg( const ProcessPkg_c& );
      void processSetMsg( const ProcessPkg_c& );

      void outOfMemory();

      class MultiSendEventHandlerProxy_c : public MultiSendEventHandler_c {
        public:
          typedef TcClientConnection_c Owner_t;

          MultiSendEventHandlerProxy_c( Owner_t &owner ) : m_owner( owner ) {}
          ~MultiSendEventHandlerProxy_c() {}

        private:
          void reactOnStateChange( const SendStream_c& sendStream ) {
            m_owner.reactOnStateChange( sendStream );
          }

          MultiSendEventHandlerProxy_c( MultiSendEventHandlerProxy_c const & ); // make class non-copyable
          MultiSendEventHandlerProxy_c &operator=( MultiSendEventHandlerProxy_c const & ); // make class non-copyable

          Owner_t &m_owner;
      }; // MultiSendEventHandlerProxy_c
      MultiSendEventHandlerProxy_c m_multiSendEventHandler;

      void reactOnStateChange( const SendStream_c& );

      class MultiSendStreamerProxy_c : public IsoAgLib::iMultiSendStreamer_c {
        public:
          typedef TcClientConnection_c Owner_t;

          MultiSendStreamerProxy_c( Owner_t &owner ) : m_owner( owner ) {}
          ~MultiSendStreamerProxy_c() {}

        private:
          virtual uint8_t getFirstByte() { return m_owner.getFirstByte(); }
          virtual void resetDataNextStreamPart() { m_owner.resetDataNextStreamPart(); }
          virtual void saveDataNextStreamPart() { m_owner.saveDataNextStreamPart(); }
          virtual void restoreDataNextStreamPart() { m_owner.restoreDataNextStreamPart(); }
          virtual uint32_t getStreamSize() { return m_owner.getStreamSize(); }
          virtual void setDataNextStreamPart ( __IsoAgLib::MultiSendPkg_c* data, uint8_t bytes ) {
            m_owner.setDataNextStreamPart( data, bytes ); }

          MultiSendStreamerProxy_c( MultiSendStreamerProxy_c const & ); // make class non-copyable
          MultiSendStreamerProxy_c &operator=( MultiSendStreamerProxy_c const & ); // make class non-copyable

          Owner_t &m_owner;
      }; // MultiSendStreamerProxy_c

      MultiSendStreamerProxy_c m_multiSendStreamer;

      void setDataNextStreamPart( MultiSendPkg_c*, uint8_t );
      void resetDataNextStreamPart();
      void saveDataNextStreamPart();
      void restoreDataNextStreamPart();
      uint32_t getStreamSize();
      uint8_t getFirstByte();

      int getMultitonInst() const {
        return m_identItem.getMultitonInst();
      }

      void handleNack( int16_t ddi, int16_t element );
      static uint32_t getMapKey(uint16_t ddi, uint16_t element) { return ( uint32_t( uint32_t( ddi ) << 16 ) ) | uint32_t(element); }

    private:
      enum PoolAction_t {
        PoolActionIdle = 0,
        PoolActionWaiting,
        PoolActionTimeout
      };

      enum UploadState_t {
        StateIdle,
        StateBusy,
        StateTimeout
      };

      enum UploadSteps_t {
        UploadNone,
        UploadUploading,
        UploadWaitForOPTransferResponse,
        UploadFailed
      };

      enum ProcessDataMsg_t {
        procCmdPar_RequestVersionMsg = 0x00,
        procCmdPar_VersionMsg = 0x10,
        procCmdPar_RequestStructureLabelMsg = 0x01,
        procCmdPar_StructureLabelMsg = 0x11,
        procCmdPar_RequestLocalizationLabelMsg = 0x21,
        procCmdPar_LocalizationLabelMsg = 0x31,
        procCmdPar_RequestOPTransferMsg = 0x41,
        procCmdPar_RequestOPTransferRespMsg = 0x51,
        procCmdPar_OPTransferMsg = 0x61,
        procCmdPar_OPTransferRespMsg = 0x71,
        procCmdPar_OPActivateMsg = 0x81,
        procCmdPar_OPActivateRespMsg = 0x91,
        procCmdPar_OPDeleteMsg = 0xA1,
        procCmdPar_OPDeleteRespMsg = 0xB1,
        procCmdPar_ChangeDesignatorMsg = 0xC1,
        procCmdPar_ChangeDesignatorRespMsg = 0xD1
      };


      void doCommand( int32_t opcode, int32_t timeout = DEF_TimeOut_NormalCommand );
      void sendMsg( uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t ) const;

      const IdentItem_c& m_identItem;
      int32_t m_timeWsAnnounceKey;
      TcClient_c* m_tcClient;
      StateHandler_c* m_stateHandler;

      IsoName_c m_serverName;
      ServerInstance_c& m_server;

      // MultiSendStreamer_c variables
      uint16_t m_currentSendPosition;
      uint16_t m_storedSendPosition;
      ByteStreamBuffer_c m_devicePoolToUpload;

      // device pool upload variables
      UploadPoolState_t m_uploadPoolState;
      UploadState_t m_uploadState;
      UploadSteps_t m_uploadStep;
      uint32_t m_uploadTimestamp; // @todo This should be an int32_t.
      uint32_t m_uploadTimeout;
      uint8_t m_commandParameter;

      // timeEvent variables
      bool m_initDone;
      bool m_tcAliveNew;
      int32_t m_timeStartWaitAfterAddrClaim;
      int32_t m_timeWsTaskMsgSent;

      STL_NAMESPACE::vector<uint8_t> m_structureLabel;
      STL_NAMESPACE::vector<uint8_t> m_localizationLabel;

      // Measure progs presorted for DDIs
      void createMeasureProgs();
      void destroyMeasureProgs();
      STL_NAMESPACE::map<uint32_t, MeasureProg_c*> m_measureProg;

      SendStream_c::sendSuccess_t m_sendSuccess;

      // device pool
      DevicePool_c& m_pool;
      DevPoolState_t m_devPoolState;
      PoolAction_t m_devPoolAction;
      void setDevPoolAction( PoolAction_t action ) {
        m_devPoolAction = action;
      }

      CLASS_SCHEDULER_TASK_PROXY( TcClientConnection_c )
      SchedulerTaskProxy_c m_schedulerTaskProxy;

  }; // TcClientConnection_c

} // __IsoAgLib

#endif
