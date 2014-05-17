/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

#ifndef _OS_INTERNAL_ARCH_CFG_H_
#define _OS_INTERNAL_ARCH_CFG_H_
/** \brief FreeOSEK Os Generated Internal Architecture Configuration Header File
 **
 ** This file content the internal generated architecture dependent
 ** configuration of FreeOSEK Os.
 **
 ** \file Os_Internal_Arch_Cfg.h
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090719 v0.1.1 MaCe rename file to Os_
 * 20080713 v0.1.0 MaCe	initial version
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
#define INTERUPTS_COUNT		32

/*==================[typedef]================================================*/
/** \brief Task Context Type */
typedef ucontext_t TaskContextType;

/** \brief Task Context Type */
typedef TaskContextType* TaskContextRefType;

/** \brief InterruptType Type definition */
typedef void (*InterruptType)(void);

/*==================[external data declaration]==============================*/
extern InterruptType InterruptTable[INTERUPTS_COUNT];

/*==================[external functions declaration]=========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_INTERNAL_ARCH_CFG_H_ */

