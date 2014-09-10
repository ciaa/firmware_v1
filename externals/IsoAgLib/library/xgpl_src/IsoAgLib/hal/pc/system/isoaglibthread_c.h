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
#ifndef ISOAGLIBTHREAD_C_H
#define ISOAGLIBTHREAD_C_H

// IsoAgLib
#include <IsoAgLib/isoaglib_config.h>


// This feature (ISOAgLib in a thread) is only enabled if Mutual-Exclusion is enabled, too.
#ifdef USE_MUTUAL_EXCLUSION

// IsoAgLib
#include <IsoAgLib/hal/hal_system.h>
#include <IsoAgLib/hal/generic_utils/system/mutex_pthread.h>

// system
#include <pthread.h>

// STL
#include <set>


namespace __IsoAgLib {

/** This class transparently handles the creation of a thread
 * that will run the ISOAgLib Scheduler main-loop
 *
 * If multiple modules start/stop the "thread", it will internally
 * be matched to only one single thread being running.
 *
 * The caller won't know if he actually started the thread or if it
 * was already running (he gets the same result "startSuccess")
 *
 * Passing a "key" secures double start-calls or things alike.
 *
 * NOTE: Only 1 ISOBUS Instance is supported by this class currently.
 *       It is still an open discussion if BUS/System-Init should belong
 *       into this class or not though...
 */
class IsoAgLibThread_c
{
  /// Singleton Part
public:
  static IsoAgLibThread_c& instance()
  {
    static IsoAgLibThread_c theSingleton;
    return theSingleton;
  }
private:
  IsoAgLibThread_c()
    : mc_protectAccess()
    , mthread_core()
	, mpf_threadSetupHook (NULL)
    , mb_requestThreadToStop (false)
    , mset_keys()
  {} // private c'tor
  ~IsoAgLibThread_c() {} // private d'tor

  IsoAgLibThread_c (IsoAgLibThread_c const &); // copy c'tor hidden
  IsoAgLibThread_c& operator= (IsoAgLibThread_c const &); // assignm.op. hidden


  /// Functional Part itself
public:
  //! Starts the ISOAgLib Thread with its main-loop.
  //! Note: This function is not thread-safe!
  //! @param key A unique key (e.g. "this" from the calling singleton)
  //!            to ensure that the thread is always running for that
  //!            "key" unless an explicit "stop(key)" is being called.
  //!            Note: This way other calls can't stop the thread!
  //!            Note: NULL is also a valid key!
  //! @param aui8_busNr The physical bus number to use [0..n-1]
  enum startResult {
    startSuccess,
    startFailAlreadyStartedWithDifferentSettings,
    startNoActionAlreadyRunningForThisKey
  };
  startResult start (void *key, uint8_t aui8_busNr);

  //! Stops the ISOAgLib Thread (and main-loop)
  //! @param key The unique key passed in "start(key, ..)"
  enum stopResult {
    stopSuccess,
    stopFailNotStartedForThisKey
  };
  stopResult stop (void *key);

  //! Checks if the ISOAgLib Thread is already running
  //! (regardless of who has started it)
  //! @return true => ISOAgLib Thread was already started
  //!         false => ISOAgLib Thread was not started
  bool isRunning() const;

  //! Checks if the ISOAgLib Thread was started from the
  //! given instance (represented by "key")
  //! @return true => ISOAgLib Thread was started by "key"
  //!         false => ISOAgLib Thread was not started by "key"
  bool isRunning (void *key) const;

  void installThreadSetupHook (void (*pf_threadSetupHook)());

private: // methods
  // thread stuff
  static void* thread_core (void* thread_param);

private: // attributes
  HAL::ExclusiveAccess_c mc_protectAccess; // make start/stop sequence thread-safe
  pthread_t mthread_core;
  void ( *mpf_threadSetupHook )();
  bool mb_requestThreadToStop;

  STL_NAMESPACE::set<void *>mset_keys;
};


inline
bool
IsoAgLibThread_c::isRunning() const
{
  return !mset_keys.empty();
}


inline
bool
IsoAgLibThread_c::isRunning (void *key) const
{
  return (mset_keys.count (key) > 0);
}


} // __IsoAgLib


// Also put this class into the interface!
namespace IsoAgLib
{

typedef __IsoAgLib::IsoAgLibThread_c iIsoAgLibThread_c;

} // IsoAgLib

#endif

#endif

