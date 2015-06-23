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

#ifndef UPDT_MASTER_H
#define UPDT_MASTER_H
/** \brief Flash Update Master Header File
 **
 ** This files shall be included by modules using the interfaces provided by
 ** the Flash Update Master
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Master
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
#include "UPDT_protocol.h"
#include "UPDT_ITransport.h"
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Expected response to a given packet type */
#define UPDT_MASTER_EXPECTED_PACKET(type) (                          \
   UPDT_PROTOCOL_PACKET_DAT == (type) ? UPDT_PROTOCOL_PACKET_ACK : ( \
   UPDT_PROTOCOL_PACKET_INF == (type) ? UPDT_PROTOCOL_PACKET_ALW | UPDT_PROTOCOL_PACKET_DNY : ( \
   UPDT_PROTOCOL_PACKET_INV)))

/*==================[typedef]================================================*/
/** \brief Master type */
typedef struct UPDT_masterType
{
   /** Transport layer */
   UPDT_ITransportType *transport;
   /** Negotiated protocol version */
   uint8_t protocol_version;
   /** Expected sequence number */
   uint8_t sequence_number;
   /** Last packet type */
   uint8_t last_packet_type;
   /** Buffer to hold outgoing packets */
   uint8_t send_buffer[UPDT_PROTOCOL_PACKET_MAX_SIZE];
   /** Buffer to hold incoming packets */
   uint8_t recv_buffer[UPDT_PROTOCOL_PACKET_MAX_SIZE];
   /** Buffer to hold INFO packets data */
   //uint8_t info_buffer[UPDT_PROTOCOL_PACKET_INF_PAYLOAD_SIZE];
} UPDT_masterType;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief Initializes a master structure.
 **
 ** This function sets up the transport layer to use, sets an initial
 ** sequence number and initializes internal variables.
 **
 ** \param Master structure to initialize.
 ** \param Transport layer to use.
 ** \return 0 on success. Non-zero on error.
 **/
int32_t UPDT_masterInit(UPDT_masterType *master, UPDT_ITransportType *transport);

/** \brief Clears a master structure.
 **
 ** This function should be called when transmission is over.
 ** \param Master structure to clear.
 **/
void UPDT_masterClear(UPDT_masterType *master);

/** \brief Passes a data payload to the transport layer.
 **
 ** This function sends the specified payload through the network, waits for
 ** an acknowledgment and retransmits if necessary. It fails if a timeout or
 ** transport layer error occurs.
 **
 ** \param master Master structure.
 ** \param payload_buffer Buffer containing the payload to send.
 ** \param payload_size Payload size.
 ** \return 0 on success. -1 on error.
 **/
ssize_t UPDT_masterSendData(UPDT_masterType *master, const uint8_t *payload_buffer, size_t payload_size);

/** \brief Passes an info payload to the transport layer.
 **
 ** This function sends the specified payload through the network, waits for
 ** a response and retransmits if necessary. It fails if a timeout or
 ** transport layer error occurs.
 **
 ** \param master Master structure.
 ** \param master_info Buffer containing the info to send.
 ** \param master_info_size Size of the master info buffer.
 ** \param response_buffer Buffer to hold the slave response.
 ** \param response_size Size of the response buffer.
 ** \return Number of bytes received on success. -1 on error.
 **/
ssize_t UPDT_masterHandshake(
   UPDT_masterType *master,
   const uint8_t *master_info,
   size_t master_info_size,
   uint8_t *response_buffer,
   size_t response_buffer_size);

/** \brief Returns the packet type of the last packet received.
 **
 ** \param master Master structure.
 ** \return Packet type. If no packet was received yet it return PACKET_INV (invalid).
 **/
 uint8_t UPDT_masterLastPacketType(UPDT_masterType *master);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef UPDT_MASTER_H */

