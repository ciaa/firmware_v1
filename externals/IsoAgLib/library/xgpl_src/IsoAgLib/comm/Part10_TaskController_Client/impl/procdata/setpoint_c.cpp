/*
  setpoint_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "setpoint_c.h"
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/devicepool_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/tcclientconnection_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/procdata/procdata_c.h>


namespace __IsoAgLib {

  Setpoint_c::Setpoint_c() :  mi32_value( 0 ), m_setpointhandler( NULL ) {
  }


  void Setpoint_c::init(SetpointHandler_c* setpointhandler) {
    m_setpointhandler = setpointhandler;
  }


  void Setpoint_c::processMsg( ProcData_c& pd, TcClientConnection_c& ecu, int32_t pdValue ) {
    if ( !pd.getDpd()->propertySetpoint() ) {
      ecu.sendNack( pd.DDI(), pd.element(), IsoAgLib::ProcData::NackProcessDataNotSetable );
      return;
    }

    const bool b_change = ( mi32_value != pdValue );
    mi32_value = pdValue;

    // call handler function if handler class is registered
    if ( m_setpointhandler ) {
      m_setpointhandler->_processSetpointSet( pd, pdValue, b_change );
    }
  }

}
