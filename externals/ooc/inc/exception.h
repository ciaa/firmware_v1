/*
 * exception.h
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


#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "ooc.h"

/** Exception class declaration
 */
 
DeclareClass( Exception, Base );

/** Error codes.
 * These error codes are included in the Exceptions thorwn by the ooc core.
 */
 
enum
error_codes
{
	err_no_error		= 0,			/**< no error */
	err_out_of_memory,					/**< no memory for allocating an object */
	err_bad_throw,						/**< bad use of throw (rethow whithout try, wrong code, etc.) */
	err_bad_cast,						/**< bad cast of an object */
	err_undefined_virtual,				/**< calling of an undefined virtual function */
	err_uninitialized,					/**< Class is uninitialized */
	err_can_not_be_duplicated,			/**< duplication of an object is invalid */
	err_wrong_position,					/**< insertion at wrong position into a container */
	err_bad_connect,					/**< signal connection failure */
	err_already_in_use,					/**< resource (Object) is already in use */
	err_interface_not_implemented,		/**< The requested interface is not implemented for the class */

	err_user_code		= ~0			/**< Non ooc core exception == user defined exception */
};

/* Class methods */

/** Creates a new ooc system Exception.
 * Creates a new ooc Exception, that must have an error_code of a valid ooc system
 * error code.
 * @param	error	ooc system error code.
 * @return	New Exception object.
 * @note	In case of the lack of dynamic memory handling (e.g. mcc18) this method is a fake!
 *			Simply returns a statically allocated Exception object, and overwrites the previous contents of it!
 *			(This way we can avoid recoding the whole ooc stuff for these platforms.)
 */
  
Exception	exception_new( enum error_codes error );

/** Gets the ooc core error code.
 * Gets the ooc core error code for the Exception.
 * @param	exception	The Exception Object.
 * @return	ooc core error code.
 * @note	For user defined exception (Objects inherited from Exception class) this function returns @c err_user_code.
 */
 
int			exception_get_error_code( const Exception exception );

/** Gets the user error code.
 * Gets the useer error code for the Exception.
 * @param	exception	The Exception Object.
 * @return	User error code.
 * @note	For ooc core exceptions (Objects of Exception class) this function returns @c 0.
 */

int			exception_get_user_code( const Exception exception );

/*	Exception handling
 */

/** Throws an exception.
 * Exceptions can be thrown anywhere but in Object destructors and in @c finally blocks.
 * @param	exception	The Object to be thrown. Must be a newly created object of Exception class
 * 						or a derived class. ooc will manage this Exception object, you must not
 * 						delete it yourself!
 * @note An uncaught exception is caught by the ooc core, and aborts the program execution. 
 */
 
void		ooc_throw( Exception exception );

/** Retrhows a caught exception.
 * Can be used only within a @c catch() or a @c catch_any block.
 * Must be the last statement in the block!
 */
 
void		ooc_rethrow( void ); 

struct ooc_try_block {
	Exception				exc_obj;
	int						status;
	struct ooc_try_block *	previous;
	JMP_BUF					buffer;
	struct ooc_Manageable * managed;
	};

#define		try			{ struct ooc_try_block TryContext;		\
						Exception exception;					\
						ooc_link_try_block( &TryContext );		\
						if( ! SETJMP(TryContext.buffer) )
#define		catch(ec)	else if( (exception = ooc_exception_caught( & ec ## Class ))) 
#define		catch_any	else if( (exception = ooc_exception_caught( NULL )))
#define		finally		;
#define		end_try		ooc_end_try(); }

void		ooc_link_try_block( struct ooc_try_block * );
Exception	ooc_exception_caught( const Class );
void		ooc_end_try( void );

/* Managed pointers
 */

struct ooc_Manageable
{
	void *		target;
	void		(* destroyer)( void * );
	struct ooc_Manageable * previous;
};

void		ooc_chain_manageable( struct ooc_Manageable * );

