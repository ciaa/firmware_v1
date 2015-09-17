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

/** \brief PLC IL Instructions
 **
 ** PLC IL Instructions
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
 * 20120204 v0.0.1 ErPe initial version
 */

/*==================[inclusions]=============================================*/
#include "PLC_IL_Instructions.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void PLC_IL_LD_BOOL_Literal(PLC_BOOL newValue, PLC_EnumModifiers modifier)
{
   extern PLC_SymbolicRegister CR;

   if (modifier == N)
   {
      CR.VALUE.BOOL = !(newValue);
   }
   else
   {
      CR.VALUE.BOOL = newValue;
   }
}

void PLC_IL_LD_INT_Literal(PLC_INT newValue, PLC_EnumModifiers modifier)
{
   extern PLC_SymbolicRegister CR;

   if (modifier == N)
   {
      CR.VALUE.INT = ~(newValue);
   }
   else
   {
      CR.VALUE.INT = newValue;
   }
}

void PLC_IL_LD_TIME_Literal(PLC_TIME newValue, PLC_EnumModifiers modifier)
{
   extern PLC_SymbolicRegister CR;

   if (modifier == N)
   {
      CR.VALUE.TIME = ~(newValue);
   }
   else
   {
      CR.VALUE.TIME = newValue;
   }
}


void PLC_IL_LD(void* newValue, PLC_WORD lenght, PLC_EnumDataTypes type, PLC_EnumModifiers modifier)
{
   extern PLC_SymbolicRegister CR;

   CR.TYPE = type;

   switch (type)
   {
      case BOOL:
      {
         if (modifier == N)
         {
            CR.VALUE.BOOL = !(*(PLC_BOOL*) newValue);
         }
         else
         {
            CR.VALUE.BOOL = *(PLC_BOOL*) newValue;
         }
      }
      break;
      case BYTE:
      {
         if (modifier == N)
         {
            CR.VALUE.BYTE = ~(*(PLC_BYTE*) newValue);
         }
         else
         {
            CR.VALUE.BYTE = *(PLC_BYTE*) newValue;
         }
      }
      break;
      case WORD:
      {
         if (modifier == N)
         {
            CR.VALUE.WORD = ~(*(PLC_WORD*) newValue);
         }
         else
         {
            CR.VALUE.WORD = *(PLC_WORD*) newValue;
         }
      }
      break;
      case DWORD:
      {
         if (modifier == N)
         {
            CR.VALUE.DWORD = ~(*(PLC_DWORD*) newValue);
         }
         else
         {
            CR.VALUE.DWORD = *(PLC_DWORD*) newValue;
         }
      }
      break;
      case LWORD:
      {
         if (modifier == N)
         {
            CR.VALUE.LWORD = ~(*(PLC_LWORD*) newValue);
         }
         else
         {
            CR.VALUE.LWORD = *(PLC_LWORD*) newValue;
         }
      }
      break;
      case SINT:
      {
         if (modifier == N)
         {
            CR.VALUE.SINT = ~(*(PLC_SINT*) newValue);
         }
         else
         {
            CR.VALUE.SINT = *(PLC_SINT*) newValue;
         }
      }
      break;
      case INT:
      {
         if (modifier == N)
         {
            CR.VALUE.INT = ~(*(PLC_INT*) newValue);
         }
         else
         {
            CR.VALUE.INT = *(PLC_INT*) newValue;
         }
      }
      break;
      case DINT:
      {
         if (modifier == N)
         {
            CR.VALUE.DINT = ~(*(PLC_DINT*) newValue);
         }
         else
         {
            CR.VALUE.DINT = *(PLC_DINT*) newValue;
         }
      }
      break;
      case LINT:
      {
         if (modifier == N)
         {
            CR.VALUE.LWORD = ~(*(PLC_LINT*) newValue);
         }
         else
         {
            CR.VALUE.LWORD = *(PLC_LINT*) newValue;
         }
      }
      break;
      case USINT:
      {
         if (modifier == N)
         {
            CR.VALUE.USINT = ~(*(PLC_USINT*) newValue);
         }
         else
         {
            CR.VALUE.USINT = *(PLC_USINT*) newValue;
         }
      }
      break;
      case UINT:
      {
         if (modifier == N)
         {
            CR.VALUE.UINT = ~(*(PLC_UINT*) newValue);
         }
         else
         {
            CR.VALUE.UINT = *(PLC_UINT*) newValue;
         }
      }
      break;
      case UDINT:
      {
         if (modifier == N)
         {
            CR.VALUE.UDINT = ~(*(PLC_UDINT*) newValue);
         }
         else
         {
            CR.VALUE.UDINT = *(PLC_UDINT*) newValue;
         }
      }
      break;
      case ULINT:
      {
         if (modifier == N)
         {
            CR.VALUE.ULINT = ~(*(PLC_ULINT*) newValue);
         }
         else
         {
            CR.VALUE.ULINT = *(PLC_ULINT*) newValue;
         }
      }
      break;
      case REAL:
      {
         CR.VALUE.REAL = *(PLC_REAL*) newValue;
      }
      break;
      case LREAL:
      {
         CR.VALUE.LREAL = *(PLC_LREAL*) newValue;
      }
      break;
      case TIME:
      {
         if (modifier == N)
         {
            CR.VALUE.TIME = ~(*(PLC_TIME*) newValue);
         }
         else
         {
            CR.VALUE.TIME = *(PLC_TIME*) newValue;
         }
      }
      break;
      case D:
      {
         CR.VALUE.D = *(PLC_DATE*) newValue;
      }
      break;
      case TOD:
      {
         CR.VALUE.TOD = *(PLC_TIME_OF_DAY*) newValue;
      }
      break;
      case DT:
      {
         CR.VALUE.DT = *(PLC_DATE_AND_TIME*) newValue;
      }
      break;
      /*case STRING:
      {
         if (modifier == N)
         {
            CR.VALUE.STRING = ~(*(PLC_STRING*) newValue);
         }
         else
         {
            CR.VALUE.STRING = *(PLC_STRING*) newValue;
         }
      }
      break;
      case WSTRING:
      {
         if (modifier == N)
         {
            CR.VALUE.WSTRING = ~(*(PLC_WSTRING*) newValue);
         }
         else
         {
            CR.VALUE.WSTRING = *(PLC_WSTRING*) newValue;
         }
      }
      break;*/
      case BCD16:
      {
         if (modifier == N)
         {
            CR.VALUE.BCD16 = ~(*(PLC_BCD16*) newValue);
         }
         else
         {
            CR.VALUE.BCD16 = *(PLC_BCD16*) newValue;
         }
      }
      break;
      case BCD32:
      {
         if (modifier == N)
         {
            CR.VALUE.BCD32 = ~(*(PLC_BCD32*) newValue);
         }
         else
         {
            CR.VALUE.BCD32 = *(PLC_BCD32*) newValue;
         }
      }
      break;
      case OTHER:
      {
         /* MEMORY COPY */
         PLC_BYTE *source;
         PLC_BYTE *destination;
         PLC_WORD i = 0;

         source = newValue;
         destination = &(CR.VALUE);

          for( i = 0; i < lenght; i++ )
          {
            if (modifier == N)
            {
               *destination = ~(*source);
            }
            else
            {
               *destination = *source;
            }
             destination++;
             source++;
          }
      }
   }
}

