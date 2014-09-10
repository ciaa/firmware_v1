/*
  measurement_c.h

  (C) Copyright 2013 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef __MEASUREMENT_C_H
#define __MEASUREMENT_C_H

#include <IsoAgLib/comm/Part10_TaskController_Client/iprocdata.h>

#include <list>


namespace __IsoAgLib {

  class ProcData_c;
  class MeasureProg_c;
  class TcClientConnection_c;

  class Measurement_c {
    public:
      Measurement_c();

      int32_t getValue() const {
        return m_value;
      }

      void setMeasurementValue( int32_t v );
      void startMeasurement( TcClientConnection_c& ecu, IsoAgLib::ProcData::MeasurementCommand_t type, int32_t inc );

      void addMeasureProgRef( MeasureProg_c& m );
      void removeMeasureProgRef( MeasureProg_c& m );

    private:
      int32_t m_value;
      STL_NAMESPACE::list<MeasureProg_c*> m_measureProgs; // only cached, do not 'own' the instances
  };
}

#endif
