/* Copyright 2015, Diego Ezequiel Vommaro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
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

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "Rtcs_StateFeedback.h"

/*==================[macros and definitions]=================================*/
#define RESET_VALUE_1 1
#define RESET_VALUE_2 1
#define RESET_VALUE_3 1
#define RESET_VALUE_4 1
#define DATA_SIZE_1 (2+2+2+1+2+2)
#define DATA_SIZE_2 (3+3+3+3+1+1+3+(0)+(3*3)+(3*(1+1)))
#define DATA_SIZE_3 (3+3+2+3+1+1+3+((3-1)*1)+((3-1)*(3-1))+((3-1)*(1+1))+(3-1))
#define DATA_SIZE_4 (0+2+0+0+1+2+2)

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static void DoNothing1 (float *data, uint16_t size);
static void ciaaLibs_MatrixInit_stub (ciaaLibs_matrix_t *mat, uint16_t n_rows, uint16_t n_columns, ciaaLibs_matrix_data_t type, void *data);
static void ciaaLibs_MatrixAdd_float_stub (ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);
static void ciaaLibs_MatrixSub_float_stub (ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);
static void ciaaLibs_MatrixMul_float_stub1 (ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);
static void * ciaaPOSIX_memcpy_stub (void *dst, void *src, size_t n);
static void ciaaLibs_MatrixCat_float_stub (ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
/* Servo Control System. Without Observer. Two state variables */
float data_array_1[DATA_SIZE_1];
/* Six matrices are required */
Rtcs_ext_matrix_t matrix_array_1[6];
/* Load data structure */
Rtcs_statefeedback_data_t controller_1 = {CONTROL_SYSTEM, NONE, 0, 2, 2, 2, 1, 2, &(data_array_1[0]), &(data_array_1[2]), 0, &(data_array_1[4]), &(data_array_1[6]), &(data_array_1[7]), &(data_array_1[9]), 0, 0, 0, 0, 0, 0, 0, &matrix_array_1[0], &matrix_array_1[1], 0, &matrix_array_1[2], &matrix_array_1[3], &matrix_array_1[4], &matrix_array_1[5], 0, 0, 0, 0, 0, 0, 0, DoNothing1, Rtcs_ControlSystemEffort, Rtcs_NoneObserver};

/* Servo Control System. Full Observer. Three state variables */
float data_array_2[DATA_SIZE_2];
/* Ten matrices are required */
Rtcs_ext_matrix_t matrix_array_2[10];
/* Load data structure */
Rtcs_statefeedback_data_t controller_2 = {CONTROL_SYSTEM, FULL, 0, 3, 3, 3, 1, 1, &(data_array_2[0]), &(data_array_2[3]), &(data_array_2[6]), &(data_array_2[9]), &(data_array_2[12]), &(data_array_2[13]), &(data_array_2[14]), &(data_array_2[12]), 0, 0, 0, &(data_array_2[17]), &(data_array_2[26]), 0, &matrix_array_2[0], &matrix_array_2[1], &matrix_array_2[2], &matrix_array_2[3], &matrix_array_2[4], &matrix_array_2[5], &matrix_array_2[6], &matrix_array_2[7], 0, 0, 0, &matrix_array_2[8], &matrix_array_2[9], 0, DoNothing1, Rtcs_ControlSystemEffort, Rtcs_FullObserver};

/* Servo Control System. Reduced Observer. Three state variables */
float data_array_3[DATA_SIZE_3];
/* Ten matrices are required */
Rtcs_ext_matrix_t matrix_array_3[12];
/* Load data structure */
Rtcs_statefeedback_data_t controller_3 = {CONTROL_SYSTEM, REDUCED, 0, 3, 3, 3, 1, 1, &(data_array_3[0]), &(data_array_3[3]), &(data_array_3[6]), &(data_array_3[9]), &(data_array_3[12]), &(data_array_3[13]), &(data_array_3[14]), &(data_array_3[12]), 0, 0, &(data_array_3[17]), &(data_array_3[19]), &(data_array_3[23]), &(data_array_3[27]), &matrix_array_3[0], &matrix_array_3[1], &matrix_array_3[2], &matrix_array_3[3], &matrix_array_3[4], &matrix_array_3[5], &matrix_array_3[6], &matrix_array_3[7], 0, 0, &matrix_array_3[8], &matrix_array_3[9], &matrix_array_3[10], &matrix_array_3[11], DoNothing1, Rtcs_ControlSystemEffort, Rtcs_ReducedObserver};

/* Regulator System. Without Observer. Two state variables */
float data_array_4[DATA_SIZE_4];
/* Six matrices are required */
Rtcs_ext_matrix_t matrix_array_4[4];
/* Load data structure */
Rtcs_statefeedback_data_t controller_4 = {REGULATOR, NONE, 0, 0, 2, 0, 1, 2, 0, &(data_array_4[0]), 0, 0, &(data_array_4[2]), &(data_array_4[3]), &(data_array_4[5]), 0, 0, 0, 0, 0, 0, 0, 0, &matrix_array_4[0], 0, 0, &matrix_array_4[1], &matrix_array_4[2], &matrix_array_4[3], 0, 0, 0, 0, 0, 0, 0, DoNothing1, Rtcs_RegulatorControlEffort, Rtcs_NoneObserver};
/*==================[internal functions definition]==========================*/
static void DoNothing1 (float *data, uint16_t size)
{
}

static void ciaaLibs_MatrixInit_stub (ciaaLibs_matrix_t *mat, uint16_t n_rows, uint16_t n_columns, ciaaLibs_matrix_data_t type, void *data)
{
   mat->n_rows = n_rows;
   mat->n_columns = n_columns;
   mat->type = type;
   mat->data = data;
}

static void ciaaLibs_MatrixAdd_float_stub (ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
   float *src1_ptr = src1->data;
   float *src2_ptr = src2->data;
   float *dst_ptr = dst->data;
   uint32_t num_elements;

   num_elements = src1->n_rows * src1->n_columns;

   while(num_elements > 0u)
   {
      *dst_ptr++ = (*src1_ptr++) + (*src2_ptr++);
      num_elements--;
   }
}

static void ciaaLibs_MatrixSub_float_stub (ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
   float *src1_ptr = src1->data;
   float *src2_ptr = src2->data;
   float *dst_ptr = dst->data;
   uint32_t num_elements;

   num_elements = src1->n_rows * src1->n_columns;

   while(num_elements > 0u)
   {
      *dst_ptr++ = (*src1_ptr++) - (*src2_ptr++);
      num_elements--;
   }
}

static void ciaaLibs_MatrixMul_float_stub1 (ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
   float *src1_ptr = src1->data;
   float *src2_ptr = src2->data;
   float *dst_ptr = dst->data;
   float acc = 0;
   uint32_t num_elements;

   num_elements = src1->n_rows * src1->n_columns;

   while(num_elements > 0u)
   {
      acc = (*src1_ptr++) + acc;
      num_elements--;
   }

   num_elements = src2->n_rows * src2->n_columns;

   while(num_elements > 0u)
   {
      acc = (*src2_ptr++) + acc;
      num_elements--;
   }

   num_elements = dst->n_rows * dst->n_columns;

   while(num_elements > 0u)
   {
      (*dst_ptr++) =  acc;
      num_elements--;
   }
}

static void * ciaaPOSIX_memcpy_stub (void *dst, void *src, size_t n)
{
   while(0 < n)
   {
      /* decrement counter */
      n--;

      /* copy 1 byte */
      ((uint8_t*)dst)[n] = ((uint8_t*)src)[n];
   }

   return dst;
}

static void ciaaLibs_MatrixCat_float_stub (ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
   uint32_t num_elements = sizeof(float) * src1->n_rows * src1->n_columns;
   uint32_t num_elements_2 = sizeof(float) * src2->n_rows * src2->n_columns;

   /* Copied of data from first matrix to destination matrix */
   ciaaPOSIX_memcpy((void *) dst->data, (void *) src1->data, num_elements);

   /* Copied of data from second matrix to destination matrix*/
   ciaaPOSIX_memcpy(((void *) dst->data) + num_elements, (void *) src2->data, num_elements_2);
}

/*==================[external functions definition]==========================*/
/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void) {
   uint32_t i;

   for(i=0; i < DATA_SIZE_1; i++)
   {
      data_array_1[i] = RESET_VALUE_1 + i;
   }

   for(i=0; i < DATA_SIZE_2; i++)
   {
      data_array_2[i] = RESET_VALUE_2 + i;
   }

   for(i=0; i < DATA_SIZE_3; i++)
   {
      data_array_3[i] = RESET_VALUE_3 + i;
   }

   for(i=0; i < DATA_SIZE_4; i++)
   {
      data_array_4[i] = RESET_VALUE_4 + i;
   }
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void) {
}

