/* Copyright 2014, Mariano Cerdeiro
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
 * MaCe         Mariano Cerdeiro
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

/*==================[internal data declaration]==============================*/

ciaaModbus_cmdLst0x04Type ciaaModbus_cmdLst0x04[] = {
{ { 0x0000, 0x0000 }, NULL /** <= last entry */ }
};

ciaaModbus_cmdLst0x06Type ciaaModbus_cmdLst0x06[] = {
   { { 0x0000, 0x0000 }, NULL /** <= last entry */ }
};


/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

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



int32_t tst_modbusPDUReadInputRegister(uint8_t *buf,
      uint16_t startAddress,
      uint16_t quantityOfRegisters)
{
   buf[0] = 0x04;
   tst_writeInt(&buf[1],startAddress);
   tst_writeInt(&buf[3],quantityOfRegisters);

   return 5;
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
 ** invalid quantity of registers (< 0x0001)
 **/
void test_ciaaModbus_process_02(void)
{
   uint8_t buf[256];
   uint8_t response[] = {0x84, 0x03};
   int32_t ret;

   /* read input register: quantity > 0x007D */
   ret = tst_modbusPDUReadInputRegister(buf, 0, 0X007E);

   ret = ciaaModbus_process(buf, ret);

   TEST_ASSERT_EQUAL_INT8(2, ret);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response, buf, 2);

   /* read input register: quantity < 0x0001 */
   ret = tst_modbusPDUReadInputRegister(buf, 0, 0X0000);

   ret = ciaaModbus_process(buf, ret);

   TEST_ASSERT_EQUAL_INT8(2, ret);
   TEST_ASSERT_EQUAL_INT8_ARRAY(response, buf, 2);

} /* end test_ciaaModbus_process_02 */







/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

