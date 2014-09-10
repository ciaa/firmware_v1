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

#include <string.h>
#include <assert.h>
#include "fsl_dspi_common.h"
#include "fsl_dspi_slave_driver.h"
#include "fsl_dspi_shared_function.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! SPI slave constants */
enum _spi_slave_constants
{
    kEmptyChar = 0,                        /*!< Empty character */
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
typedef void (*dspiSlaveDone)(void);

void DSPI_DRV_SlaveDmaCallback(void *param, edma_chn_status_t chanStatus);

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_DRV_SlaveDmaCallback
 * Description   : This function is called when the eDMA generates an interrupt.
 * The eDMA generates an interrupt when the channel is "done", meaning that the
 * expected number of bytes have been transferred.  When the interrupt occurs,
 * the eDMA will jump to this callback as it was registered in the eDMA register
 * callback service function.  The user will defined their own callback function
 * to take whatever action they deem necessary for handling the end of a transfer.
 * For example, the user may simply want their callback function to set a global
 * flag to indicate that the transfer is complete.  The user defined callback
 * is passed in through the "param" parameter.
 * The parameter chanStatus is currently not used.
 *
 *END**************************************************************************/
void DSPI_DRV_SlaveDmaCallback(void *param, edma_chn_status_t chanStatus)
{
    dspiSlaveDone doneFunction;

    /* Due to MISRA 11.1 rule:
     * Conversions shall not be performed between a pointer to a function
     * and any type other than an integral type.
     * We first have to typecast "param" as a uint32_t before typecasting as
     * a void function pointer.
     */
    uint32_t paramAsInt = (uint32_t)(param);

    if (param != NULL)
    {
        /* Define "doneFunction" as a pointer to the user defined callback function passed
         * in through the parameter "param".  Because param is a void pointer, we need to
         * typecast it as a function pointer.  The typedef definition of the function pointer
         * is defined previously as "typedef void (*dspiSlaveDone)(void);"
         * However, becasue of MISRA 11.1, we had to typecast param as an integral so that it
         * is re-named paramAsInt
         */
        doneFunction = (dspiSlaveDone)(paramAsInt);
        doneFunction(); /* Jump to the user defined callback function */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_DRV_SlaveInitDma
 * Description   : Initialize a DSPI instance for slave mode operation with DMA support.
 * This function is exactly like the DSPI_DRV_SlaveInit function but in addition, adds DMA support.
 * This function saves the callbacks to the run-time state structure for later use in the
 * interrupt handler. However, unlike the CPU driven slave driver, there is no need to define
 * callbacks for the data sink or data source since the user will simply pass in buffers
 * for the send and receive data, and the DMA engine will use those buffers.
 * An onError callback is needed to service potential errors seen during a TX FIFO underflow or
 * RX FIFO overflow.
 * The user also passes in a user defined callback for handling end of transfers (dspiDone).
 * These callbacks are set in the dspi_slave_callbacks_t structure which is part of the
 * dspi_slave_user_config_t structure.  See example below.
 * This function also ungates the clock to the DSPI module, initializes the DSPI
 * for slave mode, enables the module and corresponding interrupts and sets up the DMA channels.
 * Once initialized, the DSPI module is configured in slave mode and ready to receive data from a
 * SPI master. The following is an example of how to set up the dspi_slave_state_t and the
 * dspi_slave_user_config_t parameters and how to call the DSPI_DRV_SlaveInit function by passing
 * in these parameters:
 *   instance = slaveInstance; <- the desired module instance number
 *   dspi_slave_state_t dspiSlaveState; <- the user simply allocates memory for this struct
 *   dspi_slave_user_config_t slaveUserConfig;
 *   slaveUserConfig.callbacks.onError = on_error; <- set to user implementation of function
 *   slaveUserConfig.callbacks.dspiDone = dspi_Dma_Done; <- user defined callback
 *   slaveUserConfig.dataConfig.bitsPerFrame = 16; <- example, can be 4 to 32
 *   slaveUserConfig.dataConfig.clkPhase = kDspiClockPhase_FirstEdge; <- example setting
 *   slaveUserConfig.dataConfig.clkPolarity = kDspiClockPolarity_ActiveHigh; <- example setting
 *   sendBuffer <- (pointer) to the source data buffer, can be NULL
 *   receiveBuffer <- (pointer) to the receive data buffer, can be NULL
 *   transferCount <- number of bytes to transfer
 *
 *   DSPI_DRV_SlaveInitDma(slaveInstance, &slaveUserConfig, &dspiSlaveState,
 *                         &sendBuffer, &receiveBuffer, transferCount);
 *
 *END**************************************************************************/
dspi_status_t DSPI_DRV_SlaveInitDma(uint32_t instance,
                                  dspi_slave_state_t * dspiState,
                                  const dspi_slave_user_config_t * slaveConfig,
                                  const uint8_t * sendBuffer,
                                  uint8_t * receiveBuffer,
                                  size_t transferByteCount)
{
    assert(slaveConfig);
    assert(instance < HW_SPI_INSTANCE_COUNT);

    uint32_t dmaInstance, dmaChannel; /* For temporarily storing DMA instance and channel */
    uint32_t majorIteration; /* Number of major iteration loop count used by the eDMA */
    uint32_t baseAddr = g_dspiBaseAddr[instance];

    dspi_status_t errorCode = kStatus_DSPI_Success;

    /* Clear the run-time state struct for this instance. */
    memset(dspiState, 0, sizeof(* dspiState));

    /* Save the application info. */
    dspiState->callbacks = slaveConfig->callbacks;

    /* configure the run-time state struct with the number of bits/frame */
    dspiState->bitsPerFrame = slaveConfig->dataConfig.bitsPerFrame;

    /* Set the DSPI run-time state struct flag to indicate it will use the DMA */
    dspiState->useDma = true;

    /* Enable clock for DSPI */
    CLOCK_SYS_EnableSpiClock(instance);

    /* Reset the DSPI module, which also disables the DSPI module */
    DSPI_HAL_Init(baseAddr);

    /* Set to slave mode.*/
    DSPI_HAL_SetMasterSlaveMode(baseAddr, kDspiSlave);

    errorCode = DSPI_HAL_SetDataFormat(baseAddr, kDspiCtar0, &slaveConfig->dataConfig);

    /* Enable fifo operation (regardless of FIFO depth) */
    DSPI_HAL_SetFifoCmd(baseAddr, true, true);

    /* DSPI system enable */
    DSPI_HAL_Enable(baseAddr);

    /* flush the fifos*/
    DSPI_HAL_SetFlushFifoCmd(baseAddr, true, true);

    /* Configure IRQ state structure, so irq handler can point to the correct state structure */
    g_dspiStatePtr[instance] = dspiState;

    /*************************************************************************
     * Set up DMA channel for RX FIFO only if user passes in a receive buffer
     *************************************************************************/
    if (receiveBuffer)
    {
        /***********************************
         * Request DMA channels for RX FIFO
         * This channel transfers data from RX FIFO to receiveBuffer
         ***********************************/
        /* Request channel for receive */
        EDMA_DRV_RequestChannel(kEDMAAnyChannel, kDmaRequestMux0SPI0Rx,
                                &dspiState->dmaRxFifo2RxBuff);

        /* Set up Transfer Control Descriptor for RX DMA channel */
        /* For each transfer control descriptor set up, save off DMA instance and channel number
         * to simplified variable names to make code cleaner
         */
        dmaInstance = VIRTUAL_CHN_TO_EDMA_MODULE_REGBASE(dspiState->dmaRxFifo2RxBuff.channel);
        dmaChannel = VIRTUAL_CHN_TO_EDMA_CHN(dspiState->dmaRxFifo2RxBuff.channel);

        /* Source addr, RX FIFO */
        EDMA_HAL_HTCDSetSrcAddr(dmaInstance, dmaChannel,
                                HW_SPI_POPR_ADDR(baseAddr));

        /* Source addr offset is 0 as source addr never increments */
        EDMA_HAL_HTCDSetSrcOffset(dmaInstance, dmaChannel, 0);

        /* Source address adjust last: don't increment source address, it is constant */
        EDMA_HAL_HTCDSetSrcLastAdjust(dmaInstance, dmaChannel, 0);

        /* Destination is the receive buffer */
        EDMA_HAL_HTCDSetDestAddr(dmaInstance, dmaChannel, (uint32_t)(receiveBuffer));

        /* Dest addr offset, always increment to the next byte */
        EDMA_HAL_HTCDSetDestOffset(dmaInstance, dmaChannel, 1);

        /* No adjustment needed for destination addr for most bits/frame. This field gets
         * updated for the special case of 24-bits/frame
         */
        EDMA_HAL_HTCDSetDestLastAdjust(dmaInstance, dmaChannel, 0);

        /* The source and destination attributes (bit size) depends on bits/frame setting */
        if (dspiState->bitsPerFrame <= 8)
        {
            /* Destination size is always one byte, source size varies depending on bits/frame */
            EDMA_HAL_HTCDSetAttribute(dmaInstance, dmaChannel,
                    kEDMAModuloDisable, kEDMAModuloDisable,
                    kEDMATransferSize_1Bytes, kEDMATransferSize_1Bytes);

            /* Transfer 1 byte from RX FIFO to receive buffer */
            EDMA_HAL_HTCDSetNbytes(dmaInstance, dmaChannel, 1);

            /* Adjust majorIteration to 1 byte per transfer */
            majorIteration = transferByteCount;
        }
        else if (dspiState->bitsPerFrame <= 16)
        {
            /* Source size is two bytes */
            /* Destination size is always one byte, source size varies depending on bits/frame */
            EDMA_HAL_HTCDSetAttribute(dmaInstance, dmaChannel,
                    kEDMAModuloDisable, kEDMAModuloDisable,
                    kEDMATransferSize_2Bytes, kEDMATransferSize_1Bytes);


            /* Transfer 2 bytes from RX FIFO to receive buffer */
            EDMA_HAL_HTCDSetNbytes(dmaInstance, dmaChannel, 2);

            /* Adjust majorIteration to 2 bytes per transfer */
            majorIteration = transferByteCount/2;
        }
/* For now, we will limit the user to use 1-, 2-, or 4-byte transfers. This implementation
 * of 3-byte transfers changes the eDMA overall operation to enable minorloop mapping which may
 * be undesireable to other drivers. For future implmentations, we will look into setting up
 * three linked channels: 1 channel to transfer a word from the RX FIFO to an intermediate buffer
 * another channel to transfer 2 bytes from the intermediate buffer to the recevie buffer and
 * finally another channel to transfer the last byte from the intermediate buffer to the
 * receive buffer
 */
#if 0
        else if (dspiState->bitsPerFrame <= 24)
        {
            /* Source size is actually 3 bytes (for up to 24-bits/frame), but we only have the
             * option to go up to a 4 byte (32-bit) source size, as 3 bytes is not a supported
             * setting.  That means we are always forced to transfer 4 bytes instead of the
             * desired 3 bytes. Later, we'll adjust the destination address to "go back one byte"
             * by using the minor loop offset feature of the eDMA. Note, source = RX FIFO and
             * destination = receive buffer.
             */
            edma_hal_htcd_configure_source_transfersize(dmaInstance, dmaChannel,
                                                        kEdmaTransferSize4bytes);
            /* Enable minor loop mapping CR[EMLM]=1 which enables the minor loop offset feature */
            edma_hal_set_minor_loop_mapping(dmaInstance, true);

            /* Set the destination minor loop offset enable and set the offset MLOFF to -1
             * As stated previously, we need the destination address to "go back one byte"
             * at the end of the minor loop transfer of 4 bytes.
             */
            edma_minorloop_offset_config_t minorLoopConfig;
            minorLoopConfig.isEnableDestMinorloop = true;
            minorLoopConfig.isEnableSourceMinorloop = false;
            minorLoopConfig.offset = -1;
            edma_hal_htcd_configure_minorloop_offset(dmaInstance, dmaChannel, minorLoopConfig);

            /* Set the minor loop transfer size to 4 bytes to align with the source size of
             * 4 bytes. Basically we are reading 4 bytes from the FIFO and writing to the
             * receive buffer.
             */
            edma_hal_htcd_configure_nbytes_minorloop_enabled_offset_enabled(dmaInstance,
                                                                            dmaChannel, 4);

            /* Adjust majorIteration to 3 bytes per transfer */
            majorIteration = transferByteCount/3;
        }
#endif
        else
        {
            /* Source size 4 bytes (32-bit) */
            /* Destination size is always one byte, source size varies depending on bits/frame */
            EDMA_HAL_HTCDSetAttribute(dmaInstance, dmaChannel,
                    kEDMAModuloDisable, kEDMAModuloDisable,
                    kEDMATransferSize_4Bytes, kEDMATransferSize_1Bytes);

            /* Transfer 4 bytes from RX FIFO to receive buffer */
            EDMA_HAL_HTCDSetNbytes(dmaInstance, dmaChannel, 4);

            /* Adjust majorIteration to 4 bytes per transfer */
            majorIteration = transferByteCount/4;
        }

        /* Configure CITER and BITER fields and clear the ELINK field (disable channel linking) */
        EDMA_HAL_HTCDSetChannelMinorLink(dmaInstance, dmaChannel, 0, false);
        EDMA_HAL_HTCDSetMajorCount(dmaInstance, dmaChannel, majorIteration);

        /* Now that the TCD was set up, enable the DSPI Peripheral Hardware request for the
         * RX FIFO
         */
        EDMA_HAL_SetDmaRequestCmd(dmaInstance, (edma_channel_indicator_t)dmaChannel, true);

        /* Enable the Receive FIFO Drain Request as a DMA request */
        DSPI_HAL_SetRxFifoDrainDmaIntMode(baseAddr, kDspiGenerateDmaReq, true);

        /* RX FIFO overflow request enable (to interrupt on RX errors) */
        DSPI_HAL_SetIntMode(baseAddr, kDspiRxFifoOverflow, true);

        /* Set up eDMA interrupt handler */
        /* Due to MISRA 11.1 rule:
         * Conversions shall not be performed between a pointer to a function
         * and any type other than an integral type.
         * We first have to typecast the callback function pointer as a uint32_t before typecasting
         * as a void pointer.
         */
        uint32_t userCallbackInt = (uint32_t)(slaveConfig->callbacks.dspiDone);
        EDMA_DRV_InstallCallback(
            &dspiState->dmaRxFifo2RxBuff, DSPI_DRV_SlaveDmaCallback,(void *)(userCallbackInt));

        /* The number of bytes to transfer was configured above as the eDMA
         * major loop iteration count.  When this counts down to 0, it indicates
         * that the channel is done. Enable this channel done interrupt in the
         * eDMA CSR.  The ISR is defined in the eDMA driver with a callback registered
         * above.  This is eventually called back to the user defined callback.
         */
        EDMA_HAL_HTCDSetIntCmd(dmaInstance, dmaChannel, true);
    }

    /*************************************************************************
     * Set up DMA channel for TX FIFO only if user passes in a send buffer
     *************************************************************************/
    if (sendBuffer)
    {
        /***********************************
         * Request DMA channels for TX FIFO
         * This channel transfers data from source buffer to TX FIFO (PUSHR).
         ***********************************/
        /* Request channel for transmit */
        EDMA_DRV_RequestChannel(kEDMAAnyChannel, kDmaRequestMux0SPI0Tx, &dspiState->dmaSrc2TxFifo);

        /* Set up Transfer Control Descriptor for TX DMA channel */
        /* For each transfer control descriptor set up, save off DMA instance and channel number
         * to simplified variable names to make code cleaner
         */
        dmaInstance = VIRTUAL_CHN_TO_EDMA_MODULE_REGBASE(dspiState->dmaSrc2TxFifo.channel);
        dmaChannel = VIRTUAL_CHN_TO_EDMA_CHN(dspiState->dmaSrc2TxFifo.channel);

        /* Source addr, TX Send buffer */
        EDMA_HAL_HTCDSetSrcAddr(dmaInstance, dmaChannel,(uint32_t)(sendBuffer));

        /* Source addr offset is 1 as send buffer pointer is incremented 1 bytes for each write */
        EDMA_HAL_HTCDSetSrcOffset(dmaInstance, dmaChannel, 1);

        /* Source address adjust last: don't increment source address */
        EDMA_HAL_HTCDSetSrcLastAdjust(dmaInstance, dmaChannel, 0);


        /* Destination is the TX FIFO */
        EDMA_HAL_HTCDSetDestAddr(dmaInstance, dmaChannel,
                                 HW_SPI_PUSHR_SLAVE_ADDR(baseAddr));

        /* Dest addr offset don't increment as it is a FIFO */
        EDMA_HAL_HTCDSetDestOffset(dmaInstance, dmaChannel, 0);

        /* No adjustment needed for destination addr */
        EDMA_HAL_HTCDSetDestLastAdjust(dmaInstance, dmaChannel, 0);

        /* The source and destination attributes (bit size) depends on bits/frame setting */
        if (dspiState->bitsPerFrame <= 8)
        {
            /* Destination size is one byte */
            EDMA_HAL_HTCDSetAttribute(dmaInstance, dmaChannel,
                    kEDMAModuloDisable, kEDMAModuloDisable,
                    kEDMATransferSize_1Bytes, kEDMATransferSize_1Bytes);

            /* Transfer 1 byte from send buffer to TX FIFO  */
            EDMA_HAL_HTCDSetNbytes(dmaInstance, dmaChannel, 1);

            /* Adjust majorIteration to 1 byte per transfer */
            majorIteration = transferByteCount;
        }
        else if (dspiState->bitsPerFrame <= 16)
        {
            /* Destination size is two bytes */
            EDMA_HAL_HTCDSetAttribute(dmaInstance, dmaChannel,
                    kEDMAModuloDisable, kEDMAModuloDisable,
                    kEDMATransferSize_1Bytes, kEDMATransferSize_2Bytes);

            /* Transfer 2 bytes from send buffer to TX FIFO */
            EDMA_HAL_HTCDSetNbytes(dmaInstance, dmaChannel, 2);

            /* Adjust majorIteration to 2 bytes per transfer */
            majorIteration = transferByteCount/2;
        }
/* For now, we will limit the user to use 1-, 2-, or 4-byte transfers. This implementation
 * of 3-byte transfers changes the eDMA overall operation to enable minorloop mapping which may
 * be undesireable to other drivers. For future implmentations, we will look into setting up
 * three linked channels: 1 channel to transfer 2 bytes to an intermediate buffer, another channel
 * to transfer one byte to the intermediate buffer, and finally another channel to transfer the
 * intermediate buffer to the TX FIFO
 */
#if 0
        else if (dspiState->bitsPerFrame <= 24)
        {
            /* Destination size is actually 3 bytes (for up to 24-bits/frame), but we only have the
             * option to go up to a 4 byte (32-bit) destination size, as 3 bytes is not a supported
             * setting.  That means we are always forced to transfer 4 bytes instead of the
             * desired 3 bytes. Later, we'll adjust the source address to "go back one byte"
             * by using the minor loop offset feature of the eDMA. Note, source = source buffer
             * destination = TX FIFO.
             */
              edma_hal_htcd_configure_dest_transfersize(dmaInstance, dmaChannel,
                                                      kEdmaTransferSize4bytes);

            /* Enable minor loop mapping CR[EMLM]=1 which enables the minor loop offset feature */
            edma_hal_set_minor_loop_mapping(dmaInstance, true);

            /* Set the source minor loop offset enable and set the offset MLOFF to -1
             * As stated previously, we need the source address to "go back one byte"
             * at the end of the minor loop transfer of 4 bytes.
             */
            edma_minorloop_offset_config_t minorLoopConfig;
            minorLoopConfig.isEnableDestMinorloop = false;
            minorLoopConfig.isEnableSourceMinorloop = true;
            minorLoopConfig.offset = -1;
            edma_hal_htcd_configure_minorloop_offset(dmaInstance, dmaChannel, minorLoopConfig);

            /* Set the minor loop transfer size to 4 bytes to align with the destination size of
             * 4 bytes. Basically we are reading 4 bytes from the source buffer and writing to
             * TX FIFO.
             */
            edma_hal_htcd_configure_nbytes_minorloop_enabled_offset_enabled(dmaInstance,
                                                                            dmaChannel, 4);

            /* Adjust majorIteration to 3 bytes per transfer */
            majorIteration = transferByteCount/3;
        }
#endif
        else
        {
            /* Destination size 4 bytes (32-bit) */
            EDMA_HAL_HTCDSetAttribute(dmaInstance, dmaChannel,
                    kEDMAModuloDisable, kEDMAModuloDisable,
                    kEDMATransferSize_1Bytes, kEDMATransferSize_4Bytes);

            /* Transfer 4 bytes from RX FIFO to receive buffer */
            EDMA_HAL_HTCDSetNbytes(dmaInstance, dmaChannel, 4);

            /* Adjust majorIteration to 4 bytes per transfer */
            majorIteration = transferByteCount/4;
        }

        /* Configure CITER and BITER fields and clear the ELINK field (disable channel linking) */
        EDMA_HAL_HTCDSetChannelMinorLink(dmaInstance, dmaChannel,0,false);
        EDMA_HAL_HTCDSetMajorCount(dmaInstance, dmaChannel, majorIteration);

        /* Now that the TCD was set up, enable the DSPI Peripheral Hardware request for the
         * TX FIFO.
         */
        EDMA_HAL_SetDmaRequestCmd(dmaInstance, (edma_channel_indicator_t)dmaChannel, true);

        /* Enable the TX FIFO Fill Request as a DMA request */
        DSPI_HAL_SetTxFifoFillDmaIntMode(baseAddr, kDspiGenerateDmaReq, true);

        /* Set up eDMA interrupt handler only if it hasn't already been defined for receive */
        if (receiveBuffer == NULL)
        {
            /* Set up eDMA interrupt handler */
            /* Due to MISRA 11.1 rule:
             * Conversions shall not be performed between a pointer to a function
             * and any type other than an integral type.
             * We first have to typecast the callback function pointer as a uint32_t before
             * typecasting as a void pointer.
             */
            uint32_t userCallbackInt = (uint32_t)(slaveConfig->callbacks.dspiDone);
            EDMA_DRV_InstallCallback(&dspiState->dmaSrc2TxFifo, DSPI_DRV_SlaveDmaCallback,
                                   (void *)(userCallbackInt));

            /* The number of bytes to transfer was configured above as the eDMA
             * major loop iteration count.  When this counts down to 0, it indicates
             * that the channel is done. Enable this channel done interrupt in the
             * eDMA CSR.  The ISR is defined in the eDMA driver with a callback registered
             * above.  This is eventually called back to the user defined callback.
             */
            EDMA_HAL_HTCDSetIntCmd(dmaInstance, dmaChannel, true);
        }

        /* TX FIFO underflow request enable*/
        DSPI_HAL_SetIntMode(baseAddr, kDspiTxFifoUnderflow, true);
    }
    else
    {
        /* Write known data (zeros) if no source buffer passed in */
        DSPI_HAL_WriteDataSlavemode(baseAddr, kEmptyChar);
    }

    /* Start DSPI transfers, set to running state */
    DSPI_HAL_StartTransfer(baseAddr);

    /* Interrupts are used in this driver for detecting following DSPI error conditions:
     * TX Underflow
     * RX Overflow
     * User must define an error handler for these interrupts (refer to DSPI_DRV_SlaveInitDma
     * comments for more details).
     */
    /* Enable the interrupt */
    INT_SYS_EnableIRQ(g_dspiIrqId[instance]);

    return errorCode;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

