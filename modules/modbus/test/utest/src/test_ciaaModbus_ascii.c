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
 * 20140624 v0.0.1 MaCe implement first tests
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaModbus_ascii.h"
#include "ciaaModbus_config.h"
#include "string.h"
#include "mock_ciaaPOSIX_stdio.h"
#include "mock_ciaaPOSIX_string.h"

/*==================[macros and definitions]=================================*/
/** \brief Type for the stub read functions */
typedef struct {
   int32_t fildes;         /** <= Check for this descriptor */
   int8_t buf[600];        /** <= ascii buffer */
   int32_t totalLength;    /** <= total data length */
   int32_t length[500];    /** <= count of bytes to be returned in each call */
   int32_t count;          /** <= count the count of calls */
} stubType;

typedef struct {
   int32_t fildes;
   uint8_t buf[10][500];
   int32_t len[10];
   int32_t count;
} writeStubType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static int32_t tst_asciipdu(uint8_t * buf, int8_t addEnd, int8_t addLrc);
static void ciaaPOSIX_read_init(void);
static void ciaaPOSIX_write_init(void);

/*==================[internal data definition]===============================*/
static stubType read_stub;

static writeStubType write_stub;

static int32_t hModbusAscii;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void)
{
   ciaaPOSIX_read_init();
   ciaaPOSIX_write_init();

   ciaaModbus_asciiInit();
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

/**** Helper Functions ****/

/** \brief Init posix write stub */
static void ciaaPOSIX_write_init(void)
{
   int32_t loopi, loopj;

   write_stub.fildes = 0;
   write_stub.count = 0;

   for(loopi = 0; loopi < 10; loopi++)
   {
      write_stub.len[loopi] = 0;
      for(loopj = 0; loopj < 500; loopj++)
      {
         write_stub.buf[loopi][loopj] = 0xA5;
      }
   }
}

/** \brief Init posix read stub */
static void ciaaPOSIX_read_init(void)
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
   int32_t startpos = read_stub.totalLength;

   memcpy(&read_stub.buf[startpos], data, strlen(data)+1);
   ret = tst_asciipdu((uint8_t *)&read_stub.buf[startpos], addEnd, addLrc);

   read_stub.totalLength += ret;

   return ret;
} /* end ciaaPOSIX_read_add */

ssize_t ciaaPOSIX_read_stub(int32_t fildes, void * buf, size_t nbyte, int cmock_num_calls)
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
ssize_t ciaaPOSIX_write_stub(int32_t fildes, void const * buf, size_t nbyte, int cmock_num_calls)
{
   memcpy(write_stub.buf[write_stub.count], buf, nbyte);
   write_stub.len[write_stub.count] = nbyte;
   write_stub.count++;
   return nbyte;
}

void * memcpy_stub(void* s1, void const* s2, size_t n, int cmock_num_calls)
{
   return memcpy(s1, s2, n);
}


/** \brief convert ascii to bin for the tests
 **
 ** \param[out] dst destination buffer in binary format
 ** \param[in] src source buffer in ascii format starting with :
 **                and without CRLF
 ** \param[in] len length of the ascii buffer
 ** \return lenght of the binary buffer
 **
 **/
