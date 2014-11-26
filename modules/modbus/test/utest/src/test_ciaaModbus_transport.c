/* Copyright 2014, Gustavo Muro
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

/** \brief This file implements the test of the modbus library
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Modbus CIAA Modbus
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * GMuro        Gustavo Muro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140906 v0.0.1 GMuro implement first tests
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaModbus_transport.h"
#include "ciaaModbus_config.h"
#include "mock_ciaaPOSIX_stdio.h"
#include "mock_ciaaModbus_ascii.h"
#include "os.h"
#include "string.h"

/*==================[macros and definitions]=================================*/

#define CIAA_MODBUS_TRASNPORT_FIL_DES     0


/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void)
{
   /* ignore calls GetResource */
   GetResource_CMockIgnoreAndReturn(E_OK);

   /* ignore calls ReleaseResource */
   ReleaseResource_CMockIgnoreAndReturn(E_OK);

   /*TODO use ciaaModbus_asciiOpen_ExpectAndReturn */
   ciaaModbus_asciiOpen_IgnoreAndReturn(0);

   ciaaModbus_transportInit();
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void)
{
}

void doNothing(void)
{
}

/** \brief Test ciaaModbus_transportOpen
 **
 **/
void test_ciaaModbus_transportOpen_01(void)
{
   int32_t hModbusTransp[10];

   hModbusTransp[0] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES,
         CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER);

   hModbusTransp[1] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES,
         CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE);

   hModbusTransp[2] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES,
         CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER);

   hModbusTransp[3] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES,
         CIAAMODBUS_TRANSPORT_MODE_RTU_SLAVE);

   hModbusTransp[4] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES,
         CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER);

   hModbusTransp[5] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES,
         CIAAMODBUS_TRANSPORT_MODE_TCP_SLAVE);

   hModbusTransp[6] = ciaaModbus_transportOpen(
         CIAA_MODBUS_TRASNPORT_FIL_DES,
         0xFF);

   TEST_ASSERT_NOT_EQUAL(-1, hModbusTransp[0]);
   TEST_ASSERT_NOT_EQUAL(-1, hModbusTransp[1]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[2]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[3]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[4]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[5]);
   TEST_ASSERT_EQUAL(-1, hModbusTransp[6]);

}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

