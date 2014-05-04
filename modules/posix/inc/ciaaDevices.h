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

#ifndef _CIAADEVICES_H_
#define _CIAADEVICES_H_
/** \brief CIAA Devices header file
 **
 ** This header file describes the Devices.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
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
#include "ciaaPOSIX_stdint.h"

/*==================[macros]=================================================*/
#define ciaaDEVICES_MAXDEVICES		20

/*==================[typedef]================================================*/
/** \brief open function type */
typedef int32_t (*ciaaDevice_open)(uint8_t const * const path, uint8_t const oflag);

/** \brief close function type */
typedef int32_t (*ciaaDevice_close)(int32_t fildes);

/** \brief ioctl function type */
typedef int32_t (*ciaaDevice_ioctl)(int32_t const fildes, int32_t request, void* param);

/** \brief read function type */
typedef int32_t (*ciaaDevice_read)(int32_t const fildes, uint8_t * const buf, uint32_t nbyte);

/** \brief write function type */
typedef int32_t (*ciaaDevice_write)(int32_t const fildes, uint8_t const * const buf, uint32_t nbyte);

/** \brief Device Type */
typedef struct {
	char const * path; 			/** <- device path, eg. /dev/serlia/UART1 */
	ciaaDevice_open open; 		/** <- pointer to open function */
	ciaaDevice_close close;		/** <- pointer to close function */
	ciaaDevice_read read;		/** <- pointer to read function */
	ciaaDevice_ioctl ioctl;		/** <- pointer to ioctl function */
	ciaaDevice_write write;		/** <- pointer to write function */
} ciaaDevice_deviceType;

/** \brief Devices Status
 **
 ** Describes the possible status of a device
 **
 **/
typedef enum
{
	ciaaDevices_EStatus_Open = 0,    /** < Device is open */
	ciaaDevices_EStatus_Close = 1,   /** < Device is close */
	ciaaDevices_EStatus_Error = 2    /** < Device is open with errors */
} ciaaDevices_Enum_Status;

typedef enum
{
	ciaaDevices_EMode_ReadOnly = 0
} ciaaDevices_Enum_Mode;

/*==================[external data declaration]==============================*/
/** \brief UART1 Device
 **
 **/
char * ciaaDevices_UART1;

/** \brief UART2 Device
 **
 **/
char * ciaaDevices_UART2;

/** \brief I2C1 Device
 **
 **/
char * ciaaDevices_I2C1;


/*==================[external functions declaration]=========================*/
/** \brief Init ciaa Devices
 **
 ** This function performs the initialization of ciaaDevices
 **/
extern void ciaaDevice_init(void);

/** \brief add deivce
 **
 ** Adds the device device
 **
 ** \param[in] device device to be added
 **/
extern void ciaaDevice_addDevice(ciaaDevice_deviceType const * device);

/** \brief get a device
 **
 ** Get the device with the indicated path
 **/
extern ciaaDevice_deviceType const * ciaaDevice_getDevice(char const * const path);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAADEVICES_H_ */

