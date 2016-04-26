/* Copyright 2016, Mariano Cerdeiro
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

/** \brief This file implements the semaphore tests
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
#include "ciaaPOSIX_semaphore.h"
#include "mock_os.h"

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


/** \brief test init
 **
 **/
void test_ciaaPOSIX_sem_init(void) {
   sem_t sem;
   int ret;

   ret = ciaaPOSIX_sem_init(&sem);

   /* open group specify that this interfaces returns -1 if an error occurs,
    * any other value indicates success. */
   TEST_ASSERT_TRUE(-1 != ret);
}

void test_ciaaPOSIX_sem(void) {
   sem_t sem;
   int ret;

   ret = ciaaPOSIX_sem_init(&sem);
   TEST_ASSERT_TRUE(-1 != ret);

   GetTaskID_StubWithCallback(MyGetTaskID);

   MyGetTaskIDTaskID = 0;
   GetResource_ExpectAndReturn(POSIXR, E_OK);
   ReleaseResource_ExpectAndReturn(POSIXR, E_OK);

   ret = ciaaPOSIX_sem_wait(&sem);
   TEST_ASSERT_EQUAL_INT(0, ret);

   MyGetTaskIDTaskID = 1;
   GetResource_ExpectAndReturn(POSIXR, E_OK);
   ReleaseResource_ExpectAndReturn(POSIXR, E_OK);
   WaitEvent_ExpectAndReturn(POSIXE, E_OK);
   ClearEvent_ExpectAndReturn(POSIXE, E_OK);

   ret = ciaaPOSIX_sem_wait(&sem);
   TEST_ASSERT_EQUAL_INT(0, ret);

   MyGetTaskIDTaskID = 0;
   GetResource_ExpectAndReturn(POSIXR, E_OK);
   ReleaseResource_ExpectAndReturn(POSIXR, E_OK);
   SetEvent_ExpectAndReturn(1, POSIXE, E_OK);
   ret = ciaaPOSIX_sem_post(&sem);
   TEST_ASSERT_EQUAL_INT(0, ret);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

