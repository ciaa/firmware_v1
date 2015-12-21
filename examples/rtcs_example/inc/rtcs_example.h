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

#ifndef RTCS_EXAMPLE_H
#define RTCS_EXAMPLE_H
/** \brief real time control system example header file
 **
 ** This is a mini example of the CIAA Firmware
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup RTCS RTCS example header file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DeV          Diego Ezequiel Vommaro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151101 v0.0.1 DeV  initial version
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief Sending of the control efforts to the actuators
 **
 ** Sends control effort to the actuators.
 **
 ** \param[in] data pointer to float data
 ** \param[in] num_elements Size of float data vector
 **/
extern void SendControlEffort_1 (float *data, uint16_t num_elements);

/** \brief Sending of the control efforts to the actuators
 **
 ** Sends control effort to the actuators.
 **
 ** \param[in] data pointer to float data
 ** \param[in] num_elements Size of float data vector
 **/
extern void SendControlEffort_2 (float *data, uint16_t num_elements);

/** \brief Sending of the control efforts to the actuators
 **
 ** Sends control effort to the actuators.
 **
 ** \param[in] data pointer to float data
 ** \param[in] num_elements Size of float data vector
 **/
extern void SendControlEffort_3 (float *data, uint16_t num_elements);

/** \brief Sending of the control efforts to the actuators
 **
 ** Sends control effort to the actuators.
 **
 ** \param[in] data pointer to float data
 ** \param[in] num_elements Size of float data vector
 **/
extern void SendControlEffort_4 (float *data, uint16_t num_elements);

/** \brief Sending of the control efforts to the actuators
 **
 ** Sends control effort to the actuators.
 **
 ** \param[in] data pointer to float data
 ** \param[in] num_elements Size of float data vector
 **/
extern void SendControlEffort_5 (float *data, uint16_t num_elements);

/** \brief Sending of the control efforts to the actuators
 **
 ** Sends control effort to the actuators.
 **
 ** \param[in] data pointer to float data
 ** \param[in] num_elements Size of float data vector
 **/
extern void SendControlEffort_6 (float *data, uint16_t num_elements);
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef RTCS_EXAMPLE_H */

