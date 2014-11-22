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

/** \brief FreeOSEK Os Internal Implementation File
 **
 ** \file Os_Internal.c
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
 * KLi          Kang Li
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090719 v0.1.4 MaCe rename file to Os_
 * 20090424 v0.1.3 MaCe correct initial task priority
 * 20090128 v0.1.2 MaCe add OSEK_MEMMAP check
 * 20081113 v0.1.1 KLi  Added memory layout attribute macros
 * 20080713 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
TaskType RunningTask;

ContextType ActualContext;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void AddReady(TaskType TaskID)
{
	TaskPriorityType priority;
	TaskRefType readylist;
	TaskTotalType maxtasks;
	TaskTotalType position;

	/* get task priority */
	priority = TasksConst[TaskID].StaticPriority;

	/* set the start priority for this task */
	TasksVar[TaskID].ActualPriority = priority;

	/* conver the priority to the array index */
	/* do not remove the -1 is needed. for example if READYLIST_COUNT is 4
	* the valida entries for this array are between 0 and 3, so the -1 is needed
	* since the lower priority is 0.
	*/
	priority = (READYLISTS_COUNT-1)-priority;

	/* get ready list */
	readylist = ReadyConst[priority].TaskRef;
	/* get max number of entries */
	maxtasks = ReadyConst[priority].ListLength;

	/* get position incrementtion */
	position = ReadyVar[priority].ListStart + ReadyVar[priority].ListCount;

	/* go arround maxtasks */
	/* this if works like  % instruction */
	if (position >= maxtasks)
	{
		position -= maxtasks;
	}

	/* set the task id in ready the list */
	readylist[position] = TaskID;
	/* increment the list counter */
	ReadyVar[priority].ListCount++;
}

void RemoveTask
(
	TaskType TaskID
)
{
	TaskPriorityType priority;
	TaskTotalType maxtasks;

	/* get task priority */
	priority = TasksConst[TaskID].StaticPriority;
	/* conver the priority to the array index */
	/* do not remove the -1 is needed. for example if READYLIST_COUNT is 4
	* the valida entries for this array are between 0 and 3, so the -1 is needed
	* since the lower priority is 0.
	*/
	priority = (READYLISTS_COUNT-1)-priority;

	/* get max number of entries */
	maxtasks = ReadyConst[priority].ListLength;

	/* increment the ListStart */
	ReadyVar[priority].ListStart = ReadyVar[priority].ListStart + 1;

	/* go arround maxtasks */
	/* this if works like a % instruction */
	if (ReadyVar[priority].ListStart >= maxtasks)
	{
		ReadyVar[priority].ListStart -= maxtasks;
	}

	/* decrement the count of ready tasks */
	ReadyVar[priority].ListCount--;
}

TaskType GetNextTask
(
	void
)
{
   /* if at least one resource is configured */
#if (RESOURCES_COUNT != 0)
   TaskType resTask = INVALID_TASK;
   uint8 prio = 0;
#endif /* #if (RESOURCES_COUNT != 0) */

	uint8f loopi;
	boolean found = FALSE;
	TaskType ret = INVALID_TASK;

	/* check in all ready lists */
	for (loopi = 0; ( loopi < READYLISTS_COUNT ) && (!found) ; loopi++)
	{
		/* if one or more tasks are ready */
		if (ReadyVar[loopi].ListCount > 0)
		{
			/* return the first ready task */
			ret = ReadyConst[loopi].TaskRef[ReadyVar[loopi].ListStart];

			/* set found true */
			found = TRUE;
		}
	}

   /* if at least one resource is configured */
#if (RESOURCES_COUNT != 0)
   for (loopi = 0; loopi < TASKS_COUNT; loopi++)
   {
      /* if at least one resource is occupied */
		if ( ( 0 != TasksVar[loopi].Resources ) &&
           /* and the prio is higher */
           ( TasksVar[loopi].ActualPriority > prio ) )
      {
         /* remember this task and its prio */
         resTask = loopi;
         prio = TasksVar[loopi].ActualPriority;
      }
   }

   /* if the resource task is a valid one */
   if ( (INVALID_TASK != resTask) &&
        /* and the resource task has the same or higher prio than the task
         * found in the ready list */
        (prio >= TasksVar[ret].ActualPriority) )
   {
      /* next task to be executed is the task using the resource */
      ret = resTask;
   }
#endif /* #if (RESOURCES_COUNT != 0) */

	return ret;
}

void OSEK_ISR_NoHandler(void)
{
	while(1);
}

