/*
  util_funcs.cpp:

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "util_funcs.h"
#include <IsoAgLib/util/iassert.h>
#include <cstdio>
#include <string.h>

#ifdef USE_DATASTREAMS_IO
#include <IsoAgLib/comm/Part3_DataLink/impl/stream_c.h>
#endif

#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif

// Begin Namespace __IsoAgLib
namespace __IsoAgLib {


uint8_t convertLittleEndianStringUi8( const uint8_t* apui8_src )
{
  return apui8_src[0];
}


uint8_t bcd2dec(uint8_t ab_bcd)
{
 return uint8_t(((ab_bcd >> 4) * 10) + (ab_bcd & 0xF));
}


uint8_t dec2bcd(uint8_t ab_dec)
{
  const uint8_t ui8_v10 = ab_dec / 10;
  const uint8_t ui8_v0  = ab_dec % 10;
  const uint8_t ui8_result = uint8_t(( ui8_v10 << 4 ) + ui8_v0);
  return ui8_result;
}


int32_t mul1Div1Mul2Div2(int32_t ai32_mul_1, int32_t ai32_div_1, int32_t ai32_mul_2, int32_t ai32_div_2)
{
  if ((ai32_mul_1 == 0) || (ai32_mul_2 == 0) || (ai32_div_1 == 0) || (ai32_div_2 == 0)) return 0;
  int32_t i32_result = (ai32_mul_1 / ai32_div_1) * (ai32_mul_2 / ai32_div_2);
  i32_result += ((ai32_mul_1 % ai32_div_1) * (ai32_mul_2 % ai32_div_2)) / (ai32_div_1 * ai32_div_2);
  i32_result += ((ai32_mul_1 / ai32_div_1) * (ai32_mul_2 % ai32_div_2)) / ai32_div_2;
  i32_result += ((ai32_mul_1 % ai32_div_1) * (ai32_mul_2 / ai32_mul_2)) / ai32_div_1;
  return i32_result;
}


void littleEndianStream2FloatVar(const void *const pvFrom, float *const pf_to)
{
#ifdef NO_8BIT_CHAR_TYPE
  uint32_t *pb_to = (uint32_t*)pf_to;
  const uint8_t *const pb_from   = (const uint8_t *)pvFrom;
  #if FLOAT_WORD_ORDER == WORD_HI_LO
    // change first and last two bytes for float access
    pb_to[0]  = uint32_t(pb_from[2]) << 0;
    pb_to[0] |= uint32_t(pb_from[3]) << 8;
    pb_to[0] |= uint32_t(pb_from[0]) << 16;
    pb_to[0] |= uint32_t(pb_from[1]) << 24;
  #elif FLOAT_WORD_ORDER == BYTE_HI_LO
    pb_to[0]  = uint32_t(pb_from[3]) << 0;
    pb_to[0] |= uint32_t(pb_from[2]) << 8;
    pb_to[0] |= uint32_t(pb_from[1]) << 16;
    pb_to[0] |= uint32_t(pb_from[0]) << 24;
  #elif FLOAT_WORD_ORDER == WORD_LO_HI
    pb_to[0]  = uint32_t(pb_from[0]) << 0;
    pb_to[0] |= uint32_t(pb_from[1]) << 8;
    pb_to[0] |= uint32_t(pb_from[2]) << 16;
    pb_to[0] |= uint32_t(pb_from[3]) << 24;
  #else
  #error "PLEASE set either FLOAT_WORD_ORDER with WORD_HI_LO BYTE_HI_LO or WORD_LO_HI in the config-h from your target"
  #endif
#else
  uint8_t *pb_to = (uint8_t*)pf_to;
  const uint8_t *const pb_from   = (const uint8_t *)pvFrom;
  #if FLOAT_WORD_ORDER == WORD_HI_LO
    // change first and last two bytes for float access
    pb_to[0] = pb_from[2];
    pb_to[1] = pb_from[3];
    pb_to[2] = pb_from[0];
    pb_to[3] = pb_from[1];
  #elif FLOAT_WORD_ORDER == BYTE_HI_LO
    pb_to[0] = pb_from[3];
    pb_to[1] = pb_from[2];
    pb_to[2] = pb_from[1];
    pb_to[3] = pb_from[0];
  #elif FLOAT_WORD_ORDER == WORD_LO_HI
    pb_to[0] = pb_from[0];
    pb_to[1] = pb_from[1];
    pb_to[2] = pb_from[2];
    pb_to[3] = pb_from[3];
  #else
  #error "PLEASE set either FLOAT_WORD_ORDER with WORD_HI_LO BYTE_HI_LO or WORD_LO_HI in the config-h from your target"
  #endif
#endif // NO_8BIT_CHAR_TYPE
}


void floatVar2LittleEndianStream(const float *const pf_from, void *const pvTo)
{
#ifdef NO_8BIT_CHAR_TYPE
  uint8_t *pb_to = (uint8_t*)pvTo;
  const uint32_t *const pb_from   = (const uint32_t *)pf_from;
  #if FLOAT_WORD_ORDER == WORD_HI_LO
    pb_to[0] = (pb_from[0] >>  8) & 0xFF;
    pb_to[1] = (pb_from[0] >>  0) & 0xFF;
    pb_to[2] = (pb_from[0] >> 24) & 0xFF;
    pb_to[3] = (pb_from[0] >> 16) & 0xFF;
  #elif FLOAT_WORD_ORDER == BYTE_HI_LO
    pb_to[0] = (pb_from[0] >>  0) & 0xFF;
    pb_to[1] = (pb_from[0] >>  8) & 0xFF;
    pb_to[2] = (pb_from[0] >> 16) & 0xFF;
    pb_to[3] = (pb_from[0] >> 24) & 0xFF;
  #elif FLOAT_WORD_ORDER == WORD_LO_HI
    pb_to[0] = (pb_from[0] >> 24) & 0xFF;
    pb_to[1] = (pb_from[0] >> 16) & 0xFF;
    pb_to[2] = (pb_from[0] >>  8) & 0xFF;
    pb_to[3] = (pb_from[0] >>  0) & 0xFF;
  #else
  #error "PLEASE set either FLOAT_WORD_ORDER with WORD_HI_LO BYTE_HI_LO or WORD_LO_HI in the config-h from your target"
  #endif
#else
  uint8_t *pb_to = (uint8_t*)pvTo;
  const uint8_t *const pb_from   = (const uint8_t *)pf_from;
  #if FLOAT_WORD_ORDER == WORD_HI_LO
    // change first and last two bytes for float access
    pb_to[0] = pb_from[2];
    pb_to[1] = pb_from[3];
    pb_to[2] = pb_from[0];
    pb_to[3] = pb_from[1];
  #elif FLOAT_WORD_ORDER == BYTE_HI_LO
    pb_to[0] = pb_from[3];
    pb_to[1] = pb_from[2];
    pb_to[2] = pb_from[1];
    pb_to[3] = pb_from[0];
  #elif FLOAT_WORD_ORDER == WORD_LO_HI
    pb_to[0] = pb_from[0];
    pb_to[1] = pb_from[1];
    pb_to[2] = pb_from[2];
    pb_to[3] = pb_from[3];
  #else
  #error "PLEASE set either FLOAT_WORD_ORDER with WORD_HI_LO BYTE_HI_LO or WORD_LO_HI in the config-h from your target"
  #endif
#endif // NO_8BIT_CHAR_TYPE
}



#ifdef USE_DATASTREAMS_IO
uint16_t convertIstreamUi16( StreamInput_c& rc_stream )
{
  uint16_t ui16_temp;
  #ifdef OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN
  uint8_t* const cpui8_writePointer = (uint8_t*)(&ui16_temp);
  rc_stream >> cpui8_writePointer[0];
  rc_stream >> cpui8_writePointer[1];
  #else // BIG_ENDIAN
  uint8_t* const cpui8_writePointer = (uint8_t*)(&ui16_temp);
  rc_stream >> cpui8_writePointer[1];
  rc_stream >> cpui8_writePointer[0];
  #endif
  return ui16_temp;
};


uint32_t convertIstreamUi32( StreamInput_c& rc_stream )
{
  uint32_t ui32_temp;
  #ifdef OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN
  uint8_t* pui8_writePointer = (uint8_t*)(&ui32_temp);
  for (uint8_t* pui8_writePointerEnd = pui8_writePointer+4;
       pui8_writePointer < pui8_writePointerEnd;
       ++pui8_writePointer)
  {
    rc_stream >> *pui8_writePointer;
  }
  #else // BIG_ENDIAN
  uint8_t* pui8_writePointerRLast = (uint8_t*)(&ui32_temp);
  for (uint8_t* pui8_writePointer = pui8_writePointerRLast+(4-1);
       pui8_writePointer >= pui8_writePointerRLast;
       --pui8_writePointer)
  {
    rc_stream >> *pui8_writePointer;
  }
  #endif
  return ui32_temp;
};

#endif


uint16_t convertLittleEndianStringUi16( const uint8_t* apui8_src )
{
  uint16_t ui16_temp;
#if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( &ui16_temp, apui8_src, sizeof(uint16_t) );
#else
  ui16_temp = uint16_t(apui8_src[0]) | (uint16_t(apui8_src[1]) << 8);
#endif
  return ui16_temp;
}


int16_t convertLittleEndianStringI16( const uint8_t* apui8_src )
{
  int16_t i16_temp;
#if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( &i16_temp, apui8_src, sizeof(int16_t) );
#else
  i16_temp = int16_t(apui8_src[0]) | (int16_t(apui8_src[1]) << 8);
#endif
  return i16_temp;
}


uint32_t convertLittleEndianStringUi32( const uint8_t* apui8_src )
{
  uint32_t ui32_temp;
#if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( &ui32_temp, apui8_src, sizeof(uint32_t) );
#else
  ui32_temp = uint32_t(apui8_src[0]);
  for ( unsigned int ind = 1; ( ind < sizeof(uint32_t) ); ind++ )
  {
    ui32_temp |= (uint32_t(apui8_src[ind]) << (8*ind));
  }
#endif
  return ui32_temp;
}


int32_t convertLittleEndianStringI32( const uint8_t* apui8_src )
{
  int32_t i32_temp;
#if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( &i32_temp, apui8_src, sizeof(int32_t) );
#else
  i32_temp = int32_t(apui8_src[0]);
  for ( unsigned int ind = 1; ( ind < sizeof(int32_t) ); ind++ )
  {
    i32_temp |= (int32_t(apui8_src[ind]) << (8*ind));
  }
#endif
  return i32_temp;
}


#ifdef HAS_64BIT_INT_TYPE

uint64_t convertLittleEndianStringUi64( const uint8_t* apui8_src )
{
  uint64_t ui64_temp;
#if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( &ui64_temp, apui8_src, sizeof(uint64_t) );
#else
  ui64_temp = uint64_t(apui8_src[0]);
  for ( unsigned int ind = 1; ( ind < sizeof(uint64_t) ); ind++ )
  {
    ui64_temp |= (uint64_t(apui8_src[ind]) << (8*ind));
  }
#endif
  return ui64_temp;
}


int64_t convertLittleEndianStringI64( const uint8_t* apui8_src )
{
  int64_t i64_temp;
#if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( &i64_temp, apui8_src, sizeof(int64_t) );
#else
  i64_temp = int64_t(apui8_src[0]);
  for ( unsigned int ind = 1; ( ind < sizeof(int64_t) ); ind++ )
  {
    i64_temp |= (int64_t(apui8_src[ind]) << (8*ind));
  }
#endif
  return i64_temp;
}

#endif

float convertLittleEndianStringFloat( const uint8_t* apui8_src )
{
  float f_temp;
  littleEndianStream2FloatVar(apui8_src, &f_temp);
  return f_temp;
}


void int2littleEndianString( unsigned int input, uint8_t* pui8_target, unsigned int size )
{
  #if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( pui8_target,   &input, size );
  #else
  for ( unsigned int ind = 0; ind < size; ind++ )
  {
    pui8_target[ind] = ((input >> ( 8 * ind )) & 0xFF );
  }
  #endif
}

#if HAL_SIZEOF_INT == 1
  #define SCANF_INT_STRING "%2x"
#elif HAL_SIZEOF_INT == 2
  #define SCANF_INT_STRING "%4x"
#elif HAL_SIZEOF_INT == 4
  #define SCANF_INT_STRING "%8x"
#elif HAL_SIZEOF_INT == 8
  #define SCANF_INT_STRING "%8x"
#endif

/** convert big endian textual number representation into little endian uint8_t string of specified size */
void bigEndianHexNumberText2CanString( const char* ac_src, uint8_t* pui8_target, unsigned int size )
{
  if ( NULL == ac_src )
  {
    CNAMESPACE::memset( pui8_target, 0, size );
    return;
  }
  unsigned int temp = 0;

  const unsigned int inputLen = CNAMESPACE::strlen(ac_src);
  uint8_t* pui8_write = pui8_target;

  int ind = inputLen - ( 2 * HAL_SIZEOF_INT );
  for ( ; ind >= 0; ind -= ( 2 * HAL_SIZEOF_INT ) )
  {
    CNAMESPACE::sscanf( (ac_src+ind), SCANF_INT_STRING, &temp );
    int2littleEndianString( temp, pui8_write, HAL_SIZEOF_INT );
    pui8_write += HAL_SIZEOF_INT;
    if ( (unsigned int)( pui8_write - pui8_target ) > size ) break;
  }
  if ( ( ind < 0 ) && ( ind  > ( -1 * ( 2 * HAL_SIZEOF_INT ) ) ) )
  {
    unsigned ci_overhandByte = ind + ( 2 * HAL_SIZEOF_INT );
    switch ( ci_overhandByte )
    {
      case 1:  case 2:  CNAMESPACE::sscanf( ac_src, "%2x",  &temp );   break;
      case 3:  case 4:  CNAMESPACE::sscanf( ac_src, "%4x",  &temp );   break;
      case 5:  case 6:  CNAMESPACE::sscanf( ac_src, "%6x",  &temp );   break;
      case 7:  case 8:  CNAMESPACE::sscanf( ac_src, "%8x",  &temp );   break;
      case 9:  case 10: CNAMESPACE::sscanf( ac_src, "%10x", &temp );   break;
      case 11: case 12: CNAMESPACE::sscanf( ac_src, "%12x", &temp );   break;
    }
    unsigned int availableSize = size - ( pui8_write - pui8_target );
    if ( ci_overhandByte < availableSize ) availableSize = ci_overhandByte;
    int2littleEndianString( temp, pui8_write, availableSize );
  }
}

