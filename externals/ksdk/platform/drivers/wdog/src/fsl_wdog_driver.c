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

#include "fsl_wdog_driver.h"
#include "fsl_wdog_common.h"
#include "fsl_interrupt_manager.h"
#include "fsl_clock_manager.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/
static uint32_t wdogWctInstructionCount;

/*******************************************************************************
 * Code
 *******************************************************************************/

/*FUNCTION****************************************************************
 *
 * Function Name : WDOG_DRV_Unlock
 * Description   : Unlock watchdog register written
 * This function is used to unlock the WDOG register written because WDOG register
 * will lock automatically after 256 bus clock. Written while the register is
 * locked has no affect.
 *
 *END*********************************************************************/
static void WDOG_DRV_Unlock(void)
{
    INT_SYS_DisableIRQGlobal();

    WDOG_HAL_Unlock(g_wdogBaseAddr[0]);

    INT_SYS_EnableIRQGlobal();
}

/*FUNCTION****************************************************************
 *
 * Function Name : WDOG_DRV_WaitWctClose
 * Description   : Wait until the WCT is closed
 * This function is used wait until the WCT window is closed, WCT time is 256 bus cycles, here
 * use nop to wait timeout, one nop running time is one core cycle.
 *
 *END*********************************************************************/
static void WDOG_DRV_WaitWctClose(void)
{
    uint32_t count;
    /* here using nop instruction, otherwise empty code will be optimized in release target */
    for ( count = 0 ; count < wdogWctInstructionCount; count++ )
    {
        __NOP();
    }
}


/*FUNCTION****************************************************************
 *
 * Function Name : WDOG_DRV_Init
 * Description   : Initialize watchdog
 * This function is used to initialize the WDOG, after called, the WDOG 
 * will run immediately according to the configure.
 *
 *END*********************************************************************/
void WDOG_DRV_Init(const wdog_user_config_t* userConfigPtr)
{
    assert(userConfigPtr);

    wdog_common_config wdogCommonConfig;
    uint32_t coreClockHz, busClockHz;

    CLOCK_SYS_GetFreq(kCoreClock, &coreClockHz);
    CLOCK_SYS_GetFreq(kBusClock, &busClockHz);

    wdogWctInstructionCount = ((coreClockHz/busClockHz) << 8); /* WCT is 256 bus clock */

    wdogCommonConfig.U = 0x0U;
    wdogCommonConfig.commonConfig.workInWaitModeEnable = (uint8_t)userConfigPtr->workInWaitModeEnable;
    wdogCommonConfig.commonConfig.workInDebugModeEnable = (uint8_t)userConfigPtr->workInDebugModeEnable;
    wdogCommonConfig.commonConfig.workInStopModeEnable = (uint8_t)userConfigPtr->workInStopModeEnable;
    wdogCommonConfig.commonConfig.clockSource = (uint8_t)userConfigPtr->clockSource;
    wdogCommonConfig.commonConfig.interruptEnable = (uint8_t)false;
    wdogCommonConfig.commonConfig.windowModeEnable = (uint8_t)(0 != userConfigPtr->windowValue);
    wdogCommonConfig.commonConfig.updateRegisterEnable = (uint8_t)userConfigPtr->updateRegisterEnable; 
    wdogCommonConfig.commonConfig.wdogEnable = (uint8_t)(true);

    WDOG_DRV_Unlock();
    WDOG_HAL_SetTimeoutValue(g_wdogBaseAddr[0], userConfigPtr->timeoutValue);
    WDOG_HAL_SetWindowValue(g_wdogBaseAddr[0], userConfigPtr->windowValue);
    WDOG_HAL_SetClockPrescalerValueMode(g_wdogBaseAddr[0], userConfigPtr->clockPrescalerValue);
    WDOG_HAL_ClearIntFlag(g_wdogBaseAddr[0]);
    WDOG_HAL_SetCommonConfig(g_wdogBaseAddr[0], wdogCommonConfig);
    WDOG_DRV_WaitWctClose();

}

/*FUNCTION****************************************************************
 *
 * Function Name : WDOG_DRV_Deinit
 * Description   : Shutdown watchdog
 * This function is used to shutdown the WDOG.
 *
 *END*********************************************************************/
void WDOG_DRV_Deinit(void)
{
    WDOG_DRV_Unlock();
    WDOG_HAL_Disable(g_wdogBaseAddr[0]);
    WDOG_DRV_WaitWctClose();
}

/*FUNCTION****************************************************************
 *
 * Function Name : WDOG_DRV_IsRunning
 * Description   : Get watchdog running status
 * This function is used to get the WDOG running status.
 *
 *END*********************************************************************/
bool WDOG_DRV_IsRunning(void)
{
    return WDOG_HAL_IsEnabled(g_wdogBaseAddr[0]);
}

/*FUNCTION****************************************************************
 *
 * Function Name : WDOG_DRV_Refresh
 * Description   : Refresh watchdog.
 * This function is used to feed the WDOG, it will set the WDOG timer count to zero and 
 * should be called before watchdog timer is timeout, otherwise a RESET will assert.
 *
  *END*********************************************************************/
void WDOG_DRV_Refresh(void)
{
    INT_SYS_DisableIRQGlobal();

    WDOG_HAL_Refresh(g_wdogBaseAddr[0]);

    INT_SYS_EnableIRQGlobal();
}

/*FUNCTION****************************************************************
 *
 * Function Name : WDOG_DRV_ResetSystem
 * Description   : Reset chip by watchdog
 * This function is used to reset chip using WDOG. 
 *
 *END*********************************************************************/
void WDOG_DRV_ResetSystem(void)
{
    WDOG_HAL_ResetSystem(g_wdogBaseAddr[0]);
}

/*FUNCTION****************************************************************
 *
 * Function Name : WDOG_DRV_ClearResetCount
 * Description   : Clear watchdog reset count
 * This function is used to set the WDOG reset count to zero, the WDOG_RSTCNT
 * register will only clear on Power On Reset or clear by this function.
 *
 *END*********************************************************************/
void WDOG_DRV_ClearResetCount(void)
{
    WDOG_HAL_ClearResetCount(g_wdogBaseAddr[0]);
}

/*FUNCTION****************************************************************
 *
 * Function Name : WDOG_DRV_GetResetCount
 * Description   : Get chip reset count that reset by watchdog
 * This function is used to get the WDOG_RSTCNT value.
 *
 *END*********************************************************************/
uint32_t WDOG_DRV_GetResetCount(void)
{
    return WDOG_HAL_GetResetCount(g_wdogBaseAddr[0]);
}

/*******************************************************************************
 * EOF
 *******************************************************************************/

