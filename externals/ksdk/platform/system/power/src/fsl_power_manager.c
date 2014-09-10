/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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

#include "fsl_power_manager.h"
#include "fsl_smc_hal.h"
#include "fsl_pmc_hal.h"
#include "fsl_rcm_hal.h"
#include "fsl_os_abstraction.h"
#include <string.h>


/*******************************************************************************
 * Internal Variables
 ******************************************************************************/

/*! @brief Power manager internal structure. */
static power_manager_state_t gPowerManagerState;

/*! @brief Power manager internal structure lock. */
mutex_t gPowerManagerStateSync;

/*******************************************************************************
 * PROTOTYPES
 ******************************************************************************/

/*!
 * @brief Macros for power manager lock mechanism.
 *
 * Mutex is used when operating system is present otherwise critical section
 * (global interrupt disable).
 *
 */
#if (USE_RTOS)
    #define POWER_SYS_LOCK_INIT()    OSA_MutexCreate(&gPowerManagerStateSync)
    #define POWER_SYS_LOCK()         OSA_MutexLock(&gPowerManagerStateSync, OSA_WAIT_FOREVER)
    #define POWER_SYS_UNLOCK()       OSA_MutexUnlock(&gPowerManagerStateSync)
    #define POWER_SYS_LOCK_DEINIT()  OSA_MutexDestroy(&gPowerManagerStateSync)