/*-----------------------------------------------------------*/

void PLC_IL_ST(void* newValue, PLC_WORD lenght, PLC_EnumModifiers modifier)
{
   extern PLC_SymbolicRegister CR;

   /* MEMORY COPY */
   PLC_BYTE *source;
   PLC_BYTE *destination;
   PLC_WORD i = 0;

   destination = newValue;
   source = &(CR.VALUE);

   for( i = 0; i < lenght; i++ )
   {
      if (modifier == N)
      {
         *destination = ~(*source);
      }
      else
      {
         *destination = *source;
      }
      destination++;
      source++;
   }
}

/*-----------------------------------------------------------*/
void PLC_IL_S(PLC_BOOL* operand)
{
   extern PLC_SymbolicRegister CR;

   if((CR.TYPE == BOOL) && (CR.VALUE.BOOL))
   {
      *operand = true;
   }
}

/*-----------------------------------------------------------*/
void PLC_IL_R(PLC_BOOL* operand)
{
   extern PLC_SymbolicRegister CR;

   if((CR.TYPE == BOOL) && (CR.VALUE.BOOL))
   {
      *operand = false;
   }
}

/*-----------------------------------------------------------*/
/* Equal = */

void PLC_IL_EQ(void* operand, PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   CR.TYPE = BOOL;

   switch (type)
   {
      case BOOL:
      {
          CR.VALUE.BOOL = ( (CR.VALUE.BOOL) == (*(PLC_BOOL*) operand) );
      }
      break;
      case BYTE:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BYTE) == (*(PLC_BYTE*) operand) );
      }
      break;
      case WORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.WORD) == (*(PLC_WORD*) operand) );
      }
      break;
      case DWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DWORD) == (*(PLC_DWORD*) operand) );
      }
      break;
      case LWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LWORD) == (*(PLC_LWORD*) operand) );
      }
      break;
      case SINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.SINT) == (*(PLC_SINT*) operand) );
      }
      break;
      case INT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.INT) == (*(PLC_INT*) operand) );
      }
      break;
      case DINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DINT) == (*(PLC_DINT*) operand) );
      }
      break;
      case LINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LINT) == (*(PLC_LINT*) operand) );
      }
      break;
      case USINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.USINT) == (*(PLC_USINT*) operand) );
      }
      break;
      case UINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UINT) == (*(PLC_UINT*) operand) );
      }
      break;
      case UDINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UDINT) == (*(PLC_UDINT*) operand) );
      }
      break;
      case ULINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.ULINT) == (*(PLC_ULINT*) operand) );
      }
      break;
      case REAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.REAL) == (*(PLC_REAL*) operand) );
      }
      break;
      case LREAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LREAL) == (*(PLC_LREAL*) operand) );
      }
      break;
      case TIME:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.TIME) == (*(PLC_TIME*) operand) );
      }
      break;
      case D:
      {
         CR.VALUE.BOOL = (
            ((CR.VALUE.D.YEAR) == (*(PLC_DATE*) operand).YEAR) &&
            ((CR.VALUE.D.MONTH) == (*(PLC_DATE*) operand).MONTH) &&
            ((CR.VALUE.D.DAY) == (*(PLC_DATE*) operand).DAY)
            );
      }
      break;
      case TOD:
      {
         CR.VALUE.BOOL = (
            ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR) &&
            ((CR.VALUE.TOD.MINUTE) == (*(PLC_TIME_OF_DAY*) operand).MINUTE) &&
            ((CR.VALUE.TOD.SECOND) == (*(PLC_TIME_OF_DAY*) operand).SECOND) &&
            ((CR.VALUE.TOD.NANOSECOND) == (*(PLC_TIME_OF_DAY*) operand).NANOSECOND)
            );
      }
      break;
      case DT:
      {
         CR.VALUE.BOOL = (
            ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) &&
            ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) &&
            ((CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) &&
            ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR) &&
            ((CR.VALUE.DT.MINUTE) == (*(PLC_DATE_AND_TIME*) operand).MINUTE) &&
            ((CR.VALUE.DT.SECOND) == (*(PLC_DATE_AND_TIME*) operand).SECOND) &&
            ((CR.VALUE.DT.NANOSECOND) == (*(PLC_DATE_AND_TIME*) operand).NANOSECOND)
            );
      }
      break;
      case STRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) == (*(PLC_Bool*) operand) );
            CR.VALUE.STRING = *(PLC_String*) newValue; */
      }
      break;
      case WSTRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) == (*(PLC_Bool*) operand) );
            CR.VALUE.WSTRING = *(PLC_WString*) newValue; */
      }
      break;
      case BCD16:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD16) == (*(PLC_BCD16*) operand) );
      }
      break;
      case BCD32:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD32) == (*(PLC_BCD32*) operand) );
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* Non Equal <> */

