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
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140624 v0.0.1 MaCe implement first tests
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaModbus_ascii.h"
#include "mock_ciaaPOSIX_stdio.h"

/*==================[macros and definitions]=================================*/
/** \brief Type for the stub read functions */
typedef struct {
   int32_t fildes;         /** <= Check for this descriptor */
   int8_t buf[500];        /** <= ascii buffer */
   int32_t totalLenght;    /** <= total data length */
   int8_t lenght[500];     /** <= count of bytes to be returned in each call */
   int8_t count;           /** <= count the count of calls */
} stubType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static stubType read_stub;

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

ciaaPOSIX_read_init(void)
{
   int32_t loopi;

   read_stub.fildes = 0;
   read_stub.totalLenght = 0;
   read_stub.count = 0;

   for(loopi = 0; loopi < sizeof(read_stub.buf); loopi++)
   {
      read_stub.buf[loopi] = 0;
   }

   for(loopi = 0; loopi < sizeof(read_stub.lenght); loopi++)
   {
      read_stub.lenght[loopi] = 0;
   }
}

ciaaPOSIX_read_set(char * data)
{
   read_stub.totalLenght = strlen(data);
   memcpy(read_stub.buf, data, strlen(data));
}

ssize_t ciaaPOSIX_read_stub(int32_t fildes, void * buf, ssize_t nbyte)
{
   ssize_t ret;

   /* length to be returned */
   ret = read_stub.lenght[read_stub.count];

   if (nbyte < ret)
   {
      /* force failed */
      TEST_ASSERT_TRUE(0);
   }

   /* copy data to the modbus handler */
   memcpy(buf, read_stub.buf, ret);

   /* increment count */
   read_stub.count++;

   return ret;
}

/** \brief test ciaaModbus_ascii_convert2bin
 **/
void test_ciaaModbus_convert2bin(void) {
   char buf[10];
   int32_t ret;

   buf[0] = '3';
   buf[1] = '5';

   ret = ciaaModbus_ascii_convert2bin(buf);
   TEST_ASSERT_EQUAL_INT(0x35, ret);

   buf[0] = 'a';
   buf[1] = 'A';
   ret = ciaaModbus_ascii_convert2bin(buf);
   TEST_ASSERT_EQUAL_INT(-1, ret);

   buf[0] = 'A';
   buf[1] = '5';
   ret = ciaaModbus_ascii_convert2bin(buf);
   TEST_ASSERT_EQUAL_INT(0xA5, ret);

   buf[0] = '5';
   buf[1] = 'a';
   ret = ciaaModbus_ascii_convert2bin(buf);
   TEST_ASSERT_EQUAL_INT(-1, ret);

   buf[0] = '1';
   buf[1] = 'F';
   ret = ciaaModbus_ascii_convert2bin(buf);
   TEST_ASSERT_EQUAL_INT(0x1F, ret);
}

void test_ciaaModbus_receiveFirst(void) {
   int32_t read;
   int32_t fildes = 1;
   int8_t buf[500];

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);
   /* init read */
   ciaaPOSIX_read_init();

   /* set input buffer */
   ciaaPOSIX_read_set(":000102030405060708090D0F");
   /* set lenght to be returned by read */
   read_stub.lenght[0] = 10;
   read_stub.lenght[1] = 10;
   read_stub.lenght[2] = 10;
   read_stub.lenght[3] = 10;

   /* receive data */
   read = ciaaModbus_ascii_receive(fildes, buf);

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

