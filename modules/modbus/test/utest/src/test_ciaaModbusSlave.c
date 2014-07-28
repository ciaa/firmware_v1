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
#include "ciaaModbusSlave.h"
#include "mock_ciaaPOSIX_stdio.h"
#include "string.h"

/*==================[macros and definitions]=================================*/
#define TST_QUANTITY_OF_INPUT_REGISTRS    0x0100
#define TST_QUANTITY_OF_HOLDING_REGISTRS  0x0100

#define TST_START_ADDRESS_INPUT_REGISTERS 0X0010
#define TST_END_ADDRESS_INPUT_REGISTERS   (TST_START_ADDRESS_INPUT_REGISTERS+ \
                                           TST_QUANTITY_OF_INPUT_REGISTRS-1)

#define TST_START_ADDRESS_HOLDING_REGISTERS  0X0010
#define TST_END_ADDRESS_HOLDING_REGISTERS    (TST_START_ADDRESS_HOLDING_REGISTERS+ \
                                             TST_QUANTITY_OF_HOLDING_REGISTRS-1)


/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*
 * Modbus Map
 *
 * Input Registers:
 * Accessed through:
 *             - 0x04 Read Input Register
 *                ---------------------
 * start:0x0010  |   inputRegVal[0]    |
 *               |   inputRegVal[1]    |
 *               |   ...               |
 * end:0x010F    |   inputRegVal[255]  |
 *                ---------------------
 *
 * Holding Registers:
 * Accessed through:
 *             - 0x03 Read Holding Register
 *             - 0x06 Write single Register
 *             - 0x10 Write multiple Register
 *             - 0x17 Read/Write multiple Register
 *                ---------------------
 * start:0x0010  |  holdingRegVal[0]   |
 *               |  holdingRegVal[1]   |
 *               |   ...               |
 * end:0x00D7    |  holdingRegVal[199] |
 *                ---------------------
 *
 *
 */


static uint16_t inputRegVal[TST_QUANTITY_OF_INPUT_REGISTRS];
static uint16_t holdingRegVal[TST_QUANTITY_OF_HOLDING_REGISTRS];

/*==================[external data definition]===============================*/

ciaaModbus_cmdLst0x03Type ciaaModbus_cmdLst0x03[4] =
{
   { 0x0000, 0x0000 }, NULL
};

ciaaModbus_cmdLst0x04Type ciaaModbus_cmdLst0x04[4] =
{
   { 0x0000, 0x0000 }, NULL
};

ciaaModbus_cmdLst0x06Type ciaaModbus_cmdLst0x06[4] =
{
   { 0x0000, 0x0000 }, NULL
};

ciaaModbus_cmdLst0x10Type ciaaModbus_cmdLst0x10[4] =
{
   { 0x0000, 0x0000 }, NULL
};

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void){
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void) {
}

void doNothing(void) {
}

/**** Helper Functions ****/

/** \brief Read Integer from modbus
 **
 ** As described in modbus specification, the modbus uses a bigendian format to
 ** transmit integers. This function shall be used to access integers.
 **
 ** \param[in] add address of the first byte of the integer to be read.
 **
 **/
#define tst_readInt(add) \
   ( (((uint8_t*)(add))[0] << 8) | (((uint8_t*)(add))[1]))


/** \brief Write Integer in modbus buffer
 **
 ** As described in modbus specification, the modbus uses a bigendian format to
 ** transmit integers. This function shall be used to access integers.
 **
 ** \param[in] add address of the first byte of the integer to be write.
 ** \param[in] num integer to be write.
 **
 **/
#define tst_writeInt(add,num) \
   do {                                                        \
      (((uint8_t*)(add))[0] = ((uint16_t)(num) >> 8) & 0XFF);  \
      (((uint8_t*)(add))[1] = ((uint16_t)(num) >> 0) & 0XFF);  \
   }while (0)


int8_t tst_readHoldingRegisters(
      uint16_t startingAddress,
      uint16_t quantityOfHoldingRegisters,
      uint8_t * exceptionCode,
      uint8_t * buf
      )
{
   int8_t ret;
   uint16_t loopi;

   if (TST_START_ADDRESS_HOLDING_REGISTERS <= startingAddress)
   {
      for (loopi = 0 ; loopi < quantityOfHoldingRegisters ; loopi++)
      {
         tst_writeInt(&buf[loopi*2], holdingRegVal[loopi]);
      }
      ret = quantityOfHoldingRegisters;
   }
   else
   {
      *exceptionCode = CIAAMODBUS_E_WRONG_STR_ADDR;
      ret = -1;
   }

   return ret;
}

