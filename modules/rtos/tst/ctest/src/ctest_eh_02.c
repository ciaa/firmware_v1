

/** \brief FreeOSEK Os Conformance Test for the Error Handling, Test Sequence 2
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_eh_02.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_EH Error Handling
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_EH_02 Test Sequence 2
 ** @{ */


/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090418 v0.1.0 MaCe initial version based on old moduletest
 */

/*==================[inclusions]=============================================*/
#include "os.h"				/* include os header file */
#include "ctest_eh_02.h"	/* include test header file */
#include "ctest.h"			/* include ctest header file */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
const uint32f SequenceCounterOk = MAX_SEQUENCE;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
int main
(
	void
)
{
	/* start OS in AppMode 1 */
	StartOS(AppMode1);

	/* shall never return */
	while(1);

	return 0;
}

void ShutdownHook(void)
{
	Sequence(2);
	/* \treq EH_07 nmf B1B2E1E2 se CheckShutdownHook: ShutdownOS
	 *
	 * \result ShutdowHook is called after os shutdown
	 *
	 */
	ASSERT(EH_07,0);

	Sequence(3);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task1)
{
	AppModeType Mode;

	ASSERT(OTHER, 0);

	Sequence(0);
	/* \treq EH_01 nmf B1B2E1E2 se Call GetActiveApplicationMode()
	 *
	 * \result Return current application mode
	 */
	Mode = GetActiveApplicationMode();
	ASSERT(EH_01, Mode != AppMode1);

	Sequence(1);
	/* \treq EH_03 nmf B1B2E1E2 se Call ShutdownOS()
	 * Force error
	 *
	 * \result Shutdown operating system
	 */
	ASSERT(EH_03, 0);
	ShutdownOS(E_OK);
	ASSERT(EH_03, 1);
}

/* This task is not used, only to change the scheduling police */
TASK(Task2)
{
	TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

