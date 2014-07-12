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
#include "string.h"

/*==================[macros and definitions]=================================*/
/** \brief Type for the stub read functions */
typedef struct {
   int32_t fildes;         /** <= Check for this descriptor */
   int8_t buf[500];        /** <= ascii buffer */
   int32_t totalLength;    /** <= total data length */
   int8_t length[500];     /** <= count of bytes to be returned in each call */
   int32_t count;           /** <= count the count of calls */
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


/**** Helper Functions ****/

/** \brief Init posix read stub */
ciaaPOSIX_read_init(void)
{
   int32_t loopi;

   read_stub.fildes = 0;
   read_stub.totalLength = 0;
   read_stub.count = 0;

   for(loopi = 0; loopi < sizeof(read_stub.buf); loopi++)
   {
      read_stub.buf[loopi] = 0;
   }

   for(loopi = 0; loopi < sizeof(read_stub.length); loopi++)
   {
      read_stub.length[loopi] = 0;
   }
}

/** \brief Add data to posix stub read buffer
 **
 ** \param[in] data pointer containing the data
 ** \param[in] addEnd see table below
 ** \param[in] addLrc see table below
 **
 ** +--------+--------+-----------------------------------------------------+
 ** | addEnd | addLrc | description                                         |
 ** +--------+--------+-----------------------------------------------------+
 ** |   0    |   0    | data is used as is, no CR/LF is added at the end of |
 ** |        |        | the message.                                        |
 ** +--------+--------+-----------------------------------------------------+
 ** |   0    |   1    | LRC is set in the position data[length-3/4]         |
 ** +--------+--------+-----------------------------------------------------+
 ** |   1    |   0    | CRLF is appended after data[length-1]               |
 ** +--------+--------+-----------------------------------------------------+
 ** |   1    |   1    | LRC and CRLF are appended after data[length-1]      |
 ** +--------+--------+-----------------------------------------------------+
 **
 ** \return lenght of bytes written to the buffer
 **/
int32_t ciaaPOSIX_read_add(char * data, int8_t addEnd, int8_t addLrc)
{
   int32_t ret = 0;
   int32_t loopi;
   uint8_t lrc = 0;
   uint8_t aux;
   int32_t startpos = read_stub.totalLength;

   memcpy(&read_stub.buf[startpos], data, strlen(data));
   ret += strlen(data);
   read_stub.totalLength += strlen(data);

   if ((!addEnd) && (!addLrc))
   {
      /* nothing to do */
   }
   else if ((!addEnd) && (addLrc))
   {
      /* calculate lrc */
      for(loopi = startpos + 1; loopi < read_stub.totalLength-4; loopi++)
      {
         if ( ('0' >= read_stub.buf[loopi]) && ('9' <= read_stub.buf[loopi]) )
         {
            aux = (read_stub.buf[loopi] - '0') << 4;
         } else if ( ('A' >= read_stub.buf[loopi]) && ('F' <= read_stub.buf[loopi]) )
         {
            aux = (read_stub.buf[loopi] - 'A' + 10)  << 4;
         }

         loopi++;

         if ( ('0' >= read_stub.buf[loopi]) && ('9' <= read_stub.buf[loopi]) )
         {
            aux += (read_stub.buf[loopi] - '0');
         } else if ( ('A' >= read_stub.buf[loopi]) && ('F' <= read_stub.buf[loopi]) )
         {
            aux += (read_stub.buf[loopi] - 'A' + 10);
         }

         lrc += aux;
      }

      /* complement 2 of lrc */
      lrc = (uint8_t) -lrc;

      /* set lrc */
      read_stub.buf[read_stub.totalLength-4] = ( ( (lrc > 4) > 9 ) ? ( ( (lrc > 4) -10 ) + 'A' ) : ( (lrc > 4) + '0' ) );
      read_stub.buf[read_stub.totalLength-3] = ( ( (lrc & 0xF) > 9 ) ? ( ( (lrc & 0xF) -10 ) + 'A' ) : ( (lrc & 0xF) + '0' ) );
   }
   else if ((addEnd) && (!addLrc))
   {
      ret += 2;

      /* add CRLF */
      read_stub.totalLength += 2;
      read_stub.buf[read_stub.totalLength-2] = 0x0D;
      read_stub.buf[read_stub.totalLength-1] = 0x0A;
   }
   else
   {
      ret += 4;

      /* calculate lrc */
      for(loopi = startpos + 1; loopi < read_stub.totalLength; loopi++) {
         if ( ('0' >= read_stub.buf[loopi]) && ('9' <= read_stub.buf[loopi]) )
         {
            aux = (read_stub.buf[loopi] - '0') << 4;
         } else if ( ('A' >= read_stub.buf[loopi]) && ('F' <= read_stub.buf[loopi]) )
         {
            aux = (read_stub.buf[loopi] - 'A' + 10)  << 4;
         }

         loopi++;

         if ( ('0' >= read_stub.buf[loopi]) && ('9' <= read_stub.buf[loopi]) )
         {
            aux += (read_stub.buf[loopi] - '0');
         } else if ( ('A' >= read_stub.buf[loopi]) && ('F' <= read_stub.buf[loopi]) )
         {
            aux += (read_stub.buf[loopi] - 'A' + 10);
         }
      }

      read_stub.totalLength += 2;
      /* set lrc */
      read_stub.buf[read_stub.totalLength-2] = ( ( (lrc > 4) > 9 ) ? ( ( (lrc > 4) -10 ) + 'A' ) : ( (lrc > 4) + '0' ) );
      read_stub.buf[read_stub.totalLength-1] = ( ( (lrc & 0xF) > 9 ) ? ( ( (lrc & 0xF) -10 ) + 'A' ) : ( (lrc & 0xF) + '0' ) );

      /* add CRLF */
      read_stub.totalLength += 2;
      read_stub.buf[read_stub.totalLength-2] = 0x0D;
      read_stub.buf[read_stub.totalLength-1] = 0x0A;
   }

   return ret;
} /* end ciaaPOSIX_read_add */

ssize_t ciaaPOSIX_read_stub(int32_t fildes, void * buf, ssize_t nbyte)
{
   ssize_t ret;
   ssize_t trans = 0;
   int32_t loopi;

   /* calculate the already transmitted length */
   for(loopi = 0; loopi < read_stub.count; loopi++)
   {
      trans += read_stub.length[loopi];
   }

   /* length to be returned */
   ret = read_stub.length[read_stub.count];
   /* is 0 return the rest of the available data */
   if (0 == ret)
   {
      ret = read_stub.totalLength - trans;
   }

   /* check that the buffer is big enought */
   if (nbyte < ret)
   {
      /* force failed */
      TEST_ASSERT_TRUE(0);
   }

   /* copy data to the modbus handler */
   memcpy(buf, &read_stub.buf[trans], ret);

   /* increment count */
   read_stub.count++;

   return ret;
}

/** \brief convert ascii to bin for the tests
 **
 ** \param[out] dst destination buffer in binary format
 ** \param[in] src source buffer in ascii format starting with :
 ** \param[in] len length of the ascii buffer
 ** \return lenght of the binary buffer
 **
 **/
int32_t tst_convert2bin(uint8_t * dst, uint8_t * src, int32_t len)
{
}

/** \brief Prepare ascii pdu
 **
 ** \param[in] buf pointer containing the data
 ** \param[in] addEnd see table below
 ** \param[in] addLrc see table below
 **
 ** +--------+--------+-----------------------------------------------------+
 ** | addEnd | addLrc | description                                         |
 ** +--------+--------+-----------------------------------------------------+
 ** |   0    |   0    | data is used as is, no CR/LF is added at the end of |
 ** |        |        | the message.                                        |
 ** +--------+--------+-----------------------------------------------------+
 ** |   0    |   1    | LRC is set in the position data[length-3/4]         |
 ** +--------+--------+-----------------------------------------------------+
 ** |   1    |   0    | CRLF is appended after data[length-1]               |
 ** +--------+--------+-----------------------------------------------------+
 ** |   1    |   1    | LRC and CRLF are appended after data[length-1]      |
 ** +--------+--------+-----------------------------------------------------+
 **
 ** \return lenght of bytes written to the buffer
 **/
int32_t tst_asciipdu(char * buf, int8_t addEnd, int8_t addLrc)
{
}

/** \brief Test ciaaModbus_ascii_receive
 **
 ** Receive a complete Modbus ascii message in one ciaaPOSIX_read call.
 **
 **/
void test_ciaaModbus_ascii_receive_01(void) {
   int32_t read;
   int32_t fildes = 1;
   int8_t buf[500];

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);
   /* init read */
   ciaaPOSIX_read_init();
   memset(buf, 0, sizeof(buf));

   /* set input buffer */
   ciaaPOSIX_read_add(":00010203040506070809", 1, 1);

   /* receive data */
   read = ciaaModbus_ascii_receive(fildes, buf);

   /* check received data */
   TEST_ASSERT_EQUAL_INT8_ARRAY(read_stub.buf, buf, read_stub.totalLength);
   TEST_ASSERT_EQUAL_INT(read_stub.totalLength, read);
   TEST_ASSERT_EQUAL_INT(read_stub.count, 1);
}

