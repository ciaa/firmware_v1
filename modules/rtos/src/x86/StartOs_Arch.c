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

/** \brief FreeOSEK Os StartOs Architecture Dependece Implementation File
 **
 ** This file implements the StartOs Arch API
 **
 ** \file x86/StartOs_Arch.c
 ** \arch x86
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090130 v0.1.1 MaCe change type uint8_least to uint8f
 * 20080810 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"
#include "ciaaLibs_CircBuf.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
uint8 * OSEK_IntCircBuffer;

/*==================[external data definition]===============================*/
ciaaLibs_CircBufType * OSEK_IntCircBuf;

bool * Os_Terminate_Flag;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void StartOs_Arch(void)
{
	uint8f loopi;

	/* init every task */
	for( loopi = 0; loopi < TASKS_COUNT; loopi++)
	{
        /* init stack */
        ResetStack(loopi);
        /* set entry point */
        SetEntryPoint(loopi);
	}

   /* initialize singals handler */
   signal(SIGALRM,OsInterruptHandler);
   signal(SIGUSR1,OsInterruptHandler);
   signal(SIGCHLD,OsInterruptHandler);
   signal(SIGTERM,OsInterruptHandler);
   
   /* shared memory for circular buffer management block */
   OSEK_IntCircBuf = mmap(NULL,
         sizeof(ciaaLibs_CircBufType),
         PROT_READ | PROT_WRITE,
         MAP_SHARED | MAP_ANONYMOUS, -1, 0);

   /* shared memory for the circular buffer */
   OSEK_IntCircBuffer = mmap(NULL,
         sizeof(uint8) * 64,
         PROT_READ | PROT_WRITE,
         MAP_SHARED | MAP_ANONYMOUS, -1, 0);

		 /* init circular buffer */
   ciaaLibs_circBufInit(OSEK_IntCircBuf, OSEK_IntCircBuffer, 64);
 
   /* shared memory for the circular buffer */
   Os_Terminate_Flag = mmap(NULL,
         sizeof(bool),
         PROT_READ | PROT_WRITE,
         MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   *Os_Terminate_Flag = false;
   
   if (fork() == 0)
   {
      printf("Child %d\n", getpid());
      HWTimerFork(0);
   }
   printf("Parent %d\n", getpid());
   /* enable interrupts */
   InterruptState = 1;

   /* enable timer interrupt */
   InterruptMask = 16;

   SaveOsStack();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

