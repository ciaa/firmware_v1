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

#ifndef _CIAAPOSIX_H_
#define _CIAAPOSIX_H_
/** \brief CIAA Posix header file
 **
 ** This header is the Posix header file.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX Posix
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * EzEs         Ezequiel Esposito
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140422 v0.0.2 EzEs initial version
 * 20140420 v0.0.1 EzEs initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDevices.h"
#include "ciaaMemory.h"
#include "ciaaErrorsCodeSystem.h"
#include "ciaaMessagesCodeSystem.h"

#include "ciaaUART.h"

/*==================[macros]=================================================*/
/** \brief Max devices available
 **/
#define ciaaPOSIX_MACRO_MaxDevices 				100

/*==================[typedef]================================================*/
/** \brief TODO
 **
 **/
typedef struct
{
	int32_t fd;
	char const * name;
	ciaaDevices_Enum_Status status;
	int32_t (*pOpen) (const char* pathName, int32_t flags);
	int32_t (*pClose) (int32_t fd);
	int32_t (*pIoctl) (int32_t fd, int32_t arg, uint32_t size);
	int32_t (*pRead) (int32_t fd, uint8_t* buffer, uint32_t size);
	int32_t (*pWrite) (int32_t fd, uint8_t* buffer, uint32_t size);
	void* data;
} ciaaPOSIX_Type_Base;

/** \brief ciaaPOSIX errors enum
 ** Minimum allowed number: -1
 ** Maximum allowed number: -1000
 **/
typedef enum
{
	ciaaPOSIX_Enum_Errors_DeviceAlreadyOpen = ciaaPOSIX_MACRO_MinErrorCode,
	ciaaPOSIX_Enum_Errors_DeviceNotAllocated = ciaaPOSIX_MACRO_MinErrorCode - 1,
	ciaaPOSIX_Enum_Errors_BadFileDescriptor = ciaaPOSIX_MACRO_MinErrorCode - 2
}; ciaaPOSIX_Enum_Errors;

/** \brief ciaaPOSIX return message codes
 ** Minimum allowed number: 1
 ** Maximum allowed number: 1000
 **/
typedef enum
{
	ciaaPOSIX_Enum_Messages_Example = ciaaPOSIX_MACRO_MinMessageCode,
}; ciaaPOSIX_Enum_Messages;

/*==================[external data declaration]==============================*/
/** \brief List of posix devices
 **
 **/
extern ciaaPOSIX_Type_Base* ciaaPOSIX_devicesArray [];
extern uint32_t ciaaPOSIX_devicesArraySize;

/*==================[external functions declaration]=========================*/
extern void ciaa_init ();
extern int32_t ciaa_open (const char* pathName, int32_t flags);
extern int32_t ciaa_close (int32_t fd);
extern int32_t ciaa_ioctl (int32_t fd, int32_t arg, void* param);
extern int32_t ciaa_read (int32_t fd, uint8_t* buffer, uint32_t size);
extern int32_t ciaa_write (int32_t fd, uint8_t* buffer, uint32_t size);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAPOSIX_H_ */

