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

#ifndef RTCS_PORT_H
#define RTCS_PORT_H
/** \brief Real-Time Control System Port Header File
 **
 ** Real-Time Control System Port Header File
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup RTCS RTCS Implementation
 ** @{ */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "ciaaLibs_Matrix.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Macro for Matrix data Type
 **
 ** Macro that initializes a matrix
 **/
#define FLOAT_32 CIAA_LIBS_FLOAT_32

/** \brief Macro for Matrix Initialization
 **
 ** Macro that initializes a matrix
 **
 ** \param[in] mat pointer to the ciaa generic matrix
 ** \param[in] n_rows count of rows
 ** \param[in] n_columns count of columns
 ** \param[in] type type of data of the matrix
 ** \param[in] data pointer to matrix data
 **/
#define Rtcs_Ext_MatrixInit(mat, n_rows, n_columns, type, data)      \
   ciaaLibs_MatrixInit((mat), (n_rows), (n_columns), (type), (data))

/** \brief Macro to Copy of Matrices
 **
 ** Macro that copies a matrix to another matrix
 **
 ** \param[in] src pointer to ciaa generic matrix
 ** \param[in] dst pointer to ciaa generic matrix
 **/
#define Rtcs_Ext_MatrixCpy(src, dst)      \
   ciaaLibs_MatrixCpy((src), (dst))

/** \brief Macro to Concatenate Matrices
 **
 ** Macro that concatenates a matrix with another to a target matrix
 **
 ** \param[in] src1 pointer to ciaa generic matrix
 ** \param[in] src2 pointer to ciaa generic matrix
 ** \param[in] dst pointer to ciaa generic matrix
 **/
#define Rtcs_Ext_MatrixCat(src1, src2,  dst)      \
   ciaaLibs_MatrixCat((src1), (src2), (dst))

/** \brief Macro for Generic Matrices Addition
 **
 ** Macro that Adds two generic matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to ciaa generic matrix
 ** \param[in] src2 pointer to ciaa generic matrix
 ** \param[in] dst pointer to ciaa generic matrix
 **/
#define Rtcs_Ext_MatrixAdd(src1, src2, dst)  \
  ciaaLibs_MatrixAdd((src1), (src2), (dst))

/** \brief Macro for  Generic Matrices Substraction
 **
 ** Macro that Substracs two generic matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to ciaa generic matrix
 ** \param[in] src2 pointer to ciaa generic matrix
 ** \param[in] dst pointer to  ciaa generic matrix
 **/
#define Rtcs_Ext_MatrixSub(src1, src2, dst)  \
   ciaaLibs_MatrixSub((src1), (src2), (dst))

/** \brief Macro for  Generic Matrix Multiplication
 **
 ** Macro that Multiplies two generic matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to ciaa generic matrix
 ** \param[in] src2 pointer to ciaa generic matrix
 ** \param[in] dst pointer to  ciaa generic matrix
 **/
#define Rtcs_Ext_MatrixMul(src1, src2, dst)  \
   ciaaLibs_MatrixMul((src1), (src2), (dst))

/** \brief Macro for  Generic Multiplication of a Matrix by a Scalar
 **
 ** Macro that Multiplies a matrix by a scalar and stores the result in other matrix
 **
 ** \param[in] src1 pointer to ciaa generic matrix
 ** \param[in] src2 pointer to void
 ** \param[in] dst pointer to ciaa generic matrix
 **/
#define Rtcs_Ext_MatrixByScalarMul(src1, src2, dst)  \
   ciaaLibs_MatrixByScalarMul((src1), (src2), (dst))

/** \brief Macro for Generic Inverse Matrix
 **
 ** Macro that Inverses a generic matrix and stores the result in other matrix
 **
 ** \param[in] src pointer to ciaa generic matrix
 ** \param[in] dst pointer to ciaa generic matrix
 **/
#define Rtcs_Ext_MatrixInv(src, dst)   \
   ciaaLibs_MatrixInv(src, dst)

