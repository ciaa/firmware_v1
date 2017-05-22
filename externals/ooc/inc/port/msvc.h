/*
 * port/msvc.h
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

#ifndef MSVC_H_
#define MSVC_H_

/* Inlcude standard headers */

#include <stdlib.h>
#include <assert.h>

#ifdef COMPILING_OOC_C
#include <stddef.h>
#include <string.h>
#endif /* COMPILING_OOC_C */


/* Longjump functions */
 
#include <setjmp.h>

#define JMP_BUF		jmp_buf
#define SETJMP		setjmp
#define LONGJMP		longjmp


/* implementing static inline */
#define STIN static _inline

#ifndef OOC_NO_THREADS

/* implementing Thread Local Storage */
#define TLS static _declspec( thread )

#ifdef _OPENMP

#include <omp.h>

#define	ooc_Mutex				omp_lock_t
#define ooc_mutex_init( x )		omp_init_lock( & x )
#define ooc_mutex_release( x )	omp_destroy_lock( & x )
#define ooc_lock( x ) 			omp_set_lock( & x )	
#define ooc_unlock( x ) 		omp_unset_lock( & x )
#define ooc_try_lock( x )		omp_test_lock( & x ) 

#else

#include <Windows.h>

/* Because omp_lock_t is implemented as void *, we must implement ooc_Mutex as a pointer too
in native Windows threading mode, to keep the binary compatibility between openmp and native 
Windows threading. This let us mix the ooc libraries compiled with native Windows settings 
and user programs either with native or with /openmp settings. */

typedef	CRITICAL_SECTION *		ooc_Mutex;
#define ooc_mutex_init( x )		InitializeCriticalSection ( x = ooc_malloc( sizeof(CRITICAL_SECTION) ) )
#define ooc_mutex_release( x )	do { DeleteCriticalSection( x ); ooc_free( x ); } while(0)
#define ooc_lock( x ) 			EnterCriticalSection( x )	
#define ooc_unlock( x ) 		LeaveCriticalSection( x )
#define ooc_try_lock( x )		TryEnterCriticalSection( x ) 

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

/*	Helper: pointer read-out while nulling
 */

#include <windows.h>

#define OOC_IMPLEMENT_PTR_READ_AND_NULL				\
	return InterlockedExchangePointer( ptr_ptr, NULL);

#endif /* COMPILING_OOC_C */

/* Disable some depreciated warnings */
#pragma warning( disable:4996 )

#endif /*MSVC_H_*/
