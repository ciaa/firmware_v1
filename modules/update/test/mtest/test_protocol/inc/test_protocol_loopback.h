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

#ifndef TEST_UPDATE_LOOPBACK_H
#define TEST_UPDATE_LOOPBACK_H
/** \brief Flash Update test loopback transport layer file
 **
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
 * 20150418 v0.0.1  FS  first initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "ciaaPOSIX_assert.h"
#include "ciaaLibs_CircBuf.h"
#include "UPDT_ITransport.h"
#include "UPDT_protocol.h"
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief Loopback transport layer type. */

typedef struct test_update_loopbackType test_update_loopbackType;
typedef struct test_update_loopbackType
{
   /** Transport interface */
   UPDT_ITransportType transport;
   /** Memory block for the circular buffer */
   uint8_t own_cbuf_mem[UPDT_PROTOCOL_PACKET_MAX_SIZE * 2];
   /** Own circular buffer struct */
   ciaaLibs_CircBufType own_cbuf;
   /** Destination circular buffer struct */
   ciaaLibs_CircBufType *dest_cbuf;
   /** Task ID */
   TaskType task_id;
   /** Event Mask */
   EventMaskType recv_event;
   /** Counterpart */
   test_update_loopbackType *counterpart;
} test_update_loopbackType;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief Initializes a loopback structure.
 **
 ** This function initializes a loopback transport layer over the specified
 ** buffers.
 **
 ** \param loopback Loopback structure to initialize.
 ** \param rx_buffer Receiver circular buffer.
 ** \param tx_buffer Transmitter circular buffer.
 ** \param task_id Task ID for the calling task.
 ** \param recv_event Receive event mask used by SetEvent and WaitEvent.
 ** \return 0 on success. Non-zero on error.
 **/
int32_t test_update_loopbackInit(
   test_update_loopbackType *loopback,
   TaskType task_id,
   EventMaskType recv_event);

/** \brief Connects two loopback entities.
 **
 ** Two loopback entities must be connected before transmitting data.
 **
 ** \param loopback1 A loopback entity.
 ** \param loopback1 A different loopback entity.
 **/
void test_update_loopbackConnect(
   test_update_loopbackType *loopback1,
   test_update_loopbackType *loopback2
);


/** \brief Clears a loopback structure.
 **
 ** Clears the loopback transport layer structure.
 **
 ** \param loopback The loopback structure to clear.
 **/
void test_update_loopbackClear(test_update_loopbackType *loopback);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef TEST_UPDATE_LOOPBACK_H */

