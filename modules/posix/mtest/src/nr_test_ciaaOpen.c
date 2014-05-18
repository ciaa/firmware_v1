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

/** \brief This file implements the test of the POSIX open function
 **
 ** This test is based on the description in
 **    - http://pubs.opengroup.org/onlinepubs/009695399/functions/open.html
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
#include "ciaaPOSIX_stdio.h"

/*==================[macros and definitions]=================================*/
#define INVALID_FILE_DESCRIPTOR         0xA53Cu

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static char const * const invalid_device = "/dev/unkown/device";
static char const * const valid_device = "/dev/UART/UART1";

/*==================[external data definition]===============================*/
ciaaMemory_pfMallocType ciaaMemory_pfMalloc = stub_malloc;

char const * const ciaaDevices_UART1 = "/dev/UART/UART1";

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

void doNothing(void) {
}


/** \brief test POSIX open function with an invalid file descriptor
 **
 ** try to open an invalid device
 **
 **/
void testOpenInvalidFileDescriptor(void) {
    int fd;

    void stub_malloc_StubWithCallback(doNothing);

    /* try to open an invalid device as read only */
    fd = ciaaPOSIX_open(invalid_device, O_RDONLY);
    TEST_ASSERT_TRUE(-1==fd);

    /* try to open an invalid device as write only */
    fd = ciaaPOSIX_open(invalid_device, O_WRONLY);
    TEST_ASSERT_TRUE(-1==fd);

    /* try to open an invalid device as write only */
    fd = ciaaPOSIX_open(invalid_device, O_RDWR);
    TEST_ASSERT_TRUE(-1==fd);
}

/** \brief test POSIX open function with a valid file descriptor
 **
 ** try to open a device
 **
 **/
void testOpen(void) {
    int fd;
    char buffer[1024];
    void* bufferptr;

    bufferptr = (void*)buffer;

    stub_malloc_IgnoreAndReturn(bufferptr);
    ciaaUART_open_ExpectAndReturn(valid_device, O_RDONLY, 1);


    /* try to open a valid device */
    fd = ciaaPOSIX_open(valid_device, O_RDONLY);
    TEST_ASSERT_TRUE(fd>0);

    /* close the device */
    close(fd);
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

