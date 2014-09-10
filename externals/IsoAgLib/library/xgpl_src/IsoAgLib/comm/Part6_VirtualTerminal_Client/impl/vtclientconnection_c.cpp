/*
  vtclientconnection_c.cpp: class for managing the
    connection between vt client and server

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
//#define DEBUG_MULTIPLEVTCOMM 1
#include "vtclientconnection_c.h"
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multireceive_c.h>
#ifdef HAL_USE_SPECIFIC_FILTERS
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isofiltermanager_c.h>
#endif
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtclient_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtclientconnection_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtobjectauxiliaryfunction2_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtobjectauxiliaryinput2_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/impl/vtserverinstance_c.h>
#include <IsoAgLib/util/iassert.h>

#if DEBUG_VTCOMM
  #include <supplementary_driver/driver/rs232/impl/rs232io_c.h>
  #include <IsoAgLib/util/impl/util_funcs.h>
  #ifdef SYSTEM_PC
    #include <iostream>
  #else
    #include <supplementary_driver/driver/rs232/impl/rs232io_c.h>
  #endif
#endif

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif

#if defined( DEBUG_MULTIPLEVTCOMM ) && defined( SYSTEM_PC )
#include <iomanip>
std::ostream& operator<<(std::ostream& os, const __IsoAgLib::IsoName_c& dt)
{
  if (dt.isUnspecified())
    os << " [not specified] ";
  else
  {
    os << std::hex;
    for (uint8_t counter = 0; counter < 8; ++counter) os << std::setw(2) << std::setfill('0') << (uint16_t)dt.outputString()[counter];
    os << std::dec;
  }
  return os;
}
#endif


namespace __IsoAgLib {

bool
VtClientConnection_c::reactOnStreamStart (const ReceiveStreamIdentifier_c& ac_ident, uint32_t aui32_totalLen)
{
  // not connected to a VT (no Alive), but there could still come messages in from that SA
  if( !isVtActive() )
    return false;

  // if SA is not the address from the vt -> don't react on stream
  if ((ac_ident.getSaIsoName()) != (mpc_vtServerInstance->getIsoName()))
    return false;

  if (aui32_totalLen > (2+(255*32)) ) // Annex E.11 is the current max to receive!
    /** @todo SOON-258 Should we really ConnAbort such a stream in advance? For now don't care too much, as it shouldn't happen! */
    return false;

  return true;
}


bool
VtClientConnection_c::processPartStreamDataChunk (Stream_c& stream, bool isFirstChunk, bool isLastChunk)
{
  ( void )isFirstChunk;

  if( !isLastChunk || !isVtActive())
    return false;

  switch( men_uploadType )
  {
  case UploadPool:
    m_uploadPoolState.processMsgVtToEcu( stream );
    break;

  case UploadIdle:
    m_commandHandler.processMsgVtToEcuActivations( stream );
    break;

  case UploadCommand:
    m_commandHandler.processMsgVtToEcuActivations( stream );
  //m_commandHandler.processMsgVtToEcuResponses( pkg ); // currently no (E)TP-responses
    break;
  }

  return false;
}


VtClientConnection_c::VtClientConnection_c(
  IdentItem_c& r_wsMasterIdentItem,
  VtClient_c &r_vtclient,
  IsoAgLib::iVtClientObjectPool_c& pool,
  const char* versionLabel,
  IsoAgLib::iVtClientDataStorage_c& arc_dataStorage,
  uint8_t aui8_clientId,
  IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_en aen_mode )
  : mb_vtAliveCurrent( false ) // so we detect the rising edge when the VT gets connected!
  , mrc_wsMasterIdentItem( r_wsMasterIdentItem )
  , mrc_vtClient( r_vtclient )
  , mpc_vtServerInstance( NULL )
  , men_uploadType( UploadIdle )
#ifdef ENABLE_VTCLIENT_RETRY
  , m_uploadRetry( 0 )
