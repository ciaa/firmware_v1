/*
  fsmanager_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
// own
#include "fsmanager_c.h"

// ISOAgLib
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#include <IsoAgLib/driver/can/impl/canio_c.h>
#include <IsoAgLib/util/iassert.h>
#include <IsoAgLib/comm/impl/isobus_c.h>

// STL
#include <iterator>
#include <algorithm>

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif

// Begin Namespace __IsoAgLib
namespace __IsoAgLib {

/**
  * performe periodic tasks:
  * delete inactive fileservers and notify clientservercoms that have not been notified on fileservers yet.
  */
void
FsManager_c::timeEvent(void)
{
  STL_NAMESPACE::vector<FsServerInstance_c *>::iterator it_end = m_servers.m_serverInstances.end();
  for (STL_NAMESPACE::vector<FsServerInstance_c *>::iterator it_serverInstance = m_servers.m_serverInstances.begin();
       it_serverInstance != it_end;
       ++it_serverInstance)
  {
    (*it_serverInstance)->timeEvent();
  }

  for (STL_NAMESPACE::list<FsCommand_c *>::iterator it_command = m_commands.ml_initializingCommands.begin();
       it_command != m_commands.ml_initializingCommands.end(); )
  {
    isoaglib_assert (*it_command);
    switch ((*it_command)->getFileserver().getState())
    {
      case FsServerInstance_c::offline:
        // @todo in case of going offline delete command?
      case FsServerInstance_c::online:
        // In these cases wait until initialization is complete
        ++it_command;
        break;

      case FsServerInstance_c::usable:
      case FsServerInstance_c::unusable:
        // Finished the initialization one way or another
        delete (*it_command);
        it_command = m_commands.ml_initializingCommands.erase(it_command);
        break;
    }
  }
}


void
FsManager_c::reactOnIsoItemModification (ControlFunctionStateHandler_c::iIsoItemAction_e at_action, IsoItem_c const& acrc_isoItem)
{
  // we only care for fileservers
  if (acrc_isoItem.isoName().getEcuType() != IsoName_c::ecuTypeFileServerOrPrinter)
    return;

  if (at_action == ControlFunctionStateHandler_c::AddToMonitorList)
  {
    FsServerInstance_c *pc_fsInstance = new FsServerInstance_c (acrc_isoItem, *this);
    m_servers.m_serverInstances.push_back (pc_fsInstance);
  }
  else if (at_action == ControlFunctionStateHandler_c::RemoveFromMonitorList)
  {
    for (STL_NAMESPACE::vector<FsServerInstance_c *>::iterator it_serverInstance = m_servers.m_serverInstances.begin();
         it_serverInstance != m_servers.m_serverInstances.end();
         ++it_serverInstance)
    {
      // check if FsServerInstance to delete is in list
      if (acrc_isoItem.isoName() == (*it_serverInstance)->getIsoItem().isoName())
      { // There should only be one instance for an IsoName!
        // In all cases simply remove associated entries from l_initializingCommands
        for (STL_NAMESPACE::list<FsCommand_c *>::iterator it_command = m_commands.ml_initializingCommands.begin();
             it_command != m_commands.ml_initializingCommands.end(); )
        {
          if (&(*it_command)->getFileserver() == (*it_serverInstance))
          { // yep, command is connected to this FileServer.
            delete (*it_command);
            it_command = m_commands.ml_initializingCommands.erase(it_command);
            // normally we could break the for loop here, too.
          }
          else
          { // search next command
            ++it_command;
          }
        }

        // Set the FS to offline before removing,
        // so that any necessary cleanup will be done correctly
        (*it_serverInstance)->setState (FsServerInstance_c::offline);
        // now delete the instance
        delete (*it_serverInstance);
        // and remove it from the list of instances.
        (void) m_servers.m_serverInstances.erase (it_serverInstance);
        break;
      }
    }
  }
}


void
FsManager_c::init()
{
  isoaglib_assert (!initialized());

  m_commands.init();

  getSchedulerInstance().registerTask( *this, 0 );
  getIsoMonitorInstance4Comm().registerControlFunctionStateHandler(mc_saClaimHandler);

  setInitialized();
}


FsManager_c::FsManager_c()
  : SchedulerTask_c( 100, true )
  , mc_saClaimHandler(*this)
  , m_servers( *this )
  , m_commands( *this )
  , mv_communications()
{
}



FsClientServerCommunication_c *
FsManager_c::registerFsClient(IdentItem_c &rc_identItem, IsoAgLib::iFsClient_c &rc_Client, const IsoAgLib::iFsWhitelistList &v_fsWhitelist)
{
  FsClientServerCommunication_c *c_fscscClient = NULL;

#ifndef NDEBUG
  for (STL_NAMESPACE::vector<FsClientServerCommunication_c *>::iterator it_communications = mv_communications.begin();
       it_communications != mv_communications.end(); ++it_communications)
  {
    if (&(*it_communications)->getClientIdentItem() == &rc_identItem)
      isoaglib_assert( !"Double register of FS-Client detected!" );
  }
#endif

  c_fscscClient = new FsClientServerCommunication_c(rc_identItem, rc_Client, v_fsWhitelist);

  mv_communications.push_back(c_fscscClient);

  return c_fscscClient;
}

