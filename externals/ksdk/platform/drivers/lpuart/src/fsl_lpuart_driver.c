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
#include "fsl_lpuart_driver.h"
#include "fsl_lpuart_common.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_Init
 * Description   : This function initializes a LPUART instance for operation.
 * This function will initialize the run-time state structure to keep track of the on-going
 * transfers, ungate the clock to the LPUART module, initialize the module
 * to user defined settings and default settings, configure the IRQ state structure and enable
 * the module-level interrupt to the core, and enable the LPUART module transmitter and receiver.
 * The following is an example of how to set up the lpuart_state_t and the
 * lpuart_user_config_t parameters and how to call the LPUART_DRV_Init function by passing
 * in these parameters:
 *    lpuart_user_config_t lpuartConfig;
 *    lpuartConfig.baudRate = 9600;
 *    lpuartConfig.bitCountPerChar = klpuart8BitsPerChar;
 *    lpuartConfig.parityMode = klpuartParityDisabled;
 *    lpuartConfig.stopBitCount = klpuartOneStopBit;
 *    lpuart_state_t lpuartState;
 *    LPUART_DRV_Init(instance, &lpuartState, &lpuartConfig);
 *
 *END**************************************************************************/
lpuart_status_t LPUART_DRV_Init(uint32_t instance, lpuart_state_t * lpuartStatePtr,
                                const lpuart_user_config_t * lpuartUserConfig)
{
    assert(lpuartStatePtr && lpuartUserConfig);
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    uint32_t lpuartSourceClock;
    uint32_t baseAddr = g_lpuartBaseAddr[instance];
    
    /* Exit if current instance is already initialized. */
    if (g_lpuartStatePtr[instance])
    {
        return kStatus_LPUART_Initialized;
    }

    /* Clear the state struct for this instance. */
    memset(lpuartStatePtr, 0, sizeof(lpuart_state_t));

    /* Save runtime structure pointer.*/
    g_lpuartStatePtr[instance] = lpuartStatePtr;

    /* ungate lpuart module clock */
    CLOCK_SYS_EnableLpuartClock(instance);

    /* initialize the LPUART instance */
    LPUART_HAL_Init(baseAddr);

    /* Init the interrupt sync object. */
    OSA_SemaCreate(&lpuartStatePtr->txIrqSync, 0);
    OSA_SemaCreate(&lpuartStatePtr->rxIrqSync, 0);

    /* LPUART clock source is either system clock or bus clock depending on the instance */
    lpuartSourceClock = CLOCK_SYS_GetLpuartFreq(instance);

    /* initialize the parameters of the LPUART config structure with desired data */
    LPUART_HAL_SetBaudRate(baseAddr, lpuartSourceClock, lpuartUserConfig->baudRate);
    LPUART_HAL_SetBitCountPerChar(baseAddr, lpuartUserConfig->bitCountPerChar);
    LPUART_HAL_SetParityMode(baseAddr, lpuartUserConfig->parityMode);
    LPUART_HAL_SetStopBitCount(baseAddr, lpuartUserConfig->stopBitCount);
    
    /* finally, enable the LPUART transmitter and receiver */
    LPUART_HAL_EnableTransmitter(baseAddr);
    LPUART_HAL_EnableReceiver(baseAddr);

    /* Enable LPUART interrupt. */
    INT_SYS_EnableIRQ(g_lpuartRxTxIrqId[instance]);
    
    return kStatus_LPUART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_Deinit
 * Description   : This function shuts down the UART by disabling interrupts and the
 *                 transmitter/receiver.
 * This function disables the UART interrupts, disables the transmitter and receiver, and
 * flushes the FIFOs (for modules that support FIFOs).
 *
 *END**************************************************************************/
void LPUART_DRV_Deinit(uint32_t instance)
{
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    uint32_t baseAddr = g_lpuartBaseAddr[instance];

    /* Wait until the data is completely shifted out of shift register */
    while (!LPUART_HAL_IsTxComplete(baseAddr)) {}

    /* Disable LPUART interrupt. */
    INT_SYS_DisableIRQ(g_lpuartRxTxIrqId[instance]);

    /* disable tx and rx */
    LPUART_HAL_DisableTransmitter(baseAddr);
    LPUART_HAL_DisableReceiver(baseAddr);

    /* Clear our saved pointer to the state structure */
    g_lpuartStatePtr[instance] = NULL;

    /* gate lpuart module clock */
    CLOCK_SYS_DisableLpuartClock(instance);
}

/********************* Private Functions -- START -- **************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_StartSendData
 * Description   : Initiate (start) a transmit by beginning the process of
 * sending data and enabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static lpuart_status_t LPUART_DRV_StartSendData(uint32_t instance, 
                                                const uint8_t * txBuff,
                                                uint32_t txSize)
{
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    uint32_t baseAddr = g_lpuartBaseAddr[instance];
    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];

    /* Check that we're not busy already transmitting data from a previous function call. */
    if (lpuartState->isTxBusy)
    {
        return kStatus_LPUART_TxBusy;
    }

    /* initialize the module driver state struct  */
    lpuartState->txBuff = txBuff;
    lpuartState->txSize = txSize;
    lpuartState->isTxBusy = true;

    /* Transmit the data */
    LPUART_HAL_Putchar(baseAddr, *(lpuartState->txBuff));
    ++lpuartState->txBuff;
    --lpuartState->txSize;

    /* enable transmission complete interrupt */
    LPUART_HAL_SetTxDataRegEmptyIntCmd(baseAddr, true);

    return kStatus_LPUART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_CompleteSendData
 * Description   : Finish up a transmit by completing the process of sending 
 * data and disabling the interrupt. 
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void LPUART_DRV_CompleteSendData(uint32_t instance)
{
    assert(instance < HW_UART_INSTANCE_COUNT);

    uint32_t baseAddr = g_lpuartBaseAddr[instance];
    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];

    /* disable transmission complete interrupt */
    LPUART_HAL_SetTxDataRegEmptyIntCmd(baseAddr, false);

    /* Signal the synchronous completion object. */
    if (lpuartState->isTxBlocking) 
    {
        OSA_SemaPost(&lpuartState->txIrqSync);
    }

    /* Update the information of the module driver state */
    lpuartState->isTxBusy = false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_StartReceiveData
 * Description   : Initiate (start) a receive by beginning the process of
 * receiving data and enabling the interrupt. 
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static lpuart_status_t LPUART_DRV_StartReceiveData(uint32_t instance, 
                                                   uint8_t * rxBuff,
                                                   uint32_t rxSize)
{
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];
    uint32_t baseAddr = g_lpuartBaseAddr[instance];

    /* Check that we're not busy already receiving data from a previous function call. */
    if (lpuartState->isRxBusy)
    {
        return kStatus_LPUART_RxBusy;
    }

    /* initialize the module driver state struct to indicate transfer in progress 
     * and with the buffer and byte count data. */
    lpuartState->isRxBusy = true;
    lpuartState->rxBuff = rxBuff;
    lpuartState->rxSize = rxSize;

    /* enable the receive data full interrupt */
    LPUART_HAL_SetRxDataRegFullIntCmd(baseAddr, true);

    /* As the LPUART does not have FIFO, so it's easy that the receive run into
     * overrun status. So in the LPUART driver, we should enable the overrun
     * interrupt, and clear the overrun flag when it happen. */
    LPUART_HAL_SetIntMode(baseAddr, kLpuartIntRxOverrun, true);

    return kStatus_LPUART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_CompleteReceiveData
 * Description   : Finish up a receive by completing the process of receiving data
 * and disabling the interrupt. 
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void LPUART_DRV_CompleteReceiveData(uint32_t instance)
{
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];
    uint32_t baseAddr = g_lpuartBaseAddr[instance];

    /* disable receive data full and rx overrun interrupt. */
    LPUART_HAL_SetRxDataRegFullIntCmd(baseAddr, false);
    LPUART_HAL_SetIntMode(baseAddr, kLpuartIntRxOverrun, true);

    /* Signal the synchronous completion object. */
    if (lpuartState->isRxBlocking) 
    {
        OSA_SemaPost(&lpuartState->rxIrqSync);
    }

    /* Update the information of the module driver state */
    lpuartState->isRxBusy = false; 
}

