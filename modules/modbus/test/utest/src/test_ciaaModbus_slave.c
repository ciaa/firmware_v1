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
#include "ciaaModbus_config.h"
#include "ciaaModbus_slave.h"
#include "mock_os.h"

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
static uint8_t valueCoil[0XFFFF];
static uint16_t valueHoldReg[0XFFFF];




/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static uint16_t cmd0x01ReadCoils(
       uint16_t start,
       uint16_t quantity,
       uint8_t * exceptioncode,
       uint8_t * buf
       )
{
   uint16_t loopi;
   uint16_t totalBytes;

   totalBytes = quantity / 8;

   if (quantity % 8)
   {
      totalBytes++;
   }

   for (loopi = 0 ; loopi < totalBytes ; loopi++)
   {
      buf[loopi] = loopi;
   }

   return quantity;
}

static uint16_t cmd0x02ReadDiscreteInputs(
       uint16_t start,
       uint16_t quantity,
       uint8_t * exceptioncode,
       uint8_t * buf
       )
{
   uint16_t loopi;
   uint16_t totalBytes;

   totalBytes = quantity / 8;

   if (quantity % 8)
   {
      totalBytes++;
   }

   for (loopi = 0 ; loopi < totalBytes ; loopi++)
   {
      buf[loopi] = loopi;
   }

   return quantity;
}

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
      ciaaModbus_writeInt(buf, valueHoldReg[loopi+start]);
      buf += 2;
   }

   return quantity;
}

static uint16_t cmd0x04ReadInputReg(
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

static void cmd0x05WriteSingleCoil(
      uint16_t start,
      uint8_t * exceptioncode,
      uint8_t * buf)
{
   uint16_t value;

   value = ciaaModbus_readInt(buf);

   valueCoil[start] = (value == 0)? 0 : 1;
}

static void cmd0x06WriteSingleRegister(
      uint16_t start,
      uint8_t * exceptioncode,
      uint8_t * buf)
{
   valueHoldReg[start] = ciaaModbus_readInt(buf);;
}

static void cmd0x0FWriteMultipleCoils(
      uint16_t start,
      uint16_t quantity,
      uint8_t bytecount,
      uint8_t * exceptioncode,
      uint8_t * buf
      )
{
   uint16_t indexCoils = 0;
   uint16_t indexBuf = 0;
   uint8_t mask = 0x01;

   do
   {
      if (buf[indexBuf] & mask)
      {
         valueCoil[start+indexCoils] = 1;
      }
      else
      {
         valueCoil[start+indexCoils] = 0;
      }

      indexCoils++;
      mask = mask << 1;
      if (0 == mask)
      {
         mask = 0x01;
         indexBuf++;
      }
   }while (indexCoils < quantity);
}


static void cmd0x10WriteMultipleReg(
      uint16_t start,
      uint16_t quantity,
      uint8_t bytecount,
      uint8_t * exceptioncode,
      uint8_t * buf
      )
{
   uint16_t loopi;

   for (loopi = 0 ; loopi < quantity ; loopi++)
   {
      valueHoldReg[start+loopi] = ciaaModbus_readInt(&buf[loopi*2]);
   }
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

/** \brief test function Open
 **
 ** this function call slaveOpen with invalid id
 **
 **/
void test_ciaaModbus_slaveOpen_01(void)
{
   int32_t hModbusSlave[2];
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
   };

   /* open modbus slave id = 0*/
   hModbusSlave[0] = ciaaModbus_slaveOpen(&callbacksStruct, 0);

   /* open modbus slave id = 248*/
   hModbusSlave[1] = ciaaModbus_slaveOpen(&callbacksStruct, 248);

   /* verify */
   TEST_ASSERT_EQUAL_INT32(hModbusSlave[0],-1);

   TEST_ASSERT_EQUAL_INT32(hModbusSlave[1],-1);
}

/** \brief test function Open
 **
 ** this function call open more times than allowed
 **
 **/
void test_ciaaModbus_slaveOpen_02(void)
{
   int32_t loopi;
   int32_t hModbusSlave[CIAA_MODBUS_TOTAL_SLAVES+1];
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
   };

   for (loopi = 0 ; loopi < (CIAA_MODBUS_TOTAL_SLAVES+1) ; loopi++)
   {
      /* open modbus slave id = 0*/
      hModbusSlave[loopi] = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);
   }

   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_SLAVES ; loopi++)
   {
      /* verify */
      TEST_ASSERT_NOT_EQUAL(hModbusSlave[loopi],-1);
   }

   TEST_ASSERT_EQUAL(hModbusSlave[loopi],-1);
}

