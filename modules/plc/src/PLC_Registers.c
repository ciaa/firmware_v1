/* Copyright 2012-2015, Eric Nicolás Pernia
 * Copyright 2015, Leandro Kollenberger
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

/** \brief PLC Registers
 **
 ** PLC Registers
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
 * LeKo         Leandro Kollenberger
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150428 v0.0.1 ErPe & LeKo CIAA Firmware porting
 * 20120204 v0.0.1 ErPe initial version
 */

/*==================[inclusions]=============================================*/
#include "PLC_Registers.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
PLC_SymbolicRegister CR;    /* CURRENT RESULT REGISTER */

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/** \brief This function converts PLC_1ByteRegister to PLC_BYTE data type
   A.X.x0 - Access to one bit in byte A
   A.B.b0 - Access to byte A
*/
PLC_BYTE Convert_PLC_1ByteRegister_2_PLC_BYTE(PLC_1ByteRegister PORT)
{
   PLC_BYTE status = 0;

   if (PORT.X.x0)
      {status |= bit_0;}
   if (PORT.X.x1 != 0)
      {status |= bit_1;}
   if (PORT.X.x2)
      {status |= bit_2;}
   if (PORT.X.x3)
      {status |= bit_3;}
   if (PORT.X.x4)
      {status |= bit_4;}
   if (PORT.X.x5)
      {status |= bit_5;}
   if (PORT.X.x6)
      {status |= bit_6;}
   if (PORT.X.x7)
      {status |= bit_7;}

   return status;
}

/** \brief This function converts PLC_BYTE to PLC_1ByteRegister data type
   A.X.x0 - Access to one bit in byte A
   A.B.b0 - Access to byte A
*/
PLC_1ByteRegister Convert_PLC_BYTE_2_PLC_1ByteRegister(PLC_BYTE status)
{
   PLC_1ByteRegister PORT;

   PORT.X.x0 = 0;
   PORT.X.x1 = 0;
   PORT.X.x2 = 0;
   PORT.X.x3 = 0;
   PORT.X.x4 = 0;
   PORT.X.x5 = 0;
   PORT.X.x6 = 0;
   PORT.X.x7 = 0;

   if ((status & bit_0))
      {PORT.X.x0 = 1;}
   if ((status & bit_1))
      {PORT.X.x1 = 1;}
   if ((status & bit_2))
      {PORT.X.x2 = 1;}
   if ((status & bit_3))
      {PORT.X.x3 = 1;}
   if ((status & bit_4))
      {PORT.X.x4 = 1;}
   if ((status & bit_5))
      {PORT.X.x5 = 1;}
   if ((status & bit_6))
      {PORT.X.x6 = 1;}
   if ((status & bit_7))
      {PORT.X.x7 = 1;}

   return PORT;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
