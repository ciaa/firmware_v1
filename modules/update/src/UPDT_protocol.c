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

/** \brief This file implements the Flash UPDT protocol functionality
 **
 ** \todo Add device identification support.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup UPDT CIAA UPDT Protocol
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
#include "UPDT_osal.h"
#include "UPDT_protocol.h"
#include "UPDT_ITransport.h"
#include "UPDT_utils.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

int8_t UPDT_protocolGetPacketType(const uint8_t *header)
{
   assert(NULL != header);
   return header[0] & 0x0F;
}
uint16_t UPDT_protocolGetPayloadSize(const uint8_t *header)
{
   assert(NULL != header);
   return  ((uint16_t) header[3]) << 3;
}
uint8_t UPDT_protocolGetSequenceNumber(const uint8_t *header)
{
   assert(NULL != header);

   return header[2];
}

void UPDT_protocolSetHeader(
   uint8_t *header,
   uint8_t packet_type,
   uint8_t sequence_number,
   uint16_t payload_size)
{
   assert(NULL != header);
   /* payload must be size multiple of 8 and smaller than 2048 */
   assert(0 == (payload_size & 0xF807));

   header[0] = (header[0] & 0xF0) | (packet_type & 0x0F);
   header[1] = 0;
   header[2] = sequence_number;
   header[3] = (uint8_t) (payload_size >> 3);
}

int32_t UPDT_protocolRecv(
   UPDT_ITransportType *transport,
   uint8_t *buffer,
   size_t size)
{
   ssize_t ret;
   size_t bytes_read = 0;

   assert(NULL != buffer);

   if(0 == size)
   {
      return 0;
   }
   /* read the specified number of bytes */
   while(bytes_read < size)
   {
      ret = transport->recv(transport, buffer + bytes_read, size - bytes_read);
      if(ret < 0)
      {
         return UPDT_PROTOCOL_ERROR_TRANSPORT;
      }
      bytes_read += ret;
   }
   return UPDT_PROTOCOL_ERROR_NONE;
}

int32_t UPDT_protocolSend(
   UPDT_ITransportType *transport,
   const uint8_t *buffer,
   size_t size)
{
   ssize_t ret;
   size_t bytes_sent = 0;

   assert(NULL != buffer);

   if(0 == size)
   {
      return 0;
   }
   /* send the specified number of bytes */
   while(bytes_sent < size)
   {
      ret = transport->send(transport, buffer + bytes_sent, size - bytes_sent);
      if(ret < 0)
      {
         return UPDT_PROTOCOL_ERROR_TRANSPORT;
      }
      bytes_sent += ret;
   }
   return UPDT_PROTOCOL_ERROR_NONE;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

