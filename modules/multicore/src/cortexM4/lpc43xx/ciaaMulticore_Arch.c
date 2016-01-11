/* Copyright 2015, Pablo Ridolfi <pridolfi@proyecto-ciaa.com.ar>
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

 /** \brief Multicore module main source file.
  **
  ** With this module you will be able to design multicore applications if
  ** your current ARCH/CPU/CPUTYPE is supported.
  **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Multicore Multicore module
 ** @{ */

/*==================[inclusions]=============================================*/

#if( (lpc43xx == CPUTYPE) && (lpc4337 == CPU) )
   #include "chip.h"
#else
   #error Multicore is not supported by the current ARCH/CPUTYPE/CPU
#endif

#include "cr_start_m0.h"

#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stddef.h"
#include "ciaaMulticore.h"
#include "ciaaLibs_CircBuf.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

#define CIAA_MULTICORE_CORE_1_IMAGE ((uint8_t *)0x1B000000)

#define CIAA_MULTICORE_IPC_QUEUE_ADDR ((void *)0x20008000)

#define CIAA_MULTICORE_IPC_QUEUE_LEN (1024)

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

ciaaLibs_CircBufType * ipc_queue = (ciaaLibs_CircBufType *)(CIAA_MULTICORE_IPC_QUEUE_ADDR);

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern int32_t ciaaMulticore_init_Arch(void)
{
   int32_t rv = -1;

   /* Init IPC queue, only in master core */
   rv = ciaaLibs_circBufInit(ipc_queue,
      CIAA_MULTICORE_IPC_QUEUE_ADDR + sizeof(ciaaLibs_CircBufType),
      CIAA_MULTICORE_IPC_QUEUE_LEN);

   if(rv == 1)
   {
      NVIC_EnableIRQ(M0APP_IRQn);

      /* Start slave core */
      cr_start_m0(SLAVE_M0APP, CIAA_MULTICORE_CORE_1_IMAGE);

      rv = 0;
   }

   return rv;
}

extern int32_t ciaaMulticore_sendSignal_Arch(void)
{
   __DSB();
   __SEV();
   return 0;
}

ISR(M0_IRQHandler)
{
   static ciaaMulticore_ipcMsg_t m;
   static int32_t rv;

   LPC_CREG->M0APPTXEVENT = 0;   /* ACK */

   rv = ciaaMulticore_recvMessage(&m);
   if((rv > 0)&&(m.id.cpuid == CIAA_MULTICORE_CORE_0))
   {
      ciaaMulticore_dispatch_OSEK_API(m);
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