/*********************** Private Functions -- END -- **************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_SendDataBlocking
 * Description   : This function sends (transmits) data out through the LPUART module using a
 *                 blocking method.
 * A blocking (also known as synchronous) function means that the function does not return until
 * the transmit is complete. This blocking function is used to send data through the LPUART port.
 *
 *END**************************************************************************/
lpuart_status_t LPUART_DRV_SendDataBlocking(uint32_t instance, 
                                            const uint8_t * txBuff, 
                                            uint32_t txSize,
                                            uint32_t timeout)
{
    assert(txBuff);
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];
    osa_status_t syncStatus;

    /* Indicates this is a blocking transaction. */
    lpuartState->isTxBlocking = true;
    
    /* start the transmission process */
    if (LPUART_DRV_StartSendData(instance, txBuff, txSize) == kStatus_LPUART_TxBusy)
    {
        return kStatus_LPUART_TxBusy;
    }

    /* Wait until the transmit is complete. */
    do
    {
        syncStatus = OSA_SemaWait(&lpuartState->txIrqSync, timeout);
    } while(syncStatus == kStatus_OSA_Idle);

    if (syncStatus != kStatus_OSA_Success)
    {
        return kStatus_LPUART_Timeout;
    }
    
    return kStatus_LPUART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_StartSendData
 * Description   : Initiate (start) a transmit by beginning the process of
 * sending data and enabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