void PLC_IL_NE(void* operand, PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   CR.TYPE = BOOL;

   switch (type)
   {
      case BOOL:
      {
          CR.VALUE.BOOL = ( (CR.VALUE.BOOL) != (*(PLC_BOOL*) operand) );
      }
      break;
      case BYTE:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BYTE) != (*(PLC_BYTE*) operand) );
      }
      break;
      case WORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.WORD) != (*(PLC_WORD*) operand) );
      }
      break;
      case DWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DWORD) != (*(PLC_DWORD*) operand) );
      }
      break;
      case LWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LWORD) != (*(PLC_LWORD*) operand) );
      }
      break;
      case SINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.SINT) != (*(PLC_SINT*) operand) );
      }
      break;
      case INT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.INT) != (*(PLC_INT*) operand) );
      }
      break;
      case DINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DINT) != (*(PLC_DINT*) operand) );
      }
      break;
      case LINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LINT) != (*(PLC_LINT*) operand) );
      }
      break;
      case USINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.USINT) != (*(PLC_USINT*) operand) );
      }
      break;
      case UINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UINT) != (*(PLC_UINT*) operand) );
      }
      break;
      case UDINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UDINT) != (*(PLC_UDINT*) operand) );
      }
      break;
      case ULINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.ULINT) != (*(PLC_ULINT*) operand) );
      }
      break;
      case REAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.REAL) != (*(PLC_REAL*) operand) );
      }
      break;
      case LREAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LREAL) != (*(PLC_LREAL*) operand) );
      }
      break;
      case TIME:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.TIME) != (*(PLC_TIME*) operand) );
      }
      break;
      case D:
      {
         CR.VALUE.BOOL = (
            ((CR.VALUE.D.YEAR) != (*(PLC_DATE*) operand).YEAR) ||
            ((CR.VALUE.D.MONTH) != (*(PLC_DATE*) operand).MONTH) ||
            ((CR.VALUE.D.DAY) != (*(PLC_DATE*) operand).DAY)
            );
      }
      break;
      case TOD:
      {
         CR.VALUE.BOOL = (
            ((CR.VALUE.TOD.HOUR) != (*(PLC_TIME_OF_DAY*) operand).HOUR) ||
            ((CR.VALUE.TOD.MINUTE) != (*(PLC_TIME_OF_DAY*) operand).MINUTE) ||
            ((CR.VALUE.TOD.SECOND) != (*(PLC_TIME_OF_DAY*) operand).SECOND) ||
            ((CR.VALUE.TOD.NANOSECOND) != (*(PLC_TIME_OF_DAY*) operand).NANOSECOND)
            );
      }
      break;
      case DT:
      {
         CR.VALUE.BOOL = (
            ((CR.VALUE.DT.YEAR) != (*(PLC_DATE_AND_TIME*) operand).YEAR) ||
            ((CR.VALUE.DT.MONTH) != (*(PLC_DATE_AND_TIME*) operand).MONTH) ||
            ((CR.VALUE.DT.DAY) != (*(PLC_DATE_AND_TIME*) operand).DAY) ||
            ((CR.VALUE.DT.HOUR) != (*(PLC_DATE_AND_TIME*) operand).HOUR) ||
            ((CR.VALUE.DT.MINUTE) != (*(PLC_DATE_AND_TIME*) operand).MINUTE) ||
            ((CR.VALUE.DT.SECOND) != (*(PLC_DATE_AND_TIME*) operand).SECOND) ||
            ((CR.VALUE.DT.NANOSECOND) != (*(PLC_DATE_AND_TIME*) operand).NANOSECOND)
            );
      }
      break;
      case STRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) != (*(PLC_Bool*) operand) );
            CR.VALUE.STRING = *(PLC_String*) newValue; */
      }
      break;
      case WSTRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) != (*(PLC_Bool*) operand) );
            CR.VALUE.WSTRING = *(PLC_WString*) newValue; */
      }
      break;
      case BCD16:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD16) != (*(PLC_BCD16*) operand) );
      }
      break;
      case BCD32:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD32) != (*(PLC_BCD32*) operand) );
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* Greater Than > */

void PLC_IL_GT(void* operand, PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   CR.TYPE = BOOL;

   switch (type)
   {
      case BOOL:
      {
          CR.VALUE.BOOL = ( (CR.VALUE.BOOL) > (*(PLC_BOOL*) operand) );
      }
      break;
      case BYTE:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BYTE) > (*(PLC_BYTE*) operand) );
      }
      break;
      case WORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.WORD) > (*(PLC_WORD*) operand) );
      }
      break;
      case DWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DWORD) > (*(PLC_DWORD*) operand) );
      }
      break;
      case LWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LWORD) > (*(PLC_LWORD*) operand) );
      }
      break;
      case SINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.SINT) > (*(PLC_SINT*) operand) );
      }
      break;
      case INT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.INT) > (*(PLC_INT*) operand) );
      }
      break;
      case DINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DINT) > (*(PLC_DINT*) operand) );
      }
      break;
      case LINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LINT) > (*(PLC_LINT*) operand) );
      }
      break;
      case USINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.USINT) > (*(PLC_USINT*) operand) );
      }
      break;
      case UINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UINT) > (*(PLC_UINT*) operand) );
      }
      break;
      case UDINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UDINT) > (*(PLC_UDINT*) operand) );
      }
      break;
      case ULINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.ULINT) > (*(PLC_ULINT*) operand) );
      }
      break;
      case REAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.REAL) > (*(PLC_REAL*) operand) );
      }
      break;
      case LREAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LREAL) > (*(PLC_LREAL*) operand) );
      }
      break;
      case TIME:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.TIME) > (*(PLC_TIME*) operand) );
      }
      break;
      case D:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.D.YEAR) > (*(PLC_DATE*) operand).YEAR ) ||
            ( ((CR.VALUE.D.YEAR) == (*(PLC_DATE*) operand).YEAR ) && ((CR.VALUE.D.MONTH) > (*(PLC_DATE*) operand).MONTH) ) ||
            ( ((CR.VALUE.D.YEAR) == (*(PLC_DATE*) operand).YEAR) && ((CR.VALUE.D.MONTH) == (*(PLC_DATE*) operand).MONTH) && ( (CR.VALUE.D.DAY) > (*(PLC_DATE*) operand).DAY) );
      }
      break;
      case TOD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.TOD.HOUR) > (*(PLC_TIME_OF_DAY*) operand).HOUR ) ||
            ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR ) && ((CR.VALUE.TOD.MINUTE) > (*(PLC_TIME_OF_DAY*) operand).MINUTE) ) ||
            ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR) && ((CR.VALUE.TOD.MINUTE) == (*(PLC_TIME_OF_DAY*) operand).MINUTE) && ( (CR.VALUE.TOD.SECOND) > (*(PLC_TIME_OF_DAY*) operand).SECOND) ) ||
         ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR) && ((CR.VALUE.TOD.MINUTE) == (*(PLC_TIME_OF_DAY*) operand).MINUTE) && ( (CR.VALUE.TOD.SECOND) == (*(PLC_TIME_OF_DAY*) operand).SECOND)  && ( (CR.VALUE.TOD.NANOSECOND) > (*(PLC_TIME_OF_DAY*) operand).NANOSECOND) );
      }
      break;
      case DT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DT.YEAR) > (*(PLC_DATE_AND_TIME*) operand).YEAR ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR ) && ((CR.VALUE.DT.MONTH) > (*(PLC_DATE_AND_TIME*) operand).MONTH) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) > (*(PLC_DATE_AND_TIME*) operand).DAY) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && (CR.VALUE.DT.HOUR) > (*(PLC_DATE_AND_TIME*) operand).HOUR ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR ) && ((CR.VALUE.DT.MINUTE) > (*(PLC_DATE_AND_TIME*) operand).MINUTE) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR) && ((CR.VALUE.DT.MINUTE) == (*(PLC_DATE_AND_TIME*) operand).MINUTE) && ( (CR.VALUE.DT.SECOND) > (*(PLC_DATE_AND_TIME*) operand).SECOND) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR) && ((CR.VALUE.DT.MINUTE) == (*(PLC_DATE_AND_TIME*) operand).MINUTE) && ( (CR.VALUE.DT.SECOND) == (*(PLC_DATE_AND_TIME*) operand).SECOND)  && ( (CR.VALUE.DT.NANOSECOND) > (*(PLC_DATE_AND_TIME*) operand).NANOSECOND) );
      }
      break;
      case STRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) != (*(PLC_Bool*) operand) );
            CR.VALUE.STRING = *(PLC_String*) newValue; */
      }
      break;
      case WSTRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) != (*(PLC_Bool*) operand) );
            CR.VALUE.WSTRING = *(PLC_WString*) newValue; */
      }
      break;
      case BCD16:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD16) > (*(PLC_BCD16*) operand) );
      }
      break;
      case BCD32:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD32) > (*(PLC_BCD32*) operand) );
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* Greater than or Equal >= */

