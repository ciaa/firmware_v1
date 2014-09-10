/*
  tcclient_c.h: central managing instance for all Task Controller-Clients

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TCCLIENT_C_H
#define TCCLIENT_C_H

#include "tcclientconnection_c.h"

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/util_funcs.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/driver/can/impl/cancustomer_c.h>

#include <list>
#include <map>

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif


namespace __IsoAgLib {
  class ProcData_c;

  class TcClient_c : public Subsystem_c {
      MACRO_MULTITON_CONTRIBUTION();
    public:
      virtual ~TcClient_c() {}

      class ServerStateHandler_c {
        public:
          virtual void _eventServerAvailable( const IsoItem_c&, IsoAgLib::ProcData::RemoteType_t, bool server_status ) = 0;
          virtual void _eventDisconnectedOnServerLoss( const TcClientConnection_c& client_disconnected) = 0;
      };

      class PdMessageHandler_c {
        public:
          virtual void _eventPdMessageReceived( const IsoItem_c&, const IsoItem_c*, IsoAgLib::ProcData::CommandType_t, uint16_t, uint16_t, int32_t) = 0;
      };

      void init();
      void close();

      void setServerStateHandler( ServerStateHandler_c& hdl );
      void clearServerStateHandler();

      void setPdMessageHandler( PdMessageHandler_c& hdl );
      void clearPdMessageHandler();

      TcClientConnection_c* connect( const IdentItem_c&, TcClientConnection_c::StateHandler_c&, const IsoItem_c& tcdl, DevicePool_c& );
      void disconnect( const IdentItem_c& );
      void disconnect( const TcClientConnection_c& );

      void getAllServers( IsoAgLib::ProcData::ServerList& list_to_fill );

      void processChangeDesignator( const IdentItem_c&, uint16_t, const char* );

      void sendPdMessage( const IsoItem_c& sa_item, 
                          const IsoItem_c* da_item,
                          IsoAgLib::ProcData::CommandType_t command, uint16_t element, uint16_t ddi, int32_t value );

#ifdef HAL_USE_SPECIFIC_FILTERS
      void receivePdMessage(const IsoItem_c& sa_item, const IsoItem_c* da_item, IsoAgLib::ProcData::CommandType_t command, uint16_t element, uint16_t ddi, int32_t value);
#endif

      void notifyServerStatusChange(ServerInstance_c& server, bool new_status);

    private:
      TcClient_c();

      void processMsg( const CanPkg_c& );
      void reactOnIsoItemModification ( ControlFunctionStateHandler_c::iIsoItemAction_e, IsoItem_c const& );

      void processMsgGlobal( const ProcessPkg_c& );
      void processMsgNonGlobal( const ProcessPkg_c& );
      void processTcStatusMsg( uint8_t ui8_tcStatus, const __IsoAgLib::IsoItem_c& sender );

      /// PROXY-CLASSES
      class CanCustomerProxy_c : public CanCustomer_c {
        public:
          typedef TcClient_c Owner_t;

          CanCustomerProxy_c( Owner_t &owner ) : m_owner( owner ) {}
          virtual ~CanCustomerProxy_c() {}

        private:
          virtual void processMsg( const CanPkg_c& data ) {
            return m_owner.processMsg( data );
          }

          Owner_t &m_owner;

        private:
          // not copyable
          CanCustomerProxy_c( CanCustomerProxy_c const & );
          CanCustomerProxy_c &operator=( CanCustomerProxy_c const & );
      };
      typedef CanCustomerProxy_c Customer_t;

      class ControlFunctionStateHandlerProxy_c : public ControlFunctionStateHandler_c {
        public:
          typedef TcClient_c Owner_t;

          ControlFunctionStateHandlerProxy_c( Owner_t &owner ) : m_owner( owner ) {}
          virtual ~ControlFunctionStateHandlerProxy_c() {}

        private:
          virtual void reactOnIsoItemModification( iIsoItemAction_e action, IsoItem_c const &isoItem ) {
            m_owner.reactOnIsoItemModification( action, isoItem );
          }

          Owner_t &m_owner;

        private:
          // not copyable
          ControlFunctionStateHandlerProxy_c( ControlFunctionStateHandlerProxy_c const & );
          ControlFunctionStateHandlerProxy_c &operator=( ControlFunctionStateHandlerProxy_c const & );
      };
      typedef ControlFunctionStateHandlerProxy_c Handler_t;

    private:
      Handler_t m_handler;
      Customer_t m_customer;
      ServerStateHandler_c* m_stateHandler;
      PdMessageHandler_c* m_pdMessageHandler;

      STL_NAMESPACE::map<const __IsoAgLib::IsoItem_c*,__IsoAgLib::ServerInstance_c*> m_server;
      STL_NAMESPACE::list<TcClientConnection_c*> m_connections;

      friend TcClient_c &getTcClientInstance( uint8_t instance );
      friend class ProcData_c;
  };

  TcClient_c &getTcClientInstance( uint8_t instance = 0 );
}

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#endif
