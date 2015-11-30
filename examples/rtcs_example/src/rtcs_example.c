/* Copyright 2015,Diego Ezequiel Vommaro
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

/** \brief Real time control system example source file
 **
 ** This is a mini example of the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup RTCS RTCS example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DeV          Diego Ezequiel Vommaro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151101 v0.0.1 Dev  initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "ciaaLibs_Matrix.h"  /* <= matrix operations header */
#include "Rtcs.h"             /* <= real time control system header */
#include "rtcs_example.h"         /* <= own header */

/*==================[macros and definitions]=================================*/
#define INPUT_PLANT_QTY 1    /* <= quantity of plant inputs */
#define OUTPUT_PLANT_QTY 2   /* <= quantity of plant outputs */
#define STATE_VAR_QTY 2      /* <= quantity of plant state variables */
#define FREQ_SAMPLING 1000    /* <= control loop frequency */
#define INACTIVE_POINTS 1     /* <= quantity of points in zero of the input step */
#define ACTIVE_POINTS 100     /* <= quantity of points in one of the input step */
#define TOTAL_POINTS (INACTIVE_POINTS + ACTIVE_POINTS)   /* <= total points of the step */
#define KR 3.1847F            /* <= gain of the reference input */
#define KR2 2.9755F           /* <= gain of the reference input */
#define KR3 2.1834F           /* <= gain of the reference input */

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief File descriptor for digital output ports
 *
 * Device path /dev/dio/out/0
 */
static int32_t fd_out;

/** \brief Fundamental matrix data of an electric plant
 **
 ** RLC circuit
 **/
static float fund_matrix_data[] = { 0.8625F, 258.4922F,
                                   -0.0009F,   0.6748F };
/** \brief Transition matrix data of an electric plant
 **
 ** RLC circuit
 **/
static float tran_matrix_data[] = { 0.1375F,
                                    0.0009F };

/** \brief C matrix data of an electric plant
 **
 ** RLC circuit
 **/
static float c_matrix_data[] = { 1.0F, 0.0F,
                                 0.0F, 1.0F };

/** \brief D matrix data of an electric plant
 **
 ** RLC circuit
 **/
static float d_matrix_data[] = { 0.0F, 0.0F };

/** \brief state vector data of an electric plant
 **
 ** RLC circuit
 **/
static float x_vector_data[] = { 0.0F, 0.0F };

/** \brief auxiliar state vector data of an electric plant
 **
 ** RLC circuit
 **/
static float x_aux_vector_data[] = { 0.0F, 0.0F };

/** \brief output vector data of an electric plant
 **
 ** RLC circuit
 **/
static float y_vector_data[] = { 0.0F, 0.0F };

/** \brief output vector data of an electric plant
 **
 ** RLC circuit
 **/
static float y_aux_vector_data[] = { 0.0F, 0.0F };

/** \brief control efforts vector data of an electric plant
 **
 ** RLC circuit
 **/
static float u_vector_data[] = { 0.0F };

/** \brief Fundamental matrix of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t fund_matrix;

/** \brief Transition matrix of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t tran_matrix;

/** \brief C matrix of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t c_matrix;

/** \brief D matrix of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t d_matrix;

/** \brief state vector of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t x_vector;

/** \brief auxiliar state vector of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t x_aux_vector;

/** \brief output vector of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t y_vector;

/** \brief auxiliar output vector of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t y_aux_vector;

/** \brief control efforts vector of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t u_vector;

/** \brief Input points of the control effort vector
 **
 ** RLC circuit
 **/
static float u_in[TOTAL_POINTS];

/** \brief Input points of the control reference vector
 **
 ** RLC circuit
 **/
static float r_in[TOTAL_POINTS];

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */
int main(void)
{
   /* Starts the operating system in the Application Mode 1 */
   /* This example has only one Application Mode */
   StartOS(AppMode1);

   /* StartOs shall never returns, but to avoid compiler warnings or errors
    * 0 is returned */
   return 0;
}

/** \brief Error Hook function
 *
 * This fucntion is called from the os if an os interface (API) returns an
 * error. Is for debugging proposes. If called this function triggers a
 * ShutdownOs which ends in a while(1).
 *
 * The values:
 *    OSErrorGetServiceId
 *    OSErrorGetParam1
 *    OSErrorGetParam2
 *    OSErrorGetParam3
 *    OSErrorGetRet
 *
 * will provide you the interface, the input parameters and the returned value.
 * For more details see the OSEK specification:
 * http://portal.osek-vdx.org/files/pdf/specs/os223.pdf
 *
 */
