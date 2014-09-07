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
/** \addtogroup UnitTests Unit Tests
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140612 v0.0.2 implement ciaaLibs_circBufFull and ciaaLibs_circBufEmpty
 * 20140611 v0.0.1 MaCe implement circular buffer
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "mock_ciaaPOSIX_stdlib.h"
#include "ciaaLibs_CircBuf.h"
#include "stdlib.h"
#include "string.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
void ciaaLibs_circBufPrint(ciaaLibs_CircBufType * cbuf);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
/** \brief print debuf information of a circular buffer
 **
 ** Print into screen: free space, raw free space,
 ** count of occupied bytes, raw count of occupied bytes
 ** position of head and tail and the buffer content.
 **
 **/
void ciaaLibs_circBufPrint(ciaaLibs_CircBufType * cbuf)
{
   printf("Free: %2d RFree: %2d Count: %2d RCount: %2d\n",
         ciaaLibs_circBufSpace(cbuf, cbuf->head),
         ciaaLibs_circBufRawSpace(cbuf, cbuf->head),
         ciaaLibs_circBufCount(cbuf, cbuf->tail),
         ciaaLibs_circBufRawCount(cbuf, cbuf->tail));
   printf("                      0123456789012345678901234567890123456789012345678901234567890123\n");
   printf("Head: %2d Tail: %2d Val:%s\n\n", cbuf->head, cbuf->tail, cbuf->buf);
}
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

/** \brief test ciaaLibs_circBufNew
 **/
void test_ciaaLibs_circBufNew(void) {
   ciaaLibs_CircBufType * cbuf;

   /* try to create a buffer with less than 8 bytes */
   cbuf = ciaaLibs_circBufNew(4);
   TEST_ASSERT_TRUE(cbuf == NULL);

   /* try to create a buffer with size != power of 2 */
   cbuf = ciaaLibs_circBufNew(17);
   TEST_ASSERT_TRUE(cbuf == NULL);

   /* use linux malloc */
   ciaaPOSIX_malloc_StubWithCallback(malloc);

   /* try to create a buffer */
   cbuf = ciaaLibs_circBufNew(8);
   TEST_ASSERT_TRUE(cbuf != NULL);
   /* free reserved memory */
   free(cbuf);

   /* try to create a buffer */
   cbuf = ciaaLibs_circBufNew(16);
   TEST_ASSERT_TRUE(cbuf != NULL);
   /* free reserved memory */
   free(cbuf);
} /* end test_ciaaLibs_circBufNew */


/** \brief test ciaaLibs_circBufNew if no memory is available
 **/
void test_ciaaLibs_circBufNewNoMemory(void) {
   ciaaLibs_CircBufType * cbuf;

   /* force malloc to return NULL */
   ciaaPOSIX_malloc_IgnoreAndReturn(NULL);

   /* try to create a buffer but no mem is available */
   cbuf = ciaaLibs_circBufNew(64);
   TEST_ASSERT_TRUE(cbuf == NULL);

   /* try to create a buffer but no mem is available */
   cbuf = ciaaLibs_circBufNew(8);
   TEST_ASSERT_TRUE(cbuf == NULL);
} /* end test_ciaaLibs_circBufNewferNoMemory */


/** \brief test:
 **            - ciaaLibs_circBufSpace
 **            - ciaaLibs_circBufRawSpace
 **            - ciaaLibs_circBufCount
 **            - ciaaLibs_circBufRawCount
 **/
