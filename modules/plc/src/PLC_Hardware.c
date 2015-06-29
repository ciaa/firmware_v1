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
 ** PLC Hardware interaction layer
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
#include "PLC_StandardCDataTypes.h"   /* <= Standard C Data Types */
#include "ciaaPOSIX_stdio.h"          /* <= device handler header */
#include "PLC_Hardware.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief File descriptors for digital input and output ports
 *
 * fd_in Device path /dev/dio/in/0
 * fd_out Device path /dev/dio/out/0
 */
static int32_t fd_in, fd_out;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief This function initialize PLC Digital Inputs */
void PLC_DIs_Init (void)
{
   /* open CIAA digital inputs */
   fd_in = ciaaPOSIX_open("/dev/dio/in/0", O_RDWR);
}

/** \brief This function initialize PLC Digital Outputs */
void PLC_DOs_Init(void)
{
   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", O_RDWR);

}

/** \brief This function initialize PLC Digital Inputs and Outputs */
void PLC_DIOs_Init(void)
{
   PLC_DOs_Init();
   PLC_DIs_Init();
}

/** \brief This function read PLC Digital Inputs */
uint8_t PLC_Read_DIs(void)
{
   uint8_t status = 0;
   ciaaPOSIX_read(fd_in, &status, 1);

   return status;
}

/** \brief This function write PLC Digital Outputs */
void PLC_Write_DOs(uint8_t salidas)
{
   ciaaPOSIX_write(fd_out, &salidas, 1);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
