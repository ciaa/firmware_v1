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
#if !defined(__FSL_DSPI_SLAVE_DRIVER_H__)
#define __FSL_DSPI_SLAVE_DRIVER_H__

#include "fsl_dspi_hal.h"
#include "fsl_edma_driver.h"

/*!
 * @addtogroup dspi_slave_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief The set of callbacks for the DSPI slave mode.
 *
 * The user creates the function implementations.
 */
typedef struct DSPISlaveCallbacks {
    /*! Callback to get word to transmit. */
    dspi_status_t (*dataSource)(uint8_t * sourceWord, uint32_t instance);

    /*! Callback to put received word. */
    dspi_status_t (*dataSink)(uint8_t sinkWord, uint32_t instance);

    /*! Callback to report a DSPI error, such as an under-run or over-run error. */
    void (*onError)(dspi_status_t error, uint32_t instance);

    /*! Callback to report the slave SPI DMA is done transferring data. Used only for
     * DMA enabled slave SPI operation and not used for interrupt operation.
     */
    void (*dspiDone)(void);

} dspi_slave_callbacks_t;

/*!
 * @brief Runtime state of the DSPI slave driver.
 *
 * This structure holds data that is used by the DSPI slave peripheral driver to
 * communicate between the transfer function and the interrupt handler. The user
 * needs to pass in the memory for this structure and the driver  fills out
 * the members.
 */
typedef struct DSPISlaveState {
    dspi_slave_callbacks_t callbacks;   /*!< Application/user callbacks */
    uint32_t bitsPerFrame;              /*!< Desired number of bits per frame */
    bool useDma;                        /*!< User option to invoke usage of DMA */
    edma_chn_state_t dmaSrc2TxFifo;       /*!< Structure definition for the eDMA channel */
    edma_chn_state_t dmaRxFifo2RxBuff;    /*!< Structure definition for the eDMA channel */
} dspi_slave_state_t;

/*!
 *  @brief User configuration structure and callback functions for the DSPI slave driver.
 */
typedef struct DSPISlaveUserConfig {
    dspi_slave_callbacks_t callbacks;     /*!< Application/user callbacks. */
    dspi_data_format_config_t dataConfig; /*!< Data format configuration structure */
} dspi_slave_user_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization and shutdown
 * @{
 */

/*!
 * @brief Initializes a DSPI instance for a slave mode operation.
 *
 * This function saves the callbacks to the run-time state structure for a later use in the
 * interrupt handler. It also ungates the clock to the DSPI module, initializes the DSPI
 * for slave mode, and enables the module and corresponding interrupts. Once initialized, the
 * DSPI module is configured in slave mode and ready to receive data from the SPI master. This
 * is an example to set up the dspi_slave_state_t and the dspi_slave_user_config_t
 * parameters and to call the DSPI_DRV_SlaveInit function by passing in these parameters:
   @code
    dspi_slave_state_t dspiSlaveState; <- the user simply allocates memory for this structure
    dspi_slave_user_config_t slaveUserConfig;
    slaveUserConfig.callbacks.dataSink = data_sink; <- set to user implementation of function
    slaveUserConfig.callbacks.dataSource = data_source; <- set to user implementation of function
    slaveUserConfig.callbacks.onError = on_error; <- set to user implementation of function
    slaveUserConfig.dataConfig.bitsPerFrame = 16; <- example setting
    slaveUserConfig.dataConfig.clkPhase = kDspiClockPhase_FirstEdge; <- example setting
    slaveUserConfig.dataConfig.clkPolarity = kDspiClockPolarity_ActiveHigh; <- example setting
    DSPI_DRV_SlaveInit(slaveInstance, &dspiSlaveState, &slaveUserConfig);
   @endcode
 *
 * @param instance The instance number of the DSPI peripheral.
 * @param dspiState The pointer to the DSPI slave driver state structure.
 * @param slaveConfig The configuration structure dspi_slave_user_config_t which configures
 *   the data bus format and also includes the callbacks.
 *
 * @return An error code or kStatus_DSPI_Success.
 */
dspi_status_t DSPI_DRV_SlaveInit(uint32_t instance,
                                 dspi_slave_state_t * dspiState,
                                 const dspi_slave_user_config_t * slaveConfig);


/*!
 * @brief  Initializes a DSPI instance for slave mode operation with DMA support.
 *
 * This function is exactly like the DSPI_DRV_SlaveInit function but in addition, adds DMA support.
 * This function saves the callbacks to the run-time state structure for later use in the
 * interrupt handler. However, unlike the CPU driven slave driver, there is no need to define
 * callbacks for the data sink or data source since the user passes in buffers
 * for the send and receive data, and the DMA engine uses those buffers.
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
   @code
    instance = slaveInstance; <- the desired module instance number
    dspi_slave_state_t dspiSlaveState; <- the user simply allocates memory for this struct
    dspi_slave_user_config_t slaveUserConfig;
    slaveUserConfig.callbacks.onError = on_error; <- set to user implementation of function
    slaveUserConfig.callbacks.dspiDone = dspi_Dma_Done; <- user defined callback
    slaveUserConfig.dataConfig.bitsPerFrame = 16; <- example setting
    slaveUserConfig.dataConfig.clkPhase = kDspiClockPhase_FirstEdge; <- example setting
    slaveUserConfig.dataConfig.clkPolarity = kDspiClockPolarity_ActiveHigh; <- example setting
    sendBuffer <- (pointer) to the source data buffer, can be NULL
    receiveBuffer <- (pointer) to the receive data buffer, can be NULL
    transferCount <- number of bytes to transfer

    DSPI_DRV_SlaveInitDma(slaveInstance, &dspiSlaveState, &slaveUserConfig,
                        &sendBuffer, &receiveBuffer, transferCount);
   @endcode
 *
 * @param instance The instance number of the DSPI peripheral.
 * @param dspiState The pointer to the DSPI slave driver state structure.
 * @param slaveConfig The configuration structure dspi_slave_user_config_t which configures
 *   the data bus format and also includes the callbacks.
 * @param sendBuffer The pointer to the data buffer of the data to send. You may pass NULL for this
 *  parameter, in which case bytes with a value of 0 (zero) are sent.
 * @param receiveBuffer Pointer to the buffer where the received bytes are stored. If you pass NULL
 *  for this parameter, the received bytes are ignored.
 * @param transferByteCount The expected number of bytes to transfer.
 * @return An error code or kStatus_DSPI_Success.
 */
dspi_status_t DSPI_DRV_SlaveInitDma(uint32_t instance,
                                    dspi_slave_state_t * dspiState,
                                    const dspi_slave_user_config_t * slaveConfig,
                                    const uint8_t * sendBuffer,
                                    uint8_t * receiveBuffer,
                                    size_t transferByteCount);

/*!
 * @brief Shuts down a DSPI instance.
 * Resets the DSPI peripheral, disables the interrupt to the core, and gates its clock.
 *
 * @param instance The instance number of the DSPI peripheral.
 */
void DSPI_DRV_SlaveDeinit(uint32_t instance);


/* @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* __FSL_DSPI_SLAVE_DRIVER_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/