void 
FsManager_c::deregisterFsClient(IdentItem_c &identItem)
{
  for (STL_NAMESPACE::vector<FsClientServerCommunication_c *>::iterator iter = mv_communications.begin();
       iter != mv_communications.end(); ++iter)
  {
    if (&((*iter)->getClientIdentItem()) == &identItem)
    {
      delete *iter;
      mv_communications.erase(iter);
      return;
    }
  }

  isoaglib_assert( !"deregister of FS-Client for unregistered instance found!" );
}


// helper struct for the ::close function
struct delete_object
{
  template <typename T>
  void operator()(T *ptr){ delete ptr; }
};


void
FsManager_c::close()
{
  isoaglib_assert (initialized());

  m_commands.close();

  getIsoMonitorInstance4Comm().deregisterControlFunctionStateHandler (mc_saClaimHandler);
  getSchedulerInstance().deregisterTask(*this);

  STL_NAMESPACE::for_each( m_commands.ml_initializingCommands.begin(), m_commands.ml_initializingCommands.end(), delete_object());
  STL_NAMESPACE::for_each( mv_communications.begin(), mv_communications.end(), delete_object());
  STL_NAMESPACE::for_each( m_servers.m_serverInstances.begin(), m_servers.m_serverInstances.end(), delete_object());

  setClosed();
}


void
FsManager_c::notifyOnFileserverStateChange(FsServerInstance_c &fileserver, FsServerInstance_c::FsState_en oldState)
{
  switch (fileserver.getState())
  {
    case FsServerInstance_c::offline:
      if (oldState != FsServerInstance_c::usable)
        // don't care about offline-dropping if the FS wasn't
        // announced as usable before at all..
        break;

      for (STL_NAMESPACE::vector<FsClientServerCommunication_c *>::iterator iter = mv_communications.begin();
          iter != mv_communications.end();
          ++iter)
      {
        (*iter)->notifyOnOfflineFileServer (fileserver);
      }
      break;

    case FsServerInstance_c::online:
      if ( !mv_communications.empty() )
      {
        FsCommand_c *pc_command = new FsCommand_c(*mv_communications.front(), fileserver);
        m_commands.ml_initializingCommands.push_back(pc_command);
      }
      break;

    case FsServerInstance_c::usable:
      for (STL_NAMESPACE::vector<FsClientServerCommunication_c *>::iterator iter = mv_communications.begin();
           iter != mv_communications.end();
           ++iter)
      {
        (*iter)->notifyOnUsableFileServer (fileserver);
      }
      // now possibly some FsCSC(s) have connected itself to this FS
      break;

    case FsServerInstance_c::unusable:
      // no action currently
      break;
  }
}


void FsManager_c::FsServerManager_c::processFsToGlobal( const CanPkgExt_c& pkg ) {
  for (STL_NAMESPACE::vector<FsServerInstance_c *>::iterator it = m_serverInstances.begin(); it != m_serverInstances.end(); ++it) {
    if( pkg.getMonitorItemForSA() == &(*it)->getIsoItem() ) {
      (*it)->processFsToGlobal( pkg ); 
    }
  }
}


void FsManager_c::FsCommandManager_c::init() {
  getIsoBusInstance( m_fsManager.getMultitonInst() ).insertFilter( *this, IsoAgLib::iMaskFilterType_c( 0x3FF0000, FS_TO_CLIENT_PGN<<8, Ident_c::ExtendedIdent ), 8 ); 
}


void FsManager_c::FsCommandManager_c::close() {
  getIsoBusInstance( m_fsManager.getMultitonInst() ).deleteFilter( *this, IsoAgLib::iMaskFilterType_c( 0x3FF0000, FS_TO_CLIENT_PGN<<8, Ident_c::ExtendedIdent ) );
}


void FsManager_c::FsCommandManager_c::processMsg( const CanPkg_c& data ) {
  CanPkgExt_c pkg( data, m_fsManager.getMultitonInst() );
  if( ! pkg.isValid() || ( pkg.getMonitorItemForSA() == NULL ) )
    return;


  if( pkg.getMonitorItemForDA() == NULL )
  { // global
    m_fsManager.m_servers.processFsToGlobal( pkg );
  }
  else
  { // destination-specific
    for( STL_NAMESPACE::list<FsCommand_c*>::iterator it = ml_initializingCommands.begin(); it != ml_initializingCommands.end(); ++it )
    {
      (*it)->processMsgIso( pkg );
    }
  
    for (STL_NAMESPACE::vector<FsClientServerCommunication_c *>::iterator iter = m_fsManager.mv_communications.begin();
             iter != m_fsManager.mv_communications.end(); ++iter)
    {
      (*iter)->processMsgIso( pkg );
    }
  }
}

/** C-style function, to get access to the unique FsManager_c singleton instance
 * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
 */
FsManager_c &getFsManagerInstance(uint8_t instance)
{ // if > 1 singleton instance is used, no static reference can be used
  MACRO_MULTITON_GET_INSTANCE_BODY(FsManager_c, PRT_INSTANCE_CNT, instance);
}

//End namespace
}
