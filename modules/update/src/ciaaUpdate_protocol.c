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

/** \brief This file implements the Flash Update Protocol funcionality
 **
 ** This file implements the funcionality of the Flash Update Protocol
 **
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
 * 20141010 v0.0.1  EV  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_assert.h"
#include "ciaaUpdate_protocol.h"
#include "ciaaUpdate_transport.h"
#include "ciaaUpdate_crypto.h"
#include "ciaaUpdate_utils.h"

/*==================[macros and definitions]=================================*/
#define CIAAUPDATE_PROTOCOL_HEADER_SIZE         4
#define CIAAUPDATE_PROTOCOL_PAYLOAD_MAX_SIZE    248
#define CIAAUPDATE_PROTOCOL_VERSION             0

/* packet types */
#define CIAAUPDATE_PROTOCOL_DATA_PACKET         2
/*==================[internal data declaration]==============================*/
static ciaaUpdate_transportType *ciaaUpdate_protocol_transport = NULL;
static ciaaUpdate_protocolStoreCallback ciaaUpdate_protocol_store_cb = NULL;
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static int8_t ciaaUpdate_protocolHeaderVersion(const uint8_t *header)
{
   ciaaPOSIX_assert(NULL != header);
   return (header[0] & 0x0F);
}
static int8_t ciaaUpdate_protocolPacketType(const uint8_t *header)
{
   ciaaPOSIX_assert(NULL != header);
   return (header[0] >> 4) & 0x0F;
}
static uint16_t ciaaUpdate_protocolPayloadSize(const uint8_t *header)
{
   ciaaPOSIX_assert(NULL != header);
   return  (uint16_t) header[3] << 3;
}
static int32_t ciaaUpdate_protocolRecvHeader(uint8_t *header)
{
   size_t bytes_read = 0;
   ssize_t ret;

   /* read the whole header */
   while(bytes_read < CIAAUPDATE_PROTOCOL_HEADER_SIZE)
   {
      ret = ciaaUpdate_protocol_transport->recv(header + bytes_read, CIAAUPDATE_PROTOCOL_HEADER_SIZE - bytes_read);
      ciaaPOSIX_assert(-1 != ret);
      bytes_read += ret;
   }

   /* if the version is unknown */
   if(CIAAUPDATE_PROTOCOL_VERSION < ciaaUpdate_protocolHeaderVersion(header))
   {
      /* error */
      return CIAAUPDATE_PROTOCOL_ERROR_UNKNOWN_VERSION;
   }

   /* todo: sequence number checking */

   return CIAAUPDATE_PROTOCOL_ERROR_NONE;
}
static int32_t ciaaUpdate_protocolHandleDataPayload(
   /* pointer to the payload contents */
   const uint8_t *payload_data,
   /* payload size */
   size_t payload_size
)
{
   /* unreceived bytes in the current segment. */
   static uint32_t segment_remaining_bytes = 0;
   /* current destination address */
   static uint32_t segment_destination_address = 0;
   /* unparsed bytes in the current payload */
   int32_t payload_remaining_bytes;
   /* number of bytes to be written */
   int32_t write_size;
   /* holds return values */
   int32_t ret;

   /* reset static variables */
   if(NULL == payload_data)
   {
      segment_remaining_bytes = 0;
      segment_destination_address = 0;
      return CIAAUPDATE_PROTOCOL_ERROR_NONE;
   }

   /* the payload is completely unparsed */
   payload_remaining_bytes = payload_size;

   /* while there are still bytes to parse */
   while(payload_remaining_bytes > 0)
   {
      /* if the current segment is completely parsed or this is the start of
       * the first segment */
      if(segment_remaining_bytes == 0)
      {
         /* read the starting address from the first 4 bytes of the buffer */
         segment_destination_address = ciaaUpdate_utilsNtohl(*((uint32_t*) payload_data));
         /* increment payload pointer */
         payload_data += sizeof(uint32_t);
         /* update the unparsed bytes count */
         payload_remaining_bytes -= sizeof(uint32_t);

         /* read the segment size from the second 4 bytes of the buffer */
         segment_remaining_bytes = ciaaUpdate_utilsNtohl(*((uint32_t*) payload_data));
         /* increment payload pointer */
         payload_data += sizeof(uint32_t);
         /* update the unparsed bytes count */
         payload_remaining_bytes -= sizeof(uint32_t);
      }
      else
      {
         /* there is pending bytes from the current segment */

         /* calculate the amount of data to be written */
         write_size = ciaaUpdate_utilsMin(segment_remaining_bytes, payload_remaining_bytes);

         /* call the store function */
         ret = ciaaUpdate_protocol_store_cb(payload_data, write_size, segment_destination_address);
         ciaaPOSIX_assert(ret == write_size);

         /* update payload pointer */
         payload_data += write_size;
         /* update destination address */
         segment_destination_address += write_size;
         /* update the unparsed payload bytes count */
         payload_remaining_bytes -= write_size;
         /* update the unreceived segment bytes count */
         segment_remaining_bytes -= write_size;
      }
   }
   return CIAAUPDATE_PROTOCOL_ERROR_NONE;
}
static int32_t ciaaUpdate_protocolRecvData(size_t payload_size)
{
   ssize_t ret;
   size_t bytes_read = 0;
   uint8_t payload_buffer[CIAAUPDATE_PROTOCOL_PAYLOAD_MAX_SIZE];

   /* reset function state */
   if(0 == payload_size)
   {
      return ciaaUpdate_protocolHandleDataPayload(NULL, 0);
   }

   /* read the whole payload */
   while(bytes_read < payload_size)
   {
      ret = ciaaUpdate_protocol_transport->recv(payload_buffer + bytes_read, payload_size - bytes_read);
      ciaaPOSIX_assert(-1 != ret);

      bytes_read += ret;
   }

   /* todo: decrypt */

   /* parse and store the payload contents */
   return ciaaUpdate_protocolHandleDataPayload(payload_buffer, payload_size);
}
/*==================[external functions definition]==========================*/

int32_t ciaaUpdate_protocolRecv(
   ciaaUpdate_transportType* transport,
   ciaaUpdate_protocolStoreCallback store_cb)
{
   /* holds return values */
   int32_t ret;
   uint16_t payload_size;
   /* packet header */
   uint8_t header[CIAAUPDATE_PROTOCOL_HEADER_SIZE];

   /* initialize global variables */
   ciaaUpdate_protocol_transport = transport;
   ciaaUpdate_protocol_store_cb = store_cb;


   /* todo: receive client identification */


   /* receive client data packets */

   /* reset data packet handler state */
   ciaaUpdate_protocolRecvData(0);

   while(1)
   {
      ret = ciaaUpdate_protocolRecvHeader(header);
      if(CIAAUPDATE_PROTOCOL_ERROR_NONE != ret)
         return ret;

      if(CIAAUPDATE_PROTOCOL_DATA_PACKET != ciaaUpdate_protocolPacketType(header))
      {
         return CIAAUPDATE_PROTOCOL_ERROR_UNEXPECTED_PACKET;
      }

      payload_size = ciaaUpdate_protocolPayloadSize(header);
      /* if the payload size */
      if(0 == payload_size)
      {
         /* then we received the last data packet and the transmission is over */
         break;
      }
      /* else */

      /* receive a data packet and store its contents */
      ret = ciaaUpdate_protocolRecvData(payload_size);
      if(CIAAUPDATE_PROTOCOL_ERROR_NONE != ret)
         return ret;
   }

   /* todo: receive and verify signature if necessary */

   return CIAAUPDATE_PROTOCOL_ERROR_NONE;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

