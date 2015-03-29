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

#ifndef __FSL_ADC_DRIVER_H__
#define __FSL_ADC_DRIVER_H__

#include <stdint.h>
#include <stdbool.h>
#include "fsl_adc_hal.h"

/*!
 * @addtogroup adc_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if FSL_FEATURE_ADC_HAS_CALIBRATION

/*!
 * @brief Defines the structure to configure the ADC module calibration.
 *
 * This structure keeps the configuration for the ADC module internal calibration.
 */
typedef struct AdcCalibrationParam
{
#if FSL_FEATURE_ADC_HAS_OFFSET_CORRECTION
    uint16_t offsetValue; /*!< Offset error from correction value. */
#endif /* FSL_FEATURE_ADC_HAS_OFFSET_CORRECTION */
    uint16_t plusSideGainValue; /*!< Plus-side gain value. */
#if FSL_FEATURE_ADC_HAS_DIFF_MODE
    uint16_t minusSideGainValue; /*!< Minus-side gain value. */
#endif /* FSL_FEATURE_ADC_HAS_DIFF_MODE */
} adc_calibration_param_t;

#endif /* FSL_FEATURE_ADC_HAS_CALIBRATION */

/*!
 * @brief Defines the structure to configure the ADC module hardware compare.
 *
 * This structure keeps the configuration for the ADC internal comparator.
 */
typedef struct AdcHwCmpConfig
{
    uint16_t cmpValue1; /*!< Value for CMP value 1. */
    uint16_t cmpValue2; /*!< Value for CMP value 2. */
        /*!< Select the available range to pass the comparator. */
    adc_hw_cmp_range_mode_t cmpRangeMode;
} adc_hw_cmp_config_t;

/*!
 * @brief Defines the structure to configure the ADC channel.
 *
 * This structure keeps the ADC channel configuration.
 */
typedef struct AdcChnConfig
{
#if FSL_FEATURE_ADC_HAS_MUX_SELECT
    adc_chn_mux_mode_t chnMux; /*!< Selection of channel multiplexer for a/b channel.*/
#endif /* FSL_FEATURE_ADC_HAS_MUX_SELECT */
    uint32_t chnNum; /*!< Selection of input channel number. */
    bool intEnable; /*!< Enable trigger interrupt when the conversion is complete. */
    bool diffEnable; /*!< Enable  setting the differential mode for conversion.  */
} adc_chn_config_t;

#if FSL_FEATURE_ADC_HAS_PGA

/*!
 * @brief Defines the structure to configure the ADC module PGA.
 *
 * This structure keeps the programmable gain amplifier configuration
 * if the ADC module contains this component. 
 */
typedef struct AdcPgaConfig
{
    bool choppingEnable; /*!<  Enable chopping control for PGA. */
    bool runInLowPowerModeEnable; /*!< Enable use PGA in low power mode.  */
    bool offsetMeasurementEnable; /*!< PGA runs in offset measurement mode. */
    adc_pga_gain_mode_t gainValue; /*!< Select gain value for PGA. */
} adc_pga_config_t;

#endif /* FSL_FEATURE_ADC_HAS_PGA */

/*!
 * @brief Defines the structure to initialize the ADC module converter.
 *
 * This structure keeps the configuration for the ADC module converter.
 */
typedef struct AdcUserConfig
{
    bool intEnable; /*!< Enable internal ISR. */
    bool lowPowerEnable; /*!< Enable low power mode for converter. */
    adc_clk_divider_mode_t clkDividerMode; /*!< Select divider of input clock for converter. */
    adc_resolution_mode_t resolutionMode; /*!< Select conversion resolution for converter.  */
    adc_clk_src_mode_t clkSrcMode; /*!< Select source of input clock for converter. */
    bool asyncClkEnable; /*!< Enable asynchronous clock output at when initializing converter.  */
    bool highSpeedEnable; /*!< Enable the high speed mode for converter. */
    bool hwTriggerEnable; /*!< Enable triggering by hardware. */
#if FSL_FEATURE_ADC_HAS_DMA
    bool dmaEnable; /*! Enable generating the DMA request when conversion is complete. */
#endif /* FSL_FEATURE_ADC_HAS_DMA */
    adc_ref_volt_src_mode_t refVoltSrcMode;
        /*!< Select the reference voltage source for converter. */
    bool continuousConvEnable; /*!< Enable  launching the continuous conversion mode. */
} adc_user_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Defines the type of a user-defined callback function.
*/
typedef void (*adc_callback_t)(void);

