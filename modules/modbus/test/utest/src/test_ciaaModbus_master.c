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
/** \addtogroup Modbus CIAA Modbus master test
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * GMuro        Gustavo Muro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20141129 v0.0.1 GMuro implement first tests
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaModbus_master.h"
#include "mock_os.h"
#include "ciaaPOSIX_stdbool.h"

/*==================[macros and definitions]=================================*/

#define CIAA_MODBUS_STARTING_ADDRESS      0X1234
#define CIAA_MODBUS_QUANTITY_OF_HOLD_REG  0x0002
#define CIAA_MODBUS_QUANTITY_OF_INPUT_REG 0x0002
#define SLAVE_ID                          2

/*==================[internal functions declaration]=========================*/

/*==================[internal data declaration]==============================*/
static uint8_t callBackData_slaveId;
static uint8_t callBackData_numFunc;
static uint8_t callBackData_exceptioncode;

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void modbusMaster_cbEndOfComm(
      uint8_t slaveId,
      uint8_t numFunc,
      uint8_t exceptioncode)
{
   callBackData_slaveId = slaveId;
   callBackData_numFunc = numFunc;
   callBackData_exceptioncode = exceptioncode;
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

   GetTaskID_IgnoreAndReturn(E_OK);

   /* init module modbus master */
   ciaaModbus_masterInit();
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
 ** this function test modbus master open
 **
 **/
void test_ciaaModbus_masterOpen_01(void)
{
   uint32_t loopi;
   int32_t hModbusMaster[CIAA_MODBUS_TOTAL_MASTERS+1];

   for (loopi = 0 ; loopi < (CIAA_MODBUS_TOTAL_MASTERS+1) ; loopi++)
   {
      /* open modbus master */
      hModbusMaster[loopi] = ciaaModbus_masterOpen();
   }

   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_MASTERS ; loopi++)
   {
      /* verify */
      TEST_ASSERT_NOT_EQUAL(hModbusMaster[loopi],-1);
   }

   TEST_ASSERT_EQUAL(hModbusMaster[loopi],-1);
}

/** \brief test function read coils
 **
 ** this function test modbus master coils
 ** with callback
 **
 **/
void test_ciaaModbus_masterCmd0x01ReadCoils_01(void)
{
   int32_t hModbusMaster;
   uint8_t coilsValue[256];
   uint8_t slaveIdRecv;
   uint8_t pduRecv[256];
   uint8_t pduRecvExp[256] =
   {
      0X01,
      0x12,
      0x34,
      0x00,
      0x13,
   };
   uint8_t pduSend[256] =
   {
      0X01,
      0x03,
      0XCD,
      0X6B,
      0X05,
   };

   uint32_t sizeRecv;
   uint32_t sizeRecvExp = 5;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* request read holding register */
   ciaaModbus_masterCmd0x01ReadCoils(
         hModbusMaster,
         0X1234,
         0X0013,
         coilsValue,
         SLAVE_ID,
         modbusMaster_cbEndOfComm);

   /* perform task modbus master */
   ciaaModbus_masterTask(hModbusMaster);

   /* receive pdu from master */
   ciaaModbus_masterRecvMsg(
         hModbusMaster,
         &slaveIdRecv,
         pduRecv,
         &sizeRecv);

   /* send message to master */
   ciaaModbus_masterSendMsg(
         hModbusMaster,
         SLAVE_ID,
         pduSend,
         5);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, slaveIdRecv);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(pduRecvExp, pduRecv, 5);

   TEST_ASSERT_EQUAL_UINT32(sizeRecvExp, sizeRecv);

   TEST_ASSERT_EQUAL_UINT8(0XCD, coilsValue[0]);

   TEST_ASSERT_EQUAL_UINT16(0X6B, coilsValue[1]);

   TEST_ASSERT_EQUAL_UINT16(0X05, coilsValue[2]);

   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, callBackData_slaveId);

   TEST_ASSERT_EQUAL_UINT8(0x01, callBackData_numFunc);

   TEST_ASSERT_EQUAL_UINT8(0x00, callBackData_exceptioncode);
}