void PLC_IL_GE(void* operand, PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   CR.TYPE = BOOL;

   switch (type)
   {
      case BOOL:
      {
          CR.VALUE.BOOL = ( (CR.VALUE.BOOL) >= (*(PLC_BOOL*) operand) );
      }
      break;
      case BYTE:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BYTE) >= (*(PLC_BYTE*) operand) );
      }
      break;
      case WORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.WORD) >= (*(PLC_WORD*) operand) );
      }
      break;
      case DWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DWORD) >= (*(PLC_DWORD*) operand) );
      }
      break;
      case LWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LWORD) >= (*(PLC_LWORD*) operand) );
      }
      break;
      case SINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.SINT) >= (*(PLC_SINT*) operand) );
      }
      break;
      case INT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.INT) >= (*(PLC_INT*) operand) );
      }
      break;
      case DINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DINT) >= (*(PLC_DINT*) operand) );
      }
      break;
      case LINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LINT) >= (*(PLC_LINT*) operand) );
      }
      break;
      case USINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.USINT) >= (*(PLC_USINT*) operand) );
      }
      break;
      case UINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UINT) >= (*(PLC_UINT*) operand) );
      }
      break;
      case UDINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UDINT) >= (*(PLC_UDINT*) operand) );
      }
      break;
      case ULINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.ULINT) >= (*(PLC_ULINT*) operand) );
      }
      break;
      case REAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.REAL) >= (*(PLC_REAL*) operand) );
      }
      break;
      case LREAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LREAL) >= (*(PLC_LREAL*) operand) );
      }
      break;
      case TIME:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.TIME) >= (*(PLC_TIME*) operand) );
      }
      break;
      case D:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.D.YEAR) >= (*(PLC_DATE*) operand).YEAR ) ||
            ( ((CR.VALUE.D.YEAR) == (*(PLC_DATE*) operand).YEAR ) && ((CR.VALUE.D.MONTH) >= (*(PLC_DATE*) operand).MONTH) ) ||
            ( ((CR.VALUE.D.YEAR) == (*(PLC_DATE*) operand).YEAR) && ((CR.VALUE.D.MONTH) == (*(PLC_DATE*) operand).MONTH) && ( (CR.VALUE.D.DAY) >= (*(PLC_DATE*) operand).DAY) );
      }
      break;
      case TOD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.TOD.HOUR) >= (*(PLC_TIME_OF_DAY*) operand).HOUR ) ||
            ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR ) && ((CR.VALUE.TOD.MINUTE) >= (*(PLC_TIME_OF_DAY*) operand).MINUTE) ) ||
            ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR) && ((CR.VALUE.TOD.MINUTE) == (*(PLC_TIME_OF_DAY*) operand).MINUTE) && ( (CR.VALUE.TOD.SECOND) >= (*(PLC_TIME_OF_DAY*) operand).SECOND) ) ||
         ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR) && ((CR.VALUE.TOD.MINUTE) == (*(PLC_TIME_OF_DAY*) operand).MINUTE) && ( (CR.VALUE.TOD.SECOND) == (*(PLC_TIME_OF_DAY*) operand).SECOND)  && ( (CR.VALUE.TOD.NANOSECOND) >= (*(PLC_TIME_OF_DAY*) operand).NANOSECOND) );
      }
      break;
      case DT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DT.YEAR) >= (*(PLC_DATE_AND_TIME*) operand).YEAR ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR ) && ((CR.VALUE.DT.MONTH) >= (*(PLC_DATE_AND_TIME*) operand).MONTH) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) >= (*(PLC_DATE_AND_TIME*) operand).DAY) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && (CR.VALUE.DT.HOUR) >= (*(PLC_DATE_AND_TIME*) operand).HOUR ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR ) && ((CR.VALUE.DT.MINUTE) >= (*(PLC_DATE_AND_TIME*) operand).MINUTE) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR) && ((CR.VALUE.DT.MINUTE) == (*(PLC_DATE_AND_TIME*) operand).MINUTE) && ( (CR.VALUE.DT.SECOND) >= (*(PLC_DATE_AND_TIME*) operand).SECOND) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR) && ((CR.VALUE.DT.MINUTE) == (*(PLC_DATE_AND_TIME*) operand).MINUTE) && ( (CR.VALUE.DT.SECOND) == (*(PLC_DATE_AND_TIME*) operand).SECOND)  && ( (CR.VALUE.DT.NANOSECOND) >= (*(PLC_DATE_AND_TIME*) operand).NANOSECOND) );
      }
      break;
      case STRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) != (*(PLC_Bool*) operand) );
            CR.VALUE.STRING = *(PLC_String*) newValue; */
      }
      break;
      case WSTRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) != (*(PLC_Bool*) operand) );
            CR.VALUE.WSTRING = *(PLC_WString*) newValue; */
      }
      break;
      case BCD16:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD16) >= (*(PLC_BCD16*) operand) );
      }
      break;
      case BCD32:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD32) >= (*(PLC_BCD32*) operand) );
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* Less Than < */

