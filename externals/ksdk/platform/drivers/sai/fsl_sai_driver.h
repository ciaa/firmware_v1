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


#ifndef __FSL_SAI_DRIVER_H__
#define __FSL_SAI_DRIVER_H__

#include "fsl_sai_hal.h"
#include "fsl_sai_common.h"

/*!
 * @addtogroup sai_driver
 * @{ 
 */

/*! @file */

/*! @brief SAI callback function */
typedef void (*sai_callback_t)(void *parameter);

/*! @brief Status structure for SAI */
typedef enum _sai_status
{
    kStatus_SAI_Success = 0U,
    kStatus_SAI_Fail = 1U,
    kStatus_SAI_DeviceBusy = 2U
} sai_status_t;

typedef enum _sai_mono_streo
{
    kSaiMono = 0x0,
    kSaiStreo = 0x1
} sai_mono_streo_t;

/*! @brief Defines the PCM data format */
typedef struct SaiAudioDataFormat
{
    uint32_t sample_rate;/*!< Sample rate of the PCM file */
    uint32_t mclk;/*!< Master clock frequency */
    uint8_t  bits;/*!< How many bits in a word */
    sai_mono_streo_t  mono_streo;/*!< How many word in a frame */
} sai_data_format_t;

/*! @brief SAI internal state 
* Users should allocate and transfer memory to the PD during the initialization function.
* Note: During the SAI execution, users should not free the state. Otherwise, the driver malfunctions.
*/
typedef struct sai_state
{
    sai_data_format_t format;
    uint8_t * address;
    uint32_t len;
    uint32_t count;
    sai_callback_t  callback;
    void * callback_param;
    sai_sync_mode_t sync_mode;
    uint32_t fifo_channel;
    uint32_t watermark;
    sai_master_slave_t master_slave;
} sai_state_t;

/*! @brief The description structure for the SAI TX/RX module. */
typedef struct SaiUserConfig
{
    sai_mclk_source_t   mclk_source;/*!< Master clock source. */
    bool                mclk_divide_enable;/*!< Enable the divide of master clock to generate bit clock. */
    sai_sync_mode_t     sync_mode;/*!< Synchronous or asynchronous. */
    sai_protocol_t           protocol;/*!< I2S left, I2S right or I2S type. */
    sai_master_slave_t  slave_master;/*!< Master or slave. */
    sai_bclk_source_t   bclk_source;/*!< Bit clock from master clock or other modules. */
    uint8_t             channel;/*!< Which FIFO is used to transfer. */
    uint32_t    watermark;
}  sai_user_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the SAI module.
 *
 * This  function  initializes the SAI registers according to the configuration
 * structure. This function also initializes the basic SAI settings including
 * board-relevant settings.
 * Notice: This function does not initialize an entire SAI instance. It
 * only initializes the TX according to the value in the handler.
 * @param instance SAI module instance.
 * @param config The configuration structure of SAI.
 * @param state Pointer of SAI run state structure.
 * @return Return kStatus_SAI_Success while the initialize success and kStatus_SAI_Fail if failed.
 */
sai_status_t SAI_DRV_TxInit(uint32_t instance, sai_user_config_t * config, sai_state_t *state);

/*!
 * @brief Initializes the SAI Rx module.
 *
 * This  function  initializes the SAI registers according to the configuration
 * structure. This function also initializes the basic SAI settings including
 * board-relevant settings.
 * Note that this function does not initialize an entire SAI instance. This function
 * only initializes the TX according to the value in the handler.
 * @param instance SAI module instance.
 * @param config The configuration structure of SAI.
 * @param state Pointer of SAI run state structure.
 * @return Return kStatus_SAI_Success while the initialize success and kStatus_SAI_Fail if failed.
 */
sai_status_t SAI_DRV_RxInit(uint32_t instance, sai_user_config_t * config, sai_state_t *state);

