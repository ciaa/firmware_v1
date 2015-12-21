/* Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, 2015 Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
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

/** \brief Blinking multicore example source file
 **
 ** This is a mini example of the CIAA Firmware using multicore features.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Blinking Blinking multicore example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * PR           Pablo Ridolfi
 * JuCe         Juan Cecconi
 * GMuro        Gustavo Muro
 * ErPe         Eric Pernia
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150831 v0.0.4   PR   first multicore version
 * 20150603 v0.0.3   ErPe change uint8 type by uint8_t
 *                        in line 172
 * 20141019 v0.0.2   JuCe add printf in each task,
 *                        remove trailing spaces
 * 20140731 v0.0.1   PR   first functional version
 */

/*==================[inclusions]=============================================*/
#include "os.h"                 /* <= operating system header */
#include "ciaaPOSIX_stdio.h"    /* <= device handler header */
#include "ciaaPOSIX_string.h"   /* <= string header */
#include "ciaak.h"              /* <= ciaa kernel header */
#include "blinking_multicore.h" /* <= own header */

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
   /* First, start Cortex-M0 slave core.
      Do not forget to create and download an image for this core!
      In Makefile.mine:
         BOARD    ?= edu_ciaa_nxp
         -- or --
         BOARD    ?= ciaa_nxp
      Select the project blinking_multicore:
         PROJECT  ?= examples$(DS)blinking_multicore

      Then build and download:
         $ make mcore
         $ make mcore_download

      Enjoy :-)
   */

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
TASK(InitTaskSlave)
{
   /* init CIAA kernel and devices */
   ciaak_start();

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", ciaaPOSIX_O_RDWR);

   SetRelAlarm(ActivatePeriodicTaskSlave, 0, 500);

   /* terminate task */
   TerminateTask();
}

/** \brief Periodic Task
 *
 * This task is started automatically every time that the alarm
 * ActivatePeriodicTask expires.
 *
 */
TASK(PeriodicTaskSlave)
{
   uint8_t outputs;

   /* read outputs */
   ciaaPOSIX_read(fd_out, &outputs, 1);
   /* set another output to indicate message sending */
   outputs |= 0x10;
   /* write outputs */
   ciaaPOSIX_write(fd_out, &outputs, 1);

   /* turn off in 100ms */
   SetRelAlarm(ActivateLEDOffSlave, 100, 0);

   /* Activate a Task in the remote core */
   ActivateTask(PeriodicTaskMaster);

   /* terminate task */
   TerminateTask();
}

/** \brief Task used to turn off the LED and also send and event
 */
TASK(LEDOffSlave)
{
   uint8_t outputs;
   ciaaPOSIX_read(fd_out, &outputs, 1);
   outputs &= ~0x10;
   ciaaPOSIX_write(fd_out, &outputs, 1);

   TerminateTask();
}

/** \brief Event Task
 *
 * This task waits for an event raised in the remote core.
 *
 */
TASK(EventTaskSlave)
{
   uint8_t outputs;

   WaitEvent(RemoteEvent);
   ClearEvent(RemoteEvent);

   /* read outputs */
   ciaaPOSIX_read(fd_out, &outputs, 1);
   /* blink output */
   outputs ^= 0x08;
   /* write outputs */
   ciaaPOSIX_write(fd_out, &outputs, 1);

   /* terminate task */
   ChainTask(EventTaskSlave);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