void PLC_IL_LT(void* operand, PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   CR.TYPE = BOOL;

   switch (type)
   {
      case BOOL:
      {
          CR.VALUE.BOOL = ( (CR.VALUE.BOOL) < (*(PLC_BOOL*) operand) );
      }
      break;
      case BYTE:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BYTE) < (*(PLC_BYTE*) operand) );
      }
      break;
      case WORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.WORD) < (*(PLC_WORD*) operand) );
      }
      break;
      case DWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DWORD) < (*(PLC_DWORD*) operand) );
      }
      break;
      case LWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LWORD) < (*(PLC_LWORD*) operand) );
      }
      break;
      case SINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.SINT) < (*(PLC_SINT*) operand) );
      }
      break;
      case INT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.INT) < (*(PLC_INT*) operand) );
      }
      break;
      case DINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DINT) < (*(PLC_DINT*) operand) );
      }
      break;
      case LINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LINT) < (*(PLC_LINT*) operand) );
      }
      break;
      case USINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.USINT) < (*(PLC_USINT*) operand) );
      }
      break;
      case UINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UINT) < (*(PLC_UINT*) operand) );
      }
      break;
      case UDINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UDINT) < (*(PLC_UDINT*) operand) );
      }
      break;
      case ULINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.ULINT) < (*(PLC_ULINT*) operand) );
      }
      break;
      case REAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.REAL) < (*(PLC_REAL*) operand) );
      }
      break;
      case LREAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LREAL) < (*(PLC_LREAL*) operand) );
      }
      break;
      case TIME:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.TIME) < (*(PLC_TIME*) operand) );
      }
      break;
      case D:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.D.YEAR) < (*(PLC_DATE*) operand).YEAR ) ||
            ( ((CR.VALUE.D.YEAR) == (*(PLC_DATE*) operand).YEAR ) && ((CR.VALUE.D.MONTH) < (*(PLC_DATE*) operand).MONTH) ) ||
            ( ((CR.VALUE.D.YEAR) == (*(PLC_DATE*) operand).YEAR) && ((CR.VALUE.D.MONTH) == (*(PLC_DATE*) operand).MONTH) && ( (CR.VALUE.D.DAY) < (*(PLC_DATE*) operand).DAY) );
      }
      break;
      case TOD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.TOD.HOUR) < (*(PLC_TIME_OF_DAY*) operand).HOUR ) ||
            ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR ) && ((CR.VALUE.TOD.MINUTE) < (*(PLC_TIME_OF_DAY*) operand).MINUTE) ) ||
            ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR) && ((CR.VALUE.TOD.MINUTE) == (*(PLC_TIME_OF_DAY*) operand).MINUTE) && ( (CR.VALUE.TOD.SECOND) < (*(PLC_TIME_OF_DAY*) operand).SECOND) ) ||
         ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR) && ((CR.VALUE.TOD.MINUTE) == (*(PLC_TIME_OF_DAY*) operand).MINUTE) && ( (CR.VALUE.TOD.SECOND) == (*(PLC_TIME_OF_DAY*) operand).SECOND)  && ( (CR.VALUE.TOD.NANOSECOND) < (*(PLC_TIME_OF_DAY*) operand).NANOSECOND) );
      }
      break;
      case DT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DT.YEAR) < (*(PLC_DATE_AND_TIME*) operand).YEAR ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR ) && ((CR.VALUE.DT.MONTH) < (*(PLC_DATE_AND_TIME*) operand).MONTH) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) < (*(PLC_DATE_AND_TIME*) operand).DAY) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && (CR.VALUE.DT.HOUR) < (*(PLC_DATE_AND_TIME*) operand).HOUR ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR ) && ((CR.VALUE.DT.MINUTE) < (*(PLC_DATE_AND_TIME*) operand).MINUTE) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR) && ((CR.VALUE.DT.MINUTE) == (*(PLC_DATE_AND_TIME*) operand).MINUTE) && ( (CR.VALUE.DT.SECOND) < (*(PLC_DATE_AND_TIME*) operand).SECOND) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR) && ((CR.VALUE.DT.MINUTE) == (*(PLC_DATE_AND_TIME*) operand).MINUTE) && ( (CR.VALUE.DT.SECOND) == (*(PLC_DATE_AND_TIME*) operand).SECOND)  && ( (CR.VALUE.DT.NANOSECOND) < (*(PLC_DATE_AND_TIME*) operand).NANOSECOND) );
      }
      break;
      case STRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) != (*(PLC_Bool*) operand) );
            CR.VALUE.STRING = *(PLC_String*) newValue; */
      }
      break;
      case WSTRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) != (*(PLC_Bool*) operand) );
            CR.VALUE.WSTRING = *(PLC_WString*) newValue; */
      }
      break;
      case BCD16:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD16) < (*(PLC_BCD16*) operand) );
      }
      break;
      case BCD32:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD32) < (*(PLC_BCD32*) operand) );
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* Less than or Equal <= */

