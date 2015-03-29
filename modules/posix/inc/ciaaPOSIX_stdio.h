/* Copyright 2014, 2015, Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi (UTN-FRBA)
 * Copyright 2014, Juan Cecconi
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

#ifndef CIAAPOSIX_STDIO_H
#define CIAAPOSIX_STDIO_H
/** \brief ciaa POSIX stdio header file
 **
 ** ciaa POSIX stdio header file
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
 * MaCe         Mariano Cerdeiro
 * PaRi         Pablo Ridolfi
 * JuCe         Juan Cecconi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150221 v0.0.3 MaCe separeate ioctl macros in multiple files
 * 20140528 v0.0.2 MaCe implement printf
 * 20140420 v0.0.1 EzEs initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDevices.h"
#include "ciaaMemory.h"
#include "ciaaPOSIX_stddef.h"
#include "ciaaPOSIX_ioctl_serial.h"
#include "ciaaPOSIX_ioctl_block.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Max count of file descriptors
 **/
#define ciaaPOSIX_stdio_MAXFILDES      20

/** \brief Open for read only */
#define O_RDONLY            0x0000

/** \brief Open to write only */
#define O_WRONLY            0x0001

/** \brief Open to read write */
#define O_RDWR              0x0002

/** \brief Non blocking interface */
#define O_NONBLOCK          0x0004

/** \brief set channel for analogic input/output
 **
 ** This ioctl command is used to set the channel for any analogic input/output.
 ** Possible values for arg are:
 **   ciaaCHANNEL_0
 **   ciaaCHANNEL_1
 **   ciaaCHANNEL_2
 **   ciaaCHANNEL_3
 **
 ** Returned none
 **/
#define ciaaPOSIX_IOCTL_SET_CHANNEL                    10

/** \brief channel macros for input/output macros for analogic devices
 **/
#define ciaaCHANNEL_0        0
#define ciaaCHANNEL_1        1
#define ciaaCHANNEL_2        2
#define ciaaCHANNEL_3        3

/** \brief set resolution for analogic input/output
 **
 **/
#define ciaaPOSIX_IOCTL_SET_SAMPLE_RATE                11

/** \brief set resolution for analogic input device
 **
 **/
#define ciaaPOSIX_IOCTL_SET_RESOLUTION                 12

/** \brief resolution macros for input/output macros for analogic input device
 **/
#define ciaaRESOLUTION_10BITS       0
#define ciaaRESOLUTION_9BITS        1
#define ciaaRESOLUTION_8BITS        2
#define ciaaRESOLUTION_7BITS        3
#define ciaaRESOLUTION_6BITS        4
#define ciaaRESOLUTION_5BITS        5
#define ciaaRESOLUTION_4BITS        6
#define ciaaRESOLUTION_3BITS        7

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief ciaaPOSIX Initialization
 **
 ** Performs the initialization of the ciaaPOSIX
 **
 **/
extern void ciaaPOSIX_init(void);

/** \brief Open a file
 **
 ** Opens a file or device path for read/write/readwrite depending on oflag.
 **
 ** \param[in] path  path of the device to be opened
 ** \param[in] oflag may take one of the following values:
 **               O_RDONLY: opens files to read only
 **               O_WRONLY: opens files to write only
 **               O_RDWR: opens file to read and write
 ** \return -1 if failed, a non negative integer representing the file
 **         descriptor if success.
 **
 ** \remarks Opening twice the same path will provide two different file
 **          descriptors. Accessing them with ciaaPOISX_close, ciaaPOSIX_ioctl,
 **          ciaaPOSIX_read, ciaaPOSIX_write, ciaaPOSIX_seek may produce
 **          unexpected behaviors.
 **/
extern int32_t ciaaPOSIX_open(char const * path, uint8_t oflag);

/** \brief Close a file descriptor
 **
 ** Closes the file descriptor fildes
 **
 ** \param[in] fildes file descriptor to be closed
 ** \return    -1 if failed, 0 in other if success.
 **
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read,
 **          ciaaPOSIX_write and ciaaPOSIX_lseek may be called reentrant but with
 **          different file descriptor. If one of this function is called with a
 **          specific file descriptor the caller has to wait until return before
 **          calling other of this function using the same file handler.
 **/
