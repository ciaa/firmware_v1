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

#ifndef _CIAADRIVERUART_INTERNAL_H_
#define _CIAADRIVERUART_INTERNAL_H_
/** \brief Internal Header file of UART Driver
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
 * EsVo         Esteban Volentini
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20141121 v0.0.3 EsVo add host uart support
 * 20141116 v0.0.2 EsVo add uart emulation via sockets
 * 20140528 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include <sys/socket.h>
#include <netinet/in.h>
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stdbool.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/* Define host port to use a serial port 0 */
//#define CIAADRVUART_PORT_SERIAL_0 "/dev/ttyUSB0"
/* Define host port to use a serial port 1 */
//#define CIAADRVUART_PORT_SERIAL_1 "/dev/cu.usbserial"
/* Define TCP PORT for lisening socket emulation serial port 0 */
//#define CIAADRVUART_TCP_PORT_0  2000
/* Define TCP PORT for lisening socket emulation serial port 1 */
//#define CIAADRVUART_TCP_PORT_1 2001

/** Enable uart transmition via host interfaces */
#if defined(CIAADRVUART_PORT_SERIAL_0) || defined(CIAADRVUART_PORT_SERIAL_1)
   #include <termios.h>

   #define CIAADRVUART_ENABLE_TRANSMITION

   #ifndef CIAADRVUART_PORT_SERIAL_0
      #define CIAADRVUART_PORT_SERIAL_0      ""
   #endif

   #ifndef CIAADRVUART_PORT_SERIAL_1
      #define CIAADRVUART_PORT_SERIAL_1      ""
   #endif
#endif

/** Enable uart emulation via sockets */
#if defined(CIAADRVUART_TCP_PORT_0) || defined(CIAADRVUART_TCP_PORT_1)
   #define CIAADRVUART_ENABLE_EMULATION

   #ifndef CIAADRVUART_TCP_PORT_0
      #define CIAADRVUART_TCP_PORT_0         0
   #endif

   #ifndef CIAADRVUART_TCP_PORT_1
      #define CIAADRVUART_TCP_PORT_1         0
   #endif
#endif

/** Enable funcionality of uart driver via transmition and/or emulation */
#if defined(CIAADRVUART_ENABLE_TRANSMITION) || defined(CIAADRVUART_ENABLE_EMULATION)
   #define CIAADRVUART_ENABLE_FUNCIONALITY
#endif

/*==================[typedef]================================================*/
/** \brief Buffer Structure */
typedef struct {
   uint16_t length;              /** <= Length used */
   uint8_t buffer[2048];         /** <= Data storage */
} ciaaDriverUart_bufferType;

/** \brief Uart Type */
typedef struct {
   ciaaDriverUart_bufferType rxBuffer;
   ciaaDriverUart_bufferType txBuffer;
#ifdef CIAADRVUART_ENABLE_FUNCIONALITY
   pthread_t handlerThread;
   int fileDescriptor;
#endif /* CIAADRVUART_ENABLE_FUNCIONALITY */
#ifdef CIAADRVUART_ENABLE_TRANSMITION
   char const * deviceName;
   struct termios deviceOptions;
#endif /* CIAADRVUART_ENABLE_TRANSMITION */
#ifdef CIAADRVUART_ENABLE_EMULATION
   struct sockaddr_in serverAddress;
#endif /* CIAADRVUART_ENABLE_EMULATION */
} ciaaDriverUart_uartType;

/*==================[external data declaration]==============================*/
/** \brief Uart 0 */
extern ciaaDriverUart_uartType ciaaDriverUart_uart0;

/** \brief Uart 1 */
extern ciaaDriverUart_uartType ciaaDriverUart_uart1;

/*==================[external functions declaration]=========================*/
extern void ciaaDriverUart_uart0_rxIndication(void);

extern void ciaaDriverUart_uart0_txConfirmation(void);

extern void ciaaDriverUart_uart1_rxIndication(void);

extern void ciaaDriverUart_uart1_txConfirmation(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAADRIVERUART_INTERNAL_H_ */
