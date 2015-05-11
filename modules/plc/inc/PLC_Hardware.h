/*------------------------------------------------------------------------

Copyright © 2012-2014 Eric Nicolás Pernia.
Copyright © 2015 Eric Nicolás Pernia.
Copyright © 2015 Leandro Kollenberger.
All rights reserved.

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
 * PLC_Hardware.h
 *
 *  Created on: 2012/02/04
 *      Author: Eric Pernia
 */

#ifndef PLC_HARDWARE_H_
	#define PLC_HARDWARE_H_

	/* CONFIGURACION DE HARWARE DE PLACA CIAA-NXP */

	/*	
	#if defined (__USE_LPCOPEN)
		#if defined(NO_BOARD_LIB)
			#include "chip.h"
		#else
			#include "board.h"
		#endif
	#endif
	*/
	
	#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
	#include "ciaaPOSIX_string.h" /* <= string header */
	#include "ciaak.h"            /* <= ciaa kernel header */

	//#include <cr_section_macros.h>

	//#include "ciaaIO.h"	// CIAA I/O Driver

	#define PLC_IN_0 0
	#define PLC_IN_1 1
	#define PLC_IN_2 2
	#define PLC_IN_3 3
	#define PLC_IN_4 4
	#define PLC_IN_5 5
	#define PLC_IN_6 6
	#define PLC_IN_7 7

	#define PLC_OUT_0 0
	#define PLC_OUT_1 1
	#define PLC_OUT_2 2
	#define PLC_OUT_3 3
	#define PLC_OUT_4 4
	#define PLC_OUT_5 5
	#define PLC_OUT_6 6
	#define PLC_OUT_7 7

	#define PLC_RUN (1UL << 10)
	#define PLC_ERR (1UL << 11)


	/*Public Functions Prototypes*/

	void PLC_DIOs_Init(void);		// Inicializa las entradas y salidas digitales del PLC

	uint8_t PLC_Read_DIs(void);		// Lee entradas digitales del LPCxpresso
	void PLC_Write_DOs(uint8_t);		// Escribe las salidas digitales del LPCxpresso

#endif /* PLC_HARDWARE_H_ */