/** \brief test Rtcs_StateFeedbackFirstRun
 **
 ** Correct first run with Servo Control System, with two state variables and without observer
 **
 */
void test_Rtcs_StateFeedbackFirstRun_01(void)
{
   /* Setting of behavior to ciaaLibs_MatrixInit() */
   ciaaLibs_MatrixInit_StubWithCallback(ciaaLibs_MatrixInit_stub);

   /* Configuration of controller data */
   Rtcs_StateFeedbackFirstRun(&controller_1);

   /* ASSERTs for data controller */
   TEST_ASSERT_EQUAL_UINT32(CONTROL_SYSTEM, controller_1.system);
   TEST_ASSERT_EQUAL_UINT32(NONE, controller_1.observer);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_1[0]), (intptr_t)controller_1.r_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_1[0]), (intptr_t)(controller_1.r_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_1.r_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.r_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_1[1]), (intptr_t)controller_1.x_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_1[2]), (intptr_t)(controller_1.x_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_1.x_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.x_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_1[2]), (intptr_t)controller_1.e_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_1[4]), (intptr_t)(controller_1.e_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_1.e_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.e_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_1[3]), (intptr_t)controller_1.u_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_1[6]), (intptr_t)(controller_1.u_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.u_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.u_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_1[4]), (intptr_t)controller_1.y_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_1[7]), (intptr_t)(controller_1.y_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_1.y_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.y_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_1[5]), (intptr_t)controller_1.k_matrix);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_1[9]), (intptr_t)(controller_1.k_matrix)->data);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_1.k_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_1.k_matrix)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)DoNothing1, (intptr_t)controller_1.ControllerSendFunc);
   TEST_ASSERT_EQUAL_PTR((intptr_t)Rtcs_ControlSystemEffort, (intptr_t)controller_1.ControlEffortFunc);
   TEST_ASSERT_EQUAL_PTR((intptr_t)Rtcs_NoneObserver, (intptr_t)controller_1.ObserverFunc);
}

