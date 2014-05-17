/* Copyright 2008, 2009 Mariano Cerdeiro
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

