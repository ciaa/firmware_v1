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

#ifndef __FSL_PDB_DRIVER_H__
#define __FSL_PDB_DRIVER_H__

#include <stdint.h>
#include <stdbool.h>
#include "fsl_pdb_hal.h"

/*!
 * @addtogroup pdb_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Defines the structure to configure the PDB counter.
 *
 * This structure keeps the configuration for the PDB basic counter.
 * The PDB counter is the core part of the PDB module. When
 * initialized, the PDB module can act as a simple counter.
 */
typedef struct PdbUserConfig
{
    pdb_load_mode_t loadMode;
        /*!< Selects the mode to load timing registers after set load operation. */
    bool seqErrIntEnable; /*!< Switch to enable the PDB sequence error interrupt. */
    bool dmaEnable; /*!< Switch to enable DMA support for PDB instead of interrupt. */
    pdb_clk_prescaler_div_mode_t clkPrescalerDivMode;
        /*!< Select the prescaler that counting uses the peripheral clock 
              * divided by multiplication factor. */
    pdb_trigger_src_mode_t triggerSrcMode; /*!< Select the input source of trigger.*/
    bool intEnable; /*!< Switch to enable the PDB interrupt for counter reaches to the modulus. */
    pdb_mult_factor_mode_t multFactorMode; /*!< Select the multiplication factor for prescalar. */
    bool continuousModeEnable; /*!< Switch to enable the continuous mode. */
    uint32_t pdbModulusValue; /*!< Set the value for PDB counter's modulus. */
    uint32_t delayValue; /*!< Set the value for PDB counter to cause PDB interrupt. */
} pdb_user_config_t;

/*!
 * @brief Defines the structure to configure the ADC pre-trigger in the PDB module.
 *
 * This structure keeps the configuration for ADC pre-trigger in
 * PDB module.
 */
typedef struct PdbAdcPreTriggerConfig
{
    bool backToBackModeEnable; /*!< Switch to enable the back-to-back mode. */
    bool preTriggerOutEnable; /*!< Switch to enable trigger out the pre-channel. */
    uint32_t delayValue; /*!< Set the value for ADC pre-trigger's delay value. */
} pdb_adc_pre_trigger_config_t;

/*!
 * @brief Defines the structure to configuring the DAC trigger in the PDB module.
 *
 * This structure keeps the configuration for DAC trigger in the PDB module.
 */
typedef struct PdbDacTriggerConfig
{
    bool extTriggerInputEnable;
        /*!< Switch to enable the external trigger for DAC interval counter. */
    uint32_t intervalValue; 
        /*! Set the interval value for DAC interval trigger that will cause to trigger DAC. */
} pdb_dac_trigger_config_t;

/*!
 * @brief Defines the structure to configure the pulse-out trigger in the PDB module.
 *
 * This structure keeps the configuration for the pulse-out trigger in the
 * PDB module.
 */
typedef struct PdbPulseOutTriggerConfig
{
    uint32_t pulseOutHighValue; 
        /*!< Set the value for that pulse out goes high when the PDB counter reaches to this value. */
    uint32_t pulseOutLowValue; 
        /*!< Set the value for that pulse out goes low when the PDB counter reaches to this value. */
} pdb_pulse_out_trigger_config_t;

/*!
 * @brief Defines the type of flag for PDB pre-trigger events.
 */
typedef enum _pdb_adc_pre_trigger_flag
{
    kPdbAdcPreChnFlag = 0U, /*!< ADC pre-trigger flag when the counter reaches to pre-trigger's delay value. */
    kPdbAdcPreChnErrFlag = 1U /*!< ADC pre-trigger sequence error flag. */
} pdb_adc_pre_trigger_flag_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Defines the type of user-defined callback function for the PDB module.
 */
typedef void (*pdb_callback_t)(void);

/*!
 * @brief Internal driver state information.
 *
 * The contents of this structure are internal to the driver and should not be
 *  modified by users.
 */
typedef struct PdbState
{
    pdb_callback_t userCallbackFunc; /*!< Keep the user-defined callback function. */
} pdb_state_t;

