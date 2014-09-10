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
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static dspi_status_t DSPI_DRV_MasterStartTransfer(uint32_t instance,
                                           const dspi_device_t * restrict device);

static void DSPI_DRV_MasterCompleteTransfer(uint32_t instance);

extern dspi_status_t DSPI_DRV_MasterStartTransferDma(uint32_t instance,
                                             const dspi_device_t * restrict device);

void DSPI_DRV_MasterFillupTxFifo(uint32_t instance, bool isInitialData);
/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_DRV_MasterInit
 * Description   : Initialize a DSPI instance for master mode operation.
 * This function uses a CPU interrupt driven method for transferring data.
 * This function will initialize the run-time state structure to keep track of the on-going
 * transfers, ungate the clock to the DSPI module, reset the DSPI module, initialize the module
 * to user defined settings and default settings, configure the IRQ state structure and enable
 * the module-level interrupt to the core, and enable the DSPI module.
 * The CTAR parameter is special in that it allows the user to have different SPI devices
 * connected to the same DSPI module instance in conjunction with different peripheral chip
 * selects. Each CTAR contains the bus attributes associated with that particular SPI device.
 * For simplicity and for most use cases where only one SPI device is connected per DSPI module
 * instance, it is recommended to use CTAR0.
 * The following is an example of how to set up the dspi_master_state_t and the
 * dspi_master_user_config_t parameters and how to call the DSPI_DRV_MasterInit function by passing
 * in these parameters:
 *   dspi_master_state_t dspiMasterState; <- the user simply allocates memory for this struct
 *   uint32_t calculatedBaudRate;
 *   dspi_master_user_config_t userConfig; <- the user fills out members for this struct
 *   userConfig.isChipSelectContinuous = false;
 *   userConfig.isSckContinuous = false;
 *   userConfig.pcsPolarity = kDspiPcs_ActiveLow;
 *   userConfig.whichCtar = kDspiCtar0;
 *   userConfig.whichPcs = kDspiPcs0;
 *   DSPI_DRV_MasterInit(masterInstance, &dspiMasterState, &userConfig);
 *
 *END**************************************************************************/
