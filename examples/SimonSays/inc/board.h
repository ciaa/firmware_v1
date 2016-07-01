/* Copyright 2016, Alvaro Alonso Bivou <alonso.bivou@gmail.com>
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

#ifndef BOARD_H
#define BOARD_H
/** \brief board description implementation for EDU-CIAA
 **
 ** This is used on the Simon Says game example.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Game Simon example source file
 ** @{ */


/*==================[inclusions]=============================================*/
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif
/*==================[macros]=================================================*/
/** \brief definition for inputs, outputs and pwm devices for POSIX drivers*/
#define INPUTS        "/dev/dio/in/0"
#define OUTPUTS         "/dev/dio/out/0"

#if (edu_ciaa_nxp == BOARD)
/** board definition of EDU-CIAA */
#define RGB_RED_LED    	0x01
#define RGB_GREEN_LED   0x02
#define RGB_BLUE_LED    0x04
#define YELLOW_LED    	0x08
#define RED_LED        	0x10
#define GREEN_LED       0x20

#define LED_			RGB_BLUE_LED
#define ALL_LED			(RGB_BLUE_LED | YELLOW_LED | RED_LED | GREEN_LED)

#define BUTTON_1		0x01
#define BUTTON_2		0x02
#define BUTTON_3		0x04
#define BUTTON_4		0x08

#define BUTTON_			BUTTON_1
#define ANY_BUTTON		(BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4)

#endif

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef BOARD_H */
