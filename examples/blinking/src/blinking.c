/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

/** \brief Blinking example source file
 **
 ** This is a mini example of the CIAA Firmware
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Blinking Blinking example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * yyyymmdd v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaak.h"
#include "blinking.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
int main(void)
{
   StartOS(AppMode1);
   return 0;
}

void ErrorHook(void)
{
   ciaaPOSIX_printf("ErrorHook was called\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());
   ShutdownOS(0);
}

TASK(InitTask) {
   ciaak_start();

   uint8_t val;

   int fd = ciaaPOSIX_open("/dev/dio/out/0", O_RDWR);

   /* Example: Activate output 5 (MOSFET) */
   val = 0x20;
   ciaaPOSIX_write(fd, &val, 1);

   ciaaPOSIX_close(fd);

   ciaaPOSIX_printf("InitTask is running\n");
   ActivateTask(TaskA);

   Schedule();

   ciaaPOSIX_printf("InitTask sets TaskA Event1\n");
   SetEvent(TaskA, Event1);

   ciaaPOSIX_printf("InitTask is Terminating\n");
   TerminateTask();
}

TASK(TaskA) {
   int32_t fildes;

   ciaaPOSIX_printf("TaskA is running\n");

   /* open UART used in USB bridge */
   fildes = ciaaPOSIX_open("/dev/serial/uart/1", O_RDWR);

   ciaaPOSIX_write(fildes, "test", 4);

   ciaaPOSIX_printf("TaskA espera Event1\n");
   WaitEvent(Event1);

   ciaaPOSIX_printf("TaskA recibio la notficacion del Event1\n");
   ActivateTask(TaskB);

   ciaaPOSIX_printf("Pedimos recurso\n");
   GetResource(Res1);

   ciaaPOSIX_printf("Liberamos recurso\n");
   ReleaseResource(Res1);

   ciaaPOSIX_printf("TaskA is Terminating\n");
   TerminateTask();
}

TASK(TaskB) {
   ciaaPOSIX_printf("TaskB is running\n");

   ActivateTask(TaskC);
   ActivateTask(TaskC);

   ciaaPOSIX_printf("Activate Relative Alarm to Activate Task C");
   SetRelAlarm(ActivateTaskC, 200, 250);

   ciaaPOSIX_printf("TaskB is Terminating\n");
   TerminateTask();
}

ISR(IsrName) {
}

TASK(TaskC) {

   /*
    * Example:
    *    Read inputs 0..3, update outputs 0..3.
    *    Blink output 4
    */

   uint8_t inputs = 0, outputs = 0;

   int fd_in = ciaaPOSIX_open("/dev/dio/in/0", O_RDONLY);
   int fd_out = ciaaPOSIX_open("/dev/dio/out/0", O_RDWR);

   ciaaPOSIX_read(fd_in, &inputs, 1);

   ciaaPOSIX_read(fd_out, &outputs, 1);

   outputs &= 0xF0;
   outputs |= inputs & 0x0F;

   if(outputs & 0x10)
   {
      outputs &= ~0x10;
   }
   else
   {
      outputs |= 0x10;
   }

   ciaaPOSIX_write(fd_out, &outputs, 1);

   ciaaPOSIX_close(fd_out);

   ciaaPOSIX_close(fd_in);

   ciaaPOSIX_printf("TaskC is running\n");
   ciaaPOSIX_printf("TaskC is Terminating\n");
   TerminateTask();
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

