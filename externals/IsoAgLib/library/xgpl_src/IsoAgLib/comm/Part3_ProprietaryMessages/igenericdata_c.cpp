/*
  igenericdata_c.h: Class to independently handle any length of
    data-message

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "igenericdata_c.h"

#include <IsoAgLib/util/iliberr_c.h>
#include <IsoAgLib/util/impl/util_funcs.h>


namespace IsoAgLib
{
  void iGenericData_c::setDataUi8( uint16_t aui16_pos, uint8_t aui8_data)
  {
    /** first check size of vector */
    CheckSizeOfVectorForWrite( aui16_pos, sizeof(aui8_data) );
    /** overloaded generic method to set the right data */
    __IsoAgLib::number2LittleEndianString( aui8_data, vec_data, aui16_pos);
  }

  void iGenericData_c::setDataI8( uint16_t aui16_pos, int8_t ai8_data)
  {
      /** first check size of vector */
    CheckSizeOfVectorForWrite( aui16_pos, sizeof(ai8_data) );
    /** overloaded generic method to set the right data */
    __IsoAgLib::number2LittleEndianString( ai8_data, vec_data, aui16_pos);
  }

  void iGenericData_c::setDataUi16( uint16_t aui16_pos, uint16_t aui16_data)
  {
    /** first check size of vector */
    CheckSizeOfVectorForWrite( aui16_pos, sizeof(aui16_data) );
    /** overloaded generic method to set the right data */
    __IsoAgLib::number2LittleEndianString( aui16_data, vec_data, aui16_pos);
  }

  void iGenericData_c::setDataI16( uint16_t aui16_pos, int16_t ai16_data)
  {
    /** first check size of vector */
    CheckSizeOfVectorForWrite( aui16_pos, sizeof(ai16_data) );
    /** overloaded generic method to set the right data */
    __IsoAgLib::number2LittleEndianString( ai16_data, vec_data, aui16_pos);
  }

  void iGenericData_c::setDataUi32( uint16_t aui16_pos, uint32_t aui32_data)
  {
    /** first check size of vector */
    CheckSizeOfVectorForWrite( aui16_pos, sizeof(aui32_data) );
    /** overloaded generic method to set the right data */
    __IsoAgLib::number2LittleEndianString( aui32_data, vec_data, aui16_pos);
  }

  void iGenericData_c::setDataI32( uint16_t aui16_pos, int32_t ai32_data)
  {
    /** first check size of vector */
    CheckSizeOfVectorForWrite( aui16_pos, sizeof(ai32_data) );
    /** overloaded generic method to set the right data */
    __IsoAgLib::number2LittleEndianString( ai32_data, vec_data, aui16_pos);
  }


  uint8_t iGenericData_c::getDataUi8( uint16_t aui16_pos) const
  {
    isoaglib_assert( CheckSizeOfVectorForRead( aui16_pos, sizeof(uint8_t) ) );
    return __IsoAgLib::convertLittleEndianStringUi8 (&vec_data[ aui16_pos ]);
  }

  int8_t iGenericData_c::getDataI8( uint16_t aui16_pos) const
  {
    isoaglib_assert( CheckSizeOfVectorForRead( aui16_pos, sizeof(int8_t) ) );
    return __IsoAgLib::convertLittleEndianStringI8 (&vec_data[ aui16_pos ]);
  }

  uint16_t iGenericData_c::getDataUi16( uint16_t aui16_pos) const
  {
    isoaglib_assert( CheckSizeOfVectorForRead( aui16_pos, sizeof(uint16_t) ) );
    return __IsoAgLib::convertLittleEndianStringUi16 (&vec_data[ aui16_pos ]);
  }

  int16_t iGenericData_c::getDataI16( uint16_t aui16_pos) const
  {
    isoaglib_assert( CheckSizeOfVectorForRead( aui16_pos, sizeof(int16_t) ) );
    return __IsoAgLib::convertLittleEndianStringI16 (&vec_data[ aui16_pos ]);
  }

  uint32_t iGenericData_c::getDataUi32( uint16_t aui16_pos) const
  {
    isoaglib_assert( CheckSizeOfVectorForRead( aui16_pos, sizeof(uint32_t) ) );
    return __IsoAgLib::convertLittleEndianStringUi32 (&vec_data[ aui16_pos ]);
  }

  int32_t iGenericData_c::getDataI32( uint16_t aui16_pos) const
  {
    isoaglib_assert( CheckSizeOfVectorForRead( aui16_pos, sizeof(int32_t) ) );
    return __IsoAgLib::convertLittleEndianStringI32 (&vec_data[ aui16_pos ]);
  }

  void iGenericData_c::setDataStream(uint16_t aui16_pos, const uint8_t& arui8_data, uint16_t aui16_dataLength)
  {
    /** first check size of vector */
    CheckSizeOfVectorForWrite( aui16_pos, aui16_dataLength );
    /** then copy all the bytes over */
    CNAMESPACE::memcpy (&vec_data[ aui16_pos ], &arui8_data, aui16_dataLength);
  }

  void iGenericData_c::setDataStream(uint16_t aui16_pos, const uint8_t* apui8_data, uint16_t aui16_dataLength)
  { 
    isoaglib_assert( apui8_data );
    setDataStream (aui16_pos, *apui8_data, aui16_dataLength);
  }

  const uint8_t* iGenericData_c::getDataStream(uint16_t aui16_bytePos) const
  {
    return( &(vec_data[ aui16_bytePos ]) );
  }

  void iGenericData_c::CheckSizeOfVectorForWrite( uint16_t aui16_pos, uint16_t aui16_size )
  {
    const STL_NAMESPACE::vector<uint8_t>::size_type csize_vector = vec_data.size();
    const uint16_t cui16_limit = (aui16_pos + aui16_size);

    /** size of the vector is less than what is needed for this write */
    if (csize_vector < cui16_limit)
    { /** initialize new elements of the vector*/
      vec_data.resize (cui16_limit, 0x00);
    }
  }

  bool iGenericData_c::CheckSizeOfVectorForRead( uint16_t aui16_pos, uint16_t aui16_size ) const
  {
    return ( static_cast<uint32_t>(aui16_pos + aui16_size) <= vec_data.size() );
  }

  uint16_t iGenericData_c::getLen() const
  {
    return vec_data.size();
  }


};

