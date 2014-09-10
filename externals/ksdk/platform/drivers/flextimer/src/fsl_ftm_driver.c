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

#include "fsl_ftm_driver.h"
#include "fsl_ftm_common.h"
#include "fsl_clock_manager.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*See fsl_ftm_driver.h for documentation of this function.*/
void FTM_DRV_Init(uint8_t instance, ftm_user_config_t * info)
{
    assert(instance < HW_FTM_INSTANCE_COUNT);

    uint32_t ftmBaseAddr = g_ftmBaseAddr[instance];
    uint32_t channel = 0;

    /* clock setting initialization*/
    CLOCK_SYS_EnableFtmClock(instance);

    FTM_HAL_Reset(ftmBaseAddr, instance);

    /* Use FTM mode */
    FTM_HAL_Enable(ftmBaseAddr, true);
    FTM_HAL_SetClockPs(ftmBaseAddr, kFtmDividedBy2);
    /* Use the Enhanced PWM synchronization method */
    FTM_HAL_SetPwmSyncModeCmd(ftmBaseAddr, true);
    /* Enable needed bits for software trigger to update registers with its buffer value */
    FTM_HAL_SetCounterSoftwareSyncModeCmd(ftmBaseAddr, true);
    FTM_HAL_SetModCntinCvSoftwareSyncModeCmd(ftmBaseAddr, true);
    FTM_HAL_SetCntinPwmSyncModeCmd(ftmBaseAddr, true);
    for (channel = 0; channel < FSL_FEATURE_FTM_CHANNEL_COUNTn(instance); channel = channel + 2)
    {
        FTM_HAL_SetDualChnPwmSyncCmd(ftmBaseAddr, channel, true);
    }

    FTM_HAL_SetTofFreq(ftmBaseAddr, info->tofFrequency);
    FTM_HAL_SetWriteProtectionCmd(ftmBaseAddr, info->isWriteProtection);

    NVIC_ClearPendingIRQ(g_ftmIrqId[instance]);
    INT_SYS_EnableIRQ(g_ftmIrqId[instance]);
}

/*See fsl_ftm_driver.h for documentation of this function.*/
void FTM_DRV_Deinit(uint8_t instance)
{
    assert(instance < HW_FTM_INSTANCE_COUNT);

    /* disable clock for FTM.*/
    CLOCK_SYS_DisableFtmClock(instance);

    INT_SYS_DisableIRQ(g_ftmIrqId[instance]);
}

void FTM_DRV_SetFaultIntCmd(uint32_t instance, bool faultEnable)
{
    if (faultEnable)
    {
        FTM_HAL_EnableFaultInt(g_ftmBaseAddr[instance]);
    }
    else
    {
        FTM_HAL_DisableFaultInt(g_ftmBaseAddr[instance]);
    }

}

void FTM_DRV_SetTimeOverflowIntCmd(uint32_t instance, bool overflowEnable)
{
    if (overflowEnable)
    {
        FTM_HAL_EnableTimerOverflowInt(g_ftmBaseAddr[instance]);
    }
    else
    {
        FTM_HAL_DisableTimerOverflowInt(g_ftmBaseAddr[instance]);
    }
}

void FTM_DRV_QuadDecodeStart(uint8_t instance, ftm_phase_params_t *phaseAParams,
                                      ftm_phase_params_t *phaseBParams, ftm_quad_decode_mode_t quadMode)
{
    assert(instance < HW_FTM_INSTANCE_COUNT);
    assert(phaseAParams);
    assert(phaseBParams);

    uint32_t ftmBaseAddr = g_ftmBaseAddr[instance];

    FTM_HAL_SetQuadMode(ftmBaseAddr, quadMode);
    FTM_HAL_SetQuadPhaseAFilterCmd(ftmBaseAddr, phaseAParams->kFtmPhaseInputFilter);
    if (phaseAParams->kFtmPhaseInputFilter)
    {
        /* Set Phase A filter value if phase filter is enabled */
        FTM_HAL_SetChnInputCaptureFilter(ftmBaseAddr, HW_CHAN0, phaseAParams->kFtmPhaseFilterVal);
    }
    FTM_HAL_SetQuadPhaseBFilterCmd(ftmBaseAddr, phaseBParams->kFtmPhaseInputFilter);
    if (phaseBParams->kFtmPhaseInputFilter)
    {
        /* Set Phase B filter value if phase filter is enabled */
        FTM_HAL_SetChnInputCaptureFilter(ftmBaseAddr, HW_CHAN1, phaseBParams->kFtmPhaseFilterVal);
    }
    FTM_HAL_SetQuadPhaseAPolarity(ftmBaseAddr, phaseAParams->kFtmPhasePolarity);
    FTM_HAL_SetQuadPhaseBPolarity(ftmBaseAddr, phaseBParams->kFtmPhasePolarity);

    FTM_HAL_SetQuadDecoderCmd(ftmBaseAddr, true);

    /* Set clock source to start the counter */
    FTM_HAL_SetClockSource(ftmBaseAddr, kClock_source_FTM_SystemClk);
}

