/* Copyright 2016, Diego Ezequiel Vommaro
 * Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
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

/** \brief sleeping_example example source file
 **
 ** This is a mini example of the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Sleeping Sleeping_example example source file
 ** @{ */

/*==================[inclusions]=============================================*/
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaaPOSIX_unistd.h" /* <= sleep header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "sleeping_example.h"         /* <= own header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief File descriptor for digital output ports
 *
 * Device path /dev/dio/out/0
 */
static int32_t fd_out;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */
int main(void)
{
   /* Starts the operating system in the Application Mode 1 */
   /* This example has only one Application Mode */
   StartOS(AppMode1);

   /* StartOs shall never returns, but to avoid compiler warnings or errors
    * 0 is returned */
   return 0;
}

/** \brief Error Hook function
 *
 * This fucntion is called from the os if an os interface (API) returns an
 * error. Is for debugging proposes. If called this function triggers a
 * ShutdownOs which ends in a while(1).
 *
 * The values:
 *    OSErrorGetServiceId
 *    OSErrorGetParam1
 *    OSErrorGetParam2
 *    OSErrorGetParam3
 *    OSErrorGetRet
 *
 * will provide you the interface, the input parameters and the returned value.
 * For more details see the OSEK specification:
 * http://portal.osek-vdx.org/files/pdf/specs/os223.pdf
 *
 */
void ErrorHook(void)
{
   ciaaPOSIX_printf("ErrorHook was called\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());
   ShutdownOS(0);
}

/** \brief Initial task
 *
 * This task is started automatically in the application mode 1.
 */
TASK(InitTask)
{
   /* init CIAA kernel and devices */
   ciaak_start();

   /* print message (only on x86) */
   ciaaPOSIX_printf("Init Task...\n");

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", ciaaPOSIX_O_RDWR);

   /* activate periodic task:
    *  - for the first time after 350 ticks (350 ms)
    *  - and then every 10 ticks (10 ms)
    */
   SetRelAlarm(ActivatePeriodicTask, 350, 10);

   /* Activates the Blinking task */
   ActivateTask(BlinkingTask1);

   /* Activates the Blinking task 2 */
   ActivateTask(BlinkingTask2);

   /* Activates the Blinking task 3 */
   ActivateTask(BlinkingTask3);

   /* terminate task */
   TerminateTask();
}

/** \brief Blinking Task 1
 *
 * This task never terminates. Only sleeps and blink a led when wakes up
 *
 */
TASK(BlinkingTask1)
{
   uint8_t outputs;

   while(1)
   {
      /* write blinking message */
      ciaaPOSIX_printf("Blinking\n");

      /* blink red output */
      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= 0x10;
      ciaaPOSIX_write(fd_out, &outputs, 1);

      /* sleep 1 second */
      ciaaPOSIX_sleep(1U);
   }

   /* terminate task */
   TerminateTask();
}

/** \brief Blinking Task 2
 *
 * This task never terminates. Only sleeps and blink a led when wakes up
 *
 */
TASK(BlinkingTask2)
{
   uint8_t outputs;

   while(1)
   {
      /* write blinking message */
      ciaaPOSIX_printf("Blinking2\n");

      /* blink green  output */
      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= 0x20;
      ciaaPOSIX_write(fd_out, &outputs, 1);

      /* sleep 2 second */
      ciaaPOSIX_sleep(2U);
   }

   /* terminate task */
   TerminateTask();
}

/** \brief Blinking Task 3
 *
 * This task never terminates. Only sleeps and blink a led when wakes up
 *
 */
TASK(BlinkingTask3)
{
   uint8_t outputs;

   while(1)
   {
      /* write blinking message */
      ciaaPOSIX_printf("Blinking3\n");

      /* blink green  output */
      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= 0x8;
      ciaaPOSIX_write(fd_out, &outputs, 1);

      /* sleep 2 second */
      ciaaPOSIX_sleep(3U);
   }

   /* terminate task */
   TerminateTask();
}

/** \brief Periodic Task
 *
 * This task is started automatically every time that the alarm
 * ActivatePeriodicTask expires.
 *
 */
TASK(PeriodicTask)
{
   ciaaPOSIX_sleepMainFunction();

   /* terminate task */
   TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

