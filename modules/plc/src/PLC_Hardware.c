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
 * PLC_Hardware.c
 *
 *  Created on: 2012/02/04
 *      Author: Eric Pernia
 */

#include "PLC_StandardCDataTypes.h"	// Standard C Data Types
#include "PLC_Hardware.h"

// Parche
typedef uint8_t uint8;

/** \brief File descriptor for digital output ports
 *
 * Device path /dev/dio/out/0
 */
 
static int32_t fd_in, fd_out;

//-----------------------------------------------------------

void PLC_DIs_Init (void)
{
   /* open CIAA digital inputs */
   fd_in = ciaaPOSIX_open("/dev/dio/in/0", O_RDWR);
}

//-----------------------------------------------------------

void PLC_DOs_Init(void)
{
   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", O_RDWR);

}
//-----------------------------------------------------------

void PLC_DIOs_Init(void)
{
	//ciaaIOInit();
	PLC_DOs_Init();
	PLC_DIs_Init();
}

//-----------------------------------------------------------

uint8_t PLC_Read_DIs(void)
{
   uint8 status = 0;
   ciaaPOSIX_read(fd_in, &status, 1);

    return (uint8_t)status;

    /*if ( ciaaReadInput(PLC_IN_0) )
    	{status |= bit_0;}
    if ( ciaaReadInput(PLC_IN_1) )
    	{status |= bit_1;}
    if ( ciaaReadInput(PLC_IN_2) )
    	{status |= bit_2;}
    if ( ciaaReadInput(PLC_IN_3) )
    	{status |= bit_3;}
    if ( ciaaReadInput(PLC_IN_4) )
    	{status |= bit_4;}
    if ( ciaaReadInput(PLC_IN_5) )
    	{status |= bit_5;}
    if ( ciaaReadInput(PLC_IN_6) )
    	{status |= bit_6;}
    if ( ciaaReadInput(PLC_IN_7) )
		{status |= bit_7;}*/
	
}
//-----------------------------------------------------------

void PLC_Write_DOs(uint8_t salidas)
{
	
	ciaaPOSIX_write(fd_out, (uint8 *)&salidas, 1);

	/*if ( (salidas & bit_0) ) {
		ciaaWriteOutput(PLC_OUT_0, 1);
	}
	else {
		ciaaWriteOutput(PLC_OUT_0, 0);
	}

	if ( (salidas & bit_1) ) {
		ciaaWriteOutput(PLC_OUT_1, 1);
	}
	else {
		ciaaWriteOutput(PLC_OUT_1, 0);
	}

	if ( (salidas & bit_2) ) {
		ciaaWriteOutput(PLC_OUT_2, 1);
	}
	else {
		ciaaWriteOutput(PLC_OUT_2, 0);
	}

	if ( (salidas & bit_3) ) {
		ciaaWriteOutput(PLC_OUT_3, 1);
	}
	else {
		ciaaWriteOutput(PLC_OUT_3, 0);
	}

	if ( (salidas & bit_4) ) {
		ciaaWriteOutput(PLC_OUT_4, 1);
	}
	else {
		ciaaWriteOutput(PLC_OUT_4, 0);
	}

	if ( (salidas & bit_5) ) {
		ciaaWriteOutput(PLC_OUT_5, 1);
	}
	else {
		ciaaWriteOutput(PLC_OUT_5, 0);
	}

	if ( (salidas & bit_6) ) {
		ciaaWriteOutput(PLC_OUT_6, 1);
	}
	else {
		ciaaWriteOutput(PLC_OUT_6, 0);
	}

	if ( (salidas & bit_7) ) {
		ciaaWriteOutput(PLC_OUT_7, 1);
	}
	else {
		ciaaWriteOutput(PLC_OUT_7, 0);
	}*/

}

