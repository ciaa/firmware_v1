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

#ifndef PLC_HARDWARE_H_
#define PLC_HARDWARE_H_
/** \brief Hardware interaction layer
 **
 ** Hardware interaction layer
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
 * 20120204 v0.0.1 ErPe initial version (for LPCXpresso 1768/9)
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief PLC DIGITAL INPUT PINS */
#define PLC_IN_0 0
#define PLC_IN_1 1
#define PLC_IN_2 2
#define PLC_IN_3 3
#define PLC_IN_4 4
#define PLC_IN_5 5
#define PLC_IN_6 6
#define PLC_IN_7 7
/** \brief PLC DIGITAL OUTPUT PINS */
#define PLC_OUT_0 0
#define PLC_OUT_1 1
#define PLC_OUT_2 2
#define PLC_OUT_3 3
#define PLC_OUT_4 4
#define PLC_OUT_5 5
#define PLC_OUT_6 6
#define PLC_OUT_7 7
/** \brief PLC STATUS PINS */
#define PLC_RUN (1UL << 10)
#define PLC_ERR (1UL << 11)
/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief PLC PLC_DIOs_Init */
void PLC_DIOs_Init(void);		/* Initialize Digital Inputs and Outputs */
/** \brief PLC_Read_DIs */
uint8_t PLC_Read_DIs(void);		/* Read Digital Outputs */
/** \brief PLC_Write_DOs */
void PLC_Write_DOs(uint8_t);	/* Write Digital Outputs */

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* PLC_HARDWARE_H_ */