dspi_status_t DSPI_DRV_MasterInit(uint32_t instance,
                                  dspi_master_state_t * dspiState,
                                  const dspi_master_user_config_t * userConfig)
{
    uint32_t dspiSourceClock;
    dspi_status_t errorCode = kStatus_DSPI_Success;

    uint32_t baseAddr = g_dspiBaseAddr[instance];

    /* Clear the run-time state struct for this instance.*/
    memset(dspiState, 0, sizeof(* dspiState));

    /* Note, remember to first enable clocks to the DSPI module before making any register accesses
     * Enable clock for DSPI
     */
    CLOCK_SYS_EnableSpiClock(instance);
    /* Get module clock freq*/
    dspiSourceClock = CLOCK_SYS_GetSpiFreq(instance);

    /* Configure the run-time state struct with the DSPI source clock */
    dspiState->dspiSourceClock = dspiSourceClock;

    /* Configure the run-time state struct with the data command parameters*/
    dspiState->whichCtar = userConfig->whichCtar;  /* set the dspiState struct CTAR*/
    dspiState->whichPcs = userConfig->whichPcs;  /* set the dspiState strcut whichPcs*/
    dspiState->isChipSelectContinuous = userConfig->isChipSelectContinuous; /* continuous PCS*/

    /* Initialize the DSPI module registers to default value, which disables the module */
    DSPI_HAL_Init(baseAddr);

    /* Initialize the DSPI module with user config */

    /* Set to master mode.*/
    DSPI_HAL_SetMasterSlaveMode(baseAddr, kDspiMaster);

    /* Configure for continuous SCK operation*/
    DSPI_HAL_SetContinuousSckCmd(baseAddr, userConfig->isSckContinuous);

    /* Configure for peripheral chip select polarity*/
    DSPI_HAL_SetPcsPolarityMode(baseAddr, userConfig->whichPcs, userConfig->pcsPolarity);

    /* Enable fifo operation (regardless of FIFO depth) */
    DSPI_HAL_SetFifoCmd(baseAddr, true, true);

    /* Initialize the configurable delays: PCS-to-SCK, prescaler = 0, scaler = 1 */
    DSPI_HAL_SetDelay(baseAddr, userConfig->whichCtar, 0, 1, kDspiPcsToSck);

    /* Save runtime structure pointers to irq handler can point to the correct state structure*/
    g_dspiStatePtr[instance] = dspiState;

    /* enable the interrupt*/
    INT_SYS_EnableIRQ(g_dspiIrqId[instance]);

    /* DSPI system enable*/
    DSPI_HAL_Enable(baseAddr);

    /* Start the transfer process in the hardware */
    DSPI_HAL_StartTransfer(baseAddr);

    return errorCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_DRV_MasterDeinit
 * Description   : Shutdown a DSPI instance.
 * This function resets the DSPI peripheral, gates its clock, and disables the interrupt to
 * the core.
 *
 *END**************************************************************************/
void DSPI_DRV_MasterDeinit(uint32_t instance)
{
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];
    uint32_t baseAddr = g_dspiBaseAddr[instance];

    /* First stop transfers */
    DSPI_HAL_StopTransfer(baseAddr);

    /* Restore the module to defaults then power it down. This also disables the DSPI module.*/
    DSPI_HAL_Init(baseAddr);

    /* disable the interrupt*/
    INT_SYS_DisableIRQ(g_dspiIrqId[instance]);

    /* Gate the clock for DSPI.*/
    CLOCK_SYS_DisableSpiClock(instance);

    /* If DMA-based operations were enabled, then do the following */
    if (dspiState->useDma)
    {
        EDMA_DRV_ReleaseChannel(&dspiState->dmaCmdData2Fifo);
        EDMA_DRV_ReleaseChannel(&dspiState->dmaSrc2CmdData);
        EDMA_DRV_ReleaseChannel(&dspiState->dmaFifo2Receive);
    }

    /* Clear state pointer. */
    g_dspiStatePtr[instance] = NULL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_DRV_MasterSetDelay
 * Description   : Configures the DSPI master mode bus timing delay options.
 * This function allows the user to take advantage of the DSPI module's delay options in order to
 * "fine tune" some of the signal timings to match the timing needs of a slower peripheral device.
 * This is an optional function that can be called after the DSPI module has been initialized for
 * master mode.
 * The bus timing delays that can be adjusted are listed below:
 *
 * PCS to SCK Delay: Adjustable delay option between the assertion of the PCS signal to the
 *                   first SCK edge.
 *
 * After SCK Delay: Adjustable delay option between the last edge of SCK to the de-assertion
 *                  of the PCS signal.
 *
 * Delay after Transfer:  Adjustable delay option between the de-assertion of the PCS signal for a
 *                        frame to the assertion of the PCS signal for the next frame. Note this
 *                        is not adjustable for continuous clock mode as this delay is fixed at
 *                        one SCK period.
 *
 * Each of the above delay parameters use both a pre-scalar and scalar value to calculate the
 * needed delay. This function will take in as a parameter the desired delay type and the
 * delay value (in nano-seconds) and will calculate the values needed for the prescaler and scaler
 * and return the actual calculated delay as an exact delay match may not be acheivable. In this
 * case, the closest match will be calculated without going below the desired delay value input.
 * It is possible to input a very large delay value that exceeds the capability of the part, in
 * which case the maximum supported delay will be returned.  In addition the function will return
 * an out-of-range status to alert the user.
 *END**************************************************************************/
dspi_status_t DSPI_DRV_MasterSetDelay(uint32_t instance, dspi_delay_type_t whichDelay,
                                      uint32_t delayInNanoSec, uint32_t * calculatedDelay)
{
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];
    uint32_t baseAddr = g_dspiBaseAddr[instance];
    dspi_status_t errorCode = kStatus_DSPI_Success;

    *calculatedDelay = DSPI_HAL_CalculateDelay(baseAddr, dspiState->whichCtar, whichDelay,
                                               dspiState->dspiSourceClock, delayInNanoSec);

    /* If the desired delay exceeds the capability of the device, alert the user */
    if (*calculatedDelay < delayInNanoSec)
    {
        errorCode = kStatus_DSPI_OutOfRange;
    }

    return errorCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_DRV_MasterConfigureBus
 * Description   : Configures the DSPI port physical parameters to access a device on the bus.
 * The term "device" is used to indicate the SPI device for which the DSPI master is communicating.
 * The user has two options to configure the device parameters: either pass in the
 * pointer to the device configuration structure to the desired transfer function (see
 * DSPI_DRV_MasterTransferDataBlocking or DSPI_DRV_MasterTransferData) or pass it in to the
 * DSPI_DRV_MasterConfigureBus function.  The user can pass in a device structure to the transfer
 * function which will contain the parameters for the bus (the transfer function will then call
 * this function). However, the user has the option to call this function directly especially if
 * they wish to obtain the calculated baud rate, at which point they may pass in NULL for the device
 * struct in the transfer function (assuming they have called this configure bus function
 * first). The following is an example of how to set up the dspi_device_t structure and how to call
 * the DSPI_DRV_MasterConfigureBus function by passing in these parameters:
 *   dspi_device_t spiDevice;
 *   spiDevice.dataBusConfig.bitsPerFrame = 16;
 *   spiDevice.dataBusConfig.clkPhase = kDspiClockPhase_FirstEdge;
 *   spiDevice.dataBusConfig.clkPolarity = kDspiClockPolarity_ActiveHigh;
 *   spiDevice.dataBusConfig.direction = kDspiMsbFirst;
 *   spiDevice.bitsPerSec = 50000;
 *   DSPI_DRV_MasterConfigureBus(instance, &spiDevice, &calculatedBaudRate);
 *
 *END**************************************************************************/
dspi_status_t DSPI_DRV_MasterConfigureBus(uint32_t instance,
                                          const dspi_device_t * device,
                                          uint32_t * calculatedBaudRate)
{
    assert(device);
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];
    uint32_t baseAddr = g_dspiBaseAddr[instance];

    dspi_status_t errorCode = kStatus_DSPI_Success;

    /* Configure the bus to access the provided device.*/
    *calculatedBaudRate = DSPI_HAL_SetBaudRate(baseAddr, dspiState->whichCtar, device->bitsPerSec,
                      dspiState->dspiSourceClock);
    errorCode = DSPI_HAL_SetDataFormat(baseAddr, dspiState->whichCtar,
                                               &device->dataBusConfig);
    dspiState->bitsPerFrame = device->dataBusConfig.bitsPerFrame; /* update dspiState bits/frame */

    return errorCode;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_DRV_MasterTransferDataBlocking
 * Description   : Perform a blocking SPI master mode transfer.
 * This function simultaneously sends and receives data on the SPI bus, as SPI is naturally
 * a full-duplex bus. The function will not return until the transfer is complete.
 *
 *END**************************************************************************/
dspi_status_t DSPI_DRV_MasterTransferDataBlocking(uint32_t instance,
                                                  const dspi_device_t * restrict device,
                                                  const uint8_t * sendBuffer,
                                                  uint8_t * receiveBuffer,
                                                  size_t transferByteCount,
                                                  uint32_t timeout)
{
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];

    /* fill in members of the run-time state struct*/
    dspiState->isTransferAsync = false;
    dspiState->sendBuffer = (const uint8_t *)sendBuffer;
    dspiState->receiveBuffer = (uint8_t *)receiveBuffer;
    dspiState->remainingSendByteCount = transferByteCount;
    dspiState->remainingReceiveByteCount = transferByteCount;

    /* Init the interrupt sync object.*/
    OSA_SemaCreate(&dspiState->irqSync, 0);

    /* start the transfer process*/
    if (dspiState->useDma)
    {
        if (DSPI_DRV_MasterStartTransferDma(instance, device) == kStatus_DSPI_Busy)
        {
            return kStatus_DSPI_Busy;
        }
    }
    else
    {
        if (DSPI_DRV_MasterStartTransfer(instance, device) == kStatus_DSPI_Busy)
        {
            return kStatus_DSPI_Busy;
        }
    }

    /* As this is a synchronous transfer, wait until the transfer is complete.*/
    dspi_status_t error = kStatus_DSPI_Success;
    osa_status_t syncStatus;

    do
    {
        syncStatus = OSA_SemaWait(&dspiState->irqSync, timeout);
    }while(syncStatus == kStatus_OSA_Idle);

    if (syncStatus != kStatus_OSA_Success)
    {
        /* Abort the transfer so it doesn't continue unexpectedly.*/
        DSPI_DRV_MasterAbortTransfer(instance);

        error = kStatus_DSPI_Timeout;
    }

    return error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_DRV_MasterTransferData
 * Description   : Perform a non-blocking SPI master mode transfer.
 * This function will return immediately. It is the user's responsiblity to check back to
 * ascertain if the transfer is complete (using the DSPI_DRV_MasterGetTransferStatus function). This
 * function simultaneously sends and receives data on the SPI bus, as SPI is naturally
 * a full-duplex bus.
 *
 *END**************************************************************************/
dspi_status_t DSPI_DRV_MasterTransferData(uint32_t instance,
                                          const dspi_device_t * restrict device,
                                          const uint8_t * sendBuffer,
                                          uint8_t * receiveBuffer,
                                          size_t transferByteCount)
{
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];

    /* fill in members of the run-time state struct*/
    dspiState->isTransferAsync = true;
    dspiState->sendBuffer = sendBuffer;
    dspiState->receiveBuffer = (uint8_t *)receiveBuffer;
    dspiState->remainingSendByteCount = transferByteCount;
    dspiState->remainingReceiveByteCount = transferByteCount;

    /* start the transfer process*/
    if (dspiState->useDma)
    {
        if (DSPI_DRV_MasterStartTransferDma(instance, device) == kStatus_DSPI_Busy)
        {
            return kStatus_DSPI_Busy;
        }
    }
    else
    {
        if (DSPI_DRV_MasterStartTransfer(instance, device) == kStatus_DSPI_Busy)
        {
            return kStatus_DSPI_Busy;
        }
    }

    /* Else, return immediately as this is an async transfer */
    return kStatus_DSPI_Success;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_DRV_MasterGetTransferStatus
 * Description   : Returns whether the previous transfer has finished yet.
 * When performing an async transfer, the user can call this function to ascertain the state of the
 * current transfer: in progress (or busy) or complete (success). In addition, if the transfer
 * is still in progress, the user can obtain the number of words that have been currently
 * transferred.
 *
 *END**************************************************************************/
dspi_status_t DSPI_DRV_MasterGetTransferStatus(uint32_t instance, uint32_t * framesTransferred)
{
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];
    uint32_t baseAddr = g_dspiBaseAddr[instance];

    /* Fill in the bytes transferred.*/
    if (framesTransferred)
    {
        *framesTransferred = DSPI_HAL_GetTransferCount(baseAddr);
    }

    return (dspiState->isTransferInProgress ? kStatus_DSPI_Busy : kStatus_DSPI_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_DRV_MasterAbortTransfer
 * Description   : Terminates an asynchronous transfer early.
 * During an async transfer, the user has the option to terminate the transfer early if the transfer
 * is still in progress.
 *
 *END**************************************************************************/
dspi_status_t DSPI_DRV_MasterAbortTransfer(uint32_t instance)
{
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];

    /* Check if a transfer is running.*/
    if (!dspiState->isTransferInProgress)
    {
        return kStatus_DSPI_NoTransferInProgress;
    }

    /* Stop the running transfer.*/
    DSPI_DRV_MasterCompleteTransfer(instance);

    return kStatus_DSPI_Success;
}

/*!
 * @brief Initiate (start) a transfer. This is not a public API as it is called from other
 *  driver functions
 */
static dspi_status_t DSPI_DRV_MasterStartTransfer(uint32_t instance,
                                                  const dspi_device_t * restrict device)
{
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];
    uint32_t baseAddr = g_dspiBaseAddr[instance];
    uint32_t calculatedBaudRate;

    /* Check the transfer byte count. If bits/frame > 8, meaning 2 bytes, then
     * the transfer byte count must not be an odd count, if so, drop the last odd byte.
     * Perform this operation here to ensure we get the latest bits/frame setting.
     */
    if (dspiState->bitsPerFrame > 8)
    {
        dspiState->remainingSendByteCount &= ~1UL;
        dspiState->remainingReceiveByteCount &= ~1UL;
    }

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

    /* Save information about the transfer for use by the ISR.*/
    dspiState->isTransferInProgress = true;

    /* Enable the DSPI module for the duration of this transfer.*/
    DSPI_HAL_Enable(baseAddr);

    /* flush the fifos*/
    DSPI_HAL_SetFlushFifoCmd(baseAddr, true, true);

    /* Fill of the TX FIFO with initial data */
    DSPI_DRV_MasterFillupTxFifo(instance, true);

    /* RX FIFO Drain request: RFDF_RE to enable RFDF interrupt
     * Since SPI is a synchronous interface, we only need to enable the RX interrupt.
     * The IRQ handler will get the status of RX and TX interrupt flags.
     */
    DSPI_HAL_SetRxFifoDrainDmaIntMode(baseAddr, kDspiGenerateIntReq, true);

    return kStatus_DSPI_Success;
}

