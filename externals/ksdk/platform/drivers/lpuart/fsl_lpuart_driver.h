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

#ifndef __FSL_LPUART_DRIVER_H__
#define __FSL_LPUART_DRIVER_H__

#include <stdint.h>
#include <stdbool.h>
#include "fsl_os_abstraction.h"
#include "fsl_lpuart_hal.h"

/*! 
 * @addtogroup lpuart_driver
 * @{
 */

/*******************************************************************************
 * Definitions  
 ******************************************************************************/

/*! @brief LPUART receive callback function type. */
typedef lpuart_status_t (* lpuart_rx_callback_t)(uint8_t * rxByte, void * param);

/*! 
 * @brief Runtime state of the LPUART driver.
 *
 * Note that the caller provides memory for the driver state structures during
 * initialization because the driver does not statically allocate memory.
 */
typedef struct LpuartState {
    const uint8_t * txBuff;          /*!< The buffer of data being sent.*/
	uint8_t * rxBuff;                /*!< The buffer of received data.*/
    volatile size_t txSize;          /*!< The remaining number of bytes to be transmitted. */
    volatile size_t rxSize;          /*!< The remaining number of bytes to be received. */
    volatile bool isTxBusy;          /*!< True if there is an active transmit.*/
    volatile bool isRxBusy;          /*!< True if there is an active receive.*/
    volatile bool isTxBlocking;      /*!< True if transmit is blocking transaction. */
    volatile bool isRxBlocking;      /*!< True if receive is blocking transaction. */
    semaphore_t txIrqSync;           /*!< Used to wait for ISR to complete its Tx business.*/
    semaphore_t rxIrqSync;           /*!< Used to wait for ISR to complete its Rx business.*/
    lpuart_rx_callback_t rxCallback; /*!< Callback to invoke after receiving byte.*/
    void * rxCallbackParam;          /*!< Receive callback parameter pointer.*/
} lpuart_state_t;

/*! @brief LPUART configuration structure*/
typedef struct LpuartUserConfig {
    uint32_t baudRate;                   /*!< LPUART baud rate*/
    lpuart_parity_mode_t parityMode;     /*!< parity mode, disabled (default), even, odd */
    lpuart_stop_bit_count_t stopBitCount;/*!< number of stop bits, 1 stop bit (default) or 2 stop bits*/
    lpuart_bit_count_per_char_t bitCountPerChar; /*!< number of bits, 8-bit (default) or 9-bit in a
                                                   char (up to 10-bits in some LPUART instances.*/
} lpuart_user_config_t;

/*******************************************************************************
 * API 
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name LPUART Driver
 * @{
 */

/*!
 * @brief Initializes a LPUART operation instance.
 *
 * The caller provides memory for the driver state structures during initialization.
 * The user must select the LPUART clock source in the application to initialize the LPUART.
 *
 * @param instance  LPUART instance number
 * @param lpuartUserConfig user configuration structure of type #lpuart_user_config_t
 * @param lpuartStatePtr pointer to the LPUART driver state structure
 * @return An error code or kStatus_LPUART_Success
 */
lpuart_status_t LPUART_DRV_Init(uint32_t instance, lpuart_state_t * lpuartStatePtr,
                                const lpuart_user_config_t * lpuartUserConfig);

/*!
 * @brief Shuts down the LPUART by disabling interrupts and transmitter/receiver.
 *
 * @param instance  LPUART instance number
 */
void LPUART_DRV_Deinit(uint32_t instance);

/*!
 * @brief Sends data out through the LPUART module using a blocking method.
 *
 *  Blocking means that the function does not return until the transmission is complete.
 *
 * @param instance  LPUART instance number
 * @param txBuff  source buffer containing 8-bit data chars to send
 * @param txSize the number of bytes to send
 * @param timeout timeout value for RTOS abstraction sync control
 * @return An error code or kStatus_LPUART_Success
 */
