/*
  isoterminalstringmss_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTSTRING_C_H
#define VTOBJECTSTRING_C_H

#include <IsoAgLib/comm/Part3_DataLink/imultisendstreamer_c.h>
#include "vtobject_c.h"


namespace __IsoAgLib {

class MultiSendPkg_c;

/** helper class to avoid multiple inheritance by vtObjectString_c */
class vtObjectStringStreamer_c : public IsoAgLib::iMultiSendStreamer_c
{
 public:
  vtObjectStringStreamer_c()
    : iMultiSendStreamer_c()
    , mui16_vtObjectAId (0)
    , mpc_stringToStream (NULL)
    , mui16_strLenToSend (0)
    , mui32_streamPosition (0)
    //marr_uploadBuffer
    , mui32_streamPositionStored (0)
  {}
  
  void setDataNextStreamPart(MultiSendPkg_c* mspData,
                             uint8_t bytes);

  void resetDataNextStreamPart();
  void saveDataNextStreamPart();
  void restoreDataNextStreamPart();

  uint32_t getStreamSize();
  uint8_t getFirstByte () { return 179; /* Command: "Command" --- Parameter: "Change String Value"; */ }

  const char* getStringToStream() { return mpc_stringToStream; }

  void set5ByteCommandHeader(uint8_t* destinBuffer);

  void setStringToStream( const char* apc_stringToStream ) { mpc_stringToStream = apc_stringToStream;}
  void setStrLenToSend( uint16_t aui16_strLenToSend ) { mui16_strLenToSend = aui16_strLenToSend;}

  uint16_t getID() { return mui16_vtObjectAId; }
  void setID( uint16_t aui16_id ) { mui16_vtObjectAId = aui16_id; }

private:
  // ID from the connected __IsoAgLib::vtObject_c
  uint16_t mui16_vtObjectAId;
  // those both will be set before multisending is getting started.
  const char* mpc_stringToStream;
  uint16_t mui16_strLenToSend;

  uint32_t mui32_streamPosition;
  uint8_t marr_uploadBuffer [7+1]; /* "+1" for safety only */
  uint32_t mui32_streamPositionStored;
};


class vtObjectString_c : public vtObject_c
{
 public:
  vtObjectString_c();
  ~vtObjectString_c();
  vtObjectStringStreamer_c* getStreamer( void ) { return &mc_streamer;};
 protected:

  // those both will be set before multisending is getting started.
  void setStringToStream( const char* apc_stringToStream );
  void setStrLenToSend( uint16_t aui16_strLenToSend );

 private:
  vtObjectStringStreamer_c mc_streamer;
};

} // __IsoAgLib

#endif
