/* Copyright 2016, Mariano Cerdeiro
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

/*==================[internal data definition]===============================*/
uint32_t ciaaPOSIX_sleeps[TASKS_COUNT] = {0};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern uint32_t ciaaPOSIX_sleep(uint32_t seconds)
{
   uint32_t toSleep;
   TaskType taskID;

   /* ensure that the seconds can be stored in 10ms counts */
   ciaaPOSIX_assert(42949672U > seconds);

   toSleep = seconds * 100;
   GetTaskID(&taskID);

   ciaaPOSIX_sleeps[taskID] = toSleep;

   /* wait for the posix event */
   WaitEvent(POSIXE);
   ClearEvent(POSIXE);

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

      GetTaskID(&taskID);

      ciaaPOSIX_sleeps[taskID] = toSleep;

      /* wait for the posix event */
      WaitEvent(POSIXE);
      ClearEvent(POSIXE);
   }

   return ret;
}

extern void ciaaPOSIX_sleepMainFunction(void)
{
   uint32_t taskID;

   /* TODO the implementation is not high performance, :( every
    * time it will be checked for each task if the delay has expire
    * inlusive if no task has call sleep... :( */
   for(taskID = 0; TASKS_COUNT > taskID; taskID++) {
      if (0 < ciaaPOSIX_sleeps[taskID])
      {
         if (CIAAPOSIX_MAINFUNCTION_PERIODMS < ciaaPOSIX_sleeps[taskID])
         {
            ciaaPOSIX_sleeps[taskID] -= CIAAPOSIX_MAINFUNCTION_PERIODMS;
         }
         else
         {
            ciaaPOSIX_sleeps[taskID] = 0;
            SetEvent(taskID, POSIXE);
         }
      }
   }
} /* end of ciaaPOSIX_sleepMainFunction */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

