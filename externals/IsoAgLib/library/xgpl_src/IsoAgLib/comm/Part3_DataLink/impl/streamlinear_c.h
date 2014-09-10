/*
  streamlinear_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef STREAMLINEAR_C_H
#define STREAMLINEAR_C_H

#include "stream_c.h"

#include <cstdlib>	// Include before vector or else CNAMESPACE stuff is screwed up for Tasking
#include <vector>


namespace __IsoAgLib {


class StreamLinear_c : public Stream_c
{
public:
  StreamLinear_c (const ReceiveStreamIdentifier_c& ac_rsi, uint32_t aui32_msgSize, int32_t ai32_creationTime MULTITON_INST_PARAMETER_DEF_WITH_COMMA , bool b_skipCtsAwait);
  virtual ~StreamLinear_c();

  void immediateInitAfterConstruction() const {}

  void insert7Bytes(const uint8_t* pui8_data);
  #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  void insertFirst6Bytes(const uint8_t* pui8_data);
  #endif

  uint8_t getNextNotParsed();
  uint32_t getNotParsedSize();
  uint8_t getNotParsed(uint32_t ui32_notParsedRelativeOffset);
  bool eof() const;

  inline uint8_t* getLinearBuffer();

private:
  STL_NAMESPACE::vector<uint8_t> mvecui8_buffer;
  uint32_t mui32_parsedCnt;
};


//! ATTENTION: Only use this function if it is assured using getNotParsedSize that there's something in the buffer!
inline uint8_t
StreamLinear_c::getNextNotParsed()
{
  return mvecui8_buffer[mui32_parsedCnt++];
}


inline uint8_t*
StreamLinear_c::getLinearBuffer()
{
  return &mvecui8_buffer.front();
}


inline uint32_t
StreamLinear_c::getNotParsedSize()
{
  uint32_t ui32_bufSize = mvecui8_buffer.size();
  // if the buffer is larger than our totalMsgSize, clip at totalMsgSize.
  // this occurs as we always insert 7 bytes, even if the last 7byte packet should NOT been taken completely
  if (mui32_byteTotalSize < ui32_bufSize)
    return (mui32_byteTotalSize - mui32_parsedCnt);
  else
    return (ui32_bufSize - mui32_parsedCnt);
}


//! be sure to NOT read from an offset which overflows the unparsed buffer!
inline uint8_t
StreamLinear_c::getNotParsed (uint32_t ui32_notParsedRelativeOffset)
{
  return mvecui8_buffer[mui32_parsedCnt + ui32_notParsedRelativeOffset];
}


inline bool
StreamLinear_c::eof() const
{
  return (mui32_parsedCnt >= mui32_byteTotalSize);
}


} // __IsoAgLib

#endif
