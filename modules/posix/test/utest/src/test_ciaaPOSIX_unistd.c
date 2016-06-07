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
#include "mock_ciaaPOSIX_stdio.h"
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/
#define CIAAPOSIX_MAINFUNCTION_PERIODUS 10000

#define CIAAPOSIX_MAINFUNCTION_PERIODMS ((CIAAPOSIX_MAINFUNCTION_PERIODUS)/1000)

#define SLEEP_TIME_TO_COUNTS (1000 / CIAAPOSIX_MAINFUNCTION_PERIODMS)

#define MAX_SECONDS (MAX_COUNTS / SLEEP_TIME_TO_COUNTS)

#define MAX_USECONDS (UINT32_MAX - (CIAAPOSIX_MAINFUNCTION_PERIODUS-2))

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
TaskType MyGetTaskIDTaskID;
uint32_t WaitEvent_flag = 0;
uint32_t ClearEvent_flag = 0;
uint32_t SetEvent_flag = 0;

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

   int i;

   WaitEvent_flag = 0;
   ClearEvent_flag = 0;
   SetEvent_flag = 0;
   counts_to_wakeup = 0;
   ciaaPOSIX_counter = 0;

   for(i=0;i<TASKS_COUNT;i++)
      ciaaPOSIX_sleeps[i]=0;
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

StatusType WaitEvent_stub(EventMaskType Mask, int cmock_num_calls)
{
   WaitEvent_flag++;
}

StatusType ClearEvent_stub(EventMaskType Mask, int cmock_num_calls)
{
   ClearEvent_flag++;
}

StatusType SetEvent_stub(EventMaskType Mask, int cmock_num_calls)
{
   SetEvent_flag++;
}

/** \brief test ciaaPOSIX_sleep
 **
 ** There are not sleeping functions
 **
 **/
void test_ciaaPOSIX_sleep_01(void) {

   /* Internal data */
   int ret;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);

   /* Set Task ID */
   MyGetTaskIDTaskID = 0;

   /* Call function to test */
   ret = ciaaPOSIX_sleep(MAX_SECONDS-1);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* call WaitEvent function */
   TEST_ASSERT_EQUAL_UINT32(1, WaitEvent_flag);
   /* call ClearEvent function */
   TEST_ASSERT_EQUAL_UINT32(1, ClearEvent_flag);
   /* Now, there is a sleeping function */
   TEST_ASSERT_EQUAL_UINT32(1 << STATE_BIT, isTaskSleeping(ciaaPOSIX_counter));
   /* Correct value of counts to wake up the task */
   TEST_ASSERT_EQUAL_UINT32((MAX_SECONDS - 1)*SLEEP_TIME_TO_COUNTS, counts_to_wakeup);
   TEST_ASSERT_EQUAL_UINT32((MAX_SECONDS - 1)*SLEEP_TIME_TO_COUNTS, getSleepingCounts(ciaaPOSIX_sleeps[0]));
   TEST_ASSERT_EQUAL_UINT32(1 << STATE_BIT, isTaskSleeping(ciaaPOSIX_sleeps[0]));
}

/** \brief test ciaaPOSIX_sleep
 **
 ** There is a sleeping function
 **
 **/
void test_ciaaPOSIX_sleep_02(void) {

   /* Internal data */
   int ret_1, ret_2;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);

   /* Set Task ID */
   MyGetTaskIDTaskID = 0;

   /* Set initial condition for this test */
   ret_1 = ciaaPOSIX_sleep(MAX_SECONDS - 1);

   /* Set Task ID */
   MyGetTaskIDTaskID = 1;

   /* Call function to test */
   ret_2 = ciaaPOSIX_sleep(MAX_SECONDS - 2);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret_1);
   TEST_ASSERT_EQUAL_INT(0, ret_2);
   /* call WaitEvent function */
   TEST_ASSERT_EQUAL_UINT32(2, WaitEvent_flag);
   /* call ClearEvent function */
   TEST_ASSERT_EQUAL_UINT32(2, ClearEvent_flag);
   /* there is a sleeping function */
   TEST_ASSERT_EQUAL_UINT32(1 << STATE_BIT, isTaskSleeping(ciaaPOSIX_counter));
   /* Correct value of counts to wake up the task */
   TEST_ASSERT_EQUAL_UINT32((MAX_SECONDS - 2)*SLEEP_TIME_TO_COUNTS, counts_to_wakeup);
   TEST_ASSERT_EQUAL_UINT32((MAX_SECONDS - 2)*SLEEP_TIME_TO_COUNTS, getSleepingCounts(ciaaPOSIX_sleeps[1]));
   TEST_ASSERT_EQUAL_UINT32(1 << STATE_BIT, isTaskSleeping(ciaaPOSIX_sleeps[1]));
}

