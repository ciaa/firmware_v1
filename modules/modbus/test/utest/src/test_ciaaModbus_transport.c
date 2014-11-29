/* Copyright 2014, Gustavo Muro
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

/** \brief This file implements the test of the modbus library
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Modbus CIAA Modbus
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * GMuro        Gustavo Muro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140906 v0.0.1 GMuro implement first tests
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaModbus_transport.h"
#include "ciaaModbus_config.h"
#include "mock_os.h"
#include "mock_ciaaPOSIX_stdio.h"
#include "mock_ciaaModbus_ascii.h"
#include "os.h"
#include "string.h"

/*==================[macros and definitions]=================================*/
#define CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_ASCII       0
#define CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_RTU         1
#define CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_TCP         2

#define CIAA_MODBUS_TOTAL_TRANSPORTS   (  CIAA_MODBUS_TOTAL_TRANSPORT_ASCII + \
                                          CIAA_MODBUS_TOTAL_TRANSPORT_RTU   + \
                                          CIAA_MODBUS_TOTAL_TRANSPORT_TCP )

/** \brief Default response timeout (milliseconds) */
#define CIAA_MODBUS_TRASNPORT_DEFAULT_TIMEOUT   1500

typedef struct
{
   uint8_t* id;
   uint8_t* pdu;
   uint32_t* size;
   int cmock_num_calls;
}ciaaModbus_asciiRecvMsgMockDataType;

typedef struct
{
   uint8_t id;
   uint8_t* pdu;
   uint32_t size;
   int cmock_num_calls;
}ciaaModbus_asciiSendMsgMockDataType;

/*==================[internal data declaration]==============================*/
static int32_t ciaaModbus_asciiTaskCount[CIAA_MODBUS_TOTAL_TRANSPORT_ASCII];
static ciaaModbus_asciiRecvMsgMockDataType ciaaModbus_asciiRecvMsgMockData[CIAA_MODBUS_TOTAL_TRANSPORT_ASCII];
static ciaaModbus_asciiSendMsgMockDataType ciaaModbus_asciiSendMsgMockData[CIAA_MODBUS_TOTAL_TRANSPORT_ASCII];

static int32_t hModbusAscii;



/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static int32_t ciaaModbus_asciiOpen_CALLBACK(int32_t fildes, int cmock_num_calls)
{
   int32_t ret;

   /* check correct fd */
   if (CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_ASCII != fildes)
   {
      ret = -1;
   }
   else
   {
      ret = hModbusAscii;
      hModbusAscii++;
   }

   return ret;
}

static void ciaaModbus_asciiTask_CALLBACK(int32_t handler, int cmock_num_calls)
{
   ciaaModbus_asciiTaskCount[handler]++;
   printf("ciaaModbus_asciiTaskCount[0]:%d\r\n",ciaaModbus_asciiTaskCount[handler]);
}

static void ciaaModbus_asciiSendMsg_CALLBACK(int32_t handler, uint8_t id, uint8_t* pdu, uint32_t size, int cmock_num_calls)
{
   ciaaModbus_asciiSendMsgMockData[handler].id = id;
   ciaaModbus_asciiSendMsgMockData[handler].pdu = pdu;
   ciaaModbus_asciiSendMsgMockData[handler].size = size;
   ciaaModbus_asciiSendMsgMockData[handler].cmock_num_calls = cmock_num_calls;
}

static void ciaaModbus_asciiRecvMsg_CALLBACK(int32_t handler, uint8_t* id, uint8_t* pdu, uint32_t* size, int cmock_num_calls)
{
   ciaaModbus_asciiRecvMsgMockData[handler].id = id;
   ciaaModbus_asciiRecvMsgMockData[handler].pdu = pdu;
   ciaaModbus_asciiRecvMsgMockData[handler].size = size;
   ciaaModbus_asciiRecvMsgMockData[handler].cmock_num_calls = cmock_num_calls;
}
/*==================[external functions definition]==========================*/
/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void)
{
   /* ignore calls GetResource */
   GetResource_IgnoreAndReturn(E_OK);

   /* ignore calls ReleaseResource */
   ReleaseResource_IgnoreAndReturn(E_OK);

   /* set callback asciiTask */
   ciaaModbus_asciiTask_StubWithCallback(ciaaModbus_asciiTask_CALLBACK);

   /* set callback AsciiOpen */
   ciaaModbus_asciiOpen_StubWithCallback(ciaaModbus_asciiOpen_CALLBACK);

   /* set callback AsciiRecvMsg */
   ciaaModbus_asciiRecvMsg_StubWithCallback(ciaaModbus_asciiRecvMsg_CALLBACK);

   /* set callback AsciiSendMsg */
   ciaaModbus_asciiSendMsg_StubWithCallback(ciaaModbus_asciiSendMsg_CALLBACK);

   /* init transport module */
   ciaaModbus_transportInit();

   /* initi modbus ascii handler count */
   hModbusAscii = 0;
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void)
{
}

