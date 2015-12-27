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

/** \brief This file implements the test of the Rtcs Module
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
#include "Rtcs_Internal.h"
#include "Rtcs_StateFeedback.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
Rtcs_state_t Rtcs_state = UNINITIALIZED;
Rtcs_generic_controller_t *Rtcs_controllers_list[CONTROLLERS_LIST_SIZE];
Rtcs_generic_controller_t Rtcs_controllers_data[CONTROLLERS_LIST_SIZE];

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void) {
   uint32_t i;

   /* Load of the Generic Controllers pointers*/
   for(i = 0; i<CONTROLLERS_LIST_SIZE; i++)
   {
      Rtcs_controllers_list[i] = &Rtcs_controllers_data[i];
   }

   /* Load of the first run functions of the controllers */
   for(i = 0; i<CONTROLLERS_LIST_SIZE; i++)
   {
      Rtcs_controllers_list[i]->ControllerFirstRunFunc = Rtcs_StateFeedbackFirstRun;
   }
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void) {

   /* Set the state of tool */
   Rtcs_state = UNINITIALIZED;
}

/** \brief test Rtcs_Init
 **
 ** Correct Initialization and Incorrect Initialization due to a double call
 **
 */
void test_Rtcs_Init_01(void)
{
   int8_t ret_1, ret_2;

   Rtcs_StateFeedbackFirstRun_CMockIgnore();

   ret_1 = Rtcs_Init();

   Rtcs_StateFeedbackFirstRun_CMockIgnore();

   ret_2 = Rtcs_Init();

   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_OK, ret_1);
   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_ERROR, ret_2);
}

/** \brief test Rtcs_Init
 **
 ** Incorrect Initialization due to a call in "Inactive" state
 **
 */
void test_Rtcs_Init_02(void)
{
   int8_t ret_1, ret_2, ret_3;

   Rtcs_StateFeedbackFirstRun_CMockIgnore();

   ret_1 = Rtcs_Init();

   ret_2 = Rtcs_Stop();

   Rtcs_StateFeedbackFirstRun_CMockIgnore();

   ret_3 = Rtcs_Init();

   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_OK, ret_1);
   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_OK, ret_2);
   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_ERROR, ret_3);
}

/** \brief test Rtcs_Start
 **
 ** Correct Starting
 **
 */
void test_Rtcs_Start_01(void)
{
   int8_t ret_1, ret_2, ret_3;

   Rtcs_StateFeedbackFirstRun_CMockIgnore();

   ret_1 = Rtcs_Init();

   ret_2 = Rtcs_Stop();

   ret_3 = Rtcs_Start();

   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_OK, ret_1);
   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_OK, ret_2);
   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_OK, ret_3);
}

/** \brief test Rtcs_Start
 **
 ** Incorrect Starting due to a call in "Unintialized" state
 **
 */
void test_Rtcs_Start_02(void)
{
   int8_t ret;

   ret = Rtcs_Start();

   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_ERROR, ret);
}

/** \brief test Rtcs_Stop
 **
 ** Correct Stopping at the beginning, and after a call to "Start" function.
 **
 */
void test_Rtcs_Stop_01(void)
{
   int8_t ret_1, ret_2, ret_3, ret_4;

   Rtcs_StateFeedbackFirstRun_CMockIgnore();

   ret_1 = Rtcs_Init();

   ret_2 = Rtcs_Stop();

   ret_3 = Rtcs_Start();

   ret_4 = Rtcs_Stop();

   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_OK, ret_1);
   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_OK, ret_2);
   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_OK, ret_3);
   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_OK, ret_4);
}

/** \brief test Rtcs_Stop
 **
 ** Incorrect Stopping due to a call in "Unintialized" state
 **
 */
void test_Rtcs_Stop_02(void)
{
   int8_t ret;

   ret = Rtcs_Stop();

   TEST_ASSERT_EQUAL_INT8(RTCS_STATE_ERROR, ret);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

