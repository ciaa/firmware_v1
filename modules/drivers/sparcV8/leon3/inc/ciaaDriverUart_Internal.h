/* Copyright 2016, Gerardo Puga
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

#ifndef _CIAADRIVERUART_INTERNAL_H_
#define _CIAADRIVERUART_INTERNAL_H_
/** \brief Internal Header file of SPARC/LEON3 UART Driver
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup UART UART Drivers
 ** @{ */

/*==================[inclusions]=============================================*/


#include "ciaaPOSIX_stdint.h"
#include "ciaaDriverUart.h"
#include "grlib.h"


/*==================[macros]=================================================*/


#define SPARC_DRIVER_UART_DEFAULT_BAUDRATE 38400

#define SPARC_DRIVER_UART_QUEUE_LENGHT 32


/*==================[typedef]================================================*/


typedef struct {

   uint8_t buffer[SPARC_DRIVER_UART_QUEUE_LENGHT];

   uint32_t head;
   uint32_t tail;

   uint32_t count;

} sparcDriverUartQueueType;


typedef struct {

   /* Core configuration */
   grDeviceAddress baseAddress;
   uint32_t irq;
   uint32_t hasFIFOs;

   /* UART configuration */
   uint32_t numberOfStopBits; /* Valid values: 1 or 2 */
   uint32_t enableFlowControl;
   uint32_t useParity;
   uint32_t useOddParity;
   uint32_t enableLoopback;
   uint32_t externalClkEnabled;

   /* Interrupts configuration */
   uint32_t txInterruptEnabled;
   uint32_t rxInterruptEnabled;

   /* Baudrate configuration */
   uint32_t baudrate;

   /* device state configuration */
   uint32_t deviceIsOpen;

   /* rx and tx data queues */
   sparcDriverUartQueueType rxQueue;

   ciaaDevices_deviceType deviceDataStructure;

} sparcDriverUartInfoType;


/*==================[external data declaration]==============================*/


/*==================[external functions declaration]=========================*/


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* _CIAADRIVERUART_INTERNAL_H_ */
