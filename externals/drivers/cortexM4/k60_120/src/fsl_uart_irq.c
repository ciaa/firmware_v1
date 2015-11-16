/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Revisions
 *   -- KSDK_1.0.0 rev.1 (2014-09-10)
 *       - Add K60F120 family support.
 *      Darío Baliña, dariosb@gmail.com, Delsat Group.
 */

#include "fsl_uart_driver.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void UART_DRV_IRQHandler(uint32_t instance);
/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined (KL25Z4_SERIES)
/* Implementation of UART0 handler named in startup code. */
void UART0_IRQHandler(void)
{
    UART_DRV_IRQHandler(0);
}

/* Implementation of UART1 handler named in startup code. */
void UART1_IRQHandler(void)
{
    UART_DRV_IRQHandler(1);
}

/* Implementation of UART2 handler named in startup code. */
void UART2_IRQHandler(void)
{
    UART_DRV_IRQHandler(2);
}

#elif defined (K64F12_SERIES) || defined (K24F12_SERIES) || defined (K63F12_SERIES) || \
      defined (K22F51212_SERIES) || defined (K22F25612_SERIES) || defined (K22F12810_SERIES) || \
      defined (KV31F51212_SERIES) || defined (KV31F25612_SERIES) || defined (KV31F12810_SERIES) || \
      defined (K70F12_SERIES) || defined (K60F12_SERIES) 
/* Implementation of UART0 handler named in startup code. */
void UART0_RX_TX_IRQHandler(void)
{
    UART_DRV_IRQHandler(0);
}

/* Implementation of UART1 handler named in startup code. */
void UART1_RX_TX_IRQHandler(void)
{
    UART_DRV_IRQHandler(1);
}

/* Implementation of UART2 handler named in startup code. */
void UART2_RX_TX_IRQHandler(void)
{
    UART_DRV_IRQHandler(2);
}

/* Implementation of UART3 handler named in startup code. */
void UART3_RX_TX_IRQHandler(void)
{
    UART_DRV_IRQHandler(3);
}

/* Implementation of UART4 handler named in startup code. */
void UART4_RX_TX_IRQHandler(void)
{
    UART_DRV_IRQHandler(4);
}

/* Implementation of UART5 handler named in startup code. */
void UART5_RX_TX_IRQHandler(void)
{
    UART_DRV_IRQHandler(5);
}

#else
    #error "No valid CPU defined!"
#endif
/*******************************************************************************
 * EOF
 ******************************************************************************/

