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

/** \brief This file implements the Modbus main functionality
 **
 ** This file implements the main functionality of the Modbus
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
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150604 v0.8.1 GMuro   replace numbers for macros
 * 20141108 v0.0.2 GMuro   New Modbus implementation
 * 20140623 v0.0.1 MaCe    first functional version
 */

/*==================[inclusions]=============================================*/
#include "ciaaModbus_ascii.h"
#include "ciaaModbus_transport.h"
#include "ciaaModbus_config.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_stdbool.h"

/*==================[macros and definitions]=================================*/

/** \brief Modbus ASCII Object type */
typedef struct
{
   int32_t fildes;                              /** <- File descriptor */
   int32_t bufferSize;                          /** <- buffer size */
   int32_t timeOut;                             /** <- timeout between consecutive characters */
   uint8_t buffer[CIAAMODBUS_ASCII_MAXLENGHT];  /** <- buffer */
   bool inUse;                                  /** <- Object in use */
}ciaaModbus_asciiObjType;

/** \brief LRC correct macro */
#define CIAAMODBUS_ASCII_LRC_OK     1

/** \brief LRC incorrect macro */
#define CIAAMODBUS_ASCII_LRC_NOK    0

/** \brief timeout between characters (milliseconds) */
#define CIAAMODBUS_ASCII_TIMOUT_RCV 1000

/*==================[internal data declaration]==============================*/

/** \brief Array of Modbus ASCII Object */
static ciaaModbus_asciiObjType ciaaModbus_asciiObj[CIAA_MODBUS_TOTAL_TRANSPORT_ASCII];