/*!
 * @brief Fills the initial user configuration for software trigger mode.
 *
 * This function fills the initial user configuration.
 * It is set as one-time, software trigger mode. The PDB modulus and delay
 * value are all set to maximum. The PDB interrupt is enabled and
 * causes the PDB interrupt when the counter hits the delay value. Then, the PDB
 * module is initialized an a normal timer if no other setting is changed.
 * The settings are:
 *
 * .loadMode = kPdbLoadImmediately;
 * .seqErrIntEnable = false;
 * .dmaEnable = false;
 * .clkPrescalerDivMode = kPdbClkPreDivBy128;
 * .triggerSrcMode = kPdbSoftTrigger;
 * .intEnable = true;
 * .multFactorMode = kPdbMultFactorAs40;
 * .continuousModeEnable = false;
 * .pdbModulusValue = 0xFFFFU;
 * .delayValue = 0xFFFFU;
 *
 * @param userConfigPtr Pointer to the user configuration structure. See the "pdb_user_config_t".
 * @return Execution status.
 */
pdb_status_t PDB_DRV_StructInitUserConfigForSoftTrigger(pdb_user_config_t *userConfigPtr);

/*!
 * @brief Initializes the PDB counter and trigger input.
 *
 * This function initializes the PDB counter and triggers the input.
 * It resets PDB registers and enables the PDB clock. Therefore, it should be 
 * called before any other operation. After it is initialized, the PDB can
 * act as a triggered timer, which enables other features in PDB module.
 *
 * @param instance PDB instance ID.
 * @param userConfigPtr Pointer to the user configuration structure. See the "pdb_user_config_t".
 * @param userStatePtr Pointer to the structure for keeping an internal state. See the "pdb_state_t".
 * @return Execution status.
 */
pdb_status_t PDB_DRV_Init(uint32_t instance, pdb_user_config_t *userConfigPtr, pdb_state_t *userStatePtr);

/*!
 * @brief De-initializes the PDB module.
 *
 * This function de-initializes the PDB module.
 * Calling this function shuts down the PDB module and reduces the power consumption.
 *
 * @param instance PDB instance ID.
 */
void PDB_DRV_Deinit(uint32_t instance);

/*!
 * @brief Triggers the PDB with a software trigger.
 *
 * This function triggers the PDB with a software trigger.
 * When the PDB is set to use the software trigger as input, calling this function
 * triggers the PDB.
 *
 * @param instance PDB instance ID.
 */
void PDB_DRV_SoftTriggerCmd(uint32_t instance);

/*!
 * @brief Gets the current counter value in the PDB module.
 *
 * This function gets the current counter value.
 *
 * @param instance PDB instance ID.
 * @return Current PDB counter value.
 */
uint32_t PDB_DRV_GetCurrentCounter(uint32_t instance);

/*!
 * @brief Gets the PDB interrupt flag.
 *
 * This function gets the PDB interrupt flag. It is asserted if the PDB interrupt occurs.
 *
 * @param instance PDB instance ID.
 * @return Assertion of indicated event.
 */
bool PDB_DRV_GetPdbCounterIntFlag(uint32_t instance);

/*!
 * @brief Clears the interrupt flag.
 *
 * This function clears the interrupt flag.
 *
 * @param instance PDB instance ID.
 */
void PDB_DRV_ClearPdbCounterIntFlag(uint32_t instance);

/*!
 * @brief Enables the ADC pre-trigger with its configuration.
 *
 * This function enables the ADC pre-trigger with its configuration.
 * The setting value takes effect according to the load-mode configured
 * during the PDB initialization, although the load operation was done inside
 * the function. This is an additional function based on the PDB counter.
 *
 * @param instance PDB instance ID.
 * @param adcChn ADC trigger channel, related to the ADC instance.
 * @param preChn ADC pre-trigger channel, related to the ADC channel group instance.
 * @param adcPreTriggerConfigPtr Pointer to structure of configuration, see to "pdb_adc_pre_trigger_config_t".
 * @return Execution status.
 */
pdb_status_t PDB_DRV_EnableAdcPreTrigger(uint32_t instance, uint32_t adcChn, uint32_t preChn,
    pdb_adc_pre_trigger_config_t *adcPreTriggerConfigPtr);

/*!
 * @brief Disables the ADC pre-trigger.
 *
 * This function disables the ADC pre-trigger. The PDB works the same way as
 * when the pre-trigger was not enabled.
 *
 * @param instance PDB instance ID.
 * @param adcChn ADC trigger channel, related to the ADC instance.
 * @param preChn ADC pre-trigger channel, related to the ADC channel group instance.
 */
void PDB_DRV_DisableAdcPreTrigger(uint32_t instance, uint32_t adcChn, uint32_t preChn);