#endif
  , mi32_nextWsMaintenanceMsg( -1 )
  , mui8_clientId( aui8_clientId )
  , men_displayState( VtClientDisplayStateHidden )
  , mlist_auxAssignments()
  , m_aux2Inputs( r_wsMasterIdentItem )
  , m_aux2Functions( *this )
  , m_uploadPoolState( *this, pool, versionLabel, ( aen_mode != IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_Slave ) )
  , m_commandHandler( *this )
  , m_multipleVt(m_uploadPoolState, r_vtclient)
  , mi32_timeWsAnnounceKey( -1 ) // no announce tries started yet...
  , mi32_fakeVtOffUntil( -1 ) // no faking initially
  , m_cmdTimedOut( 0x00 ) // nothing failed initially
  , men_registerPoolMode( aen_mode )
  , mc_preferredVt( IsoName_c::IsoNameUnspecified() )
  , mi32_bootTime_ms( 0 )
  , m_dataStorageHandler( arc_dataStorage )
  , m_schedulerTaskProxy( *this, 100, false )
{
  // can't be done in c'tor due to back-ref to *this
  m_uploadPoolState.initPool();

  r_wsMasterIdentItem.getDiagnosticFunctionalities().addFunctionalitiesUniversalTerminal(
    true,
    static_cast<uint8_t>(getPool().getVersion()),
    UniversalTerminalOptionsBitMask_t() );

#if defined(USE_VTOBJECT_auxiliaryfunction2) || defined (USE_VTOBJECT_auxiliaryinput2)
  for (uint16_t ui16_objIndex = 0; ui16_objIndex < getPool().getNumObjects(); ui16_objIndex++)
  {
    IsoAgLib::iVtObject_c* p_obj = getPool().getIVtObjects()[0][ui16_objIndex];
#ifdef USE_VTOBJECT_auxiliaryfunction2
    if (p_obj->getObjectType() == IsoAgLib::iVtObjectAuxiliaryFunction2_c::objectType())
    { // collect all available AUX 2 function objects in list
      m_aux2Functions.getObjects()[ p_obj->getID() ] = static_cast<IsoAgLib::iVtObjectAuxiliaryFunction2_c*>(p_obj);
    }
#endif
#ifdef USE_VTOBJECT_auxiliaryinput2
    if (p_obj->getObjectType() == IsoAgLib::iVtObjectAuxiliaryInput2_c::objectType())
    { // collect all available AUX 2 input objects in list
      m_aux2Inputs.getObjectList().push_back(static_cast<IsoAgLib::iVtObjectAuxiliaryInput2_c*>(p_obj));
    }
#endif
  }
#endif

#ifdef USE_VTOBJECT_auxiliaryfunction2
  if (!m_aux2Functions.getObjects().empty())
  {
    m_aux2Functions.loadAssignment();
  }
#endif
#ifdef USE_VTOBJECT_auxiliaryinput2
  if (!m_aux2Inputs.getObjectList().empty())
  {
    m_aux2Inputs.init(this);
  }
#endif

  // load the preferred ISOVT
  uint8_t bootTime_s = 0;
  m_dataStorageHandler.loadPreferredVt( mc_preferredVt.toIisoName_c(), bootTime_s );
  mi32_bootTime_ms = (bootTime_s!=0xFF) ? (bootTime_s * 1000) : 0; // 0xFF means the feature is not supported by the VT
#if defined( DEBUG_MULTIPLEVTCOMM ) && defined( SYSTEM_PC )
  INTERNAL_DEBUG_DEVICE << "LOAD PreferredVt with timeout " << mi32_bootTime_ms << " and NAME = " << mc_preferredVt << INTERNAL_DEBUG_DEVICE_ENDL;
#endif

  getSchedulerInstance().registerTask( m_schedulerTaskProxy, 0 );

  getMultiReceiveInstance4Comm().registerClientIso (*this, getIdentItem().isoName(), VT_TO_ECU_PGN);
#ifdef HAL_USE_SPECIFIC_FILTERS
  getIsoFilterManagerInstance4Comm().insertIsoFilter (IsoFilter_s (mrc_vtClient.mt_customer, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (VT_TO_ECU_PGN << 8) ), &getIdentItem().isoName(), NULL, 8));
  getIsoFilterManagerInstance4Comm().insertIsoFilter (IsoFilter_s (mrc_vtClient.mt_customer, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (ACKNOWLEDGEMENT_PGN << 8) ), &getIdentItem().isoName(), NULL, 8));
#endif
}


