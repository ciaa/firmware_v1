/*
  objectpoolstreamer_c.h: class for streaming all the
    objects of an object pool with a small buffer.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef OBJECTPOOLSTREAMER_H
#define OBJECTPOOLSTREAMER_H

#include <IsoAgLib/comm/Part3_DataLink/imultisendstreamer_c.h>


namespace IsoAgLib {
  class iVtObject_c;
}


namespace __IsoAgLib {


class UploadPoolState_c;


/** helper class for low level streaming.
  This function was excluded from VtClient_c,
  as some STL aware compilers don't support multiple inheritance
  (e.g. IAR). So this helper construction was defined.
*/
class ObjectPoolStreamer_c : public IsoAgLib::iMultiSendStreamer_c
{
public:
  ObjectPoolStreamer_c( UploadPoolState_c& uploadPoolState )
    : m_uploadPoolState( uploadPoolState )
  {}

  virtual ~ObjectPoolStreamer_c() {}

  virtual void setDataNextStreamPart (MultiSendPkg_c* mspData, uint8_t bytes);
  virtual void resetDataNextStreamPart();
  virtual void saveDataNextStreamPart();
  virtual void restoreDataNextStreamPart();
  virtual uint32_t getStreamSize() { return mui32_size; }
  virtual uint8_t getFirstByte() { return 0x11; }

  void setStreamSize(uint32_t aui32_size) { mui32_size = aui32_size; }

public:
  uint32_t mui32_objectStreamPosition;
  uint32_t mui32_objectStreamPositionStored;
  uint32_t mui32_size;
  IsoAgLib::iVtObject_c*HUGE_MEM* mpc_objectsToUpload; // @todo maybe this variable can be optimized away and mpc_iterObjects be directly used...

  /** pointers needed by MultiSendStreamer */
  IsoAgLib::iVtObject_c*HUGE_MEM* mpc_iterObjects;
  IsoAgLib::iVtObject_c*HUGE_MEM* mpc_iterObjectsStored;

  UploadPoolState_c& m_uploadPoolState;

#define ISO_VT_UPLOAD_BUFFER_SIZE 128
  uint8_t marr_uploadBuffer [ISO_VT_UPLOAD_BUFFER_SIZE];
  uint8_t m_uploadBufferFilled;
  uint8_t m_uploadBufferPosition;

  uint8_t marr_uploadBufferStored [ISO_VT_UPLOAD_BUFFER_SIZE];
  uint8_t m_uploadBufferFilledStored;
  uint8_t m_uploadBufferPositionStored;

private:
  /** not copyable : copy constructor is only declared, never defined */
  ObjectPoolStreamer_c(const ObjectPoolStreamer_c&);
  /** not copyable : copy operator is only declared, never defined */
  ObjectPoolStreamer_c& operator=(const ObjectPoolStreamer_c&); 
};


} //__IsoAgLib

#endif
