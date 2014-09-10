/*
  measuresubprog_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "measuresubprog_c.h"
#include <IsoAgLib/scheduler/impl/scheduler_c.h>
#include <IsoAgLib/util/impl/util_funcs.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/procdata/procdata_c.h>

#if defined(USE_BASE) || defined(USE_TRACTOR_MOVE)
  #include <IsoAgLib/comm/Part7_ApplicationLayer/impl/tracmove_c.h>
#endif


namespace __IsoAgLib {



/// ------------- MeasureDistProp_c -------------



MeasureDistProp_c::MeasureDistProp_c( MeasureProg_c& measureProg )
  : SchedulerTask_c( 50, false )
  , m_measureProg( measureProg )
  , mi32_lastVal( 0 )
  , mi32_increment( 0 )
{
  getSchedulerInstance().registerTask( *this, 0 );
}


MeasureDistProp_c::~MeasureDistProp_c()
{
  getSchedulerInstance().deregisterTask( *this );
}


void
MeasureDistProp_c::start( int32_t ai32_lastVal, int32_t ai32_increment )
{
  mi32_increment = ai32_increment;
  mi32_lastVal = ai32_lastVal;

  if( m_measureProg.minMaxLimitsPassed() )
    m_measureProg.connection().sendProcMsg( m_measureProg.procData().DDI(), m_measureProg.procData().element(), m_measureProg.getValue() );
}


bool
MeasureDistProp_c::updateTrigger( int32_t ai32_val )
{
  if (__IsoAgLib::abs(ai32_val - mi32_lastVal) >= mi32_increment)
  {
    mi32_lastVal = ai32_val;
    return true;
  }
  else
  {
    return false;
  }
}


int32_t
MeasureDistProp_c::nextTriggerTime( const ProcData_c& ac_processData, int32_t ai32_val )
{
#if defined(USE_BASE) || defined(USE_TRACTOR_MOVE)
  const int32_t ci32_restDistance = mi32_lastVal + mi32_increment - ai32_val;
  const int32_t ci32_speed = __IsoAgLib::abs(getTracMoveInstance(ac_processData.identItem().getMultitonInst()).selectedSpeed());  // speed can be negative

  if (0 == ci32_speed)
    // speed == 0
    return 500;

  if ( ! getTracMoveInstance(ac_processData.identItem().getMultitonInst()).isSelectedSpeedUsable() )
  { // invalid speed, no tractor available
    return 200;
  }

  if (ci32_restDistance < 0)
    // should not happen if distance does only grow
    return 100;

  int32_t i32_nextTriggerTime = (ci32_restDistance * 1000 ) / ci32_speed; // distance in mm, div speed in mm/sec, result in msec

  if (i32_nextTriggerTime > 500)
  {
    i32_nextTriggerTime = 500;
  }

  return i32_nextTriggerTime;  // distance (in mm) div speed (in mm/sec) => time in msec
#else
  (void)ac_processData;
  return 200; // 200 msec
#endif
}


void MeasureDistProp_c::timeEvent() {
#if defined(USE_BASE) || defined(USE_TRACTOR_MOVE)
    const int32_t distTheor = getTracMoveInstance(m_measureProg.procData().identItem().getMultitonInst()).distTheor();
    const bool sendProcMsg = updateTrigger( distTheor );
#else
    const int32_t distTheor = 0;
    const bool sendProcMsg = false;
#endif
    const int32_t nextTimePeriod = nextTriggerTime( m_measureProg.procData(), distTheor );

  if( nextTimePeriod > 0 )
    setPeriod( nextTimePeriod, false );
  else
    setPeriod( 10, false ); // fallback, TODO later...

  if( sendProcMsg && m_measureProg.minMaxLimitsPassed() )
    m_measureProg.connection().sendProcMsg( m_measureProg.procData().DDI(), m_measureProg.procData().element(), m_measureProg.getValue() );
}



/// ------------- MeasureTimeProp_c -------------



MeasureTimeProp_c::MeasureTimeProp_c( MeasureProg_c& measureProg )
  : SchedulerTask_c( 50, false )
  , m_measureProg( measureProg )
  , mi32_lastVal( 0 )
  , mi32_increment( 0 )
{
  getSchedulerInstance().registerTask( *this, 0 );
}


MeasureTimeProp_c::~MeasureTimeProp_c()
{
  getSchedulerInstance().deregisterTask( *this );
}


void
MeasureTimeProp_c::start( int32_t ai32_lastVal, int32_t ai32_increment )
{
  mi32_increment = ai32_increment;
  mi32_lastVal = ai32_lastVal;

  setPeriod( ai32_increment, true );

  if( m_measureProg.minMaxLimitsPassed() )
    m_measureProg.connection().sendProcMsg( m_measureProg.procData().DDI(), m_measureProg.procData().element(), m_measureProg.getValue() );
}


bool
MeasureTimeProp_c::updateTrigger( int32_t ai32_val )
{
  if (__IsoAgLib::abs(ai32_val - mi32_lastVal) >= mi32_increment)
  {
    mi32_lastVal = ai32_val;
    return true;
  }
  else
  {
    return false;
  }
}


int32_t
MeasureTimeProp_c::nextTriggerTime( int32_t ai32_val )
{
  return (mi32_lastVal + mi32_increment - ai32_val);
}


void MeasureTimeProp_c::timeEvent()
{
  const int32_t now = System_c::getTime();
  const bool sendProcMsg = updateTrigger( now );
  const int32_t nextTimePeriod = nextTriggerTime( now );

  if( nextTimePeriod > 0 )
    setPeriod( nextTimePeriod, false );
  else
    setPeriod( 10, false ); // fallback, TODO later...

  if( sendProcMsg && m_measureProg.minMaxLimitsPassed() )
    m_measureProg.connection().sendProcMsg( m_measureProg.procData().DDI(), m_measureProg.procData().element(), m_measureProg.getValue() );
}



/// ------------- MeasureOnChange_c -------------



MeasureOnChange_c::MeasureOnChange_c( MeasureProg_c& measureProg )
  : m_measureProg( measureProg )
  , mi32_lastVal( 0 )
  , mi32_increment( 0 )
{
}


void
MeasureOnChange_c::start( int32_t ai32_lastVal, int32_t ai32_increment )
{
  mi32_increment = ai32_increment;
  mi32_lastVal = ai32_lastVal;

  if( m_measureProg.minMaxLimitsPassed() )
    m_measureProg.connection().sendProcMsg( m_measureProg.procData().DDI(), m_measureProg.procData().element(), m_measureProg.getValue() );
}


bool
MeasureOnChange_c::updateTrigger( int32_t ai32_val )
{
  if (__IsoAgLib::abs(ai32_val - mi32_lastVal) >= mi32_increment)
  {
    mi32_lastVal = ai32_val;
    return true;
  }
  else
  {
    return false;
  }
}


void
MeasureOnChange_c::setValue( int32_t value )
{
  const bool sendProcMsg = updateTrigger( value );

  if( sendProcMsg && m_measureProg.minMaxLimitsPassed() )
    m_measureProg.connection().sendProcMsg( m_measureProg.procData().DDI(), m_measureProg.procData().element(), m_measureProg.getValue() );
}



}