int32_t tst_convert2bin(uint8_t * dst, uint8_t * src, int32_t len)
{
   int32_t ret = 0;
   int32_t loopi;
   uint8_t aux;

   for(loopi = 1; (loopi < len) && (-1 != ret); loopi++)
   {
      if ( ('0' <= src[loopi]) && ('9' >= src[loopi]) )
      {
         aux = (src[loopi] - '0') << 4;
      } else if ( ('A' <= src[loopi]) && ('F' >= src[loopi]) )
      {
         aux = (src[loopi] - 'A' + 10)  << 4;
      } else
      {
         ret = -2;
      }

      loopi++;

      if ( ('0' <= src[loopi]) && ('9' >= src[loopi]) )
      {
         aux += (src[loopi] - '0');
      } else if ( ('A' <= src[loopi]) && ('F' >= src[loopi]) )
      {
         aux += (src[loopi] - 'A' + 10);
      } else
      {
         ret = -2;
      }

      *dst = aux;
      dst++;

      ret++;
   }

   return ret;
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
static int32_t tst_asciipdu(uint8_t * buf, int8_t addEnd, int8_t addLrc)
{
   int32_t len = strlen((char*)buf);
   int32_t loopi;
   int32_t ret = strlen((char*)buf);
   uint8_t aux;
   uint8_t lrc = 0;

   if ((!addEnd) && (!addLrc))
   {
      /* nothing to do */
   }
   else if ((!addEnd) && (addLrc))
   {
      /* calculate lrc */
      for(loopi = 1; loopi < len-4; loopi++)
      {
         if ( ('0' <= buf[loopi]) && ('9' >= buf[loopi]) )
         {
            aux = (buf[loopi] - '0') << 4;
         } else if ( ('A' <= buf[loopi]) && ('F' >= buf[loopi]) )
         {
            aux = (buf[loopi] - 'A' + 10)  << 4;
         }

         loopi++;

         if ( ('0' <= buf[loopi]) && ('9' >= buf[loopi]) )
         {
            aux += (buf[loopi] - '0');
         } else if ( ('A' <= buf[loopi]) && ('F' >= buf[loopi]) )
         {
            aux += (buf[loopi] - 'A' + 10);
         }

         lrc += aux;
      }

      /* complement 2 of lrc */
      lrc = (uint8_t) -lrc;

      /* set lrc */
      buf[ret-4] = ( ( (lrc >> 4) > 9 ) ? ( ( (lrc >> 4) -10 ) + 'A' ) : ( (lrc >> 4) + '0' ) );
      buf[ret-3] = ( ( (lrc & 0xF) > 9 ) ? ( ( (lrc & 0xF) -10 ) + 'A' ) : ( (lrc & 0xF) + '0' ) );
   }
   else if ((addEnd) && (!addLrc))
   {
      ret += 2;

      /* add CRLF */
      buf[ret-2] = 0x0D;
      buf[ret-1] = 0x0A;
   }
   else
   {
      /* calculate lrc */
      for(loopi = 1; loopi < len; loopi++)
      {
         if ( ('0' <= buf[loopi]) && ('9' >= buf[loopi]) )
         {
            aux = (buf[loopi] - '0') << 4;
         } else if ( ('A' <= buf[loopi]) && ('F' >= buf[loopi]) )
         {
            aux = (buf[loopi] - 'A' + 10)  << 4;
         }

         loopi++;

         if ( ('0' <= buf[loopi]) && ('9' >= buf[loopi]) )
         {
            aux += (buf[loopi] - '0');
         } else if ( ('A' <= buf[loopi]) && ('F' >= buf[loopi]) )
         {
            aux += (buf[loopi] - 'A' + 10);
         }

         lrc += aux;
      }

      /* complement 2 of lrc */
      lrc = (uint8_t) -lrc;

      /* set lrc */
      ret += 2;
      buf[ret-2] = ( ( (lrc >> 4) > 9 ) ? ( ( (lrc >> 4) -10 ) + 'A' ) : ( (lrc >> 4) + '0' ) );
      buf[ret-1] = ( ( (lrc & 0xF) > 9 ) ? ( ( (lrc & 0xF) -10 ) + 'A' ) : ( (lrc & 0xF) + '0' ) );

      /* add CRLF */
      ret += 2;
      buf[ret-2] = 0x0D;
      buf[ret-1] = 0x0A;
   }

   return ret;
} /* end tst_asciipdu */


/** \brief test ciaaModbus_ascii_ascii2bin
 */
void test_ciaaModbus_ascii_ascii2bin_01(void)
{
   uint32_t lenin[10];
   int32_t lenout[10];
   uint8_t buf[10][2][500];

   strcpy((char*)buf[0][0],
          (const char*)":000102030405060708090A0B0C0D0E0F");
        /*012345678901234567890123456789012345678901234567890123456789*/
        /*          1         2         3         4         5         */
   /* set input buffer (only add LRC) */
   lenin[0] = tst_asciipdu(buf[0][0], 0, 1);
   /* copy the buffer */
   strcpy((char*)buf[0][1], (const char*)buf[0][0]);

   /* call tested function */
   lenout[0] = ciaaModbus_ascii_ascii2bin(buf[0][0], lenin[0]);

   TEST_ASSERT_EQUAL_INT(tst_convert2bin(buf[0][1], buf[0][1], lenin[0]), lenout[0]);
   TEST_ASSERT_EQUAL_UINT8_ARRAY(buf[0][1], buf[0][0], lenout[0]);
}

/** \brief test ciaaModbus_ascii_ascii2bin
 ** second nibble wrong
 */
void test_ciaaModbus_ascii_ascii2bin_02(void)
{
   int32_t lenin[10];
   int32_t lenout[10];
   uint8_t buf[10][2][500];

   strcpy((char*)buf[0][0],
         (const char*)":000G");
   /* set input buffer (only add LRC) */
   lenin[0] = tst_asciipdu(buf[0][0], 0, 1);
   /* copy the buffer */
   strcpy((char*)buf[0][1], (const char*)buf[0][0]);

   /* call tested function */
   lenout[0] = ciaaModbus_ascii_ascii2bin(buf[0][0], lenin[0]);

   TEST_ASSERT_EQUAL_INT(-1, lenout[0]);
}

/** \brief test ciaaModbus_ascii_ascii2bin
 ** first nibble wrong
 */
void test_ciaaModbus_ascii_ascii2bin_03(void)
{
   int32_t lenin[10];
   int32_t lenout[10];
   uint8_t buf[10][2][500];

   strcpy((char*)buf[0][0],
         (const char*)":00G0");
   /* set input buffer (only add LRC) */
   lenin[0] = tst_asciipdu(buf[0][0], 0, 1);
   /* copy the buffer */
   strcpy((char*)buf[0][1], (const char*)buf[0][0]);

   /* call tested function */
   lenout[0] = ciaaModbus_ascii_ascii2bin(buf[0][0], lenin[0]);

   TEST_ASSERT_EQUAL_INT(-1, lenout[0]);
}


/** \brief test ciaaModbus_asciiRecvMsg*/
void test_ciaaModbus_asciiRecvMsg_01(void) {
   uint32_t read;
   uint8_t buf[500];
   char msgAscii[] = ":00010203040506070809";
   uint8_t msgBin[100];
   int32_t lenMsgBin;

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);
   memset(buf, 0, sizeof(buf));

   /* obtain msg in binary */
   lenMsgBin = tst_convert2bin(msgBin, (uint8_t *)msgAscii, strlen(msgAscii));

   /* set input buffer */
   ciaaPOSIX_read_add(msgAscii, 1, 1);

   /* open modbus ascii */
   hModbusAscii = ciaaModbus_asciiOpen(1);

   ciaaModbus_asciiTask(hModbusAscii);

   /* receive data */
   ciaaModbus_asciiRecvMsg(hModbusAscii, &buf[0], &buf[1], &read);

   /* check received data */
   TEST_ASSERT_EQUAL_INT8_ARRAY(msgBin, buf, lenMsgBin);
   TEST_ASSERT_EQUAL_INT(lenMsgBin, read+1);
}

