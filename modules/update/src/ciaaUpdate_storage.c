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
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Storage
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
 * 20150408 v0.0.1  FS  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_assert.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaUpdate_storage.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static size_t ciaaUpdate_storageBlockStateSize(uint32_t block_count)
{
   /* 1 bit per block */
   return (block_count >> 3) + ((block_count & 0x07) != 0);
}
static void ciaaUpdate_storageUnmarkAll(ciaaUpdate_storageType *storage)
{
   int block_num;
   int last_block;

   ciaaPOSIX_assert(NULL != storage);
   last_block = ciaaUpdate_storageBlockStateSize(storage->block_count);
   for(block_num = 0; block_num < last_block; ++block_num)
   {
      storage->block_state[block_num] = 0;
   }
}
static void ciaaUpdate_storageMark(ciaaUpdate_storageType *storage, int block_num)
{
   ciaaPOSIX_assert(NULL != storage && block_num < storage->block_count);
   storage->block_state[block_num >> 3] |= 1 << (block_num & 0x07);
}
static int ciaaUpdate_storageIsMarked(ciaaUpdate_storageType *storage, int block_num)
{
   ciaaPOSIX_assert(NULL != storage && block_num < storage->block_count);

   return storage->block_state[block_num >> 3] & (1 << (block_num & 0x07));
}
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void ciaaUpdate_storageDel(ciaaUpdate_storageType *storage)
{
   ciaaPOSIX_assert(NULL != storage && storage->fd >= 0 && NULL != storage->block_state);

   ciaaPOSIX_free(storage->block_state);
   storage->block_state = NULL;
   storage->fd = -1;
   storage->block_count = 0;
   storage->block_size = 0;
   ciaaPOSIX_free(storage);
}
ciaaUpdate_storageType *ciaaUpdate_storageNew(int32_t fd)
{
   ciaaUpdate_storageType *storage;
   int32_t ret;
   ciaaDevices_blockType blockInfo;

   ciaaPOSIX_assert(fd >= 0);

   storage = ciaaPOSIX_malloc(sizeof(ciaaUpdate_storageType));
   ciaaPOSIX_assert(NULL != storage);

   ret = ciaaPOSIX_ioctl(fd, ciaaPOSIX_IOCTL_BLOCK_GETINFO, (void*)&blockInfo);
   ciaaPOSIX_assert(ret >= 0);
   storage->block_size = blockInfo.blockSize;
   storage->block_count = blockInfo.lastPosition / blockInfo.blockSize;
   storage->fd = fd;
   storage->block_state = ciaaPOSIX_malloc(ciaaUpdate_storageBlockStateSize(storage->block_count));
   ciaaPOSIX_assert(NULL != storage->block_state);

   ciaaUpdate_storageUnmarkAll(storage);
   return storage;
}
ssize_t ciaaUpdate_storageStore(ciaaUpdate_storageType *storage, const void *data, size_t size, uint32_t address)
{
   int32_t ret;
   uint32_t block_address;
   int block_num;
   int last_block;
   ciaaPOSIX_assert(NULL != storage && storage->fd >= 0 && NULL != storage->block_state);

   /* erase the blocks to be written */
   last_block = (address + size - 1) / storage->block_size;
   for(block_num = address / storage->block_size; block_num <= last_block; ++block_num)
   {
      /* if the block was not erased before */
      if(0 == ciaaUpdate_storageIsMarked(storage, block_num))
      {
         /* then erase it */

         /* seek to the block address */
         block_address = block_num * storage->block_size;
         ret = ciaaPOSIX_lseek(storage->fd, block_address, SEEK_SET);
         ciaaPOSIX_assert(block_address == ret);

         /* erase */
         ret = ciaaPOSIX_ioctl(storage->fd, ciaaPOSIX_IOCTL_BLOCK_ERASE, NULL);
         ciaaPOSIX_assert(-1 != ret);

         /* mark it as dirty */
         ciaaUpdate_storageMark(storage, block_num);
      }
   }

   /* seek to the specified address */
   ret = ciaaPOSIX_lseek(storage->fd, address, SEEK_SET);
   ciaaPOSIX_assert(address == ret);

   /* write in the specified address */
   ret = ciaaPOSIX_write(storage->fd, data, size);
   ciaaPOSIX_assert(size == ret);

   return ret;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