/** \brief test function GetId
 **
 ** this function test slaveGetId
 **
 **/
void test_ciaaModbus_slaveGetId_01(void)
{
   int32_t hModbusSlave;
   uint8_t id;
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
   };

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   /* get id of slave */
   id = ciaaModbus_slaveGetId(hModbusSlave);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id);
}

/** \brief test function not supported
 **
 ** this function test response "function not supported"
 **
 **/
void test_ciaaModbus_functionNotSupported_00(void)
{
   uint8_t pduSend[10][256] =
   {
      /* pdu: invalid function */
      {0x00, 0x00, 0x00, 0x00, 0x00},
      /* pdu: function 0x01 */
      {0x01, 0x00, 0x00, 0x00, 0x00},
      /* pdu: function 0x02 */
      {0x02, 0x00, 0x00, 0x00, 0x00},
      /* pdu: function 0x03 */
      {0x03, 0x00, 0x00, 0x00, 0x00},
      /* pdu: function 0x04 */
      {0x04, 0x00, 0x00, 0x00, 0x00},
      /* pdu: function 0x05 */
      {0x05, 0x00, 0x00, 0x00, 0x00},
      /* pdu: function 0x06 */
      {0x06, 0x00, 0x00, 0x00, 0x00},
      /* pdu: function 0x0F */
      {0x0F, 0x00, 0x00, 0x00, 0x00},
      /* pdu: function 0x10 */
      {0x10, 0x00, 0x00, 0x00, 0x00},
      /* pdu: function 0x1F */
      {0x17, 0x00, 0x00, 0x00, 0x00},
   };

   uint8_t pduExpected[10][256] =
   {
      /* response: invalid function */
      {0x80, 0x01},
      /* response: invalid function */
      {0x81, 0x01},
      /* response: invalid function */
      {0x82, 0x01},
      /* response: invalid function */
      {0x83, 0x01},
      /* response: invalid function */
      {0x84, 0x01},
      /* response: invalid function */
      {0x85, 0x01},
      /* response: invalid function */
      {0x86, 0x01},
      /* response: invalid function */
      {0x8F, 0x01},
      /* response: invalid function */
      {0x90, 0x01},
      /* response: invalid function */
      {0x97, 0x01},
   };
   uint8_t pduRecv[10][256];
   uint8_t id[10];
   uint32_t size[10];
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
      NULL,
   };

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   for (loopi = 0 ; loopi < 10 ; loopi++)
   {
      /* send */
      ciaaModbus_slaveSendMsg(
            hModbusSlave,
            SLAVE_ID,
            pduSend[loopi],
            5);

      /* task and recv */
      ciaaModbus_slaveTask(hModbusSlave);

      /* recv */
      ciaaModbus_slaveRecvMsg(
            hModbusSlave,
            &id[loopi],
            pduRecv[loopi],
            &size[loopi]);
   }

   for (loopi = 0 ; loopi < 10 ; loopi++)
   {
      /* verify PDU */
      TEST_ASSERT_EQUAL_UINT8_ARRAY(
            pduExpected[loopi],
            pduRecv[loopi],
            2);

      /* verify size */
      TEST_ASSERT_EQUAL_UINT8(2, size[loopi]);

      /* verify id */
      TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[loopi]);
   }
}

/** \brief test function not supported
 **
 ** this function test response "function not supported"
 **
 **/
void test_ciaaModbus_functionNotSupported_01(void)
{
   uint8_t pduSend[256] = {0x17, 0x00, 0x00, 0x00, 0x00};

   uint8_t pduExpected[256] = {0x97, 0x01};
   uint8_t pduRecv[256];
   uint8_t id;
   uint32_t size;

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
   };

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   /* send */
   ciaaModbus_slaveSendMsg(
         hModbusSlave,
         SLAVE_ID,
         pduSend,
         5);

   /* task and recv */
   ciaaModbus_slaveTask(hModbusSlave);

   /* recv */
   ciaaModbus_slaveRecvMsg(
         hModbusSlave,
         &id,
         pduRecv,
         &size);

   /* verify PDU */
   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected,
         pduRecv,
         2);

   /* verify size */
   TEST_ASSERT_EQUAL_UINT8(2, size);

   /* verify id */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id);
}

