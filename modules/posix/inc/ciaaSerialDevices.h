/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * Copyright 2015, Mariano Cerdeiro
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

#ifndef CIAASERIALDEVICES_H
#define CIAASERIALDEVICES_H
/** \brief CIAA Serial Devices
 **
 ** Provides support for serial devices
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140525 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaDevices.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
#define CIAASERIALDEVICES_USERBUF   0x01

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief ciaaSerialDevices initialization
 **
 ** Performs the initialization of the ciaaSerialDevices
 **
 **/
extern void ciaaSerialDevices_init(void);

/** \brief Open a serial device
 **
 ** Opens a serial device with the path for read/write/readwrite depending on
 ** oflag.
 **
 ** \param[in] path path of the device to be opened
 ** \param[in] device device to be opened
 ** \param[in] oflag may take one of the following values:
 **               O_RDONLY: opens files to read only
 **               O_WRONLY: opens files to write only
 **               O_RDWR: opens file to read and write
 ** \return NULL if an error occurs, in other case the address of the opened
 **         device.
 **/
extern ciaaDevices_deviceType * ciaaSerialDevices_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag);

/** \brief Close a serial device
 **
 ** Closes the serial device with file desciptor fildes
 **
 ** \param[in]  device pointer to device
 ** \return    a negative value if failed, a positive value
 **             if success.
 **/
extern int32_t ciaaSerialDevices_close(ciaaDevices_deviceType const * const device);

/** \brief Control a serial device
 **
 ** Performs special control of a serial device
 **
 ** \param[in]  device pointer to the device
 ** \param[in]  request type of the request, following values are accepted:
 **               ciaaPOSIX_IOCTL_GET_RX_COUNT
 **                  param shall be an uint32_t*
 **                  the count of bytes in the RX circular buffer are returned
 **               ciaaPOSIX_IOCTL_GET_TX_SPACE
 **                  param shall be an uint32_t*
 **                  the count free of bytes in the TX circular buffer are returned
 **               other values see serial device driver
 ** \param[in]  param
 ** \return     a negative value if failed, a positive value
 **             if success.
 **/
extern int32_t ciaaSerialDevices_ioctl(ciaaDevices_deviceType const * const device, int32_t request, void* param);

/** \brief Reads from a serial device
 **
 ** Reads nbyte from the file descriptor fildes and store them in buf.
 **
 ** \param[in]  device  pointer to the device to be read
 ** \param[out] buf     buffer to store the read data
 ** \param[in]  nbyte   count of bytes to be read
 ** \return     the count of read bytes is returned
 **
 **/
extern ssize_t ciaaSerialDevices_read(ciaaDevices_deviceType const * const device, uint8_t * const buf, size_t const nbyte);

/** \brief Writes to a serial device
 **
 ** Writes nbyte to the file descriptor fildes from the buffer buf
 **
 ** \param[in]  device  device to be written
 ** \param[in]  buf     buffer with the data to be written
 ** \param[in]  nbyte   count of bytes to be written
 ** \return     the count of bytes written
 **/
extern ssize_t ciaaSerialDevices_write(ciaaDevices_deviceType const * device, uint8_t const * const buf, size_t const nbyte);

/** \brief Transmit confirmation of a serial device
 **
 ** This interface informs the serial device that a recepction has been completed
 **
 ** \param[in]    TODO fildes file descriptor of the confirmation
 ** \param[in]    nbyte count of received bytes
 **
 ** \remarks This interface may be called from ISR context
 **/
extern void ciaaSerialDevices_txConfirmation(ciaaDevices_deviceType const * const device, uint32_t const nbyte);

/** \brief Receive indication of a serial device
 **
 ** This interface informs the serial device that a transmittion has been completed
 **
 ** \param[in]    TODO filedes file descriptor of the received data
 ** \param[in]    nbyte count of transmitted bytes
 **
 ** \remarks This interface may be called from ISR context
 **/
extern void ciaaSerialDevices_rxIndication(ciaaDevices_deviceType const * const device, uint32_t const nbyte);

/** \brief add driver
 **
 ** Adds the driver
 **
 ** \param[in] driver driver to be added
 **/
extern void ciaaSerialDevices_addDriver(ciaaDevices_deviceType * driver);

/** \brief release driver
 **
 ** Rleases a driver
 **/
extern void ciaaSerialDevices_releaseDriver(ciaaDevices_deviceType const * driver);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAASERIALDEVICES_H */

