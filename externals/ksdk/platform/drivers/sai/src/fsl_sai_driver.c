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

#include "fsl_sai_driver.h"
#include "fsl_interrupt_manager.h"
#include "fsl_clock_manager.h"

/*******************************************************************************
 *Definitation
 ******************************************************************************/
static sai_state_t * volatile sai_state_ids[HW_I2S_INSTANCE_COUNT][2];

/*******************************************************************************
 * Code
 ******************************************************************************/
 
/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxInit
 * Description   : Initialize sai tx module, and initialize sai state.
 *
 *END**************************************************************************/
sai_status_t SAI_DRV_TxInit(uint32_t instance, sai_user_config_t * config, sai_state_t *state)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    /* Open clock gate for sai instance */
    CLOCK_SYS_EnableSaiClock(instance);
    /*Check if the device is busy */
    if(sai_state_ids[instance][0] != NULL)
    {
        return kStatus_SAI_DeviceBusy;
    }
    sai_state_ids[instance][0] = state;
    SAI_HAL_TxInit(reg_base);
    /* Mclk source select */
    if (config->slave_master == kSaiMaster)
    {
        SAI_HAL_SetMclkSrc(reg_base, config->mclk_source);
        SAI_HAL_TxSetBclkSrc(reg_base, config->bclk_source);
    }
    SAI_HAL_TxSetSyncMode(reg_base, config->sync_mode);
    SAI_HAL_TxSetMasterSlave(reg_base, config->slave_master);
    SAI_HAL_TxSetProtocol(reg_base, config->protocol);
    SAI_HAL_TxSetDataChn(reg_base, config->channel);
    SAI_HAL_TxSetWatermark(reg_base, config->watermark);

    /* Fill the state strucutre */
    sai_state_ids[instance][0]->sync_mode = config->sync_mode;
    sai_state_ids[instance][0]->fifo_channel = config->channel;
    sai_state_ids[instance][0]->watermark = config->watermark;
    sai_state_ids[instance][0]->master_slave = config->slave_master;
    INT_SYS_EnableIRQ(g_saiTxIrqId[instance]);
 
    return kStatus_SAI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxInit
 * Description   : Initialize sai rx module, and initialize sai state.
 *
 *END**************************************************************************/
sai_status_t SAI_DRV_RxInit(uint32_t instance, sai_user_config_t * config, sai_state_t *state)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    /* Open clock gate for sai instance */
    CLOCK_SYS_EnableSaiClock(instance);
    /*Check if the device is busy */
    if(sai_state_ids[instance][1] != NULL)
    {
        return kStatus_SAI_DeviceBusy;
    }
    sai_state_ids[instance][1] = state;
    SAI_HAL_RxInit(reg_base);
    /* Mclk source select */
    if (config->slave_master == kSaiMaster)
    {
        SAI_HAL_SetMclkSrc(reg_base, config->mclk_source);
        SAI_HAL_RxSetBclkSrc(reg_base, config->bclk_source);
    }
    SAI_HAL_RxSetSyncMode(reg_base, config->sync_mode);
    SAI_HAL_RxSetMasterSlave(reg_base, config->slave_master);
    SAI_HAL_RxSetProtocol(reg_base, config->protocol);
    SAI_HAL_RxSetDataChn(reg_base, config->channel);
    SAI_HAL_RxSetWatermark(reg_base, config->watermark);

    /* Fill the state strucutre */
    sai_state_ids[instance][1]->sync_mode = config->sync_mode;
    sai_state_ids[instance][1]->fifo_channel = config->channel;
    sai_state_ids[instance][1]->watermark = config->watermark;
    sai_state_ids[instance][1]->master_slave = config->slave_master;
    INT_SYS_EnableIRQ(g_saiRxIrqId[instance]);
 
    return kStatus_SAI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxGetDefaultSetting
 * Description   : Get default settings for sai tx module.
 *
 *END**************************************************************************/