/** \brief Test ciaaModbus_ascii_receive
 **
 ** Receive 10 bytes in the first ciaaPOSIX_read call and the rest in the
 ** second call.
 **
 **/
void test_ciaaModbus_ascii_receive_02(void) {
   int32_t read;
   int32_t fildes = 1;
   int8_t buf[500];

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);
   /* init read */
   ciaaPOSIX_read_init();
   memset(buf, 0, sizeof(buf));

   /* set input buffer */
   ciaaPOSIX_read_add(":00010203040506070809", 1, 1);
   /* only return 10 bytes in the first read call */
   read_stub.length[0] = 10;

   /* receive data */
   read = ciaaModbus_ascii_receive(fildes, buf);

   /* check received data */
   TEST_ASSERT_EQUAL_INT8_ARRAY(read_stub.buf, buf, read_stub.totalLength);
   TEST_ASSERT_EQUAL_INT(read_stub.totalLength, read);
   TEST_ASSERT_EQUAL_INT(read_stub.count, 2);
}


/** \brief Test ciaaModbus_ascii_receive
 **
 ** Receive 10, 8, 2, rest from ciaaPOSIX_read.
 **
 **/
void test_ciaaModbus_ascii_receive_03(void) {
   int32_t read;
   int32_t fildes = 1;
   int8_t buf[500];

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);
   /* init read */
   ciaaPOSIX_read_init();
   memset(buf, 0, sizeof(buf));

   /* set input buffer */
   ciaaPOSIX_read_add(":000102030405060708090A0B0C0D0E0F", 1, 1);
   /* only return 10 bytes in the first read call */
   read_stub.length[0] = 10;
   read_stub.length[1] = 8;
   read_stub.length[2] = 2;

   /* receive data */
   read = ciaaModbus_ascii_receive(fildes, buf);

   /* check received data */
   TEST_ASSERT_EQUAL_INT8_ARRAY(read_stub.buf, buf, read_stub.totalLength);
   TEST_ASSERT_EQUAL_INT(read_stub.totalLength, read);
   TEST_ASSERT_EQUAL_INT(read_stub.count, 4);
}

