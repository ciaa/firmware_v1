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
#include "fsl_dspi_common.h"
#include "fsl_dspi_master_driver.h"
#include "fsl_dspi_shared_function.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* For storing DMA intermediate buffers between the source buffer and TX FIFO */
static uint32_t s_cmdData;      /* Intermediate 16-bit command and 16-bit data buffer */
static uint32_t s_lastCmdData;  /* Consists of the last command and the final source data */
static uint16_t s_wordToSend; /* Word to send, if no send buffer, this variable is used
                                 as the word to send, which should be initialized to 0. Needs
                                 to be static and stored in data section as this variable
                                 address is the source address if no source buffer.  */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
dspi_status_t DSPI_DRV_MasterStartTransferDma(uint32_t instance,
                                              const dspi_device_t * restrict device);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_DRV_MasterInitDma
 * Description   : Initialize a DSPI instance for master mode operation with DMA support.
 * This function is exactly like the dspi_master_init function but in addition, adds DMA support.
 * If the user desires to use DMA based transfers, then the user should use this function call
 * instead of the dspi_master_init function call.  Like the dspi_master_init,
 * this function initializes the run-time state structure to track the ongoing
 * transfers, ungates the clock to the DSPI module, resets the DSPI module, initializes the module
 * to user defined settings and default settings, configures the IRQ state structure, enables
 * the module-level interrupt to the core, and enables the DSPI module.
 * The CTAR parameter is special in that it allows the user to have different SPI devices
 * connected to the same DSPI module instance in addition to different peripheral chip
 * selects. Each CTAR contains the bus attributes associated with that particular SPI device.
 * For most use cases where only one SPI device is connected per DSPI module
 * instance, use CTAR0.
 * This is an example to set up the dspi_master_state_t and the
 * dspi_master_user_config_t parameters and to call the dspi_master_init function by passing
 * in these parameters:
 *   dspi_master_state_t dspiMasterState; <- the user simply allocates memory for this struct
 *    uint32_t calculatedBaudRate;
 *    dspi_master_user_config_t userConfig; <- the user fills out members for this struct
 *    userConfig.isChipSelectContinuous = false;
 *    userConfig.isSckContinuous = false;
 *    userConfig.pcsPolarity = kDspiPcs_ActiveLow;
 *    userConfig.whichCtar = kDspiCtar0;
 *    userConfig.whichPcs = kDspiPcs0;
 *    edma_software_tcd_t stcdSrc2CmdDataLast; <- needs to be aligned to a 32-byte boundary
 *    dspi_master_init(masterInstance, &dspiMasterState, &userConfig, &calculatedBaudRate
 *                     &stcdSrc2CmdDataLast);
 *
 * This init function also configures the eDMA module by requesting channels for DMA operation
 * and sets a "use_dma" flag in the run-time state structure to notify transfer functions
 * to use DMA driven operations.
 *
 * Note: The pointer param "stcdSrc2CmdDataLast" needs to be aligned to a 32-byte boundary.
 *       Also note, the only DSPI instance supported for DMA based operation is instnace 0.
 *
 *END**************************************************************************/
