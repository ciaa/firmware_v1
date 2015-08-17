/* Copyright 2015, ACSE & CADIEEL & Diego Ezequiel Vommaro
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

/** \brief State Feedback Controller
 **
 ** Implements State Feedback Controller
 **
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
 * 20150724 v0.0.1 DeV  initial version
 */

/*==================[inclusions]=============================================*/
#include "Rtcs_StateFeedback.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
/** \brief Error calculation for regulator system
 **
 ** Calculates the error for regulator system
 **
 **/
static void RegulatorError (Rtcs_statefeedback_data_t *data);

/** \brief Error calculation for servo system
 **
 ** Calculates the error for servo system
 **
 **/
static void ServoError (Rtcs_statefeedback_data_t *data);

/** \brief Error calculation for full system
 **
 ** Calculates the error for complete system
 **
 **/
static void FullControlError (Rtcs_statefeedback_data_t *data);

/** \brief Full Order State Observer
 **
 ** Estimates complete state vector
 **
 **/
static void FullObserver (Rtcs_statefeedback_data_t *data);

/** \brief Reduced Order State Observer
 **
 ** Estimates reduced state vector
 **
 **/
static void ReducedObserver (Rtcs_statefeedback_data_t *data);

/** \brief Update observer internal data
 **
 ** Updates observer internal state
 **
 **/
static void UpdateObserverData (Rtcs_statefeedback_data_t *data);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void RegulatorError (Rtcs_statefeedback_data_t *data)
{
}

static void ServoError (Rtcs_statefeedback_data_t *data)
{
}

static void FullControlError (Rtcs_statefeedback_data_t *data)
{
}

static void FullObserver (Rtcs_statefeedback_data_t *data)
{
}

static void ReducedObserver (Rtcs_statefeedback_data_t *data)
{
}

static void UpdateObserverData (Rtcs_statefeedback_data_t *data)
{
}

/*==================[external functions definition]==========================*/
extern void Rtcs_StateFeedbackRun(void *data)
{
   /* Store data pointer in a correct type pointer */
   Rtcs_statefeedback_data_t *statefeedback_data = (Rtcs_statefeedback_data_t *) data;

   /* Stimate the state of the dynamic system from measurements of its outputs */
   statefeedback_data->ObserverFunc(statefeedback_data);

   /* Calculate the difference betwen the desired state and the actual state */
   statefeedback_data->ErrorFunc(statefeedback_data);

   /* Calculate control efforts */
   Rtcs_Ext_MatrixMul_float(&statefeedback_data->e_vector, &statefeedback_data->k_matrix, &statefeedback_data->u_vector);

   /* Send control efforts to the actuators */
   statefeedback_data->ControllerSendFunc(statefeedback_data->u, statefeedback_data->u_size );

   /* Update internal data */
   statefeedback_data->DataUpdateFunc(statefeedback_data);
} /* end Rtcs_StateFeedbackRun */

extern void Rtcs_StateFeedbackFirstRun(void *data)
{
}

extern void Rtcs_StateFeedbackWorstRun(void *data)
{
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
