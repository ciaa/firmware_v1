/*
  setpoint_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef SETPOINT_C_H
#define SETPOINT_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/iprocdata.h>


namespace __IsoAgLib {

  class TcClientConnection_c;
  class ProcData_c;
  class SetpointHandler_c;

  class Setpoint_c {
    public:
      Setpoint_c();
      ~Setpoint_c() {}

      void init(SetpointHandler_c* setpointhandler);

      int32_t setpointValue() const {
        return mi32_value;
      }

      void processMsg( ProcData_c& ac_processData, TcClientConnection_c& ecu, int32_t pdValue );

    private:
      int32_t mi32_value;

      SetpointHandler_c* m_setpointhandler;

    private:
      /** not copyable : copy constructor/operator only declared, not defined */
      Setpoint_c( const Setpoint_c& );
      Setpoint_c& operator=( const Setpoint_c& );
  };

}

#endif
