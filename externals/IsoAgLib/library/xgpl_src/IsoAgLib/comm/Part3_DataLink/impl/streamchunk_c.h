/*
  streamchunk_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef STREAMCHUNK_C_H
#define STREAMCHUNK_C_H


#include "stream_c.h"
#include "chunk_c.h"

// stl
#include <list>




// Begin Namespace __IsoAgLib
namespace __IsoAgLib {



//  +X2C Class 755 : StreamChunk_c
class StreamChunk_c : public Stream_c
{
public:
  StreamChunk_c (const ReceiveStreamIdentifier_c& ac_rsi, uint32_t aui32_msgSize, int32_t ai32_creationTime MULTITON_INST_PARAMETER_DEF_WITH_COMMA , bool b_skipCtsAwait);

  StreamChunk_c (const StreamChunk_c& acrc_src);

  const StreamChunk_c& operator=( const StreamChunk_c& acrc_src );

  //! Destructor: deletes the list
  virtual ~StreamChunk_c();


  //! Important!! Call this after Construction!
  void immediateInitAfterConstruction();

  //  Operation: insert
  //! Parameter:
  //! @param pui8_data:
  void insert7Bytes(const uint8_t* pui8_data);

  #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  void insertFirst6Bytes(const uint8_t* pui8_data);
  #endif

  //  Operation: getNextNotParsed
  //! Also moves one byte forward!
  uint8_t getNextNotParsed();

  //  Operation: getNotParsedSize
  uint32_t getNotParsedSize();

  //  Operation: getNotParsed
  uint8_t getNotParsed(uint32_t ui32_notParsedRelativeOffset);

  bool eof() const;

private: // function
  inline void copyIterator (const STL_NAMESPACE::list<Chunk_c>& rc_listSource, const STL_NAMESPACE::list<Chunk_c>::iterator& rc_iterSource, STL_NAMESPACE::list<Chunk_c>& rc_listDestin, STL_NAMESPACE::list<Chunk_c>::iterator& rc_iterDestin);


private:
  //  Attribute: mlist_chunks
  STL_NAMESPACE::list<Chunk_c> mlist_chunks;

  //  Attribute: mpc_iterWriteChunk
  STL_NAMESPACE::list<Chunk_c>::iterator mpc_iterWriteChunk;

  //  Attribute: mpc_iterParsedChunk
  STL_NAMESPACE::list<Chunk_c>::iterator mpc_iterParsedChunk;

  //  Attribute: mui32_writeCnt
  uint32_t mui32_writeCnt;

  //  Attribute: mui32_parsedCnt
  uint32_t mui32_parsedCnt;

}; // ~X2C



inline void
StreamChunk_c::copyIterator (const STL_NAMESPACE::list<Chunk_c>& rc_listSource, const STL_NAMESPACE::list<Chunk_c>::iterator& rc_iterSource, STL_NAMESPACE::list<Chunk_c>& rc_listDestin, STL_NAMESPACE::list<Chunk_c>::iterator& rc_iterDestin)
{
  STL_NAMESPACE::list<Chunk_c>::const_iterator pc_iterSource = rc_listSource.begin();
                                   rc_iterDestin = rc_listDestin.begin();
  while (pc_iterSource != rc_listSource.end())
  {
    if (&(*pc_iterSource) == &(*rc_iterSource))
    { // destinIter found.
      return;
    }
    pc_iterSource++;
    rc_iterDestin++;
  }
}

} // end namespace __IsoAgLib
#endif // -X2C
