/* Copyright 2015, Juan Pablo Moreno (UNCa)
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

#ifndef _CIAADRIVERPWM_INTERNAL_H_
#define _CIAADRIVERPWM_INTERNAL_H_
/** \brief Internal Header file of PWM Driver
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup PWM PWM Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * JPM       Juan Pablo Moreno
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150923 v0.0.1 JPM first functional version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief PWM Type */
typedef struct ciaaDriverPwm_portStruct {
   uint8_t port;				/** <= PWM port number */
   uint8_t pin;					/** <= pin number of port */
   uint8_t  function;			/** <= function number */
   uint8_t  ctout;			    /** <= PWM CTOUT */

} ciaaDriverPwm_portType;

typedef struct {
	struct {
		uint32_t frequence;		/** <= PWM frequence */
		uint8_t  index_counter; /** <= PWM counter */
	} config;
} ciaaDriverPwm_pwmType;

/*==================[external data declaration]==============================*/
/** \brief Pwm 0 */
extern ciaaDriverPwm_pwmType ciaaDriverPwm_pwm0;

/** \brief Pwm 1 */
extern ciaaDriverPwm_pwmType ciaaDriverPwm_pwm1;

/** \brief Pwm 2 */
extern ciaaDriverPwm_pwmType ciaaDriverPwm_pwm2;
/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAADRIVERPWM_INTERNAL_H_ */