/** \brief test Rtcs_StateFeedbackFirstRun
 **
 ** Correct first run with Servo Control System, with three state variables and full observer
 **
 */
void test_Rtcs_StateFeedbackFirstRun_02(void)
{
   /* Setting of behavior to ciaaLibs_MatrixInit() */
   ciaaLibs_MatrixInit_StubWithCallback(ciaaLibs_MatrixInit_stub);

   /* Configuration of controller data */
   Rtcs_StateFeedbackFirstRun(&controller_2);

   /* ASSERTs for data controller */
   TEST_ASSERT_EQUAL_UINT32(CONTROL_SYSTEM, controller_2.system);
   TEST_ASSERT_EQUAL_UINT32(FULL, controller_2.observer);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_2[0]), (intptr_t)controller_2.r_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_2[0]), (intptr_t)(controller_2.r_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.r_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.r_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_2[1]), (intptr_t)controller_2.x_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_2[3]), (intptr_t)(controller_2.x_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.x_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.x_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_2[3]), (intptr_t)controller_2.e_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_2[9]), (intptr_t)(controller_2.e_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.e_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.e_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_2[4]), (intptr_t)controller_2.u_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_2[12]), (intptr_t)(controller_2.u_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.u_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.u_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_2[5]), (intptr_t)controller_2.y_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_2[13]), (intptr_t)(controller_2.y_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.y_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.y_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_2[6]), (intptr_t)controller_2.k_matrix);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_2[14]), (intptr_t)(controller_2.k_matrix)->data);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.k_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.k_matrix)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_2[2]), (intptr_t)controller_2.xo_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_2[6]), (intptr_t)(controller_2.xo_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.xo_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.xo_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_2[7]), (intptr_t)controller_2.uo_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_2[12]), (intptr_t)(controller_2.uo_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_2.uo_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_2.uo_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_2[8]), (intptr_t)controller_2.mf_obsvr_matrix);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_2[17]), (intptr_t)(controller_2.mf_obsvr_matrix)->data);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.mf_obsvr_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.mf_obsvr_matrix)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_2[9]), (intptr_t)controller_2.mt_obsvr_matrix);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_2[26]), (intptr_t)(controller_2.mt_obsvr_matrix)->data);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_2.mt_obsvr_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_2.mt_obsvr_matrix)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)DoNothing1, (intptr_t)controller_2.ControllerSendFunc);
   TEST_ASSERT_EQUAL_PTR((intptr_t)Rtcs_ControlSystemEffort, (intptr_t)controller_2.ControlEffortFunc);
   TEST_ASSERT_EQUAL_PTR((intptr_t)Rtcs_FullObserver, (intptr_t)controller_2.ObserverFunc);
}