VtClientConnection_c::~VtClientConnection_c()
{
#ifdef HAL_USE_SPECIFIC_FILTERS
  getIsoFilterManagerInstance4Comm().removeIsoFilter (IsoFilter_s (mrc_vtClient.mt_customer, IsoAgLib::iMaskFilter_c( (0x3FFFF00UL), (ACKNOWLEDGEMENT_PGN << 8) ), &getIdentItem().isoName(), NULL, 8));
  getIsoFilterManagerInstance4Comm().removeIsoFilter (IsoFilter_s (mrc_vtClient.mt_customer, IsoAgLib::iMaskFilter_c( (0x3FFFF00UL), (VT_TO_ECU_PGN << 8) ), &getIdentItem().isoName(), NULL, 8));
#endif
  getMultiReceiveInstance4Comm().deregisterClient (*this);

  getSchedulerInstance().deregisterTask( m_schedulerTaskProxy );
}


void 
VtClientConnection_c::timeEvent()
{
  if (!mrc_wsMasterIdentItem.isClaimedAddress())
    return;

  // VT Alive checks - Will trigger "doStart" / "doStop"
  /// @todo in Slave mode, don't call doStart as it sets to UploadPool!
  /// but for now, Slave mode is not yet really supported, so beware!

  // setup all states properly first (for example: UploadState_c::men_uploadPoolState will be used in m_multipleVt.timeEvent())
  checkAndHandleVtStateChange();
  
  if (IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_MasterToAnyVt == men_registerPoolMode)
  {
    switch(m_multipleVt.timeEvent())
    {
      case MultipleVt_c::TIME_EVENT_RESULT_TRY_NEXT_VT:
        restartWithNextVt();
        break;
      case MultipleVt_c::TIME_EVENT_RESULT_NO_ACTION:
        break;
    }
  }

  
  // Do nothing if there's no VT active
  if (!isVtActive())
  {
    timeEventSearchForNewVt();
    return;
  }

  // Only Master needs a proper announce-sequence / pool upload!
  if (IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_Slave != men_registerPoolMode)
  {
    if( !timeEventMaster() )
      return;
  }

  m_uploadPoolState.timeEventLanguageUpdate();

  switch( men_uploadType )
  {
  case UploadPool:
    m_uploadPoolState.timeEvent();
    break;

  case UploadIdle:
    if( commandHandler().tryToStart() )
    {
      men_uploadType = UploadCommand;
#ifdef ENABLE_VTCLIENT_RETRY
      m_uploadRetry = 0; // first try
#endif
    }
    break;

  case UploadCommand:
    const uint8_t cmdTimedOut = commandHandler().timeEventCommandTimeoutCheck();
    if( cmdTimedOut == 0x00 )
      break;

#ifdef ENABLE_VTCLIENT_RETRY
    if( m_uploadRetry < 2 )
    {
      ( void )commandHandler().tryToStart();
      ++m_uploadRetry;
      //std::cout << "MISSED RESPONSE - RETRY #" << m_uploadRetry << std::endl;
      break;
    }
#endif

    // let the application know of the failed command!
    m_cmdTimedOut = cmdTimedOut;
#ifdef CONFIG_VT_CLIENT_ON_MISSING_RESPONSE_CONTINUE
    commandHandler().finishUploadCommand(); 
    getPool().eventAllRetriesFailedNowContinuing();
#else
    // It's the safest thing to just reconnect to the VT.
    // So let's get disconnected (can't do this actively)
    fakeVtOffPeriod (6000); // fake the VT 6 seconds off!
#endif
    break;
  }
}