/** \brief test read coils
 **
 ** this function test read coils on slave
 **
 **/
void test_ciaaModbus_function0x01Msg_01(void)
{
   uint8_t pduSend[3][256] =
   {
      /* pdu: invalid quantity of registers */
      {0x01, 0x00, 0x00, 0x07, 0xD1},
      /* pdu: invalid quantity of registers */
      {0x01, 0x00, 0x00, 0x00, 0x00},
      /* pdu: ok */
      {0x01, 0x00, 0x00, 0x00, 0x01},
   };

   uint8_t pduExpected[3][256] =
   {
      /* response: invalid quantity */
      {0x81, 0x03},
      /* response: invalid quantity */
      {0x81, 0x03},
      /* response: ok */
      {0x01, 0x01, 0x00, 0x00},
   };
   uint8_t pduRecv[3][256];
   uint8_t id[3];
   uint32_t size[3];
   uint32_t loopi;

   const ciaaModbus_slaveCmd_type callbacksStruct =
   {
      cmd0x01ReadCoils,
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

   for (loopi = 0 ; loopi < 3 ; loopi++)
   {
      /* send, task and recv */
      ciaaModbus_slaveSendMsg(
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
         3);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[2]);
   TEST_ASSERT_EQUAL_UINT32(3, size[2]);
}

/** \brief test read discrete input
 **
 ** this function test read discrete input on slave
 **
 **/
void test_ciaaModbus_function0x02Msg_01(void)
{
   uint8_t pduSend[3][256] =
   {
      /* pdu: invalid quantity of registers */
      {0x02, 0x00, 0x00, 0x07, 0xD1},
      /* pdu: invalid quantity of registers */
      {0x02, 0x00, 0x00, 0x00, 0x00},
      /* pdu: ok */
      {0x02, 0x00, 0x00, 0x00, 0x01},
   };

   uint8_t pduExpected[3][256] =
   {
      /* response: invalid quantity */
      {0x82, 0x03},
      /* response: invalid quantity */
      {0x82, 0x03},
      /* response: ok */
      {0x02, 0x01, 0x00, 0x00},
   };
   uint8_t pduRecv[3][256];
   uint8_t id[3];
   uint32_t size[3];
   uint32_t loopi;

   const ciaaModbus_slaveCmd_type callbacksStruct =
   {
      NULL,
      cmd0x02ReadDiscreteInputs,
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
      ciaaModbus_slaveSendMsg(
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
         3);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[2]);
   TEST_ASSERT_EQUAL_UINT32(3, size[2]);
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
      {0x03, 0x65, 0x43, 0x00, 0x01},
   };

   uint8_t pduExpected[3][256] =
   {
      /* response: invalid quantity */
      {0x83, 0x03},
      /* response: invalid quantity */
      {0x83, 0x03},
      /* response: ok */
      {0x03, 0x02, 0x98, 0x76},
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
   };

   valueHoldReg[0x6543] = 0x9876;

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   for (loopi = 0 ; loopi < 3 ; loopi++)
   {
      /* send, task and recv */
      ciaaModbus_slaveSendMsg(
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

/** \brief test read input registers
 **
 ** this function test read input register on slave
 **
 **/
void test_ciaaModbus_function0x04Msg_01(void)
{
   uint8_t pduSend[3][256] =
   {
      /* pdu: invalid quantity of registers */
      {0x04, 0x00, 0x00, 0x00, 0x7E},
      /* pdu: invalid quantity of registers */
      {0x04, 0x00, 0x00, 0x00, 0x00},
      /* pdu: ok */
      {0x04, 0x00, 0x00, 0x00, 0x01},
   };

   uint8_t pduExpected[3][256] =
   {
      /* response: invalid quantity */
      {0x84, 0x03},
      /* response: invalid quantity */
      {0x84, 0x03},
      /* response: ok */
      {0x04, 0x02, 0x00, 0x00},
   };
   uint8_t pduRecv[3][256];
   uint8_t id[3];
   uint32_t size[3];
   uint32_t loopi;

   const ciaaModbus_slaveCmd_type callbacksStruct =
   {
      NULL,
      NULL,
      NULL,
      cmd0x04ReadInputReg,
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
      ciaaModbus_slaveSendMsg(
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

/** \brief test write single coil
 **
 ** this function test write single coil with invalid value
 **
 **/
void test_ciaaModbus_function0x05Msg_01(void)
{
   uint8_t pduSend[256] = {0x05, 0x00, 0x00, 0x11, 0x22};
   uint8_t pduExpected[256] = {0x85, 0x03};
   uint8_t pduRecv[256];
   uint8_t id;
   uint32_t size;

   const ciaaModbus_slaveCmd_type callbacksStruct =
   {
      NULL,
      NULL,
      NULL,
      NULL,
      cmd0x05WriteSingleCoil,
      NULL,
      NULL,
      NULL,
   };

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   /* send, task and recv */
   ciaaModbus_slaveSendMsg(
         hModbusSlave,
         SLAVE_ID,
         pduSend,
         5);

   ciaaModbus_slaveTask(hModbusSlave);

   ciaaModbus_slaveRecvMsg(
         hModbusSlave,
         &id,
         pduRecv,
         &size);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected,
         pduRecv,
         2);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id);
   TEST_ASSERT_EQUAL_UINT32(2, size);
}


/** \brief test write single coil
 **
 ** this function test write single coil ON
 **
 **/
void test_ciaaModbus_function0x05Msg_02(void)
{
   uint8_t pduSend[256] = {0x05, 0x12, 0x34, 0xFF, 0x00};
   uint8_t pduExpected[256] = {0x05, 0x12, 0x34, 0xFF, 0x00};
   uint8_t pduRecv[256];
   uint8_t id;
   uint32_t size;

   const ciaaModbus_slaveCmd_type callbacksStruct =
   {
      NULL,
      NULL,
      NULL,
      NULL,
      cmd0x05WriteSingleCoil,
      NULL,
      NULL,
      NULL,
   };

   valueCoil[0x1234] = 0;

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   /* send, task and recv */
   ciaaModbus_slaveSendMsg(
         hModbusSlave,
         SLAVE_ID,
         pduSend,
         5);

   ciaaModbus_slaveTask(hModbusSlave);

   ciaaModbus_slaveRecvMsg(
         hModbusSlave,
         &id,
         pduRecv,
         &size);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected,
         pduRecv,
         5);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id);
   TEST_ASSERT_EQUAL_UINT32(5, size);

   TEST_ASSERT_EQUAL_UINT8(1, valueCoil[0x1234]);
}

/** \brief test write single coil
 **
 ** this function test write single coil Off
 **
 **/
void test_ciaaModbus_function0x05Msg_03(void)
{
   uint8_t pduSend[256] = {0x05, 0x12, 0x34, 0x00, 0x00};
   uint8_t pduExpected[256] = {0x05, 0x12, 0x34, 0x00, 0x00};
   uint8_t pduRecv[256];
   uint8_t id;
   uint32_t size;

   const ciaaModbus_slaveCmd_type callbacksStruct =
   {
      NULL,
      NULL,
      NULL,
      NULL,
      cmd0x05WriteSingleCoil,
      NULL,
      NULL,
      NULL,
   };

   valueCoil[0x1234] = 1;

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   /* send, task and recv */
   ciaaModbus_slaveSendMsg(
         hModbusSlave,
         SLAVE_ID,
         pduSend,
         5);

   ciaaModbus_slaveTask(hModbusSlave);

   ciaaModbus_slaveRecvMsg(
         hModbusSlave,
         &id,
         pduRecv,
         &size);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected,
         pduRecv,
         5);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id);
   TEST_ASSERT_EQUAL_UINT32(5, size);

   TEST_ASSERT_EQUAL_UINT8(0, valueCoil[0x1234]);
}

