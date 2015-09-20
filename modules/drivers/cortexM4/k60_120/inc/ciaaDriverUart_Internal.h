/* Copyright 2014 Mariano Cerdeiro
 * Copyright 2014, 2015 Esteban Volentini
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
/** \brief Internal Header file of DIO Driver
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup DIO DIO Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * EsVo			 Esteban Volentini
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150913 v0.0.3 EsVo elimintation of reception buffer
 * 20150801 v0.0.2 EsVo first operational version
 * 20140913 v0.0.1 MaCe first stub version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "fsl_uart_hal.h"
#include "fsl_port_hal.h"
#include "fsl_sim_hal.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Size of Uart Rx buffer */
   #define UART_RX_FIFO_SIZE       (32)

/*==================[typedef]================================================*/
/** \brief Pin port resource descriptor Type */
typedef struct ciaaDriverUart_pinStruct {
   PORT_Type * port;                      /** <= i/o port base address */
   uint32_t pin;                          /** <= pin number of i/o port */
   port_mux_t mux;                        /** <= function multiplexor value */
   sim_clock_gate_name_t gate;            /** <= Port clock gate name */
} ciaaDriverUart_pinType;

/** \brief Uart port resource descriptor Type */
typedef struct ciaaDriverUart_portStruct {
   UART_Type * base;                      /** <= uart port base address */
   uint32_t instance;                     /** <= uart port instance */
   IRQn_Type irq;                         /** <= uart port interrupt */
   sim_clock_gate_name_t gate;            /** <= uart clock gate name */
   ciaaDriverUart_pinType rx;             /** <= i/o pin to rx function */
   ciaaDriverUart_pinType tx;             /** <= i/o pin to tx function */
} ciaaDriverUart_portType;

/** \brief Uart driver state Type */
typedef struct {
   uint32_t instance;                     /** <= uart instance diver */
//   ciaaDriverUart_portType const * port;
   struct {
      uint32_t baudRate;
      uart_bit_count_per_char_t bitCountPerChar;
      uart_parity_mode_t parityMode;
#if FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
      uart_stop_bit_count_t stopBitCount;
#endif
   } config;
} ciaaDriverUart_uartType;

/*==================[external data declaration]==============================*/
/** \brief Uart 0 */
extern ciaaDriverUart_uartType ciaaDriverUart_uart0;

/** \brief Uart 1 */
extern ciaaDriverUart_uartType ciaaDriverUart_uart1;

/** \brief Uart 2 */
extern ciaaDriverUart_uartType ciaaDriverUart_uart2;

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAADRIVERDIO_INTERNAL_H_ */