void SAI_DRV_TxGetDefaultSetting(sai_user_config_t * config)
{
    config->bclk_source = kSaiBclkSourceMclkDiv;
    config->channel = 0;
    config->mclk_divide_enable = true;
    config->mclk_source = kSaiMclkSourceSysclk;
    config->protocol = kSaiBusI2SType;
    config->slave_master = kSaiMaster;
    config->watermark = 4;
    config->sync_mode = kSaiModeAsync;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxGetDefaultSetting
 * Description   : Get default settings for sai rx module.
 *
 *END**************************************************************************/
void SAI_DRV_RxGetDefaultSetting(sai_user_config_t * config)
{
    config->bclk_source = kSaiBclkSourceMclkDiv;
    config->channel = 0;
    config->mclk_divide_enable = true;
    config->mclk_source = kSaiMclkSourceSysclk;
    config->protocol = kSaiBusI2SType;
    config->slave_master = kSaiMaster;
    config->watermark = 4;
    config->sync_mode = kSaiModeSync;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxDeinit
 * Description   :Deinit the sai tx module, free the resources.
 * 
 *END**************************************************************************/
sai_status_t SAI_DRV_TxDeinit(uint32_t instance)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    SAI_DRV_TxSetDmaCmd(instance, false);
    SAI_DRV_TxSetIntCmd(instance, false);
    SAI_HAL_TxDisable(reg_base);
    SAI_HAL_TxSetReset(reg_base, kSaiResetTypeSoftware);
    SAI_HAL_TxClearStateFlag(reg_base, kSaiStateFlagSoftReset);

    sai_state_ids[instance][0] = NULL;
    /* Check if need to close the clock gate */
    if  ((sai_state_ids[instance][0] == NULL) && (sai_state_ids[instance][1] == NULL))
    {
        CLOCK_SYS_DisableSaiClock(instance);
    }
    return kStatus_SAI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxDeinit
 * Description   :Deinit the sai rx module, free the resources.
 * 
 *END**************************************************************************/
sai_status_t SAI_DRV_RxDeinit(uint32_t instance)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    SAI_DRV_RxSetDmaCmd(instance, false);
    SAI_DRV_RxSetIntCmd(instance, false);
    SAI_HAL_RxDisable(reg_base);
    SAI_HAL_RxSetReset(reg_base, kSaiResetTypeSoftware);
    SAI_HAL_RxClearStateFlag(reg_base, kSaiStateFlagSoftReset);

    sai_state_ids[instance][1] = NULL;
    /* Check if need to close the clock gate */
    if  ((sai_state_ids[instance][0] == NULL) && (sai_state_ids[instance][1] == NULL))
    {
        CLOCK_SYS_DisableSaiClock(instance);
    }
    return kStatus_SAI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxSetWatermark
 * Description   :Set the watermark value of sai tx.
 * 
 *END**************************************************************************/
void SAI_DRV_TxSetWatermark(uint32_t instance,uint32_t watermark)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    SAI_HAL_TxSetWatermark(reg_base,watermark);
    sai_state_ids[instance][0]->watermark = watermark;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxSetWatermark
 * Description   :Set the watermark value of sai rx.
 * 
 *END**************************************************************************/
void SAI_DRV_RxSetWatermark(uint32_t instance,uint32_t watermark)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    SAI_HAL_RxSetWatermark(reg_base,watermark);
    sai_state_ids[instance][1]->watermark = watermark;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxConfigDataFormat
 * Description   :Configure audio format information of tx.
 * The audio format information includes the sample rate, data length and so on.
 *END**************************************************************************/
sai_status_t SAI_DRV_TxConfigDataFormat(uint32_t instance, sai_data_format_t *format)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    memcpy(&sai_state_ids[instance][0]->format, format, sizeof(sai_data_format_t));
    if(sai_state_ids[instance][0]->master_slave == kSaiMaster)
    {
        uint32_t frequency = 0;
        uint32_t bclk = format->sample_rate * format->bits * 2;
        uint8_t divider;
        if(SAI_HAL_TxGetBclkSrc(reg_base) == 0)
        {
            divider = (CLOCK_SYS_GetSaiFreq(instance))/bclk;
        }
        else
        {
            divider = format->mclk/bclk;
        }
        /* Get the clock source frequency */
        CLOCK_SYS_GetFreq(kSystemClock,&frequency);
        /* Configure master clock */
        SAI_HAL_SetMclkDiv(reg_base, format->mclk, frequency);
        /* Master clock and bit clock setting */
        SAI_HAL_TxSetBclkDiv(reg_base, divider);
    }
    SAI_HAL_TxSetFrameSyncWidth(reg_base, format->bits);
    /* Frmae size and word size setting */
    SAI_HAL_TxSetFirstWordSize(reg_base, format->bits);
    SAI_HAL_TxSetWordSize(reg_base, format->bits);
    SAI_HAL_TxSetWordStartIndex(reg_base, 0);
    SAI_HAL_TxSetFirstBitShifted(reg_base, format->bits);
    /* The chennl number configuration */
    if (format->mono_streo == kSaiMono)
    {
        SAI_HAL_TxSetWordMask(reg_base, 2u);
    }
    else
    {
        SAI_HAL_TxSetWordMask(reg_base, 0u);
    }
  
    return kStatus_SAI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxConfigDataFormat
 * Description   :Configure audio format information of rx.
 * The audio format information includes the sample rate, data length and so on.
 *END**************************************************************************/
sai_status_t SAI_DRV_RxConfigDataFormat(uint32_t instance, sai_data_format_t *format)
{
    uint32_t reg_base = g_saiBaseAddr[instance];

    memcpy(&sai_state_ids[instance][1]->format, format, sizeof(sai_data_format_t));
    if(sai_state_ids[instance][1]->master_slave == kSaiMaster)
    {
        uint32_t frequency = 0;
        uint32_t bclk = format->sample_rate * format->bits * 2;
        uint8_t divider;
        if(SAI_HAL_RxGetBclkSrc(reg_base) == 0)
        {

            divider = (CLOCK_SYS_GetSaiFreq(instance))/bclk;
        }
        else
        {
            divider = format->mclk/bclk;
        }
        /* Get the clock source frequency */
        CLOCK_SYS_GetFreq(kSystemClock,&frequency);
        /* Configure master clock */
        SAI_HAL_SetMclkDiv(reg_base, format->mclk, frequency);
        /* Master clock and bit clock setting */
        SAI_HAL_RxSetBclkDiv(reg_base, divider);
    }
    SAI_HAL_RxSetFrameSyncWidth(reg_base, format->bits);
    /* Frmae size and word size setting */
    SAI_HAL_RxSetFirstWordSize(reg_base, format->bits);
    SAI_HAL_RxSetWordSize(reg_base, format->bits);
    SAI_HAL_RxSetWordStartIndex(reg_base, 0);
    SAI_HAL_RxSetFirstBitShifted(reg_base, format->bits);
    /* The chennl number configuration */
    if (format->mono_streo == kSaiMono)
    {
        SAI_HAL_RxSetWordMask(reg_base, 2u);
    }
    else
    {
        SAI_HAL_RxSetWordMask(reg_base, 0u);
    }
  
    return kStatus_SAI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxStartModule
 * Description   : Start the writing process.
 * 
 *END**************************************************************************/
void SAI_DRV_TxStartModule(uint32_t instance)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    /* If the sync mode is synchronous, it will need Rx enable bit clock */
    if(sai_state_ids[instance][0]->sync_mode == kSaiModeSync)
    {
        SAI_HAL_TxEnable(reg_base);
        SAI_HAL_RxEnable(reg_base);
    }
    else
    {
        SAI_HAL_TxEnable(reg_base);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxStartModule
 * Description   : Start the reading process.
 * 
 *END**************************************************************************/
void SAI_DRV_RxStartModule(uint32_t instance)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    /* If the sync mode is synchronous, it will need Tx enable bit clock */
    if(sai_state_ids[instance][1]->sync_mode == kSaiModeSync)
    {
        SAI_HAL_RxEnable(reg_base);
        SAI_HAL_TxEnable(reg_base);
    }
    else
    {
        SAI_HAL_RxEnable(reg_base);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxIRQHandler
 * Description   : The interrupt handle of tx FIFO request or FIFO warning.
 * The interrupt handle is used to transfer data from sai buffer to sai fifo. 
 *END**************************************************************************/
void SAI_DRV_TxIRQHandler(uint32_t instance)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    uint8_t data_size = 0;
    uint8_t i = 0, j = 0;
    sai_data_format_t format = sai_state_ids[instance][0]->format;
    uint32_t watermark = sai_state_ids[instance][0]->watermark;
    uint8_t space = FSL_FEATURE_SAI_FIFO_COUNT - watermark;
    uint32_t data = 0, temp = 0;
    uint32_t len = sai_state_ids[instance][0]->len;
    uint32_t count = sai_state_ids[instance][0]->count;

    /* Judge if FIFO error */
    if(SAI_HAL_TxGetStateFlag(reg_base, kSaiStateFlagFIFOError))
    {
        SAI_HAL_TxClearStateFlag(reg_base, kSaiStateFlagFIFOError);
    }
    /* Interrupt used to transfer data. */
    if((SAI_HAL_TxGetStateFlag(reg_base, kSaiStateFlagFIFORequest))
        && (SAI_HAL_TxGetIntCmd(reg_base, kSaiIntrequestFIFORequest)))
    {
        data_size = format.bits/8;
        if((data_size == 3) || (format.bits & 0x7))
        {
            data_size = 4;
        }
        /*Judge if the data need to transmit is less than space */
        if(space > (len -count)/data_size)
        {
            space = (len -count)/data_size;
        }
        /* If normal, copy the data from sai buffer to FIFO */
        for(i = 0; i < space; i++)
        {
            for(j = 0; j < data_size; j ++)
            {
                temp = (uint32_t)(*sai_state_ids[instance][0]->address);
                data |= (temp << (8U * j));
                sai_state_ids[instance][0]->address ++;
            }
            SAI_HAL_SendData(reg_base, sai_state_ids[instance][0]->fifo_channel, (uint32_t )data);
            sai_state_ids[instance][0]->count += data_size;
            data = 0;
        }
        /* If a block is finished, just callback */
        count = sai_state_ids[instance][0]->count;
        if(count == len)
        {
            void * callback_param = sai_state_ids[instance][0]->callback_param;
            sai_state_ids[instance][0]->count = 0;
            (sai_state_ids[instance][0]->callback)(callback_param);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxIRQHandler
 * Description   : The interrupt handle of rx FIFO request or FIFO warning.
 * The interrupt handle is used to transfer data from sai fifo to sai buffer. 
 *END**************************************************************************/
void SAI_DRV_RxIRQHandler(uint32_t instance)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    uint8_t i = 0, j = 0;
    uint8_t data_size = 0;
    uint32_t data = 0;
    sai_data_format_t format = sai_state_ids[instance][1]->format;
    uint8_t space = sai_state_ids[instance][1]->watermark;
    uint32_t len = sai_state_ids[instance][1]->len;
    uint32_t count = sai_state_ids[instance][1]->count;

    /* Judge if FIFO error */
    if(SAI_HAL_RxGetStateFlag(reg_base, kSaiStateFlagFIFOError))
    {
        SAI_HAL_RxClearStateFlag(reg_base, kSaiStateFlagFIFOError);    
    }
    /* Interrupt used to transfer data. */
    if((SAI_HAL_RxGetStateFlag(reg_base, kSaiStateFlagFIFORequest))
        && (SAI_HAL_RxGetIntCmd(reg_base, kSaiIntrequestFIFORequest)))
    {
        data_size = format.bits/8;
        if((data_size == 3) || (format.bits & 0x7))
        {
            data_size = 4;
        }

        /*Judge if the data need to transmit is less than space */
        if(space > (len - count)/data_size)
        {
            space = (len -count)/data_size;
        }
        /* Read data from FIFO to the buffer */
        for (i = 0; i < space; i ++)
        {
            data = SAI_HAL_ReceiveData(reg_base, sai_state_ids[instance][1]->fifo_channel);
            for(j = 0; j < data_size; j ++)
            {
                *sai_state_ids[instance][1]->address = (data >> (8U * j)) & 0xFF;
                sai_state_ids[instance][1]->address ++;
            }
            sai_state_ids[instance][1]->count += data_size;
        }
        /* If need to callback the function */
        count = sai_state_ids[instance][1]->count;
        if (count == len)
        {
            void *callback_param = sai_state_ids[instance][1]->callback_param;
            sai_state_ids[instance][1]->count = 0;
            (sai_state_ids[instance][1]->callback)(callback_param);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxRegisterCallback
 * Description   : The function would register the callback function to tell sai
 * driver what to do after the transfer. 
 *END**************************************************************************/
void SAI_DRV_TxRegisterCallback
(
uint32_t instance, 
sai_callback_t callback, 
void *callback_param
)
{
    sai_state_ids[instance][0]->callback = callback;
    sai_state_ids[instance][0]->callback_param = callback_param;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxRegisterCallback
 * Description   : The function would register the callback function to tell sai
 * driver what to do after the receive. 
 *END**************************************************************************/
void SAI_DRV_RxRegisterCallback
(
uint32_t instance, 
sai_callback_t callback, 
void *callback_param
)
{
    sai_state_ids[instance][1]->callback = callback;
    sai_state_ids[instance][1]->callback_param = callback_param;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_SendData
 * Description   : The function would tell sai driver to start send a period of
 * data to sai tx fifo.
 *END**************************************************************************/
uint32_t SAI_DRV_SendData(uint32_t instance, uint8_t *addr, uint32_t len)
{
    sai_state_ids[instance][0]->len = len;
    sai_state_ids[instance][0]->address= addr;
    SAI_DRV_TxStartModule(instance);
    return len;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_ReceiveData
 * Description   : The function would tell sai driver to start receive a period of
 * data from sai rx fifo.
 *END**************************************************************************/
uint32_t SAI_DRV_ReceiveData(uint32_t instance, uint8_t *addr, uint32_t len)
{
    sai_state_ids[instance][1]->len = len;
    sai_state_ids[instance][1]->address= addr;
    SAI_DRV_RxStartModule(instance);
    return len;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_SendDataBlocking.
 * Description   : The function would tell sai driver to start send a period of
 * data to sai tx fifo i n polling way.
 *END**************************************************************************/
uint32_t SAI_DRV_SendDataBlocking(uint32_t instance, uint8_t * addr,uint32_t len)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    uint32_t i, j = 0;
    uint32_t data = 0;
    sai_data_format_t *format = &sai_state_ids[instance][0]->format;
    uint32_t channel = sai_state_ids[instance][0]->fifo_channel;
    for(i = 0; i < len/(format->bits/8); i++)
    {
        for(j = 0; j < format->bits/8; j ++)
        {
            data |= ((uint32_t)(*addr) << (j * 8u));
            addr ++;
        }
        SAI_HAL_SendDataBlocking(reg_base,channel,data);
        data = 0;
    }
    return len;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_ReceiveDataBlocking
 * Description   : The function would tell sai driver to start receive a period of
 * data from sai rx fifo in polling way.
 *END**************************************************************************/
uint32_t SAI_DRV_ReceiveDataBlocking(uint32_t instance,uint8_t * addr,uint32_t len)
{
    uint32_t reg_base = g_saiBaseAddr[instance];
    uint32_t i = 0, j = 0;
    uint32_t data = 0;
    sai_data_format_t *format = &sai_state_ids[instance][1]->format;
    uint32_t channel = sai_state_ids[instance][1]->fifo_channel;
    for(i = 0; i < len/(format->bits/8); i ++)
    {
        data = SAI_HAL_ReceiveDataBlocking(reg_base,channel);
        for(j = 0; j < format->bits/8; j++)
        {
            *addr = (data >> (j * 8)) & 0xFF;
            addr ++;
        }
    }
    return len;
}

/*******************************************************************************
 *EOF
 ******************************************************************************/
