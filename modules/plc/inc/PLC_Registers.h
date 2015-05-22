/* Copyright 2012-2015, Eric Nicolás Pernia
 * Copyright 2015, Leandro Kollenberger
 * All rights reserved.

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
 * PLC_Registers.h
 *
 *  Created on: 2012/02/04
 *      Author: Eric Pernia
 */

#ifndef PLC_REGISTERS_H_
	#define PLC_REGISTERS_H_

	#include "PLC_1KByteStructures.h"


	// Registros del PLC de 1KByte.
	// Esta estructura se utiliza para las variables representadas Directamente.
	typedef union
	{
		PLC_1KByteAsBits	X;
		PLC_1KByteAsBytes	B;
		PLC_1KByteAsWords	W;
		PLC_1KByteAsDoubles	D;
		PLC_1KByteAsLongs	L;
	}PLC_1KByteRegister;



	/* I/O Structures */

	typedef struct
	{
		boolean x0:1;
		boolean x1:1;
		boolean x2:1;
		boolean x3:1;
		boolean x4:1;
		boolean x5:1;
		boolean x6:1;
		boolean x7:1;
	}PLC_1ByteAsBits;

	typedef struct
	{
		uint8_t b0;
	}PLC_1ByteAsByte;

	// Registros del PLC de 1 Byte.
	// Esta estructura se utiliza para las variables representadas Directamente.
	typedef union
	{
		PLC_1ByteAsBits	X;
		PLC_1ByteAsByte	B;
	}PLC_1ByteRegister;



	// Enum de tipos de datos elementales
	typedef enum{ BOOL, BYTE, WORD, DWORD, LWORD, SINT, INT, DINT, LINT,
				  USINT, UINT, UDINT, ULINT, REAL, LREAL, TIME, D, TOD, DT,
				  STRING, WSTRING, BCD16, BCD32, OTHER
				} PLC_EnumDataTypes;

	typedef union
	{
		PLC_BOOL			BOOL;		//  0
		PLC_BYTE			BYTE;		//  1
		PLC_WORD			WORD;		//  2
		PLC_DWORD			DWORD;		//  3
		PLC_LWORD			LWORD;		//  4

		PLC_SINT			SINT;		//  5
		PLC_INT				INT;		//  6
		PLC_DINT			DINT;		//  7
		PLC_LINT			LINT;		//  8

		PLC_USINT			USINT;		//  9
		PLC_UINT			UINT;		// 10
		PLC_UDINT			UDINT;		// 11
		PLC_ULINT			ULINT;		// 12

		PLC_REAL 			REAL;		// 13
		PLC_LREAL 			LREAL;		// 14

		PLC_TIME 			TIME;		// 15
		PLC_DATE 			D;			// 16
		PLC_TIME_OF_DAY		TOD;		// 17
		PLC_DATE_AND_TIME	DT;			// 18

	  	PLC_STRING			STRING;		// 19
		PLC_WSTRING 		WSTRING;	// 20

		PLC_BCD16			BCD16;		// 21
		PLC_BCD32 			BCD32;		// 22

		PLC_1KByteAsLongs	KBYTE;

	}PLC_SyRegDataTypesAccess;


	typedef struct
	{
		PLC_EnumDataTypes			TYPE;
		PLC_SyRegDataTypesAccess	VALUE;
	}PLC_SymbolicRegister;








	/*Functions Prototypes*/

	PLC_1ByteRegister Convert_PLC_BYTE_2_PLC_1ByteRegister(PLC_BYTE);
	PLC_BYTE Convert_PLC_1ByteRegister_2_PLC_BYTE(PLC_1ByteRegister);


#endif /* PLC_REGISTERS_H_ */