/** \brief Test ciaaModbus_ascii_receive
 **
 ** Receive reads with multi frames. One of the calls to ciaaPOSIX_read returns
 ** the end of a ascii modbus message and the beginning of the next one.
 **
 **/
void test_ciaaModbus_ascii_receive_04(void) {
   int32_t read[2];
   int32_t len[2];
   int32_t fildes = 1;
   int8_t buf[2][500];

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);
   ciaaPOSIX_memcpy_StubWithCallback(memcpy);

   /* init read */
   ciaaPOSIX_read_init();
   memset(buf, 0, sizeof(buf));

   /* set input buffer */
   len[0] = ciaaPOSIX_read_add(
         ":0001020304050607", 1, 1);
        /*012345678901234567890123456789012345678901234567890123456789*/
        /*          1         2         3         4         5         */

   len[1] = ciaaPOSIX_read_add(
         ":000102030405060708090A0B0C0D0E0F", 1, 1);
        /*012345678901234567890123456789012345678901234567890123456789*/
        /*          1         2         3         4         5         */

   /* only return 10 bytes in the first read call */
   read_stub.length[0] = 10;
   read_stub.length[1] = 8;
   read_stub.length[2] = 10;
   read_stub.length[3] = 10;

   /* receive data */
   read[0] = ciaaModbus_ascii_receive(fildes, buf[0]);
   read[1] = ciaaModbus_ascii_receive(fildes, buf[1]);

   /* check received data */
   TEST_ASSERT_EQUAL_INT8_ARRAY(&read_stub.buf[0], &buf[0], len[0]);
   TEST_ASSERT_EQUAL_INT(len[0], read[0]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(&read_stub.buf[21], &buf[1], read[1]);
   TEST_ASSERT_EQUAL_INT(len[1], read[1]);
   TEST_ASSERT_EQUAL_INT(read_stub.count, 5);
}

