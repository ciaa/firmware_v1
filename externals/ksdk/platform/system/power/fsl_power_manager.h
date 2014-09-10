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

#ifndef __FSL_POWER_MANAGER_H__
#define __FSL_POWER_MANAGER_H__

#include <stdint.h>
#include <stdbool.h>

#include "fsl_smc_hal.h"

/*!
 * @addtogroup power_manager
 * @{
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Power modes enumeration.
 *
 * Defines power mode. Used in the power mode configuration structure
 * (power_manager_user_config_t). From ARM core perspective, Power modes
 * can be generally divided to run modes (High speed run, Run and
 * Very low power run), sleep (Wait and Very low power wait) and deep sleep modes
 * (all Stop modes).
 * List of power modes supported by specific chip along with requirements for entering
 * and exiting of these modes can be found in chip documentation.
 * List of all supported power modes:
 *  kPowerManagerHsrun - High speed run mode. Chip-specific.
 *  kPowerManagerRun - Run mode. All Kinetis chips.
 *  kPowerManagerVlpr - Very low power run mode. All Kinetis chips.
 *  kPowerManagerWait - Wait mode. All Kinetis chips.
 *  kPowerManagerVlpw - Very low power wait mode. All Kinetis chips.
 *  kPowerManagerStop - Stop mode. All Kinetis chips.
 *  kPowerManagerVlps - Very low power stop mode. All Kinetis chips.
 *  kPowerManagerPstop1 - Partial stop 1 mode. Chip-specific.
 *  kPowerManagerPstop2 - Partial stop 2 mode. Chip-specific.
 *  kPowerManagerLls - Low leakage stop mode. All Kinetis chips.
 *  kPowerManagerLls2 - Low leakage stop 2 mode. Chip-specific.
 *  kPowerManagerLls3 - Low leakage stop 3 mode. Chip-specific.
 *  kPowerManagerVlls0 - Very low leakage stop 0 mode. All Kinetis chips.
 *  kPowerManagerVlls1 - Very low leakage stop 1 mode. All Kinetis chips.
 *  kPowerManagerVlls2 - Very low leakage stop 2 mode. All Kinetis chips.
 *  kPowerManagerVlls3 - Very low leakage stop 3 mode. All Kinetis chips.
 */
typedef enum _power_manager_modes {
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
    kPowerManagerHsrun,
#endif
    kPowerManagerRun,
    kPowerManagerVlpr,
    kPowerManagerWait,
    kPowerManagerVlpw,
    kPowerManagerStop,
    kPowerManagerVlps,
#if FSL_FEATURE_SMC_HAS_PSTOPO
    kPowerManagerPstop1,
    kPowerManagerPstop2,
#endif
    kPowerManagerLls,
#if FSL_FEATURE_SMC_HAS_LLS_SUBMODE
    kPowerManagerLls2,
    kPowerManagerLls3,
#endif
    kPowerManagerVlls0,
    kPowerManagerVlls1,
    kPowerManagerVlls2,
    kPowerManagerVlls3,
    kPowerManagerMax
} power_manager_modes_t;

/*!
 * @brief Power manager success code and error codes.
 *
 * Used as return value of Power manager functions.
 */
typedef enum _power_manager_error_code {
    kPowerManagerSuccess,
    kPowerManagerError,
    kPowerManagerErrorNotAvailable,
    kPowerManagerErrorOutOfRange,
    kPowerManagerErrorSwitch,
    kPowerManagerErrorNotificationBefore,
    kPowerManagerErrorNotificationAfter
} power_manager_error_code_t;

/*!
 * @brief Power manager policies.
 *
 * Define whether the power mode change is forced or not. Used in the power mode
 * configuration structure (power_manager_user_config_t) to specify whether
 * the mode switch initiated by the POWER_SYS_SetMode() depends on the callback
 * notification results. For kPowerManagerPolicyForcible the power mode is changed
 * regardless of the results, while kPowerManagerPolicyAgreement policy is used
 * the POWER_SYS_SetMode() is exited when any of the callbacks returns error code.
 * See also POWER_SYS_SetMode() description.
 */
