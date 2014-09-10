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
#include "fsl_pdb_driver.h"
#include "fsl_pdb_common.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"

/*!
 * @brief Table of pointers to internal state structure for PDB instances.
 */
static pdb_state_t *volatile g_pdbStatePtr[HW_PDB_INSTANCE_COUNT];

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_StructInitUserConfigForSoftTrigger
 * Description   : Fill the initial user configuration for PDB module.
 * It will be set as one-time, software trigger mode. The PDB modulus and delay
 * value are all set to the maximum. The PDB interrupt is enabled that will
 * cause the PDB interrupt when the counter hits the delay value. Then the PDB
 * module will be initialized an normal timer if no other setting is changed.
 *
 *END*************************************************************************/
pdb_status_t PDB_DRV_StructInitUserConfigForSoftTrigger(pdb_user_config_t *userConfigPtr)
{
    if (!userConfigPtr)
    {
        return kStatus_PDB_InvalidArgument;
    }
    userConfigPtr->loadMode = kPdbLoadImmediately;
    userConfigPtr->seqErrIntEnable = false;
    userConfigPtr->dmaEnable = false;
    userConfigPtr->clkPrescalerDivMode = kPdbClkPreDivBy128;
    userConfigPtr->triggerSrcMode = kPdbSoftTrigger;
    userConfigPtr->intEnable = true;
    userConfigPtr->multFactorMode = kPdbMultFactorAs40;
    userConfigPtr->continuousModeEnable = false;
    userConfigPtr->pdbModulusValue = 0xFFFFU;
    userConfigPtr->delayValue = 0xFFFFU;

    return kStatus_PDB_Success;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_Init
 * Description   : Initialize the PDB counter and trigger input for PDB module.
 * It resets PDB registers and enables the clock for PDB. So it should be 
 * called before any operation to PDB module. After initialized, the PDB can
 * ack as a triggered timer, which lays the foundation for other features in
 * PDB module.
 *
 *END*************************************************************************/
pdb_status_t PDB_DRV_Init(uint32_t instance, pdb_user_config_t *userConfigPtr, pdb_state_t *userStatePtr)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);
    uint32_t baseAddr = g_pdbBaseAddr[instance];
    bool mEnable;

    if (!userConfigPtr)
    {
        return kStatus_PDB_InvalidArgument;
    }
    /* Enable the clock gate from clock manager. */
    mEnable = CLOCK_SYS_GetPdbGateCmd(instance);
    if (!mEnable)
    {
        CLOCK_SYS_EnablePdbClock(instance);
    }
    /* Reset the registers for PDB module to reset state. */
    PDB_HAL_Init(baseAddr);

    PDB_HAL_Enable(baseAddr);
    PDB_HAL_SetLoadMode(baseAddr,userConfigPtr->loadMode);
    PDB_HAL_SetSeqErrIntCmd(baseAddr,userConfigPtr->seqErrIntEnable);
    PDB_HAL_SetDmaCmd(baseAddr,userConfigPtr->dmaEnable);
    PDB_HAL_SetPreDivMode(baseAddr, userConfigPtr->clkPrescalerDivMode);
    PDB_HAL_SetTriggerSrcMode(baseAddr, userConfigPtr->triggerSrcMode);
    PDB_HAL_SetIntCmd(baseAddr, userConfigPtr->intEnable);
    PDB_HAL_SetPreMultFactorMode(baseAddr, userConfigPtr->multFactorMode);
    PDB_HAL_SetContinuousModeCmd(baseAddr, userConfigPtr->continuousModeEnable);
    PDB_HAL_SetModulusValue(baseAddr, userConfigPtr->pdbModulusValue);
    PDB_HAL_SetIntDelayValue(baseAddr, userConfigPtr->delayValue);
    PDB_HAL_SetLoadRegsCmd(baseAddr);

    /* Configure NVIC. */
    if (userConfigPtr->intEnable)
    {
        /* Enable PDB interrupt in NVIC level.*/
        INT_SYS_EnableIRQ(g_pdbIrqId[instance] );
    }
    else
    {
        /* Disable PDB interrupt in NVIC level.*/
        INT_SYS_DisableIRQ(g_pdbIrqId[instance] );
    }

    /* Keep the context for DPB driver. */
    g_pdbStatePtr[instance] = userStatePtr;

    return kStatus_PDB_Success;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_Deinit
 * Description   : De-initialize the PDB module.
 * When the PDB module is not used. Calling this function would shutdown the 
 * PDB module and reduce the power consumption.
 *
 *END*************************************************************************/
