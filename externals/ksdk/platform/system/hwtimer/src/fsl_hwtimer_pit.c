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

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_hwtimer.h"
#include "fsl_hwtimer_pit.h"
#include "fsl_pit_hal.h"
#include "fsl_pit_features.h"
#include "fsl_pit_common.h"
#include "fsl_os_abstraction.h"
#include "fsl_interrupt_manager.h"

/*******************************************************************************
 * Internal type definition
 ******************************************************************************/

 #if defined (KL25Z4_SERIES)
/*!
 * @cond DOXYGEN_PRIVATE
 * @brief Table to save PIT IRQ enum numbers defined in CMSIS files.
 *
 * They are used by PIT_DRV_InitChannel to enable or disable PIT interrupts. This table is
 * indexed by channel number which could return PIT IRQ numbers.
 */
const static IRQn_Type kpitIrqIds[FSL_FEATURE_PIT_TIMER_COUNT] =
{
    PIT_IRQn, PIT_IRQn
};
#elif defined (K64F12_SERIES) || defined (K70F12_SERIES) || defined(K22F12810_SERIES) || \
      defined(K22F25612_SERIES) || defined(K22F51212_SERIES) || defined(KV31F12810_SERIES) || \
      defined(KV31F25612_SERIES) || defined(KV31F51212_SERIES) 
const static IRQn_Type kpitIrqIds[FSL_FEATURE_PIT_TIMER_COUNT] =
{
    PIT0_IRQn, PIT1_IRQn, PIT2_IRQn, PIT3_IRQn
};
#endif

/*******************************************************************************
 * Internal Variables
 ******************************************************************************/

static void HWTIMER_SYS_PitIsr(void);
static _hwtimer_error_code_t HWTIMER_SYS_PitInit(hwtimer_t *hwtimer, uint32_t pitId, uint32_t isrPrior, void *data);
static _hwtimer_error_code_t HWTIMER_SYS_PitDeinit(hwtimer_t *hwtimer);
static _hwtimer_error_code_t HWTIMER_SYS_PitSetDiv(hwtimer_t *hwtimer, uint32_t divider);
static _hwtimer_error_code_t HWTIMER_SYS_PitStart(hwtimer_t *hwtimer);
static _hwtimer_error_code_t HWTIMER_SYS_PitStop(hwtimer_t *hwtimer);
static _hwtimer_error_code_t HWTIMER_SYS_PitGetTime(hwtimer_t *hwtimer, hwtimer_time_t *time);


const hwtimer_devif_t kPitDevif =
{
    HWTIMER_SYS_PitInit,
    HWTIMER_SYS_PitDeinit,
    HWTIMER_SYS_PitSetDiv,
    HWTIMER_SYS_PitStart,
    HWTIMER_SYS_PitStop,
    HWTIMER_SYS_PitGetTime,
};

static hwtimer_t *g_hwtimersPit[FSL_FEATURE_PIT_TIMER_COUNT] = {0U};
static mutex_t g_lock_data; /* Lock used for g_hwtimersPit locking */
static mutex_t *g_lock = NULL; /* pointer to lock data */


 /*******************************************************************************
 * Internal Code
 ******************************************************************************/
/*!
 * @cond DOXYGEN_PRIVATE
 *
 * @brief Interrupt service routine.
 *
 * Checks whether callback_func is not NULL,
 * and unless callback is blocked by callback_blocked being non-zero it calls the callback function with callback_data as parameter,
 * otherwise callback_pending is set to non-zero value.
 *
 * @return void
 *
 * @see HWTIMER_SYS_Pitdeinit
 * @see HWTIMER_SYS_PitsetDiv
 * @see HWTIMER_SYS_Pitstart
 * @see HWTIMER_SYS_Pitstop
 * @see HWTIMER_SYS_PitgetTime
 * @see HWTIMER_SYS_PitisrShared
 */
static void HWTIMER_SYS_PitIsr(void)
{
    uint32_t baseAddr = g_pitBaseAddr[0];
    int i;
    for (i = 0U; i < FSL_FEATURE_PIT_TIMER_COUNT; i++)
    {
        hwtimer_t *hwtimer =  g_hwtimersPit[i];
        /* If hwtimer exist*/
        if (NULL != hwtimer)
        {
            uint32_t pitChannel =  hwtimer->llContext[0U];
            assert(pitChannel < FSL_FEATURE_PIT_TIMER_COUNT);

            /* Check if interrupt is enabled for this channel. Cancel spurious interrupt */
            if (!(BR_PIT_TCTRLn_TIE(baseAddr, pitChannel)))
            {
                continue;
            }

            /* If interrupt occurred for this pit and channel*/
            if(PIT_HAL_IsIntPending(baseAddr, pitChannel))
            {
                /* Clear interrupt flag */
                PIT_HAL_ClearIntFlag(baseAddr, pitChannel);
                /* Following part of function is typically the same for all low level hwtimer drivers */
                hwtimer->ticks++;

                if (NULL != hwtimer->callbackFunc)
                {
                    if (hwtimer->callbackBlocked)
                    {
                        hwtimer->callbackPending = 1U;
                    }
                    else
                    {
                        /* Run user function*/
                        hwtimer->callbackFunc(hwtimer->callbackData);
                    }
                }
            }
        }
    }
}