/** \brief test write single register
 **
 ** this function test write single register
 **
 **/
void test_ciaaModbus_function0x06Msg_01(void)
{
   uint8_t pduSend[256] = {0x06, 0x12, 0x34, 0xAB, 0xCD};
   uint8_t pduExpected[256] = {0x06, 0x12, 0x34, 0xAB, 0xCD};
   uint8_t pduRecv[256];
   uint8_t id;
   uint32_t size;

   const ciaaModbus_slaveCmd_type callbacksStruct =
   {
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      cmd0x06WriteSingleRegister,
      NULL,
      NULL,
   };

   valueHoldReg[0x1234] = 0;

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   /* send, task and recv */
   ciaaModbus_slaveSendMsg(
         hModbusSlave,
         SLAVE_ID,
         pduSend,
         5);

   ciaaModbus_slaveTask(hModbusSlave);

   ciaaModbus_slaveRecvMsg(
         hModbusSlave,
         &id,
         pduRecv,
         &size);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected,
         pduRecv,
         5);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id);
   TEST_ASSERT_EQUAL_UINT32(5, size);

   TEST_ASSERT_EQUAL_UINT16(0xABCD, valueHoldReg[0x1234]);
}

/** \brief test write single register
 **
 ** this function test write single register with wrong pdu length
 **
 **/
