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

/** \brief This file implements the Flash UPDT Master functionality
 **
 ** This file implements the functionality of the Flash UPDT Master
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup UPDT CIAA UPDT Master
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
#include "UPDT_osal.h"
#include "UPDT_master.h"
#include "UPDT_protocol.h"
#include "UPDT_ITransport.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static ssize_t UPDT_masterWaitAndRecv(
   UPDT_masterType *master,
   int8_t sent_type)
{
   /* holds return values */
   int32_t ret;
   /* buffer pointer */
   uint8_t *buffer;
   /* received payload size */
   ssize_t payload_size;
   /* received sequence number */
   uint8_t sequence_number;
   /* received packet type */
   uint8_t recv_type;

   /* initialize the buffer pointer */
   buffer = master->recv_buffer;
   do
   {
      /* receive header */
      ret = UPDT_protocolRecv(master->transport, buffer, UPDT_PROTOCOL_HEADER_SIZE);
      if(UPDT_PROTOCOL_ERROR_NONE != ret)
      {
         /* transport layer error or timeout */
         return -1;
      }

      payload_size = UPDT_protocolGetPayloadSize(buffer);
      sequence_number = UPDT_protocolGetSequenceNumber(buffer);
      recv_type = UPDT_protocolGetPacketType(buffer);

      /* receive the payload */
      ret = UPDT_protocolRecv(master->transport, buffer + UPDT_PROTOCOL_HEADER_SIZE, payload_size);
      if(UPDT_PROTOCOL_ERROR_NONE != ret)
      {
         /* transport layer error or timeout */
         return -1;
      }
   }
   /* receive again if the packet is corrupted */
   while((sent_type == UPDT_PROTOCOL_PACKET_DAT && recv_type != UPDT_PROTOCOL_PACKET_ACK) ||
         (sent_type == UPDT_PROTOCOL_PACKET_INF &&
         recv_type != UPDT_PROTOCOL_PACKET_DNY && recv_type != UPDT_PROTOCOL_PACKET_ALW) ||
         master->sequence_number + 1 != sequence_number);

   master->last_packet_type = recv_type;
   return payload_size;
 }

static ssize_t UPDT_masterSendAndRecv(
   UPDT_masterType *master,
   const uint8_t *payload_buffer,
   uint32_t payload_size,
   uint8_t packet_type)
{
   int32_t ret = -1;
   uint8_t *buffer;

   assert(NULL != master);
   assert(NULL != payload_buffer);
   /* packet has valid type */
   assert(0 != UPDT_PROTOCOL_PACKET_VALID(packet_type));
   /* and that type expects a response (master cannot send ACK) */
   assert(UPDT_PROTOCOL_PACKET_INV != UPDT_MASTER_EXPECTED_PACKET(packet_type));

   /* initialize buffer */
   buffer = master->send_buffer;

   /* create header */
   UPDT_protocolSetHeader(buffer, packet_type, master->sequence_number, payload_size);

   /* copy data */
   memcpy(buffer + UPDT_PROTOCOL_HEADER_SIZE, payload_buffer, payload_size);

   /** \todo add retransmission limit */
   do
   {
      /* send data packet */
      ret = UPDT_protocolSend(master->transport, buffer, UPDT_PROTOCOL_HEADER_SIZE + payload_size);
      if(UPDT_PROTOCOL_ERROR_NONE != ret)
      {
         /* transport layer error or timeout */
         return -1;
      }

      /* wait for response */
      ret = UPDT_masterWaitAndRecv(master, packet_type);
   } while(ret != UPDT_PROTOCOL_ERROR_NONE);

   ++master->sequence_number;

   return ret;
}
/*==================[external functions definition]==========================*/
int32_t UPDT_masterInit(UPDT_masterType *master, UPDT_ITransportType *transport)
{
   assert(NULL != master);
   assert(NULL != transport);

   master->transport = transport;
   master->protocol_version = 0;
   master->sequence_number = 0;
   master->last_packet_type = UPDT_PROTOCOL_PACKET_INV;

   return 0;
}

void UPDT_masterClear(UPDT_masterType *master)
{
   assert(NULL != master);

   master->sequence_number = 0;
   master->protocol_version = 0;
   master->transport = NULL;
}

ssize_t UPDT_masterSendData(UPDT_masterType *master, const uint8_t *payload_buffer, size_t payload_size)
{
   ssize_t resp_size;
   assert(NULL != master);
   assert(NULL != payload_buffer);

   /* send the packet and wait the response */
   resp_size = UPDT_masterSendAndRecv(master, payload_buffer, payload_size, UPDT_PROTOCOL_PACKET_DAT);

   /* if no error happened */
   if(resp_size >= 0)
   {
      /* then an ACK packet was received */

      /* return the payload size */
      return payload_size;
   }

   /* return transmission error */
   return -1;
}

ssize_t UPDT_masterHandshake(
   UPDT_masterType *master,
   const uint8_t *master_info,
   size_t master_info_size,
   uint8_t *response_buffer,
   size_t response_buffer_size)
{
   ssize_t resp_size;
   assert(NULL != master);
   assert(NULL != master_info);
   assert(NULL != response_buffer);

   /* send the INFO packet and receive the response into the receive buffer */
   resp_size = UPDT_masterSendAndRecv(
      /* master structure */
      master,
      /* master info */
      master_info,
      /* INFO packet size */
      master_info_size,
      /* INFO packet */
      UPDT_PROTOCOL_PACKET_INF);

   /* if no error happened */
   if(resp_size >= 0)
   {
      /* then a response packet was received */

      if(resp_size > response_buffer_size)
         /* response buffer too small */
         return -2;
      /* else */

      /* copy the response to the destination buffer */
      memcpy(response_buffer, master->recv_buffer, resp_size);

      /* return payload size */
      return resp_size;
   }

   /* return transmission error */
   return -1;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

