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

#include <assert.h>
#include <string.h>
#include "fsl_uart_driver.h"
#include "fsl_uart_common.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_Init
 * Description   : This function initializes a UART instance for operation.
 * This function will initialize the run-time state structure to keep track of the on-going
 * transfers, ungate the clock to the UART module, initialize the module
 * to user defined settings and default settings, configure the IRQ state structure and enable
 * the module-level interrupt to the core, and enable the UART module transmitter and receiver.
 * The following is an example of how to set up the uart_state_t and the
 * uart_user_config_t parameters and how to call the UART_DRV_Init function by passing
 * in these parameters:
 *    uart_user_config_t uartConfig;
 *    uartConfig.baudRate = 9600;
 *    uartConfig.bitCountPerChar = kUart8BitsPerChar;
 *    uartConfig.parityMode = kUartParityDisabled;
 *    uartConfig.stopBitCount = kUartOneStopBit;
 *    uart_state_t uartState;
 *    UART_DRV_Init(instance, &uartState, &uartConfig);
 *
 *END**************************************************************************/
uart_status_t UART_DRV_Init(uint32_t instance, uart_state_t * uartStatePtr,
                            const uart_user_config_t * uartUserConfig)
{
    assert(uartStatePtr && uartUserConfig);
    assert(instance < HW_UART_INSTANCE_COUNT);

    uint32_t baseAddr = g_uartBaseAddr[instance];
    uint32_t uartSourceClock;

    /* Exit if current instance is already initialized. */
    if (g_uartStatePtr[instance])
    {
        return kStatus_UART_Initialized;
    }

    /* Clear the state structure for this instance. */
    memset(uartStatePtr, 0, sizeof(uart_state_t));

    /* Save runtime structure pointer.*/
    g_uartStatePtr[instance] = uartStatePtr;

    /* Un-gate UART module clock */
    CLOCK_SYS_EnableUartClock(instance);

    /* Initialize UART to a known state. */
    UART_HAL_Init(baseAddr);

    /* Create Semaphore for txIrq and rxIrq. */
    OSA_SemaCreate(&uartStatePtr->txIrqSync, 0);
    OSA_SemaCreate(&uartStatePtr->rxIrqSync, 0);

    /* UART clock source is either system clock or bus clock depending on the instance */
    uartSourceClock = CLOCK_SYS_GetUartFreq(instance);

    /* Initialize UART baud rate, bit count, parity and stop bit. */
    UART_HAL_SetBaudRate(baseAddr, uartSourceClock, uartUserConfig->baudRate);
    UART_HAL_SetBitCountPerChar(baseAddr, uartUserConfig->bitCountPerChar);
    UART_HAL_SetParityMode(baseAddr, uartUserConfig->parityMode);
#if FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
    UART_HAL_SetStopBitCount(baseAddr, uartUserConfig->stopBitCount);
#endif

#if FSL_FEATURE_UART_HAS_FIFO
    uint8_t fifoSize;
    /* Obtain raw TX FIFO size bit setting */
    fifoSize = UART_HAL_GetTxFifoSize(baseAddr);
    /* Now calculate the number of data words per given FIFO size */
    uartStatePtr->txFifoEntryCount = (fifoSize == 0 ? 1 : 0x1 << (fifoSize + 1));

    /* Configure the TX FIFO watermark to be 1/2 of the total entry or 0 if entry count = 1
     * A watermark setting of 0 for TX FIFO entry count of 1 means that TDRE will only interrupt
     * when the TX buffer (the one entry in the TX FIFO) is empty. Otherwise, if we set the
     * watermark to 1, the TDRE will always be set regardless if the TX buffer was empty or not
     * as the spec says TDRE will set when the FIFO is at or below the configured watermark. */
    if (uartStatePtr->txFifoEntryCount > 1)
    {
        UART_HAL_SetTxFifoWatermark(baseAddr, (uartStatePtr->txFifoEntryCount >> 1U));
    }
    else
    {
        UART_HAL_SetTxFifoWatermark(baseAddr, 0);
    }

    /* Configure the RX FIFO watermark to be 1. 
     * Note about RX FIFO support: There is only one RX data full interrupt that is
     * associated with the RX FIFO Watermark.  The watermark cannot be dynamically changed.
     * This means if the rxSize is less than the programmed watermark the interrupt will 
     * never occur. If we try to change the watermark, this will involve shutting down
     * the receiver first - which is not a desirable operation when the UART is actively
     * receiving data. Hence, the best solution is to set the RX FIFO watermark to 1. */
    UART_HAL_SetRxFifoWatermark(baseAddr, 1);

    /* Enable and flush the FIFO prior to enabling the TX/RX */
    UART_HAL_SetTxFifoCmd(baseAddr, true);
    UART_HAL_SetRxFifoCmd(baseAddr, true);
    UART_HAL_FlushTxFifo(baseAddr);
    UART_HAL_FlushRxFifo(baseAddr);
#else
    /* For modules that do not support a FIFO, they have a data buffer that essentially
     * acts likes a one-entry FIFO, thus to make the code cleaner, we'll
     * equate txFifoEntryCount to 1.  Also note that TDRE flag will set only when the tx
     * buffer is empty. */
    uartStatePtr->txFifoEntryCount = 1;
#endif

    /* Enable UART interrupt on NVIC level. */
    INT_SYS_EnableIRQ(g_uartRxTxIrqId[instance]);

    /* Finally, enable the UART transmitter and receiver*/
    UART_HAL_EnableTransmitter(baseAddr);
    UART_HAL_EnableReceiver(baseAddr);

    return kStatus_UART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_Deinit
 * Description   : This function shuts down the UART by disabling interrupts and the
 *                 transmitter/receiver.
 * This function disables the UART interrupts, disables the transmitter and receiver, and
 * flushes the FIFOs (for modules that support FIFOs).
 *
 *END**************************************************************************/
void UART_DRV_Deinit(uint32_t instance)
{
    assert(instance < HW_UART_INSTANCE_COUNT);

    uint32_t baseAddr = g_uartBaseAddr[instance];

    /* In case there is still data in the TX FIFO or shift register that is being transmitted
     * wait till transmit is complete. */
#if FSL_FEATURE_UART_HAS_FIFO
    /* Wait until there all of the data has been drained from the TX FIFO */
    while(UART_HAL_GetTxDatawordCountInFifo(baseAddr) != 0) { }
#endif
    /* Wait until the data is completely shifted out of shift register */
    while(!(UART_HAL_IsTxComplete(baseAddr))) { }

    /* Disable the interrupt */
    INT_SYS_DisableIRQ(g_uartRxTxIrqId[instance]);

    /* Disable TX and RX */
    UART_HAL_DisableTransmitter(baseAddr);
    UART_HAL_DisableReceiver(baseAddr);

#if FSL_FEATURE_UART_HAS_FIFO
    /* Disable the FIFOs; should be done after disabling the TX/RX */
    UART_HAL_SetTxFifoCmd(baseAddr, false);
    UART_HAL_SetRxFifoCmd(baseAddr, false);
    UART_HAL_FlushTxFifo(baseAddr);
    UART_HAL_FlushRxFifo(baseAddr);
#endif

    /* Cleared state pointer. */
    g_uartStatePtr[instance] = NULL;

    /* Gate UART module clock */
    CLOCK_SYS_DisableUartClock(instance);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_InstallRxCallback
 * Description   : Install receive data callback function.
 *
 *END**************************************************************************/
uart_rx_callback_t UART_DRV_InstallRxCallback(uint32_t instance, 
                                              uart_rx_callback_t function, 
                                              void * callbackParam)
{
    assert(instance < HW_UART_INSTANCE_COUNT);
    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];
    
    uart_rx_callback_t currentCallback = uartState->rxCallback;
    uartState->rxCallback = function;
    uartState->rxCallbackParam = callbackParam;

    return currentCallback;
}

/********************* Private Functions -- START -- **************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_CompleteSendData
 * Description   : Finish up a transmit by completing the process of sending 
 * data and disabling the interrupt. 
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void UART_DRV_CompleteSendData(uint32_t instance)
{
    assert(instance < HW_UART_INSTANCE_COUNT);

    uint32_t baseAddr = g_uartBaseAddr[instance];
    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];

    /* Disable the transmitter data register empty interrupt */
    UART_HAL_SetTxDataRegEmptyIntCmd(baseAddr, false);

    /* Signal the synchronous completion object. */
    if (uartState->isTxBlocking)
    {
        OSA_SemaPost(&uartState->txIrqSync);
    }

    /* Update the information of the module driver state */
    uartState->isTxBusy = false; 
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_StartSendData
 * Description   : Initiate (start) a transmit by beginning the process of
 * sending data and enabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static uart_status_t UART_DRV_StartSendData(uint32_t instance,
                                            const uint8_t * txBuff,
                                            uint32_t txSize)
{
    assert(instance < HW_UART_INSTANCE_COUNT);

    uint32_t baseAddr = g_uartBaseAddr[instance];
    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];

    /* Check that we're not busy already transmitting data from a previous function call. */
    if (uartState->isTxBusy)
    {
        return kStatus_UART_TxBusy;
    }

    /* Initialize the module driver state structure. */
    uartState->txBuff = txBuff;
    uartState->txSize = txSize;
    uartState->isTxBusy = true;

    /* Fill the TX FIFO or TX data buffer. In the event that there still might be data in the
     * TX FIFO, first ascertain the nubmer of empty spaces and then fill those up. */
    uint8_t emptyEntryCountInFifo;
#if FSL_FEATURE_UART_HAS_FIFO
    emptyEntryCountInFifo = uartState->txFifoEntryCount -
                            UART_HAL_GetTxDatawordCountInFifo(baseAddr);
#else
    /* For modules that don't have a FIFO, there is no FIFO data count register */
    emptyEntryCountInFifo = uartState->txFifoEntryCount;
    /* Make sure the transmit data register is empty and ready for data */
    while(!UART_HAL_IsTxDataRegEmpty(baseAddr)) { }
#endif

    /* Fill up FIFO, if only a 1-entry FIFO, then just fill the data buffer */
    while(emptyEntryCountInFifo--)
    {
        /* put data into FIFO */
        UART_HAL_Putchar(baseAddr, *(uartState->txBuff)); 
        ++uartState->txBuff;
        --uartState->txSize;
        /* If there are no more bytes in the buffer to send, then complete transmit. No need
         * to spend time enabling the interrupt and going to the ISR.  */
        if (uartState->txSize == 0)
        {
            UART_DRV_CompleteSendData(instance);
            return kStatus_UART_Success;
        }
    }

    /* Enable the transmitter data register empty interrupt. The TDRE flag will set whenever
     * the TX buffer is emptied into the TX shift register (for non-FIFO IPs) or when the
     * data in the TX FIFO is at or below the programmed watermark (for FIFO-supported IPs). */
    UART_HAL_SetTxDataRegEmptyIntCmd(baseAddr, true);

    return kStatus_UART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_CompleteReceiveData
 * Description   : Finish up a receive by completing the process of receiving data
 * and disabling the interrupt. 
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void UART_DRV_CompleteReceiveData(uint32_t instance)
{
    assert(instance < HW_UART_INSTANCE_COUNT);

    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];
    uint32_t baseAddr = g_uartBaseAddr[instance];

    /* Disable receive data full interrupt */
    UART_HAL_SetRxDataRegFullIntCmd(baseAddr, false);

    /* Signal the synchronous completion object. */
    if (uartState->isRxBlocking)
    {
        OSA_SemaPost(&uartState->rxIrqSync);
    }

    /* Update the information of the module driver state */
    uartState->isRxBusy = false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_StartReceiveData
 * Description   : Initiate (start) a receive by beginning the process of
 * receiving data and enabling the interrupt. 
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static uart_status_t UART_DRV_StartReceiveData(uint32_t instance, uint8_t * rxBuff,
                                 uint32_t rxSize)
{
    assert(instance < HW_UART_INSTANCE_COUNT);

    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];
    uint32_t baseAddr = g_uartBaseAddr[instance];

    /* Check that we're not busy already receiving data from a previous function call. */
    if (uartState->isRxBusy)
    {
        return kStatus_UART_RxBusy;
    }

    /* Initialize the module driver state struct to indicate transfer in progress
     * and with the buffer and byte count data */
    uartState->rxBuff = rxBuff;
    uartState->rxSize = rxSize;
    uartState->isRxBusy = true;

    /* enable the receive data full interrupt */
    UART_HAL_SetRxDataRegFullIntCmd(baseAddr, true);

    return kStatus_UART_Success;
}

