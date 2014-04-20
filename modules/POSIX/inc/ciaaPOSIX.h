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
 * 20140420 v0.0.1 EzEs initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDevices.h"
#include "ciaaMemory.h"

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief TODO
 **
 **/
typedef struct
{
	int32_t fd;
	ciaaDevices_EStatus status;
	int32_t (*pOpen) (const char* pathName, int32_t flags);
	int32_t (*pClose) (int32_t fd);
	int32_t (*pIoctl) (int32_t fd, int32_t arg, uint32_t size);
	int32_t (*pRead) (int32_t fd, uint8_t* buffer, uint32_t size);
	int32_t (*pWrite) (int32_t fd, uint8_t* buffer, uint32_t size);
	void* data;
} ciaaPOSIX_TBase;

// TODO implement a generic list container
// TODO move list definition and methods to System/Resources/ciaaGenericList.h
struct ciaaPOSIX_Type_DeviceNode
{
	struct ciaaPOSIX_Type_DeviceNode* pNext;
	ciaaPOSIX_TBase* pData;
};
typedef struct ciaaPOSIX_TDeviceNode;

struct ciaaPOSIX_Type_DeviceList
{
	ciaaPOSIX_Type_DeviceNode* pFirs;
	uint32_t size;
};
typedef struct ciaaPOSIX_Type_DeviceList ciaaPOSIX_TDeviceList;


/*==================[external data declaration]==============================*/
/** \brief List of posix devices
 **
 **/
extern ciaaPOSIX_TDeviceList ciaaPOSIX_devices;

/*==================[external functions declaration]=========================*/
extern int32_t ciaaPOSIX_open (const char* pathName, int32_t flags);
extern int32_t ciaaPOSIX_close (int32_t fd);
extern int32_t ciaaPOSIX_ioctl (int32_t fd, int32_t arg, void* param);
extern int32_t ciaaPOSIX_read (int32_t fd, uint8_t* buffer, uint32_t size);
extern int32_t ciaaPOSIX_write (int32_t fd, uint8_t* buffer, uint32_t size);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAPOSIX_H_ */

