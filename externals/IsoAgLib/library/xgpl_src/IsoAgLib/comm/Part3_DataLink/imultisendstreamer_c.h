/*
  imulti_send_streamer.h: transfer of multi message data with help of streamer

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef IMULTI_SEND_STREAMER_C_H
#define IMULTI_SEND_STREAMER_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/singleton.h>

/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */

namespace __IsoAgLib {
class MultiSendPkg_c;
}


namespace IsoAgLib {


/**
  This class implements the sending of streams that are no

  @author Dipl.-Inform. Achim Spangler
*/
class iMultiSendStreamer_c : public ClientBase
{
public:
  virtual ~iMultiSendStreamer_c(){};

  /** place next data to send direct into send buffer of pointed
      stream send package - MultiSend_c will send this
      buffer afterwards
    */
  virtual void setDataNextStreamPart (__IsoAgLib::MultiSendPkg_c* mspData, uint8_t bytes)=0;

  #if defined(ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
  /** place next data to send direct into send buffer of pointed
      stream send package - MultiSend_c will send this
      buffer afterwards
   */
  virtual void setDataNextFastPacketStreamPart (__IsoAgLib::MultiSendPkg_c* /*mspData*/, uint8_t /*bytes*/, uint8_t /*aui8_offset*/ = 0 ) {};
  #endif

  /** set cache for data source to stream start */
  virtual void resetDataNextStreamPart()=0;

  /** save current send position in data source - neeed for resend on send problem */
  virtual void saveDataNextStreamPart ()=0;

  /** reactivate previously stored data source position - used for resend on problem */
  virtual void restoreDataNextStreamPart ()=0;

  /** calculate the size of the data source */
  virtual uint32_t getStreamSize ()=0;

  /** get the first byte, which is the COMMAND-byte and should be given back CONST! */
  virtual uint8_t getFirstByte ()=0;
};

} // end namespace IsoAgLib
#endif
