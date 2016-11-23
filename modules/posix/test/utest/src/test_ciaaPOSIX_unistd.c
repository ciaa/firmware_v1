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
#include "ciaaPOSIX_unistd_Internal.h"
#include "mock_os.h"
#include "mock_ciaaPOSIX_stdio.h"
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/
/* Limit for testing avoiding test times too long */
#define MAX_SECONDS_FOR_THIS_TEST 1000

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
TaskType MyGetTaskIDTaskID = 0;
EventMaskType WaitEvent_ev = 0;
EventMaskType ClearEvent_ev = 0;
EventMaskType SetEvent_ev = 0;

bool WaitEvent_flag = false;
bool ClearEvent_flag = false;
bool SetEvent_flag = false;
bool assert_flag = false;
bool ciaaPOSIX_sleepsTest[TASKS_COUNT] = {false};

char const * const ciaaPOSIX_assert_msg = \
      "ASSERT Failed in %s:%d in expression %s\n";

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
StatusType MyGetTaskID(TaskRefType TaskID, int cmock_num_calls)
{
   (void)cmock_num_calls;
   *TaskID = MyGetTaskIDTaskID;
   return E_OK;
}

StatusType WaitEvent_stub(EventMaskType Mask, int cmock_num_calls)
{
   WaitEvent_ev = Mask;
}

StatusType ClearEvent_stub(EventMaskType Mask, int cmock_num_calls)
{
   /* ClearEvent is seted if WaitEvent was called before */
   if(POSIXE == WaitEvent_ev)
   {
      ClearEvent_ev = POSIXE;
   }

//   ciaaPOSIX_sleepsTest[MyGetTaskIDTaskID] = true;
}

StatusType SetEvent_stub(TaskType TaskID, EventMaskType Mask, int cmock_num_calls)
{
   SetEvent_flag = true;

   ciaaPOSIX_sleepsTest[TaskID] = false;
}

void ciaaPOSIX_assert_stub(uint32_t expr, int cmock_num_calls)
{
   if(expr == 0)
   {
      assert_flag = true;
   }
}

/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void) {

   int i;

   /* setUp auxiliar information */
   WaitEvent_ev = 0;
   ClearEvent_ev = 0;

   SetEvent_StubWithCallback(SetEvent_stub);
   
   TaskType MyGetTaskIDTaskID = 0;
   EventMaskType WaitEvent_ev = 0;
   EventMaskType ClearEvent_ev = 0;
   EventMaskType SetEvent_ev = 0;

   WaitEvent_flag = false;
   ClearEvent_flag = false;
   SetEvent_flag = false;

   assert_flag = false;

   for(i=0;i<TASKS_COUNT;i++)
   {
      ciaaPOSIX_sleepsTest[i] = false;
   }

   /* Wake up the tasks */
   for(i = 0; i < (MAX_SECONDS_FOR_THIS_TEST * SLEEP_TIME_TO_COUNTS); i++)
   {
     // ciaaPOSIX_sleepTick();
   }
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void) {
}

/** \brief test case for test doubles
 **
 ** The WaitEvent stub intercepts information about event to wait
 **
 ** \remarks The focus here is on test doubles and the setUp function
 **/
void test_ciaaPOSIX_unistd_testDoubles_01(void)
{
   /* Internal data */
   EventMaskType WaitEvent_ev_setUp = WaitEvent_ev;

   /* Set SetEvent stub */
   WaitEvent_StubWithCallback(WaitEvent_stub);

   /* Functions to test */
   WaitEvent(POSIXE);

   /* ASSERTs */
   /* The setUp event is zero */
   TEST_ASSERT_EQUAL_INT(0, WaitEvent_ev_setUp);
   /* Then, the waiting event is a POSIX event */
   TEST_ASSERT_EQUAL_INT(POSIXE, WaitEvent_ev);
}

/** \brief test case for test doubles
 **
 ** The ClearEvent stub intercepts information about event to clear
 ** and verify the correct sequence of excecution (Wait and then Clear)
 **
 ** \remarks The focus here is on test doubles and the setUp function
 **/
