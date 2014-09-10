/*
  schedulertask_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "schedulertask_c.h"

#include <IsoAgLib/driver/system/impl/system_c.h>
#include <IsoAgLib/scheduler/impl/scheduler_c.h>

namespace __IsoAgLib {

  SchedulerTask_c::SchedulerTask_c( int32_t period, bool hardTiming )
    : Subsystem_c()
    , m_hardTiming( hardTiming )
    , m_nextTriggerTimeSet( false )
    , m_registered( false )
    , m_nextTriggerTime( -1 )
    , m_period( period )
#if defined( ISOAGLIB_DEBUG_TIMEEVENT ) || defined( ISOAGLIB_TASK_MAX_TIMEEVENT )
    , m_startTime( -1 )
    , m_thisTimeEvent( -1 )
    , m_maxTimeEvent( -1 )
#endif
  {
    isoaglib_assert( period != 0 );
  }


  void SchedulerTask_c::setPeriod( int32_t period, bool retrigger ) {
    isoaglib_assert( period != 0 );
    isoaglib_assert( !retrigger || isRegistered() );
    isoaglib_assert( !(period < 0) || !retrigger );

    m_period = period;

    if( retrigger )
      setNextTriggerTime( System_c::getTime() + getPeriod() );
  }


  void SchedulerTask_c::setNextTriggerTime( int32_t time ) {
    isoaglib_assert( isRegistered() );

    m_nextTriggerTime = time;
    getSchedulerInstance().rescheduleTask( *this );
    m_nextTriggerTimeSet = true;
  }


  void SchedulerTask_c::timeEventPre() {
    m_nextTriggerTimeSet = false;
#if defined( ISOAGLIB_DEBUG_TIMEEVENT ) || defined( ISOAGLIB_TASK_MAX_TIMEEVENT )
    m_startTime = System_c::getTime();
#endif
  }


  void SchedulerTask_c::timeEventPost() {
#if defined( ISOAGLIB_DEBUG_TIMEEVENT ) || defined( ISOAGLIB_TASK_MAX_TIMEEVENT )
    m_thisTimeEvent = System_c::getTime() - m_startTime;

    if( m_maxTimeEvent < m_thisTimeEvent )
      m_maxTimeEvent = m_thisTimeEvent;

#if defined( ISOAGLIB_TASK_MAX_TIMEEVENT )
    isoaglib_assert( m_thisTimeEvent <= ISOAGLIB_TASK_MAX_TIMEEVENT );
#endif
#endif

    if( m_nextTriggerTimeSet ) {
      // next trigger time is set during time event call
      // so we don't have to adjust our next trigger time here and
      // we signal to the scheduler, that he don't has to schedule
      // us cause this already happend within the setNextTriggerTime
      // call
      m_nextTriggerTimeSet = false;
    } else {
      if( getPeriod() > 0 )
      {
        // task could've been legally deregistered in timeEvent.
        if( isRegistered() ) {
          // relax timing if not set to hard timing: calculate
          // the next trigger time from now and not from the time
          // we would have been theoretically called
          if( ! m_hardTiming ) {
            m_nextTriggerTime = System_c::getTime();
          }

          m_nextTriggerTime += getPeriod();
          while( m_nextTriggerTime < System_c::getTime() )
            m_nextTriggerTime += getPeriod();

          getSchedulerInstance().rescheduleTask( *this );
        }
      }
      else
      {
        getSchedulerInstance().deregisterTask( *this );
      }
    }
  }


  void SchedulerTask_c::setRegistered( bool r ) {
    m_registered = r;
  }

} /// end of namespace