/** \brief test function read discrete inputs
 **
 ** this function test modbus master read discrete inputs
 ** with callback
 **
 **/
void test_ciaaModbus_masterCmd0x02ReadDiscreteInputs_01(void)
{
   int32_t hModbusMaster;
   uint8_t discreteInputsValue[256];
   uint8_t slaveIdRecv;
   uint8_t pduRecv[256];
   uint8_t pduRecvExp[256] =
   {
      0X02,
      0x12,
      0x34,
      0x00,
      0x13,
   };
   uint8_t pduSend[256] =
   {
      0X02,
      0x03,
      0XCD,
      0X6B,
      0X05,
   };

   uint32_t sizeRecv;
   uint32_t sizeRecvExp = 5;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* request read holding register */
   ciaaModbus_masterCmd0x02ReadDiscreteInputs(
         hModbusMaster,
         0X1234,
         0X0013,
         discreteInputsValue,
         SLAVE_ID,
         modbusMaster_cbEndOfComm);

   /* perform task modbus master */
   ciaaModbus_masterTask(hModbusMaster);

   /* receive pdu from master */
   ciaaModbus_masterRecvMsg(
         hModbusMaster,
         &slaveIdRecv,
         pduRecv,
         &sizeRecv);

   /* send message to master */
   ciaaModbus_masterSendMsg(
         hModbusMaster,
         SLAVE_ID,
         pduSend,
         5);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, slaveIdRecv);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(pduRecvExp, pduRecv, 5);

   TEST_ASSERT_EQUAL_UINT32(sizeRecvExp, sizeRecv);

   TEST_ASSERT_EQUAL_UINT8(0XCD, discreteInputsValue[0]);

   TEST_ASSERT_EQUAL_UINT16(0X6B, discreteInputsValue[1]);

   TEST_ASSERT_EQUAL_UINT16(0X05, discreteInputsValue[2]);

   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, callBackData_slaveId);

   TEST_ASSERT_EQUAL_UINT8(0x02, callBackData_numFunc);

   TEST_ASSERT_EQUAL_UINT8(0x00, callBackData_exceptioncode);
}

/** \brief test function read holding registers
 **
 ** this function test modbus master read holding registers
 ** with callback
 **
 **/
void test_ciaaModbus_masterCmd0x03ReadHoldingRegisters_01(void)
{
   int32_t hModbusMaster;
   int16_t hrValue[CIAA_MODBUS_QUANTITY_OF_HOLD_REG];
   uint8_t slaveIdRecv;
   uint8_t pduRecv[256];
   uint8_t pduRecvExp[256] =
   {
      0X03,
      (CIAA_MODBUS_STARTING_ADDRESS >> 8) & 0XFF,
      (CIAA_MODBUS_STARTING_ADDRESS >> 0) & 0XFF,
      (CIAA_MODBUS_QUANTITY_OF_HOLD_REG >> 8) & 0XFF,
      (CIAA_MODBUS_QUANTITY_OF_HOLD_REG >> 0) & 0XFF,
   };
   uint8_t pduSend[256] =
   {
      0X03,
      CIAA_MODBUS_QUANTITY_OF_HOLD_REG * 2,
      0XAA,
      0X55,
      0X11,
      0X22,
   };

   uint32_t sizeRecv;
   uint32_t sizeRecvExp = 5;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* request read holding register */
   ciaaModbus_masterCmd0x03ReadHoldingRegisters(
         hModbusMaster,
         CIAA_MODBUS_STARTING_ADDRESS,
         CIAA_MODBUS_QUANTITY_OF_HOLD_REG,
         hrValue,
         SLAVE_ID,
         modbusMaster_cbEndOfComm);

   /* perform task modbus master */
   ciaaModbus_masterTask(hModbusMaster);

   /* receive pdu from master */
   ciaaModbus_masterRecvMsg(
         hModbusMaster,
         &slaveIdRecv,
         pduRecv,
         &sizeRecv);

   /* send message to master */
   ciaaModbus_masterSendMsg(
         hModbusMaster,
         SLAVE_ID,
         pduSend,
         6);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, slaveIdRecv);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(pduRecvExp, pduRecv, 5);

   TEST_ASSERT_EQUAL_UINT32(sizeRecvExp, sizeRecv);

   TEST_ASSERT_EQUAL_UINT16(0XAA55, hrValue[0]);

   TEST_ASSERT_EQUAL_UINT16(0X1122, hrValue[1]);

   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, callBackData_slaveId);

   TEST_ASSERT_EQUAL_UINT8(0x03, callBackData_numFunc);

   TEST_ASSERT_EQUAL_UINT8(0x00, callBackData_exceptioncode);
}

