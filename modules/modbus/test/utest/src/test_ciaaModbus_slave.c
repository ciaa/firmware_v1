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
 * 20140722 v0.0.1 GMuro implement first tests
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaModbus_slave.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

#define SLAVE_ID  2

/*==================[internal functions declaration]=========================*/
static uint16_t cmd0x03ReadHoldingReg(
       uint16_t start,
       uint16_t quantity,
       uint8_t * exceptioncode,
       uint8_t * buf
       );

static void cmd0x10WriteMultipleReg(
      uint16_t start,
      uint16_t quantity,
      uint8_t bytecount,
      uint8_t * exceptioncode,
      uint8_t * buf
      );

/*==================[internal data declaration]==============================*/
static int32_t hModbusSlave;




/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static uint16_t cmd0x03ReadHoldingReg(
       uint16_t start,
       uint16_t quantity,
       uint8_t * exceptioncode,
       uint8_t * buf
       )
{
   uint16_t loopi;

   for (loopi = 0 ; loopi < quantity ; loopi++)
   {
      ciaaModbus_writeInt(buf, (loopi+start));
      buf += 2;
   }

   return quantity;
}

static void cmd0x10WriteMultipleReg(
      uint16_t start,
      uint16_t quantity,
      uint8_t bytecount,
      uint8_t * exceptioncode,
      uint8_t * buf
      )
{

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
   GetResource_CMockIgnoreAndReturn(E_OK);

   /* ignore calls ReleaseResource */
   ReleaseResource_CMockIgnoreAndReturn(E_OK);

   /* perform the initialization of modbus slave */
   ciaaModbus_slaveInit();
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

/** \brief test function not supported
 **
 ** this function test response "function not supported"
 **
 **/
void test_ciaaModbus_functionNotSupported_00(void)
{
   uint8_t pduSend[2][256] =
   {
      /* pdu: invalid function */
      {0x00, 0x00, 0x00, 0x00, 0x00},
      /* pdu: function 0x01 */
      {0x03, 0x00, 0x00, 0x00, 0x00},
   };

   uint8_t pduExpected[2][256] =
   {
      /* response: invalid function */
      {0x80, 0x01},
      /* response: invalid function */
      {0x83, 0x01},
   };
   uint8_t pduRecv[2][256];
   uint8_t id[2];
   uint32_t size[2];

   const ciaaModbus_slaveCmd_type callbacksStruct =
   {
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
   };

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   /* send, task and recv */
   ciaaModbus_slaveSendMsgType(
         hModbusSlave,
         SLAVE_ID,
         pduSend[0],
         5);

   ciaaModbus_slaveTask(hModbusSlave);

   ciaaModbus_slaveRecvMsg(
         hModbusSlave,
         &id[0],
         pduRecv[0],
         &size[0]);

   /* send, task and recv */
   ciaaModbus_slaveSendMsgType(
            hModbusSlave,
            SLAVE_ID,
            pduSend[1],
            5);

   ciaaModbus_slaveTask(hModbusSlave);

   ciaaModbus_slaveRecvMsg(
         hModbusSlave,
         &id[1],
         pduRecv[1],
         &size[1]);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[0],
         pduRecv[0],
         2);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[1],
         pduRecv[1],
         2);
}

/** \brief test read holding registers
 **
 ** this function test read holding register on slave
 **
 **/
void test_ciaaModbus_function0x03Msg_01(void)
{
   uint8_t pduSend[3][256] =
   {
      /* pdu: invalid quantity of registers */
      {0x03, 0x00, 0x00, 0x00, 0x7E},
      /* pdu: invalid quantity of registers */
      {0x03, 0x00, 0x00, 0x00, 0x00},
      /* pdu: ok */
      {0x03, 0x00, 0x00, 0x00, 0x01},
   };

   uint8_t pduExpected[3][256] =
   {
      /* response: invalid quantity */
      {0x83, 0x03},
      /* response: invalid quantity */
      {0x83, 0x03},
      /* response: ok */
      {0x03, 0x02, 0x00, 0x00},
   };
   uint8_t pduRecv[3][256];
   uint8_t id[3];
   uint32_t size[3];
   uint32_t loopi;

   const ciaaModbus_slaveCmd_type callbacksStruct =
   {
      NULL,
      NULL,
      cmd0x03ReadHoldingReg,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
   };


   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   for (loopi = 0 ; loopi < 3 ; loopi++)
   {
      /* send, task and recv */
      ciaaModbus_slaveSendMsgType(
            hModbusSlave,
            SLAVE_ID,
            pduSend[loopi],
            5);

      ciaaModbus_slaveTask(hModbusSlave);

      ciaaModbus_slaveRecvMsg(
            hModbusSlave,
            &id[loopi],
            pduRecv[loopi],
            &size[loopi]);
   }

   /* verify */
   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[0],
         pduRecv[0],
         2);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[0]);
   TEST_ASSERT_EQUAL_UINT32(2, size[0]);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[1],
         pduRecv[1],
         2);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[1]);
   TEST_ASSERT_EQUAL_UINT32(2, size[1]);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[2],
         pduRecv[2],
         4);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[2]);
   TEST_ASSERT_EQUAL_UINT32(4, size[2]);
}

/** \brief test write multiple registers
 **
 ** this function test write multiple registers on slave
 **
 **/
void test_ciaaModbus_function0x10Msg_01(void)
{
   uint8_t pduSend[4][256] =
   {
      /* pdu: invalid quantity of registers */
      {0x10, 0x00, 0x00, 0x00, 0x7C, 0xF8, 0X00, 0X00},
      /* pdu: invalid quantity of registers */
      {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0X00, 0X00},
      /* pdu: invalid byte count */
      {0x10, 0x00, 0x00, 0x00, 0x01, 0x03, 0X00, 0X00},
      /* pdu: ok */
      {0x10, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00},
   };

   uint8_t pduExpected[4][256] =
   {
      /* response: invalid quantity */
      {0x90, 0x03},
      /* response: invalid quantity */
      {0x90, 0x03},
      /* response: invalid quantity */
      {0x90, 0x03},
      /* response: ok */
      {0x10, 0x00, 0x00, 0x00, 0x01},
   };
   uint8_t pduRecv[4][256];
   uint8_t id[4];
   uint32_t size[4];
   uint32_t loopi;

   const ciaaModbus_slaveCmd_type callbacksStruct =
   {
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      cmd0x10WriteMultipleReg,
      NULL,
   };

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   for (loopi = 0 ; loopi < 4 ; loopi++)
   {
      /* send, task and recv */
      ciaaModbus_slaveSendMsgType(
            hModbusSlave,
            SLAVE_ID,
            pduSend[loopi],
            8);

      ciaaModbus_slaveTask(hModbusSlave);

      ciaaModbus_slaveRecvMsg(
            hModbusSlave,
            &id[loopi],
            pduRecv[loopi],
            &size[loopi]);
   }

   /* verify */
   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[0],
         pduRecv[0],
         2);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[0]);
   TEST_ASSERT_EQUAL_UINT32(2, size[0]);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[1],
         pduRecv[1],
         2);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[1]);
   TEST_ASSERT_EQUAL_UINT32(2, size[1]);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[2],
         pduRecv[2],
         2);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[2]);
   TEST_ASSERT_EQUAL_UINT32(2, size[2]);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[3],
         pduRecv[3],
         5);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[3]);
   TEST_ASSERT_EQUAL_UINT32(5, size[3]);

}




/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

