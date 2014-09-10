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

#if defined (FSL_RTOS_MQX)

#include <assert.h>
#include "fsl_os_abstraction.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskCreate
 * Description   : This function is used to create a task and make it ready.
 * Return kStatus_OSA_Success if register successfully, otherwise return
 * kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_TaskCreate(task_t          task,
                         uint8_t        *name,
                         uint16_t        stackSize,
                         task_stack_t   *stackMem,
                         uint16_t        priority,
                         task_param_t    param,
                         bool            usesFloat,
                         task_handler_t *handler)
{
    TASK_TEMPLATE_STRUCT taskTemplate =
        {
            .TASK_TEMPLATE_INDEX = 0,   /* Don't care */
            .TASK_ADDRESS = task,
            .TASK_STACKSIZE = stackSize,
            .TASK_PRIORITY = PRIORITY_OSA_TO_RTOS(priority),
            .TASK_NAME = (char *)name,
            .TASK_ATTRIBUTES = 0,
            .CREATION_PARAMETER = param,
            .DEFAULT_TIME_SLICE = 0,   /* Don't care */
        };

    if (usesFloat)
    {
        taskTemplate.TASK_ATTRIBUTES |= MQX_FLOATING_POINT_TASK;
    }

    if (NULL == stackMem)
    {
        /* Create task on local processor and passing a task template */
        *handler = _task_create(0, 0, (uint32_t)&taskTemplate);
    }
    else
    {
        /* Create task on local processor and passing a task template */
        *handler = _task_create_at(0, 0, (uint32_t)&taskTemplate, stackMem, stackSize);
    }

    if (MQX_NULL_TASK_ID != *handler)
    {
        return kStatus_OSA_Success;
    }
    else
    {
        return kStatus_OSA_Error;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskDestroy
 * Description   : This function destroy a task. Return kStatus_OSA_Success if
 * the task is destroied, otherwise return kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_TaskDestroy(task_handler_t handler)
{
    return (MQX_OK == _task_destroy(handler)) ? kStatus_OSA_Success : kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskYield
 * Description   : When a task calls this function, it will give up CPU and put
 * itself to the tail of ready list.
 *
 *END**************************************************************************/
osa_status_t OSA_TaskYield(void)
{
    _sched_yield();
    return kStatus_OSA_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskGetHandler
 * Description   : This function is used to get current active task's handler.
 *
 *END**************************************************************************/
task_handler_t OSA_TaskGetHandler(void)
{
    return _task_get_id_from_td(_task_get_td(MQX_NULL_TASK_ID));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskGetPriority
 * Description   : This function returns task's priority by task handler.
 *
 *END**************************************************************************/
uint16_t OSA_TaskGetPriority(task_handler_t handler)
{
    _mqx_uint priority;
    _task_get_priority(handler, &priority);
    return PRIORITY_RTOS_TO_OSA(priority);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskSetPriority
 * Description   : This function sets task's priority by task handler.
 *
 *END**************************************************************************/
osa_status_t OSA_TaskSetPriority(task_handler_t handler, uint16_t priority)
{
    _mqx_uint oldPriority;

    if (MQX_OK == _task_set_priority(handler, PRIORITY_OSA_TO_RTOS(priority), &oldPriority))
    {
        return kStatus_OSA_Success;
    }
    else
    {
        return kStatus_OSA_Error;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : wait_timeout_msec_to_tick
 * Description   : This function converts timeout from millisecond to tick for
 * wait functions.
 *
 *END**************************************************************************/
static uint32_t wait_timeout_msec_to_tick(uint32_t timeout)
{
    if (OSA_WAIT_FOREVER == timeout)   /* Wait forever. */
    {
        return 0U; /* Timeout 0 means wait forever for MQX. */
    }
    else
    {
        timeout *= _time_get_ticks_per_sec();
        timeout /= 1000U;
        /*
         * If timeout is too short that changed to 0 tick, reset it to 1 tick
         * in case of infinitely wait.
         */
        if (!timeout)
        {
            timeout = 1U;
        }
        return timeout;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaCreate
 * Description   : This function is used to create a semaphore. Return
 * kStatus_OSA_Success if create successfully, otherwise return kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaCreate(semaphore_t *pSem, uint8_t initValue)
{
    return MQX_OK == _lwsem_create(pSem, initValue) ?
                     kStatus_OSA_Success            :
                     kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaWait
 * Description   : This function checks the semaphore's counting value, if it is
 * positive, decreases it and returns kStatus_OSA_Success, otherwise, timeout
 * will be used for wait. The parameter timeout indicates how long should wait
 * in milliseconds. Pass OSA_WAIT_FOREVER to wait indefinitely, pass 0 will
 * return kStatus_OSA_Timeout immediately if semaphore is not positive.
 * This function returns kStatus_OSA_Success if the semaphore is received, returns
 * kStatus_OSA_Timeout if the semaphore is not received within the specified
 * 'timeout', returns kStatus_OSA_Error if any errors occur during waiting.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaWait(semaphore_t *pSem, uint32_t timeout)
{
    _mqx_uint retVal;

    /* If timeout is 0, try to get the semaphore. */
    if (!timeout)
    {
        return _lwsem_poll(pSem) ? kStatus_OSA_Success : kStatus_OSA_Timeout;
    }
    else
    {
        /* If timeout is not 0, convert it to tickes. */
        timeout = wait_timeout_msec_to_tick(timeout);
        retVal = _lwsem_wait_ticks(pSem, timeout);

        if(MQX_OK == retVal)
        {
            return kStatus_OSA_Success;
        }
        else if (MQX_LWSEM_WAIT_TIMEOUT == retVal)
        {
            return kStatus_OSA_Timeout;
        }
        else
        {
            return kStatus_OSA_Error;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaPost
 * Description   : This function is used to wake up one task that wating on the
 * semaphore. If no task is waiting, increase the semaphore. The function returns
 * kStatus_OSA_Success if the semaphre is post successfully, otherwise returns
 * kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaPost(semaphore_t *pSem)
{
    return (MQX_OK == _lwsem_post(pSem)) ? kStatus_OSA_Success : kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaDestroy
 * Description   : This function is used to destroy a semaphore.
 * Return kStatus_OSA_Success if the semaphore is destroyed successfully, otherwise
 * return kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaDestroy(semaphore_t *pSem)
{
    return (MQX_OK == _lwsem_destroy(pSem)) ? kStatus_OSA_Success : kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexCreate
 * Description   : This function is used to create a mutex.
 * Return kStatus_OSA_Success if create successfully, otherwise return
 * kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexCreate(mutex_t *pMutex)
{
    _mqx_uint retVal;

    MUTEX_ATTR_STRUCT mutexattr;

    retVal = _mutatr_init(&mutexattr);
    if (MQX_OK == retVal)
    {
        retVal = _mutatr_set_sched_protocol(&mutexattr, MUTEX_PRIO_INHERIT);
        if (MQX_OK == retVal)
        {
            retVal = _mutex_init(pMutex, &mutexattr);
        }
    }

    return MQX_OK == retVal ? kStatus_OSA_Success : kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexLock
 * Description   : This function checks the mutex's status, if it is unlocked,
 * lock it and returns kStatus_OSA_Success, otherwise, wait for the mutex.
 * MQX does not support timeout to wait for a mutex.
 * This function returns kStatus_OSA_Success if the mutex is obtained, returns
 * kStatus_OSA_Error if any errors occur during waiting. If the mutex has been
 * locked, pass 0 as timeout will return kStatus_OSA_Timeout immediately.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexLock(mutex_t *pMutex, uint32_t timeout)
{
    if (!timeout)
    {
        return MQX_OK == _mutex_try_lock(pMutex) ? kStatus_OSA_Success : kStatus_OSA_Error;
    }
    else
    {
        return MQX_OK == _mutex_lock(pMutex) ? kStatus_OSA_Success : kStatus_OSA_Error;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexUnlock
 * Description   : This function is used to unlock a mutex.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexUnlock(mutex_t *pMutex)
{
    return MQX_OK == _mutex_unlock(pMutex) ? kStatus_OSA_Success : kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexDestroy
 * Description   : This function is used to destroy a mutex.
 * Return kStatus_OSA_Success if the lock object is destroyed successfully,
 * otherwise return kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexDestroy(mutex_t *pMutex)
{
    return MQX_OK == _mutex_destroy(pMutex) ? kStatus_OSA_Success : kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventCreate
 * Description   : This function is used to create a event object. Return
 * kStatus_OSA_Success if create successfully, otherwise return kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_EventCreate(event_t *pEvent, osa_event_clear_mode_t clearMode)
{
    _mqx_uint retVal;

    retVal = _lwevent_create(pEvent, (kEventAutoClear == clearMode) ? LWEVENT_AUTO_CLEAR : 0);

    return (retVal == MQX_OK) ? kStatus_OSA_Success : kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventWait
 * Description   : This function checks the event's status, if it meets the wait
 * condition, return kStatus_OSA_Success, otherwise, timeout will be used for
 * wait. The parameter timeout indicates how long should wait in milliseconds.
 * Pass OSA_WAIT_FOREVER to wait indefinitely, pass 0 will return the value
 * kStatus_OSA_Timeout immediately if wait condition is not met. The event flags
 * will be cleared if the event is auto clear mode. Flags that wakeup waiting
 * task could be obtained from the parameter setFlags.
 * This function returns kStatus_OSA_Success if wait condition is met, returns
 * kStatus_OSA_Timeout if wait condition is not met within the specified
 * 'timeout', returns kStatus_OSA_Error if any errors occur during waiting.
 *
 *END**************************************************************************/
osa_status_t OSA_EventWait(event_t       *pEvent,
                        event_flags_t  flagsToWait,
                        bool           waitAll,
                        uint32_t       timeout,
                        event_flags_t *setFlags)
{
    _mqx_uint retVal;

    *setFlags = 0;
    /*
     * TODO:
     * MQX lwevent doesn't provide API to poll event, so use a work around here.
     */
    if (!timeout)
    {
        _int_disable();
        if ((waitAll && ((pEvent->VALUE & flagsToWait) == flagsToWait)) ||
           ((!waitAll) && (pEvent->VALUE & flagsToWait)))
        {
            retVal = _lwevent_wait_ticks(pEvent, flagsToWait, waitAll, 1);
            _int_enable();
            if (MQX_OK == retVal)
            {
                *setFlags = _lwevent_get_signalled();
                return kStatus_OSA_Success;
            }
            else
            {
                return kStatus_OSA_Error;
            }
        }
        else
        {
            _int_enable();
            return kStatus_OSA_Timeout;
        }
    }
    else
    {
        timeout = wait_timeout_msec_to_tick(timeout);
        retVal = _lwevent_wait_ticks(pEvent, flagsToWait, waitAll, timeout);

        if (MQX_OK == retVal)
        {
            *setFlags = _lwevent_get_signalled();
            return kStatus_OSA_Success;
        }
        else if (LWEVENT_WAIT_TIMEOUT == retVal)
        {
            return kStatus_OSA_Timeout;
        }
        else
        {
            return kStatus_OSA_Error;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventSet
 * Description   : Set one or more event flags of an event object.
 * Return kStatus_OSA_Success if set successfully, kStatus_OSA_Error if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_EventSet(event_t *pEvent, event_flags_t flagsToSet)
{
    return (MQX_OK == _lwevent_set(pEvent, flagsToSet)) ?
                                    kStatus_OSA_Success :
                                    kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventClear
 * Description   : Clear one or more event flags of an event object.
 * Return kStatus_OSA_Success if clear successfully, kStatus_OSA_Error if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_EventClear(event_t *pEvent, event_flags_t flagsToClear)
{
    return (MQX_OK == _lwevent_clear(pEvent, flagsToClear)) ?
                                        kStatus_OSA_Success :
                                        kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventDestroy
 * Description   : This function is used to destroy a event object. Return
 * kStatus_OSA_Success if the event object is destroyed successfully, otherwise
 * return kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_EventDestroy(event_t *pEvent)
{
    return (MQX_OK == _lwevent_destroy(pEvent)) ? kStatus_OSA_Success : kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQCreate
 * Description   : This function is used to create a message queue.
 * Return the handle to the message queue if create successfully, otherwise
 * return 0.
 *
 *END**************************************************************************/
msg_queue_handler_t OSA_MsgQCreate(msg_queue_t *queue,
                                        uint16_t  message_number,
                                        uint16_t  message_size)
{
    _mqx_uint retVal;

    /* Create the message queue where each element is a pointer to the message item. */
    retVal = _lwmsgq_init(queue, message_number, SIZE_IN_MMT_UNITS(message_size * 4));

    if (MQX_OK == retVal)
    {
        return (msg_queue_handler_t)queue;
    }
    else
    {
        return (msg_queue_handler_t)NULL;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQPut
 * Description   : This function is used to put a message to a message queue.
 * Return kStatus_OSA_Success if the message is put successfully, otherwise
 * return kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQPut(msg_queue_handler_t handler, void* pMessage)
{
    /* Does not block if full */
    return (MQX_OK == _lwmsgq_send(handler, (_mqx_max_type_ptr)pMessage, 0)) ?
                                                         kStatus_OSA_Success :
                                                         kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQGet
 * Description   : This function checks the queue's status, if it is not empty,
 * get message from it and return kStatus_OSA_Success, otherwise, timeout will
 * be used for wait. The parameter timeout indicates how long should wait in
 * milliseconds. Pass OSA_WAIT_FOREVER to wait indefinitely, pass 0 will return
 * kStatus_OSA_Timeout immediately if queue is empty.
 * This function returns kStatus_OSA_Success if message is got successfully,
 * returns kStatus_OSA_Timeout if message queue is empty within the specified
 * 'timeout', returns kStatus_OSA_Error if any errors occur during waiting.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQGet(msg_queue_handler_t handler,
                           void               *pMessage,
                           uint32_t            timeout)
{
    _mqx_uint retVal;

    if(!timeout)
    {
        retVal = _lwmsgq_receive(handler, (_mqx_max_type_ptr)pMessage, 0, 0, NULL);
    }
    else
    {
        timeout = wait_timeout_msec_to_tick(timeout);
        retVal = _lwmsgq_receive(handler, (_mqx_max_type_ptr)pMessage,
                LWMSGQ_RECEIVE_BLOCK_ON_EMPTY, timeout, NULL);
    }

    if (MQX_OK == retVal)
    {
        return kStatus_OSA_Success;
    }
    else if ((LWMSGQ_TIMEOUT == retVal) || (LWMSGQ_EMPTY == retVal))
    {
        return kStatus_OSA_Timeout;
    }
    else
    {
        return kStatus_OSA_Error;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQDestroy
 * Description   : This function is used to destroy the message queue.
 * Return kStatus_OSA_Success if the message queue is destroyed successfully,
 * otherwise return kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQDestroy(msg_queue_handler_t handler)
{
    return (MQX_OK == _lwmsgq_deinit(handler)) ? kStatus_OSA_Success : kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MemAlloc
 * Description   : This function is used to allocate amount of memory in bytes.
 * Return the pointer to the memory if success, otherwise return NULL;
 *
 *END**************************************************************************/
void * OSA_MemAlloc(size_t size)
{
    /* We allocate memory as a system, not as a calling task. That will prevent
     * us from failures during freeing the same memory from another task.
     */
    return _mem_alloc_system(size);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MemAllocZero
 * Description   : This function is used to allocate amount of memory in bytes
 * and initializes it to 0.
 * Return the pointer to the memory if success, otherwise return NULL;
 *
 *END**************************************************************************/
void * OSA_MemAllocZero(size_t size)
{
    return _mem_alloc_system_zero(size);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MemFree
 * Description   : This function is used to free the memory previously allocated.
 *
 *END**************************************************************************/
osa_status_t OSA_MemFree(void *ptr)
{
    return MQX_OK == _mem_free(ptr) ? kStatus_OSA_Success : kStatus_OSA_Error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EnterCritical
 * Description   : This function is used to ensure some code will not be preempted.
 *
 *END**************************************************************************/
void OSA_EnterCritical(osa_critical_section_mode_t mode)
{
    if (kCriticalDisableInt == mode)
    {
        _int_disable();
    }
    else
    {
        _task_stop_preemption();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_ExitCritical
 * Description   : This function is used to exit critical section.
 *
 *END**************************************************************************/
void OSA_ExitCritical(osa_critical_section_mode_t mode)
{
    if (kCriticalDisableInt == mode)
    {
        _int_enable();
    }
    else
    {
        _task_start_preemption();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimeDelay
 * Description   : This function is used to delay for a number of milliseconds.
 *
 *END**************************************************************************/
void OSA_TimeDelay(uint32_t delay)
{
    _time_delay(delay);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimeGetMsec
 * Description   : This function gets current time in milliseconds.
 *
 *END**************************************************************************/
uint32_t OSA_TimeGetMsec(void)
{
    /*
     * NOTE: Time overflow is not checked.
     */
    bool overflow;
    MQX_TICK_STRUCT tickStruct;
    _time_get_elapsed_ticks(&tickStruct);
    return(PSP_TICKS_TO_MILLISECONDS(&tickStruct, &overflow));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_InstallIntHandler
 * Description   : This function is used to install interrupt handler.
 *
 *END**************************************************************************/
osa_status_t OSA_InstallIntHandler (int32_t IRQNumber,
                                                        void (*handler)(void))
{
    // For MISRA 2004 11.1.
#if defined ( __IAR_SYSTEMS_ICC__ )
_Pragma ("diag_suppress = Pm138")
#endif
    if (!_int_install_isr(IRQNumber, (INT_ISR_FPTR)handler, NULL))
#if defined ( __IAR_SYSTEMS_ICC__ )
_Pragma ("diag_default = PM138")
#endif
    {
        return kStatus_OSA_Error;
    }
    else
    {
        return kStatus_OSA_Success;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_Init
 * Description   : This function is used to setup the basic services, it should
 * be called first in function main. Return kStatus_OSA_Success if services
 * are initialized successfully, otherwise return kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_Init(void)
{
    return kStatus_OSA_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_Start
 * Description   : This function is used to start RTOS scheduler.
 *
 *END**************************************************************************/
osa_status_t OSA_Start(void)
{
    return kStatus_OSA_Success;
}


#endif
/*******************************************************************************
 * EOF
 ******************************************************************************/
