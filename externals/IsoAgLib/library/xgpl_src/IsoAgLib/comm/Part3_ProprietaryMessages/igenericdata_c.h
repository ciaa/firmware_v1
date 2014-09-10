/*
  igenericdata_c.h: Class to independently handle any length of
    data-message (along with CAN-identifier)

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IGENERICDATA_H
#define IGENERICDATA_H

#include <IsoAgLib/isoaglib_config.h>
#include <cstdlib>	// Include before vector or else CNAMESPACE stuff is screwed up for Tasking
#include <vector>


/** Begin Namespace IsoAgLib */
namespace IsoAgLib
{
  /** storing the data in a vector of uint8_t allows to store any length of data
    for length > 8 the data is sent by Transport-Protocol
   */
  class iGenericData_c
  {
  public:
    iGenericData_c() : vec_data() {}
    ~iGenericData_c() {}

    // using implicit copy/assgn c'tors for now

    /** storing data
      @param aui16_pos position to store data in vector
      @param aui8_data data byte
    */
    void setDataUi8( uint16_t aui16_pos, uint8_t aui8_data);

    /** storing data
      @param aui16_pos position to store data in vector
      @param ai8_data data byte
    */
    void setDataI8( uint16_t aui16_pos, int8_t ai8_data);

    /** storing data
      @param aui16_pos position to store data in vector
      @param aui16_data data bytes
    */
    void setDataUi16( uint16_t aui16_pos, uint16_t aui16_data);

    /** storing data
      @param aui16_pos position to store data in vector
      @param ai16_data data bytes
    */
    void setDataI16( uint16_t aui16_pos, int16_t ai16_data);

    /** storing data
      @param aui16_pos position to store data in vector
      @param aui32_data data bytes
    */
    void setDataUi32( uint16_t aui16_pos, uint32_t aui32_data);

    /** storing data
      @param aui16_pos position to store data in vector
      @param ai32_data data bytes
    */
    void setDataI32( uint16_t aui16_pos, int32_t ai32_data);

    /** deliver data from vector
      @param  aui16_pos position of byte in vector
      @return unsigned data byte at position aui16_pos
    */
    uint8_t getDataUi8( uint16_t aui16_pos) const;

    /** deliver data from vector
      @param  aui16_pos position of byte in vector
      @return signed data byte at position aui16_pos
    */
    int8_t getDataI8( uint16_t aui16_pos) const;

    /** deliver data from vector
      @param  aui16_pos position of byte in vector
      @return unsigned data bytes at position aui16_pos
    */
    uint16_t getDataUi16( uint16_t aui16_pos) const;

    /** deliver data from vector
      @param  aui16_pos position of byte in vector
      @return signed data bytes at position aui16_pos
    */
    int16_t getDataI16( uint16_t aui16_pos) const;

    /** deliver data from vector
      @param  aui16_pos position of byte in vector
      @return unsigned data bytes at position aui16_pos
    */
    uint32_t getDataUi32( uint16_t aui16_pos) const;

    /** deliver data from vector
      @param  aui16_pos position of byte in vector
      @return signed data bytes at position aui16_pos
    */
    int32_t getDataI32( uint16_t aui16_pos) const;

    /** return the length of data vector */
    uint16_t getLen() const;

    /** set data stream to vector vec_data
      @param aui16_pos position of byte in vector
      @param apui8_data pointer to data for read
      @param aui16_dataLength num of data bytes at position aui16_pos
    */
    void setDataStream(uint16_t aui16_pos, const uint8_t* apui8_data, uint16_t aui16_dataLength);

    /** set data stream to vector vec_data
      @param aui16_pos position of byte in vector
      @param arui8_data reference to data for read
      @param aui16_dataLength num of data bytes at position aui16_pos
    */
    void setDataStream(uint16_t aui16_pos, const uint8_t& arui8_data, uint16_t aui16_dataLength);

    /** deliver a pointer to the wanted data
      @param aui16_bytePos position of data
      @return uint8_t* pointer to data
    */
    const uint8_t* getDataStream(uint16_t aui16_bytePos=0) const;

    /** this method is clearing the vector */
    void clearVector() { vec_data.clear(); }

    /** this method is checking the size of the vector for writing data */
    void CheckSizeOfVectorForWrite(uint16_t aui16_pos, uint16_t aui16_size );

    /** this method is checking the size of the vector for reading data */
    bool CheckSizeOfVectorForRead(uint16_t aui16_pos, uint16_t aui16_size ) const;

    /** type definition of vector */
    typedef STL_NAMESPACE::vector<uint8_t> vec_data_t;

    /** type definition of an iterator to write the data */
    typedef STL_NAMESPACE::vector<uint8_t>::iterator vec_data_iterator_t;

    /** type definition of a constant iterator to read the data */
    typedef STL_NAMESPACE::vector<uint8_t>::const_iterator vec_data_const_iterator_t;

  private:
    /** vector to store the data */
    vec_data_t vec_data;
  };
};
#endif
