/*
  tcclientconnection_c.cpp: class for managing the
    communication between tc client and server

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "tcclientconnection_c.h"

#include "serverinstance_c.h"

#include <IsoAgLib/util/iassert.h>
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisend_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/identitem_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/iisoitem_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#ifdef HAL_USE_SPECIFIC_FILTERS
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isofiltermanager_c.h>
#endif
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/processpkg_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/procdata/procdata_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/tcclient_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/idevicepool_c.h>

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif




namespace __IsoAgLib {

  void TcClientConnection_c::ByteStreamBuffer_c::format( uint8_t val ) {
    push_back( val );
  }


  void TcClientConnection_c::ByteStreamBuffer_c::format( uint16_t val ) {
    push_back( ( uint8_t )( val & 0xff ) );
    push_back( ( uint8_t )( ( val >> 8 ) & 0xff ) );
  }


  void TcClientConnection_c::ByteStreamBuffer_c::format( uint32_t val ) {
    format( ( uint16_t )( val & 0xffff ) );
    format( ( uint16_t )( ( val >> 16 ) & 0xffff ) );
  }


  void TcClientConnection_c::ByteStreamBuffer_c::format( const uint8_t* bp, size_t len ) {
    while ( len-- )
      push_back( *bp++ );
  }


  void TcClientConnection_c::ByteStreamBuffer_c::format( const char* str ) {
    const size_t l = CNAMESPACE::strlen( str );
    push_back( uint8_t( l ) );
    format( ( const uint8_t* )str, l );
  }


  void TcClientConnection_c::ByteStreamBuffer_c::format( int32_t val ) {
    format( ( uint32_t )val );
  }


  void TcClientConnection_c::ByteStreamBuffer_c::format( float val ) {
    uint32_t iVal = 0;
    CNAMESPACE::memcpy( &iVal, &val, sizeof( float ) );
#if defined(__TSW_CPP__) // Tasking uses mixed endian
    uint16_t lo = iVal >> 16;
    iVal = ( iVal << 16 ) | lo;
#endif
    format( iVal );
  }

  TcClientConnection_c::TcClientConnection_c( const IdentItem_c& identItem, TcClient_c& tcClient, StateHandler_c& sh, ServerInstance_c& server, DevicePool_c& pool )
    : m_multiSendEventHandler( *this )
    , m_multiSendStreamer( *this )
    , m_identItem( identItem )
    , m_timeWsAnnounceKey( -1 )
    , m_tcClient( &tcClient )
    , m_stateHandler( &sh )
    , m_serverName( server.getIsoItem().isoName() )
    , m_server( server )
    , m_currentSendPosition( 0 )
    , m_storedSendPosition( 0 )
    , m_devicePoolToUpload()
    , m_uploadPoolState( DDOPRegistered )
    , m_uploadState( StateIdle )
    , m_uploadStep( UploadNone )
    , m_uploadTimestamp( 0 )
    , m_uploadTimeout( 0 )
    , m_commandParameter()
    , m_initDone( false )
    , m_tcAliveNew( false )
    , m_timeStartWaitAfterAddrClaim( -1 )
    , m_timeWsTaskMsgSent( -1 )
    , m_structureLabel()
    , m_localizationLabel()
    , m_measureProg()
    , m_sendSuccess( SendStream_c::SendSuccess )
    , m_pool( pool )
    , m_devPoolState( PoolStateInit )
    , m_devPoolAction( PoolActionIdle )
    , m_schedulerTaskProxy( *this, 100, false )
  {
    m_pool.init( m_identItem );
    createMeasureProgs();

    getSchedulerInstance().registerTask( m_schedulerTaskProxy, 0 );
#ifdef HAL_USE_SPECIFIC_FILTERS
    getIsoFilterManagerInstance4Comm().insertIsoFilter(
      IsoFilter_s( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, ( PROCESS_DATA_PGN << 8 ) ),
        &m_identItem.isoName(), &m_serverName, 8 ) );
#endif

    m_server.addConnection( *this );
  }


  TcClientConnection_c::~TcClientConnection_c()
  {
    m_server.removeConnection( *this );
      
#ifdef HAL_USE_SPECIFIC_FILTERS
    getIsoFilterManagerInstance4Comm().removeIsoFilter(
      IsoFilter_s( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, ( PROCESS_DATA_PGN << 8 ) ),
        &m_identItem.isoName(), &m_serverName, 8 ) );
#endif
    getSchedulerInstance().deregisterTask( m_schedulerTaskProxy );

    // TODO: send deacticate msg

    stopRunningMeasurement();
    destroyMeasureProgs();

    if (m_server.getLastActiveTaskTC())
      eventTaskStopped();

    m_pool.close();
  }

  
  const IsoItem_c&
  TcClientConnection_c::getIsoItem() const {
    return m_server.getIsoItem();
  }
      

  void
  TcClientConnection_c::timeEvent( void ) {
    if ( !m_identItem.isClaimedAddress() )
      return;

    // Wait until we are properly initialized before doing anything else
    if ( ! m_initDone ) {
      // Address claim is complete. Set time at which our address was claimed if it is
      // still in an initialized state    
      
      if ( m_timeStartWaitAfterAddrClaim == -1 ) {
        m_timeStartWaitAfterAddrClaim = HAL::getTime();
        return;
      }

      // init is finished when more then 6sec after addr claim and at least one TC status message was received
      if ( ( HAL::getTime() - m_timeStartWaitAfterAddrClaim >= 6000 ) && ( m_server.getLastStatusTime() != -1 ) )
      {
        m_timeWsAnnounceKey = m_identItem.getIsoItem()->startWsAnnounce();
        m_initDone = true;
      }
      return;
    }

    // Check if the working-set is completely announced
    if ( !m_identItem.getIsoItem()->isWsAnnounced ( m_timeWsAnnounceKey ) )
      return;

    // Send the WS task message to maintain connection with the TC
    const int32_t now = HAL::getTime();
    if ( now - m_timeWsTaskMsgSent >= 2000 ) {
      m_timeWsTaskMsgSent = now;
      sendMsg( 0xff, 0xff, 0xff, 0xff, m_server.getLastServerState(), 0x00, 0x00, 0x00 );
    }

    timeEventDevicePool();
  }


  void
  TcClientConnection_c::timeEventDevicePool() {
    if ( m_uploadPoolState == DDOPCannotBeUploaded )
      return;

    switch( m_uploadStep ) {
      case UploadUploading:
        if ( ( ( uint32_t ) HAL::getTime() ) > ( m_uploadTimeout + m_uploadTimestamp ) ) {
          startUpload();
          break;
        }
        switch ( m_sendSuccess ) {
          case __IsoAgLib::SendStream_c::Running:
            break;
          case __IsoAgLib::SendStream_c::SendAborted:
            // aborted sending
            m_uploadState = StateIdle;
            m_uploadStep = UploadFailed;
            break;

            // when the pool has been sent successfully, wait on the transfer response
          case __IsoAgLib::SendStream_c::SendSuccess:
            m_uploadStep = UploadWaitForOPTransferResponse;
            m_uploadTimeout = DEF_TimeOut_NormalCommand;
            break;
        }
        break;

        // Waiting for request to transfer response
      case UploadWaitForOPTransferResponse:
        if ( ( ( uint32_t ) HAL::getTime() ) > ( m_uploadTimeout + m_uploadTimestamp ) )
          startUpload();
        break;

      case UploadFailed:
        m_commandParameter = procCmdPar_OPTransferMsg;
        m_uploadTimestamp = HAL::getTime();
        m_uploadTimeout = DEF_WaitFor_Reupload;
        break;

      case UploadNone:
      default:
        break;
    }

    if ( m_devPoolAction == PoolActionIdle ) {
        switch ( getDevPoolState() ) {
          case PoolStateActive:
            //if ( m_pool.isDirty() )
            //  setDevPoolState(PoolStateStale);
            // @todo trigger re-announce for fresh pool upload with changed objects
            break;
          case PoolStateInit:
            // Initialization state
            //	Retrieve the structure label from the current pool
            if ( m_pool.isEmpty() )
              break;

            setDevPoolState( PoolStatePresetting );
            doCommand( procCmdPar_RequestStructureLabelMsg );
            setDevPoolAction( PoolActionWaiting );
            break;

//      case  PoolStateError:
//        setDevPoolState( PoolStateInit );
//        m_PoolAction = PoolActionIdle;
//        break;

          case PoolStateInvalid:
            // Pool is present, but it is the wrong version
            doCommand( procCmdPar_OPDeleteMsg );
            setDevPoolAction( PoolActionWaiting );
            break;

          case PoolStateNotPresent:
            // No device pool present - upload it
            if ( m_pool.isEmpty() )
              break;

            m_devicePoolToUpload = m_pool.getBytestream( procCmdPar_OPTransferMsg );
            startUpload();
            setDevPoolAction( PoolActionWaiting );
            break;

          case PoolStateStale: {
              // Upload changed descriptions
              /*STL_NAMESPACE::vector<uint8_t> newBytes;
              if (m_pool.getDirtyBytestream(newBytes))
              {
                m_devicePoolToUpload = newBytes
                startUpload();
                setDevPoolAction( PoolActionWaiting );
              }
              else
                setDevPoolState(PoolStateActive);*/
              // @todo trigger re-announce for fresh pool upload with changed objects
            }
            break;

          case PoolStateUploaded:
            doCommand( procCmdPar_OPActivateMsg );
            setDevPoolAction( PoolActionWaiting );
            break;

          default:
            break;
        }
      }
  }


  void
  TcClientConnection_c::startUpload() {
    const uint32_t byteStreamLength = m_devicePoolToUpload.getEnd() - 1; // -1 to remove 0x61 opcode

    sendMsg( procCmdPar_RequestOPTransferMsg, ( uint8_t )( ( byteStreamLength ) & 0xff ),
             ( uint8_t )( ( byteStreamLength >> 8 ) & 0xff ), ( uint8_t )( ( byteStreamLength >> 16 ) & 0xff ),
             ( uint8_t )( ( byteStreamLength >> 24 ) & 0xff ), 0xff, 0xff, 0xff );

    m_uploadTimestamp = HAL::getTime();
    m_uploadTimeout = DEF_TimeOut_OPTransfer;

    m_uploadState = StateBusy;
    m_uploadStep = UploadNone;
  }

