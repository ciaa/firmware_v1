

/** \brief FreeOSEK Os CancelAlarm Implementation File
 **
 ** This file implements the CancelAlarm API
 **
 ** \file CancelAlarm.c
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
StatusType CancelAlarm
(
	AlarmType AlarmID
)
{
	/* \req OSEK_SYS_3.23 The system service StatusType
	 ** CancelAlarm ( AlarmType AlarmID ) shall be defined */

	/* \req OSEK_SYS_3.23.2-1/2 Possible return values in Standard mode are
	 ** E_OK, E_OS_NOFUNC */
	StatusType ret = E_OK;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	/* check if alarm id is in the valid range */
	if(AlarmID >= ALARMS_COUNT)
	{
		/* \req SEK_SYS_3.23.3: Extra possible return values in Extended mode ar
		 **  E_OS_ID */
		ret = E_OS_ID;
	}
	else
#endif
	if(AlarmsVar[AlarmID].AlarmState == 0)
	{
		/* \req OSEK_SYS_3.23.2-2/2 Possible return values in Standard mode are
		 ** E_OK, E_OS_NOFUNC */
		ret = E_OS_NOFUNC;
	}
	else
	{
		/* \req OSEK_SYS_3.23.1 The system service shall cancel the alarm AlarmID */
		AlarmsVar[AlarmID].AlarmState = 0;
	}

#if (HOOK_ERRORHOOK == ENABLE)
	/* \req OSEK_ERR_1.3-15/xx The ErrorHook hook routine shall be called if a
	 ** system service returns a StatusType value not equal to E_OK.*/
	/* \req OSEK_ERR_1.3.1-15/xx The hook routine ErrorHook is not called if a
	 ** system service is called from the ErrorHook itself. */
   if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
	{
		SetError_Api(OSServiceId_CancelAlarm);
		SetError_Param1(AlarmID);
		SetError_Ret(ret);
		SetError_Msg("CancelAlarm returns != than E_OK");
		SetError_ErrorHook();
	}
#endif

	return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