extern int32_t ciaaPOSIX_close(int32_t fildes);

/** \brief Control a stream device
 **
 ** Performs special control of a stream device
 **
 ** \param[in] fildes  file descriptor to be controled
 ** \param[in] request type of the request, depends on the device
 ** \param[in] param   parameter for io control
 ** \return     -1 if failed, != -1 if success
 **
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read,
 **          ciaaPOSIX_write and ciaaPOSIX_lseek may be called reentrant but with
 **          different file descriptor. If one of this function is called with a
 **          specific file descriptor the caller has to wait until return before
 **          calling other of this function using the same file handler.
 **/
extern int32_t ciaaPOSIX_ioctl(int32_t fildes, int32_t request, void* param);

/** \brief Reads from a file descriptor
 **
 ** Reads nbyte from the file descriptor fildes and store them in buf.
 **
 ** \param[in]  fildes  file descriptor to read from
 ** \param[out] buf     buffer to store the read data
 ** \param[in]  nbyte   count of bytes to be read
 ** \return -1 if failed, a non negative integer representing the count of
 **         read bytes if success
 **
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read,
 **          ciaaPOSIX_write and ciaaPOSIX_lseek may be called reentrant but with
 **          different file descriptor. If one of this function is called with a
 **          specific file descriptor the caller has to wait until return before
 **          calling other of this function using the same file handler.
 **/
extern ssize_t ciaaPOSIX_read(int32_t fildes, void * buf, size_t nbyte);

/** \brief Writes to a file descriptor
 **
 ** Writes nbyte to the file descriptor fildes from the buffer buf. If used to
 ** transfer data over a device a successul completion does not guarantee the
 ** correct delivery of the message.
 **
 ** \param[in] fildes   file descriptor to write to
 ** \param[in] buf      buffer with the data to be written
 ** \param[in] nbyte    count of bytes to be written
 ** \return -1 if failed, a non negative integer representing the count of
 **         written bytes if success
 **
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read,
 **          ciaaPOSIX_write and ciaaPOSIX_lseek may be called reentrant but with
 **          different file descriptor. If one of this function is called with a
 **          specific file descriptor the caller has to wait until return before
 **          calling other of this function using the same file handler.
 **/
extern ssize_t ciaaPOSIX_write(int32_t fildes, void const * buf, size_t nbyte);

/** \brief Seek into a file descriptor
 **
 ** Set the read/write position to a given offset.
 **
 ** \param[in] fildes   file descriptor to set the position
 ** \param[in] offset   depending on the value of whence offset represents:
 **                     offset from the beggining if whence is set to SEEK_SET.
 **                     offset from the end if whence is set to SEEK_END.
 **                     offset from the current position if whence is set to
 **                     SEEK_CUR.
 ** \param[in] whence   SEEK_CUR, SEEK_SET or SEEK_END
 ** \return -1 if failed, a non negative integer representing the count of
 **         written bytes if success
 **
 ** \remarks Setting offset to a positive value and whence to SEEK_END will
 **          return -1.
 **          Setting offset to a negative value and whence to SEEK_SET will
 **          return -1.
 **
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read,
 **          ciaaPOSIX_write and ciaaPOSIX_lseek may be called reentrant but with
 **          different file descriptor. If one of this function is called with a
 **          specific file descriptor the caller has to wait until return before
 **          calling other of this function using the same file handler.
 **/
extern off_t ciaaPOSIX_lseek(int32_t fildes, off_t offset, uint8_t whence);

/** \brief print formated output
 **
 ** In Windows and posix this interface calls the system printf, in the CIAA HW
 ** calling this function has no effects
 **
 ** \param[in] format
 **
 ** \return a negative value is returned if failed, a non negative integer
 **         representing the count of transmitted bytes if success.
 **/
extern int32_t ciaaPOSIX_printf(const char * format, ...);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAAPOSIX_STDIO_H */