bool
VtClientConnection_c::timeEventMaster()
{
  // Check if the working-set is completely announced
  if( !mrc_wsMasterIdentItem.getIsoItem()->isWsAnnounced( mi32_timeWsAnnounceKey ) )
    return false;

  // Check if periodic WS-Maintenance is needed
  if( (mi32_nextWsMaintenanceMsg <= 0) || (HAL::getTime() >= mi32_nextWsMaintenanceMsg) )
  {
    uint8_t ui8_sendAtStartup = (mi32_nextWsMaintenanceMsg <= 0) ? 1 : 0;

    uint8_t ui8_version = 0xFF;
    switch (getPool().getVersion())
    {
      case IsoAgLib::iVtClientObjectPool_c::ObjectPoolVersion2:
        ui8_version = 0xFF;
        ui8_sendAtStartup = 0xFF; // "send at startup" is only used for version 3 and later
        break;
      case IsoAgLib::iVtClientObjectPool_c::ObjectPoolVersion3:
        ui8_version = 3;
        break;
      case IsoAgLib::iVtClientObjectPool_c::ObjectPoolVersion4:
        ui8_version = 4;
        break;
    }

    // G.2: Function: 255 / 0xFF Working Set Maintenance Message
    sendMessage( 0xFF, ui8_sendAtStartup, ui8_version, 0xff, 0xff, 0xff, 0xff, 0xff );
    mi32_nextWsMaintenanceMsg = HAL::getTime() + 1000;
  }

  if( !m_uploadPoolState.timeEventCalculateLanguage() )
    return false;

  return true;
}


void
VtClientConnection_c::timeEventSearchForNewVt()
{
  switch (men_registerPoolMode)
  {
    case IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_Slave:
      return; // Slave needs to get the Master's VT-ISONAME told proprietarily!

    case IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_MasterToSpecificVt:
      mpc_vtServerInstance = getVtClientInstance4Comm().getSpecificVtServer( getPool() );
      return;

    case IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_MasterToAnyVt:
        if(m_multipleVt.isSwitching())
        {
          mpc_vtServerInstance = getVtClientInstance4Comm().getActiveVtServer(false /* mustBePrimary */,
                                                                              m_multipleVt.getLastVtServerInstance());
#if defined( DEBUG_MULTIPLEVTCOMM ) && defined( SYSTEM_PC )
          if (mpc_vtServerInstance)
          {
            INTERNAL_DEBUG_DEVICE << "VT not active and searching for next VT -> FOUND NEW VT" << INTERNAL_DEBUG_DEVICE_ENDL;
          }
#endif            
          return;
        }
        else
        {
          // break intentionally left out
        }
        
    case IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_MasterToPrimaryVt:
      // check if initial timeout is done
      if (isPreferredVTTimeOut())
      {
        // check if other VT is available
        mpc_vtServerInstance = getVtClientInstance4Comm().getActiveVtServer(
            (IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_MasterToPrimaryVt == men_registerPoolMode),
            NULL );
        
#if defined( DEBUG_MULTIPLEVTCOMM ) && defined( SYSTEM_PC )
        static bool sb_foundNewVt = false;
        if (mpc_vtServerInstance && !sb_foundNewVt)
        {
          sb_foundNewVt = true;
          INTERNAL_DEBUG_DEVICE << "VT not active and PreferredVTTimeOut is out -> FOUND NEW VT" << INTERNAL_DEBUG_DEVICE_ENDL;
        }
        else if (!mpc_vtServerInstance && sb_foundNewVt)
        {
          sb_foundNewVt = false;
          INTERNAL_DEBUG_DEVICE << "VT not active and PreferredVTTimeOut is out -> NO NEW VT FOUND" << INTERNAL_DEBUG_DEVICE_ENDL;
        }
#endif
      }
      else
      {
        mpc_vtServerInstance = getVtClientInstance4Comm().getPreferredVtServer(mc_preferredVt);
        if (mpc_vtServerInstance != NULL)
          mi32_bootTime_ms = 0;
#if defined( DEBUG_MULTIPLEVTCOMM ) && defined( SYSTEM_PC )
        static bool sb_foundNewVt = false;
        if (mpc_vtServerInstance && !sb_foundNewVt)
        {
          sb_foundNewVt = true;
          INTERNAL_DEBUG_DEVICE << "take PreferredVT" << INTERNAL_DEBUG_DEVICE_ENDL;
        }
#endif
      }
      break;
  }
}


