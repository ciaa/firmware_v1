/*
  measureprog_c.cpp: object for managing local measure programs

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "measureprog_c.h"
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/tcclient_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/procdata/procdata_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/procdata/measuresubprog_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/tcclientconnection_c.h>

#if defined(USE_BASE) || defined(USE_TRACTOR_MOVE)
#include <IsoAgLib/comm/Part7_ApplicationLayer/impl/tracmove_c.h>
#endif


namespace __IsoAgLib {

  static const int32_t sci32_stopValTimeInterval = 0;
  static const int32_t sci32_stopValDistanceInterval = 0;
  static const int32_t sci32_stopValOnChange = 0;
  static const int32_t sci32_stopValThresholdMaximum = -2147483647L; // Standard specifies (-2^31+1) instead of (-2^31)
  static const int32_t sci32_stopValThresholdMinimum = 2147483647L;

  MeasureProg_c::MeasureProg_c( TcClientConnection_c& connection, ProcData_c& procdata )
    : m_subProgOnChange( NULL )
    , m_subProgTimeProp( NULL )
    , m_subProgDistProp( NULL )
    , m_value( 0 )
    , m_minThreshold( sci32_stopValThresholdMinimum )
    , m_maxThreshold( sci32_stopValThresholdMaximum )
    , m_connection( connection )
    , m_procdata( procdata ) {
    m_procdata.getMeasurement().addMeasureProgRef( *this );
  }


  MeasureProg_c::~MeasureProg_c() {
    m_procdata.getMeasurement().removeMeasureProgRef( *this );
  }


  void MeasureProg_c::processMeasurementMsg( IsoAgLib::ProcData::MeasurementCommand_t command, int32_t pdValue ) {
    if( !startMeasurement( IsoAgLib::ProcData::MeasurementCommand_t( command ), pdValue ) ) {
      m_connection.sendNack( m_procdata.DDI(), m_procdata.element(), IsoAgLib::ProcData::NackTriggerMethodNotSupported );
    }
  }


  void MeasureProg_c::processRequestMsg() {
    m_connection.sendProcMsg( m_procdata.DDI(), m_procdata.element(), m_value );
  }


  void MeasureProg_c::processSetMsg( int32_t pdValue ) {
    m_procdata.getSetpoint().processMsg( m_procdata, m_connection, pdValue );
  }


  void MeasureProg_c::setValue( int32_t value ) {
    m_value = value;

    if( m_subProgOnChange )
        m_subProgOnChange->setValue( value );
  }


  bool MeasureProg_c::minMaxLimitsPassed() const {
    const bool checkMin = m_minThreshold != sci32_stopValThresholdMinimum;
    const bool checkMax = m_maxThreshold != sci32_stopValThresholdMaximum;

    if( checkMin )
    {
      if( checkMax )
      { // both given
        if ( m_maxThreshold < m_minThreshold )
        { // outside range
          return( ( m_value <= m_maxThreshold ) || ( m_value >= m_minThreshold ) );
        }
        else
        { // inside range
          return( ( m_value >= m_minThreshold ) && ( m_value <= m_maxThreshold ) );
        }
      }
      else
      { // only min
        return( m_value >= m_minThreshold );
      }
    }
    else
    {
      if( checkMax )
      { // only max
        return( m_value <= m_maxThreshold );
      }
      else
      { // no min, no max
        return true;
      }
    }
  }


  bool MeasureProg_c::startMeasurement( IsoAgLib::ProcData::MeasurementCommand_t ren_type, int32_t ai32_increment ) {
    bool b_validTriggerMethod = false;

    switch ( ren_type ) {
      case IsoAgLib::ProcData::MeasurementCommandTimeProp: // time proportional
        if ( m_procdata.isMethodSet( IsoAgLib::ProcData::TimeInterval ) ) {
          if ( ai32_increment == sci32_stopValTimeInterval ) {
            delete m_subProgTimeProp;
            m_subProgTimeProp = NULL;
          } else {
            if( m_subProgTimeProp == NULL )
              m_subProgTimeProp = new MeasureTimeProp_c( *this );
            // 0, 100..60000 allowed per standard
            if( ai32_increment < 100 )
              ai32_increment = 100;
            m_subProgTimeProp->start( System_c::getTime(), ai32_increment );
          }          
          b_validTriggerMethod = true;
        }
        break;

      case IsoAgLib::ProcData::MeasurementCommandDistProp: // distance proportional
#if defined(USE_BASE) || defined(USE_TRACTOR_MOVE) // if no distance available, NACK will be sent
        if ( m_procdata.isMethodSet( IsoAgLib::ProcData::DistInterval ) ) {
          if ( ai32_increment == sci32_stopValDistanceInterval ) {
            delete m_subProgDistProp;
            m_subProgDistProp = NULL;
          } else {
            if( m_subProgDistProp == NULL )
              m_subProgDistProp = new MeasureDistProp_c( *this );
            if( ai32_increment < 0 )
              ai32_increment = -ai32_increment;
            m_subProgDistProp->start(int32_t(getTracMoveInstance(m_procdata.identItem().getMultitonInst()).distTheor()), ai32_increment);
          }       
          b_validTriggerMethod = true;
        }
#endif
        break;

      case IsoAgLib::ProcData::MeasurementCommandOnChange: // change threshold proportional
        if ( m_procdata.isMethodSet( IsoAgLib::ProcData::OnChange ) ) {
          if ( ai32_increment == sci32_stopValOnChange ) {
            delete m_subProgOnChange;
            m_subProgOnChange = NULL;
          } else {
            if( m_subProgOnChange == NULL )
              m_subProgOnChange = new MeasureOnChange_c( *this );
            if( ai32_increment < 0 )
              ai32_increment = -ai32_increment;
            m_subProgOnChange->start( m_value, ai32_increment );
          }      
          b_validTriggerMethod = true;
        }
        break;

      case IsoAgLib::ProcData::MeasurementCommandMaximumThreshold: // change threshold proportional
        if ( m_procdata.isMethodSet( IsoAgLib::ProcData::ThresholdLimit ) ) {
          m_maxThreshold = ai32_increment;
          b_validTriggerMethod = true;
          // TODO send process data value if necessary ?
        }
        break;

      case IsoAgLib::ProcData::MeasurementCommandMinimumThreshold: // change threshold proportional
        if ( m_procdata.isMethodSet( IsoAgLib::ProcData::ThresholdLimit ) ) {
          m_minThreshold = ai32_increment;
          b_validTriggerMethod = true;
          // TODO send process data value if necessary ?
        }
        break;
    }

    return b_validTriggerMethod;
  }


  void MeasureProg_c::stopAllMeasurements() {
    delete m_subProgDistProp;
    m_subProgDistProp = NULL;

    delete m_subProgOnChange;
    m_subProgOnChange = NULL;

    delete m_subProgTimeProp;
    m_subProgTimeProp = NULL;

    m_minThreshold = sci32_stopValThresholdMinimum;
    m_maxThreshold = sci32_stopValThresholdMaximum;
  }

}
