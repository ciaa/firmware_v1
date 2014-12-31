/* Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi (UTN-FRBA)
 * Copyright 2014, Juan Cecconi
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
 * 20140528 v0.0.2 MaCe implement printf
 * 20140420 v0.0.1 EzEs initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDevices.h"
#include "ciaaMemory.h"
#include "ciaaErrorsCodeSystem.h"
#include "ciaaMessagesCodeSystem.h"
#include "ciaaPOSIX_stddef.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Max count of file descriptors
 **/
#define ciaaPOSIX_stdio_MAXFILDES      20

/** \brief Open for read only */
#define O_RDONLY            1

/** \brief Open to write only */
#define O_WRONLY            2

/** \brief Open to read write */
#define O_RDWR              4

/** \brief Non blocking interface */
#define O_NONBLOCK          8

/** \brief request to configure ioctl with rxindication
 **
 ** if ioctl is called with this request and param is != NULL the device
 ** will be configured non blocking wiht rx indication.
 **
 **/
#define ciaaPOSIX_IOCTL_RXINDICATION            1

/** \brief start transmission if not already on going
 **
 ** This ioctl command can be used to force the transmission of data. If the
 ** transmission is already on going the call hay no effects.
 **
 **/
#define ciaaPOSIX_IOCTL_STARTTX                 2

/** \brief get current number of bytes stored in rx buffer
 **
 ** This ioctl command is used to see how many bytes are currently in the
 ** receive queue, in order to avoid task blocking when calling read.
 **
 **/
#define ciaaPOSIX_IOCTL_GET_RX_COUNT            3

/** \brief get current space left in tx buffer (in bytes)
 **
 ** This ioctl command is used to see how many bytes can be written in the
 ** transmit queue without being blocked by calling write
 **
 **/
#define ciaaPOSIX_IOCTL_GET_TX_SPACE            4

/** \brief set baudrate for serial devices
 **
 ** This ioctl command is used to set the baudrate for any serial device.
 ** Possible values for arg are:
 **   ciaaBAUDRATE_300
 **   ciaaBAUDRATE_600
 **   ciaaBAUDRATE_1200
 **   ciaaBAUDRATE_1800
 **   ciaaBAUDRATE_2400
 **   ciaaBAUDRATE_4800
 **   ciaaBAUDRATE_9600
 **   ciaaBAUDRATE_14400
 **   ciaaBAUDRATE_19200
 **   ciaaBAUDRATE_38400
 **   ciaaBAUDRATE_57600
 **   ciaaBAUDRATE_115200
 **   ciaaBAUDRATE_230400
 **   ciaaBAUDRATE_460800
 **   ciaaBAUDRATE_921600
 **
 ** Returned value for ioctl is the actual baud rate, or 0 if no rate can be found.
 **/
#define ciaaPOSIX_IOCTL_SET_BAUDRATE            5

/** \brief baudrate macros for serial devices
 **/
#define ciaaBAUDRATE_300     ( 300     )
#define ciaaBAUDRATE_600     ( 600     )
#define ciaaBAUDRATE_1200    ( 1200    )
#define ciaaBAUDRATE_1800    ( 1800    )
#define ciaaBAUDRATE_2400    ( 2400    )
#define ciaaBAUDRATE_4800    ( 4800    )
#define ciaaBAUDRATE_9600    ( 9600    )
#define ciaaBAUDRATE_14400   ( 14400   )
#define ciaaBAUDRATE_19200   ( 19200   )
#define ciaaBAUDRATE_38400   ( 38400   )
#define ciaaBAUDRATE_57600   ( 57600U  )
#define ciaaBAUDRATE_115200  ( 115200U )
#define ciaaBAUDRATE_230400  ( 230400U )
#define ciaaBAUDRATE_460800  ( 460800U )
#define ciaaBAUDRATE_921600  ( 921600U )

/** \brief set FIFO RX Trigger Level for serial devices
 **
 ** This ioctl command is used to set the FIFO RX Trigger Level for any serial device.
 ** Possible values for arg are:
 **   ciaaFIFO_TRIGGER_LEVEL0
 **   ciaaFIFO_TRIGGER_LEVEL1
 **   ciaaFIFO_TRIGGER_LEVEL2
 **   ciaaFIFO_TRIGGER_LEVEL3
 **
 ** Returned value for ioctl is 0
 **/
#define ciaaPOSIX_IOCTL_SET_FIFO_TRIGGER_LEVEL         6

/** \brief FIFO RX Trigger Level macros for serial devices
 **/
#define ciaaFIFO_TRIGGER_LEVEL0     (0)            /*!< UART FIFO trigger level 0: 1 character */
#define ciaaFIFO_TRIGGER_LEVEL1     (1 << 6)       /*!< UART FIFO trigger level 1: 4 character */
#define ciaaFIFO_TRIGGER_LEVEL2     (2 << 6)       /*!< UART FIFO trigger level 2: 8 character */
#define ciaaFIFO_TRIGGER_LEVEL3     (3 << 6)       /*!< UART FIFO trigger level 3: 14 character */

/** \brief set Enabled/Disabled TX Interrupt for serial devices
 **
 ** This ioctl command is used to set Enabled/Disabled TX Interrupt for any serial device.
 ** Possible values for arg are:
 **   true (Enabled)
 **   false (Enabled)
 **
 ** Returned none
 **/
#define ciaaPOSIX_IOCTL_SET_ENABLE_TX_INTERRUPT        7

/** \brief set Enabled/Disabled RX Interrupt for serial devices
 **
 ** This ioctl command is used to set Enabled/Disabled RX Interrupt for any serial device.
 ** Possible values for arg are:
 **   true (Enabled)
 **   false (Enabled)
 **
 ** Returned none
 **/
#define ciaaPOSIX_IOCTL_SET_ENABLE_RX_INTERRUPT        8

/** \brief set Enabled/Disabled NonBlock Mode
 **
 ** This ioctl command is used to set Enabled/Disabled NonBlock Mode for any serial device.
 ** Possible values for arg are:
 **   true (Enabled)
 **   false (Enabled)
 **
 ** Returned none
 **/
#define ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE              9

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
 **          ciaaPOSIX_read and ciaaPOSIX_write may produce unexpected behaviors.
 **/
extern int32_t ciaaPOSIX_open(char const * path, uint8_t oflag);

/** \brief Close a file descriptor
 **
 ** Closes the file descriptor fildes
 **
 ** \param[in] fildes file descriptor to be closed
 ** \return    -1 if failed, 0 in other if success.
 **
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read and
 **          ciaaPOSIX_write may be called reentrant but with different file
 **          descriptor. If one of this function is called with a specific file
 **          descriptor the caller has to wait until return before calling other
 **          of this function using the same file handler.
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
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read and
 **          ciaaPOSIX_write may be called reentrant but with different file
 **          descriptor. If one of this function is called with a specific file
 **          descriptor the caller has to wait until return before calling other
 **          of this function using the same file handler.
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
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read and
 **          ciaaPOSIX_write may be called reentrant but with different file
 **          descriptor. If one of this function is called with a specific file
 **          descriptor the caller has to wait until return before calling other
 **          of this function using the same file handler.
 **/
extern ssize_t ciaaPOSIX_read(int32_t fildes, void * buf, ssize_t nbyte);

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
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read and
 **          ciaaPOSIX_write may be called reentrant but with different file
 **          descriptor. If one of this function is called with a specific file
 **          descriptor the caller has to wait until return before calling other
 **          of this function using the same file handler.
 **/
extern ssize_t ciaaPOSIX_write(int32_t fildes, void const * buf, size_t nbyte);

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

