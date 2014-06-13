/* Copyright 2014, Mariano Cerdeiro
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

#ifndef _CIAALIBS_MATHS_H_
#define _CIAALIBS_MATHS_H_
/** \brief Math Library header
 **
 ** This library provides a Maths functionalities
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Libs CIAA Libraries
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140613 v0.0.2 MaCe implement min, max, setBit and clearBit
 * 20140611 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Check if a number is power of 2
 **
 ** \param[in] num   number to be checked
 ** \return    1 if power of two, 0 in other case
 **/
#define ciaaLibs_isPowerOfTwo(num) ( ((num) != 0) && (!((num) & ((num) - 1)))  )

/** \brief Return the minimum of two values
 **
 ** \param[in] x first value to be compared
 ** \param[in] y second value to be compared
 ** \return the smaller of both given values
 **/
#define ciaaLibs_min(x,y)        \
   ( ( (x) > (y) ) ?             \
     (y) :                       \
     (x) )

/** \brief Return the maximum of two values
 **
 ** \param[in] x first value to be compared
 ** \param[in] y second value to be compared
 ** \return the bigues of both given values
 **/
#define ciaaLibs_max(x,y)        \
   ( ( (x) > (y) ) ?             \
     (x) :                       \
     (y) )

/** \brief Set a specific bit of a variable
 **
 ** \param[inout] var variable to set a bit
 ** \param[in] bit bit to be set
 **/
#define ciaaLibs_setBit(var, bit)   \
   ((var) |= ( 1 << (bit) ))

/** \brief Clear a specific bit of a variable
 **
 ** \param[inout] var variable to clear a bit
 ** \param[in] bit bit to be cleared
 **/
#define ciaaLibs_clearBit(var, bit)   \
   ((var) &= (~( 1 << (bit) )))

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAALIBS_MATHS_H_ */

