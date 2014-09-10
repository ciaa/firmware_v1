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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsl_debug_console.h"
#include "fsl_uart_hal.h"
#include "fsl_uart_common.h"
#include "fsl_uart_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
#if defined(HW_LPUART_INSTANCE_COUNT)
#include "fsl_lpuart_driver.h"
#endif


#if __ICCARM__
#include <yfuns.h>
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Operation functions definiations for debug console. */
typedef struct DebugConsoleOperationFunctions {
   uint32_t (* Send)(uint32_t instance, const uint8_t *buf, uint32_t count, uint32_t timeout);
   uint32_t (* Receive)(uint32_t instance, uint8_t *buf, uint32_t count, uint32_t timeout);
} debug_console_ops_t;

/*! @brief State structure storing debug console. */
typedef struct DebugConsoleState {
    debug_console_device_type_t type;/*<! Indicator telling whether the debug console is inited. */
    union {
#if defined(HW_UART_INSTANCE_COUNT)
        uart_state_t uartState;
#endif
#if defined(HW_LPUART_INSTANCE_COUNT)
        lpuart_state_t lpuartState;
#endif
    } state;                        /*<! State structure for the hardware uart state. */
    uint8_t instance;               /*<! Instance number indicator. */
    debug_console_ops_t ops;        /*<! Operation function pointers for debug uart operations. */
} debug_console_state_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Debug UART state information.*/
static debug_console_state_t s_debugConsole;


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static uint32_t UART_DRV_SendPollBlocking(
        uint32_t instance, const uint8_t *buf, uint32_t count, uint32_t timeout);
static uint32_t UART_DRV_ReceivePollBlocking(
        uint32_t instance, uint8_t *buf, uint32_t count, uint32_t timeout);
/*******************************************************************************
 * Code
 ******************************************************************************/
/* Function below will be removed when the UART PD driver support the poll mode later. */
static uint32_t UART_DRV_SendPollBlocking(
        uint32_t instance, const uint8_t *buf, uint32_t count, uint32_t timeout)
{
    uint32_t size = count;
    uint32_t baseAddr = g_uartBaseAddr[s_debugConsole.instance];

    /* No timeout mechanism now. */
    while (size--)
    {
        while (!UART_HAL_IsTxDataRegEmpty(baseAddr))
        {}

        UART_HAL_Putchar(baseAddr, *buf++);

    }

    return count;
}

/* Function below will be removed when the UART PD driver support the poll mode later. */
static uint32_t UART_DRV_ReceivePollBlocking(
        uint32_t instance, uint8_t *buf, uint32_t count, uint32_t timeout)
{
    uint32_t size = count;
    uint32_t baseAddr = g_uartBaseAddr[s_debugConsole.instance];

    for (; size > 0; --size)
    {
        while (!UART_HAL_IsRxDataRegFull(baseAddr))
        {}

        UART_HAL_Getchar(baseAddr, buf++);
    }

    return count;
}

/* See fsl_debug_console.h for documentation of this function.*/
debug_console_status_t DbgConsole_Init(
        uint32_t uartInstance, uint32_t baudRate, debug_console_device_type_t device)
{
    if (s_debugConsole.type != kDebugConsoleNone)
    {
        return kStatus_DEBUGCONSOLE_Failed;
    }

    /* Set debug console to initialized to avoid duplicated init operation.*/
    s_debugConsole.type = device;

    /* Switch between different device. */
    switch (device)
    {
#if defined(HW_UART_INSTANCE_COUNT)
        case kDebugConsoleUART:
            {
                /* Declare config sturcuture to initialize a uart instance. */
                uart_user_config_t uartConfig;
                uart_status_t status;


                /* Config the structure. */
                uartConfig.baudRate = baudRate;
                uartConfig.bitCountPerChar = kUart8BitsPerChar;
                uartConfig.parityMode = kUartParityDisabled;
                uartConfig.stopBitCount = kUartOneStopBit;

                /* Init UART device. */
                status = UART_DRV_Init(uartInstance, &s_debugConsole.state.uartState, &uartConfig);

                if ((status != kStatus_UART_Success)&&(status != kStatus_UART_Initialized))
                {
                    s_debugConsole.type = kDebugConsoleNone;
                    return kStatus_DEBUGCONSOLE_Failed;
                }

                /* Set the funciton pointer for send and receive for this kind of device. */
                s_debugConsole.ops.Send = UART_DRV_SendPollBlocking;
                s_debugConsole.ops.Receive = UART_DRV_ReceivePollBlocking;
            }
            break;
#endif
#if 0
#if defined(HW_LPUART_INSTANCE_COUNT)
        case kDebugConsoleLPUART:
            {
                /* Declare config sturcuture to initialize a uart instance. */
                lpuart_user_config_t lpuartConfig;
                lpuart_status_t status;

                /* Config the structure. */
                lpuartConfig.baudRate = baudRate;
                lpuartConfig.bitCountPerChar = kLpuart8BitsPerChar;
                lpuartConfig.parityMode = kLpuartParityDisabled;
                lpuartConfig.stopBitCount = kLpuartOneStopBit;

                /* Init LPUART device. */
                status =
                    LPUART_DRV_Init(uartInstance, &s_debugConsole.state.lpuartState, &lpuartConfig);
                if ((status != kStatus_UART_Success)&&(status != kStatus_UART_Initialized))
                {
                    s_debugConsole.type = kDebugConsoleNone;
                    return kStatus_DEBUGCONSOLE_Failed;
                }

                /* Set the funciton pointer for send and receive for this kind of device. */
                s_debugConsole.ops.Send = LPUART_DRV_SendPollBlocking;
                s_debugConsole.ops.Receive = LPUART_DRV_ReceivePollBlocking;

            }
            break;
#endif
#endif
        /* If new device is requried as the low level device for debug console,
         * Add the case branch and add the preprocessor macro to judge whether
         * this kind of device exist in this SOC. */
        default:
            /* Device identified is invalid, return invalid device error code. */
            return kStatus_DEBUGCONSOLE_InvalidDevice;
    }

    /* Configure the s_debugConsole structure only when the inti operation is successful. */
    s_debugConsole.instance = uartInstance;

#if ((defined(__GNUC__)) && (!defined(FSL_RTOS_MQX)))
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
#endif

    return kStatus_DEBUGCONSOLE_Success;
}