/** \brief test ciaaModbus_asciiRecvMsg
 ** incomplete msg */
void test_ciaaModbus_asciiRecvMsg_02(void) {
   uint32_t read;
   uint8_t buf[500];
   char msgAscii[] = ":00010203040506070809";

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);

   /* set input buffer without CRLF */
   ciaaPOSIX_read_add(msgAscii, 0, 1);

   /* open modbus ascii */
   hModbusAscii = ciaaModbus_asciiOpen(1);

   ciaaModbus_asciiTask(hModbusAscii);

   /* receive data */
   ciaaModbus_asciiRecvMsg(hModbusAscii, &buf[0], &buf[1], &read);

   /* check received data */
   TEST_ASSERT_EQUAL_INT(0, read);
}

/** \brief test ciaaModbus_asciiRecvMsg
 ** invalid LRC */
void test_ciaaModbus_asciiRecvMsg_03(void) {
   uint32_t read;
   uint8_t buf[500];
   char msgAscii[] = ":00010203040506070809";

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);

   /* set input buffer without LRC */
   ciaaPOSIX_read_add(msgAscii, 1, 0);

   /* open modbus ascii */
   hModbusAscii = ciaaModbus_asciiOpen(1);

   ciaaModbus_asciiTask(hModbusAscii);

   /* receive data */
   ciaaModbus_asciiRecvMsg(hModbusAscii, &buf[0], &buf[1], &read);

   /* check received data */
   TEST_ASSERT_EQUAL_INT(0, read);
}