/*!
 * @brief Internal driver state information.
 *
 * The contents of this structure are internal to the driver and should not be
 *  modified by users. Also, contents of the structure are subject to change in
 *  future releases.
 */
typedef struct AdcState
{
    struct
    {
        adc_callback_t userCallbackFunc; 
            /*!< Callback function that would be executed in ISR. */
        uint16_t convBuff;
            /*!< Data buffer when interrupt mode is used. */
    } configGroup[FSL_FEATURE_ADC_CONVERSION_CONTROL_COUNT];
} adc_state_t;

/******************************************************************************
 * API
 *****************************************************************************/

#if FSL_FEATURE_ADC_HAS_CALIBRATION

/*!
 * @brief Gets the calibration parameters by auto calibration.
 *
 * This function executes  auto calibration and fetches
 * the calibration parameters that are kept in the "adc_calibration_param_t"
 * type variable. When executing auto calibration, the ADC
 * is configured internally to function with highest
 * precision. Because this API function may be called before the initialization, it enables
 * the ADC clock internally.
 *
 * @param instance ADC instance id.
 * @param paramPtr Pointer to the parameter structure. See the "adc_calibration_param_t".
 * @return Execution status.
 */
adc_status_t ADC_DRV_GetAutoCalibrationParam(uint32_t instance, adc_calibration_param_t *paramPtr);

/*!
 * @brief Sets the calibration parameters.
 *
 * This function sets the ADC module calibration parameters. These
 * parameters can be fetched either by launching the auto calibration
 * or using predefined values in the   
 * "adc_calibration_param_t" structure. For higher precision,  it is recommended to
 * execute the calibration before initializing the ADC module.
 * Because this API function can be called before the initialization, it enables the ADC
 * clock internally.
 *
 * @param instance ADC instance id.
 * @param paramPtr Pointer to parameter structure. See the "adc_calibration_param_t".
 * @return Execution status.
 */
adc_status_t ADC_DRV_SetCalibrationParam(uint32_t instance, adc_calibration_param_t *paramPtr);

#endif /* FSL_FEATURE_ADC_HAS_CALIBRATION */

/*!
 * @brief Fills the initial user configuration for the interrupt mode.
 *
 * This function fills the initial user configuration for interrupt mode.
 * Calling the initialization function with the filled parameter configures
 * the ADC module to function in the interrupt mode. The settings are:
 *
 * .intEnable = true;
 * .lowPowerEnable = true;
 * .clkDividerMode = kAdcClkDividerInputOf8;
 * .resolutionMode = kAdcResolutionBitOf12or13;
 * .clkSrcMode = kAdcClkSrcOfAsynClk;
 * .asyncClkEnable = true;
 * .highSpeedEnable = false;
 * .hwTriggerEnable = false;
 * .dmaEnable = false;
 * .refVoltSrcMode = kAdcRefVoltSrcOfVref;
 * .continuousConvEnable = true;
 *
 * @param userConfigPtr Pointer to the user configuration structure. See the "adc_user_config_t".
 * @return Execution status.
 */
adc_status_t ADC_DRV_StructInitUserConfigForIntMode(adc_user_config_t *userConfigPtr);

/*!
 * @brief Fills the initial user configuration for blocking mode.
 *
 * This function fills the initial user configuration for blocking mode.
 * Calling the initialization function with the filled parameter  configures
 * the ADC module to function in the blocking mode. The settings are:
 *
 * .intEnable = false;
 * .lowPowerEnable = false;
 * .clkDividerMode = kAdcClkDividerInputOf8;
 * .resolutionMode = kAdcResolutionBitOf12or13;
 * .clkSrcMode = kAdcClkSrcOfBusClk;
 * .asyncClkEnable = false;
 * .highSpeedEnable = false;
 * .hwTriggerEnable = false;
 * .dmaEnable = false;
 * .refVoltSrcMode = kAdcRefVoltSrcOfVref;
 * .continuousConvEnable = true;
 *
 * @param userConfigPtr Pointer to the user configuration structure. See the "adc_user_config_t".
 * @return Execution status.
 */
adc_status_t ADC_DRV_StructInitUserConfigForBlockingMode(adc_user_config_t *userConfigPtr);

