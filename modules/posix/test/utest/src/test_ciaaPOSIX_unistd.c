/* Copyright 2016, Diego Ezequiel Vommaro
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

/** \brief This file implements the unistd functions tests
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */
/** \addtogroup ModuleTests Module Tests
 ** @{ */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaPOSIX_unistd.h"
#include "mock_os.h"
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
TaskType MyGetTaskIDTaskID;

char const * const ciaaPOSIX_assert_msg = \
      "ASSERT Failed in %s:%d in expression %s\n";

/*==================[internal functions definition]==========================*/

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


StatusType MyGetTaskID(TaskRefType TaskID, int cmock_num_calls)
{
   (void)cmock_num_calls;
   *TaskID = MyGetTaskIDTaskID;
   return E_OK;
}


/** \brief test sleep
 **
 ** test the function ciaaPOSIX_sleep
 **
 **/
void test_ciaaPOSIX_sleep(void) {

   TEST_ASSERT_EQUAL_INT(0, TASKS_COUNT);
}

/** \brief test usleep
 **
 ** test the function ciaaPOSIX_usleep
 **
 **/
void test_ciaaPOSIX_usleep(void) {
}

/** \brief test sleepMainFunction
 **
 ** test the function ciaaPOSIX_sleepMainFunction
 **
 **/
void test_ciaaPOSIX_sleepMainFunction(void) {
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

