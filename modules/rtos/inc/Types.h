

#ifndef _TYPES_H_
#define _TYPES_H_
/** \brief FreeOSKE Gen Generic Types definition
 **
 ** \file Types.h
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Gen Generator and Generic Files
 ** @{ */
/** \addtogroup FreeOSEK_Gen_Global Global
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe                  Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * v0.1.2 20090330 MaCe improve Types.h (many changes)
 * v0.1.1 20090128 MaCe add DISABLE and ENABLE macros
 * v0.1.0 20081126 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Compiler.h"

/*==================[macros]=================================================*/
#ifndef ENABLE
/** \brief ENABLE macro with the value 1 */
#define ENABLE 1
#elif ENABLE != 1
#error ENABLE defined somewhere else with a different value
#endif

#ifndef DISABLE
/** \brief DISABLE macro 2 */
#define DISABLE 2
#elif DISABLE != 2
#error DISABLE defined somewhere else with a different value
#endif

/** \brief Check for C99-Compiler */
#undef  C99_COMPILER
#if defined(__STDC_VERSION__)
#if __STDC_VERSION__>=199901L
#define C99_COMPILER
#endif
#endif

/** \brief Check for C++-Compiler */
#undef  CPP_COMPILER
#if defined(__cplusplus)
#define CPP_COMPILER
#endif

/** \brief Check for Freestanding-Environment */
#undef  FREESTANDING_ENV
#if !defined(__STDC_HOSTED__)
#define FREESTANDING_ENV
#endif

/** \brief Define INLINE- and RESTRICT-Macros */
#if defined(C99_COMPILER)
#define INLINE inline
#define RESTRICT restrict
#elif defined(CPP_COMPILER)
#define INLINE inline
#define RESTRICT
#else
#define INLINE
#define RESTRICT
#endif

/** \brief Definition of FALSE */
#if !defined(FALSE)
#if defined(C99_COMPILER) || defined(CPP_COMPILER)
#define FALSE false
#else
#define FALSE ((boolean)0)
#endif
#endif

/** \brief Definiton of TRUE */
#if !defined(TRUE)
#if defined(C99_COMPILER) || defined(CPP_COMPILER)
#define TRUE true
#else
#define TRUE ((boolean)1)
#endif
#endif

/*==================[typedef]================================================*/
#if defined(C99_COMPILER)
#include <stdint.h>
#include <stdbool.h>
#elif defined(CPP_COMPILER)
#include <cstdint>
#include <cstdbool>
#else
#include "Types_Arch.h"
#endif

#ifndef TYPES_NULL
#define TYPES_NULL
/** \brief NULL definition */
#define NULL ((void *)0)
#endif

#if defined(C99_COMPILER) || defined(CPP_COMPILER)
/** \brief boolean type type definition */
typedef _Bool				boolean;

/** \brief default usigned 8 bits integer type definition */
typedef int8_t				sint8;
/** \brief default signed 8 bits integer type definition */
typedef uint8_t			uint8;
/** \brief default signed 16 bits integer type definition */
typedef int16_t			sint16;
/** \brief default unsigned 16 bits integer type definition */
typedef uint16_t			uint16;
/** \brief default signed 32 bits integer type definition */
typedef int32_t			sint32;
/** \brief default unsigned 32 bits integer type definition */
typedef uint32_t			uint32;

/** \brief default signed 8 bits fast integer type definition */
typedef int_least8_t		sint8_least;
/** \brief default usigned 8 bits fast integer type definition */
typedef uint_least8_t   uint8_least;
/** \brief default signed 16 bits fast integer type definition */
typedef int_least16_t   sint16_least;
/** \brief default unsigned 16 bits fast integer type definition */
typedef uint_least16_t  uint16_least;
/** \brief default signed 32 bits fast integer type definition */
typedef int_least32_t   sint32_least;
/** \brief default unsigned 32 bits fast integer type definition */
typedef uint_least32_t  uint32_least;

/** \brief default signed 8 bits fast integer type definition */
typedef int_fast8_t		sint8f;
/** \brief default unsigned 8 bits fast integer type definition */
typedef uint_fast8_t		uint8f;
/** \brief default signed 16 bits fast integer type definition */
typedef int_fast16_t		sint16f;
/** \brief default unsigned 16 bits fast integer type definition */
typedef uint_fast16_t   uint16f;
/** \brief default signed 32 bits fast integer type definition */
typedef int_fast32_t		sint32f;
/** \brief default unsigned 32 bits fast integer type definition */
typedef uint_fast32_t   uint32f;

#else /* #if defined(C99_COMPILER) || defined(CPP_COMPILER) */

#ifndef TYPES_BOOLEAN
/** \brief boolean type type definition */
typedef unsigned char boolean;
#endif

#ifndef TYPES_UINT8
#define TYPES_UINT8
/** \brief default usigned 8 bits integer type definition */
typedef unsigned char uint8;
#endif

#ifndef TYPES_SINT8
#define TYPES_SINT8
/** \brief default signed 8 bits integer type definition */
typedef signed char sint8;
#endif

#ifndef TYPES_UINT8F
#define TYPES_UINT8F
/** \brief default unsigned 8 bits fast integer type definition */
typedef unsigned int uint8f;
#endif

#ifndef TYPES_SINT8F
#define TYPES_SINT8F
/** \brief default signed 8 bits fast integer type definition */
typedef signed int sint8f;
#endif

#ifndef TYPES_UINT16
#define TYPES_UINT16
/** \brief default unsigned 16 bits integer type definition */
typedef unsigned short uint16;
#endif

#ifndef TYPES_SINT16
#define TYPES_SINT16
/** \brief default signed 16 bits integer type definition */
typedef signed short sint16;
#endif

#ifndef TYPES_UINT16F
#define TYPES_UINT16F
/** \brief default unsigned 16 bits fast integer type definition */
typedef unsigned int uint16f;
#endif

#ifndef TYPES_SINT16F
#define TYPES_SINT16F
/** \brief default signed 16 bits fast integer type definition */
typedef signed int sint16f;
#endif

#ifndef TYPES_UINT32
#define TYPES_UINT32
/** \brief default unsigned 32 bits integer type definition */
typedef unsigned int uint32;
#endif

#ifndef TYPES_SINT32
#define TYPES_SINT32
/** \brief default signed 32 bits integer type definition */
typedef signed int sint32;
#endif

#ifndef TYPES_UINT32F
#define TYPES_UINT32F
/** \brief default unsigned 32 bits fast integer type definition */
typedef unsigned int uint32f;
#endif

#ifndef TYPES_SINT32F
#define TYPES_SINT32F
/** \brief default signed 32 bits fast integer type definition */
typedef signed int sint32f;
#endif

#endif  /* #if defined(C99_COMPILER) || defined(CPP_COMPILER) */

#ifndef TYPES_STD_RETURNTYPE
#define TYPES_STD_RETURNTYPE
/** \brief default Standard Return type definition */
typedef uint8f Std_ReturnType;
#endif

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _TYPES_H_ */