#if HAL_SIZEOF_INT <= 2
  #define SCANF_HEX_INT16_STRING "%4x"
#else
  #define SCANF_HEX_INT16_STRING "%4hx"
#endif

/** convert big endian textual unsigned int 8Bit number representation into little endian uint8_t string */
void bigEndianHexNumberText2CanStringUint8( const char* ac_src, uint8_t* pui8_target )
{
  if ( NULL == ac_src )
  {
    CNAMESPACE::memset( pui8_target, 0, 1 );
    return;
  }
  unsigned int temp;
  CNAMESPACE::sscanf( ac_src, "%2x",  &temp );
  pui8_target[0] = ( temp & 0xFF );
}

/** convert big endian textual unsigned int 16Bit number representation into little endian uint8_t string of specified size */
void bigEndianHexNumberText2CanStringUint16( const char* ac_src, uint8_t* pui8_target )
{
  if ( NULL == ac_src )
  {
    CNAMESPACE::memset( pui8_target, 0, 2 );
    return;
  }
#if HAL_SIZEOF_INT <= 4
  uint16_t temp;
  CNAMESPACE::sscanf( ac_src, SCANF_HEX_INT16_STRING, &temp );
  #else
  unsigned int temp;
  sscanf( ac_src, "%4x",  &temp );
  #endif
  #if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( pui8_target,   &temp, 2 );
  #else
  pui8_target[0] =   ( temp        & 0xFF );
  pui8_target[1] = ( ( temp >> 8 ) & 0xFF );
  #endif
}
/** convert big endian textual unsigned int 32Bit number representation into little endian uint8_t string of specified size */
void bigEndianHexNumberText2CanStringUint32( const char* ac_src, uint8_t* pui8_target )
{
  if ( NULL == ac_src )
  {
    CNAMESPACE::memset( pui8_target, 0, 4 );
    return;
  }
#if HAL_SIZEOF_INT <= 2
  uint32_t temp;
  CNAMESPACE::sscanf( ac_src, "%8lx", &temp );
  #elif HAL_SIZEOF_INT == 4
  uint32_t temp;
  sscanf( ac_src, "%8x", &temp );
  #else
  unsigned int temp;
  sscanf( ac_src, "%8x", &temp );
  #endif
  #if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( pui8_target,   &temp, 4 );
  #else
  pui8_target[0] =   ( temp         & 0xFF );
  pui8_target[1] = ( ( temp >> 8  ) & 0xFF );
  pui8_target[2] = ( ( temp >> 16 ) & 0xFF );
  pui8_target[3] = ( ( temp >> 24 ) & 0xFF );
  #endif
}

