/*
  convert.h: central conversion functions

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef __CONVERT_H__
#define __CONVERT_H__

#include <IsoAgLib/isoaglib_config.h>


/** Common convert helpers (e.g. type conversion with type check).
  */
namespace convert_n
{
// Casting signed/unsigned by pointer not by value:

  //! Convert any integer to unsigned by cast (not by value) even if allready.
  inline uint8_t  castUI( int8_t  value ) { return *((uint8_t* )(&value)); }
  inline uint16_t castUI( int16_t value ) { return *((uint16_t*)(&value)); }
  inline uint32_t castUI( int32_t value ) { return *((uint32_t*)(&value)); }
  inline uint8_t  castUI(uint8_t  value ) { return value; }
  inline uint16_t castUI(uint16_t value ) { return value; }
  inline uint32_t castUI(uint32_t value ) { return value; }
  //! Convert any integer to signed by cast (not by value) even if allready.
  inline  int8_t  castI( uint8_t  value ) { return *(( int8_t* )(&value)); }
  inline  int16_t castI( uint16_t value ) { return *(( int16_t*)(&value)); }
  inline  int32_t castI( uint32_t value ) { return *(( int32_t*)(&value)); }
  inline  int8_t  castI(  int8_t  value ) { return value; }
  inline  int16_t castI(  int16_t value ) { return value; }
  inline  int32_t castI(  int32_t value ) { return value; }

  //! Template to get n-th significant byte of integer (no matter if signed or not).
  template<class T> inline uint8_t getByte(   T value, int n ) {
    return (castUI(value) >> (8*n)) & 0xFFU; }
  //! Template to get n-th significant nibble of integer (no matter if signed or not).
  template<class T> inline uint8_t getNibble( T value, int n ) {
    return (castUI(value) >> (4*n)) & 0xFU; }
}

#endif // __CONVERT_H__