void PDB_DRV_Deinit(uint32_t instance)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);
    uint32_t baseAddr = g_pdbBaseAddr[instance];

    INT_SYS_DisableIRQ( g_pdbIrqId[instance] );
    PDB_HAL_SetDmaCmd(baseAddr, false);
    PDB_HAL_SetIntCmd(baseAddr, false);
    PDB_HAL_Disable(baseAddr);
    CLOCK_SYS_DisablePdbClock(instance);
    g_pdbStatePtr[instance] = NULL;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_SoftTriggerCmd
 * Description   : Trigger PDB by software trigger. 
 * When the PDB is set to use software trigger as input, Calling this function
 * would trigger the PDB.
 *
 *END*************************************************************************/
void PDB_DRV_SoftTriggerCmd(uint32_t instance)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);
    uint32_t baseAddr = g_pdbBaseAddr[instance];

    PDB_HAL_SetSoftTriggerCmd(baseAddr);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_GetCurrentCounter
 * Description   : Get the current counter value in PDB module.
 *
 *END*************************************************************************/
uint32_t PDB_DRV_GetCurrentCounter(uint32_t instance)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);
    uint32_t baseAddr = g_pdbBaseAddr[instance];

    return PDB_HAL_GetCounterValue(baseAddr);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_GetPdbCounterIntFlag
 * Description   : Get the interrupt flag for PDB module. It will be
 * asserted if the PDB interrupt occurs.
 *
 *END*************************************************************************/
bool PDB_DRV_GetPdbCounterIntFlag(uint32_t instance)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);
    uint32_t baseAddr = g_pdbBaseAddr[instance];

    return PDB_HAL_GetIntFlag(baseAddr);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_ClearPdbCounterIntFlag
 * Description   : Clear the interrupt flag for PDB module.
 *
 *END*************************************************************************/
void PDB_DRV_ClearPdbCounterIntFlag(uint32_t instance)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);
    uint32_t baseAddr = g_pdbBaseAddr[instance];

    PDB_HAL_ClearIntFlag(baseAddr);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_EnableAdcPreTrigger
 * Description   : Enable the ADC pre-trigger with its configuration. 
 * The setting value will take effect according to the load mode configured
 * when initializing the PDB, though the load operation has been done inside
 * the function. This is an additional function based on the PDB counter.
 *
 *END*************************************************************************/
pdb_status_t PDB_DRV_EnableAdcPreTrigger(uint32_t instance, uint32_t adcChn, uint32_t preChn,
        pdb_adc_pre_trigger_config_t *adcPreTriggerConfigPtr)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);

    uint32_t baseAddr = g_pdbBaseAddr[instance];

    PDB_HAL_SetPreTriggerCmd(baseAddr, adcChn, preChn, true);
    PDB_HAL_SetPreTriggerBackToBackCmd(baseAddr, adcChn, preChn,
        adcPreTriggerConfigPtr->backToBackModeEnable);
    PDB_HAL_SetPreTriggerOutputCmd(baseAddr, adcChn, preChn,
        adcPreTriggerConfigPtr->preTriggerOutEnable);
    PDB_HAL_SetPreTriggerDelayCount(baseAddr, adcChn, preChn, 
        adcPreTriggerConfigPtr->delayValue);
    PDB_HAL_SetLoadRegsCmd(baseAddr);

    return kStatus_PDB_Success;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_DisableAdcPreTrigger
 * Description   : Disable the ADC pre-trigger. The PDB would work normally
 * as before the pre-trigger had not been enabled.
 *
 *END*************************************************************************/
void PDB_DRV_DisableAdcPreTrigger(uint32_t instance, uint32_t adcChn, uint32_t preChn)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);

    uint32_t baseAddr = g_pdbBaseAddr[instance];

    PDB_HAL_SetPreTriggerCmd(baseAddr, adcChn, preChn, false);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_GetAdcPreTriggerFlag
 * Description   : Get the flag of ADC pre-trigger for PDB module. It will be
 * asserted if related event occurs.
 *
 *END*************************************************************************/
bool PDB_DRV_GetAdcPreTriggerFlag(uint32_t instance, uint32_t adcChn, uint32_t preChn,
    pdb_adc_pre_trigger_flag_t flag)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);

    uint32_t baseAddr = g_pdbBaseAddr[instance];
    bool bRet;
    switch (flag)
    {
    case kPdbAdcPreChnFlag:
        bRet = PDB_HAL_GetPreTriggerFlag(baseAddr, adcChn, preChn);
        break;
    case kPdbAdcPreChnErrFlag:
        bRet = PDB_HAL_GetPreTriggerSeqErrFlag(baseAddr, adcChn, preChn);
        break;
    default:
        bRet = false;
        break;
    }
    return bRet;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_ClearAdcPreTriggerFlag
 * Description   : Clear the flag of ADC pre-trigger for PDB module.
 *
 *END*************************************************************************/