/** convert big endian textual unsigned int 64Bit number representation into little endian uint8_t string of specified size */
void bigEndianHexNumberText2CanStringUint64( const char* ac_src, uint8_t* pui8_target )
{
  if ( NULL == ac_src )
  {
    CNAMESPACE::memset( pui8_target, 0, 8 );
    return;
  }

#ifdef HAS_64BIT_INT_TYPE
  uint64_t temp;
  #if (HAL_SIZEOF_INT == 8)
  CNAMESPACE::sscanf( ac_src, "%16x", &temp );
  #else
  CNAMESPACE::sscanf( ac_src, "%16llx", &temp );
  #endif
  #if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( pui8_target, &temp, 8 );
  #else
  for ( unsigned int ind = 0; ind < 8; ind++ ) pui8_target[ind] = ( ( temp >> (ind*8) ) & 0xFF );
  #endif
#else
  long unsigned int temp[2] = {0UL, 0UL};
  const unsigned int len = CNAMESPACE::strlen( ac_src );
  const int lowerPartValStart = len - 8;
  if ( lowerPartValStart >= 0 )
  {
    CNAMESPACE::sscanf( ac_src+lowerPartValStart, "%8lx", &(temp[0]) );
    switch ( lowerPartValStart )
    {
      case 0: break;
      case 1: CNAMESPACE::sscanf( ac_src, "%1lx", &(temp[1]) ); break;
      case 2: CNAMESPACE::sscanf( ac_src, "%2lx", &(temp[1]) ); break;
      case 3: CNAMESPACE::sscanf( ac_src, "%3lx", &(temp[1]) ); break;
      case 4: CNAMESPACE::sscanf( ac_src, "%4lx", &(temp[1]) ); break;
      case 5: CNAMESPACE::sscanf( ac_src, "%5lx", &(temp[1]) ); break;
      case 6: CNAMESPACE::sscanf( ac_src, "%6lx", &(temp[1]) ); break;
      case 7: CNAMESPACE::sscanf( ac_src, "%7lx", &(temp[1]) ); break;
      case 8: CNAMESPACE::sscanf( ac_src, "%8lx", &(temp[1]) ); break;
    }
  }
  else
  { // source string contains only digits for lower 4-byte value
    CNAMESPACE::sscanf( ac_src, "%8lx", &(temp[0]) );
  }
  #if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( pui8_target,   &(temp[0]), 4 );
  CNAMESPACE::memcpy( pui8_target+4, &(temp[1]), 4 );
  #else
  for ( unsigned int ind = 0; ind < 2; ind++ )
  {
    pui8_target[(ind*4)]   =   ( temp[ind]         & 0xFF );
    pui8_target[(ind*4)+1] = ( ( temp[ind] >> 8  ) & 0xFF );
    pui8_target[(ind*4)+2] = ( ( temp[ind] >> 16 ) & 0xFF );
    pui8_target[(ind*4)+3] = ( ( temp[ind] >> 24 ) & 0xFF );
  }
  #endif
#endif
}