/*! @brief This function gets the default setting of the user configuration.
* 
* The default settings for SAI are:
* - Audio protocol is I2S format
* - Watermark is 4
* - Use SAI0
* - Channel is channel0
* - SAI as master 
* - MCLK from system core clock
* - Tx is in an asynchronous mode
* @param config Pointer of user configure structure.
*/
void SAI_DRV_TxGetDefaultSetting(sai_user_config_t *config);

/*! @brief This function gets the default setting of the user configuration.
* 
* The default settings for SAI are:
* - Audio protocol is I2S format
* - Watermark is 4
* - Use SAI0
* - Data channel is channel0
* - SAI as master
* - MCLK from system core clock
* - Rx is in synchronous way
* @param config Pointer of user configure structure.
*/
void SAI_DRV_RxGetDefaultSetting(sai_user_config_t *config);

/*!
 * @brief De-initializes the SAI Tx module.
 *
 * This function  closes the SAI Tx device. However, it does not close the entire SAI instance.
 * It only closes the clock gate while both Tx and Rx are closed in the same instance.
 * @param instance SAI module instance.
 * @return Return kStatus_SAI_Success while the process success and kStatus_SAI_Fail if failed.
 */
sai_status_t SAI_DRV_TxDeinit(uint32_t instance);

/*!
 * @brief De-initializes the SAI Rx module.
 *
 * This function closes the SAI Rx device. However, it does not close the entire SAI instance.
 * It only closes the clock gate while both Tx and Rx are closed in the same instance.
 * @param instance SAI module instance.
 * @return Return kStatus_SAI_Success while the process success and kStatus_SAI_Fail if failed.
 */
sai_status_t SAI_DRV_RxDeinit(uint32_t instance);

/*!
 * @brief Configures audio data format of Tx.
 *
 * The function  configures  an audio sample rate, data bits, and a channel number.
 * @param instance SAI module instance.
 * @param format PCM data format structure pointer.
 * @return Return kStatus_SAI_Success while the process success and kStatus_SAI_Fail if failed.
 */
sai_status_t SAI_DRV_TxConfigDataFormat(uint32_t instance,sai_data_format_t *format);

/*!
 * @brief Configures audio data format of Rx.
 *
 * The function  configures  an audio sample rate, data bits, and a channel number.
 * @param instance SAI module instance of the SAI module.
 * @param format PCM data format structure pointer.
 * @return Return kStatus_SAI_Success while the process success and kStatus_SAI_Fail if failed.
 */
sai_status_t SAI_DRV_RxConfigDataFormat(uint32_t instance,sai_data_format_t *format);

/*!
 * @brief Starts the Tx transfer.
 *
 * The function  enables the interrupt/DMA request source and the transmit channel.
 * @param instance SAI module instance.
 */
void SAI_DRV_TxStartModule(uint32_t instance);

/*!
 * @brief Starts the Rx receive process.
 *
 * The function  enables the interrupt/DMA request source and the transmit channel.
 * @param instance SAI module instance of the SAI module.
 */
void SAI_DRV_RxStartModule(uint32_t instance);

/*!
 * @brief Stops writing data to FIFO to disable the DMA or the interrupt request bit.
 *
 * This function provides the method to pause writing data.  
 * @param instance SAI module instance.
 */
static inline void SAI_DRV_TxStopModule(uint32_t instance)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    SAI_HAL_TxSetIntCmd(reg_base,kSaiIntrequestFIFORequest,false);
    SAI_HAL_TxSetDmaCmd(reg_base,kSaiDmaReqFIFORequest, false);
}

/*!
 * @brief Stops receiving data from FIFO to disable the DMA or the interrupt request bit.
 *
 * This function provides the method to pause writing data.  
 * @param instance SAI module instance.
 */
static inline void SAI_DRV_RxStopModule(uint32_t instance)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    SAI_HAL_RxSetIntCmd(reg_base,kSaiIntrequestFIFORequest,false);
    SAI_HAL_RxSetDmaCmd(reg_base,kSaiDmaReqFIFORequest, false);
}


