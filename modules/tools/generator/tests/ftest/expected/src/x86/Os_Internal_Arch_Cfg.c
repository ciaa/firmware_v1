 /********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

/* Copyright 2008, 2009 Mariano Cerdeiro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
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

/** \brief FreeOSEK Os Generated Internal Achitecture Configuration Implementation File
 **
 ** \file Os_Internal_Arch_Cfg.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
 ** @{ */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
/** \brief InitTask context */
TaskContextType TaskContextInitTask;
/** \brief PeriodicTask context */
TaskContextType TaskContextPeriodicTask;


InterruptType InterruptTable[INTERRUPTS_COUNT] =
{
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 0 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 1 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 2 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 3 */
   OSEK_ISR_HWTimer0, /* HW Timer 0 Interrupt handler */
   OSEK_ISR_HWTimer1, /* HW Timer 1 Interrupt handler */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 6 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 7 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 8 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 9 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 10 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 11 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 12 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 13 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 14 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 15 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 16 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 17 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 18 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 19 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 20 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 21 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 22 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 23 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 24 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 25 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 26 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 27 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 28 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 29 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 30 */
   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt 31 */
};

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

