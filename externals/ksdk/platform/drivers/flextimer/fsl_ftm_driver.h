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
#ifndef __FSL_FTM_DRIVER_H__
#define __FSL_FTM_DRIVER_H__

#include "fsl_ftm_hal.h"
#include "fsl_interrupt_manager.h"

/*!
 * @addtogroup ftm_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Configuration structure that the user needs to set */
typedef struct FtmUserConfig {
    uint8_t tofFrequency;
    uint8_t BDMMode;
    bool isWriteProtection;
} ftm_user_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the FTM driver.
 *
 * @param instance The FTM peripheral instance number.
 * @param info The FTM user configuration structure.
 */
void FTM_DRV_Init(uint8_t instance, ftm_user_config_t *info);

/*!
 * @brief Shuts down the FTM driver.
 *
 * @param instance The FTM peripheral instance number.
 */
void FTM_DRV_Deinit(uint8_t instance);

/*!
 * @brief Stops channel PWM.
 *
 * @param instance The FTM peripheral instance number.
 * @param param FTM driver PWM parameter to configure PWM options
 * @param channel The channel number. In combined mode, the code will find the channel
 *                pair associated with the channel number passed in.
 */
void FTM_DRV_PwmStop(uint8_t instance, ftm_pwm_param_t *param, uint8_t channel);

/*!
 * @brief Configures duty cycle and frequency and starts outputting PWM on specified channel .
 *
 * @param instance The FTM peripheral instance number.
 * @param param FTM driver PWM parameter to configure PWM options
 * @param channel The channel number. In combined mode, the code will find the channel
 *                pair associated with the channel number passed in.
 */
void FTM_DRV_PwmStart(uint8_t instance, ftm_pwm_param_t *param, uint8_t channel);

/*!
 * @brief Configures the parameters needed and activates quadrature decode mode.
 *
 * @param instance     Instance number of the FTM module.
 * @param phaseAParams Phase A configuration parameters
 * @param phaseBParams Phase B configuration parameters
 * @param quadMode     Selects encoding mode used in quadrature decoder mode
 */
void FTM_DRV_QuadDecodeStart(uint8_t instance, ftm_phase_params_t *phaseAParams,
                                      ftm_phase_params_t *phaseBParams, ftm_quad_decode_mode_t quadMode);

/*!
 * @brief De-activates quadrature decode mode.
 *
 * @param instance  Instance number of the FTM module.
 */
void FTM_DRV_QuadDecodeStop(uint8_t instance);

/*!
 * @brief Starts the FTM counter.
 *
 * This function provides access to the FTM counter. The counter can be run in
 * Up-counting and Up-down counting modes. To run the counter in Free running mode,
 * choose Up-counting option and provide 0x0 for the countStartVal and 0xFFFF for
 * countFinalVal.
 *
 * @param instance The FTM peripheral instance number.
 * @param countMode The FTM counter mode defined by ftm_counting_mode_t.
 * @param countStartVal The starting value that is stored in the CNTIN register.
 * @param countFinalVal The final value that is stored in the MOD register.
 * @param enableOverflowInt true: enable timer overflow interrupt; false: disable
 */
void FTM_DRV_CounterStart(uint8_t instance, ftm_counting_mode_t countMode, uint32_t countStartVal,
                                 uint32_t countFinalVal, bool enableOverflowInt);

/*!
 * @brief Stops the FTM counter.
 *
 * @param instance The FTM peripheral instance number.
 */
void FTM_DRV_CounterStop(uint8_t instance);

/*!
 * @brief Reads back the current value of the FTM counter.
 *
 * @param instance The FTM peripheral instance number.
 */
uint32_t FTM_DRV_CounterRead(uint8_t instance);

/*!
 * @brief Enables or disables the timer overflow interrupt.
 *
 * @param instance The FTM peripheral instance number.
 * @param overflowEnable true: enable the timer overflow interrupt, false: disable
 */
void FTM_DRV_SetTimeOverflowIntCmd(uint32_t instance, bool overflowEnable);

/*!
 * @brief Enables or disables the fault interrupt.
 *
 * @param instance The FTM peripheral instance number.
 * @param faultEnable true: enable the fault interrupt, false: disable
 */
void FTM_DRV_SetFaultIntCmd(uint32_t instance, bool faultEnable);

/*!
 * @brief Action to take when an FTM interrupt is triggered.
 *
 * The timer overflow flag is checked and cleared if set.
 *
 * @param instance   Instance number of the FTM module.
 */
void FTM_DRV_IRQHandler(uint8_t instance);


/*Other API functions are for input capture, output compare, dual edge capture, and quadrature. */
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_FTM_DRIVER_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