/** \brief test Rtcs_StateFeedbackFirstRun
 **
 ** Correct first run with Servo Control System, with three state variables and reduced observer
 **
 */
void test_Rtcs_StateFeedbackFirstRun_03(void)
{
   /* Setting of behavior to ciaaLibs_MatrixInit() */
   ciaaLibs_MatrixInit_StubWithCallback(ciaaLibs_MatrixInit_stub);

   /* Configuration of controller data */
   Rtcs_StateFeedbackFirstRun(&controller_3);

   /* ASSERTs for data controller */
   TEST_ASSERT_EQUAL_UINT32(CONTROL_SYSTEM, controller_3.system);
   TEST_ASSERT_EQUAL_UINT32(REDUCED, controller_3.observer);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[0]), (intptr_t)controller_3.r_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[0]), (intptr_t)(controller_3.r_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_3.r_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.r_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[1]), (intptr_t)controller_3.x_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[3]), (intptr_t)(controller_3.x_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_3.x_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.x_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[3]), (intptr_t)controller_3.e_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[9]), (intptr_t)(controller_3.e_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_3.e_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.e_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[4]), (intptr_t)controller_3.u_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[12]), (intptr_t)(controller_3.u_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.u_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.u_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[5]), (intptr_t)controller_3.y_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[13]), (intptr_t)(controller_3.y_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.y_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.y_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[6]), (intptr_t)controller_3.k_matrix);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[14]), (intptr_t)(controller_3.k_matrix)->data);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.k_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(3, (controller_3.k_matrix)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[2]), (intptr_t)controller_3.xo_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[6]), (intptr_t)(controller_3.xo_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_3.xo_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.xo_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[7]), (intptr_t)controller_3.uo_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[12]), (intptr_t)(controller_3.uo_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_3.uo_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.uo_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[9]), (intptr_t)controller_3.mf_obsvr_matrix);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[19]), (intptr_t)(controller_3.mf_obsvr_matrix)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_3.mf_obsvr_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_3.mf_obsvr_matrix)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[10]), (intptr_t)controller_3.mt_obsvr_matrix);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[23]), (intptr_t)(controller_3.mt_obsvr_matrix)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_3.mt_obsvr_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_3.mt_obsvr_matrix)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[8]), (intptr_t)controller_3.l_matrix);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[17]), (intptr_t)(controller_3.l_matrix)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_3.l_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.l_matrix)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_3[11]), (intptr_t)controller_3.xo_aux_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_3[27]), (intptr_t)(controller_3.xo_aux_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_3.xo_aux_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_3.xo_aux_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)DoNothing1, (intptr_t)controller_3.ControllerSendFunc);
   TEST_ASSERT_EQUAL_PTR((intptr_t)Rtcs_ControlSystemEffort, (intptr_t)controller_3.ControlEffortFunc);
   TEST_ASSERT_EQUAL_PTR((intptr_t)Rtcs_ReducedObserver, (intptr_t)controller_3.ObserverFunc);
}

