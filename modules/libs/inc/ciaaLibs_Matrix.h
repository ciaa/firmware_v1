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

#ifndef CIAALIBS_MATRIX_H
#define CIAALIBS_MATRIX_H
/** \brief Matrix Library header
 **
 ** This library provides Matricial functionalities
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Libs CIAA Libraries
 ** @{ */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdlib.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief data types type */
typedef enum
{
   CIAA_LIBS_FLOAT_64,
   CIAA_LIBS_FLOAT_32,
   CIAA_LIBS_INT_32,
   CIAA_LIBS_INT_16,
   CIAA_LIBS_INT_8
}ciaaLibs_matrix_data_t;

/** \brief Matrix data type */
typedef struct
{
   uint16_t n_rows;
   uint16_t n_columns;
   ciaaLibs_matrix_data_t type;
   void *data;
}ciaaLibs_matrix_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief Matrix Initialization
 **
 ** Initializes a matrix
 **
 ** \param[in] mat pointer to the ciaa generic matrix
 ** \param[in] n_rows count of rows
 ** \param[in] n_columns count of columns
 ** \param[in] type type of data of the matrix
 ** \param[in] data pointer to matrix data
 **/
extern void ciaaLibs_MatrixInit(ciaaLibs_matrix_t *mat, uint16_t n_rows, uint16_t n_columns, ciaaLibs_matrix_data_t type, void *data);

/** \brief Matrix Copy
 **
 ** Copies a matrix to another
 **
 ** \param[in] src Source Matrix
 ** \param[in] dst Destination Matrix
 **/
extern void ciaaLibs_MatrixCpy(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst);

/** \brief Matrix Concatenated
 **
 ** Concatenates a matrix with another to a target matrix
 **
 ** \param[in] src1 Source Matrix
 ** \param[in] src2 Source Matrix
 ** \param[in] dst Destination Matrix
 **
 ** \remarks The input parameter "dst" can be any matrix excepting "src1" and "src2" matrices
 **/
extern void ciaaLibs_MatrixCat(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Generic Matrices Addition
 **
 ** Adds two generic matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa generic matrix
 ** \param[in] src2 pointer to the ciaa generic matrix
 ** \param[in] dst pointer to the ciaa generic matrix
 **
 ** \remarks The input parameter "dst" can be any matrix inlcuding "src1" and "src2" matrices
 **/
extern void ciaaLibs_MatrixAdd(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Generic Matrices Substraction
 **
 ** Substracs two generic matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa generic matrix
 ** \param[in] src2 pointer to the ciaa generic matrix
 ** \param[in] dst pointer to the ciaa generic matrix
 **
 ** \remarks The input parameter "dst" can be any matrix including "src1" or "src2" matrices
 **/
extern void ciaaLibs_MatrixSub(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Generic Matrix Multiplication
 **
 ** Multiplies two generic matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa generic matrix
 ** \param[in] src2 pointer to the ciaa generic matrix
 ** \param[in] dst pointer to the ciaa generic matrix
 **
 ** \remarks The input parameter "dst" can be any matrix including "src1" or "src2" matrices
 **/
extern void ciaaLibs_MatrixMul(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Generic Multiplication of a Matrix by a Scalar
 **
 ** Multiplies a matrix by a scalar and stores the result in other matrix
 **
 ** \param[in] src1 pointer to the ciaa generic matrix
 ** \param[in] src2 pointer to void (scalar operand)
 ** \param[in] dst pointer to the ciaa generic matrix
 **
 ** \remarks The input parameter "dst" can be any matrix including "src1" matrix
 **/
extern void ciaaLibs_MatrixByScalarMul(ciaaLibs_matrix_t *src1, void *src2, ciaaLibs_matrix_t *dst);

/** \brief Generic Inverse Matrix
 **
 ** Inverses a generic matrix and stores the result in other matrix
 **
 ** \param[in] src pointer to the ciaa generic matrix
 ** \param[in] dst pointer to the ciaa generic matrix
 **
 ** \remarks The input parameter "dst" can be any matrix including "src" matrix
 **/
extern void ciaaLibs_MatrixInv(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst);

/** \brief Generic Transposed Matrix
 **
 ** Transposes a matrix
 **
 ** \param[in] src pointer to the ciaa generic matrix
 ** \param[in] dst pointer to the ciaa generic matrix
 **
 ** \remarks The input parameter "dst" can be any matrix including "src" matrix if It's square
 **/
extern void ciaaLibs_MatrixTran(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst);

/** \brief Float Matrix Concatenated
 **
 ** Concatenates a float matrix with another to a target float matrix
 **
 ** \param[in] src1 Source Matrix
 ** \param[in] src2 Source Matrix
 ** \param[in] dst Destination Matrix
 **
 ** \remarks The input parameter "dst" can be any matrix excepting "src1" or "src2" matrices
 **/
extern void ciaaLibs_MatrixCat_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Float Matrices Addition
 **
 ** Adds two float matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa float matrix
 ** \param[in] src2 pointer to the ciaa float matrix
 ** \param[in] dst pointer to the ciaa float matrix
 **
 ** \remarks The input parameter "dst" can be any matrix including "src1" or "src2" matrices
 **/
extern void ciaaLibs_MatrixAdd_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Float Matrices Substraction
 **
 ** Substracs two float matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa float matrix
 ** \param[in] src2 pointer to the ciaa float matrix
 ** \param[in] dst pointer to the ciaa float matrix
 **
 ** \remarks The input parameter "dst" can be any matrix inlcuding "src1" or "src2" matrices
 **/
extern void ciaaLibs_MatrixSub_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Float Matrix Multiplication
 **
 ** Multiplies two float matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa float matrix
 ** \param[in] src2 pointer to the ciaa float matrix
 ** \param[in] dst pointer to the ciaa float matrix
 **
 ** \remarks The input parameter "dst" can be any matrix including "src1" or "src2" matrices
 **/
extern void ciaaLibs_MatrixMul_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Float Multiplication of a Matrix by a Scalar
 **
 ** Multiplies a matrix by a scalar and stores the result in other matrix
 **
 ** \param[in] src1 pointer to the ciaa generic matrix
 ** \param[in] src2 pointer to float (scalar operand)
 ** \param[in] dst pointer to the ciaa generic matrix
 **
 ** \remarks The input parameter "dst" can be any matrix including "src1" matrix
 **/
extern void ciaaLibs_MatrixByScalarMul_float(ciaaLibs_matrix_t *src1, float *src2, ciaaLibs_matrix_t *dst);

/** \brief Float Inverse Matrix
 **
 ** Inverses a float matrix and stores the result in other matrix
 **
 ** \param[in] src pointer to the ciaa float matrix
 ** \param[in] dst pointer to the ciaa float matrix
 **
 ** \remarks The input parameter "dst" can be any matrix including "src" matrix
 **/
extern void ciaaLibs_MatrixInv_float(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst);

/** \brief Float Transposed Matrix
 **
 ** Transposes a matrix
 **
 ** \param[in] src pointer to the ciaa float matrix
 ** \param[in] dst pointer to the ciaa float matrix
 **
 ** \remarks The input parameter "dst" can be any matrix including "src" if It's square
 **/
extern void ciaaLibs_MatrixTran_float(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAALIBS_MATRIX_H */

