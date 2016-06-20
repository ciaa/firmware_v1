/* Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
 * Copyright 2016, Alvaro Alonso Bivou <alonso.bivou@gmail.com>
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

#ifndef SIMONSAYS_H
#define SIMONSAYS_H
/** \brief Simon Says game implementation for EDU-CIAA
 **
 ** This is the Simon Says game. The light sequence must be
 ** replicated by the user pressing the buttons.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Game Simon example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * AA         Alvaro Alonso
 */

 /*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160607 v0.0.1   AA   first functional version
 */

/*==================[inclusions]=============================================*/
#include "stdlib.h"			  /* <= standard library */
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "board.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief  RANDOM_SEQUENCE: choose if the sequence must be generated at random.
 **/
#define RANDOM_SEQUENCE true
/** \brief REFRESH_RATE_INPUT_MS: (aka polling cycle) is the time
 ** in milliseconds in which the status of input buttons are
 ** actively sampled (polling).
 **
 ** Range: 50 ~ 100
 **/
#define REFRESH_RATE_INPUT_MS 50

/** \brief REFRESH_RATE_OUTPUT_MS: is the time in milliseconds that
 ** a given led will remain in its state until the next change.
 **
 ** This value could became a variable in further versions
 ** in order to speed up the sequence in higher levels of the game.
 **/
#define REFRESH_RATE_OUTPUT_MS 500

/** \brief  START_DELAY_OUTPUT_MS: is the time in milliseconds for
 ** the output task to start.
 **/
#define START_DELAY_OUTPUT_MS 350

/*==================[typedef]================================================*/
/** \brief Enum of states for the States Machine
 */
typedef enum {IDLE, START, SEQUENCE, LISTEN, INCREASE, FAIL} states;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef SIMONSAYS_H */