static const uint8_t ciaaModbus_binToAsciiTable[16] =
   {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F'};


/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static uint8_t ciaaModbus_calcLRC(uint8_t * buf, int32_t len)
{
   int32_t loopi;
   uint8_t lrc = 0;

   /* calculate lrc */
   for(loopi = 0; loopi < len; loopi++)
   {
      lrc += buf[loopi];
   }

   /* complement 2 */
   lrc = -lrc;

   return lrc;
}

/** \brief check lrc of ASCII Modbus Message
 **
 ** \param[in] buf buffer
 ** \param[in] length length of the data stored in the buffer
 **            (include lrc)
 ** \return CIAAMODBUS_ASCII_LRC_OK if correct
 **         CIAAMODBUS_ASCII_LRC_NOK if wrong
 **/
static int32_t ciaaModbus_checkLRC(uint8_t * buf, int32_t len)
{
   int32_t ret = CIAAMODBUS_ASCII_LRC_NOK;
   uint8_t lrc = 0;

   lrc = ciaaModbus_calcLRC(buf, len-1);

   /* check lrc */
   if (buf[len-1] == lrc)
   {
      ret = CIAAMODBUS_ASCII_LRC_OK;
   }

   return ret;
}

/** \brief Check if Complete the reception of a ASCII Modbus Message
 **
 ** Check if the provided buffer has a complete ascii message.
 **
 ** \param[in] buf buffer
 ** \param[in] length length of the data stored in the buffer
 ** \return the length of the modbus ascii message
 **         -1 if no complete message
 **/
static int32_t ciaaModbus_asciiCheckCompleteMsg(uint8_t * buf, int8_t length)
{
   int32_t loopi;
   /* set end to an invalid value */
   int32_t end = -1;

   /* if the minimal bus length has been received */
   if (CIAAMODBUS_ASCII_MINLENGHT <= length)
   {
      /* search for the end of a modbus message */
      for (loopi = CIAAMODBUS_ASCII_MINLENGHT-2;
            (loopi < length -1) &&
            (-1 == end);
            loopi++)
      {
         /* check if end has been found */
         if ( (CIAAMODBUS_ASCII_END_1 == buf[loopi]) &&
              (CIAAMODBUS_ASCII_END_2 == buf[loopi+1]) &&
              (CIAAMODBUS_ASCII_START == buf[0]) )
         {
            /* set end position */
            end = loopi + 2;
         }
      }
   }

   return end;
} /* end ciaaModbus_ascii_completeReception */

/*==================[external functions definition]==========================*/
extern int32_t ciaaModbus_ascii_ascii2bin(uint8_t * buf, int32_t len)
{
   int32_t ret = 0;
   int32_t loopi = 0;
   uint8_t * dest = buf;
   uint8_t aux;

   /* for the complete modbus ascii */
   for(loopi = 1; (loopi < len) && (-1 != ret); loopi+=2)
   {
      /* initialize aux to 0 */
      aux = 0;

      /* calculate bin value between 0 .. 9 */
      if ( (CIAAMODBUS_ASCII_0 <= buf[loopi]) && (CIAAMODBUS_ASCII_9 >= buf[loopi]) )
      {
         aux = (buf[loopi] - CIAAMODBUS_ASCII_0) << 4;
      }
      /* calculate bin value if between A .. F */
      else if ( (CIAAMODBUS_ASCII_A <= buf[loopi]) && (CIAAMODBUS_ASCII_F >= buf[loopi]) )
      {
         aux = (buf[loopi] - CIAAMODBUS_ASCII_A + 10) << 4;
      }
      else
      {
         /* buf is not in the range 0..9 or A..F */
         /* report error, due to the ret++ the value is set to -2 */
         ret = -2;
      }

      /* calculate bin value between 0 .. 9 */
      if ( (CIAAMODBUS_ASCII_0 <= buf[loopi+1]) && (CIAAMODBUS_ASCII_9 >= buf[loopi+1]) )
      {
         aux += (buf[loopi+1] - CIAAMODBUS_ASCII_0);
      }
      /* calculate bin value if between A .. F */
      else if ( (CIAAMODBUS_ASCII_A <= buf[loopi+1]) && (CIAAMODBUS_ASCII_F >= buf[loopi+1]) )
      {
         aux += (buf[loopi+1] - CIAAMODBUS_ASCII_A + 10);
      }
      else
      {
         /* buf is not in the range 0..9 or A..F */
         /* report error, due to the ret++ the value is set to -2 */
         ret = -2;
      }

      /* store binary data */
      *dest = aux;

      /* increment destination buffer */
      dest++;

      /* increment count of converted bytes */
      ret++;
   }

   return ret;
} /* end ciaaModbus_ascii_ascii2bin */


extern void ciaaModbus_asciiInit(void)
{
   int32_t loopi;

   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_TRANSPORT_ASCII ; loopi++)
   {
      ciaaModbus_asciiObj[loopi].inUse = false;
   }
}
extern int32_t ciaaModbus_asciiOpen(int32_t fildes)
{
   int32_t hModbusAscii;

   /* initialize handler with valid value */
   hModbusAscii = 0;

   /* search a modbus ascii Object not in use */
   while ( (hModbusAscii < CIAA_MODBUS_TOTAL_TRANSPORT_ASCII) &&
           (ciaaModbus_asciiObj[hModbusAscii].inUse == true) )
   {
      hModbusAscii++;
   }

   /* if object available, use it */
   if (hModbusAscii < CIAA_MODBUS_TOTAL_TRANSPORT_ASCII)
   {
      /* set object in use */
      ciaaModbus_asciiObj[hModbusAscii].inUse = true;

      /* set low layer file descriptor */
      ciaaModbus_asciiObj[hModbusAscii].fildes = fildes;

      /* empty buffer */
      ciaaModbus_asciiObj[hModbusAscii].bufferSize = 0;
   }
   else
   {
      hModbusAscii = -1;
   }

   return hModbusAscii;
}

extern void ciaaModbus_asciiTask(int32_t handler)
{
   int32_t begin;
   int32_t read;
   int32_t loopi;

   uint8_t *buf;

   if (0 == ciaaModbus_asciiObj[handler].timeOut)
   {
      ciaaModbus_asciiObj[handler].bufferSize = 0;
   }
   else
   {
      ciaaModbus_asciiObj[handler].timeOut --;
   }

   /* set pointer to buffer */
   buf = ciaaModbus_asciiObj[handler].buffer;

   /* set begin to an invalid value */
   begin = -1;

   /* max read */
   read = CIAAMODBUS_ASCII_MAXLENGHT -
         ciaaModbus_asciiObj[handler].bufferSize;

   /* checks if the maximum size reached */
   if (0 == read)
   {
      /* if reached discards all data */
      ciaaModbus_asciiObj[handler].bufferSize = 0;

      /* max read */
      read = CIAAMODBUS_ASCII_MAXLENGHT;
   }

   /* read from device */
   read = ciaaPOSIX_read(
         ciaaModbus_asciiObj[handler].fildes,
         &buf[ciaaModbus_asciiObj[handler].bufferSize],
         read);

   /* if received data process */
   if (read > 0)
   {
      ciaaModbus_asciiObj[handler].timeOut = CIAAMODBUS_ASCII_TIMOUT_RCV / CIAA_MODBUS_TIME_BASE;
      /* increment buffer size */
      ciaaModbus_asciiObj[handler].bufferSize += read;

      /* search for START character */
      for (loopi = ciaaModbus_asciiObj[handler].bufferSize-1 ;
           (0 <= loopi) && (-1 == begin) ;
           loopi--)
      {
         /* check for the begin of a ascii modbus message */
         if (CIAAMODBUS_ASCII_START == buf[loopi])
         {
            begin = loopi;
         }
      }

      /* move the received part to the beginning of the buffer */
      if (begin > 0)
      {
         for (loopi = begin; loopi < ciaaModbus_asciiObj[handler].bufferSize; loopi++)
         {
            buf[loopi-begin] = buf[loopi];
         }

         /* set new buffer size */
         ciaaModbus_asciiObj[handler].bufferSize -= begin;
      }
   }
}

