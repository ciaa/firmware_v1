/// following defines/types are needed for the library/tool-shared header file can_server.h
#define OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN
#define CNAMESPACE

// prevent inclusion of isoaglib config
#define _ISOAGLIB_CONFIG_H_

#if !defined(_MSC_VER) && !defined(WINCE)
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
  typedef int int_fast8_t;
  typedef unsigned int uint_fast8_t;
  #endif
  typedef int int_fast16_t;
  typedef int int_fast32_t;
  typedef long long int int_fast64_t;
  typedef unsigned int uint_fast16_t;
  typedef unsigned int uint_fast32_t;
  typedef unsigned long long int uint_fast64_t;
#endif

typedef uint8_t boolean;

// include ISOAgLib's "can_server.h"
#include <can_server_interface.h>
