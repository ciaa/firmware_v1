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

/** \brief This file implements the test of the string library
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */
/** \addtogroup ModuleTests Module Tests
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140613 v0.0.2 MaCe implement min, max, setBit and clearBit
 * 20140611 v0.0.1 MaCe implement power of two
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaLibs_Maths.h"

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

void doNothing(void) {
}

/** \brief test ciaaLibs_isPowerOfTwo
 **/
void test_ciaaLibs_isPowerOfTwo(void) {
   int32_t val;

   val = 500;
   TEST_ASSERT_TRUE(ciaaLibs_isPowerOfTwo(val) == 0);

   val = 1024;
   TEST_ASSERT_TRUE(ciaaLibs_isPowerOfTwo(val) == 1);

   val = 1023;
   TEST_ASSERT_TRUE(ciaaLibs_isPowerOfTwo(val) == 0);

   val = 2048;
   TEST_ASSERT_TRUE(ciaaLibs_isPowerOfTwo(val) == 1);

   val = 1;
   TEST_ASSERT_TRUE(ciaaLibs_isPowerOfTwo(val) == 1);
}

/** \brief test ciaaLibs_min
 **/
void test_ciaaLibs_min(void) {
   TEST_ASSERT_EQUAL_INT(ciaaLibs_min(10, 15), 10);
   TEST_ASSERT_EQUAL_INT(ciaaLibs_min(-10, 15), -10);
   TEST_ASSERT_EQUAL_INT(ciaaLibs_min(-10, -15), -15);
}

/** \brief test ciaaLibs_max
 **/
void test_ciaaLibs_max(void) {
   TEST_ASSERT_EQUAL_INT(ciaaLibs_max(10, 15), 15);
   TEST_ASSERT_EQUAL_INT(ciaaLibs_max(-10, 15), 15);
   TEST_ASSERT_EQUAL_INT(ciaaLibs_max(-10, -15), -10);
}

/** \brief test ciaaLibs_setBit
 **/
void  test_ciaaLibs_setBit(void) {
   int32_t val = 0;

   ciaaLibs_setBit(val, 1);
   TEST_ASSERT_EQUAL_INT(0x2, val);

   ciaaLibs_setBit(val, 7);
   TEST_ASSERT_EQUAL_INT(0x82, val);

   ciaaLibs_setBit(val, 2);
   TEST_ASSERT_EQUAL_INT(0x86, val);

   ciaaLibs_setBit(val, 6);
   TEST_ASSERT_EQUAL_INT(0xC6, val);
}

/** \brief test ciaaLibs_clearBit
 **/
void  test_ciaaLibs_clearBit(void) {
   int32_t val = 0xFF;

   ciaaLibs_clearBit(val, 1);
   TEST_ASSERT_EQUAL_INT(0xFD, val);

   ciaaLibs_clearBit(val, 7);
   TEST_ASSERT_EQUAL_INT(0x7D, val);

   ciaaLibs_clearBit(val, 2);
   TEST_ASSERT_EQUAL_INT(0x79, val);

   ciaaLibs_clearBit(val, 6);
   TEST_ASSERT_EQUAL_INT(0x39, val);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

