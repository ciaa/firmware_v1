/*
  ischeduler_c.h: header for central hardware independent object which
    manages all functional IsoAgLib member objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISCHEDULER_H
#define ISCHEDULER_H

#include "impl/scheduler_c.h"
#include <IsoAgLib/scheduler/ischedulertask_c.h>


/// Begin Namespace IsoAgLib
namespace IsoAgLib {

  /**
    Central class for all other IsoAgLib classes.
    Only define one instance of iScheduler_c per
    thread, and the other classes are
    instanziated as members of this iScheduler_c class.
    The member objects represent the different scopes of functions of IsoAgLib.
    @author Dipl.-Inform. Achim Spangler
    @short central manager object for all hardware independent IsoAgLib objects.
  */
  class iScheduler_c : private __IsoAgLib::Scheduler_c {
    public:
      /** initialisation for the central IsoAgLib object
          @return true if Scheduler/System could be initialized/opened
        */
      void init( IsoAgLib::iErrorObserver_c *apc_observer = NULL ) {
        Scheduler_c::init( apc_observer );
      }

      /** every subsystem of IsoAgLib has explicit function for controlled shutdown
        */
      void close() {
        Scheduler_c::close();
      }


      /**
        call the timeEvent for CanIo_c and all communication classes (derived from SchedulerTask_c) which
        registered within iScheduler_c for periodic timeEvent.
        @return idleTime for main application (> 0 wait for next call; == 0 call function again)
                idleTime == -100 only in case of USE_MUTUAL_EXCLUSION, it means that the Mutex was already locked and the resource cannot
                be acquired.
      */
      int32_t timeEvent() {
#ifdef USE_MUTUAL_EXCLUSION
        /** Try to acquire the Resource */
        Scheduler_c::waitAcquireResource( true );

        const int32_t i32_idleTimeSpread = Scheduler_c::timeEvent();
        Scheduler_c::releaseResource();
        return i32_idleTimeSpread;
#else
        return Scheduler_c::timeEvent();
#endif
      };

      /** wait until specified timeout or until next CAN message receive
        *  @return true -> there are CAN messages waiting for process. else: return due to timeout
        */
      bool waitUntilCanReceiveOrTimeout( int32_t timeoutInterval ) {
        return Scheduler_c::waitUntilCanReceiveOrTimeout( timeoutInterval );
      }

#ifdef USE_MUTUAL_EXCLUSION
      /**
          Lock the resource TimeEvent and call it for CanIo_c
          @param i32_demandedExecEndScheduler optional timestamp, where timeEvent shall return execution to calling function
                 -> allow tight integration of IsoAgLib into application specific scheduler, as In-Time execution is
                 guaranteed (default -1 -> no execution stop defined)
        @return idleTime for main application (> 0 wait for next call; == 0 call function again)
                idleTime == -1 One Client could not finish his Job
        */
      int32_t timeEventWithWaitMutex() {
        Scheduler_c::waitAcquireResource( true );
        int32_t i32_idleTimeSpread = Scheduler_c::timeEvent();
        Scheduler_c::releaseResource();
        return i32_idleTimeSpread;
      }

      int releaseResource() {
        return Scheduler_c::releaseResource();
      }

      int tryAcquireResource() {
        return Scheduler_c::tryAcquireResource();
      }

      int waitAcquireResource() {
        return Scheduler_c::waitAcquireResource( false );
      }
#endif

      void registerTask( iSchedulerTask_c& task, int32_t delay ) {
        Scheduler_c::registerTask( task, delay );
      }

      void deregisterTask( iSchedulerTask_c& task ) {
        Scheduler_c::deregisterTask( task );
      }


    private:
      /** allow getISchedulerInstance() access to shielded base class.
          otherwise __IsoAgLib::getSchedulerInstance() wouldn't be accepted by compiler
        */
      friend iScheduler_c& getISchedulerInstance( void );

  };

  /** C-style function, to get access to the unique iScheduler_c singleton instance */
  inline iScheduler_c& getISchedulerInstance( void ) {
    return static_cast<iScheduler_c&>( __IsoAgLib::getSchedulerInstance() );
  }

}

#endif
