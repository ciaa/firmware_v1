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
 ** \file posix/StartOs_Arch.c
 ** \arch posix
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
		TasksConst[loopi].TaskContext->uc_stack.ss_sp = TasksConst[loopi].StackPtr;		/* set stack pointer */
		TasksConst[loopi].TaskContext->uc_stack.ss_size = TasksConst[loopi].StackSize;	/* set stack size */
		TasksConst[loopi].TaskContext->uc_stack.ss_flags = 0;										/* clear stack flags */

		/* create task context */
		(void)getcontext(TasksConst[loopi].TaskContext);	/* get actual context as start point */
	}

	/* set message queue attributes */
	MessageQueueAttr.mq_flags = O_NONBLOCK;
	MessageQueueAttr.mq_maxmsg = 10;
	MessageQueueAttr.mq_msgsize = 8;
	MessageQueueAttr.mq_curmsgs = 0;

	/* init interrupts queue*/
	/* open the Queue with followings options:
 	 *		- name as defined in the compiler macro PROJECT
 	 *		- queue is read only
 	 *		- create the queue
 	 *		- if the queue exists return an error
 	 *		-
 	 */
	MessageQueue = mq_open("/FreeOSEK", O_RDWR | O_CREAT | O_NONBLOCK, 0666, &MessageQueueAttr);
	if (MessageQueue == (mqd_t)-1)
	{
		switch (errno)
		{
			case EINVAL:
				printf ("Error: Invalid argument\n");
				break;
			default:
				break;
		}
		printf ("Error: Message Queue for interrupts can not be configured, error: %d %s\n",errno, strerror(errno));
		sleep(2);
	}

	if (mq_getattr(MessageQueue, &MessageQueueAttr) == -1)
	{
		printf ("Error: Get Attribte error, error number: %d %s\n",errno, strerror(errno));
      sleep(2);
	}
	else
	{
		/* printf("Maxmsg: %d, Msgsize: %d, Curmsg: %d, Flags: %d\n", MessageQueueAttr.mq_maxmsg, MessageQueueAttr.mq_msgsize, MessageQueueAttr.mq_curmsgs, MessageQueueAttr.mq_flags);
		sleep(2); */
	}

	sigemptyset(&KillSignal.sa_mask);
	KillSignal.sa_handler = OsekKillSigHandler;
	KillSignal.sa_flags = 0;

	if (sigaction(SIGINT, &KillSignal, NULL) == -1)
	{
		printf("Error: SIGKILL can not be configured, error number: %d %s\n",errno, strerror(errno));
	}

	(void)sigemptyset(&MessageSignal.sa_mask);
	MessageSignal.sa_handler = PosixInterruptHandler;
	MessageSignal.sa_flags = 0;

	if (sigaction(SIGUSR1, &MessageSignal, NULL) == -1)
	{
		printf("Error: SIGUSR1 can not be configured, error number: %d %s\n",errno, strerror(errno));
	}

	SignalEvent.sigev_notify = SIGEV_SIGNAL;
	SignalEvent.sigev_signo = SIGUSR1;
	SignalEvent.sigev_value.sival_int = MessageQueue;
	SignalEvent.sigev_notify_function = NULL;
	SignalEvent.sigev_notify_attributes = NULL;

	if (mq_notify(MessageQueue, &SignalEvent) == -1)
	{
		printf("Error: Message Notification can not be activated, error: %d %s\n",errno, strerror(errno));
	}

	if (fork() == 0)
	{
		HWTimerFork(0);
	}

	/* enable interrupts */
	InterruptState = 1;

	/* enable timer interrupt */
	InterruptMask = 16;

	SavePosixStack();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

