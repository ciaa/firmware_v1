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
#include "ciaaPOSIX_unistd_Internal.h"
#include "ciaaPOSIX_assert.h"
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/
#define CIAAPOSIX_MAINFUNCTION_PERIODUS 10000

#define CIAAPOSIX_MAINFUNCTION_PERIODMS ((CIAAPOSIX_MAINFUNCTION_PERIODUS)/1000)

#define SLEEP_TIME_TO_COUNTS (1000 / CIAAPOSIX_MAINFUNCTION_PERIODMS)

#define MAX_SECONDS (MAX_COUNTS / SLEEP_TIME_TO_COUNTS)

#define MAX_USECONDS (UINT32_MAX - (CIAAPOSIX_MAINFUNCTION_PERIODUS-2))

/*==================[internal data declaration]==============================*/
/** \brief Remaining counts to wake up the next task */
static ciaaPOSIX_counter_t ciaaPOSIX_next_to_wake;

/** \brief Current counts */
static ciaaPOSIX_counter_t ciaaPOSIX_counter;

/** task list where are allocated its sleeping state */
static ciaaPOSIX_counter_t ciaaPOSIX_sleeps[];

/*==================[internal functions declaration]=========================*/
/** \brief ciaaPOSIX_sleepAlgorithm
 **
 ** Sleeps the current task and reload the global counter if required
 **
 ** \param[in] toSleep counts to sleep the execution of the calling task.
 **
 **/
static void ciaaPOSIX_sleepAlgorithm(uint32_t toSleep);

/*==================[internal data definition]===============================*/
static ciaaPOSIX_counter_t ciaaPOSIX_next_to_wake = {0, 0};
static ciaaPOSIX_counter_t ciaaPOSIX_counter = {0, 0};
static ciaaPOSIX_counter_t ciaaPOSIX_sleeps[TASKS_COUNT] = {{0}};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void ciaaPOSIX_sleepAlgorithm(uint32_t toSleep)
{
   TaskType taskID;

   /* Are there sleeping tasks? */
   if(ciaaPOSIX_counter.isCounting)
   {
      if((ciaaPOSIX_next_to_wake.counter - ciaaPOSIX_counter.counter) > toSleep)
      {
         ciaaPOSIX_next_to_wake.counter = toSleep + ciaaPOSIX_counter.counter;
      }
   }
   /* So, there is a sleeping task now */
   else
   {
      ciaaPOSIX_counter.isCounting = 1;

      ciaaPOSIX_next_to_wake.counter = toSleep + ciaaPOSIX_counter.counter;
   }

   /* Get task id */
   GetTaskID(&taskID);

   /* Set sleeping state for this task */
   ciaaPOSIX_sleeps[taskID].isCounting = 1;

   /* Store counts */
   ciaaPOSIX_sleeps[taskID].counter = (toSleep + ciaaPOSIX_counter.counter);

   /* wait for the posix event */
   WaitEvent(POSIXE);
   ClearEvent(POSIXE);
}

/*==================[external functions definition]==========================*/
extern uint32_t ciaaPOSIX_sleep(uint32_t seconds)
{
   uint32_t toSleep;

   /* ensure that the seconds can be stored in 10ms counts */
   ciaaPOSIX_assert((uint32_t)MAX_SECONDS > seconds);

   /* store the sleep time in 10ms counts */
   toSleep = seconds * SLEEP_TIME_TO_COUNTS;

   /* sleep time processing*/
   ciaaPOSIX_sleepAlgorithm(toSleep);

   return 0;
}

extern int32_t ciaaPOSIX_usleep(useconds_t useconds)
{
   int32_t ret = 0;
   uint32_t toSleep;

#if (CIAAPOSIX_DEBUG == CIAAPOSIX_ENABLE)
   if (1000000 <= useconds)
   {
      ret = -1;
   }
   else
#endif
   {
      if(MAX_USECONDS > useconds)
      {
         /* calculate how many main function cycles shall be sleep */
         toSleep = (useconds + (CIAAPOSIX_MAINFUNCTION_PERIODUS-1))
         / CIAAPOSIX_MAINFUNCTION_PERIODUS;

         /* sleep time processing */
         ciaaPOSIX_sleepAlgorithm(toSleep);
      }
      else
      {
         ret = -1;
      }
   }

   return ret;
}

extern void ciaaPOSIX_sleepMainFunction(void)
{
   uint32_t taskID;
   uint32_t aux_distance = MAX_COUNTS;

   /* Are there sleeping tasks? */
   if(ciaaPOSIX_counter.isCounting)
   {
      /* Increment counter */
      ciaaPOSIX_counter.counter++;

      /* Is it time to wake up? */
      if(ciaaPOSIX_next_to_wake.counter == ciaaPOSIX_counter.counter)
      {
         /* Explore the task array to find tasks to wake up \
            and the next counts to wake up if any */
         for(taskID = 0; TASKS_COUNT > taskID; taskID++)
         {
            if (ciaaPOSIX_sleeps[taskID].isCounting)
            {
               if(ciaaPOSIX_counter.counter == ciaaPOSIX_sleeps[taskID].counter)
               {
                  ciaaPOSIX_sleeps[taskID].isCounting = 0;
                  SetEvent(taskID, POSIXE);
               }
               else
               {
                  if(aux_distance > (ciaaPOSIX_sleeps[taskID].counter - ciaaPOSIX_counter.counter))
                  {
                     aux_distance = ciaaPOSIX_sleeps[taskID].counter - ciaaPOSIX_counter.counter;
                     ciaaPOSIX_next_to_wake.counter = ciaaPOSIX_sleeps[taskID].counter;
                  }
               }
            }
         }
         /* there are not sleeping tasks */
         if (MAX_COUNTS == aux_distance)
         {
            ciaaPOSIX_counter.isCounting = 0;
         }
      }
   }

} /* end of ciaaPOSIX_sleepMainFunction */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

