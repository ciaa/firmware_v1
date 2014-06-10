/* Copyright 2008, 2009 Mariano Cerdeiro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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

