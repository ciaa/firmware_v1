/* Copyright 2014, Mariano Cerdeiro
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

#ifndef _CIAADRIVERUART_H_
#define _CIAADRIVERUART_H_
/** \brief CIAA Uart driver header file
 **
 ** This files contains the header file of the CIAA UART driver
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup UART UART Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140420 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaSerialDevices.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief open the uart device
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
extern ciaaDevices_deviceType * ciaaDriverUart_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag);

/** \brief close the uart device
 **
 **
 ** \param[in] device pointer to device
 ** \return    -1 if failed, 0 if success.
 **/
extern int32_t ciaaDriverUart_close(ciaaDevices_deviceType const * const device);

/** \brief controls the uart device
 **
 ** Performs special control of a uart device
 **
 ** \param[in] device pointer to the device
 ** \param[in] request type of the request, depends on the device
 ** \param[in] param
 ** \return    a negative value if failed, a positive value
 **            if success.
 **/
extern int32_t ciaaDriverUart_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param);

/** \brief read from a uart device
 **
 ** Reads nbyte from the uart device device in buf.
 **
 ** \param[in]  device  pointer to the device to be read
 ** \param[out] buf     buffer to store the read data
 ** \param[in]  nbyte   count of bytes to be read
 ** \return     the count of read bytes is returned
 **/
extern ssize_t ciaaDriverUart_read(ciaaDevices_deviceType const * const device, uint8_t * const buffer, size_t const size);

/** \brief writes to a uart device
 **
 ** Writes nbyte to the device device from the buffer buf
 **
 ** \param[in]  device  device to be written
 ** \param[in]  buf     buffer with the data to be written
 ** \param[in]  nbyte   count of bytes to be written
 ** \return     the count of bytes written
 **/
extern ssize_t ciaaDriverUart_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size);

/** \brief initialize the uart deriver
 **
 ** Is called at system startup, the driver shall register all available uart
 ** devices.
 **/
extern void ciaaDriverUart_init(void);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAADRIVERUART_H_ */

