

#ifndef _COMPILER_H_
#define _COMPILER_H_
/** \brief FreeOSEK Gen Compiler Header File
 **
 ** Compiler Header File
 **
 ** \file Compiler.h
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Gen
 ** @{ */
/** \addtogroup FreeOSEK_Gen_Global
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090130 v0.1.1 MaCe add OpenGEN_MEMMAP check
 * 20081201 v0.1.0 MaCe	initial version
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/ 
/* check for C99 compiler */
#if ( ( defined __STDC_VERSION__ ) && 	\
		( __STDC_VERSION__ >= 199901L ) )
#define C99_COMPILER
#else
#undef  C99_COMPILER
#endif

/* check for C++-Compiler */
#if defined(__cplusplus)
#define CPP_COMPILER
#else
#undef  CPP_COMPILER
#endif

/* define INLINE and RESTRICT depending on _OS_CPP_COMPILER and
 * _OS_C99_COMPILER
 */
#if (defined C99_COMPILER)
#define INLINE inline
#define RESTRICT restrict
#elif (defined OS_CPP_COMPILER)
#define INLINE inline
#define RESTRICT
#else
#define INLINE
#define RESTRICT
#endif

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _COMPILER_H_ */

