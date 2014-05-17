

/** \brief FreeOSEK Os Arch Implementation File
 **
 ** \file win/Os_Arch.c
 ** \arch win
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
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090819 v0.2.1 MaCe rename file to Os_
 * 20080922 v0.2.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
InterruptFlagsType InterruptMask;

InterruptStateType InterruptState;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void ScheduleInterrupts(void)
{
	int loopi = 0;
	uint32 InterruptToBeExecuted;

	if (InterruptState)
	{
		InterruptToBeExecuted = ( InterruptFlag & ( (InterruptFlagsType) ~InterruptMask ) );
		while(InterruptToBeExecuted != 0)
		{
			if (InterruptToBeExecuted & 1)
			{
				InterruptFlag &= ~(1<<loopi);

				InterruptTable[loopi]();
			}

			InterruptToBeExecuted >>=1;
			loopi++;
		}
	}
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