/** @name Managed pointers, managed Objects
 * Using managed pointers you will not get the program control in case of an exception, but it is guaranteed,
 * that the memory is freed or the Object is deleted in case of an exception.
 * (You may consider this as analogie for @c std::auto_ptr<> in C++.)
 * Using managed pointers is faster than using the @c try...finally...end_try constructs, 
 * so it is more advisable if you do not need the program control in case of an exception.
 * @warning Use @c ooc_manage() / @c ooc_manage_object() and @c ooc_pass() always as a pair in the same 
 * name scope! These macros use local variables, and the variable created by @c ooc_manage() must be 
 * accessible by @c ooc_pass()! Never let @c ooc_manage() be executed more than once without executing
 * the corresponding @c ooc_pass() before!
 */

/*@{ Managed pointers */

/** Manage a pointer.
 * Provides protection to a pointer, preventing memory leak in case of an exception.
 * Pushes a pointer onto the top of the managed pointers' stack.
 * @param	p_target	Pointer to the resource to be managed.
 * @param	p_destroyer	Appropriate destroyer function for the target (typically @c ooc_delete or @c ooc_free).
 * @return	Does not return anything.
 * @see ooc_pass()
 * @hideinitializer
 */

#define ooc_manage( p_target, p_destroyer ) \
	struct ooc_Manageable _managed_; \
		_managed_.target = p_target; \
		_managed_.destroyer =  p_destroyer; \
		ooc_chain_manageable( & _managed_ );
	
/** Manage an Object.
 * Provides protection to an Object, preventing memory leak in case of an exception.
 * This is a comfortable shortcut for
 * @code
 * ooc_manage( my_object, (ooc_destroyer) ooc_delete );
 * @endcode
 * @param	p_target	Pointer to the @c Objcet to be managed.
 * @see ooc_pass()
 * @note In non-dynamic memory models (OOC_NO_DYNAMIC_MEM) this macro uses
 *       ooc_release() instead of ooc_delete(),
 *       ensuring proper object destruction but without freeing the allocated memory.
 * @hideinitializer
 */

#ifndef OOC_NO_DYNAMIC_MEM
#define ooc_manage_object( p_target ) \
 	struct ooc_Manageable _managed_; \
		_managed_.target = p_target; \
		_managed_.destroyer =  (ooc_destroyer) ooc_delete; \
		ooc_chain_manageable( & _managed_ );
#else
#define ooc_manage_object( p_target ) \
 	struct ooc_Manageable _managed_; \
		_managed_.target = p_target; \
		_managed_.destroyer =  (ooc_destroyer) ooc_release; \
		ooc_chain_manageable( & _managed_ );
#endif

/** @def ooc_pass( target )
 * Removes the most recently pushed pointer from the managed pointers' stack.
 * Always use in the reverse order of using @c ooc_manage()!
 * The name is coming from passing the ownership of the pointer to an other object or function.
 * @param	target	The pointer to be removed.
 * @return	The pointer to the @c target itself.
 * @note	Please note that since the most recently pushed pointer is removed, the parameter is used only for
 * verifying that the push an pop orders are correct!
 * @warning	In release versions this verification is skipped to gain some speed.
 * @warning	Pass a pure pointer only! This macro has a side effect otherwise!
 * (e.g. passing a function as a parameter that returns the object is called twice!) 
 * @see ooc_manage(), ooc_manage_object()
 * @hideinitializer
 */
	
/*@} Managed pointers */

#ifdef NDEBUG
void		ooc_unchain_last_manageable( void );
#define		ooc_pass( target )	( ooc_unchain_last_manageable(), target )
#else
void		ooc_unchain_manageable( void * target );
#define		ooc_pass( target )	( ooc_unchain_manageable( target ), target )
#endif

/* Virtual function definitions
 */

Virtuals( Exception, Base )

	/* No virtuals, yet */

EndOfVirtuals;

#endif  /* EXCEPTION_H */