#if HAL_SIZEOF_INT <= 2
#define SCANF_DEC_INT16_STRING "%4d"
#else
#define SCANF_DEC_INT16_STRING "%4hd"
#endif

/** convert big endian textual unsigned int up to 16Bit number representation into little endian uint8_t string of specified size */
void bigEndianDecNumberText2CanStringUint( const char* ac_src, uint8_t* pui8_target )
{
  if ( NULL == ac_src )
  {
    CNAMESPACE::memset( pui8_target, 0, 2 );
    return;
  }
#if HAL_SIZEOF_INT <= 4
  uint16_t temp;
  CNAMESPACE::sscanf( ac_src, SCANF_DEC_INT16_STRING, &temp );
#else
  unsigned int temp;
  CNAMESPACE::sscanf( ac_src, "%4d",  &temp );
#endif
  #if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  CNAMESPACE::memcpy( pui8_target, &temp, 2 );
  #else
  pui8_target[0] =   ( temp        & 0xFF );
  pui8_target[1] = ( ( temp >> 8 ) & 0xFF );
  #endif
}

#ifdef USE_VT_UNICODE_SUPPORT

#define DEF_uR DEF_UNICODE_REPLACEMENT_CHAR
// latin-1 (west)
// gets directly mapped to unicode, no lookup-table needed

