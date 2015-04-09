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

#ifndef _CIAAUPDATE_PROTOCOL_H_
#define _CIAAUPDATE_PROTOCOL_H_
/** \brief Flash Update Protocol Header File
 **
 ** This files shall be included by modules using the interfaces provided by
 ** the Flash Update Protocol
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Protocol
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
 * 20150408 v0.0.2  FS  first operating version
 * 20141010 v0.0.1  EV  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaUpdate_transport.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define CIAAUPDATE_PROTOCOL_ERROR_NONE                0
#define CIAAUPDATE_PROTOCOL_ERROR_UNKNOWN_VERSION     1
#define CIAAUPDATE_PROTOCOL_ERROR_TRANSPORT           2

#define CIAAUPDATE_PROTOCOL_VERSION                0

/* packet types */
#define CIAAUPDATE_PROTOCOL_PACKET_DATA            2
#define CIAAUPDATE_PROTOCOL_PACKET_ACK             3

#define CIAAUPDATE_PROTOCOL_HEADER_SIZE            4
#define CIAAUPDATE_PROTOCOL_PAYLOAD_MAX_SIZE       248

#define CIAAUPDATE_PROTOCOL_PACKET_MAX_SIZE        (CIAAUPDATE_PROTOCOL_PAYLOAD_MAX_SIZE + CIAAUPDATE_PROTOCOL_HEADER_SIZE)
/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
int8_t ciaaUpdate_protocolGetHeaderVersion(const uint8_t *header);
int8_t ciaaUpdate_protocolGetPacketType(const uint8_t *header);
uint16_t ciaaUpdate_protocolGetPayloadSize(const uint8_t *header);
uint8_t ciaaUpdate_protocolGetSequenceNumber(const uint8_t *header);


int ciaaUpdate_protocolRecv(ciaaUpdate_transportType *transport, uint8_t *buffer, size_t size);
int ciaaUpdate_protocolSend(ciaaUpdate_transportType *transport, const uint8_t *buffer, size_t size);

void ciaaUpdate_protocolSetHeaderVersion(uint8_t *header, uint8_t version);
void ciaaUpdate_protocolSetPacketType(uint8_t *header, uint8_t packet_type);
void ciaaUpdate_protocolSetPayloadSize(uint8_t *header, uint16_t payload_size);
void ciaaUpdate_protocolSetSequenceNumber(uint8_t *header, uint8_t sequence_number);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAUPDATE_PROTOCOL_H_ */

