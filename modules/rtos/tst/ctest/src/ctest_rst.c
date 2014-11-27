/* Copyright 2008, 2009, 2014 Mariano Cerdeiro
 * Copyright 2014, Juan Cecconi
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

/** \brief FreeOSEK Os Conformance Test Result
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_rst.c
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
 * MaCe         Mariano Cerdeiro
 * JuCe         Juan Cecconi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090406 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "ctest.h"      /* include ctest header file */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
uint8 TestResults[35];


const uint8 TestResultsOk[35] =
{
#if (defined ctest_tm_01)
   ( OK << 0 )         /* TM_01 index 00 */
#else
   ( INIT << 0 )      /* TM_01 index 00 */
#endif
#if (defined ctest_tm_02)
   | ( OK << 2 )         /* TM_02 index 01*/
#else
   | ( INIT << 2 )      /* TM_02 index 01*/
#endif
#if (defined ctest_tm_03)
   | ( OK << 4 )         /* TM_03 index 02*/
#else
   | ( INIT << 4 )      /* TM_03 index 02*/
#endif
#if (defined ctest_tm_03)
   | ( OK << 6 ),         /* TM_04 index 03 */
#else
   | ( INIT << 6 ),      /* TM_04 index 03*/
#endif
#if (defined ctest_tm_08)
   ( OK << 0 )         /* TM_05 index 04 */
#else
   ( INIT << 0 )      /* TM_05 index 04*/
#endif
#if (defined ctest_tm_04)
   | ( OK << 2 )         /* TM_06 index 05 */
#else
   | ( INIT << 2 )      /* TM_06 index 05*/
#endif
#if (defined ctest_tm_05)
   | ( OK << 4 )         /* TM_07 index 06 */
#else
   | ( INIT << 4 )      /* TM_07 index 06 */
#endif
#if (defined ctest_tm_05)
   | ( OK << 6 ),         /* TM_08 index 07 */
#else
   | ( INIT << 6 ),      /* TM_08 index 07 */
#endif
#if (defined ctest_tm_10)
   ( OK << 0 )         /* TM_09 index 08 */
#else
   ( INIT << 0 )      /* TM_09 index 08 */
#endif
#if (defined ctest_tm_01)
   | ( OK << 2 )         /* TM_10 index 09 */
#else
   | ( INIT << 2 )      /* TM_10 index 09 */
#endif
#if (defined ctest_tm_06)
   | ( OK << 4 )         /* TM_11 index 10 */
#else
   | ( INIT << 4 )      /* TM_11 index 10 */
#endif
#if (defined ctest_tm_07)
   | ( OK << 6 ),         /* TM_12 index 11 */
#else
   | ( INIT << 6 ),      /* TM_12 index 11 */
#endif
#if (defined ctest_tm_08)
   ( OK << 0 )         /* TM_13 index 12 */
#else
   ( INIT << 0 )      /* TM_13 index 12 */
#endif
#if (defined ctest_tm_08)
   | ( OK << 2 )         /* TM_14 index 13 */
#else
   | ( INIT << 2 )      /* TM_14 index 13 */
#endif
#if (defined ctest_tm_01)
   | ( OK << 4 )         /* TM_15 index 14 */
#else
   | ( INIT << 4 )      /* TM_15 index 14 */
#endif
#if (defined ctest_tm_06)
   | ( OK << 6 ),         /* TM_16 index 15 */
#else
   | ( INIT << 6 ),      /* TM_16 index 15 */
#endif
#if (defined ctest_tm_07)
   ( OK << 0 )         /* TM_17 index 16 */
#else
   ( INIT << 0 )      /* TM_17 index 16 */
#endif
#if (defined ctest_tm_08)
   | ( OK << 2 )         /* TM_18 index 17 */
#else
   | ( INIT << 2 )      /* TM_18 index 17 */
#endif
#if (defined ctest_tm_06)
   | ( OK << 4 )         /* TM_19 index 18 */
#else
   | ( INIT << 4 )      /* TM_19 index 18 */
#endif
#if (defined ctest_tm_01)
   | ( OK << 6 ),         /* TM_20 index 19 */
#else
   | ( INIT << 6 ),      /* TM_20 index 19 */
#endif
#if ( (defined ctest_tm_01) && (ISR_CATEGORY_3 == OSEK_ENABLE) )
   ( OK << 0 )         /* TM_21 index 20 */
#else
   ( INIT << 0 )      /* TM_21 index 20 */
#endif
#if (defined ctest_tm_01)
   | ( OK << 2 )         /* TM_22 index 21 */
#else
   | ( INIT << 2 )      /* TM_22 index 21 */
#endif
#if (defined ctest_tm_09)
   | ( OK << 4 )         /* TM_23 index 22 */
#else
   | ( INIT << 4 )      /* TM_23 index 22 */
#endif
#if (defined ctest_tm_01)
   | ( OK << 6 ),         /* TM_24 index 23 */
#else
   | ( INIT << 6 ),      /* TM_24 index 23 */
#endif
#if (defined ctest_tm_01)
   ( OK << 0 )         /* TM_25 index 24 */
#else
   ( INIT << 0 )      /* TM_25 index 24 */
#endif
#if ( (defined ctest_tm_01) && (ISR_CATEGORY_3 == OSEK_ENABLE) )
   | ( OK << 2 )         /* TM_26 index 25 */
#else
   | ( INIT << 2 )      /* TM_26 index 25 */
#endif
#if (defined ctest_tm_01)
   | ( OK << 4 )         /* TM_27 index 26 */
#else
   | ( INIT << 4 )      /* TM_27 index 26 */
#endif
#if (defined ctest_tm_09)
   | ( OK << 6 ),         /* TM_28 index 27 */
#else
   | ( INIT << 6 ),      /* TM_28 index 27 */
#endif
#if (defined ctest_tm_09)
   ( OK << 0 )         /* TM_29 index 28 */
#else
   ( INIT << 0 )      /* TM_29 index 28 */
#endif
#if (defined ctest_tm_01)
   | ( OK << 2 )         /* TM_30 index 29 */
#else
   | ( INIT << 2 )      /* TM_30 index 29 */
#endif
#if (defined ctest_tm_06)
   | ( OK << 4 )         /* TM_31 index 30 */
#else
   | ( INIT << 4 )      /* TM_31 index 30 */
#endif
#if (defined ctest_tm_07)
   | ( OK << 6 ),         /* TM_32 index 31 */
#else
   | ( INIT << 6 ),      /* TM_32 index 31 */
#endif
#if (defined ctest_tm_06)
   ( OK << 0 )         /* TM_33 index 32 */
#else
   ( INIT << 0 )      /* TM_33 index 32 */
#endif
#if (defined ctest_tm_02)
   | ( OK << 2 )         /* TM_34 index 33 */
#else
   | ( INIT << 2 )      /* TM_34 index 33 */
#endif
#if (defined ctest_tm_01)
   | ( OK << 4 )         /* TM_35 index 34 */
#else
   | ( INIT << 4 )      /* TM_35 index 34 */
#endif
#if ( (defined ctest_tm_01) && (ISR_CATEGORY_3 == OSEK_ENABLE) )
   | ( OK << 6 ),         /* TM_36 index 35 */
#else
   | ( INIT << 6 ),      /* TM_36 index 35 */
#endif
#if (defined ctest_tm_01)
   ( OK << 0 )         /* TM_37 index 36 */
#else
   ( INIT << 0 )      /* TM_37 index 36 */
#endif
#if ( (defined ctest_tm_01) && (ISR_CATEGORY_3 == OSEK_ENABLE) )
   | ( OK << 2 )         /* TM_38 index 37 */
#else
   | ( INIT << 2 )      /* TM_38 index 37 */
#endif
#if (defined ctest_tm_09)
   | ( OK << 4 )         /* TM_39 index 38 */
#else
   | ( INIT << 4 )      /* TM_39 index 38 */
#endif
#if (defined ctest_tm_01)
   | ( OK << 6 ),         /* TM_40 index 39 */
#else
   | ( INIT << 6 ),      /* TM_40 index 39 */
#endif
#if (defined ctest_tm_09)
   ( OK << 0 )         /* TM_41 index 40 */
#else
   ( INIT << 0 )      /* TM_41 index 40 */
#endif
#if (defined ctest_ip_01)
   | ( OK << 2 )         /* IP_01 index 41 */
#else
   | ( INIT << 2 )      /* IP_01 index 41 */
#endif
   | ( INIT << 4 )      /* IP_02 index 42 */
#if (defined ctest_ip_01)
   | ( OK << 6 ),         /* IP_03 index 43 */
#else
   | ( INIT << 6 ),      /* IP_03 index 43 */
#endif
   ( INIT << 0 )      /* IP_04 index 44 */
   | ( INIT << 2 )      /* IP_05 index 45 */
#if (defined ctest_ip_01)
   | ( OK << 4 )        /* IP_06 index 46 */
#else
   | ( INIT << 4 )      /* IP_06 index 46 */
#endif
   | ( INIT << 6 ),      /* IP_07 index 47 */
   ( INIT << 0 )      /* IP_08 index 48 */
#if (defined ctest_ip_03)
   | ( OK << 2 )      /* IP_09 index 49 */
#else
   | ( INIT << 2 )      /* IP_09 index 49 */
#endif
   | ( INIT << 4 )      /* IP_10 index 50 */
#if (defined ctest_ip_04)
   | ( OK << 6 ),      /* IP_11 index 51 */
#else
   | ( INIT << 6 ),      /* IP_11 index 51 */
#endif
   ( INIT << 0 )      /* IP_12 index 52 */
#if (defined ctest_em_01)
   | ( OK << 2 )         /* EM_01 index 53 */
#else
   | ( INIT << 2 )      /* EM_01 index 53 */
#endif
#if (defined ctest_em_01)
   | ( OK << 4 )         /* EM_02 index 54 */
#else
   | ( INIT << 4 )      /* EM_02 index 54 */
#endif
#if (defined ctest_em_01)
   | ( OK << 6 ),         /* EM_03 index 55 */
#else
   | ( INIT << 6 ),      /* EM_03 index 55 */
#endif
#if (defined ctest_em_03)
   ( OK << 0 )         /* EM_04 index 56 */
#else
   ( INIT << 0 )      /* EM_04 index 56 */
#endif
#if (defined ctest_em_03)
   | ( OK << 2 )         /* EM_05 index 57 */
#else
   | ( INIT << 2 )      /* EM_05 index 57 */
#endif
#if (defined ctest_em_04)
   | ( OK << 4 )         /* EM_06 index 58 */
#else
   | ( INIT << 4 )      /* EM_06 index 58 */
#endif
#if (defined ctest_em_04)
   | ( OK << 6 ),         /* EM_07 index 59 */
#else
   | ( INIT << 6 ),      /* EM_07 index 59 */
#endif
#if (defined ctest_em_04)
   ( OK << 0 )         /* EM_08 index 60 */
#else
   ( INIT << 0 )      /* EM_08 index 60 */
#endif
#if (defined ctest_em_03)
   | ( OK << 2 )         /* EM_09 index 61 */
#else
   | ( INIT << 2 )      /* EM_09 index 61 */
#endif
#if (defined ctest_em_04)
   | ( OK << 4 )         /* EM_10 index 62 */
#else
   | ( INIT << 4 )      /* EM_10 index 62 */
#endif
#if (defined ctest_em_01)
   | ( OK << 6 ),         /* EM_11 index 63 */
#else
   | ( INIT << 6 ),      /* EM_11 index 63 */
#endif
#if (defined ctest_em_01)
   ( OK << 0 )         /* EM_12 index 64 */
#else
   ( INIT << 0 )      /* EM_12 index 64 */
#endif
   | ( INIT << 2 )      /* EM_13 index 65 */
#if (defined ctest_em_02)
   | ( OK << 4 )         /* EM_14 index 66 */
#else
   | ( INIT << 4 )      /* EM_14 index 66 */
#endif
#if (defined ctest_em_01)
   | ( OK << 6 ),         /* EM_15 index 67 */
#else
   | ( INIT << 6 ),      /* EM_15 index 67 */
#endif
#if (defined ctest_em_01)
   ( OK << 0 )         /* EM_16 index 68 */
#else
   ( INIT << 0 )      /* EM_16 index 68 */
#endif
#if (defined ctest_em_01)
   | ( OK << 2 )         /* EM_17 index 69 */
#else
   | ( INIT << 2 )      /* EM_17 index 69 */
#endif
#if (defined ctest_em_02)
   | ( OK << 4 )         /* EM_18 index 70 */
#else
   | ( INIT << 4 )      /* EM_18 index 70 */
#endif
#if (defined ctest_em_02)
   | ( OK << 6 ),         /* EM_19 index 71 */
#else
   | ( INIT << 6 ),      /* EM_19 index 71 */
#endif
#if (defined ctest_em_02)
   ( OK << 0 )         /* EM_20 index 72 */
#else
   ( INIT << 0 )      /* EM_20 index 72 */
#endif
#if (defined ctest_em_01)
   | ( OK << 2 )         /* EM_21 index 73 */
#else
   | ( INIT << 2 )      /* EM_21 index 73 */
#endif
#if (defined ctest_em_01)
   | ( OK << 4 )         /* EM_22 index 74 */
#else
   | ( INIT << 4 )      /* EM_22 index 74 */
#endif
#if (defined ctest_em_01)
   | ( OK << 6 ),         /* EM_23 index 75 */
#else
   | ( INIT << 6 ),      /* EM_23 index 75 */
#endif
   ( INIT << 0 )      /* EM_24 index 76 */
#if (defined ctest_em_02)
   | ( OK << 2 )         /* EM_25 index 77 */
#else
   | ( INIT << 2 )      /* EM_25 index 77 */
#endif
#if (defined ctest_em_02)
   | ( OK << 4 )         /* EM_26 index 78 */
#else
   | ( INIT << 4 )      /* EM_26 index 78 */
#endif
#if (defined ctest_rm_01)
   | ( OK << 6 ),         /* RM_01 index 79 */
#else
   | ( INIT << 6 ),      /* RM_01 index 79 */
#endif
#if ( (defined ctest_rm_01) || (defined ctest_rm_05) )
   ( OK << 0 )         /* RM_02 index 80 */
#else
   ( INIT << 0 )      /* RM_02 index 80 */
#endif
#if ( (defined ctest_rm_01) || (defined ctest_rm_05) )
   | ( OK << 2 )         /* RM_03 index 81 */
#else
   | ( INIT << 2 )      /* RM_03 index 81 */
#endif
   | ( INIT << 4 )      /* RM_04 index 82 */
#if (defined ctest_rm_01)
   | ( OK << 6 ),         /* RM_05 index 83 */
#else
   | ( INIT << 6 ),      /* RM_05 index 83 */
#endif
#if (defined ctest_rm_02)
   ( OK << 0 )         /* RM_06 index 84 */
#else
   ( INIT << 0 )      /* RM_06 index 84 */
#endif
#if (defined ctest_rm_03)
   | ( OK << 2 )         /* RM_07 index 85 */
#else
   | ( INIT << 2 )      /* RM_07 index 85 */
#endif
#if (defined ctest_rm_04)
   | ( OK << 4 )         /* RM_08 index 86 */
#else
   | ( INIT << 4 )      /* RM_08 index 86 */
#endif
#if ( (defined ctest_rm_01) || (defined ctest_rm_05) )
   | ( OK << 6 ),         /* RM_09 index 87 */
#else
   | ( INIT << 6 ),      /* RM_09 index 87 */
#endif
#if ( (defined ctest_rm_01) || (defined ctest_rm_05) )
   ( OK << 0 )         /* RM_10 index 88 */
#else
   ( INIT << 0 )      /* RM_10 index 88 */
#endif
   | ( INIT << 2 )      /* RM_11 index 89 */
#if ( (defined ctest_rm_01) || (defined ctest_rm_05) )
   | ( OK << 4 )         /* RM_12 index 90 */
#else
   | ( INIT << 4 )      /* RM_12 index 90 */
#endif
#if (defined ctest_rm_02)
   | ( OK << 6 ),         /* RM_13 index 91 */
#else
   | ( INIT << 6 ),      /* RM_13 index 91 */
#endif
#if (defined ctest_rm_03)
   ( OK << 0 )         /* RM_14 index 92 */
#else
   ( INIT << 0 )      /* RM_14 index 92 */
#endif
#if (defined ctest_rm_04)
   | ( OK << 2 )         /* RM_15 index 93 */
#else
   | ( INIT << 2 )      /* RM_15 index 93 */
#endif
#if (defined ctest_rm_04)
   | ( OK << 4 )         /* RM_16 index 94 */
#else
   | ( INIT << 4 )      /* RM_16 index 94 */
#endif
#if (defined ctest_al_01)
   | ( OK << 6 ),         /* AL_01 index 95 */
#else
   | ( INIT << 6 ),      /* AL_01 index 95 */
#endif
#if (defined ctest_al_02)
   ( OK << 0 )         /* AL_02 index 96 */
#else
   ( INIT << 0 )      /* AL_02 index 96 */
#endif
#if (defined ctest_al_01)
   | ( OK << 2 )         /* AL_03 index 97 */
#else
   | ( INIT << 2 )      /* AL_03 index 97 */
#endif
#if (defined ctest_al_02)
   | ( OK << 4 )         /* AL_04 index 98 */
#else
   | ( INIT << 4 )      /* AL_04 index 98 */
#endif
#if (defined ctest_al_02)
   | ( OK << 6 ),         /* AL_05 index 99 */
#else
   | ( INIT << 6 ),      /* AL_05 index 99 */
#endif
#if (defined ctest_al_03)
   ( OK << 0 )         /* AL_06 index 100 */
#else
   ( INIT << 0 )      /* AL_06 index 100 */
#endif
#if (defined ctest_al_01)
   | ( OK << 2 )         /* AL_07 index 101 */
#else
   | ( INIT << 2 )      /* AL_07 index 101 */
#endif
#if (defined ctest_al_02)
   | ( OK << 4 )         /* AL_08 index 102 */
#else
   | ( INIT << 4 )      /* AL_08 index 102 */
#endif
#if (defined ctest_al_03)
   | ( OK << 6 ),         /* AL_09 index 103 */
#else
   | ( INIT << 6 ),      /* AL_09 index 103 */
#endif
#if (defined ctest_al_01)
   ( OK <<  0 )         /* AL_10 index 104 */
#else
   ( INIT << 0 )      /* AL_10 index 104 */
#endif
#if (defined ctest_al_01)
   | ( OK << 2 )         /* AL_11 index 105 */
#else
   | ( INIT << 2 )      /* AL_11 index 105 */
#endif
#if (defined ctest_al_01)
   | ( OK << 4 )         /* AL_12 index 106 */
#else
   | ( INIT << 4 )      /* AL_12 index 106 */
#endif
#if (defined ctest_al_01)
   | ( OK << 6 ),         /* AL_13 index 107 */
#else
   | ( INIT << 6 ),      /* AL_13 index 107 */
#endif
#if (defined ctest_al_02)
   ( OK << 0 )         /* AL_14 index 108 */
#else
   ( INIT << 0 )      /* AL_14 index 108 */
#endif
#if (defined ctest_al_03)
   | ( OK << 2 )         /* AL_15 index 109 */
#else
   | ( INIT << 2 )      /* AL_15 index 109 */
#endif
#if (defined ctest_al_01)
   | ( OK << 4 )         /* AL_16 index 110 */
#else
   | ( INIT << 4 )      /* AL_16 index 110 */
#endif
#if (defined ctest_al_02)
   | ( OK << 6 ),         /* AL_17 index 111 */
#else
   | ( INIT << 6 ),      /* AL_17 index 111 */
#endif
#if (defined ctest_al_03)
   ( OK <<  0 )         /* AL_18 index 112 */
#else
   ( INIT << 0 )      /* AL_18 index 112 */
#endif
#if (defined ctest_al_01)
   | ( OK << 2 )         /* AL_19 index 113 */
#else
   | ( INIT << 2 )      /* AL_19 index 113 */
#endif
#if (defined ctest_al_01)
   | ( OK << 4 )         /* AL_20 index 114 */
#else
   | ( INIT << 4 )      /* AL_20 index 114 */
#endif
#if (defined ctest_al_01)
   | ( OK << 6 ),         /* AL_21 index 115 */
#else
   | ( INIT << 6 ),      /* AL_21 index 115 */
#endif
#if (defined ctest_al_01)
   ( OK <<  0 )         /* AL_22 index 116 */
#else
   ( INIT << 0 )      /* AL_22 index 116 */
#endif
#if (defined ctest_al_02)
   | ( OK << 2 )         /* AL_23 index 117 */
#else
   | ( INIT << 2 )      /* AL_23 index 117 */
#endif
#if (defined ctest_al_03)
   | ( OK <<  4 )         /* AL_24 index 118 */
#else
   | ( INIT << 4 )      /* AL_24 index 118 */
#endif
#if (defined ctest_al_01)
   | ( OK << 6 ),         /* AL_25 index 119 */
#else
   | ( INIT << 6 ),      /* AL_25 index 119 */
#endif
#if (defined ctest_al_02)
   ( OK << 0 )         /* AL_26 index 120 */
#else
   ( INIT << 0 )      /* AL_26 index 120 */
#endif
#if (defined ctest_al_02)
   | ( OK << 2 )         /* AL_27 index 121 */
#else
   | ( INIT << 2 )      /* AL_27 index 121 */
#endif
#if (defined ctest_al_03)
   | ( OK <<  4 )         /* AL_28 index 122 */
#else
   | ( INIT << 4 )      /* AL_28 index 122 */
#endif
   | ( INIT << 6 ),      /* AL_29 index 123 */
#if (defined ctest_al_04)
   ( OK <<  0 )         /* AL_30 index 124 */
#else
   ( INIT << 0 )      /* AL_30 index 124 */
#endif
#if (defined ctest_al_05)
   | ( OK <<  2 )         /* AL_31 index 125 */
#else
   | ( INIT << 2 )      /* AL_31 index 125 */
#endif
#if (defined ctest_al_05)
   | ( OK <<  4 )         /* AL_32 index 126 */
#else
   | ( INIT << 4 )      /* AL_32 index 126 */
#endif
#if (defined ctest_al_06)
   | ( OK <<  6 ),      /* AL_33 index 127 */
#else
   | ( INIT << 6 ),      /* AL_33 index 127 */
#endif
#if ( ( (defined ctest_al_03) && (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE) ) || \
      (defined ctest_al_06) )
   ( OK <<  0 )         /* AL_34 index 128 */