void FTM_DRV_QuadDecodeStop(uint8_t instance)
{
    /* Stop the FTM counter */
    FTM_HAL_SetClockSource(g_ftmBaseAddr[instance], kClock_source_FTM_None);

    FTM_HAL_SetQuadDecoderCmd(g_ftmBaseAddr[instance], false);
}

void FTM_DRV_CounterStart(uint8_t instance, ftm_counting_mode_t countMode, uint32_t countStartVal,
                                 uint32_t countFinalVal, bool enableOverflowInt)
{
    assert(instance < HW_FTM_INSTANCE_COUNT);

    uint32_t ftmBaseAddr = g_ftmBaseAddr[instance];
    uint32_t channel = 0;

    /* Clear the overflow flag */
    FTM_HAL_ClearTimerOverflow(ftmBaseAddr);
    FTM_HAL_SetCounterInitVal(ftmBaseAddr, countStartVal);
    FTM_HAL_SetMod(ftmBaseAddr, countFinalVal);
    FTM_HAL_SetCounter(ftmBaseAddr, 0);

    /* Use FTM as counter, disable all the channels */
    for (channel = 0; channel < FSL_FEATURE_FTM_CHANNEL_COUNTn(instance); channel++)
    {
        FTM_HAL_SetChnEdgeLevel(ftmBaseAddr, channel, 0);
    }

    if (countMode == kCounting_FTM_UP)
    {
        FTM_HAL_SetQuadDecoderCmd(ftmBaseAddr, false);
        FTM_HAL_SetCpwms(ftmBaseAddr, 0);
    }
    else if (countMode == kCounting_FTM_UpDown)
    {
        FTM_HAL_SetQuadDecoderCmd(ftmBaseAddr, false);
        FTM_HAL_SetCpwms(ftmBaseAddr, 1);
    }

    /* Activate interrupts if required */
    FTM_DRV_SetTimeOverflowIntCmd(instance, enableOverflowInt);

    /* Issue a software trigger to update registers */
    FTM_HAL_SetSoftwareTriggerCmd(ftmBaseAddr, true);

    /* Set clock source to start the counter */
    FTM_HAL_SetClockSource(ftmBaseAddr, kClock_source_FTM_SystemClk);
}

void FTM_DRV_CounterStop(uint8_t instance)
{
    /* Stop the FTM counter */
    FTM_HAL_SetClockSource(g_ftmBaseAddr[instance], kClock_source_FTM_None);

    FTM_HAL_SetCpwms(g_ftmBaseAddr[instance], 0);

    /* Disable the overflow interrupt */
    FTM_DRV_SetTimeOverflowIntCmd(instance, false);
}

uint32_t FTM_DRV_CounterRead(uint8_t instance)
{
    assert(instance < HW_FTM_INSTANCE_COUNT);

    return FTM_HAL_GetCounter(g_ftmBaseAddr[instance]);
}

/*See fsl_ftm_driver.h for documentation of this function.*/
void FTM_DRV_PwmStop(uint8_t instance, ftm_pwm_param_t *param, uint8_t channel)
{
    assert((param->mode == kFtmEdgeAlignedPWM) || (param->mode == kFtmCenterAlignedPWM) ||
           (param->mode == kFtmCombinedPWM));
    assert(instance < HW_FTM_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_FTM_CHANNEL_COUNTn(instance));

    uint32_t ftmBaseAddr = g_ftmBaseAddr[instance];

    /* Stop the FTM counter */
    FTM_HAL_SetClockSource(ftmBaseAddr, kClock_source_FTM_None);

    FTM_HAL_DisablePwmMode(ftmBaseAddr, param, channel);

    /* Clear out the registers */
    FTM_HAL_SetMod(ftmBaseAddr, 0);
    FTM_HAL_SetCounter(ftmBaseAddr, 0);
}

