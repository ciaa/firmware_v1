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
 ** \bug The last packet is not being stored.
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
#include "ciaaUpdate_storage.h"
#include "ciaaUpdate_unpacker.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
int32_t ciaaUpdate_servicesStart(
   ciaaUpdate_transportType *transport,
   int32_t flash_fd
)
{
   /* server instance reference */
   ciaaUpdate_serverType *server;
   /* unpacker instance reference */
   ciaaUpdate_unpackerType *unpacker;
   /* storage instance reference */
   ciaaUpdate_storageType *storage;
   /* pointer to the unparsed data in the buffer */
   const uint8_t *unparsed_data;
   /* number of bytes received */
   ssize_t size;
   /* number of unparsed bytes in the buffer */
   size_t unparsed_bytes_count;
   /* buffer to hold the packets */
   uint8_t buffer[CIAAUPDATE_PROTOCOL_PACKET_MAX_SIZE];

   ciaaPOSIX_assert(-1 != flash_fd && NULL != transport);

   /* create a server instance */
   server = ciaaUpdate_serverNew(transport);
   ciaaPOSIX_assert(NULL != server);
   /* create an unpacker instance */
   unpacker = ciaaUpdate_unpackerNew();
   ciaaPOSIX_assert(NULL != unpacker);
   /* create a storage instance */
   storage = ciaaUpdate_storageNew(flash_fd);
   ciaaPOSIX_assert(NULL != storage);

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
         ciaaUpdate_storageStore(storage, unpacker->start, unpacker->size, unpacker->address);
      } while(unparsed_bytes_count > 0);
      ciaaPOSIX_assert(0 == unparsed_bytes_count);
   }
   ciaaPOSIX_assert(0 == size);

   /* TODO: signature verification */

   /* delete storage instance */
   ciaaUpdate_storageDel(storage);
   /* delete unpacker instance */
   ciaaUpdate_unpackerDel(unpacker);
   /* delete server instance */
   ciaaUpdate_serverDel(server);
   return 0;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
