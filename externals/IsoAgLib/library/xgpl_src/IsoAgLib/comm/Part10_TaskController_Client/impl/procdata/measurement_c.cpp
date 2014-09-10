/*
  measurement_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "measurement_c.h"
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/procdata/measureprog_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/tcclientconnection_c.h>


namespace __IsoAgLib {

  Measurement_c::Measurement_c() : m_value( 0 ), m_measureProgs() {}


  void Measurement_c::setMeasurementValue( int32_t v ) {
    m_value = v;
    for( STL_NAMESPACE::list<MeasureProg_c*>::iterator i = m_measureProgs.begin(); i != m_measureProgs.end(); ++i ) {
      ( *i )->setValue( v );
    }
  }


  void Measurement_c::startMeasurement( TcClientConnection_c& ecu, IsoAgLib::ProcData::MeasurementCommand_t type, int32_t inc ) {
    for( STL_NAMESPACE::list<MeasureProg_c*>::iterator i = m_measureProgs.begin(); i != m_measureProgs.end(); ++i ) {
      if ( &(*i)->connection() == &ecu ) {
        (*i)->startMeasurement( type, inc );
        break;
      }
    }
  }


  void Measurement_c::addMeasureProgRef( MeasureProg_c& m ) {
    m_measureProgs.push_front( &m );
    m_measureProgs.front()->setValue( m_value );
  }


  void Measurement_c::removeMeasureProgRef( MeasureProg_c& m ) {
    for( STL_NAMESPACE::list<MeasureProg_c*>::iterator i = m_measureProgs.begin(); i != m_measureProgs.end(); ++i ) {
      if ( (*i) == &m ) {
        m_measureProgs.erase(i);
        break;
      }
    } 
  }

}
