

#ifndef _MAIN_H_
#define _MAIN_H_
/** \brief Module Test Header File
 **
 ** \file mtst01/inc/main.h
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_MT Module Tests
 ** @{ */
/** \addtogroup FreeOSEK_Os_MT_01 Test 1
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090130 v0.1.1 MaCe add MAIN_MEMMAP check
 * 20081205 v0.1.0 MaCe	initial version
 */

/*==================[inclusions]=============================================*/
#include "Types.h"

/*==================[macros]=================================================*/
#define MAIN_MEMMAP DISABLE

#define INVALID_TASK 0xFE

#define INVALID_RESOURCE 0xFE

#define INVALID_ALARM 0xFF

#define mt_GetMark() (mt_StateCounter)

#define mt_SetMark(mark)                                                   \
   {                                                                       \
      if ( (mark) == 0 )                                                   \
      {                                                                    \
         mt_StateCounter = 0;                                              \
      }                                                                    \
      else if ( (mt_StateCounter+1) == (mark) )                            \
      {                                                                    \
         mt_StateCounter++;                                                \
      }                                                                    \
      else                                                                 \
      {                                                                    \
      }                                                                    \
   }

#define mt_SetTaskAssert(state) (mt_TaskAssert = (state) )

#define mt_GetTaskAssert() (mt_TaskAssert)

#define TASKASSERT(cond)                                                                              \
   {                                                                                                  \
      if (cond)                                                                                       \
      {                                                                                               \
         ASSERT_ARCH();                                                                               \
         mt_TaskAssert = TS_FAILED;                                                                   \
      }                                                                                               \
   }

#define mt_SetTestCase(testcase) (mt_TestCase = (testcase))

#define mt_GetTestCase() (mt_TestCase)

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief main function
 **
 ** Project main function. This function is called after the c conformance
 ** initialisation. This function shall call the StartOS in the right
 ** Application Mode. The StartOS API shall never return.
 **
 **/
int main
(
   void
);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _MAIN_H_ */

