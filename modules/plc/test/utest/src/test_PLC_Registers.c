/* Copyright 2015, Juan Cecconi
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

/** \brief This file implements the test of the string library
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup PLC PLC Implementation
 ** @{ */
/** \addtogroup ModuleTests Module Tests
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * JuCe         Juan Cecconi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150522 v0.0.1 JuCe Initial version
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaPOSIX_stdint.h"
#include "PLC_Registers.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

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
void setUp(void) {
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void) {
}

/** \brief test Convert_PLC_1ByteRegister_2_PLC_BYTE
 **
 ** test the function Convert_PLC_1ByteRegister_2_PLC_BYTE with all zeros
 **
 **/
void test_Convert_PLC_1ByteRegister_2_PLC_BYTE_01(void) {
   PLC_BYTE ret_PLC_Byte;
   PLC_1ByteRegister PORT;

   memset(&PORT, 0, sizeof(PORT));
   ret_PLC_Byte = Convert_PLC_1ByteRegister_2_PLC_BYTE(PORT);
   TEST_ASSERT_EQUAL_INT(0, ret_PLC_Byte);
}


/** \brief test Convert_PLC_1ByteRegister_2_PLC_BYTE
 **
 ** test the function Convert_PLC_1ByteRegister_2_PLC_BYTE with bit 0
 **
 **/
void test_Convert_PLC_1ByteRegister_2_PLC_BYTE_02(void) {
   PLC_BYTE ret_PLC_Byte;
   PLC_1ByteRegister PORT;

   memset(&PORT, 0, sizeof(PORT));
   PORT.X.x0 = 1;
   ret_PLC_Byte = Convert_PLC_1ByteRegister_2_PLC_BYTE(PORT);
   TEST_ASSERT_BIT_HIGH(0, ret_PLC_Byte);
}

/** \brief test Convert_PLC_1ByteRegister_2_PLC_BYTE
 **
 ** test the function Convert_PLC_1ByteRegister_2_PLC_BYTE with bit 1
 **
 **/
void test_Convert_PLC_1ByteRegister_2_PLC_BYTE_03(void) {
   PLC_BYTE ret_PLC_Byte;
   PLC_1ByteRegister PORT;

   memset(&PORT, 0, sizeof(PORT));
   PORT.X.x1 = 1;
   ret_PLC_Byte = Convert_PLC_1ByteRegister_2_PLC_BYTE(PORT);
   TEST_ASSERT_BIT_HIGH(1, ret_PLC_Byte);
}

/** \brief test Convert_PLC_1ByteRegister_2_PLC_BYTE
 **
 ** test the function Convert_PLC_1ByteRegister_2_PLC_BYTE with bit 2
 **
 **/
void test_Convert_PLC_1ByteRegister_2_PLC_BYTE_04(void) {
   PLC_BYTE ret_PLC_Byte;
   PLC_1ByteRegister PORT;

   memset(&PORT, 0, sizeof(PORT));
   PORT.X.x2 = 1;
   ret_PLC_Byte = Convert_PLC_1ByteRegister_2_PLC_BYTE(PORT);
   TEST_ASSERT_BIT_HIGH(2, ret_PLC_Byte);
}

/** \brief test Convert_PLC_1ByteRegister_2_PLC_BYTE
 **
 ** test the function Convert_PLC_1ByteRegister_2_PLC_BYTE with bit 3
 **
 **/
void test_Convert_PLC_1ByteRegister_2_PLC_BYTE_05(void) {
   PLC_BYTE ret_PLC_Byte;
   PLC_1ByteRegister PORT;

   memset(&PORT, 0, sizeof(PORT));
   PORT.X.x3 = 1;
   ret_PLC_Byte = Convert_PLC_1ByteRegister_2_PLC_BYTE(PORT);
   TEST_ASSERT_BIT_HIGH(3, ret_PLC_Byte);
}

/** \brief test Convert_PLC_1ByteRegister_2_PLC_BYTE
 **
 ** test the function Convert_PLC_1ByteRegister_2_PLC_BYTE with bit 4
 **
 **/
void test_Convert_PLC_1ByteRegister_2_PLC_BYTE_06(void) {
   PLC_BYTE ret_PLC_Byte;
   PLC_1ByteRegister PORT;

   memset(&PORT, 0, sizeof(PORT));
   PORT.X.x4 = 1;
   ret_PLC_Byte = Convert_PLC_1ByteRegister_2_PLC_BYTE(PORT);
   TEST_ASSERT_BIT_HIGH(4, ret_PLC_Byte);
}

/** \brief test Convert_PLC_1ByteRegister_2_PLC_BYTE
 **
 ** test the function Convert_PLC_1ByteRegister_2_PLC_BYTE with bit 5
 **
 **/