/*!
 * @brief Fills the initial user configuration for a one-time trigger mode.
 *
 * This function fills the initial user configuration for a one-time trigger mode.
 * Calling the  initialization function with the filled parameter  configures
 * the ADC module to function in the one-time trigger mode. The settings are:
 *
 * .intEnable = false;
 * .lowPowerEnable = false;
 * .clkDividerMode = kAdcClkDividerInputOf8;
 * .resolutionMode = kAdcResolutionBitOf12or13;
 * .clkSrcMode = kAdcClkSrcOfBusClk;
 * .asyncClkEnable = false;
 * .highSpeedEnable = false;
 * .hwTriggerEnable = false;
 * .dmaEnable = false;
 * .refVoltSrcMode = kAdcRefVoltSrcOfVref;
 * .continuousConvEnable = false;
 *
 * @param userConfigPtr Pointer to the user configuration structure. See the "adc_user_config_t".
 * @return Execution status.
 */
adc_status_t ADC_DRV_StructInitUserConfigForOneTimeTriggerMode(adc_user_config_t *userConfigPtr);

/*!
 * @brief Initializes the ADC module converter. 
 *
 * This function initializes the converter in the ADC module. Regardless of the completed
 * calibration for the device, this API function, with initial configuration
 * should be called before any other operations.
 * Initial configurations are mainly for the converter itself. For advanced
 * features, the corresponding APIs should be called after this function.
 *
 * @param instance ADC instance ID.
 * @param userConfigPtr Pointer to the initialization structure. See the "adc_user_config_t".
 * @param userStatePtr Pointer to the context memory structure. See the "adc_state_t".
 * @return Execution status.
 */
adc_status_t ADC_DRV_Init(uint32_t instance, adc_user_config_t *userConfigPtr, adc_state_t *userStatePtr);

/*!
 * @brief De-initializes the ADC module converter.
 *
 * This function de-initializes the ADC module. It  gates
 * the ADC module. When ADC is no longer used in application, calling
 * this API function shuts down the device to reduce the power consumption.
 *
 * @param instance ADC instance ID.
 */
void ADC_DRV_Deinit(uint32_t instance);

/*!
 * @brief Enables the long sample mode feature.
 *
 * This function enables the long sample mode feature and sets it with the
 * indicated configuration. Launching the long sample mode  adjusts the sample 
 * period to allow the higher impedance inputs to be accurately sampled.
 *
 * @param instance ADC instance ID.
 * @param mode Selection of configuration, see to "adc_long_sample_cycle_mode_t".
 */
void ADC_DRV_EnableLongSample(uint32_t instance, adc_long_sample_cycle_mode_t mode);

/*!
 * @brief Disables the long sample mode feature.
 *
 * This API function disables  the long sample mode feature and
 *
 * @param instance ADC instance ID.
 */
void ADC_DRV_DisableLongSample(uint32_t instance);

/*!
 * @brief Enables the hardware compare feature.
 *
 * This API enables the hardware compare feature  with the
 * indicated configuration. Launching the hardware compare ensures that the
 * conversion, which results in a predefined range, can be only accepted. Values out of
 * range are ignored during conversion.
 *
 * @param instance ADC instance ID.
 * @param hwCmpConfigPtr Pointer to a configuration structure. See the "adc_hw_cmp_config_t".
 * @return Execution status.
 */
adc_status_t ADC_DRV_EnableHwCmp(uint32_t instance, adc_hw_cmp_config_t *hwCmpConfigPtr);

/*!
 * @brief Disables the hardware compare feature.
 *
 * This API function disables the  hardware compare feature.
 *
 * @param instance ADC instance ID.
 */
void ADC_DRV_DisableHwCmp(uint32_t instance);

#if FSL_FEATURE_ADC_HAS_HW_AVERAGE

/*!
 * @brief Enables the hardware average feature.
 *
 * This function enables the hardware average feature.
 * This API function enables the hardware average feature with the
 * indicated configuration. Launching the hardware average performs a hardware
 * average of multiple conversions and helps improve the stability of the
 * conversion result.
 *
 * @param instance ADC instance ID.
 * @param mode Selection of the configuration. See to "adc_hw_average_count_mode_t".
 */
void ADC_DRV_EnableHwAverage(uint32_t instance, adc_hw_average_count_mode_t mode);

/*!
 * @brief Disables the hardware average feature.
 *
 * This API function disables the hardware average feature.
 *
 * @param instance ADC instance ID.
 */
void ADC_DRV_DisableHwAverage(uint32_t instance);

#endif /* FSL_FEATURE_ADC_HAS_HW_AVERAGE */

#if FSL_FEATURE_ADC_HAS_PGA

/*!
 * @brief Enables the Programmable Gain Amplifier (PGA) feature.
 *
 * This function enables the PGA feature.
 * This API enables the PGA feature with the indicated
 * configuration. The PGA is designed to increase the dynamic range by
 * amplifying low-amplitude signals before they are fed into the ADC module.
 *
 * @param instance ADC instance ID.
 * @param pgaConfigPtr Pointer to configuration structure. See the "adc_pga_config_t".
 * @return Execution status.
 */
