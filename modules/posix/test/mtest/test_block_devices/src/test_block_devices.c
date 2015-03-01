/* Copyright 2015, Mariano Cerdeiro
 * All rights reserved
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

/** \brief Test Block Devices header file
 **
 ** This is the module test of Block Devices CIAA Firmware
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup MTests CIAA Firmware Module Tests
 ** @{ */
/** \addtogroup Block Block Devices Tests
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150130 v0.0.1 MaCe Initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"                  /* <= operating system header */
#include "ciaaPOSIX_stdio.h"     /* <= device handler header */
#include "ciaaPOSIX_string.h"    /* <= string header */
#include "ciaak.h"               /* <= ciaa kernel header */
#include "test_block_devices.h"  /* <= own header */

/*==================[macros and definitions]=================================*/
#define ASSERT(cond) assert((cond), __FILE__, __LINE__)
#define ASSERT_MSG(cond, msg) assert_msg((cond), (msg), __FILE__, __LINE__)
#define ASSERT_SEQ(seq) assert_seq((seq), __FILE__, __LINE__)

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/* buffer to store 10 blocks of the block device */
uint8_t buffer[10][512];

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
      ciaaPOSIX_printf("ERROR: Assert Failed in %s:%d\n", file, line);
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
   int32_t filedes1;
   ciaaDevices_blockType blockInfo;

   int32_t ret;

   /* init CIAA kernel and devices */
   ciaak_start();

   /* print message (only on x86) */
   ciaaPOSIX_printf("Init Task...\n");


   ASSERT_SEQ(0);

   /* open a block device */
   filedes1 = ciaaPOSIX_open("/dev/block/hd/0", O_RDWR);
   ASSERT_MSG(-1 < filedes1, "ciaaPOSIX_open returns an invalid handler");

   ASSERT_SEQ(1);

   /* read block device information */
   ret = ciaaPOSIX_ioctl(filedes1, ciaaPOSIX_IOCTL_GETBLOCKINFO, (void*)&blockInfo);
   ASSERT(1 == ret);
   ASSERT(512 == blockInfo.blockSize);

   ASSERT_SEQ(2);

   /* read a block */
   ret = ciaaPOSIX_read(filedes1, buffer[0], blockInfo.blockSize);
   ASSERT_MSG(blockInfo.blockSize == ret, "Trying to read a block failed");

   ASSERT_SEQ(3);

   /* write a block */
   ret = ciaaPOSIX_write(filedes1, buffer[0], blockInfo.blockSize);
   ASSERT_MSG(blockInfo.blockSize == ret, "Trying to write a block failed");

   ASSERT_SEQ(4);

   /* set position to 0 */
   ret = ciaaPOSIX_seek(filedes1, 0, SEEK_SET);
   ASSERT(0 == ret);

   ASSERT_SEQ(5);

   /* write a block with 0xA5 and the next with 0x5A */
   ciaaPOSIX_memset(buffer[0], 0xA5, sizeof(buffer[0]));
   ciaaPOSIX_memset(buffer[1], 0x5A, sizeof(buffer[1]));
   ret = ciaaPOSIX_write(filedes1, buffer[0], blockInfo.blockSize * 2);
   ASSERT_MSG(ret == blockInfo.blockSize * 2, "Wrong count of bytes have been written");

   ASSERT_SEQ(6);

   /* set position to 0 */
   ret = ciaaPOSIX_seek(filedes1, 0, SEEK_SET);
   ASSERT(0 == ret);

   /* read 2 blocks and compare */
   ciaaPOSIX_memset(buffer[2], 0x00, sizeof(buffer[2]) * 2);
   ret = ciaaPOSIX_read(filedes1, buffer[2], blockInfo.blockSize * 2);
   ASSERT_MSG(ret == blockInfo.blockSize * 2, "Wrong count of bytes have been read");
   ASSERT_MSG(0 == ciaaPOSIX_memcmp(buffer[0], buffer[2], blockInfo.blockSize * 2), "Blocks written and read are not equal");

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
   /* write blinking message */
   ciaaPOSIX_printf("Blinking\n");

   /* terminate task */
   TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