void ErrorHook(void)
{
   ciaaPOSIX_printf("ErrorHook was called\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());
   ShutdownOS(0);
}

/** \brief Main task
 *
 * This task is started automatically in the application mode 1.
 * It executes one off-line example for each supported controller
 */
TASK(MainTask)
{
   uint32_t k;

   /* init CIAA kernel and devices */
   ciaak_start();

   /* print message (only on x86) */
   ciaaPOSIX_printf("Main Task...\n");

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", ciaaPOSIX_O_RDWR);

   /*Real tiem control system init*/
   Rtcs_Init();

   /* matrix initialization */
   ciaaLibs_MatrixInit(&fund_matrix, STATE_VAR_QTY, STATE_VAR_QTY, CIAA_LIBS_FLOAT_32, fund_matrix_data);
   ciaaLibs_MatrixInit(&tran_matrix, STATE_VAR_QTY, INPUT_PLANT_QTY, CIAA_LIBS_FLOAT_32, tran_matrix_data);
   ciaaLibs_MatrixInit(&c_matrix, OUTPUT_PLANT_QTY, STATE_VAR_QTY, CIAA_LIBS_FLOAT_32, c_matrix_data);
   ciaaLibs_MatrixInit(&d_matrix, OUTPUT_PLANT_QTY, INPUT_PLANT_QTY, CIAA_LIBS_FLOAT_32, d_matrix_data);
   ciaaLibs_MatrixInit(&x_vector, STATE_VAR_QTY, 1, CIAA_LIBS_FLOAT_32, x_vector_data);
   ciaaLibs_MatrixInit(&x_aux_vector, STATE_VAR_QTY, 1, CIAA_LIBS_FLOAT_32, x_aux_vector_data);
   ciaaLibs_MatrixInit(&y_vector, OUTPUT_PLANT_QTY, 1, CIAA_LIBS_FLOAT_32, y_vector_data);
   ciaaLibs_MatrixInit(&y_aux_vector, OUTPUT_PLANT_QTY, 1, CIAA_LIBS_FLOAT_32, y_aux_vector_data);
   ciaaLibs_MatrixInit(&u_vector, INPUT_PLANT_QTY, 1, CIAA_LIBS_FLOAT_32, u_vector_data);

   /* control efforts vector initialization */
   for(k = 0; k < INACTIVE_POINTS; k++)
   {
      u_in[k] = 0.0F;
   }
   for(k = INACTIVE_POINTS; k < TOTAL_POINTS; k++)
   {
      u_in[k] = 1.0F;
   }

   /* control reference vector initialization */
   for(k = 0; k < INACTIVE_POINTS; k++)
   {
      r_in[k] = 0.0F;
   }
   for(k = INACTIVE_POINTS; k < TOTAL_POINTS; k++)
   {
      r_in[k] = 1.0F * KR;
   }

   /* print message (only on x86) */
   ciaaPOSIX_printf("\nOpen loop without controller:\n");

   /* simulation cycle */
   for(k = 0; k < TOTAL_POINTS; k++)
   {
     *u_vector_data = u_in[k];
     ciaaLibs_MatrixMul_float(&fund_matrix, &x_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&tran_matrix, &u_vector, &x_aux_vector);
     ciaaLibs_MatrixAdd_float(&x_vector, &x_aux_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&c_matrix, &x_vector, &y_vector);
     ciaaLibs_MatrixMul_float(&d_matrix, &u_vector, &y_aux_vector);
     ciaaLibs_MatrixAdd_float(&y_vector, &y_aux_vector, &y_vector);
     ciaaPOSIX_printf("%f %f %d\n", x_vector_data[0], x_vector_data[1], k);
   }

   /* print message (only on x86) */
   ciaaPOSIX_printf("\nClosed loop without observer:\n");

   /* reset of the system's state */
   for(k = 0; k < STATE_VAR_QTY; k++)
   {
      x_vector_data[k]=0.0F;
   }

   /* closed loop simulation cycle */
   for(k = 0; k < TOTAL_POINTS; k++)
   {
     Rtcs_InputRefX1_system_1(&r_in[k]);
     Rtcs_InputY1_system_1(&x_vector_data[0]);
     Rtcs_InputY2_system_1(&x_vector_data[1]);
     Rtcs_system_1_1ms();
     ciaaLibs_MatrixMul_float(&fund_matrix, &x_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&tran_matrix, &u_vector, &x_aux_vector);
     ciaaLibs_MatrixAdd_float(&x_vector, &x_aux_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&c_matrix, &x_vector, &y_vector);
     ciaaLibs_MatrixMul_float(&d_matrix, &u_vector, &y_aux_vector);
     ciaaLibs_MatrixAdd_float(&y_vector, &y_aux_vector, &y_vector);
     ciaaPOSIX_printf("%f %f %d\n", x_vector_data[0], x_vector_data[1], k);
   }

   /* print message (only on x86) */
   ciaaPOSIX_printf("\nClosed loop with observer:\n");

   /* control reference vector initialization */
   for(k = 0; k < INACTIVE_POINTS; k++)
   {
      r_in[k] = 0.0F;
   }
   for(k = INACTIVE_POINTS; k < TOTAL_POINTS; k++)
   {
      r_in[k] = 1.0F * KR2;
   }

   /* reset of the system's state */
   for(k = 0; k < STATE_VAR_QTY; k++)
   {
      x_vector_data[k]=0.0F;
   }

   /* closed loop simulation cycle */
   for(k = 0; k < TOTAL_POINTS; k++)
   {
     Rtcs_InputRefX1_system_2(&r_in[k]);
     Rtcs_InputY1_system_2(&x_vector_data[0]);
     Rtcs_system_2_1ms();
     ciaaLibs_MatrixMul_float(&fund_matrix, &x_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&tran_matrix, &u_vector, &x_aux_vector);
     ciaaLibs_MatrixAdd_float(&x_vector, &x_aux_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&c_matrix, &x_vector, &y_vector);
     ciaaLibs_MatrixMul_float(&d_matrix, &u_vector, &y_aux_vector);
     ciaaLibs_MatrixAdd_float(&y_vector, &y_aux_vector, &y_vector);
     ciaaPOSIX_printf("%f %f %d\n", x_vector_data[0], x_vector_data[1], k);
   }

   /* print message (only on x86) */
   ciaaPOSIX_printf("\nClosed loop with reduced observer:\n");

   /* control reference vector initialization */
   for(k = 0; k < INACTIVE_POINTS; k++)
   {
      r_in[k] = 0.0F;
   }
   for(k = INACTIVE_POINTS; k < TOTAL_POINTS; k++)
   {
      r_in[k] = 1.0F * KR3;
   }

   /* reset of the system's state */
   for(k = 0; k < STATE_VAR_QTY; k++)
   {
      x_vector_data[k]=0.0F;
   }

   /* closed loop simulation cycle */
   for(k = 0; k < TOTAL_POINTS; k++)
   {
     Rtcs_InputRefX1_system_3(&r_in[k]);
     Rtcs_InputY1_system_3(&x_vector_data[0]);
     Rtcs_system_3_1ms();
     ciaaLibs_MatrixMul_float(&fund_matrix, &x_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&tran_matrix, &u_vector, &x_aux_vector);
     ciaaLibs_MatrixAdd_float(&x_vector, &x_aux_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&c_matrix, &x_vector, &y_vector);
     ciaaLibs_MatrixMul_float(&d_matrix, &u_vector, &y_aux_vector);
     ciaaLibs_MatrixAdd_float(&y_vector, &y_aux_vector, &y_vector);
     ciaaPOSIX_printf("%f %f %d\n", x_vector_data[0], x_vector_data[1], k);
   }

   /* print message (only on x86) */
   ciaaPOSIX_printf("\nClosed loop regulator without observer:\n");

   /* set of initial values of the system's state */
   x_vector_data[0] = 1.0F;
   x_vector_data[1] = 0.001F;

   /* closed loop simulation cycle */
   for(k = 0; k < TOTAL_POINTS; k++)
   {
     Rtcs_InputY1_system_4(&x_vector_data[0]);
     Rtcs_InputY2_system_4(&x_vector_data[1]);
     Rtcs_system_4_1ms();
     ciaaLibs_MatrixMul_float(&fund_matrix, &x_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&tran_matrix, &u_vector, &x_aux_vector);
     ciaaLibs_MatrixAdd_float(&x_vector, &x_aux_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&c_matrix, &x_vector, &y_vector);
     ciaaLibs_MatrixMul_float(&d_matrix, &u_vector, &y_aux_vector);
     ciaaLibs_MatrixAdd_float(&y_vector, &y_aux_vector, &y_vector);
     ciaaPOSIX_printf("%f %f %d\n", x_vector_data[0], x_vector_data[1], k);
   }

   /* print message (only on x86) */
   ciaaPOSIX_printf("\nClosed loop regulator with observer:\n");

   /* set of initial values of the system's state */
   x_vector_data[0] = 1.0F;
   x_vector_data[1] = 0.001F;

   /* closed loop simulation cycle */
   for(k = 0; k < TOTAL_POINTS; k++)
   {
     Rtcs_InputY1_system_5(&x_vector_data[0]);
     Rtcs_system_5_1ms();
     ciaaLibs_MatrixMul_float(&fund_matrix, &x_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&tran_matrix, &u_vector, &x_aux_vector);
     ciaaLibs_MatrixAdd_float(&x_vector, &x_aux_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&c_matrix, &x_vector, &y_vector);
     ciaaLibs_MatrixMul_float(&d_matrix, &u_vector, &y_aux_vector);
     ciaaLibs_MatrixAdd_float(&y_vector, &y_aux_vector, &y_vector);
     ciaaPOSIX_printf("%f %f %d\n", x_vector_data[0], x_vector_data[1], k);
   }

   /* print message (only on x86) */
   ciaaPOSIX_printf("\nClosed loop regulator with reduced observer:\n");

   /* reset of the system's state */
   x_vector_data[0] = 1.0F;
   x_vector_data[1] = 0.001F;

   /* closed loop simulation cycle */
   for(k = 0; k < TOTAL_POINTS; k++)
   {
     Rtcs_InputY1_system_6(&x_vector_data[0]);
     Rtcs_system_6_1ms();
     ciaaLibs_MatrixMul_float(&fund_matrix, &x_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&tran_matrix, &u_vector, &x_aux_vector);
     ciaaLibs_MatrixAdd_float(&x_vector, &x_aux_vector, &x_vector);
     ciaaLibs_MatrixMul_float(&c_matrix, &x_vector, &y_vector);
     ciaaLibs_MatrixMul_float(&d_matrix, &u_vector, &y_aux_vector);
     ciaaLibs_MatrixAdd_float(&y_vector, &y_aux_vector, &y_vector);
     ciaaPOSIX_printf("%f %f %d\n", x_vector_data[0], x_vector_data[1], k);
   }
   /* terminate task */
   TerminateTask();
}

/** \brief Send Control Efforts to plant
 *
 * This function sends control efforts to plant.
 */
extern void SendControlEffort_1 (float *data, uint16_t num_elements)
{
   float *u_vector_data_ptr = u_vector_data;

   while(num_elements > 0u)
   {
     *u_vector_data_ptr++ = *data++;
     num_elements--;
   }
}

/** \brief Send Control Efforts to plant
 *
 * This function sends control efforts to plant.
 */
extern void SendControlEffort_2 (float *data, uint16_t num_elements)
{
   float *u_vector_data_ptr = u_vector_data;

   while(num_elements > 0u)
   {
     *u_vector_data_ptr++ = *data++;
     num_elements--;
   }
}

/** \brief Send Control Efforts to plant
 *
 * This function sends control efforts to plant.
 */
extern void SendControlEffort_3 (float *data, uint16_t num_elements)
{
   float *u_vector_data_ptr = u_vector_data;

   while(num_elements > 0u)
   {
     *u_vector_data_ptr++ = *data++;
     num_elements--;
   }
}

/** \brief Send Control Efforts to plant
 *
 * This function sends control efforts to plant.
 */
extern void SendControlEffort_4 (float *data, uint16_t num_elements)
{
   float *u_vector_data_ptr = u_vector_data;

   while(num_elements > 0u)
   {
     *u_vector_data_ptr++ = *data++;
     num_elements--;
   }
}

/** \brief Send Control Efforts to plant
 *
 * This function sends control efforts to plant.
 */
extern void SendControlEffort_5 (float *data, uint16_t num_elements)
{
   float *u_vector_data_ptr = u_vector_data;

   while(num_elements > 0u)
   {
     *u_vector_data_ptr++ = *data++;
     num_elements--;
   }
}

/** \brief Send Control Efforts to plant
 *
 * This function sends control efforts to plant.
 */
extern void SendControlEffort_6 (float *data, uint16_t num_elements)
{
   float *u_vector_data_ptr = u_vector_data;

   while(num_elements > 0u)
   {
     *u_vector_data_ptr++ = *data++;
     num_elements--;
   }
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

