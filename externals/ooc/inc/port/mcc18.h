/*
 * port/mcc18.h
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

#ifndef MCC18_H_
#define MCC18_H_

/* Compilation control
 */

#ifndef __DEBUG
#define NDEBUG
#endif

#define NO_RELATIVE_INCLUDE 1

#define OOC_NO_FINALIZE	1

/* Inlcude standard headers */

#include <stdlib.h>

#ifdef COMPILING_OOC_C
#include <stddef.h>
#include <string.h>
#endif /* COMPILING_OOC_C */

/* Memory model */

#define OOC_NO_DYNAMIC_MEM 1

#define ROM const rom

#define GEN_PTR	void *

#define _OOC_VTAB_INITIALIZER = { NULL }

/* Longjump functions */
 
#include <setjmp.h>

#define JMP_BUF		jmp_buf
#define SETJMP		setjmp
#define LONGJMP		longjmp

/* Implementing missing assert */

#ifdef NDEBUG
#define assert(e)
#else
extern void _fassert( int, ROM char * );
#define assert(e) if(!(e)) { _fassert(  __LINE__, __FILE__ ); }
#endif


/* implementing static inline */
#define STIN static
#define NO_INLINE
#define OOC_NO_THREADS

/* Threadless implementation */

#define TLS static

#define	ooc_Mutex				void
#define ooc_mutex_init( x )
#define ooc_mutex_release( x )
#define ooc_lock( x )	
#define ooc_unlock( x )
#define ooc_try_lock( x )		TRUE 


/* Definition of missing library functions
 */

#define abort() for(;;)
#define setbuf(stream, param)



#ifdef COMPILING_OOC_C

/*	Helper: pointer read-out while nulling
 */

#define OOC_IMPLEMENT_PTR_READ_AND_NULL						\
	void * tmp = * ptr_ptr;									\
	* ptr_ptr = NULL;										\
	return tmp;

#endif /* COMPILING_OOC_C */

/* TestCase format strings
 */

#define _FMT_Failed_ssu		"\tFailed: %HS [%HS : %u]\n"
#define _FMT_Func_ss		"%HS::%HS()"
#define _FMT_Func_dss		"[%d] %HS::%HS()"
#define _FMT_Func_dsss		"[%d] %HS::%HS.%HS()"
#define _FMT_Name_dss		"[%d] %HS::%s"
#define _FMT_Name_dsss		"[%d] %HS::%s.%HS()"
#define _FMT_Exc_sdd		"\tUnexpected exception: %HS, code: %d, user code: %d\n"
#define _FMT_Exc_ssdd		"\n\tUnexpected exception %HS in %HS, code: %d, user code: %d\n"
#define _FMT_Failed_sdd		"Test case %HS failed: %d/%d (methods run/failed)\n"

#define strlenROM(x) strlenpgm(x)

#define TESTCASE_MAIN		void main( void )


#endif /*MCC18_H_*/
