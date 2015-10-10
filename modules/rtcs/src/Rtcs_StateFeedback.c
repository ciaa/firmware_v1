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

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void Rtcs_StateFeedbackRun(void *data)
{
   /* Storing of the "data" pointer in a correct type pointer */
   Rtcs_statefeedback_data_t *statefeedback_data = (Rtcs_statefeedback_data_t *) data;

   /* Stimating of the state of the dynamic system from measurements of its outputs */
   statefeedback_data->ObserverFunc(statefeedback_data);

   /* Calculating of control efforts */
   statefeedback_data->ControlEffortFunc(statefeedback_data);

   /* Sending of control efforts to the actuators */
   statefeedback_data->ControllerSendFunc(statefeedback_data->u, statefeedback_data->u_size);

   /* Updating of internal data */
   statefeedback_data->DataUpdateFunc(statefeedback_data);
} /* end Rtcs_StateFeedbackRun */

extern void Rtcs_StateFeedbackFirstRun(void *data)
{
}

extern void Rtcs_StateFeedbackWorstRun(void *data)
{
}

extern void RegulatorControlEffort (Rtcs_statefeedback_data_t *data)
{
   /* Calculating of control efforts using K matrix with opposite sign*/
   Rtcs_Ext_MatrixMul_float(&data->k_matrix, &data->x_vector, &data->u_vector);
}

extern void ControlSystemEffort (Rtcs_statefeedback_data_t *data)
{
   /* Calculating of the difference betwen the desired state and the actual state */
   Rtcs_Ext_MatrixSub_float(&data->r_vector, &data->x_vector, &data->e_vector);

   /* Calculating of control efforts */
   Rtcs_Ext_MatrixMul_float(&data->k_matrix, &data->e_vector, &data->u_vector);
}

extern void FullObserver (Rtcs_statefeedback_data_t *data)
{
}

extern void ReducedObserver (Rtcs_statefeedback_data_t *data)
{
}

extern void UpdateObserverData (Rtcs_statefeedback_data_t *data)
{
   Rtcs_Ext_MatrixCpy(&data->xo_vector, &data->x_vector);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
