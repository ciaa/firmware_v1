/*
  isoaglibthreah_c.h: Singleton'd ISOAgLib Scheduler-main-loop thread
    This feature is only enabled if Mutual-Exclusion is enabled, too.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <IsoAgLib/isoaglib_config.h>


#ifdef USE_MUTUAL_EXCLUSION


#include "isoaglibthread_c.h"

// ISOAgLib
#include <IsoAgLib/scheduler/ischeduler_c.h>
#include <IsoAgLib/driver/system/isystem_c.h>
#include <IsoAgLib/comm/iisobus_c.h>

// system (we know we're on PC!)
#include <assert.h>


namespace __IsoAgLib {

void
IsoAgLibThread_c::installThreadSetupHook (void (*pf_threadSetupHook)())
{
  mpf_threadSetupHook = pf_threadSetupHook;
}


IsoAgLibThread_c::startResult
IsoAgLibThread_c::start (void *key, uint8_t aui8_busNr)
{
  // make start/stop thread-safe
  mc_protectAccess.waitAcquireAccess();

  // need to save that state before inserting the key!
  const bool cb_wasRunning = isRunning();

  const bool cb_wasNewlyInserted = mset_keys.insert (key).second;
  if (cb_wasNewlyInserted)
  { /// key was not yet registered, but is now
#ifdef DEBUG_THREAD
    std::cout << "IsoAgLibThread_c::start - Newly added key " << key << "." << std::endl;
#endif
    if (cb_wasRunning)
    { /// was already running before insertion
      if (aui8_busNr == IsoAgLib::getIIsoBusInstance().getBusNumber())
      { // same setup
        mc_protectAccess.releaseAccess();
        return startSuccess;
      }
      else
      { // different setup
        mc_protectAccess.releaseAccess();
        return startFailAlreadyStartedWithDifferentSettings;
      }
    }
    else
    { /// was not yet running (before insertion)
      // If we do the actions before the thread is running,
      // we don't need to mutex :-)

#ifdef DEBUG_THREAD
      std::cout << "IsoAgLibThread_c::start - Init ISOAgLib-core and -CAN/ISOBUS & Start THREAD for key " << key << "." << std::endl;
#endif

      IsoAgLib::getIsystemInstance().init();
      IsoAgLib::getISchedulerInstance().init();
      bool isoRetVal = IsoAgLib::getIIsoBusInstance().init (aui8_busNr);
      isoaglib_assert (isoRetVal == true); (void)isoRetVal;

      mb_requestThreadToStop = false;
      int createRetVal = pthread_create (&mthread_core, NULL, thread_core, (void *)this);
      isoaglib_assert (createRetVal == 0); (void)createRetVal;

      mc_protectAccess.releaseAccess();
      return startSuccess;
    }
  }
  else
  { /// key already registered
    mc_protectAccess.releaseAccess();
    return startNoActionAlreadyRunningForThisKey;
  }
}


IsoAgLibThread_c::stopResult
IsoAgLibThread_c::stop (void *key)
{
  // make start/stop thread-safe
  mc_protectAccess.waitAcquireAccess();

  // erase and check if it was erased or not
  if (mset_keys.erase (key) == 0)
  { /// was not started for this key at all
    mc_protectAccess.releaseAccess();
    return stopFailNotStartedForThisKey;
  }
  else
  { /// was running for this key - key is now already erased
    if (mset_keys.empty())
    { /// No more keys -> stop thread then of course, too!
#ifdef DEBUG_THREAD
      std::cout << "IsoAgLibThread_c::stop - Stop THREAD for key " << key << "." << std::endl;
#endif
      mb_requestThreadToStop = true;
      int joinRetVal = pthread_join (mthread_core, NULL);
      isoaglib_assert (joinRetVal == 0); (void)joinRetVal;
      // Thread is not running now anymore...
      bool isoRetVal = IsoAgLib::getIIsoBusInstance().close();
      isoaglib_assert (isoRetVal == true); (void)isoRetVal;
      IsoAgLib::getISchedulerInstance().close();
      IsoAgLib::getIsystemInstance().close();
      // Last one closed the door.
    }
    else
    { // still some other key(s) registered, keep thread running
#ifdef DEBUG_THREAD
      std::cout << "IsoAgLibThread_c::stop - Thread still running, only removed key " << key << "." << std::endl;
#endif
    }
    mc_protectAccess.releaseAccess();
    return stopSuccess;
  }
}


void *
IsoAgLibThread_c::thread_core (void* thread_param)
{
  IsoAgLibThread_c &thisThread = *(static_cast<IsoAgLibThread_c *>(thread_param));

  // call user-hook for setting up things like thread-name, -priority, etc.
  if( thisThread.mpf_threadSetupHook != NULL )
    thisThread.mpf_threadSetupHook();

  while (!thisThread.mb_requestThreadToStop)
  {
    const int32_t i32_sleepTime = IsoAgLib::getISchedulerInstance().timeEventWithWaitMutex();

    if (i32_sleepTime > 0) {
      IsoAgLib::getISchedulerInstance().waitUntilCanReceiveOrTimeout (i32_sleepTime);
    }
  }

  return NULL;
}

}

#endif