/** \brief test Rtcs_StateFeedbackFirstRun
 **
 ** Correct first run with Regulator System, with two state variables and without observer
 **
 */
void test_Rtcs_StateFeedbackFirstRun_04(void)
{
   /* Setting of behavior to ciaaLibs_MatrixInit() */
   ciaaLibs_MatrixInit_StubWithCallback(ciaaLibs_MatrixInit_stub);

   /* Configuration of controller data */
   Rtcs_StateFeedbackFirstRun(&controller_4);

   /* ASSERTs for data controller */
   TEST_ASSERT_EQUAL_UINT32(REGULATOR, controller_4.system);
   TEST_ASSERT_EQUAL_UINT32(NONE, controller_4.observer);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_4[0]), (intptr_t)controller_4.x_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_4[0]), (intptr_t)(controller_4.x_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_4.x_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_4.x_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_4[1]), (intptr_t)controller_4.u_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_4[2]), (intptr_t)(controller_4.u_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_4.u_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_4.u_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_4[2]), (intptr_t)controller_4.y_vector);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_4[3]), (intptr_t)(controller_4.y_vector)->data);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_4.y_vector)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_4.y_vector)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(matrix_array_4[3]), (intptr_t)controller_4.k_matrix);
   TEST_ASSERT_EQUAL_PTR((intptr_t)&(data_array_4[5]), (intptr_t)(controller_4.k_matrix)->data);
   TEST_ASSERT_EQUAL_UINT16(1, (controller_4.k_matrix)->n_rows);
   TEST_ASSERT_EQUAL_UINT16(2, (controller_4.k_matrix)->n_columns);
   TEST_ASSERT_EQUAL_PTR((intptr_t)DoNothing1, (intptr_t)controller_4.ControllerSendFunc);
   TEST_ASSERT_EQUAL_PTR((intptr_t)Rtcs_RegulatorControlEffort, (intptr_t)controller_4.ControlEffortFunc);
   TEST_ASSERT_EQUAL_PTR((intptr_t)Rtcs_NoneObserver, (intptr_t)controller_4.ObserverFunc);
}

/** \brief test Rtcs_StateFeedbackRun
 **
 ** Correct run with Servo Control System, with two state variables and without observer
 **
 */
void test_Rtcs_StateFeedbackRun_01(void)
{
   float expected_e_vector[] = {-8, -9};
   float expected_u_vector[] = {4};

   /* Setting of behavior to ciaaLibs_MatrixInit() */
   ciaaLibs_MatrixInit_StubWithCallback(ciaaLibs_MatrixInit_stub);

   /* Configuration of controller data */
   Rtcs_StateFeedbackFirstRun(&controller_1);

   /* Setting of behavior to matrix operation functions */
   ciaaLibs_MatrixAdd_float_StubWithCallback(ciaaLibs_MatrixAdd_float_stub);
   ciaaLibs_MatrixSub_float_StubWithCallback(ciaaLibs_MatrixSub_float_stub);
   ciaaLibs_MatrixMul_float_StubWithCallback(ciaaLibs_MatrixMul_float_stub1);
   ciaaPOSIX_memcpy_StubWithCallback(ciaaPOSIX_memcpy_stub);
   ciaaLibs_MatrixCat_float_StubWithCallback(ciaaLibs_MatrixCat_float_stub);

   /* Contorller_1 execution using the state feedback strategy */
   Rtcs_StateFeedbackRun(&controller_1);

   /* ASSERTs for data controller after executing of the controller */
   TEST_ASSERT_EQUAL_FLOAT_ARRAY((float *)(controller_1.y_vector)->data, (float *)(controller_1.x_vector)->data, controller_1.x_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_e_vector, (float *)(controller_1.e_vector)->data, controller_1.x_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_u_vector, (float *)(controller_1.u_vector)->data, controller_1.u_size);
}