/*********************** Private Functions -- END -- **************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SendDataBlocking
 * Description   : This function sends (transmits) data out through the UART module using a
 *                 blocking method.
 * A blocking (also known as synchronous) function means that the function does not return until
 * the transmit is complete. This blocking function is used to send data through the UART port.
 *
 *END**************************************************************************/
uart_status_t UART_DRV_SendDataBlocking(uint32_t instance,
                                        const uint8_t * txBuff,
                                        uint32_t txSize,
                                        uint32_t timeout)
{
    assert(txBuff);
    assert(instance < HW_UART_INSTANCE_COUNT);

    uint32_t baseAddr = g_uartBaseAddr[instance];
    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];
    uart_status_t error = kStatus_UART_Success;
    osa_status_t syncStatus;

    /* Indicates current transaction is blocking.*/
    uartState->isTxBlocking = true;

    /* Start the transmission process */
    if (UART_DRV_StartSendData(instance, txBuff, txSize) == kStatus_UART_TxBusy)
    {
        return kStatus_UART_TxBusy;
    }

    /* Wait until the transmit is complete. */
    do
    {
        syncStatus = OSA_SemaWait(&uartState->txIrqSync, timeout);
    }while(syncStatus == kStatus_OSA_Idle);

    if (syncStatus != kStatus_OSA_Success)
    {
        /* Abort the transfer so it doesn't continue unexpectedly.*/
        UART_DRV_AbortSendingData(instance);
        error = kStatus_UART_Timeout;
    }

