/* Copyright 2008, 2009, Mariano Cerdeiro
 *
 * This file is part of FreeOSEK.
 *
 * FreeOSEK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Linking FreeOSEK statically or dynamically with other modules is making a
 * combined work based on FreeOSEK. Thus, the terms and conditions of the GNU
 * General Public License cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of FreeOSEK give
 * you permission to combine FreeOSEK program with free software programs or
 * libraries that are released under the GNU LGPL and with independent modules
 * that communicate with FreeOSEK solely through the FreeOSEK defined interface.
 * You may copy and distribute such a system following the terms of the GNU GPL
 * for FreeOSEK and the licenses of the other code concerned, provided that you
 * include the source code of that other code when and as the GNU GPL requires
 * distribution of source code.
 *
 * Note that people who make modified versions of FreeOSEK are not obligated to
 * grant this special exception for their modified versions; it is their choice
 * whether to do so. The GNU General Public License gives permission to release
 * a modified version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this exception.
 *
 * FreeOSEK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FreeOSEK. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _TYPES_ARCH_H_
#define _TYPES_ARCH_H_
/** \brief FreeOSEK Gen Architecture Types definition
 **
 ** \file posix/Types_Arch.h
 ** \arch posix
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Gen
 ** @{ */
/** \addtogroup FreeOSEK_Gen_Global
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe                  Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * v0.1.0 20090129 MaCe initial version
 * v0.1.1 20140503 MaCe add support for 64 bits
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
#ifndef TYPES_NULL
#define TYPES_NULL
/** \brief NULL definition */
#define NULL ((void *)0)
#else
#error NULL defined before
#endif

/*==================[typedef]================================================*/
#ifndef TYPES_BOOLEAN
#define TYPES_BOOLEAN
/** \brief boolean type type definition */
typedef unsigned char boolean;
#else
#error boolean defined before
#endif

#ifndef TYPES_UINT8
#define TYPES_UINT8
/** \brief usigned 8 bits integer type definition */
typedef unsigned char uint8;
#else
#error uint8 defined before
#endif

#ifndef TYPES_SINT8
#define TYPES_SINT8
/** \brief signed 8 bits integer type definition */
typedef signed char sint8;
#else
#error sint8 defined before
#endif

#ifndef TYPES_UINT8F
#define TYPES_UINT8F
/** \brief unsigned 8 bits fast integer type definition */
typedef unsigned int uint8f;
#else
#error uint8f defined before
#endif

#ifndef TYPES_SINT8F
#define TYPES_SINT8F
/** \brief signed 8 bits fast integer type definition */
typedef signed int sint8f;
#else
#error sint8f defined before
#endif

#ifndef TYPES_UINT16
#define TYPES_UINT16
/** \brief unsigned 16 bits integer type definition */
typedef unsigned short uint16;
#else
#error uint16 defined before
#endif

#ifndef TYPES_SINT16
#define TYPES_SINT16
/** \brief signed 16 bits integer type definition */
typedef signed short sint16;
#else
#error sint16 defined before
#endif

#ifndef TYPES_UINT16F
#define TYPES_UINT16F
/** \brief unsigned 16 bits fast integer type definition */
typedef unsigned int uint16f;
#else
#error uint16f defined before
#endif

#ifndef TYPES_SINT16F
#define TYPES_SINT16F
/** \brief signed 16 bits fast integer type definition */
typedef signed int sint16f;
#else
#error uint16f defined before
#endif

#ifndef TYPES_UINT32
#define TYPES_UINT32
/** \brief unsigned 32 bits integer type definition */
typedef unsigned int uint32;
#else
#error uint32 defined before
#endif

#ifndef TYPES_SINT32
#define TYPES_SINT32
/** \brief signed 32 bits integer type definition */
typedef signed int sint32;
#else
#error sint32 defined before
#endif

#ifndef TYPES_UINT32F
#define TYPES_UINT32F
/** \brief unsigned 32 bits fast integer type definition */
typedef unsigned int uint32f;
#else
#error uint32f defined before
#endif

#ifndef TYPES_SINT32F
#define TYPES_SINT32F
/** \brief signed 32 bits fast integer type definition */
typedef signed int sint32f;
#else
#error sint32 defined before
#endif

#ifndef TYPES_UINT64
#define TYPES_UINT64
/** \brief unsigned 64 bits integer type definition */
typedef unsigned long long uint64;
#else
#error uint64 defined before
#endif

#ifndef TYPES_SINT64
#define TYPES_SINT64
/** \brief signed 64 bits integer type definition */
typedef signed long long sint64;
#else
#error sint64 defined before
#endif

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _TYPES_ARCH_H_ */