/** \brief test function read input registers
 **
 ** this function test modbus master read input registers
 ** with callback
 **
 **/
void test_ciaaModbus_masterCmd0x04ReadInputRegisters_01(void)
{
   int32_t hModbusMaster;
   int16_t irValue[CIAA_MODBUS_QUANTITY_OF_INPUT_REG];
   uint8_t slaveIdRecv;
   uint8_t pduRecv[256];
   uint8_t pduRecvExp[256] =
   {
      0X04,
      (CIAA_MODBUS_STARTING_ADDRESS >> 8) & 0XFF,
      (CIAA_MODBUS_STARTING_ADDRESS >> 0) & 0XFF,
      (CIAA_MODBUS_QUANTITY_OF_INPUT_REG >> 8) & 0XFF,
      (CIAA_MODBUS_QUANTITY_OF_INPUT_REG >> 0) & 0XFF,
   };
   uint8_t pduSend[256] =
   {
      0X04,
      CIAA_MODBUS_QUANTITY_OF_INPUT_REG * 2,
      0XAA,
      0X55,
      0X11,
      0X22,
   };

   uint32_t sizeRecv;
   uint32_t sizeRecvExp = 5;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* request read holding register */
   ciaaModbus_masterCmd0x04ReadInputRegisters(
         hModbusMaster,
         CIAA_MODBUS_STARTING_ADDRESS,
         CIAA_MODBUS_QUANTITY_OF_INPUT_REG,
         irValue,
         SLAVE_ID,
         modbusMaster_cbEndOfComm);

   /* perform task modbus master */
   ciaaModbus_masterTask(hModbusMaster);

   /* receive pdu from master */
   ciaaModbus_masterRecvMsg(
         hModbusMaster,
         &slaveIdRecv,
         pduRecv,
         &sizeRecv);

   /* send message to master */
   ciaaModbus_masterSendMsg(
         hModbusMaster,
         SLAVE_ID,
         pduSend,
         6);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, slaveIdRecv);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(pduRecvExp, pduRecv, 5);

   TEST_ASSERT_EQUAL_UINT32(sizeRecvExp, sizeRecv);

   TEST_ASSERT_EQUAL_UINT16(0XAA55, irValue[0]);

   TEST_ASSERT_EQUAL_UINT16(0X1122, irValue[1]);

   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, callBackData_slaveId);

   TEST_ASSERT_EQUAL_UINT8(0x04, callBackData_numFunc);

   TEST_ASSERT_EQUAL_UINT8(0x00, callBackData_exceptioncode);
}

/** \brief test function write single coil
 **
 ** this function test modbus master write single coil
 ** with callback
 **
 **/
void test_ciaaModbus_masterCmd0x05WriteSingleCoil_01(void)
{
   int32_t hModbusMaster;
   uint8_t slaveIdRecv;
   uint8_t pduRecv[256];
   uint8_t pdu[256] =
   {
      0X05,
      0x00,
      0xAC,
      0XFF,
      0x00,
   };

   uint32_t sizeRecv;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* request read holding register */
   ciaaModbus_masterCmd0x05WriteSingleCoil(
         hModbusMaster,
         0x00AC,
         true,
         SLAVE_ID,
         modbusMaster_cbEndOfComm);

   /* perform task modbus master */
   ciaaModbus_masterTask(hModbusMaster);

   /* receive pdu from master */
   ciaaModbus_masterRecvMsg(
         hModbusMaster,
         &slaveIdRecv,
         pduRecv,
         &sizeRecv);

   /* send message to master */
   ciaaModbus_masterSendMsg(
         hModbusMaster,
         SLAVE_ID,
         pdu,
         5);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, slaveIdRecv);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(pdu, pduRecv, 5);

   TEST_ASSERT_EQUAL_UINT32(5, sizeRecv);

   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, callBackData_slaveId);

   TEST_ASSERT_EQUAL_UINT8(0x05, callBackData_numFunc);

   TEST_ASSERT_EQUAL_UINT8(0x00, callBackData_exceptioncode);
}

