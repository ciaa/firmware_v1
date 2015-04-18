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

#ifndef _CIAAUPDATE_SERVER_H_
#define _CIAAUPDATE_SERVER_H_
/** \brief Flash Update Server Header File
 **
 ** This files shall be included by modules using the interfaces provided by
 ** the Flash Update Server
 **
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
#include "ciaaUpdate_transport.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief Server type. */
typedef struct
{
   /** Transport layer */
   ciaaUpdate_transportType *transport;
   /** Negotiated protocol version */
   uint8_t protocol_version;
   /** Expected sequence number */
   uint8_t sequence_number;
} ciaaUpdate_serverType;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief Initializes a server structure.
 **
 ** \param server Server structure to initialize.
 ** \param transport Transport layer to use.
 ** \return 0 on success. -1 on error.
 **/
int32_t ciaaUpdate_serverInit(ciaaUpdate_serverType *server, ciaaUpdate_transportType *transport);

/** \brief Clears a server structure.
 **
** This function should be called when reception is over.
 ** \param server Server structure to clear.
 **/
void ciaaUpdate_serverClear(ciaaUpdate_serverType *server);

/** \brief Waits and receives a data packet.
 **
 ** This function waits a data packet, extracts its payload and sends an
 ** acknowledgment. If an unexpected packet is received then it is discarded
 ** silently. It fails if a timeout or transport layer error occurs.
 **
 ** \param server Server structure.
 ** \param payload_buffer Buffer where the payload must be returned.
 ** \param buffer_size Buffer size. Must be at least
 ** CIAAUPDATE_PAYLOAD_MAX_SIZE
 ** \return Number of bytes received. -1 on error.
 **/
ssize_t ciaaUpdate_serverRecvData(ciaaUpdate_serverType *server, uint8_t *payload_buffer, size_t buffer_size);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAUPDATE_SERVER_H_ */