/*!
 * @brief Fill up the TX FIFO with data.
 * This function fills up the TX FIFO with initial data for start of transfers where it will
 * first clear the transfer count.  Otherwise, if the TX FIFO fill is part of an ongoing transfer
 * then do not clear the transfer count.  The param "isInitialData" is used to determine if this
 * is an initial data fill.
 * This is not a public API as it is called from other driver functions.
 */
void DSPI_DRV_MasterFillupTxFifo(uint32_t instance, bool isInitialData)
{
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];
    uint32_t baseAddr = g_dspiBaseAddr[instance];
    dspi_command_config_t command;  /* create an instance of the data command struct*/
    uint16_t wordToSend = 0;
    uint32_t firstWordFlag;
    /* Declare variables for storing volatile data later in the code */
    uint32_t remainingReceiveByteCount, remainingSendByteCount;

    /* Before sending the data, we first need to initialize the data command struct
     * Configure the data command attributes for the desired PCS, CTAR, and continuous PCS
     * which are derived from the run-time state struct
     */
    command.whichPcs = dspiState->whichPcs;
    command.whichCtar = dspiState->whichCtar;
    command.isChipSelectContinuous = dspiState->isChipSelectContinuous;
    command.isEndOfQueue = 0;

    /* If the data is the first word to be sent, then enable "clear the transfer count"
     * and set the firstWord flag. After the first word is sent, clear both of these.
     * Also, clear the end of queue member and set when it is determined that the last data
     * word is to be sent
     */
    if (isInitialData)
    {
        command.clearTransferCount = 1;
        firstWordFlag = 1;
    }
    /* Otherwise, if filling the TX FIFO is part of an ongoing transfer (like being called from
     * the ISR), then clear these.
     */
    else
    {
        command.clearTransferCount = 0;
        firstWordFlag = 0;
    }

    /* Architectural note: When developing the TX FIFO fill functionality, it was found that to
     * achieve more efficient run-time performance, it was better to first check the bits/frame
     * setting and then proceed with the FIFO fill management process, rather than to clutter the
     * FIFO fill process with continual checks of the bits/frame setting.
     */

    /* If bits/frame is greater than one byte */
    if (dspiState->bitsPerFrame > 8)
    {
        /* Fill the fifo until it is full or until the send word count is 0 or until the difference
         * between the remainingReceiveByteCount and remainingSendByteCount equals the FIFO depth.
         * The reason for checking the difference is to ensure we only send as much as the
         * RX FIFO can receive.
         * For this case wher bitsPerFrame > 8, each entry in the FIFO contains 2 bytes of the
         * send data, hence the difference between the remainingReceiveByteCount and
         * remainingSendByteCount must be divided by 2 to convert this difference into a
         * 16-bit (2 byte) value.
         */
        /* Store the DSPI state struct volatile member variables into temporary
         * non-volatile variables to allow for MISRA compliant calculations
         */
        remainingReceiveByteCount = dspiState->remainingReceiveByteCount;
        remainingSendByteCount = dspiState->remainingSendByteCount;

        while((DSPI_HAL_GetStatusFlag(baseAddr, kDspiTxFifoFillRequest) == 1) &&
              ((remainingReceiveByteCount - remainingSendByteCount)/2 <
                FSL_FEATURE_DSPI_FIFO_SIZEn(instance)))
        {
            /* On the last word to be sent, set the end of queue flag in the data command struct
             * and ensure that the CONT bit in the PUSHR is also cleared even if it was cleared to
             * begin with. If CONT is set it means continuous chip select operation and to ensure
             * the chip select is de-asserted, this bit must be cleared on the last data word.
             */
            if ((dspiState->remainingSendByteCount - 2) == 0)
            {
                command.isEndOfQueue = 1;
                command.isChipSelectContinuous = 0;
            }

            /* If this is the first word to be transmitted, clear firstWord flag after transmit */
            if (firstWordFlag == 1)
            {
                /* If a send buffer was provided, the word comes from there. Otherwise we just send
                 * a zero (initialized above).
                 */
                if (dspiState->sendBuffer)
                {
                    wordToSend = *(dspiState->sendBuffer);
                    ++dspiState->sendBuffer; /* increment to next data byte */
                    wordToSend |= (unsigned)(*(dspiState->sendBuffer)) << 8U;
                    ++dspiState->sendBuffer; /* increment to next data byte */
                }

                DSPI_HAL_WriteDataMastermode(baseAddr, &command, wordToSend);
                dspiState->remainingSendByteCount -= 2; /* decrement remainingSendByteCount by 2 */

                firstWordFlag = 0; /* clear firstWordFlag */
                command.clearTransferCount = 0; /* Disable clear transfer count for remaining data*/
            }
            else
            {
                /* If a send buffer was provided, the word comes from there. Otherwise we just send
                 * a zero (initialized above).
                 */
                if (dspiState->sendBuffer)
                {
                    wordToSend = *(dspiState->sendBuffer);
                    ++dspiState->sendBuffer; /* increment to next data byte */
                    wordToSend |= (unsigned)(*(dspiState->sendBuffer)) << 8U;
                    ++dspiState->sendBuffer; /* increment to next data byte */
                }
                DSPI_HAL_WriteDataMastermode(baseAddr, &command, wordToSend);
                dspiState->remainingSendByteCount -= 2; /* decrement remainingSendByteCount by 2 */
            }

            /* try to clear TFFF by writing a one to it; it will not clear if TX FIFO not full*/
            DSPI_HAL_ClearStatusFlag(baseAddr, kDspiTxFifoFillRequest);

            /* Store the DSPI state struct volatile member variables into temporary
             * non-volatile variables to allow for MISRA compliant calculations
             */
            remainingReceiveByteCount = dspiState->remainingReceiveByteCount;
            remainingSendByteCount = dspiState->remainingSendByteCount;

            /* exit loop if send count is zero */
            if (dspiState->remainingSendByteCount == 0)
            {
                break;
            }
        } /* End of TX FIFO fill while loop */
    }
    /* Optimized for bits/frame less than or equal to one byte. */
    else
    {
        /* Fill the fifo until it is full or until the send word count is 0 or until the difference
         * between the remainingReceiveByteCount and remainingSendByteCount equals the FIFO depth.
         * The reason for checking the difference is to ensure we only send as much as the
         * RX FIFO can receive.
         */
        /* Store the DSPI state struct volatile member variables into temporary
         * non-volatile variables to allow for MISRA compliant calculations
         */
        remainingReceiveByteCount = dspiState->remainingReceiveByteCount;
        remainingSendByteCount = dspiState->remainingSendByteCount;

        while((DSPI_HAL_GetStatusFlag(baseAddr, kDspiTxFifoFillRequest) == 1) &&
              ((remainingReceiveByteCount - remainingSendByteCount) <
                FSL_FEATURE_DSPI_FIFO_SIZEn(instance)))
        {
            /* On the last word to be sent, set the end of queue flag in the data command struct
             * and ensure that the CONT bit in the PUSHR is also cleared even if it was cleared to
             * begin with. If CONT is set it means continuous chip select operation and to ensure
             * the chip select is de-asserted, this bit must be cleared on the last data word.
             */
            if ((dspiState->remainingSendByteCount - 1) == 0)
            {
                command.isEndOfQueue = 1;
                command.isChipSelectContinuous = 0;
            }

            /* If this is the first word to be transmitted, clear firstWord flag after transmit*/
            if (firstWordFlag == 1)
            {
                /* If a send buffer was provided, the word comes from there. Otherwise we just send
                 * a zero (initialized above).
                 */
                if (dspiState->sendBuffer)
                {
                    wordToSend = *(dspiState->sendBuffer);
                    ++dspiState->sendBuffer; /* increment to next data byte */
                }
                DSPI_HAL_WriteDataMastermode(baseAddr, &command, wordToSend);
                --dspiState->remainingSendByteCount; /* decrement remainingSendByteCount */

                firstWordFlag = 0; /* clear firstWordFlag */
                command.clearTransferCount = 0; /* Disable clear transfer count for remaining data*/
            }
            else
            {
                /* If a send buffer was provided, the word comes from there. Otherwise we just send
                 * a zero (initialized above).
                 */
                if (dspiState->sendBuffer)
                {
                    wordToSend = *(dspiState->sendBuffer);
                    ++dspiState->sendBuffer; /* increment to next data word*/
                }
                DSPI_HAL_WriteDataMastermode(baseAddr, &command, wordToSend);
                --dspiState->remainingSendByteCount; /* decrement remainingSendByteCount*/
            }

            /* try to clear TFFF by writing a one to it; it will not clear if TX FIFO not full*/
            DSPI_HAL_ClearStatusFlag(baseAddr, kDspiTxFifoFillRequest);

            /* Store the DSPI state struct volatile member variables into temporary
             * non-volatile variables to allow for MISRA compliant calculations
             */
            remainingReceiveByteCount = dspiState->remainingReceiveByteCount;
            remainingSendByteCount = dspiState->remainingSendByteCount;

            /* exit loop if send count is zero */
            if (dspiState->remainingSendByteCount == 0)
            {
                break;
            }
        } /* End of TX FIFO fill while loop */
    }
}

