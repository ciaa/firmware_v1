/*
  isbclient_c.h: central ISB client management
                 (Stop all implement operations)

  (C) Copyright 2013 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISBCLIENT_C_H
#define ISBCLIENT_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/driver/can/impl/cancustomer_c.h>
#include <IsoAgLib/driver/system/impl/system_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/cfstatehandler_c.h>
#include <IsoAgLib/scheduler/impl/schedulertask_c.h>
#include <map>
#include <list>


namespace IsoAgLib { class iIsbStateHandler_c; }


namespace __IsoAgLib {

class IdentItem_c;


class IsbClient_c : private SchedulerTask_c
{
  MACRO_MULTITON_CONTRIBUTION();
  IsbClient_c(); // only created via Multiton, never application

public:
  virtual ~IsbClient_c() {}

  void init();
  void close();

  void registerStateHandler( IdentItem_c&, IsoAgLib::iIsbStateHandler_c& );
  void deregisterStateHandler( IdentItem_c&, IsoAgLib::iIsbStateHandler_c& );

private:
  virtual void timeEvent();
  void processMsg( const CanPkg_c& arc_data );
  void reactOnIsoItemModification( ControlFunctionStateHandler_c::iIsoItemAction_e, IsoItem_c const& );

  enum SwitchState_e {
    StopImplementOperations = 0x0,
    PermitAllImplementsToOperationON = 0x1,
    ErrorIndication = 0x2,
    NotAvailable = 0x3
  };

  static const int32_t sc_serverTimeOut = 3000;
  struct IsbState_s
  {
    IsbState_s( int32_t timeReceived, uint8_t transitionNr, SwitchState_e state )
      : m_timeReceived( timeReceived ), m_transitionNr( transitionNr ), m_state( state ) {}

    // -1 is for enforced timeOut (on Node drop-off)
    bool timedOut() const { return( ( m_timeReceived < 0 ) || ( ( HAL::getTime() - m_timeReceived ) >= sc_serverTimeOut ) ); }

    int32_t m_timeReceived;
    uint8_t m_transitionNr;
    SwitchState_e m_state;
  };

  void handleChangedState();

  class CanCustomerProxy_c : public CanCustomer_c {
  public:
    typedef IsbClient_c Owner_t;
    CanCustomerProxy_c( Owner_t &owner ) : m_owner( owner ) {}
    virtual ~CanCustomerProxy_c() {}

  private:
    virtual void processMsg( const CanPkg_c& canPkg ) { m_owner.processMsg( canPkg ); }

    CanCustomerProxy_c(CanCustomerProxy_c const &); // not copyable, ref would be wrong!
    CanCustomerProxy_c &operator=(CanCustomerProxy_c const &);

    Owner_t &m_owner;
  };
  typedef CanCustomerProxy_c Customer_t;

  class ControlFunctionStateHandlerProxy_c : public ControlFunctionStateHandler_c {
  public:
    typedef IsbClient_c Owner_t;
    ControlFunctionStateHandlerProxy_c( Owner_t &owner ) : m_owner( owner ) {}
    virtual ~ControlFunctionStateHandlerProxy_c() {}

  private:
    virtual void reactOnIsoItemModification( iIsoItemAction_e action, IsoItem_c const &item )
    { m_owner.reactOnIsoItemModification( action, item ); }

    ControlFunctionStateHandlerProxy_c(ControlFunctionStateHandlerProxy_c const &); // not copyable
    ControlFunctionStateHandlerProxy_c &operator=(ControlFunctionStateHandlerProxy_c const &);

    Owner_t &m_owner;
  };
  typedef ControlFunctionStateHandlerProxy_c Handler_t;

private:
  Subsystem_c m_runState;

  typedef STL_NAMESPACE::map< IsoItem_c*, IsbState_s > ServerMap_t;
  ServerMap_t m_servers;

  typedef STL_NAMESPACE::list< IsoAgLib::iIsbStateHandler_c* > CallbackList_t;
  CallbackList_t m_callbacks;

  bool m_lastCallbackStopAllImplementOperations;
  unsigned m_lastCallbackServerCnt;

  Handler_t m_handler;
  Customer_t m_customer;

  friend IsbClient_c &getIsbClientInstance( unsigned instance );
};


/** C-style function, to get access to the unique multiton instance */
IsbClient_c &getIsbClientInstance( unsigned instance = 0 );

}

#endif