#if FSL_FEATURE_UART_HAS_FIFO
    /* Wait until the TX FIFO is empty before returning. However, do not wait until the TX
     * is complete (when all data is shifted from the TX shift register). The reason is, this may
     * prevent us from exiting in time to queue up more data into the TX FIFO or TX Buffer for
     * another transmission. */
    while(UART_HAL_GetTxDatawordCountInFifo(baseAddr) != 0) { }
#endif

    return error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SendData
 * Description   : This function sends (transmits) data through the UART module using a
 *                 non-blocking method.
 * A non-blocking (also known as asynchronous) function means that the function returns
 * immediately after initiating the transmit function. The application has to get the
 * transmit status to see when the transmit is complete. In other words, after calling non-blocking
 * (asynchronous) send function, the application must get the transmit status to check if transmit
 * is completed or not.
 * The asynchronous method of transmitting and receiving allows the UART to perform a full duplex
 * operation (simultaneously transmit and receive).
 *
 *END**************************************************************************/
uart_status_t UART_DRV_SendData(uint32_t instance,
                                const uint8_t * txBuff,
                                uint32_t txSize)
{
    assert(txBuff);
    assert(instance < HW_UART_INSTANCE_COUNT);

    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];

    /* Indicates current transaction is non-blocking.*/
    uartState->isTxBlocking = false;

    /* Start the transmission process*/
    if (UART_DRV_StartSendData(instance, txBuff, txSize) == kStatus_UART_TxBusy)
    {
        return kStatus_UART_TxBusy;
    }

    return kStatus_UART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_GetTransmitStatus
 * Description   : This function returns whether the previous UART transmit has finished.
 * When performing an async transmit, the user can call this function to ascertain the state of the
 * current transmission: in progress (or busy) or complete (success). In addition, if the
 * transmission is still in progress, the user can obtain the number of words that have been
 * currently transferred.
 *
 *END**************************************************************************/
