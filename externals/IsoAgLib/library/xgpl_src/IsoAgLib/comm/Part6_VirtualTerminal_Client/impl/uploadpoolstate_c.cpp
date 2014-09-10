/*
  uploadpoolstate_c.cpp: 

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "uploadpoolstate_c.h"
#include <IsoAgLib/comm/Part3_DataLink/impl/stream_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/impl/vtclientconnection_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/impl/vtserverinstance_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtclientobjectpool_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtobjectworkingset_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtclientobjectpool_c.h>
#include <IsoAgLib/util/iliberr_c.h>

#ifdef USE_CCI_ISB_WORKAROUND
#include <IsoAgLib/comm/impl/isobus_c.h>
#endif

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif


namespace __IsoAgLib
{

// Some old GS2 will force us to run into a time-out, so we need to continue in case of a time-out!
static const int32_t s_timeOutGetVersions = 6000;


UploadPoolState_c::UploadPoolState_c(
  VtClientConnection_c &connection,
  IsoAgLib::iVtClientObjectPool_c& pool,
  const char *versionLabel,
  bool wsMaster )
  : m_connection( connection )
  , m_pool( pool )
  , mb_usingVersionLabel( versionLabel != NULL )
  //marrp7c_versionLabel[ 7 ] body!
  , m_uploadingVersion( 0 )
  , mc_iVtObjectStreamer( *this )
  , men_uploadPoolState( UploadPoolEndSuccess ) // default for Slaves!
  , men_uploadPoolType( UploadPoolTypeCompleteInitially ) // dummy
  , mui32_uploadTimestamp( 0 )
  , mui32_uploadTimeout( 0 ) // will be set when needed
  //ms_uploadPhasesAutomatic[..] // body!
  , mui_uploadPhaseAutomatic( UploadPhaseIVtObjectsFix )
  , ms_uploadPhaseUser() // body!
  , mppc_uploadPhaseUserObjects( NULL )
  , mi8_objectPoolUploadingLanguage( 0 )
  , mi8_objectPoolUploadedLanguage( 0 )
  , mui16_objectPoolUploadingLanguageCode( 0x0000 )
  , mui16_objectPoolUploadedLanguageCode( 0x0000 )
  , mi8_vtLanguage( -2 )
{
  if( versionLabel )
  {
    const uint32_t cui_len = CNAMESPACE::strlen( versionLabel );
    isoaglib_assert( ! ( ( (m_pool.getNumLang() == 0) && (cui_len > 7) ) || ( (m_pool.getNumLang() > 0) && (cui_len > 5) ) ) ); 
    unsigned int i=0;
    for( ; i<cui_len; ++i ) marrp7c_versionLabel[ i ] = versionLabel[ i ];
    for( ; i<7;       ++i ) marrp7c_versionLabel[ i ] = ' '; // ASCII: Space

    isoaglib_assert( m_langRejectedUseDefaultAsFallback.bits() >= m_pool.getNumLang() );
  }

  if( wsMaster )
  {
    ms_uploadPhasesAutomatic[0] = UploadPhase_s( &mc_iVtObjectStreamer, 0 );
    ms_uploadPhasesAutomatic[1] = UploadPhase_s( &mc_iVtObjectStreamer, 0 );
    ms_uploadPhaseUser = UploadPhase_s( &mc_iVtObjectStreamer, 0 );
    men_uploadPoolState = UploadPoolInit;
  }
}


void
UploadPoolState_c::processMsgVtToEcu( Stream_c &stream )
{
  switch( stream.getFirstByte() )
  {
    case 0xE0:
      handleGetVersionsResponse( &stream );
      break;
  }
}


void
UploadPoolState_c::processMsgVtToEcu( const CanPkgExt_c& pkg )
{
  switch( pkg.getUint8Data( 0 ) )
  {
    case 0x12: // Command: "End of Object Pool Transfer", parameter "Object Pool Ready Response"
      handleEndOfObjectPoolResponse( pkg.getUint8Data( 1 ) == 0 );
      break;

    case 0xC0: // Command: "Get Technical Data", parameter "Get Memory Size Response"
      handleGetMemoryResponse( pkg );
      break;

    case 0xC2: // Command: "Get Technical Data", parameter "Get Number Of Soft Keys Response"
      m_connection.getVtServerInst().setSoftKeyData( pkg );
      break;

    case 0xC3: // Command: "Get Technical Data", parameter "Get Text Font Data Response"
      m_connection.getVtServerInst().setTextFontData( pkg );
      break;

    case 0xC7: // Command: "Get Technical Data", parameter "Get Hardware Response"
      m_connection.getVtServerInst().setHardwareData( pkg );
      break;

    case 0xD0: // Command: "Non Volatile Memory", parameter "Store Version Response"
      handleStoreVersionResponse( pkg.getUint8Data( 5 ) & 0x0F );
      break;

    case 0xD1: // Command: "Non Volatile Memory", parameter "Load Version Response"
      handleLoadVersionResponse( pkg.getUint8Data( 5 ) & 0x0F );
      break;

    case 0xE0: // Command: "Non Volatile Memory", parameter "Get Versions Response"
      handleGetVersionsResponse( NULL );
      break;
  }
}


void
UploadPoolState_c::initPool()
{
  getPool().initAllObjectsOnce( m_connection.getMultitonInst() );

  // now let all clients know which client they belong to
  const uint8_t clientId = m_connection.getClientId();
  if( clientId > 0 ) // the iVtObjects are initialised with 0 as default index
  {
    for (uint16_t ui16_objIndex = 0; ui16_objIndex < getPool().getNumObjects(); ++ui16_objIndex)
      getPool().getIVtObjects()[0][ui16_objIndex]->setClientID( clientId );
    for (uint8_t ui8_objLangIndex = 0; ui8_objLangIndex < getPool().getNumLang(); ++ui8_objLangIndex)
    {
      for (uint16_t ui16_objIndex = 0; ui16_objIndex < getPool().getNumObjectsLang(); ++ui16_objIndex)
      {
        getPool().getIVtObjects()[ui8_objLangIndex+1][ui16_objIndex]->setClientID( clientId );
        // do not allow language dependent AUX2 objects
#ifdef USE_VTOBJECT_auxiliaryfunction2
        isoaglib_assert(getPool().getIVtObjects()[ui8_objLangIndex+1][ui16_objIndex]->getObjectType() != IsoAgLib::iVtObjectAuxiliaryFunction2_c::objectType());
#endif
#ifdef USE_VTOBJECT_auxiliaryinput2
        isoaglib_assert(getPool().getIVtObjects()[ui8_objLangIndex+1][ui16_objIndex]->getObjectType() != IsoAgLib::iVtObjectAuxiliaryInput2_c::objectType());
#endif
      }
    }
  }
}

// 1.) Search for version-label
// 2.) Mark all rejected languages
bool
UploadPoolState_c::searchVersionsAndMarkRejected( Stream_c& stream, uint8_t numVersions )
{
  bool versionFound = false;

  // don't break on this search, because still all need to be marked!
  for( uint8_t counter = 0; counter < numVersions; ++counter )
  {
    char c_nextversion[ 7 ];
    for( uint16_t i = 0; i < 7; ++i )
      c_nextversion[i] = stream.get();

    // check if this is a rejected language
    if( m_pool.multiLanguage() )
    {
      if( 0 == CNAMESPACE::memcmp( c_nextversion, marrp7c_versionLabel, 5 ) )
      {
        if( ( c_nextversion[ 5 ] >= 'A' ) && ( c_nextversion[ 5 ] <= 'Z' ) &&
            ( c_nextversion[ 6 ] >= 'A' ) && ( c_nextversion[ 6 ] <= 'Z' ) )
        { // "rejected" pool
          const int8_t langIndex = getLanguageIndex(
            c_nextversion[ 5 ]+('a'-'A'),
            c_nextversion[ 6 ]+('a'-'A') );
          
          if( langIndex >= 0 )
          {
            m_langRejectedUseDefaultAsFallback.setBit( unsigned( langIndex ) );
            versionFound = true; // Pool-name (without language extension) matches!
          }
          // else: Some version with a language not used in this pool (maybe some old pool that had this version)
        }
        else
        { // "normal" pool
          const int8_t langIndex = getLanguageIndex(
            c_nextversion[ 5 ],
            c_nextversion[ 6 ] );
          
          if( langIndex >= 0 )
            versionFound = true; // Pool-name (without language extension) matches!
          // else: Some version with a language not used in this pool (maybe some old pool that had this version)
        }
      }
      // else: some other version, don't care.
    }
    else // no multilanguage
    {
      if( 0 == CNAMESPACE::memcmp( c_nextversion, marrp7c_versionLabel, 7 ) )
      {
        versionFound = true; 
        break;
      }
    }
  }

  return versionFound;
}


void
UploadPoolState_c::handleGetVersionsResponse( Stream_c *stream )
{
  if( men_uploadPoolState != UploadPoolWaitingForGetVersionsResponse )
    return;

  uint8_t number_of_versions = 0;
  if( stream != NULL )
  {
    number_of_versions = stream->get();
    if( uint32_t(stream->getByteTotalSize()) != uint32_t(2 + 7*uint16_t(number_of_versions)) )
      return; // malformed message

    if( searchVersionsAndMarkRejected( *stream, number_of_versions ) )
    {
      startLoadVersion();
      return;
    }
  }

  startUploadVersion();
}


// This command is used for both:
// Getting VT's version and checking for available memory
void
UploadPoolState_c::handleGetMemoryResponse( const CanPkgExt_c &pkg )
{
  switch( men_uploadPoolState )
  {
  case UploadPoolWaitingForVtVersionResponse:
    m_connection.getVtServerInst().setVersion( pkg );

    // Special case when adapting a v3 pool to upload to v2 VTs, omitting all the new aux...
    m_uploadingVersion = (m_connection.getVtServerInst().getVtIsoVersion() == 2) && (m_pool.getVersion() == 3)
                           ? 2 : m_pool.getVersion();

#if DEBUG_VTCOMM || DEBUG_VTPOOLUPLOAD
    INTERNAL_DEBUG_DEVICE << "Upload pool as v" << (unsigned)m_uploadingVersion << " to a v" << (unsigned)m_connection.getVtServerInst().getVtIsoVersion() << " VT." << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
    
    // check for matching VT version and object pool version
    if( m_connection.getVtServerInst().getVtIsoVersion() < m_uploadingVersion )
      uploadFailed( UploadError_VtVersionError );
    else
    {
      // Take the language that's been set in the VT right NOW
      setObjectPoolUploadingLanguage();

      if( mb_usingVersionLabel )
        startGetVersions();
      else
        startUploadVersion();
    }
    break;

  case UploadPoolWaitingForMemoryResponse:
    if( pkg.getUint8Data( 2 ) == 0 )
    { // start uploading with all partial OPs (as init'd before Get Memory!), there MAY BE enough memory
      men_uploadPoolState = UploadPoolUploading;
    //men_uploadPhaseAutomatic [already initialized in "initObjectPoolUploadingPhases" to the correct starting phase]
      startCurrentUploadPhase();
    }
    else
      uploadFailed( UploadError_OutOfMemoryError );
    break;

  default:
    ; // unsolicited msg.
  }
}


void
UploadPoolState_c::handleStoreVersionResponse( unsigned errorNibble )
{
  if( men_uploadPoolState != UploadPoolWaitingForStoreVersionResponse )
    return;

  switch( errorNibble )
  {
    case 0: // Successfully stored
    case 1: // Not used
    case 2: // Version label not known
    case 8: // General error
      break;
    case 4: // Insufficient memory available
    default: // well....
      IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::VtOutOfStorageSpace, m_connection.getMultitonInst() );
      break;
  }
  finalizeUploading();
}


void
UploadPoolState_c::handleLoadVersionResponse( unsigned errorNibble )
{
  if( men_uploadPoolState != UploadPoolWaitingForLoadVersionResponse )
    return;

  if( errorNibble == 0 )
  {
    finalizeUploading();
#if DEBUG_VTCOMM || DEBUG_VTPOOLUPLOAD
    INTERNAL_DEBUG_DEVICE << "Received Load Version Response (D1) without error..." << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
  }
  else
  {
    if( errorNibble & (1<<2) )
    { 
#if DEBUG_VTCOMM || DEBUG_VTPOOLUPLOAD
      INTERNAL_DEBUG_DEVICE << "Received Load Version Response (D1) with error OutOfMem..." << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
      uploadFailed( UploadError_OutOfMemoryError );
    }
    else
    { // Not used
      // General error
      // Version label not known -> upload the pool
      startUploadVersion(); // Send out pool! send out "Get Technical Data - Get Memory Size", etc. etc.
#if DEBUG_VTCOMM || DEBUG_VTPOOLUPLOAD
      INTERNAL_DEBUG_DEVICE << "Received Load Version Response (D1) with VersionNotFound..." << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
    }
  }
}

void
UploadPoolState_c::uploadFailed( UploadError aen_uploadError )
{
  IsoAgLib::iVtClientObjectPool_c::UploadErrorData poolUpLoadErrorData(IsoAgLib::iVtClientObjectPool_c::UploadError_NoError,
                                                                       m_connection.getVtServerInst().getIsoName().funcInst());
  
  switch(aen_uploadError)
  {
    case UploadError_NoError:
      break;
    case UploadError_OutOfMemoryError:
      poolUpLoadErrorData.error = IsoAgLib::iVtClientObjectPool_c::UploadError_OutOfMemoryError;
      break;
    case UploadError_VtVersionError:
      poolUpLoadErrorData.error = IsoAgLib::iVtClientObjectPool_c::UploadError_VtVersionError;
      break;
    case UploadError_InvalidLanguageError:
      poolUpLoadErrorData.error = IsoAgLib::iVtClientObjectPool_c::UploadError_InvalidLanguageError;
      break;
    case UploadError_EoopError:
      poolUpLoadErrorData.error = IsoAgLib::iVtClientObjectPool_c::UploadError_EoopError;
      break;
  }
  
  m_connection.getPool().UploadError(poolUpLoadErrorData);

  switch(aen_uploadError)
  {
    case UploadError_OutOfMemoryError:
      IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::VtOutOfMemory, m_connection.getMultitonInst() );
      break;
    default:
        ;
  }
  
  men_uploadPoolState = UploadPoolEndFailed;
}


void
UploadPoolState_c::startUploadVersion()
{
  initObjectPoolUploadingPhases( UploadPoolTypeCompleteInitially );

  sendGetMemory( false );
}


void
UploadPoolState_c::handleEndOfObjectPoolResponse( bool success )
{
  if( men_uploadPoolState != UploadPoolWaitingForEOOResponse )
    return;

  if( success )
  {
    if( mb_usingVersionLabel )
    {
      const uint8_t rejectOff = rejectOffset( marrp7c_versionLabel[ 5 ], marrp7c_versionLabel[ 6 ] );

      men_uploadPoolState = UploadPoolWaitingForStoreVersionResponse;
      m_connection.sendMessage( // Command: Non Volatile Memory --- Parameter: Store Version
        208, marrp7c_versionLabel [0], marrp7c_versionLabel [1], marrp7c_versionLabel [2], marrp7c_versionLabel [3], marrp7c_versionLabel [4],
        marrp7c_versionLabel [5]-rejectOff,
        marrp7c_versionLabel [6]-rejectOff );
    }
    else
      finalizeUploading();
  }
  else
  {
    if( m_pool.multiLanguage() )
    {
      const int8_t langIndex = getLanguageIndex(
        mui16_objectPoolUploadingLanguageCode >> 8,
        mui16_objectPoolUploadingLanguageCode & 0xFF );
        
      isoaglib_assert( langIndex >= 0 );

      const bool retry = !m_langRejectedUseDefaultAsFallback.isBitSet( unsigned( langIndex ) );
      m_langRejectedUseDefaultAsFallback.setBit( unsigned( langIndex ) );
      if( retry )
        m_connection.restart(); // with fallback language
      else
        uploadFailed( UploadError_InvalidLanguageError );
    }
    else
      uploadFailed( UploadError_EoopError );
  }
}


bool
UploadPoolState_c::handleEndOfObjectPoolResponseOnLanguageUpdate( bool success )
{
  if( success )
  {
    // do not StoreVersion, only do this on INITIAL
    // Upload where objects are yet unmodified.
    finalizeUploading();
    return false;
  }
  else
  {
    if( men_uploadPoolType == UploadPoolTypeLanguageUpdate )
    {
      const int8_t langIndex = getLanguageIndex(
        mui16_objectPoolUploadingLanguageCode >> 8,
        mui16_objectPoolUploadingLanguageCode & 0xFF );
        
      isoaglib_assert( langIndex >= 0 );

      // We can't do anything other than fallback to the default-language.
      m_langRejectedUseDefaultAsFallback.setBit( unsigned( langIndex ) );
    }
    // It will stall when reuploading completely
    // but we can't stall in upload-command mode right now...
    return true; // need restart
  }
}


void
UploadPoolState_c::sendGetMemory( bool requestVtVersion )
{
  // Right now don't care if several 0x11s are counted from each partial object pool...
  uint32_t ui32_size = 0;
  if( requestVtVersion )
    men_uploadPoolState = UploadPoolWaitingForVtVersionResponse;
  else
  {
    men_uploadPoolState = UploadPoolWaitingForMemoryResponse;

    for( int i=0; i <= UploadPhaseLAST; ++i )
      ui32_size += ms_uploadPhasesAutomatic[ i ].ui32_size;
  }

  m_connection.sendMessage(
    192, 0xff, (ui32_size) & 0xFF, (ui32_size >>  8) & 0xFF,
    (ui32_size >> 16) & 0xFF, ui32_size >> 24, 0xff, 0xff);
}


void
UploadPoolState_c::startGetVersions()
{
  m_connection.sendMessage( 223, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF );

  men_uploadPoolState = UploadPoolWaitingForGetVersionsResponse;
  mui32_uploadTimeout = s_timeOutGetVersions;
  mui32_uploadTimestamp = HAL::getTime();
}


// Given Language must exist in Objectpool!
uint8_t
UploadPoolState_c::rejectOffset( uint8_t langCode0, uint8_t langCode1 ) const
{
  if( !m_pool.multiLanguage() )
    return 0;

  const int8_t langIndex = getLanguageIndex( langCode0, langCode1 );
  isoaglib_assert( langIndex >= 0 );

  return m_langRejectedUseDefaultAsFallback.isBitSet( unsigned( langIndex ) )
    ? 'a'-'A' : 0;
}


void
UploadPoolState_c::startLoadVersion()
{
  const uint8_t rejectOff = rejectOffset( marrp7c_versionLabel[ 5 ], marrp7c_versionLabel[ 6 ] );
                           
  m_connection.sendMessage( 209,
    marrp7c_versionLabel[ 0 ], marrp7c_versionLabel[ 1 ], marrp7c_versionLabel[ 2 ], marrp7c_versionLabel[ 3 ], marrp7c_versionLabel[ 4 ],
    marrp7c_versionLabel[ 5 ]-rejectOff,
    marrp7c_versionLabel[ 6 ]-rejectOff );

  men_uploadPoolState = UploadPoolWaitingForLoadVersionResponse;
  men_uploadPoolType = UploadPoolTypeCompleteInitially; // need to set this, so that eventObjectPoolUploadedSucessfully is getting called (also after load, not only after upload)
#if DEBUG_VTCOMM || DEBUG_VTPOOLUPLOAD
  INTERNAL_DEBUG_DEVICE << "Trying Load Version (D1) for Version ["<<marrp7c_versionLabel [0]<< marrp7c_versionLabel [1]<< marrp7c_versionLabel [2]<< marrp7c_versionLabel [3]<< marrp7c_versionLabel [4]<< marrp7c_versionLabel [5]<< marrp7c_versionLabel [6]<<"] with rejectOffset=" << unsigned(rejectOff) << "..." << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
}


bool
UploadPoolState_c::dontUpload( const vtObject_c& object ) const
{
  return( object.isOmittedFromUpload()
       || ((m_uploadingVersion == 2) && (object.getObjectType() >= VT_OBJECT_TYPE_AUXILIARY_FUNCTION_2) && (object.getObjectType() <= VT_OBJECT_TYPE_AUXILIARY_POINTER) ) );
}


uint32_t
UploadPoolState_c::fitTerminalWrapper( const vtObject_c& object ) const
{
  return dontUpload( object ) ? 0 : object.fitTerminal();
}


void
UploadPoolState_c::timeEvent()
{
  if( !(m_connection.getVtServerInst().getVtCapabilities()->lastReceivedFont
     && m_connection.getVtServerInst().getVtCapabilities()->lastReceivedHardware
     && m_connection.getVtServerInst().getVtCapabilities()->lastReceivedSoftkeys) )
    timeEventRequestProperties();
  else
    timeEventPoolUpload();
}


void
UploadPoolState_c::timeEventRequestProperties()
{
  VtServerInstance_c &server = m_connection.getVtServerInst();

  /// first you have to get number of softkeys, text font data and hardware before you could upload
  if( !server.getVtCapabilities()->lastReceivedSoftkeys
      && ((server.getVtCapabilities()->lastRequestedSoftkeys == 0)
      || ((HAL::getTime() - server.getVtCapabilities()->lastRequestedSoftkeys) > 1000)))
  { // Command: Get Technical Data --- Parameter: Get Number Of Soft Keys
    m_connection.sendMessage( 194, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff );
    server.getVtCapabilities()->lastRequestedSoftkeys = HAL::getTime();
  }

  if (server.getVtCapabilities()->lastReceivedSoftkeys
      && (!server.getVtCapabilities()->lastReceivedFont)
      && ((server.getVtCapabilities()->lastRequestedFont == 0) || ((HAL::getTime() - server.getVtCapabilities()->lastRequestedFont) > 1000)))
  { // Command: Get Technical Data --- Parameter: Get Text Font Data
    m_connection.sendMessage( 195, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff );
    server.getVtCapabilities()->lastRequestedFont = HAL::getTime();
  }

  if (server.getVtCapabilities()->lastReceivedSoftkeys
      && server.getVtCapabilities()->lastReceivedFont
      && (!server.getVtCapabilities()->lastReceivedHardware)
      && ((server.getVtCapabilities()->lastRequestedHardware == 0)
      || ((HAL::getTime() - server.getVtCapabilities()->lastRequestedHardware) > 1000)))
  { // Command: Get Technical Data --- Parameter: Get Hardware
    m_connection.sendMessage( 199, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff );
    server.getVtCapabilities()->lastRequestedHardware = HAL::getTime();
  }
}


void
UploadPoolState_c::timeEventPoolUpload()
{
  switch( men_uploadPoolState )
  {
  case UploadPoolInit:
    sendGetMemory( true );
    break;

  case UploadPoolWaitingForGetVersionsResponse:
    // There are normally no time-out checks as the VT has to respond!
    // The Get Versions time-out is only for VTs that do incorrectly
    // answer with a DLC < 8 and hence we don't see that answer.
    // Should be removed in the future if all VTs do properly answer with DLC 8
    if (((uint32_t) HAL::getTime()) > (mui32_uploadTimeout + mui32_uploadTimestamp))
    {
#if DEBUG_VTCOMM || DEBUG_VTPOOLUPLOAD
      INTERNAL_DEBUG_DEVICE << "Version couldn't be checked (GVResp missing/short DLC) -> Upload pool" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
      startUploadVersion(); // Send out pool! send out "Get Technical Data - Get Memory Size", etc. etc.
    }
    break;

  default:
    ; // all others are fine
  }
}


//! Handle language Update as a command, not upload!
void
UploadPoolState_c::timeEventLanguageUpdate()
{
  if( !successfullyUploaded() )
    return;

  if( (mi8_objectPoolUploadingLanguage == -2) // indicates no update running
   && (mi8_vtLanguage != mi8_objectPoolUploadedLanguage) )
  { // update languages on the fly
    setObjectPoolUploadingLanguage();
    /// NOTIFY THE APPLICATION so it can enqueue some commands that are processed BEFORE the update is done
    /// e.g. switch to a "Wait while changing language..." datamask.
    m_pool.eventPrepareForLanguageChange( calcAppUploadingLanguage(), mui16_objectPoolUploadingLanguageCode );

    m_connection.commandHandler().sendCommandUpdateLanguagePool();
    // we keep (mi8_objectPoolUploadingLanguage != -2), so a change in between doesn't care and won't happen!!
  }
}


bool
UploadPoolState_c::timeEventCalculateLanguage()
{
  if( mi8_vtLanguage != -2 )
    return true;

  // Try to calculate VT's language
  if( m_connection.getVtServerInst().receivedLocalSettings() )
  { // can calculate the language
    mi8_vtLanguage = getLanguageIndex(
      m_connection.getVtServerInst().getLocalSettings()->languageCode >> 8,
      m_connection.getVtServerInst().getLocalSettings()->languageCode & 0xFF);
    m_pool.eventLanguagePgn( *m_connection.getVtServerInst().getLocalSettings() );
    return true;
  }
  else
  { // cannot calculate the language YET, LANGUAGE_PGN not yet received, REQUEST & WAIT!
    m_connection.getVtServerInst().requestLocalSettings( m_connection.getIdentItem() );
    // do not proceed if VT's language not yet calculated!
    return false;
  }
}

int8_t
UploadPoolState_c::getLanguageIndex( uint8_t langCode0, uint8_t langCode1 ) const
{
  for( int i=0; i<m_pool.getNumLang(); ++i )
  {
    const uint8_t* lang = m_pool.getWorkingSetObject().get_vtObjectWorkingSet_a().languagesToFollow[ i ].language;
    if(  ( langCode0 == lang[ 0 ] )
      && ( langCode1 == lang[ 1 ] ) )
      return i;
  }

  // indicate that the given language is not supported by this WS, so the default language should be used
  return -1;
}


void
UploadPoolState_c::reactOnStateChange( const SendStream_c& stream )
{
  if( !m_connection.isVtActive() )
    return;

  switch( stream.getSendSuccess() )
  {
    case __IsoAgLib::SendStream_c::Running:
      break;

    case __IsoAgLib::SendStream_c::SendAborted:
      startCurrentUploadPhase(); // re-send the current stream (partial OP)
      break;

    case __IsoAgLib::SendStream_c::SendSuccess:
      indicateUploadPhaseCompletion(); // may complete the upload or switch to the next phase
      break;
  }
}


void
UploadPoolState_c::indicateUploadCompletion()
{
  if( successfullyUploaded() )
  { // user / language updates are being sent as "command"
    m_connection.commandHandler().finishUploadCommand();
  }
  else
  { // successfully uploaded complete initial pool
    // Command: Object Pool Transfer --- Parameter: Object Pool Ready
    m_connection.sendMessage( 0x12, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff );

    men_uploadPoolState = UploadPoolWaitingForEOOResponse;
  }
}


void
UploadPoolState_c::indicateUploadPhaseCompletion()
{
  if (men_uploadPoolType == UploadPoolTypeUserPoolUpdate)
  { // we only have one part, so we're done!
    mc_iVtObjectStreamer.mpc_objectsToUpload = NULL; // just for proper cleanup.
    // We don't need that pointer anymore. It can be invalid after we told the client
    // that we're done with partial user objectpool upload/update.
    indicateUploadCompletion(); // Send "End of Object Pool" message
  }
  else
  { // we may have multiple parts, so check that..
    // move to next possible one.
    if (men_uploadPoolType == UploadPoolTypeLanguageUpdate)
      mui_uploadPhaseAutomatic += 2; // skip the GENERAL parts, move on directly to next LANGUAGE part!
    else
      mui_uploadPhaseAutomatic += 1;

    startCurrentUploadPhase();
  }
}


void
UploadPoolState_c::startCurrentUploadPhase()
{
  IsoAgLib::iMultiSendStreamer_c* streamer = NULL;
  switch( men_uploadPoolType )
  {
  case UploadPoolTypeUserPoolUpdate:
    streamer = ms_uploadPhaseUser.pc_streamer;
    mc_iVtObjectStreamer.mpc_objectsToUpload = mppc_uploadPhaseUserObjects;
    mc_iVtObjectStreamer.setStreamSize (ms_uploadPhaseUser.ui32_size);
    break;

  case UploadPoolTypeCompleteInitially:
  case UploadPoolTypeLanguageUpdate:
    // First, check current phase.
    // while the current phase is n/a, move to next.
    while ((mui_uploadPhaseAutomatic <= UploadPhaseLAST) && (ms_uploadPhasesAutomatic [mui_uploadPhaseAutomatic].ui32_size == 0))
    { // prepare for the next part
      if (men_uploadPoolType == UploadPoolTypeLanguageUpdate)
        mui_uploadPhaseAutomatic += 2; // skip the GENERAL parts, move on directly to next LANGUAGE part!
      else
        mui_uploadPhaseAutomatic += 1;
    }
    if (mui_uploadPhaseAutomatic > UploadPhaseLAST)
    { // done with all phases!
      indicateUploadCompletion(); // Send "End of Object Pool" message
      return;
    }
    // else: start next phase
    streamer = ms_uploadPhasesAutomatic [mui_uploadPhaseAutomatic].pc_streamer;
    // first, prepare the individual upload phases.
    switch (UploadPhase_t (mui_uploadPhaseAutomatic)) // allowed cast, we're in enum-bounds!
    {
      case UploadPhaseIVtObjectsFix:
        mc_iVtObjectStreamer.mpc_objectsToUpload = m_pool.getIVtObjects()[0]; // main FIX (lang. indep) iVtObject part
        mc_iVtObjectStreamer.setStreamSize (ms_uploadPhasesAutomatic [mui_uploadPhaseAutomatic].ui32_size);
        break;

      case UploadPhaseIVtObjectsLang:
      { // phase 0 & 1 use iVtObjectStreamer, so prepare for that!
        const int8_t realUploadingLanguageAsIndex = calcRealUploadingLanguage( true ) + 1; // skip language-independent objects.
        mc_iVtObjectStreamer.mpc_objectsToUpload = m_pool.getIVtObjects()[ realUploadingLanguageAsIndex ];
        mc_iVtObjectStreamer.setStreamSize (ms_uploadPhasesAutomatic [mui_uploadPhaseAutomatic].ui32_size);
      } break;

      case UploadPhaseAppSpecificFix:
        break; // nop
      case UploadPhaseAppSpecificLang:
        break; // nop
    }
    break;
  }

  getMultiSendInstance( m_connection.getMultitonInst() ).sendIsoTarget(
    m_connection.getIdentItem().isoName(),
    m_connection.getVtServerInst().getIsoName(),
    streamer,
    ECU_TO_VT_PGN, this );
}


void
UploadPoolState_c::setObjectPoolUploadingLanguage()
{
  mi8_objectPoolUploadingLanguage = mi8_vtLanguage;
  mui16_objectPoolUploadingLanguageCode = 0x0000;
  if( m_pool.multiLanguage() )
  {
    const int8_t realUploadingLanguage = calcRealUploadingLanguage( false );
    const uint8_t* lang = m_pool.getWorkingSetObject().get_vtObjectWorkingSet_a().languagesToFollow[ realUploadingLanguage ].language;
    mui16_objectPoolUploadingLanguageCode = (lang [0] << 8) | lang[1];
    marrp7c_versionLabel[ 5 ] = lang[ 0 ];
    marrp7c_versionLabel[ 6 ] = lang[ 1 ];
  }
}


void
UploadPoolState_c::finalizeUploading()
{
  if( men_uploadPoolType == UploadPoolTypeUserPoolUpdate )
  { /// Was user-pool-update
    m_pool.eventPartialPoolUploadedSuccessfully();
  }
  else
  { /// Was complete initial pool or language pool update.
    /// in both cases we uploaded in one specific language!! so do the following:
    mi8_objectPoolUploadedLanguage = mi8_objectPoolUploadingLanguage;
    mui16_objectPoolUploadedLanguageCode = mui16_objectPoolUploadingLanguageCode;
    mi8_objectPoolUploadingLanguage = -2; // -2 indicated that the language-update while pool is up IS IDLE!
    mui16_objectPoolUploadingLanguageCode = 0x0000;

  #if DEBUG_VTCOMM || DEBUG_VTPOOLUPLOAD
    INTERNAL_DEBUG_DEVICE << "===> finalizeUploading () with language: "<<(int)mi8_objectPoolUploadedLanguage;
    if (mi8_objectPoolUploadedLanguage >= 0) INTERNAL_DEBUG_DEVICE <<" ["<<uint8_t(mui16_objectPoolUploadedLanguageCode>>8) <<uint8_t(mui16_objectPoolUploadedLanguageCode&0xFF)<<"]";
    INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_ENDL;
  #endif
    if( men_uploadPoolType == UploadPoolTypeLanguageUpdate )
    {
      // no need to set "men_objectPoolState" and "men_uploadType", this is done in "finishUploadCommand()"
    }
    else
    {
  #if DEBUG_VTCOMM || DEBUG_VTPOOLUPLOAD
      INTERNAL_DEBUG_DEVICE << "Now men_uploadPoolState = UploadPoolEndSuccess;" << INTERNAL_DEBUG_DEVICE_ENDL;
  #endif
      men_uploadPoolState = UploadPoolEndSuccess;
    }

#ifdef USE_CCI_ISB_WORKAROUND
    CanPkgExt_c sendData;
    
    sendData.setIsoPri( 6 );
    sendData.setIsoPgn( 0x00CC00 );
    sendData.setMonitorItemForDA( &const_cast<IsoItem_c&>( m_connection.getVtServerInst().getIsoItem() ) );
    sendData.setMonitorItemForSA( m_connection.getIdentItem().getIsoItem() );
    sendData.setUint8Data( 0, 0x02 );
    sendData.setUint8Data( 1, 0xFD );
    sendData.setUint8Data( 2, 0x00 );
    sendData.setUint8Data( 3, 0xE8 );
    sendData.setUint8Data( 4, 0x03 );
    sendData.setUint8Data( 5, 0xFF );
    sendData.setUint8Data( 6, 0xFF );
    sendData.setUint8Data( 7, 0xFF );
    sendData.setLen( 8 );
    
    getIsoBusInstance( m_connection.getMultitonInst() ) << sendData;
#endif

    m_connection.notifyOnFinishedNonUserPoolUpload(
      men_uploadPoolType == UploadPoolTypeCompleteInitially );

    m_pool.eventObjectPoolUploadedSuccessfully(
      men_uploadPoolType == UploadPoolTypeLanguageUpdate, 
      mi8_objectPoolUploadedLanguage,
      mui16_objectPoolUploadedLanguageCode );
  }
}


void
UploadPoolState_c::initObjectPoolUploadingPhases(
  UploadPoolType_t ren_uploadPoolType, 
  IsoAgLib::iVtObject_c** rppc_listOfUserPoolUpdateObjects, 
  uint16_t aui16_numOfUserPoolUpdateObjects )
{
  isoaglib_assert( m_uploadingVersion != 0 );

  if (ren_uploadPoolType == UploadPoolTypeUserPoolUpdate)
  { // Activate User triggered Partial Pool Update
    if (aui16_numOfUserPoolUpdateObjects == 0)
      return;

    /// INIT FIRST
    ms_uploadPhaseUser.pc_streamer = &mc_iVtObjectStreamer;
    ms_uploadPhaseUser.ui32_size = 1; // the 0x11 command-byte is always there.
    mppc_uploadPhaseUserObjects = rppc_listOfUserPoolUpdateObjects;

    /// COUNT
    for (uint32_t curObject=0; curObject < aui16_numOfUserPoolUpdateObjects; ++curObject)
      ms_uploadPhaseUser.ui32_size += fitTerminalWrapper( *static_cast<vtObject_c*>( mppc_uploadPhaseUserObjects[curObject] ) );
  }
  else
  { // *CONDITIONALLY* Calculate GENERAL Parts sizes
    if( ren_uploadPoolType == UploadPoolTypeCompleteInitially )
    { // start with first phase
      mui_uploadPhaseAutomatic = UploadPhaseFIRSTfix;

      /// Phase 0
      ms_uploadPhasesAutomatic [UploadPhaseIVtObjectsFix].pc_streamer = &mc_iVtObjectStreamer;
      ms_uploadPhasesAutomatic [UploadPhaseIVtObjectsFix].ui32_size = 1; // the 0x11 command-byte is always there.
      for (uint32_t curObject=0; curObject < m_pool.getNumObjects(); ++curObject)
        ms_uploadPhasesAutomatic [UploadPhaseIVtObjectsFix].ui32_size += fitTerminalWrapper( *static_cast<vtObject_c*>( m_pool.getIVtObjects()[0][curObject] ) );

      /// Phase 2
      const STL_NAMESPACE::pair<uint32_t, IsoAgLib::iMultiSendStreamer_c*> cpair_retval = m_pool.getAppSpecificFixPoolData();
      ms_uploadPhasesAutomatic [UploadPhaseAppSpecificFix].pc_streamer = cpair_retval.second;
      ms_uploadPhasesAutomatic [UploadPhaseAppSpecificFix].ui32_size = cpair_retval.first;
    }
    else
    { // start with second phase (lang. dep that is)
      mui_uploadPhaseAutomatic = UploadPhaseFIRSTlang;
    }

    // *ALWAYS* Calculate LANGUAGE Part size (if objectpool has multilanguage!)
    /// Phase 1
    ms_uploadPhasesAutomatic [UploadPhaseIVtObjectsLang].pc_streamer = &mc_iVtObjectStreamer;
    ms_uploadPhasesAutomatic [UploadPhaseIVtObjectsLang].ui32_size = 0; // there may not always be a language part.
    if( m_pool.multiLanguage() )
    {
      // check if we need to fallback to the default-language
      const int8_t realUploadingLanguageAsIndex = calcRealUploadingLanguage( true ) + 1; // skip language-independent objects.

      for (uint32_t curObject=0; curObject < m_pool.getNumObjectsLang(); ++curObject)
        ms_uploadPhasesAutomatic[ UploadPhaseIVtObjectsLang ].ui32_size
          += fitTerminalWrapper( *static_cast<vtObject_c*>( m_pool.getIVtObjects()[ realUploadingLanguageAsIndex ][ curObject ] ) );
      if (ms_uploadPhasesAutomatic[ UploadPhaseIVtObjectsLang ].ui32_size > 0)
      { // only if there's at least one object being streamed up as user-partial-objectpool-update add the CMD byte for size calculation...
        ++ms_uploadPhasesAutomatic[ UploadPhaseIVtObjectsLang ].ui32_size; // add the 0x11 byte!
      }
    } // else: no LANGUAGE SPECIFIC objectpool, so keep this at 0 to indicate this!

    /// Phase 3
    const STL_NAMESPACE::pair<uint32_t, IsoAgLib::iMultiSendStreamer_c*> cpair_retval
      = m_pool.getAppSpecificLangPoolData( calcAppUploadingLanguage(), mui16_objectPoolUploadingLanguageCode );

    ms_uploadPhasesAutomatic [UploadPhaseAppSpecificLang].pc_streamer = cpair_retval.second;
    ms_uploadPhasesAutomatic [UploadPhaseAppSpecificLang].ui32_size = cpair_retval.first;
  }

  men_uploadPoolType = ren_uploadPoolType;
}


unsigned
UploadPoolState_c::calcRealUploadingLanguage( bool considerReject ) const
{
  if( mi8_objectPoolUploadingLanguage < 0 )
    return 0;

  if( considerReject && ( m_langRejectedUseDefaultAsFallback.isBitSet( mi8_objectPoolUploadingLanguage ) ) )
    return 0;

  return unsigned( mi8_objectPoolUploadingLanguage );
}


int8_t
UploadPoolState_c::calcAppUploadingLanguage() const
{
  if( mi8_objectPoolUploadingLanguage < 0 )
    return mi8_objectPoolUploadingLanguage;

  if( m_langRejectedUseDefaultAsFallback.isBitSet( mi8_objectPoolUploadingLanguage ) )
    return -1;

  return mi8_objectPoolUploadingLanguage;
}


bool
UploadPoolState_c::activeAuxO() const
{
  return( m_uploadingVersion == IsoAgLib::iVtClientObjectPool_c::ObjectPoolVersion2 );
}


bool
UploadPoolState_c::activeAuxN() const
{
  return( ( m_uploadingVersion != 0 ) &&
          ( m_uploadingVersion != IsoAgLib::iVtClientObjectPool_c::ObjectPoolVersion2 ) &&
          m_connection.getVtServerInst().isPrimaryVt() );
}

} // __IsoAgLib
