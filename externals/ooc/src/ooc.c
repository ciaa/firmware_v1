/*
 * ooc.c
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



#define COMPILING_OOC_C

#include "ooc.h"

#include "exception.h"

/** @file ooc.h
 * @brief Object Oriented C - macros and definitions.
 */

/**	Base Class.
 * Used for root Class in ooc. It must be a superclass for all classes.
 */

ROM_ALLOC struct ClassTable BaseClass;

/* Class initialization
 */

static
void
invalid_virtual_function( void )
{
	 ooc_throw( exception_new( err_undefined_virtual ) ); 
}

/*	Calculating the offset of the first virtual function in the Vtable.
	Must be done this tricky, because the following assumption is not necessarily true:
	sizeof(struct BaseVtable) == offsetof( _dummyVtable, first_virtual_function)
	This assumption may fail, depending on the compiler and the system ! */

struct _dummyVtable {
	struct	BaseVtable_stru	Base;
	void	(* first_virtual_function)( void );
};
#define virtual_function_alignment (offsetof( struct _dummyVtable, first_virtual_function))


static
void
invalidate_vtable( const Class type )
{
	size_t i;
	void (**fnp)(void);

	if( type->vtab_size > virtual_function_alignment ) {

		i = ( type->vtab_size - virtual_function_alignment ) / sizeof( void(*)() );
		fnp = (void (**)(void)) ( ((char*) type->c.vtable) + virtual_function_alignment);

		for( ; i ; i--,  fnp++ )
			*fnp = invalid_virtual_function;
		}
}

static
void
inherit_vtable_from_parent( const Class self )
{
	if( ooc_class_has_parent(self) ) {

		assert( self->vtab_size >= self->parent->vtab_size );
		
		/* Inherit the overridden operators */
		self->c.vtable->_destroy_check = self->parent->c.vtable->_destroy_check;

		/* Inherit the virtual functions */
		if( self->parent->vtab_size > virtual_function_alignment )
		    memcpy( ((GEN_PTR) self->c.vtable)+ virtual_function_alignment,			/* destination */
			    ((GEN_PTR) self->parent->c.vtable) + virtual_function_alignment,	/* source */
			    self->parent->vtab_size - virtual_function_alignment );				/* bytes to copy */
		}
}

#ifndef OOC_NO_FINALIZE

static ClassCommons	class_register = NULL;		/* Points to the most recently initialized Class */

#ifndef OOC_NO_THREADS
static ooc_Mutex	class_register_change;
#endif

static
void
ooc_register_class( ClassCommons self )
{
	if( class_register == NULL )					/* Race condition, but however we declared _ooc_init_class() as non-thread-safe */
		ooc_mutex_init( class_register_change );

	ooc_lock( class_register_change );
	if( class_register )
		class_register->vtable->_class_register_next = self;
	self->vtable->_class_register_prev = class_register;
	self->vtable->_class_register_next = NULL;
	class_register = self;
	ooc_unlock( class_register_change );
}

#endif /* OOC_NO_FINALIZE */

static
void
ooc_init_interfaces( const Class self )
{
	Itable	it;
	size_t	in;

	for( in = self->itab_size, it = self->itable; in--; it++ )
	{
		if( it->id->type.value == _OOC_TYPE_MIXIN )
		{
			ROM struct MixinTable * mixin = (ROM struct MixinTable *) it->id;

			if( mixin->c.vtable->_class == NULL )
			{
				mixin->c.vtable->_class = (Class) mixin;
				mixin->init();
				#ifndef OOC_NO_FINALIZE
				ooc_register_class( (ClassCommons) mixin );
				#endif
			}

			mixin->populate( (void (**)() ) ( (char*) self->c.vtable + it->vtab_offset ) );
		}
	}
}

void
_ooc_init_class( const Class self )
{
	if( self->c.vtable->_class == NULL ) {

		if( ooc_class_has_parent( self ) )
			_ooc_init_class( self->parent );

		self->c.vtable->_class = self;
		self->c.vtable->_destroy_check = NULL;

		invalidate_vtable( self );

		inherit_vtable_from_parent( self );

		ooc_init_interfaces( self );

		self->init( self );

#ifndef OOC_NO_FINALIZE
		ooc_register_class( (ClassCommons) self );
#endif /* OOC_NO_FINALIZE */
	}
}

#ifndef OOC_NO_FINALIZE

