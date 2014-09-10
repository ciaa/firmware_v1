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

#ifndef __FSL_DAC_DRIVER_H__
#define __FSL_DAC_DRIVER_H__

#include <stdint.h>
#include <stdbool.h>
#include "fsl_dac_hal.h"

/*!
 * @addtogroup dac_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Defines the configuration buffer structure inside the DAC module.
 *
 * This structure  keeps the configuration for the internal buffer
 * inside the DAC module. The DAC buffer is an advanced feature, which helps
 * improve the performance of an application.
 */
typedef struct DacBuffConfig
{
#if FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
    bool buffIndexWatermarkIntEnable; 
        /*!< Switcher to enable interrupt when buffer index hits the watermark. */
    dac_buff_watermark_mode_t watermarkMode;
        /*!< Selection of watermark setting, see to "dac_buff_watermark_mode_t". */
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */
    bool buffIndexStartIntEnable;
        /*!< Switcher to enable interrupt when buffer index hits the start (0). */
    bool buffIndexUpperIntEnable;
        /*!< Switcher to enable interrupt when buffer index hits the upper. */
    bool dmaEnable; /*!< Switcher to enable DMA request by original interrupts. */
    dac_buff_work_mode_t buffWorkMode;
        /*!< Selection of buffer's work mode, see to "dac_buff_work_mode_t". */
    uint8_t buffUpperIndex; /* Setting of the buffer's upper limit, 0-15. */
} dac_buff_config_t;

/*!
 * @brief Defines the converted configuration structure.
 *
 * This structure  keeps the configuration for DAC module basic converter.
 * The DAC converter is the core part of the DAC module. When
 * initialized, the DAC module can act as a simple DAC converter. 
 */
typedef struct DacUserConfig
{
    dac_ref_volt_src_mode_t refVoltSrcMode;
        /*!< Selection of reference voltage source for DAC module. */
    dac_trigger_mode_t triggerMode;
        /*!< Selection of hardware mode or software mode. */
    bool lowPowerEnable; /*!< Switcher to enable working in low power mode. */
} dac_user_config_t;

/*!
 * @brief Defines the type of event flags.
 */
typedef enum _dac_flag_t
{
#if FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
    kDacBuffIndexWatermarkFlag = 0U, /*!< Event for the buffer index hit the watermark. */
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */
    kDacBuffIndexStartFlag = 1U, /*!< Event for the buffer index hit the start (0). */
    kDacBuffIndexUpperFlag = 2U /*!< Event for the buffer index hit the upper. */
} dac_flag_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Defines the type of the user-defined callback function.
 */
typedef void (*dac_callback_t)(void);

/*!
 * @brief Internal driver state information.
 *
 * The contents of this structure are internal to the driver and should not be
 *  modified by users.
 */
typedef struct DacState
{
    dac_callback_t userCallbackFunc; /*!< Keep the user-defined callback function. */
} dac_state_t;

/*!
 * @brief Fills the initial user configuration for the DAC module without the interrupt and the buffer. 
 *
 * This function  fills the initial user configuration 
 * without the interrupt and buffer features. Calling the initialization
 * function with the filled parameter configures the DAC module to function as
 * a simple converter. The settings are:
 *
 * .refVoltSrcMode = kDacRefVoltSrcOfVref2; // Vdda
 * .triggerMode = kDacTriggerBySoftware;
 * .lowPowerEnable = false;
 *
 * @param userConfigPtr Pointer to the user configuration structure. See the "dac_user_config_t".
 * @return Execution status.
 */
dac_status_t DAC_DRV_StructInitUserConfigNormal(dac_user_config_t *userConfigPtr);

/*!
 * @brief Initializes the converter. 
 *
 * This function initializes the converter. It 
 * configures the DAC converter itself but does not include the advanced features, such as 
 * interrupt and internal buffer. This API should be called before any
 * operations in the DAC module. After it is initialized, the DAC module can function  
 * as a simple DAC converter.
 *
 * @param instance DAC instance ID.
 * @param userConfigPtr Pointer to the initialization structure. See the "dac_user_config_t".
 * @return Execution status.
 */
dac_status_t DAC_DRV_Init(uint32_t instance, dac_user_config_t *userConfigPtr);

/*!
 * @brief De-initializes the DAC module converter.
 *
 * This function de-initializes the converter. It  disables the
 * DAC module and shuts down the clock to reduce the power consumption. 
 *
 * @param instance DAC instance ID.
 */