typedef enum _power_manager_policy {
    kPowerManagerPolicyAgreement,
    kPowerManagerPolicyForcible
} power_manager_policy_t;

/*!
 * @brief Internal constant.
 *
 * Used to identify callbacks invoked before the power mode change.
 * See power_manager_callback_type_t.
 */
#define POWER_SYS_CALLBACK_BEFORE 1

/*!
 * @brief Internal constant.
 *
 * Used to identify callbacks invoked after the power mode change.
 * See power_manager_callback_type_t.
 */
#define POWER_SYS_CALLBACK_AFTER  2

/*!
 * @brief Callback invocation types.
 *
 * Used in the callback configuration structures (power_manager_static_callback_user_config_t
 * and power_manager_dynamic_callback_user_config_t) to specify when the registered callback
 * will be called during power mode change initiated by POWER_SYS_SetMode(). Also used when 
 * callback is invoked to specify notification type. 
 * Callback can be invoked in following situations:
 *  - before the power mode change (Callback return value can affect POWER_SYS_SetMode()
 *    execution. Refer to the  POWER_SYS_SetMode() and power_manager_policy_t documentation).
 *  - after entering one of the run modes or after exiting from one of the (deep) sleep power
 *    modes back to the run mode.
 *  - after unsuccessful attempt to switch power mode
 * Values used during callback registration:
 *  - kPowerManagerCallbackBefore
 *  - kPowerManagerCallbackAfter
 *  - kPowerManagerCallbackBeforeAfter
 * Values used during callback invocation:
 *  - kPowerManagerCallbackBefore
 *  - kPowerManagerCallbackAfter
 *  - kPowerManagerCallbackFailed
 */
typedef enum _power_manager_callback_type {
    kPowerManagerCallbackNone,
    kPowerManagerCallbackBefore,
    kPowerManagerCallbackAfter,
    kPowerManagerCallbackBeforeAfter,
    kPowerManagerCallbackFailed,
} power_manager_callback_type_t;

/*!
 * @brief Callback invocation priority.
 *
 * Used in the dynamically registered callback configuration structure
 * (power_manager_dynamic_callback_user_config_t). Defines invocation order of the registered
 * callbacks (callbacks registered by the POWER_SYS_RegisterCallbackFunction()). Lower number
 * has higher priority. In cases of multiple callbacks registered with the same priority,
 * the registration order is decisive (first registered is first serviced).
 * Supported priorities are in range of 0-255, however, priorities in range 0-7 are reserved
 * for system purposes.
 * Statically registered callbacks (power_manager_static_callback_user_config_t) passed
 * during Power manager initialization (see POWER_SYS_Init()) have fixed priority and are
 * always serviced after the dynamically registered callbacks (as if they had priority 256).
 */
typedef uint8_t power_manager_callback_priority_t;

/*!
 * @brief Callback handle.
 *
 * Defines unique identification of registered callback. Used for last failed callback
 * identification (see POWER_SYS_GetErroneousDriver()) and callback registration
 * removal (see POWER_SYS_UnregisterCallbackFunction()).
 * For statically registered callbacks (see POWER_SYS_Init()) the handle is equal to callback
 * position in the array of registered callbacks counted from 1 (e.g. if two callbacks are
 * registered the callback with index 0 has handle 1 and callback with index 1 has handle 2).
 * For dynamically registered callbacks the handle is returned by POWER_SYS_RegisterCallbackFunction()
 * and is part of power_manager_dynamic_callback_user_config_t structure.
 * Handlers generation is reset after POWER_SYS_Init() call. Valid handle has non-zero,
 * 32-bit value.
 */
typedef uint32_t power_manager_callback_handle_t;

/*!
 * @brief Callback-specific data.
 *
 * Reference to data of this type is passed during callback registration. The reference is
 * part of the power_manager_static_callback_user_config_t or power_manager_dynamic_callback_user_config_t
 * structure and is passed to the callback during power mode change notifications (see POWER_SYS_SetMode()
 * and power_manager_callback_t).
 */