/** \brief test ciaaPOSIX_sleep
 **
 ** There is a sleeping function but the current sleeping   \
 ** function has a count larger than the rest sleeping functions
 **
 **/
void test_ciaaPOSIX_sleep_03(void) {

   /* Internal data */
   int ret_1, ret_2;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);

   /* Set Task ID */
   MyGetTaskIDTaskID = 0;

   /* Set initial condition for this test */
   ret_1 = ciaaPOSIX_sleep(MAX_SECONDS - 3);

   /* Set Task ID */
   MyGetTaskIDTaskID = 1;

   /* Call function to test */
   ret_2 = ciaaPOSIX_sleep(MAX_SECONDS - 2);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret_1);
   TEST_ASSERT_EQUAL_INT(0, ret_2);
   /* call WaitEvent function */
   TEST_ASSERT_EQUAL_UINT32(2, WaitEvent_flag);
   /* call ClearEvent function */
   TEST_ASSERT_EQUAL_UINT32(2, ClearEvent_flag);
   /* there is a sleeping function */
   TEST_ASSERT_EQUAL_UINT32(1 << STATE_BIT, isTaskSleeping(ciaaPOSIX_counter));
   /* Correct value of counts to wake up the task */
   TEST_ASSERT_EQUAL_UINT32((MAX_SECONDS - 3)*SLEEP_TIME_TO_COUNTS, counts_to_wakeup);
   TEST_ASSERT_EQUAL_UINT32((MAX_SECONDS - 2)*SLEEP_TIME_TO_COUNTS, getSleepingCounts(ciaaPOSIX_sleeps[1]));
   TEST_ASSERT_EQUAL_UINT32(1 << STATE_BIT, isTaskSleeping(ciaaPOSIX_sleeps[1]));
}

/** \brief test ciaaPOSIX_usleep function
 **
 ** call ciaaPOSIX_usleep two times with correct paremeters
 **
 **/
void test_ciaaPOSIX_usleep_01(void) {
   /* Internal data */
   int ret_1, ret_2;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);

   /* Set Task ID */
   MyGetTaskIDTaskID = 0;

   /* Set initial condition for this test */
   ret_1 = ciaaPOSIX_usleep(1000001);

   /* Set Task ID */
   MyGetTaskIDTaskID = 1;

   /* Call function to test */
   ret_2 = ciaaPOSIX_usleep(1010001);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret_1);
   TEST_ASSERT_EQUAL_INT(0, ret_2);
   /* call WaitEvent function */
   TEST_ASSERT_EQUAL_UINT32(2, WaitEvent_flag);
   /* call ClearEvent function */
   TEST_ASSERT_EQUAL_UINT32(2, ClearEvent_flag);
   /* there is a sleeping function */
   TEST_ASSERT_EQUAL_UINT32(1 << STATE_BIT, isTaskSleeping(ciaaPOSIX_counter));
   /* Correct value of counts to wake up the task */
   TEST_ASSERT_EQUAL_UINT32(101, counts_to_wakeup);
   TEST_ASSERT_EQUAL_UINT32(102, getSleepingCounts(ciaaPOSIX_sleeps[1]));
   TEST_ASSERT_EQUAL_UINT32(1 << STATE_BIT, isTaskSleeping(ciaaPOSIX_sleeps[1]));
}

/** \brief test ciaaPOSIX_usleep function
 **
 ** call ciaaPOSIX_usleep two times with inccorrect paremeters
 **
 **/
