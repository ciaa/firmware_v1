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

/** \brief This file implements the Flash Update serial transport layer
 **
 ** This file implements the Flash Update serial transport layer. It should
 ** be used for debug proposes only.
 **
 ** \todo change the serial struct. add a void * field to the transport struct.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Serial
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
#include "ciaaPOSIX_stdio.h"
#include "UPDT_ITransport.h"
#include "UPDT_serial.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
/** \brief Sends a packet.
 **
 ** \param serial Serial structure.
 ** \param data Data to send.
 ** \param size Number of bytes to send.
 ** \return Number of bytes sent. -1 on error.
 **/
ssize_t UPDT_serialSend(UPDT_ITransportType *serial, const void *data, size_t size)
{
   ciaaPOSIX_assert(NULL != serial);

   return ciaaPOSIX_write(((UPDT_serialType *) serial)->fd, data, size);
}
/** \brief Receives a packet.
 **
 ** \param serial Serial structure.
 ** \param data Buffer to receive.
 ** \param size Number of bytes to receive.
 ** \return Number of bytes received. -1 on error.
 **/
ssize_t UPDT_serialRecv(UPDT_ITransportType *serial, void *data, size_t size)
{
   ciaaPOSIX_assert(NULL != serial);

   return ciaaPOSIX_read(((UPDT_serialType *) serial)->fd, data, size);
}
/*==================[external functions definition]==========================*/
int32_t UPDT_serialInit(UPDT_serialType *serial, const char *dev)
{
   ciaaPOSIX_assert(NULL != serial && NULL != dev);

   serial->fd = ciaaPOSIX_open(dev, O_RDWR);
   ciaaPOSIX_assert(serial->fd >= 0);

   serial->recv = UPDT_serialRecv;
   serial->send = UPDT_serialSend;
   return 0;
}
void UPDT_serialClear(UPDT_serialType *serial)
{
   ciaaPOSIX_assert(NULL != serial);

   serial->send = NULL;
   serial->recv = NULL;
   ciaaPOSIX_close(serial->fd);
   serial->fd = -1;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