#ifdef HAL_USE_SPECIFIC_FILTERS
  void
  TcClientConnection_c::processMsg( const CanPkg_c& data ) {
    // NOTE: Convertin to ProcessPkg_c does resolving with CanPkgExt_c
    // which would be not necessary, because of the specific SA/DA filters!
    ProcessPkg_c pkg( data, getMultitonInst() );
    // only PROCESS_DATA_PGN with SA/DA from IsoFilterManager, no need to check anything!
    processProcMsg( pkg );

    getTcClientInstance().receivePdMessage(
            *pkg.getMonitorItemForSA(),
            pkg.getMonitorItemForDA(),
            pkg.men_command,
            pkg.mui16_element,
            pkg.mui16_DDI,      
            pkg.mi32_pdValue);
  }
#endif

  void
  TcClientConnection_c::processProcMsg( const ProcessPkg_c& pkg ) {

    switch (pkg.men_command)
    {
      case IsoAgLib::ProcData::requestConfiguration:
      case IsoAgLib::ProcData::configurationResponse:
      case IsoAgLib::ProcData::nack:
        processMsgTc( pkg );
        break;
      case IsoAgLib::ProcData::requestValue:
        if( pkg.mui16_DDI == IsoAgLib::ProcData::DefaultDataLoggingDDI )
          m_stateHandler->_eventDefaultLoggingStarted( *this );
        else
          processRequestMsg( pkg );
        break;
      case IsoAgLib::ProcData::setValue:
        processSetMsg( pkg );
        break;
      case IsoAgLib::ProcData::measurementTimeValueStart:
      case IsoAgLib::ProcData::measurementDistanceValueStart:
      case IsoAgLib::ProcData::measurementMinimumThresholdValueStart:
      case IsoAgLib::ProcData::measurementMaximumThresholdValueStart:
      case IsoAgLib::ProcData::measurementChangeThresholdValueStart:
        processMeasurementMsg( pkg );
        break;
      case IsoAgLib::ProcData::commandReserved1:
      case IsoAgLib::ProcData::commandReserved2:
      case IsoAgLib::ProcData::commandReserved3:
      case IsoAgLib::ProcData::commandReserved4:
      case IsoAgLib::ProcData::taskControllerStatus:
      case IsoAgLib::ProcData::workingsetMasterMaintenance:
      case IsoAgLib::ProcData::CommandUndefined:
        break;
    }
  }

  
  void TcClientConnection_c::handleNack( int16_t ddi, int16_t element ) {
    // Note: element without DPD will not be processed properly.
    // Response will be NackInvalidElementNumber instead of NackDDINoSupportedByElement
    for( DevicePool_c::procDataList_t::iterator i = m_pool.m_procDatas.begin(); i != m_pool.m_procDatas.end(); ++i ) {
      if ( ( *i )->element() == element ) {
        sendNack( ddi, element, IsoAgLib::ProcData::NackDDINoSupportedByElement );
        return;
      }
    }
    sendNack( ddi, element, IsoAgLib::ProcData::NackInvalidElementNumber );
  }


  void TcClientConnection_c::processRequestMsg( const ProcessPkg_c& data ) {
    const uint32_t key = getMapKey( data.mui16_DDI, data.mui16_element );
    STL_NAMESPACE::map<uint32_t, MeasureProg_c*>::iterator iter = m_measureProg.find(key);

    if ( iter != m_measureProg.end())
      iter->second->processRequestMsg();
    else
      handleNack( data.mui16_DDI, data.mui16_element );
  }


  void TcClientConnection_c::processSetMsg( const ProcessPkg_c& data ) {
    const uint32_t key = getMapKey( data.mui16_DDI, data.mui16_element );
    STL_NAMESPACE::map<uint32_t, MeasureProg_c*>::iterator iter = m_measureProg.find(key);

    if ( iter != m_measureProg.end())
      iter->second->processSetMsg( data.mi32_pdValue );
    else
      handleNack( data.mui16_DDI, data.mui16_element );
  }

  void TcClientConnection_c::processMeasurementMsg( const ProcessPkg_c& data ) {
    const uint32_t key = getMapKey( data.mui16_DDI, data.mui16_element );
    STL_NAMESPACE::map<uint32_t, MeasureProg_c*>::iterator iter = m_measureProg.find(key);

    if ( iter != m_measureProg.end())
    {
      switch ( data.men_command ) {
        case IsoAgLib::ProcData::measurementDistanceValueStart:
        case IsoAgLib::ProcData::measurementTimeValueStart:
        case IsoAgLib::ProcData::measurementChangeThresholdValueStart:
        case IsoAgLib::ProcData::measurementMinimumThresholdValueStart:
        case IsoAgLib::ProcData::measurementMaximumThresholdValueStart:
          // measurementCommand_t and CommandType_t are unified for all measurement types
          iter->second->processMeasurementMsg(
            IsoAgLib::ProcData::MeasurementCommand_t( data.men_command ),
            data.mi32_pdValue );
          break;

        default:
          isoaglib_assert( !"Method shall not be called for this Process command" );
          break;
      }
    }
    else
      handleNack( data.mui16_DDI, data.mui16_element );
  }


  void TcClientConnection_c::processMsgTc( const ProcessPkg_c& data ) {

    // handling of NACK
    //  means that no device description is uploaded before
    //  so extract all necessary information from the byte-stream (structure and localization label)
    //  add the stream to the map
    if ( ( data.getUint8Data( 0 ) & 0xF ) == 0xD ) {
      //here starts nack handling
      // these two cmds could be answered with a NACK
      switch ( ( data.getUint8Data( 0 ) >> 4 ) & 0xF ) {
        case 0x0: //NACK Request StructureLabel
          m_uploadState = StateIdle;
          m_structureLabel.clear();
          eventStructureLabelResponse( 1, m_structureLabel );
          break;
        case 0x2: //NACK Request LocalizationLabel
          m_uploadState = StateIdle;
          m_localizationLabel.clear();
          eventLocalizationLabelResponse( 1, m_localizationLabel );
          break;
        default:
          break;
      }
    }

    switch ( data.getUint8Data ( 0 ) ) {
      case procCmdPar_VersionMsg:
        m_uploadState = StateIdle;
        break;

      case procCmdPar_StructureLabelMsg:
        // command is always successful if not NACK'd above
        m_structureLabel.clear();
        for ( int i = 1; i < 8; i++ )
          m_structureLabel.push_back( ( char )data.getUint8Data( i ) );

        m_uploadState = StateIdle;
        m_uploadStep = UploadNone;

        eventStructureLabelResponse( 0, m_structureLabel );
        break;

      case procCmdPar_LocalizationLabelMsg:
        // command is always successful if not NACK'd above
        m_localizationLabel.clear();
        for ( int i = 1; i < 8; i++ )
          m_localizationLabel.push_back( data.getUint8Data( i ) );

        m_uploadState = StateIdle;
        m_uploadStep = UploadNone;
        eventLocalizationLabelResponse( 0, m_localizationLabel );
        break;

      case procCmdPar_RequestOPTransferRespMsg:
        if ( data.getUint8Data( 1 ) == 0 ) { // on success, send the object pool
          m_sendSuccess = SendStream_c::SendSuccess;
          getMultiSendInstance4Comm().sendIsoTarget( m_identItem.isoName(), m_serverName, m_devicePoolToUpload.getBuffer(),
              m_devicePoolToUpload.getEnd(), PROCESS_DATA_PGN, &m_multiSendEventHandler );

          m_uploadState = StateBusy;
          m_uploadStep = UploadUploading;
        } else
          outOfMemory();
        break;

      case procCmdPar_OPTransferRespMsg:
        m_uploadState = StateIdle;
        m_uploadStep = UploadNone;
        eventPoolUploadResponse( data.getUint8Data( 1 ) );
        break;

      case procCmdPar_OPActivateRespMsg:
        m_uploadState = StateIdle;
        m_uploadStep = UploadNone;
        eventPoolActivateResponse( data.getUint8Data( 1 ) );
        break;

      case procCmdPar_OPDeleteRespMsg:
        m_uploadState = StateIdle;
        m_uploadStep = UploadNone;
        eventPoolDeleteResponse( data.getUint8Data( 1 ) );
        break;

      case procCmdPar_ChangeDesignatorRespMsg:
        m_uploadState = StateIdle;
        m_uploadStep = UploadNone;
        break;
      default:
        break;
    }
  }


  void
  TcClientConnection_c::outOfMemory() {
    m_uploadStep = UploadFailed;
    m_uploadPoolState = DDOPCannotBeUploaded;
  }


  void TcClientConnection_c::eventTaskStarted() {
    isoaglib_assert( m_stateHandler );
    m_stateHandler->_eventTaskStarted( *this );
  }


  void TcClientConnection_c::eventTaskStopped() {
    isoaglib_assert( m_stateHandler );
    m_stateHandler->_eventTaskStopped( *this );
  }


  void
  TcClientConnection_c::stopRunningMeasurement() {
    for( STL_NAMESPACE::map<uint32_t, MeasureProg_c*>::iterator i = m_measureProg.begin(); i != m_measureProg.end(); ++i ) {
      i->second->stopAllMeasurements();
    }
  }


  void
  TcClientConnection_c::sendNack( int16_t ddi, int16_t element, IsoAgLib::ProcData::NackResponse_t errorcodes ) const {
    isoaglib_assert( errorcodes != IsoAgLib::ProcData::NackReserved1 );
    isoaglib_assert( errorcodes != IsoAgLib::ProcData::NackReserved2 );
    isoaglib_assert( errorcodes != IsoAgLib::ProcData::NackUndefined );

    const uint8_t ui8_cmd = 0xd;
    sendMsg( ( uint8_t )( ( ui8_cmd & 0xf ) | ( ( element & 0xf ) << 4 ) ), ( uint8_t )( element >> 4 ), ( uint8_t )( ddi & 0x00FF ),
             ( uint8_t )( ( ddi & 0xFF00 ) >> 8 ), ( uint8_t )errorcodes, 0xff, 0xff, 0xff );
  }


  void
  TcClientConnection_c::sendMsg( uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4,
                                 uint8_t b5, uint8_t b6, uint8_t b7 ) const {
    ProcessPkg_c pkg( b0, b1, b2, b3, b4, b5, b6, b7);

    pkg.setMonitorItemForDA( const_cast<IsoItem_c*>(&m_server.getIsoItem()) );
    pkg.setMonitorItemForSA( m_identItem.getIsoItem() );

    getIsoBusInstance4Comm() << pkg;
  }


  void
  TcClientConnection_c::eventStructureLabelResponse( uint8_t result, const STL_NAMESPACE::vector<uint8_t>& label ) {
    if ( result == 0 && !label.empty() ) {
      DeviceObjectDvc_c* dvc = m_pool.getDvcObject();
      if ( dvc ) {
        const IsoAgLib::StructureLabel_s& strLbl = dvc->getStructureLabel();
        if ( STL_NAMESPACE::memcmp( ( void * )&strLbl, ( void * )&label[0], 7 ) != 0 ) {
          setDevPoolState( PoolStateInvalid );
        }
      }
    } else {
      //m_pool.setDirty();
      setDevPoolState( PoolStateNotPresent );
    }

    if ( getDevPoolState() == PoolStatePresetting ) {
      doCommand( procCmdPar_RequestLocalizationLabelMsg );
      setDevPoolAction( PoolActionWaiting );
    } else
      setDevPoolAction( PoolActionIdle );
  }


  void
  TcClientConnection_c::eventLocalizationLabelResponse( uint8_t result, const STL_NAMESPACE::vector<uint8_t>& label ) {
    if ( result == 0 && !label.empty() ) {
      DeviceObjectDvc_c* dvc = m_pool.getDvcObject();
      if ( dvc ) {
        const IsoAgLib::Localization_s& locale = dvc->getLocalization();
        if ( STL_NAMESPACE::memcmp( ( void* )&locale, ( void * )&label[0], 7 ) != 0 ) {
          m_pool.updateLocale();
        }
        setDevPoolState( PoolStateUploaded );
      }
    } else {
      //m_pool.setDirty();
      setDevPoolState( PoolStateNotPresent );
    }

    setDevPoolAction( PoolActionIdle );
  }


  void
  TcClientConnection_c::eventPoolUploadResponse( uint8_t result ) {
    if ( result == 0 ) {
      setDevPoolState( PoolStateUploaded );
      m_pool.freeByteStreamBuffer( m_devicePoolToUpload.getBuffer() );
    } else {
      setDevPoolState( PoolStateError );
    }

    setDevPoolAction( PoolActionIdle );
  }


  void
  TcClientConnection_c::eventPoolActivateResponse( uint8_t result ) {
    if ( result == 0 ) {
      setDevPoolState( PoolStateActive );
    } else {
      setDevPoolState( PoolStateError );
    }

    setDevPoolAction( PoolActionIdle );
  }


  void
  TcClientConnection_c::eventPoolDeleteResponse( uint8_t /* result */ ) {
    setDevPoolState( PoolStateNotPresent );
    setDevPoolAction( PoolActionIdle );
  }


  void TcClientConnection_c::sendProcMsg( uint16_t ddi, uint16_t element, int32_t pdValue ) const {
    ProcessPkg_c pkg( IsoAgLib::ProcData::setValue, element, ddi, pdValue );

    pkg.setMonitorItemForDA( const_cast<IsoItem_c*>( &m_server.getIsoItem() ) );
    pkg.setMonitorItemForSA( m_identItem.getIsoItem() );

    getIsoBusInstance4Comm() << pkg;
  }


  void
  TcClientConnection_c::doCommand( int32_t opcode, int32_t timeout ) {
    if ( m_uploadState == StateBusy )
      return; // StatusBusy
    if ( m_uploadState == StateTimeout )
      return; // StatusCannotProcess

    ProcessPkg_c pkg( opcode, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF );

    pkg.setMonitorItemForDA( const_cast<IsoItem_c*>( &m_server.getIsoItem() ) );
    pkg.setMonitorItemForSA( m_identItem.getIsoItem() );

    getIsoBusInstance4Comm() << pkg;

    m_uploadTimestamp = HAL::getTime();
    m_uploadTimeout = timeout;
    m_uploadStep = UploadNone;
    m_commandParameter = opcode;
    m_uploadState = StateBusy;
  }


  void TcClientConnection_c::createMeasureProgs() {
    for( DevicePool_c::procDataList_t::iterator i = m_pool.m_procDatas.begin(); i != m_pool.m_procDatas.end(); ++i ) {
      ProcData_c* pd = ( *i );
      const uint32_t key = getMapKey( pd->DDI(), pd->element());

      MeasureProg_c* mp = new MeasureProg_c( *this, *pd );
      m_measureProg[ key ] = mp;
    }
  }


  void TcClientConnection_c::destroyMeasureProgs() {
    for( STL_NAMESPACE::map<uint32_t, MeasureProg_c*>::iterator i = m_measureProg.begin(); i != m_measureProg.end(); ++i ) {
      delete i->second;
    }
  }


// MultiSendEventHandler_c implementation
  void
  TcClientConnection_c::reactOnStateChange( const SendStream_c& sendStream ) {
    m_sendSuccess = sendStream.getSendSuccess();
  }

// MultiSendStreamer_c implementation
  void
  TcClientConnection_c::setDataNextStreamPart( MultiSendPkg_c* data, uint8_t bytes ) {
    data->setDataPart ( m_devicePoolToUpload.getBuffer(), m_currentSendPosition, bytes );
    m_currentSendPosition += bytes;
  }

  void
  TcClientConnection_c::resetDataNextStreamPart() {
    m_currentSendPosition = 0;
  }

  void
  TcClientConnection_c::saveDataNextStreamPart() {
    m_storedSendPosition = m_currentSendPosition;
  }

  void
  TcClientConnection_c::restoreDataNextStreamPart() {
    m_currentSendPosition = m_storedSendPosition;
  }

  uint32_t
  TcClientConnection_c::getStreamSize() {
    return m_devicePoolToUpload.getEnd();
  }

  uint8_t
  TcClientConnection_c::getFirstByte() {
    return m_devicePoolToUpload[0];
  }


}; // __IsoAgLib
