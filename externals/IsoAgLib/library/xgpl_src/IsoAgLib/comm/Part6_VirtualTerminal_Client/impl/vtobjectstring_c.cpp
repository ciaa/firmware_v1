/*
  isoterminalstringmss_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "vtobjectstring_c.h"
#include "../ivtobjectstring_c.h"

#include <IsoAgLib/comm/Part3_DataLink/impl/multisendpkg_c.h>

namespace IsoAgLib {
  // implement here a normal constructor/destructor, as the compiler dislikes inlining of that simple
  // constructor/destructor direct in scope of iVtObjectString_c
  iVtObjectString_c::iVtObjectString_c() : vtObjectString_c() {}
  iVtObjectString_c::~iVtObjectString_c(){}
}

namespace __IsoAgLib {

// Operation : setDataNextStreamPart
//! @param mspData:
//! @param bytes:
void
vtObjectStringStreamer_c::setDataNextStreamPart(__IsoAgLib::MultiSendPkg_c* mspData, uint8_t bytes)
{
  uint8_t bytesDone=0;
  if (mui32_streamPosition == 0) {
    set5ByteCommandHeader(marr_uploadBuffer);
    bytesDone = 5;

    mui32_streamPosition += 5;
  }

  while (bytesDone < bytes) {
    marr_uploadBuffer [bytesDone] = mpc_stringToStream [mui32_streamPosition-5];
    bytesDone++;
    mui32_streamPosition++;
  }

  mspData->setDataPart (marr_uploadBuffer, 0, bytes);
}


void
vtObjectStringStreamer_c::set5ByteCommandHeader(uint8_t* destinBuffer)
{
  destinBuffer [0] = getFirstByte(); // 179 == Command: "Command" --- Parameter: "Change String Value"
  destinBuffer [1] = getID () & 0xFF;
  destinBuffer [2] = getID () >> 8;
  destinBuffer [3] = mui16_strLenToSend & 0xFF;
  destinBuffer [4] = mui16_strLenToSend >> 8;
}

// Operation : resetDataNextStreamPart
void
vtObjectStringStreamer_c::resetDataNextStreamPart()
{
  mui32_streamPosition = 0;
}


// Operation : saveDataNextStreamPart
void
vtObjectStringStreamer_c::saveDataNextStreamPart()
{
  mui32_streamPositionStored = mui32_streamPosition;
}


// Operation : restoreDataNextStreamPart
void
vtObjectStringStreamer_c::restoreDataNextStreamPart()
{
  mui32_streamPosition = mui32_streamPositionStored;
}

// Operation : getStreamSize
uint32_t
vtObjectStringStreamer_c::getStreamSize()
{
  return 5+mui16_strLenToSend;
}


vtObjectString_c::vtObjectString_c()
{}

vtObjectString_c::~vtObjectString_c()
{}

void vtObjectString_c::setStringToStream( const char* apc_stringToStream )
{
  mc_streamer.setStringToStream( apc_stringToStream );
  // additionally set the ID
  mc_streamer.setID( getID() );
}

void vtObjectString_c::setStrLenToSend( uint16_t aui16_strLenToSend )
{
  mc_streamer.setStrLenToSend( aui16_strLenToSend );
}

} // end namespace __IsoAgLib
