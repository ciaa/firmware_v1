/*
  scheduler_c.cpp: implementation for central hardware independent
    object which manages all functional IsoAgLib member objects


  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <IsoAgLib/isoaglib_config.h>
#include "scheduler_c.h"
#include <IsoAgLib/driver/system/impl/system_c.h>
#include <IsoAgLib/driver/can/impl/canio_c.h>
#include <IsoAgLib/util/iliberr_c.h>
#include <IsoAgLib/util/iassert.h>
#include <IsoAgLib/util/impl/util_funcs.h>

namespace __IsoAgLib {

  Scheduler_c &getSchedulerInstance() {
    MACRO_SINGLETON_GET_INSTANCE_BODY( Scheduler_c );
  }

  Scheduler_c::Scheduler_c()
    : Subsystem_c()
    ,mpc_registeredErrorObserver( NULL )
    ,m_taskQueue()
#ifdef USE_MUTUAL_EXCLUSION
    ,mc_protectAccess()
    ,m_breakTimeEvent( false )
#endif
  { }


  void
  Scheduler_c::init( IsoAgLib::iErrorObserver_c *apc_observer ) {
    isoaglib_assert ( !initialized() );

    IsoAgLib::getILibErrInstance().init();
    if ( apc_observer != NULL ) {
      IsoAgLib::getILibErrInstance().registerObserver ( *apc_observer );
      mpc_registeredErrorObserver = apc_observer;
    }

    // either way regardless of result avoid double init!
    setInitialized();
  }


  void
  Scheduler_c::close() {
    isoaglib_assert( initialized() );

    if ( mpc_registeredErrorObserver != NULL ) {
      IsoAgLib::getILibErrInstance().deregisterObserver( *mpc_registeredErrorObserver );
      mpc_registeredErrorObserver = NULL;
    }

    IsoAgLib::getILibErrInstance().close();

    isoaglib_assert ( m_taskQueue.empty() );

    setClosed();
  }


  void Scheduler_c::registerTask( SchedulerTask_c& task, int32_t delay ) {
    isoaglib_assert( delay >= 0) ;
    isoaglib_assert( ! task.isRegistered() );

    m_taskQueue.push_front( &task );
    task.setRegistered( true );
    task.setNextTriggerTime( System_c::getTime() + delay );
  }


  void Scheduler_c::deregisterTask( SchedulerTask_c& task ) {
    isoaglib_assert( task.isRegistered() );

    m_taskQueue.remove( &task );
    task.setRegistered( false );
  }


  int32_t Scheduler_c::timeEvent() {

#if defined( ISOAGLIB_SCHEDULER_MAX_TIMEEVENT ) && ( ISOAGLIB_SCHEDULER_MAX_TIMEEVENT > 0 )
    const int32_t startTime = System_c::getTime();
#endif

    for ( int ind = 0; ind < CAN_INSTANCE_CNT; ind++ ) {
#if defined( ISOAGLIB_SCHEDULER_MAX_TIMEEVENT ) && ( ISOAGLIB_SCHEDULER_MAX_TIMEEVENT > 0 )
      if( (System_c::getTime() - startTime) > ISOAGLIB_SCHEDULER_MAX_TIMEEVENT )
        return 0; // ran out of time in this timeEvent!  Must exit and try again next time.
#endif

#ifdef USE_MUTUAL_EXCLUSION
      getCanInstance( ind ).processMsg( m_breakTimeEvent );
      if( m_breakTimeEvent ) {
        m_breakTimeEvent = false;
        return 0;
      }
#else
      static bool sb_break = false;
      getCanInstance( ind ).processMsg( sb_break );
#endif
    }


    int32_t timeToNextTrigger;
    for( ;; ) {

      if( m_taskQueue.empty() ) {
        // we have to return some amount of mss that we have nothing todo
        // but we cannot return any usefull value. Thus 1h is used what won't
        // hurt.
        return 3600000L;
      }

#if defined( ISOAGLIB_SCHEDULER_MAX_TIMEEVENT ) && ( ISOAGLIB_SCHEDULER_MAX_TIMEEVENT > 0 )
      if( (System_c::getTime() - startTime) > ISOAGLIB_SCHEDULER_MAX_TIMEEVENT )
        return 0; // ran out of time in this timeEvent!  Must exit and try again next time.
#endif

      SchedulerTask_c& task = *( m_taskQueue.front() );

      timeToNextTrigger = task.getTimeToNextTrigger();

      if ( timeToNextTrigger > 0 )
        break;

#ifdef USE_MUTUAL_EXCLUSION
      if( m_breakTimeEvent ) {
        break;
      }
#endif

      task.timeEventPre();
      task.timeEvent();
      task.timeEventPost();
    }

#ifdef USE_MUTUAL_EXCLUSION
    m_breakTimeEvent = false;
#endif

    return timeToNextTrigger;
  }


  void
  Scheduler_c::rescheduleTask( const SchedulerTask_c& task ) {

    STL_NAMESPACE::list<SchedulerTask_c*>::iterator i;
    STL_NAMESPACE::list<SchedulerTask_c*>::iterator p;

    // find task in queue
    for( i = m_taskQueue.begin(); i != m_taskQueue.end(); ++i ) {
      if( *i == &task ) {
        p = i;
        break;
      }
    }

    // find position to insert, otherwise push to the end
    for( i = m_taskQueue.begin(); i != m_taskQueue.end(); ++i ) {
      if( ( *i )->getNextTriggerTime() > ( *p )->getNextTriggerTime() ) {
        break;
      }
    }

    if( i != p ) {
      m_taskQueue.splice( i, m_taskQueue, p );
    }
  }



} // end of namespace __IsoAgLib
