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

/** \brief Matrix Library implementation
 **
 ** This file implements matricial operations
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Libs CIAA Libraries
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * Dev          Diego Ezequiel Vommaro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150801 v0.0.1 DeV  initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaLibs_Matrix.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
extern void ciaaLibs_MatrixInit(ciaaLibs_matrix_t *mat, uint16_t n_rows, uint16_t n_columns, data_type type, void *data)
{

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

extern void ciaaLibs_MatrixAdd_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{

}

extern void ciaaLibs_MatrixSub_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{

}

extern void ciaaLibs_MatrixMul_float(ciaaLibs_matrix_t *src1, ciaaLibs_matrix_t *src2, ciaaLibs_matrix_t *dst)
{

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