const uint16_t p96ui16_8859_2 [96] =
{ // latin-2 (east)
/*  0x0080,0x0081,0x0082,0x0083,0x0084,0x0085,0x0086,0x0087,0x0088,0x0089,0x008a,0x008b,0x008c,0x008d,0x008e,0x008f,
  0x0090,0x0091,0x0092,0x0093,0x0094,0x0095,0x0096,0x0097,0x0098,0x0099,0x009a,0x009b,0x009c,0x009d,0x009e,0x009f,*/
  0x00a0,0x0104,0x02d8,0x0141,0x00a4,0x013d,0x015a,0x00a7,0x00a8,0x0160,0x015e,0x0164,0x0179,0x00ad,0x017d,0x017b,
  0x00b0,0x0105,0x02db,0x0142,0x00b4,0x013e,0x015b,0x02c7,0x00b8,0x0161,0x015f,0x0165,0x017a,0x02dd,0x017e,0x017c,
  0x0154,0x00c1,0x00c2,0x0102,0x00c4,0x0139,0x0106,0x00c7,0x010c,0x00c9,0x0118,0x00cb,0x011a,0x00cd,0x00ce,0x010e,
  0x0110,0x0143,0x0147,0x00d3,0x00d4,0x0150,0x00d6,0x00d7,0x0158,0x016e,0x00da,0x0170,0x00dc,0x00dd,0x0162,0x00df,
  0x0155,0x00e1,0x00e2,0x0103,0x00e4,0x013a,0x0107,0x00e7,0x010d,0x00e9,0x0119,0x00eb,0x011b,0x00ed,0x00ee,0x010f,
  0x0111,0x0144,0x0148,0x00f3,0x00f4,0x0151,0x00f6,0x00f7,0x0159,0x016f,0x00fa,0x0171,0x00fc,0x00fd,0x0163,0x02d9
};

const uint16_t p96ui16_8859_4 [96] =
{ // latin-4 (north)
//0x0080,0x0081,0x0082,0x0083,0x0084,0x0085,0x0086,0x0087,0x0088,0x0089,0x008a,0x008b,0x008c,0x008d,0x008e,0x008f,
//0x0090,0x0091,0x0092,0x0093,0x0094,0x0095,0x0096,0x0097,0x0098,0x0099,0x009a,0x009b,0x009c,0x009d,0x009e,0x009f,
  0x00a0,0x0104,0x0138,0x0156,0x00a4,0x0128,0x013b,0x00a7,0x00a8,0x0160,0x0112,0x0122,0x0166,0x00ad,0x017d,0x00af,
  0x00b0,0x0105,0x02db,0x0157,0x00b4,0x0129,0x013c,0x02c7,0x00b8,0x0161,0x0113,0x0123,0x0167,0x014a,0x017e,0x014b,
  0x0100,0x00c1,0x00c2,0x00c3,0x00c4,0x00c5,0x00c6,0x012e,0x010c,0x00c9,0x0118,0x00cb,0x0116,0x00cd,0x00ce,0x012a,
  0x0110,0x0145,0x014c,0x0136,0x00d4,0x00d5,0x00d6,0x00d7,0x00d8,0x0172,0x00da,0x00db,0x00dc,0x0168,0x016a,0x00df,
  0x0101,0x00e1,0x00e2,0x00e3,0x00e4,0x00e5,0x00e6,0x012f,0x010d,0x00e9,0x0119,0x00eb,0x0117,0x00ed,0x00ee,0x012b,
  0x0111,0x0146,0x014d,0x0137,0x00f4,0x00f5,0x00f6,0x00f7,0x00f8,0x0173,0x00fa,0x00fb,0x00fc,0x0169,0x016b,0x02d9
};

const uint16_t p96ui16_8859_5 [96] =
{ // cyrillic
//0x0080,0x0081,0x0082,0x0083,0x0084,0x0085,0x0086,0x0087,0x0088,0x0089,0x008a,0x008b,0x008c,0x008d,0x008e,0x008f,
//0x0090,0x0091,0x0092,0x0093,0x0094,0x0095,0x0096,0x0097,0x0098,0x0099,0x009a,0x009b,0x009c,0x009d,0x009e,0x009f,
  0x00a0,0x0401,0x0402,0x0403,0x0404,0x0405,0x0406,0x0407,0x0408,0x0409,0x040a,0x040b,0x040c,0x00ad,0x040e,0x040f,
  0x0410,0x0411,0x0412,0x0413,0x0414,0x0415,0x0416,0x0417,0x0418,0x0419,0x041a,0x041b,0x041c,0x041d,0x041e,0x041f,
  0x0420,0x0421,0x0422,0x0423,0x0424,0x0425,0x0426,0x0427,0x0428,0x0429,0x042a,0x042b,0x042c,0x042d,0x042e,0x042f,
  0x0430,0x0431,0x0432,0x0433,0x0434,0x0435,0x0436,0x0437,0x0438,0x0439,0x043a,0x043b,0x043c,0x043d,0x043e,0x043f,
  0x0440,0x0441,0x0442,0x0443,0x0444,0x0445,0x0446,0x0447,0x0448,0x0449,0x044a,0x044b,0x044c,0x044d,0x044e,0x044f,
  0x2116,0x0451,0x0452,0x0453,0x0454,0x0455,0x0456,0x0457,0x0458,0x0459,0x045a,0x045b,0x045c,0x00a7,0x045e,0x045f
};

