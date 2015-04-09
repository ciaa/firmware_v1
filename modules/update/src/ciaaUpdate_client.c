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

/** \brief This file implements the Flash Update Client functionality
 **
 ** This file implements the functionality of the Flash Update Client
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Client
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
#include "ciaaPOSIX_string.h"
#include "ciaaUpdate_client.h"
#include "ciaaUpdate_protocol.h"
#include "ciaaUpdate_transport.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static int ciaaUpdate_clientRecvAck(ciaaUpdate_clientType *client)
{/* holds return values */
   int32_t ret;
   /* received payload size */
   ssize_t payload_size;
   /* received packet buffer. Although a 4 bytes packet is expected, a bigger
    * packet can arrive and a buffer is necessary to discard it. */
   uint8_t buffer[CIAAUPDATE_PROTOCOL_PAYLOAD_MAX_SIZE];
   /* received sequence number */
   uint8_t sequence_number;

   do
   {
      /* receive header */
      ret = ciaaUpdate_protocolRecv(client->transport, buffer, CIAAUPDATE_PROTOCOL_HEADER_SIZE);
      if(CIAAUPDATE_PROTOCOL_ERROR_NONE != ret)
      {
         /* transport layer error or timeout */
         return -1;
      }

      payload_size = ciaaUpdate_protocolGetPayloadSize(buffer);
      sequence_number = ciaaUpdate_protocolGetSequenceNumber(buffer);

      /* if received packet has non empty payload as expected */
      if(0 != payload_size)
      {
         /* then discard it */
         ret = ciaaUpdate_protocolRecv(client->transport, buffer, payload_size);
         if(CIAAUPDATE_PROTOCOL_ERROR_NONE != ret)
         {
            /* transport layer error or timeout */
            return -1;
         }
      }
   }
   /* receive again if the packet is corrupted */
   while(ciaaUpdate_protocolGetHeaderVersion(buffer) != client->protocol_version ||
         ciaaUpdate_protocolGetPacketType(buffer) != CIAAUPDATE_PROTOCOL_PACKET_DATA ||
         client->sequence_number != sequence_number ||
         0 != payload_size);

   return 0;
}
/*==================[external functions definition]==========================*/
ciaaUpdate_clientType *ciaaUpdate_clientNew(ciaaUpdate_transportType *transport)
{
   ciaaUpdate_clientType *client;

   ciaaPOSIX_assert(NULL != transport);

   client = ciaaPOSIX_malloc(sizeof(ciaaUpdate_clientType));
   if(NULL != client)
   {
      client->transport = transport;
      client->protocol_version = 0;
      client->sequence_number = 0;
   }
   return client;
}
void ciaaUpdate_clientDel(ciaaUpdate_clientType *client)
{
   ciaaPOSIX_assert(NULL != client);

   client->transport = NULL;
   client->protocol_version = 0;
   client->sequence_number = 0;
   ciaaPOSIX_free(client);
}

ssize_t ciaaUpdate_clientSendData(ciaaUpdate_clientType *client, const uint8_t *payload_buffer, size_t payload_size)
{
   int ret;
   uint8_t packet_buffer[CIAAUPDATE_PROTOCOL_PACKET_MAX_SIZE];

   /* create header */
   ciaaUpdate_protocolSetHeaderVersion(packet_buffer, client->protocol_version);
   ciaaUpdate_protocolSetPacketType(packet_buffer, CIAAUPDATE_PROTOCOL_PACKET_DATA);
   ciaaUpdate_protocolSetPayloadSize(packet_buffer, payload_size);
   ciaaUpdate_protocolSetSequenceNumber(packet_buffer, client->sequence_number);

   /* copy data */
   ciaaPOSIX_memcpy(packet_buffer + CIAAUPDATE_PROTOCOL_HEADER_SIZE, payload_buffer, payload_size);

   /* TODO: add a retransmission limit */
   /* retransmission loop */
   do
   {
      /* send data packet */
      ret = ciaaUpdate_protocolSend(client->transport, packet_buffer, payload_size + CIAAUPDATE_PROTOCOL_HEADER_SIZE);
      if(CIAAUPDATE_PROTOCOL_ERROR_NONE != ret)
      {
         /* transport layer error or timeout */
         return -1;
      }

      /* wait for ACK */
      ret = ciaaUpdate_clientRecvAck(client);
   } while(ret != CIAAUPDATE_PROTOCOL_ERROR_NONE);

   /* increment sequence number */
   ++client->sequence_number;
   return payload_size;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

