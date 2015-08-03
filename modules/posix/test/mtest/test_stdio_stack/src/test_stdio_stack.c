/* Copyright 2014, Mariano Cerdeiro
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

/** \brief Test com stack
 **
 ** This is a complete test of the communication stack
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Test CIAA Firmware Tests
 ** @{ */
/** \addtogroup Com Communication Stack Test
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * yyyymmdd v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaak.h"                   /* <= ciaa kernel header */
#include "ciaaDriverUart_Internal.h"

/*==================[macros and definitions]=================================*/
#define ASSERT(cond) assert((cond), __FILE__, __LINE__)
#define ASSERT_MSG(cond, msg) assert_msg((cond), (msg), __FILE__, __LINE__)
#define ASSERT_SEQ(seq) assert_seq((seq), __FILE__, __LINE__)
/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
uint8_t buffer[1024];
uint8_t testCase = 0;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void assert(int cond, char * file, int line)
{
   if (cond)
   {
      /* assertion is ok */
      ciaaPOSIX_printf("OK: Assert in %s:%d\n", file, line);
   }
   else
   {
      ciaaPOSIX_printf("Assert Failed in %s:%d\n", file, line);
   }
}

static void assert_msg(int cond, char* msg, char * file, int line)
{
   if (cond)
   {
      /* assertion is ok */
      ciaaPOSIX_printf("OK: Assert in %s:%d\n", file, line);
   }
   else
   {
      ciaaPOSIX_printf("ERROR: Assert Failed in %s:%d %s\n", file, line, msg);
   }
}

static void assert_seq(int seq, char * file, int line)
{
   static unsigned int sequence = 0;

   if (sequence == seq)
   {
      ciaaPOSIX_printf("OK: Sequence %d\n", seq);
      sequence++;
   }
   else
   {
      ciaaPOSIX_printf("ERROR: Wrong sequence: %d on %s:%d\n", seq, file, line);
   }
}

void dumpBuffer(uint8_t buffer[]) {
   uint16_t i;

   ciaaPOSIX_printf("===============================================================================\r\n");
   for(i = 0; i < 512; i++)
   {
      ciaaPOSIX_printf("0x%02X ", buffer[i]);
      if (((i + 1) & 0x0F) == 0) ciaaPOSIX_printf("\r\n");
   }

}

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
void UserRxIndication(void) {

}


/*==================[tasks]==================================================*/

TASK(InitTask) {
   ciaak_start();

   ChainTask(TaskA);
}


TASK(TaskA) {
   int32_t fildes1;
   int32_t fildes2;
   int32_t ret;
   int32_t loopi;

   /******* TEST BLOCKING RECEPTION *******/

   ASSERT_SEQ(0);

   /* open an invalid device */
   fildes1 = ciaaPOSIX_open("/dev/serial/uart/23", O_RDWR);
   ASSERT_MSG(-1 == fildes1, "ciaaPOSIX_open returns a valid handler for an invalid device");

   ASSERT_SEQ(1);

   /* open a valid device */
   fildes1 = ciaaPOSIX_open("/dev/serial/uart/0", O_RDWR);
   ASSERT_MSG(0 <= fildes1, "ciaaPOSIX_open returns an invalid handler");

   ASSERT_SEQ(2);

   /* close the opened device */
   ret = ciaaPOSIX_close(fildes1);
   ASSERT_MSG(0 == ret, "ciaaPOSIX_close returns other than 0 for a valid fildes");

   ASSERT_SEQ(3);

   /* close a not opened device */
   ret = ciaaPOSIX_close(fildes1);
   ASSERT_MSG(-1 == ret, "ciaaPOSIX_close returns other than -1 for an invalid fildes");

   ASSERT_SEQ(4);

   /* open a valid device */
   fildes1 = ciaaPOSIX_open("/dev/serial/uart/0", O_RDWR);
   ASSERT_MSG(0 <= fildes1, "ciaaPOSIX_open returns an invalid handler");

   ASSERT_SEQ(5);

   /* indicate taste case to TaskB */
   testCase = 1;
   ActivateTask(TaskB);

   /* read some data knowing that data is not available (task will be blocked) */
   ret = ciaaPOSIX_read(fildes1, buffer, sizeof(buffer));
   ASSERT_MSG(ret == 8, "ciaaPOSIX_read returns with less bytes than expected");

   ASSERT_SEQ(8);

   for(loopi = 0; loopi < 8; loopi++)
   {
      ASSERT(buffer[loopi] == loopi);
   }

   ASSERT_SEQ(9);

   /* indicate reception of 16 bytes */
   ciaaDriverUart_uart0.rxBuffer.length = 16;
   for(loopi = 0; loopi <20; loopi++) {
      ciaaDriverUart_uart0.rxBuffer.buffer[loopi] = loopi + 10;
   }
   ciaaDriverUart_uart0_rxIndication();

   ASSERT_SEQ(10);

   /* read 6 bytes knowing that more data is already available */
   ret = ciaaPOSIX_read(fildes1, buffer, 6);
   ASSERT(6 == ret);

   /* compare received data */
   for(loopi = 0; loopi < 6; loopi++)
   {
      ASSERT(buffer[loopi] == loopi + 10);
   }
   ASSERT_SEQ(11);

   /* read 16 bytes more data */
   ret = ciaaPOSIX_read(fildes1, buffer, 16);
   ASSERT(10 == ret);

   /* compare received data */
   for(loopi = 0; loopi < 10; loopi++)
   {
      ASSERT(buffer[loopi] == loopi + 16);
   }

   ASSERT_SEQ(12);

   /******* TEST BLOCKING TRANSMISSION *******/

   /* set test case 2 for TaskB */
   testCase = 2;
   ASSERT_SEQ(13);

   ASSERT_SEQ(14);

   /* write 16 bytes */
   ret = ciaaPOSIX_write(fildes1, buffer, 16);
   ASSERT(16 == ret);

   ASSERT_SEQ(15);

   /* indicate taste case to TaskB */
   testCase = 2;
   ActivateTask(TaskB);

   /* write 300 bytes (more than fifo size) */
   /* function shall be blocked */
   ret = ciaaPOSIX_write(fildes1, buffer, 300);
   ASSERT(300 == ret);

   ASSERT_SEQ(17);

   fildes2 = ciaaPOSIX_open("/dev/dio/in/0", O_RDWR);
   ASSERT_MSG(0 <= fildes2, "ciaaPOSIX_open returns an invalid handler");

   TerminateTask();
}

TASK(TaskB)
{
   int32_t loopi;

   switch(testCase)
   {
      case 1:
         ASSERT_SEQ(6);
         /* simulates a reception of 8 bytes */
         ciaaDriverUart_uart0.rxBuffer.length = 8;
         for(loopi = 0; loopi < 20; loopi++)
         {
            ciaaDriverUart_uart0.rxBuffer.buffer[loopi] = loopi;
         }
         ciaaDriverUart_uart0_rxIndication();

         ASSERT_SEQ(7);
         break;

      case 2:
         ASSERT_SEQ(16);
         ciaaDriverUart_uart0.txBuffer.length = 0;
         ciaaDriverUart_uart0_txConfirmation();
         break;

      default:
         ASSERT_MSG(1, "Variable testCase contains an invalid value");
         break;
   }


   TerminateTask();
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

