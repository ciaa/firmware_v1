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

/*
 * Initials     Name
 * ---------------------------
 * DeV          Diego Ezequiel Vommaro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150727 v0.0.1 DeV  initial version
 */

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief data types type */
typedef enum
{
   FLOAT_64,
   FLOAT_32,
   INT_32,
   INT_16,
   INT_8
}data_type;

/** \brief Matrix data type */
typedef struct
{
   uint16_t n_rows;
   uint16_t n_columns;
   data_type type;
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
extern void ciaaLibs_MatrixInit(ciaaLibs_matrix_t *mat, uint16_t n_rows, uint16_t n_columns, data_type type, void *data);

/** \brief Generic Matrices Addition
 **
 ** Adds two generic matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa generic matrix
 ** \param[in] src2 pointer to the ciaa generic matrix
 ** \param[in] dst pointer to the ciaa generic matrix
 **/
extern void ciaaLibs_MatrixAdd(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Generic Matrices Substraction
 **
 ** Substracs two generic matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa generic matrix
 ** \param[in] src2 pointer to the ciaa generic matrix
 ** \param[in] dst pointer to the ciaa generic matrix
 **/
extern void ciaaLibs_MatrixSub(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Generic Matrix Multiplication
 **
 ** Multiplies two generic matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa generic matrix
 ** \param[in] src2 pointer to the ciaa generic matrix
 ** \param[in] dst pointer to the ciaa generic matrix
 **/
extern void ciaaLibs_MatrixMul(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Generic Inverse Matrix
 **
 ** Inverses a generic matrix and stores the result in other matrix
 **
 ** \param[in] src pointer to the ciaa generic matrix
 ** \param[in] dst pointer to the ciaa generic matrix
 **/
extern void ciaaLibs_MatrixInv(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst);

/** \brief Generic Transposed Matrix
 **
 ** Transposes a matrix
 **
 ** \param[in] src pointer to the ciaa generic matrix
 ** \param[in] dst pointer to the ciaa generic matrix
 **/
extern void ciaaLibs_MatrixTran(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst);

/** \brief Float Matrices Addition
 **
 ** Adds two float matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa float matrix
 ** \param[in] src2 pointer to the ciaa float matrix
 ** \param[in] dst pointer to the ciaa float matrix
 **/
extern void ciaaLibs_MatrixAdd_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Float Matrices Substraction
 **
 ** Substracs two float matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa float matrix
 ** \param[in] src2 pointer to the ciaa float matrix
 ** \param[in] dst pointer to the ciaa float matrix
 **/
extern void ciaaLibs_MatrixSub_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Float Matrix Multiplication
 **
 ** Multiplies two float matrices and stores the result in a third matrix
 **
 ** \param[in] src1 pointer to the ciaa float matrix
 ** \param[in] src2 pointer to the ciaa float matrix
 ** \param[in] dst pointer to the ciaa float matrix
 **/
extern void ciaaLibs_MatrixMul_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst);

/** \brief Float Inverse Matrix
 **
 ** Inverses a float matrix and stores the result in other matrix
 **
 ** \param[in] src pointer to the ciaa float matrix
 ** \param[in] dst pointer to the ciaa float matrix
 **/
extern void ciaaLibs_MatrixInv_float(ciaaLibs_matrix_t *src, ciaaLibs_matrix_t *dst);

/** \brief Float Transposed Matrix
 **
 ** Transposes a matrix
 **
 ** \param[in] src pointer to the ciaa float matrix
 ** \param[in] dst pointer to the ciaa float matrix
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