void
VtClientConnection_c::processMsgAck( const CanPkgExt_c& arc_data )
{
  isoaglib_assert( mpc_vtServerInstance );

  if( arc_data.getUint8Data( 0 ) != 0x01 )
    return; // Only react if "NOT ACKNOWLEDGE"!

#if !defined(IGNORE_VTSERVER_NACK)  // The NACK must be ignored for the Mueller VT Server
  // check if we have Agrocom/Mller with Version < 3, so we IGNORE this NACK BEFORE the pool is finally uploaded.
  bool b_ignoreNack = false; // normally DO NOT ignore NACK

  const uint16_t cui16_manufCode = getVtServerInst().getIsoName().manufCode();
  if (((cui16_manufCode == 98) /*Mueller Elektronik*/ || (cui16_manufCode == 103) /*Agrocom*/) &&
        ((mpc_vtServerInstance->getVtCapabilities()->lastReceivedVersion == 0) ||
        (mpc_vtServerInstance->getVtCapabilities()->iso11783version < 3)))
  {
    if( !m_uploadPoolState.successfullyUploaded() )
    { // mueller/agrocom hack - ignore upload while no objectpool is displayed
      b_ignoreNack = true;
    }
  }

  if( !b_ignoreNack )
  {
    // for now ignore source address which must be VT of course. (but in case a NACK comes in before the first VT Status Message
    // Check if a VT-related message was NACKed. Check embedded PGN for that
    const uint32_t cui32_pgn =  uint32_t (arc_data.getUint8Data (5)) |
                               (uint32_t (arc_data.getUint8Data (6)) << 8) |
                               (uint32_t (arc_data.getUint8Data (7)) << 16);
    switch( cui32_pgn )
    {
      case ECU_TO_VT_PGN:
      case WORKING_SET_MEMBER_PGN:
      case WORKING_SET_MASTER_PGN:
#if DEBUG_VTCOMM
        INTERNAL_DEBUG_DEVICE << "\n==========================================================================================="
                              << "\n=== VT NACKed "<<cui32_pgn<<", starting all over again -> faking VT loss in the following: ===";
#endif
        restart();
        break;
    }
  }
#endif
}


void
VtClientConnection_c::restart()
{
  mrc_wsMasterIdentItem.getIsoItem()->sendSaClaim(); // optional, but better do this: Repeat address claim!
  /// passing "true": fake NOT-alive state of VT for now!
  fakeVtOffPeriod( 1000 ); // arbitrary time-span > 0 so checkAndHandle..() will call doStop!
  checkAndHandleVtStateChange(); // will also notify application via "eventEnterSafeState"
  fakeVtOffStop(); // enough faking, let it get restart asap in the timeEvent!
}


void
VtClientConnection_c::notifyOnVtsLanguagePgn()
{
  m_uploadPoolState.notifyOnVtsLanguagePgn();
}


void
VtClientConnection_c::notifyOnVtStatusMessage()
{
  getPool().eventVtStatusMsg();

  // set client display state appropriately
  setVtDisplayState( getVtServerInst().getVtState()->saOfActiveWorkingSetMaster );

  if( m_uploadPoolState.activeAuxN() )
  {
    m_aux2Inputs.setLearnMode((getVtServerInst().getVtState()->busyCodes & (1<<6)) != 0);
    m_aux2Functions.setLearnMode((getVtServerInst().getVtState()->busyCodes & (1<<6)) != 0);
  }
}


void
VtClientConnection_c::notifyOnAuxInputStatus( const CanPkgExt_c& pkg )
{
  const IsoName_c& ac_inputIsoName = pkg.getISONameForSA();
  uint8_t const cui8_inputNumber = pkg.getUint8Data(2-1);

  // Look for all Functions that are controlled by this Input right now!
  for( STL_NAMESPACE::list<AuxAssignment_s>::iterator it = mlist_auxAssignments.begin();
       it != mlist_auxAssignments.end(); ++it )
  {
    if ( (it->mui8_inputNumber == cui8_inputNumber)
      && (it->mc_inputIsoName == ac_inputIsoName) )
    { // notify application on this new Input Status!
      uint16_t const cui16_inputValueAnalog = pkg.getUint16Data (3-1);
      uint16_t const cui16_inputValueTransitions = pkg.getUint16Data (5-1);
      uint8_t const cui8_inputValueDigital = pkg.getUint8Data (7-1);
      
      getPool().eventAuxFunctionValue(
        it->mui16_functionUid, 
        cui16_inputValueAnalog, 
        cui16_inputValueTransitions, 
        cui8_inputValueDigital );
    }
  }
}


