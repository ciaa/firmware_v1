/* Copyright 2016, Mariano Cerdeiro
 * Copyright 2016, Diego Ezequiel Vommaro 
 * All rights reserved.
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

/** \brief CIAA POSIX source file
 **
 ** This file contains the POSIX implementation
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */
/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_unistd.h"
#include "ciaaPOSIX_assert.h"
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/
#define CIAAPOSIX_MAINFUNCTION_PERIODUS 10000

#define CIAAPOSIX_MAINFUNCTION_PERIODMS ((CIAAPOSIX_MAINFUNCTION_PERIODUS)/10)

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
/** \brief ciaaPOSIX_sleepAlgorithm
 **
 ** Sleeps the calling application and reload the global counter if required
 **
 ** \param[in] toSleep counts to sleep the execution of the calling task.
 **
 **/
static void ciaaPOSIX_sleepAlgorithm(uint32_t toSleep);

/*==================[internal data definition]===============================*/
bool sleeping_tasks = false;
bool ciaaPOSIX_sleeping_states[TASKS_COUNT]={false};
uint32_t counts_to_wakeup = 0;
uint32_t ciaaPOSIX_counter = 0;
uint32_t ciaaPOSIX_sleeps[TASKS_COUNT]={0};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void ciaaPOSIX_sleepAlgorithm(uint32_t toSleep)
{
   TaskType taskID;

   /* Are there sleeping tasks? */
   if(sleeping_tasks)
   {
      if((counts_to_wakeup - ciaaPOSIX_counter) > (toSleep - ciaaPOSIX_counter))
      {
         counts_to_wakeup = toSleep + ciaaPOSIX_counter;
      }
   }
   /* So, there is a sleeping task now */
   else
   {
      sleeping_tasks = true;
   }

   /* Get task id */
   GetTaskID(&taskID);

   /* Store counts */
   ciaaPOSIX_sleeps[taskID] = (int32_t)(toSleep + ciaaPOSIX_counter);

   /* Set sleeping task state for this task */
   ciaaPOSIX_sleeping_states[taskID] = true;

   /* wait for the posix event */
   WaitEvent(POSIXE);
   ClearEvent(POSIXE);
}

/*==================[external functions definition]==========================*/
extern uint32_t ciaaPOSIX_sleep(uint32_t seconds)
{
   uint32_t toSleep;
   TaskType taskID;

   /* ensure that the seconds can be stored in 10ms counts */
   ciaaPOSIX_assert(42949672U > seconds);

   /* store the sleep time in 10ms counts */
   toSleep = seconds * 100;

   /* sleep time processing*/
   ciaaPOSIX_sleepAlgorithm(toSleep);

   return 0;
}

extern int32_t ciaaPOSIX_usleep(useconds_t useconds)
{
   int32_t ret = 0;
   uint32_t toSleep;
   TaskType taskID;

#if (CIAAPOSIX_DEBUG == CIAAPOSIX_ENABLE)
   if (1000000 >= useconds)
   {
      ret = -1;
   }
   else
#endif
   {
      /* calculate how many main function cycles shall be sleep */
      toSleep = (useconds + (CIAAPOSIX_MAINFUNCTION_PERIODUS-1))
         / CIAAPOSIX_MAINFUNCTION_PERIODUS;

      /* sleep time processing */
      ciaaPOSIX_sleepAlgorithm(toSleep);
   }

   return ret;
}

extern void ciaaPOSIX_sleepMainFunction(void)
{
   uint32_t taskID;
   uint32_t aux_distance;

   /* Are there sleeping tasks? */
   if(sleeping_tasks)
   {
      ciaaPOSIX_counter++;

      /* Is it time to wake up? */
      if(counts_to_wakeup == ciaaPOSIX_counter)
      {
         /* Maximum distance to find the minimum */
         aux_distance = 4294967295U;

         /* Explore the task array to find task or tasks \
            to wake up and the next counts to wake up if any */
         for(taskID = 0; TASKS_COUNT > taskID; taskID++)
         {
            if (ciaaPOSIX_sleeping_states[taskID])
            {
               if(counts_to_wakeup == ciaaPOSIX_sleeps[taskID])
               {
                  ciaaPOSIX_sleeping_states[taskID] = false;
                  SetEvent(taskID, POSIXE);
               }
               else
               {
                  if(aux_distance > (ciaaPOSIX_sleeps[taskID] - ciaaPOSIX_counter))
                  {
                     aux_distance = ciaaPOSIX_sleeps[taskID] - ciaaPOSIX_counter;
                     counts_to_wakeup = ciaaPOSIX_sleeps[taskID];
                  }
               }
            }
         }
         /* there are not sleeping tasks */
         if (429467295U == aux_distance)
         {
            sleeping_tasks = false;
         }
      }
   }

} /* end of ciaaPOSIX_sleepMainFunction */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

