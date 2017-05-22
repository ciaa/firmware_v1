/*
 * implement/exception.h
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


#ifndef	IMPL_EXCEPTION_H
#define IMPL_EXCEPTION_H

#ifndef EXCEPTION_H
#error Do #include "exception.h" right before #include "implement/exception.h"
#endif

/** Exception class data members.
 * These members are protected members, so the derived classes
 * can access them.
 */
 
ClassMembers( Exception, Base )

	int		code;			/**< The error code (used only by the Exception thrown by the ooc core) */
	int     user_code;		/**< Can be used for error codes in derived exception classes.
								 Using user_code can simplify the use of derived classes. */

EndOfClassMembers;

#endif /* IMPL_EXCEPTION_H */