/** \brief Macro for Generic Transposed Matrix
 **
 ** Macro that Transposes a matrix
 **
 ** \param[in] src pointer to ciaa generic matrix
 ** \param[in] dst pointer to ciaa generic matrix
 **/
#define Rtcs_Ext_MatrixTran(src, dst)  \
   ciaaLibs_MatrixTran((src), (dst))

/** \brief Macro to Copy of float Matrices
 **
 ** Macro that copies a matrix to another matrix
 **
 ** \param[in] src pointer to ciaa float matrix
 ** \param[in] dst pointer to ciaa float matrix
 **/
#define Rtcs_Ext_MatrixCpy_float(src, dst)      \
   ciaaPOSIX_memcpy((dst)->data, (src)->data, sizeof(float) * ((src)->n_rows * (src)->n_columns))

/** \brief Macro to Concatenate float Matrices
 **
 ** Macro that concatenates a matrix with another to a target matrix
 **
 ** \param[in] src1 pointer to ciaa float matrix
 ** \param[in] src2 pointer to ciaa float matrix
 ** \param[in] dst pointer to ciaa float matrix
 **/
#define Rtcs_Ext_MatrixCat_float(src1, src2,  dst)      \
   ciaaLibs_MatrixCat_float((src1), (src2), (dst))

/** \brief Macro for Float Matrices Addition
 **
 ** Macro that Adds two float matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to ciaa float matrix
 ** \param[in] src2 pointer to ciaa float matrix
 ** \param[in] dst pointer to ciaa float matrix
 **/
#define Rtcs_Ext_MatrixAdd_float(src1, src2, dst)  \
   ciaaLibs_MatrixAdd_float((src1), (src2), (dst))

/** \brief Macro for Float Matrices Substraction
 **
 ** Macro that Substracs two float matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to ciaa float matrix
 ** \param[in] src2 pointer to ciaa float matrix
 ** \param[in] dst pointer to ciaa float matrix
 **/
#define Rtcs_Ext_MatrixSub_float(src1, src2, dst)  \
   ciaaLibs_MatrixSub_float((src1), (src2), (dst))

/** \brief Macro for Float Matrix Multiplication
 **
 ** Macro that Multiplies two float matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to ciaa float matrix
 ** \param[in] src2 pointer to ciaa float matrix
 ** \param[in] dst pointer to ciaa float matrix
 **/
#define Rtcs_Ext_MatrixMul_float(src1, src2, dst)  \
   ciaaLibs_MatrixMul_float((src1), (src2), (dst))

/** \brief Macro for floating point Multiplication of a Matrix by a Scalar
 **
 ** Macro that Multiplies a matrix by a scalar and stores the result in other matrix
 **
 ** \param[in] src1 pointer to ciaa float matrix
 ** \param[in] src2 pointer to float
 ** \param[in] dst pointer to the ciaa float matrix
 **/
#define Rtcs_Ext_MatrixByScalarMul_float(src1, src2, dst)  \
   ciaaLibs_MatrixByScalarMul_float((src1), (src2), (dst))

/** \brief Macro for Float Inverse Matrix
 **
 ** Macro that Inverses a float matrix and stores the result in other matrix
 **
 ** \param[in] src pointer to ciaa float matrix
 ** \param[in] dst pointer to ciaa float matrix
 **/
#define Rtcs_Ext_MatrixInv_float(src, dst)   \
   ciaaLibs_MatrixInv_float((src), (dst))

/** \brief Macro for Float Transposed Matrix
 **
 ** Macro that Transposes a matrix
 **
 ** \param[in] src pointer to ciaa float matrix
 ** \param[in] dst pointer to ciaa float matrix
 **/
#define Rtcs_Ext_MatrixTran_float(src, dst)  \
   ciaaLibs_MatrixTran_float((src), (dst))

/*==================[typedef]================================================*/
/** \brief Definition of the Rtcs matrix type */
typedef ciaaLibs_matrix_t Rtcs_ext_matrix_t;
typedef ciaaLibs_matrix_data_t Rtcs_ext_data_type;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef RTCS_PORT_H */

