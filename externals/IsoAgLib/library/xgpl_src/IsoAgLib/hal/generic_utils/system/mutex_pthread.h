/*
  mutex_pthread.h: Wrapper class for MUTEXes.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _HAL_MUTEX_PTHREAD_H_
#define _HAL_MUTEX_PTHREAD_H_


#ifdef USE_MUTUAL_EXCLUSION
#include <pthread.h>
#include <assert.h>


namespace HAL
{


/**
 * Class wrapping for a MUTEX
 * @short Class wrapping a MUTEX.
*/

class ExclusiveAccess_c {
public:

  ExclusiveAccess_c() {
                        const int i_retV = pthread_mutex_init( &m_exclusiveAccess, NULL );
                        ( void )i_retV; assert( i_retV == 0 );
                      }

  ~ExclusiveAccess_c() { pthread_mutex_destroy( &m_exclusiveAccess ); }

  int waitAcquireAccess() { return pthread_mutex_lock( &m_exclusiveAccess ); }
  int tryAcquireAccess() { return pthread_mutex_trylock( &m_exclusiveAccess ); }
  int releaseAccess() { return pthread_mutex_unlock( &m_exclusiveAccess ); }

private:
   pthread_mutex_t m_exclusiveAccess;
};

}

#endif

#endif