bool
VtClientConnection_c::storeAuxAssignment( const CanPkgExt_c& pkg )
{
  uint8_t const cui8_inputSaNew = pkg.getUint8Data (2-1);
  uint8_t const cui8_inputNrNew = pkg.getUint8Data (3-1); /// 0xFF means unassign!
  uint16_t const cui16_functionUidNew = pkg.getUint16Data (4-1);
  IsoItem_c *inputIsoNameNew = getIsoMonitorInstance4Comm().item( cui8_inputSaNew );
  if ( (inputIsoNameNew == NULL) && (cui8_inputNrNew != 0xFF))
    return false;

  for( STL_NAMESPACE::list<AuxAssignment_s>::iterator it = mlist_auxAssignments.begin();
       it != mlist_auxAssignments.end(); )
  {
    if (it->mui16_functionUid == cui16_functionUidNew)
    { // we already have an assignment for this function
      // do we have to replace OR unassign?
      if (cui8_inputNrNew == 0xFF)
      { /// Unassign
        it = mlist_auxAssignments.erase (it);
        continue; // look through all assignments anyway, although we shouldn't have any doubles in there...
      }
      else
      { /// Reassign
        isoaglib_assert( inputIsoNameNew != NULL ); // else the check above would've "return false;"
        const IsoName_c& ac_inputIsoNameNew = inputIsoNameNew->isoName();
        it->mc_inputIsoName = ac_inputIsoNameNew;
        it->mui8_inputNumber = cui8_inputNrNew;
        return true;
      }
    }
    ++it;
  }

  // Function not found, so we need to add (in case it was NOT an unassignment)
  if( cui8_inputNrNew == 0xFF )
    return true; // unassignment is always okay!

  AuxAssignment_s s_newAuxAssignment;
  isoaglib_assert( inputIsoNameNew != NULL ); // else the check above would've "return true;"
  const IsoName_c& ac_inputIsoNameNew = inputIsoNameNew->isoName();
  s_newAuxAssignment.mc_inputIsoName = ac_inputIsoNameNew;
  s_newAuxAssignment.mui8_inputNumber = cui8_inputNrNew;
  s_newAuxAssignment.mui16_functionUid = cui16_functionUidNew;

  mlist_auxAssignments.push_back( s_newAuxAssignment );
  return true;
}


uint8_t
VtClientConnection_c::storeAux2Assignment( Stream_c& stream, uint16_t& rui16_functionObjId )
{
  return m_aux2Functions.storeAux2Assignment( stream, rui16_functionObjId, getPool() );
}


void
VtClientConnection_c::processMsgVtToEcu( const CanPkgExt_c& pkg )
{
  if( !isVtActive() )
    return;

  switch( men_uploadType )
  {
  case UploadPool:
    m_uploadPoolState.processMsgVtToEcu( pkg );
    break;

  case UploadIdle:
    m_commandHandler.processMsgVtToEcuActivations( pkg );
    break;

  case UploadCommand:
    m_commandHandler.processMsgVtToEcuActivations( pkg );
    m_commandHandler.processMsgVtToEcuResponses( pkg );
    break;
  }
}


uint8_t
VtClientConnection_c::getUserClippedColor (uint8_t colorValue, IsoAgLib::iVtObject_c* obj, IsoAgLib::e_vtColour whichColour)
{
  if( !poolSuccessfullyUploaded() )
    return colorValue;

  uint8_t colorDepth = mpc_vtServerInstance->getVtCapabilities()->hwGraphicType;
  if( ((colorDepth == 0) && (colorValue > 1)) || ((colorDepth == 1) && (colorValue > 16)) )
    return getPool().convertColour (colorValue, colorDepth, obj, whichColour);
  else
    return colorValue;
}


bool
VtClientConnection_c::moveToNextVt()
{
  bool result = false;
  
  result = m_commandHandler.sendCommandDeleteObjectPool();

  if(result)
  {
    m_multipleVt.trySwitchingState();
  }
  
  return result;
}

void
VtClientConnection_c::restartWithNextVt()
{
  if(m_multipleVt.isSwitching())
  {
    // call doStop() and set mb_vtAliveCurrent properly
    restart(); 
   
    m_multipleVt.setLastVtServerInstance(mpc_vtServerInstance);
    
    mpc_vtServerInstance = NULL;
    
    m_multipleVt.resetInfoShown();

    // call doStart() in next checkAndHandleVtStateChange()
  }
}