/** \brief Test ciaaModbus_ascii_receive
 **
 ** Receive reads with multi frames. One of the calls to ciaaPOSIX_read returns
 ** the end of a ascii modbus message and the beginning of the next one.
 **
 **/
void test_ciaaModbus_ascii_receive_05(void) {
   int32_t read[4];
   int32_t len[4];
   int32_t fildes = 1;
   int8_t buf[4][500];

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);
   ciaaPOSIX_memcpy_StubWithCallback(memcpy);

   /* init read */
   ciaaPOSIX_read_init();
   memset(buf, 0, sizeof(buf));

   /* set input buffer */
   len[0] = ciaaPOSIX_read_add(
         ":0001020304050607", 1, 1);
        /*012345678901234567890123456789012345678901234567890123456789*/
        /*          1         2         3         4         5         */

   len[1] = ciaaPOSIX_read_add(
         ":000102030405060708090A0B0C0D0E0F", 1, 1);
        /*012345678901234567890123456789012345678901234567890123456789*/
        /*          1         2         3         4         5         */

   len[2] = ciaaPOSIX_read_add(
         ":AC13A1123445060708090A0B0E0F", 1, 1);
        /*012345678901234567890123456789012345678901234567890123456789*/
        /*          1         2         3         4         5         */

   len[3] = ciaaPOSIX_read_add(
         ":0102030405060708090A0B0C330E0F", 1, 1);
        /*012345678901234567890123456789012345678901234567890123456789*/
        /*          1         2         3         4         5         */

   /* only return 10 bytes in the first read call */
   read_stub.length[0] = 10;
   read_stub.length[1] = 8;
   read_stub.length[2] = 10;
   read_stub.length[3] = 10;
   read_stub.length[4] = 10;
   read_stub.length[5] = 10;
   read_stub.length[6] = 10;
   read_stub.length[7] = 10;

   /* receive data */
   read[0] = ciaaModbus_ascii_receive(fildes, buf[0]);
   read[1] = ciaaModbus_ascii_receive(fildes, buf[1]);
   read[2] = ciaaModbus_ascii_receive(fildes, buf[2]);
   read[3] = ciaaModbus_ascii_receive(fildes, buf[3]);

   /* check received data */
   TEST_ASSERT_EQUAL_INT8_ARRAY(&read_stub.buf[0], &buf[0], len[0]);
   TEST_ASSERT_EQUAL_INT(len[0], read[0]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(&read_stub.buf[21], &buf[1], read[1]);
   TEST_ASSERT_EQUAL_INT(len[1], read[1]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(&read_stub.buf[21+37], &buf[2], read[2]);
   TEST_ASSERT_EQUAL_INT(len[2], read[2]);
   TEST_ASSERT_EQUAL_INT8_ARRAY(&read_stub.buf[21+37+33], &buf[3], read[3]);
   TEST_ASSERT_EQUAL_INT(len[3], read[3]);
   TEST_ASSERT_EQUAL_INT(read_stub.count, 9);
}


/** \brief test ciaaModbus_ascii_ascii2bin
 */
void test_ciaaModbus_ascii_ascii2bin_01(void)
{
   int32_t len[10];
   uint8_t buf[10][500];
   int32_t buflen[10];

   /* set input buffer */
   len[0] = ciaaPOSIX_read_add(
         ":0001020304050607", 1, 1);
        /*012345678901234567890123456789012345678901234567890123456789*/
        /*          1         2         3         4         5         */
/*   len[0] = ciaaModbus_ascii_ascii2bin(buf[0], buflen[0]; */
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