lpuart_status_t LPUART_DRV_SendDataBlocking(uint32_t instance, 
                                            const uint8_t * txBuff, 
                                            uint32_t txSize,
                                            uint32_t timeout);

/*!
 * @brief Sends data out through the LPUART module using a non-blocking method.
 *  This enablesS an async method for transmitting data. When used with
 *  a non-blocking receive, the LPUART can perform a full duplex operation.
 *  Non-blocking  means that the function returns immediately.
 *  The application has to get the transmit status to know when the transmit is complete.
 *
 * @param instance  LPUART instance number
 * @param txBuff  source buffer containing 8-bit data chars to send
 * @param txSize  the number of bytes to send
 * @return An error code or kStatus_LPUART_Success
 */
lpuart_status_t LPUART_DRV_SendData(uint32_t instance,
                                    const uint8_t * txBuff,
                                    uint32_t txSize);

/*!
 * @brief Returns whether the previous transmit is complete.
 *
 * @param instance  LPUART instance number
 * @param bytesRemaining Pointer to value that will be filled in with the number of bytes that
 *      have been sent in the active transfer
 *
 * @retval kStatus_LPUART_Success The transmit has completed successfully.
 * @retval kStatus_LPUART_TxBusy The transmit is still in progress. @a bytesTransmitted will be
 *     filled with the number of bytes that have been transmitted so far.
 */
lpuart_status_t LPUART_DRV_GetTransmitStatus(uint32_t instance, uint32_t * bytesRemaining);

/*!
 * @brief Terminates an asynchronous transmission early.
 *
 * @param instance  LPUART instance number
 *
 * @retval kStatus_LPUART_Success The transmit was successful.
 */
lpuart_status_t LPUART_DRV_AbortSendingData(uint32_t instance);

/*!
 * @brief Gets data from the LPUART module by using a blocking method.
 *  Blocking means that the function does not return until the
 *  receive is complete.
 *
 * @param instance  LPUART instance number
 * @param rxBuff  buffer containing 8-bit read data chars received
 * @param rxSize the number of bytes to receive
 * @param timeout timeout value for RTOS abstraction sync control
 * @return An error code or kStatus_LPUART_Success
 */
lpuart_status_t LPUART_DRV_ReceiveDataBlocking(uint32_t instance,
                                               uint8_t * rxBuff,
                                               uint32_t rxSize,
                                               uint32_t timeout);

/*!
 * @brief Gets data from the LPUART module by using a non-blocking method.
 *  This enables an async method for receiving data. When used with
 *  a non-blocking transmission, the LPUART can perform a full duplex operation.
 *  Non-blocking means that the function returns immediately.
 *  The application has to get the receive status to know when the receive is complete.
 *
 * @param instance  LPUART instance number
 * @param rxBuff  buffer containing 8-bit read data chars received
 * @param rxSize  the number of bytes to receive
 * @return An error code or kStatus_LPUART_Success
 */
lpuart_status_t LPUART_DRV_ReceiveData(uint32_t instance,
                                       uint8_t * rxBuff,
                                       uint32_t rxSize);

/*!
 * @brief Returns whether the previous receive is complete.
 *
 * @param instance  LPUART instance number
 * @param bytesRemaining pointer to value that is filled  with the number of bytes that
 *        still need to be received in the active transfer.
 *
 * @retval kStatus_LPUART_Success the receive has completed successfully.
 * @retval kStatus_LPUART_RxBusy the receive is still in progress. @a bytesReceived will be
 *     filled with the number of bytes that have been received so far.
 */
lpuart_status_t LPUART_DRV_GetReceiveStatus(uint32_t instance, uint32_t * bytesRemaining);

/*!
 * @brief Terminates an asynchronous receive early.
 *
 * @param instance  LPUART instance number
 *
 * @retval kStatus_LPUART_Success The receive was successful.
 */
lpuart_status_t LPUART_DRV_AbortReceivingData(uint32_t instance);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_LPUART_DRIVER_H__ */
/******************************************************************************/
/* EOF */
/******************************************************************************/

