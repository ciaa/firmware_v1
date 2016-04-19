/* Copyright 2016, Franco Bucafusco (BuckLabs)
 * Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * Copyright 2014, 2015 Mariano Cerdeiro
 *
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

#ifndef CIAAPLATFORMS_H
#define CIAAPLATFORMS_H
/** \brief Platform file
 **
 ** Platform header file
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Platform
 ** @{ */

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief check for ARCH/CPUTYPE and CPU */
#ifndef ARCH
#error ARCH is not defined, this define shall be included in the compile \
   command
#endif

#ifndef CPUTYPE
#error CPUTYPE is not defined, this define shall be included in the compile \
   command
#endif

#ifndef CPU
#error CPU is not defined, this define shall be included in the compile \
   command
#endif

/****** ARCH macro definitions ******/
/** \brief ARCH x86 */
#define x86                   1
/** \brief ARCH cortexM4 */
#define cortexM4              2
/** \brief ARCH mips */
#define mips                  3
/** \brief ARCH cortexM0 */
#define cortexM0              4
/** \brief ARCH msp430 */
#define msp430                5

/****** CPUTYPE macro definitions ******/
/*** CPUTYPES for ARCH=x86 ***/
/** \brief CPUTTYPE ia32 */
#define ia32                  1

/** \brief CPUTTYPE ia64 */
#define ia64                  2

/*** CPUTYPES for ARCH=cortexM4 and ARCH=cortexM0 ***/
#define lpc43xx               1

/*** CPUTYPES for ARCH=cortexM4 ***/
#define k60_120               2

/*** CPUTYPES for ARCH=mips ***/
#define pic32                 1

/*** CPUTYPES for ARCH=msp430 ***/
#define msp430g2x             1
#define msp430f5x_6x          2

/****** CPU macro definitions ******/
/*** CPU for ARCH=x86 CPUTYPE=ia32 ***/

/*** CPU for ARCH=x86 CPUTYPE=ia64 ***/

/*** CPU for ARCH=cortexM4 or ARCH=cortexM0 and CPUTYPE=lpc43xx ***/
#define lpc4337               1

/*** CPU for ARCH=cortexM4 CPUTYPE=k60_120 ***/
#define mk60fx512vlq15        2

/*** CPU for ARCH=mips CPUTYPE=pic32 ***/
#define pic32mz               3

/*** CPU for ARCH=msp430 CPUTYPE=msp430g2x_ ***/
#define msp430g2231    	      4

/*** CPU for ARCH=msp430 CPUTYPE=msp430f5x_6x ***/
#define msp430f5529           5

/****** BOARDS macro definitions ******/
/*** BOARD for ARCH=x86 CPUTYPE=ia32 ***/
#define ciaa_sim_ia32         1

/*** BOARD for ARCH=x86 CPUTYPE=ia64 ***/
#define ciaa_sim_ia64         2

/*** BOARD for ARCH=cortexM4 CPUTYPE=lpc43xx ***/
#define ciaa_nxp              3

/*** BOARD for ARCH=cortexM4 CPUTYPE=lpc43xx ***/
#define edu_ciaa_nxp          4

/*** CPU for ARCH=cortexM4 CPUTYPE=k60_120 ***/
#define ciaa_fsl              5

/*** BOARD for ARCH=mips CPUTYPE=pic32 ***/
#define ciaa_pic              6

/*** BOARD for ARCH=msp430 CPUTYPE=msp430_ ***/
#define msp_exp430f5529       7
#define ciaa_430              8

/****** CIAAPLATFORM_REGLENGTH ******/
/** \brief definition of the lenght of the register */
#if (ARCH == x86)

#if (CPUTYPE == ia32)
#define CIAAPLATFORM_REGLENGTH      32
#elif (CPUTYPE == ia64)
#define CIAAPLATFORM_REGLENGTH      64
#endif

#endif //ARCH == x86

#if (ARCH == msp430)
#if (  CPUTYPE == msp430  )
#define CIAAPLATFORM_REGLENGTH      16
#endif
#if (  CPUTYPE == msp430x  )
#define CIAAPLATFORM_REGLENGTH      32
#endif
#endif //ARCH == msp430

/****** CIAAPLATFORM_ENDIANESS ******/
#define CIAAPLATFORM_BIGENDIAN      0
#define CIAAPLATFORM_LITTLEENDIAN   1

#if (ARCH == posix)
#define CIAAPLATFORM_ENDIANESS CIAAPLATFORM_LITTLEENDIAN
#endif

#if ( (ARCH == cortexM4) && (CPUTPYE == lpc43xx) )

#endif

#if   ( ( ARCH == x86 ) && \
      ( ( CPUTYPE == ia32 ) || \
        ( CPUTYPE == ia64 ) ) )
#elif ( ( ARCH == cortexM4 ) && \
        ( CPUTYPE == lpc43xx ) && \
        ( CPU == lpc4337 ) )
#elif ( ( ARCH == cortexM0 ) && \
        ( CPUTYPE == lpc43xx ) && \
        ( CPU == lpc4337 ) )
#elif ( ( ARCH == cortexM4 ) && \
        ( CPUTYPE == k60_120 ) && \
        ( CPU == mk60fx512vlq15 ) )
#elif ( ( ARCH == mips ) && \
        ( CPUTYPE == pic32 ) && \
        ( CPU == pic32mz ) )
#elif ( ( ARCH == msp430 ) && \
        ( CPUTYPE == msp430g2x ) && \
        ( CPU == msp430g2231 ) )
#elif ( ( ARCH == msp430 ) && \
        ( CPUTYPE == msp430f5x_6x ) && \
        ( CPU == msp430f5529 ) )
#else
#error the entered architecture is not supported... :(
#endif

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAAPLATFORMS_H */