void test_ciaaModbus_function0x06Msg_02(void)
{
   uint8_t pduSend[256] = {0x06, 0x12, 0x34, 0xAB, 0xCD, 0x00};
   uint8_t pduExpected[256] = {0x86, 0x03};
   uint8_t pduRecv[256];
   uint8_t id;
   uint32_t size;

   const ciaaModbus_slaveCmd_type callbacksStruct =
   {
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      cmd0x06WriteSingleRegister,
      NULL,
      NULL,
   };

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   /* send, task and recv */
   ciaaModbus_slaveSendMsg(
         hModbusSlave,
         SLAVE_ID,
         pduSend,
         6);

   ciaaModbus_slaveTask(hModbusSlave);

   ciaaModbus_slaveRecvMsg(
         hModbusSlave,
         &id,
         pduRecv,
         &size);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected,
         pduRecv,
         2);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id);
   TEST_ASSERT_EQUAL_UINT32(2, size);
}

/** \brief test write multiple coils
 **
 ** this function test write multiple coils on slave
 **
 **/
void test_ciaaModbus_function0x0FMsg_01(void)
{
   uint8_t pduSend[4][256] =
   {
      /* pdu: invalid quantity of coils */
      {0x0F, 0x00, 0x00, 0x07, 0xB1, 0xF7, 0X00, 0X00},
      /* pdu: invalid quantity of registers */
      {0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0X00, 0X00},
      /* pdu: invalid byte count */
      {0x0F, 0x00, 0x00, 0x00, 0x09, 0x01, 0X00, 0X00},
      /* pdu: ok */
      {0x0F, 0x12, 0x34, 0x00, 0x0A, 0x02, 0xCD, 0x01},
   };
   uint16_t lengthSend[4] =
   {
      8,
      8,
      8,
      8,
   };

   uint8_t pduExpected[4][256] =
   {
      /* response: invalid quantity */
      {0x8F, 0x03},
      /* response: invalid quantity */
      {0x8F, 0x03},
      /* response: invalid quantity */
      {0x8F, 0x03},
      /* response: ok */
      {0x0F, 0x12, 0x34, 0x00, 0x0A},
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
      cmd0x0FWriteMultipleCoils,
      NULL,
   };

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   for (loopi = 0 ; loopi < 4 ; loopi++)
   {
      /* send, task and recv */
      ciaaModbus_slaveSendMsg(
            hModbusSlave,
            SLAVE_ID,
            pduSend[loopi],
            lengthSend[loopi]);

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

   TEST_ASSERT_EQUAL_UINT8(0X01, valueCoil[0x1234+0]);
   TEST_ASSERT_EQUAL_UINT8(0X00, valueCoil[0x1234+1]);
   TEST_ASSERT_EQUAL_UINT8(0X01, valueCoil[0x1234+2]);
   TEST_ASSERT_EQUAL_UINT8(0X01, valueCoil[0x1234+3]);
   TEST_ASSERT_EQUAL_UINT8(0X00, valueCoil[0x1234+4]);
   TEST_ASSERT_EQUAL_UINT8(0X00, valueCoil[0x1234+5]);
   TEST_ASSERT_EQUAL_UINT8(0X01, valueCoil[0x1234+6]);
   TEST_ASSERT_EQUAL_UINT8(0X01, valueCoil[0x1234+7]);
   TEST_ASSERT_EQUAL_UINT8(0X01, valueCoil[0x1234+8]);
   TEST_ASSERT_EQUAL_UINT8(0X00, valueCoil[0x1234+9]);
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
      {0x10, 0x12, 0x34, 0x00, 0x02, 0x04, 0xAA, 0xBB, 0xCC, 0xDD},
   };
   uint16_t lengthSend[4] =
   {
      8,
      8,
      8,
      10,
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
      {0x10, 0x12, 0x34, 0x00, 0x02},
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
   };

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   for (loopi = 0 ; loopi < 4 ; loopi++)
   {
      /* send, task and recv */
      ciaaModbus_slaveSendMsg(
            hModbusSlave,
            SLAVE_ID,
            pduSend[loopi],
            lengthSend[loopi]);

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

   TEST_ASSERT_EQUAL_UINT16(0XAABB, valueHoldReg[0x1234]);
   TEST_ASSERT_EQUAL_UINT16(0XCCDD, valueHoldReg[0x1235]);
}

/** \brief test write multiple registers
 **
 ** this function test write multiple registers on slave
 **
 **/
void test_ciaaModbus_function0x17Msg_01(void)
{
   uint8_t pduSend[6][256] =
   {
      /* pdu: invalid quantity of registers to read */
      {0x17, 0x00, 0x00, 0x00, 0x7E, 0X00, 0X00, 0X00, 0X01, 0X02, 0X00, 0X00},
      /* pdu: invalid quantity of registers to read */
      {0x17, 0x00, 0x00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X01, 0X02, 0X00, 0X00},
      /* pdu: invalid quantity of registers to write */
      {0x17, 0x00, 0x00, 0x00, 0x01, 0X00, 0X00, 0X00, 0X7A, 0XF4, 0X00, 0X00},
      /* pdu: invalid quantity of registers to write */
      {0x17, 0x00, 0x00, 0x00, 0x01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00},
      /* pdu: invalid byte count */
      {0x17, 0x00, 0x00, 0x00, 0x01, 0X00, 0X00, 0X00, 0X01, 0X03, 0X00, 0X00},
      /* pdu: ok */
      {0x17, 0x12, 0x34, 0x00, 0x01, 0x11, 0x22, 0x00, 0x01, 0x02, 0X33, 0X44},
   };

   uint8_t pduExpected[2][256] =
   {
      {0x97, 0x03},
      /* response: ok */
      {0x17, 0x02, 0x55, 0xAA},
   };
   uint8_t pduRecv[6][256];
   uint8_t id[6];
   uint32_t size[6];
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
      cmd0x10WriteMultipleReg,
   };

   valueHoldReg[0x1234] = 0x55AA;

   /* open modbus slave */
   hModbusSlave = ciaaModbus_slaveOpen(&callbacksStruct, SLAVE_ID);

   for (loopi = 0 ; loopi < 6 ; loopi++)
   {
      /* send, task and recv */
      ciaaModbus_slaveSendMsg(
            hModbusSlave,
            SLAVE_ID,
            pduSend[loopi],
            12);

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
         pduExpected[0],
         pduRecv[1],
         2);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[1]);
   TEST_ASSERT_EQUAL_UINT32(2, size[1]);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[0],
         pduRecv[2],
         2);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[2]);
   TEST_ASSERT_EQUAL_UINT32(2, size[2]);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[0],
         pduRecv[3],
         2);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[3]);
   TEST_ASSERT_EQUAL_UINT32(2, size[3]);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[0],
         pduRecv[4],
         2);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[4]);
   TEST_ASSERT_EQUAL_UINT32(2, size[4]);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(
         pduExpected[1],
         pduRecv[5],
         4);
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, id[5]);
   TEST_ASSERT_EQUAL_UINT32(4, size[5]);

   TEST_ASSERT_EQUAL_UINT16(0X3344, valueHoldReg[0x1122]);
}



/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

