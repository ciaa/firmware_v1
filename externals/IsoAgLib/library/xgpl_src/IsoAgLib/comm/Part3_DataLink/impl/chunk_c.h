/*
  chunk_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef CHUNK_C_H
#define CHUNK_C_H

#include <IsoAgLib/isoaglib_config.h>

#include <cstdlib>	// Include before vector or else CNAMESPACE stuff is screwed up for Tasking
#include <vector>



// Begin Namespace __IsoAgLib
namespace __IsoAgLib {


//  +X2C Class 756 : Chunk_c
class Chunk_c
{

public:

  //  Operation: Chunk_c
  Chunk_c() {}

  //  Operation: ~Chunk_c
  //! Destructor: Clears the Vector.
  ~Chunk_c() { marr_data.clear(); }


  // Copy constructor
  Chunk_c( const Chunk_c& acrc_src );

  //  Operation: init
  bool init();

  //  Operation: insert
  uint8_t insert(uint8_t aui8_data);

  //  Operation: insert7bytes
  uint8_t insert7Bytes(const uint8_t* pui8_data);

  //  Operation: getData
  uint16_t getData(uint32_t aui32_pos);

  //  Operation: setFree
  void setFree();

  //  Operation: getFreeCnt
  uint8_t getFreeCnt() const;

  //  Operation: free
  bool free();

  //  Operation: full
  bool full();

  //  Attribute: sui16_chunkSize
  static const uint16_t mscui16_chunkSize = (CONFIG_MULTI_RECEIVE_CHUNK_SIZE_IN_PACKETS)*7;

protected:

private:

  //  Attribute: marr_data
  STL_NAMESPACE::vector<uint8_t> marr_data;

}; // ~X2C

} // end namespace __IsoAgLib
#endif // -X2C