/** \brief test ciaaModbus_asciiRecvMsg
 ** receive twice */
void test_ciaaModbus_asciiRecvMsg_04(void) {
   uint32_t read;
   uint8_t buf[500];
   char msgAscii1[] = ":0001020304";
   char msgAscii2[] = ":0506070809";
   uint8_t msgBin[100];
   int32_t lenMsgBin;

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);

   memset(buf, 0, sizeof(buf));

   /* obtain msg in binary */
   lenMsgBin = tst_convert2bin(msgBin, (uint8_t *)msgAscii2, strlen(msgAscii2));

   /* set input buffer */
   ciaaPOSIX_read_add(msgAscii1, 1, 1);
   ciaaPOSIX_read_add(msgAscii2, 1, 1);

   /* open modbus ascii */
   hModbusAscii = ciaaModbus_asciiOpen(1);

   /* perform task */
   ciaaModbus_asciiTask(hModbusAscii);

   /* receive data */
   ciaaModbus_asciiRecvMsg(hModbusAscii, &buf[0], &buf[1], &read);

   /* check received data */
   TEST_ASSERT_EQUAL_INT(lenMsgBin, read+1);
   TEST_ASSERT_EQUAL_INT8_ARRAY(msgBin, buf, lenMsgBin);
}

/** \brief test ciaaModbus_asciiRecvMsg
 ** incorrect format */
void test_ciaaModbus_asciiRecvMsg_05(void) {
   uint32_t read;
   uint8_t buf[500];
   char msgAscii[] = ":0001020304";

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);

   memset(buf, 0, sizeof(buf));

   /* set input buffer */
   ciaaPOSIX_read_add(msgAscii, 1, 1);

   /* wrong data in device buffer */
   read_stub.buf[1] = 'X';

   /* wrong data in device buffer */
   read_stub.buf[2] = 'Y';

   /* open modbus ascii */
   hModbusAscii = ciaaModbus_asciiOpen(1);

   /* perform task */
   ciaaModbus_asciiTask(hModbusAscii);

   /* receive data */
   ciaaModbus_asciiRecvMsg(hModbusAscii, &buf[0], &buf[1], &read);

   /* check received data */
   TEST_ASSERT_EQUAL_INT(0, read);
}

/** \brief test ciaaModbus_asciiRecvMsg
 ** reception too large */
