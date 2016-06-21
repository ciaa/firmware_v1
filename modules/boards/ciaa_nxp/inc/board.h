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
/** \brief External Header file of CIAA_NXP Board
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Boards CIAA Boards
 ** @{ */
/** \addtogroup CIAA_NXP CIAA_NXP Board
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
#define DIGITAL_OUTPUT_7         0x40
#define DIGITAL_OUTPUT_8         0x80

#define DIGITAL_INPUT_1          0x01
#define DIGITAL_INPUT_2          0x02
#define DIGITAL_INPUT_3          0x04
#define DIGITAL_INPUT_4          0x08
#define DIGITAL_INPUT_5          0x10
#define DIGITAL_INPUT_6          0x20
#define DIGITAL_INPUT_7          0x40
#define DIGITAL_INPUT_8          0x80

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