typedef void power_manager_callback_data_t;

/*!
 * @brief Power mode user configuration structure.
 *
 * This structure defines Kinetis power mode with additional power options and specifies
 * transition to and out of this mode. Application may define multiple power modes and
 * switch between them. List of defined power modes is passed to the Power manager during
 * initialization as an array of references to structures of this type (see POWER_SYS_Init()).
 * Power modes can be switched by calling POWER_SYS_SetMode() which accepts index to the list
 * of power modes passed during manager initialization. Currently used power mode can be
 * retrieved by calling POWER_SYS_GetMode(), which returns index of the current power mode, or
 * by POWER_SYS_GetModeConfig(), which returns reference to the structure of current mode.
 * List of power mode configuration structure members depends on power options available
 * for specific chip. Complete list contains:
 *  mode - Kinetis power mode. List of available modes is chip-specific. See power_manager_modes_t
 *   list of modes. This item is common for all Kinetis chips.
 *  policy - Power mode change policy. Specifies whether the callbacks notified before the
 *   power mode change can prohibit the switch or callback notification results are ignored forcing
 *   the switch. This item is common for all Kinetis chips.
 *  sleepOnExitOption - Controls whether the sleep-on-exit option value is used (when set to true)
 *   or ignored (when set to false). See sleepOnExitValue. This item is common for all Kinetis chips.
 *  sleepOnExitValue - When set to true, ARM core returns to sleep (Kientis wait modes) or deep sleep
 *   state (Kinetis stop modes) after interrupt service finishes. When set to false, core stays
 *   woken-up. This item is common for all Kinetis chips.
 *  lowPowerWakeUpOnInterruptOption - Controls whether the wake-up-on-interrupt option value is used
 *   (when set to true) or ignored (when set to false). See lowPowerWakeUpOnInterruptOption. This
 *   item is chip-specific.
 *  sleepOnExitValue - When set to true, system exits to Run mode when any interrupt occurs while in
 *   Very low power run, Very low power wait or Very low power stop mode. This item is chip-specific.
 *  powerOnResetDetectionOption - Controls whether the power on reset detection option value is used
 *   (when set to true) or ignored (when set to false). See powerOnResetDetectionOption. This item is
 *   chip-specific.
 *  powerOnResetDetectionValue - When set to true, power on reset detection circuit is enabled in
 *   Very low leakage stop 0 mode. When set to false, circuit is disabled. This item is chip-specific.
 *  RAM2PartitionOption - Controls whether the RAM2 partition power option value is used (when set to
 *   true) or ignored (when set to false). See RAM2PartitionValue. This item is chip-specific.
 *  RAM2PartitionValue - When set to true, RAM2 partition content is retained through Very low
 *   leakage stop 2 mode. When set to false, RAM2 partition power is disabled and memory content lost.
 *   This item is chip-specific.
 *  lowPowerOscillatorOption - Controls whether the Low power oscillator power option value is used
 *   (when set to true) or ignored (when set to false). See lowPowerOscillatorValue. This item is
 *   chip-specific.
 *  lowPowerOscillatorValue - When set to true, the 1 kHz Low power oscillator is enabled in any
 *   Low leakage or Very low leakage stop mode. When set to false, oscillator is disabled in these modes.
 *   This item is chip-specific.
 */
typedef struct power_manager_mode_user_config {
    power_manager_modes_t mode;
    power_manager_policy_t policy;
    bool sleepOnExitOption;
    bool sleepOnExitValue;
#if FSL_FEATURE_SMC_HAS_LPWUI
    bool lowPowerWakeUpOnInterruptOption;
    smc_lpwui_option_t lowPowerWakeUpOnInterruptValue;
#endif
#if FSL_FEATURE_SMC_HAS_PORPO
    bool powerOnResetDetectionOption;
    smc_por_option_t powerOnResetDetectionValue;
#endif
#if FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION
    bool RAM2PartitionOption;
    smc_ram2_option_t RAM2PartitionValue;
#endif
#if FSL_FEATURE_SMC_HAS_LPOPO
    bool lowPowerOscillatorOption;
    smc_lpo_option_t lowPowerOscillatorValue;
#endif
} power_manager_user_config_t;