#if (ALARMS_COUNT != 0)
AlarmIncrementType IncrementAlarm(AlarmType AlarmID, AlarmIncrementType Increment)
{
	AlarmIncrementType RestIncrements;
	AlarmIncrementType AlarmCount;
	CounterIncrementType CounterIncrement;

	/* init arlarms count */
	AlarmCount = 0;

	/* check if the increment is smaller than the expiration time */
	if ( AlarmsVar[AlarmID].AlarmTime > Increment )
	{
		/* decrement the alarm*/
		AlarmsVar[AlarmID].AlarmTime -= Increment;

		/* alarm doesn't expires now */
		RestIncrements = AlarmsVar[AlarmID].AlarmTime;
	}
	else
	{
		/* increment is graeter or equal than the alarm time, the alarm may
			expire one or more times */

		/* check if new alarm time has to be set */
		if(AlarmsVar[AlarmID].AlarmCycleTime == 0)
		{
			/* in case of a non cyclic alarm */

			/* alarm has expires 1 time */
			AlarmCount = 1;

			/* set alarm to 0 */
			AlarmsVar[AlarmID].AlarmTime = 0;

			/* disable alarm */
			AlarmsVar[AlarmID].AlarmState = 0;

			/* set rest increments to zero */
			RestIncrements = 0;
		}
		else
		{
			/* the alarm is cyclic */

			/* decrement alarm */
			while ( AlarmsVar[AlarmID].AlarmTime <= Increment )
			{
				/* add cycle time */
				AlarmsVar[AlarmID].AlarmTime += AlarmsVar[AlarmID].AlarmCycleTime;

				/* increment Alarms expiration times */
				AlarmCount++;
			}

			/* decrement the increments of this alarm */
			AlarmsVar[AlarmID].AlarmTime -= Increment;

			/* store the rest increments */
			RestIncrements = AlarmsVar[AlarmID].AlarmTime;
		}

		if (AlarmsConst[AlarmID].AlarmAction == INCREMENT)
		{
			/* call counter function */
			CounterIncrement = IncrementCounter(AlarmsConst[AlarmID].AlarmActionInfo.Counter, AlarmCount);

			/* re-calculate the rest of the increments */
			RestIncrements += AlarmsVar[AlarmID].AlarmCycleTime * ( CounterIncrement-1 );
		}
		else
		{
			/* execute the alarm so many times as needed */
			for ( ;AlarmCount > 0; AlarmCount--)
			{
				/* check alarm actions differents to INCREMENT */
				switch(AlarmsConst[AlarmID].AlarmAction)
				{
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
#if (NO_EVENTS == OSEK_DISABLE)
					case SETEVENT:
						/* set event */
						SetEvent(AlarmsConst[AlarmID].AlarmActionInfo.TaskID, AlarmsConst[AlarmID].AlarmActionInfo.Event);
						break;
#endif /* #if (NO_EVENTS == OSEK_DISABLE) */
					default:
						/* some error */
						/* possibly TODO, report an error */
						break;
				}
			}
		}
	}

	return RestIncrements;
}
#endif /* #if (ALARMS_COUNT != 0) */

#if (ALARMS_COUNT != 0)
CounterIncrementType IncrementCounter(CounterType CounterID, CounterIncrementType Increment)
{
	uint8f loopi;
	AlarmType AlarmID;
	AlarmIncrementType MinimalCount = -1;
	AlarmIncrementType TmpCount;

	/* increment counter */
	CountersVar[CounterID].Time+=Increment;

	/* check if the timer has an overvlow */
	while ( CountersVar[CounterID].Time >= CountersConst[CounterID].MaxAllowedValue )
	{
		/* reset counter */
		CountersVar[CounterID].Time -= CountersConst[CounterID].MaxAllowedValue;
	}

	/* for alarms on this counter */
	for(loopi = 0; loopi < CountersConst[CounterID].AlarmsCount; loopi++)
	{
		/* get alarm id */
		AlarmID = CountersConst[CounterID].AlarmRef[loopi];

		/* check if the alarm is eanble */
		if (AlarmsVar[AlarmID].AlarmState == 1)
		{
			/* increment alarm and get the next alarm time */
			TmpCount = IncrementAlarm(AlarmID, Increment);

			/* if the actual count is smaller */
			if (MinimalCount > TmpCount)
			{
				/* set it as minimal count */
				MinimalCount = TmpCount;
			}
		}
	}

	/* return the minimal increment */
	return (CounterIncrementType)MinimalCount;
}
#endif /* #if (ALARMS_COUNT != 0) */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

