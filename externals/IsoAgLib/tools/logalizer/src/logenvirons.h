/*
  logenvirons.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef LOGENVIRONS_H
#define LOGENVIRONS_H


#include <stdlib.h>
#ifdef WIN32
#if ( _MSC_VER > 1500 ) // Studio 2010 comes with cstdint
#include <cstdint>
#else // before Studio 2010 we have to defines int types ourself
typedef signed __int8 int8_t;
typedef signed __int16 int16_t;
typedef signed __int32 int32_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif
#else
#include <stdint.h>
#endif
#include <iostream>


char getAscii(uint8_t val);

void exit_with_error(const char* error_message);


inline
char getAscii( uint8_t val )
{
  return ((val >= 0x20) && (val < 0x7F)) ? (char(val)) : '.';
}


inline
void exit_with_error( const char* error_message )
{
  std::cerr << error_message << std::endl;
  exit(-1);
}


#endif