/*See fsl_ftm_driver.h for documentation of this function.*/
void FTM_DRV_PwmStart(uint8_t instance, ftm_pwm_param_t *param, uint8_t channel)
{
    uint32_t uFTMhz;
    uint16_t uMod, uCnv, uCnvFirstEdge = 0;

    assert(instance < HW_FTM_INSTANCE_COUNT);
    assert(param->uDutyCyclePercent <= 100);
    assert(channel < FSL_FEATURE_FTM_CHANNEL_COUNTn(instance));

    uint32_t ftmBaseAddr = g_ftmBaseAddr[instance];

    /* Clear the overflow flag */
    FTM_HAL_ClearTimerOverflow(g_ftmBaseAddr[instance]);

    FTM_HAL_EnablePwmMode(ftmBaseAddr, param, channel);

#if FSL_FEATURE_FTM_BUS_CLOCK
    CLOCK_SYS_GetFreq(kBusClock, &uFTMhz);
#else
    CLOCK_SYS_GetFreq(kSystemClock, &uFTMhz);
#endif

    /* Based on Ref manual, in PWM mode CNTIN is to be set 0*/
    FTM_HAL_SetCounterInitVal(ftmBaseAddr, 0);

    uFTMhz = uFTMhz / (1 << FTM_HAL_GetClockPs(ftmBaseAddr));

    switch(param->mode)
    {
        case kFtmEdgeAlignedPWM:
            uMod = uFTMhz / (param->uFrequencyHZ) - 1;
            uCnv = uMod * param->uDutyCyclePercent / 100;
            /* For 100% duty cycle */
            if(uCnv >= uMod)
            {
                uCnv = uMod + 1;
            }
            FTM_HAL_SetMod(ftmBaseAddr, uMod);
            FTM_HAL_SetChnCountVal(ftmBaseAddr, channel, uCnv);
            break;
        case kFtmCenterAlignedPWM:
            uMod = uFTMhz / (param->uFrequencyHZ * 2);
            uCnv = uMod * param->uDutyCyclePercent / 100;
            /* For 100% duty cycle */
            if(uCnv >= uMod)
            {
                uCnv = uMod + 1;
            }
            FTM_HAL_SetMod(ftmBaseAddr, uMod);
            FTM_HAL_SetChnCountVal(ftmBaseAddr, channel, uCnv);
            break;
        case kFtmCombinedPWM:
            uMod = uFTMhz / (param->uFrequencyHZ) - 1;
            uCnv = uMod * param->uDutyCyclePercent / 100;
            uCnvFirstEdge = uMod * param->uFirstEdgeDelayPercent / 100;
            /* For 100% duty cycle */
            if(uCnv >= uMod)
            {
                uCnv = uMod + 1;
            }
            FTM_HAL_SetMod(ftmBaseAddr, uMod);
            FTM_HAL_SetChnCountVal(ftmBaseAddr, FTM_HAL_GetChnPairIndex(channel), uCnvFirstEdge);
            FTM_HAL_SetChnCountVal(ftmBaseAddr, FTM_HAL_GetChnPairIndex(channel) + 1,
                                   uCnv + uCnvFirstEdge);
            break;
        default:
            assert(0);
            break;
    }
    /* Issue a software trigger to update registers */
    FTM_HAL_SetSoftwareTriggerCmd(ftmBaseAddr, true);

    /* Set clock source to start counter */
    FTM_HAL_SetClockSource(ftmBaseAddr, kClock_source_FTM_SystemClk);
}

void FTM_DRV_IRQHandler(uint8_t instance)
{
    uint32_t ftmBaseAddr = g_ftmBaseAddr[instance];

    /* Clear the Status flag if the interrupt is enabled */
    if (FTM_HAL_IsOverflowIntEnabled(ftmBaseAddr))
    {
        FTM_HAL_ClearTimerOverflow(ftmBaseAddr);
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

