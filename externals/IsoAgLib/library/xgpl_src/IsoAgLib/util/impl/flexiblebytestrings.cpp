/*
  flexiblebytestrings.cpp:

  (C) Copyright 2012 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/


#include "flexiblebytestrings.h"

namespace __IsoAgLib {

  void Flexible4ByteString_c::setFlexible4DataValueInd( uint8_t aui8_ind, const Flexible8ByteString_c& ac_value ) {
#ifdef NO_8BIT_CHAR_TYPE
    for ( uint8_t index = 0; index < 4; index++ )
      uint8[index] = ac_value.uint8[( aui8_ind<<2 )+index];
#else
    uint32[0] = ac_value.uint32[aui8_ind];
#endif
  };


  Flexible8ByteString_c::Flexible8ByteString_c( const Flexible8ByteString_c& acrc_src ) {
#ifdef NO_8BIT_CHAR_TYPE
    for ( uint8_t index = 0; index < 8; index++ )
      uint8[index] = acrc_src.uint8[index];
#else
#ifdef HAS_64BIT_INT_TYPE
    uint64[0] = acrc_src.uint64[0];
#else
    uint32[1] = acrc_src.uint32[1];
    uint32[0] = acrc_src.uint32[0];
#endif
#endif
  };


  Flexible4ByteString_c::Flexible4ByteString_c( const uint8_t* apui8_srcStream ) {
    if ( apui8_srcStream != NULL )
      CNAMESPACE::memcpy( uint8, apui8_srcStream, 4 );
#ifndef NO_8BIT_CHAR_TYPE
    else uint32[0] = ~uint32_t( 0UL );
#else
    else
      for ( uint8_t index = 0; index < 4; index++ )
        uint8[index] = ~uint8_t( 0U );
#endif
  };


  Flexible8ByteString_c::Flexible8ByteString_c( const uint8_t* apui8_srcStream ) {
    if ( apui8_srcStream != NULL )
      CNAMESPACE::memcpy( uint8, apui8_srcStream, 8 );
    else {
#ifdef NO_8BIT_CHAR_TYPE
      for ( uint8_t index = 0; index < 8; index++ )
        uint8[index] = ~uint8_t( 0U );
#else
#ifdef HAS_64BIT_INT_TYPE
      uint64[0] = ~0ULL;
#else
      uint32[0] = uint32[1] = ~uint32_t( 0UL );
#endif
#endif
    }
  };


  void
  Flexible4ByteString_c::setDataFromString( uint8_t aui8_offset, const uint8_t* apui8_srcStream, uint8_t aui8_len ) {
    if( aui8_len == 0 )
      return;

    isoaglib_assert ( ( apui8_srcStream != NULL ) && ( ( aui8_len+aui8_offset )<=4 ) );
    CNAMESPACE::memcpy ( uint8+aui8_offset, apui8_srcStream, aui8_len );
  }


  void
  Flexible4ByteString_c::getDataToString( uint8_t aui8_offset, uint8_t* pui8_targetStream, uint8_t aui8_len ) const {
    isoaglib_assert ( ( pui8_targetStream != NULL ) && ( ( aui8_len+aui8_offset )<=4 ) );
    CNAMESPACE::memcpy ( pui8_targetStream, uint8+aui8_offset, aui8_len );
  }


  void
  Flexible8ByteString_c::setDataFromString( uint8_t aui8_offset, const uint8_t* apui8_srcStream, uint8_t aui8_len ) {
    if( aui8_len == 0 )
      return;

    isoaglib_assert ( ( apui8_srcStream != NULL ) && ( ( aui8_len+aui8_offset )<=8 ) );
    CNAMESPACE::memcpy ( uint8+aui8_offset, apui8_srcStream, aui8_len );
  }


  void
  Flexible8ByteString_c::getDataToString( uint8_t aui8_offset, uint8_t* pui8_targetStream, uint8_t aui8_len ) const {
    isoaglib_assert ( ( pui8_targetStream != NULL ) && ( ( aui8_len+aui8_offset )<=8 ) );
    CNAMESPACE::memcpy ( pui8_targetStream, uint8+aui8_offset, aui8_len );
  }


  const Flexible8ByteString_c &
  Flexible8ByteString_c::operator=( const Flexible8ByteString_c& acrc_src ) {
#if defined ( NO_8BIT_CHAR_TYPE )
    for ( uint8_t index = 0; index < 8; index++ )
      uint8[index] = acrc_src.uint8[index];
#elif defined ( HAS_64BIT_INT_TYPE )
    uint64[0] = acrc_src.uint64[0];
#else
    uint32[1] = acrc_src.uint32[1];
    uint32[0] = acrc_src.uint32[0];
#endif
    return *this;
  }


  bool
  Flexible8ByteString_c::operator==( const Flexible8ByteString_c& acrc_cmp ) const {
#if defined ( NO_8BIT_CHAR_TYPE )
    for ( uint8_t index = 0; index < 8; index++ )
      if ( uint8[index] != acrc_cmp.uint8[index] )
        return false;

    return true;
#elif defined ( HAS_64BIT_INT_TYPE )
    return ( uint64[0] == acrc_cmp.uint64[0] );
#else
    return ( ( uint32[1] == acrc_cmp.uint32[1] )
             && ( uint32[0] == acrc_cmp.uint32[0] ) );
#endif
  }


  bool
  Flexible8ByteString_c::operator!=( const Flexible8ByteString_c& acrc_cmp ) const {
#if defined ( NO_8BIT_CHAR_TYPE )
    for ( uint8_t index = 0; index < 8; index++ )
      if ( uint8[index] != acrc_cmp.uint8[index] )
        return true;
    return false;
#elif defined ( HAS_64BIT_INT_TYPE )
    return ( uint64[0] != acrc_cmp.uint64[0] );
#else
    return ( ( uint32[1] != acrc_cmp.uint32[1] ) || ( uint32[0] != acrc_cmp.uint32[0] ) );
#endif
  }


#if !defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) || defined(NO_8BIT_CHAR_TYPE)
  bool
  Flexible4ByteString_c::operator<( const Flexible4ByteString_c& acrc_cmp ) const {
    for ( int ind = 3; ind >= 0; ind-- ) {
      if ( uint8[ind] != acrc_cmp.uint8[ind] )
        return ( uint8[ind] < acrc_cmp.uint8[ind] );
    }
    // if reached here - all comparisons led to EQUAL
    return false;
  }


  bool
  Flexible4ByteString_c::operator>( const Flexible4ByteString_c& acrc_cmp ) const {
    for ( int ind = 3; ind >= 0; ind-- ) {
      if ( uint8[ind] != acrc_cmp.uint8[ind] )
        return ( uint8[ind] > acrc_cmp.uint8[ind] );
    }
    // if reached here - all comparisons led to EQUAL
    return false;
  }


  int
  Flexible4ByteString_c::compare( const Flexible4ByteString_c& acrc_cmp ) const {
    for ( int ind = 3; ind >= 0; ind-- ) {
      if      ( uint8[ind] < acrc_cmp.uint8[ind] ) return -1;
      else if ( uint8[ind] > acrc_cmp.uint8[ind] ) return +1;
    }
    return 0;
  }
#endif // end !defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN)


  bool
  Flexible8ByteString_c::operator<( const Flexible8ByteString_c& acrc_cmp ) const {
#if !defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) || defined(NO_8BIT_CHAR_TYPE)
    for ( int ind = 7; ind >= 0; ind-- ) {
      if ( uint8[ind] != acrc_cmp.uint8[ind] )
        return ( uint8[ind] < acrc_cmp.uint8[ind] );
    }
    return false;
#elif defined ( HAS_64BIT_INT_TYPE )
    return uint64[0] < acrc_cmp.uint64[0];
#else
    if ( uint32[1] != acrc_cmp.uint32[1] )
      return ( uint32[1] < acrc_cmp.uint32[1] );
    else
      return ( uint32[0] < acrc_cmp.uint32[0] );
#endif
  }


  bool
  Flexible8ByteString_c::operator>( const Flexible8ByteString_c& acrc_cmp ) const {
#if !defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) || defined(NO_8BIT_CHAR_TYPE)
    for ( int ind = 7; ind >= 0; ind-- ) {
      if ( uint8[ind] != acrc_cmp.uint8[ind] )
        return ( uint8[ind] > acrc_cmp.uint8[ind] );
    }
    return false;
#elif defined ( HAS_64BIT_INT_TYPE )
    return ( uint64[0] > acrc_cmp.uint64[0] );
#else
    if ( uint32[1] != acrc_cmp.uint32[1] )
      return ( uint32[1] > acrc_cmp.uint32[1] );
    else
      return ( uint32[0] > acrc_cmp.uint32[0] );
#endif
  }


  int
  Flexible8ByteString_c::compare( const Flexible8ByteString_c& acrc_cmp ) const {
#if !defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) || defined(NO_8BIT_CHAR_TYPE)
    for ( int ind = 7; ind >= 0; ind-- ) {
      if      ( uint8[ind] < acrc_cmp.uint8[ind] ) return -1;
      else if ( uint8[ind] > acrc_cmp.uint8[ind] ) return +1;
    }
#elif defined ( HAS_64BIT_INT_TYPE )
    if      ( uint64[0] < acrc_cmp.uint64[0] ) return -1;
    else if ( uint64[0] > acrc_cmp.uint64[0] ) return +1;
#else
    if      ( uint32[1] < acrc_cmp.uint32[1] ) return -1;
    else if ( uint32[1] > acrc_cmp.uint32[1] ) return +1;
    if      ( uint32[0] < acrc_cmp.uint32[0] ) return -1;
    else if ( uint32[0] > acrc_cmp.uint32[0] ) return +1;
#endif
    return 0;
  };

  void
  Flexible8ByteString_c::setFloatData( uint8_t aui8_pos, const float af_val ) {
    isoaglib_assert ( aui8_pos <= 4 );
    floatVar2LittleEndianStream( &af_val, ( uint8+aui8_pos ) );
  }


#if defined(OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN) && !defined(NO_8BIT_CHAR_TYPE)
  void
  Flexible4ByteString_c::setUint16Data( uint8_t aui8_pos, uint16_t aui16_val ) {
    isoaglib_assert ( aui8_pos <= 2 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st uint16_t*/
        uint16[0] = aui16_val;
        break;
      case 2: /* Byte2 -> 2nd uint16_t*/
        uint16[1] = aui16_val;
        break;
      default:
        numberRef2LittleEndianString( aui16_val, ( uint8+1 ) );
        break;
    };
  }


  void
  Flexible4ByteString_c::setInt16Data( uint8_t aui8_pos, int16_t ai16_val ) {
    isoaglib_assert ( aui8_pos <= 2 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st uint16_t*/
        int16[0] = ai16_val;
        break;
      case 2: /* Byte2 -> 2nd  int16_t*/
        int16[1] = ai16_val;
        break;
      default:
        numberRef2LittleEndianString( ai16_val, ( uint8+1 ) );
        break;
    };
  }


  uint16_t
  Flexible4ByteString_c::getUint16Data( uint8_t aui8_pos ) const {
    isoaglib_assert ( aui8_pos <= 2 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st uint16_t*/
        return uint16[0];
      case 2: /* Byte2 -> 2nd uint16_t*/
        return uint16[1];
      default:
        return convertLittleEndianStringUi16( uint8+1 );
    };
  }


  int16_t
  Flexible4ByteString_c::getInt16Data( uint8_t aui8_pos ) const {
    isoaglib_assert ( aui8_pos <= 2 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st uint16_t*/
        return int16[0];
      case 2: /* Byte2 -> 2nd uint16_t*/
        return int16[1];
      default:
        return convertLittleEndianStringI16( uint8+1 );
    };
  }


  void
  Flexible8ByteString_c::setUint16Data( uint8_t aui8_pos, uint16_t aui16_val ) {
    isoaglib_assert ( aui8_pos <= 6 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st uint16_t*/
        uint16[0] = aui16_val;
        break;
      case 2: /* Byte2 -> 2nd uint16_t*/
        uint16[1] = aui16_val;
        break;
      case 4: /* Byte4 -> 3rd uint16_t*/
        uint16[2] = aui16_val;
        break;
      case 6: /* Byte6 -> 4th uint16_t*/
        uint16[3] = aui16_val;
        break;
      default:
        numberRef2LittleEndianString( aui16_val, ( uint8+aui8_pos ) );
        break;
    };
  }


  void
  Flexible8ByteString_c::setInt16Data( uint8_t aui8_pos, int16_t ai16_val ) {
    isoaglib_assert ( aui8_pos <= 6 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st int16_t*/
        int16[0] = ai16_val;
        break;
      case 2: /* Byte2 -> 2nd int16_t*/
        int16[1] = ai16_val;
        break;
      case 4: /* Byte4 -> 3rd int16_t*/
        int16[2] = ai16_val;
        break;
      case 6: /* Byte6 -> 4th int16_t*/
        int16[3] = ai16_val;
        break;
      default:
        numberRef2LittleEndianString( ai16_val, ( uint8+aui8_pos ) );
        break;
    };
  }


  uint16_t
  Flexible8ByteString_c::getUint16Data( uint8_t aui8_pos ) const {
    isoaglib_assert ( aui8_pos <= 6 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st uint16_t*/
        return uint16[0];
      case 2: /* Byte2 -> 2nd uint16_t*/
        return uint16[1];
      case 4: /* Byte4 -> 3rd uint16_t*/
        return uint16[2];
      case 6: /* Byte6 -> 4th uint16_t*/
        return uint16[3];
      default:
        return convertLittleEndianStringUi16( uint8+aui8_pos );
    };
  }


  int16_t
  Flexible8ByteString_c::getInt16Data( uint8_t aui8_pos ) const {
    isoaglib_assert ( aui8_pos <= 6 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st int16_t*/
        return int16[0];
      case 2: /* Byte2 -> 2nd int16_t*/
        return int16[1];
      case 4: /* Byte4 -> 3rd int16_t*/
        return int16[2];
      case 6: /* Byte6 -> 4th int16_t*/
        return int16[3];
      default:
        return convertLittleEndianStringI16( uint8+aui8_pos );
    };
  }


  float
  Flexible8ByteString_c::getFloatData( uint8_t aui8_pos ) const {
    isoaglib_assert ( aui8_pos <= 4 );
    float f;
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st float*/
        littleEndianStream2FloatVar( &uint32[0], &f );
        return f;
      case 4: /* Byte4 -> 2nd float*/
        littleEndianStream2FloatVar( &uint32[1], &f );
        return f;
      default:
        littleEndianStream2FloatVar( uint8+aui8_pos, &f );
        return f;
    }
  }


  void
  Flexible8ByteString_c::setUint32Data( uint8_t aui8_pos, uint32_t aui32_val ) {
    isoaglib_assert ( aui8_pos <= 4 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st uint32_t*/
        uint32[0] = aui32_val;
        break;
      case 4: /* Byte4 -> 2nd uint32_t*/
        uint32[1] = aui32_val;
        break;
      default:
        numberRef2LittleEndianString( aui32_val, ( uint8+aui8_pos ) );
        break;
    };
  }


  void
  Flexible8ByteString_c::setInt32Data( uint8_t aui8_pos, int32_t ai32_val ) {
    isoaglib_assert ( aui8_pos <= 4 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st int32_t*/
        int32[0] = ai32_val;
        break;
      case 4: /* Byte4 -> 2nd int32_t*/
        int32[1] = ai32_val;
        break;
      default:
        numberRef2LittleEndianString( ai32_val, ( uint8+aui8_pos ) );
        break;
    };
  }


  uint32_t
  Flexible8ByteString_c::getUint32Data( uint8_t aui8_pos ) const {
    isoaglib_assert ( aui8_pos <= 4 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st uint32_t*/
        return uint32[0];
      case 4: /* Byte4 -> 2nd uint32_t*/
        return uint32[1];
      default:
        return convertLittleEndianStringUi32( uint8+aui8_pos );
    };
  }


  int32_t
  Flexible8ByteString_c::getInt32Data( uint8_t aui8_pos ) const {
    isoaglib_assert ( aui8_pos <= 4 );
    switch ( aui8_pos ) {
      case 0: /* Byte0 -> 1st int32_t*/
        return int32[0];
      case 4: /* Byte4 -> 2nd int32_t*/
        return int32[1];
      default:
        return convertLittleEndianStringI32( uint8+aui8_pos );
    };
  }
#endif // end OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN

}
