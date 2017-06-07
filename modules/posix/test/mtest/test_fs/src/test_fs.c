/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

/** \brief Test file system source file
 **
 ** This is the module test of file system CIAA Firmware
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup MTests CIAA Firmware Module Tests
 ** @{ */
/** \addtogroup filesystem Tests
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MZ           Marcos Ziegler
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160101 v0.0.1 MZ Initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"                  /* <= operating system header */
#include "ciaaPOSIX_stdio.h"     /* <= device handler header */
#include "ciaaPOSIX_string.h"    /* <= string header */
#include "ciaak.h"               /* <= ciaa kernel header */
#include "ciaaBlockDevices.h"
#include "test_fs.h"  /* <= own header */
#include "ext2.h"

/*==================[macros and definitions]=================================*/
#define ASSERT(cond) assert((cond), __FILE__, __LINE__)
#define ASSERT_MSG(cond, msg) assert_msg((cond), (msg), __FILE__, __LINE__)
#define ASSERT_SEQ(seq) assert_seq((seq), __FILE__, __LINE__)

#define TEST_BUFFER_SIZE 1024

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static void test_fill_buffer(uint8_t *buffer, uint16_t size);
static int test_check_buffer(uint8_t *buffer, uint16_t size);

/*==================[internal data definition]===============================*/
/* buffer to store 10 blocks of the block device */
uint8_t buffer[TEST_BUFFER_SIZE];

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