void
_ooc_finalize_class( const Class _self )
{
	ClassCommons self = (ClassCommons) _self;

	if( self->vtable->_class != NULL ) {

		self->vtable->_class = NULL;		/* Class is marked uninitalized */
		
		ooc_lock( class_register_change );

		if( class_register == self )		/* Move class register pointer if necessary */
			class_register = self->vtable->_class_register_prev;

		if( self->vtable->_class_register_prev )	/* Unchain the current class I. */
			self->vtable->_class_register_prev->vtable->_class_register_next = self->vtable->_class_register_next;
			
		if( self->vtable->_class_register_next )	/* Unchain the current class II. */
			self->vtable->_class_register_next->vtable->_class_register_prev = self->vtable->_class_register_prev;

		self->vtable->_class_register_prev = NULL;
		self->vtable->_class_register_next = NULL;
				
		if( class_register == NULL )
			ooc_mutex_release( class_register_change );
		else
			ooc_unlock( class_register_change );
			
		self->finz( self );					/* Finalize the current class */
	}	
}

void
ooc_finalize_all( void )
{
	while( class_register )
		_ooc_finalize_class( (Class) class_register );
}

#endif /* OOC_NO_FINALIZE */


/*	Creating a new Object
 */

static
void
ooc_build_mixins( Object carrier, Class type )
{
	if( ooc_class_has_parent( type ) )
		ooc_build_mixins( carrier, type->parent );

	if( type->itable != NULL )
	{
		Itable 		it;
		size_t		itn;

		for( it = type->itable, itn = type->itab_size; itn--; it++ )
		{
			if( it->id->type.value == _OOC_TYPE_MIXIN )
				((ROM struct MixinTable *) it->id )->ctor(
						(void (**)()) ((char*) type->c.vtable + it->vtab_offset),
						(char*) carrier + it->data_offset );
		}
	}
}

STIN
void
ooc_build_object( Object object, const Class type, const void * params )
{
	/* Building the Object header */
	object->_vtab = type->c.vtable;		/* Vtable pointer */
	
		assert( sizeof( struct BaseObject ) == sizeof( struct BaseVtable * ) );
		/* If struct BaseObject has been changed, additional initialization might be missing here! */

	ooc_build_mixins( object, type );

	/* Constructs the object instance, that may fail */
	type->ctor( object, params );	
}

void
ooc_use_classptr( void * mem, const Class type, const void * params )
{
	Object object = (Object) mem;
	
	/* The class type must be initialized already! */
	assert( _ooc_isInitialized( type ) );
	
	memset( object, 0, type->size );  /* the constructor expects everything to be zeroed */
	
	ooc_build_object( object, type, params );
}
	
#ifndef OOC_NO_DYNAMIC_MEM

Object
ooc_new_classptr( const Class type, const void * params )
{
	Object object;

	/* The class type must be initialized already! */
	assert( _ooc_isInitialized( type ) );

	/* Allocates a memory block for the object instance, and initializes it with zeros */
	object = ooc_calloc(  1, type->size );

	{
	ooc_manage_object( object );

	ooc_build_object( object, type, params );

	return ooc_pass( object );
	}
}

#endif /* OOC_NO_DYNAMIC_MEM */

/*	Copying an existing object
 */

static
size_t
ooc_copy_mixins( Object to_carrier, Object from_carrier, Class type )
{
	size_t max_object_length = type->size;

	if( type->itable != NULL )
	{
		Itable 		it;
		size_t		itn;

		for( it = type->itable, itn = type->itab_size; itn--; it++ )
		{
			if( it->id->type.value == _OOC_TYPE_MIXIN )
			{
				if( max_object_length > it->data_offset )
					max_object_length = it->data_offset;

				switch( ((ROM struct MixinTable *) it->id )->copy(
							(void (**)()) ((char*) type->c.vtable + it->vtab_offset),
							(GEN_PTR) ((char*) to_carrier + it->data_offset),
							(GEN_PTR) ((char*) from_carrier + it->data_offset )) )
				{
				case OOC_COPY_DONE:		break;

				case OOC_COPY_DEFAULT:	if( ((ROM struct MixinTable *) it->id )->size != 0 )
											memcpy( (GEN_PTR)((char*) to_carrier + it->data_offset),
													(GEN_PTR)((char*) from_carrier + it->data_offset),
													((ROM struct MixinTable *) it->id )->size );
										break;

				case OOC_NO_COPY:
				default:				ooc_throw( exception_new( err_can_not_be_duplicated ) );
										break;
				}
			}
		}
	}
	return max_object_length;
}

