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
 * MaCe         Mariano Cerdeiro
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140623 v0.0.1 initials initial
 */

/*==================[inclusions]=============================================*/
#include "ciaaModbus_ascii.h"
#include "ciaaModbus_transport.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"

/*==================[macros and definitions]=================================*/
typedef struct {
   uint8_t buf[CIAAMODBUS_ASCII_MAXLENGHT-
      CIAAMODBUS_ASCII_MINLENGHT];
   uint8_t length;
} ciaaModbus_ascii_bufType;
/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
/** \brief Receive the beggining of a ASCII Modbus Message
 **
 ** This function reads until the beggining of a modbus ascii
 ** is found.
 **
 ** \param[in] fildes file descriptor to read the data from
 ** \param[out] buf buffer to store the data, shall be at least
 **             CIAAMODBUS_ASCII_MAXLENGTH bytes long
 ** \return the length of the modbus ascii message
 **/
static int32_t ciaaModbus_ascii_receiveFirst(int32_t fildes, uint8_t * buf);

/** \brief Complete the reception of a ASCII Modbus Message
 **
 ** Check if the provided buffer has a complete ascii message. If not read the
 ** device to complete the ascii modbus message.
 **
 ** \param[in] fildes file descriptor to read the data from
 ** \param[out] buf buffer to store the data, shall be at least
 **             CIAAMODBUS_ASCII_MAXLENGTH bytes long
 ** \param[in] length length of the data already stored in the buffer
 ** \return the length of the modbus ascii message
 **/
static int32_t ciaaModbus_ascii_completeReception(int32_t fildes,uint8_t * buf,
      int8_t length);

/*==================[internal data definition]===============================*/
/** \brief data not proceeded from the last frame */
ciaaModbus_ascii_bufType oldData = { { 0 }, 0 };

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

static int32_t ciaaModbus_checkLRC(uint8_t * buf, int32_t len)
{
   int32_t ret = -1;
   uint8_t lrc = 0;

   lrc = ciaaModbus_calcLRC(buf, len-1);

   /* check lrc */
   if (buf[len-1] == lrc)
   {
      ret = 1;
   }

   return ret;
}

static int32_t ciaaModbus_ascii_receiveFirst(int32_t fildes, uint8_t * buf)
{
   int32_t begin;
   int32_t read;
   int32_t loopi;

   /* read until the beginning of a ascii modbus is found */
   do
   {
      /* set begin to an invalid value */
      begin = -1;

      /* if old data available start reading this data */
      if (0 != oldData.length)
      {
         /* copy read data */
         ciaaPOSIX_memcpy(buf, oldData.buf, oldData.length);
         read = oldData.length;
         oldData.length = 0;
      }
      else
      {
         /* no data, also read from device */
         read = ciaaPOSIX_read(fildes, buf, CIAAMODBUS_ASCII_MAXLENGHT);
      }

      /* search for the begin of a modbus message */
      for(loopi = 0; (loopi < read) && (-1 == begin); loopi++)
      {
         /* check for the begin of a ascii modbus message */
         if (CIAAMODBUS_ASCII_START == buf[loopi])
         {
            begin = loopi;
         }
      }
   } while (-1 == begin);

   /* move the received part to the beginning of the buffer */
   if (begin != 0)
   {
      for (loopi = begin; loopi < read; loopi ++)
      {
         buf[loopi-begin] = buf[loopi];
      }
   }

   return read;
} /* end ciaaModbus_ascii_receiveFirst */

static int32_t ciaaModbus_ascii_completeReception(int32_t fildes, uint8_t * buf, int8_t length)
{
   int32_t end;
   int32_t loopi;

   do
   {
      /* set end to an invalid value */
      end = -1;

      /* if the minimal bus length has been received */
      if (CIAAMODBUS_ASCII_MINLENGHT < length)
      {
         /* search for the end of a modbus message */
         for (loopi = CIAAMODBUS_ASCII_MINLENGHT-2;
               (loopi < length -1) &&
               (-1 == end);
               loopi++)
         {
            /* check if end has been found */
            if ( (CIAAMODBUS_ASCII_END_1 == buf[loopi]) &&
               (CIAAMODBUS_ASCII_END_2 == buf[loopi+1]) )
            {
               /* set end position */
               end = loopi + 2;
            }
         }
      }

      /* if not ascii end found */
      if (-1 == end)
      {
         /* and still place on the buffer */
         if (CIAAMODBUS_ASCII_MAXLENGHT > length)
         {
            length += ciaaPOSIX_read(fildes, &buf[length], CIAAMODBUS_ASCII_MAXLENGHT - length);
         }
         else
         {
            /* the buffer is full, an ASCII message can not be so big,
            * an invalid message has been received, the complete data
            * will be ignored */
            end = 0xFFFF;
         }
      }
      else
      {
         /* end found */
         if (length > end)
         {
            /* copy left bytes to old data for the next call */
            oldData.length = length-end;
            /* copy data to old data buffer for the next calle */
            ciaaPOSIX_memcpy(oldData.buf, &buf[end], length-end);
         }
      }
   } while (-1 == end);

   return end;
} /* end ciaaModbus_ascii_completeReception */

/*==================[external functions definition]==========================*/
extern int32_t ciaaModbus_ascii_receive(int32_t fildes, uint8_t * buf)
{
   int32_t read;

   do
   {
      /* start reading the modbus */
      /* this functions reads the device until a start of modbus ascii is
       * detected */
      read = ciaaModbus_ascii_receiveFirst(fildes, buf);

      /* this function check if the message is complete, if not continues reading
       * the device until a complete modbus ascii message is available */
      read = ciaaModbus_ascii_completeReception(fildes,
         buf,
         read);

      /* complete reception returns 0xFFFF if an error occurs */
   } while(0xFFFF == read);

   /* return the length of the read buffer */
   return read;

} /* end ciaaModbus_ascii_receive */

extern int32_t ciaaModbus_ascii_read(int32_t fildes, uint8_t * buf)
{
   int32_t len_ascii;
   int32_t len_bin;
   int32_t lrccheck;

   do
   {
      /* read modbus ascii */
      len_ascii = ciaaModbus_ascii_receive(fildes, buf);

      /* conver to bin */
      len_bin = ciaaModbus_ascii_ascii2bin(buf, len_ascii);

	  /* check lrc */
      if (CIAAMODBUS_MSG_MINLENGTH <= len_bin)
         lrccheck = ciaaModbus_checkLRC(buf, len_bin);

      /* repeat while
       * invalid lrc
       * len_bin > CIAAMODBUS_MSG_MINLENGTH */
   } while (CIAAMODBUS_MSG_MINLENGTH > len_bin || 0 > lrccheck);

   return len_bin;

} /* end ciaaModbus_ascii_read */

extern void ciaaModbus_ascii_write(int32_t fildes, uint8_t * buf, int32_t len)
{
}

extern int32_t ciaaModbus_ascii_ascii2bin(uint8_t * buf, int32_t len)
{
   int32_t ret = 0;
   int32_t loopi = 0;
   uint8_t * dest = buf;
   uint8_t aux;

   /* for the complete modbus ascii */
   for(loopi = 1; (loopi < len - 2) && (-1 != ret); loopi+=2)
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

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

