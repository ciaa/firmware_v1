/* Copyright 2014, Daniel Cohen
 * Copyright 2014, Esteban Volentini
 * Copyright 2014, Matias Giori
 * Copyright 2014, Franco Salinas
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

/** \brief Bootloader source file
 **
 ** This is an bootloader program to implement remote software update.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Bootloader Bootloader source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * EsVo         Esteban Volentini
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20141127 v0.0.2  EsVo  rename to serial echo and clean code
 * 20141006 v0.0.1  EsVo  first initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "serial_echo.h"      /* <= own header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief File descriptor of the USB uart
 *
 * Device path /dev/serial/uart/1
 */
static int32_t fd_uart1;

/** \brief File descriptor of the RS232 uart
 *
 * Device path /dev/serial/uart/2
 */
static int32_t fd_uart2;

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

   ciaaPOSIX_printf("Starting Serial Echo Tasks\r\n");

   /* open UART connected to USB bridge (FT2232) */
   fd_uart1 = ciaaPOSIX_open("/dev/serial/uart/0", O_RDWR);

   /* open UART connected to RS232 connector */
   fd_uart2 = ciaaPOSIX_open("/dev/serial/uart/1", O_RDWR);

   /* Activates the SerialEchoTask task */
   ActivateTask(SerialEchoTask);

   /* end InitTask */
   TerminateTask();
}

/** \brief Serial Echo Task
 *
 * This tasks waits for input data from fd_uart1 and writes the received data
 * to fd_uart1 and fd_uart2. This taks alos blinkgs the output 5.
 *
 */
TASK(SerialEchoTask)
{
   int8_t buf[20];   /* buffer for uart operation              */
   int32_t ret;      /* return value variable for posix calls  */

   /* send a message to the world :) */
   char message[] = "Hi! :)\nSerialEchoTask: Waiting for characters...\n";
   ciaaPOSIX_write(fd_uart1, message, ciaaPOSIX_strlen(message));

   while(1)
   {
      /* wait for any character ... */
      ret = ciaaPOSIX_read(fd_uart1, buf, 19);

      if(ret > 0)
      {
         buf[ret] = 0;
         ciaaPOSIX_printf("Data Read: %s \r\n", buf);
         /* ... and write them to the same device */
         ciaaPOSIX_write(fd_uart1, buf, ret);

         /* also write them to the other device */
         ciaaPOSIX_write(fd_uart2, buf, ret);
      }
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

