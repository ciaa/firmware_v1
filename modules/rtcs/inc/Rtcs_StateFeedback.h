/* Copyright 2015, Diego Ezequiel Vommaro
 * Copyright 2015, ACSE & CADIEEL
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

/*==================[inclusions]=============================================*/
#include "Rtcs_Port.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief System Control type */
typedef enum {REGULATOR, CONTROL_SYSTEM} system_type_t;

/** \brief Luenberger's observer type */
typedef enum {NONE, REDUCED, FULL} observer_type_t;

/** \brief State Space Controller type */
typedef struct Rtcs_statefeedback_data_type
{
   system_type_t system;
   observer_type_t observer;
   uint32_t period_in_ms;
   uint16_t r_size;
   uint16_t x_size;
   uint16_t e_size;
   uint16_t u_size;
   uint16_t y_size;
   float *r;
   float *x;
   float *xo;
   float *e;
   float *u;
   float *y;
   float *k;
   float *uo;
   float *a_obsvr;
   float *b_obsvr;
   float *l;
   float *mf_obsvr;
   float *mt_obsvr;
   float *xo_aux;
   Rtcs_ext_matrix_t *r_vector;
   Rtcs_ext_matrix_t *x_vector;
   Rtcs_ext_matrix_t *xo_vector;
   Rtcs_ext_matrix_t *e_vector;
   Rtcs_ext_matrix_t *u_vector;
   Rtcs_ext_matrix_t *y_vector;
   Rtcs_ext_matrix_t *k_matrix;
   Rtcs_ext_matrix_t *uo_vector;
   Rtcs_ext_matrix_t *a_obsvr_matrix;
   Rtcs_ext_matrix_t *b_obsvr_matrix;
   Rtcs_ext_matrix_t *l_matrix;
   Rtcs_ext_matrix_t *mf_obsvr_matrix;
   Rtcs_ext_matrix_t *mt_obsvr_matrix;
   Rtcs_ext_matrix_t *xo_aux_vector;
   void (*ControllerSendFunc) (float *, uint16_t);
   void (*ControlEffortFunc) (struct Rtcs_statefeedback_data_type *data);
   void (*ObserverFunc) (struct Rtcs_statefeedback_data_type *data);
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

/** \brief Control efforts calculation for regulator system
 **
 ** Calculates the control efforts for regulator system
 **
 ** \param[in] data structure of the controller
 **/
extern void Rtcs_RegulatorControlEffort (Rtcs_statefeedback_data_t *data);

/** \brief Control efforts calculation for servo or full system
 **
 ** Calculates the contorl efforts for reference tracking contorl system
 **
 ** \param[in] data structure of the controller
 **/
extern void Rtcs_ControlSystemEffort (Rtcs_statefeedback_data_t *data);

/** \brief Full Order State Observer
 **
 ** Estimates complete state vector
 **
 ** \param[in] data structure of the controller
 **/
extern void Rtcs_FullObserver (Rtcs_statefeedback_data_t *data);

/** \brief Reduced Order State Observer
 **
 ** Estimates reduced state vector
 **
 ** \param[in] data structure of the controller
 **/
extern void Rtcs_ReducedObserver (Rtcs_statefeedback_data_t *data);

/** \brief None observer
 **
 ** Copies the measured states from y_vector to x_vector
 **
 ** \param[in] data structure of the controller
 **/
extern void Rtcs_NoneObserver (Rtcs_statefeedback_data_t *data);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef RTCS_STATESPACE_H */