void PDB_DRV_ClearAdcPreTriggerFlag(uint32_t instance, uint32_t adcChn, uint32_t preChn,
    pdb_adc_pre_trigger_flag_t flag)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);

    uint32_t baseAddr = g_pdbBaseAddr[instance];
    switch (flag)
    {
    case kPdbAdcPreChnFlag:
        PDB_HAL_ClearPreTriggerFlag(baseAddr, adcChn, preChn);
        break;
    case kPdbAdcPreChnErrFlag:
        PDB_HAL_ClearPreTriggerSeqErrFlag(baseAddr, adcChn, preChn);
        break;
    default:
        PDB_HAL_ClearPreTriggerFlag(baseAddr, adcChn, preChn);
        PDB_HAL_ClearPreTriggerSeqErrFlag(baseAddr, adcChn, preChn);
        break;
    }
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_EnableDacTrigger
 * Description   : Enable the DAC trigger with its configuration.
 * The setting value will take effect according to the load mode configured
 * when initializing the PDB, though the load operation has been done inside
 * the function. This is an additional function based on the PDB counter.
 *
 *END*************************************************************************/
pdb_status_t PDB_DRV_EnableDacTrigger(uint32_t instance, uint32_t dacChn, pdb_dac_trigger_config_t *dacTriggerConfigPtr)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);

    uint32_t baseAddr = g_pdbBaseAddr[instance];

    if (!dacTriggerConfigPtr)
    {
        return kStatus_PDB_InvalidArgument;
    }

    PDB_HAL_SetDacIntervalTriggerCmd(baseAddr, dacChn, true);
    PDB_HAL_SetDacExtTriggerInputCmd(baseAddr, dacChn, dacTriggerConfigPtr->extTriggerInputEnable);
    PDB_HAL_SetDacIntervalValue(baseAddr,dacChn, dacTriggerConfigPtr->intervalValue);
    PDB_HAL_SetLoadRegsCmd(baseAddr);

    return kStatus_PDB_Success;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_DisableDacTrigger
 * Description   : Disable the DAC trigger. The PDB would work normally
 * as before the DAC trigger had not been enabled.
 *
 *END*************************************************************************/
void PDB_DRV_DisableDacTrigger(uint32_t instance, uint32_t dacChn)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);
    
    uint32_t baseAddr = g_pdbBaseAddr[instance];

    PDB_HAL_SetDacIntervalTriggerCmd(baseAddr, dacChn, false);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_EnablePulseOutTrigger
 * Description   : Enable the pulse out trigger with its configuration. 
 * The setting value will take effect according to the load mode configured
 * when initializing the PDB, though the load operation has been done inside
 * the function. This is an additional function based on the PDB counter.
 *
 *END*************************************************************************/
pdb_status_t PDB_DRV_EnablePulseOutTrigger(uint32_t instance, uint32_t pulseChn, 
    pdb_pulse_out_trigger_config_t *pulseOutTriggerConfigPtr)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);

    uint32_t baseAddr = g_pdbBaseAddr[instance];

    PDB_HAL_SetPulseOutCmd(baseAddr, pulseChn, true);
    PDB_HAL_SetPulseOutDelayForHigh(baseAddr, pulseChn, 
        pulseOutTriggerConfigPtr->pulseOutHighValue);
    PDB_HAL_SetPulseOutDelayForLow(baseAddr, pulseChn, 
        pulseOutTriggerConfigPtr->pulseOutLowValue);
    return kStatus_PDB_Success;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_DisablePulseOutTrigger
 * Description   : Disable the pulse out trigger. The PDB would work normally
 * as before the pulse out trigger had not been enabled.
 *
 *END*************************************************************************/
void PDB_DRV_DisablePulseOutTrigger(uint32_t instance, uint32_t pulseChn)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);

    uint32_t baseAddr = g_pdbBaseAddr[instance];

    PDB_HAL_SetPulseOutCmd(baseAddr, pulseChn, false);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_InstallCallback
 * Description   : Install the user-defined callback for PDB module.
 * When an PDB interrupt request is served, the callback will be executed
 * inside the ISR.
 *
 *END*************************************************************************/
pdb_status_t PDB_DRV_InstallCallback(uint32_t instance, pdb_callback_t userCallback)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);
    if ( !g_pdbStatePtr[instance] )
    {
        return kStatus_PDB_Failed;
    }
    g_pdbStatePtr[instance]->userCallbackFunc = userCallback;
    return kStatus_PDB_Success;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_IRQHandler
 * Description   : The driver-defined ISR in PDB module. 
 * It includes the process for interrupt mode defined by driver. Currently, it
 * will be called inside the system-defined ISR.
 * 
 *END*************************************************************************/
void PDB_DRV_IRQHandler(uint32_t instance)
{
    assert(instance < HW_PDB_INSTANCE_COUNT);
    if (g_pdbStatePtr[instance])
    {
        /* Call the user-defined callback. */
        if (g_pdbStatePtr[instance]->userCallbackFunc)
        {
            (*(g_pdbStatePtr[instance]->userCallbackFunc))();
        }
    }
    if ( PDB_DRV_GetPdbCounterIntFlag(instance))
    {
        PDB_DRV_ClearPdbCounterIntFlag(instance);
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
