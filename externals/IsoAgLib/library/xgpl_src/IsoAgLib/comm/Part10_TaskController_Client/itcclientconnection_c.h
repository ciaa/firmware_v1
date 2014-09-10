/*
  itcclientconnection_c.h: Class for managing a connection

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ITCCLIENTCONNECTION_C_H
#define ITCCLIENTCONNECTION_C_H

#include "impl/tcclientconnection_c.h"

namespace IsoAgLib {

  class iTcClient_c;
  class iProcData_c;

  class iTcClientConnection_c : private __IsoAgLib::TcClientConnection_c {
    public:
      class iStateHandler_c : private __IsoAgLib::TcClientConnection_c::StateHandler_c {
        public:
          virtual void eventDefaultLoggingStarted( iTcClientConnection_c& ecu ) = 0;
          virtual void eventTaskStarted( iTcClientConnection_c& ecu ) = 0;
          virtual void eventTaskStopped( iTcClientConnection_c& ecu ) = 0;

        private:
          virtual void _eventDefaultLoggingStarted( TcClientConnection_c& ecu ) {
            eventDefaultLoggingStarted( static_cast<iTcClientConnection_c&>( ecu ) );
          }
          virtual void _eventTaskStarted( TcClientConnection_c& ecu ) {
            eventTaskStarted( static_cast<iTcClientConnection_c&>( ecu ) );
          }
          virtual void _eventTaskStopped( TcClientConnection_c& ecu ) {
            eventTaskStopped( static_cast<iTcClientConnection_c&>( ecu ) );
          }
          friend class iTcClient_c;
      };

      /* TODO */
      bool sendCommandChangeDesignator( uint16_t objectID, const char* newString, uint8_t stringLength ) {
        return TcClientConnection_c::sendCommandChangeDesignator( objectID, newString, stringLength );
      }

    private:
      iTcClientConnection_c();

      friend class iTcClient_c;
      friend class iProcData_c;
  };

} // IsoAgLib

#endif