/*! @brief Enables or disables the Tx interrupt source.
* @param instance SAI module instance.
* @param enable True means enable interrupt source, false means disable interrupt source.
*/
static inline void SAI_DRV_TxSetIntCmd(uint32_t instance, bool enable)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    SAI_HAL_TxSetIntCmd(reg_base,kSaiIntrequestFIFORequest,enable);
    SAI_HAL_TxSetIntCmd(reg_base, kSaiIntrequestFIFOError, enable);
}

/*! @brief Enables or disables the Rx interrupt source.
* @param instance SAI module instance.
* @param enable True means enable interrupt source, false means disable interrupt source.
*/
static inline void SAI_DRV_RxSetIntCmd(uint32_t instance, bool enable)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    SAI_HAL_RxSetIntCmd(reg_base,kSaiIntrequestFIFORequest,enable);
    SAI_HAL_RxSetIntCmd(reg_base, kSaiIntrequestFIFOError,enable);
}

/*! @brief Enables or disables the Tx DMA source.
* @param instance SAI module instance.
* @param enable True means enable DMA source, false means disable DMA source.
*/
static inline void SAI_DRV_TxSetDmaCmd(uint32_t instance,  bool enable)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    SAI_HAL_TxSetDmaCmd(reg_base, kSaiDmaReqFIFORequest,enable);
    SAI_HAL_TxSetIntCmd(reg_base, kSaiIntrequestFIFOError,enable);
}

/*! @brief Enables or disables the Rx interrupt source.
* @param instance SAI module instance.
* @param enable True means enable DMA source, false means disable DMA source.
*/
static inline void SAI_DRV_RxSetDmaCmd(uint32_t instance, bool enable)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    SAI_HAL_RxSetDmaCmd(reg_base, kSaiDmaReqFIFORequest,enable);
    SAI_HAL_RxSetIntCmd(reg_base, kSaiIntrequestFIFOError,enable);    
}

/*! @brief Sets the Tx watermark.
*
* Setting the watermark means that while the data number in FIFO is less or equal to the watermark, it generates an interrupt
* request or the DMA request.
* @param instance SAI module instance.
* @param watermark Watermark number needs to set.
*/
void SAI_DRV_TxSetWatermark(uint32_t instance,uint32_t watermark);

/*! @brief Sets the Rx watermark.
*
* Setting the watermark means that while the data number in FIFO is more or equal to the watermark, it generates an interrupt
* request or the DMA request.
* @param instance SAI module instance.
* @param watermark Watermark number needs to set.
*/
void SAI_DRV_RxSetWatermark(uint32_t instance,uint32_t watermark);

/*! @brief Gets the Tx watermark.
*
* The watermark should be changed according to a different audio sample rate.
* @param instance SAI module instance.
* @return Watermark number in TCR1.
*/
static inline uint32_t SAI_DRV_TxGetWatermark(uint32_t instance)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    return SAI_HAL_TxGetWatermark(reg_base);
}

/*! @brief Gets the Rx watermark.
*
* The watermark should be changed according to a different audio sample rate.
* @param instance SAI module instance.
* @return Watermark number in RCR1.
*/
static inline uint32_t SAI_DRV_RxGetWatermark(uint32_t instance)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    return SAI_HAL_RxGetWatermark(reg_base);
}

/*!
 * @brief Gets the Tx FIFO address of the data channel.
 *
 * This function is mainly used for the DMA settings, which the DMA
 * configuration needs for the source/destination address of SAI.
 * @param instance SAI module instance of the SAI module.
 * @param fifo_channel FIFO channel of SAI Tx.
 * @return Returns the address of the data channel FIFO.
 */
static inline uint32_t* SAI_DRV_TxGetFifoAddr(uint32_t instance, uint32_t fifo_channel)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    return SAI_HAL_TxGetFifoAddr(reg_base, fifo_channel);
}

