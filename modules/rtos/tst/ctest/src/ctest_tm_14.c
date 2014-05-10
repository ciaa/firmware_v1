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

/** \brief FreeOSEK Os Conformance Test for the Task Managment, Test Sequence 14
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_tm_14.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM Task Management
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM_14 Test Sequence 14
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090414 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"				/* include os header file */
#include "ctest_tm_14.h"	/* include test header file */
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

TASK(Task1)
{
	StatusType ret;

	Sequence(0);
	ret = ActivateTask(Task2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(1);
	ret = ActivateTask(Task3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(2);
	ret = ActivateTask(Task4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(3);
	ret = ActivateTask(Task5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(4);
	ret = ActivateTask(Task6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(5);
	ret = ActivateTask(Task7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(6);
	ret = ActivateTask(Task8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(7);
	ret = ActivateTask(Task9);
	ASSERT(OTHER, ret != E_OK);

	Sequence(8);
	ret = ActivateTask(Task10);
	ASSERT(OTHER, ret != E_OK);

	Sequence(9);
	ret = ActivateTask(Task11);
	ASSERT(OTHER, ret != E_OK);

	Sequence(10);
	ret = ActivateTask(Task12);
	ASSERT(OTHER, ret != E_OK);

	Sequence(11);
	ret = ActivateTask(Task13);
	ASSERT(OTHER, ret != E_OK);

	Sequence(12);
	ret = ActivateTask(Task14);
	ASSERT(OTHER, ret != E_OK);

	Sequence(13);
	ret = ActivateTask(Task15);
	ASSERT(OTHER, ret != E_OK);

	Sequence(14);
	ret = ActivateTask(Task16);
	ASSERT(OTHER, ret != E_OK);

	Sequence(15);
	ret = ClearEvent(Task1_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(16);
	ret = ClearEvent(Task1_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(17);
	ret = ClearEvent(Task1_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(18);
	ret = ClearEvent(Task1_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(19);
	ret = ClearEvent(Task1_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(20);
	ret = ClearEvent(Task1_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(21);
	ret = ClearEvent(Task1_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(22);
	ret = ClearEvent(Task1_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(23);
	TerminateTask();
}

TASK(Task2)
{
	StatusType ret;

	Sequence(24);
	ret = ClearEvent(Task2_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(25);
	ret = ClearEvent(Task2_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(26);
	ret = ClearEvent(Task2_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(27);
	ret = ClearEvent(Task2_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(28);
	ret = ClearEvent(Task2_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(29);
	ret = ClearEvent(Task2_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(30);
	ret = ClearEvent(Task2_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(31);
	ret = ClearEvent(Task2_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(32);
	TerminateTask();
}

TASK(Task3)
{
	StatusType ret;

	Sequence(33);
	ret = ClearEvent(Task3_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(34);
	ret = ClearEvent(Task3_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(35);
	ret = ClearEvent(Task3_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(36);
	ret = ClearEvent(Task3_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(37);
	ret = ClearEvent(Task3_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(38);
	ret = ClearEvent(Task3_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(39);
	ret = ClearEvent(Task3_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(40);
	ret = ClearEvent(Task3_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(41);
	TerminateTask();
}

TASK(Task4)
{
	StatusType ret;

	Sequence(42);
	ret = ClearEvent(Task4_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(43);
	ret = ClearEvent(Task4_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(44);
	ret = ClearEvent(Task4_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(45);
	ret = ClearEvent(Task4_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(46);
	ret = ClearEvent(Task4_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(47);
	ret = ClearEvent(Task4_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(48);
	ret = ClearEvent(Task4_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(49);
	ret = ClearEvent(Task4_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(50);
	TerminateTask();
}

TASK(Task5)
{
	StatusType ret;

	Sequence(51);
	ret = ClearEvent(Task5_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(52);
	ret = ClearEvent(Task5_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(53);
	ret = ClearEvent(Task5_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(54);
	ret = ClearEvent(Task5_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(55);
	ret = ClearEvent(Task5_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(56);
	ret = ClearEvent(Task5_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(57);
	ret = ClearEvent(Task5_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(58);
	ret = ClearEvent(Task5_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(59);
	TerminateTask();
}

TASK(Task6)
{
	StatusType ret;

	Sequence(60);
	ret = ClearEvent(Task6_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(61);
	ret = ClearEvent(Task6_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(62);
	ret = ClearEvent(Task6_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(63);
	ret = ClearEvent(Task6_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(64);
	ret = ClearEvent(Task6_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(65);
	ret = ClearEvent(Task6_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(66);
	ret = ClearEvent(Task6_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(67);
	ret = ClearEvent(Task6_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(68);
	TerminateTask();
}

TASK(Task7)
{
	StatusType ret;

	Sequence(69);
	ret = ClearEvent(Task7_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(70);
	ret = ClearEvent(Task7_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(71);
	ret = ClearEvent(Task7_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(72);
	ret = ClearEvent(Task7_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(73);
	ret = ClearEvent(Task7_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(74);
	ret = ClearEvent(Task7_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(75);
	ret = ClearEvent(Task7_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(76);
	ret = ClearEvent(Task7_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(77);
	TerminateTask();
}

TASK(Task8)
{
	StatusType ret;

	Sequence(78);
	ret = ClearEvent(Task8_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(79);
	ret = ClearEvent(Task8_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(80);
	ret = ClearEvent(Task8_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(81);
	ret = ClearEvent(Task8_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(82);
	ret = ClearEvent(Task8_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(83);
	ret = ClearEvent(Task8_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(84);
	ret = ClearEvent(Task8_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(85);
	ret = ClearEvent(Task8_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(86);
	TerminateTask();
}

TASK(Task9)
{
	StatusType ret;

	Sequence(87);
	ret = ClearEvent(Task9_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(88);
	ret = ClearEvent(Task9_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(89);
	ret = ClearEvent(Task9_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(90);
	ret = ClearEvent(Task9_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(91);
	ret = ClearEvent(Task9_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(92);
	ret = ClearEvent(Task9_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(93);
	ret = ClearEvent(Task9_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(94);
	ret = ClearEvent(Task9_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(95);
	TerminateTask();
}

TASK(Task10)
{
	StatusType ret;

	Sequence(96);
	ret = ClearEvent(Task10_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(97);
	ret = ClearEvent(Task10_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(98);
	ret = ClearEvent(Task10_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(99);
	ret = ClearEvent(Task10_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(100);
	ret = ClearEvent(Task10_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(101);
	ret = ClearEvent(Task10_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(102);
	ret = ClearEvent(Task10_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(103);
	ret = ClearEvent(Task10_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(104);
	TerminateTask();
}

TASK(Task11)
{
	StatusType ret;

	Sequence(105);
	ret = ClearEvent(Task11_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(106);
	ret = ClearEvent(Task11_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(107);
	ret = ClearEvent(Task11_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(108);
	ret = ClearEvent(Task11_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(109);
	ret = ClearEvent(Task11_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(110);
	ret = ClearEvent(Task11_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(111);
	ret = ClearEvent(Task11_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(112);
	ret = ClearEvent(Task11_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(113);
	TerminateTask();
}

TASK(Task12)
{
	StatusType ret;

	Sequence(114);
	ret = ClearEvent(Task12_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(115);
	ret = ClearEvent(Task12_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(116);
	ret = ClearEvent(Task12_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(117);
	ret = ClearEvent(Task12_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(118);
	ret = ClearEvent(Task12_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(119);
	ret = ClearEvent(Task12_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(120);
	ret = ClearEvent(Task12_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(121);
	ret = ClearEvent(Task12_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(122);
	TerminateTask();
}

TASK(Task13)
{
	StatusType ret;

	Sequence(123);
	ret = ClearEvent(Task13_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(124);
	ret = ClearEvent(Task13_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(125);
	ret = ClearEvent(Task13_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(126);
	ret = ClearEvent(Task13_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(127);
	ret = ClearEvent(Task13_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(128);
	ret = ClearEvent(Task13_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(129);
	ret = ClearEvent(Task13_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(130);
	ret = ClearEvent(Task13_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(131);
	TerminateTask();
}

TASK(Task14)
{
	StatusType ret;

	Sequence(132);
	ret = ClearEvent(Task14_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(133);
	ret = ClearEvent(Task14_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(134);
	ret = ClearEvent(Task14_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(135);
	ret = ClearEvent(Task14_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(136);
	ret = ClearEvent(Task14_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(137);
	ret = ClearEvent(Task14_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(138);
	ret = ClearEvent(Task14_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(139);
	ret = ClearEvent(Task14_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(140);
	TerminateTask();
}

TASK(Task15)
{
	StatusType ret;

	Sequence(141);
	ret = ClearEvent(Task15_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(142);
	ret = ClearEvent(Task15_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(143);
	ret = ClearEvent(Task15_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(144);
	ret = ClearEvent(Task15_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(145);
	ret = ClearEvent(Task15_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(146);
	ret = ClearEvent(Task15_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(147);
	ret = ClearEvent(Task15_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(148);
	ret = ClearEvent(Task15_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(149);
	TerminateTask();
}

TASK(Task16)
{
	StatusType ret;

	Sequence(150);
	ret = ClearEvent(Task16_Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(151);
	ret = ClearEvent(Task16_Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(152);
	ret = ClearEvent(Task16_Event3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(153);
	ret = ClearEvent(Task16_Event4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(154);
	ret = ClearEvent(Task16_Event5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(155);
	ret = ClearEvent(Task16_Event6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(156);
	ret = ClearEvent(Task16_Event7);
	ASSERT(OTHER, ret != E_OK);

	Sequence(157);
	ret = ClearEvent(Task16_Event8);
	ASSERT(OTHER, ret != E_OK);

	Sequence(158);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

