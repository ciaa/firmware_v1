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

/** \brief This file implements the UPDT slave functionality
 **
 ** \todo Define a proper a way to return the error type.
 ** \todo Add device identification support.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup UPDT CIAA UPDT Slave
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
#include "UPDT_protocol.h"
#include "UPDT_slave.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static int32_t UPDT_slaveSendResponse(
   UPDT_slaveType *slave,
   const uint8_t* payload,
   size_t payload_size,
   uint8_t response_type)
{
   uint8_t *buffer;
   ciaaPOSIX_assert(NULL != slave);
   ciaaPOSIX_assert(NULL != payload);

   buffer = slave->send_buffer;
   UPDT_protocolSetHeader(buffer, response_type, slave->sequence_number, payload_size);

   ciaaPOSIX_memcpy(buffer + UPDT_PROTOCOL_HEADER_SIZE, payload, payload_size);

   return UPDT_protocolSend(slave->transport, buffer, UPDT_PROTOCOL_HEADER_SIZE + payload_size);
}
/*==================[external functions definition]==========================*/
int32_t UPDT_slaveInit(UPDT_slaveType *slave, UPDT_ITransportType *transport)
{
   ciaaPOSIX_assert(NULL != slave);
   ciaaPOSIX_assert(NULL != transport);

   slave->transport = transport;
   slave->protocol_version = 0;
   slave->sequence_number = 0;
   slave->payload_size = 0;
   slave->done = 0;

   return 0;
}
void UPDT_slaveClear(UPDT_slaveType *slave)
{
   ciaaPOSIX_assert(NULL != slave);

   slave->sequence_number = 0;
   slave->protocol_version = 0;
   slave->transport = NULL;
   slave->payload_size = 0;
   slave->done = 0;
}

ssize_t UPDT_slaveRecvInfo(
   UPDT_slaveType *slave,
   uint8_t *payload_buffer,
   size_t buffer_size)
{
   /* holds return values */
   int32_t ret;
   /* received payload size */
   ssize_t payload_size = 0;
   /* received packet header */
   uint8_t header[UPDT_PROTOCOL_HEADER_SIZE];
   /* received sequence number */
   uint8_t sequence_number;

   ciaaPOSIX_assert(buffer_size >= UPDT_PROTOCOL_PAYLOAD_MAX_SIZE);


   do
   {
      /* receive header */
      /** \todo review the timeouts. In this state a timeout does not matter */
      ret = UPDT_protocolRecv(slave->transport, header, UPDT_PROTOCOL_HEADER_SIZE);
      if(UPDT_PROTOCOL_ERROR_NONE != ret)
      {
         /* transport layer error or timeout */
         return -1;
      }

      payload_size = UPDT_protocolGetPayloadSize(header);
      sequence_number = UPDT_protocolGetSequenceNumber(header);

      /* get the data packet and remove it from the buffer */

      ret = UPDT_protocolRecv(slave->transport, payload_buffer, payload_size);
      if(UPDT_PROTOCOL_ERROR_NONE != ret)
      {
         /* transport layer error or timeout */
         return -1;
      }
   }
   /* receive again if the packet is corrupted */
   while(UPDT_protocolGetPacketType(header) != UPDT_PROTOCOL_PACKET_INF);

   /* set the expected sequence number */
   slave->sequence_number = sequence_number + 1;

   return payload_size;
}

/** \todo Go back to the initial state if an info packet is received */
ssize_t UPDT_slaveRecvData(
   UPDT_slaveType *slave,
   uint8_t *payload_buffer,
   size_t buffer_size)
{
   /* holds return values */
   int32_t ret;
   /* received payload size */
   ssize_t payload_size = 0;
   /* received packet header */
   uint8_t header[UPDT_PROTOCOL_HEADER_SIZE];
   /* received sequence number */
   uint8_t sequence_number;

   ciaaPOSIX_assert(buffer_size >= UPDT_PROTOCOL_PAYLOAD_MAX_SIZE);

   if(0 == slave->done)
   {
      do
      {
         /* receive header */
         ret = UPDT_protocolRecv(slave->transport, header, UPDT_PROTOCOL_HEADER_SIZE);
         if(UPDT_PROTOCOL_ERROR_NONE != ret)
         {
            /* transport layer error or timeout */
            return -1;
         }

         payload_size = UPDT_protocolGetPayloadSize(header);
         sequence_number = UPDT_protocolGetSequenceNumber(header);

         /* if the expected payload size does not match with the received */
         if(slave->payload_size != payload_size)
         {
            /* if this is the first packet */
            if(0 == slave->payload_size)
            {
               /* then set the expected payload size */
               slave->payload_size = payload_size;
            } else {
               /* this is the last packet */
               slave->done = 1;
            }
         }

         /* if we receive the same sequence number again */
         if(slave->sequence_number - 1 == sequence_number)
         {
            /* then ACK sent may be lost. does not matter the packet type */

            /* resend ACK */
            UPDT_slaveSendResponse(slave, slave->send_buffer, 0, UPDT_PROTOCOL_PACKET_ACK);
         }

         /* get the data packet and remove it from the buffer */
         ret = UPDT_protocolRecv(slave->transport, payload_buffer, payload_size);
         if(UPDT_PROTOCOL_ERROR_NONE != ret)
         {
            /* transport layer error or timeout */
            return -1;
         }
      }
      /* receive again if the packet is corrupted */
      while(UPDT_protocolGetPacketType(header) != UPDT_PROTOCOL_PACKET_DAT ||
            slave->sequence_number != sequence_number);

      /* increment expected sequence number */
      ++slave->sequence_number;

      /* send acknowledge */
      UPDT_slaveSendResponse(slave, slave->send_buffer, 0, UPDT_PROTOCOL_PACKET_ACK);
   }

   return payload_size;
}

ssize_t UPDT_slaveSendAllow(
   UPDT_slaveType *slave,
   uint8_t *payload,
   size_t payload_size)
{
   return UPDT_slaveSendResponse(slave, payload, payload_size, UPDT_PROTOCOL_PACKET_ALW);
}

ssize_t UPDT_slaveSendDeny(
   UPDT_slaveType *slave,
   uint8_t *payload,
   size_t payload_size)
{
   return UPDT_slaveSendResponse(slave, payload, payload_size, UPDT_PROTOCOL_PACKET_DNY);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
