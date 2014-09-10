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
#include <string.h>
#include "fsl_lptmr_driver.h"
#include "fsl_lptmr_common.h"
#include "fsl_interrupt_manager.h"
#include "fsl_clock_manager.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static lptmr_state_t *volatile lptmr_state_ptrs[HW_LPTMR_INSTANCE_COUNT];

/*******************************************************************************
 * Code
 *******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_Init
 * Description   : initializes the LPTMR driver.
 * This function will initialize the LPTMR driver according to user configure 
 * strcuture.
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_DRV_Init(uint32_t instance, const lptmr_user_config_t* userConfigPtr, lptmr_state_t *userStatePtr)
{
    assert(instance < HW_LPTMR_INSTANCE_COUNT);

    uint32_t baseAddr = g_lptmrBaseAddr[instance];

    if ((!userConfigPtr) || (!userStatePtr))
    {
        return kStatus_LPTMR_NullArgument;
    }

    /* prescaler value 0 is invalid while working as pluse counter */
    if ((kLptmrTimerModePluseCounter == userConfigPtr->timerMode) && 
         (true == userConfigPtr->prescalerEnable) &&
         (kLptmrPrescalerDivide2 == userConfigPtr->prescalerValue))
    {
        return kStatus_LPTMR_InvalidPrescalerValue;
    }
    
    /* Enable clock for lptmr */
    CLOCK_SYS_EnableLptimerClock(instance);

    /* Disable lptmr and reset lptmr logic */
    LPTMR_HAL_Disable(baseAddr);    
    
    /* Working Mode configure */
    LPTMR_HAL_SetTimerModeMode(baseAddr,userConfigPtr->timerMode);

    /* Pluse counter pin select */
    LPTMR_HAL_SetPinSelectMode(baseAddr,userConfigPtr->pinSelect);

    /* Pluse counter pin polarity */
    LPTMR_HAL_SetPinPolarityMode(baseAddr,userConfigPtr->pinPolarity);

    /* LPTMR free-running configure */
    LPTMR_HAL_SetFreeRunningCmd(baseAddr,userConfigPtr->freeRunningEnable);

    /* LPTMR prescaler */
    LPTMR_HAL_SetPrescalerCmd(baseAddr,userConfigPtr->prescalerEnable);

    /* LPTMR prescaler clock source */
    LPTMR_HAL_SetPrescalerClockSourceMode(baseAddr,userConfigPtr->prescalerClockSource);

    /* LPTMR prescaler value */
    LPTMR_HAL_SetPrescalerValueMode(baseAddr,userConfigPtr->prescalerValue);

    /* Internal context */
    lptmr_state_ptrs[instance] = userStatePtr;

    userStatePtr->userCallbackFunc = NULL;

    /* LPTMR interrupt */
    if (userConfigPtr->isInterruptEnabled)
    {
        LPTMR_HAL_SetIntCmd(baseAddr,true);
        INT_SYS_EnableIRQ(g_lptmrIrqId[instance]);
    }
    else
    {
        LPTMR_HAL_SetIntCmd(baseAddr,false);
        INT_SYS_DisableIRQ(g_lptmrIrqId[instance]);
    }

    /* Caculate prescaler clock frequency or glitch filter divider */
    if ( kLptmrTimerModeTimeCounter == userConfigPtr->timerMode)
    {
        switch(userConfigPtr->prescalerClockSource)
        {
            case kLptmrPrescalerClockSourceMcgIrcClk:
                CLOCK_SYS_GetFreq(kMcgIrClock, &userStatePtr->u.prescalerClockHz);    
                break;
            case kLptmrPrescalerClockSourceLpo:
                CLOCK_SYS_GetFreq(kLpoClock, &userStatePtr->u.prescalerClockHz);    
                break;
            case kLptmrPrescalerClockSourceErClk32K:
                CLOCK_SYS_GetFreq(kOsc32kClock, &userStatePtr->u.prescalerClockHz);    
                break;
            case kLptmrPrescalerClockSourceOscErClk:
                CLOCK_SYS_GetFreq(kOsc0ErClock, &userStatePtr->u.prescalerClockHz);    
                break;
            default:
                break;
        }

        if (userConfigPtr->prescalerEnable)
        {
            userStatePtr->u.prescalerClockHz = (userStatePtr->u.prescalerClockHz >> ((uint32_t)(userConfigPtr->prescalerValue+1)));
        }
    }
    else
    {
        if (userConfigPtr->prescalerEnable)
        {
            userStatePtr->u.glitchFilterDivider = (2 << (userConfigPtr->prescalerValue));
        }
        else
        {
            userStatePtr->u.glitchFilterDivider = 1;
        }
    }
    
    return kStatus_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_Deinit
 * Description   : Deinit the LPTMR driver.
 * This function will deinit the LPTMR driver, disable LPTMR clock,
 * and disable LPTMR interrupt.
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_DRV_Deinit(uint32_t instance)
{
    assert(instance < HW_LPTMR_INSTANCE_COUNT);

    uint32_t baseAddr = g_lptmrBaseAddr[instance];

    /* Turn off lptmr hal */
    LPTMR_HAL_Disable(baseAddr);

    /* Reset all register to reset value */
    LPTMR_HAL_Init(baseAddr);    

    /* Disable the interrupt */
    INT_SYS_DisableIRQ(g_lptmrIrqId[instance]);
    
    /* Disable clock for lptmr */
    CLOCK_SYS_DisableLptimerClock(instance);

    /* Cleared state pointer */
    lptmr_state_ptrs[instance] = NULL;

    return kStatus_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_Start
 * Description   : Start LPTMR counter
 * This function will start LPTMR internal counter to count the time or pluse
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_DRV_Start(uint32_t instance)
{
    assert(instance < HW_LPTMR_INSTANCE_COUNT);

    uint32_t baseAddr = g_lptmrBaseAddr[instance];

    LPTMR_HAL_Enable(baseAddr);

    return kStatus_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_Stop
 * Description   : Stop LPTMR counter
 * This function will stop LPTMR internal counter
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_DRV_Stop(uint32_t instance)
{
    assert(instance < HW_LPTMR_INSTANCE_COUNT);

    uint32_t baseAddr = g_lptmrBaseAddr[instance];

    LPTMR_HAL_Disable(baseAddr);

    return kStatus_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_SetTimerPeriodUs
 * Description   : Set LPTMR timer counter period with unit microsecond
 * This function is used to set LPTMR timer counter period with unit microsecond
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_DRV_SetTimerPeriodUs(uint32_t instance, uint32_t us)
{
    assert(instance < HW_LPTMR_INSTANCE_COUNT);
    assert(us > 0);

    uint32_t baseAddr = g_lptmrBaseAddr[instance];
    uint32_t tick_count;

    /* This function is invalid while in Pluse Counter mode */
    if(kLptmrTimerModePluseCounter == LPTMR_HAL_GetTimerModeMode(baseAddr))
    {
        return kStatus_LPTMR_InvalidInPluseCounterMode;
    }

    /* If LPTMR is enabled, Compare register can only altered when TCF is set */
    if((true == LPTMR_HAL_IsEnabled(baseAddr)) && (false == LPTMR_HAL_IsIntPending(baseAddr)))
    {
        return kStatus_LPTMR_TcfNotSet;
    }

    if (lptmr_state_ptrs[instance]->u.prescalerClockHz < 1000000U)
    {
        if (us < (1000000U/lptmr_state_ptrs[instance]->u.prescalerClockHz))
        {
            return kStatus_LPTMR_TimerPeriodUsTooSmall;
        } 
        else
        {
            tick_count = (us/(1000000U/lptmr_state_ptrs[instance]->u.prescalerClockHz));

            /* CMR register is 16 Bits */
            if ( tick_count > 0xFFFFU )
            {
                return kStatus_LPTMR_TimerPeriodUsTooLarge;
            }
            else
            {
                LPTMR_HAL_SetCompareValue(baseAddr,tick_count); 
            }
        }
    }
    else
    {
        tick_count = (us*(lptmr_state_ptrs[instance]->u.prescalerClockHz/1000000U));

        /* CMR register is 16 Bits */
        if ( tick_count > 0xFFFFU )
        {
            return kStatus_LPTMR_TimerPeriodUsTooLarge;
        }
        else
        {
            LPTMR_HAL_SetCompareValue(baseAddr,tick_count);
        }
    }

    return kStatus_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_GetCurrentTimeUs
 * Description   : Get LPTMR current time with unit microsecond
 * This function is used to get LPTMR current time with unit microsecond
 *
 *END**************************************************************************/
uint32_t LPTMR_DRV_GetCurrentTimeUs(uint32_t instance)
{
    assert(instance < HW_LPTMR_INSTANCE_COUNT);

    uint32_t baseAddr = g_lptmrBaseAddr[instance];

    uint32_t us;
    
    /* This function is invalid while in Pluse Counter mode */
    if(kLptmrTimerModePluseCounter == LPTMR_HAL_GetTimerModeMode(baseAddr))
    {
        return kStatus_LPTMR_InvalidInPluseCounterMode;
    }

    if (lptmr_state_ptrs[instance]->u.prescalerClockHz < 1000000U)
    {
        us = LPTMR_HAL_GetCounterValue(baseAddr)*(1000000U/lptmr_state_ptrs[instance]->u.prescalerClockHz);
    }
    else
    {
        us = LPTMR_HAL_GetCounterValue(baseAddr)/(lptmr_state_ptrs[instance]->u.prescalerClockHz/1000000U);
    }

    return us;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_SetPlusePeriodCount
 * Description   : Set the pluse period value while LPTMR working in pluse counter mode
 * This function is used to set the pluse period value while LPTMR working in pluse counter mode
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_DRV_SetPlusePeriodCount(uint32_t instance, uint32_t plusePeriodCount)
{   
    assert(instance < HW_LPTMR_INSTANCE_COUNT);
    assert(plusePeriodCount > 0);

    uint32_t baseAddr = g_lptmrBaseAddr[instance];

     /* This function is invalid while in time Counter mode */
    if(kLptmrTimerModeTimeCounter == LPTMR_HAL_GetTimerModeMode(baseAddr))
    {
        return kStatus_LPTMR_InvalidInTimeCounterMode;
    }

    /* If LPTMR is enabled, Compare register can only altered when TCF is set */
    if((true == LPTMR_HAL_IsEnabled(baseAddr)) && (false == LPTMR_HAL_IsIntPending(baseAddr)))
    {
        return kStatus_LPTMR_TcfNotSet;
    }

    /* pluse period should be integer multiple of the glitch filter divider */
    if (0 != (plusePeriodCount % lptmr_state_ptrs[instance]->u.glitchFilterDivider))
    {
        return kStatus_LPTMR_InvalidPlusePeriodCount;
    }

    LPTMR_HAL_SetCompareValue(baseAddr, plusePeriodCount % lptmr_state_ptrs[instance]->u.glitchFilterDivider);

    return kStatus_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_GetCurrentPluseCount
 * Description   : Get current pluse count captured in the pluse input pin
 * This function is used to get current pluse count captured in the pluse input pin
 *
 *END**************************************************************************/
uint32_t LPTMR_DRV_GetCurrentPluseCount(uint32_t instance)
{
    assert(instance < HW_LPTMR_INSTANCE_COUNT);

    uint32_t baseAddr = g_lptmrBaseAddr[instance];
    uint32_t count;
    
    /* This function is invalid while in time Counter mode */
    if(kLptmrTimerModeTimeCounter == LPTMR_HAL_GetTimerModeMode(baseAddr))
    {
        return kStatus_LPTMR_InvalidInTimeCounterMode;
    }

    count = LPTMR_HAL_GetCounterValue(baseAddr) * lptmr_state_ptrs[instance]->u.glitchFilterDivider;

    return count;
}


/*FUNCTION*********************************************************************
 *
 * Function Name : LPTMR_DRV_InstallCallback
 * Description   : Install the user-defined callback in LPTMR module.
 * When an LPTMR interrupt request is served, the callback will be executed 
 * inside the ISR. 
 *
 *END*************************************************************************/
lptmr_status_t LPTMR_DRV_InstallCallback(uint32_t instance, lptmr_callback_t userCallback)
{
    assert(instance < HW_LPTMR_INSTANCE_COUNT);

    assert (instance < HW_LPTMR_INSTANCE_COUNT);
    if (!lptmr_state_ptrs[instance])
    {
        return kStatus_LPTMR_NotInitlialized;
    }
    /* Fill callback function into state structure. */
    lptmr_state_ptrs[instance]->userCallbackFunc = userCallback;
    
    return kStatus_LPTMR_Success;
}


/*FUNCTION*********************************************************************
 *
 * Function Name : LPTMR_DRV_IRQHandler
 * Description   : The driver-defined ISR in LPTMR module. 
 * It includes the process for interrupt mode defined by driver. Currently, it
 * will be called inside the system-defined ISR.
 *
 *END*************************************************************************/
void LPTMR_DRV_IRQHandler(uint32_t instance)
{
    assert(instance < HW_LPTMR_INSTANCE_COUNT);

    uint32_t baseAddr = g_lptmrBaseAddr[instance];

    /* Clear interrupt flag */
    LPTMR_HAL_ClearIntFlag(baseAddr);

    if (lptmr_state_ptrs[instance])
    {
        if (lptmr_state_ptrs[instance]->userCallbackFunc)
        {
            /* Execute user-defined callback function. */
            (*(lptmr_state_ptrs[instance]->userCallbackFunc))();
        }
    }
}
/*******************************************************************************
 * EOF
 *******************************************************************************/

