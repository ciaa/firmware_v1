/*
  fsmanager_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef FSMANAGER_C_H
#define FSMANAGER_C_H

// ISOAgLib
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/iidentitem_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/cfstatehandler_c.h>
#include <IsoAgLib/scheduler/impl/schedulertask_c.h>

// own
#include "fsserverinstance_c.h"
#include "fsclientservercommunication_c.h"
#include "fscommand_c.h"
#include "../ifsclient_c.h"

// STL (include at end due to Tasking-compiler!
#include <vector>
#include <list>


// Begin Namespace __IsoAgLib
namespace __IsoAgLib {

/** central IsoAgLib terminal management object */
class FsManager_c : public SchedulerTask_c
{
  MACRO_MULTITON_CONTRIBUTION();
  private:
    FsManager_c();

  public:

  class SaClaimHandlerProxy_c : public ControlFunctionStateHandler_c {
    public:
      typedef FsManager_c Owner_t;

      SaClaimHandlerProxy_c(Owner_t &owner) : mrt_owner(owner) {}

      virtual ~SaClaimHandlerProxy_c() {}

    private:
      virtual void reactOnIsoItemModification(iIsoItemAction_e action, IsoItem_c const &isoItem)
      {
        mrt_owner.reactOnIsoItemModification(action, isoItem);
      }

      // SaClaimHandlerProxy_c shall not be copyable. Otherwise the
      // reference to the containing object would become invalid.
      SaClaimHandlerProxy_c(SaClaimHandlerProxy_c const &);

      SaClaimHandlerProxy_c &operator=(SaClaimHandlerProxy_c const &);

      Owner_t &mrt_owner;
  };
  typedef SaClaimHandlerProxy_c Handler_t;


  class FsServerManager_c {
    public:
      FsServerManager_c( FsManager_c& fsManager ) : m_serverInstances(), m_fsManager( fsManager ) {}

      void processFsToGlobal( const CanPkgExt_c& pkg );

      STL_NAMESPACE::vector<FsServerInstance_c *> m_serverInstances;
      FsManager_c& m_fsManager;

    private:
      /** not copyable : copy constructor is only declared, never defined */
      FsServerManager_c(const FsServerManager_c&);
      /** not copyable : copy operator is only declared, never defined */
      FsServerManager_c& operator=(const FsServerManager_c&);
  };


  class FsCommandManager_c : public CanCustomer_c {
    public:
      FsCommandManager_c( FsManager_c& fsManager ) : ml_initializingCommands(), m_fsManager( fsManager ) {}
      void init();
      void close();

      virtual void processMsg( const CanPkg_c& data );

      STL_NAMESPACE::list<FsCommand_c*> ml_initializingCommands;

      FsManager_c& m_fsManager;

    private:
      /** not copyable : copy constructor is only declared, never defined */
      FsCommandManager_c(const FsCommandManager_c&);
      /** not copyable : copy operator is only declared, never defined */
      FsCommandManager_c& operator=(const FsCommandManager_c&);
  };


  void init();
  void close();

  virtual ~FsManager_c() {}

/**
  * performe periodic tasks:
  * delete inactive fileservers and notify clientservercoms that have not been notified on fileservers yet.
  * Once a fileserver's properties have been requested, the fscommand object, taking care of the request
  * is deleted.
  */
  void timeEvent(void);

  FsClientServerCommunication_c *registerFsClient(IdentItem_c &identItem, IsoAgLib::iFsClient_c &Client, const IsoAgLib::iFsWhitelistList &v_fsWhitelist);
  void deregisterFsClient(IdentItem_c &identItem);
  
  virtual void reactOnIsoItemModification (ControlFunctionStateHandler_c::iIsoItemAction_e /*action*/, IsoItem_c const& /*isoItem*/);

  public:
    /**
      * A FsServerInstance_c will notify the Manager
      * that it has its state changed.
      */
    void notifyOnFileserverStateChange(FsServerInstance_c &fileserver, FsServerInstance_c::FsState_en en_oldState);

  private:
    Handler_t mc_saClaimHandler;
    FsServerManager_c m_servers;
    FsCommandManager_c m_commands;

    STL_NAMESPACE::vector<FsClientServerCommunication_c *> mv_communications;

    friend FsManager_c &getFsManagerInstance(uint8_t instance);
};

/** C-style function, to get access to the unique FsManager_c singleton instance
 * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
 */
FsManager_c &getFsManagerInstance(uint8_t instance = 0);

//End namespace
}


#endif
