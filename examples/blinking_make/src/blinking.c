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

/** \brief Project Main Entry Point
 **
 ** main function implementation file
 **
 ** \file example01/src/main.c
 **
 **/

/** \addtogroup Examples Examples
 ** @{ */
/** \addtogroup Example1 Example 1
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20080812 v0.1.0 MaCe       - initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "stdio.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief main function
 **
 ** Project main function. This function is called after the c conformance
 ** initialisation. This function shall call the StartOS in the right
 ** Application Mode. The StartOS API shall never return.
 **
 **/
int main
(
	void
)
{
//	printf("Hola Mundo 1\n");
	StartOS(AppMode1);
//	printf("Hola Mundo 2\n");
	/* shall never return */
	while(1);

	return 0;
}

void ErrorHook(void)
{
	printf("ErrorHook was called\n");
	printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());
	ShutdownOS(0);
}

TASK(InitTask) {
    printf("InitTask is running\n");

    ActivateTask(TaskA);

    Schedule();

    printf("InitTask sets TaskA Event1\n");
    SetEvent(TaskA, Event1);

    printf("InitTask is Terminating\n");
    TerminateTask();
}

TASK(TaskA) {
    printf("TaskA is running\n");

    printf("TaskA espera Event1\n");
    WaitEvent(Event1);
    printf("TaskA recibio la notficacion del Event1\n");

   
    ActivateTask(TaskB);

    printf("Pedimos recurso\n");
    GetResource(Res1);


    printf("Liberamos recurso\n");
    ReleaseResource(Res1);

    printf("TaskA is Terminating\n");
    TerminateTask();
}

TASK(TaskB) {
    printf("TaskB is running\n");

   ActivateTask(TaskC);
   ActivateTask(TaskC);

   SetRelAlarm(ActivateTaskC, 350, 100);

    printf("TaskB is Terminating\n");
    TerminateTask();
}

ISR(IsrName) {
}


TASK(TaskC) {
    //static int count = 0;

    printf("TaskC is running\n");

    

    printf("TaskC is Terminating\n");
    TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

