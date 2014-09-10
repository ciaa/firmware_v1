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

#if !defined(__FSL_DEBUG_CONSOLE_H__)
#define __FSL_DEBUG_CONSOLE_H__

#include <stdint.h>

/*
 * @addtogroup debug_console
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Error code for the debug console driver. */
typedef enum _debug_console_status {
    kStatus_DEBUGCONSOLE_Success = 0U,
    kStatus_DEBUGCONSOLE_InvalidDevice,
    kStatus_DEBUGCONSOLE_AllocateMemoryFailed,
    kStatus_DEBUGCONSOLE_Failed 
} debug_console_status_t;

/*! @brief Supported debug console hardware device type. */
typedef enum _debug_console_device_type {
    kDebugConsoleNone = 0U,
    kDebugConsoleUART = 16U,    /*<! Use strange start number to avoid treating 0
                                     as correct device type. Sometimes user forget
                                     to specify the device type but only use the 
                                     default value '0' as the device type.  */
    kDebugConsoleLPUART = 17U
} debug_console_device_type_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! @name Initialization*/
/*@{*/

/*!
 * @brief Init the UART/LPUART used for debug messages.
 *
 * Call this function to enable debug log messages to be output via the specified UART/LPUART
 * base address and at the specified baud rate. Just initializes the UART/LPUART to the given baud
 * rate and 8N1. After this function has returned, stdout and stdin will be connected to the
 * selected UART/LPUART. The debug_printf() function also uses this UART/LPUART.
 *
 * @param uartInstance Which UART/LPUART instance is used to send debug messages.
 * @param baudRate The desired baud rate in bits per second.
 * @param device Low level device type for the debug console.
 */
debug_console_status_t DbgConsole_Init(
        uint32_t uartInstance, uint32_t baudRate, debug_console_device_type_t device);

/*!
 * @brief Deinit the UART/LPUART used for debug messages.
 *
 * Call this function to disable debug log messages to be output via the specified UART/LPUART
 * base address and at the specified baud rate.
 *
 */
debug_console_status_t DbgConsole_DeInit(void);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_DEBUG_CONSOLE_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/
