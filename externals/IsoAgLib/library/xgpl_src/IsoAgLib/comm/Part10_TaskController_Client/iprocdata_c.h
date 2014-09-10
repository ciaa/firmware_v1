/*
  iprocdata_c.h: managing of local process data

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IPROCDATA_C_H
#define IPROCDATA_C_H

#include "impl/procdata/procdata_c.h"
#include <IsoAgLib/comm/Part10_TaskController_Client/iprocdata.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/idevicepool_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/itcclientconnection_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/iisoname_c.h>

namespace __IsoAgLib {
  class Setpoint_c;
}

namespace IsoAgLib {

  class iProcData_c : private __IsoAgLib::ProcData_c {
    public:

      iProcData_c() : ProcData_c() {}

      class iSetpointHandler_c : private __IsoAgLib::SetpointHandler_c {
        public:
          virtual ~iSetpointHandler_c() {}
          virtual void processSetpointSet( iProcData_c& procdata, int32_t value, bool change ) = 0;

        private:
          virtual void _processSetpointSet( __IsoAgLib::ProcData_c& procdata, int32_t value, bool change ) {
            processSetpointSet( static_cast<iProcData_c&>( procdata ), value, change );
          }
          friend class IsoAgLib::iProcData_c;
      };

      void init( iIdentItem_c& ident, const iDeviceObjectDpd_c& dpd, const iDeviceObjectDet_c& det, iSetpointHandler_c* setpointhandler = NULL ) {
        ProcData_c::init( ident, dpd, det, static_cast<__IsoAgLib::SetpointHandler_c*>( setpointhandler ) );
      }

      const iIsoName_c& isoName() const {
        return ProcData_c::isoName().toConstIisoName_c();
      }

      uint16_t DDI() const {
        return ProcData_c::DDI();
      }
      uint16_t element() const  {
        return ProcData_c::element();
      }
      bool isMethodSet( ProcData::TriggerMethod_t method ) const {
        return ProcData_c::isMethodSet( method );
      }

      int32_t measurementValue() const {
        return ProcData_c::getMeasurement().getValue();
      }

      /* sets and send value (according to running programs */
      void setMeasurementValue( int32_t ai32_val ) {
        ProcData_c::getMeasurement().setMeasurementValue( ai32_val );
      }

      int32_t setpointValue() const {
        return ProcData_c::getSetpoint().setpointValue();
      }

      void startMeasurement( IsoAgLib::iTcClientConnection_c& ecu, ProcData::MeasurementCommand_t ren_type, int32_t ai32_increment ) {
        ProcData_c::getMeasurement().startMeasurement( static_cast<__IsoAgLib::TcClientConnection_c&>( ecu ), ren_type, ai32_increment );
      }

      friend class iDevicePool_c;
  };

}

#endif