/*!
 * @brief Callback prototype.
 *
 * Declaration of callback. It is common for statically and dynamically registered callbacks.
 * Reference to function of this type is part of power_manager_static_callback_user_config_t
 * and power_manager_dynamic_callback_user_config_t callback configuration structure.
 * Depending on callback type, function of this prototype is called during power mode change
 * (see POWER_SYS_SetMode()) before the mode change, after it or in both cases to notify about
 * the change progress (see power_manager_callback_type_t). When called, type of the notification
 * is passed as parameter along with reference to entered power mode configuration structure
 * (see power_manager_user_config_t) and any data passed during the callback registration (see
 * power_manager_callback_data_t).
 * When notified before the mode change, depending on the power mode change policy (see
 * power_manager_policy_t) the callback may deny the mode change by returning any error code different
 * from kPowerManagerSuccess (see POWER_SYS_SetMode()).
 * @param type Notification type. Denotes whether the callback is invoked before the power mode change
 *  (then the type has value kPowerManagerCallbackBefore) or after successful or unsuccessful mode change
 *  (value kPowerManagerCallbackAfter).
 * @param configPtr Entered power mode. Refers to the power mode configuration structure which contains
 *  settings of now entered (when type is kPowerManagerCallbackBefore) or exited mode (when type is
 *  kPowerManagerCallbackAftrer).
 * @param dataPtr Callback data. Refers to the data passed during callback registration. Intended to
 *  pass any driver or application data such as internal state information.
 * @return An error code or kPowerManagerSuccess.
 */
typedef power_manager_error_code_t (* power_manager_callback_t)(
    power_manager_callback_type_t type,
    power_manager_user_config_t * configPtr,
    power_manager_callback_data_t * dataPtr
);

/*!
 * @brief Statically allocated callback
 *
 * This structure holds configuration of callbacks passed
 * to the Power manager during its initialization.
 * Callbacks of this type are expected to be statically
 * allocated.
 * This structure contains following application-defined data:
 *  callback - pointer to the callback function
 *  callbackType - specifies when the callback is called
 *  callbackData - pointer to the data passed to the callback
 */
typedef struct _power_manager_static_callback_user_config {
    power_manager_callback_t callback;
    power_manager_callback_type_t callbackType;
    power_manager_callback_data_t * callbackData;
} power_manager_static_callback_user_config_t;

/*!
 * @brief Dynamically allocated callback
 *
 * This structure holds configuration of callbacks
 * registered to the Power manager during its run-time through
 * POWER_SYS_RegisterCallbackFunction().
 * Dynamically registered callbacks are managed as a double
 * linked list where link order is handled by the Power manager.
 * This structure contains both application-defined data and
 * data handled by the Power manager:
 *  callback - pointer to the callback function, defined by the application
 *  callbackType - specifies when the callback is called, defined by the application
 *  callbackData - pointer to the data passed to the callback, defined by the application
 *  callbackPriority - specifies order in which are callbacks called, defined by the application
 *  handle - callback ID, defined by the Power manager
 *  prev - previous list node reference, defined by the Power manager
 *  next - next list node reference, defined by the Power manager
 */
typedef struct _power_manager_dynamic_callback_user_config {
    power_manager_callback_t callback;
    power_manager_callback_type_t callbackType;
    power_manager_callback_data_t * callbackData;
    power_manager_callback_priority_t callbackPriority;
    power_manager_callback_handle_t handle;
    struct _power_manager_dynamic_callback_user_config * prev;
    struct _power_manager_dynamic_callback_user_config * next;
} power_manager_dynamic_callback_user_config_t;