lpuart_status_t LPUART_DRV_SendData(uint32_t instance,
                                    const uint8_t * txBuff,
                                    uint32_t txSize)
{
    assert(txBuff);
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];

    /* Indicates this is a non-blocking transaction. */
    lpuartState->isTxBlocking = false;

    /* start the transmission process */
    if (LPUART_DRV_StartSendData(instance, txBuff, txSize) == kStatus_LPUART_TxBusy)
    {
        return kStatus_LPUART_TxBusy;
    }

    return kStatus_LPUART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_GetTransmitStatus
 * Description   : This function returns whether the previous LPUART transmit has finished.
 * When performing an async transmit, the user can call this function to ascertain the state of the
 * current transmission: in progress (or busy) or complete (success). In addition, if the
 * transmission is still in progress, the user can obtain the number of words that have been
 * currently transferred.
 *
 *END**************************************************************************/
lpuart_status_t LPUART_DRV_GetTransmitStatus(uint32_t instance, uint32_t * bytesRemaining)
{
    assert(instance < HW_UART_INSTANCE_COUNT);
    
    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];

    /* Fill in the bytes transferred. */
    if (bytesRemaining)
    {
        *bytesRemaining = lpuartState->txSize;
    }

    return (lpuartState->isTxBusy ? kStatus_LPUART_TxBusy : kStatus_LPUART_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_AbortSendingData
 * Description   : This function terminates an asynchronous LPUART transmission early.
 * During an async LPUART transmission, the user has the option to terminate the transmission early
 * if the transmission is still in progress.
 *
 *END**************************************************************************/
lpuart_status_t LPUART_DRV_AbortSendingData(uint32_t instance)
{
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];

    /* Check if a transfer is running. */
    if (!lpuartState->isTxBusy)
    {
        return kStatus_LPUART_NoTransmitInProgress;
    }

    /* Stop the running transfer. */
    LPUART_DRV_CompleteSendData(instance);

    return kStatus_LPUART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_ReceiveDataBlocking
 * Description   : This function gets (receives) data from the LPUART module using a blocking method.
 * A blocking (also known as synchronous) function means that the function does not return until
 * the receive is complete. This blocking function is used to send data through the LPUART port.
 *
 *END**************************************************************************/
lpuart_status_t LPUART_DRV_ReceiveDataBlocking(uint32_t instance,
                                               uint8_t * rxBuff,
                                               uint32_t rxSize,
                                               uint32_t timeout)
{
    assert(rxBuff);
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];
    osa_status_t syncStatus;

    /* Indicates this is a blocking transaction. */
    lpuartState->isRxBlocking = true;

    if (LPUART_DRV_StartReceiveData(instance, rxBuff, rxSize) == kStatus_LPUART_RxBusy)
    {
        return kStatus_LPUART_RxBusy;
    }

    /* Wait until the receive is complete. */
    do
    {
        syncStatus = OSA_SemaWait(&lpuartState->rxIrqSync, timeout);
    } while(syncStatus == kStatus_OSA_Idle);

    if (syncStatus != kStatus_OSA_Success)
    {
        return kStatus_LPUART_Timeout;
    }
    
    return kStatus_LPUART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_ReceiveData
 * Description   : This function gets (receives) data from the LPUART module using a non-blocking
 *                 method.
 * A non-blocking (also known as synchronous) function means that the function returns
 * immediately after initiating the receive function. The application has to get the
 * receive status to see when the receive is complete. In other words, after calling non-blocking
 * (asynchronous) get function, the application must get the receive status to check if receive
 * is completed or not.
 * The asynchronous method of transmitting and receiving allows the LPUART to perform a full duplex
 * operation (simultaneously transmit and receive).
 *
 *END**************************************************************************/
lpuart_status_t LPUART_DRV_ReceiveData(uint32_t instance,
                                       uint8_t * rxBuff,
                                       uint32_t rxSize)
{
    assert(rxBuff);
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];

    /* Indicates this is a non-blocking transaction. */
    lpuartState->isRxBlocking = false;

    if (LPUART_DRV_StartReceiveData(instance, rxBuff, rxSize) == kStatus_LPUART_RxBusy)
    {
        return kStatus_LPUART_RxBusy;
    }

    return kStatus_LPUART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_GetReceiveStatus
 * Description   : This function returns whether the previous LPUART receive is complete.
 * When performing an async receive, the user can call this function to ascertain the state of the
 * current receive progress: in progress (or busy) or complete (success). In addition, if the
 * receive is still in progress, the user can obtain the number of words that have been
 * currently received.
 *
 *END**************************************************************************/
lpuart_status_t LPUART_DRV_GetReceiveStatus(uint32_t instance, uint32_t * bytesRemaining)
{
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];

    /* Fill in the bytes transferred. */
    if (bytesRemaining)
    {
        *bytesRemaining = lpuartState->rxSize;
    }

    return (lpuartState->isRxBusy ? kStatus_LPUART_RxBusy : kStatus_LPUART_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_AbortReceivingData
 * Description   : This function shuts down the LPUART by disabling interrupts and the
 *                 transmitter/receiver.
 * This function disables the LPUART interrupts, disables the transmitter and receiver, and
 * flushes the FIFOs (for modules that support FIFOs).
 *
 *END**************************************************************************/
lpuart_status_t LPUART_DRV_AbortReceivingData(uint32_t instance)
{
    assert(instance < HW_LPUART_INSTANCE_COUNT);

    lpuart_state_t * lpuartState = (lpuart_state_t *)g_lpuartStatePtr[instance];

    /* Check if a transfer is running. */
    if (!lpuartState->isRxBusy)
    {
        return kStatus_LPUART_NoReceiveInProgress;
    }

    /* Stop the running transfer. */
    LPUART_DRV_CompleteReceiveData(instance);

    return kStatus_LPUART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_DRV_IRQHandler
 * Description   : Interrupt handler for LPUART.
 * This handler uses the buffers stored in the lpuart_state_t structs to transfer data.
 * This is not a public API as it is called whenever an interrupt occurs.
 *
 *END**************************************************************************/
void LPUART_DRV_IrqHandler(uint32_t instance)
{
    lpuart_state_t * lpuartState = g_lpuartStatePtr[instance];
    uint32_t baseAddr = g_lpuartBaseAddr[instance];

    /* Exit the ISR if no transfer is happening for this instance. */
    if ((!lpuartState->isTxBusy) && (!lpuartState->isRxBusy))
    {
        return;
    }

    /* check to see if the interrupt is due to transmit complete */
    /* first see if the interrupt is enabled */
    if (LPUART_HAL_GetTxDataRegEmptyIntCmd(baseAddr))
    {
        if(LPUART_HAL_IsTxDataRegEmpty(baseAddr))
        {
            /* check to see if there are any more bytes to send */
            if (lpuartState->txSize)
            {
                /* Transmit the data */
               LPUART_HAL_Putchar(baseAddr, *(lpuartState->txBuff)); 
               ++lpuartState->txBuff;
               --lpuartState->txSize; 
            }
            else
            {
                /* We're done with this transfer. Complete the transfer.
                 * This disables the interrupts, so we don't wind up in the ISR again. */
                LPUART_DRV_CompleteSendData(instance);
            }
        }
    }

    /* check to see if the interrupt is due to receive data full */
    /* first see if the interrupt is enabled */
    if(LPUART_HAL_GetRxDataRegFullIntCmd(baseAddr))
    {
        if(LPUART_HAL_IsRxDataRegFull(baseAddr))
        {
            /* get data and put in receive buffer  */
            LPUART_HAL_Getchar(baseAddr, lpuartState->rxBuff);
            ++lpuartState->rxBuff;
            --lpuartState->rxSize;

            /* Check to see if this was the last byte received */
            if ((lpuartState->rxSize == 0))
            {
                /* Complete the transfer. This disables the interrupts, so we don't wind up in */
                /* the ISR again. */
                LPUART_DRV_CompleteReceiveData(instance);
            }
        }
    }

    /* Clear LPUART rx overrun flag. */
    if (LPUART_HAL_GetStatusFlag(baseAddr, kLpuartRxOverrun))
    {
        LPUART_HAL_ClearStatusFlag(baseAddr, kLpuartRxOverrun);
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

