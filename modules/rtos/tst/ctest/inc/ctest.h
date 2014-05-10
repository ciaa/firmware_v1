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

#ifndef _CTEST_H_
#define _CTEST_H_
/** \brief FreeOSEK Os Conformance Test
 **
 ** \file FreeOSEK/Os/tst/ctest/inc/ctest.h
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090403 v0.1.0 MaCe	initial version based on module tests
 */

/*==================[inclusions]=============================================*/
#include "Types.h"
#include "ctest_arch.h"
#include "ctest_cfg.h"
#include "Os_Internal.h"

/*==================[macros]=================================================*/
#define TM_01		0
#define TM_02		1
#define TM_03		2
#define TM_04		3
#define TM_05		4
#define TM_06		5
#define TM_07		6
#define TM_08		7
#define TM_09		8
#define TM_10		9
#define TM_11		10
#define TM_12		11
#define TM_13		12
#define TM_14		13
#define TM_15		14
#define TM_16		15
#define TM_17		16
#define TM_18		17
#define TM_19		18
#define TM_20		19
#define TM_21		20
#define TM_22		21
#define TM_23		22
#define TM_24		23
#define TM_25		24
#define TM_26		25
#define TM_27		26
#define TM_28		27
#define TM_29		28
#define TM_30		29
#define TM_31		30
#define TM_32		31
#define TM_33		32
#define TM_34		33
#define TM_35		34
#define TM_36		35
#define TM_37		36
#define TM_38		37
#define TM_39		38
#define TM_40		39
#define TM_41		40

#define IP_01		41
#define IP_02		42
#define IP_03		43
#define IP_04		44
#define IP_05		45
#define IP_06		46
#define IP_07		47
#define IP_08		49
#define IP_09		49
#define IP_10		50
#define IP_11		51
#define IP_12		52

#define EM_01		53
#define EM_02		54
#define EM_03		55
#define EM_04		56
#define EM_05		57
#define EM_06		58
#define EM_07		59
#define EM_08		60
#define EM_09		61
#define EM_10		62
#define EM_11		63
#define EM_12		64
#define EM_13		65
#define EM_14		66
#define EM_15		67
#define EM_16		68
#define EM_17		69
#define EM_18		70
#define EM_19		71
#define EM_20		72
#define EM_21		73
#define EM_22		74
#define EM_23		75
#define EM_24		76
#define EM_25		77
#define EM_26		78

#define RM_01		79
#define RM_02		80
#define RM_03		81
#define RM_04		82
#define RM_05		83
#define RM_06		84
#define RM_07		85
#define RM_08		86
#define RM_09		87
#define RM_10		88
#define RM_11		89
#define RM_12		90
#define RM_13		91
#define RM_14		92
#define RM_15		93
#define RM_16		94

#define AL_01		95
#define AL_02		96
#define AL_03		97
#define AL_04		98
#define AL_05		99
#define AL_06		100
#define AL_07		101
#define AL_08		102
#define AL_09		103
#define AL_10		104
#define AL_11		105
#define AL_12		106
#define AL_13		107
#define AL_14		108
#define AL_15		109
#define AL_16		110
#define AL_17		111
#define AL_18		112
#define AL_19		113
#define AL_20		114
#define AL_21		115
#define AL_22		116
#define AL_23		117
#define AL_24		118
#define AL_25		119
#define AL_26		120
#define AL_27		121
#define AL_28		122
#define AL_29		123
#define AL_30		124
#define AL_31		125
#define AL_32		126
#define AL_33		127
#define AL_34		128
#define AL_35		129
#define AL_36		130

#define EH_01		131
#define EH_02		132
#define EH_03		133
#define EH_04		134
#define EH_05		135
#define EH_06		136
#define EH_07		137
#define ER_08		138

#define OTHER		139

#ifndef INVALID_TASK
#error INVALID_TASK not defined
#endif

#define INVALID_RESOURCE 0xFE

#define INVALID_ALARM 0xFE

/** \brief Conformance Test INIT value */
#define INIT		0

/** \brief Conformance Test FAILED value */
#define FAILED		1

/** \brief Conformance Test UNKNOWN value */
#define UNKNOWN	2

/** \brief Conformance Test OK value */
#define OK			3