/*!
 * @cond DOXYGEN_PRIVATE
 *
 * @brief This function initializes caller allocated structure according to given
 * numerical identifier of the timer.
 *
 * Called by hwtimer_init().
 * Initializes the HWTIMER structure.
 * Sets interrupt priority and registers ISR.
 *
 * @param hwtimer[in]   Returns initialized hwtimer structure handle.
 * @param pitId[in]     Determines PIT module and pit channel.
 * @param isrPrior[in]  Interrupt priority for PIT
 * @param data[in]      Specific data. Not used in this timer.
 *
 * @return kHwtimerSuccess              Success.
 * @return kHwtimerInvalidInput         When channel number does not exist in pit module.
 * @return kHwtimerLockError            When Locking failed.
 * @return kHwtimerRegisterHandlerError When registration of the interrupt service routine failed.
 *
 * @see HWTIMER_SYS_PitDeinit
 * @see HWTIMER_SYS_PitSetDiv
 * @see HWTIMER_SYS_PitStart
 * @see HWTIMER_SYS_PitStop
 * @see HWTIMER_SYS_PitGetTime
 * @see HWTIMER_SYS_PitIsr
 * @see HWTIMER_SYS_PitIsrShared
 */
static _hwtimer_error_code_t HWTIMER_SYS_PitInit(hwtimer_t * hwtimer, uint32_t pitId, uint32_t isrPrior, void *data)
{
    uint32_t pitChannel;
    uint32_t baseAddr = g_pitBaseAddr[0];
    if (FSL_FEATURE_PIT_TIMER_COUNT < pitId)
    {
        return kHwtimerInvalidInput;
    }

    assert(NULL != hwtimer);

    /* We need to store pitId of timer in context struct */
    hwtimer->llContext[0U] = pitId;

    pitChannel = hwtimer->llContext[0U];

   /* Un-gate pit clock */
    CLOCK_SYS_EnablePitClock(0U);

    /* Enable PIT module clock */
    PIT_HAL_Enable(baseAddr);

    /* Allows the timers to be stopped when the device enters the Debug mode. */
    PIT_HAL_SetTimerRunInDebugCmd(baseAddr, false);

    /* Disable timer and interrupt */
    PIT_HAL_StopTimer(baseAddr, pitChannel);
    PIT_HAL_SetIntCmd(baseAddr, pitChannel, false);
    /* Clear any pending interrupt */
    PIT_HAL_ClearIntFlag(baseAddr, pitChannel);

    /* Create lock if does not exists*/
    /* Enter critical section to avoid interrupt create locking */
    OSA_EnterCritical(kCriticalDisableInt);
    if (g_lock == NULL)
    {
        /* Initialize synchronization object */
        if (kStatus_OSA_Success != OSA_MutexCreate(&g_lock_data))
        {
            return kHwtimerLockError;
        }
        g_lock = &g_lock_data;
    }
    OSA_ExitCritical(kCriticalDisableInt);
    assert(g_lock == &g_lock_data);

    /* Critical section. Access to global variable */
    if (kStatus_OSA_Success != OSA_MutexLock(g_lock, OSA_WAIT_FOREVER))
    {
        return kHwtimerLockError;
    }
    /* Store hwtimer in global array */
    g_hwtimersPit[pitChannel] = hwtimer;

    if (kStatus_OSA_Success != OSA_MutexUnlock(g_lock))
    {
        return kHwtimerLockError;
    }

    /* Enable PIT interrupt.*/
    if (kStatus_OSA_Success != OSA_InstallIntHandler(kpitIrqIds[pitChannel], HWTIMER_SYS_PitIsr))
    {
        return kHwtimerRegisterHandlerError;
    }

    PIT_HAL_SetIntCmd(baseAddr, pitChannel, true);
    INT_SYS_EnableIRQ(kpitIrqIds[pitChannel]);

    return kHwtimerSuccess;
}

/*!
 * @cond DOXYGEN_PRIVATE
 *
 * @brief Initialization of pit timer module
 *
 * Called by hwtimer_deinit. Disables the peripheral. Unregisters ISR.
 *
 * @param hwtimer[in] Pointer to hwtimer structure.
 *
 * @return kHwtimerSuccess              Success.
 * @return kHwtimerLockError            When Locking failed.
 * @return kHwtimerRegisterHandlerError When un-registration of the interrupt service routine failed.
 *
 * @see HWTIMER_SYS_PitInit
 * @see HWTIMER_SYS_PitSetDiv
 * @see HWTIMER_SYS_PitStart
 * @see HWTIMER_SYS_PitStop
 * @see HWTIMER_SYS_PitGetTime
 * @see HWTIMER_SYS_PitIsr
 * @see HWTIMER_SYS_PitIsrShared
 */