/** \brief test function write single coil
 **
 ** this function test modbus master write single coil
 ** with callback
 **
 **/
void test_ciaaModbus_masterCmd0x05WriteSingleCoil_02(void)
{
   int32_t hModbusMaster;
   uint8_t slaveIdRecv;
   uint8_t pduRecv[256];
   uint8_t pdu[256] =
   {
      0X05,
      0x00,
      0xAC,
      0X00,
      0x00,
   };

   uint32_t sizeRecv;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* request read holding register */
   ciaaModbus_masterCmd0x05WriteSingleCoil(
         hModbusMaster,
         0x00AC,
         false,
         SLAVE_ID,
         modbusMaster_cbEndOfComm);

   /* perform task modbus master */
   ciaaModbus_masterTask(hModbusMaster);

   /* receive pdu from master */
   ciaaModbus_masterRecvMsg(
         hModbusMaster,
         &slaveIdRecv,
         pduRecv,
         &sizeRecv);

   /* send message to master */
   ciaaModbus_masterSendMsg(
         hModbusMaster,
         SLAVE_ID,
         pdu,
         5);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, slaveIdRecv);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(pdu, pduRecv, 5);

   TEST_ASSERT_EQUAL_UINT32(5, sizeRecv);

   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, callBackData_slaveId);

   TEST_ASSERT_EQUAL_UINT8(0x05, callBackData_numFunc);

   TEST_ASSERT_EQUAL_UINT8(0x00, callBackData_exceptioncode);
}

/** \brief test function write single register
 **
 ** this function test modbus master write single register
 ** with callback
 **
 **/
void test_ciaaModbus_masterCmd0x06WriteSingleRegister_01(void)
{
   int32_t hModbusMaster;
   uint8_t slaveIdRecv;
   uint8_t pduRecv[256];
   uint8_t pdu[256] =
   {
      0X06,
      0x00,
      0x01,
      0X00,
      0x03,
   };

   uint32_t sizeRecv;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* request write single register */
   ciaaModbus_masterCmd0x06WriteSingleRegister(
         hModbusMaster,
         0x0001,
         0x0003,
         SLAVE_ID,
         modbusMaster_cbEndOfComm);

   /* perform task modbus master */
   ciaaModbus_masterTask(hModbusMaster);

   /* receive pdu from master */
   ciaaModbus_masterRecvMsg(
         hModbusMaster,
         &slaveIdRecv,
         pduRecv,
         &sizeRecv);

   /* send message to master */
   ciaaModbus_masterSendMsg(
         hModbusMaster,
         SLAVE_ID,
         pdu,
         5);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, slaveIdRecv);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(pdu, pduRecv, 5);

   TEST_ASSERT_EQUAL_UINT32(5, sizeRecv);

   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, callBackData_slaveId);

   TEST_ASSERT_EQUAL_UINT8(0x06, callBackData_numFunc);

   TEST_ASSERT_EQUAL_UINT8(0x00, callBackData_exceptioncode);
}

/** \brief test function write multiple coils
 **
 ** this function test modbus master write multiple coils
 ** with callback
 **
 **/