uart_status_t UART_DRV_GetTransmitStatus(uint32_t instance, uint32_t * bytesRemaining)
{
    assert(instance < HW_UART_INSTANCE_COUNT);

    uint32_t baseAddr = g_uartBaseAddr[instance];
    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];

    /* Fill in the bytes transferred. This may return that all bytes were transmitted,
     * however, for IPs with FIFO support, there still may be data in the TX FIFO still
     * in the process of being transmitted. */
    if (bytesRemaining)
    {
        *bytesRemaining = uartState->txSize;
    }

    /* Return kStatus_UART_TxBusy or kStatus_UART_Success depending on whether or not
     * the UART has a FIFO. If it does have a FIFO, we'll need to wait until the FIFO is
     * completely drained before indicating success in addition to isTxBusy = 0.
     * If there is no FIFO, then we need to only worry about isTxBusy. */
#if FSL_FEATURE_UART_HAS_FIFO
    if ((uartState->isTxBusy == false) && (UART_HAL_GetTxDatawordCountInFifo(baseAddr) == 0))
    {
        return kStatus_UART_Success; /* No more data to send, FIFO is empty */
    }
    else
    {
        return kStatus_UART_TxBusy; /* Either more data to send, or FIFO has data */
    }
#else
    return (uartState->isTxBusy ? kStatus_UART_TxBusy : kStatus_UART_Success);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_AbortSendingData
 * Description   : This function terminates an asynchronous UART transmission early.
 * During an async UART transmission, the user has the option to terminate the transmission early
 * if the transmission is still in progress.
 *
 *END**************************************************************************/