static _hwtimer_error_code_t HWTIMER_SYS_PitDeinit(hwtimer_t * hwtimer)
{
    /* We belive that if isr is shared ,than is shared for every chanells */
    uint32_t baseAddr = g_pitBaseAddr[0];
    uint32_t pitChannel;
    int i;

    assert(NULL != hwtimer);

    pitChannel = hwtimer->llContext[0U];
    assert(pitChannel < FSL_FEATURE_PIT_TIMER_COUNT);

    /* Remove Hwtimer from global array and disable interrupt on this channel */
    PIT_HAL_StopTimer(baseAddr, pitChannel);
    PIT_HAL_SetIntCmd(baseAddr, pitChannel, false);
    PIT_HAL_ClearIntFlag(baseAddr, pitChannel);

    /* Critical section. Access to global variable */
    if (kStatus_OSA_Success != OSA_MutexLock(g_lock, OSA_WAIT_FOREVER))
    {
        return kHwtimerLockError;
    }
    /* Pit can have shared interrupt vectors. We need un-register interrupt only when all hwtimers are de-inited(set to NULL) */
    g_hwtimersPit[pitChannel] = NULL;

    if (kStatus_OSA_Success != OSA_MutexUnlock(g_lock))
    {
        return kHwtimerLockError;
    }

    /* Check if this is last hwtimer in pit_hwtimers_array */
    for (i = 0U; i < FSL_FEATURE_PIT_TIMER_COUNT; i++)
    {
        if (NULL != g_hwtimersPit[i])
        {
            break;
        }
    }

    if (i == FSL_FEATURE_PIT_TIMER_COUNT)
    {
        if(kStatus_OSA_Success != OSA_InstallIntHandler(kpitIrqIds[pitChannel], NULL))
        {
            return kHwtimerRegisterHandlerError;
        }
    }

    /* Release lock if does not exists*/
    /* Enter critical section to avoid interrupt release locking */
    OSA_EnterCritical(kCriticalDisableInt);
    if (kStatus_OSA_Success != OSA_MutexUnlock(g_lock))
    {
        return kHwtimerLockError;
    }
    g_lock = NULL;
    OSA_ExitCritical(kCriticalDisableInt);


    return kHwtimerSuccess;
}

/*!
 * @cond DOXYGEN_PRIVATE
 *
 * @brief Sets up timer with divider settings closest to the requested total divider factor.
 *
 * Called by hwtimer_set_freq() and hwtimer_set_period().
 * Fills in the divider (actual total divider) and modulo (sub-tick resolution) members of the HWTIMER structure.
 *
 * @param hwtimer[in] Pointer to hwtimer structure.
 * @param divider[in] Value which divide input clock of pit timer module to obtain requested period of timer.
 *
 * @return kHwtimerSuccess                Success.
 *
 * @see HWTIMER_SYS_PitInit
 * @see HWTIMER_SYS_PitDeinit
 * @see HWTIMER_SYS_PitStart
 * @see HWTIMER_SYS_PitStop
 * @see HWTIMER_SYS_PitGetTime
 * @see HWTIMER_SYS_PitIsr
 * @see HWTIMER_SYS_PitIsrShared
 */
static _hwtimer_error_code_t HWTIMER_SYS_PitSetDiv(hwtimer_t * hwtimer, uint32_t divider)
{
    uint32_t pitChannel;
    uint32_t baseAddr = g_pitBaseAddr[0];
    assert(NULL != hwtimer);
    assert(0U != divider);

    pitChannel = hwtimer->llContext[0U];
    assert(pitChannel < FSL_FEATURE_PIT_TIMER_COUNT);

    /* Set divider for pit chanell */
    PIT_HAL_SetTimerPeriodByCount(baseAddr, pitChannel, divider - 1U);

    hwtimer->divider    = divider;
    hwtimer->modulo     = divider;

    return kHwtimerSuccess;
}


/*!
 * @cond DOXYGEN_PRIVATE
 *
 * @brief Start pit timer module
 *
 * This function enables the timer and leaves it running, timer is
 * periodically generating interrupts.
 *
 * @param hwtimer[in] Pointer to hwtimer structure.
 *
 * @return kHwtimerSuccess Success.
 *
 * @see HWTIMER_SYS_PitInit
 * @see HWTIMER_SYS_PitDeinit
 * @see HWTIMER_SYS_PitSetDiv
 * @see HWTIMER_SYS_PitStop
 * @see HWTIMER_SYS_PitGet_time
 * @see HWTIMER_SYS_PitIsr
 * @see HWTIMER_SYS_PitIsrShared
 */
