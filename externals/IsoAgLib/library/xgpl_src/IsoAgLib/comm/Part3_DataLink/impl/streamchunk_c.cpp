/*
  streamchunk_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "streamchunk_c.h"
#include "chunk_c.h"
#include "stdio.h"



// Begin Namespace __IsoAgLib
namespace __IsoAgLib {



//! Constructor: initializes the list and local variables
//! create one \<list\> element including one Chunk,
//! init mpc_iterWriteChunk, mpc_iterParsedChunk, mui32_writeCnt, mui32_parsedCnt
StreamChunk_c::StreamChunk_c (const ReceiveStreamIdentifier_c& ac_rsi,
                              uint32_t aui32_msgSize,
                              int32_t ai32_creationTime
                              MULTITON_INST_PARAMETER_DEF_WITH_COMMA
                             ,bool ab_skipCtsAwait)
  : Stream_c (ac_rsi, aui32_msgSize, ai32_creationTime MULTITON_INST_PARAMETER_USE_WITH_COMMA , ab_skipCtsAwait)
{
  /// Do NOTHING more here
  /// User HAS TO call "immediateInitAfterConstruction" **immediately** AFTER Construction!
};


StreamChunk_c::~StreamChunk_c()
{
  mlist_chunks.clear();
}


void
StreamChunk_c::immediateInitAfterConstruction()
{
  mlist_chunks.push_back( Chunk_c() );
  mpc_iterWriteChunk  = mlist_chunks.begin();
  mpc_iterParsedChunk = mlist_chunks.begin();
  mui32_writeCnt  = 0;
  mui32_parsedCnt = 0;

  mpc_iterWriteChunk->init();
}


StreamChunk_c::StreamChunk_c( const StreamChunk_c& acrc_src )
  : Stream_c( acrc_src ),
    mlist_chunks( acrc_src.mlist_chunks ),
    mui32_writeCnt ( acrc_src.mui32_writeCnt ),
    mui32_parsedCnt( acrc_src.mui32_parsedCnt )
{
  copyIterator (acrc_src.mlist_chunks, acrc_src.mpc_iterWriteChunk,  mlist_chunks, mpc_iterWriteChunk);
  copyIterator (acrc_src.mlist_chunks, acrc_src.mpc_iterParsedChunk, mlist_chunks, mpc_iterParsedChunk);
}


const StreamChunk_c& StreamChunk_c::operator=( const StreamChunk_c& acrc_src )
{
  Stream_c::operator=( acrc_src );

  mlist_chunks = acrc_src.mlist_chunks;
  copyIterator (acrc_src.mlist_chunks, acrc_src.mpc_iterWriteChunk,  mlist_chunks, mpc_iterWriteChunk);
  copyIterator (acrc_src.mlist_chunks, acrc_src.mpc_iterParsedChunk, mlist_chunks, mpc_iterParsedChunk);
  mui32_writeCnt  = acrc_src.mui32_writeCnt;
  mui32_parsedCnt = acrc_src.mui32_parsedCnt;
  return acrc_src;
}


#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
//! use this function only for the first 6 bytes. this exception is for fast-packet protocol's FirstFrame
//! *** ATTENTION *** THIS FUNCTION ASSUMES TO BE CALLED ON A FRESH STREAM, SO NO OVERFLOW CHECKS ARE DONE!
void
StreamChunk_c::insertFirst6Bytes(const uint8_t* pui8_data)
{
  // write bytes into current chunk, assuming it's the first one so NO checks for full chunk are performed!!!!
  int nbr = 0;
  while (nbr < 6)
    if (mpc_iterWriteChunk->insert( pui8_data[nbr++] ))
      mui32_writeCnt++;
}
#endif


// //////////////////////////////// +X2C Operation 772 : insert
//! Insert 7 Bytes into one or two chunks
//! if the current chunk is full the next one is selcted respectively created
//! Parameter:
//! @param pui8_data: pointer to 7 bytes of data!
void
StreamChunk_c::insert7Bytes(const uint8_t* pui8_data)
{ // ~X2C
  uint16_t nbr;
  STL_NAMESPACE::list<Chunk_c>::iterator pc_iterTmpChunk = mpc_iterWriteChunk;

  // 1) write bytes into current writeChunk
  nbr = mpc_iterWriteChunk->insert7Bytes( pui8_data );
  mui32_writeCnt += nbr;

  // 2) if current writeChunk is full then check if the next one is free
  //    if the next is free then 'increment' the writeChunk
  //    otherwise insert a new Chunk after the current writeChunk
  if (mpc_iterWriteChunk->full())
  {
    ++pc_iterTmpChunk;
    if (pc_iterTmpChunk == mlist_chunks.end())
      pc_iterTmpChunk = mlist_chunks.begin();

    if (pc_iterTmpChunk->free())
    {
      mpc_iterWriteChunk = pc_iterTmpChunk;
    }
    else
    {
      ++mpc_iterWriteChunk;
      mpc_iterWriteChunk = mlist_chunks.insert( mpc_iterWriteChunk, Chunk_c() );
      mpc_iterWriteChunk->init();
    }
  }

  // 3) write remaining bytes into the next writeChunk if necessary
  while (nbr < 7)
    if (mpc_iterWriteChunk->insert( pui8_data[nbr++] ))
      mui32_writeCnt++;

} // -X2C



// //////////////////////////////// +X2C Operation 838 : getNotParsedSize
//! Checks the number of unparsed bytes
//! Parameter:
//! @return number of unparsed bytes
uint32_t
StreamChunk_c::getNotParsedSize()
{ // ~X2C
  // if the buffer is larger than our totalMsgSize, clip at totalMsgSize.
  // this occurs as we always insert 7 bytes, even if the last 7byte packet should NOT been taken completely
  if (mui32_byteTotalSize < mui32_writeCnt)
    return (mui32_byteTotalSize - mui32_parsedCnt);
  else
    return (mui32_writeCnt - mui32_parsedCnt);
} // -X2C



// //////////////////////////////// +X2C Operation 783 : getNextNotParsed
uint8_t
//! Returns the next byte to be parsed
//! if the parsed-counter reached the chunk-end then the next one is selcted
//! Parameter:
//! @return next byte to be parsed or 0xff (parse-counter equals write-counter)
StreamChunk_c::getNextNotParsed()
{ // ~X2C
  const uint16_t chunkLen = Chunk_c::mscui16_chunkSize;
  uint16_t chunkCnt = uint16_t(mui32_parsedCnt % chunkLen);
  uint8_t  chunkVal;

  if (mui32_parsedCnt >= mui32_writeCnt) return 0xff;

  // 1) get Chunk-Value
  // don't care for special 0xFFFF (meaning read-after-end-of-chunk), because we check the length for ourself before
  chunkVal = (uint8_t) mpc_iterParsedChunk->getData( chunkCnt++ );
  mui32_parsedCnt++;

  // 2) if current parsed-Chunk has been completely parsed
  //    then set free the current Chunk and 'increment' the parsed-Chunk
  if (chunkCnt >= chunkLen)
  {
    mpc_iterParsedChunk->setFree();
    ++mpc_iterParsedChunk;
    if (mpc_iterParsedChunk == mlist_chunks.end())
      mpc_iterParsedChunk = mlist_chunks.begin();
  }

  return chunkVal;
} // -X2C



// //////////////////////////////// +X2C Operation : getNotParsed
//! Returns a byte to be checked by the parser
//! The 'address' is specified by the current parsed-counter and the given offset
//! The parsed-counter is not modified
//! Parameter:
//! @param ui32_notParsedRelativeOffset: address-offset for the parsed-counter
//! @return byte to be parsed or 0xff (if the address is out of range)
uint8_t
StreamChunk_c::getNotParsed (uint32_t ui32_notParsedRelativeOffset)
{
  STL_NAMESPACE::list<Chunk_c>::iterator pc_iterTmpChunk = mpc_iterParsedChunk;
  const uint16_t chunkLen = Chunk_c::mscui16_chunkSize;
  uint16_t chunkCnt = uint16_t(mui32_parsedCnt % chunkLen);
  uint32_t chunkCntReq = chunkCnt + ui32_notParsedRelativeOffset;

  if ((mui32_parsedCnt + ui32_notParsedRelativeOffset) >= mui32_writeCnt) return 0xff;

  if (chunkCntReq >= chunkLen)
  {
    do
    {
      chunkCntReq -= chunkLen;
      ++pc_iterTmpChunk;
      if (pc_iterTmpChunk == mlist_chunks.end())
        pc_iterTmpChunk = mlist_chunks.begin();
    } while (chunkCntReq >= chunkLen);
  }

  // don't care for special 0xFFFF (meaning read-after-end-of-chunk), because we check the length for ourself before
  return (uint8_t) pc_iterTmpChunk->getData( chunkCntReq );
}



// //////////////////////////////// +X2C Operation 2878 : eof
//! Checks if the stream has been completely parsed
//! @return True: stream has been completely parsed
bool
StreamChunk_c::eof() const
{ // ~X2C
  if (mui32_parsedCnt >= mui32_byteTotalSize)
    return true;
  else
    return false;
} // -X2C


} // end namespace __IsoAgLib
