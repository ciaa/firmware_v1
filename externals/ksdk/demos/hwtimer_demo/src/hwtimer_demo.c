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
 */

#include <stdio.h>

#include "fsl_uart_driver.h"
#include "board.h"
#include "fsl_hwtimer.h"

#ifdef DEBUG
#include "fsl_debug_console.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HWTIMER_LL_DEVIF    kSystickDevif
//#define HWTIMER_LL_DEVIF    kPitDevif
#define HWTIMER_LL_SRCCLK   kCoreClock
//#define HWTIMER_LL_SRCCLK   kBusClock
#define HWTIMER_LL_ID       0
//#define HWTIMER_LL_ID       3

#define HWTIMER_PERIOD          100000
#define HWTIMER_DOTS_PER_LINE   40
#define HWTIMER_LINES_COUNT     2

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
extern const hwtimer_devif_t kSystickDevif;
extern const hwtimer_devif_t kPitDevif;
hwtimer_t hwtimer;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void hwtimer_callback(void* data)
{
    printf(".");
    if ((HWTIMER_SYS_GetTicks(&hwtimer) % HWTIMER_DOTS_PER_LINE) == 0)
    {
        printf("\r\n");
    }
    if ((HWTIMER_SYS_GetTicks(&hwtimer) % (HWTIMER_LINES_COUNT * HWTIMER_DOTS_PER_LINE)) == 0)
    {
        if (kHwtimerSuccess != HWTIMER_SYS_Stop(&hwtimer))
        {
            printf("\r\nError: hwtimer stop.\r\n");
        }
        printf("End\r\n");
    }
}
/********************************************************************/
int main (void)
{
    /* Initialize standard SDK demo application pins */
    hardware_init();

    /* Configure the UART TX/RX pins */
    configure_uart_pins(BOARD_DEBUG_UART_INSTANCE);

    /* Call this function to initialize the console UART.  This function
       enables the use of STDIO functions (printf, scanf, etc.) */
    dbg_uart_init();

    /*  Print the initial banner */
    printf("\r\nHwtimer Example \r\n");

    /* Hwtimer initialization */
    if (kHwtimerSuccess != HWTIMER_SYS_Init(&hwtimer, &HWTIMER_LL_DEVIF, HWTIMER_LL_ID, 5, NULL))
    {
        printf("\r\nError: hwtimer initialization.\r\n");
    }
    if (kHwtimerSuccess != HWTIMER_SYS_SetPeriod(&hwtimer, HWTIMER_LL_SRCCLK, HWTIMER_PERIOD))
    {
        printf("\r\nError: hwtimer set period.\r\n");
    }
    if (kHwtimerSuccess != HWTIMER_SYS_RegisterCallback(&hwtimer, hwtimer_callback, NULL))
    {
        printf("\r\nError: hwtimer callback registration.\r\n");
    }
    if (kHwtimerSuccess != HWTIMER_SYS_Start(&hwtimer))
    {
        printf("\r\nError: hwtimer start.\r\n");
    }

    /* Wait for Hardware Timer interrupts */
    while(1)
    {}
}
/********************************************************************/
/********************************************************************/
