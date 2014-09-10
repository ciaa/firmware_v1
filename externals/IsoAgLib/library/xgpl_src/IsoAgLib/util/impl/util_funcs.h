/*
  util_funcs.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef UTIL_FUNCS_H
#define UTIL_FUNCS_H

#include <IsoAgLib/isoaglib_config.h>
#ifdef USE_DATASTREAMS_IO
class StreamInput_c;
#endif
#include <cstdlib>	// Include before vector or else CNAMESPACE stuff is screwed up for Tasking
#include <cstring>
#include <vector>
#ifdef USE_VT_UNICODE_SUPPORT
#include <string>
#endif

// Begin Namespace __IsoAgLib
namespace __IsoAgLib
{
  template <class T> inline T abs(const T& val)
  {
    return (val < 0) ? (-val) : val;
  }

  uint8_t bcd2dec(uint8_t ab_bcd);
  uint8_t dec2bcd(uint8_t ab_dec);
/**
  calculate res = (mul_1 / div_1) * (mul_2 / div_2) with 4 uint8_t
  integers without overflow problems caused by (mul_1 * mul_2)
  and without rounding errors by (mul_1 / div_1) * (mul_2 / div_2)
  @param ai32_mul_1 first factor for multiplication
  @param ai32_div_1 first divisor for first factor
  @param ai32_mul_2 second factor for multiplication
  @param ai32_div_2 second divisor for first factor
  @return result as int32_t value
*/
int32_t mul1Div1Mul2Div2(int32_t ai32_mul_1, int32_t ai32_div_1, int32_t ai32_mul_2, int32_t ai32_div_2);

/**
  copy 4 uint8_t data string into pointer to float value;
  use e.g. to access uint8_t memory representation of
  int32_t value as float value;
  often needed for CanPkg_c formating
  @param pvFrom source data string
  @param pf_to target float value
*/
void littleEndianStream2FloatVar(const void *const pvFrom, float *const pf_to);
/**
  copy float value to 4 uint8_t data string into pointer;
  use e.g. to access uint8_t memory representation of
  float value as int32_t value;
  often needed for CanPkg_c formating
  @param pf_from source float value
  @param pvTo target data string
*/
void floatVar2LittleEndianStream(const float *const pf_from, void *const pvTo);


/** convert little endian byte string into an unsigned variable */
template<class T> void convertLittleEndianString( const uint8_t* apui8_src, T& r_result )
{
  #if defined(NO_8BIT_CHAR_TYPE)
  for ( int ind = sizeof(T)*2-1; ind >= 0; ind-- )
  {
    r_result |= (T(apui8_src[ind] & 0xFF) << (8*ind));
  }
  #elif defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN)
  CNAMESPACE::memcpy( &r_result, apui8_src, sizeof(T) );
  #else
  for ( int ind = sizeof(T)-1; ind >= 0; ind-- )
  {
    r_result |= (T(apui8_src[ind]) << (8*ind));
  }

  #endif
}

/** convert receive multistream into an unsigned variable */
uint8_t convertLittleEndianStringUi8( const uint8_t* apui8_src );
/** convert receive multistream into an unsigned variable */
inline int8_t convertLittleEndianStringI8( const uint8_t* apui8_src ){ return (int8_t)(apui8_src[0]);};
/** convert receive multistream into an unsigned variable */
uint16_t convertLittleEndianStringUi16( const uint8_t* apui8_src );
/** convert receive multistream into an unsigned variable */
int16_t convertLittleEndianStringI16( const uint8_t* apui8_src );
/** convert receive multistream into an unsigned variable */
uint32_t convertLittleEndianStringUi32( const uint8_t* apui8_src );
/** convert receive multistream into an unsigned variable */
int32_t convertLittleEndianStringI32( const uint8_t* apui8_src );

