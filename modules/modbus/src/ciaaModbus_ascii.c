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

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
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