/*!
 * @brief Power manager internal state structure.
 *
 * Power manager internal structure. Contains data necessary for Power manager proper
 * function. Stores references to registered power mode configurations,
 * statically and dynamically registered callbacks, information about their numbers
 * and other internal data.
 * This structure is statically allocated and initialized after POWER_SYS_Init() call.
 * It contains:
 *  configs - Reference to array of power mode configuration structures. Filled once
 *   during POWER_SYS_Init() call.
 *  staticCallbacks - Reference to array of statically registered callbacks. Filled
 *   once during POWER_SYS_Init() call.
 *  dynamicCallbacks - Reference to linked list of dynamically registered callbacks.
 *   Accessed during POWER_SYS_RegisterCallbackFunction() and POWER_SYS_UnregisterCallbackFunction()
 *   calls.
 *  callbacksHandleCounter - Internal generator of callback handles.
 *  lastErroneousHandle - Stores handle of last callback which failed during notification
 *   call while running POWER_SYS_SetMode().
 *  configsNumber - Number of power mode configuration passed during Power manager initialization.
 *   Size of array reference by configs.
 *  staticCallbacksNumber - Number of statically registered callbacks passed during Power manager
 *   initialization. Size of array reference by staticCallbacks.
 *  dynamicCallbacksNumber - Number of dynamically registered callbacks. Size of linked list
 *   referenced by dynamicCallbacks.
 *  currentConfig - POwer mode set as the last one. Index to the array referenced by configs.
 */
