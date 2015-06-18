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

#ifndef UPDT_PROTOCOL_H_
#define UPDT_PROTOCOL_H_
/** \brief Flash Update Protocol Header File
 **
 ** This files shall be included by modules using the interfaces provided by
 ** the Flash Update Protocol
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Update CIAA Update Protocol
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
#include "UPDT_ITransport.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define UPDT_PROTOCOL_ERROR_NONE                0
#define UPDT_PROTOCOL_ERROR_UNKNOWN_VERSION     1
#define UPDT_PROTOCOL_ERROR_TRANSPORT           2

#define UPDT_PROTOCOL_VERSION                0x00u

/* packet types */
#define UPDT_PROTOCOL_PACKET_INV             0xFFu
#define UPDT_PROTOCOL_PACKET_ACK             0x00u
#define UPDT_PROTOCOL_PACKET_DAT             0x01u
#define UPDT_PROTOCOL_PACKET_INF             0x02u
#define UPDT_PROTOCOL_PACKET_ALW             0x03u
#define UPDT_PROTOCOL_PACKET_DNY             0x04u

#define UPDT_PROTOCOL_PACKET_VALID(t)  ((t) <= 4)

/* header */
#define UPDT_PROTOCOL_HEADER_SIZE            4

/* payload */
/* payload sizes in bytes */
#define UPDT_PROTOCOL_PACKET_ACK_PAYLOAD_SIZE    0
#define UPDT_PROTOCOL_PACKET_DAT_PAYLOAD_SIZE    248 /* <= maximum */
#define UPDT_PROTOCOL_PACKET_INF_PAYLOAD_SIZE    28

#define UPDT_PROTOCOL_PAYLOAD_MAX_SIZE UPDT_PROTOCOL_PACKET_DAT_PAYLOAD_SIZE

#define UPDT_PROTOCOL_PAYLOAD_SIZE(t) (                                       \
   UPDT_PROTOCOL_PACKET_ACK == (t) ? UPDT_PROTOCOL_PACKET_ACK_PAYLOAD_SIZE : (\
   UPDT_PROTOCOL_PACKET_DAT == (t) ? UPDT_PROTOCOL_PACKET_DAT_PAYLOAD_SIZE : (\
   UPDT_PROTOCOL_PACKET_INF == (t) ? UPDT_PROTOCOL_PACKET_INF_PAYLOAD_SIZE :  \
   UPDT_PROTOCOL_PACKET_INV)))


#define UPDT_PROTOCOL_PACKET_MAX_SIZE    (UPDT_PROTOCOL_PAYLOAD_MAX_SIZE + UPDT_PROTOCOL_HEADER_SIZE)
/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/* header parsing */
int8_t   UPDT_protocolGetPacketType(const uint8_t *header);
uint16_t UPDT_protocolGetPayloadSize(const uint8_t *header);
uint8_t  UPDT_protocolGetSequenceNumber(const uint8_t *header);

/** If size = 0 returns immediately */
int32_t UPDT_protocolRecv(UPDT_ITransportType *transport, uint8_t *buffer, size_t size);

/** If size = 0 returns immediately */
int32_t UPDT_protocolSend(UPDT_ITransportType *transport, const uint8_t *buffer, size_t size);

void UPDT_protocolSetHeader(
   uint8_t *header,
   uint8_t packet_type,
   uint8_t sequence_number,
   uint16_t payload_size);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef UPDT_PROTOCOL_H_ */

