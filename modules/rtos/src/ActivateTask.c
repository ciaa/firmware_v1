/* Copyright 2008, 2009 Mariano Cerdeiro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief FreeOSEK Os ActivateTask Implementation File
 **
 ** This file implements the ActiveTask API
 **
 ** \file ActivateTask.c
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Global
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * KLi          Kang Li
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090330 v0.1.4 MaCe add support to NON_PREEMPTIVE systems
 * 20090330 v0.1.3 MaCe separate ifs that may be interpreted in a wrong order
 * 20090128 v0.1.2 MaCe add OSEK_MEMMAP check
 * 20081113 v0.1.1 KLi  Added memory layout attribute macros
 * 20080713 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
   StatusType ActivateTask
(
 TaskType TaskID
 )
{
   /* \req OSEK_SYS_3.1 The system service StatusType
    * ActivateTask ( TaskType TaskID ) shall be defined. */

   /* \req OSEK_SYS_3.1.3 The service may be called from interrupt category 2
    * level and from task level. */
   /* nothing to do for this req. */

   /* \req OSEK_SYS_3.1.7-1/3 Possible return values in Standard mode are E_OK or E_OS_LIMIT */
   StatusType ret = E_OK;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   /* check if the task id is valid */
   if ( TaskID >= TASKS_COUNT )
   {
      /* if an invalid task id return E_OS_ID */
      /* \req OSEK_SYS_3.1.5-1/3 If other than E_OK is returned the activation
       * is ignored */
      /* \req OSEK_SYS_3.1.8 Added possible return values in Extended mode is
       * E_OS_ID */
      ret = E_OS_ID;
   }
   else
#endif
   {
      IntSecure_Start();

      /* check if the task is susspended */
      /* \req OSEK_SYS_3.1.1-1/2 The task TaskID shall be transferred from the
       * suspended state into the ready state. */
      if ( TasksVar[TaskID].Flags.State == TASK_ST_SUSPENDED )
      {
         /* increment activation counter */
         TasksVar[TaskID].Activations++;
         /* if the task was suspended set it to ready */
         /* OSEK_SYS_3.1.1-2/2 The task TaskID shall be transferred from the
          * suspended state into the ready state.*/
         TasksVar[TaskID].Flags.State = TASK_ST_READY;
         /* clear all events */
         /* \req OSEK_SYS_3.1.6 When an extended task is transferred from
          * suspended state into ready state all its events are cleared. */
         TasksVar[TaskID].Events = 0;
         /* add the task to the ready list */
         AddReady(TaskID);
      }
      else
      {
         /* task is not suspended */

         /* check if the task is a extended task */
         if ( TasksConst[TaskID].ConstFlags.Extended )
         {
            /* return E_OS_LIMIT */
            /* \req OSEK_SYS_3.1.5-2/3 If other than E_OK is returned the activation
             * is ignored */
            /* \req OSEK_SYS_3.1.7-2/3 Possible return values in Standard mode are
             * E_OK or E_OS_LIMIT */
            ret = E_OS_LIMIT;
         }
         else
         {
            /* check if more activations are allowed */
            if ( TasksVar[TaskID].Activations < TasksConst[TaskID].MaxActivations )
            {
               /* increment activation counter */
               TasksVar[TaskID].Activations++;
               /* add the task to the ready list */
               AddReady(TaskID);
            }
            else
            {
               /* maximal activation reached, return E_OS_LIMIT */
               /* \req OSEK_SYS_3.1.5-3/3 If other than E_OK is returned the
                * activation is ignored */
               /* \req OSEK_SYS_3.1.7-3/3 Possible return values in Standard mode are
                * E_OK or E_OS_LIMIT */
               ret = E_OS_LIMIT;
            }
         }
      }

      IntSecure_End();

#if (NON_PREEMPTIVE == OSEK_DISABLE)
      /* check if called from a Task Context */
      if ( GetCallingContext() ==  CONTEXT_TASK )
      {
         if ( ( TasksConst[GetRunningTask()].ConstFlags.Preemtive ) &&
               ( ret == E_OK )	)
         {
            /* This is needed to avoid Schedule to perform standard checks
             * which are done when normally called from the application
             * the actual context has to be task so is not need to store it */
            SetActualContext(CONTEXT_SYS);

            /* \req OSEK_SYS_3.1.4 Rescheduling shall take place only if called from a
             * preemptable task. */
            (void)Schedule();

            /* restore the old context */
            SetActualContext(CONTEXT_TASK);
         }
      }
#endif /* #if (NON_PREEMPTIVE == OSEK_DISABLE) */
   }


#if (HOOK_ERRORHOOK == OSEK_ENABLE)
   /* \req OSEK_ERR_1.3-1/xx The ErrorHook hook routine shall be called if a
    * system service returns a StatusType value not equal to E_OK.*/
   /* \req OSEK_ERR_1.3.1-1/xx The hook routine ErrorHook is not called if a
    * system service is called from the ErrorHook itself. */
   if ( ( ret != E_OK ) && (ErrorHookRunning != 1U))
   {
      SetError_Api(OSServiceId_ActivateTask);
      SetError_Param1(TaskID);
      SetError_Ret(ret);
      SetError_Msg("ActivateTask returns != than E_OK");
      SetError_ErrorHook();
   }
#endif

   return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

