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
#include "fsl_dac_driver.h"
#include "fsl_dac_common.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"

/*! @brief Table of pointers to internal state structure for DAC instances. */
static dac_state_t *volatile g_dacStatePtr[HW_DAC_INSTANCE_COUNT];

/*FUNCTION*********************************************************************
 *
 * Function Name : DAC_DRV_StructInitUserConfigNormal
 * Description   : Fill the initial user configuration for DAC module 
 * without the feature of interrupt and buffer. Then call initialization
 * function with the filled parameter would configure
 * the DAC module work as a common and simple converter.
 *
 *END*************************************************************************/
dac_status_t DAC_DRV_StructInitUserConfigNormal(dac_user_config_t *userConfigPtr)
{
    if (!userConfigPtr)
    {
        return kStatus_DAC_InvalidArgument;
    }
    userConfigPtr->refVoltSrcMode = kDacRefVoltSrcOfVref2; /* Vdda */
    userConfigPtr->triggerMode = kDacTriggerBySoftware;
    userConfigPtr->lowPowerEnable = false;
    return kStatus_DAC_Success;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : DAC_DRV_Init
 * Description   : Initialize the converter in DAC module. It will just
 * configure the DAC converter itself but not including advanced features like
 * interrupt and internal buffer. This API should be called before any
 * operation to DAC module. After initialized, the DAC module can work at 
 * least as a common simple DAC converter.
 *
 *END*************************************************************************/
dac_status_t DAC_DRV_Init(uint32_t instance, dac_user_config_t *userConfigPtr)
{
    assert(instance < HW_DAC_INSTANCE_COUNT);
    uint32_t baseAddr = g_dacBaseAddr[instance];
    bool mEnable;

    if (!userConfigPtr)
    {
        return kStatus_DAC_InvalidArgument;
    }

    /* Enable the clock gate from clock manager. */
    mEnable = CLOCK_SYS_GetDacGateCmd(instance);
    if (!mEnable)
    {
        CLOCK_SYS_EnableDacClock(instance);
    }

    /* Reset the registers for DAC module to reset state. */
    DAC_HAL_Init(baseAddr);
    DAC_HAL_Enable(baseAddr);
    DAC_HAL_SetRefVoltSrcMode(baseAddr, userConfigPtr->refVoltSrcMode);
    DAC_HAL_SetTriggerMode(baseAddr, userConfigPtr->triggerMode);
    DAC_HAL_SetLowPowerCmd(baseAddr, userConfigPtr->lowPowerEnable);

    return kStatus_DAC_Success;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : DAC_DRV_Deinit
 * Description   : De-initialize the converter in DAC module. It will disable
 * DAC module and shut down its clock to reduce the power consumption.
 *
 *END*************************************************************************/
void DAC_DRV_Deinit(uint32_t instance)
{
    assert(instance < HW_DAC_INSTANCE_COUNT);
    uint32_t baseAddr = g_dacBaseAddr[instance];

    DAC_HAL_Disable(baseAddr);
    CLOCK_SYS_DisableDacClock(instance);
    g_dacStatePtr[instance] = NULL;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : DAC_DRV_Output
 * Description   : Drive the converter to output DAC value. It will force
 * the buffer index to be the first one, load the setting value to this item. 
 * Then the converter will output the voltage indicated by the indicated value
 * immediately. 
 *
 *END*************************************************************************/
void DAC_DRV_Output(uint32_t instance, uint16_t value)
{
    assert(instance < HW_DAC_INSTANCE_COUNT);
    uint32_t baseAddr = g_dacBaseAddr[instance];

    DAC_HAL_SetBuffValue(baseAddr, 0U, value);
    DAC_HAL_SetBuffCurrentIndex(baseAddr, 0U);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : DAC_DRV_EnableBuff
 * Description   : Configure the feature of internal buffer for DAC module.
 * By default, the feature of buffer is disabled. Calling this API will enable
 * the buffer and configure it.
 *
 *END*************************************************************************/
dac_status_t DAC_DRV_EnableBuff(uint32_t instance, dac_buff_config_t *buffConfigPtr, dac_state_t *userStatePtr)
{
    assert(instance < HW_DAC_INSTANCE_COUNT);
    uint32_t baseAddr = g_dacBaseAddr[instance];

    if ( (!buffConfigPtr) || (!userStatePtr) )
    {
        return kStatus_DAC_InvalidArgument;
    }

    /* Configure the buffer for DAC. */
    DAC_HAL_SetBuffCmd(baseAddr, true);
    DAC_HAL_SetBuffUpperIndex(baseAddr, buffConfigPtr->buffUpperIndex);
#if FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
    DAC_HAL_SetBuffWatermarkMode(baseAddr, buffConfigPtr->watermarkMode);
    DAC_HAL_SetBuffWorkMode(baseAddr, buffConfigPtr->buffWorkMode);
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */    
    /* Configure the interrupt for DAC. */    
    DAC_HAL_SetBuffIndexStartIntCmd(baseAddr, buffConfigPtr->buffIndexStartIntEnable);
#if FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
    DAC_HAL_SetBuffIndexWatermarkIntCmd(baseAddr, buffConfigPtr->buffIndexWatermarkIntEnable);
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */
    DAC_HAL_SetBuffIndexUpperIntCmd(baseAddr, buffConfigPtr->buffIndexUpperIntEnable);

    DAC_HAL_SetDmaCmd(baseAddr, buffConfigPtr->dmaEnable);

    /* Configure the DAC IRQ in NVIC. */
    if (    (buffConfigPtr->buffIndexStartIntEnable)
#if FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
         || (buffConfigPtr->buffIndexWatermarkIntEnable)
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */
         || (buffConfigPtr->buffIndexUpperIntEnable)  )
    {
        /* Enable ADC interrupt in NVIC level.*/
        INT_SYS_EnableIRQ(g_dacIrqId[instance] );
    }
    else
    {
        /* Disable ADC interrupt in NVIC level.*/
        INT_SYS_DisableIRQ(g_dacIrqId[instance] );
    }

    g_dacStatePtr[instance] = userStatePtr;

    return kStatus_DAC_Success;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : DAC_DRV_DisableBuff
 * Description   : Disable the feature of internal buffer for DAC module.
 * Calling this API will disable the feature of internal buffer and reset the
 * DAC module as a common and simple DAC converter.
 *
 *END*************************************************************************/
void DAC_DRV_DisableBuff(uint32_t instance)
{
    assert(instance < HW_DAC_INSTANCE_COUNT);
    uint32_t baseAddr = g_dacBaseAddr[instance];

    /* Disable ADC interrupt in NVIC level.*/
    INT_SYS_DisableIRQ(g_dacIrqId[instance] );

    DAC_HAL_SetDmaCmd(baseAddr, false);
    DAC_HAL_SetBuffIndexStartIntCmd(baseAddr, false);
#if FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
    DAC_HAL_SetBuffIndexWatermarkIntCmd(baseAddr, false);
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */
    DAC_HAL_SetBuffIndexUpperIntCmd(baseAddr, false);
    DAC_HAL_SetBuffCmd(baseAddr, false);
    g_dacStatePtr[instance] = 0U;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : DAC_DRV_SetBuffValue
 * Description   : Set values into DAC's internal buffer. Note the buffer
 * size is defined by MACRO "FSL_FEATURE_DAC_BUFFER_SIZE" and the available
 * value is 12-bit.
 *
 *END*************************************************************************/
dac_status_t DAC_DRV_SetBuffValue(uint32_t instance, uint8_t start, uint8_t offset, uint16_t arr[])
{
    assert(instance < HW_DAC_INSTANCE_COUNT);
    uint32_t baseAddr = g_dacBaseAddr[instance];

    uint8_t i;

    if (  (!arr) || (start + offset > HW_DAC_DATnL_COUNT) )
    {
        return kStatus_DAC_InvalidArgument;
    }

    for (i = 0; i < offset; i++)
    {
        DAC_HAL_SetBuffValue(baseAddr, start+i, arr[i]);
    }

    return kStatus_DAC_Success;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : DAC_DRV_SoftTriggerBuff
 * Description   : Trigger the buffer by software and return the current
 * value. After triggered, the buffer index will update according to work mode.
 * Then the value kept inside the pointed item will be output immediately.
 *
 *END*************************************************************************/
uint16_t DAC_DRV_SoftTriggerBuff(uint32_t instance)
{
    assert(instance < HW_DAC_INSTANCE_COUNT);
    uint32_t baseAddr = g_dacBaseAddr[instance];

    uint8_t i;
    DAC_HAL_SetSoftTriggerCmd(baseAddr);
    i = DAC_HAL_GetBuffCurrentIndex(baseAddr);
    return DAC_HAL_GetBuffValue(baseAddr, i);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : DAC_DRV_GetBufferIndex
 * Description   : Get the current index of DAC's buffer.
 *
 *END*************************************************************************/
uint8_t DAC_DRV_GetBufferIndex(uint32_t instance)
{
    assert(instance < HW_DAC_INSTANCE_COUNT);
    uint32_t baseAddr = g_dacBaseAddr[instance];

    return DAC_HAL_GetBuffCurrentIndex(baseAddr);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : DAC_DRV_ClearFlag
 * Description   : Clear the flag for indicated event causing interrupt.
 *
 *END*************************************************************************/
void DAC_DRV_ClearFlag(uint32_t instance, dac_flag_t flag)
{
    assert(instance < HW_DAC_INSTANCE_COUNT);
    uint32_t baseAddr = g_dacBaseAddr[instance];

    switch (flag)
    {
    case kDacBuffIndexStartFlag:
        DAC_HAL_ClearBuffIndexStartFlag(baseAddr);
        break;
#if FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
    case kDacBuffIndexWatermarkFlag:
        DAC_HAL_ClearBuffIndexWatermarkFlag(baseAddr);
        break;
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */
    case kDacBuffIndexUpperFlag:
        DAC_HAL_ClearBuffIndexUpperFlag(baseAddr);
        break;
    default:
        DAC_HAL_ClearBuffIndexStartFlag(baseAddr);
#if FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
        DAC_HAL_ClearBuffIndexWatermarkFlag(baseAddr);
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */
        DAC_HAL_ClearBuffIndexUpperFlag(baseAddr);
        break;
    }
}

/*FUNCTION*********************************************************************
 *
 * Function Name : DAC_DRV_GetFlag
 * Description   : Get the flag for indicated event causing interrupt.
 * If the event occurs, the return value will be asserted.
 *
 *END*************************************************************************/
bool DAC_DRV_GetFlag(uint32_t instance, dac_flag_t flag)
{
    assert(instance < HW_DAC_INSTANCE_COUNT);
    uint32_t baseAddr = g_dacBaseAddr[instance];
    bool bRet;

    switch (flag)
    {
    case kDacBuffIndexStartFlag:
        bRet = DAC_HAL_GetBuffIndexStartFlag(baseAddr);
        break;
#if FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
    case kDacBuffIndexWatermarkFlag:
        bRet = DAC_HAL_GetBuffIndexWatermarkFlag(baseAddr);
        break;
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */
    case kDacBuffIndexUpperFlag:
        bRet = DAC_HAL_GetBuffIndexUpperFlag(baseAddr);
        break;
    default:
        bRet = false;
        break;
    }
    return bRet;
}

/*!
 * @brief Install the user-defined callback for DAC module.
 *
 * This function is to install the user-defined callback for DAC module.
 * When an DAC interrupt request is served, the callback will be executed 
 * inside the ISR.
 *
 * @param instance DAC instance ID.
 * @param userCallback User-defined callback function.
 * @return Execution status.
 */
dac_status_t DAC_DRV_InstallCallback(uint32_t instance, dac_callback_t userCallback)
{
    assert(instance < HW_DAC_INSTANCE_COUNT);
    if ( !g_dacStatePtr[instance] )
    {
        return kStatus_DAC_Failed;
    }
    g_dacStatePtr[instance]->userCallbackFunc = userCallback;

    return kStatus_DAC_Success;
}

/*!
 * @brief Driver-defined ISR in DAC module.
 *
 * This function is the driver-defined ISR in DAC module.
 * It includes the process for interrupt mode defined by driver. Currently, it
 * will be called inside the system-defined ISR.
 *
 * @param instance DAC instance ID.
 */
void DAC_DRV_IRQHandler(uint32_t instance)
{
    assert(instance < HW_DAC_INSTANCE_COUNT);

    if (g_dacStatePtr[instance])
    {
        /* Call the user-defined callback. */
        if (g_dacStatePtr[instance]->userCallbackFunc)
        {
            (*(g_dacStatePtr[instance]->userCallbackFunc))();
        }
    }
    /* Clear all the flags if the operations are missing in user-defined
         * callback function. */
    if ( DAC_DRV_GetFlag(instance, kDacBuffIndexStartFlag) )
    {
        DAC_DRV_ClearFlag(instance, kDacBuffIndexStartFlag);
    }
#if FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
    if ( DAC_DRV_GetFlag(instance, kDacBuffIndexWatermarkFlag) )
    {
        DAC_DRV_ClearFlag(instance, kDacBuffIndexWatermarkFlag);
    }
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */
    if ( DAC_DRV_GetFlag(instance, kDacBuffIndexUpperFlag) )
    {
        DAC_DRV_ClearFlag(instance, kDacBuffIndexUpperFlag);
    }
}

/******************************************************************************
 * EOF
 *****************************************************************************/