void test_ciaaPOSIX_usleep_02(void) {
   /* Internal data */
   int ret_1, ret_2;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);

   /* Set Task ID */
   MyGetTaskIDTaskID = 0;

   /* Set initial condition for this test */
   ret_1 = ciaaPOSIX_usleep(999999);

   /* Set Task ID */
   MyGetTaskIDTaskID = 1;

   /* Call function to test */
   ret_2 = ciaaPOSIX_usleep(MAX_USECONDS);

   /* ASSERTs */
   /* return ERROR */
   TEST_ASSERT_EQUAL_INT(-1, ret_1);
   TEST_ASSERT_EQUAL_INT(-1, ret_2);
}

/** \brief test ciaaPOSIX_sleepMainFunction
 **
 ** there are not sleeping function, so nothing must be changed
 **
 **/
void test_ciaaPOSIX_sleepMainFunction_01(void) {
   /* Internal data */
   uint32_t i;
   uint32_t aux_counts_to_wakeup;
   uint32_t aux_ciaaPOSIX_counter;
   uint32_t aux_ciaaPOSIX_sleeps[TASKS_COUNT];

   /* Initial conditions */
   aux_counts_to_wakeup = counts_to_wakeup;
   aux_ciaaPOSIX_counter = ciaaPOSIX_counter;
   for(i=0; i < TASKS_COUNT; i++)
   {
      aux_ciaaPOSIX_sleeps[i] = ciaaPOSIX_sleeps[i];
   }

   /* Call function to test */
   ciaaPOSIX_sleepMainFunction();

   /* ASSERTs (Nothing must change) */
   TEST_ASSERT_EQUAL_UINT32(aux_counts_to_wakeup, counts_to_wakeup);
   TEST_ASSERT_EQUAL_UINT32(aux_ciaaPOSIX_counter, ciaaPOSIX_counter);
   TEST_ASSERT_EQUAL_UINT32_ARRAY(aux_ciaaPOSIX_sleeps, ciaaPOSIX_sleeps, TASKS_COUNT);
}

/** \brief test ciaaPOSIX_sleepMainFunction
 **
 ** there is a sleeping function, so the counter must be incremented
 **
 **/
void test_ciaaPOSIX_sleepMainFunction_02(void) {
   /* Internal data */
   uint32_t ret_1;
   uint32_t aux_ciaaPOSIX_sleeps[TASKS_COUNT] = {0};

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);

   /* Initial conditions */
   /* Set Task ID */
   MyGetTaskIDTaskID = 0;
   ret_1 = ciaaPOSIX_sleep(1);
   aux_ciaaPOSIX_sleeps[MyGetTaskIDTaskID] = ((1*100)|(1 << STATE_BIT));

   /* Call function to test */
   ciaaPOSIX_sleepMainFunction();

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret_1);
   TEST_ASSERT_EQUAL_UINT32(1*100, counts_to_wakeup);
   TEST_ASSERT_EQUAL_UINT32(1|(1 << STATE_BIT), ciaaPOSIX_counter);
   TEST_ASSERT_EQUAL_UINT32_ARRAY(aux_ciaaPOSIX_sleeps, ciaaPOSIX_sleeps, TASKS_COUNT);
}

/** \brief test ciaaPOSIX_sleepMainFunction
 **
 ** There are sleeping two tasks, and then, one wakes up
 **
 **/
