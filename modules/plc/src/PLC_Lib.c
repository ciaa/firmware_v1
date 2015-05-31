/*------------------------------------------------------------------------

Copyright © 2012-2014 Eric Nicolás Pernia. All rights reserved.

This file is part of IDE4PLC Firmware. http://ide4plc.wordpress.com

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

 * Redistributions of source code and project files must retain the above 
   copyright notice, this list of conditions and the following disclaimer.
  
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither the name of the copyright holders nor the names of its contributors
   may be used to endorse or promote products derived from this product without
   specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.

------------------------------------------------------------------------*/
/*
 * PLC_Lib.h
 *
 *  Created on: 2014/09/11
 *      Author: Eric Pernia
 */


#include "PLC_Lib.h"

//#include "basic_io.h"	// Para enviar datos a la consola. Viene de los Ejemplos de FreeRTOS
/*
// Error_Handler: Manejo de errores cometidos por mal uso de las instrucciones.
void PLC_Error_Handler(PLC_USINT ErrCode)
{
	switch(ErrCode)
	{
		case 1:
			vPrintString( "Error 1: Pila de parentesis desbordada.\n" );
		break;
		case 2:
			vPrintString( "Error 2: Pila de parentesis vacia.\n" );
		break;
		default:
		break;
	}
}

//-----------------------------------------------------------

// Algoritmo para calcular el dia de la semana
PLC_INT DT_Weekday(PLC_INT Year, PLC_INT Month, PLC_INT Day)
{

	int a, y, m, NumZeller, NumDT;
	a = (14 - Month) / 12;
	y = Year - a;
	m = Month + 12 * a - 2;

	// Para el calendario Juliano:
	// NumZeller = (5 + Day + y + y/4 + (31*m)/12) % 7

	// Para el calendario Gregoriano:
	NumZeller = (Day + y + y/4 - y/100 + y/400 + (31*m)/12) % 7;

	// El resultado en NumZeller es:
	//	 0 = Domingo
	//	 1 = Lunes
	//	 2 = Martes
	//	 3 = Miercoles
	//	 4 = Jueves
	//	 5 = Viernes
	//	 6 = Sabado

	NumDT = NumZeller + 1;

	return NumDT;
}
//-----------------------------------------------------------

// Muestra por consola el dia de la semana.
void Day_Display(PLC_INT NumDT)
{
	switch(NumDT)
	{
		case 1:
			vPrintString( "Sunday. Domingo.\n" );
		break;
		case 2:
			vPrintString( "Monday. Lunes.\n" );
		break;
		case 3:
			vPrintString( "Tuesday. Martes.\n" );
		break;
		case 4:
			vPrintString( "Wednesday. Miercoles.\n" );
		break;
		case 5:
			vPrintString( "Thursday. Jueves.\n" );
		break;
		case 6:
			vPrintString( "Friday. Viernes.\n" );
		break;
		case 7:
			vPrintString( "Saturday. Sabado.\n" );
		break;
		default:
			vPrintString( "No es una fecha valida.\n" );
	}
}
*/
