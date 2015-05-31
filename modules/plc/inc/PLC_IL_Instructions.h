/* Copyright 2012-2014, Eric Nicolás Pernia
 * All rights reserved.
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

/*
 * PLC_Instructions.h
 *
 *  Created on: 2012/02/04
 *      Author: Eric Pernia
 */

#ifndef PLC_IL_INSTRUCTIONS_H_
	#define PLC_IL_INSTRUCTIONS_H_

   #include "PLC_Registers.h"
   #include "PLC_Lib.h"

	/* Data type enumeration for PLC Boolean Operations (U, UN, O, ON, X, XN) */

	typedef enum{AND, ANDN, OR, ORN, XOR, XORN} PLC_Boolean_Operation;

	// Enum de Modificadores de IL
	typedef enum{ NullModifier, N, C, NullModifierC, NC} PLC_EnumModifiers;


	/*Functions Prototypes*/

	// Carga y almacenamiento
	void PLC_IL_LD_BOOL_Literal( PLC_BOOL, PLC_EnumModifiers);
	void PLC_IL_LD_INT_Literal( PLC_INT, PLC_EnumModifiers);
	void PLC_IL_LD_TIME_Literal( PLC_TIME, PLC_EnumModifiers);
	void PLC_IL_LD(void*, PLC_WORD, PLC_EnumDataTypes, PLC_EnumModifiers);
	void PLC_IL_ST(void*, PLC_WORD, PLC_EnumModifiers);

	// Set y Reset
	void PLC_IL_S(PLC_BOOL* operand);
	void PLC_IL_R(PLC_BOOL* operand);

	// Comparación
	void PLC_IL_EQ(void*, PLC_EnumDataTypes);
	void PLC_IL_NE(void*, PLC_EnumDataTypes);
	void PLC_IL_GT(void*, PLC_EnumDataTypes);
	void PLC_IL_GE(void*, PLC_EnumDataTypes);
	void PLC_IL_LT(void*, PLC_EnumDataTypes);
	void PLC_IL_LE(void*, PLC_EnumDataTypes);

	// Artitméticas
	void PLC_IL_ADD(void*, PLC_EnumDataTypes);
	void PLC_IL_SUB(void*, PLC_EnumDataTypes);
	void PLC_IL_MUL(void*, PLC_EnumDataTypes);
	void PLC_IL_DIV(void*, PLC_EnumDataTypes);
	void PLC_IL_MOD(void*, PLC_EnumDataTypes);

	// Lógicas
	void PLC_IL_AND(void*, PLC_EnumDataTypes, PLC_EnumModifiers);
	void PLC_IL_OR(void*, PLC_EnumDataTypes, PLC_EnumModifiers);
	void PLC_IL_XOR(void*, PLC_EnumDataTypes, PLC_EnumModifiers);
	void PLC_IL_NOT(PLC_EnumDataTypes);

#endif /* PLC_IL_INSTRUCTIONS_H_ */
