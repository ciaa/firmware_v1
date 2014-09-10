/*
  isbclient_c.cpp: central ISB client management
                   (Stop all implement operations)

  (C) Copyright 2013 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "isbclient_c.h"

#include <IsoAgLib/scheduler/impl/scheduler_c.h>
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/canpkgext_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#include <IsoAgLib/comm/Part7_ApplicationLayer/iisbstatehandler_c.h>
#include <IsoAgLib/util/iassert.h>

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif


//#define DEBUG_ISB_CLIENT


namespace __IsoAgLib {

IsbClient_c &getIsbClientInstance( unsigned instance )
{ // if > 1 singleton instance is used, no static reference can be used
  MACRO_MULTITON_GET_INSTANCE_BODY(IsbClient_c, PRT_INSTANCE_CNT, instance);
}


IsbClient_c::IsbClient_c()
  : SchedulerTask_c( 5000, false ) // dummy, will be set always, used as time-out!
  , m_runState()
  , m_servers()
  , m_callbacks()
  , m_lastCallbackStopAllImplementOperations( false )
  , m_lastCallbackServerCnt( 0 )
  , m_handler( *this )
  , m_customer( *this )
{
}


void
IsbClient_c::init()
{
  isoaglib_assert( !m_runState.initialized() );

  getIsoMonitorInstance4Comm().registerControlFunctionStateHandler( m_handler );
  getIsoBusInstance4Comm().insertFilter( m_customer, IsoAgLib::iMaskFilterType_c(
    0x3FFFF00UL, ALL_IMPLEMENTS_STOP_OPERATIONS_SWITCH_STATE_PGN<<8, Ident_c::ExtendedIdent ), 8 );

  m_runState.setInitialized();
}


void
IsbClient_c::close()
{
  isoaglib_assert( m_runState.initialized() );
  isoaglib_assert( m_callbacks.empty() );

  m_servers.clear();
  m_lastCallbackServerCnt = 0;
  m_lastCallbackStopAllImplementOperations = false;

  getIsoBusInstance4Comm().deleteFilter( m_customer, IsoAgLib::iMaskFilterType_c(
    0x3FFFF00UL, ALL_IMPLEMENTS_STOP_OPERATIONS_SWITCH_STATE_PGN<<8, Ident_c::ExtendedIdent ) );
  getIsoMonitorInstance4Comm().deregisterControlFunctionStateHandler( m_handler );

  m_runState.setClosed();
}


void
IsbClient_c::registerStateHandler( IdentItem_c& identItem, IsoAgLib::iIsbStateHandler_c& callback )
{
  identItem.getDiagnosticFunctionalities().addFunctionalitiesStopAllImplementOperations( true, 1, __IsoAgLib::StopAllImplementOperationsOptionsBitMask_t() );

  m_callbacks.push_back( &callback );
}


void
IsbClient_c::deregisterStateHandler( IdentItem_c& identItem, IsoAgLib::iIsbStateHandler_c& callback )
{
  for( CallbackList_t::iterator iter = m_callbacks.begin(); iter != m_callbacks.end(); ++iter )
  {
    if( *iter != &callback )
      continue;

    m_callbacks.erase( iter );
    break;
  }

  identItem.getDiagnosticFunctionalities().remFunctionalities( StopAllImplementOperationsImplement );
}


void 
IsbClient_c::processMsg( const CanPkg_c& canPkg )
{
  CanPkgExt_c canResolved( canPkg, getMultitonInst() );
  if( ( ! canResolved.isValid() ) || ( canResolved.getMonitorItemForSA() == NULL ) )
    return;

  isoaglib_assert( canResolved.isoPgn() == ALL_IMPLEMENTS_STOP_OPERATIONS_SWITCH_STATE_PGN );
  
  const uint8_t transitions = canResolved.getUint8Data( 7-1 );
  const SwitchState_e switchState = static_cast<SwitchState_e>( canResolved.getUint8Data( 8-1 ) & 0x03 );
  IsbState_s newState( canResolved.time(), transitions, switchState );

  IsoItem_c *serverItem = canResolved.getMonitorItemForSA();
  ServerMap_t::iterator iter = m_servers.find( serverItem );
  if( iter != m_servers.end() )
  { // update state
    IsbState_s &oldState = iter->second;

    // check for erroneous state change
    bool badMessageSequence = false;
    if( ( oldState.m_state == PermitAllImplementsToOperationON )
      &&( newState.m_state == StopImplementOperations ) )
    {
      if( uint8_t( oldState.m_transitionNr+1 ) != newState.m_transitionNr )
        badMessageSequence = true;
    }
    else
    {
      if( oldState.m_transitionNr != newState.m_transitionNr )
        badMessageSequence = true;
    }

    if( badMessageSequence )
    { // internally treat as ERROR INDICATION, should be fine?
      newState.m_state = ErrorIndication;
    }

    oldState = newState;
  }
  else
  { // add new state
    if( m_servers.empty() )
    { // first one
      getSchedulerInstance().registerTask( *this, sc_serverTimeOut );
#ifdef DEBUG_ISB_CLIENT
      std::cout << HAL::getTime() << ": timeOut expected at NOW + " << sc_serverTimeOut << "... = " << (HAL::getTime() + sc_serverTimeOut) << std::endl;
#endif
    }
    m_servers.insert( STL_NAMESPACE::pair<IsoItem_c*, IsbState_s >( serverItem, newState ) );
  }

  handleChangedState();
}


void
IsbClient_c::timeEvent()
{
#ifdef DEBUG_ISB_CLIENT
  std::cout << HAL::getTime() << ": timeEvent(): One server timed out! " << std::endl;
#endif
  handleChangedState();
  // important to call it twice
  // (ISB drop-off => STOP)
  // (then: no ISBs => RELEASE STOP immediately)
  handleChangedState();
}


void
IsbClient_c::handleChangedState()
{
  bool stopAllImplementOperationsNew = false;
  unsigned serverCntNew = 0;

  int32_t nextTimeOut = -1;

  for( ServerMap_t::iterator iter = m_servers.begin(); iter != m_servers.end(); )
  {
    if( iter->second.timedOut() )
    {
      stopAllImplementOperationsNew = true;
      m_servers.erase( iter++ );
    }
    else
    {
      const int32_t thisTimeOut = iter->second.m_timeReceived + sc_serverTimeOut;
      if( ( nextTimeOut < 0 ) || ( thisTimeOut < nextTimeOut ) )
        nextTimeOut = thisTimeOut;

      switch( iter->second.m_state )
      {
      case StopImplementOperations:
      case ErrorIndication:
        stopAllImplementOperationsNew = true;
        break;

      case PermitAllImplementsToOperationON:
      case NotAvailable: // @todo Not sure what to do in N/A case...
        break;
      }
      
      ++serverCntNew;
      ++iter;
    }
  }

  if( nextTimeOut != -1 )
  {
    SchedulerTask_c::setNextTriggerTime( nextTimeOut );
#ifdef DEBUG_ISB_CLIENT
    std::cout << HAL::getTime() << ": TimeOut expected at " << nextTimeOut << "..." << std::endl;
#endif
  }
  else // last one
  {
    if( isRegistered() )
    {
      getSchedulerInstance().deregisterTask( *this );
#ifdef DEBUG_ISB_CLIENT
      std::cout << HAL::getTime() << ": No more time-out detection needed..." << std::endl;
#endif
    }
  }

  if( m_lastCallbackStopAllImplementOperations != stopAllImplementOperationsNew )
    for( CallbackList_t::iterator iter = m_callbacks.begin(); iter != m_callbacks.end(); ++iter )
      (*iter)->stopAllImplementOperations( stopAllImplementOperationsNew );

  if( m_lastCallbackServerCnt != serverCntNew )
    for( CallbackList_t::iterator iter = m_callbacks.begin(); iter != m_callbacks.end(); ++iter )
      (*iter)->numberOfServers( serverCntNew );

  m_lastCallbackStopAllImplementOperations = stopAllImplementOperationsNew;
  m_lastCallbackServerCnt = serverCntNew;
}


void
IsbClient_c::reactOnIsoItemModification( ControlFunctionStateHandler_c::iIsoItemAction_e action, IsoItem_c const &item )
{
  if( action != ControlFunctionStateHandler_c::RemoveFromMonitorList )
    return;

  for( ServerMap_t::iterator iter = m_servers.begin(); iter != m_servers.end(); ++iter )
  {
    if( iter->first != &item )
      continue;

    iter->second.m_timeReceived = -1; // act as if it just "timed out..."
    retriggerNow(); // gets timeEvent calledf ro proper "timeOut"-handling
    break;
  }
}


} // __IsoAgLib
