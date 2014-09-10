/*
  streamlinear_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "streamlinear_c.h"

namespace __IsoAgLib {


StreamLinear_c::StreamLinear_c (
  const ReceiveStreamIdentifier_c& ac_rsi,
  uint32_t aui32_msgSize,
  int32_t ai32_creationTime
  MULTITON_INST_PARAMETER_DEF_WITH_COMMA ,
  bool ab_skipCtsAwait)
  : Stream_c (ac_rsi, aui32_msgSize, ai32_creationTime MULTITON_INST_PARAMETER_USE_WITH_COMMA , ab_skipCtsAwait)
  , mui32_parsedCnt (0)
{
  // reserve one byte less than the maximum insert-command inserts at once (7 byte). 
  mvecui8_buffer.reserve (aui32_msgSize+6); // as reactOnStreamStart told we have enough memory!
};

StreamLinear_c::~StreamLinear_c()
{
}


#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
void
StreamLinear_c::insertFirst6Bytes(const uint8_t* pui8_data)
{
  mvecui8_buffer.push_back (pui8_data[0]);
  mvecui8_buffer.push_back (pui8_data[1]);
  mvecui8_buffer.push_back (pui8_data[2]);
  mvecui8_buffer.push_back (pui8_data[3]);
  mvecui8_buffer.push_back (pui8_data[4]);
  mvecui8_buffer.push_back (pui8_data[5]);
}
#endif


void
StreamLinear_c::insert7Bytes(const uint8_t* pui8_data)
{
  mvecui8_buffer.push_back (pui8_data[0]);
  mvecui8_buffer.push_back (pui8_data[1]);
  mvecui8_buffer.push_back (pui8_data[2]);
  mvecui8_buffer.push_back (pui8_data[3]);
  mvecui8_buffer.push_back (pui8_data[4]);
  mvecui8_buffer.push_back (pui8_data[5]);
  mvecui8_buffer.push_back (pui8_data[6]);
  // as ui32_recCnt is mvecui8_buffer.size(), it is automatically increased!
}


} // __IsoAgLib
