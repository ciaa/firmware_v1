/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

#ifndef CIAAPOSIX_STDLIB_H
#define CIAAPOSIX_STDLIB_H
/** \brief ciaa POSIX stdlib header file
 **
 ** ciaa POSIX stdlib header file
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DiFe         Diego Fernandez
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140524 v0.0.1 DiFe initial version
 */

/*==================[inclusions]=============================================*/

#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stddef.h"
#include "ciaaPOSIX_stdbool.h"
#include "ciaaPOSIX_semaphore.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
#ifndef NULL
/*@-namechecks@*/
#define NULL ((void*)0)
/*@=namechecks@*/
#endif

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief ciaaPOSIX stdlib init
 **
 ** Performs the initialization of ciaaPOSIX stdlib
 **/
void ciaaPOSIX_stdlib_init(void);

/** \brief ciaaPOSIX malloc
 **
 ** Allocates unused space
 **
 ** \param[in] number of bytes to allocate
 **/
void *ciaaPOSIX_malloc(size_t size);

/** \brief ciaaPOSIX free
 **
 ** Frees allocated memory
 **
 ** \param[in] pointer to a previously allocated region
 **/
void ciaaPOSIX_free(void *);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAAPOSIX_STDLIB_H */