void test_ciaaPOSIX_unistd_testDoubles_02(void)
{
   /* Internal data */
   EventMaskType WaitEvent_ev_setUp = WaitEvent_ev;
   EventMaskType ClearEvent_ev_setUp = ClearEvent_ev;
   EventMaskType ClearEvent_ev_incorrect = 0;

   /* Set SetEvent stub */
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);

   /* Functions to test. Incorrect sequence */
   ClearEvent(POSIXE);
   ClearEvent_ev_incorrect = ClearEvent_ev;

   /* Function to test. Correct sequence */
   WaitEvent(POSIXE);
   ClearEvent(POSIXE);

   /* ASSERTs */
   /* The setUp events are zero */
   TEST_ASSERT_EQUAL_INT(0, WaitEvent_ev_setUp);
   TEST_ASSERT_EQUAL_INT(0, ClearEvent_ev_setUp);
   /* The event of incorrect sequence is zero*/
   TEST_ASSERT_EQUAL_INT(0, ClearEvent_ev_incorrect);
   /* Then, the waiting and clear events are POSIX events */
   TEST_ASSERT_EQUAL_INT(POSIXE, WaitEvent_ev);
   TEST_ASSERT_EQUAL_INT(POSIXE, ClearEvent_ev);
}

/** \brief test ciaaPOSIX_sleepAddTask
 **
 ** There are no sleeping functions
 **
 **/
void test_ciaaPOSIX_sleepAddTask_01(void)
{
   /* Internal data */
   ciaaPOSIX_counter_t next_to_weak;
   ciaaPOSIX_counter_t counter;
   ciaaPOSIX_counter_t sleeping_list[TASKS_COUNT];
   ciaaPOSIX_counter_t expected_next_to_weak;
   ciaaPOSIX_counter_t expected_counter;
   ciaaPOSIX_counter_t expected_sleeping_list[TASKS_COUNT];

   /* Ignoring resource functions */
   GetResource_IgnoreAndReturn(1);
   ReleaseResource_IgnoreAndReturn(1);

   /* Call function to test */
   ciaaPOSIX_sleepAddTask(100);

   /* ASSERTs */

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
   SetEvent_StubWithCallback(SetEvent_stub);
   ciaaPOSIX_assert_StubWithCallback(ciaaPOSIX_assert_stub);

   /* Set Task ID */
   MyGetTaskIDTaskID = 0;

   /* There are not sleeping tasks */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);

   /* Call function to test */
   ret = ciaaPOSIX_sleep(10);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* call WaitEvent function */
   TEST_ASSERT_TRUE(WaitEvent_flag);
   /* call ClearEvent function */
   TEST_ASSERT_TRUE(ClearEvent_flag);
   /* Task 0 is sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[0]);
   /* The other tasks are not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);
}

/** \brief test ciaaPOSIX_sleep
 **
 ** There is a sleeping function
 **
 **/
void test_ciaaPOSIX_sleep_02(void) {

   /* Internal data */
   int ret;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);
   ciaaPOSIX_assert_StubWithCallback(ciaaPOSIX_assert_stub);

   /* Set Task ID */
   MyGetTaskIDTaskID = 0;

   /* There are not sleeping tasks */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);

   /* Set initial condition for this test */
   ret = ciaaPOSIX_sleep(9);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* call WaitEvent function */
   TEST_ASSERT_TRUE(WaitEvent_flag);
   /* call ClearEvent function */
   TEST_ASSERT_TRUE(ClearEvent_flag);
   /* Task 0 is sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[0]);
   /* The other tasks are not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);

   /* Set Task ID */
   MyGetTaskIDTaskID = 1;

   /* Reset Wait and Clear flags */
   WaitEvent_flag = false;
   ClearEvent_flag = false;

   /* Call function to test */
   ret = ciaaPOSIX_sleep(8);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* call WaitEvent function */
   TEST_ASSERT_TRUE(WaitEvent_flag);
   /* call ClearEvent function */
   TEST_ASSERT_TRUE(ClearEvent_flag);
   /* Task 0 and 1 are sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[1]);
   /* The other tasks are not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);
}

/** \brief test ciaaPOSIX_sleep
 **
 ** There is a sleeping function but the current sleeping   \
 ** function has a count larger than the rest sleeping functions
 **
 **/
