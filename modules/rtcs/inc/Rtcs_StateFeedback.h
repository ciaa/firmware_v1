/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2014, Your Name <youremail@domain.com>
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2014, ACSE & CADIEEL & Your Name <youremail@domain.com
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

#ifndef RTCS_STATESPACE_H
#define RTCS_STATESPACE_H
/** \brief State Space Controller Header File
 **
 ** State Space Controller Header File
 **
 ** \file Rtcs_StateSpace.h
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup RTCS RTCS Implementation
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DeV          Diego Ezequiel Vommaro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150722 v0.0.1 DeV  initial version
 */

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief System Control type */
typedef enum {REGULATOR, SERVO, FULL_CONTROL} system_type_t;

/** \brief Luenberger's observer type */
typedef enum {REDUCED, FULL} observer_type_t;

/** \brief State Space Controller type */
typedef struct
{
   uint32_t period_in_ms;
   uint32_t r_size;
   uint32_t x_size;
   uint32_t e_size;
   uint32_t u_size;
   uint32_t y_size;
   float *r_vector;
   float *x_vector;
   float *e_vector;
   float *u_vector;
   float *y_vector;
   float *k_matrix;
   float *ao_matrix;
   float *bo_matrix;
   float *l_matrix;
   void (*ControllerSendFunc) (float *, uint16_t);
   void (*ErrorFunc) (void *data);
   void (*ObserverFunc) (void *data);
   system_type_t system;
   observer_type_t observer;
}Rtcs_statefeedback_data_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief Execution of the control algorithm
 **
 ** Executes control algorithm. It must be called cyclically
 **
 ** \param[in] data structure of the controller
 **/
extern void Rtcs_StateFeedbackRun(void *data);

/** \brief Execution of the control algorithm for the first time
 **
 ** It runs algorithm that performs all mathematical operations of
 ** the controller which do not require to be updated in each iteration.
 **
 ** \param[in] data structure of the controller
 **/
extern void Rtcs_StateFeedbackFirstRun(void *data);

/** \brief Worst-case execution of the control algorithm
 **
 ** Executes the worst case of the control algorithm. It is useful
 ** for measuring computation time of the controller.
 **
 ** \param[in] data structure of the controller
 **/
extern void Rtcs_StateFeedbackWorstRun(void *data);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef RTCS_STATESPACE_H */

