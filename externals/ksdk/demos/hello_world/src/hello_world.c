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
#include <stdlib.h>

#include "fsl_device_registers.h"
#include "fsl_uart_driver.h"
#include "fsl_clock_manager.h"
#include "board.h"

#ifdef DEBUG
#include "fsl_debug_console.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define USE_STDIO_FUNCTIONS  /* Define this symbol to use STDIO functions */

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/********************************************************************/
int main (void)
{
    /***************************************************************************
     *  RX buffers
     **************************************************************************/
    /*! @param receiveBuff Buffer used to hold received data */
    uint8_t receiveBuff[19] = {0};

    /* Initialize standard SDK demo application pins */
    hardware_init();

    /* Configure the UART TX/RX pins */
    configure_uart_pins(BOARD_DEBUG_UART_INSTANCE);

#ifdef USE_STDIO_FUNCTIONS
    /* Call this function to initialize the console UART.  This function
       enables the use of STDIO functions (printf, scanf, etc.) */
    dbg_uart_init();
    
    /*  Print the initial banner */
    printf("\r\nHello World!\n\n\r");

    while(1)
    {
        /********************************************
         * Main routine that simply echoes received
         * characters forever
         *********************************************/

        /* First, get character.  */
        receiveBuff[0] = getchar();
        
        /* Now echo the received character */
        putchar(receiveBuff[0]);
    }
#else
    /***************************************************************************
     * UART configuration and state structures
     **************************************************************************/
    /*! @param uartConfig UART configuration structure */
    /*! @param uartState UARt state structure which is used internally by the*/
    /*! by the UART driver to keep track of the UART states */
    uart_user_config_t uartConfig;
    uart_state_t uartState;
    
    /***************************************************************************
     *  TX buffers
     **************************************************************************/
    /*! @param sourceBuff Buffer used to hold the string to be transmitted */
    uint8_t sourceBuff[19] = {"\r\nHello World!\n\n\r"};
    
    /* Configure the UART for 115200, 8 data bits, No parity, and one stop bit*/
    uartConfig.baudRate = 115200;
    uartConfig.bitCountPerChar = kUart8BitsPerChar;
    uartConfig.parityMode = kUartParityDisabled;
    uartConfig.stopBitCount = kUartOneStopBit;
    
    /* Must call the OSA Init function to use Communication drivers */
    OSA_Init();
    
    /* Initialize the UART module */
    UART_DRV_Init(BOARD_DEBUG_UART_INSTANCE, &uartState, &uartConfig);
    
    /*  Print the initial banner */
    UART_DRV_SendDataBlocking(BOARD_DEBUG_UART_INSTANCE, sourceBuff, 17, 200);

    while(1)
    {
        /********************************************
         * Main routine that simply echoes received
         * characters forever
         *********************************************/

        /* First, get character.  */
        UART_DRV_ReceiveDataBlocking(BOARD_DEBUG_UART_INSTANCE, receiveBuff, 1, 
                                     OSA_WAIT_FOREVER);

        /* Now, stuff the buffer for the TX side and send the character*/
        sourceBuff[0] = receiveBuff[0];

        /* Now echo the received character */
        UART_DRV_SendDataBlocking(BOARD_DEBUG_UART_INSTANCE, sourceBuff, 1, 
                                  200);
    }
#endif
}
/********************************************************************/
/********************************************************************/
