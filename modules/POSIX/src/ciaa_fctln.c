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

/** \brief CIAA POSIX source file
 **
 ** This file contains the POSIX implementation
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Posix Posix
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
#include "kciaa.h"
#include "ciaa_fctln.h"
#include "string.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
ciaaPOSIX_Type_Base* ciaaPOSIX_devicesArray [ciaaPOSIX_MACRO_MaxDevices];
uint32_t ciaaPOSIX_devicesArraySize;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void ciaa_fctlnInit ()
{
	uint32_t i;
	for (i = 0; i < ciaaPOSIX_MACRO_MaxDevices; i++)
		ciaaPOSIX_devicesArray[i] = NULL;

	ciaaPOSIX_devicesArraySize = 0;
}

int32_t ciaa_open (const char* pathName, int32_t flags)
{
	uint32_t i;
	uint32_t openCode;

	// Search if the device is already open
	// TODO add a found flag
	for (i = 0; i < ciaaPOSIX_devicesArraySize; i++)
	{
		if (strcmp(ciaaPOSIX_devicesArray[i]->name, pathName))
		{
			if ( kciaa_atomicTrySetAndCheck(ciaaPOSIX_devicesArray[i]->status, ciaaDevices_EStatus_Close))
			{
				return ciaaPOSIX_Enum_Errors_DeviceAlreadyOpen;
			}
		}
	}


	// TODO check found flag
	// Crate memory for the new device
	ciaaPOSIX_devicesArray[ciaaPOSIX_devicesArraySize] = kciaa_malloc (sizeof(ciaaPOSIX_Type_Base));

	// Load device pointer functions, data and open it
	if (strcmp(pathName, ciaaDevices_UART1)) {
		ciaaPOSIX_devicesArray[ciaaPOSIX_devicesArraySize]->fd = ciaaPOSIX_devicesArraySize;
		strcpy(ciaaPOSIX_devicesArray[ciaaPOSIX_devicesArraySize]->name, ciaaDevices_UART1);
		ciaaPOSIX_devicesArray[ciaaPOSIX_devicesArraySize]->pOpen = ciaaUART_open;
		ciaaPOSIX_devicesArray[ciaaPOSIX_devicesArraySize]->pClose = ciaaUART_close;
		ciaaPOSIX_devicesArray[ciaaPOSIX_devicesArraySize]->pIoctl = ciaaUART_ioctl;
		ciaaPOSIX_devicesArray[ciaaPOSIX_devicesArraySize]->pRead = ciaaUART_read;
		ciaaPOSIX_devicesArray[ciaaPOSIX_devicesArraySize]->pWrite = ciaaUART_write;

		openCode = ciaaPOSIX_devicesArray[ciaaPOSIX_devicesArraySize]->pOpen (pathName, flags);
		if (openCode > 0)
			ciaaPOSIX_devicesArray[ciaaPOSIX_devicesArraySize]->status = ciaaDevices_EStatus_Open;
		else
			ciaaPOSIX_devicesArray[ciaaPOSIX_devicesArraySize]->status = ciaaDevices_EStatus_Error;
	} else if (strcmp(pathName, "/dev/UART/UART2")) {
		// TODO
	} else if (strcmp(pathName, "/dev/I2C/I2C1")) {
		// TODO
	}

	// if error release memory and do not increment device array

	ciaaPOSIX_devicesArraySize++;

	return openCode;
}

int32_t ciaa_close (int32_t fd)
{
	if (fd >= 0) {
		if (ciaaPOSIX_devicesArray[fd] != NULL) {
			return ciaaPOSIX_devicesArray[fd]->pClose (fd);
		} else {
			return ciaaPOSIX_Enum_Errors_DeviceNotAllocated;
		}
	} else {
		return ciaaPOSIX_Enum_Errors_BadFileDescriptor;
	}
}

int32_t ciaa_ioctl (int32_t fd, int32_t arg, void* param)
{
	if (fd >= 0) {
		if (ciaaPOSIX_devicesArray[fd] != NULL) {
			return ciaaPOSIX_devicesArray[fd]->pIoctl (fd, arg, param);
		} else {
			return ciaaPOSIX_Enum_Errors_DeviceNotAllocated;
		}
	} else {
		return ciaaPOSIX_Enum_Errors_BadFileDescriptor;
	}
}

int32_t ciaa_read (int32_t fd, uint8_t* buffer, uint32_t size)
{
	if (fd >= 0) {
		if (ciaaPOSIX_devicesArray[fd] != NULL) {
			return ciaaPOSIX_devicesArray[fd]->pRead (fd, buffer, size);
		} else {
			return ciaaPOSIX_Enum_Errors_DeviceNotAllocated;
		}
	} else {
		return ciaaPOSIX_Enum_Errors_BadFileDescriptor;
	}
}

int32_t ciaa_write (int32_t fd, uint8_t* buffer, uint32_t size)
{
	if (fd >= 0) {
		if (ciaaPOSIX_devicesArray[fd] != NULL) {
			return ciaaPOSIX_devicesArray[fd]->pWrite (fd, buffer, size);
		} else {
			return ciaaPOSIX_Enum_Errors_DeviceNotAllocated;
		}
	} else {
		return ciaaPOSIX_Enum_Errors_BadFileDescriptor;
	}
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

