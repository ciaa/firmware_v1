/* Copyright 2015, Mariano Cerdeiro
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

#ifndef CIAAPOSIX_IOCTL_SERIAL
#define CIAAPOSIX_IOCTL_SERIAL
/** \brief IO Control macros for serial devices
 **
 ** This files contains the macros for IO control for serial devices
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150221 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
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

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAAPOSIX_IOCTL_SERIAL */