#else
   ( INIT << 0 )      /* AL_34 index 128 */
#endif
#if (defined ctest_al_07)
   | ( OK <<  2 )         /* AL_35 index 129 */
#else
   | ( INIT << 2 )      /* AL_35 index 129 */
#endif
#if (defined ctest_al_07)
   | ( OK <<  4 )         /* AL_36 index 130 */
#else
   | ( INIT << 4 )      /* AL_36 index 130 */
#endif
#if (defined ctest_eh_02)
   | ( OK << 6 ),         /* EH_01 index 131 */
#else
   | ( INIT << 6 ),      /* EH_01 index 131 */
#endif
   ( INIT << 0 )      /* EH_02 index 132 */
#if (defined ctest_eh_02)
   | ( OK << 2 )         /* EH_03 index 133 */
#else
   | ( INIT << 2 )      /* EH_03 index 133 */
#endif
#if (defined ctest_eh_01)
   | ( OK << 4 )         /* EH_04 index 134 */
#else
   | ( INIT << 4 )      /* EH_04 index 134 */
#endif
#if (defined ctest_eh_01)
   | ( OK << 6 ),         /* EH_05 index 135 */
#else
   | ( INIT << 6 ),      /* EH_05 index 135 */
#endif
#if (defined ctest_eh_01)
   ( OK << 0 )         /* EH_06 index 136 */
