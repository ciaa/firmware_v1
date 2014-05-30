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

#ifdef CPUTYPE
#if ( CPUTYPE == posix64 )
uint64 PosixStack;

uint64 OsekStack;
#elif ( CPUTYPE == posix32 )
uint32 PosixStack;

uint32 OsekStack;
#else /* #if ( CPUTYPE == posix64 ) */
#error Unknown CPUTYPE for ARCH posix
#endif /* #if ( CPUTYPE == posix64 ) */
#else /* #ifdef CPUTYPE */
#error CPUTPYE is not defined
#endif /* #idef CPUTYPE */

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void OSEK_ISR_HWTimer0(void)
{
#if (ALARMS_COUNT != 0)
	IncrementCounter(HardwareCounter, 1);
#endif /* #if (ALARMS_COUNT != 0) */
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
    	rqtp.tv_nsec=10000000;

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

