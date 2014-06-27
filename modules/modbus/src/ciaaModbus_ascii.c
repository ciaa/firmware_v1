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
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"

/*==================[macros and definitions]=================================*/
typedef struct {
   uint8_t buf[CIAAMODBUS_ASCII_MAXLENGHT-
      CIAAMODBUS_ASCII_MINLENGHT];
   uint8_t lenght;
} ciaaModbus_ascii_bufType;
/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern int32_t ciaaModbus_ascii_receive(int32_t fildes, uint8_t * buf)
{
   int32_t begin;
   int32_t end;
   int32_t read;
   int32_t loopi;
   int32_t base;
   static ciaaModbus_ascii_bufType oldData = { { 0 }, 0 };

   do {

      /* read until the beginning of a ascii modbus is found */
      do {
         /* set begin to an invalid value */
         begin = -1;

         /* if old data available start reading this data */
         if (0 != oldData.lenght)
         {
            /* copy read data */
            ciaaPOSIX_memcpy(buf, oldData.buf, oldData.lenght);
            read = oldData.lenght;
            oldData.lenght = 0;
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
      for (loopi = begin; loopi < read; loopi ++)
      {
         buf[loopi-begin] = buf[loopi];
      }

      /* update read and begin after updating the buffer */
      read = read - begin;
      begin = 0;

      /* set base to the begin */
      base = begin;

      do {
         /* set end to an invalid value */
         end = -1;

         /* search for the end of a modbus message */
         for (loopi = base; (loopi < base + read -1) && (-1 == end); loopi++)
         {
            /* check if end has been found */
            if ( (CIAAMODBUS_ASCII_END_1 == buf[loopi]) &&
                 (CIAAMODBUS_ASCII_END_2 == buf[loopi+1]) )
            {
               end = loopi + 1;
            }
         }

         /* update base */
         base = read;

         /* if not ascii end found */
         if (-1 == end)
         {
            /* and still place on the buffer */
            if (0 < CIAAMODBUS_ASCII_MAXLENGHT - base -1)
            {
               read = ciaaPOSIX_read(fildes, &buf[base+1], CIAAMODBUS_ASCII_MAXLENGHT - base -1);
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
            if (read > end)
            {
               /* copy left bytes to old data for the next call */
               oldData.lenght = read-end;
               /* copy data to old data buffer for the next calle */
               ciaaPOSIX_memcpy(oldData.buf, &buf[end+1], read-end);
            }
         }
      } while (-1 == end);

   /* repeat until a valid modbus message has been found */
   } while (CIAAMODBUS_ASCII_MAXLENGHT >= end);

   /* return the lenght of the buffer */
   return end;
} /* end ciaaModbus_ascii_receive */

extern int32_t ciaaModbus_ascii_convert2bin(uint8_t * buf)
{
   int32_t ret = 0;
   int32_t loopi;
   int32_t mult;
   int32_t count = 2;

   for(loopi = 0; (loopi < count) && (-1 != ret); loopi++)
   {
      /* calculate multiplicator */
      mult = 1 << (4 * (count - 1 - loopi) );

      /* calculate bin value if value between 0 .. 9 */
      if ( ( CIAAMODBUS_ASCII_0 <= *buf ) && ( CIAAMODBUS_ASCII_9 >= *buf ) )
      {
         ret += ( *buf - CIAAMODBUS_ASCII_0 ) * mult;
      }
      /* calculate bin value if between A .. F */
      else if ( ( CIAAMODBUS_ASCII_A <= *buf ) && ( CIAAMODBUS_ASCII_F >= *buf ) )
      {
         ret += ( *buf - CIAAMODBUS_ASCII_A + 10 ) * mult;
      }
      else
      {
         /* buf is not in the range 0..9 or A..F */
         ret = -1;
      }

      /* increment pointer */
      buf++;
   }

   return ret;
}

extern int32_t ciaaModbus_ascii2bin(uint8_t * buf)
{
   int32_t ret = 0;
   int32_t length = 0;
   uint8_t * dest = buf;

   /* check correct start */
   if (CIAAMODBUS_ASCII_START != buf[0])
   {
      /* invalid header */
      ret = -1;
   }

   (void)dest;
   (void)length;
   return ret;

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

