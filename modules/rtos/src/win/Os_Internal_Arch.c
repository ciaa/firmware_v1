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
 ** \file win/Os_Internal_Arch.c
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
 * 20090819 v0.1.1 MaCe rename file to Os_
 * 20080713 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[typedef]================================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
MessageQueueType *MessageQueue;

/*==================[external data definition]===============================*/
uint8	InterruptState;

struct sigaction MessageSignal;

/* struct sigaction KillSignal; */

/* pid_t OsekProcessID; */

/* struct sigevent SignalEvent; */

uint32 OsekHWTimer0;

InterruptFlagsType InterruptFlag;

int SharedMemory;

uint32 WinStack;

uint32 OsekStack;

/*==================[internal functions definition]==========================*/
/** \brief SendMessage
 **
 ** This function transmit a message to the indicated
 ** message queue.
 **
 ** \param[in] Msg pointer to the Message Queue
 ** \param[in] Val value to be transmiteed
 ** \return 0 if error occurs
 **         1 if no error
 **/
uint8 SendMessage(MessageQueueType *Msg, uint8 Val)
{
   uint8 ret = 1;

   if (Msg == NULL)
   {
      ret = 0;
   }
   else
   {

      Msg->Lock++;
      while(Msg->Lock != 1)
      {
         Msg->Lock--;
         osekpause();
         Msg->Lock++;
      }

      if (Msg->Count >= MESSAGE_QUEUE_LENGTH)
      {
         ret = 0;
      }
      else
      {
         Msg->Buffer[Msg->Last] = Val;
         Msg->Last = (Msg->Last + 1) % MESSAGE_QUEUE_LENGTH;
         Msg->Count++;
      }

      Msg->Lock--;
   }

   return ret;
}

/** \brief ReceiveMessage
 **
 ** This function receive a message from the indicated message
 ** queue.
 **
 ** \param[in] Msg pointer to the Message Queue
 ** \param[in] Val variabel where the received message shall be stored
 ** \return 0 if an error occurs
 **         1 if no errors.
 **/
uint8 ReceiveMessage(MessageQueueType *Msg, uint8 *Val)
{
   uint8 ret = 1;

   if (Msg == NULL)
   {
      ret = 0;
   }
   else
   {

      Msg->Lock++;
      while(Msg->Lock != 1)
      {
         Msg->Lock--;
         osekpause();
         Msg->Lock++;
      }

      if (Msg->Count == 0)
      {
         ret = 0;
      }
      else
      {
         *Val = Msg->Buffer[Msg->First];
         Msg->First = (Msg->First + 1) % MESSAGE_QUEUE_LENGTH;
         Msg->Count--;
      }

      Msg->Lock--;

   }

   return ret;
}

/*==================[external functions definition]==========================*/
void CallTask(TaskType OldTask, TaskType NewTask)
{
   /* save actual esp */
   __asm__ __volatile__ ("movl %%esp, %%eax; addl $12, %%eax; movl %%eax, %0;" : "=g" (TasksConst[OldTask].TaskContext->tss_esp) : : "%eax" );

   /* save actual ebp */
   __asm__ __volatile__ ("movl %%ebp, %%eax; addl $16, %%eax; movl %%eax, %0;" : "=g" (TasksConst[OldTask].TaskContext->tss_ebp) : : "%eax" );

   /* save return eip */
   __asm__ __volatile__ ("movl 4(%%ebp), %%eax; movl %%eax, %0" : "=g" (TasksConst[OldTask].TaskContext->tss_eip) : : "%eax");

   /* load new stack pointer */
   __asm__ __volatile__ ("movl %0, %%esp;" : :  "g" (TasksConst[NewTask].TaskContext->tss_esp));

   /* load new ebp and jmp to the new task */
   __asm__ __volatile__ ("movl %0, %%ebx;" \
                         "movl %1, %%ebp;" \
                         "jmp *%%ebx;" : :  "g" (TasksConst[NewTask].TaskContext->tss_eip), "g" (TasksConst[NewTask].TaskContext->tss_ebp));
}

