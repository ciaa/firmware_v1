/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * Copyright 2016, Mariano Cerdeiro
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

/** \brief CIAA Semaphores source file
 **
 ** This header file describes the Devices.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */
/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_semaphore.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_assert.h"
#include "Os_Internal.h"
#include "ciaaPOSIX_unistd_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
/** \brief Semaphore Variables
 **
 ** This structure is used to indicate which semaphore is blocking a specific
 ** task. If the pointer is null the task is not beeing blocked by any
 ** semaphore, if there is a pointer the pointer indicates which sem. is
 ** blocking this task.
 **
 **/
sem_t * ciaaPOSIX_semVar[TASKS_COUNT] = { NULL };

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern int8_t ciaaPOSIX_sem_init(sem_t * const sem)
{
   sem->counter = 0;
   return 1;
}

extern int8_t ciaaPOSIX_sem_wait(sem_t * const sem)
{
   TaskType taskID;

   ciaaPOSIX_assert(255 > sem->counter);
   GetTaskID(&taskID);

   GetResource(POSIXR);
   sem->counter++;
   if (1 == sem->counter) {
      ReleaseResource(POSIXR);
   } else {
      ciaaPOSIX_semVar[taskID] = sem;
      ReleaseResource(POSIXR);
      WaitEvent(POSIXE);
      ClearEvent(POSIXE);
   }

   return 0;
}

/** INFO: this interface is an extension */
extern uint32_t ciaaPOSIX_sem_waitfor(sem_t * const sem, uint32_t timeout)
{
   uint32_t retVal = 0;
   uint32_t toSleep;
   TaskType taskID;

   ciaaPOSIX_assert(255 > sem->counter);
   GetTaskID(&taskID);

   GetResource(POSIXR);
   sem->counter++;
   if (1 == sem->counter) {
      ReleaseResource(POSIXR);
   } else {
      ciaaPOSIX_semVar[taskID] = sem;

      /* convert the timout from milliseconds to micro seconds */
      timeout = 1000 * timeout;

      /* calculate how many main function cycles shall be sleep */
      toSleep = (timeout + (CIAAPOSIX_SLEEP_TICK-1))
                / CIAAPOSIX_SLEEP_TICK;

      /* call sleep algorithm */
      ciaaPOSIX_sleepAddTask(toSleep);

      ReleaseResource(POSIXR);
      WaitEvent(POSIXE);
      ClearEvent(POSIXE);

      /* we are awake, there are to options, or a timeout wake us or we
       * have the semaphore let check that */
      GetResource(POSIXR);

      /* if our task ID is still in the list we had a timeout */
      if (ciaaPOSIX_semVar[taskID] == sem) {
         /* remove our task of the waiting list */
         ciaaPOSIX_semVar[taskID] = NULL;

         /* indicate that a timeout occurred */
         retVal = CIAAPOSIX_TIMEOUT;
      } else {
         /* timeout does not occur, we have to remove our task of the timeout list
          * and indicate the elapsed time */

      }

      ReleaseResource(POSIXR);
   }

   return retVal;

}

extern int8_t ciaaPOSIX_sem_post(sem_t * const sem)
{
   TaskType taskID;
   bool found = false;

   /* if the counter is already 0 the user is calling more post than wait for
    * a semaphore... TODO maybe in the future we can improve and report
    * an error here. */
   ciaaPOSIX_assert(0 < sem->counter);

   GetResource(POSIXR);
   sem->counter--;
   if (0 == sem->counter) {
      ReleaseResource(POSIXR);
   } else {
      /* other task is also waiting and shall be activated */
      /* TODO here is a lot of work to do, this can be imporved a lot,
       * e.g. here we are freeing the next task by id but not by priority
       * so this make generate conflicts, neither is we have a first come
       * first serve strategy regarding the sempahore :(. */
      for(taskID = 0; (TASKS_COUNT > taskID) && (false == found); taskID++) {
         if (sem == ciaaPOSIX_semVar[taskID]) {
            ciaaPOSIX_semVar[taskID] = NULL;
            found = true;
         }
      }
      /* if the counter is not 0 but we do not found any task waiting for this
       * semaphore something goes really wrong :(. */
      ciaaPOSIX_assert(true == found);

      /* decrement the counter */
      sem->counter--;
      ReleaseResource(POSIXR);

      SetEvent(taskID-1, POSIXE);
   }

   return 0;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

