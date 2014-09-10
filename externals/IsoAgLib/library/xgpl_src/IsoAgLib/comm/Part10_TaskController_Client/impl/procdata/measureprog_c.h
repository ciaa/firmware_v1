/*
  measureprog_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef MEASUREPROG_C_H
#define MEASUREPROG_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/iprocdata.h>

#include <list>


namespace __IsoAgLib {

  class ProcData_c;
  class TcClientConnection_c;
  class MeasureOnChange_c;
  class MeasureTimeProp_c;
  class MeasureDistProp_c;

  class MeasureProg_c{
    private:
      /** not copyable */
      MeasureProg_c( const MeasureProg_c& );
      MeasureProg_c& operator=( const MeasureProg_c& );

    public:
      MeasureProg_c( TcClientConnection_c& c, ProcData_c& procdata);
      virtual ~MeasureProg_c();

      bool startMeasurement( IsoAgLib::ProcData::MeasurementCommand_t ren_type, int32_t ai32_increment );
      void stopAllMeasurements();

      void processMeasurementMsg( IsoAgLib::ProcData::MeasurementCommand_t command, int32_t pdValue );
      void processRequestMsg();
      void processSetMsg( int32_t pdValue );

      bool minMaxLimitsPassed() const;

      int32_t getValue() const { return m_value; }
      void setValue( int32_t ai32_val );

      const TcClientConnection_c& connection() const { return m_connection; }
      const ProcData_c &procData() const { return m_procdata; }

    private:
      MeasureOnChange_c *m_subProgOnChange;
      MeasureTimeProp_c *m_subProgTimeProp;
      MeasureDistProp_c *m_subProgDistProp;

      int32_t m_value;
      int32_t m_minThreshold;
      int32_t m_maxThreshold;

      TcClientConnection_c& m_connection;
      ProcData_c& m_procdata;
  };

}

#endif
