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
 ** by the protocol until there is a full block to write or until the current
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
 * 20150419 v0.0.3  FS  change prefixes
 * 20150408 v0.0.2  FS  first operating version
 * 20141010 v0.0.1  EV  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_assert.h"
#include "UPDT_protocol.h"
#include "UPDT_slave.h"
#include "UPDT_unpacker.h"
#include "UPDT_config.h"
#include "UPDT_services.h"
#include "UPDT_storage.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
static UPDT_slaveType UPDT_services_slave;
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
int32_t UPDT_servicesStart(
   UPDT_ITransportType *transport,
   int32_t flash_fd
)
{
   /* unpacker instance reference */
   UPDT_unpackerType *unpacker;
   /* pointer to the unparsed data in the buffer */
   const uint8_t *unparsed_data;
   /* installation allowed flag */
   int32_t installation_allowed = 0;
   /* number of bytes received */
   ssize_t size;
   /* number of unparsed bytes in the buffer */
   size_t unparsed_bytes_count;
   /* buffer to hold the packets */
   uint8_t buffer[UPDT_PROTOCOL_PACKET_MAX_SIZE];


   ciaaPOSIX_assert(-1 != flash_fd);
   ciaaPOSIX_assert(NULL != transport);

   /* initialize a slave instance */
   ciaaPOSIX_assert(0 == UPDT_slaveInit(&UPDT_services_slave, transport));
   /* create an unpacker instance */
   unpacker = UPDT_unpackerNew();
   ciaaPOSIX_assert(NULL != unpacker);
   /* initialize a storage instance */
   ciaaPOSIX_assert(0 == UPDT_storageInit(flash_fd));

#if 0
   /* device identification */
   do
   {
      /* receive image description from the master */
      /* if there is a connection error */
      if((size = UPDT_slaveRecvInfo(&UPDT_services_slave, buffer, sizeof(buffer))) < 0)
      {
         /* then notify and retry */
         /** \todo notify the CIAA user through a console message */
         continue;
      }

      /* check if it is compatible with the current installation */
      installation_allowed = UPDT_configSet(buffer, size) == 0;

      /* then create a response payload */
      size = UPDT_configSetResponse(buffer, sizeof(buffer));

      /* send the response */
      if(0 != installation_allowed)
         UPDT_slaveSendAllow(&UPDT_services_slave, buffer, size);
      else
         UPDT_slaveSendDeny(&UPDT_services_slave, buffer, size);

   } while(installation_allowed == 0);
#else

   /* preset the sequence number */
   /* this should be done during the handshake */
   UPDT_services_slave.sequence_number = 0;
#endif

   /* receive binary image data */
   while((size = UPDT_slaveRecvData(&UPDT_services_slave, buffer, sizeof(buffer))) > 0)
   {
      /** \todo decrypt */

      /* point to the beginning of the buffer */
      unparsed_data = buffer;
      /* data_size is unsigned */
      unparsed_bytes_count = (size_t) size;

      /* extract all the segments from the received packet */
      do
      {
         unparsed_data = UPDT_unpackerExtract(unpacker, unparsed_data, &unparsed_bytes_count);
         UPDT_storageWrite(unpacker->start, unpacker->size, unpacker->address);
      } while(unparsed_bytes_count > 0);
      ciaaPOSIX_assert(0 == unparsed_bytes_count);
   }
   ciaaPOSIX_assert(0 == size);

   /** \todo signature verification */

   /** \todo add success checking */
   /* if verified */
   /* if ... */
      /* store the new configuration */
      UPDT_configStore();

   /* delete storage instance */
   UPDT_storageClear();
   /* delete unpacker instance */
   UPDT_unpackerDel(unpacker);
   /* clear slave instance */
   UPDT_slaveClear(&UPDT_services_slave);
   return 0;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