void doNothing(void)
{
}



/** \brief Test ciaaModbus_transportOpen
 **
 **/
void test_ciaaModbus_transportOpen_01(void)
{
   int32_t hModbusTransp[10];

   /* open transport in ASCII MASTER mode */
   hModbusTransp[0] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_ASCII,
         CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER);

   /* open transport in ASCII SLAVE mode */
   hModbusTransp[1] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_ASCII,
         CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE);

   /* open transport in RTU MASTER mode */
   hModbusTransp[2] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_RTU,
         CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER);

   /* open transport in RTU SLAVE mode */
   hModbusTransp[3] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_RTU,
         CIAAMODBUS_TRANSPORT_MODE_RTU_SLAVE);

   /* open transport in TCP MASTER mode */
   hModbusTransp[4] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_TCP,
         CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER);

   /* open transport in TCP SLAVE mode */
   hModbusTransp[5] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_TCP,
         CIAAMODBUS_TRANSPORT_MODE_TCP_SLAVE);

   /* open transport in invalid mode */
   hModbusTransp[6] = ciaaModbus_transportOpen(
         0,
         0xFF);

   TEST_ASSERT_EQUAL(0, hModbusTransp[0]);
   TEST_ASSERT_EQUAL(1, hModbusTransp[1]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[2]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[3]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[4]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[5]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[6]);
}

/** \brief test function Open
 **
 ** this function call open more times than allowed
 **
 **/
void test_ciaaModbus_transportOpen_02(void)
{
   /* TODO: implement this test when transport rtu and tcp is implemented */
}

/** \brief test function Task
 **
 ** this function test transport Task
 **
 **/
void test_ciaaModbus_transportTask_01(void)
{
   int32_t hModbusTransp[10];
   uint32_t loopi;

   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_TRANSPORT_ASCII; loopi++)
   {
      ciaaModbus_asciiTaskCount[loopi] = 0;
   }

   hModbusTransp[0] = ciaaModbus_transportOpen(
            CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_ASCII,
            CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER);

   hModbusTransp[1] = ciaaModbus_transportOpen(
               CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_RTU,
               CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER);

   hModbusTransp[2] = ciaaModbus_transportOpen(
               CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_TCP,
               CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER);

   ciaaModbus_transportTask(hModbusTransp[0]);

   TEST_ASSERT_NOT_EQUAL(-1, hModbusTransp[0]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[1]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[2]);
   TEST_ASSERT_EQUAL(1, ciaaModbus_asciiTaskCount[0]);
}

/** \brief test function RecvMsg
 **
 ** this function test receive message
 **
 **/
void test_ciaaModbus_transportRecvMsg_01(void)
{
   int32_t hModbusTransp[10];
   uint32_t loopi;
   uint8_t *id = (uint8_t*)0x11;
   uint8_t *pPdu = (uint8_t*)0x22;
   uint32_t *size = (uint32_t*)0x33;

   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_TRANSPORT_ASCII; loopi++)
   {
      ciaaModbus_asciiRecvMsgMockData[loopi].id = NULL;
      ciaaModbus_asciiRecvMsgMockData[loopi].pdu = NULL;
      ciaaModbus_asciiRecvMsgMockData[loopi].size = NULL;
      ciaaModbus_asciiRecvMsgMockData[loopi].cmock_num_calls = 0;
   }

   hModbusTransp[0] = ciaaModbus_transportOpen(
            CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_ASCII,
            CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER);

   hModbusTransp[1] = ciaaModbus_transportOpen(
               CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_RTU,
               CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER);

   hModbusTransp[2] = ciaaModbus_transportOpen(
               CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_TCP,
               CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER);

   ciaaModbus_transportRecvMsg(hModbusTransp[0], id, pPdu, size);

   TEST_ASSERT_NOT_EQUAL(-1, hModbusTransp[0]);
   TEST_ASSERT_EQUAL_PTR(id, ciaaModbus_asciiRecvMsgMockData[0].id);
   TEST_ASSERT_EQUAL_PTR(pPdu, ciaaModbus_asciiRecvMsgMockData[0].pdu);
   TEST_ASSERT_EQUAL_PTR(size, ciaaModbus_asciiRecvMsgMockData[0].size);
   TEST_ASSERT_EQUAL(0, ciaaModbus_asciiRecvMsgMockData[0].cmock_num_calls);
}

