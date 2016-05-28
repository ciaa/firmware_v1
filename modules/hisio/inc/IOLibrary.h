/* Copyright 2015, Mariano Cerdeiro
 * All rights reserved.
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

#ifndef IOLIBRARY_H
#define IOLIBRARY_H
/** \brief Short description of this file
 **
 ** Long description of this file
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Template Template to start a new module
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * yyyymmdd v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Possible return values of IO_ErrorType
 **
 ** Values between 0 and 15 are reserved for the IO Library, between 16 and 63
 ** for the IO Drivers and between 64 and 127 are implementation specific.
 ** Values above 127 are reserved for future implementations.
 **
 **/
#define IO_E_OK               0
#define IO_E_BUSY             1
#define IO_E_UNKOWN_MODE      2

/*==================[typedef]================================================*/
typedef uint8_t IO_ErrorType;

typedef uint32_t IO_SymbolicName;

typedef uint32_t IO_SizeType;

typedef uint8_t IO_ModeType;

typedef uint8_t * IO_MemPtrType;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief
 **/
extern IO_ErrorType IO_InitSync(IO_SymbolicName device, void * address);

extern IO_ErrorType IO_DeInitSync(IO_SymbolicName device, void * address);

extern IO_ErrorType IO_IoctlSync(IO_SymbolicName device, IO_ModeType mode,
      void * address);

extern IO_ErrorType IO_ReadSync(IO_SymbolicName device, IO_MemPtrType address,
      IO_SizeType size);

extern IO_ErrorType IO_WriteSync(IO_SymbolicName device, IO_MemPtrType address,
      IO_SizeType size);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef IOLIBRARY_H*/

