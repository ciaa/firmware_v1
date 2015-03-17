/* Copyright 2015, Mariano Cerdeiro
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

#ifndef CIAAPOSIX_IOCTL_BLOCK
#define CIAAPOSIX_IOCTL_BLOCK
/** \brief IO Control macros for block devices
 **
 ** This files contains the macros for IO control for block devices
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150221 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Request the device information
 **/
#define ciaaPOSIX_IOCTL_BLOCK_GETINFO     0x8000U

/** \brief Request a block to be erased
 **
 ** This ioctl command is only available in flash or block devices
 ** which needs to be cleared/erased before written
 **/
#define ciaaPOSIX_IOCTL_BLOCK_ERASE       0x8001U
/*==================[typedef]================================================*/
/** TODO document */
typedef struct {
   int eraseBeforeWrite : 1;
} ciaaDevices_blockFlagsType;

/** TODO document */
typedef struct {
   int32_t minRead;        /** <- minimal read size shall be 0 or 2^n, if 0 no minimal read size */
   int32_t maxRead;        /** <- maximal read size shall be 0 or 2^n, if 0 no maximal read size */
   int32_t minWrite;       /** <- minimal write size shall be 0 or 2^n, if 0 no minimal write size */
   int32_t maxWrite;       /** <- maximal write size shall be 0 or 2^n, if 0 no maximal write size */
   int32_t blockSize;      /** <- block size, shall be 2^n with n > 0 */
   uint32_t lastPosition;  /** <- position of end of file (device) */
   ciaaDevices_blockFlagsType flags;   /** <- information flags of the device */
} ciaaDevices_blockType;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAAPOSIX_IOCTL_BLOCK */

