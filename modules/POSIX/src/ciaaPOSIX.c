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
 * 20140420 v0.0.1 EzEs initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX.h"
#include "string.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
ciaaPOSIX_TDeviceList ciaaPOSIX_devices;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
int32_t ciaaPOSIX_open (const char* pathName, int32_t flags)
{
	// TODO Search the device's status flag in ciaaPOSIX_devices list

	// TODO Use GenericList to create new node and get the pointer

	if (strcmp(pathName, "/dev/UART/UART1"))
	{
		// TODO Add data to new node
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].fd = ciaaPOSIX_requestedDevices;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].status = ciaaPOSIX_EStatus_Open;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pOpen = ciaaPOSIX_open;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pClose = ciaaUART_close;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pIoctl = ciaaUART_ioctl;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pRead = ciaaUART_read;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pWrite = ciaaUART_write;

		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pOpen (pathName, flags);
	} else if (strcmp(pathName, "/dev/UART/UART2")) {

	} else if (strcmp(pathName, "/dev/I2C/I2C1")) {

	}

	// TODO return ciaaPOSIX_devices size (file descriptor)
}

int32_t ciaaPOSIX_close (int32_t fd)
{
	// TODO search in ciaaPOSIX_devices by fd
	// TODO close the device
}

int32_t ciaaPOSIX_ioctl (int32_t fd, int32_t arg, void* param)
{

}

int32_t ciaaPOSIX_read (int32_t fd, uint8_t* buffer, uint32_t size)
{

}

int32_t ciaaPOSIX_write (int32_t fd, uint8_t* buffer, uint32_t size)
{

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