uart_status_t UART_DRV_AbortSendingData(uint32_t instance)
{
    assert(instance < HW_UART_INSTANCE_COUNT);

    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];

    /* Check if a transfer is running. */
    if (!uartState->isTxBusy)
    {
        return kStatus_UART_NoTransmitInProgress;
    }

    /* Stop the running transfer. */
    UART_DRV_CompleteSendData(instance);

    return kStatus_UART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_ReceiveDataBlocking
 * Description   : This function gets (receives) data from the UART module using a blocking method.
 * A blocking (also known as synchronous) function means that the function does not return until
 * the receive is complete. This blocking function is used to send data through the UART port.
 *
 *END**************************************************************************/
uart_status_t UART_DRV_ReceiveDataBlocking(uint32_t instance, uint8_t * rxBuff,
                                           uint32_t rxSize, uint32_t timeout)
{
    assert(rxBuff);
    assert(instance < HW_UART_INSTANCE_COUNT);

    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];
    uart_status_t error = kStatus_UART_Success;
    osa_status_t syncStatus;

    /* Indicates current transaction is blocking.*/
    uartState->isRxBlocking = true;

    if (UART_DRV_StartReceiveData(instance, rxBuff, rxSize) == kStatus_UART_RxBusy)
    {
        return kStatus_UART_RxBusy;
    }

    /* Wait until all the data is received or for timeout.*/
    do
    {
        syncStatus = OSA_SemaWait(&uartState->rxIrqSync, timeout);
    }while(syncStatus == kStatus_OSA_Idle);

    if (syncStatus != kStatus_OSA_Success)
    {
        /* Abort the transfer so it doesn't continue unexpectedly.*/
        UART_DRV_AbortReceivingData(instance);
        error = kStatus_UART_Timeout;
    }

    return error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_ReceiveData
 * Description   : This function gets (receives) data from the UART module using a non-blocking
 *                 method.
 * A non-blocking (also known as synchronous) function means that the function returns
 * immediately after initiating the receive function. The application has to get the
 * receive status to see when the receive is complete. In other words, after calling non-blocking
 * (asynchronous) get function, the application must get the receive status to check if receive
 * is completed or not.
 * The asynchronous method of transmitting and receiving allows the UART to perform a full duplex
 * operation (simultaneously transmit and receive).
 *
 *END**************************************************************************/
uart_status_t UART_DRV_ReceiveData(uint32_t instance, uint8_t * rxBuff, uint32_t rxSize)
{
    assert(rxBuff);
    assert(instance < HW_UART_INSTANCE_COUNT);

    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];

    /* Indicates current transaction is non-blocking.*/
    uartState->isRxBlocking = false;

    if (UART_DRV_StartReceiveData(instance, rxBuff, rxSize) == kStatus_UART_RxBusy)
    {
        return kStatus_UART_RxBusy;
    }

    return kStatus_UART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_GetReceiveStatus
 * Description   : This function returns whether the previous UART receive is complete.
 * When performing an async receive, the user can call this function to ascertain the state of the
 * current receive progress: in progress (or busy) or complete (success). In addition, if the
 * receive is still in progress, the user can obtain the number of words that have been
 * currently received.
 *
 *END**************************************************************************/