/** \brief test function SendMsg
 **
 ** this function test send message
 **
 **/
void test_ciaaModbus_transportSendMsg_01(void)
{
   int32_t hModbusTransp[10];
   uint32_t loopi;
   uint8_t id = 0x11;
   uint8_t *pPdu = (uint8_t*)0x22;
   uint32_t size = 0x33;

   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_TRANSPORT_ASCII; loopi++)
   {
      ciaaModbus_asciiSendMsgMockData[loopi].id = 0;
      ciaaModbus_asciiSendMsgMockData[loopi].pdu = NULL;
      ciaaModbus_asciiSendMsgMockData[loopi].size = 0;
      ciaaModbus_asciiSendMsgMockData[loopi].cmock_num_calls = 0;
   }

   hModbusTransp[0] = ciaaModbus_transportOpen(
            CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_ASCII,
            CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER);

   hModbusTransp[1] = ciaaModbus_transportOpen(
               CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_RTU,
               CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER);

   hModbusTransp[2] = ciaaModbus_transportOpen(
               CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_TCP,
               CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER);

   ciaaModbus_transportSendMsg(hModbusTransp[0], id, pPdu, size);

   TEST_ASSERT_NOT_EQUAL(-1, hModbusTransp[0]);
   TEST_ASSERT_EQUAL(id, ciaaModbus_asciiSendMsgMockData[0].id);
   TEST_ASSERT_EQUAL_PTR(pPdu, ciaaModbus_asciiSendMsgMockData[0].pdu);
   TEST_ASSERT_EQUAL(size, ciaaModbus_asciiSendMsgMockData[0].size);
   TEST_ASSERT_EQUAL(0, ciaaModbus_asciiSendMsgMockData[0].cmock_num_calls);
}

/** \brief test function transportGetType
 **
 ** this function test transport get type
 **
 **/
void test_ciaaModbus_transportGetType_01(void)
{
   int32_t hModbusTransp[2];
   int8_t type[2];

   hModbusTransp[0] = ciaaModbus_transportOpen(
            CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_ASCII,
            CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER);

   hModbusTransp[1] = ciaaModbus_transportOpen(
               CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_ASCII,
               CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE);

   type[0] = ciaaModbus_transportGetType(hModbusTransp[0]);

   type[1] = ciaaModbus_transportGetType(hModbusTransp[1]);

   TEST_ASSERT_EQUAL(CIAAMODBUS_TRANSPORT_TYPE_MASTER, type[0]);
   TEST_ASSERT_EQUAL(CIAAMODBUS_TRANSPORT_TYPE_SLAVE, type[1]);
}

/** \brief test function GetRespTimeout
 **
 ** this function test transport get response timeout
 ** and set response timeout
 **
 **/
void test_ciaaModbus_transportGetRespTimeout_01(void)
{
   int32_t hModbusTransp;
   uint32_t timeout;

   hModbusTransp = ciaaModbus_transportOpen(
            CIAA_MODBUS_TRASNPORT_FIL_DES_MODBUS_ASCII,
            CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER);

   ciaaModbus_transportSetRespTimeout(hModbusTransp, CIAA_MODBUS_TRASNPORT_DEFAULT_TIMEOUT);

   timeout = ciaaModbus_transportGetRespTimeout(hModbusTransp);

   TEST_ASSERT_NOT_EQUAL(-1, hModbusTransp);
   TEST_ASSERT_EQUAL(CIAA_MODBUS_TRASNPORT_DEFAULT_TIMEOUT, timeout);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

