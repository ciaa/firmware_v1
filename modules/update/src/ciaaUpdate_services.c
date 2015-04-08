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

/** \brief This file implements the Flash Update Services functionality
 **
 ** This file implements the functionality of the Flash Update Services
 **
 ** \todo Add writing address restrictions
 ** \todo Implement a write optimization method which buffers the data sent
 ** by the protocol until there is a full block to write or the current
 ** data segment finishes.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Service
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
 * 20150408 v0.0.2  FS  first operating version
 * 20141010 v0.0.1  EV  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_assert.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaUpdate_services.h"
#include "ciaaUpdate_protocol.h"
#include "ciaaUpdate_server.h"
#include "ciaaUpdate_unpacker.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/** \brief flash file descriptor */
static int32_t ciaaUpdate_services_flash_fd = -1;
static uint8_t *ciaaUpdate_services_dirty_blocks;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void ciaaUpdate_servicesDirtyBlocksClearAll()
{
   int i;
   for(i = 0; i < sizeof(ciaaUpdate_services_dirty_blocks); ++i)
   {
      ciaaUpdate_services_dirty_blocks = 0;
   }
}
static void ciaaUpdate_servicesDirtyBlockMark(int address)
{
   int block_num = address / CIAADRVFLASH_BLOCK_SIZE;

   ciaaUpdate_services_dirty_blocks[block_num >> 3] |= 1 << (block_num & 3);
}
static int ciaaUpdate_servicesDirtyBlockGet(int address)

   int block_num = address / CIAADRVFLASH_BLOCK_SIZE;

   return ciaaUpdate_services_dirty_blocks[block_num >> 3] & (1 << (block_num & 3));
}
static ssize_t ciaaUpdate_servicesStore(const void *data, size_t size, uint32_t address)
{
   int32_t ret;
   ciaaPOSIX_assert(-1 != ciaaUpdate_services_flash_fd);

   /* seek to the specified address */
   ret = ciaaPOSIX_lseek(ciaaUpdate_services_flash_fd, address, SEEK_SET);
   ciaaPOSIX_assert(address == ret);

   /* if the block was not written */
   if(0 == ciaaUpdate_servicesDirtyBlockGet(address))
   {
      /* then erase it */
      ret = ciaaPOSIX_ioctl(ciaaUpdate_services_flash_fd, ciaaPOSIX_IOCTL_BLOCK_ERASE, NULL);
      ciaaPOSIX_assert(-1 != ret);

      /* mark it as dirty */
      ckaaUpdate_servicesDirtyBlockMark(address);
   }

   /* write in the specified address */
   ret = ciaaPOSIX_write(ciaaUpdate_services_flash_fd, data, size);
   ciaaPOSIX_assert(size == ret);

   return ret;
}
/*==================[external functions definition]==========================*/
int32_t ciaaUpdate_servicesStart(
   ciaaUpdate_transportType *transport,
   int32_t flash_fd
)
{
   /* unpacker instance reference */
   ciaaUpdate_unpackerType *unpacker;
   /* server instance reference */
   ciaaUpdate_serverType *server;
   /* pointer to the unparsed data in the buffer */
   const uint8_t *unparsed_data;
   /* number of bytes received */
   ssize_t size;
   /* number of unparsed bytes in the buffer */
   size_t unparsed_bytes_count;
   /* buffer to hold the packets */
   uint8_t buffer[CIAAUPDATE_PROTOCOL_PACKET_MAX_SIZE];

   ciaaUpdate_services_flash_fd = flash_fd;
   ciaaPOSIX_assert(-1 != ciaaUpdate_services_flash_fd);


   ciaaUpdate_services_diry_block = malloc([CIAADRVFLASH_BLOCK_CANT >> 3]
   /* mark all flash blocks as not dirty */
   ciaaUpdate_servicesDirtyBlocksClearAll();

   /* create a server instance */
   server = ciaaUpdate_serverNew(transport);
   ciaaPOSIX_assert(NULL != server);
   /* create an unpacker instance */
   unpacker = ciaaUpdate_unpackerNew();
   ciaaPOSIX_assert(NULL != unpacker);
   /* TODO: device identification */
   while((size = ciaaUpdate_serverRecvData(server, buffer)) > 0)
   {
      /* TODO: decrypt */
      /* point to the beginning of the buffer */
      unparsed_data = buffer;
      /* data_size is unsigned */
      unparsed_bytes_count = (size_t) size;

      /* extract all the segments from the received packet */
      do
      {
         unparsed_data = ciaaUpdate_unpackerExtract(unpacker, unparsed_data, &unparsed_bytes_count);
         ciaaUpdate_servicesStore(unpacker->start, unpacker->size, unpacker->address);
      } while(unparsed_bytes_count > 0);
      ciaaPOSIX_assert(0 == unparsed_bytes_count);
   }
   ciaaPOSIX_assert(0 == size);

   /* TODO: signature verification */

   /* delete unpacker instance */
   ciaaUpdate_unpackerDel(unpacker);
   /* delete server instance */
   ciaaUpdate_serverDel(server);

   ciaaUpdate_services_flash_fd = -1;
   return 0;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