void test_ciaaModbus_masterCmd0x0FWriteMultipleCoilss_01(void)
{
   int32_t hModbusMaster;
   uint8_t coilsValue[] =
   {
      0xCD,
      0x01,
   };
   uint8_t slaveIdRecv;
   uint8_t pduRecv[256];
   uint8_t pduRecvExp[256] =
   {
      0x0F,
      0x00,
      0x13,
      0x00,
      0x0A,
      0x02,
      0xCD,
      0x01,
   };
   uint8_t pduSend[256] =
   {
      0x0F,
      0x00,
      0x13,
      0x00,
      0x0A,
   };

   uint32_t sizeRecv;
   uint32_t sizeRecvExp = 8;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* request read holding register */
   ciaaModbus_masterCmd0x0FWriteMultipleCoils(
         hModbusMaster,
         0X0013,
         0X000A,
         coilsValue,
         SLAVE_ID,
         modbusMaster_cbEndOfComm);

   /* perform task modbus master */
   ciaaModbus_masterTask(hModbusMaster);

   /* receive pdu from master */
   ciaaModbus_masterRecvMsg(
         hModbusMaster,
         &slaveIdRecv,
         pduRecv,
         &sizeRecv);

   /* send message to master */
   ciaaModbus_masterSendMsg(
         hModbusMaster,
         SLAVE_ID,
         pduSend,
         5);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, slaveIdRecv);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(pduRecvExp, pduRecv, sizeRecvExp);

   TEST_ASSERT_EQUAL_UINT32(sizeRecvExp, sizeRecv);

   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, callBackData_slaveId);

   TEST_ASSERT_EQUAL_UINT8(0x0F, callBackData_numFunc);

   TEST_ASSERT_EQUAL_UINT8(0x00, callBackData_exceptioncode);
}

/** \brief test function write multiple register
 **
 ** this function test modbus master write multiple registers
 ** with callback
 **
 **/
void test_ciaaModbus_masterCmd0x10WriteMultipleRegisters_01(void)
{
   int32_t hModbusMaster;
   int16_t hrValue[CIAA_MODBUS_QUANTITY_OF_HOLD_REG] =
   {
      0x1234,
      0x5678,
   };
   uint8_t slaveIdRecv;
   uint8_t pduRecv[256];
   uint8_t pduRecvExp[256] =
   {
      0X10,
      (CIAA_MODBUS_STARTING_ADDRESS >> 8) & 0XFF,
      (CIAA_MODBUS_STARTING_ADDRESS >> 0) & 0XFF,
      (CIAA_MODBUS_QUANTITY_OF_HOLD_REG >> 8) & 0XFF,
      (CIAA_MODBUS_QUANTITY_OF_HOLD_REG >> 0) & 0XFF,
      CIAA_MODBUS_QUANTITY_OF_HOLD_REG * 2,
      0x12,
      0x34,
      0x56,
      0x78,
   };
   uint8_t pduSend[256] =
   {
      0X10,
      (CIAA_MODBUS_STARTING_ADDRESS >> 8) & 0XFF,
      (CIAA_MODBUS_STARTING_ADDRESS >> 0) & 0XFF,
      (CIAA_MODBUS_QUANTITY_OF_HOLD_REG >> 8) & 0XFF,
      (CIAA_MODBUS_QUANTITY_OF_HOLD_REG >> 0) & 0XFF,
   };

   uint32_t sizeRecv;
   uint32_t sizeRecvExp = 10;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* request read holding register */
   ciaaModbus_masterCmd0x10WriteMultipleRegisters(
         hModbusMaster,
         CIAA_MODBUS_STARTING_ADDRESS,
         CIAA_MODBUS_QUANTITY_OF_HOLD_REG,
         hrValue,
         SLAVE_ID,
         modbusMaster_cbEndOfComm);

   /* perform task modbus master */
   ciaaModbus_masterTask(hModbusMaster);

   /* receive pdu from master */
   ciaaModbus_masterRecvMsg(
         hModbusMaster,
         &slaveIdRecv,
         pduRecv,
         &sizeRecv);

   /* send message to master */
   ciaaModbus_masterSendMsg(
         hModbusMaster,
         SLAVE_ID,
         pduSend,
         5);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, slaveIdRecv);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(pduRecvExp, pduRecv, sizeRecvExp);

   TEST_ASSERT_EQUAL_UINT32(sizeRecvExp, sizeRecv);

   TEST_ASSERT_EQUAL_UINT16(0X1234, hrValue[0]);

   TEST_ASSERT_EQUAL_UINT16(0X5678, hrValue[1]);

   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, callBackData_slaveId);

   TEST_ASSERT_EQUAL_UINT8(0x10, callBackData_numFunc);

   TEST_ASSERT_EQUAL_UINT8(0x00, callBackData_exceptioncode);
}

