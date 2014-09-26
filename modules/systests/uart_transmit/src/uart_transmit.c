/* Copyright 2014, Gustavo Muro
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

/** \brief UART Transmit Test source file
 **
 ** This is UART transmit test of the CIAA Firmware
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup SysTest CIAA Firmware System Test
 ** @{ */
/** \addtogroup UartTransmit UART Transmit System Test
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * GMuro        Gustavo Muro
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140804 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "ciaak.h"            /* <= ciaa kernel header */
#include "uart_transmit.h"

/*==================[macros and definitions]=================================*/
#define BUFFER_SIZE  512

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief file descriptor of the uart to write from */
static int32_t fd_uart;
static uint8_t buffer[BUFFER_SIZE];
static const uint8_t pattern[] = "0123456789";

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

TASK(InitTask)
{
   /* init CIAA kernel and devices */
   ciaak_start();

   /* open UART connected to USB bridge (FT2232) */
   fd_uart = ciaaPOSIX_open("/dev/serial/uart/1", O_RDWR);

   /* change baud rate for uart usb */
   ciaaPOSIX_ioctl(fd_uart, ciaaPOSIX_IOCTL_SET_BAUDRATE, (void *)ciaaBAUDRATE_115200);

   /* change FIFO TRIGGER LEVEL for uart usb */
   ciaaPOSIX_ioctl(fd_uart, ciaaPOSIX_IOCTL_SET_FIFO_TRIGGER_LEVEL, (void *)ciaaFIFO_TRIGGER_LEVEL3);

   /* activate periodic tasks */
   SetRelAlarm(ActivatePeriodicTask, 1000, 1000);

   /* end InitTask */
   TerminateTask();
}

TASK(PeriodicTask)
{
   static uint32_t total_transmit = 16;
   uint32_t loopi;
   uint32_t index = 0;

   /* start character */
   buffer[0] = ':';

   /* filla buffer with pattern */
   for (loopi = 1 ; loopi < total_transmit-2 ; loopi++)
   {
      buffer[loopi] = pattern[index];
      index++;
      if (index > 9)
         index = 0;
   }

   /* add CRLF */
   buffer[loopi] = '\r';
   loopi++;
   buffer[loopi] = '\n';
   loopi++;

   /* write to UART */
   ciaaPOSIX_write(fd_uart, buffer, loopi);

   /* intrement total transmit +16 */
   total_transmit += 16;

   if (total_transmit >= BUFFER_SIZE)
      total_transmit = 16;

   TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

