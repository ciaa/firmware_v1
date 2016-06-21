/* Copyright 2014, 2015 Pablo Ridolfi (UTN-FRBA)
 * Copyright 2016 Esteban Volentini (LabMicro - UNT)
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

/** \brief Internal source file of CIAA_NXP Board
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Boards CIAA Boards
 ** @{ */
/** \addtogroup CIAA_NXP CIAA_NXP Board
 ** @{ */


/*==================[inclusions]=============================================*/
#include "board_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

const ciaaDriverDio_portType ciaaDriverDio_pins0[] = {
   { { 4, 0, MD_PUP|MD_EZI|MD_ZI|FUNC0 }, 2,  0, false},
   { { 4, 1, MD_PUP|MD_EZI|MD_ZI|FUNC0 }, 2,  1, false},
   { { 4, 2, MD_PUP|MD_EZI|MD_ZI|FUNC0 }, 2,  2, false},
   { { 4, 3, MD_PUP|MD_EZI|MD_ZI|FUNC0 }, 2,  3, false},
   { { 7, 3, MD_PUP|MD_EZI|MD_ZI|FUNC0 }, 3, 11, false},
   { { 7, 4, MD_PUP|MD_EZI|MD_ZI|FUNC0 }, 3, 12, false},
   { { 7, 5, MD_PUP|MD_EZI|MD_ZI|FUNC0 }, 3, 13, false},
   { { 7, 6, MD_PUP|MD_EZI|MD_ZI|FUNC0 }, 3, 14, false}
};

const ciaaDriverDio_portType ciaaDriverDio_pins1[] = {
   { { 4,  8, MD_PUP|MD_EZI|FUNC4 }, 5, 12, true},
   { { 4,  9, MD_PUP|MD_EZI|FUNC4 }, 5, 13, true},
   { { 4, 10, MD_PUP|MD_EZI|FUNC4 }, 5, 14, true},
   { { 1,  5, MD_PUP|MD_EZI|FUNC0 }, 1,  8, true},
   { { 4,  4, MD_PUP|MD_EZI|FUNC0 }, 2,  4, true},
   { { 4,  5, MD_PUP|MD_EZI|FUNC0 }, 2,  5, true},
   { { 4,  6, MD_PUP|MD_EZI|FUNC0 }, 2,  6, true},
   { { 2,  1, MD_PUP|MD_EZI|FUNC0 }, 5,  1, true},
};

/*==================[external data definition]===============================*/

const ciaaDriverDio_portType ciaaDriverDio_port0 = {
   ciaaDriverDio_pins0,
   8
};

const ciaaDriverDio_portType ciaaDriverDio_port1 = {
   ciaaDriverDio_pins1,
   8
};

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*==================[interrupt handlers]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
