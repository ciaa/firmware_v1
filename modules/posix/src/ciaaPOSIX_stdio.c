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
#include "ciaak.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_stdlib.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
ciaaPOSIX_Type_Base* ciaaPOSIX_devicesArray [ciaaPOSIX_MAX_DEVICES];

uint32_t ciaaPOSIX_devicesArraySize;



/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void ciaaPOSIX_init(void)
{
	uint32_t i;
	/* init all posix devices */
	for (i = 0; i < ciaaPOSIX_MAX_DEVICES; i++) {
		ciaaPOSIX_devicesArray[i] = NULL;
   }

   /* set first device */
	ciaaPOSIX_devicesArraySize = 0;
}

extern int32_t ciaaPOSIX_open(char const * const path, uint8_t const oflag)
{
	return 0;
}

int32_t ciaaPOSIX_close(int32_t fildes)
{
	if (fildes >= 0) {
		if (ciaaPOSIX_devicesArray[fildes] != NULL) {
			return ciaaPOSIX_devicesArray[fildes]->pClose (fildes);
		} else {
			return ciaaPOSIX_Enum_Errors_DeviceNotAllocated;
		}
	} else {
		return ciaaPOSIX_Enum_Errors_BadFileDescriptor;
	}
}

int32_t ciaaPOSIX_ioctl (int32_t fd, int32_t arg, void* param)
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

int32_t ciaaPOSIX_read (int32_t fd, uint8_t* buffer, uint32_t size)
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

extern int32_t ciaaPOSIX_write (int32_t const fildes, uint8_t const * const buf, uint32_t nbyte)
{
	if (fildes >= 0) {
		if (ciaaPOSIX_devicesArray[fildes] != NULL) {
			return ciaaPOSIX_devicesArray[fildes]->pWrite (fildes, buf, nbyte);
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