const uint16_t p96ui16_8859_7 [96] =
{ // greek
//0x0080,0x0081,0x0082,0x0083,0x0084,0x0085,0x0086,0x0087,0x0088,0x0089,0x008a,0x008b,0x008c,0x008d,0x008e,0x008f,
//0x0090,0x0091,0x0092,0x0093,0x0094,0x0095,0x0096,0x0097,0x0098,0x0099,0x009a,0x009b,0x009c,0x009d,0x009e,0x009f,
  0x00a0,0x2018,0x2019,0x00a3,DEF_uR,DEF_uR,0x00a6,0x00a7,0x00a8,0x00a9,DEF_uR,0x00ab,0x00ac,0x00ad,DEF_uR,0x2015,
  0x00b0,0x00b1,0x00b2,0x00b3,0x0384,0x0385,0x0386,0x00b7,0x0388,0x0389,0x038a,0x00bb,0x038c,0x00bd,0x038e,0x038f,
  0x0390,0x0391,0x0392,0x0393,0x0394,0x0395,0x0396,0x0397,0x0398,0x0399,0x039a,0x039b,0x039c,0x039d,0x039e,0x039f,
  0x03a0,0x03a1,DEF_uR,0x03a3,0x03a4,0x03a5,0x03a6,0x03a7,0x03a8,0x03a9,0x03aa,0x03ab,0x03ac,0x03ad,0x03ae,0x03af,
  0x03b0,0x03b1,0x03b2,0x03b3,0x03b4,0x03b5,0x03b6,0x03b7,0x03b8,0x03b9,0x03ba,0x03bb,0x03bc,0x03bd,0x03be,0x03bf,
  0x03c0,0x03c1,0x03c2,0x03c3,0x03c4,0x03c5,0x03c6,0x03c7,0x03c8,0x03c9,0x03ca,0x03cb,0x03cc,0x03cd,0x03ce,DEF_uR
};

const uint16_t p96ui16_8859_15 [96] =
{ // latin-9 (euro)
//0x0080,0x0081,0x0082,0x0083,0x0084,0x0085,0x0086,0x0087,0x0088,0x0089,0x008a,0x008b,0x008c,0x008d,0x008e,0x008f,
//0x0090,0x0091,0x0092,0x0093,0x0094,0x0095,0x0096,0x0097,0x0098,0x0099,0x009a,0x009b,0x009c,0x009d,0x009e,0x009f,
  0x00a0,0x00a1,0x00a2,0x00a3,0x20ac,0x00a5,0x0160,0x00a7,0x0161,0x00a9,0x00aa,0x00ab,0x00ac,0x00ad,0x00ae,0x00af,
  0x00b0,0x00b1,0x00b2,0x00b3,0x017d,0x00b5,0x00b6,0x00b7,0x017e,0x00b9,0x00ba,0x00bb,0x0152,0x0153,0x0178,0x00bf,
  0x00c0,0x00c1,0x00c2,0x00c3,0x00c4,0x00c5,0x00c6,0x00c7,0x00c8,0x00c9,0x00ca,0x00cb,0x00cc,0x00cd,0x00ce,0x00cf,
  0x00d0,0x00d1,0x00d2,0x00d3,0x00d4,0x00d5,0x00d6,0x00d7,0x00d8,0x00d9,0x00da,0x00db,0x00dc,0x00dd,0x00de,0x00df,
  0x00e0,0x00e1,0x00e2,0x00e3,0x00e4,0x00e5,0x00e6,0x00e7,0x00e8,0x00e9,0x00ea,0x00eb,0x00ec,0x00ed,0x00ee,0x00ef,
  0x00f0,0x00f1,0x00f2,0x00f3,0x00f4,0x00f5,0x00f6,0x00f7,0x00f8,0x00f9,0x00fa,0x00fb,0x00fc,0x00fd,0x00fe,0x00ff
};

const uint16_t* pp96ui16_8859s [8] =
{
  /* 0 */ NULL, // would be "p96ui16_8859_1", which is directly mapped to unicode - so no table lookup needed here! see special handling in code! */
  /* 1 */ p96ui16_8859_15,
  /* 2 */ p96ui16_8859_2,
  /* 3 */ NULL, // p96ui16_8859_3,
  /* 4 */ p96ui16_8859_4,
  /* 5 */ p96ui16_8859_5,
  /* 6 */ NULL, // p96ui16_8859_6,
  /* 7 */ p96ui16_8859_7,
};


void convertString8859ToUnicode (const STL_NAMESPACE::string& rrefstr_iso8859, STL_NAMESPACE::string& rrefstr_unicode, uint8_t aui8_encoding)
{
  STL_NAMESPACE::string::const_iterator cit_str8859;
  rrefstr_unicode.erase ();
  for (cit_str8859=rrefstr_iso8859.begin(); cit_str8859 != rrefstr_iso8859.end(); cit_str8859++)
  {
    uint8_t byte = *cit_str8859;
    const uint16_t cui16_unicode = __IsoAgLib::convert8859ToUnicode (byte, aui8_encoding);
    __IsoAgLib::push_backUTF8 (rrefstr_unicode, cui16_unicode);
  }
}