/*!
 * @brief Finish up a transfer.
 * Cleans up after a transfer is complete. Interrupts are disabled, and the DSPI module
 * is disabled. This is not a public API as it is called from other driver functions.
 */
static void DSPI_DRV_MasterCompleteTransfer(uint32_t instance)
{
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];
    uint32_t baseAddr = g_dspiBaseAddr[instance];

    /* The transfer is complete.*/
    dspiState->isTransferInProgress = false;

    /* Disable interrupt requests*/
    /* RX FIFO Drain request: RFDF_RE */
    DSPI_HAL_SetRxFifoDrainDmaIntMode(baseAddr, kDspiGenerateIntReq, false);

    /* Disable TX FIFO Fill request that may be enabled by the DMA operation, which is ok to
     * disable even if DMA is not enabled since this should be disabled anyway
     */
    DSPI_HAL_SetTxFifoFillDmaIntMode(baseAddr, kDspiGenerateIntReq, false);

    /* Disable End of Queue request */
    DSPI_HAL_SetIntMode(baseAddr, kDspiEndOfQueue, false);

    /* Signal the synchronous completion object regardless if the transfer was blocking or not.
     * There is no harm in calling this for async functions and the blocking function will
     * re-initialize the sync object
     */
    OSA_SemaPost(&dspiState->irqSync);

    /* Transfer is complete, so disable the module. It gets re-enabled during the
     * start transfer function
     */
    DSPI_HAL_Disable(baseAddr);
}

