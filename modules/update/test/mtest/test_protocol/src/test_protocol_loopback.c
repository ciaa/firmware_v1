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

/** \brief This file implements a loopback transport layer for the update
 ** protocol.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup MTests CIAA Firmware Module Tests
 ** @{ */
/** \addtogroup Update Update Module Tests
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
#include "UPDT_ITransport.h"

#include "test_protocol_loopback.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static uint32_t test_update_nextPowerOfTwo(uint32_t v)
{
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    ++v;
    return v;
}
/** \brief Sends a packet.
 **
 ** \param loopback Loopback structure.
 ** \param data Data to send.
 ** \param size Number of bytes to send.
 ** \return Number of bytes sent. -1 on error.
 **/
static ssize_t test_update_loopbackSend(UPDT_ITransportType *transport, const void *data, size_t size)
{
   ssize_t ret;
   test_update_loopbackType *loopback = (test_update_loopbackType *) transport;

   ciaaPOSIX_assert(NULL != loopback);
   ciaaPOSIX_assert(NULL != loopback->counterpart);

   ret = ciaaLibs_circBufPut(((test_update_loopbackType *) loopback)->dest_cbuf, data, size);

   /* set the receiver event */
   SetEvent(loopback->counterpart->task_id, loopback->counterpart->recv_event);
   return ret;
}
/** \brief Receives a packet.
 **
 ** \param loopback Loopback structure.
 ** \param data Buffer to receive.
 ** \param size Number of bytes to receive.
 ** \return Number of bytes received. -1 on error.
 **/
static ssize_t test_update_loopbackRecv(UPDT_ITransportType *transport, void *data, size_t size)
{
   ssize_t ret;
   size_t bytes_read = 0;
   test_update_loopbackType *loopback = (test_update_loopbackType *) transport;

   ciaaPOSIX_assert(NULL != loopback);
   ciaaPOSIX_assert(NULL != loopback->counterpart);

   while(bytes_read < size)
   {
      ret = ciaaLibs_circBufGet(&((test_update_loopbackType *) loopback)->own_cbuf, data + bytes_read, size - bytes_read);
      ciaaPOSIX_assert(ret >= 0);
      bytes_read += ret;

      /* if there is not enough data */
      if(bytes_read < size)
      {
         /* then wait for it */
         WaitEvent(loopback->recv_event);
         ClearEvent(loopback->recv_event);
      }
   }
   return ret;
}
/*==================[external functions definition]==========================*/

int32_t test_update_loopbackInit(
   test_update_loopbackType *loopback,
   TaskType task_id,
   EventMaskType recv_event)
{
   size_t buffer_size;

   ciaaPOSIX_assert(NULL != loopback);

   loopback->transport.recv = test_update_loopbackRecv;
   loopback->transport.send = test_update_loopbackSend;
   loopback->task_id = task_id;
   loopback->recv_event = recv_event;
   loopback->counterpart = NULL;
   loopback->dest_cbuf = NULL;

   buffer_size = test_update_nextPowerOfTwo(UPDT_PROTOCOL_PACKET_MAX_SIZE);

   /* return non-zero on error */
   return -1 == ciaaLibs_circBufInit(&loopback->own_cbuf, loopback->own_cbuf_mem, buffer_size);
}

void test_update_loopbackConnect(
   test_update_loopbackType *loopback1,
   test_update_loopbackType *loopback2
)
{
   ciaaPOSIX_assert(NULL != loopback1);
   ciaaPOSIX_assert(NULL != loopback2);
   ciaaPOSIX_assert(loopback1 != loopback2);

   loopback1->counterpart = loopback2;
   loopback2->counterpart = loopback1;

   loopback1->dest_cbuf = &loopback2->own_cbuf;
   loopback2->dest_cbuf = &loopback1->own_cbuf;
}

void test_update_loopbackClear(test_update_loopbackType *loopback)
{
   ciaaPOSIX_assert(NULL != loopback);

   loopback->transport.send = NULL;
   loopback->transport.recv = NULL;
   loopback->counterpart = NULL;
   loopback->dest_cbuf = NULL;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

