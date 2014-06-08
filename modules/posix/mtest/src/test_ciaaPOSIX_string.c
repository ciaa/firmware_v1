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
 * 20140608 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaPOSIX_stdint.h"

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

/** \brief test strncmp
 **
 ** test the function ciaaPOSIX_strncmp
 **
 **/
void test_ciaaPOSIX_strncmp(void) {
   int8_t ret;

   ret = ciaaPOSIX_strncmp("/dev/serial", "/dev", 4);
   TEST_ASSERT_TRUE(0 == ret);
   ret = ciaaPOSIX_strncmp("/dev/serial", "/dev", 3);
   TEST_ASSERT_TRUE(0 == ret);
   ret = ciaaPOSIX_strncmp("/dev/serial", "/dev", 5);
   TEST_ASSERT_TRUE(0 < ret);
   ret = ciaaPOSIX_strncmp("/dev/serial", "/dev", 6);
   TEST_ASSERT_TRUE(0 < ret);

   ret = ciaaPOSIX_strncmp("/dev/serial", "/dev/a", 6);
   TEST_ASSERT_TRUE(0 < ret);
   ret = ciaaPOSIX_strncmp("/dev/serial", "/dev/z", 6);
   TEST_ASSERT_TRUE(0 > ret);
   ret = ciaaPOSIX_strncmp("/dev/serial", "/dev/afoo", 6);
   TEST_ASSERT_TRUE(0 < ret);
   ret = ciaaPOSIX_strncmp("/dev/serial", "/dev/zfoo", 6);
   TEST_ASSERT_TRUE(ret < 0);

   ret = ciaaPOSIX_strncmp("/dev/serial", "/dev/zfoo", 6);
   TEST_ASSERT_TRUE(ret < 0);
   ret = ciaaPOSIX_strncmp("/dev/serial", "/dev/afoo", 8);
   TEST_ASSERT_TRUE(0 < ret);
   ret = ciaaPOSIX_strncmp("/dev/serial", "/dev/zfoo", 8);
   TEST_ASSERT_TRUE(0 > ret);
}

/** \brief test strcmp
 **
 ** test the function ciaaPOSIX_strcmp
 **
 **/
void test_ciaaPOSIX_strcmp(void) {
   int8_t ret;

   ret = ciaaPOSIX_strcmp("/dev/serial", "/dev/serial");
   TEST_ASSERT_TRUE(0 == ret);
   ret = ciaaPOSIX_strcmp("/dev/serial", "/dev/aerial");
   TEST_ASSERT_TRUE(0 < ret);
   ret = ciaaPOSIX_strcmp("/dev/serial", "/dev/zerial");
   TEST_ASSERT_TRUE(0 > ret);

   ret = ciaaPOSIX_strcmp("/dev/se", "/dev/serial");
   TEST_ASSERT_TRUE(0 > ret);
   ret = ciaaPOSIX_strcmp("/dev/serial", "/dev/s");
   TEST_ASSERT_TRUE(0 < ret);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

