/* Copyright 2015, Daniel Cohen
 * Copyright 2015, Esteban Volentini
 * Copyright 2015, Matias Giori
 * Copyright 2015, Franco Salinas
 * Copyright 2015, Pablo Alcorta
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

/** \brief this file implements the unit tests for the functions of the file UPDT_protocol
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup update Implementation
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DC           Daniel Cohen
 * EV           Esteban Volentini
 * MG           Matias Giori
 * FS           Franco Salinas
 * PA           Pablo Alcorta
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151124 v0.0.1  PA  first initial version
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "protocol.h"
#include "UPDT_ITransport.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

static size_t test_UPDT_ITransportSend (UPDT_ITransportType* transport, const void* data, size_t size){
   return size;
}

static size_t test_UPDT_ITransportRecv (UPDT_ITransportType* transport, const void* data, size_t size){
   return size;
}
/*==================[internal data definition]===============================*/
const uint8_t header[10];

UPDT_ITransportType transport;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void test_UPDT_protocolGetPacketType ()
{
   header[0] = 0x15;
   int8_t b = UPDT_protocolGetPacketType(header);
   TEST_ASSERT_TRUE (0x05==b);
}

void test_UPDT_protocolGetPayloadSize()
{
   header[3]=0x15;
   uint16_t b = UPDT_protocolGetPayloadSize(header);
   TEST_ASSERT_TRUE (b == 0xA8);
}

void test_UPDT_protocolGetSequenceNumber ()
{
   header[2]=0x15;
   uint8_t b = UPDT_protocolGetSequenceNumber(header);
   TEST_ASSERT_TRUE (b == 0x15);
}

void test_UPDT_protocolSetHeader()
{
   header[0] = 0x10;
   UPDT_protocolSetHeader(header,0x01,0x02,0x48);
   TEST_ASSERT_TRUE (0x11 = header[0]);
   TEST_ASSERT_TRUE (header[1] == 0);
   TEST_ASSERT_TRUE (header[2] == 0x02);
   TEST_ASSERT_TRUE (header[3] == 0x40);
}

void test_UPDT_protocolRecvSizeNull ()
{
   transport.recv = test_UPDT_ITransportSend;
   int32_t b = UPDT_protocolRecv (transport,header,0);
   TEST_ASSERT_TRUE (b == 0);
}

void test_UPDT_protocolRecvSizeNoNullOk ()
{
   transport.recv = test_UPDT_ITransportSend;
   int32_t b = UPDT_protocolRecv (transport,header,5);
   TEST_ASSERT_TRUE (b == UPDT_PROTOCOL_ERROR_NONE);
}

void test_UPDT_protocolRecvSizeError()
{
   transport.recv = test_UPDT_ITransportSend;
   int32_t b = UPDT_protocolRecv (transport,header,5);
   TEST_ASSERT_TRUE (b == UPDT_PROTOCOL_ERROR_TRANSPORT);
}

void test_UPDT_protocolSendSizeNull ()
{
   transport.send = test_UPDT_ITransportSend();
   int32_t b = UPDT_protocolSend(transport,header,0);
   TEST_ASSERT_TRUE (b == 0);
}

void test_UPDT_protocolSendSizeNoNullOk(){
   transport.send = test_UPDT_ITransportSend();
   int32_t b = UPDT_protocolSend(transport,header,5);
   TEST_ASSERT_TRUE (b==UPDT_PROTOCOL_ERROR_NONE);
}

void test_UPDT_protocolSendSizeError(){
   transport.send = test_UPDT_ITransportSend();
   int32_t b = UPDT_protocolSend(transport,header,5);
   TEST_ASSERT_TRUE (b == UPDT_PROTOCOL_ERROR_TRANSPORT);
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