adc_status_t ADC_DRV_EnablePGA(uint32_t instance, adc_pga_config_t *pgaConfigPtr);

/*!
 * @brief Disables the PGA feature. 
 *
 * This API disables the PGA feature. 
 *
 * @param instance ADC instance ID.
 */
void ADC_DRV_DisablePGA(uint32_t instance);

#endif /* FSL_FEATURE_ADC_HAS_PGA */

/*!
 * @brief Configures the conversion channel by software.
 *
 * This function configures the conversion channel. When the ADC module has
 * been initialized by enabling the software trigger (disable hardware trigger),
 * calling this API triggers the conversion.
 *
 * @param instance ADC instance ID.
 * @param chnGroup Selection of the configuration group.
 * @param chnConfigPtr Pointer to the configuration structure. See the "adc_chn_config_t".
 * @return Execution status.
 */
adc_status_t ADC_DRV_ConfigConvChn(uint32_t instance,
    uint32_t chnGroup, adc_chn_config_t *chnConfigPtr);

/*!
 * @brief Waits for the latest conversion to be complete.
 *
 * This function waits for the latest conversion to be complete. When
 * triggering  the conversion by configuring the available channel, the converter is
 * launched. This API function should be called to wait for the conversion to be
 * complete when no interrupt or DMA mode is used for the ADC module. After the
 * waiting period, the available data from the conversion result are fetched. 
 * The complete flag is not cleared until the result data is read.
 *
 * @param instance ADC instance ID.
 * @param chnGroup Selection of configuration group.
 * @return Execution status.
 */
void ADC_DRV_WaitConvDone(uint32_t instance, uint32_t chnGroup);

/*!
 * @brief Pauses the current conversion by software.
 *
 * This function pauses the current conversion setting by software.
 *
 * @param instance ADC instance ID.
 * @param chnGroup Selection of configuration group.
 */
void ADC_DRV_PauseConv(uint32_t instance, uint32_t chnGroup);

/*!
 * @brief Gets the latest conversion value. 
 *
 * This function  gets the conversion value from the ADC module. 
 *
 * @param instance ADC instance ID.
 * @param chnGroup Selection of configuration group.
 * @return Unformatted conversion value.
 */
uint16_t ADC_DRV_GetConvValueRAW(uint32_t instance, uint32_t chnGroup);

/*!
 * @brief Gets the latest conversion value in the buffer when using the interrupt mode.
 *
 * This function gets the latest conversion value in the buffer when using the
 * interrupt mode. Note that this function is only available in the interrupt mode.
 *
 * @param instance ADC instance ID.
 * @param chnGroup Selection of configuration group.
 * @return Unformatted conversion value.
 */
uint16_t ADC_DRV_GetConvValueRAWInt(uint32_t instance, uint32_t chnGroup);

/*!
 * @brief Formats the initial value fetched from the ADC module.
 *
 * This function  formats the initial value fetched from the ADC module. 
 * Initial value fetched from the ADC module can't be read as a number,
 * especially for the signed value generated by the differential conversion. This
 * function can format the initial value to be a readable one.
 *
 * @param convValue Initial value directly from the register.
 * @param diffEnable Identifier for the differential mode.
 * @param mode Formatted data resolution mode.
 * @return Formatted conversion value.
 */
int32_t ADC_DRV_ConvRAWData(uint16_t convValue, bool diffEnable, adc_resolution_mode_t mode);

/*!
 * @brief Installs the user-defined callback in the ADC module.
 *
 * This function installs the user-defined callback in the ADC module.
 * When an ADC interrupt request is served, the callback is executed 
 * inside the ISR. 
 *
 * @param instance ADC instance ID.
 * @param chnGroup Selection of the configuration group.
 * @param userCallback User-defined callback function.
 * @return Execution status.
 */
adc_status_t ADC_DRV_InstallCallback(uint32_t instance, uint32_t chnGroup, adc_callback_t userCallback);

/*!
 * @brief Driver-defined ISR in the ADC module.
 *
 * This function is the driver-defined ISR in the ADC module.
 * It includes the interrupt mode processes defined by the driver. Currently, it
 * is called inside the system-defined ISR.
 *
 * @param instance ADC instance ID.
 */
void ADC_DRV_IRQHandler(uint32_t instance);

#if defined(__cplusplus)
extern }
#endif

/*!
 *@}
 */

#endif /* __FSL_ADC_DRIVER_H__ */

/******************************************************************************
 * EOF
 *****************************************************************************/

