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
 * PLC_StandardCDataTypes.h
 *
 *  Created on: 2014/09/10
 *      Author: Eric Pernia
 */

#ifndef PLC_STANDARD_DATA_TYPES_H
	#define PLC_STANDARD_DATA_TYPES_H

	/* REDlib includes. */
	//#include <stdbool.h>
	#include <stdint.h>
	#include <string.h>

	#define true 1 
	#define false 0


	/*Defino el tipo boolean usando el tipo bool definido en stdbool.h */
	//typedef bool boolean;

	/* Bit masks for 8 bit variables like unsigned char.*/
	#define bit_0 0x01
	#define bit_1 0x02
	#define bit_2 0x04
	#define bit_3 0x08
	#define bit_4 0x10
	#define bit_5 0x20
	#define bit_6 0x40
	#define bit_7 0x80

#endif /* PLC_STANDARD_DATA_TYPES_H */
