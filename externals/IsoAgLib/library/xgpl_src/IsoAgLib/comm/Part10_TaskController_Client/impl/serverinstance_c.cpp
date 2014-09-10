/*
  serverinstance_c.cpp: Instance of a TC or DL

  (C) Copyright 2013 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "serverinstance_c.h"
#include "tcclient_c.h"
#include "processpkg_c.h"
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoitem_c.h>


namespace __IsoAgLib {


  ServerInstance_c::ServerInstance_c( const IsoItem_c& it, IsoAgLib::ProcData::RemoteType_t type ) :
    SchedulerTask_c( 100, false ),
    m_isoItem( it ),
    m_tcAliveCached( false ),
    m_lastActiveTaskTC( false ),
    m_lastTcState( 0 ),
    m_lastTcStateReceivedTime( -1 ),
    m_type( type ),
    m_connections()
  {
    getSchedulerInstance().registerTask(*this,0);
  }


  ServerInstance_c::~ServerInstance_c() {
    isoaglib_assert(m_connections.empty());
    getSchedulerInstance().deregisterTask(*this);
  }


  void
  ServerInstance_c::processMsgNonGlobal( const ProcessPkg_c& pkg ) {
    for( STL_NAMESPACE::list<TcClientConnection_c*>::const_iterator it = m_connections.begin();
         it != m_connections.end(); ++it )
    {
      if (&(*it)->getIsoItem() == pkg.getMonitorItemForSA())
      {
        ( *it )->processProcMsg( pkg );
        return;
      }
    }
  }


  void ServerInstance_c::timeEvent() {
    const bool tcStatusOld = m_tcAliveCached;
    m_tcAliveCached = isAlive();
    if (m_tcAliveCached != tcStatusOld) {
      getTcClientInstance(m_isoItem.getMultitonInst()).notifyServerStatusChange(*this, m_tcAliveCached);
    }
  }


  bool ServerInstance_c::isAlive() const {
    return ( getLastStatusTime() != -1 ) && ( HAL::getTime() - getLastStatusTime() <= 6000 );
  }


  void  ServerInstance_c::addConnection( TcClientConnection_c& c ) {
    // avoid double connect
    // - manual connect from event-callback (probably due to app. only connecting later)
    // - reconnect inside of ISOAgLib
    for( STL_NAMESPACE::list<TcClientConnection_c*>::iterator it = m_connections.begin();
         it != m_connections.end(); ++it )
    {
      if( *it == &c )
        return;
    }
    m_connections.push_back( &c );
  }


  void ServerInstance_c::removeConnection( TcClientConnection_c& c ) {
    for( STL_NAMESPACE::list<TcClientConnection_c*>::iterator it = m_connections.begin();
         it != m_connections.end(); ++it )
    {
      if( *it == &c ) {
        m_connections.erase( it );
        break;
      }
    }
  }


  void ServerInstance_c::processStatus( uint8_t status ) {

    m_lastTcState = status;
    m_lastTcStateReceivedTime = HAL::getTime();

    const bool activeTask = status & 0x01;

    if ( activeTask != m_lastActiveTaskTC ) {
      if ( activeTask ) {
        for( STL_NAMESPACE::list<TcClientConnection_c*>::const_iterator it = m_connections.begin();
             it != m_connections.end(); ++it )
        {
          ( *it )->eventTaskStarted(); 
        }
      } else {
        for( STL_NAMESPACE::list<TcClientConnection_c*>::const_iterator it = m_connections.begin();
             it != m_connections.end(); ++it )
        {
          ( *it )->stopRunningMeasurement();
          ( *it )->eventTaskStopped();
        }
      }
    }

    m_lastActiveTaskTC = activeTask;
  }

}
