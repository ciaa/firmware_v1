/*
  typedef.h: system dependent basic typedef's

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TYPEDEF_P1MC_H
#define TYPEDEF_P1MC_H

typedef unsigned short byte;
typedef unsigned int word;

typedef byte uint8_t;
typedef char int8_t;
typedef word uint16_t;
typedef int int16_t;
typedef unsigned long uint32_t;
typedef long int32_t;


//typedef long off_t;
#define off_t long

typedef unsigned long long int uint64_t;
typedef long long int int64_t;
#endif