#ifdef HAS_64BIT_INT_TYPE
uint64_t convertLittleEndianStringUi64( const uint8_t* apui8_src );
int64_t convertLittleEndianStringI64( const uint8_t* apui8_src );
#endif

/** convert receive multistream into a float variable */
float convertLittleEndianStringFloat( const uint8_t* apui8_src );

/** convert number reference variable to little endian byte string */
template<class T> void numberRef2LittleEndianString( const T& acrc_src, uint8_t* pui8_target )
{
  #if defined(NO_8BIT_CHAR_TYPE)
  const unsigned int BitSize = sizeof(T) * 16;
  for ( unsigned int ind = 0; ( ind < BitSize ); ind += 8 )
  {
   *pui8_target = ((acrc_src >> ind) & 0xFF);
    pui8_target++;
  }
  #elif defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN)
  CNAMESPACE::memcpy( pui8_target, &acrc_src, sizeof(T) );
  #else
  const unsigned int BitSize = sizeof(T) * 8;
  for ( unsigned int ind = 0; ( ind < BitSize ); ind += 8 )
  {
    *pui8_target = ((acrc_src >> ind) & 0xFF);
    pui8_target++;
  }
  #endif
}
/** convert number call-by-val variable to little endian byte string */
template<class T> void number2LittleEndianString( const T at_src, uint8_t* pui8_target )
{
  #if defined(NO_8BIT_CHAR_TYPE)
  const unsigned int BitSize = sizeof(T) * 16;
  for ( unsigned int ind = 0; ( ind < BitSize ); ind += 8 )
  {
   *pui8_target = ((at_src >> ind) & 0xFF);
    pui8_target++;
  }
  #elif defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN)
  CNAMESPACE::memcpy( pui8_target, &at_src, sizeof(T) );
  #else
  const unsigned int BitSize = sizeof(T) * 8;
  for ( unsigned int ind = 0; ( ind < BitSize ); ind += 8 )
  {
    *pui8_target = ((at_src >> ind) & 0xFF);
    pui8_target++;
  }
  #endif
}


/** convert number reference variable to little endian byte string */
template<class T> void numberRef2LittleEndianString( const T& acrc_src, STL_NAMESPACE::vector<uint8_t>& acrc_target )
{
#if defined(NO_8BIT_CHAR_TYPE)
  const unsigned int BitSize = sizeof(T) * 16;
  for ( unsigned int ind = 0; ( ind < BitSize ); ind += 8 )
  {
    acrc_target.push_back((acrc_src >> ind) & 0xFF);
  }
#elif defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN)
  const uint8_t* pui8_src = (const uint8_t*)(&acrc_src);
  const unsigned int size = sizeof(T);
  for ( unsigned int ind = 0; ind < size; ind++ )
  {
    acrc_target.push_back( pui8_src[ind] );
  }
#else
  const unsigned int BitSize = sizeof(T) * 8;
  for ( unsigned int ind = 0; ( ind < BitSize ); ind += 8 )
  {
    acrc_target.push_back((acrc_src >> ind) & 0xFF);
  }
#endif
}
/** convert number call-by-val variable to little endian byte string */
template<class T> void number2LittleEndianString( const T at_src, STL_NAMESPACE::vector<uint8_t>& rc_target )
{
#if defined(NO_8BIT_CHAR_TYPE)
  const unsigned int BitSize = sizeof(T) * 16;
  for ( unsigned int ind = 0; ( ind < BitSize ); ind += 8 )
  {
    rc_target.push_back((at_src >> ind) & 0xFF);
  }
#elif defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN)
  const uint8_t* pui8_src = (const uint8_t*)(&at_src);
  const unsigned int size = sizeof(T);
  for ( unsigned int ind = 0; ind < size; ind++ )
  {
    rc_target.push_back( pui8_src[ind] );
  }
#else
  const unsigned int BitSize = sizeof(T) * 8;
  for ( unsigned int ind = 0; ( ind < BitSize ); ind += 8 )
  {
    rc_target.push_back((at_src >> ind) & 0xFF);
  }
#endif
}

