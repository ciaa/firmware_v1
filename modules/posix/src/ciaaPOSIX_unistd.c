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
/** \brief ciaaPOSIX_sleepAlgorithm
 **
 ** Sleeps the calling application and reload the global counter if required
 **
 ** \param[in] toSleep counts to sleep the execution of the calling task.
 **
 **/
void ciaaPOSIX_sleepAlgorithm(uint32_t toSleep);

/*==================[internal data definition]===============================*/
uint32_t ciaaPOSIX_counter = 0;
int32_t ciaaPOSIX_sleeps[TASKS_COUNT] = {-1};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void ciaaPOSIX_sleepAlgorithm(uint32_t toSleep)
{
   TaskType taskID;

   if(ciaaPOSIX_counter)
   {
      if(toSleep < ciaaPOSIX_counter)
      {
         for(taskID = 0; TASKS_COUNT > taskID; taskID++)
         {
            if(ciaaPOSIX_sleeps[taskID] > -1)
               ciaaPOSIX_sleeps[taskID] += (int32_t)(ciaaPOSIX_counter - toSleep);
         }

         ciaaPOSIX_counter = toSleep;
      }
   }
   else
   {
      ciaaPOSIX_counter = toSleep;
   }

   GetTaskID(&taskID);

   ciaaPOSIX_sleeps[taskID] = (int32_t)(toSleep - ciaaPOSIX_counter);

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
   ciaaPOSIX_assert(21474836U > seconds);

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

   if(0 < ciaaPOSIX_counter)
   {
      if(CIAAPOSIX_MAINFUNCTION_PERIODMS < ciaaPOSIX_counter)
      {
         ciaaPOSIX_counter -= CIAAPOSIX_MAINFUNCTION_PERIODMS;
      }
      else
      {
         ciaaPOSIX_counter = 2147483600U;

         for(taskID = 0; TASKS_COUNT > taskID; taskID++)
         {
            if (0 == ciaaPOSIX_sleeps[taskID])
            {
               ciaaPOSIX_sleeps[taskID] = -1;
               SetEvent(taskID, POSIXE);
            }
            else if(0 < ciaaPOSIX_sleeps[taskID])
            {
               if(ciaaPOSIX_counter > ciaaPOSIX_sleeps[taskID])
                  ciaaPOSIX_counter = ciaaPOSIX_sleeps[taskID];
            }
         }
         if (2147483600U == ciaaPOSIX_counter)
         {
            ciaaPOSIX_counter = 0;
         }
         else
         {
            for(taskID = 0; TASKS_COUNT > taskID; taskID++)
            {
               if(0 < ciaaPOSIX_sleeps[taskID])
               {
                  ciaaPOSIX_sleeps[taskID] -= ciaaPOSIX_counter;
               }
            }
         }

      }
   }

} /* end of ciaaPOSIX_sleepMainFunction */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

