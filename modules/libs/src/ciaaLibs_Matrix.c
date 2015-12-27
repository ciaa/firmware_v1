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

/** \brief Matrix Library implementation
 **
 ** This file implements matricial operations
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Libs CIAA Libraries
 ** @{ */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_string.h"
#include "ciaaLibs_Matrix.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
extern void ciaaLibs_MatrixInit(ciaaLibs_matrix_t *mat, uint16_t n_rows, uint16_t n_columns, ciaaLibs_matrix_data_t type, void *data)
{
   /* Load number of rows */
   mat->n_rows = n_rows;

   /* Load number of columns */
   mat->n_columns = n_columns;

   /* Load type of data  */
   mat->type = type;

   /* Load data pointer*/
   mat->data = data;
}

extern void ciaaLibs_MatrixCpy(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst)
{
}

extern void ciaaLibs_MatrixCat(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
}

extern void ciaaLibs_MatrixAdd(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
}

extern void ciaaLibs_MatrixSub(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
}

extern void ciaaLibs_MatrixMul(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
}

extern void ciaaLibs_MatrixByScalarMul(ciaaLibs_matrix_t *src1, void *src2, ciaaLibs_matrix_t *dst)
{
}

extern void ciaaLibs_MatrixInv(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst)
{
}

extern void ciaaLibs_MatrixTran(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst)
{
}

extern void ciaaLibs_MatrixCat_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
   uint32_t num_elements = sizeof(float) * src1->n_rows * src1->n_columns;
   uint32_t num_elements_2 = sizeof(float) * src2->n_rows * src2->n_columns;

   /* Copied of data from first matrix to destination matrix */
   ciaaPOSIX_memcpy((void *) dst->data, (void *) src1->data, num_elements);

   /* Copied of data from second matrix to destination matrix*/
   ciaaPOSIX_memcpy(((void *) dst->data) + num_elements, (void *) src2->data, num_elements_2);
}

extern void ciaaLibs_MatrixAdd_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
   float *src1_ptr = src1->data;
   float *src2_ptr = src2->data;
   float *dst_ptr = dst->data;
   uint32_t num_elements;

   num_elements = src1->n_rows * src1->n_columns;

   /* Loop that adds each element from each surce matrix and saves the result in target matrix*/
   while(num_elements > 0u)
   {
      *dst_ptr++ = (*src1_ptr++) + (*src2_ptr++);
      num_elements--;
   }
}

extern void ciaaLibs_MatrixSub_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
   float *src1_ptr = src1->data;
   float *src2_ptr = src2->data;
   float *dst_ptr = dst->data;
   uint32_t num_elements;

   num_elements = src1->n_rows * src1->n_columns;

   /* Loop that subtracts each element from each surce matrix and saves the result in target matrix*/
   while(num_elements > 0u)
   {
      *dst_ptr++ = (*src1_ptr++) - (*src2_ptr++);
      num_elements--;
   }
}

extern void ciaaLibs_MatrixMul_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{
   float *src1_ptr = src1->data;
   float *src2_ptr = src2->data;
   float *dst_ptr = dst->data;
   uint32_t num_rows_src1 = src1->n_rows;
   float *src1_aux_ptr;
   float *src2_aux_ptr;
   uint32_t num_columns_src1;
   uint32_t num_columns_src2;
   float acc;
   float aux_buffer[dst->n_rows * dst->n_columns];
   float *aux_buffer_ptr = aux_buffer;
   uint32_t cant_elements = dst->n_rows * dst->n_columns;

   /* Loop for each row of src1 */
   do
   {
      num_columns_src2 = src2->n_columns;
      src2_aux_ptr = src2_ptr;

      /* It performs the product-dot between one row of src1 and each column of src2 */
      do
      {
         src1_aux_ptr = src1_ptr;
         num_columns_src1 = src1->n_columns;
         acc = 0;

         /* This loops performs the product-dot between one row of src1 and one column of src2 */
         while(num_columns_src1 > 0u)
         {
            acc += (*src1_aux_ptr++) * (*src2_aux_ptr);
            src2_aux_ptr += src2->n_columns;
            num_columns_src1--;
         }

         /* Load the result in target matrix */
         *aux_buffer_ptr++ = acc;

         /* Decrement the column loop counter */
         num_columns_src2--;

         src2_aux_ptr = src2_ptr + (src2->n_columns - num_columns_src2);

      } while(num_columns_src2 > 0u);

      src1_ptr += src1->n_columns;

      /* Decrement the row loop counter */
      num_rows_src1--;

   } while(num_rows_src1 > 0u);

   aux_buffer_ptr = aux_buffer;

   while (cant_elements > 0u)
   {
      *dst_ptr++ = *aux_buffer_ptr++;
      cant_elements--;
   }
}

extern void ciaaLibs_MatrixByScalarMul_float(ciaaLibs_matrix_t *src1, float *src2, ciaaLibs_matrix_t *dst)
{
}

extern void ciaaLibs_MatrixInv_float(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst)
{
}

extern void ciaaLibs_MatrixTran_float(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst)
{
}

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