void test_ciaaLibs_circBufSpaceAndCount(void) {
   ciaaLibs_CircBufType * cbuf;
   char * data = "hallo world 123456789012345678901234567890213456789012345678901234567890";
   char to[100];
   size_t ret;

   /* use linux malloc */
   ciaaPOSIX_malloc_StubWithCallback(malloc);
   ciaaPOSIX_memcpy_StubWithCallback(memcpy);

   /* try to create a buffer but no mem is available */
   cbuf = ciaaLibs_circBufNew(64);
   TEST_ASSERT_TRUE(cbuf != NULL);

   /* 63 bytes shall be free */
   TEST_ASSERT_EQUAL_INT(63, ciaaLibs_circBufSpace(cbuf, cbuf->head));
   TEST_ASSERT_EQUAL_INT(63, ciaaLibs_circBufRawSpace(cbuf, cbuf->head));

   /* 0 bytes shall be occupied */
   TEST_ASSERT_EQUAL_INT(0, ciaaLibs_circBufCount(cbuf, cbuf->tail));
   TEST_ASSERT_EQUAL_INT(0, ciaaLibs_circBufRawCount(cbuf, cbuf->tail));

   /* put 40 bytes */
   TEST_ASSERT_TRUE(40 == ciaaLibs_circBufPut(cbuf, data, 40));

   /* 23 bytes shall be free */
   TEST_ASSERT_EQUAL_INT(23, ciaaLibs_circBufSpace(cbuf, cbuf->head));
   TEST_ASSERT_EQUAL_INT(23, ciaaLibs_circBufRawSpace(cbuf, cbuf->head));

   /* 40 bytes shall be occupied */
   TEST_ASSERT_EQUAL_INT(40, ciaaLibs_circBufCount(cbuf, cbuf->tail));
   TEST_ASSERT_EQUAL_INT(40, ciaaLibs_circBufRawCount(cbuf, cbuf->tail));

   /* get 30 bytes */
   ret = ciaaLibs_circBufGet(cbuf, to, 30);
   TEST_ASSERT_EQUAL_INT(30, ret);

   /* 53 bytes shall be free */
   TEST_ASSERT_EQUAL_INT(53, ciaaLibs_circBufSpace(cbuf, cbuf->head));
   TEST_ASSERT_EQUAL_INT(24, ciaaLibs_circBufRawSpace(cbuf, cbuf->head));

   /* 10 bytes shall be occupied */
   TEST_ASSERT_EQUAL_INT(10, ciaaLibs_circBufCount(cbuf, cbuf->tail));
   TEST_ASSERT_EQUAL_INT(10, ciaaLibs_circBufRawCount(cbuf, cbuf->tail));

   /* put 50 */
   TEST_ASSERT_EQUAL_INT(50, ciaaLibs_circBufPut(cbuf, data, 50));

   /* 3 bytes shall be free */
   TEST_ASSERT_EQUAL_INT(3, ciaaLibs_circBufSpace(cbuf, cbuf->head));
   TEST_ASSERT_EQUAL_INT(3, ciaaLibs_circBufRawSpace(cbuf, cbuf->head));

   /* 60 bytes shall be occupied */
   TEST_ASSERT_EQUAL_INT(60, ciaaLibs_circBufCount(cbuf, cbuf->tail));
   TEST_ASSERT_EQUAL_INT(34, ciaaLibs_circBufRawCount(cbuf, cbuf->tail));

   /* free memory */
   free(cbuf);
}

/** \brief test ciaaLibs_circBufGet and ciaaLibs_circBufPut
 **/
