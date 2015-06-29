/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

/* Copyright 2008, 2009, 2015 Mariano Cerdeiro
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


#ifndef _OS_CFG_H_
#define _OS_CFG_H_
/** \brief FreeOSEK Os Generated Configuration Header File
 **
 ** This file contents the generated configuration of FreeOSEK Os
 **
 ** \file Os_Cfg.h
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
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150619 v0.1.4 MaCe fix issue #279
 * 20090719 v0.1.3 MaCe rename file to Os_
 * 20090424 v0.1.2 MaCe add counters defines
 * 20090128 v0.1.1 MaCe add MEMMAP off configuration
 * 20080810 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
/** \brief Definition of the  DeclareTask Macro */
#define DeclareTask(name) void OSEK_TASK_ ## name (void)

#define OSEK_OS_INTERRUPT_MASK ((InterruptFlagsType)0xFFFFFFFFU)

/** \brief Task Definition */
#define dummyTask 0


/** \brief Definition of the Event POSIXE */
#define POSIXE 0x1U
/** \brief Definition of the Event MODBUSE */
#define MODBUSE 0x2U

/** \brief Definition of the resource POSIXR */
#define POSIXR ((ResourceType)0)
/** \brief Definition of the resource MODBUSR */
#define MODBUSR ((ResourceType)1)



/** \brief OS Error Get Service Id */
/* \req OSEK_ERR_0.1 The macro OSErrorGetServiceId() shall provide the service
 * identifier with a OSServiceIdType type where the error has been risen
 * \req OSEK_ERR_0.1.1 Possibly return values are: OSServiceId_xxxx, where
 * xxxx is the name of the system service
 */
#define OSErrorGetServiceId() (Osek_ErrorApi)

#define OSErrorGetParam1() (Osek_ErrorParam1)

#define OSErrorGetParam2() (Osek_ErrorParam2)

#define OSErrorGetParam3() (Osek_ErrorParam3)

#define OSErrorGetRet() (Osek_ErrorRet)

/** \brief OSEK_MEMMAP macro (OSEK_DISABLE not MemMap is used for FreeOSEK, OSEK_ENABLE
 ** MemMap is used for FreeOSEK) */
#define OSEK_MEMMAP OSEK_DISABLE
/** \brief Schedule this Task if higher priority Task are Active
 **
 ** \remarks if the system is configured with extended errors the
 **          function Schedule is implemented as a macro and calls the
 **          internal function Schedule_Int. If Standard errores are
 **          configured the function Schedule is implemented as a function.
 **
 ** This API shall Schedule the calling Task if a higher priority Task
 ** is active. This API shall only be used from non preemtive tasks.
 **
 ** \return E_OK if no error
 ** \return E_OS_CALLEVEL if call at interrupt level
 ** \return E_OS_RESOURCE if the calling task occupies resources
 **/
#define Schedule() Schedule_Int(TRUE)

/*==================[typedef]================================================*/
/** \brief Type definition of StatusType
 **
 ** This type is used to represent the status returned by all FreeOSEK APIs
 **/
/* \req OSEK_SYS_1.1 */
typedef unsigned char StatusType;


/*==================[external data declaration]==============================*/
/** \brief Error Api Variable
 **
 ** This variable contents the api which generate the last error
 **/
extern unsigned int Osek_ErrorApi;

/** \brief Error Param1 Variable
 **
 ** This variable contents the first parameter passed to the api which has
 ** generted the last error.
 **/
extern unsigned int Osek_ErrorParam1;

/** \brief Error Param2 Variable
 **
 ** This variable contents the second parameter passed to the api which has
 ** generted the last error.
 **/
extern unsigned int Osek_ErrorParam2;

/** \brief Error Param3 Variable
 **
 ** This variable contents the third parameter passed to the api which has
 ** generted the last error.
 **/
extern unsigned int Osek_ErrorParam3;

/** \brief Error Return Variable
 **
 ** This variable contents return value of the api which has generated
 ** the last error.
 **/
extern unsigned int Osek_ErrorRet;


/*==================[external functions declaration]=========================*/
/** \brief Error Hook */
extern void ErrorHook(void);

/** \brief Task Declaration of Task dummyTask */
DeclareTask(dummyTask);



/** \brief Schedule this Task if higher priority Task are Active
 **
 ** This API shall Schedule the calling Task if a higher priority Task
 ** is active. This API shall only be used from non preemtive tasks.
 **
 ** \remarks This interface may be used by the end user over the
 **          macro Schedule or from the system itself. Therefore
 **          the parameter PerformChecks is provided. When
 **          the user calls the scheduler the checks shall be
 **          performed if the error checking is set as extended.
 **          If the system calls the Schedule no error checking
 **          shall be performed, the system shall be trusted.
 **
 ** \param[in] PerformChecks indicates if the function shall or not
 **                          perform the extended checks. This parameter
 **                          is only available if the error checks are set
 **                          to extended.
 ** \return E_OK if no error
 ** \return E_OS_CALLEVEL if call at interrupt level
 ** \return E_OS_RESOURCE if the calling task occupies resources
 **/
extern StatusType Schedule_Int(boolean PerformChecks);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_CFG_H_ */

