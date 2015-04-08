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

/** \brief This file implements the Flash Update Protocol functionality
 **
 ** This file implements the functionality of the Flash Update Protocol
 **
 ** \todo Define a proper a way to return the error type.
 ** \todo Add device identification support.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Server
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
#include "ciaaUpdate_protocol.h"
#include "ciaaUpdate_server.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static int ciaaUpdate_serverSendAck(ciaaUpdate_serverType *server, uint8_t sequence_number)
{
   uint8_t header[4];

   ciaaUpdate_protocolSetHeaderVersion(header, server->protocol_version);
   ciaaUpdate_protocolSetPacketType(header, CIAAUPDATE_PROTOCOL_PACKET_ACK);
   ciaaUpdate_protocolSetPayloadSize(header, 0);
   ciaaUpdate_protocolSetSequenceNumber(header, sequence_number);

   return server->transport->send(server->transport, header, 4);
}
/*==================[external functions definition]==========================*/
ciaaUpdate_serverType *ciaaUpdate_serverNew(ciaaUpdate_transportType *transport)
{
   ciaaUpdate_serverType *server;

   ciaaPOSIX_assert(NULL != transport);

   server = ciaaPOSIX_malloc(sizeof(ciaaUpdate_serverType));
   if(NULL != server)
   {
      server->transport = transport;
      server->protocol_version = 0;
      server->sequence_number = 0;
   }
   return server;
}
void ciaaUpdate_serverDel(ciaaUpdate_serverType *server)
{
   ciaaPOSIX_assert(NULL != server);

   server->transport = NULL;
   server->protocol_version = 0;
   server->sequence_number = 0;
   ciaaPOSIX_free(server);
}
ssize_t ciaaUpdate_serverRecvData(
   ciaaUpdate_serverType *server,
   uint8_t *payload_buffer)
{
   /* holds return values */
   int32_t ret;
   /* received payload size */
   ssize_t payload_size;
   /* received packet header */
   uint8_t header[CIAAUPDATE_PROTOCOL_HEADER_SIZE];
   /* received sequence number */
   uint8_t sequence_number;
   do
   {
      /* receive header */
      ret = ciaaUpdate_protocolRecv(server->transport, header, CIAAUPDATE_PROTOCOL_HEADER_SIZE);
      if(CIAAUPDATE_PROTOCOL_ERROR_NONE != ret)
      {
         /* transport layer error or timeout */
         return -1;
      }

      payload_size = ciaaUpdate_protocolGetPayloadSize(header);
      sequence_number = ciaaUpdate_protocolGetSequenceNumber(header);

      /* if we receive the same sequence number again */
      if(server->sequence_number - 1 == sequence_number)
      {
         /* then ACK sent may be lost. does not matter the packet type */

         /* resend ACK */
         ciaaUpdate_serverSendAck(server, sequence_number);
      }

      /* get the data packet and remove it from the buffer */
      ret = ciaaUpdate_protocolRecv(server->transport, payload_buffer, payload_size);
      if(CIAAUPDATE_PROTOCOL_ERROR_NONE != ret)
      {
         /* transport layer error or timeout */
         return -1;
      }
   }
   /* receive again if the packet is corrupted */
   while(ciaaUpdate_protocolGetHeaderVersion(header) != server->protocol_version ||
         ciaaUpdate_protocolGetPacketType(header) != CIAAUPDATE_PROTOCOL_PACKET_DATA ||
         server->sequence_number != sequence_number);

   /* send acknowledge */
   ciaaUpdate_serverSendAck(server, sequence_number);

   /* increment expected sequence number */
   ++server->sequence_number;

   return payload_size;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

