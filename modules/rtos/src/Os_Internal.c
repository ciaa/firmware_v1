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
#if (NO_EVENTS == DISABLE)
					case SETEVENT:
						/* set event */
						SetEvent(AlarmsConst[AlarmID].AlarmActionInfo.TaskID, AlarmsConst[AlarmID].AlarmActionInfo.Event);
						break;
#endif /* #if (NO_EVENTS == DISABLE) */
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