extern void ciaaModbus_asciiRecvMsg(
      int32_t handler,
      uint8_t *id,
      uint8_t *pdu,
      uint32_t *size)
{
   int32_t loopi;
   int32_t len_ascii;
   int32_t len_bin;

   len_ascii = ciaaModbus_asciiCheckCompleteMsg(
         ciaaModbus_asciiObj[handler].buffer,
         ciaaModbus_asciiObj[handler].bufferSize);

   if (CIAAMODBUS_ASCII_MINLENGHT <= len_ascii)
   {
      /* empty buffer for the upcoming receptions */
      ciaaModbus_asciiObj[handler].bufferSize = 0;

      /* convert to bin (not convert CRLF)*/
      len_bin = ciaaModbus_ascii_ascii2bin(
            ciaaModbus_asciiObj[handler].buffer,
            len_ascii-2);

      /* if ascii to binary correct conversion */
      if (0 < len_bin)
      {
         /* check lrc */
         if ( ciaaModbus_checkLRC(
              ciaaModbus_asciiObj[handler].buffer,
              len_bin) == CIAAMODBUS_ASCII_LRC_OK)
         {
            /* discard LRC */
            len_bin--;

            /* copy pdu */
            for (loopi = 0 ; loopi < len_bin ; loopi++)
            {
               pdu[loopi] = ciaaModbus_asciiObj[handler].buffer[loopi+1];
            }

            /* copy id */
            *id = ciaaModbus_asciiObj[handler].buffer[0];

            /* copy size */
            *size = len_bin - 1;
         }
         else
         {
            *size = 0;
         }
      }
      else
      {
         *size = 0;
      }
   }
   else
   {
      *size = 0;
   }
}

void ciaaModbus_asciiSendMsg(
      int32_t handler,
      uint8_t id,
      uint8_t *pdu,
      uint32_t size)
{
   int32_t loopi, lenAscii;
   uint32_t upper, lower;
   uint8_t *buf;

   /* set pointer to buffer */
   buf = ciaaModbus_asciiObj[handler].buffer;

   /* copy id */
   buf[0] = id;

   /* copy pdu */
   for (loopi = 0 ; loopi < size ; loopi++)
   {
      buf[loopi+1] = pdu[loopi];
   }

   /* increment size to include id */
   size++;

   /* Add LRC at end and increment size */
   buf[size] = ciaaModbus_calcLRC(buf, size);
   size++;

   /* Verify correct len */
   if (CIAAMODBUS_ASCII_MAXLENGHT >= (size * 2 + 3))
   {
      /* Convert to ASCII */
      for (loopi = size ; loopi > 0 ; loopi--)
      {
         upper = (buf[loopi-1] >> 4) & 0x0F;
         lower = (buf[loopi-1] >> 0) & 0x0F;

         buf[loopi * 2 - 1] = ciaaModbus_binToAsciiTable[upper];
         buf[loopi * 2 - 0] = ciaaModbus_binToAsciiTable[lower];
      }

      /* Add start character and update lenAscii */
      buf[0] = CIAAMODBUS_ASCII_START;
      lenAscii = size * 2 + 1;

      /* Add CRLF at end and increment lenAscii */
      buf[lenAscii] = CIAAMODBUS_ASCII_END_1;
      lenAscii++;
      buf[lenAscii] = CIAAMODBUS_ASCII_END_2;
      lenAscii++;

      ciaaPOSIX_write(
            ciaaModbus_asciiObj[handler].fildes,
            buf,
            lenAscii);
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

