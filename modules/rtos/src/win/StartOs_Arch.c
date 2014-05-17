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
 ** \file win/StartOs_Arch.c
 ** \arch win
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
 * 20080810 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

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
      SetEntryPoint(loopi)
	}

	/* set event */
	MessageSignal.sa_handler = PosixInterruptHandler;
	(void)sigemptyset(&MessageSignal.sa_mask);
	MessageSignal.sa_flags = 0;

	if (sigaction(SIGUSR1, &MessageSignal, NULL) == -1)
	{
		printf("Error: SIGUSR1 can not be configured, error number: %d\n", errno);
	}

	/* create shared memory */
   if ((SharedMemory = shmget(1, sizeof(MessageQueueType), IPC_CREAT | 0666)) < 0) {
      printf("shmget error\n");
      exit(1);
   }

   /* atach to the segment */
   MessageQueue = (MessageQueueType*) shmat(SharedMemory, NULL, 0);
   if (MessageQueue == (MessageQueueType *) -1)
   {
      printf("shmat error\n");
      exit(1);
   }

	if (fork() == 0)
	{
		HWTimerFork(0);
	}

	if (fork() == 0)
	{
		HWTimerFork(1);
	}

	/* enable interrupts */
	InterruptState = 1;

	/* enable timer interrupt */
	InterruptMask = 16;

	SaveWinStack();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