/** \brief ISR Category 3 state macro
 **
 ** if DISABLE the ISR Category 3 test cases will not be compiled
 ** if ENABLE the ISR Category 3 test cases will be compiled
 **/
#define ISR_CATEGORY_3 DISABLE

/** \brief ISR2 Trigger Macro
 **
 ** This macro shall implement a mechanismus to trigger the ISR2 interruption
 **/
#define TriggerISR2()   \
   TriggerISR2_Arch()

/** \brief Initialise Alarm Counter Macro
 **
 ** This macro shall implement a mechanismus to initialise the alarm counter
 **/
#define InitAlarmCounter()

/** \brief Increment Alarm Counter Macro
 **
 ** This macro shall implement a mechanismus to increment the alarm counter
 **/
#define IncAlarmCounter() (void)IncrementCounter(Counter1, 1);

#if (ISR_CATEGORY_3 == ENABLE)
/** \brief ISR3 Trigger Macro
 **
 ** This macro shall implement a mechanismus to trigger the ISR3 interruption
 ** only needed if ISR_CATEGORY_3 is ENABLE
 **/
#define TriggerISR3()
#endif /* #if (ISR_CATEGORY_3 == ENABLE) */

/** \brief Conformance Test Error Checking Type Extended */
#define CT_ERROR_CHECKING_EXTENDED	1

/** \brief Conformance Test Error Checking Type Standard*/
#define CT_ERROR_CHECKING_STANDARD	2

/** \brief Conformance Test Preemptive */
#define CT_PREEMPTIVE					1

/** \brief Conformance Test Non Preemptive */
#define CT_NON_PREEMPTIVE				2

/** \brief Bit used to indicate that the sequence is invalid */
#define SEQUENCE_INVALID ((uint32f)0x80000000)

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/
/** \brief Test Result array
 **
 ** This arrays saves the conformance test results. Every Test case uses 2
 ** bits which status can be: INIT, OK, FAILED, OTHER.
 **
 ** Available test cases are:
 ** 	- TM_01 to TM_41
 ** 	- IP_01 to IP_12
 ** 	- EM_01 to EM_26
 ** 	- RM_01 to RM_16
 ** 	- AL_01 to AL_36
 ** 	- EH_01 to EH_08
 **/
extern uint8 TestResults[35];

/** \brief Test Result Ok array
 **
 ** This arrays represent the correct state for each test case after the test
 ** execution. Every Test case uses 2 bits which status can be: INIT, OK,
 ** FAILED, OTHER.
 **
 ** Available test cases are:
 ** 	- TM_01 to TM_41
 ** 	- IP_01 to IP_12
 ** 	- EM_01 to EM_26
 ** 	- RM_01 to RM_16
 ** 	- AL_01 to AL_36
 ** 	- EH_01 to EH_08
 **/
extern const uint8 TestResultsOk[35];

/** \brief Conformance Test Result Sumary
 **
 ** This variable will be set to 0xff 0x80 after calling ConfTestEvaluation
 ** function. If all test results are equal between TestResutls and
 ** TestResultsOk and the SequenceCounter is equal to the SequenceCounterOk
 ** this variable will be set to 0xff. In other case will be set to 0x80.
 ** The variable is initalisied to 0.
 **/
extern uint8 ConfTestResult;

/** \brief Sequencer Counter
 **
 ** This variable is used to analyse the sequence of the tested functions
 **/
extern uint32f SequenceCounter;

/** \brief Sequencer Counter Ok
 **
 ** This value represent the correct counter of sequences after the
 ** test execuction.
 **/
extern const uint32f SequenceCounterOk;

/*==================[external functions declaration]=========================*/
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
);

/** \brief Conformance Test Evaluation
 **
 ** This service check the result of all executed tests and set the status
 ** of ConfTestResult to 0xff if all test cases were execupted OK or
 ** 0x80 in other case.
 **/
void ConfTestEvaluation
(
	void
);

/** \brief ASSERT
 **
 ** This service is used to assert one condition.
 **
 ** \param[in] tc represent the test case
 ** \param[in] cond condition to be tested
 **/
extern void ASSERT
(
	uint8f tc,
	boolean cond
);

extern void Sequence
(
	uint32f seq
);

extern void ConfTestFinish
(
	void
);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CTEST_H_ */