void DAC_DRV_Deinit(uint32_t instance);

/*!
 * @brief Drives the converter to output the DAC value.
 *
 * This function drives the converter to output the DAC value. It forces
 * the buffer index to be the first one and load the setting value to this item. 
 * Then, the converter outputs the voltage indicated by the indicated value
 * immediately. 
 *
 * @param instance DAC instance ID.
 * @param value Setting value for DAC.
 */
void DAC_DRV_Output(uint32_t instance, uint16_t value);

/*!
 * @brief Configures the internal buffer.
 *
 * This function configures the feature of the internal buffer for the DAC module.
 * By default, the buffer feature  is disabled. Calling this API enables
 * the buffer and configures it.
 *
 * @param instance DAC instance ID.
 * @param buffConfigPtr Pointer to the configuration structure. See the "dac_buff_config_t".
 * @param userStatePtr Pointer to the structure for keeping internal state. See the "dac_state_t".
 * @return Execution status.
 */
dac_status_t DAC_DRV_EnableBuff(uint32_t instance, dac_buff_config_t *buffConfigPtr, dac_state_t *userStatePtr);

/*!
 * @brief Disables the internal buffer.
 *
 * This function  disables the internal buffer feature.
 * Calling this API  disables the internal buffer feature and resets the
 * DAC module as a simple DAC converter.
 *
 * @param instance DAC instance ID.
 */
void DAC_DRV_DisableBuff(uint32_t instance);

/*!
 * @brief Sets values into the DAC internal buffer.
 *
 * This function  sets values into the DAC internal buffer. Note that the buffer
 * size is defined by the  "FSL_FEATURE_DAC_BUFFER_SIZE" macro and the available
 * value is 12 bit.
 *
 * @param instance DAC instance ID.
 * @param start Start index of setting values.
 * @param offset Length of setting values' array.
 * @param arr Setting values' array.
 * @return Execution status.
 */
dac_status_t DAC_DRV_SetBuffValue(uint32_t instance, uint8_t start, uint8_t offset, uint16_t arr[]);

/*!
 * @brief Triggers the buffer by software and returns the current value.
 *
 * This function triggers the buffer by software and returns the current
 * value. After it is triggered, the buffer index  updates according to work mode.
 * Then, the value kept inside the pointed item  is immediately output.
 *
 * @param instance DAC instance ID.
 * @return Current output value.
 */
uint16_t DAC_DRV_SoftTriggerBuff(uint32_t instance);

/*!
 * @brief Gets the DAC buffer current index.
 *
 * This function gets the DAC buffer current index.
 *
 * @param instance DAC instance ID.
 * @return Current index of DAC buffer.
 */
uint8_t DAC_DRV_GetBufferIndex(uint32_t instance);

/*!
 * @brief Clears the flag for an indicated event causing an interrupt.
 *
 * This function clears the flag for an indicated event causing an interrupt.
 *
 * @param instance DAC instance ID.
 * @param flag Indicated flag, see to "dac_flag_t".
 */
void DAC_DRV_ClearFlag(uint32_t instance, dac_flag_t flag);

/*!
 * @brief Gets the flag for an indicated event causing an interrupt.
 *
 * This function gets the flag for an indicated event causing an interrupt.
 * If the event occurs, the return value is asserted.
 *
 * @param instance DAC instance ID.
 * @param flag Indicated flag, see to "dac_flag_t".
 * @return Assertion of indicated event.
 */
bool DAC_DRV_GetFlag(uint32_t instance, dac_flag_t flag);

/*!
 * @brief Install the user-defined callback.
 *
 * This function installs the user-defined callback.
 * When the DAC interrupt request is served, the callback is executed
 * inside the ISR. 
 *
 * @param instance DAC instance ID.
 * @param userCallback User-defined callback function.
 * @return Execution status.
 */
dac_status_t DAC_DRV_InstallCallback(uint32_t instance, dac_callback_t userCallback);

/*!
 * @brief Driver-defined ISR in DAC module.
 *
 * This function is the driver-defined ISR in DAC module.
 * It includes the process for interrupt mode defined by the driver. Currently, it
 * is called inside the system-defined ISR.
 *
 * @param instance DAC instance ID.
 */
void DAC_DRV_IRQHandler(uint32_t instance);

#if defined(__cplusplus)
extern }
#endif

/*!
 *@}
 */

#endif /* __FSL_DAC_DRIVER_H__ */

/******************************************************************************
 * EOF
 *****************************************************************************/