/* See fsl_debug_console.h for documentation of this function.*/
debug_console_status_t DbgConsole_DeInit(void)
{
    if (s_debugConsole.type == kDebugConsoleNone)
    {
        return kStatus_DEBUGCONSOLE_Success;
    }

    switch(s_debugConsole.type)
    {
#if defined(HW_UART_INSTANCE_COUNT)
        case kDebugConsoleUART:
            UART_DRV_Deinit(s_debugConsole.instance);
            break;
#endif
#if 0
#if defined(HW_LPUART_INSTANCE_COUNT)
        case kDebugConsoleLPUART:
                LPUART_DRV_Deinit(s_debugConsole.instance);
            break;
#endif
#endif
        default:
            return kStatus_DEBUGCONSOLE_InvalidDevice;
    }

    s_debugConsole.type = kDebugConsoleNone;

    return kStatus_DEBUGCONSOLE_Success;
}

#if __ICCARM__

#pragma weak __write
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    if (buffer == 0)
    {
        /* This means that we should flush internal buffers.  Since we*/
        /* don't we just return.  (Remember, "handle" == -1 means that all*/
        /* handles should be flushed.)*/
        return 0;
    }

    /* This function only writes to "standard out" and "standard err",*/
    /* for all other file handles it returns failure.*/
    if ((handle != _LLIO_STDOUT) && (handle != _LLIO_STDERR))
    {
        return _LLIO_ERROR;
    }

    /* Do nothing if the debug uart is not initialized.*/
    if (s_debugConsole.type == kDebugConsoleNone)
    {
        return _LLIO_ERROR;
    }

    /* Send data.*/
    return s_debugConsole.ops.Send(s_debugConsole.instance, (uint8_t const *)buffer, size, 1000);
}

#pragma weak __read
size_t __read(int handle, unsigned char * buffer, size_t size)
{
    /* This function only reads from "standard in", for all other file*/
    /* handles it returns failure.*/
    if (handle != _LLIO_STDIN)
    {
        return _LLIO_ERROR;
    }

    /* Do nothing if the debug uart is not initialized.*/
    if (s_debugConsole.type == kDebugConsoleNone)
    {
        return _LLIO_ERROR;
    }

    /* Receive data.*/
    return s_debugConsole.ops.Receive(s_debugConsole.instance, buffer, size, 1000);
}

#elif (defined(__GNUC__))
#pragma weak _write
int _write (int handle, char *buffer, int size)
{
    if (buffer == 0)
    {
        /* return -1 if error */
        return -1;
    }

    /* This function only writes to "standard out" and "standard err",*/
    /* for all other file handles it returns failure.*/
    if ((handle != 1) && (handle != 2))
    {
        return -1;
    }

    /* Do nothing if the debug uart is not initialized.*/
    if (s_debugConsole.type == kDebugConsoleNone)
    {
        return -1;
    }

    /* Send data.*/
    return s_debugConsole.ops.Send(s_debugConsole.instance, (uint8_t *)buffer, size, 1000);
}

#pragma weak _read
int _read(int handle, char *buffer, int size)
{
    /* This function only reads from "standard in", for all other file*/
    /* handles it returns failure.*/
    if (handle != 0)
    {
        return -1;
    }

    /* Do nothing if the debug uart is not initialized.*/
    if (s_debugConsole.type == kDebugConsoleNone)
    {
        return -1;
    }

    /* Receive data.*/
    return s_debugConsole.ops.Receive(s_debugConsole.instance, (uint8_t *)buffer, size, 1000);
}
#elif (defined(__CC_ARM))
struct __FILE
{
	int handle;
	/* Whatever you require here. If the only file you are using is */
	/* standard output using printf() for debugging, no file handling */
	/* is required. */
};

/* FILE is typedef in stdio.h. */
#pragma weak __stdout
FILE __stdout;
FILE __stdin;

#pragma weak fputc
int fputc(int ch, FILE *f)
{
    /* Do nothing if the debug uart is not initialized.*/
    if (s_debugConsole.type == kDebugConsoleNone)
    {
        return -1;
    }

    /* Send data.*/
    return s_debugConsole.ops.Send(s_debugConsole.instance, (const uint8_t*)&ch, 1, 1000);
}

#pragma weak fgetc
int fgetc(FILE *f)
{
    uint8_t temp;
    /* Do nothing if the debug uart is not initialized.*/
    if (s_debugConsole.type == kDebugConsoleNone)
    {
        return -1;
    }

    /* Receive data.*/
    s_debugConsole.ops.Receive(s_debugConsole.instance, &temp, 1, 1000);
    return temp;
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