dspi_status_t DSPI_DRV_MasterInitDma(uint32_t instance,
                                   dspi_master_state_t * dspiState,
                                   const dspi_master_user_config_t * userConfig,
                                   edma_software_tcd_t * stcdSrc2CmdDataLast)
{
    dspi_status_t errorCode = kStatus_DSPI_Success;

    errorCode = DSPI_DRV_MasterInit(instance, dspiState, userConfig);

    /* Set the DSPI run-time state struct flag to indicate it will use the DMA */
    dspiState->useDma = true;

    /***********************************
     * Request DMA channels for RX FIFO
     ***********************************/
    /* This channel transfers data from RX FIFO to receiveBuffer */
    EDMA_DRV_RequestChannel(kEDMAAnyChannel, kDmaRequestMux0SPI0Rx,&dspiState->dmaFifo2Receive);

   /***********************************
     * Request DMA channels for TX FIFO
     ***********************************/
    /* "Channel 1" Intermediate command/data to TX FIFO (PUSHR).
     * Note, actual channel number may very depending on DMA system usage.
     * Only use DSPI instance 0 since it has a dedicated RX and TX DMA mux source
     */
    EDMA_DRV_RequestChannel(kEDMAAnyChannel, kDmaRequestMux0SPI0Tx, &dspiState->dmaCmdData2Fifo);

    /* "Channel 2" Source buffer to intermediate command/data
     * Note, actual channel number may very depending on DMA system usage.
     */
    EDMA_DRV_RequestChannel(kEDMAAnyChannel, kDmaRequestMux0AlwaysOn63, &dspiState->dmaSrc2CmdData);

    /**************************************************************************************
     * Update run-time state struct with the pointer to Software Transfer Control Descriptor
     **************************************************************************************/
    dspiState->stcdSrc2CmdDataLast = stcdSrc2CmdDataLast;

    return errorCode;
}

/*!
 * @brief Initiate (start) a transfer using DMA. This is not a public API as it is called from
 *  other driver functions
 */
