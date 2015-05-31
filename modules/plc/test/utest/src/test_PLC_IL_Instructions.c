/* Copyright 2015, Mariano Cerdeiro
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

/** \brief This file implements the test os PLC IL
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup PLC PLC Module
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150522 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "PLC_Registers.h"
#include "PLC_IL_Instructions.h"

/*==================[macros and definitions]=================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data declaration]==============================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
PLC_SymbolicRegister CR;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void)
{
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void)
{
}

void test_PLC_IL_LD_BOOL_Literal_01(void)
{
   CR.VALUE.BOOL = 0;

   PLC_IL_LD_BOOL_Literal(0, N);

   TEST_ASSERT_EQUAL_INT(1, CR.VALUE.BOOL);
}

void test_PLC_IL_LD_BOOL_Literal_02(void)
{
   CR.VALUE.BOOL = 1;

   PLC_IL_LD_BOOL_Literal(1, N);

   TEST_ASSERT_EQUAL_INT(0, CR.VALUE.BOOL);
}

void test_PLC_IL_LD_BOOL_Literal_03(void)
{
   CR.VALUE.BOOL = 1;

   PLC_IL_LD_BOOL_Literal(0, C);

   TEST_ASSERT_EQUAL_INT(0, CR.VALUE.BOOL);
}

void test_PLC_IL_LD_BOOL_Literal_04(void)
{
   CR.VALUE.BOOL = 0;

   PLC_IL_LD_BOOL_Literal(1, C);

   TEST_ASSERT_EQUAL_INT(1, CR.VALUE.BOOL);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

