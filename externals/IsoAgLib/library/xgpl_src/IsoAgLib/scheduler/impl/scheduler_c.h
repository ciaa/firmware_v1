/*
  scheduler_c.h: header for central hardware independent object which
    manages all functional IsoAgLib member objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef SCHEDULER_H
#define SCHEDULER_H


namespace IsoAgLib {
  class iErrorObserver_c;
}

#include <IsoAgLib/isoaglib_config.h>
#include <list>

#include "schedulertask_c.h"

#include <IsoAgLib/driver/can/impl/canio_c.h>
#include <IsoAgLib/hal/hal_system.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/util/impl/util_funcs.h>
#include <IsoAgLib/scheduler/impl/schedulertask_c.h>


/// Begin Namespace __IsoAgLib
namespace __IsoAgLib {
  /**
    Central class for all other IsoAgLib classes.
    Only define one instance of Scheduler_c per
    thread, and the other classes are
    instanziated as members of this Scheduler_c class.
    The member objects represent the different scopes of functions of IsoAgLib.
    @author Dipl.-Inform. Achim Spangler
    @short central manager object for all hardware independent IsoAgLib objects.
  */
  class Scheduler_c : public Subsystem_c {
    public:
      void init( IsoAgLib::iErrorObserver_c *apc_observer = NULL );
      void close();

      /**
        call the timeEvent for CanIo_c and all communication classes (derived from SchedulerTask_c) which
        registered within Scheduler_c for periodic timeEvent.
        @return idleTime for main application (> 0 wait for next call; == 0 call function again)
      */
      int32_t timeEvent();

      /** wait until specified timeout or until next CAN message receive
       *  @return true -> there are CAN messages waiting for process. else: return due to timeout
       */
      bool waitUntilCanReceiveOrTimeout( int32_t timeoutInterval ) {
        return CanIo_c::waitUntilCanReceiveOrTimeout( timeoutInterval );
      }

      void registerTask( SchedulerTask_c& task, int32_t delay );
      void deregisterTask( SchedulerTask_c& task );

#ifdef USE_MUTUAL_EXCLUSION
      int releaseResource() {
        return mc_protectAccess.releaseAccess();
      }

      int tryAcquireResource() {
        return mc_protectAccess.tryAcquireAccess();
      }

      //! Lock the resource to prevent other threads to use it. If the resource is already locked,
      //! the calling threads is in blocking state until the unlock.
      int waitAcquireResource( bool isoaglibTimeEventThread ) {
        if( ! isoaglibTimeEventThread ) {
          HAL::canRxWaitBreak();
          m_breakTimeEvent = true;
        }
        return mc_protectAccess.waitAcquireAccess();
      }
#endif

    private:
      friend class SchedulerTask_c;

      Scheduler_c();

      /** registered Error-Observer. Stored to be able to
          automatically deregister at close(). */
      IsoAgLib::iErrorObserver_c *mpc_registeredErrorObserver;

      void rescheduleTask( const SchedulerTask_c& task );

      //  Attribute: mc_taskQueue
      STL_NAMESPACE::list<SchedulerTask_c*> m_taskQueue;

#ifdef USE_MUTUAL_EXCLUSION
      /** Attribute for the exclusive access of the IsoAgLib for threads */
      HAL::ExclusiveAccess_c mc_protectAccess;

      bool m_breakTimeEvent;
#endif

      friend Scheduler_c &getSchedulerInstance();
  };

  /** C-style function, to get access to the unique Scheduler_c singleton instance */
  Scheduler_c &getSchedulerInstance();

}

#endif