void test_ciaaModbus_asciiRecvMsg_06(void) {
   uint32_t read[3];
   uint8_t buf[600];
   char msgAscii[600];
   int32_t loopi;

   /* set stub callback */
   ciaaPOSIX_read_StubWithCallback(ciaaPOSIX_read_stub);

   /* begin character */
   msgAscii[0] = ':';

   /* fill ascii message */
   memset(&msgAscii[1],'0',550);

   /* end of string */
   msgAscii[551] = 0;

   /* set input buffer */
   ciaaPOSIX_read_add(msgAscii, 1, 1);

   /* first read return 256 bytes */
   read_stub.length[0] = 256;

   /* second read return 256 bytes */
   read_stub.length[1] = 256;

   /* third read return 256 bytes */
   read_stub.length[2] = 10;

   /* open modbus ascii */
   hModbusAscii = ciaaModbus_asciiOpen(1);

   for (loopi = 0 ; loopi < 3 ; loopi++)
   {
      /* perform task */
      ciaaModbus_asciiTask(hModbusAscii);

      /* receive data */
      ciaaModbus_asciiRecvMsg(hModbusAscii, &buf[0], &buf[1], &read[loopi]);
   }

   /* check received data */
   for (loopi = 0 ; loopi < 3 ; loopi++)
   {
      TEST_ASSERT_EQUAL_INT(0, read[loopi]);
   }
}


/** \brief test ciaaModbus_asciiSendMsg */
void test_ciaaModbus_asciiSendMsg_01(void)
{
   uint8_t buf[10][2][500];
   int32_t lenin[10];
   int32_t lenout[10];

   /* set stub callback */
   ciaaPOSIX_write_StubWithCallback(ciaaPOSIX_write_stub);
   ciaaPOSIX_memcpy_StubWithCallback(memcpy_stub);

   strcpy((char*)buf[0][0],
          (const char*)":0001020304050607");
        /*012345678901234567890123456789012345678901234567890123456789*/
        /*          1         2         3         4         5         */
   strcpy((char*)buf[1][0],
          (const char*)":000102030405060712AF3DA1");
        /*012345678901234567890123456789012345678901234567890123456789*/
        /*          1         2         3         4         5         */

   /* set input buffer */
   lenin[0] = tst_asciipdu(buf[0][0], 1, 1);
   lenin[1] = tst_asciipdu(buf[1][0], 1, 1);

   /* copy the buffer */
   strcpy((char*)buf[0][1], (const char*)buf[0][0]);
   strcpy((char*)buf[1][1], (const char*)buf[1][0]);

   /* convert to binary. LRC and CRLF no converted to binary*/
   lenout[0] = tst_convert2bin(buf[0][1], buf[0][1], lenin[0] - 4);
   lenout[1] = tst_convert2bin(buf[1][1], buf[1][1], lenin[1] - 4);

   /* open modbus ascii */
   hModbusAscii = ciaaModbus_asciiOpen(1);

   /* transmit binary */
   ciaaModbus_asciiSendMsg(
         hModbusAscii,
         buf[0][1][0],
         &buf[0][1][1],
         lenout[0]-1);

   ciaaModbus_asciiSendMsg(
            hModbusAscii,
            buf[1][1][0],
            &buf[1][1][1],
            lenout[1]-1);

   TEST_ASSERT_EQUAL_INT(2, write_stub.count);
   TEST_ASSERT_EQUAL_INT(lenin[0], write_stub.len[0]);
   TEST_ASSERT_EQUAL_INT(lenin[1], write_stub.len[1]);
   TEST_ASSERT_EQUAL_UINT8_ARRAY(buf[0][0], write_stub.buf[0], lenin[0]);
   TEST_ASSERT_EQUAL_UINT8_ARRAY(buf[1][0], write_stub.buf[1], lenin[1]);
}

/** \brief test function Open
 **
 ** this function call open more times than allowed
 **
 **/
void test_ciaaModbus_asciiOpen_01(void)
{
   int32_t loopi;
   int32_t hModbusAscii[CIAA_MODBUS_TOTAL_TRANSPORT_ASCII+1];

   for (loopi = 0 ; loopi < (CIAA_MODBUS_TOTAL_TRANSPORT_ASCII+1) ; loopi++)
   {
      /* open modbus ascii fd = 0*/
      hModbusAscii[loopi] = ciaaModbus_asciiOpen(0);
   }

   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_TRANSPORT_ASCII ; loopi++)
   {
      /* verify */
      TEST_ASSERT_NOT_EQUAL(hModbusAscii[loopi],-1);
   }

   TEST_ASSERT_EQUAL(hModbusAscii[loopi],-1);
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

