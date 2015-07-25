/* Copyright 2012-2015, Eric Nicolás Pernia
 * All rights reserved.
 *
 * This file is part of IDE4PLC and CIAA Firmware.
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

/** \brief PLC Libraries
 **
 ** PLC Libraries
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup PLC PLC Module
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * ErPe         Eric Pernia
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140911 v0.0.1 ErPe initial version
 */

/*==================[inclusions]=============================================*/
#include "PLC_Lib.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/** \brief PLC Day of week algorithm. */
PLC_INT DT_Weekday(PLC_INT Year, PLC_INT Month, PLC_INT Day)
{
   int a, y, m, NumZeller, NumDT;
   a = (14 - Month) / 12;
   y = Year - a;
   m = Month + 12 * a - 2;

   /* For Julius calendar:
      NumZeller = (5 + Day + y + y/4 + (31*m)/12) % 7 */

   /* For Gregorian calendar: */
   NumZeller = (Day + y + y/4 - y/100 + y/400 + (31*m)/12) % 7;

   /* The NumZeller result is:
         0 = Sunday
         1 = Monday
         2 = Tuesday
         3 = Wednesday
         4 = Thursday
         5 = Friday
         6 = Saturday */

   NumDT = NumZeller + 1;

   /** \brief The NumDT result is:
    1 = Sunday
    2 = Monday
    3 = Tuesday
    4 = Wednesday
    5 = Thursday
    6 = Friday
    7 = Saturday
   */

   return NumDT;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
