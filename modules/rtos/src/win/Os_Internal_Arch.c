

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
   __asm__ __volatile__ ("movl %%esp, %%eax; addl $16, %%eax; movl %%eax, %0;" : "=g" (TasksConst[OldTask].TaskContext->tss_esp) : : "%eax" );

   /* save actual ebp */
   __asm__ __volatile__ ("movl %%ebp, %%eax; addl $48, %%eax; movl %%eax, %0;" : "=g" (TasksConst[OldTask].TaskContext->tss_ebp) : : "%eax" );

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

