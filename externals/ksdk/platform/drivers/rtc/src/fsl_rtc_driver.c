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
#include <string.h>
#include "fsl_rtc_driver.h"
#include "fsl_rtc_common.h"
#include "fsl_clock_manager.h"

/*!
 * @addtogroup rtc_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! Stores state of the RTC alarm when repeated periodically */
static rtc_repeat_alarm_state_t *s_rtcRepeatAlarmState = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_Init
 * Description   : initializes the Real Time Clock module
 * This function will initialize the Real Time Clock module.
 *
 *END**************************************************************************/

void RTC_DRV_Init(uint32_t instance)
{
    uint32_t rtcBaseAddr = g_rtcBaseAddr[instance];

    /* Enable clock gate to RTC module */
    CLOCK_SYS_EnableRtcClock( 0U);

    /* Initialize the general configuration for RTC module.*/
    RTC_HAL_Init(rtcBaseAddr);
    RTC_HAL_Enable(rtcBaseAddr);

    NVIC_ClearPendingIRQ(g_rtcIrqId[instance]);
    NVIC_ClearPendingIRQ(g_rtcSecondsIrqId[instance]);
    INT_SYS_EnableIRQ(g_rtcIrqId[instance]);
    INT_SYS_EnableIRQ(g_rtcSecondsIrqId[instance]);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_Deinit
 * Description   : Disable RTC module clock gate control
 * This function will disable clock gate to RTC module.
 *
 *END**************************************************************************/
void RTC_DRV_Deinit(uint32_t instance)
{
    /* Disable RTC interrupts.*/
    INT_SYS_DisableIRQ(g_rtcIrqId[instance]);
    INT_SYS_DisableIRQ(g_rtcSecondsIrqId[instance]);

    /* Disable the RTC counter */
    RTC_HAL_Disable(g_rtcBaseAddr[instance]);

    /* Disable clock gate to RTC module */
    CLOCK_SYS_DisableRtcClock( 0U);
    s_rtcRepeatAlarmState = NULL;
}

bool RTC_DRV_IsCounterEnabled(uint32_t instance)
{
    return RTC_HAL_IsCounterEnabled(g_rtcBaseAddr[instance]);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_SetDatetime
 * Description   : sets the RTC date and time according to the given time struct.
 * This function will set the RTC date and time according to the given time
 * struct, if start_after_set is true, the RTC oscillator will be enabled and
 * the counter will start.
 *
 *END**************************************************************************/
bool RTC_DRV_SetDatetime(uint32_t instance, rtc_datetime_t *datetime)
{
    assert(datetime);

    /* Return error if the time provided is not valid */
    if (!(RTC_HAL_IsDatetimeCorrectFormat(datetime)))
    {
        return false;
    }

    RTC_HAL_SetDatetime(g_rtcBaseAddr[instance], datetime);

    return true;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetDatetime
 * Description   : gets the actual RTC time and stores it in the given time struct.
 * This function will get the actual RTC time and stores it in the given time
 * struct.
 *
 *END**************************************************************************/
void RTC_DRV_GetDatetime(uint32_t instance, rtc_datetime_t *datetime)
{
    assert(datetime);

    RTC_HAL_GetDatetime(g_rtcBaseAddr[instance], datetime);
}

void RTC_DRV_SetSecsIntCmd(uint32_t instance, bool secondsEnable)
{
    RTC_HAL_SetSecsIntCmd(g_rtcBaseAddr[instance], secondsEnable);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_SetAlarm
 * Description   : sets the RTC alarm.
 * This function will first check if the date time has correct format. If yes,
 * convert the date time to seconds, and set the alarm in seconds.
 *
 *END**************************************************************************/
bool RTC_DRV_SetAlarm(uint32_t instance, rtc_datetime_t *alarmTime, bool enableAlarmInterrupt)
{
    assert(alarmTime);

    bool ret = false;
    uint32_t rtcBaseAddr = g_rtcBaseAddr[instance];

    /* Return error if the alarm time provided is not valid */
    if (!(RTC_HAL_IsDatetimeCorrectFormat(alarmTime)))
    {
        return ret;
    }

    ret = RTC_HAL_SetAlarm(rtcBaseAddr, alarmTime);

    if (ret)
    {
        /* Activate the Alarm interrupt if user wishes to use interrupts */
        RTC_HAL_SetAlarmIntCmd(rtcBaseAddr, enableAlarmInterrupt);
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : rtc_get_alarm
 * Description   : returns the RTC alarm time.
 * This function will first get alarm time in seconds, then convert the seconds to
 * date time.
 *
 *END**************************************************************************/
void RTC_DRV_GetAlarm(uint32_t instance, rtc_datetime_t *date)
{
    assert(date);
    RTC_HAL_GetAlarm(g_rtcBaseAddr[instance], date);
}

void RTC_DRV_InitRepeatAlarm(uint32_t instance, rtc_repeat_alarm_state_t *repeatAlarmState)
{
    assert(repeatAlarmState);

    /* Init driver repeat alarm struct.*/
    memset(repeatAlarmState, 0, sizeof(*repeatAlarmState));
    s_rtcRepeatAlarmState = repeatAlarmState;
}

bool RTC_DRV_SetAlarmRepeat(uint32_t instance, rtc_datetime_t *alarmTime, rtc_datetime_t *alarmRepInterval)
{
    assert(s_rtcRepeatAlarmState);
    assert(alarmRepInterval);

    if (!(RTC_DRV_SetAlarm(instance, alarmTime, true)))
    {
        return false;
    }

    memcpy(&s_rtcRepeatAlarmState->alarmTime, alarmTime, sizeof(*alarmTime));
    memcpy(&s_rtcRepeatAlarmState->alarmRepTime, alarmRepInterval, sizeof(*alarmRepInterval));

    return true;
}

void RTC_DRV_DeinitRepeatAlarm(uint32_t instance)
{
    s_rtcRepeatAlarmState = NULL;
}

void RTC_DRV_SetAlarmIntCmd(uint32_t instance, bool alarmEnable)
{
    RTC_HAL_SetAlarmIntCmd(g_rtcBaseAddr[instance], alarmEnable);
}

bool RTC_DRV_GetAlarmIntCmd(uint32_t instance)
{
    return RTC_HAL_ReadAlarmInt(g_rtcBaseAddr[instance]);
}

bool RTC_DRV_IsAlarmPending(uint32_t instance)
{
    /* Return alarm time and status (triggered or not) */
    return RTC_HAL_HasAlarmOccured(g_rtcBaseAddr[instance]);
}

void RTC_DRV_SetTimeCompensation(uint32_t instance, uint32_t compensationInterval,
                                            uint32_t compensationTime)
{
    uint32_t rtcBaseAddr = g_rtcBaseAddr[instance];

    RTC_HAL_SetCompensationIntervalRegister(rtcBaseAddr, compensationInterval);
    RTC_HAL_SetTimeCompensationRegister(rtcBaseAddr, compensationTime);
}

void RTC_DRV_GetTimeCompensation(uint32_t instance, uint32_t *compensationInterval,
                                            uint32_t *compensationTime)
{
    uint32_t rtcBaseAddr = g_rtcBaseAddr[instance];

    *compensationInterval = RTC_HAL_GetCompensationIntervalCounter(rtcBaseAddr);
    *compensationTime = RTC_HAL_GetTimeCompensationValue(rtcBaseAddr);
}

void RTC_DRV_AlarmIntAction(uint32_t instance)
{
    uint32_t rtcBaseAddr = g_rtcBaseAddr[instance];

    if (s_rtcRepeatAlarmState != NULL)
    {
        s_rtcRepeatAlarmState->alarmTime.year += s_rtcRepeatAlarmState->alarmRepTime .year;
        s_rtcRepeatAlarmState->alarmTime.month += s_rtcRepeatAlarmState->alarmRepTime.month;
        s_rtcRepeatAlarmState->alarmTime.day += s_rtcRepeatAlarmState->alarmRepTime.day;
        s_rtcRepeatAlarmState->alarmTime.hour += s_rtcRepeatAlarmState->alarmRepTime.hour;
        s_rtcRepeatAlarmState->alarmTime.minute += s_rtcRepeatAlarmState->alarmRepTime.minute;
        RTC_HAL_SetAlarm(rtcBaseAddr, &s_rtcRepeatAlarmState->alarmTime);
    }
    else
    {
        /* Writing to the alarm register clears the TAF flag in the Status register */
        RTC_HAL_SetAlarmReg(rtcBaseAddr, 0x0);
        RTC_HAL_SetAlarmIntCmd(rtcBaseAddr, false);
    }
}

void RTC_DRV_SecsIntAction(uint32_t instance)
{
    RTC_HAL_SetSecsIntCmd(g_rtcBaseAddr[instance], false);
}

#if FSL_FEATURE_RTC_HAS_MONOTONIC
/*FUNCTION**********************************************************************
 *
 * Function Name : rtc_increment_monotonic
 * Description   : increments monotonic counter by one.
 * This function will increment monotonic counter by one.
 *
 *END**************************************************************************/
bool RTC_DRV_IncrementMonotonic(uint32_t instance)
{
    return RTC_HAL_IncrementMonotonicCounter(g_rtcBaseAddr[instance]);
}
#endif

/*! @}*/

/*******************************************************************************
 * EOF
 ******************************************************************************/

