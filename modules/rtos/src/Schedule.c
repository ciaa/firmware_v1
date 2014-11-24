/* Copyright 2008, 2009, 2014 Mariano Cerdeiro
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

/** \brief FreeOSEK Os Schedule Implementation File
 **
 ** This file implements the Schedule API
 **
 ** \file Schedule.c
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
 * 20141121 v0.2.0 MaCe rework Schedule
 * 20090418 v0.1.5 MaCe add Pre/Post TaskHook handling
 * 20090417 v0.1.4 MaCe update license
 * 20090330 v0.1.3 MaCe use new CallTask macro and add use of SetActualContext
 * 20090130 v0.1.2 MaCe add OSEK_MEMMAP check
 * 20081113 v0.1.1 KLi  Added memory layout attribute macros
 * 20080810 v0.1.0 MaCe initial version
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
StatusType Schedule
(
   void
)
{
   /* \req OSEK_SYS_3.4 The system service StatusType Schedule ( void ) shall
    ** be defined */

   /* \req OSEK_SYS_3.4.4 Possible return values in Standard mode is E_OK */
   StatusType ret = E_OK;
   TaskType nextTask;
   TaskType actualTask;
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   ContextType actualContext;
#endif

   IntSecure_Start();

   /* get actual running task */
   actualTask = GetRunningTask();

   /* \req OSEK_SYS_3.3.5 Extra possible return values in Extended mode are E_OS
    ** CALLEVEL, E_OS_RESOURCE  */
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)

   /* get actual context */
   actualContext = GetCallingContext();


   if ( ( CONTEXT_TASK != actualContext ) &&
        ( CONTEXT_SYS != actualContext ) )
   {
      /* \req OSEK_SYS_3.3.5 Extra possible return values in Extended mode
       ** are E_OS_CALLEVEL, E_OS_RESOURCE */
      ret = E_OS_CALLEVEL;
   }
   else if ( ( INVALID_TASK != actualTask ) &&
             ( CONTEXT_TASK == actualContext ) )
   {
      if ( TasksVar[actualTask].Resources != 0 )
      {
         /* \req OSEK_SYS_3.3.5 Extra possible return values in Extended mode
          ** are E_OS_CALLEVEL, E_OS_RESOURCE */
         ret = E_OS_RESOURCE;
      }
   }
   else
   {
      /* nothing to check Runngin Task is invalid */
   }

   if (ret == E_OK)
#endif
   {
      /* get next task */
      nextTask = GetNextTask();

      /* while until one or boths are not more invalid tasks */
      while (	( actualTask == INVALID_TASK ) &&
            ( nextTask == INVALID_TASK) )
      {
         IntSecure_End();

         /* macro used to indicate the processor that we are in idle time */
         osekpause();

         IntSecure_Start();

         /* get next task */
         nextTask = GetNextTask();
      };

      /* if the actual task is invalid */
      if ( actualTask == INVALID_TASK )
      {
         /* set task state to running */
         TasksVar[nextTask].Flags.State = TASK_ST_RUNNING;

         /* set as running task */
         SetRunningTask(nextTask);

         /* set actual context task */
         SetActualContext(CONTEXT_TASK);

         IntSecure_End();

#if (HOOK_PRETASKHOOK == OSEK_ENABLE)
         PreTaskHook();
#endif /* #if (HOOK_PRETASKHOOK == OSEK_ENABLE) */

         /* jmp tp the next task */
         JmpTask(nextTask);
      }
      else
      {
         /* check priorities */
         /* \req OSEK_SYS_3.4.1 If a task with a lower or equal priority than the
          ** ceiling priority of the internal resource and higher priority than
          ** the priority of the calling task is ready */
         if ( TasksConst[nextTask].StaticPriority > TasksVar[actualTask].ActualPriority )
         {

#if (HOOK_POSTTASKHOOK == OSEK_ENABLE)
            PostTaskHook();
#endif /* #if (HOOK_POSTTASKHOOK == OSEK_ENABLE) */

            /* \req OSEK_SYS_3.4.1.1 the internal resource of the task shall be
             ** released */
            ReleaseInternalResources();

            /* \req OSEK_SYS_3.4.1.2 the current task is put into the ready state */
            TasksVar[actualTask].Flags.State = TASK_ST_READY;

            /* set the new task to running */
            TasksVar[nextTask].Flags.State = TASK_ST_RUNNING;

            /* set as running task */
            SetRunningTask(nextTask);

            /* set actual context task */
            SetActualContext(CONTEXT_TASK);

            IntSecure_End();

#if (HOOK_PRETASKHOOK == OSEK_ENABLE)
            PreTaskHook();
#endif /* #if (HOOK_PRETASKHOOK == OSEK_ENABLE) */

            /* \req OSEK_SYS_3.4.1.3 its context is saved */
            /* \req OSEK_SYS_3.4.1.4 and the higher-priority task is executed */
            CallTask(actualTask, nextTask);
         }
         else
         {
            IntSecure_End();

            /* \req OSEK_SYS_3.4.2 Otherwise the calling task is continued */
         }
      }
   }
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   else
   {
      IntSecure_End();
   }
#endif /* #if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) */

#if (HOOK_ERRORHOOK == OSEK_ENABLE)
   /* \req OSEK_ERR_1.3-4/xx The ErrorHook hook routine shall be called if a
    ** system service returns a StatusType value not equal to E_OK.*/
   /* \req OSEK_ERR_1.3.1-4/xx The hook routine ErrorHook is not called if a
    ** system service is called from the ErrorHook itself. */
   if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
   {
      SetError_Api(OSServiceId_Schedule);
      SetError_Ret(ret);
      SetError_Msg("Schedule Task returns != than E_OK");
      SetError_ErrorHook();
   }
#endif

   return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