static
void
copy_object_members( Object to, const Object from, const Class type )
{
	size_t offset, length;
							
	if( ooc_class_has_parent( type ) )
		copy_object_members( to, from, type->parent );
		
	length = ooc_copy_mixins( to, from, type );

	switch( type->copy( to, from ) ) {
		
		case OOC_COPY_DONE:		break;
		
		case OOC_COPY_DEFAULT:	offset = ooc_class_has_parent( type ) ?  type->parent->size : sizeof( struct BaseObject );
								length = length - offset;
					
								if( length )
									memcpy( ((GEN_PTR)to)+offset, ((GEN_PTR)from)+offset, length );
								break;
							
		case OOC_NO_COPY:
		default:				ooc_throw( exception_new( err_can_not_be_duplicated ) );
								break;
		};
}

STIN
void
ooc_make_a_copy( Object to, const Object from, const Class type )
{
	/* Initializes the Object header */
	to->_vtab = from->_vtab;
		assert( sizeof( struct BaseObject ) == sizeof( struct BaseVtable * ) );
		/* If struct BaseObject has been changed, additional copying is missing here!
			memcpy( duplicate, from, sizeof(struct BaseObject)); would be a general solution, but much slover! */

	/* Calls the copy constructor */
	copy_object_members( to, from, type );
}

#ifndef OOC_NO_DYNAMIC_MEM

Object
ooc_duplicate( const Object from )
{
	Object	duplicate;
	Class	type;
	
	if( from != NULL )
	{
		type = from->_vtab->_class;

		/* Allocates memory for the object instance */
		duplicate = ooc_calloc( 1, type->size );
		{
			ooc_manage_object( duplicate );

			ooc_make_a_copy( duplicate, from, type );

			return ooc_pass( duplicate );
		}
	}
	else
		return NULL;
}

#endif /* OOC_NO_DYNAMIC_MEM */

Object
ooc_copy( void * to, const Object from )
{
	Class	type = from->_vtab->_class;

	ooc_manage( to, (ooc_destroyer) ooc_release );

	memset( to, 0, type->size );  /* the constructor expects everything to be zeroed */

	ooc_make_a_copy( to, from, type );

	return ooc_pass( to );
}


/*	Deletes an object
 */

static
void
ooc_destroy_mixins( Class type, Object carrier )
{
	if( type->itable != NULL )
	{
		Itable 		it;
		size_t		itn;

		for( it = type->itable, itn = type->itab_size; itn--; it++ )
		{
			if( it->id->type.value == _OOC_TYPE_MIXIN )
				((ROM struct MixinTable *) it->id )->dtor(
						(void (**)()) ((char*) type->c.vtable + it->vtab_offset),
						(char*) carrier + it->data_offset );
		}
	}
}

static
void
ooc_destroy_object( Object self )
{
	Class type, next;
	Vtable vtab;

	assert( self != NULL );
	
	/* Makes the Object invalid */
	vtab = ooc_ptr_read_and_null( (void**) &self->_vtab );
	
	if( vtab )
	{
		next = vtab->_class;
		
		do {
			type = next;

			type->dtor( self, vtab );
			ooc_destroy_mixins( type, self );
	
			next = type->parent;
		} while( ooc_class_has_parent( type ) );
	}
}

void
ooc_release( Object self )
{
	if( self && self->_vtab )
		if( self->_vtab->_destroy_check == NULL || (self->_vtab->_destroy_check)(self) == TRUE )
		{
			ooc_destroy_object( self );
		}
}

#ifndef OOC_NO_DYNAMIC_MEM

void
ooc_delete( Object self )
{
	if( self && self->_vtab )
		if( self->_vtab->_destroy_check == NULL || (self->_vtab->_destroy_check)(self) == TRUE )
		{
			ooc_destroy_object( self );
			ooc_free( self );
		}
}

void
ooc_delete_and_null( Object * obj_ptr )
{
	assert( obj_ptr );

	/* Thread safe release of the object */
	ooc_delete( (Object) ooc_ptr_read_and_null( (void**) obj_ptr ) );
}

#endif /* OOC_NO_DYNAMIC_MEM */

/* Type checking helpers
 *
 ***********************************************************/

int
_ooc_isInitialized( const Class type )
{
	return ( type == & BaseClass || type->c.vtable->_class == type ) ? TRUE : FALSE;
}

static
int
ooc_isClassChildOf( const Class checkable, const Class base )
{
	Class actual = checkable;
	
	do {
		if( actual->parent == base )
			return TRUE;
		actual = actual->parent;
		} while( actual );

	return FALSE;
}

int
_ooc_isInstanceOf( const Object self, const Class base )
{
	assert( _ooc_isInitialized( base ) );

	if( ! self )
		return FALSE;

	if( ! self->_vtab )
		return FALSE;

	if( ! self->_vtab->_class )
		return FALSE;

	if( self->_vtab->_class == base )
		return TRUE;

	if( self->_vtab->_class->c.vtable != self->_vtab )
		return FALSE;

	return ooc_isClassChildOf( self->_vtab->_class, base );
}

