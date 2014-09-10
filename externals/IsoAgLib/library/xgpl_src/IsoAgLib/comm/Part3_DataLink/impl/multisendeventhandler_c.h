/*
  multisendeventhandler.h: transfer of a stream of multi message data

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef MULTI_SEND_EVENT_HANDLER_H
#define MULTI_SEND_EVENT_HANDLER_H

#include <IsoAgLib/isoaglib_config.h>


namespace __IsoAgLib {

class SendStream_c;

class MultiSendEventHandler_c {
public:
  MultiSendEventHandler_c() {}
  virtual ~MultiSendEventHandler_c() {}

  /** call back function called when the TP is finished
    @param sendStream stream that has just finished (success or abort)
  */
  virtual void reactOnStateChange(const SendStream_c& sendStream) = 0;
};

} // __IsoAgLib
#endif