void test_ciaaLibs_circBufPutGet(void) {
   ciaaLibs_CircBufType * cbuf;
   size_t ret;
   char * from = "0hallo123-10HALLO12-20hallo12-30HALLO12-40HALLO12-50hallo12-60-4";
                /*0123456789012345678901234567890123456789012345678901234567890123*/
   char to[100];

   /* use linux malloc, free and memcpy */
   ciaaPOSIX_malloc_StubWithCallback(malloc);
   ciaaPOSIX_memcpy_StubWithCallback(memcpy);
   ciaaPOSIX_free_StubWithCallback(free);

   /* creates a cicular buffer with 64 bytes */
   cbuf = ciaaLibs_circBufNew(64);
   TEST_ASSERT_TRUE(NULL != cbuf);

   /* write 25 */
   ret = ciaaLibs_circBufPut(cbuf, from, 25);
   TEST_ASSERT_TRUE(25 == ret);
   memset((void*)to, 0, 100);

   /* get 20 */
   ret = ciaaLibs_circBufGet(cbuf, to, 20);
   TEST_ASSERT_EQUAL_INT(20, ret);
   TEST_ASSERT_EQUAL_UINT8_ARRAY(to, from, 20);

   /* put 51 */
   ret = ciaaLibs_circBufPut(cbuf, &from[10], 51);
   TEST_ASSERT_EQUAL_INT(51, ret);
   memset((void*)to, 0, 100);

   /* get 50: 5 old and 45 new */
   memset((void*)to, 0, 100);
   ret = ciaaLibs_circBufGet(cbuf, to, 50);
   TEST_ASSERT_EQUAL_INT(50, ret);
   TEST_ASSERT_EQUAL_UINT8_ARRAY(to, &from[20], 5);
   TEST_ASSERT_EQUAL_UINT8_ARRAY(&to[5], &from[10], 45);

   memset((void*)to, 0, 100);
   ret = ciaaLibs_circBufGet(cbuf, to, 50);
   TEST_ASSERT_EQUAL_INT(6, ret);
   TEST_ASSERT_EQUAL_UINT8_ARRAY(to, &from[55], 6);

   /* release buffer */
   ciaaLibs_circBufRel(cbuf);
} /* end test_ciaaLibs_circBufPutGet */


/** \brief test ciaaLibs_circBufEmpty and ciaaLibs_circBufFull
 **/
void test_ciaaLibs_circBufEmptyFull(void) {
   ciaaLibs_CircBufType * cbuf;
   size_t ret;
   char * from = "0hallo123-10HALLO12-20hallo12-30HALLO12-40HALLO12-50hallo12-60-4";
                /*0123456789012345678901234567890123456789012345678901234567890123*/
   char to[100];

   /* use linux malloc, free and memcpy */
   ciaaPOSIX_malloc_StubWithCallback(malloc);
   ciaaPOSIX_memcpy_StubWithCallback(memcpy);
   ciaaPOSIX_free_StubWithCallback(free);

   /* creates a cicular buffer with 64 bytes */
   cbuf = ciaaLibs_circBufNew(64);
   TEST_ASSERT_TRUE(NULL != cbuf);

   /* buffer is empty */
   TEST_ASSERT_TRUE(ciaaLibs_circBufEmpty(cbuf));
   TEST_ASSERT_FALSE(ciaaLibs_circBufFull(cbuf));

   ciaaLibs_circBufPut(cbuf, (void*)from, 10);

   /* buffer is not empty neither full */
   TEST_ASSERT_FALSE(ciaaLibs_circBufEmpty(cbuf));
   TEST_ASSERT_FALSE(ciaaLibs_circBufFull(cbuf));

   ciaaLibs_circBufPut(cbuf, (void*)from, 53);

   /* buffer is full */
   TEST_ASSERT_FALSE(ciaaLibs_circBufEmpty(cbuf));
   TEST_ASSERT_TRUE(ciaaLibs_circBufFull(cbuf));

   ciaaLibs_circBufGet(cbuf, to, 30);

   /* buffer is not empty neither full */
   TEST_ASSERT_FALSE(ciaaLibs_circBufEmpty(cbuf));
   TEST_ASSERT_FALSE(ciaaLibs_circBufFull(cbuf));

   ciaaLibs_circBufPut(cbuf, from, 30);

   /* buffer is full */
   TEST_ASSERT_FALSE(ciaaLibs_circBufEmpty(cbuf));
   TEST_ASSERT_TRUE(ciaaLibs_circBufFull(cbuf));

   ciaaLibs_circBufGet(cbuf, to, 63);

   /* buffer is empty */
   TEST_ASSERT_TRUE(ciaaLibs_circBufEmpty(cbuf));
   TEST_ASSERT_FALSE(ciaaLibs_circBufFull(cbuf));

   /* release buffer */
   ciaaLibs_circBufRel(cbuf);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