void test_ciaaPOSIX_sleep_03(void) {

   /* Internal data */
   int ret;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);
   ciaaPOSIX_assert_StubWithCallback(ciaaPOSIX_assert_stub);

   /* Set Task ID */
   MyGetTaskIDTaskID = 0;

   /* There are not sleeping tasks */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);

   /* Set initial condition for this test */
   ret = ciaaPOSIX_sleep(9);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* call WaitEvent function */
   TEST_ASSERT_TRUE(WaitEvent_flag);
   /* call ClearEvent function */
   TEST_ASSERT_TRUE(ClearEvent_flag);
   /* Task 0 is sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[0]);
   /* The other tasks are not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);

   /* Set Task ID */
   MyGetTaskIDTaskID = 1;

   /* Reset Wait and Clear flags */
   WaitEvent_flag = false;
   ClearEvent_flag = false;

   /* Call function to test */
   ret = ciaaPOSIX_sleep(10);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* call WaitEvent function */
   TEST_ASSERT_TRUE(WaitEvent_flag);
   /* call ClearEvent function */
   TEST_ASSERT_TRUE(ClearEvent_flag);
   /* Task 0 and 1 are sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[1]);
   /* The other tasks are not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);
}

/** \brief test ciaaPOSIX_sleep
 **
 ** Call this function with incorrect paremeter
 **
 **/
void test_ciaaPOSIX_sleep_04(void) {

   /* Internal data */
   int ret;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);
   ciaaPOSIX_assert_StubWithCallback(ciaaPOSIX_assert_stub);

   /* Set Task ID */
   MyGetTaskIDTaskID = 0;


   /* Call function to test */
   ret = ciaaPOSIX_sleep(MAX_SECONDS);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   TEST_ASSERT_TRUE(assert_flag);

   /* Reset values */
   assert_flag = false;

   /* Call function to test */
   ret = ciaaPOSIX_sleep(10);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   TEST_ASSERT_FALSE(assert_flag);
}
/** \brief test ciaaPOSIX_usleep function
 **
 ** call ciaaPOSIX_usleep two times with correct paremeters
 **
 **/
void test_ciaaPOSIX_usleep_01(void) {
   /* Internal data */
   int ret;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);

   /* Set Task ID */
   MyGetTaskIDTaskID = 0;

   /* There are not sleeping tasks */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);

   /* Set initial condition for this test */
   ret = ciaaPOSIX_usleep(999999);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* call WaitEvent function */
   TEST_ASSERT_TRUE(WaitEvent_flag);
   /* call ClearEvent function */
   TEST_ASSERT_TRUE(ClearEvent_flag);
   /* Task 0 and 1 are sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[0]);
   /* The other tasks are not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);
}

/** \brief test ciaaPOSIX_usleep function
 **
 ** call ciaaPOSIX_usleep two times with inccorrect paremeters
 **
 **/
void test_ciaaPOSIX_usleep_02(void) {
   /* Internal data */
   int ret;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);

   /* Set Task ID */
   MyGetTaskIDTaskID = 0;

   /* There are not sleeping tasks */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);

   /* Set initial condition for this test */
   ret = ciaaPOSIX_usleep(1000000);

   /* ASSERTs */
   /* return ERROR */
   TEST_ASSERT_EQUAL_INT(-1, ret);
   /* There are not sleeping tasks */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);

   /* Set Task ID */
   MyGetTaskIDTaskID = 1;

   /* Call function to test */
   ret = ciaaPOSIX_usleep(MAX_USECONDS);

   /* ASSERTs */
   /* return ERROR */
   TEST_ASSERT_EQUAL_INT(-1, ret);
   /* There are not sleeping tasks */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);
}

/** \brief test ciaaPOSIX_sleepTick
 **
 ** there are not sleeping function, so nothing must be changed
 **
 **/
void test_ciaaPOSIX_sleepTick_01(void) {

   SetEvent_StubWithCallback(SetEvent_stub);

   /* Call function to test */
   ciaaPOSIX_sleepTick();

   /* There are not sleeping tasks */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);
}