dspi_status_t DSPI_DRV_MasterStartTransferDma(uint32_t instance,
                                             const dspi_device_t * restrict device)
{
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];
    /* For temporarily storing DMA instance and channel */
    uint32_t dmaInstance, dmaChannel;
    uint32_t calculatedBaudRate;
    dspi_command_config_t command;  /* create an instance of the data command struct*/
    uint32_t baseAddr = g_dspiBaseAddr[instance];

    /* Initialize s_wordToSend */
    s_wordToSend = 0;

    /* If the transfer count is zero, then return immediately.*/
    if (dspiState->remainingSendByteCount == 0)
    {
        /* Signal the synchronous completion object if the transfer wasn't async.
         * Otherwise, when we return the the sync function we'll get stuck in the sync wait loop.
         */
        if (!dspiState->isTransferAsync)
        {
            OSA_SemaPost(&dspiState->irqSync);
        }

        return kStatus_DSPI_Success;
    }

    /* Check that we're not busy.*/
    if (dspiState->isTransferInProgress)
    {
        return kStatus_DSPI_Busy;
    }

    /* Configure bus for this device. If NULL is passed, we assume the caller has
     * preconfigured the bus using DSPI_DRV_MasterConfigureBus().
     * Do nothing for calculatedBaudRate. If the user wants to know the calculatedBaudRate
     * then they can call this function separately.
     */
    if (device)
    {
        DSPI_DRV_MasterConfigureBus(instance, device, &calculatedBaudRate);
        dspiState->bitsPerFrame = device->dataBusConfig.bitsPerFrame;/*update dspiState bits/frame*/
    }

    /* Check the transfer byte count. If bits/frame > 8, meaning 2 bytes, then
     * the transfer byte count must not be an odd count, if so, drop the last odd byte.
     * Perform this operation here to ensure we get the latest bits/frame setting.
     */
    if (dspiState->bitsPerFrame > 8)
    {
        dspiState->remainingSendByteCount &= ~1UL;
        dspiState->remainingReceiveByteCount &= ~1UL;
    }

    /* Save information about the transfer for use by the ISR.*/
    dspiState->isTransferInProgress = true;

    /* Enable the DSPI module for the duration of this transfer.*/
    DSPI_HAL_Enable(baseAddr);

    /* Reset the transfer counter to 0. Normally this is done via the PUSHR[CTCNT], but as we
     * are under DMA controller, we won't be able to change this bit dynamically after the
     * first word is transferred.
     */
    DSPI_HAL_PresetTransferCount(baseAddr, 0);

    /* flush the fifos*/
    DSPI_HAL_SetFlushFifoCmd(baseAddr, true, true);

    /* Clear the EOQ flag as it may have been set from a previous transfer. Also, make sure
     * to do this after flushing the FIFOs.
     */
    DSPI_HAL_ClearStatusFlag(baseAddr, kDspiEndOfQueue);

    /* Enable the End Of Queue interrupt, which will set when DSPI sees EOQ bit set in the
     * last data word being sent. The ISR should clear this flag.
     */
    DSPI_HAL_SetIntMode(baseAddr, kDspiEndOfQueue, true);

    /* Each DMA channel's CSR[DONE] bit may be set if a previous transfer occurred.  The DONE
     * bit, as the name implies, sets when the channel is finished (completed it's MAJOR
     * LOOP). The DONE needs to be cleared before programming the channel's TCDs for the next
     * transfer.
     */
    dmaInstance = VIRTUAL_CHN_TO_EDMA_MODULE_REGBASE(dspiState->dmaCmdData2Fifo.channel);
    dmaChannel = VIRTUAL_CHN_TO_EDMA_CHN(dspiState->dmaCmdData2Fifo.channel);
    EDMA_HAL_ClearDoneStatusFlag(dmaInstance, (edma_channel_indicator_t)dmaChannel);

    dmaInstance = VIRTUAL_CHN_TO_EDMA_MODULE_REGBASE(dspiState->dmaSrc2CmdData.channel);
    dmaChannel = VIRTUAL_CHN_TO_EDMA_CHN(dspiState->dmaSrc2CmdData.channel);
    EDMA_HAL_ClearDoneStatusFlag(dmaInstance, (edma_channel_indicator_t)dmaChannel);

    dmaInstance = VIRTUAL_CHN_TO_EDMA_MODULE_REGBASE(dspiState->dmaFifo2Receive.channel);
    dmaChannel = VIRTUAL_CHN_TO_EDMA_CHN(dspiState->dmaFifo2Receive.channel);
    EDMA_HAL_ClearDoneStatusFlag(dmaInstance, (edma_channel_indicator_t)dmaChannel);

    /************************************************************************************
     * Set up the RX DMA channel Transfer Control Descriptor (TCD)
     * Do this before filling the TX FIFO.
     * Note, if there is no receive buffer (if user passes in NULL), then bypass RX DMA
     * set up.
     ***********************************************************************************/
    /* If a receive buffer is used */
    if (dspiState->receiveBuffer)
    {
        /* For each transfer control descriptor set up, save off DMA instance and channel number
         * to simplified variable names to make code cleaner
         */
        dmaInstance = VIRTUAL_CHN_TO_EDMA_MODULE_REGBASE(dspiState->dmaFifo2Receive.channel);
        dmaChannel = VIRTUAL_CHN_TO_EDMA_CHN(dspiState->dmaFifo2Receive.channel);

        /* Source addr, RX FIFO */
        EDMA_HAL_HTCDSetSrcAddr(dmaInstance,dmaChannel, HW_SPI_POPR_ADDR(baseAddr));

        /* Source addr offset is 0 as source addr never increments */
        EDMA_HAL_HTCDSetSrcOffset(dmaInstance, dmaChannel, 0);

        /* The source and destination attributes (bit size) depends on bits/frame setting */
        if (dspiState->bitsPerFrame <= 8)
        {
            /* Source size is one byte, destination size is one byte */
            EDMA_HAL_HTCDSetAttribute(
                dmaInstance, dmaChannel,
                kEDMAModuloDisable, kEDMAModuloDisable,
                kEDMATransferSize_1Bytes,kEDMATransferSize_1Bytes);

            /* Transfer 1 byte from RX FIFO to receive buffer */
            EDMA_HAL_HTCDSetNbytes(dmaInstance, dmaChannel, 1);
        }
        else /* For 16-bit words, but the receive buffer is still an 8-bit buffer */
        {

            /* Source size is 2 byte, destination size is one byte */
            EDMA_HAL_HTCDSetAttribute(
                dmaInstance, dmaChannel,
                kEDMAModuloDisable, kEDMAModuloDisable,
                kEDMATransferSize_2Bytes,kEDMATransferSize_1Bytes);

            /* Transfer 2 bytes from RX FIFO to receive buffer */
            EDMA_HAL_HTCDSetNbytes(dmaInstance, dmaChannel, 2);
        }

        /* Don't increment source address, it is constant */
        EDMA_HAL_HTCDSetSrcLastAdjust(dmaInstance, dmaChannel, 0);

        /* Destination is the receive buffer */
        EDMA_HAL_HTCDSetDestAddr(dmaInstance, dmaChannel,(uint32_t)(dspiState->receiveBuffer));

        /* Dest addr offset, increment to the next byte */
        EDMA_HAL_HTCDSetDestOffset(dmaInstance, dmaChannel, 1);

        /* Set MAJOR count to remaining receive byte count. Configure both the
         * CITER and BITER fields.  Also, clear the ELINK field (disable channel linking)
         */
        EDMA_HAL_HTCDSetChannelMinorLink(dmaInstance, dmaChannel, 0, false);
        EDMA_HAL_HTCDSetMajorCount(dmaInstance, dmaChannel,
                                   (uint32_t)dspiState->remainingReceiveByteCount);

        /* No adjustment needed for destination addr */
        EDMA_HAL_HTCDSetDestLastAdjust(dmaInstance,dmaChannel, 0);

        /* Now that TCD was set up, enable the DSPI Peripheral Hardware request for the RX FIFO */
        EDMA_HAL_SetDmaRequestCmd(dmaInstance,(edma_channel_indicator_t)dmaChannel,true);

        /* Enable the Receive FIFO Drain Request  as a DMA request */
        DSPI_HAL_SetRxFifoDrainDmaIntMode(baseAddr, kDspiGenerateDmaReq, true);
    }

    /************************************************************************************
     * Set up the Last Command/data Word Intermediate Buffer and fill up the TX FIFO.
     ***********************************************************************************/
    /* Before sending the data, we first need to initialize the data command struct
     * Configure the data command attributes for the desired PCS, CTAR, and continuous PCS
     * which are derived from the run-time state struct
     */
    command.whichPcs = dspiState->whichPcs;
    command.whichCtar = dspiState->whichCtar;
    command.isChipSelectContinuous = dspiState->isChipSelectContinuous;

    /************************************************************************
     * Next, set up the Last Command/data Word Intermediate Buffer before
     * filling up the TX FIFO
     * Create a 32-bit word with the final 16-bit command settings. This means
     * that EOQ = 1 and CONT = 0.
     * This 32-bit word will also be initialized with the final data byte/word
     * from the send source buffer and then the entire 32-bit word will be
     * transferred to the DSPI PUSHR.
     ************************************************************************/
    /* Store the original transferByteCount before remainingSendByteCount is decremented
     * and declare a variable for storing the last send data (either 8- or 16-bit).
     */
    uint32_t transferByteCount = dspiState->remainingSendByteCount;
    uint32_t lastWord = 0;

    /* If a send buffer was provided, the word comes from there. Otherwise we just send
     * a zero (initialized above).
     */
    if (dspiState->sendBuffer)
    {
        /* Store the last byte from the send buffer */
        if (dspiState->bitsPerFrame <= 8)
        {
            lastWord = dspiState->sendBuffer[transferByteCount-1]; /* Last byte */
        }
        /* Store the last two bytes the send buffer */
        else
        {
            lastWord = dspiState->sendBuffer[transferByteCount-1] ; /* Save off the last byte */
            lastWord = lastWord << 8U; /* Shift to MSB (separate step due to MISHA) */
            lastWord |= dspiState->sendBuffer[transferByteCount-2]; /* OR with next to last byte */
        }
    }

    /* Now, build the last command/data word intermediate buffer */
    command.isChipSelectContinuous = false; /* Always clear CONT for last data word */
    command.isEndOfQueue = true; /* Set EOQ for last data word */
    s_lastCmdData = BF_SPI_PUSHR_CONT(command.isChipSelectContinuous) |
                    BF_SPI_PUSHR_CTAS(command.whichCtar) |
                    BF_SPI_PUSHR_PCS(command.whichPcs) |
                    BF_SPI_PUSHR_EOQ(command.isEndOfQueue) |
                    BF_SPI_PUSHR_TXDATA(lastWord); /* last byte/word in send buffer */

    /************************************************************************
     * Begin TX DMA channels transfer control descriptor set up.
     * 1. First, set up intermediate buffers which contain 16-bit commands.
     * 2. Set up the DMA Software Transfer Control Descriptors (STCD) for various
     *    scenarios:
     *    - Channel for intermediate buffer to TX FIFO
     *    - Channel for source buffer to intermediate buffer
     *    - STCD for scatter/gather for end of previous channel to replace
     *      intermediate buffer with last-command buffer.
     ************************************************************************/

    /************************************************************************
     * Intermediate Buffer
     * Create a 32-bit word with the 16-bit command settings. Data from
     * the send source buffer will be transferred here and then the entire
     * 32-bit word will be transferred to the DSPI PUSHR.
     * This buffer will be preloaded with the next data word in the send buffer
     ************************************************************************/
    command.isEndOfQueue = 0; /* Clear End of Queue (previously set for last cmd/data word)*/
    s_cmdData = BF_SPI_PUSHR_CONT(command.isChipSelectContinuous) |
                BF_SPI_PUSHR_CTAS(command.whichCtar) |
                BF_SPI_PUSHR_PCS(command.whichPcs) |
                BF_SPI_PUSHR_EOQ(command.isEndOfQueue);

    /* Place the next data from the send buffer into the intermediate buffer (preload it)
     * based on whether it is one byte or two.
     */
    if (dspiState->bitsPerFrame <= 8)
    {
        /* If a send buffer was provided, the word comes from there. Otherwise we just send
         * a zero (initialized above).
         */
        if (dspiState->sendBuffer)
        {
            s_wordToSend = *(dspiState->sendBuffer);  /* queue up the next data */
            ++dspiState->sendBuffer; /* increment to next data word*/
        }
        --dspiState->remainingSendByteCount; /* decrement remainingSendByteCount*/
    }
    else
    {
        /* If a send buffer was provided, the word comes from there. Otherwise we just send
         * a zero (initialized above).
         */
        if (dspiState->sendBuffer)
        {
            s_wordToSend = *(dspiState->sendBuffer);
            ++dspiState->sendBuffer; /* increment to next data byte */
            s_wordToSend |= (unsigned)(*(dspiState->sendBuffer)) << 8U;
            ++dspiState->sendBuffer; /* increment to next data byte */
        }
        dspiState->remainingSendByteCount -= 2; /* decrement remainingSendByteCount by 2 */
    }

    s_cmdData |= BF_SPI_PUSHR_TXDATA(s_wordToSend); /* write s_wordToSend to intermediate buffer */

    /************************************************************************************
     * Transfer Control Descriptor set up for Intermediate command/data to TX
     * FIFO (PUSHR). AKA "Channel 1"
     * Note, actual channel number may very depending on DMA system usage.
     * This channels links to "Channel 1" on completion of MAJOR loop.  "Channel 1" is
     * the channel that transfers data from the send source buffer to the intermediate
     * command/data buffer.
     * Note that the channel linking is based on the MAJOR loop complete and not on minor
     * loop.  This is because we are only sending one 32-bit word so when the minor loop
     * completes, the MAJOR loop also completes.  The eDMA does not channel link on the
     * last iteration of the MAJOR loop using the ELINK mechanism, hence we have to link
     * on the MAJOR loop completion using MAJORLINK channel linking.
     ************************************************************************************/
    /* For each transfer control descriptor set up, save off DMA instance and channel number
     * to simplified variable names to make code cleaner
     */
    dmaInstance = VIRTUAL_CHN_TO_EDMA_MODULE_REGBASE(dspiState->dmaCmdData2Fifo.channel);
    dmaChannel = VIRTUAL_CHN_TO_EDMA_CHN(dspiState->dmaCmdData2Fifo.channel);

    /* If remainingSendByteCount is 0, then send last command/data since this is the
     * last data word to send
     */
    if (dspiState->remainingSendByteCount == 0)
    {
        /* Source address is the last command/data intermediate buffer */
        EDMA_HAL_HTCDSetSrcAddr(dmaInstance, dmaChannel,(uint32_t)(&s_lastCmdData));

        /* Disable ERQ request at end of major count */
        EDMA_HAL_HTCDSetDisableDmaRequestAfterTCDDoneCmd(dmaInstance, dmaChannel, true);

        /* Disable majorlink request */
        EDMA_HAL_HTCDSetChannelMajorLink(dmaInstance, dmaChannel, 0, false);
    }
    /* Else, send the intermediate buffer  */
    else
    {
        /* Source addr, intermediate command/data*/
        EDMA_HAL_HTCDSetSrcAddr(dmaInstance, dmaChannel,(uint32_t)(&s_cmdData));

        /* Set the MAJOR link channel to link the the next channel that will pull data from
         * the source buffer into the intermediate command/data buffer and enable MAJOR link
         */
        EDMA_HAL_HTCDSetChannelMajorLink(dmaInstance, dmaChannel,
                        VIRTUAL_CHN_TO_EDMA_CHN(dspiState->dmaSrc2CmdData.channel), true);

        /* Do not disable ERQ request at end of major count */
        EDMA_HAL_HTCDSetDisableDmaRequestAfterTCDDoneCmd(dmaInstance, dmaChannel, false);
    }

    /* Source addr offset is 0 as source addr never increments */
    EDMA_HAL_HTCDSetSrcOffset(dmaInstance, dmaChannel, 0);

    /* source size 32-bits */
    /* destination size 32bits*/
    /* Clear the SMOD and DMOD fields */
    EDMA_HAL_HTCDSetAttribute(dmaInstance, dmaChannel,
                              kEDMAModuloDisable, kEDMAModuloDisable,
                              kEDMATransferSize_4Bytes, kEDMATransferSize_4Bytes);

    /* Transfer 4 bytes or one word */
    EDMA_HAL_HTCDSetNbytes(dmaInstance, dmaChannel, 4);

    /* Don't increment source address, it is constant */
    EDMA_HAL_HTCDSetSrcLastAdjust(dmaInstance, dmaChannel, 0);

    /* Destination is SPI PUSHR TX FIFO */
    EDMA_HAL_HTCDSetDestAddr(dmaInstance, dmaChannel, HW_SPI_PUSHR_ADDR(baseAddr));

    /* No dest addr offset, since we never increment the dest addr */
    EDMA_HAL_HTCDSetDestOffset(dmaInstance, dmaChannel, 0);

    /* We are only sending one 32-bit word, so MAJOR count is "1". Do not use "ELINK"
     * to link channels, use MAJORLINK in CSR, therefore disable minor link (ELINK=0)
     */
    EDMA_HAL_HTCDSetChannelMinorLink(dmaInstance, dmaChannel, 0, false);
    EDMA_HAL_HTCDSetMajorCount(dmaInstance, dmaChannel, 1);

    /* No adjustment needed for destination addr or scatter/gather */
    EDMA_HAL_HTCDSetScatterGatherCmd(dmaInstance, dmaChannel, false);

    /* Implement the following DMA channel set up only if we still have data yet to send
     * Otherwise, bypass this and enable the DSPI Transmit DMA request.
     */
    dmaInstance = VIRTUAL_CHN_TO_EDMA_MODULE_REGBASE(dspiState->dmaSrc2CmdData.channel);
    dmaChannel = VIRTUAL_CHN_TO_EDMA_CHN(dspiState->dmaSrc2CmdData.channel);

    if (dspiState->remainingSendByteCount != 0)
    {
        /************************************************************************************
         * Scatter/gather STCD set up
         * STCD ONLY for Last intermediate command/data to PUSHR. Do not call
         * edma_hal_stcd_push_to_htcd as this is used for "Channel 2" scatter/gather.
         * Hence we will call this "Channel 2 prime". This needs to be defined before
         * setting up "Channel 2" as it needs the address for this STCD.
         *
         * IMPORTANT: Per eDMA spec, the pointer address for this STCD structure needs to be
         *            aligned on 32-byte boundaries.
         ************************************************************************************/
        /* Source addr is last data + last command */
        dspiState->stcdSrc2CmdDataLast->SADDR = (uint32_t)(&s_lastCmdData);

        /* Source addr offset is 0 as source addr never increments */
        dspiState->stcdSrc2CmdDataLast->SOFF = 0;

        dspiState->stcdSrc2CmdDataLast->ATTR =
            (2 << BP_DMA_TCDn_ATTR_SSIZE) | /* source size 32-bits */
            (2 << BP_DMA_TCDn_ATTR_DSIZE);  /* destination size 32bits*/

        /* Transfer 4 bytes or one word */
        dspiState->stcdSrc2CmdDataLast->NBYTES.MLNO = 4;

        /* Don't increment source address, it is constant */
        dspiState->stcdSrc2CmdDataLast->SLAST = 0;

        /* Destination is the intermediate command/data buffer. Overwrite it with last data word */
        dspiState->stcdSrc2CmdDataLast->DADDR = (uint32_t)(&s_cmdData);

        /* No dest addr offset since we never increment the dest addr */
        dspiState->stcdSrc2CmdDataLast->DOFF = 0;

        /* We are only sending one 32-bit word, so MAJOR count is "1", no chan-to-chan linking */
        dspiState->stcdSrc2CmdDataLast->CITER.ELINKYES = 1; /* major count only */
        dspiState->stcdSrc2CmdDataLast->BITER.ELINKYES = 1; /* major count only */

        /* No adjustment needed for destination addr */
        dspiState->stcdSrc2CmdDataLast->DLAST_SGA = 0;

        /* CSR set up */
        dspiState->stcdSrc2CmdDataLast->CSR =
                              (1 << BP_DMA_TCDn_CSR_DREQ) | /* Disable ERQ request when CITER=0 */
                              (0 << BP_DMA_TCDn_CSR_ESG);   /* clr the ESG */

        /* If at this point we are left with only sending one more data byte/word, then this
         * is the last command/data to send.  So the transfer control descriptor should move the
         * last command/data word into the intermediate buffer and this will get transferred to the
         * DSPI when the FIFO is ready for this.
         */
        if (((dspiState->bitsPerFrame <= 8) && ((dspiState->remainingSendByteCount-1) == 0)) ||
            ((dspiState->bitsPerFrame > 8) && ((dspiState->remainingSendByteCount-2) == 0)))
        {
            /* push the contents of the SW TCD to the HW TCD registers */
            EDMA_HAL_PushSTCDToHTCD(dmaInstance, dmaChannel, dspiState->stcdSrc2CmdDataLast);
        }
        /* Otherwise, we are left with more data to send, so use the transfer control
         * descriptor that will move data from the send source buffer to the intermediate
         * command/data buffer.
         */
        else
        {
            /************************************************************************************
             * Transfer Control Descriptor set up for Source buffer to intermediate
             * command/data (this is a linked channel). AKA "Channel 2"
             * Note, actual channel number may very depending on DMA system usage
             * This channel is triggered by the completion of "Channel 1". It transfers data from
             * the send source buffer to the intermediate command/data word. When the source
             * buffer transfers the word before the last data word, the MAJOR loop completes
             * and triggers the scatter/gather (ESG = 1) and loads the STCD that is set up to
             * transfer the last command/data word to the PUSHR.
             ************************************************************************************/

            /* If a send buffer was provided, the word comes from there. Otherwise we set
             * the source address to point to the s_wordToSend variable that was set to 0.
             */
            if (dspiState->sendBuffer)
            {
                /* Source addr is the "send" data buffer */
                EDMA_HAL_HTCDSetSrcAddr(dmaInstance, dmaChannel,
                                        (uint32_t)(dspiState->sendBuffer));
                /* Increment the source address by one byte after every transfer */
                EDMA_HAL_HTCDSetSrcOffset(dmaInstance, dmaChannel, 1);
            }
            else
            {
                /* Source addr is the "send" data buffer */
                EDMA_HAL_HTCDSetSrcAddr(dmaInstance, dmaChannel,
                                        (uint32_t)(&s_wordToSend));
                /* Don't increment the source address  */
                EDMA_HAL_HTCDSetSrcOffset(dmaInstance, dmaChannel, 0);
            }

            if (dspiState->bitsPerFrame <= 8)
            {
                /* Source and destination size: byte */
                EDMA_HAL_HTCDSetAttribute(dmaInstance, dmaChannel,
                            kEDMAModuloDisable, kEDMAModuloDisable,
                            kEDMATransferSize_1Bytes, kEDMATransferSize_1Bytes);

                /* minor byte transfer: 1 byte (8-bit word) */
                EDMA_HAL_HTCDSetNbytes(dmaInstance, dmaChannel, 1);

                /* Major loop count is equal to remaining number of bytes to send minus 1.
                 * That is because the last data byte/word is written to the last command/data
                 * intermediate buffer.
                 * Also, disable minor link (ELINK=0)
                 */
                EDMA_HAL_HTCDSetChannelMinorLink(dmaInstance, dmaChannel, 0, false);
                EDMA_HAL_HTCDSetMajorCount(
                            dmaInstance, dmaChannel, (dspiState->remainingSendByteCount-1));

            }
            else
            {
                /* Source size: byte and destination size: halfword */
                EDMA_HAL_HTCDSetAttribute(dmaInstance, dmaChannel,
                            kEDMAModuloDisable, kEDMAModuloDisable,
                            kEDMATransferSize_1Bytes, kEDMATransferSize_2Bytes);

                /* minor byte transfer: 2 bytes (16-bit word) */
                EDMA_HAL_HTCDSetNbytes(dmaInstance, dmaChannel, 2);

                /* Major loop count is equal to remaining number of 16-bit words to send
                 * hence need to convert remainingSendByteCount from byte to 16-bit word
                 * Also, disable minor link (ELINK=0)
                 */
                EDMA_HAL_HTCDSetChannelMinorLink(dmaInstance, dmaChannel, 0, false);
                EDMA_HAL_HTCDSetMajorCount(
                            dmaInstance, dmaChannel, (dspiState->remainingSendByteCount-2)/2);

            }

            /* Set SLAST to 0 */
            EDMA_HAL_HTCDSetSrcLastAdjust(dmaInstance, dmaChannel, 0);

            /* Destination addr, intermediate command/data */
            EDMA_HAL_HTCDSetDestAddr(dmaInstance, dmaChannel,(uint32_t)(&s_cmdData));

            /* No dest addr offset, since we never increment the dest addr */
            EDMA_HAL_HTCDSetDestOffset(dmaInstance, dmaChannel, 0);

            /* Place the address of the scatter/gather in order to reload STCD for the final
             * last command/data word to be loaded to the intermediate buffer.
             * IMPORTANT: This address needs to be 32-byte aligned.
             */
            EDMA_HAL_HTCDSetScatterGatherLink(dmaInstance, dmaChannel,
                                    (edma_software_tcd_t *)(dspiState->stcdSrc2CmdDataLast));
        }
    }

    /* Now that the TCD was set up for each channel, enable the DSPI
     * Peripheral Hardware request for the first DMA channel, dmaSrc2CmdData
     */
    dmaInstance = VIRTUAL_CHN_TO_EDMA_MODULE_REGBASE(dspiState->dmaCmdData2Fifo.channel);
    dmaChannel = VIRTUAL_CHN_TO_EDMA_CHN(dspiState->dmaCmdData2Fifo.channel);

    EDMA_HAL_SetDmaRequestCmd(dmaInstance, (edma_channel_indicator_t)dmaChannel,true);

    /* Enable TFFF request in the DSPI module */
    DSPI_HAL_SetTxFifoFillDmaIntMode(baseAddr, kDspiGenerateDmaReq, true);

    return kStatus_DSPI_Success;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

