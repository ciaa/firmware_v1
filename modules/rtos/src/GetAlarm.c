

/** \brief FreeOSEK GetAlarm Implementation File
 **
 ** This file implements the GetAlarm API
 **
 ** \file GetAlarm.c
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
 * 20090418 v0.1.3 MaCe bugfix E_OS_NOFUNC can be always returned, not only in
 *						 extended mode.
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
StatusType GetAlarm
(
	AlarmType AlarmID,
	TickRefType Tick
)
{
	/* \req OSEK_SYS_3.20 The system service StatusType
	 ** GetAlarm ( AlarmType AlarmID, TickRefType Tick) shall be defined */

	/* \req OSEK_SYS_3.20.2 Possible return values in Standard mode is E_OK */
	StatusType ret = E_OK;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	/* check if it handle of a valid AlarmID */
	if (AlarmID >= ALARMS_COUNT)
	{
		/* \req OSEK_SYS_3.20.3-1/2 Extra possible return values in Extended mode
		 ** are E_OS_NOFUNC, E_OS_ID */
		ret = E_OS_ID;
	}
	else
#endif /* #if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) */
	/* check if the alarm is running */
	if(AlarmsVar[AlarmID].AlarmState == 0)
	{
		/* \req OSEK_SYS_3.20.3-2/2 Extra possible return values in Extended mode
		 ** are E_OS_NOFUNC, E_OS_ID */
		ret = E_OS_NOFUNC;
	}
	else

	{
		/* \req OSEK_SYS_3.20.1 The system service GetAlarm shall return the
		 ** relative value in ticks before the alarm AlarmID expires */
		*Tick = AlarmsVar[AlarmID].AlarmTime;
	}

#if (HOOK_ERRORHOOK == ENABLE)
	/* \req OSEK_ERR_1.3-12/xx The ErrorHook hook routine shall be called if a
	 ** system service returns a StatusType value not equal to E_OK.*/
	/* \req OSEK_ERR_1.3.1-12/xx The hook routine ErrorHook is not called if a
	 ** system service is called from the ErrorHook itself. */
   if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
   {
	  SetError_Api(OSServiceId_GetAlarm);
      SetError_Param1(AlarmID);
      SetError_Param2((size_t)Tick);
      SetError_Ret(ret);
      SetError_Msg("GetAlarm returns != than E_OK");
      SetError_ErrorHook();
   }
#endif /* #if (HOOK_ERRORHOOK == ENABLE) */

	return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