/** \brief test function read write multiple register
 **
 ** this function test modbus master read write multiple registers
 ** with callback
 **
 **/
void test_ciaaModbus_masterCmd0x17ReadWriteMultipleRegisters_01(void)
{
   int32_t hModbusMaster;
   int16_t hrValueR[128];
   int16_t hrValueW[] =
   {
      0x1122,
      0x3344,
      0x5566,
   };
   uint8_t slaveIdRecv;
   uint8_t pduRecv[256];
   uint8_t pduRecvExp[256] =
   {
      0X17,
      0X00,
      0X03,
      0X00,
      0X06,
      0X00,
      0X0E,
      0X00,
      0X03,
      0X06,
      0X11,
      0X22,
      0X33,
      0X44,
      0X55,
      0X66,
   };
   uint8_t pduSend[256] =
   {
      0X17,
      0X0C,
      0XFF,
      0XEE,
      0XDD,
      0XCC,
      0XBB,
      0XAA,
      0X99,
      0X88,
      0X77,
      0X66,
      0X55,
      0X44,
   };

   uint32_t sizeRecv;
   uint32_t sizeRecvExp = 16;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* request read holding register */
   ciaaModbus_masterCmd0x17ReadWriteMultipleRegisters(
         hModbusMaster,
         0X0003,
         0X0006,
         hrValueR,
         0X000E,
         0X0003,
         hrValueW,
         SLAVE_ID,
         modbusMaster_cbEndOfComm);

   /* perform task modbus master */
   ciaaModbus_masterTask(hModbusMaster);

   /* receive pdu from master */
   ciaaModbus_masterRecvMsg(
         hModbusMaster,
         &slaveIdRecv,
         pduRecv,
         &sizeRecv);

   /* send message to master */
   ciaaModbus_masterSendMsg(
         hModbusMaster,
         SLAVE_ID,
         pduSend,
         14);

   /* verify */
   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, slaveIdRecv);

   TEST_ASSERT_EQUAL_UINT8_ARRAY(pduRecvExp, pduRecv, sizeRecvExp);

   TEST_ASSERT_EQUAL_UINT32(sizeRecvExp, sizeRecv);

   TEST_ASSERT_EQUAL_UINT16(0XFFEE, hrValueR[0]);

   TEST_ASSERT_EQUAL_UINT16(0XDDCC, hrValueR[1]);

   TEST_ASSERT_EQUAL_UINT16(0XBBAA, hrValueR[2]);

   TEST_ASSERT_EQUAL_UINT16(0X9988, hrValueR[3]);

   TEST_ASSERT_EQUAL_UINT16(0X7766, hrValueR[4]);

   TEST_ASSERT_EQUAL_UINT16(0X5544, hrValueR[5]);

   TEST_ASSERT_EQUAL_UINT8(SLAVE_ID, callBackData_slaveId);

   TEST_ASSERT_EQUAL_UINT8(0x17, callBackData_numFunc);

   TEST_ASSERT_EQUAL_UINT8(0x00, callBackData_exceptioncode);
}

/** \brief test get Response timeout
 **
 ** this function test get Response timeout
 **
 **/
void test_ciaaModbus_masterGetRespTimeout_01(void)
{
   int32_t hModbusMaster;
   uint32_t timeout;

   /* open modbus master */
   hModbusMaster = ciaaModbus_masterOpen();

   timeout = ciaaModbus_masterGetRespTimeout(hModbusMaster);

   /* default response timeout 300 ms */
   TEST_ASSERT_EQUAL_UINT32(300,timeout);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

