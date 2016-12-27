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

#ifndef CIAAPOSIX_UNISTD_H
#define CIAAPOSIX_UNISTD_H
/** \brief ciaa POSIX unistd header file
 **
 ** ciaa POSIX unistd header file
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stddef.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_unistd_nonstd.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief micro seconds type
 **
 ** Type to store a count of micro seconds. The valid range is between 0 and
 ** 1000000.
 **
 **/
#if ( !defined(_USECONDS_T_DECLARED) && !defined(__CYGWIN__) )
typedef uint32_t useconds_t;
#endif
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief ciaaPOSIX sleep
 **
 ** Sleeps the calling application for a number of seconds.
 **
 ** \param[in] seconds seconds to sleep the execution of the calling task.
 ** \return 0
 **
 ** \remarks in posix this funcion may also return some value in seconds,
 **          but this is not possible as far as signals are not supported.
 **/
extern uint32_t ciaaPOSIX_sleep(uint32_t seconds);

/** \brief ciaaPOSIX usleep
 **
 ** Sleeps the calling application for a number of micro seconds.
 **
 ** \param[in] useconds micro seconds to sleep the application. Value shall be
 **            between 0 and 1000000.
 ** \return 0 if success and -1 if an error occured
 **
 ** \remarks the function will sleep the application in a multiple of x
 **          miliseconds, depending how ciaaPOSIX_mainFunction is scheduled.
 **          The parameter useconds will be rounded to the next bigger possible
 **          value. If ciaaPOSIX_mainFunction is scheduled every 10 ms and a
 **          a useconds of 1 is passed, the sleep will take 10 ms this means
 **          9999 useconds more than requested. This behaviour may be improved
 **          in the future.
 **/
extern int32_t ciaaPOSIX_usleep(useconds_t useconds);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAAPOSIX_UNISTD_H */