/** \brief test ciaaPOSIX_sleepTick
 **
 ** there is a sleeping function, so the counter must be incremented
 **
 **/
void test_ciaaPOSIX_sleepTick_02(void) {
   /* Internal data */
   uint32_t ret;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);

   /* Initial conditions */
   /* Set Task ID */
   MyGetTaskIDTaskID = 0;
   ret = ciaaPOSIX_sleep(1);

   /* Call function to test */
   ciaaPOSIX_sleepTick();

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* Task 0 is sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[0]);
   /* The other tasks are not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);
}

/** \brief test ciaaPOSIX_sleepTick
 **
 ** There are sleeping two tasks, and then, one wakes up
 **
 **/
void test_ciaaPOSIX_sleepTick_03(void) {
   /* Internal data */
   uint32_t ret, i;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);

   /* Initial conditions */
   /* Set Task ID */
   MyGetTaskIDTaskID = 0;
   ret = ciaaPOSIX_sleep(1);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* Task 0 is sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[0]);
   /* The other tasks are not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);

   /* Set Task ID */
   MyGetTaskIDTaskID = 2;
   ret = ciaaPOSIX_sleep(2);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* Task 0 and 1 are sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[2]);
   /* The other tasks are not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);

   /* Call function to test */
   for(i = 0; i < (1 * SLEEP_TIME_TO_COUNTS); i++)
   {
      ciaaPOSIX_sleepTick();
   }

   /* ASSERTs */
   /* Task 2 is sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[2]);
   /* The other tasks are not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   /* call SetEvent function */
   TEST_ASSERT_TRUE(SetEvent_flag);
}

/** \brief test ciaaPOSIX_sleepTick
 **
 ** There are sleeping one task, and then, It wakes up
 **
 **/
void test_ciaaPOSIX_sleepTick_04(void) {
   /* Internal data */
   uint32_t ret, i;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);

   /* Initial conditions */
   /* Set Task ID */
   MyGetTaskIDTaskID = 2;
   ret = ciaaPOSIX_sleep(2);

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* Task 0 and 1 are sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[2]);
   /* The other tasks are not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);

   /* Call function to test */
   for(i = 0; i < (2 * SLEEP_TIME_TO_COUNTS); i++)
   {
      ciaaPOSIX_sleepTick();
   }

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* There are not sleeping tasks */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[2]);
   /* call SetEvent function */
   TEST_ASSERT_TRUE(SetEvent_flag);
}

/** \brief test ciaaPOSIX_sleepTick
 **
 ** There are sleeping three tasks, and then, one wakes up
 **
 **/
void test_ciaaPOSIX_sleepTick_05(void) {
   /* Internal data */
   uint32_t ret, i;

   /* Set behaivor and stubs */
   GetTaskID_StubWithCallback(MyGetTaskID);
   ciaaPOSIX_printf_IgnoreAndReturn(-1);
   WaitEvent_StubWithCallback(WaitEvent_stub);
   ClearEvent_StubWithCallback(ClearEvent_stub);
   SetEvent_StubWithCallback(SetEvent_stub);

   /* Initial conditions */
   /* Set Task ID */
   MyGetTaskIDTaskID = 0;
   ret = ciaaPOSIX_sleep(1);

   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);

   /* Set Task ID */
   MyGetTaskIDTaskID = 1;
   ret = ciaaPOSIX_sleep(2);

   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);

   /* Set Task ID */
   MyGetTaskIDTaskID = 2;
   ret = ciaaPOSIX_sleep(3);

   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);

   /* Call function to test */
   for(i = 0; i < (1 * SLEEP_TIME_TO_COUNTS); i++)
   {
      ciaaPOSIX_sleepTick();
   }

   /* ASSERTs */
   /* return OK */
   TEST_ASSERT_EQUAL_INT(0, ret);
   /* The task 0 is not sleeping */
   TEST_ASSERT_FALSE(ciaaPOSIX_sleepsTest[0]);
   /* The others tasks are sleeping */
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[1]);
   TEST_ASSERT_TRUE(ciaaPOSIX_sleepsTest[2]);
   /* call SetEvent function */
   TEST_ASSERT_TRUE(SetEvent_flag);
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