void
VtClientConnection_c::notifyOnVtServerInstanceLoss( VtServerInstance_c& oldVtServerInst )
{
  if( &oldVtServerInst == mpc_vtServerInstance )
    mpc_vtServerInstance = NULL;

  if(&oldVtServerInst == m_multipleVt.getLastVtServerInstance())
  {
    m_multipleVt.setLastVtServerInstance(NULL);
    // switching state can continue
  }
}


void
VtClientConnection_c::doStart()
{
  mi32_timeWsAnnounceKey = mrc_wsMasterIdentItem.getIsoItem()->startWsAnnounce();
  mi32_nextWsMaintenanceMsg = 0;

  men_uploadType = UploadPool;
  m_uploadPoolState.doStart();
}


void
VtClientConnection_c::doStop()
{
  m_commandHandler.doStop();
  m_uploadPoolState.doStop();

  if( getVtServerInstPtr() )
    getMultiSendInstance( getMultitonInst() ).abortSend(
      getIdentItem().isoName(), 
      getVtServerInst().getIsoName() );

  // Do NOT do this only if "m_uploadPoolState.activeAuxN()"
  // because that would require a valid VtServerInst_c for checking the Function Instance
  // So we just always re-set the Aux2 states now. This could be done in doStart() maybe
  // if it doesn't hurt to have some states set in Aux2 while it's stopped.......
  m_aux2Functions.setState(Aux2Functions_c::State_WaitForPoolUploadSuccessfully);
#ifdef USE_VTOBJECT_auxiliaryinput2
  if (!m_aux2Inputs.getObjectList().empty())
  {
    m_aux2Inputs.setState(Aux2Inputs_c::Aux2InputsState_Initializing);
  }
#endif

  getPool().eventEnterSafeState();

  // set display state of the client to a defined state
  men_displayState = VtClientDisplayStateHidden;
}


bool
VtClientConnection_c::isPreferredVTTimeOut() const
{
  return( HAL::getTime() > mi32_bootTime_ms );
}


void
VtClientConnection_c::checkAndHandleVtStateChange()
{
  const bool b_vtAliveOld = mb_vtAliveCurrent;
  mb_vtAliveCurrent = isVtActive();

  if (!b_vtAliveOld && mb_vtAliveCurrent)
  { /// OFF --> ON  ***  VT has (re-)entered the system
#if DEBUG_VTCOMM || DEBUG_MULTIPLEVTCOMM
    INTERNAL_DEBUG_DEVICE
      << INTERNAL_DEBUG_DEVICE_NEWLINE << "=========================================================================="
      << INTERNAL_DEBUG_DEVICE_NEWLINE << "=== VT has entered the system, trying to receive all Properties now... ==="
      << INTERNAL_DEBUG_DEVICE_NEWLINE << "=== time: " << HAL::getTime() << " ==="
      << INTERNAL_DEBUG_DEVICE_NEWLINE << "=========================================================================="
      << INTERNAL_DEBUG_DEVICE_NEWLINE << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
    doStart();
  }
  else if (b_vtAliveOld && !mb_vtAliveCurrent)
  { /// ON -> OFF  ***  Connection to VT lost
#if DEBUG_VTCOMM || DEBUG_MULTIPLEVTCOMM
    const bool cb_fakeVtOff = ((mi32_fakeVtOffUntil >= 0) && (HAL::getTime() < mi32_fakeVtOffUntil));
    INTERNAL_DEBUG_DEVICE
      << INTERNAL_DEBUG_DEVICE_NEWLINE << "=============================================================================="
      << INTERNAL_DEBUG_DEVICE_NEWLINE << "=== VT has left the system, clearing queues --> eventEnterSafeState called ==="
      << INTERNAL_DEBUG_DEVICE_NEWLINE << (cb_fakeVtOff ? "=== (as it was forced to)                                                  ===" : "")
      << INTERNAL_DEBUG_DEVICE_NEWLINE << "=== time: " << HAL::getTime() << " ==="
      << INTERNAL_DEBUG_DEVICE_NEWLINE << "=============================================================================="
      << INTERNAL_DEBUG_DEVICE_NEWLINE << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
    doStop();
  }
}