/** convert number call-by-val variable to little endian byte string */
template<class T> void number2LittleEndianString( const T at_src, STL_NAMESPACE::vector<uint8_t>& rc_target, uint16_t aui16_bytePos)
{
#if defined(NO_8BIT_CHAR_TYPE)
  const unsigned int BitSize = sizeof(T) * 16;
  for ( unsigned int ind = 0; ( ind < BitSize ); ind += 8 )
  {
    rc_target[ aui16_bytePos ] = (at_src >> ind) & 0xFF;
    aui16_bytePos++;
  }
#elif defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN)
  const uint8_t* pui8_src = (const uint8_t*)(&at_src);
  const unsigned int size = sizeof(T);
  for ( unsigned int ind = 0; ind < size; ind++ )
  {
    rc_target[ aui16_bytePos ]= pui8_src[ind];
    aui16_bytePos++;
  }
#else
  const unsigned int BitSize = sizeof(T) * 8;
  for ( unsigned int ind = 0; ( ind < BitSize ); ind += 8 )
  {
    rc_target[ aui16_bytePos ] = (at_src >> ind) & 0xFF;
    aui16_bytePos++;
  }
#endif
}

/** convert big endian textual number representation into little endian uint8_t string of specified size */
void bigEndianHexNumberText2CanString( const char* ac_src, uint8_t* pui8_target, unsigned int size );

/** convert big endian textual unsigned int 8Bit number representation into little endian uint8_t string */
void bigEndianHexNumberText2CanStringUint8( const char* ac_src, uint8_t* pui8_target );
/** convert big endian textual unsigned int 16Bit number representation into little endian uint8_t string of specified size */
void bigEndianHexNumberText2CanStringUint16( const char* ac_src, uint8_t* pui8_target );
/** convert big endian textual unsigned int 32Bit number representation into little endian uint8_t string of specified size */
void bigEndianHexNumberText2CanStringUint32( const char* ac_src, uint8_t* pui8_target );
/** convert big endian textual unsigned int 64Bit number representation into little endian uint8_t string of specified size */
void bigEndianHexNumberText2CanStringUint64( const char* ac_src, uint8_t* pui8_target );
/** convert big endian textual unsigned int up to 16Bit number representation into little endian uint8_t string of specified size */
void bigEndianDecNumberText2CanStringUint( const char* ac_src, uint8_t* pui8_target );


#ifdef USE_DATASTREAMS_IO
/** convert receive multistream (LE) into an unsigned 16-bit variable */
uint16_t convertIstreamUi16( StreamInput_c& rc_stream );
/** convert receive multistream (LE) into a signed 16-bit variable */
inline
int16_t convertIstreamI16( StreamInput_c& rc_stream )
{ return static_cast <int16_t>(convertIstreamUi16 (rc_stream)); }

/** convert receive multistream (LE) into an unsigned 32-bit variable */
uint32_t convertIstreamUi32( StreamInput_c& rc_stream );
/** convert receive multistream (LE) into a signed 32-bit variable */
inline
int32_t convertIstreamI32( StreamInput_c& rc_stream )
{ return static_cast <int32_t>(convertIstreamUi32 (rc_stream)); }
#endif


/// Some CString functions
/// (replacing the need for C standard library calls)

int
getCStringLength (const char *acstr_text);

void
addCStringWithoutTermination (char **dest, const char *src);

int
getCStringCount (const char *src, char countChar);