int8_t tst_readInputRegisters(
      uint16_t startingAddress,
      uint16_t quantityOfInputRegisters,
      uint8_t * exceptionCode,
      uint8_t * buf
      )
{
   int8_t ret;
   uint16_t loopi;

   if (TST_START_ADDRESS_INPUT_REGISTERS <= startingAddress)
   {
      for (loopi = 0 ; loopi < quantityOfInputRegisters ; loopi++)
      {
         tst_writeInt(&buf[loopi*2], inputRegVal[loopi]);
      }
      ret = quantityOfInputRegisters;
   }
   else
   {
      *exceptionCode = CIAAMODBUS_E_WRONG_STR_ADDR;
      ret = -1;
   }

   return ret;
}

int8_t tst_writeSingleRegisters(
      uint16_t registerAddress,
      uint16_t registerValue,
      uint8_t * exceptionCode
      )
{
   int8_t ret;

   if ( (registerAddress >= TST_START_ADDRESS_INPUT_REGISTERS) &&
        (registerAddress <= TST_END_ADDRESS_INPUT_REGISTERS) )
   {
      registerAddress -= TST_START_ADDRESS_INPUT_REGISTERS;
      holdingRegVal[registerAddress] = registerValue;
      ret = 1;
   }
   else
   {
      *exceptionCode = CIAAMODBUS_E_WRONG_STR_ADDR;
      ret = -1;
   }

   return ret;
}

int8_t tst_writeMultipleRegisters(
      uint16_t startingAddress,
      uint16_t quantityOfRegisters,
      uint8_t * exceptionCode,
      uint8_t * buf
      )
{
   int8_t ret;
   uint16_t loopi;

   if (TST_START_ADDRESS_HOLDING_REGISTERS <= startingAddress)
   {
      for (loopi = 0 ; loopi < quantityOfRegisters ; loopi++)
      {
         holdingRegVal[loopi] = tst_readInt(&buf[loopi*2]);
      }
      ret = quantityOfRegisters;
   }
   else
   {
      *exceptionCode = CIAAMODBUS_E_WRONG_STR_ADDR;
      ret = -1;
   }

   return ret;
}

int32_t tst_modbusPDUReadHoldingRegister(uint8_t *buf,
      uint16_t startAddress,
      uint16_t quantityOfRegisters)
{
   int32_t ret = 0;

   buf[0] = 0x03;
   ret += 1;
   tst_writeInt(&buf[1], startAddress);
   ret += 2;
   tst_writeInt(&buf[3], quantityOfRegisters);
   ret += 2;

   return ret;
}

int32_t tst_modbusPDUReadInputRegister(uint8_t *buf,
      uint16_t startAddress,
      uint16_t quantityOfRegisters)
{
   int32_t ret = 0;

   buf[0] = 0x04;
   ret += 1;
   tst_writeInt(&buf[1], startAddress);
   ret += 2;
   tst_writeInt(&buf[3], quantityOfRegisters);
   ret += 2;

   return ret;
}

int32_t tst_modbusPDUWriteSingleRegister(uint8_t *buf,
      uint16_t registerAddress,
      uint16_t registerValue)
{
   int32_t ret = 0;

   buf[0] = 0x06;
   ret += 1;
   tst_writeInt(&buf[1], registerAddress);
   ret += 2;
   tst_writeInt(&buf[3], registerValue);
   ret += 2;

   return ret;
}

int32_t tst_modbusPDUWriteMultipleRegisters(uint8_t *buf,
      uint16_t startAddress,
      uint16_t quantityOfRegisters,
      uint16_t *regValue)
{
   int32_t ret = 0;
   uint32_t loopi;

   /* function code */
   buf[0] = 0x10;
   ret += 1;

   /* staring address */
   tst_writeInt(&buf[1], startAddress);
   ret += 2;

   /* quantity of registers */
   tst_writeInt(&buf[3], quantityOfRegisters);
   ret += 2;

   /* byte count */
   buf[5] = quantityOfRegisters * 2;
   ret++;

   /* registers value */
   for (loopi = 0 ; loopi < quantityOfRegisters ; loopi++)
   {
      tst_writeInt(&buf[ret], regValue[loopi]);
      ret += 2;
   }

   return ret;
}


