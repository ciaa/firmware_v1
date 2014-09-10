/*
  typedef.h: system dependent basic typedef's

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _TYPEDEF_PC_H
#define _TYPEDEF_PC_H

#define HIGH              1
#define LOW               0

#define ON                1
#define OFF               0

#define HAS_64BIT_INT_TYPE

// 1600 == Visual Studio 2010
#if (!defined(_MSC_VER) || (_MSC_VER >= 1600)) && !defined(WINCE)
  #include <stdint.h>
#else
  typedef unsigned char uint8_t;
  typedef signed char int8_t;
  typedef unsigned short uint16_t;
  typedef short int16_t;
  typedef unsigned int uint32_t;
  typedef int int32_t;
  typedef unsigned long long int uint64_t;
  typedef long int long int64_t;

  #if ( _MSC_VER < 1600 )
  typedef int           int_fast8_t;
  typedef unsigned int           uint_fast8_t;
  #endif
  typedef int           int_fast16_t;
  typedef int           int_fast32_t;
  typedef long long int int_fast64_t;
  typedef unsigned int           uint_fast16_t;
  typedef unsigned int           uint_fast32_t;
  typedef unsigned long long int uint_fast64_t;
#endif

typedef uint8_t boolean;


namespace __HAL {

typedef struct
{
  bool started;
} tSystem;

} // __HAL


#endif