void convertStringUnicodeTo8859 (const char* apui8_stringUnicode, uint16_t aui16_stringUnicodeLength, char* apui8_iso8859buffer, uint8_t aui8_encoding)
{
  int i_more=0;
  uint32_t ui32_unicode = 0;
  for (int i=0; i<aui16_stringUnicodeLength; i++)
  {
    const uint8_t cui8_byte = *apui8_stringUnicode++;
    if ((cui8_byte & 0xC0) == 0x80) // check for highest 2 bits to be "10xx xxxx"
    { // continuation byte
      if (i_more > 0)
      { // yes, we did have something to continue - okay.
        ui32_unicode <<= 6;
        ui32_unicode |= cui8_byte & 0x3F;
        i_more--;
        if (i_more == 0)
        { // yes, that was the last continuation - reverse lookup and store!
          if (ui32_unicode < 0x10000)
          {
            *apui8_iso8859buffer++ = convertUnicodeTo8859 (ui32_unicode, aui8_encoding);
          }
          else // we don't support such characters, so in case they occur, we put an ISO8859-Replacement-Char there!
          {
            *apui8_iso8859buffer++ = DEF_ISO8859_REPLACEMENT_CHAR;
          }
        }
      }
      else break; // error - continuation when NOT expected!
    }
    else
    { // no continuation, is it ASCII or MULTI-BYTE BEGIN?
      if (i_more > 0) break; // error - expected CONTINUATION, but got other!

      if (cui8_byte < 0x80)
      { // ASCII, take unconverted
        *apui8_iso8859buffer++ = cui8_byte;
        continue;
        // i_more is still 0, so the value will be taken below.
      }
      else if ((cui8_byte & 0xE0) == 0xC0)
      { // expect one continuation byte
        ui32_unicode = cui8_byte & 0x1F;
        i_more = 1;
      }
      else if ((cui8_byte & 0xF0) == 0xE0)
      { // expect two continuation bytes
        ui32_unicode = cui8_byte & 0x0F;
        i_more = 2;
      }
      else if ((cui8_byte & 0xF8) == 0xF0)
      { // expect three continuation bytes
        ui32_unicode = cui8_byte & 0x07;
        i_more = 3;
      }
      else if ((cui8_byte & 0xFC) == 0xF8)
      { // expect four continuation bytes
        ui32_unicode = cui8_byte & 0x03;
        i_more = 4;
      }
      else if ((cui8_byte & 0xFE) == 0xFC)
      { // expect five continuation bytes
        ui32_unicode = cui8_byte & 0x01;
        i_more = 5;
      }
    }
  }
  *apui8_iso8859buffer = 0x00;
}


// only supports 2-byte UTF-8 encodings, but this should be enough for us...
void push_backUTF8 (STL_NAMESPACE::string& rrefstr_string, uint16_t aui16_unicode)
{
  if (aui16_unicode < 0x80)
  {
    rrefstr_string.push_back (uint8_t (aui16_unicode));
  }
  else if (aui16_unicode < 0x800)
  {
    rrefstr_string.push_back (uint8_t (0xC0 | ((aui16_unicode >> 6)       ))); // 110kjihg - no need to AND, because it's < 0x800 !
    rrefstr_string.push_back (uint8_t (0x80 | ((aui16_unicode     ) & 0x3F))); // 10fedcba
  }
  else
  {
    rrefstr_string.push_back (uint8_t (0xE0 | ((aui16_unicode >>12)       ))); // 1110ponm - no need to AND, because it's < 0x10000 !
    rrefstr_string.push_back (uint8_t (0x80 | ((aui16_unicode >> 6) & 0x3F))); // 10lkjihg
    rrefstr_string.push_back (uint8_t (0x80 | ((aui16_unicode     ) & 0x3F))); // 10fedcba
  }
}

void spacePadBomUTF16 (char* utf16Str, uint16_t byteStrLen, uint16_t bytesToPad)
{
  //Check BOM
  if( (uint8_t) utf16Str[0] == (uint8_t) 0xFE )
  {
    //-1 incase bytesToPad is odd
    for( int i = byteStrLen; i < ( bytesToPad - 1 ); i+=2 )
    {
      utf16Str[ i ] = 0x00;
      utf16Str[ i + 1 ] = 0x20;
    }
  }
  else
  {
    //-1 incase bytesToPad is odd
    for( int i = byteStrLen; i < ( bytesToPad - 1 ); i+=2 )
    {
      utf16Str[ i ] = 0x20;
      utf16Str[ i + 1 ] = 0x00;
    }
  }
}

#endif