typedef struct _power_manager_state {
    power_manager_user_config_t * (* configs)[];
    power_manager_static_callback_user_config_t * (* staticCallbacks)[];
    power_manager_dynamic_callback_user_config_t * dynamicCallbacks;
    power_manager_callback_handle_t callbacksHandleCounter;
    power_manager_callback_handle_t lastErroneousHandle;
    uint8_t configsNumber;
    uint8_t staticCallbacksNumber;
    uint8_t dynamicCallbacksNumber;
    uint8_t currentConfig;
} power_manager_state_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Power manager initialization for operation.
 *
 * This function initializes the Power manager and its run-time state structure.
 * Reference to an array of Power mode configuration structures has to be passed
 * as a parameter along with a parameter specifying its size. At least one power mode
 * configuration is required. Optionally, reference to the array of predefined
 * callbacks can be passed with its size parameter. This can be used to pass
 * callbacks directly without need of dynamic, run-time registration (see
 * POWER_SYS_RegisterCallbackFunction() and POWER_SYS_UnregisterCallbackFunction()).
 * For details about static callbacks, refer to the power_manager_static_callback_user_config_t.
 * As Power manager stores only references to array of these structures, they have
 * to exist while Power manager is used.
 * It is expected that prior to the POWER_SYS_Init() call the write-once protection
 * register was configured appropriately allowing for entry to all required low power
 * modes.
 * The following is an example of how to set up two power modes and three
 * callbacks, and initialize the Power manager with structures containing their settings.
 * The example shows two possible ways the configuration structures can be stored
 * (ROM or RAM), although it is expected that they will be placed in the read-only
 * memory to save the RAM space. (Note: In the example it is assumed that the programmed chip
 * doesn't support any optional power options described in the power_manager_user_config_t)
 * :
 * @code
    const power_manager_user_config_t waitConfig = {
        kPowerManagerVlpw,
        kPowerManagerPolicyForcible,
        true,
        true,
    };

    const power_manager_static_callback_user_config_t callbackCfg0 = {
        callback0,
        kPowerManagerCallbackBefore,
        &callback_data0
    };

    const power_manager_static_callback_user_config_t callbackCfg1 = {
        callback1,
        kPowerManagerCallbackAfter,
        &callback_data1
    };

    const power_manager_static_callback_user_config_t callbackCfg2 = {
        callback2,
        kPowerManagerCallbackBeforeAfter,
        &callback_data2
    };

    const power_manager_static_callback_user_config_t * const callbacks[] = {&callbackCfg0, &callbackCfg1, &callbackCfg2};

    void main(void)
    {
        power_manager_user_config_t idleConfig;
        power_manager_user_config_t *powerConfigs[POWER_SYS_CONFIGURATIONS] = {&idleConfig, &idleConfig};

        idleConfig.mode = kPowerManagerVlps;
        idleConfig.policy = kPowerManagerPolicyForcible;
        idleConfig.sleepOnExitOption = true;
        idleConfig.sleepOnExitValue = false;

        POWER_SYS_Init(&powerConfigs, 2U, &callbacks, 3U);

        POWER_SYS_SetMode(0U);

    }
 * @endcode
 *
 * @param powerConfigsPtr A pointer to an array with references to all power
 *  configurations which will be handled by Power manager.
 * @param configsNumber Number of power configurations. Size of powerConfigsPtr
 *  array.
 * @param callbacksPtr A pointer to an array with references to callback configurations.
 *  These callbacks are statically registered in the Power manager. If there are
 *  no callbacks to register during Power manager initialization, use NULL value.
 * @param callbacksNumber Number of statically registered callbacks. Size of callbacksPtr
 *  array.
 * @return An error code or kPowerManagerSuccess.
 */
power_manager_error_code_t POWER_SYS_Init(power_manager_user_config_t * (* powerConfigsPtr)[],
                                          uint8_t configsNumber,
                                          power_manager_static_callback_user_config_t * (* callbacksPtr)[],
                                          uint8_t callbacksNumber);

/*!
 * @brief This function deinitializes the Power manager.
 *
 * @return An error code or kPowerManagerSuccess.
 */
power_manager_error_code_t POWER_SYS_Deinit(void);

/*!
 * @brief This function configures the power mode.
 *
 * This function switches to one of the defined power modes. Requested mode number is passed
 * as an input parameter. This function notifies all registered callback functions before
 * the mode change (using  kPowerManagerCallbackBefore set as callback type parameter),
 * sets specific power options defined in the power mode configuration and enters the specified
 * mode. In case of run modes (for example, Run, Very low power run, or High speed run), this function
 * also invokes all registered callbacks after the mode change (using kPowerManagerCallbackAfter).
 * In case of sleep or deep sleep modes, if the requested mode is not exited through
 * a reset, these notifications are sent after the core wakes up.
 * Callbacks are invoked in the following order: first, all dynamically registered callbacks
 * are invoked ordered by the priority set during their registrations (see
 * POWER_SYS_RegisterCallbackFunction). Then all statically registered callbacks are notified
 * ordered by index in the static callbacks array (see callbacksPtr parameter of POWER_SYS_Init()).
 * The same order is used for before and after switch notifications.
 * The notifications before the power mode switch can be used to obtain confirmation about
 * the change from registered callbacks. If any registered callback denies the power
 * mode change, further execution of this function depends on mode change policy defined
 * in the user power mode configuration (see policy in the power_manager_user_config_t): the mode
 * change is either forced (kPowerManagerPolicyForcible) or exited (kPowerManagerPolicyAgreement).
 * When mode change is forced, the result of the before switch notifications are ignored. If
 * agreement is required, if any callback returns an error code then further notifications
 * before switch notifications are cancelled and all already notified callbacks are re-invoked
 * with kPowerManagerCallbackAfter set as callback type parameter. The handler of the callback
 * which returned error code during pre-switch notifications is stored (any error codes during
 * callbacks re-invocation are ignored) and POWER_SYS_GetErroneousDriver() can be used to get it.
 * Regardless of the policies, if any callback returned an error code, an error code denoting in which phase
 * the error occurred is returned when POWER_SYS_SetMode() exits.
 * It is possible to enter any mode supported by the processor. Refer to the chip reference manual
 * for list of available power modes. If it is necessary to switch into intermediate power mode prior to
 * entering requested mode (for example, when switching from Run into Very low power wait through Very low
 * power run mode), then the intermediate mode is entered without invoking the callback mechanism.
 *
 * @param powerkModeIndex Requested power mode represented as an index into
 *  array of user-defined power mode configurations passed to the POWER_SYS_Init().
 * @return An error code or kPowerManagerSuccess.
 */
power_manager_error_code_t POWER_SYS_SetMode(uint8_t powerModeIndex);

/*!
 * @brief This function returns power mode set as the last one.
 *
 * This function returns index of power mode which was set using POWER_SYS_SetMode() as the last one.
 * If the power mode was entered even though some of the registered callback denied the mode change,
 * or if any of the callbacks invoked after the entering/restoring run mode failed, then the return
 * code of this function has kPowerManagerError value.
 *
 * @param powerkModeIndex Power mode which has been set represented as an index into array of power mode
 * configurations passed to the POWER_SYS_Init().
 * @return An error code or kPowerManagerSuccess.
 */
power_manager_error_code_t POWER_SYS_GetMode(uint8_t *powerModeIndexPtr);

/*!
 * @brief This function returns user configuration structure of power mode set as the last one.
 *
 * This function returns reference to configuration structure which was set using POWER_SYS_SetMode()
 * as the last one. If the current power mode was entered even though some of the registered callback denied
 * the mode change, or if any of the callbacks invoked after the entering/restoring run mode failed, then
 * the return code of this function has kPowerManagerError value.
 *
 * @param powerModeIndexPtr Pointer to power mode configuration structure of power mode set as last one.
 * @return An error code or kPowerManagerSuccess.
 */
power_manager_error_code_t POWER_SYS_GetModeConfig(power_manager_user_config_t ** powerModePtr);

/*!
 * @brief This function returns currently running power mode.
 *
 * This function reads hardware settings and returns currently running power mode. Generally,
 * this function can return only kPowerManagerRun, kPowerManagerVlpr or kPowerManagerHsrun value.
 *
  * @return Currently used run power mode.
 */
power_manager_modes_t POWER_SYS_GetRunningMode(void);

/*!
 * @brief This function returns the last failed notification callback.
 *
 * This function returns handle of the last callback that failed during the power mode switch while
 * the last POWER_SYS_SetMode() was called. If the last POWER_SYS_SetMode() call ended successfully, zero
 * value is returned. Returned callback ID represents either the handle assigned to the callback
 * during its dynamic registration (the handle return parameter of the POWER_SYS_RegisterCallbackFunction())
 * or, in cases of statically registered callbacks, index in the array of static callbacks counted from 1
 * (for example, static callback at index 0 of the array has handle value 1, index 1 is represented by handle
 * value 2 etc.)
 *
 * @return Zero or callback function handle.
 */
power_manager_callback_handle_t POWER_SYS_GetErroneousDriver(void);

/*!
 * @brief This function registers notification callback.
 *
 * This function is used to dynamically register driver or application callback function into
 * the Power manager. Callbacks are used during power mode switch to notify and receive acknowledge from
 * registered functions: all functions which were registered with kPowerManagerCallbackBefore or
 * kPowerManagerCallbackBeforeAfter callback type (see power_manager_dynamic_callback_user_config_t and
 * power_manager_callback_type_t) are called before the power mode is switched. It is expected that these
 * callbacks will acknowledge readiness for the power mode change. If any callback signals that it is not
 * ready for the change, then subsequent mode change execution depends on the policy of the requested power mode
 * configuration structure (see power_manager_policy_t). When the power mode is switched (in cases of run modes)
 * or exited without a reset (in cases of sleep modes) then callbacks registered with kPowerManagerCallbackAfter
 * or kPowerManagerCallbackBeforeAfter are invoked. Registered callback is invoked during each POWER_SYS_SetMode()
 * call. An order of invocation of callbacks is specified by priority number (see power_manager_callback_priority_t).
 * Callbacks with lower priority number are called first. In cases of multiple callbacks having the same priority,
 * the first registered callback is serviced as first (FIFO). When called, reference to
 * power_manager_callback_data_t registered with the callback are passed to the callback by the power manager,
 * along with reference to power mode configuration reference to provide callback with all necessary
 * information about power mode which being entered.
 * Following successful registration, a handle identifying the callback is returned. This handle is used to
 * identify last unsuccessfully invoked callback (see POWER_SYS_GetErroneousDriver()) and for callback
 * unregistration (see POWER_SYS_UnregisterCallbackFunction).
 * See power_manager_callback_t for required callback prototype.
 * Following is an example of usage:
 * @code
    struct {

      //...

    } callbackData;

    power_manager_error_code_t callback(power_manager_callback_type_t type,
                                        power_manager_user_config_t *config,
                                        power_manager_callback_data_t *dataPtr)
    {
        switch (type) {
        case kPowerManagerCallbackBefore:
            if (config->mode <= kPowerManagerVlpr)
            {
                // Run modes
                //...
            }
            else
            {
                // Sleep/deep sleep modes
                //...
            }
            break;
        case kPowerManagerCallbackAfter:
            if (POWER_SYS_GetRunningMode() == kPowerManagerRun)
            {
                //...
            }
            else
            {
                //...
            }
            break;
        }
        return kPowerManagerSuccess;
    }

    void main (void) {
        power_manager_error_code_t registrationStatus;
        power_manager_callback_handle_t callbackHandle;
        power_manager_dynamic_callback_user_config_t callbackDescriptor = {
          &callback,
          kPowerManagerCallbackBeforeAfter,
          &callbackData,
          255,
          0,
          NULL,
          NULL
        };

        //Power manager initialization
        //...

        registrationStatus = POWER_SYS_RegisterCallbackFunction(&callbackDescriptor, &callbackHandle);
    }
 * @endcode
 * @param callbackPtr Structure with callback configuration. Used to specify callback address
 *  (power_manager_callback_t callback), type (power_manager_callback_type_t callbackType), data
 *  (power_manager_callback_data_t callbackData) and priority (power_manager_callback_priority_t callbackPriority).
 *  The rest of the structure content is used by the Power manager: the unique handle assigned to the callback and
 *  linked list references.
 *  Creation of this structure, including memory allocation, is managed by the application; only reference
 *  is stored in the Power manager therefore it has to be available during Power manager usage. If callback
 *  and its configuration structure has to be destroyed/deallocated it has to be unregistered from Power manager
 *  first.
 * @param callbackHandlePtr Output parameter to return unique handle of registered callback.
 *  Only non-zero values are valid.
 * @return An error code or kPowerManagerSuccess.
 */
power_manager_error_code_t POWER_SYS_RegisterCallbackFunction(power_manager_dynamic_callback_user_config_t * callbackPtr,
                                                              power_manager_callback_handle_t * callbackHandlePtr);

/*!
 * @brief This function unregisters specified notification callback.
 *
 * This function is used to unregister callback from the Power manager. After that, callback is no longer
 * invoked. Callback to be removed is identified by handle received during its registration.
 * If no registered callback matches the handle, kPowerManagerOutOfRangeError is returned.
 * Only dynamically registered callbacks can be unregistered. This function can't be used to
 * stop invocation of statically registered callbacks.
 *
 * @param callbackHandle Handle identifying callback to be removed.
 * @return An error code or kPowerManagerSuccess.
 */
power_manager_error_code_t POWER_SYS_UnregisterCallbackFunction(power_manager_callback_handle_t callbackHandle);

/*!
 * @brief This function returns whether very low power mode is running.
 *
 * This function is used to detect whether very low power mode is running.
 *
 * @return Returns true if processor runs in very low power mode, otherwise false.
 */
bool POWER_SYS_GetVeryLowPowerModeStatus(void);

/*!
 * @brief This function returns whether reset was caused by low leakage wake up.
 *
 * This function is used to check that processor exited low leakage power mode
 * through reset.
 *
 * @return Returns true if processor was reset by low leakage wake up,
 *  otherwise false.
 */
bool POWER_SYS_GetLowLeakageWakeupResetStatus(void);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_POWER_MANAGER_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/