/** \brief Test ciaaModbus_init
 **
 **/
void test_ciaaModbus_init_01(void)
{
   ciaaPOSIX_open_ExpectAndReturn("/test/device", O_RDWR, 1);

   ciaaModbus_init("/test/device");
} /* end test_ciaaModbus_init_01 */


/** \brief Test ciaaModbus_process
 **
 ** Invalid function code
 **
 **/
void test_ciaaModbus_process_01(void)
{
   uint8_t buf[256] = {0};
   uint8_t response[] = {0x80, 0x01};
   int32_t ret = 5;

   ret = ciaaModbus_process(buf, ret);

   TEST_ASSERT_EQUAL_INT8(2, ret);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response, buf, 2);
} /* end test_ciaaModbus_process_01 */



/** \brief Test ciaaModbus_process
 **
 ** Test read input registers
 ** invalid quantity of registers (> 0x007D)
 **/
void test_ciaaModbus_process_02_01(void)
{
   uint8_t buf[256];
   uint8_t response[] = {0x84, 0x03};
   int32_t ret;

   /* read input register: quantity > 0x007D */
   ret = tst_modbusPDUReadInputRegister(buf, 0x0010, 0X007E);

   ret = ciaaModbus_process(buf, ret);

   TEST_ASSERT_EQUAL_INT8(2, ret);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response, buf, 2);
} /* end test_ciaaModbus_process_02_01 */

/** \brief Test ciaaModbus_process
 **
 ** Test read input registers
 ** invalid quantity of registers (< 0x0001)
 **/
void test_ciaaModbus_process_02_02(void)
{
   uint8_t buf[256];
   uint8_t response[] = {0x84, 0x03};
   int32_t ret;

   /* read input register: quantity < 0x0001 */
   ret = tst_modbusPDUReadInputRegister(buf, 0x0010, 0X0000);

   ret = ciaaModbus_process(buf, ret);

   TEST_ASSERT_EQUAL_INT8(2, ret);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response, buf, 2);

} /* end test_ciaaModbus_process_02_02 */



/** \brief Test ciaaModbus_process
 **
 ** test: function 0x04 not implemented
 **
 **/
void test_ciaaModbus_process_03_01(void)
{
   uint8_t buf[256];
   uint8_t response[] = {0x84, 0x01};
   int32_t ret;

   /* function not implemented */
   ciaaModbus_cmdLst0x04[0].fct = NULL;

   /* read input register: address = 0, quantity of registers = 1 */
   ret = tst_modbusPDUReadInputRegister(buf, 0x0000, 0X0001);

   ret = ciaaModbus_process(buf, ret);

   TEST_ASSERT_EQUAL_INT8(2, ret);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response, buf, 2);

} /* end test_ciaaModbus_process_03_01 */

/** \brief Test ciaaModbus_process
 **
 ** test: address out of range in read input registers
 **
 **/
void test_ciaaModbus_process_03_02(void)
{
   uint8_t buf[2][256];
   uint8_t response[] = {0x84, 0x02};
   int32_t ret[2];

   /* function implemented. Address out of range */
   ciaaModbus_cmdLst0x04[0].fct = tst_readInputRegisters;
   ciaaModbus_cmdLst0x04[0].range.maxAdd = TST_END_ADDRESS_INPUT_REGISTERS;
   ciaaModbus_cmdLst0x04[0].range.minAdd = TST_START_ADDRESS_INPUT_REGISTERS;
   ciaaModbus_cmdLst0x04[1].fct = NULL;

   /* create pdu: address = 0X0000, quantity of registers = 0X0001 */
   ret[0] = tst_modbusPDUReadInputRegister(buf[0],0X0000 , 0X0001);

   /* create pdu: address = 0X1000, quantity of registers = 0X0001 */
   ret[1] = tst_modbusPDUReadInputRegister(buf[1], 0x1000, 0X0001);

   ret[0] = ciaaModbus_process(buf[0], ret[0]);
   ret[1] = ciaaModbus_process(buf[1], ret[1]);

   TEST_ASSERT_EQUAL_INT8(2, ret[0]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response, buf[0], 2);
   TEST_ASSERT_EQUAL_INT8(2, ret[1]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response, buf[1], 2);

} /* end test_ciaaModbus_process_03_02 */