#else
    #define POWER_SYS_LOCK_INIT()    do {}while(0)
    #define POWER_SYS_LOCK()         OSA_EnterCritical(kCriticalDisableInt)
    #define POWER_SYS_UNLOCK()       OSA_ExitCritical(kCriticalDisableInt)
    #define POWER_SYS_LOCK_DEINIT()  do {}while(0)
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : POWER_SYS_Init
 * Description   : Initializes the Power manager for operation.
 * This function initializes the Power manager and its run-time state structure.
 * Reference to an array of Power mode configuration structures has to be passed
 * as parameter along with parameter specifying its size. At least one power mode
 * configuration is required. Optionally, reference to array of predefined
 * call-backs can be passed with its size parameter. This can be used to pass
 * call-backs directly without need of dynamic, run-time registration (see
 * POWER_SYS_RegisterCallbackFunction() and POWER_SYS_UnregisterCallbackFunction()).
 * For details about static call-backs refer to the power_manager_static_callback_user_config_t.
 * As Power manager stores only references to array of these structures they have
 * to exist while Power manager is used.
 * It is expected that prior POWER_SYS_Init() call the write-once protection
 * register was configured appropriately allowing to enter all required low power
 * modes.
 * The following is an example of how to set up two power modes and three
 * call-backs and initialize the Power manager with structures containing their settings.
 * The example shows two possible ways how where the configuration structures can be stored
 * (ROM or RAM) although it is expected that they will be placed rather in the read-only
 * memory to save the RAM space. (Note: In the example it is assumed that the programmed chip
 * doesn't support any optional power options described in the power_manager_user_config_t)
 *
 *END**************************************************************************/
power_manager_error_code_t POWER_SYS_Init(power_manager_user_config_t * (* powerConfigsPtr)[],
                                          uint8_t configsNumber,
                                          power_manager_static_callback_user_config_t * (* callbacksPtr)[],
                                          uint8_t callbacksNumber)
{
    /* Check input parameter - at least one power mode configuration is required */
    if ((powerConfigsPtr == NULL) || (configsNumber == 0U))
    {
        return kPowerManagerError;
    }
    /* Initialize internal state structure lock */
    POWER_SYS_LOCK_INIT();
    POWER_SYS_LOCK();
    /* Initialize internal state structure */
    memset(&gPowerManagerState, 0, sizeof(power_manager_state_t));
    /* Store references to user-defined power mode configurations */
    gPowerManagerState.configs = powerConfigsPtr;
    gPowerManagerState.configsNumber = configsNumber;
    /* Store references to user-defined callback configurations and increment call-back handle counter */
    if (callbacksPtr != NULL)
    {
        gPowerManagerState.staticCallbacks = callbacksPtr;
        gPowerManagerState.staticCallbacksNumber = callbacksNumber;
        gPowerManagerState.callbacksHandleCounter = callbacksNumber;
    }
    POWER_SYS_UNLOCK();
    return kPowerManagerSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : POWER_SYS_Deinit
 * Description   : Deinitializes the Power manager.
 *
 *END**************************************************************************/
power_manager_error_code_t POWER_SYS_Deinit(void)
{
    /* Deinitialize internal state structure lock */
    POWER_SYS_LOCK_DEINIT();
    return kPowerManagerSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : POWER_SYS_SetMode
 * Description   : Configures the power mode.
 *
 * This function switches to one of the defined power mode. Requested mode number is passed
 * as an input parameter. This function notifies all registered call-back functions before
 * the mode change (using  kPowerManagerCallbackBefore set as call-back type parameter),
 * sets specific power options defined in the power mode configuration and enters the specified
 * mode. In case of run modes (i.e. Run, Very low power run or High speed run), this function
 * also invokes all registered call-backs after the mode change (using kPowerManagerCallbackAfter).
 * In case of sleep or deep sleep modes, if the requested mode  is not exited through
 * a reset then these notifications are sent after the core wakes up.
 * Call-backs are invoked in the following order: first, all dynamically registered call-backs
 * are invoked ordered by the priority set during their registrations (see
 * POWER_SYS_RegisterCallbackFunction). Then all statically registered call-backs are notified
 * ordered by index in the static call-backs array (see callbacksPtr parameter of POWER_SYS_Init()).
 * Same order is used for before and after switch notifications.
 * The notifications before the power mode switch can be used to obtain confirmation about
 * the change from registered call-backs. In case that any registered call-back denies the power
 * mode change then further execution of this function depends on mode change policy defined
 * in the user power mode configuration (see policy in the power_manager_user_config_t): the mode
 * change is either forced (kPowerManagerPolicyForcible) or exited (kPowerManagerPolicyAgreement).
 * When mode change is forced the result of the before switch notifications are ignored. If
 * agreement is required then if any call-back returns an error code then further notifications
 * before switch notifications are cancelled and all already notified call-backs are re-invoked
 * with kPowerManagerCallbackAfter set as call-back type parameter. The handler of call-back
 * which returned error code during pre-switch notifications is stored (any error codes during
 * call-backs re-invocation are ignored) and POWER_SYS_GetErroneousDriver() can be used to get it.
 * Regardless the policies, if any call-back returned error code, error code denoting in which phase
 * the error occurred is returned when POWER_SYS_SetMode() exits.
 * It is possible to enter any mode supported by the processor. Refer to the chip reference manual
 * for list of available power modes. If it is necessary to switch into intermediate power mode prior
 * entering requested mode (e.g. when switching from Run into Very low power wait through Very low
 * power run mode) then the intermediate mode is entered without invoking the call-back mechanism.
 *
 *END**************************************************************************/
power_manager_error_code_t POWER_SYS_SetMode(uint8_t powerModeIndex)
{
    power_manager_user_config_t * configPtr; /* Local pointer to the requested user-defined power mode configuration */
    power_manager_modes_t mode;         /* Local variable with requested power mode */
    smc_power_mode_config_t halModeConfig; /* SMC HAL layer configuration structure */
    power_manager_dynamic_callback_user_config_t * currentDynamicCallback; /* Pointer to node in linked list of dynamically registered call-backs */
    uint8_t currentStaticCallback = 0U;      /* Index to array of statically registered call-backs */
    power_manager_dynamic_callback_user_config_t * lastDynamicCallback = NULL; /* Pointer to call-back which will be serviced as last */
    uint8_t lastStaticCallback;         /* Index to call-back which will be serviced as last */
    gPowerManagerState.lastErroneousHandle = 0U; /* Default value of handle of last call-back that returned error */
    power_manager_error_code_t returnCode = kPowerManagerSuccess; /* Function return */

    POWER_SYS_LOCK();
    /* Requested power mode configuration availability check */
    if (powerModeIndex >= gPowerManagerState.configsNumber)
    {
        POWER_SYS_UNLOCK();
        return kPowerManagerErrorOutOfRange;
    }

    /* Initialization of local variables from the Power manager state structure */
    configPtr = (*gPowerManagerState.configs)[powerModeIndex];
    mode = configPtr->mode;

    /* Check that requested power mode is not protected */
    if ((mode == kPowerManagerVlpr) || (mode == kPowerManagerVlpw) || (mode == kPowerManagerVlps))
    {
        if (SMC_HAL_GetProtectionMode(SMC_BASE, kAllowVlp) == 0U)
        {
            POWER_SYS_UNLOCK();
            return kPowerManagerError;
        }
    }
    else if ((mode >= kPowerManagerLls) && (mode < kPowerManagerVlls0))
    {
        if (SMC_HAL_GetProtectionMode(SMC_BASE, kAllowLls) == 0U)
        {
            POWER_SYS_UNLOCK();
            return kPowerManagerError;
        }
    }
    else if (mode >= kPowerManagerVlls0)
    {
        if (SMC_HAL_GetProtectionMode(SMC_BASE, kAllowVlls) == 0U)
        {
            POWER_SYS_UNLOCK();
            return kPowerManagerError;
        }
    }

    /* From all dynamically registered call-backs... */
    for (currentDynamicCallback = gPowerManagerState.dynamicCallbacks; currentDynamicCallback != NULL; currentDynamicCallback = currentDynamicCallback->next)
    {
        /* ...notify only those which asked to be called before the power mode change */
        if ((uint32_t)(currentDynamicCallback->callbackType) & POWER_SYS_CALLBACK_BEFORE)
        {
            /* In case that call-back returned error code mark it, store the call-back handle and eventually cancel the mode switch */
            if (currentDynamicCallback->callback(kPowerManagerCallbackBefore, configPtr, currentDynamicCallback->callbackData) != kPowerManagerSuccess)
            {
                returnCode = kPowerManagerErrorNotificationBefore;
                gPowerManagerState.lastErroneousHandle = currentDynamicCallback->handle;
                /* If not forced power mode switch, call all already notified call-backs to revert their state as the mode change is canceled */
                if (configPtr->policy != kPowerManagerPolicyForcible)
                {
                    /* Only already notified dynamic call-backs will receive the "revert" callback */
                    lastDynamicCallback = currentDynamicCallback->next;
                    lastStaticCallback = 0U;
                    break;
                }
            }
        }
    }
    /* Continue only if dynamic call-backs were successfully notified or forced switch of mode is required */
    if ((configPtr->policy == kPowerManagerPolicyForcible) || (returnCode == kPowerManagerSuccess))
    {
        /* From all statically registered call-backs... */
        for (currentStaticCallback = 0U; currentStaticCallback < gPowerManagerState.staticCallbacksNumber; currentStaticCallback++)
        {
            /* ...notify only those which asked to be called before the power mode change */
            if ((uint32_t)((*gPowerManagerState.staticCallbacks)[currentStaticCallback]->callbackType) & POWER_SYS_CALLBACK_BEFORE)
            {
                /* In case that call-back returned error code mark it, store the call-back handle and eventually cancel the mode switch */
                if ((*gPowerManagerState.staticCallbacks)[currentStaticCallback]->callback(kPowerManagerCallbackBefore, configPtr, (*gPowerManagerState.staticCallbacks)[currentStaticCallback]->callbackData) != kPowerManagerSuccess)
                {
                    returnCode = kPowerManagerErrorNotificationBefore;
                    gPowerManagerState.lastErroneousHandle = currentStaticCallback + 1;
                    /* If not forcing power mode switch, call all already notified call-backs to revert their state as the mode change is canceled */
                    if (configPtr->policy != kPowerManagerPolicyForcible)
                    {
                        /* Only already notified static call-backs will receive the "revert" callback */
                        lastStaticCallback = currentStaticCallback + 1;
                        break;
                    }
                }
            }
        }
    }
    POWER_SYS_UNLOCK();

    /* Power mode switch */

    /* In case that any call-back returned error code and clock configuration policy doesn't force the mode switch go to after switch call-backs */
    if ((configPtr->policy == kPowerManagerPolicyForcible) || (returnCode == kPowerManagerSuccess))
    {
#if  FSL_FEATURE_SMC_HAS_LPWUI
        halModeConfig.lpwuiOption = configPtr->lowPowerWakeUpOnInterruptOption;
        halModeConfig.lpwuiOptionValue = configPtr->lowPowerWakeUpOnInterruptValue;
#endif

        /* Configure the HAL layer */
        switch (mode) {
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
        /* High speed run mode */
        case kPowerManagerHsrun:
            /* High speed run mode can be entered only from Run mode */
            if (SMC_HAL_GetStat(SMC_BASE) != kStatRun)
            {
                SMC_HAL_SetRunMode(SMC_BASE, kSmcRun);
                while (!PMC_HAL_GetRegulatorStatus(PMC_BASE)) {}
                while (SMC_HAL_GetStat(SMC_BASE) != kStatRun) {}
            }
            halModeConfig.powerModeName = kPowerModeHsrun;
            break;
#endif
        /* Run mode */
        case kPowerManagerRun:
            halModeConfig.powerModeName = kPowerModeRun;
            break;
        /* Very low power run mode */
        case kPowerManagerVlpr:
            halModeConfig.powerModeName = kPowerModeVlpr;
            break;
        /* Wait mode */
        case kPowerManagerWait:
            /* Wait mode can be entered only from Run mode */
            if (SMC_HAL_GetStat(SMC_BASE) != kStatRun)
            {
                SMC_HAL_SetRunMode(SMC_BASE, kSmcRun);
                while (!PMC_HAL_GetRegulatorStatus(PMC_BASE)) {}
                while (SMC_HAL_GetStat(SMC_BASE) != kStatRun) {}
            }
            halModeConfig.powerModeName = kPowerModeWait;
            break;
        /* Very low power wait mode */
        case kPowerManagerVlpw:
            /* Very low power wait mode can be netered only from Very low power run mode */
            if (SMC_HAL_GetStat(SMC_BASE) != kStatVlpr)
            {
                SMC_HAL_SetRunMode(SMC_BASE, kSmcVlpr);
                while (SMC_HAL_GetStat(SMC_BASE) != kStatVlpr) {}
            }
            halModeConfig.powerModeName = kPowerModeVlpw;
            break;
        /* Low leakage stop modes */
        case kPowerManagerLls:
            halModeConfig.powerModeName = kPowerModeLls;
            break;
#if FSL_FEATURE_SMC_HAS_LLS_SUBMODE
        case kPowerManagerLls2:
            halModeConfig.powerModeName = kPowerModeLls;
            halModeConfig.stopSubMode = kSmcStopSub2;
            break;
        case kPowerManagerLls3:
            halModeConfig.powerModeName = kPowerModeLls;
            halModeConfig.stopSubMode = kSmcStopSub3;
            break;
#endif
        /* Very low leakage stop modes */
        case kPowerManagerVlls0:
            halModeConfig.powerModeName = kPowerModeVlls;
            halModeConfig.stopSubMode = kSmcStopSub0;
#if FSL_FEATURE_SMC_HAS_PORPO
            /* Optionally setup the power-on-reset detect circuit in VLLS0 */
            halModeConfig.porOption = configPtr->powerOnResetDetectionOption;
            halModeConfig.porOptionValue = configPtr->powerOnResetDetectionValue;
#endif
            break;
        case kPowerManagerVlls1:
            halModeConfig.powerModeName = kPowerModeVlls;
            halModeConfig.stopSubMode = kSmcStopSub1;
            break;
        case kPowerManagerVlls2:
            halModeConfig.powerModeName = kPowerModeVlls;
            halModeConfig.stopSubMode = kSmcStopSub2;
#if FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION
            /* Optionally setup the RAM2 partition retention in VLLS2 */
            halModeConfig.ram2Option = configPtr->RAM2PartitionOption;
            halModeConfig.ram2OptionValue = configPtr->RAM2PartitionValue;
#endif
            break;
        case kPowerManagerVlls3:
            halModeConfig.powerModeName = kPowerModeVlls;
            halModeConfig.stopSubMode = kSmcStopSub3;
            break;
#if FSL_FEATURE_SMC_HAS_PSTOPO
        /* Partial stop modes */
        case kPowerManagerPstop1:
            halModeConfig.powerModeName = kPowerModeStop;
            halModeConfig.pstopOption = true;
            halModeConfig.pstopOptionValue = kSmcPstopStop1;
            break;
        case kPowerManagerPstop2:
            halModeConfig.powerModeName = kPowerModeStop;
            halModeConfig.pstopOption = true;
            halModeConfig.pstopOptionValue = kSmcPstopStop2;
            break;
#endif
        /* Stop mode */
        case kPowerManagerStop:
            halModeConfig.powerModeName = kPowerModeStop;
            /* Stop mode can be entered only from Run mode */
            if (SMC_HAL_GetStat(SMC_BASE) != kStatRun)
            {
                SMC_HAL_SetRunMode(SMC_BASE, kSmcRun);
                while (!PMC_HAL_GetRegulatorStatus(PMC_BASE)) {}
                while (SMC_HAL_GetStat(SMC_BASE) != kStatRun) {}
            }
            break;
        /* Very low power stop mode */
        case kPowerManagerVlps:
            halModeConfig.powerModeName = kPowerModeVlps;
            break;
        default:
            return kPowerManagerErrorSwitch;
        }

#if FSL_FEATURE_SMC_HAS_LPOPO
        if ((mode >= kPowerManagerLls) && (mode <= kPowerManagerVlls3))
        {
            /* Optionally setup the LPO operation in LLSx/VLLSx */
            halModeConfig.lpoOption = configPtr->lowPowerOscillatorOption;
            halModeConfig.lpoOptionValue = configPtr->lowPowerOscillatorValue;
        }
#endif

        /* Configure ARM core what to do after interrupt invoked in (deep) sleep state */
        if ((configPtr->sleepOnExitOption) && (mode >= kPowerManagerWait))
        {
            if (configPtr->sleepOnExitValue)
            {
                /* Go back to (deep) sleep state on ISR exit */
                SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
            }
            else
            {
                /* Do not re-enter (deep) sleep state on ISR exit */
                SCB->SCR &= ~(SCB_SCR_SLEEPONEXIT_Msk);
            }
        }

        /* Switch the mode */
        if (SMC_HAL_SetMode(SMC_BASE, &halModeConfig) != kSmcHalSuccess)
        {
            returnCode = kPowerManagerErrorSwitch;
        }

        /* Wait until new run mode is entered */
        if (mode == kPowerManagerRun)
        {
            while (!PMC_HAL_GetRegulatorStatus(PMC_BASE)) {}
            while (SMC_HAL_GetStat(SMC_BASE) != kStatRun) {}
        }
        else if (mode == kPowerManagerVlpr)
        {
            while (SMC_HAL_GetStat(SMC_BASE) != kStatVlpr) {}
        }

        /* End of successful switch */

        POWER_SYS_LOCK();
        /* Update current configuration index */
        gPowerManagerState.currentConfig = powerModeIndex;

        /* From all dynamically registered call-backs... */
        for (currentDynamicCallback = gPowerManagerState.dynamicCallbacks; currentDynamicCallback != NULL; currentDynamicCallback = currentDynamicCallback->next)
        {
            /* ...notify only those which asked to be called after the power mode change */
            if ((uint32_t)(currentDynamicCallback->callbackType) & POWER_SYS_CALLBACK_AFTER)
            {
                /* In case that call-back returned error code mark it and store the call-back handle */
                if (currentDynamicCallback->callback(kPowerManagerCallbackAfter, configPtr, currentDynamicCallback->callbackData) != kPowerManagerSuccess)
                {
                    returnCode = kPowerManagerErrorNotificationAfter;
                    gPowerManagerState.lastErroneousHandle = currentDynamicCallback->handle;
                }
            }
        }

        /* From all statically registered call-backs... */
        for (currentStaticCallback = 0U; currentStaticCallback < gPowerManagerState.staticCallbacksNumber; currentStaticCallback++)
        {
            /* ...notify only those which asked to be called after the power mode change */
            if ((uint32_t)((*gPowerManagerState.staticCallbacks)[currentStaticCallback]->callbackType) & POWER_SYS_CALLBACK_AFTER)
            {
                /* In case that call-back returned error code mark it and store the call-back handle */
                if ((*gPowerManagerState.staticCallbacks)[currentStaticCallback]->callback(kPowerManagerCallbackAfter, configPtr, (*gPowerManagerState.staticCallbacks)[currentStaticCallback]->callbackData) != kPowerManagerSuccess)
                {
                    returnCode = kPowerManagerErrorNotificationAfter;
                    gPowerManagerState.lastErroneousHandle = currentStaticCallback + 1;

                }
            }
        }
        POWER_SYS_UNLOCK();
    }
    else
    {
        /* End of unsuccessful switch */

        POWER_SYS_LOCK();
        /* Notify those call-backs called before notification failed... */
        for (currentDynamicCallback = gPowerManagerState.dynamicCallbacks; currentDynamicCallback != lastDynamicCallback; currentDynamicCallback = currentDynamicCallback->next)
        {
            if ((uint32_t)(currentDynamicCallback->callbackType) & POWER_SYS_CALLBACK_BEFORE)
            {
                /* Keep marked call-back which caused switch fail */
                currentDynamicCallback->callback(kPowerManagerCallbackFailed, configPtr, currentDynamicCallback->callbackData);
            }
        }
        for (currentStaticCallback = 0U; currentStaticCallback < lastStaticCallback; currentStaticCallback++)
        {
            if ((uint32_t)((*gPowerManagerState.staticCallbacks)[currentStaticCallback]->callbackType) & POWER_SYS_CALLBACK_BEFORE)
            {
                /* Keep marked call-back which caused switch fail */
                (*gPowerManagerState.staticCallbacks)[currentStaticCallback]->callback(kPowerManagerCallbackFailed, configPtr, (*gPowerManagerState.staticCallbacks)[currentStaticCallback]->callbackData);
            }
        }
        POWER_SYS_UNLOCK();

    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : POWER_SYS_GetMode
 * Description   : This function returns power mode set as the last one.
 *
 * This function returns index of power mode which was set using POWER_SYS_SetMode() as the last one.
 * If the power mode was entered although some of the registered call-back denied the mode change
 * or if any of the call-backs invoked after the entering/restoring run mode failed then the return
 * code of this function has kPowerManagerError value.
 * value.
 *
 *END**************************************************************************/
power_manager_error_code_t POWER_SYS_GetMode(uint8_t *powerModeIndexPtr)
{
    POWER_SYS_LOCK();
    /* Pass index of user-defined configuration structure of currently running power mode */
    *powerModeIndexPtr = gPowerManagerState.currentConfig;
    /* Return whether all call-backs executed without error */
    if (gPowerManagerState.lastErroneousHandle == 0U)
    {
        POWER_SYS_UNLOCK();
        return kPowerManagerSuccess;
    }
    else
    {
        POWER_SYS_UNLOCK();
        return kPowerManagerError;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : POWER_SYS_GetModeConfig
 * Description   : This function returns user configuration structure of power mode set as the last one.
 *
 * This function returns reference to configuration structure which was set using POWER_SYS_SetMode()
 * as the last one. If the current power mode was entered although some of the registered call-back denied
 * the mode change or if any of the call-backs invoked after the entering/restoring run mode failed then
 * the return code of this function has kPowerManagerError value.
 *
 *END**************************************************************************/
power_manager_error_code_t POWER_SYS_GetModeConfig(power_manager_user_config_t ** powerModePtr)
{
    POWER_SYS_LOCK();
    /* Pass reference to user-defined configuration structure of currently running power mode */
    *powerModePtr = (*gPowerManagerState.configs)[gPowerManagerState.currentConfig];
    /* Return whether all call-backs executed without error */
    if (gPowerManagerState.lastErroneousHandle == 0U)
    {
        POWER_SYS_UNLOCK();
        return kPowerManagerSuccess;
    }
    else
    {
        POWER_SYS_UNLOCK();
        return kPowerManagerError;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : POWER_SYS_GetRunningMode
 * Description   : Returns currently running power mode.
 *
 *END**************************************************************************/
power_manager_modes_t POWER_SYS_GetRunningMode(void)
{
    power_manager_modes_t retVal;
    switch (SMC_HAL_GetStat(SMC_BASE))
    {
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
        /* High speed run mode */
        case kStatHsrun:
            retVal = kPowerManagerHsrun;
            break;
#endif
        /* Run mode */
        case kStatRun:
            retVal = kPowerManagerRun;
            break;
        /* Very low power run mode */
        case kStatVlpr:
            retVal = kPowerManagerVlpr;
            break;
        /* This should never happen - core has to be in some run mode to execute code */
        default:
            retVal = kPowerManagerMax;
            break;
    }
    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : POWER_SYS_RegisterCallbackFunction
 * Description   : Returns the last failed notification callback.
 *
 * This function returns handle of the last call-back that failed during the power mode switch while
 * the last POWER_SYS_SetMode() was called. If the last POWER_SYS_SetMode() call ended successfully zero
 * value is returned. Returned call-back ID represents either the handle assigned to the call-back
 * during its dynamic registration (the handle return parameter of the POWER_SYS_RegisterCallbackFunction())
 * or, in case of statically registered call-backs, index in the array of static call-backs counted from 1
 * (e.g. static call-back at index 0 of the array has handle value 1, index 1 is represented by handle
 * value 2 etc.)
 *
 *END**************************************************************************/
power_manager_callback_handle_t POWER_SYS_GetErroneousDriver (void)
{
    return gPowerManagerState.lastErroneousHandle;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : POWER_SYS_RegisterCallbackFunction
 * Description   : Registers notification call-back.
 *
 * This function is used to dynamically register driver or application call-back function into
 * the Power manager. Call-backs are used during power mode switch to notify and receive acknowledge from
 * registered functions: all functions which were registered with kPowerManagerCallbackBefore or
 * kPowerManagerCallbackBeforeAfter call-back type (see power_manager_dynamic_callback_user_config_t and
 * power_manager_callback_type_t) are called before the power mode is switched. It is expected that these
 * call-backs will acknowledge readiness for the power mode change. If any call-back signals that it is not
 * ready for the change then subsequent mode change execution depends on the policy of the requested power mode
 * configuration structure (see power_manager_policy_t). When the power mode is switched (in case of run modes)
 * or exited without a reset (in case of sleep modes) then call-backs registered with kPowerManagerCallbackAfter
 * or kPowerManagerCallbackBeforeAfter are invoked. Registered call-back is invoked during each POWER_SYS_SetMode()
 * call. An order of invocation of call-backs is specified by priority number (see power_manager_callback_priority_t).
 * Call-backs with lower priority number are called first. In case of more call-back having the same priority
 * the first registered call-back is serviced as first (FIFO). When called, reference to
 * power_manager_callback_data_t registered with the call-back are passed to the call-back by the power manager
 * along with reference to power mode configuration reference to provide callback with all necessary
 * informations about power mode which is going to be entered.
 * Following successful registration a handle identifying the call-back is returned. This handle is used to
 * identify last unsuccessfully invoked call-back (see POWER_SYS_GetErroneousDriver()) and for call-back
 * un-registration (see POWER_SYS_UnregisterCallbackFunction).
 * See power_manager_callback_t for required call-back prototype.
 *
 *END**************************************************************************/
power_manager_error_code_t POWER_SYS_RegisterCallbackFunction(power_manager_dynamic_callback_user_config_t * callbackPtr,
                                                              power_manager_callback_handle_t * callbackHandlePtr)
{
    power_manager_dynamic_callback_user_config_t * currentCallback; /* Pointer to node in linked list of dynamically registered call-backs */
    power_manager_callback_priority_t priority = callbackPtr->callbackPriority; /* Local variable with priority of registered call-backs */

    POWER_SYS_LOCK();
    /* Any already registered call-back? */
    if (gPowerManagerState.dynamicCallbacks != NULL)
    {
        /* Go trough already registered call-backs... */
        for (currentCallback = gPowerManagerState.dynamicCallbacks; currentCallback != NULL; currentCallback = currentCallback->next)
        {
            /* ...ordered by their priority */
            if (currentCallback->callbackPriority > priority)
            {
                /* Add the new call-back */
                callbackPtr->next = currentCallback;
                callbackPtr->prev = currentCallback->prev;
                if (currentCallback->prev == NULL)
                {
                    /* If registered call-back is first in linked list (has highest priority) update the list head reference */
                    gPowerManagerState.dynamicCallbacks = callbackPtr;
                }
                else
                {
                    currentCallback->prev->next = callbackPtr;
                }
                currentCallback->prev = callbackPtr;
                break;
            }
            /* Add the call-back to the end of list (has the lowest priority) */
            if (currentCallback->next == NULL)
            {
                currentCallback->next = callbackPtr;
                callbackPtr->next = NULL;
                callbackPtr->prev = currentCallback;
            }
        }
    }
    /* This is first registered call-back */
    else
    {
        gPowerManagerState.dynamicCallbacks = callbackPtr;
        callbackPtr->next = NULL;
        callbackPtr->prev = NULL;
    }
    /* Increment call-backs counter */
    callbackPtr->handle = ++gPowerManagerState.callbacksHandleCounter;
    /* Increment number of registered call-backs */
    gPowerManagerState.dynamicCallbacksNumber++;
    /* Return handle of registered call-back */
    *callbackHandlePtr = gPowerManagerState.callbacksHandleCounter;
    POWER_SYS_UNLOCK();
    return kPowerManagerSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : POWER_SYS_UnregisterCallbackFunction
 * Description   : Unregisters specified notification call-back.
 *
 * This function is used to unregister call-back from the Power manager. After that, call-back is not
 * invoked anymore. Call-back to be removed is identified by handle received during its registration.
 * If no registered call-back matches the handle kPowerManagerErrorOutOfRange is returned.
 * Only dynamically registered call-backs can be unregistered. This function can't be used to
 * stop invocation of statically registered call-backs.
 *
 *END**************************************************************************/
power_manager_error_code_t POWER_SYS_UnregisterCallbackFunction(power_manager_callback_handle_t callbackHandle)
{
    power_manager_dynamic_callback_user_config_t * currentCallback; /* Pointer to node in linked list of dynamically registered call-backs */

    POWER_SYS_LOCK();
    /* Is the handle valid? */
    if ((callbackHandle <= gPowerManagerState.staticCallbacksNumber) || (callbackHandle > gPowerManagerState.callbacksHandleCounter))
    {
        /* Handle of static call-back or not yet given handle */
        return kPowerManagerErrorOutOfRange;
    }

    /* Go through all already registered call-backs... */
    for (currentCallback = gPowerManagerState.dynamicCallbacks; currentCallback != NULL; currentCallback = currentCallback->next)
    {
        /* ...find the referenced one... */
        if (currentCallback->handle == callbackHandle)
        {
            /* ...and remove it from linked list */
            if (currentCallback->next != NULL)
            {
                currentCallback->next->prev = currentCallback->prev;
            }
            if (currentCallback->prev != NULL)
            {
                currentCallback->prev->next = currentCallback->next;
            }
            else
            {
                /* If unregistered call-back is the first in list update the list head reference  */
                gPowerManagerState.dynamicCallbacks = currentCallback->next;
            }
            /* Decrement number of registered call-backs */
            gPowerManagerState.dynamicCallbacksNumber--;
            break;
        }
    }
    POWER_SYS_UNLOCK();
    /* Call-back to be removed not found? */
    if (currentCallback == NULL)
    {
        return kPowerManagerErrorOutOfRange;
    }
    return kPowerManagerSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : POWER_SYS_GetVeryLowPowerModeStatus
 * Description   : Returns whether very low power mode is running.
 *
 *END**************************************************************************/
bool POWER_SYS_GetVeryLowPowerModeStatus(void)
{
    /* Get current power mode and return true if it is very low power mode */
    uint8_t status = SMC_HAL_GetStat(SMC_BASE);
    if (status == kPowerModeVlpr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : POWER_SYS_GetVeryLowPowerModeStatus
 * Description   : Returns whether reset was caused by low leakage wake up.
 *
 *END**************************************************************************/
bool POWER_SYS_GetLowLeakageWakeupResetStatus(void)
{
    return RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmWakeup);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