uint8_t
VtFontScaling::getScaledFont(uint8_t aui8_originalSize, int32_t ai32_vtDimension, int32_t ai32_opDimension, uint16_t aui16_vtSupportedFonts,
                             bool ab_buttonParent, bool ab_inSkm,
                             uint16_t aui16_opButtonWidth, uint16_t aui16_opButtonHeight,
                             uint16_t aui16_opSoftKeyWidth, uint16_t aui16_opSoftKeyHeight, uint32_t aui32_vtSoftKeyWidth, uint32_t aui32_vtSoftKeyHeight)
{

  uint8_t ui8_fontSizeScaled = aui8_originalSize;
  if (ui8_fontSizeScaled > (15-1)) ui8_fontSizeScaled = (15-1);

  int32_t i32_vtButtonWidth, i32_vtButtonHeight;
  int32_t i32_factorM = 0, i32_factorD = 1; /* zaehler, nenner */
  // if it is inside button => first priority
  if (ab_buttonParent)
  {
#ifdef USE_VT_CLIENT_OLD_INCORRECT_BUTTON_CONTENT_SCALING
    if (ai32_opDimension && (aui16_opButtonWidth > 8) && (aui16_opButtonHeight > 8))
    {
      i32_vtButtonWidth  = (aui16_opButtonWidth * ai32_vtDimension) / ai32_opDimension - 8;
      i32_vtButtonHeight = (aui16_opButtonHeight * ai32_vtDimension) / ai32_opDimension - 8;
      const int32_t ci_factorX = ((i32_vtButtonWidth << 20) / (aui16_opButtonWidth-8));
      const int32_t ci_factorY = ((i32_vtButtonHeight << 20) / (aui16_opButtonHeight-8));
      if (ci_factorX < ci_factorY) {
        i32_factorM = i32_vtButtonWidth;
        i32_factorD = aui16_opButtonWidth;
      } else {
        i32_factorM = i32_vtButtonHeight;
        i32_factorD = aui16_opButtonHeight;
      }
    }
    else
      ab_buttonParent = false; // forget about it
#else
    const int buttonBorderPixel = 8;
    if (ai32_opDimension && (aui16_opButtonWidth > buttonBorderPixel) && (aui16_opButtonHeight > buttonBorderPixel))
    {
        i32_vtButtonWidth  = (aui16_opButtonWidth * ai32_vtDimension) / ai32_opDimension;
        i32_vtButtonHeight = (aui16_opButtonHeight * ai32_vtDimension) / ai32_opDimension;
        const int32_t ci_factorX = (((i32_vtButtonWidth - buttonBorderPixel) << 20) / (aui16_opButtonWidth - buttonBorderPixel));
        const int32_t ci_factorY = (((i32_vtButtonHeight - buttonBorderPixel) << 20) / (aui16_opButtonHeight - buttonBorderPixel));
        if (ci_factorX < ci_factorY) {
        i32_factorM = i32_vtButtonWidth - buttonBorderPixel;
        i32_factorD = aui16_opButtonWidth - buttonBorderPixel;
        } else {
        i32_factorM = i32_vtButtonHeight - buttonBorderPixel;
        i32_factorD = aui16_opButtonHeight - buttonBorderPixel;
        }
    }
    else
        ab_buttonParent = false; // forget about it
#endif
  }
  // if it is inside SKM => second priority
  else if (ab_inSkm)
  {
    if (aui16_opSoftKeyWidth && aui16_opSoftKeyHeight)
    {
      const int32_t ci_factorX = (aui32_vtSoftKeyWidth  << 20) / aui16_opSoftKeyWidth;
      const int32_t ci_factorY = (aui32_vtSoftKeyHeight << 20) / aui16_opSoftKeyHeight;
      if (ci_factorX < ci_factorY)
      {
        i32_factorM = aui32_vtSoftKeyWidth;
        i32_factorD = aui16_opSoftKeyWidth;
      }
      else
      {
        i32_factorM = aui32_vtSoftKeyHeight;
        i32_factorD = aui16_opSoftKeyHeight;
      }
    }
    else
      ab_inSkm = false; // forget about it
  }

  uint32_t ui32_width, ui32_height;
  uint8_t ui8_wIndex=0, ui8_hIndex=0;
  if (ab_buttonParent || ab_inSkm) {
    ui32_width = (((uint32_t) i32_factorM * (marr_font2PixelDimensionTableW [ui8_fontSizeScaled]) <<10)/i32_factorD); // (8 bit shifted fixed floating)
    ui32_height= (((uint32_t) i32_factorM * (marr_font2PixelDimensionTableH [ui8_fontSizeScaled]) <<10)/i32_factorD); // (8 bit shifted fixed floating)
  } else {
    ui32_width = (((uint32_t) ai32_vtDimension * (marr_font2PixelDimensionTableW [ui8_fontSizeScaled]) <<10)/ai32_opDimension); // (8 bit shifted fixed floating)
    ui32_height= (((uint32_t) ai32_vtDimension * (marr_font2PixelDimensionTableH [ui8_fontSizeScaled]) <<10)/ai32_opDimension); // (8 bit shifted fixed floating)
  }

  /** @todo ON REQUEST-174 maybe keep aspect ratio?? Make it a user-flag on registerIsoObjectPool? Or put it into the objects itself?? */
  // now get the lower possible size...
  int i, j;
  for (i=14; i>=0; i--) {
    if (((uint32_t (marr_font2PixelDimensionTableW [i])) << 10) <= ui32_width) {
      ui8_wIndex = uint8_t(i);
      break;
    }
  }
  for (j=14; j>=0; j--) {
    if (((uint32_t (marr_font2PixelDimensionTableH [j])) << 10) <= ui32_height) {
      ui8_hIndex = uint8_t(j);
      break;
    }
  }
  if ((i < 0) || (j < 0))
  { // too small font, smaller than 6x8... ==> take 6x8
    ui8_fontSizeScaled = 0;
  }
  else
  { // match indices together... take the lowest one, that'll do!
    if (ui8_wIndex < ui8_hIndex)
      ui8_fontSizeScaled = ui8_wIndex;
    else
      ui8_fontSizeScaled = ui8_hIndex;
  }

  /// Always check if the font is available!
  while (!(aui16_vtSupportedFonts & (1 << ui8_fontSizeScaled))) {
    ui8_fontSizeScaled--; // try a smaller font, but "6x8" should be there in any way, 'cause we set it in processMsg!!
    if (!ui8_fontSizeScaled)
      break;
  }

  return ui8_fontSizeScaled;
}

uint8_t VtFontScaling::marr_font2PixelDimensionTableW [15] = {6,  8,  8, 12, 16, 16, 24, 32, 32, 48, 64, 64, 96,128,128};
uint8_t VtFontScaling::marr_font2PixelDimensionTableH [15] = {8,  8, 12, 16, 16, 24, 32, 32, 48, 64, 64, 96,128,128,192};



int
getCStringLength (const char *acstr_text)
{
  isoaglib_assert (acstr_text);

  const char *nullByte = acstr_text;
  while ((*nullByte) != 0x00)
    ++nullByte;

  return nullByte - acstr_text;
}


// add src-string without 0x00!
// note: be sure that your dest-buffer has enough space!
void
addCStringWithoutTermination (char **dest, const char *src)
{
  while (*src != 0x00)
  {
    *(*dest) = *src;
    ++(*dest);
    ++src;
  }
}


int
getCStringCount (const char *src, char countChar)
{
  int count = 0;

  while (*src != 0x00)
  {
    if (*src == countChar)
      ++count;
    ++src;
  }

  return count;
}



} // end of namespace __IsoAgLib
