/* Copyright 2015, Diego Ezequiel Vommaro
 * Copyright 2015, ACSE & CADIEEL
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

/** \brief This file implements the test of the Rtcs Module
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */
/** \addtogroup UnitTests Unit Tests
 ** @{ */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaLibs_Matrix.h"

/*==================[macros and definitions]=================================*/
#define DATA_SIZE_1 4
#define COLUMN_SIZE_1 2
#define ROW_SIZE_1 2

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static void * ciaaPOSIX_memcpy_stub (void *s1, void const *s2, size_t n);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
float data_tst_1[] = {1, 2, 3, 4};
float data_tst_2[] = {5, 6, 7, 8};
float data_tst_3[DATA_SIZE_1];
float data_tst_4[DATA_SIZE_1+DATA_SIZE_1];
float matrix_3_expected_values_1[]={6, 8, 10, 12};
float matrix_3_expected_values_2[]={-4, -4, -4, -4};
float matrix_3_expected_values_3[]={19, 22, 43, 50};
float matrix_4_expected_values_1[]={1, 2, 3, 4, 5, 6, 7, 8};

ciaaLibs_matrix_t matrix_1;
ciaaLibs_matrix_t matrix_2;
ciaaLibs_matrix_t matrix_3;
ciaaLibs_matrix_t matrix_4;

