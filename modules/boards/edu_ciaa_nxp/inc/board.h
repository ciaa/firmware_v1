/* Copyright 2016, Esteban Volentini (LabMicro - UNT)
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
/** \brief External Header file of EDU_CIAA_NXP Board
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Boards CIAA Boards
 ** @{ */
/** \addtogroup EDU_CIAA_NXP EDU_CIAA_NXP Board
 ** @{ */

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief input device */
#define DIGITAL_INPUTS_DEVICE    "/dev/dio/gpio/0"

/** \brief output device */
#define DIGITAL_OUTPUT_DEVICE    "/dev/dio/gpio/1"

#define DIGITAL_OUTPUT_1         0x01
#define DIGITAL_OUTPUT_2         0x02
#define DIGITAL_OUTPUT_3         0x04
#define DIGITAL_OUTPUT_4         0x08
#define DIGITAL_OUTPUT_5         0x10
#define DIGITAL_OUTPUT_6         0x20

#define DIGITAL_INPUT_1          0x01
#define DIGITAL_INPUT_2          0x02
#define DIGITAL_INPUT_3          0x04
#define DIGITAL_INPUT_4          0x08

/** \brief Keys device */
#define KEYS                     DIGITAL_INPUTS_DEVICE

/** \brief Leds device */
#define LEDS                     DIGITAL_OUTPUT_DEVICE

/** \brief Red channel of rgb led mask */
#define LED_R                    DIGITAL_OUTPUT_1
/** \brief Green channel of rgb led mask */
#define LED_G                    DIGITAL_OUTPUT_2
/** \brief Blue channel of rgb led mask */
#define LED_B                    DIGITAL_OUTPUT_3
/** \brief Single yellow led mask */
#define LED_1                    DIGITAL_OUTPUT_4
/** \brief Single red led mask */
#define LED_2                    DIGITAL_OUTPUT_5
/** \brief Single green led mask */
#define LED_3                    DIGITAL_OUTPUT_6

#define LED_RGB_RED              LED_R
#define LED_RGB_GREEN            LED_G
#define LED_RGB_BLUE             LED_B
#define LED_YELLOW               LED_1
#define LED_RED                  LED_2
#define LED_GREEN                LED_3

#define KEY_1                    DIGITAL_INPUT_1
#define KEY_2                    DIGITAL_INPUT_2
#define KEY_3                    DIGITAL_INPUT_3
#define KEY_4                    DIGITAL_INPUT_4

#define TEC_1                    KEY_1
#define TEC_2                    KEY_2
#define TEC_3                    KEY_3
#define TEC_4                    KEY_4

#define PULS_0                   KEY_1
#define PULS_1                   KEY_2
#define PULS_2                   KEY_3
#define PULS_3                   KEY_4

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* BOARD_H */