void PLC_IL_LE(void* operand, PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   CR.TYPE = BOOL;

   switch (type)
   {
      case BOOL:
      {
          CR.VALUE.BOOL = ( (CR.VALUE.BOOL) <= (*(PLC_BOOL*) operand) );
      }
      break;
      case BYTE:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BYTE) <= (*(PLC_BYTE*) operand) );
      }
      break;
      case WORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.WORD) <= (*(PLC_WORD*) operand) );
      }
      break;
      case DWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DWORD) <= (*(PLC_DWORD*) operand) );
      }
      break;
      case LWORD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LWORD) <= (*(PLC_LWORD*) operand) );
      }
      break;
      case SINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.SINT) <= (*(PLC_SINT*) operand) );
      }
      break;
      case INT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.INT) <= (*(PLC_INT*) operand) );
      }
      break;
      case DINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DINT) <= (*(PLC_DINT*) operand) );
      }
      break;
      case LINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LINT) <= (*(PLC_LINT*) operand) );
      }
      break;
      case USINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.USINT) <= (*(PLC_USINT*) operand) );
      }
      break;
      case UINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UINT) <= (*(PLC_UINT*) operand) );
      }
      break;
      case UDINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.UDINT) <= (*(PLC_UDINT*) operand) );
      }
      break;
      case ULINT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.ULINT) <= (*(PLC_ULINT*) operand) );
      }
      break;
      case REAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.REAL) <= (*(PLC_REAL*) operand) );
      }
      break;
      case LREAL:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.LREAL) <= (*(PLC_LREAL*) operand) );
      }
      break;
      case TIME:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.TIME) <= (*(PLC_TIME*) operand) );
      }
      break;
      case D:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.D.YEAR) <= (*(PLC_DATE*) operand).YEAR ) ||
            ( ((CR.VALUE.D.YEAR) == (*(PLC_DATE*) operand).YEAR ) && ((CR.VALUE.D.MONTH) <= (*(PLC_DATE*) operand).MONTH) ) ||
            ( ((CR.VALUE.D.YEAR) == (*(PLC_DATE*) operand).YEAR) && ((CR.VALUE.D.MONTH) == (*(PLC_DATE*) operand).MONTH) && ( (CR.VALUE.D.DAY) <= (*(PLC_DATE*) operand).DAY) );
      }
      break;
      case TOD:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.TOD.HOUR) <= (*(PLC_TIME_OF_DAY*) operand).HOUR ) ||
            ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR ) && ((CR.VALUE.TOD.MINUTE) <= (*(PLC_TIME_OF_DAY*) operand).MINUTE) ) ||
            ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR) && ((CR.VALUE.TOD.MINUTE) == (*(PLC_TIME_OF_DAY*) operand).MINUTE) && ( (CR.VALUE.TOD.SECOND) <= (*(PLC_TIME_OF_DAY*) operand).SECOND) ) ||
         ( ((CR.VALUE.TOD.HOUR) == (*(PLC_TIME_OF_DAY*) operand).HOUR) && ((CR.VALUE.TOD.MINUTE) == (*(PLC_TIME_OF_DAY*) operand).MINUTE) && ( (CR.VALUE.TOD.SECOND) == (*(PLC_TIME_OF_DAY*) operand).SECOND)  && ( (CR.VALUE.TOD.NANOSECOND) <= (*(PLC_TIME_OF_DAY*) operand).NANOSECOND) );
      }
      break;
      case DT:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.DT.YEAR) <= (*(PLC_DATE_AND_TIME*) operand).YEAR ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR ) && ((CR.VALUE.DT.MONTH) <= (*(PLC_DATE_AND_TIME*) operand).MONTH) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) <= (*(PLC_DATE_AND_TIME*) operand).DAY) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && (CR.VALUE.DT.HOUR) <= (*(PLC_DATE_AND_TIME*) operand).HOUR ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR ) && ((CR.VALUE.DT.MINUTE) <= (*(PLC_DATE_AND_TIME*) operand).MINUTE) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR) && ((CR.VALUE.DT.MINUTE) == (*(PLC_DATE_AND_TIME*) operand).MINUTE) && ( (CR.VALUE.DT.SECOND) <= (*(PLC_DATE_AND_TIME*) operand).SECOND) ) ||
            ( ((CR.VALUE.DT.YEAR) == (*(PLC_DATE_AND_TIME*) operand).YEAR) && ((CR.VALUE.DT.MONTH) == (*(PLC_DATE_AND_TIME*) operand).MONTH) && ( (CR.VALUE.DT.DAY) == (*(PLC_DATE_AND_TIME*) operand).DAY) && ((CR.VALUE.DT.HOUR) == (*(PLC_DATE_AND_TIME*) operand).HOUR) && ((CR.VALUE.DT.MINUTE) == (*(PLC_DATE_AND_TIME*) operand).MINUTE) && ( (CR.VALUE.DT.SECOND) == (*(PLC_DATE_AND_TIME*) operand).SECOND)  && ( (CR.VALUE.DT.NANOSECOND) <= (*(PLC_DATE_AND_TIME*) operand).NANOSECOND) );
      }
      break;
      case STRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) != (*(PLC_Bool*) operand) );
            CR.VALUE.STRING = *(PLC_String*) newValue; */
      }
      break;
      case WSTRING:
      {
         /* CR.VALUE.BOOL = ( (CR.VALUE.BOOL) != (*(PLC_Bool*) operand) );
            CR.VALUE.WSTRING = *(PLC_WString*) newValue; */
      }
      break;
      case BCD16:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD16) <= (*(PLC_BCD16*) operand) );
      }
      break;
      case BCD32:
      {
         CR.VALUE.BOOL = ( (CR.VALUE.BCD32) <= (*(PLC_BCD32*) operand) );
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* ADD + Overloaded for:
   SINT, INT, DINT, LINT, USINT, UINT, UDINT, ULINT, REAL, LREAL, TIME

   CHECK CARRY AND OVERFLOW (O y V) bits implementation of
   microprocessors status registers work only for signed
   and unsigned integers, see how to do for floats (TODO). */

void PLC_IL_ADD(void* operand, PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   switch (type)
   {
      case SINT:
      {
         CR.VALUE.SINT += (*(PLC_SINT*) operand);
      }
      break;
      case INT:
      {
         CR.VALUE.INT += (*(PLC_INT*) operand);
      }
      break;
      case DINT:
      {
         CR.VALUE.DINT += (*(PLC_DINT*) operand);
      }
      break;
      case LINT:
      {
         CR.VALUE.LINT += (*(PLC_LINT*) operand);
      }
      break;
      case USINT:
      {
         CR.VALUE.USINT += (*(PLC_USINT*) operand);
      }
      break;
      case UINT:
      {
         CR.VALUE.UINT += (*(PLC_UINT*) operand);
      }
      break;
      case UDINT:
      {
         CR.VALUE.UDINT += (*(PLC_UDINT*) operand);
      }
      break;
      case ULINT:
      {
         CR.VALUE.ULINT += (*(PLC_ULINT*) operand);
      }
      break;
      case REAL:
      {
         CR.VALUE.REAL += (*(PLC_REAL*) operand);
      }
      break;
      case LREAL:
      {
         CR.VALUE.LREAL += (*(PLC_LREAL*) operand);
      }
      break;
      case TIME:
      {
         CR.VALUE.TIME += (*(PLC_TIME*) operand);
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* SUB - Overloaded for:
   SINT, INT, DINT, LINT, USINT, UINT, UDINT, ULINT, REAL, LREAL, TIME

   CHECK CARRY AND OVERFLOW (O y V) bits implementation of
   microprocessors status registers work only for signed
   and unsigned integers, see how to do for floats (TODO). */

void PLC_IL_SUB(void* operand, PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   switch (type)
   {
      case SINT:
      {
         CR.VALUE.SINT -= (*(PLC_SINT*) operand);
      }
      break;
      case INT:
      {
         CR.VALUE.INT -= (*(PLC_INT*) operand);
      }
      break;
      case DINT:
      {
         CR.VALUE.DINT -= (*(PLC_DINT*) operand);
      }
      break;
      case LINT:
      {
         CR.VALUE.LINT -= (*(PLC_LINT*) operand);
      }
      break;
      case USINT:
      {
         CR.VALUE.USINT -= (*(PLC_USINT*) operand);
      }
      break;
      case UINT:
      {
         CR.VALUE.UINT -= (*(PLC_UINT*) operand);
      }
      break;
      case UDINT:
      {
         CR.VALUE.UDINT -= (*(PLC_UDINT*) operand);
      }
      break;
      case ULINT:
      {
         CR.VALUE.ULINT -= (*(PLC_ULINT*) operand);
      }
      break;
      case REAL:
      {
         CR.VALUE.REAL -= (*(PLC_REAL*) operand);
      }
      break;
      case LREAL:
      {
         CR.VALUE.LREAL -= (*(PLC_LREAL*) operand);
      }
      break;
      case TIME:
      {
         CR.VALUE.TIME -= (*(PLC_TIME*) operand);
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* MUL * Overloaded for:
   SINT, INT, DINT, LINT, USINT, UINT, UDINT, ULINT, REAL, LREAL */

void PLC_IL_MUL(void* operand, PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   switch (type)
   {
      case SINT:
      {
         CR.VALUE.SINT *= (*(PLC_SINT*) operand);
      }
      break;
      case INT:
      {
         CR.VALUE.INT *= (*(PLC_INT*) operand);
      }
      break;
      case DINT:
      {
         CR.VALUE.DINT *= (*(PLC_DINT*) operand);
      }
      break;
      case LINT:
      {
         CR.VALUE.LINT *= (*(PLC_LINT*) operand);
      }
      break;
      case USINT:
      {
         CR.VALUE.USINT *= (*(PLC_USINT*) operand);
      }
      break;
      case UINT:
      {
         CR.VALUE.UINT *= (*(PLC_UINT*) operand);
      }
      break;
      case UDINT:
      {
         CR.VALUE.UDINT *= (*(PLC_UDINT*) operand);
      }
      break;
      case ULINT:
      {
         CR.VALUE.ULINT *= (*(PLC_ULINT*) operand);
      }
      break;
      case REAL:
      {
         CR.VALUE.REAL *= (*(PLC_REAL*) operand);
      }
      break;
      case LREAL:
      {
         CR.VALUE.LREAL *= (*(PLC_LREAL*) operand);
      }
      break;
      case TIME:
      {
         CR.VALUE.TIME *= (*(PLC_TIME*) operand);
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* DIV / Overloaded for:
   SINT, INT, DINT, LINT, USINT, UINT, UDINT, ULINT, REAL, LREAL */

void PLC_IL_DIV(void* operand, PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   switch (type)
   {
      case SINT:
      {
         CR.VALUE.SINT /= (*(PLC_SINT*) operand);
      }
      break;
      case INT:
      {
         CR.VALUE.INT /= (*(PLC_INT*) operand);
      }
      break;
      case DINT:
      {
         CR.VALUE.DINT /= (*(PLC_DINT*) operand);
      }
      break;
      case LINT:
      {
         CR.VALUE.LINT /= (*(PLC_LINT*) operand);
      }
      break;
      case USINT:
      {
         CR.VALUE.USINT /= (*(PLC_USINT*) operand);
      }
      break;
      case UINT:
      {
         CR.VALUE.UINT /= (*(PLC_UINT*) operand);
      }
      break;
      case UDINT:
      {
         CR.VALUE.UDINT /= (*(PLC_UDINT*) operand);
      }
      break;
      case ULINT:
      {
         CR.VALUE.ULINT /= (*(PLC_ULINT*) operand);
      }
      break;
      case REAL:
      {
         CR.VALUE.REAL /= (*(PLC_REAL*) operand);
      }
      break;
      case LREAL:
      {
         CR.VALUE.LREAL /= (*(PLC_LREAL*) operand);
      }
      break;
      case TIME:
      {
         CR.VALUE.TIME /= (*(PLC_TIME*) operand);
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* MOD / Overloaded for:
   SINT, INT, DINT, LINT, USINT, UINT, UDINT, ULINT */

void PLC_IL_MOD(void* operand, PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   switch (type)
   {
      case SINT:
      {
         CR.VALUE.SINT %= (*(PLC_SINT*) operand);
      }
      break;
      case INT:
      {
         CR.VALUE.INT %= (*(PLC_INT*) operand);
      }
      break;
      case DINT:
      {
         CR.VALUE.DINT %= (*(PLC_DINT*) operand);
      }
      break;
      case LINT:
      {
         CR.VALUE.LINT %= (*(PLC_LINT*) operand);
      }
      break;
      case USINT:
      {
         CR.VALUE.USINT %= (*(PLC_USINT*) operand);
      }
      break;
      case UINT:
      {
         CR.VALUE.UINT %= (*(PLC_UINT*) operand);
      }
      break;
      case UDINT:
      {
         CR.VALUE.UDINT %= (*(PLC_UDINT*) operand);
      }
      break;
      case ULINT:
      {
         CR.VALUE.ULINT %= (*(PLC_ULINT*) operand);
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* AND - logical AND - Overloaded for:
   BOOL, BYTE, WORD, DWORD, LWORD */

void PLC_IL_AND(void* operand, PLC_EnumDataTypes type, PLC_EnumModifiers modifier)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   switch (type)
   {
      case BOOL:
      {
         if (modifier == N)
         {
            CR.VALUE.BOOL &= !(*(PLC_BOOL*) operand);
         }
         else
         {
            CR.VALUE.BOOL &= (*(PLC_BOOL*) operand);
         }
      }
      break;
      case BYTE:
      {
         if (modifier == N)
         {
            CR.VALUE.BYTE &= ~(*(PLC_BYTE*) operand);
         }
         else
         {
            CR.VALUE.BYTE &= (*(PLC_BYTE*) operand);
         }
      }
      break;
      case WORD:
      {
         if (modifier == N)
         {
            CR.VALUE.WORD &= ~(*(PLC_WORD*) operand);
         }
         else
         {
            CR.VALUE.WORD &= (*(PLC_WORD*) operand);
         }
      }
      break;
      case DWORD:
      {
         if (modifier == N)
         {
            CR.VALUE.DWORD &= ~(*(PLC_DWORD*) operand);
         }
         else
         {
            CR.VALUE.DWORD &= (*(PLC_DWORD*) operand);
         }
      }
      break;
      case LWORD:
      {
         if (modifier == N)
         {
            CR.VALUE.LWORD &= ~(*(PLC_LWORD*) operand);
         }
         else
         {
            CR.VALUE.LWORD &= (*(PLC_LWORD*) operand);
         }
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* OR - logical OR - Overloaded for:
   BOOL, BYTE, WORD, DWORD, LWORD */

void PLC_IL_OR(void* operand, PLC_EnumDataTypes type, PLC_EnumModifiers modifier)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   switch (type)
   {
      case BOOL:
      {
         if (modifier == N)
         {
            CR.VALUE.BOOL |= !(*(PLC_BOOL*) operand);
         }
         else
         {
            CR.VALUE.BOOL |= (*(PLC_BOOL*) operand);
         }
      }
      break;
      case BYTE:
      {
         if (modifier == N)
         {
            CR.VALUE.BYTE |= ~(*(PLC_BYTE*) operand);
         }
         else
         {
            CR.VALUE.BYTE |= (*(PLC_BYTE*) operand);
         }
      }
      break;
      case WORD:
      {
         if (modifier == N)
         {
            CR.VALUE.WORD |= ~(*(PLC_WORD*) operand);
         }
         else
         {
            CR.VALUE.WORD |= (*(PLC_WORD*) operand);
         }
      }
      break;
      case DWORD:
      {
         if (modifier == N)
         {
            CR.VALUE.DWORD |= ~(*(PLC_DWORD*) operand);
         }
         else
         {
            CR.VALUE.DWORD |= (*(PLC_DWORD*) operand);
         }
      }
      break;
      case LWORD:
      {
         if (modifier == N)
         {
            CR.VALUE.LWORD |= ~(*(PLC_LWORD*) operand);
         }
         else
         {
            CR.VALUE.LWORD |= (*(PLC_LWORD*) operand);
         }
      }
      break;
   }
}

/*-----------------------------------------------------------*/
/* XOR - logical XOR - Overloaded for:
   BOOL, BYTE, WORD, DWORD, LWORD */

void PLC_IL_XOR(void* operand, PLC_EnumDataTypes type, PLC_EnumModifiers modifier)
{
   extern PLC_SymbolicRegister CR;

   /* If CR and OPERAND are not of the same type then ERROR (TODO). */
   if(CR.TYPE != type)
   {
      return;
   }

   switch (type)
   {
      case BOOL:
      {
         if (modifier == N)
         {
            CR.VALUE.BOOL ^= !(*(PLC_BOOL*) operand);
         }
         else
         {
            CR.VALUE.BOOL ^= (*(PLC_BOOL*) operand);
         }
      }
      break;
      case BYTE:
      {
         if (modifier == N)
         {
            CR.VALUE.BYTE ^= ~(*(PLC_BYTE*) operand);
         }
         else
         {
            CR.VALUE.BYTE ^= (*(PLC_BYTE*) operand);
         }
      }
      break;
      case WORD:
      {
         if (modifier == N)
         {
            CR.VALUE.WORD ^= ~(*(PLC_WORD*) operand);
         }
         else
         {
            CR.VALUE.WORD ^= (*(PLC_WORD*) operand);
         }
      }
      break;
      case DWORD:
      {
         if (modifier == N)
         {
            CR.VALUE.DWORD ^= ~(*(PLC_DWORD*) operand);
         }
         else
         {
            CR.VALUE.DWORD ^= (*(PLC_DWORD*) operand);
         }
      }
      break;
      case LWORD:
      {
         if (modifier == N)
         {
            CR.VALUE.LWORD ^= ~(*(PLC_LWORD*) operand);
         }
         else
         {
            CR.VALUE.LWORD ^= (*(PLC_LWORD*) operand);
         }
      }
      break;
   }
}

/*-----------------------------------------------------------
/* NOT - logical negation ( one´s complement) - Overloaded for:
   BOOL, BYTE, WORD, DWORD, LWORD */

void PLC_IL_NOT(PLC_EnumDataTypes type)
{
   extern PLC_SymbolicRegister CR;

   switch (type)
   {
      case BOOL:
      {
         CR.VALUE.BOOL = !(CR.VALUE.BOOL);

      }
      break;
      case BYTE:
      {
         CR.VALUE.BYTE = ~(CR.VALUE.BYTE);
      }
      break;
      case WORD:
      {
         CR.VALUE.WORD = ~(CR.VALUE.WORD);
      }
      break;
      case DWORD:
      {
         CR.VALUE.DWORD = ~(CR.VALUE.DWORD);
      }
      break;
      case LWORD:
      {
         CR.VALUE.LWORD = ~(CR.VALUE.LWORD);
      }
      break;
   }
}

/*-----------------------------------------------------------*/

/** \brief PLC IL Instructions that are not represented as C Functions (just inserted in POU C code).

RET
---

return;


RETC
----

if(CR.VALUE.BOOL)
{
return;
}


RETCN
-----

if(!(CR.VALUE.BOOL))
{
return;
}


JMP LABEL
---------

goto LABEL;


JMPC LABEL
----------

if(CR.VALUE.BOOL)
{
   goto LABEL;
}


JMPCN LABEL
-----------

if(!(CR.VALUE.BOOL))
{
   goto LABEL;
}


Jump label
----------

LABEL:

*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