/*==================[internal functions definition]==========================*/
static void * ciaaPOSIX_memcpy_stub (void *s1, void const *s2, size_t n)
{
   while(0 < n)
   {
      /* decrement counter */
      n--;

      /* copy 1 byte */
      ((uint8_t*)s1)[n] = ((uint8_t*)s2)[n];
   }

   return s1;
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

/** \brief test ciaaLibs_MatrixInit
 **
 ** Correct initialization of some matrices
 **
 */
void test_ciaaLibs_MatrixInit_01(void)
{
   /* Initialization of matrices */
   ciaaLibs_MatrixInit(&matrix_1, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_1);
   ciaaLibs_MatrixInit(&matrix_2, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_2);
   ciaaLibs_MatrixInit(&matrix_3, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_3);
   ciaaLibs_MatrixInit(&matrix_4, ROW_SIZE_1 + ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_4);

   /* ASSERTs for matrix_1 data */
   TEST_ASSERT_EQUAL_PTR((intptr_t) data_tst_1, (intptr_t) matrix_1.data);
   TEST_ASSERT_EQUAL_UINT16(ROW_SIZE_1, matrix_1.n_rows);
   TEST_ASSERT_EQUAL_UINT16(COLUMN_SIZE_1, matrix_1.n_columns);
   TEST_ASSERT_EQUAL_UINT16(CIAA_LIBS_FLOAT_32, matrix_1.type);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(data_tst_1, (float *) matrix_1.data, DATA_SIZE_1);

   /* ASSERTs for matrix_2 data */
   TEST_ASSERT_EQUAL_PTR((intptr_t) data_tst_2, (intptr_t) matrix_2.data);
   TEST_ASSERT_EQUAL_UINT16(ROW_SIZE_1, matrix_2.n_rows);
   TEST_ASSERT_EQUAL_UINT16(COLUMN_SIZE_1, matrix_2.n_columns);
   TEST_ASSERT_EQUAL_UINT16(CIAA_LIBS_FLOAT_32, matrix_2.type);
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(data_tst_2, (float *) matrix_2.data, DATA_SIZE_1);

   /* ASSERTs for matrix_3 data */
   TEST_ASSERT_EQUAL_PTR((intptr_t) data_tst_3, (intptr_t) matrix_3.data);
   TEST_ASSERT_EQUAL_UINT16(ROW_SIZE_1, matrix_3.n_rows);
   TEST_ASSERT_EQUAL_UINT16(COLUMN_SIZE_1, matrix_3.n_columns);
   TEST_ASSERT_EQUAL_UINT16(CIAA_LIBS_FLOAT_32, matrix_3.type);

   /* ASSERTs for matrix_4 data */
   TEST_ASSERT_EQUAL_PTR((intptr_t) data_tst_4, (intptr_t) matrix_4.data);
   TEST_ASSERT_EQUAL_UINT16(ROW_SIZE_1 + ROW_SIZE_1, matrix_4.n_rows);
   TEST_ASSERT_EQUAL_UINT16(COLUMN_SIZE_1, matrix_4.n_columns);
   TEST_ASSERT_EQUAL_UINT16(CIAA_LIBS_FLOAT_32, matrix_4.type);
}

/** \brief test ciaaLibs_MatrixAdd_float
 **
 ** Correct add of two matrices
 **
 */
void test_ciaaLibs_MatrixAdd_float_01(void)
{
   /* Initialization of matrices */
   ciaaLibs_MatrixInit(&matrix_1, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_1);
   ciaaLibs_MatrixInit(&matrix_2, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_2);
   ciaaLibs_MatrixInit(&matrix_3, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_3);

   /* Adding matrix_1 with matrix_2 and storing in matrix_3 */
   ciaaLibs_MatrixAdd_float(&matrix_1, &matrix_2, &matrix_3);

   /* Asserting of matrix_3 with its expected value */
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(matrix_3_expected_values_1, (float *) matrix_3.data, DATA_SIZE_1);
}

/** \brief test ciaaLibs_MatrixSub_float
 **
 ** Correct Substraction of two matrices
 **
 */
void test_ciaaLibs_MatrixSub_float_01(void)
{
   /* Initialization of matrices */
   ciaaLibs_MatrixInit(&matrix_1, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_1);
   ciaaLibs_MatrixInit(&matrix_2, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_2);
   ciaaLibs_MatrixInit(&matrix_3, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_3);

   /* Subtracting of matrix_1 with matrix_2 and storing in matrix_3 */
   ciaaLibs_MatrixSub_float(&matrix_1, &matrix_2, &matrix_3);

   /* Asserting of matrix_3 with its expected value */
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(matrix_3_expected_values_2, (float *) matrix_3.data, DATA_SIZE_1);
}

/** \brief test ciaaLibs_MatrixMul_float
 **
 ** Correct Multiplication of two matrices
 **
 */
void test_ciaaLibs_MatrixMul_float_01(void)
{
   /* Initialization of matrices */
   ciaaLibs_MatrixInit(&matrix_1, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_1);
   ciaaLibs_MatrixInit(&matrix_2, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_2);
   ciaaLibs_MatrixInit(&matrix_3, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_3);

   /* Multiplicating of matrix_1 with matrix_2 and storing in matrix_3 */
   ciaaLibs_MatrixMul_float(&matrix_1, &matrix_2, &matrix_3);

   /* Asserting of matrix_3 with its expected value */
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(matrix_3_expected_values_3, (float *) matrix_3.data, DATA_SIZE_1);
}

/** \brief test ciaaLibs_MatrixCat_float
 **
 ** Correct Catting from two matrices to another matrix
 **
 */
void test_ciaaLibs_MatrixCat_float_01(void)
{
   /* Initialization of matrices */
   ciaaLibs_MatrixInit(&matrix_1, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_1);
   ciaaLibs_MatrixInit(&matrix_2, ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_2);
   ciaaLibs_MatrixInit(&matrix_4, ROW_SIZE_1 + ROW_SIZE_1, COLUMN_SIZE_1, CIAA_LIBS_FLOAT_32, data_tst_4);

   /* Setting of behavior to ciaaLibs_MatrixInit() */
   ciaaPOSIX_memcpy_StubWithCallback(ciaaPOSIX_memcpy_stub);

   /* Concatenating of matrix_1 with matrix_2 and storing in matrix_4 */
   ciaaLibs_MatrixCat_float(&matrix_1, &matrix_2, &matrix_4);

   /* Asserting of matrix_4 with its expected value */
   TEST_ASSERT_EQUAL_FLOAT_ARRAY(matrix_4_expected_values_1, (float *) matrix_4.data, DATA_SIZE_1 + DATA_SIZE_1);
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

