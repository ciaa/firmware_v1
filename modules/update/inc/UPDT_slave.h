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

#ifndef UPDT_SLAVE_H_
#define UPDT_SLAVE_H_
/** \brief Flash Update Slave Header File
 **
 ** This files shall be included by modules using the interfaces provided by
 ** the Flash Update Slave
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Slave
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
#include "UPDT_ITransport.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief Slave type. */
typedef struct
{
   /** Transport layer */
   UPDT_ITransportType *transport;
   /** Buffer to hold outgoing packets */
   uint8_t send_buffer[UPDT_PROTOCOL_PACKET_MAX_SIZE];
   /** Buffer to hold incoming packets */
   uint8_t recv_buffer[UPDT_PROTOCOL_PACKET_MAX_SIZE];
   /** Expected payload size */
   uint8_t payload_size;
   /** Negotiated protocol version */
   uint8_t protocol_version;
   /** Expected sequence number */
   uint8_t sequence_number;
   /** Data transfer finished flag */
   uint8_t done;
} UPDT_slaveType;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief Initializes a slave structure.
 **
 ** \param slave Slave structure to initialize.
 ** \param transport Transport layer to use.
 ** \return 0 on success. -1 on error.
 **/
int32_t UPDT_slaveInit(UPDT_slaveType *slave, UPDT_ITransportType *transport);

/** \brief Clears a slave structure.
 **
** This function should be called when reception is over.
 ** \param slave Slave structure to clear.
 **/
void UPDT_slaveClear(UPDT_slaveType *slave);

/** \brief Waits and receives a data packet.
 **
 ** This function waits a data packet, extracts its payload and sends an
 ** acknowledgment. If an unexpected packet is received then it is discarded
 ** silently. It fails if a timeout or transport layer error occurs.
 **
 ** \param slave Slave structure.
 ** \param payload_buffer Buffer where the payload must be returned.
 ** \param buffer_size Buffer size. Must be at least
 ** UPDT_PAYLOAD_MAX_SIZE
 ** \return Number of bytes received. -1 on error.
 **/
ssize_t UPDT_slaveRecvData(UPDT_slaveType *slave, uint8_t *payload_buffer, size_t buffer_size);

/** \brief Waits and receives an info packet.
 **
 ** This function waits an info packet, extracts its payload and returns it.
 ** It does not send an acknowledgment because info packets expect an info
 ** response whose payload creation is a not responsibility of this library.
 ** \param slave Slave structure
 ** \param payload_buffer Buffer where the payload must be returned.
 ** \param buffer_size Buffer size. Must be big enough to hold the payload.
 ** \return Number of bytes received. -1 on error.
 **/
ssize_t UPDT_slaveRecvInfo(UPDT_slaveType *slave, uint8_t *payload_buffer, size_t buffer_size);

ssize_t UPDT_slaveSendAllow(
   UPDT_slaveType *slave,
   uint8_t *payload,
   size_t payload_size);

ssize_t UPDT_slaveSendDeny(
   UPDT_slaveType *slave,
   uint8_t *payload,
   size_t payload_size);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef UPDT_SLAVE_H_ */

