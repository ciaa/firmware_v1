/*
 * port/mcc32.h
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

#ifndef MCC32_H_
#define MCC32_H_

/* Compilation control
 */

#ifndef __DEBUG
#define NDEBUG
#endif

#define OOC_NO_FINALIZE	1

/* Inlcude standard headers */

#include <stdlib.h>
#include <assert.h>

#ifdef COMPILING_OOC_C
#include <stddef.h>
#include <string.h>
#endif /* COMPILING_OOC_C */

/* Memory model */

#define ROM const

/* Longjump functions */
 
#include <setjmp.h>

#define JMP_BUF		jmp_buf
#define SETJMP		setjmp
#define LONGJMP		longjmp

/* implementing static inline */
#if !defined( __NO_INLINE__ )
#define STIN static __inline__
#else 
#define STIN static
#define NO_INLINE
#endif

#define OOC_NO_THREADS

/* Threadless implementation */

#define TLS static

#define	ooc_Mutex				void
#define ooc_mutex_init( x )
#define ooc_mutex_release( x )
#define ooc_lock( x )	
#define ooc_unlock( x )
#define ooc_try_lock( x )		TRUE 

/* Compilation control
 */

/* #define OOC_HAS_UNIX_SIGNALS
	Has, but does not work as expected :-(
 */

#ifdef COMPILING_OOC_C

/*	Helper: pointer read-out while nulling
 */

#define OOC_IMPLEMENT_PTR_READ_AND_NULL						\
	void * tmp = * ptr_ptr;									\
	* ptr_ptr = NULL;										\
	return tmp;

#endif /* COMPILING_OOC_C */

#define TESTCASE_MAIN 	int main (void)

#endif /*MCC32_H_*/