static _hwtimer_error_code_t HWTIMER_SYS_PitStart(hwtimer_t * hwtimer)
{
    uint32_t pitChannel;
    uint32_t baseAddr = g_pitBaseAddr[0];
    assert(NULL != hwtimer);

    pitChannel = hwtimer->llContext[0U];
    assert(pitChannel < FSL_FEATURE_PIT_TIMER_COUNT);

    PIT_HAL_StopTimer(baseAddr, pitChannel);
    PIT_HAL_ClearIntFlag(baseAddr, pitChannel);
    PIT_HAL_SetIntCmd(baseAddr, pitChannel, true);
    PIT_HAL_StartTimer(baseAddr, pitChannel);

    return kHwtimerSuccess;
}

/*!
 * @cond DOXYGEN_PRIVATE
 *
 * @brief Stop pit timer module
 *
 * Disable timer and interrupt
 *
 * @param hwtimer[in] Pointer to hwtimer structure.
 *
 * @return kHwtimerSuccess Success.
 *
 * @see HWTIMER_SYS_PitInit
 * @see HWTIMER_SYS_PitDeinit
 * @see HWTIMER_SYS_PitSetDiv
 * @see HWTIMER_SYS_PitStart
 * @see HWTIMER_SYS_PitGetTime
 * @see HWTIMER_SYS_PitIsr
 * @see HWTIMER_SYS_PitIsrShared
 */
static _hwtimer_error_code_t HWTIMER_SYS_PitStop(hwtimer_t * hwtimer)
{
    uint32_t pitChannel;
    uint32_t baseAddr = g_pitBaseAddr[0];
    assert(NULL != hwtimer);

    pitChannel = hwtimer->llContext[0U];
    assert(pitChannel < FSL_FEATURE_PIT_TIMER_COUNT);

    /* Disable timer and interrupt */
    PIT_HAL_StopTimer(baseAddr, pitChannel);
    PIT_HAL_SetIntCmd(baseAddr, pitChannel, false);
    PIT_HAL_ClearIntFlag(baseAddr, pitChannel);

    return kHwtimerSuccess;
}

/*!
 * @cond DOXYGEN_PRIVATE
 *
 * @brief Atomically captures current time into HWTIMER_TIME_STRUCT structure
 *
 * Corrects/normalizes the values if necessary (interrupt pending, etc.)
 *
 * @param hwtimer[in] Pointer to hwtimer structure.
 * @param time[out]   Pointer to time structure. This value is filled with current value of the timer.
 *
 * @return kHwtimerSuccess Success.
 *
 * @see HWTIMER_SYS_PitInit
 * @see HWTIMER_SYS_PitDeinit
 * @see HWTIMER_SYS_PitSetDiv
 * @see HWTIMER_SYS_PitStart
 * @see HWTIMER_SYS_PitStop
 * @see HWTIMER_SYS_PitIsr
 * @see HWTIMER_SYS_PitIsrShared
 */
static _hwtimer_error_code_t HWTIMER_SYS_PitGetTime(hwtimer_t *hwtimer, hwtimer_time_t *time)
{
    uint32_t    pitChannel;
    uint32_t    tempCval;
    uint32_t baseAddr = g_pitBaseAddr[0];
    assert(NULL != hwtimer);
    assert(NULL != time);

    pitChannel = hwtimer->llContext[0U];
    assert(pitChannel < FSL_FEATURE_PIT_TIMER_COUNT);

    /* Enter critical section to avoid disabling interrupt from pit for very long time */
    OSA_EnterCritical(kCriticalDisableInt);
    PIT_HAL_SetIntCmd(baseAddr, pitChannel, false);

    time->ticks = hwtimer->ticks;

    tempCval = PIT_HAL_ReadTimerCount(baseAddr, pitChannel);
    /* Check pending interrupt flag */
   if (PIT_HAL_IsIntPending(baseAddr, pitChannel))
    {
        PIT_HAL_SetIntCmd(baseAddr, pitChannel, true);
        OSA_ExitCritical(kCriticalDisableInt);
        time->subTicks = hwtimer->modulo - 1U;
    }
    else
    {
        PIT_HAL_SetIntCmd(baseAddr, pitChannel, true);
        OSA_ExitCritical(kCriticalDisableInt);
        /* todo: following line should be updated when HAL will be updated with this functionality. */
        time->subTicks = HW_PIT_LDVALn_RD(baseAddr, pitChannel) - tempCval;
    }

    return kHwtimerSuccess;
}
 /*******************************************************************************
 * Code
 ******************************************************************************/
/*******************************************************************************
 * EOF
 ******************************************************************************/
