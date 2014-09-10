/*
  itcclient_c.h: central managing instance for all process data

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ITCCLIENT_C_H
#define ITCCLIENT_C_H

#include "impl/tcclient_c.h"
#include "iprocdata.h"
#include "idevicepool_c.h"
#include <IsoAgLib/comm/Part5_NetworkManagement/iidentitem_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/itcclientconnection_c.h>


namespace IsoAgLib {

  class iTcClient_c : private __IsoAgLib::TcClient_c {
    public:
      class iServerStateHandler_c : private __IsoAgLib::TcClient_c::ServerStateHandler_c {
        public:
          // server_status is true: server is now available/online
          // server_status is false: server is now unavailable/offline
          virtual void eventServerAvailable( const iIsoItem_c&, ProcData::RemoteType_t, bool server_status ) = 0;
          // a iTcClientConnection_c is about to get closed/destructed: IsoAgLib will delete
          // the iTcClientConnection_c after this call. It happens on server alive timeout and if server IsoItem
          // is removed from the monitor list
          // Warning: do not "disconnect" any client during this call !
          virtual void eventDisconnectedOnServerLoss( const iTcClientConnection_c& client_disconnected ) = 0;
      private:
          virtual void _eventServerAvailable( const __IsoAgLib::IsoItem_c& item, IsoAgLib::ProcData::RemoteType_t type, bool server_status ) {
            eventServerAvailable( (const iIsoItem_c&) item , type, server_status );
          }
          virtual void _eventDisconnectedOnServerLoss( const __IsoAgLib::TcClientConnection_c& client_disconnected ) {
            eventDisconnectedOnServerLoss( (const iTcClientConnection_c&)client_disconnected );
          }
          friend class iTcClient_c;
      };
      // Warning: only use if you know what you are doing
      class iPdMessageHandler_c : private __IsoAgLib::TcClient_c::PdMessageHandler_c {
        public:
          virtual void eventPdMessageReceived(
              const iIsoItem_c& sa_item, 
              const iIsoItem_c* da_item,
              IsoAgLib::ProcData::CommandType_t command, uint16_t element, uint16_t ddi, int32_t value) = 0;

        private:
          virtual void _eventPdMessageReceived(
              const __IsoAgLib::IsoItem_c& sa_item, 
              const __IsoAgLib::IsoItem_c* da_item,
              IsoAgLib::ProcData::CommandType_t command, uint16_t element, uint16_t ddi, int32_t value) {
            eventPdMessageReceived(
                (const iIsoItem_c&) sa_item ,
                (const iIsoItem_c*) da_item ,
                command, element, ddi, value );
          }
          friend class iTcClient_c;
      };

      void init() {
        TcClient_c::init();
      }

      void setServerStateHandler( iServerStateHandler_c& hdl ) {
        TcClient_c::setServerStateHandler( hdl );
      }

      void clearServerStateHandler() {
        TcClient_c::clearServerStateHandler();
      }

      // Warning: only use if you know what you are doing
      void setPdMessageHandler( iPdMessageHandler_c& hdl ) {
        TcClient_c::setPdMessageHandler( hdl );
      }
 
      void clearPdMessageHandler() {
        TcClient_c::clearPdMessageHandler();
      }

      void close() {
        TcClient_c::close();
      }

      iTcClientConnection_c* connect( iIdentItem_c& identItem, iTcClientConnection_c::iStateHandler_c& statehandler, const iIsoItem_c& server, iDevicePool_c& pool ) {
        return static_cast<iTcClientConnection_c*>( TcClient_c::connect(
            static_cast<__IsoAgLib::IdentItem_c&>( identItem ),
            static_cast<__IsoAgLib::TcClientConnection_c::StateHandler_c&>( statehandler ),
            static_cast<const __IsoAgLib::IsoItem_c&>( server ),
            static_cast<__IsoAgLib::DevicePool_c&>( pool ) ) );
      }

      void disconnect( iIdentItem_c& identItem ) {
        TcClient_c::disconnect( static_cast<__IsoAgLib::IdentItem_c&>( identItem ) );
      }

      void disconnect( iTcClientConnection_c& connection) {
        TcClient_c::disconnect( static_cast<__IsoAgLib::TcClientConnection_c&>( connection ) );
      }
      
      void getAllServers( IsoAgLib::ProcData::ServerList& list_to_fill ) {
        TcClient_c::getAllServers( list_to_fill );
      }

      /* changing designators is not yet supported */
      void processChangeDesignator( iIdentItem_c& identItem, uint16_t objID, const char* newDesignator ) {
        return TcClient_c::processChangeDesignator( static_cast<__IsoAgLib::IdentItem_c&>( identItem ), objID, newDesignator );
      }

      // Warning: only use if you know what you are doing
      void sendPdMessage( const iIsoItem_c& sa_item, 
                          const iIsoItem_c* da_item,
                          IsoAgLib::ProcData::CommandType_t command,
                          uint16_t element,
                          uint16_t ddi,
                          int32_t value ) {
        return TcClient_c::sendPdMessage(
            static_cast<const __IsoAgLib::IsoItem_c&>( sa_item ),
            static_cast<const __IsoAgLib::IsoItem_c*>( da_item ),
            command, element, ddi, value );
      }

    private:
      friend iTcClient_c& getItcClientInstance( uint8_t instance );
  };

  inline iTcClient_c& getItcClientInstance( uint8_t instance = 0 ) {
    return static_cast<iTcClient_c&>( __IsoAgLib::getTcClientInstance( instance ) );
  }

}

#endif
