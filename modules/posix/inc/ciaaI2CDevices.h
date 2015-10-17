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

#ifndef CIAAI2CDEVICES_H
#define CIAAI2CDEVICES_H
/** \brief CIAA I2C Devices
 **
 ** Provides support for I2C devices
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
 * 201550402 v0.0.1 MaCe initial version
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
/** \brief ciaaI2CDevices initialization
 **
 ** Performs the initialization of the ciaaI2CDevices
 **
 **/
extern void ciaaI2CDevices_init(void);

/** \brief Open a I2C device
 **
 ** Opens a I2C device with the path for read/write/readwrite depending on
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
extern ciaaDevices_deviceType * ciaaI2CDevices_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag);

/** \brief Close a I2C device
 **
 ** Closes the I2C device with file desciptor fildes
 **
 ** \param[in]  device pointer to device
 ** \return    a negative value if failed, a positive value
 **             if success.
 **/
extern int32_t ciaaI2CDevices_close(ciaaDevices_deviceType const * const device);

/** \brief Control a I2C device
 **
 ** Performs special control of a I2C device
 **
 ** \param[in]  device pointer to the device
 ** \param[in]  request type of the request
 ** \param[in]  param
 ** \return     a negative value if failed, a positive value
 **             if success.
 **/
extern int32_t ciaaI2CDevices_ioctl(ciaaDevices_deviceType const * const device, int32_t request, void* param);

/** \brief Reads from a I2C device
 **
 ** Reads nbyte from the file descriptor fildes and store them in buf.
 **
 ** \param[in]  device  pointer to the device to be read
 ** \param[out] buf     buffer to store the read data
 ** \param[in]  nbyte   count of bytes to be read
 ** \return     the count of read bytes is returned
 **
 **/
extern ssize_t ciaaI2CDevices_read(ciaaDevices_deviceType const * const device, uint8_t * const buf, size_t const nbyte);

/** \brief Writes to a I2C device
 **
 ** Writes nbyte to the file descriptor fildes from the buffer buf
 **
 ** \param[in]  device  device to be written
 ** \param[in]  buf     buffer with the data to be written
 ** \param[in]  nbyte   count of bytes to be written
 ** \return     the count of bytes written
 **/
extern ssize_t ciaaI2CDevices_write(ciaaDevices_deviceType const * device, uint8_t const * const buf, size_t const nbyte);

/** \brief Seek into a i2c device
 **
 ** Set the read/write position to a given offset.
 **
 ** \param[in] fildes   device to seek into
 ** \param[in] offset   depending on the value of whence offset represents:
 **                     offset from the beggining if whence is set to SEEK_SET.
 **                     offset from the end if whence is set to SEEK_END.
 **                     offset from the current position if whence is set to
 **                     SEEK_CUR.
 ** \param[in] whence   SEEK_CUR, SEEK_SET or SEEK_END
 ** \return -1 if failed, a non negative integer representing the count of
 **         written bytes if success
 **/
extern off_t ciaaI2CDevices_lseek(ciaaDevices_deviceType const * const device, off_t const offset, uint8_t const whence);

/** \brief Transmit confirmation of a I2C device
 **
 ** This interface informs the I2C device that a recepction has been completed
 **
 ** \param[in]    TODO fildes file descriptor of the confirmation
 ** \param[in]    nbyte count of received bytes
 **
 ** \remarks This interface may be called from ISR context
 **/
extern void ciaaI2CDevices_txConfirmation(ciaaDevices_deviceType const * const device, uint32_t const nbyte);

/** \brief Receive indication of a I2C device
 **
 ** This interface informs the I2C device that a transmittion has been completed
 **
 ** \param[in]    TODO filedes file descriptor of the received data
 ** \param[in]    nbyte count of transmitted bytes
 **
 ** \remarks This interface may be called from ISR context
 **/
extern void ciaaI2CDevices_rxIndication(ciaaDevices_deviceType const * const device, uint32_t const nbyte);

/** \brief add driver
 **
 ** Adds the driver
 **
 ** \param[in] driver driver to be added
 **/
extern void ciaaI2CDevices_addDriver(ciaaDevices_deviceType * driver);

/** \brief release driver
 **
 ** Rleases a driver
 **/
extern void ciaaI2CDevices_releaseDriver(ciaaDevices_deviceType const * driver);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAAI2CDEVICES_H */