/** \brief test Rtcs_StateFeedbackRun
 **
 ** Correct run with Servo Control System, with three state variables and full observer
 **
 */
void test_Rtcs_StateFeedbackRun_02(void)
{
   float expected_xo_vector[] = {204, 204, 204};
   float expected_x_vector[] = {417, 417, 417};
   float expected_e_vector[] = {-417, -417, -417};
   float expected_u_vector[] = {-1203};

   /* Setting of behavior to ciaaLibs_MatrixInit() */
   ciaaLibs_MatrixInit_StubWithCallback(ciaaLibs_MatrixInit_stub);

   /* Configuration of controller data */
   Rtcs_StateFeedbackFirstRun(&controller_2);

   /* Setting of behavior to matrix operation functions */
   ciaaLibs_MatrixAdd_float_StubWithCallback(ciaaLibs_MatrixAdd_float_stub);
   ciaaLibs_MatrixSub_float_StubWithCallback(ciaaLibs_MatrixSub_float_stub);
   ciaaLibs_MatrixMul_float_StubWithCallback(ciaaLibs_MatrixMul_float_stub1);
   ciaaPOSIX_memcpy_StubWithCallback(ciaaPOSIX_memcpy_stub);
   ciaaLibs_MatrixCat_float_StubWithCallback(ciaaLibs_MatrixCat_float_stub);

   /* Contorller_2 execution using the state feedback strategy */
   Rtcs_StateFeedbackRun(&controller_2);

   /* ASSERTs for data controller after executing of the controller */
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_xo_vector, (float *)(controller_2.xo_vector)->data, controller_2.x_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_x_vector, (float *)(controller_2.x_vector)->data, controller_2.x_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_e_vector, (float *)(controller_2.e_vector)->data, controller_2.x_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_u_vector, (float *)(controller_2.u_vector)->data, controller_2.u_size);
}

/** \brief test Rtcs_StateFeedbackRun
 **
 ** Correct run with Servo Control System, with three state variables and reduced observer
 **
 */
void test_Rtcs_StateFeedbackRun_03(void)
{
   float expected_xo_vector[] = {230, 230};
   float expected_xo_aux_vector[] = {281, 281};
   float expected_x_vector[] = {14, 281, 281};
   float expected_e_vector[] = {-14, -281, -281};
   float expected_u_vector[] = {-528};

   /* Setting of behavior to ciaaLibs_MatrixInit() */
   ciaaLibs_MatrixInit_StubWithCallback(ciaaLibs_MatrixInit_stub);

   /* Configuration of controller data */
   Rtcs_StateFeedbackFirstRun(&controller_3);

   /* Setting of behavior to matrix operation functions */
   ciaaLibs_MatrixAdd_float_StubWithCallback(ciaaLibs_MatrixAdd_float_stub);
   ciaaLibs_MatrixSub_float_StubWithCallback(ciaaLibs_MatrixSub_float_stub);
   ciaaLibs_MatrixMul_float_StubWithCallback(ciaaLibs_MatrixMul_float_stub1);
   ciaaPOSIX_memcpy_StubWithCallback(ciaaPOSIX_memcpy_stub);
   ciaaLibs_MatrixCat_float_StubWithCallback(ciaaLibs_MatrixCat_float_stub);

   /* Contorller_3 execution using the state feedback strategy */
   Rtcs_StateFeedbackRun(&controller_3);

   /* ASSERTs for data controller after executing of the controller */
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_xo_vector, (float *)(controller_3.xo_vector)->data, controller_3.x_size - controller_3.y_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_xo_aux_vector, (float *)(controller_3.xo_aux_vector)->data, controller_3.x_size - controller_3.y_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_x_vector, (float *)(controller_3.x_vector)->data, controller_3.x_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_e_vector, (float *)(controller_3.e_vector)->data, controller_3.x_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_u_vector, (float *)(controller_3.u_vector)->data, controller_3.u_size);
}

