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

/** \brief This file implements the test of the Rtcs Module
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
 * 20150925 v0.0.1 DeV  initial version
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "Rtcs_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
state_t Rtcs_state = UNINITIALIZED;
Rtcs_generic_controller_t *Rtcs_controllers_list[CONTROLLERS_LIST_SIZE];

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void doNothing(void *ptr){
}

/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void) {
   uint32_t i;

   for(i = 0; i<CONTROLLERS_LIST_SIZE; i++)
   {
      Rtcs_controllers_list[i]->ControllerFirstRunFunc = doNothing;
   }
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void) {

   Rtcs_state = UNINITIALIZED;
}

void test_Rtcs_Init_01(void)
{
   int8_t ret;

   ret = Rtcs_Init();

   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_OK, ret);
}


void test_Rtcs_Init_02(void)
{
   int8_t ret;

   ret = Rtcs_Init();

   ret = Rtcs_Init();

   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_ERROR, ret);
}

void test_Rtcs_Init_03(void)
{
   int8_t ret;

   ret = Rtcs_Init();

   ret = Rtcs_Stop();

   ret = Rtcs_Init();

   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_ERROR, ret);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