/*!
 * @brief Interrupt handler for DSPI master mode.
 * This handler uses the buffers stored in the dspi_master_state_t structs to transfer data.
 * This is not a public API as it is called whenever an interrupt occurs.
 */
void DSPI_DRV_MasterIRQHandler(uint32_t instance)
{
    /* instantiate local variable of type dspi_master_state_t and point to global state */
    dspi_master_state_t * dspiState = (dspi_master_state_t *)g_dspiStatePtr[instance];

    uint32_t baseAddr = g_dspiBaseAddr[instance];

    /* For interrupt driver operation (non-DMA operation) */
    if (!(dspiState->useDma))
    {
        /* Check read buffer.*/
        uint16_t wordReceived; /* Maximum supported data bit length in master mode is 16-bits */

        /* If bits/frame is greater than one byte */
        if (dspiState->bitsPerFrame > 8)
        {
            while (DSPI_HAL_GetStatusFlag(baseAddr, kDspiRxFifoDrainRequest))
            {
                wordReceived = DSPI_HAL_ReadData(baseAddr);
                /* clear the rx fifo drain request, needed for non-DMA applications as this flag
                 * will remain set even if the rx fifo is empty. By manually clearing this flag, it
                 * either remain clear if no more data is in the fifo, or it will set if there is
                 * more data in the fifo.
                 */
                DSPI_HAL_ClearStatusFlag(baseAddr, kDspiRxFifoDrainRequest);

                if (dspiState->receiveBuffer)
                {
                    *dspiState->receiveBuffer = wordReceived; /* Write first data byte */
                    ++dspiState->receiveBuffer; /* increment to next data byte */
                    *dspiState->receiveBuffer = wordReceived >> 8; /* Write second data byte */
                    ++dspiState->receiveBuffer; /* increment to next data byte */
                }
                dspiState->remainingReceiveByteCount -= 2;

                if (dspiState->remainingReceiveByteCount == 0)
                {
                    break;
                }
            } /* End of RX FIFO drain while loop */
        }
        /* Optimized for bits/frame less than or equal to one byte. */
        else
        {
            while (DSPI_HAL_GetStatusFlag(baseAddr, kDspiRxFifoDrainRequest))
            {
                wordReceived = DSPI_HAL_ReadData(baseAddr);
                /* clear the rx fifo drain request, needed for non-DMA applications as this flag
                 * will remain set even if the rx fifo is empty. By manually clearing this flag, it
                 * either remain clear if no more data is in the fifo, or it will set if there is
                 * more data in the fifo.
                 */
                DSPI_HAL_ClearStatusFlag(baseAddr, kDspiRxFifoDrainRequest);

                if (dspiState->receiveBuffer)
                {
                    *dspiState->receiveBuffer = wordReceived;
                    ++dspiState->receiveBuffer;
                }
                --dspiState->remainingReceiveByteCount;

                if (dspiState->remainingReceiveByteCount == 0)
                {
                    break;
                }
            } /* End of RX FIFO drain while loop */
        }

        /* Check write buffer. We always have to send a word in order to keep the transfer
         * moving. So if the caller didn't provide a send buffer, we just send a zero.
         */
        if (dspiState->remainingSendByteCount)
        {
            /* Fill of the TX FIFO with ongoing data */
            DSPI_DRV_MasterFillupTxFifo(instance, false);
        }

        /* Check if we're done with this transfer.*/
        if ((dspiState->remainingSendByteCount == 0) && (dspiState->remainingReceiveByteCount == 0))
        {
            /* Complete the transfer. This disables the interrupts, so we don't wind up in
             * the ISR again.
             */
            DSPI_DRV_MasterCompleteTransfer(instance);
        }
    }
    else /* For DMA based operations */
    {
        /* If the interrupt is due to an end-of-queue, then complete. This interrupt is
         * is used during DMA operations and we want to handle this interrupt only
         * when DMA is being used.  For interrupt-driven transfer, we complete the transfer only
         * when remaining send and receive byte counts are zero.
         */
        if (DSPI_HAL_GetStatusFlag(baseAddr, kDspiEndOfQueue))
        {
            /* Complete the transfer. This disables the interrupts, so we don't wind up in
             * the ISR again.
             */
            DSPI_DRV_MasterCompleteTransfer(instance);
        }
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