uart_status_t UART_DRV_GetReceiveStatus(uint32_t instance, uint32_t * bytesRemaining)
{
    assert(instance < HW_UART_INSTANCE_COUNT);
    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];

    /* Fill in the bytes transferred. */
    if (bytesRemaining)
    {
        *bytesRemaining = uartState->rxSize;
    }

    return (uartState->isRxBusy ? kStatus_UART_RxBusy : kStatus_UART_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_AbortReceivingData
 * Description   : This function shuts down the UART by disabling interrupts and the
 *                 transmitter/receiver.
 * This function disables the UART interrupts, disables the transmitter and receiver, and
 * flushes the FIFOs (for modules that support FIFOs).
 *
 *END**************************************************************************/
uart_status_t UART_DRV_AbortReceivingData(uint32_t instance)
{
    assert(instance < HW_UART_INSTANCE_COUNT);
    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];

    /* Check if a transfer is running. */
    if (!uartState->isRxBusy)
    {
        return kStatus_UART_NoReceiveInProgress;
    }

    /* Stop the running transfer. */
    UART_DRV_CompleteReceiveData(instance);

    return kStatus_UART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_IRQHandler
 * Description   : Interrupt handler for UART.
 * This handler uses the buffers stored in the uart_state_t structs to transfer data.
 * This is not a public API as it is called whenever an interrupt occurs.
 *
 *END**************************************************************************/
void UART_DRV_IRQHandler(uint32_t instance)
{
    uart_state_t * uartState = (uart_state_t *)g_uartStatePtr[instance];
    uint32_t baseAddr = g_uartBaseAddr[instance];
    bool rxCallbackEnd = false;

    /* Exit the ISR if no transfer is happening for this instance. */
    if ((!uartState->isTxBusy) && (!uartState->isRxBusy))
    {
        return;
    }

    /* Check to see if the interrupt is due to receive data full
     * first see if the interrupt is enabled. */
    if(UART_HAL_GetRxDataRegFullIntCmd(baseAddr))
    {
        if(UART_HAL_IsRxDataRegFull(baseAddr))
        {
#if FSL_FEATURE_UART_HAS_FIFO
            /* Read from RX FIFO while the RX count indicates there's data in the FIFO.
             * Even though the watermark is set to 1, it might be possible to have more than one
             * byte in the FIFO, so lets make sure to drain it. */
            while(UART_HAL_GetRxDatawordCountInFifo(baseAddr))
            {
#endif
                /* Get data and put in receive buffer */
                UART_HAL_Getchar(baseAddr, uartState->rxBuff);

                /* Invoke callback if have one. */
                if (uartState->rxCallback != NULL)
                {
                    /* The callback will end the receiving early if not return
                     * kStatus_UART_Success. */
                    if (uartState->rxCallback(uartState->rxBuff, uartState->rxCallbackParam) !=
                            kStatus_UART_Success)
                    {
                        rxCallbackEnd = true;
                    }
                }

                ++uartState->rxBuff;  /* Increment the rxBuff pointer */
                --uartState->rxSize;  /* Decrement the byte count  */

                /* Check to see if this was the last byte received */
                if ((uartState->rxSize == 0) || rxCallbackEnd)
                {
                    /* Complete the transfer. This disables the interrupts, so we don't wind up in*/
                    /* the ISR again. */
                    UART_DRV_CompleteReceiveData(instance);
                    #if FSL_FEATURE_UART_HAS_FIFO
                    break;
                    #endif
                }
            
#if FSL_FEATURE_UART_HAS_FIFO
            }
#endif
        }
    }

    /* Check to see if the interrupt is due to transmit data register empty
     * first see if the interrupt is enabled. */
    if (UART_HAL_GetTxDataRegEmptyIntCmd(baseAddr))
    {
        if(UART_HAL_IsTxDataRegEmpty(baseAddr))
        {
            /* Check to see if there are any more bytes to send */
            if (uartState->txSize)
            {
                uint8_t emptyEntryCountInFifo;
#if FSL_FEATURE_UART_HAS_FIFO
                emptyEntryCountInFifo = uartState->txFifoEntryCount -
                                        UART_HAL_GetTxDatawordCountInFifo(baseAddr);
#else
                /* For modules that don't have a FIFO, there is no FIFO data count register */
                emptyEntryCountInFifo = uartState->txFifoEntryCount;
#endif

                /* Fill up FIFO, if only a 1-entry FIFO, then just fill the data buffer */
                while(emptyEntryCountInFifo--)
                {
                    /* Transmit data and update tx size/buff. */
                    UART_HAL_Putchar(baseAddr, *(uartState->txBuff));
                    ++uartState->txBuff; 
                    --uartState->txSize;

                    /* If there are no more bytes in the buffer to send, complete the transmit
                     * process and break out of the while loop. We should not re-enter the ISR again
                     * as all of the data has been put into the FIFO (or the TX data buffer). */
                    if (!uartState->txSize)
                    {
                        UART_DRV_CompleteSendData(instance);
                        break;
                    }
                }
            }
        }
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

