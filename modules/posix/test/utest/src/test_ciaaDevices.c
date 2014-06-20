/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

/** \brief This file implements the test of the Devices
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */
/** \addtogroup ModuleTests Module Tests
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * yyyymmdd v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "string.h"
#include "ciaaDevices.h"
#include "test_ciaaDevices.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
ciaaDevices_deviceType const dev_uart0 = {
   "/dev/serial/uart/0",
   testOpen0,
   testClose0,
   testRead0,
   testWrite0,
   testIoctl0
};

ciaaDevices_deviceType const dev_uart1 = {
   "/dev/serial/uart/1",
   testOpen1,
   testClose1,
   testRead1,
   testWrite1,
   testIoctl1
};

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
int32_t testOpen0(uint8_t const * const path, uint8_t const oflag)
{
   return 0;
}

int32_t testClose0(int32_t fildes)
{
   return 0;
}

int32_t testIoctl0(int32_t const fildes, int32_t request, void* param)
{
   return 0;
}

int32_t testRead0(int32_t const fildes, uint8_t * const buf, uint32_t nbyte)
{
   return 0;
}

int32_t testWrite0(int32_t const fildes, uint8_t const * const buf, uint32_t nbyte)
{
   return 0;
}

int32_t testSeek0(int32_t const fildes, int32_t offset, uint8_t whence)
{
   return 0;
}

int32_t testOpen1(uint8_t const * const path, uint8_t const oflag)
{
   return 0;
}

int32_t testClose1(int32_t fildes)
{
   return 0;
}

int32_t testIoctl1(int32_t const fildes, int32_t request, void* param)
{
   return 0;
}

int32_t testRead1(int32_t const fildes, uint8_t * const buf, uint32_t nbyte)
{
   return 0;
}

int32_t testWrite1(int32_t const fildes, uint8_t const * const buf, uint32_t nbyte)
{
   return 0;
}

int32_t testSeek1(int32_t const fildes, int32_t offset, uint8_t whence)
{
   return 0;
}

/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void) {
   /* ignore calls to sem_init */
   ciaaPOSIX_sem_init_CMockIgnoreAndReturn(1);
   /* perform the initialization of ciaa Devices */
   ciaaDevices_init();
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void) {
}

/** \brief test POSIX get device before adding one
 **
 **/
void testGetDeviceBeforeAddAny(void) {
   ciaaDevices_deviceType const * device1 = (void*)1;
   ciaaDevices_deviceType const * device2 = (void*)2;

   ciaaPOSIX_sem_wait_CMockIgnoreAndReturn(1);
   ciaaPOSIX_sem_post_CMockIgnoreAndReturn(1);

   /* call getDevice before adding any device */
   device1 = ciaaDevices_getDevice("/dev/serial/uart/0");
   device2 = ciaaDevices_getDevice("/dev/serial/uart/1");

   TEST_ASSERT_TRUE(NULL == device1);
   TEST_ASSERT_TRUE(NULL == device2);
}

/** \brief test POSIX test get a device which has been added before
 **
 **/
void testGetAfterAddDevice(void) {
   ciaaDevices_deviceType const * device1;
   ciaaDevices_deviceType const * device2;

   /* add devices */
   ciaaDevices_addDevice(&dev_uart0);

   /* get uart 1 device */
   ciaaPOSIX_strncmp_StubWithCallback(strncmp);
   ciaaPOSIX_strlen_StubWithCallback(strlen);
   device1 = ciaaDevices_getDevice("/dev/serial/uart/0");
   device2 = ciaaDevices_getDevice("/dev/serial/uart/1");

   TEST_ASSERT_TRUE(&dev_uart0 == device1)
   TEST_ASSERT_TRUE(NULL == device2);
}

void testGetAfterAddingMultipleDevices(void) {
   ciaaDevices_deviceType const * device1;
   ciaaDevices_deviceType const * device2;

   /* add devices */
   ciaaDevices_addDevice(&dev_uart0);
   ciaaDevices_addDevice(&dev_uart1);

   /* get uart 1 device */
   ciaaPOSIX_strcmp_StubWithCallback(strcmp);
   device1 = ciaaDevices_getDevice("/dev/serial/uart/0");
   device2 = ciaaDevices_getDevice("/dev/serial/uart/1");

   TEST_ASSERT_TRUE(&dev_uart0 == device1)
   TEST_ASSERT_TRUE(&dev_uart1 == device2);
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

