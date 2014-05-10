/* Copyright 2008, 2009, Mariano Cerdeiro
 *
 * This file is part of FreeOSEK.
 *
 * FreeOSEK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Linking FreeOSEK statically or dynamically with other modules is making a
 * combined work based on FreeOSEK. Thus, the terms and conditions of the GNU
 * General Public License cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of FreeOSEK give
 * you permission to combine FreeOSEK program with free software programs or
 * libraries that are released under the GNU LGPL and with independent modules
 * that communicate with FreeOSEK solely through the FreeOSEK defined interface.
 * You may copy and distribute such a system following the terms of the GNU GPL
 * for FreeOSEK and the licenses of the other code concerned, provided that you
 * include the source code of that other code when and as the GNU GPL requires
 * distribution of source code.
 *
 * Note that people who make modified versions of FreeOSEK are not obligated to
 * grant this special exception for their modified versions; it is their choice
 * whether to do so. The GNU General Public License gives permission to release
 * a modified version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this exception.
 *
 * FreeOSEK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FreeOSEK. If not, see <http://www.gnu.org/licenses/>.
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

	if (fork() == 0)
	{
		HWTimerFork(1);
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

