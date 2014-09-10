/*
  chunk_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "chunk_c.h"
#include "stream_c.h"


// Begin Namespace __IsoAgLib
namespace __IsoAgLib {




// Copy constructor
Chunk_c::Chunk_c( const Chunk_c& acrc_src )
{
  marr_data.reserve (acrc_src.marr_data.capacity());
  STL_NAMESPACE::copy(acrc_src.marr_data.begin(), acrc_src.marr_data.end(), marr_data.begin());
}



// //////////////////////////////// +X2C Operation : init
//! Reserves Memory for the Vector
//! basing on the Packet-burst-size times the Packet-size (7)
bool
Chunk_c::init()
{ // ~X2C
  marr_data.reserve( mscui16_chunkSize );
  return marr_data.capacity() == mscui16_chunkSize;
} // -X2C



// //////////////////////////////// +X2C Operation : insert
//! inserts (push-back) a byte into the vector
//! Parameter:
//! @param aui8_data: Byte to be inserted
//! @return Number of inserted bytes
uint8_t
Chunk_c::insert(uint8_t aui8_data)
{ // ~X2C
  if (full()) return 0;
  marr_data.push_back( aui8_data );
  return 1;
} // -X2C



// //////////////////////////////// +X2C Operation : insert7Bytes
//! inserts (push-back) 7 bytes into the vector if possible
//! Parameter:
//! @param pui8_data: Bytes to be inserted
//! @return Number of inserted bytes
uint8_t
Chunk_c::insert7Bytes(const uint8_t* pui8_data)
{ // ~X2C
  uint8_t  nbr=0;

  while (!full() && (nbr < 7))
    marr_data.push_back(pui8_data[nbr++]);
  return nbr;
} // -X2C



// //////////////////////////////// +X2C Operation : getData
//! Returns the spezified byte-value
//! Parameter:
//! @param aui32_pos: position (0..x)
//! @return Byte-value or 0xffff (out of range)
uint16_t
Chunk_c::getData(uint32_t aui32_pos)
{ // ~X2C
  if (aui32_pos >= marr_data.size()) return 0xffff;
  return marr_data[aui32_pos];
} // -X2C



// //////////////////////////////// +X2C Operation : setFree
//! Delets all objects of the vector
void
Chunk_c::setFree()
{ // ~X2C
  marr_data.clear();
} // -X2C



// //////////////////////////////// +X2C Operation : getFreeCnt
//! Returns the Number of objects
//! @return Number of objects
uint8_t
Chunk_c::getFreeCnt() const
{ // ~X2C
  return marr_data.capacity() - marr_data.size();
} // -X2C



// //////////////////////////////// +X2C Operation : free
//! Returns True if the Chunk is empty
//! @return True if the Chunk is empty
bool
Chunk_c::free()
{ // ~X2C
  return marr_data.empty();
} // -X2C



// //////////////////////////////// +X2C Operation : full
//! Returns True if the Chunk is full
//! @return True if the Chunk is full
bool
Chunk_c::full()
{ // ~X2C
  return marr_data.size() == marr_data.capacity();
} // -X2C

} // end namespace __IsoAgLib