bool
VtClientConnection_c::isVtActive() const
{
  if( mpc_vtServerInstance == NULL )
    return false;

  const bool cb_fakeVtOff = ((mi32_fakeVtOffUntil >= 0) && (HAL::getTime() < mi32_fakeVtOffUntil));

  return !cb_fakeVtOff && mpc_vtServerInstance->isVtActive();
}


void
VtClientConnection_c::notifyOnFinishedNonUserPoolUpload( bool initialUpload )
{
  if( initialUpload )
    men_uploadType = UploadIdle;

  // save this ISOVT as the preferred one
  mc_preferredVt = mpc_vtServerInstance->getIsoName();
  m_dataStorageHandler.storePreferredVt(mc_preferredVt.toConstIisoName_c(), mpc_vtServerInstance->getConstVtCapabilities()->bootTime );
#if defined( DEBUG_MULTIPLEVTCOMM ) && defined( SYSTEM_PC )
  INTERNAL_DEBUG_DEVICE << "SAVE preferredVT with current address " << (uint16_t)mpc_vtServerInstance->getIsoItem().nr()
                        << " NAME = " << mpc_vtServerInstance->getIsoName()
                        << " Boottime = " << (uint16_t)mpc_vtServerInstance->getConstVtCapabilities()->bootTime
                        << INTERNAL_DEBUG_DEVICE_ENDL;
#endif

  if( m_uploadPoolState.activeAuxN() )
  {
    // set internal state and send empty preferred AUX2 assignment message, if we don't have any preferred assignments
    m_aux2Functions.objectPoolUploadedSuccessfully();
#ifdef USE_VTOBJECT_auxiliaryinput2
    if (!m_aux2Inputs.getObjectList().empty())
    {
      m_aux2Inputs.setState(Aux2Inputs_c::Aux2InputsState_Ready);
    }
#endif
  }

  m_multipleVt.resetSwitchingData();
}


void
VtClientConnection_c::setVtDisplayState( uint8_t ui8_sa )
{
  if( !m_uploadPoolState.successfullyUploaded() )
    return;

  // as we don't properly seem to reset "men_objectPoolState" at doStop(), we'll for now add the extra
  // isAddress-Claimed-check here for safety:
  if( !getIdentItem().isClaimedAddress() )
    return;

  vtClientDisplayState_t newDisplayState = 
    (ui8_sa == getIdentItem().getIsoItem()->nr())
      ? VtClientDisplayStateActive
      : VtClientDisplayStateHidden;

  if (newDisplayState != getVtDisplayState())
  {
    men_displayState = newDisplayState;
    getPool().eventDisplayActivation();
  }
}


void
VtClientConnection_c::sendMessage(
  uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 )
{
  CanPkgExt_c sendData;
  
  sendData.setIsoPri( 7 );
  sendData.setIsoPgn( ECU_TO_VT_PGN );
  sendData.setMonitorItemForDA( &const_cast<IsoItem_c&>( getVtServerInst().getIsoItem() ) );
  sendData.setMonitorItemForSA( getIdentItem().getIsoItem() );
  sendData.setUint8Data( 0, d0 );
  sendData.setUint8Data( 1, d1 );
  sendData.setUint8Data( 2, d2 );
  sendData.setUint8Data( 3, d3 );
  sendData.setUint8Data( 4, d4 );
  sendData.setUint8Data( 5, d5 );
  sendData.setUint8Data( 6, d6 );
  sendData.setUint8Data( 7, d7 );
  sendData.setLen( 8 );
  
  getIsoBusInstance4Comm() << sendData;
}


IsoAgLib::iVtClientConnection_c&
VtClientConnection_c::toInterfaceReference()
{
  return static_cast<IsoAgLib::iVtClientConnection_c&>( *this );
}


IsoAgLib::iVtClientConnection_c*
VtClientConnection_c::toInterfacePointer()
{
  return static_cast<IsoAgLib::iVtClientConnection_c*>( this );
}


uint16_t
VtClientConnection_c::getVtObjectPoolSoftKeyWidth() const
{
  return getPool().getSkWidth();
}


uint16_t
VtClientConnection_c::getVtObjectPoolDimension() const
{
  return getPool().getDimension();
}


uint16_t
VtClientConnection_c::getVtObjectPoolSoftKeyHeight() const
{
  return getPool().getSkHeight();
}


} // __IsoAgLib