void test_Convert_PLC_1ByteRegister_2_PLC_BYTE_07(void) {
   PLC_BYTE ret_PLC_Byte;
   PLC_1ByteRegister PORT;

   memset(&PORT, 0, sizeof(PORT));
   PORT.X.x5 = 1;
   ret_PLC_Byte = Convert_PLC_1ByteRegister_2_PLC_BYTE(PORT);
   TEST_ASSERT_BIT_HIGH(5, ret_PLC_Byte);
}

/** \brief test Convert_PLC_1ByteRegister_2_PLC_BYTE
 **
 ** test the function Convert_PLC_1ByteRegister_2_PLC_BYTE with bit 6
 **
 **/
void test_Convert_PLC_1ByteRegister_2_PLC_BYTE_08(void) {
   PLC_BYTE ret_PLC_Byte;
   PLC_1ByteRegister PORT;

   memset(&PORT, 0, sizeof(PORT));
   PORT.X.x6 = 1;
   ret_PLC_Byte = Convert_PLC_1ByteRegister_2_PLC_BYTE(PORT);
   TEST_ASSERT_BIT_HIGH(6, ret_PLC_Byte);
}

/** \brief test Convert_PLC_1ByteRegister_2_PLC_BYTE
 **
 ** test the function Convert_PLC_1ByteRegister_2_PLC_BYTE with bit 7
 **
 **/
void test_Convert_PLC_1ByteRegister_2_PLC_BYTE_09(void) {
   PLC_BYTE ret_PLC_Byte;
   PLC_1ByteRegister PORT;

   memset(&PORT, 0, sizeof(PORT));
   PORT.X.x7 = 1;
   ret_PLC_Byte = Convert_PLC_1ByteRegister_2_PLC_BYTE(PORT);
   TEST_ASSERT_BIT_HIGH(7, ret_PLC_Byte);
}

/** \brief test Convert_PLC_BYTE_2_PLC_1ByteRegister
 **
 ** test the function Convert_PLC_BYTE_2_PLC_1ByteRegister
 **
 **/
 void test_Convert_PLC_BYTE_2_PLC_1ByteRegister(void) {
   PLC_BYTE status;
   PLC_1ByteRegister PORT;

   memset(&PORT, 0, sizeof(PORT));
   status = 0x00;
   PORT = Convert_PLC_BYTE_2_PLC_1ByteRegister(status);
   TEST_ASSERT_TRUE(PORT.B.b0 == 0x00);

   memset(&PORT, 0, sizeof(PORT));
   status = 0x01;
   PORT = Convert_PLC_BYTE_2_PLC_1ByteRegister(status);
   TEST_ASSERT_TRUE(PORT.X.x0 == 1 && PORT.B.b0 == 0x01);

   memset(&PORT, 0, sizeof(PORT));
   status = 0x02;
   PORT = Convert_PLC_BYTE_2_PLC_1ByteRegister(status);
   TEST_ASSERT_TRUE(PORT.X.x1 == 1 && PORT.B.b0 == 0x02);

   memset(&PORT, 0, sizeof(PORT));
   status = 0x04;
   PORT = Convert_PLC_BYTE_2_PLC_1ByteRegister(status);
   TEST_ASSERT_TRUE(PORT.X.x2 == 1 && PORT.B.b0 == 0x04);

   memset(&PORT, 0, sizeof(PORT));
   status = 0x08;
   PORT = Convert_PLC_BYTE_2_PLC_1ByteRegister(status);
   TEST_ASSERT_TRUE(PORT.X.x3 == 1 && PORT.B.b0 == 0x08);

   memset(&PORT, 0, sizeof(PORT));
   status = 0x10;
   PORT = Convert_PLC_BYTE_2_PLC_1ByteRegister(status);
   TEST_ASSERT_TRUE(PORT.X.x4 == 1 && PORT.B.b0 == 0x10);

   memset(&PORT, 0, sizeof(PORT));
   status = 0x20;
   PORT = Convert_PLC_BYTE_2_PLC_1ByteRegister(status);
   TEST_ASSERT_TRUE(PORT.X.x5 == 1 && PORT.B.b0 == 0x20);

   memset(&PORT, 0, sizeof(PORT));
   status = 0x40;
   PORT = Convert_PLC_BYTE_2_PLC_1ByteRegister(status);
   TEST_ASSERT_TRUE(PORT.X.x6 == 1 && PORT.B.b0 == 0x40);

   memset(&PORT, 0, sizeof(PORT));
   status = 0x80;
   PORT = Convert_PLC_BYTE_2_PLC_1ByteRegister(status);
   TEST_ASSERT_TRUE(PORT.X.x7 == 1 && PORT.B.b0 == 0x80);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