/** \brief Test ciaaModbus_process
 **
 ** read input registers
 **
 **/
void test_ciaaModbus_process_04_01(void)
{
   uint8_t buf[2][256];
   uint8_t response[2][256];
   int32_t ret[2];

   /* configure call back */
   ciaaModbus_cmdLst0x04[0].fct = tst_readInputRegisters;
   ciaaModbus_cmdLst0x04[0].range.maxAdd = TST_END_ADDRESS_INPUT_REGISTERS;
   ciaaModbus_cmdLst0x04[0].range.minAdd = TST_START_ADDRESS_INPUT_REGISTERS;
   ciaaModbus_cmdLst0x04[1].fct = NULL;

   /* create pdu: address = 0X0010, quantity of registers = 0X0001 */
   ret[0] = tst_modbusPDUReadInputRegister(buf[0], 0x0010, 0X0001);

   /* create pdu: address = 0X0010, quantity of registers = 0X0004 */
   ret[1] = tst_modbusPDUReadInputRegister(buf[1], 0x0010, 0X0004);

   /* set value of input registers */
   inputRegVal[0] = 0x1234;
   inputRegVal[1] = 0x1111;
   inputRegVal[2] = 0x0000;
   inputRegVal[3] = 0xFFFF;

   /* set response 1 */
   response[0][0] = 0x04;
   response[0][1] = 0x02;
   tst_writeInt(&response[0][2], inputRegVal[0]);

   /* set response 2 */
   response[1][0] = 0x04;
   response[1][1] = 0x08;
   tst_writeInt(&response[1][2], inputRegVal[0]);
   tst_writeInt(&response[1][4], inputRegVal[1]);
   tst_writeInt(&response[1][6], inputRegVal[2]);
   tst_writeInt(&response[1][8], inputRegVal[3]);

   ret[0] = ciaaModbus_process(buf[0], ret[0]);
   ret[1] = ciaaModbus_process(buf[1], ret[1]);

   TEST_ASSERT_EQUAL_INT8(4, ret[0]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response[0], buf[0], 4);
   TEST_ASSERT_EQUAL_INT8(10, ret[1]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response[1], buf[1], 10);

} /* end test_ciaaModbus_process_04_01 */

/** \brief Test ciaaModbus_process
 **
 ** write single registers
 **
 **/
void test_ciaaModbus_process_05_01(void)
{
   uint8_t buf[2][256];
   uint8_t response[2][256];
   int32_t ret[2];

   /* configure call back */
   ciaaModbus_cmdLst0x06[0].fct = tst_writeSingleRegisters;
   ciaaModbus_cmdLst0x06[0].range.maxAdd = TST_END_ADDRESS_HOLDING_REGISTERS;
   ciaaModbus_cmdLst0x06[0].range.minAdd = TST_START_ADDRESS_HOLDING_REGISTERS;
   ciaaModbus_cmdLst0x06[1].fct = NULL;

   /* create pdu: address = 0X0010, register value = 0X1234 */
   ret[0] = tst_modbusPDUWriteSingleRegister(buf[0], 0x0010, 0X1234);

   /* create pdu: address = 0X0011, register value = 0X9876 */
   ret[1] = tst_modbusPDUWriteSingleRegister(buf[1], 0x0011, 0X9876);

   /* set response 1 (equal to request) */
   memcpy(response[0], buf[0], ret[0]);

   /* set response 2 (equal to request) */
   memcpy(response[1], buf[1], ret[1]);

   ret[0] = ciaaModbus_process(buf[0], ret[0]);
   ret[1] = ciaaModbus_process(buf[1], ret[1]);

   TEST_ASSERT_EQUAL_INT8(5, ret[0]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response[0], buf[0], 5);
   TEST_ASSERT_EQUAL_INT16(0x1234, holdingRegVal[0]);

   TEST_ASSERT_EQUAL_INT8(5, ret[1]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response[1], buf[1], 5);
   TEST_ASSERT_EQUAL_INT16(0x9876, holdingRegVal[1]);

} /* end test_ciaaModbus_process_05_01 */