/*!
 * @brief Gets the ADC pre-trigger flag.
 *
 * This function gets the pre-trigger flag for the PDB module. It is
 * asserted if a related event occurs.
 *
 * @param instance PDB instance ID.
 * @param adcChn ADC trigger channel, related to the ADC instance.
 * @param preChn ADC pre-trigger channel, related to the ADC channel group instance.
 * @param flag Indicated flag for event.
 * @return Assertion of indicated event.
 */
bool PDB_DRV_GetAdcPreTriggerFlag(uint32_t instance, uint32_t adcChn, uint32_t preChn,
    pdb_adc_pre_trigger_flag_t flag);

/*!
 * @brief Clears the ADC pre-trigger flag.
 *
 * This function clears the ADC pre-trigger flag  for the PDB module.
 *
 * @param instance PDB instance ID.
 * @param adcChn ADC trigger channel, related to the ADC instance.
 * @param preChn ADC pre-trigger channel, related to the ADC channel group instance.
 * @param flag Indicated flag for event.
 */
void PDB_DRV_ClearAdcPreTriggerFlag(uint32_t instance, uint32_t adcChn, uint32_t preChn,
    pdb_adc_pre_trigger_flag_t flag);

/*!
 * @brief Enables the DAC trigger with its configuration.
 *
 * This function enables the DAC trigger with its configuration.
 * The setting value takes effect according to the load mode configured
 * during PDB initialization, although the load operation is done inside
 * the function. This is an additional function based on the PDB counter.
 *
 * @param instance PDB instance ID.
 * @param dacChn DAC trigger channel, related to the DAC instance.
 * @param dacTriggerConfigPtr Pointer to structure of configuration, see to "pdb_dac_trigger_config_t".
 * @return Execution status.
 */
pdb_status_t PDB_DRV_EnableDacTrigger(uint32_t instance, uint32_t dacChn,
    pdb_dac_trigger_config_t *dacTriggerConfigPtr);

/*!
 * @brief Disables the DAC trigger.
 *
 * This function disables the DAC trigger. The PDB works the same as
 * when the DAC trigger was not enabled.
 *
 * @param instance PDB instance ID.
 * @param dacChn DAC trigger channel, related to the DAC instance.
 */
void PDB_DRV_DisableDacTrigger(uint32_t instance, uint32_t dacChn);

/*!
 * @brief Enables the pulse-out trigger with its configuration.
 *
 * This function enables the pulse-out trigger with its configuration.
 * The setting value takes effect according to the load mode configured
 * during the PDB initialization, although the load operation is done inside
 * the function. This is an additional function based on the PDB counter.
 *
 * @param instance PDB instance ID.
 * @param pulseChn Pulse out trigger channel.
 * @param pulseOutTriggerConfigPtr Pointer to structure of configuration, see to "pdb_pulse_out_trigger_config_t".
 * @return Execution status.
 */
pdb_status_t PDB_DRV_EnablePulseOutTrigger(uint32_t instance, uint32_t pulseChn,
    pdb_pulse_out_trigger_config_t *pulseOutTriggerConfigPtr);

/*!
 * @brief Disables the pulse-out trigger.
 *
 * This function disables the pulse-out trigger. The PDB works the same as
 * when the pulse out-trigger was not been enabled.
 *
 * @param instance PDB instance ID.
 * @param pulseChn Pulse out trigger channel.
 */
void PDB_DRV_DisablePulseOutTrigger(uint32_t instance, uint32_t pulseChn);

/*!
 * @brief Installs the user-defined callback for PDB module.
 *
 * This function installs the user-defined callback.
 * When the PDB interrupt request is served, the callback  is executed
 * inside the ISR.
 *
 * @param instance PDB instance ID.
 * @param userCallback User-defined callback function.
 * @return Execution status.
 */
pdb_status_t PDB_DRV_InstallCallback(uint32_t instance, pdb_callback_t userCallback);

/*!
 * @brief Driver-defined ISR in the PDB module.
 *
 * This function is the driver-defined ISR in the PDB module.
 * It includes the process for interrupt mode defined by the driver. Currently, it
 * is called inside the system-defined ISR.
 *
 * @param instance PDB instance ID.
 */
void PDB_DRV_IRQHandler(uint32_t instance);

#if defined(__cplusplus)
extern }
#endif

/*!
 *@}
 */

#endif /* __FSL_PDB_DRIVER_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/

