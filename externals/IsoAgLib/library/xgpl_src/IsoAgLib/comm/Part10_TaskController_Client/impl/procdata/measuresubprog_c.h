/*
  measuresubprog_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef MEASURESUBPROG_C_H
#define MEASURESUBPROG_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/iprocdata.h>
#include <IsoAgLib/scheduler/impl/schedulertask_c.h>


namespace __IsoAgLib {

class ProcData_c;
class MeasureProg_c;

class MeasureDistProp_c : public SchedulerTask_c 
{
private: // non-copyable
  MeasureDistProp_c( const MeasureDistProp_c& );

public:
  MeasureDistProp_c( MeasureProg_c& measureProg );
  virtual ~MeasureDistProp_c();

  void start( int32_t ai32_lastVal, int32_t ai32_increment );

private:
  bool updateTrigger(int32_t ai32_val);
  int32_t nextTriggerTime(const ProcData_c& ac_processData, int32_t ai32_val);

  virtual void timeEvent();

private:
  MeasureProg_c &m_measureProg;
  int32_t mi32_lastVal;
  int32_t mi32_increment;
};


class MeasureTimeProp_c : public SchedulerTask_c 
{
private: // non-copyable
  MeasureTimeProp_c( const MeasureTimeProp_c& );

public:
  MeasureTimeProp_c( MeasureProg_c& measureProg );
  virtual ~MeasureTimeProp_c();

  void start( int32_t ai32_lastVal, int32_t ai32_increment );

private:
  bool updateTrigger(int32_t ai32_val);
  int32_t nextTriggerTime(int32_t ai32_val);

  virtual void timeEvent();

private:
  MeasureProg_c &m_measureProg;
  int32_t mi32_lastVal;
  int32_t mi32_increment;
};


class MeasureOnChange_c
{
private: // non-copyable
  MeasureOnChange_c( const MeasureOnChange_c& );

public:
  MeasureOnChange_c( MeasureProg_c& measureProg );
  virtual ~MeasureOnChange_c() {}

  void start( int32_t ai32_lastVal, int32_t ai32_increment );

  void setValue( int32_t value );

private:
  bool updateTrigger(int32_t ai32_val);

private:
  MeasureProg_c &m_measureProg;
  int32_t mi32_lastVal;
  int32_t mi32_increment;
};

}

#endif