/** \brief Test ciaaModbus_process
 **
 ** read holding registers
 **
 **/
void test_ciaaModbus_process_06_01(void)
{
   uint8_t buf[2][256];
   uint8_t response[2][256];
   int32_t ret[2];

   /* configure call back */
   ciaaModbus_cmdLst0x03[0].fct = tst_readHoldingRegisters;
   ciaaModbus_cmdLst0x03[0].range.maxAdd = TST_END_ADDRESS_HOLDING_REGISTERS;
   ciaaModbus_cmdLst0x03[0].range.minAdd = TST_START_ADDRESS_HOLDING_REGISTERS;
   ciaaModbus_cmdLst0x03[1].fct = NULL;

   /* create pdu: address = 0X0010, quantity of registers = 0X0001 */
   ret[0] = tst_modbusPDUReadHoldingRegister(buf[0], 0x0010, 0X0001);

   /* create pdu: address = 0X0010, quantity of registers = 0X0004 */
   ret[1] = tst_modbusPDUReadHoldingRegister(buf[1], 0x0010, 0X0004);

   /* set value of input registers */
   holdingRegVal[0] = 0x1234;
   holdingRegVal[1] = 0x1111;
   holdingRegVal[2] = 0x0000;
   holdingRegVal[3] = 0xFFFF;

   /* set response 1 */
   response[0][0] = 0x03;
   response[0][1] = 0x02;
   tst_writeInt(&response[0][2], holdingRegVal[0]);

   /* set response 2 */
   response[1][0] = 0x03;
   response[1][1] = 0x08;
   tst_writeInt(&response[1][2], holdingRegVal[0]);
   tst_writeInt(&response[1][4], holdingRegVal[1]);
   tst_writeInt(&response[1][6], holdingRegVal[2]);
   tst_writeInt(&response[1][8], holdingRegVal[3]);

   ret[0] = ciaaModbus_process(buf[0], ret[0]);
   ret[1] = ciaaModbus_process(buf[1], ret[1]);

   TEST_ASSERT_EQUAL_INT8(4, ret[0]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response[0], buf[0], 4);
   TEST_ASSERT_EQUAL_INT8(10, ret[1]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response[1], buf[1], 10);

} /* end test_ciaaModbus_process_06_01 */


/** \brief Test ciaaModbus_process
 **
 ** write multiple registers
 **
 **/
void test_ciaaModbus_process_07_01(void)
{
   uint8_t buf[2][256];
   uint8_t response[2][256];
   int32_t ret[2];
   uint16_t regValue[] = {
      0x4884,
      0x1111,
      0x2222,
      0x3333,
      0x4444,
      0x1234,
      0x5678,
      0x55AA,
   };

   /* configure call back */
   ciaaModbus_cmdLst0x10[0].fct = tst_writeMultipleRegisters;
   ciaaModbus_cmdLst0x10[0].range.maxAdd = TST_END_ADDRESS_HOLDING_REGISTERS;
   ciaaModbus_cmdLst0x10[0].range.minAdd = TST_START_ADDRESS_HOLDING_REGISTERS;
   ciaaModbus_cmdLst0x10[1].fct = NULL;

   /* create pdu: address = 0X0010, */
   ret[0] = tst_modbusPDUWriteMultipleRegisters(buf[0], 0x0010, 0X0001, &regValue[0]);

   /* create pdu: address = 0X0011, */
   ret[1] = tst_modbusPDUWriteMultipleRegisters(buf[1], 0x0011, 0X0007, &regValue[1]);

   /* set response 1 */
   memcpy(response[0], buf[0], 5);

   /* set response 2 */
   memcpy(response[1], buf[1], 5);

   ret[0] = ciaaModbus_process(buf[0], ret[0]);
   ret[1] = ciaaModbus_process(buf[1], ret[1]);

   TEST_ASSERT_EQUAL_INT8(5, ret[0]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response[0], buf[0], 5);
   TEST_ASSERT_EQUAL_INT16(regValue[0], holdingRegVal[0]);

   TEST_ASSERT_EQUAL_INT8(5, ret[1]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response[1], buf[1], 5);
   TEST_ASSERT_EQUAL_INT16_ARRAY(&regValue[1], &holdingRegVal[1], 7);

} /* end test_ciaaModbus_process_07_01 */



/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

