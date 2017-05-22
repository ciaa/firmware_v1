/*
 * port/gnuc.h
 * 
 * Copyright (c) 2011, old_fashioned. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

#ifndef GNUC_H_
#define GNUC_H_

/* we are in the GNU world */
#define _GNU_SOURCE		1

/* Inlcude standard headers */

#include <stdlib.h>
#include <assert.h>

#ifdef COMPILING_OOC_C
#include <stddef.h>
#include <string.h>
#endif /* COMPILING_OOC_C */

/* Longjump functions */
 
#include <setjmp.h>

#ifdef	__USE_POSIX

#define JMP_BUF					sigjmp_buf
#define SETJMP(buff)			sigsetjmp ( buff, TRUE )
#define LONGJMP(env, val)		siglongjmp( env, val )

#else

#define JMP_BUF		jmp_buf
#define SETJMP		setjmp
#define LONGJMP		longjmp

#endif

/* implementing static inline */
#if !defined( __NO_INLINE__ )
#define STIN static __inline__
#else 
#define STIN static
#define NO_INLINE
#endif

#ifndef OOC_NO_THREADS

/* implementing Thread Local Storage */

#define TLS static __thread

#ifdef _OPENMP

#include <omp.h>

#define	ooc_Mutex				omp_lock_t
#define ooc_mutex_init( x )		omp_init_lock( & x )
#define ooc_mutex_release( x )	omp_destroy_lock( & x )
#define ooc_lock( x ) 			omp_set_lock( & x )	
#define ooc_unlock( x ) 		omp_unset_lock( & x )
#define ooc_try_lock( x )		omp_test_lock( & x ) 

#else

#include <pthread.h>

/* Because omp_lock_t is implemented as 4 bytes of char[], we must implement ooc_Mutex as a pointer too
in pthreads threading mode, to keep the binary compatibility between openmp and native 
pthread threading. This let us mix the ooc libraries compiled with native -pthread settings 
and user programs either with -pthread or with -fopenmp settings. */

typedef	pthread_mutex_t *		ooc_Mutex;
#define ooc_mutex_init( x )		pthread_mutex_init ( x = ooc_malloc( sizeof(pthread_mutex_t) ), NULL )
#define ooc_mutex_release( x )	do { pthread_mutex_destroy( x ); ooc_free( x ); } while(0)
#define ooc_lock( x ) 			pthread_mutex_lock( x )	
#define ooc_unlock( x ) 		pthread_mutex_unlock( x )
#define ooc_try_lock( x )		pthread_mutex_trylock( x ) 

#endif

#else /* threadless implementation */

#define TLS static

#define	ooc_Mutex				void
#define ooc_mutex_init( x )
#define ooc_mutex_release( x )
#define ooc_lock( x )	
#define ooc_unlock( x )
#define ooc_try_lock( x )		TRUE 

#endif

/* Compilation control
 */

#define OOC_HAS_UNIX_SIGNALS


#ifdef COMPILING_OOC_C

/*	Helper: pointer read-out while NULLing
 */

#define OOC_IMPLEMENT_PTR_READ_AND_NULL			\
	return __sync_fetch_and_and (ptr_ptr, 0);

#endif /* COMPILING_OOC_C */

#endif /*GNUC_H_*/