/** \brief test Rtcs_StateFeedbackRun
 **
 ** Correct run with Regulator System, with two state variables and without observer
 **
 */
void test_Rtcs_StateFeedbackRun_04(void)
{
   float expected_x_vector[] = {4, 5};
   float expected_k_matrix[] = {-6, -7};
   float expected_u_vector[] = {-4};

   /* Setting of behavior to ciaaLibs_MatrixInit() */
   ciaaLibs_MatrixInit_StubWithCallback(ciaaLibs_MatrixInit_stub);

   /* Configuration of controller data */
   Rtcs_StateFeedbackFirstRun(&controller_4);

   ciaaLibs_MatrixAdd_float_StubWithCallback(ciaaLibs_MatrixAdd_float_stub);
   ciaaLibs_MatrixSub_float_StubWithCallback(ciaaLibs_MatrixSub_float_stub);
   ciaaLibs_MatrixMul_float_StubWithCallback(ciaaLibs_MatrixMul_float_stub1);
   ciaaPOSIX_memcpy_StubWithCallback(ciaaPOSIX_memcpy_stub);
   ciaaLibs_MatrixCat_float_StubWithCallback(ciaaLibs_MatrixCat_float_stub);

   /* Contorller_4 execution using the state feedback strategy */
   Rtcs_StateFeedbackRun(&controller_4);

   /* ASSERTs for data controller after executing of the controller */
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_x_vector, (float *)(controller_4.x_vector)->data, controller_4.x_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_k_matrix, (float *)(controller_4.k_matrix)->data, controller_4.x_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_u_vector, (float *)(controller_4.u_vector)->data, controller_4.u_size);
}

/** \brief test Rtcs_StateFeedbackWorstRun
 **
 ** Correct worst run with Servo Control System, with two state variables and without observer
 **
 */
void test_Rtcs_StateFeedbackWorstRun_01(void)
{
   float expected_e_vector[] = {-8, -9};
   float expected_u_vector[] = {4};

   /* Setting of behavior to ciaaLibs_MatrixInit() */
   ciaaLibs_MatrixInit_StubWithCallback(ciaaLibs_MatrixInit_stub);

   /* Configuration of controller data */
   Rtcs_StateFeedbackFirstRun(&controller_1);

   /* Setting of behavior to matrix operation functions */
   ciaaLibs_MatrixAdd_float_StubWithCallback(ciaaLibs_MatrixAdd_float_stub);
   ciaaLibs_MatrixSub_float_StubWithCallback(ciaaLibs_MatrixSub_float_stub);
   ciaaLibs_MatrixMul_float_StubWithCallback(ciaaLibs_MatrixMul_float_stub1);
   ciaaPOSIX_memcpy_StubWithCallback(ciaaPOSIX_memcpy_stub);
   ciaaLibs_MatrixCat_float_StubWithCallback(ciaaLibs_MatrixCat_float_stub);

   /* Worst Case of Contorller_1 execution using the state feedback strategy */
   Rtcs_StateFeedbackWorstRun(&controller_1);

   /* ASSERTs for data controller after executing of the Worst Case of controller */
   TEST_ASSERT_EQUAL_FLOAT_ARRAY((float *)(controller_1.y_vector)->data, (float *)(controller_1.x_vector)->data, controller_1.x_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_e_vector, (float *)(controller_1.e_vector)->data, controller_1.x_size);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_u_vector, (float *)(controller_1.u_vector)->data, controller_1.u_size);
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