void CounterInterrupt(CounterType CounterID)
{
	uint8f loopi;
	AlarmType AlarmID;

	/* increment counter */
	CountersVar[CounterID].Time = ( CountersVar[CounterID].Time + 1 ) % CountersConst[CounterID].MaxAllowedValue;

	/* for alarms on this counter */
	for(loopi = 0; loopi < CountersConst[CounterID].AlarmsCount; loopi++)
	{
		/* get alarm id */
		AlarmID = CountersConst[CounterID].AlarmRef[loopi];

		/* check if the alarm is eanble */
		if (AlarmsVar[AlarmID].AlarmState == 1)
		{
			/* decrement alarm time */
			AlarmsVar[AlarmID].AlarmTime--;

			/* check if alarm time was reached */
			if (AlarmsVar[AlarmID].AlarmTime == 0)
			{
				/* check if new alarm time has to be set */
				if(AlarmsVar[AlarmID].AlarmCycleTime == 0)
				{
					/* disable alarm if no cycle was configured */
					AlarmsVar[AlarmID].AlarmState = 0;
				}
				else
				{
					/* set new alarm cycle */
					AlarmsVar[AlarmID].AlarmTime = AlarmsVar[AlarmID].AlarmCycleTime;
				}

				/* check the alarm action */
				switch(AlarmsConst[AlarmID].AlarmAction)
				{
					case INCREMENT:
						/* call counter function */
						CounterInterrupt(AlarmsConst[AlarmID].AlarmActionInfo.Counter);
						break;
					case ACTIVATETASK:
						/* activate task */
						ActivateTask(AlarmsConst[AlarmID].AlarmActionInfo.TaskID);
						break;
					case ALARMCALLBACK:
						/* callback */
						if(AlarmsConst[AlarmID].AlarmActionInfo.CallbackFunction != NULL)
						{
							AlarmsConst[AlarmID].AlarmActionInfo.CallbackFunction();
						}
						break;
#if (NO_EVENTS == DISABLE)
					case SETEVENT:
						/* set event */
						SetEvent(AlarmsConst[AlarmID].AlarmActionInfo.TaskID, AlarmsConst[AlarmID].AlarmActionInfo.Event);
						break;
#endif
					default:
						/* some error ? */
						break;
				}
			}
		}
	}
}

void OSEK_ISR_HWTimer0(void)
{
	CounterInterrupt(0);
}

void OSEK_ISR_HWTimer1(void)
{
	CounterInterrupt(1);
}

void PosixInterruptHandler(int status)
{
	uint8 Msg;
	sint8 ret;

	ret = ReceiveMessage(MessageQueue, &Msg);
	if (ret)
	{
		if (Msg < 32)
		{
			/* printf("Interrupt: %d\n",msg[0]); */
			if ( (InterruptState) &&
				  ( (InterruptMask & (1 << Msg ) )  == 0 ) )
			{
				InterruptTable[Msg]();
			}
			else
			{
				InterruptFlag |= 1 << Msg;
			}
		}

	}
}

void HWTimerFork(uint8 timer)
{
	char Msg;
	struct timespec rqtp;
	pid_t ppid;

	ppid = getppid();
	MessageQueueType *MessageQueue;

	MessageQueue = (MessageQueueType*) shmat(SharedMemory, NULL, 0);

	if (timer <= 2)
	{
		Msg = timer + 4;

		rqtp.tv_sec=0;
   	rqtp.tv_nsec=1000000;

		while(1)
		{
			SendMessage(MessageQueue, Msg);
			kill(ppid, SIGUSR1);
			nanosleep(&rqtp,NULL);
		}
	}

	exit(0);
}

void OsekKillSigHandler(int status)
{
	exit(0);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

