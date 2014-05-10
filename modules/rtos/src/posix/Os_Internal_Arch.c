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

/** \brief FreeOSEK Os Internal Arch Implementation File
 **
 ** \file posix/Os_Internal_Arch.c
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
 * 20090719 v0.1.3 MaCe rename file to Os_
 * 20090424 v0.1.2 MaCe use the right counter macros
 * 20090130 v0.1.1 MaCe change type uint8_least to uint8f
 * 20080713 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
uint8	InterruptState;

mqd_t MessageQueue;

struct mq_attr MessageQueueAttr;

struct sigaction MessageSignal;

struct sigaction KillSignal;

pid_t OsekProcessID;

struct sigevent SignalEvent;

uint32 OsekHWTimer0;

InterruptFlagsType InterruptFlag;

#ifdef posix64
uint64 PosixStack;

uint64 OsekStack;
#else
uint32 PosixStack;

uint32 OsekStack;
#endif

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void OSEK_ISR_HWTimer0(void)
{
#if (defined HWCOUNTER0)
#if (ALARMS_COUNT != 0)
	IncrementCounter(HWCOUNTER0, 1);
#endif /* #if (ALARMS_COUNT != 0) */
#endif /* #if (defined HWCOUNTER0) */
}

void OSEK_ISR_HWTimer1(void)
{
#if (defined HWCOUNTER1)
#if (ALARMS_COUNT != 0)
	IncrementCounter(HWCOUNTER1, 1);
#endif /* #if (ALARMS_COUNT != 0) */
#endif /* #if (defined HWCOUNTER1) */
}

void PosixInterruptHandler(int status)
{
	uint8 msg[10];
	ssize_t mq_ret;

	mq_ret = mq_receive(MessageQueue, (char*)msg, sizeof(msg), NULL);
	if (mq_ret > 0)
	{
		if (msg[0] < 32)
		{
			/* printf("Interrupt: %d\n",msg[0]); */
			if ( (InterruptState) &&
				  ( (InterruptMask & (1 << msg[0] ) )  == 0 ) )
			{
				InterruptTable[msg[0]]();
			}
			else
			{
				InterruptFlag |= 1 << msg[0];
			}
		}

	}
	else
	{
		switch(errno)
		{
			case EAGAIN:
				printf("Queue Empty\n");
				break;
			case EBADF:
				printf("Not valued queue descriptor\n");
				break;
			case EMSGSIZE:
				printf("Message buffer to small\n");
				break;
			case EINTR:
				printf("Reception interrupted by a signal\n");
				break;
			default:
				printf("other error\n");
				break;
		}
		printf("Error by reading the Message Queue, returned value: %d, error number: %d\n",(int)mq_ret,errno);
	}

	if (mq_notify(MessageQueue, &SignalEvent) == -1)
	{
		printf("Error: Message Notification can not be activated, error: %d.\n",errno);
		sleep(3);
	}
}

void HWTimerFork(uint8 timer)
{
	int mq_ret;
	char msg;
	struct timespec rqtp;
   int lasterror = -1;

	if (timer <= 2)
	{
		/* set timer interrupts
         * HWCOUNTER0: Interrupt 4
         * HWCOUNTER1: Interrupt 5
		 */
		msg = timer + 4;

		/* intererupt every
         * 0 seconds and
         * 10 ms */
		rqtp.tv_sec=0;
    	rqtp.tv_nsec=100000000;

		while(1)
		{
			/* send message */
			mq_ret = mq_send(MessageQueue, &msg, sizeof(msg), 0);
			if (mq_ret < 0)
			{
				switch(errno)
				{
					case EAGAIN:
                  if (lasterror != errno) {
                     lasterror = errno;
      					printf("Error HW Timer can not generate an interrupt: %s\n", strerror(errno));
                  }
						break;
					case EBADF:
						printf("Not valued queue descriptor\n");
						break;
					case EMSGSIZE:
						printf("Message buffer to small\n");
						break;
					case EINTR:
						printf("Reception interrupted by a signal\n");
						break;
					default:
						printf("other error\n");
						break;
				}
				sleep(1);
			}
			else
			{
				if (-1 != lasterror)
				{
					printf("HW Timer generates interrupts\n");
					lasterror = -1;
				}
			}
			nanosleep(&rqtp,NULL);
		}
	}
	exit(0);
}

void OsekKillSigHandler(int status)
{
	PreCallService();
	mq_unlink("/FreeOSEK");
	exit(0);
	PostCallService();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

