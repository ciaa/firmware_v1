/* Copyright 2015, Diego Ezequiel Vommaro
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

/** \brief This file implements the test of the StateFeedback Module
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup RTCS RTCS Unit Test
 ** @{ */
/** \addtogroup UnitTests Unit Tests
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DeV          Diego Ezequiel Vommaro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151013 v0.0.1 DeV  initial version
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "Rtcs_StateFeedback.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static void DoNothing1 (float *data, uint16_t size);
static void DoNothing2 (Rtcs_statefeedback_data_t *data);
static void DoThing1 (ciaaLibs_matrix_t *mat, uint16_t n_rows, uint16_t n_columns, data_type type, void *data);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
/* Servo Control System. Without Observer. Two state variables */
float data_array_1[2+2+2+1+2+2];
/* Six matrices are required */
Rtcs_ext_matrix_t matrix_array_1[6];
/* Load data structure */
Rtcs_statefeedback_data_t controller_1 = {CONTROL_SYSTEM, NONE, 0, 2, 2, 2, 1, 2, &(data_array_1[0]), &(data_array_1[2]), 0, &(data_array_1[4]), &(data_array_1[6]), &(data_array_1[7]), &(data_array_1[9]), 0, 0, 0, 0, 0, 0, 0, &matrix_array_1[0], &matrix_array_1[1], 0, &matrix_array_1[2], &matrix_array_1[3], &matrix_array_1[4], &matrix_array_1[5], 0, 0, 0, 0, 0, 0, 0, DoNothing1, ControlSystemEffort, NoneObserver};

/* Servo Control System. Full Observer. Three state variables */
float data_array_2[3+3+3+3+1+1+3+(0)+(3*3)+(3*(1+1))];
/* Ten matrices are required */
Rtcs_ext_matrix_t matrix_array_2[10];
/* Load data structure */
Rtcs_statefeedback_data_t controller_2 = {CONTROL_SYSTEM, FULL, 0, 3, 3, 3, 1, 1, &(data_array_2[0]), &(data_array_2[3]), &(data_array_2[6]), &(data_array_2[9]), &(data_array_2[12]), &(data_array_2[13]), &(data_array_2[14]), &(data_array_2[12]), 0, 0, 0, &(data_array_2[17]), &(data_array_2[26]), 0, &matrix_array_2[0], &matrix_array_2[1], &matrix_array_2[2], &matrix_array_2[3], &matrix_array_2[4], &matrix_array_2[5], &matrix_array_2[6], &matrix_array_2[7], 0, 0, 0, &matrix_array_2[8], &matrix_array_2[9], 0, DoNothing1, ControlSystemEffort, FullObserver};
/*==================[internal functions definition]==========================*/
static void DoNothing1 (float *data, uint16_t size)
{
}

static void DoNothing2 (Rtcs_statefeedback_data_t *data)
{
}

static void DoThing1 (ciaaLibs_matrix_t *mat, uint16_t n_rows, uint16_t n_columns, data_type type, void *data)
{
   mat->n_rows = n_rows;
   mat->n_columns = n_columns;
   mat->type = type;
   mat->data = data;
}

/*==================[external functions definition]==========================*/
/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void) {
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void) {
}

/** \brief test Rtcs_StateFeedback
 **
 ** Correct first run with Servo Control System, with two state variables and without observer
 **
 */
void test_Rtcs_StateFeedbackFirstRun_01(void)
{
   ciaaLibs_MatrixInit_StubWithCallback(DoThing1);  

   Rtcs_StateFeedbackFirstRun(&controller_1);

   TEST_ASSERT_EQUAL_UINT16(2, (controller_1.r_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.r_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_1.x_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.x_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_1.e_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.e_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.u_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.u_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_1.y_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.y_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.k_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_1.k_matrix)->n_columns);
   TEST_ASSERT_EQUAL_UINT32(DoNothing1, controller_1.ControllerSendFunc);
   TEST_ASSERT_EQUAL_UINT32(ControlSystemEffort, controller_1.ControlEffortFunc);
   TEST_ASSERT_EQUAL_UINT32(NoneObserver, controller_1.ObserverFunc);
}

/** \brief test Rtcs_StateFeedback
 **
 ** Correct first run with Servo Control System, with three state variables and full observer
 **
 */
void test_Rtcs_StateFeedbackFirstRun_02(void)
{
   ciaaLibs_MatrixInit_StubWithCallback(DoThing1);  

   Rtcs_StateFeedbackFirstRun(&controller_2);

   TEST_ASSERT_EQUAL_UINT32(&(data_array_2[0]), (controller_2.r_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.r_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.r_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.x_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.x_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.e_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.e_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.u_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.u_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.y_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.y_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.k_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.k_matrix)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.xo_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.xo_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_2.uo_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.uo_vector)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.mf_obsvr_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.mf_obsvr_matrix)->n_columns);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.mt_obsvr_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_2.mt_obsvr_matrix)->n_columns);
   TEST_ASSERT_EQUAL_UINT32(DoNothing1, controller_2.ControllerSendFunc);
   TEST_ASSERT_EQUAL_UINT32(ControlSystemEffort, controller_2.ControlEffortFunc);
   TEST_ASSERT_EQUAL_UINT32(FullObserver, controller_2.ObserverFunc);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