TASK(InitTask)
{

   int32_t j;
   int32_t fd0, fd1, fd2, fd3;
   uint8_t buffer[TEST_BUFFER_SIZE];
   ext2_format_param_t format_parameters;

   int32_t ret;
   uint32_t lret;

   /* init CIAA kernel and devices */
   ciaak_start();
   ret=ciaaFS_init();
   ASSERT_MSG(-1 != ret, "VFS initialization failed");
   /* print message (only on x86) */
   ciaaPOSIX_printf("Init Task...\n");

   /* Basic test
    * This secuence test the basic operation of flash device.
    * Open the device, get info block, erase a block and verify or cleared,
    * write a block, read this block and verify the data read is same as writed
    * finally close the device
    */
   ASSERT_SEQ(0);
   /* format */
   /* Set ext2 format parameters */
   format_parameters.partition_size = 8*1024*1024;
   format_parameters.block_size = 2048;
   format_parameters.block_node_factor = 4;
   ret = ciaaFS_format("/dev/block/fd/0", "EXT2", &format_parameters);
   ASSERT_MSG(-1 < ret, "Problem formatting device");

   ASSERT_SEQ(1);

   /* mount */
   ret = ciaaFS_mkdir("/mount", 0);
   ASSERT_MSG(-1 < ret, "Trying to create /mount failed");

   ASSERT_SEQ(2);

   ret = ciaaFS_mount("/dev/block/fd/0", "/mount/ext2", "EXT2");
   ASSERT_MSG(-1 < ret, "Problem mounting directory");
   ASSERT_SEQ(3);

   ret = ciaaFS_mkdir("/mount/ext2/dir0", 0);
   ASSERT_MSG(-1 < ret, "Trying to create /mount/ext2/dir0 failed");
   ret = ciaaFS_mkdir("/mount/ext2/dir1", 0);
   ASSERT_MSG(-1 < ret, "Trying to create /mount/ext2/dir1 failed");
   ret = ciaaFS_mkdir("/mount/ext2/dir2", 0);
   ASSERT_MSG(-1 < ret, "Trying to create /mount/ext2/dir2 failed");
   ret = ciaaFS_mkdir("/mount/ext2/dir2/dir3", 0);
   ASSERT_MSG(-1 < ret, "Trying to create /mount/ext2/dir2/dir3 failed");

   ASSERT_SEQ(4);

   /* Show actual vfs tree */
   ret=vfs_print_tree();

   ASSERT_SEQ(5);
   /* Fixme: Duplicated file in ext2. Error not handled */
   fd0 = ciaaFS_open("/mount/ext2/file0", VFS_O_CREAT);
   ciaaPOSIX_printf("fd0: %d\n", fd0);
   ASSERT_MSG(-1 < fd0, "Trying to open file0 failed");
   fd1 = ciaaFS_open("/mount/ext2/file1", VFS_O_CREAT);
   ciaaPOSIX_printf("fd1: %d\n", fd1);
   ASSERT_MSG(-1 < fd1, "Trying to open file1 failed");
   ret=vfs_print_tree();

   ASSERT_SEQ(6);

   fd2 = ciaaFS_open("/mount/ext2/dir2/file2", VFS_O_CREAT);
   ASSERT_MSG(-1 < fd2, "Trying to open file2 failed");
   fd3 = ciaaFS_open("/mount/ext2/dir2/file3", VFS_O_CREAT);
   ASSERT_MSG(-1 < fd3, "Trying to open file3 failed");
   ret = ciaaFS_close(fd0);
   ASSERT_MSG(-1 < ret, "Trying to close /mount/ext2/file0 failed");
   ret = ciaaFS_close(fd1);
   ASSERT_MSG(-1 < ret, "Trying to close /mount/ext2/file1 failed");
   ret = ciaaFS_close(fd2);
   ASSERT_MSG(-1 < ret, "Trying to close /mount/ext2/dir2/file2 failed");
   ret = ciaaFS_close(fd3);
   ASSERT_MSG(-1 < ret, "Trying to close /mount/ext2/dir2/file3 failed");

   ASSERT_SEQ(7);

   fd0 = ciaaFS_open("/mount/ext2/file0", 0);
   ASSERT_MSG(-1 < fd0, "Trying to open file0 for read failed");
   test_fill_buffer(buffer, TEST_BUFFER_SIZE);
   lret = ciaaFS_write(fd0, buffer, TEST_BUFFER_SIZE);
   ASSERT_MSG(lret==TEST_BUFFER_SIZE, "Trying to write to file0 failed");
   ret = ciaaFS_close(fd0);
   ASSERT_MSG(-1 < ret, "Trying to close /mount/ext2/file0 failed");
   fd0 = ciaaFS_open("/mount/ext2/file0", 0);
   ASSERT_MSG(-1 < fd0, "Trying to open file0 for read failed");
   ciaaPOSIX_memset(buffer, 0, TEST_BUFFER_SIZE);
   lret = ciaaFS_read(fd0, buffer, TEST_BUFFER_SIZE);
   ASSERT_MSG(lret==TEST_BUFFER_SIZE, "Trying to read file0 failed");
   ret = test_check_buffer(buffer, TEST_BUFFER_SIZE);
   ASSERT_MSG(lret==TEST_BUFFER_SIZE, "read data corrupt");
   lret = ciaaFS_read(fd0, buffer, TEST_BUFFER_SIZE);
   ASSERT_MSG(lret==0, "Read beyond file0 limits. Implementation error");

   ASSERT_SEQ(8);

   ret=vfs_print_tree();
   ret = ciaaFS_close(fd0);
   ASSERT_MSG(0 == ret, "Trying to close fd0 failed");
   ret = ciaaFS_unlink("/mount/ext2/dir2/file2"); /* FIXME: Causes file0 to become bad type. /mount/ext2 entry corrupted */
                                                  /* Root entries in block 12 */
   ASSERT_MSG(-1 < ret, "Trying to unlink file2 failed");
   ret = ciaaFS_unlink("/mount/ext2/dir2/file3");
   ASSERT_MSG(-1 < ret, "Trying to unlink file3 failed");
   fd0 = ciaaFS_open("/mount/ext2/dir2/file4", VFS_O_CREAT);
   ASSERT_MSG(-1 < fd0, "Trying to create file4 failed");
   fd1 = ciaaFS_open("/mount/ext2/dir2/file5", VFS_O_CREAT);
   ASSERT_MSG(-1 < fd1, "Trying to create file5 failed");
   ret=vfs_print_tree();

   ASSERT_SEQ(9);

   fd3 = ciaaFS_open("/mount/ramfile0", VFS_O_CREAT);
   ASSERT_MSG(-1 < fd3, "Trying to create ramfile0 for read failed");
   test_fill_buffer(buffer, TEST_BUFFER_SIZE);
   lret = ciaaFS_write(fd3, buffer, TEST_BUFFER_SIZE);
   ASSERT_MSG(lret == TEST_BUFFER_SIZE, "Trying to write to ramfile0 failed");
   ciaaPOSIX_memset(buffer, 0, TEST_BUFFER_SIZE);
   lret = ciaaFS_lseek(fd3, 0, SEEK_SET);
   ASSERT_MSG(lret == 0, "Trying to set ramfile0 position failed");
   ciaaPOSIX_printf("ciaaFS_lseek: Expected to return %lu, actually return %lu\n", 0, lret);
   lret = ciaaFS_read(fd3, buffer, TEST_BUFFER_SIZE);
   ASSERT_MSG(lret == TEST_BUFFER_SIZE, "Trying to read ramfile0 failed");
   ciaaPOSIX_printf("ciaaFS_read: Expected to return %lu, actually return %lu\n", TEST_BUFFER_SIZE, lret);
   ret = test_check_buffer(buffer, TEST_BUFFER_SIZE);
   ciaaPOSIX_printf("%s\n", buffer);
   ret = ciaaFS_close(fd3);

   ShutdownOS(E_OK);

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

static void test_fill_buffer(uint8_t *buffer, uint16_t size)
{
   uint16_t i;

   for(i=0; i<size; i++)
   {
      buffer[i] = i%10 + 0x30;   /* ASCII desde 0 a 9 */
   }
}

static int test_check_buffer(uint8_t *buffer, uint16_t size)
{
   uint16_t i;
   ciaaPOSIX_printf( "test_assert_buffer(): Differences\n",
                     "buff\t\tseq\n");
   for(i=0; i<size; i++)
   {
      if(buffer[i] != i%10 + 0x30)
      {
         //ciaaPOSIX_printf("%c\t\t%c\n",buffer[i],i%10 + 0x30);
      }
   }
   if(i==size)
   {
   ciaaPOSIX_printf( "test_assert_buffer(): OK, no differences\n");
   }
   return 0;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

