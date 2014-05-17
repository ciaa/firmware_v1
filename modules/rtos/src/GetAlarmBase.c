

/** \brief FreeOSEK Os ActivateTask Implementation File
 **
 ** This file implements the ActiveTask API
 **
 ** \file GetAlarmBase.c
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Global
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
 * 20090420 v0.1.4 MaCe correct AlarmBaseType members to lower cases
 * 20090417 v0.1.3 MaCe update license
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

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
StatusType GetAlarmBase
(
	AlarmType AlarmID,
	AlarmBaseRefType Info
)
{
	/* \req OSEK_SYS_3.19 The system service StatusType
	 ** GetAlarmBase ( AlarmType AlarmID, AlarmBaseRefType Info )
	 ** shall be defined. */

	/* \req OSEK_SYS_3.19.2 Possible return values in Standard mode is E_OK */
	StatusType ret = E_OK;

	CounterType counter;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	/* check that the AlarmID is in range */
	if(AlarmID >= ALARMS_COUNT)
	{
		/* \req OSEK_SYS_3.19.: Extra possible return values in Extended mode
		 ** is E_OS_ID */
		ret = E_OS_ID;
	}
	else
#endif
	{
		/* get counter of this alarm */
		counter = AlarmsConst[AlarmID].Counter;

		/* \req OSEK_SYS_3.19.1 The system service GetAlarmBase reads the alarm base
		 ** characteristics. The return value Info is a structure in which the
		 ** information of data type AlarmBaseType is stored */
		Info->maxallowedvalue = CountersConst[counter].MaxAllowedValue;
		Info->ticksperbase = CountersConst[counter].TicksPerBase;
		Info->mincycle = CountersConst[counter].MinCycle;
	}

#if ( (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) && \
      (HOOK_ERRORHOOK == ENABLE) )
	/* \req OSEK_ERR_1.3-12/xx The ErrorHook hook routine shall be called if a
	 ** system service returns a StatusType value not equal to E_OK.*/
	/* \req OSEK_ERR_1.3.1-12/xx The hook routine ErrorHook is not called if a
	 ** system service is called from the ErrorHook itself. */
	if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
	{
		SetError_Api(OSServiceId_GetAlarmBase);
		SetError_Param1(AlarmID);
		SetError_Param2((size_t)Info);
		SetError_Ret(ret);
		SetError_Msg("GetAlarmBase returns != than E_OK");
		SetError_ErrorHook();
	}
#endif

	return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

