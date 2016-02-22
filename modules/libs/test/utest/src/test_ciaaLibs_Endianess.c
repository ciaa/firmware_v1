/* Copyright 2015, Daniel Cohen
 * Copyright 2015, Esteban Volentini
 * Copyright 2015, Matias Giori
 * Copyright 2015, Franco Salinas
 * Copyright 2015, Pablo Alcorta
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

/** \brief this file implements the unit tests for the functions of the file ciaaLibs_Endianess
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup update Implementation
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DC           Daniel Cohen
 * EV           Esteban Volentini
 * MG           Matias Giori
 * FS           Franco Salinas
 * PA           Pablo Alcorta
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151124 v0.0.1  PA  first initial version
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaLibs_Endianess.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void setUp(void)
{
}

void tearDown(void)
{
}

void test_ciaaLibs_utilsNtohsBigEnd (){
   #if CIAAPLATFORM_BIGENDIAN == 0
   TEST_ASSERT_EQUAL_INT (0x3412 ,ciaaLibs_utilsNtohs(0x1234));
   #else
   TEST_ASSERT_EQUAL_INT (0x1234,ciaaLibs_utilsNtohs (0x1234));
   #endif
}

void test_ciaaLibs_utilsNtohlBigEnd (){
   #if CIAAPLATFORM_BIGENDIAN == 0
   TEST_ASSERT_EQUAL_INT (0x78563412 ,ciaaLibs_utilsNtohl(0x12345678));
   #else
   TEST_ASSERT_EQUAL_INT (0x12345678,ciaaLibs_utilsNtohl (0x12345678));
   #endif
}

void test_ciaaLibs_utilsNtohllBigEnd (){
   #if CIAAPLATFORM_BIGENDIAN == 0
   TEST_ASSERT_EQUAL_INT (0x6758231478563412 ,ciaaLibs_utilsNtohll (0x1234567814235867));
   #else
   TEST_ASSERT_EQUAL_INT (0x1234567814235867,ciaaLibs_utilsNtohll (0x1234567814235867));
   #endif
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