/*!
 * @brief Gets the Rx FIFO address of the data channel.
 *
 * This function is mainly used for the DMA settings, which the DMA
 * configuration needs for the source/destination address of SAI.
 * @param instance SAI module instance of the SAI module.
 * @param fifo_channel FIFO channel of SAI Rx.
 * @return Returns the address of the data channel FIFO.
 */
static inline uint32_t* SAI_DRV_RxGetFifoAddr(uint32_t instance, uint32_t fifo_channel)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    return SAI_HAL_RxGetFifoAddr(reg_base, fifo_channel);
}

/*!
 * @brief Sends date of a certain length.
 *
 * This function  sends the data to the Tx FIFO. This function  
 * starts the transfer, and, while finishing the transfer, calls the callback
 * function registered by users.
 * @param instance SAI module instance of the SAI module.
 * @param addr Address of the data which needs to be transferred.
 * @param len The data length which need to be sent.
 * @return Returns the length which was sent.
 */
uint32_t SAI_DRV_SendData(uint32_t instance, uint8_t *addr, uint32_t len);

/*!
 * @brief Receives a certain length data.
 *
 * This function  receives the data from the RX FIFO. This function  
 * starts the transfer, and, while finishing the transfer,  calls the callback
 * function registered by users.
 * @param instance SAI module instance.
 * @param addr Address of the data which needs to be transferred.
 * @param len The data length which needs to be received.
 * @return Returns the length received.
 */
uint32_t SAI_DRV_ReceiveData(uint32_t instance, uint8_t *addr, uint32_t len);

/*!
 * @brief Sends a certain length data in blocking way.
 *
 * This function  sends the data to the Tx FIFO, does not exit until
 * the data is sent to FIFO, and uses the polling way to send audio data.
 * @param instance SAI module instance.
 * @param addr Address of the data which needs to be transferred.
 * @param len The data length which need to be sent.
 * @return Returns the length which was sent.
 */
uint32_t SAI_DRV_SendDataBlocking(uint32_t instance, uint8_t *addr, uint32_t len);

/*!
 * @brief Receives data of a certain length in blocking way.
 *
 * This function receives data from the Rx FIFO, does not exit until
 * the data is received from FIFO, and uses the polling way to send audio data.
 * @param instance SAI module instance.
 * @param addr Address of the data which needs to be transferred.
 * @param len The data length which need to be sent.
 * @return Returns the length which was sent.
 */
uint32_t SAI_DRV_ReceiveDataBlocking(uint32_t instance,uint8_t * addr,uint32_t len);

/*!
 * @brief Registers the callback function after completing a send.
 *
 * This function  tells SAI which function needs to be called after a 
 * period length sending. This callback function is used for non-blocking sending.
 * @param instance SAI module instance.
 * @param callback Callback function defined by users.
 * @param callback_param The parameter of the callback function.
 */
void SAI_DRV_TxRegisterCallback(uint32_t instance, sai_callback_t callback, void *callback_param);

/*!
 * @brief Registers the callback function after completing a receive.
 *
 * This function  tells SAI which function needs to be called after a 
 * period length receive. This callback function is used for non-blocking receiving.
 * @param instance SAI module instance.
 * @param callback Callback function defined by users.
 * @param callback_param The parameter of the callback function.
 */
void SAI_DRV_RxRegisterCallback(uint32_t instance, sai_callback_t callback, void *callback_param);

/*!
 * @brief Default SAI Tx interrupt handler.
 *
 * This function sends data in the interrupt and checks the FIFO error.
 * @param instance SAI module instance.
 */
void SAI_DRV_TxIRQHandler(uint32_t instance);

/*!
 * @brief Default SAI Rx interrupt handler.
 *
 * This function receives data in the interrupt and checks the FIFO error.
 * @param instance SAI module instance.
 */
void SAI_DRV_RxIRQHandler(uint32_t instance);


#if defined(__cplusplus)
}
#endif

/*! @} */

#endif/* __FSL_SAI_DRIVER_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/

