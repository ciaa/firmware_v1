/* Copyright 2015, Juan Pablo Vecchio, Joaquin Jorge Rodriguez
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

#ifndef _CIAAGPIODEVICES_H_
#define _CIAAGPIODEVICES_H_
/** \brief CIAA General Purpose Input/Output Devices
 **
 ** Provides support for Input Output Devices
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * JPV         Juan Pablo Vecchio
 * JJR         Joaquin Jorge Rodriguez
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150612 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaDevices.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief ciaaGpioDevices initialization
 **
 ** Performs the initialization of the ciaaDioDevices
 **
 **/
extern void ciaaKeyboardDevices_init(void);

/** \brief Open a Keyboard device
 **
 ** Opens a Keyboard device with the path for read/write/readwrite depending on
 ** oflag.
 **
 ** \param[in] path of the device to be opened
 ** \param[in] oflag may take one of the following values:
 **               O_RDONLY: opens files to read only
 **               O_WRONLY: opens files to write only
 **               O_RDWR: opens file to read and write
 ** \return a negative value if failed, a positive value if success.
 **/
extern ciaaDevices_deviceType * ciaaKeyboardDevices_open(char const * path,
      ciaaDevices_deviceType * device,
      uint8_t const oflag);

/** \brief Close a Keyboard device
 **
 ** Closes the Keyboard device with file desciptor fildes
 **
 ** \param[in] device pointer to device
 ** \return    a negative value if failed, a positive value if success.
 **/
extern int32_t ciaaKeyboardDevices_close(
      ciaaDevices_deviceType const * const device);

/** \brief Control a Keyboard device
 **
 ** Performs special control of a Keyboard device
 **
 ** \param[in] device pointer to the device
 ** \param[in] request type of the request, depends on the device
 ** \param[in] param
 ** \return    a negative value if failed, a positive value if success.
 **/
extern int32_t ciaaKeyboardDevices_ioctl(
      ciaaDevices_deviceType const * const device,
      int32_t request, void* param);

/** \brief Reads from Keyboard device
 **
 ** Reads nbyte from the file descriptor fildes and store them in buf.
 **
 ** \param[in]  device  pointer to the device to be read
 ** \param[out] buf     buffer to store the read data
 ** \param[in]  nbyte   count of bytes to be read
 ** \return     the count of read bytes is returned
 **
 **/
extern uint16_t ciaaKeyboardDevices_read(ciaaDevices_deviceType const * const device,
		uint16_t * const buf, size_t const nbyte);

/** \brief Writes to Keyboard device
 **
 ** Writes nbyte to the file descriptor fildes from the buffer buf
 **
 ** \param[in]  device  device to be written
 ** \param[in]  buf     buffer with the data to be written
 ** \param[in]  nbyte   count of bytes to be written
 ** \return     the count of bytes written
 **/
extern uint16_t ciaaKeyboardDevices_write(ciaaDevices_deviceType const * device,
		uint16_t const * const buf, size_t nbyte);

/** \brief add driver
 **
 ** Adds the driver
 **
 ** \param[in] driver driver to be added
 **/
extern void ciaaKeyboardDevices_addDriver(ciaaDevices_deviceType * driver);

/** \brief release driver
 **
 ** Releases a driver
 **/
extern void ciaaKeyboardDevices_releaseDriver(ciaaDevices_deviceType const * driver);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif
