/* Copyright 2016, 
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

#ifndef _BOARD_H_
#define _BOARD_H_
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
#define ENTRADAS        "/dev/dio/in/0"
#define SALIDAS         "/dev/dio/out/0"
#define SALIDAS_PWM     "/dev/dio/pwm/2"

#if (edu_ciaa_nxp == BOARD)
/** board definition of EDU-CIAA */
#define LED_RGB_ROJO    0x01
#define LED_RGB_VERDE   0x02
#define LED_RGB_AZUL    0x04
#define LED_AMARILLO    0x08
#define LED_ROJO        0x10
#define LED_VERDE       0x20

#define LED_R           LED_RGB_ROJO
#define LED_G           LED_RGB_VERDE
#define LED_B           LED_RGB_AZUL
#define LED_1           LED_AMARILLO
#define LED_2           LED_ROJO
#define LED_3           LED_VERDE

#define TECLA_1         0x01
#define TECLA_2         0x02
#define TECLA_3         0x04
#define TECLA_4         0x08

#define TEC_1           TECLA_1
#define TEC_2           TECLA_2
#define TEC_3           TECLA_3
#define TEC_4           TECLA_4

#define PULS_0          TECLA_1
#define PULS_1          TECLA_2
#define PULS_2          TECLA_3
#define PULS_3          TECLA_4
#endif

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif