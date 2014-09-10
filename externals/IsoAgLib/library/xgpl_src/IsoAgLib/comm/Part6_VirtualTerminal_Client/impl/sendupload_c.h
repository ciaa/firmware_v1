/*
  sendupload_c.h: class for managing the queued upload sendings

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef SENDUPLOAD_H
#define SENDUPLOAD_H

#include <IsoAgLib/comm/Part3_DataLink/impl/multisend_c.h>
#include <vector>

namespace IsoAgLib {
  class iVtObject_c;
}


namespace __IsoAgLib {

class vtObjectString_c;


class SendUpload_c : public SendUploadBase_c
{
public:
  SendUpload_c()
    : SendUploadBase_c()
    , mssObjectString (NULL)
    , ppc_vtObjects (NULL)
    , ui16_numObjects (0)
  {}

  SendUpload_c (vtObjectString_c* apc_objectString)
    : SendUploadBase_c()
    , mssObjectString (NULL)
    , ppc_vtObjects (NULL)
    , ui16_numObjects (0)
  {
    set (apc_objectString);
  }

  SendUpload_c (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, IsoAgLib::iVtObject_c** rppc_vtObjects, uint16_t aui16_numObjects)
    : SendUploadBase_c( byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8)
    , mssObjectString(NULL)  /// Use BUFFER - NOT MultiSendStreamer!
    , ppc_vtObjects (rppc_vtObjects)
    , ui16_numObjects (aui16_numObjects)
  {}

  SendUpload_c (uint8_t* apui8_buffer, uint32_t bufferSize)
    : SendUploadBase_c (apui8_buffer, bufferSize)
    , mssObjectString(NULL)  /// Use BUFFER - NOT MultiSendStreamer!
    , ppc_vtObjects (NULL)
    , ui16_numObjects (0)
  {}

  void set (vtObjectString_c* apc_objectString);
  void set (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, uint8_t byte9);
  void set (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, IsoAgLib::iVtObject_c** rppc_vtObjects, uint16_t aui16_numObjects);
#ifdef USE_VT_UNICODE_SUPPORT
  void set (uint16_t aui16_objId, const char* apc_string, uint16_t overrideSendLength, bool utf16 = false);
#else
  void set (uint16_t aui16_objId, const char* apc_string, uint16_t overrideSendLength);
#endif
  void set (uint8_t* apui8_buffer, uint32_t bufferSize);

  SendUpload_c (const SendUpload_c& r_source)
    : SendUploadBase_c(r_source)
    , mssObjectString(r_source.mssObjectString)
    , ppc_vtObjects (r_source.ppc_vtObjects)
    , ui16_numObjects (r_source.ui16_numObjects)
  {}

  const SendUpload_c& operator= (const SendUpload_c& r_source)
  {
    SendUploadBase_c::operator=(r_source);
    mssObjectString = r_source.mssObjectString;
    ppc_vtObjects = r_source.ppc_vtObjects;
    ui16_numObjects = r_source.ui16_numObjects;
    return r_source;
  }

  /// Use either an MultiSendStreamer or a direct ui8-Buffer
  __IsoAgLib::vtObjectString_c* mssObjectString;

  IsoAgLib::iVtObject_c** ppc_vtObjects;
  uint16_t ui16_numObjects; // don't care for if "ppc_vtObjects==NULL"
};


} // __IsoAgLib

#endif
