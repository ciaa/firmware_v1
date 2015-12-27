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

/** \brief State Feedback Controller
 **
 ** Implements State Feedback Controller
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup RTCS RTCS Implementation
 ** @{ */

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
} /* end Rtcs_StateFeedbackRun */

extern void Rtcs_StateFeedbackFirstRun(void *data)
{
   uint32_t i;

   /* Storing of the "data" pointer in a correct type pointer */
   Rtcs_statefeedback_data_t *statefeedback_data = (Rtcs_statefeedback_data_t *) data;

   Rtcs_Ext_MatrixInit(statefeedback_data->x_vector, statefeedback_data->x_size , 1, FLOAT_32, statefeedback_data->x);

   Rtcs_Ext_MatrixInit(statefeedback_data->u_vector, statefeedback_data->u_size , 1, FLOAT_32, statefeedback_data->u);

   Rtcs_Ext_MatrixInit(statefeedback_data->y_vector, statefeedback_data->y_size , 1, FLOAT_32, statefeedback_data->y);

   Rtcs_Ext_MatrixInit(statefeedback_data->k_matrix, 1, statefeedback_data->x_size, FLOAT_32, statefeedback_data->k);

   if(statefeedback_data->system == REGULATOR)
   {
      for(i=0; i<statefeedback_data->x_size; i++)
      {
         statefeedback_data->k[i] = statefeedback_data->k[i] * (-1);
      }
   }
   else  /* If the system isn't Regulator type, It's Control System type */
   {
      Rtcs_Ext_MatrixInit(statefeedback_data->r_vector, statefeedback_data->x_size, 1, FLOAT_32, statefeedback_data->r);
      Rtcs_Ext_MatrixInit(statefeedback_data->e_vector, statefeedback_data->x_size, 1, FLOAT_32, statefeedback_data->e);

      for(i=0; i<statefeedback_data->x_size; i++)
      {
         statefeedback_data->r[i] = 0;
      }
   }

   if(statefeedback_data->observer == FULL)
   {
      Rtcs_Ext_MatrixInit(statefeedback_data->xo_vector, statefeedback_data->x_size, 1, FLOAT_32, statefeedback_data->xo);

      Rtcs_Ext_MatrixInit(statefeedback_data->uo_vector, (statefeedback_data->y_size + statefeedback_data->u_size), 1, FLOAT_32, statefeedback_data->uo);

      Rtcs_Ext_MatrixInit(statefeedback_data->mf_obsvr_matrix, statefeedback_data->x_size , statefeedback_data->x_size, FLOAT_32, statefeedback_data->mf_obsvr);

      Rtcs_Ext_MatrixInit(statefeedback_data->mt_obsvr_matrix, statefeedback_data->x_size , (statefeedback_data->u_size + statefeedback_data->y_size), FLOAT_32, statefeedback_data->mt_obsvr);
   }

   else if(statefeedback_data->observer == REDUCED)
   {
      Rtcs_Ext_MatrixInit(statefeedback_data->l_matrix, (statefeedback_data->x_size - statefeedback_data->y_size), statefeedback_data->y_size, FLOAT_32, statefeedback_data->l);

      Rtcs_Ext_MatrixInit(statefeedback_data->xo_vector, (statefeedback_data->x_size - statefeedback_data->y_size), 1, FLOAT_32, statefeedback_data->xo);

      Rtcs_Ext_MatrixInit(statefeedback_data->uo_vector, (statefeedback_data->y_size + statefeedback_data->u_size), 1, FLOAT_32, statefeedback_data->uo);

      Rtcs_Ext_MatrixInit(statefeedback_data->xo_aux_vector, (statefeedback_data->x_size - statefeedback_data->y_size), 1, FLOAT_32, statefeedback_data->xo_aux);

      Rtcs_Ext_MatrixInit(statefeedback_data->mf_obsvr_matrix, (statefeedback_data->x_size - statefeedback_data->y_size), (statefeedback_data->x_size - statefeedback_data->y_size), FLOAT_32, statefeedback_data->mf_obsvr);

      Rtcs_Ext_MatrixInit(statefeedback_data->mt_obsvr_matrix, (statefeedback_data->x_size - statefeedback_data->y_size), (statefeedback_data->u_size + statefeedback_data->y_size), FLOAT_32, statefeedback_data->mt_obsvr);
   }
}

extern void Rtcs_StateFeedbackWorstRun(void *data)
{
   /* Storing of the "data" pointer in a correct type pointer */
   Rtcs_statefeedback_data_t *statefeedback_data = (Rtcs_statefeedback_data_t *) data;

   /* Stimating of the state of the dynamic system from measurements of its outputs */
   statefeedback_data->ObserverFunc(statefeedback_data);

   /* Calculating of control efforts */
   statefeedback_data->ControlEffortFunc(statefeedback_data);

   /* Sending of control efforts to the actuators */
   statefeedback_data->ControllerSendFunc(statefeedback_data->u, statefeedback_data->u_size);
} /* end Rtcs_StateFeedbackWorstRun */

extern void Rtcs_RegulatorControlEffort (Rtcs_statefeedback_data_t *data)
{
   /* Calculating of control efforts using K matrix with opposite sign*/
   Rtcs_Ext_MatrixMul_float(data->k_matrix, data->x_vector, data->u_vector);
}

extern void Rtcs_ControlSystemEffort (Rtcs_statefeedback_data_t *data)
{
   /* Calculating of the difference betwen the desired state and the actual state */
   Rtcs_Ext_MatrixSub_float(data->r_vector, data->x_vector, data->e_vector);

   /* Calculating of control efforts */
   Rtcs_Ext_MatrixMul_float(data->k_matrix, data->e_vector, data->u_vector);
}

extern void Rtcs_FullObserver (Rtcs_statefeedback_data_t *data)
{
   Rtcs_Ext_MatrixMul_float(data->mf_obsvr_matrix, data->x_vector, data->x_vector);
   Rtcs_Ext_MatrixMul_float(data->mt_obsvr_matrix, data->uo_vector, data->xo_vector);
   Rtcs_Ext_MatrixAdd_float(data->x_vector, data->xo_vector, data->x_vector);
}

extern void Rtcs_ReducedObserver (Rtcs_statefeedback_data_t *data)
{
   Rtcs_Ext_MatrixMul_float(data->mf_obsvr_matrix, data->xo_vector, data->xo_vector);
   Rtcs_Ext_MatrixMul_float(data->mt_obsvr_matrix, data->uo_vector, data->xo_aux_vector);
   Rtcs_Ext_MatrixAdd_float(data->xo_vector, data->xo_aux_vector, data->xo_vector);
   Rtcs_Ext_MatrixMul_float(data->l_matrix, data->y_vector, data->xo_aux_vector);
   Rtcs_Ext_MatrixAdd_float(data->xo_vector, data->xo_aux_vector, data->xo_aux_vector);
   Rtcs_Ext_MatrixCat_float(data->y_vector, data->xo_aux_vector, data->x_vector);
}

extern void Rtcs_NoneObserver (Rtcs_statefeedback_data_t *data)
{
   Rtcs_Ext_MatrixCpy_float(data->y_vector, data->x_vector);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