#else
   ( INIT << 0 )      /* EH_06 index 136 */
#endif
#if (defined ctest_eh_02)
   | ( OK << 2 )         /* EH_07 index 137 */
#else
   | ( INIT << 2 )      /* EH_07 index 137 */
#endif
   | ( INIT << 4 )      /* EH_08 index 138 */
#if ( (!defined ctest_ip_03) && (!defined ctest_ip_04) )
   | ( OK << 6 )         /* OTHER index 139 */
#else
   | ( INIT << 6 )         /* OTHER index 139 */
#endif
};

uint8 ConfTestResult;

uint32f SequenceCounter;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
   void ConfTestEvaluation
(
 void
 )
{
   uint32f loopi;
   boolean testok = TRUE;

   for( loopi = 0; loopi < 35; loopi++)
   {
      if ( TestResultsOk[loopi] != TestResults[loopi] )
      {
         testok = FALSE;
      }
   }

   if ( ( testok == TRUE ) && ( SequenceCounter == SequenceCounterOk ) )
   {
      ConfTestResult = 255;
   }
   else
   {
      ConfTestResult = 128;
   }

}

   void Sequence
(
 uint32f seq
 )
{
   if ( (seq) == 0 )
   {
      SequenceCounter = 0;
   }
   else if ( (SequenceCounter+1) == (seq) )
   {
      SequenceCounter++;
   }
   else
   {
      SequenceCounter |= SEQUENCE_INVALID;
   }
}

   void ConfTestFinish
(
 void
 )
{
   while(1);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

