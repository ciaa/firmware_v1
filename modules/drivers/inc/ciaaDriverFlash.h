/* Copyright 2014, Daniel Cohen
 * Copyright 2014, Esteban Volentini
 * Copyright 2014, Matias Giori
 * Copyright 2014, Franco Salinas
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

#ifndef CIAADRIVERFLASH_H
#define CIAADRIVERFLASH_H
/** \brief CIAA Flash driver header file
 **
 ** This files contains the header file of the CIAA Flash driver
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup Flash Flash Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DC           Daniel Cohen
 * EV           Esteban Volentini
 * MG           Matias Giori
 * FS           Franco Salinas
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150328 v0.0.3 FS   renamed seek to lseek
 * 20150201 v0.0.2 MaCe add seek function
 * 20141006 v0.0.1 EV   first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaSerialDevices.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief open the flash device
 **
 ** \param[in] path path of the device to be opened
 ** \param[in] device device to be opened
 ** \param[in] oflag may take one of the following values:
 **               ciaaPOSIX_O_RDONLY: opens files to read only
 **               ciaaPOSIX_O_WRONLY: opens files to write only
 **               ciaaPOSIX_O_RDWR: opens file to read and write
 ** \return NULL if an error occurs, in other case the address of the opened
 **         device.
 **/
extern ciaaDevices_deviceType * ciaaDriverFlash_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag);

/** \brief close the flash device
 **
 **
 ** \param[in] device pointer to device
 ** \return    -1 if failed, 0 if success.
 **/
extern int32_t ciaaDriverFlash_close(ciaaDevices_deviceType const * const device);

/** \brief controls the flash device
 **
 ** Performs special control of a flash device
 **
 ** \param[in] device pointer to the device
 ** \param[in] request type of the request, depends on the device
 ** \param[in] param
 ** \return    a negative value if failed, a positive value
 **            if success.
 **/
extern int32_t ciaaDriverFlash_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param);

/** \brief read from a flash device
 **
 ** Reads nbyte from the flash device device in buf.
 **
 ** \param[in]  device  pointer to the device to be read
 ** \param[out] buf     buffer to store the read data
 ** \param[in]  nbyte   count of bytes to be read
 ** \return     the count of read bytes is returned
 **/
extern ssize_t ciaaDriverFlash_read(ciaaDevices_deviceType const * const device, uint8_t * const buffer, size_t const size);

/** \brief writes to a flash device
 **
 ** Writes nbyte to the device device from the buffer buf
 **
 ** \param[in]  device  device to be written
 ** \param[in]  buf     buffer with the data to be written
 ** \param[in]  nbyte   count of bytes to be written
 ** \return     the count of bytes written
 **/
extern ssize_t ciaaDriverFlash_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size);

/** \brief seek a flash device
 **
 ** Set the position in the stream
 **
 ** \param[in]  device  device to set the position
 ** \param[in]  offset  offset to the new position
 ** \param[in]  whence  if set to SEEK_SET offset indicates the position from the beginning of the device
                           (offset shall be positive)
                        if set to SEEK_CUR offset indicates the position from the actual position
                           (offset may be positive or negative)
                        if set to SEEK_END offset indicates the position from the end of the device
                           (offset shall be negative)
 ** \return     0 if success and -1 in other case
 **/
extern off_t ciaaDriverFlash_lseek(ciaaDevices_deviceType const * const device, off_t const offset, uint8_t const whence);

/** \brief initialize the flash deriver
 **
 ** Is called at system startup, the driver shall register all available flash
 ** devices.
 **/
extern void ciaaDriverFlash_init(void);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAADRIVERFLASH_H */