void test_ciaaPOSIX_sleepMainFunction_03(void) {
   /* Internal data */
   uint32_t ret_1, ret_2, i;
   uint32_t aux_ciaaPOSIX_sleeps[TASKS_COUNT] = {0};

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);

   /* Initial conditions */
   /* Set Task ID */
   MyGetTaskIDTaskID = 0;
   ret_1 = ciaaPOSIX_sleep(1);
   aux_ciaaPOSIX_sleeps[MyGetTaskIDTaskID] = (1*100);
   /* Set Task ID */
   MyGetTaskIDTaskID = 2;
   ret_2 = ciaaPOSIX_sleep(2);
   aux_ciaaPOSIX_sleeps[MyGetTaskIDTaskID] = ((2*100)|(1 << STATE_BIT));

   /* Call function to test */
   for(i = 0; i < (1*100); i++)
   {
      ciaaPOSIX_sleepMainFunction();
   }

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret_1);
   TEST_ASSERT_EQUAL_INT(0, ret_2);
   TEST_ASSERT_EQUAL_UINT32(1*200, counts_to_wakeup);
   TEST_ASSERT_EQUAL_UINT32((1*100)|(1 << STATE_BIT), ciaaPOSIX_counter);
   TEST_ASSERT_EQUAL_UINT32_ARRAY(aux_ciaaPOSIX_sleeps, ciaaPOSIX_sleeps, TASKS_COUNT);
   /* call SetEvent function */
   TEST_ASSERT_EQUAL_UINT32(1, SetEvent_flag);

}

/** \brief test ciaaPOSIX_sleepMainFunction
 **
 ** There are sleeping one task, and then, It wakes up
 **
 **/
void test_ciaaPOSIX_sleepMainFunction_04(void) {
   /* Internal data */
   uint32_t ret_1, i;
   uint32_t aux_ciaaPOSIX_sleeps[TASKS_COUNT] = {0};

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);

   /* Initial conditions */
   /* Set Task ID */
   MyGetTaskIDTaskID = 2;
   ret_1 = ciaaPOSIX_sleep(2);
   aux_ciaaPOSIX_sleeps[MyGetTaskIDTaskID] = ((2*100));

   /* Call function to test */
   for(i = 0; i < (2*100); i++)
   {
      ciaaPOSIX_sleepMainFunction();
   }

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret_1);
   TEST_ASSERT_EQUAL_UINT32(200, counts_to_wakeup);
   TEST_ASSERT_EQUAL_UINT32((2*100), ciaaPOSIX_counter);
   TEST_ASSERT_EQUAL_UINT32_ARRAY(aux_ciaaPOSIX_sleeps, ciaaPOSIX_sleeps, TASKS_COUNT);
   /* call SetEvent function */
   TEST_ASSERT_EQUAL_UINT32(1, SetEvent_flag);
}

/** \brief test ciaaPOSIX_sleepMainFunction
 **
 ** There are sleeping three tasks, and then, one wakes up
 **
 **/
void test_ciaaPOSIX_sleepMainFunction_05(void) {
   /* Internal data */
   uint32_t ret_1, ret_2, ret_3, i;
   uint32_t aux_ciaaPOSIX_sleeps[TASKS_COUNT] = {0};

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);

   /* Initial conditions */
   /* Set Task ID */
   MyGetTaskIDTaskID = 0;
   ret_1 = ciaaPOSIX_sleep(1);
   aux_ciaaPOSIX_sleeps[MyGetTaskIDTaskID] = (1*100);
   /* Set Task ID */
   MyGetTaskIDTaskID = 1;
   ret_2 = ciaaPOSIX_sleep(2);
   aux_ciaaPOSIX_sleeps[MyGetTaskIDTaskID] = ((2*100)|(1 << STATE_BIT));
   /* Set Task ID */
   MyGetTaskIDTaskID = 2;
   ret_3 = ciaaPOSIX_sleep(3);
   aux_ciaaPOSIX_sleeps[MyGetTaskIDTaskID] = ((3*100)|(1 << STATE_BIT));

   /* Call function to test */
   for(i = 0; i < (1*100); i++)
   {
      ciaaPOSIX_sleepMainFunction();
   }

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret_1);
   TEST_ASSERT_EQUAL_INT(0, ret_2);
   TEST_ASSERT_EQUAL_INT(0, ret_3);
   TEST_ASSERT_EQUAL_UINT32(1*200, counts_to_wakeup);
   TEST_ASSERT_EQUAL_UINT32((1*100)|(1 << STATE_BIT), ciaaPOSIX_counter);
   TEST_ASSERT_EQUAL_UINT32_ARRAY(aux_ciaaPOSIX_sleeps, ciaaPOSIX_sleeps, TASKS_COUNT);
   /* call SetEvent function */
   TEST_ASSERT_EQUAL_UINT32(1, SetEvent_flag);
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