int
_ooc_isClassOf( const Class this, const Class base )
{
	if( this == base )
		return TRUE;
	else
		return ooc_isClassChildOf( this, base );
}


void
ooc_check_cast( void * _self, const Class target )
{
	if( ! _ooc_isInstanceOf( _self, target ) )
		ooc_throw( exception_new( err_bad_cast ) );
		
	return;
}


Class
ooc_get_type( const Object self )
{
	if( (! self) || (! self->_vtab) || (! self->_vtab->_class) )
		ooc_throw( exception_new( err_bad_cast ) );

	return ( Class ) ( self->_vtab->_class );
}


/* Implementation of memory handling
 *
 ***********************************************************/

#ifndef OOC_NO_DYNAMIC_MEM

void *
ooc_malloc( size_t size )
{
	void * allocated;

	allocated = malloc( size );

	if( allocated == NULL )
		ooc_throw( exception_new( err_out_of_memory ) );

	return allocated;
}

void *
ooc_calloc( size_t num, size_t size )
{
	void * allocated;

	allocated = calloc( num, size );

	if( allocated == NULL )
		ooc_throw( exception_new( err_out_of_memory ) );

	return allocated;
}

void *
ooc_realloc( void *ptr, size_t size )
{
	void * allocated;

	allocated = realloc( ptr, size );

	if( allocated == NULL )
		ooc_throw( exception_new( err_out_of_memory ) );

	return allocated;
}

void *
ooc_memdup( const void * ptr, size_t size )
{
	void * duplicate = NULL;

	if( ptr ) {
		
		duplicate = ooc_malloc( size );
	
		memcpy( duplicate, ptr, size );

		}
		
	return duplicate;
}

char *
ooc_strdup( const char * s )
{
	return s ? (char *) ooc_memdup( s, strlen( s ) + 1 ) : NULL;	
}


void
ooc_free( void * mem )
{
	if( mem )
		free( mem );
}

void
ooc_free_and_null( void ** mem )
{
	void * mem_ptr;

	assert( mem );

	/* Thread safe release of the memory while "nulling" the memory pointer */
	mem_ptr = ooc_ptr_read_and_null( mem );
	if( mem_ptr )
		free( mem_ptr );
}

#endif /* OOC_NO_DYNAMIC_MEM */

/*	Helper: pointer read-out while nulling
 */

void *
ooc_ptr_read_and_null( void ** ptr_ptr )
{
	OOC_IMPLEMENT_PTR_READ_AND_NULL
}

/* Interface handling
 *
 * ***********************************************************/

TLS		Vtable									prev_vtab		= NULL;		/* previously checked Vtable */
TLS		InterfaceID								prev_id			= NULL;		/* previously checked InterfaceID */
TLS		ROM struct InterfaceOffsets_struct * 	prev_itab_entry = NULL;		/* previously found and returned itab entry */

void *
_ooc_get_interface( const Object self, InterfaceID id )
{
	Class 									type, next_type;
	ROM struct InterfaceOffsets_struct * 	itable;
	size_t 									i;

	/* Add some cache functionality to accelerate subsequent calls. Will be useful in mixins. */
	if( ( id == prev_id ) && ( self->_vtab == prev_vtab ) )
		return ( prev_itab_entry != NULL ) ? (char*) self->_vtab + prev_itab_entry->vtab_offset : NULL;

	prev_vtab	= self->_vtab;
	prev_id		= id;
	prev_itab_entry = NULL;

	next_type	= self->_vtab->_class;

	do
	{
		type = next_type,	next_type = type->parent;

		itable	= type->itable;
		if( itable )
		{
			for( i = type->itab_size; i; i--, itable++ )
				if( itable->id == id ) {
					prev_itab_entry = itable;
					return (char*) self->_vtab + itable->vtab_offset;
				}
		}
	} while( ooc_class_has_parent( type ) );

	return NULL;
}

void *
_ooc_get_interface_must_have( const Object self, InterfaceID id )
{
	void * ifc = _ooc_get_interface( self, id );

	if( ifc == NULL )
		ooc_throw( exception_new( err_interface_not_implemented ) );

	return ifc;
}

void *
_ooc_get_mixin_data( const Object carrier, InterfaceID id )
{
	/* Add some cache functionality to accelerate subsequent calls. Will be useful in mixins. */
	if( ( id != prev_id ) || ( carrier->_vtab != prev_vtab ) )
		_ooc_get_interface_must_have( carrier, id );

	return (char*) carrier + prev_itab_entry->data_offset;
}