#ifdef USE_VT_UNICODE_SUPPORT
  /// Define UNICODE REPLACEMENT CHARACTER
  #define DEF_UNICODE_REPLACEMENT_CHAR 0xFFFD
  /// Define ISO-8859 REPLACEMENT CHARACTER (for any iso8859)
  #define DEF_ISO8859_REPLACEMENT_CHAR 0x20

  // forward declarations for inline functions!
  extern const uint16_t p96ui16_8859_2 [96];
  extern const uint16_t p96ui16_8859_4 [96];
  extern const uint16_t p96ui16_8859_5 [96];
  extern const uint16_t p96ui16_8859_7 [96];
  extern const uint16_t p96ui16_8859_15 [96];
  extern const uint16_t* pp96ui16_8859s [8];

  inline uint16_t convert8859ToUnicode (uint8_t aui8_char, uint8_t aui8_encoding)
  {
    if ((aui8_char < 0xA0) || (aui8_encoding == 0))
    { // direct mapping for [0x00..0x9F] and latin-1
      return uint16_t (aui8_char);
    }
    else // (aui8_char >= 0xA0) && (aui8_encoding != 0)
    { // lookup!
      return pp96ui16_8859s [aui8_encoding] [aui8_char - 0xA0]; // checking for valid encoding is omitted for performance reasons - there shouldn't be wrong entries in FontAttributes anyway!
    }
  }

  /// @todo ON REQUEST OPTIMIZE: Reverse lookup up to now done a slow but non-space-consuming way. To be optimized later on using a generated reverse lookup table */
  /// keep inlined, as it will be optimized, so it's smaller then!
  inline uint8_t convertUnicodeTo8859 (uint16_t aui16_unicode, uint8_t aui8_encoding)
  {
    if (aui8_encoding == 0)
    {
      if (aui16_unicode >= 0x100)
        return DEF_ISO8859_REPLACEMENT_CHAR; // return iso8859-Replacement Character
      else
        return uint8_t (aui16_unicode);
    }
    const uint16_t* p96ui16_8859s = pp96ui16_8859s [aui8_encoding];
    if (aui16_unicode != DEF_UNICODE_REPLACEMENT_CHAR)
    {
      for (uint16_t ui16_cur=0xA0; ui16_cur < 0x100; ui16_cur++)
      {
        if (*p96ui16_8859s++ == aui16_unicode)
        {
          return uint8_t (ui16_cur);
        }
      }
    }
    return DEF_ISO8859_REPLACEMENT_CHAR; // default to iso8859-Replacement Character in case the char's not found;
  }

  void convertString8859ToUnicode (const STL_NAMESPACE::string& rrefstr_iso8859, STL_NAMESPACE::string& rrefstr_unicode, uint8_t aui8_encoding);
  void convertStringUnicodeTo8859 (const char* apui8_stringUnicode, uint16_t aui16_stringUnicodeLength, char* apui8_iso8859buffer, uint8_t aui8_encoding);
  void push_backUTF8 (STL_NAMESPACE::string& rrefstr_string, uint16_t aui16_unicode);
  void spacePadBomUTF16 (char* utf16Str, uint16_t byteStrLen, uint16_t bytesToPad);
#endif

class VtFontScaling
{
public:
  static uint8_t getScaledFont(uint8_t aui8_originalSize, int32_t ai32_vtDimension, int32_t ai32_opDimension, uint16_t aui16_vtSupportedFonts,
                               bool ab_buttonParent, bool ab_inSkm,
                               uint16_t aui16_opButtonWidth, uint16_t aui16_opButtonHeight,
                               uint16_t aui16_opSoftKeyWidth, uint16_t aui16_opSoftKeyHeight, uint32_t aui32_vtSoftKeyWidth, uint32_t aui32_vtSoftKeyHeight);
private:
  static uint8_t marr_font2PixelDimensionTableW[15];
  static uint8_t marr_font2PixelDimensionTableH[15];
};


class Subsystem_c
{
public:
  Subsystem_c()
    : mb_initialized (false)
  {}

  void setInitialized()   { mb_initialized = true; }
  void setClosed()        { mb_initialized = false; }
  bool initialized() const { return mb_initialized; }

private:
  bool mb_initialized;
};

} // end of namespace __IsoAgLib
#endif
