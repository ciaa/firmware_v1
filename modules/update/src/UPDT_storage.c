/* Copyright 2015, Daniel Cohen
 * Copyright 2015, Esteban Volentini
 * Copyright 2015, Matias Giori
 * Copyright 2015, Franco Salinas
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

/** \brief This file implements the Flash Update Storage functionality
 **
 ** This file implements the functionality of the Flash Update Storage
 **
 ** \todo remove ciaaPOSIX_malloc call from init
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Update CIAA Update Storage
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DC           Daniel Cohen
 * EV           Esteban Volentini
 * MG           Matias Giori
 * FS           Franco Salinas
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150419 v0.0.2  FS  change prefixes. modify API
 * 20150408 v0.0.1  FS  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_assert.h"
#include "ciaaPOSIX_stdlib.h"
#include "UPDT_storage.h"

/*==================[macros and definitions]=================================*/
typedef struct
{
   int32_t fd;
   uint8_t *block_state;
   uint32_t block_count;
   size_t block_size;
} UPDT_storageType;
/*==================[internal data declaration]==============================*/

uint32_t UPDT_storage_init_flag = 0;
UPDT_storageType UPDT_storage;
/*==================[internal functions declaration]=========================*/
static size_t UPDT_storageBlockStateSize(uint32_t block_count)
{
   ciaaPOSIX_assert(0 != UPDT_storage_init_flag);
   /* 1 bit per block */
   return (block_count >> 3) + ((block_count & 0x07) != 0);
}
static void UPDT_storageUnmarkAll()
{
   int block_num;
   int last_block;

   ciaaPOSIX_assert(0 != UPDT_storage_init_flag);
   last_block = UPDT_storageBlockStateSize(UPDT_storage.block_count);
   for(block_num = 0; block_num < last_block; ++block_num)
   {
      UPDT_storage.block_state[block_num] = 0;
   }
}
static void UPDT_storageMark(int block_num)
{
   ciaaPOSIX_assert(0 != UPDT_storage_init_flag);

   ciaaPOSIX_assert( block_num < UPDT_storage.block_count);
   UPDT_storage.block_state[block_num >> 3] |= 1 << (block_num & 0x07);
}
static int UPDT_storageIsMarked(int block_num)
{
   ciaaPOSIX_assert(0 != UPDT_storage_init_flag);
   ciaaPOSIX_assert(block_num < UPDT_storage.block_count);

   return UPDT_storage.block_state[block_num >> 3] & (1 << (block_num & 0x07));
}
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void UPDT_storageClear()
{
   ciaaPOSIX_assert(0 != UPDT_storage_init_flag);

   ciaaPOSIX_free(UPDT_storage.block_state);
   UPDT_storage.block_state = NULL;
   UPDT_storage.fd = -1;
   UPDT_storage.block_count = 0;
   UPDT_storage.block_size = 0;

   UPDT_storage_init_flag = 0;
}

int32_t UPDT_storageInit(int32_t fd)
{
   int32_t ret;
   ciaaDevices_blockType blockInfo;

   ciaaPOSIX_assert(0 == UPDT_storage_init_flag);
   ciaaPOSIX_assert(fd >= 0);

   UPDT_storage_init_flag = 1;

   ret = ciaaPOSIX_ioctl(fd, ciaaPOSIX_IOCTL_BLOCK_GETINFO, (void*)&blockInfo);
   ciaaPOSIX_assert(ret >= 0);
   UPDT_storage.block_size = blockInfo.blockSize;
   UPDT_storage.block_count = blockInfo.lastPosition / blockInfo.blockSize;
   UPDT_storage.fd = fd;
   UPDT_storage.block_state = ciaaPOSIX_malloc(UPDT_storageBlockStateSize(UPDT_storage.block_count));
   ciaaPOSIX_assert(NULL != UPDT_storage.block_state);
   UPDT_storageUnmarkAll();

   return 0;
}
ssize_t UPDT_storageWrite(const uint8_t *data, size_t size, uint32_t address)
{
   int32_t ret;
   uint32_t block_address;
   int block_num;
   int last_block;

   ciaaPOSIX_assert(0 != UPDT_storage_init_flag);

   /* erase the blocks to be written */
   last_block = (address + size - 1) / UPDT_storage.block_size;
   for(block_num = address / UPDT_storage.block_size; block_num <= last_block; ++block_num)
   {
      /* if the block was not erased before */
      if(0 == UPDT_storageIsMarked(block_num))
      {
         /* then erase it */

         /* seek to the block address */
         block_address = block_num * UPDT_storage.block_size;
         ret = ciaaPOSIX_lseek(UPDT_storage.fd, block_address, SEEK_SET);
         ciaaPOSIX_assert(block_address == ret);

         /* erase */
         ret = ciaaPOSIX_ioctl(UPDT_storage.fd, ciaaPOSIX_IOCTL_BLOCK_ERASE, NULL);
         ciaaPOSIX_assert(-1 != ret);

         /* mark it as dirty */
         UPDT_storageMark(block_num);
      }
   }

   /* seek to the specified address */
   ret = ciaaPOSIX_lseek(UPDT_storage.fd, address, SEEK_SET);
   ciaaPOSIX_assert(address == ret);

   /* write in the specified address */
   ret = ciaaPOSIX_write(UPDT_storage.fd, data, size);
   ciaaPOSIX_assert(size == ret);

   return ret;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
