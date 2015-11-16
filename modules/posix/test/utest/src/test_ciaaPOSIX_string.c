/* Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Juan Cecconi
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
 * JuCe         Juan Cecconi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150311 v0.0.5 JuCe implement memcmp
 * 20150309 v0.0.4 JuCe implement memset
 * 20141116 v0.0.3 JuCe improve strcpy
 * 20140610 v0.0.2 MaCe implement memcpy
 * 20140608 v0.0.1 MaCe implement strlen, strcat, strcmp and strncmp
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_string.h"

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


/** \brief test strcpy
 **
 ** test the function ciaaPOSIX_strcpy
 **
 **/
void test_ciaaPOSIX_strcpy(void) {
   char buffer[100];
   char *ret;
   uint32_t loopi;

   for(loopi = 0; loopi < sizeof(buffer); loopi++)
   {
      buffer[loopi] = 0;
   }
   ret = ciaaPOSIX_strcpy(buffer, "/dev");
   TEST_ASSERT_TRUE(&buffer[4] == ret);
   TEST_ASSERT_TRUE(0 == strcmp(buffer, "/dev"));
   ciaaPOSIX_strcpy(ciaaPOSIX_strcpy(buffer, "/first"), "/second");
   TEST_ASSERT_TRUE(0 == strcmp(buffer, "/first/second"));
}

/** \brief test strcat
 **
 ** test the function ciaaPOSIX_strcat
 **
 **/
void test_ciaaPOSIX_strcat(void) {
   char buffer[100];
   char *ret;
   uint32_t loopi;

   for(loopi = 0; loopi < sizeof(buffer); loopi++)
   {
      buffer[loopi] = 0;
   }
   ret = ciaaPOSIX_strcat(buffer, "/dev");
   TEST_ASSERT_TRUE(buffer == ret);
   ret = ciaaPOSIX_strcat(buffer, "/serial");
   TEST_ASSERT_TRUE(0 == strcmp(buffer, "/dev/serial"));
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
   ret = ciaaPOSIX_strncmp("/dev", "/dev/serial", 6);
   TEST_ASSERT_TRUE(0 > ret);

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

/** \brief test strlen
 **
 ** test the function ciaaPOSIX_strlen
 **
 **/
void test_ciaaPOSIX_strlen(void) {
   size_t ret;

   ret = ciaaPOSIX_strlen("/dev/serial");
   TEST_ASSERT_TRUE(11 == ret);

   ret = ciaaPOSIX_strlen("");
   TEST_ASSERT_TRUE(0 == ret);

   ret = ciaaPOSIX_strlen("/");
   TEST_ASSERT_TRUE(1 == ret);

   ret = ciaaPOSIX_strlen("/dev/s");
   TEST_ASSERT_TRUE(6 == ret);
}


/** \brief test memcpy
 **
 ** test the function ciaaPOSIX_memcpy
 **
 **/
void test_ciaaPOSIX_memcpy(void) {
   char * from = "hello world";
   char to[1000];
   void * ret;
   uint32_t loopi;

   for(loopi = 0; loopi < sizeof(to); loopi++)
   {
      to[loopi] = 0;
   }

   ret = ciaaPOSIX_memcpy((void*)to, (void*)from, 4);
   TEST_ASSERT_TRUE(0 == strncmp(from, to, 4));
   TEST_ASSERT_TRUE(0 != strncmp(from, to, 5));
   TEST_ASSERT_TRUE(to == ret);

   ret = ciaaPOSIX_memcpy((void*)to, (void*)from, strlen(from)+1);
   TEST_ASSERT_TRUE(0 == strcmp(from, to));
   TEST_ASSERT_TRUE(to == ret);
}

/** \brief test memset
 **
 ** test the function ciaaPOSIX_memset
 **
 **/
void test_ciaaPOSIX_memset(void) {
   char buffer[16];
   void * ret;
   uint32_t loopi;

   for(loopi = 0; loopi < sizeof(buffer)-1; loopi++)
   {
      buffer[loopi] = 'A';
   }
   buffer[sizeof(buffer)-1] = 0;

   ret = ciaaPOSIX_memset((void*)&buffer[5], (int)'B', 5);
   TEST_ASSERT_TRUE(0 == strncmp(buffer, "AAAAA", 5));
   TEST_ASSERT_TRUE(0 == strncmp(&buffer[5], "BBBBB", 5));
   TEST_ASSERT_TRUE(0 == strncmp(&buffer[10], "AAAAA", 5));
   TEST_ASSERT_TRUE(&buffer[5] == ret);
}

/** \brief test memcmp
 **
 ** test the function ciaaPOSIX_memcmp
 **
 **/
void test_ciaaPOSIX_memcmp(void) {
   char str1[] = "1234567890";
   char str2[] = "1234567891";
   int32_t ret;

   ret = ciaaPOSIX_memcmp(str1, str2, 9);
   TEST_ASSERT_TRUE(0 == ret);
   ret = ciaaPOSIX_memcmp(str1, str2, 10);
   TEST_ASSERT_TRUE(0 > ret);
   ret = ciaaPOSIX_memcmp(str2, str1, 10);
   TEST_ASSERT_TRUE(0 < ret);
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

