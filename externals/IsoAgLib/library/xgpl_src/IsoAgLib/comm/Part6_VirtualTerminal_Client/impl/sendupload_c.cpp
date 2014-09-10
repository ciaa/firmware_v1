/*
  sendupload_c.h: class for managing the queued upload sendings

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "sendupload_c.h"


namespace __IsoAgLib {


void
SendUpload_c::set (vtObjectString_c* apc_objectString)
{
  ppc_vtObjects = NULL;

  mssObjectString = apc_objectString;
}


void
SendUpload_c::set (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, uint8_t byte9)
{
  SendUploadBase_c::set(byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9);
  mssObjectString = NULL;
  ppc_vtObjects = NULL; /// Use BUFFER - NOT MultiSendStreamer!
  ui16_numObjects = 0;
}


void
SendUpload_c::set (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, IsoAgLib::iVtObject_c** rppc_vtObjects, uint16_t aui16_numObjects)
{
  SendUploadBase_c::set( byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8 );
  mssObjectString = NULL;  /// Use BUFFER - NOT MultiSendStreamer!
  ppc_vtObjects = rppc_vtObjects;
  ui16_numObjects = aui16_numObjects;
}

#ifdef STRIP_TRAILING_SPACES_FROM_STRING_UPLOADS
uint16_t
getStringLengthWithoutTrailingSpaces( const char* apc_string, uint16_t overrideSendLength, bool utf16 )
{
  uint16_t ret = (CNAMESPACE::strlen(apc_string) < overrideSendLength && !utf16) ? CNAMESPACE::strlen(apc_string) : overrideSendLength;

  if( !utf16 )
  {
    while( ( ret >= 1 ) && ( apc_string[ret-1] == ' ' ) )
      --ret;
  }
  else
  {
    while( ( ret >= 4 ) &&
       ( ( (uint8_t)apc_string[0] == (uint8_t)0xff && (uint8_t)apc_string[ret-1] == (uint8_t)0x00 && (uint8_t)apc_string[ret-2] == 0x20 )
      || ( (uint8_t)apc_string[0] == (uint8_t)0xfe && (uint8_t)apc_string[ret-1] == (uint8_t)0x20 && (uint8_t)apc_string[ret-2] == 0x00 ) ) )
      ret -= 2;
  }
  return ret;
}
#endif


void
#ifdef USE_VT_UNICODE_SUPPORT
SendUpload_c::set (uint16_t aui16_objId, const char* apc_string, uint16_t overrideSendLength, bool utf16)
{
#else
SendUpload_c::set (uint16_t aui16_objId, const char* apc_string, uint16_t overrideSendLength)
{
  const bool utf16 = false;
#endif

#ifdef STRIP_TRAILING_SPACES_FROM_STRING_UPLOADS
  uint16_t strLen = getStringLengthWithoutTrailingSpaces( apc_string, overrideSendLength, utf16 );
#else
  uint16_t strLen = (CNAMESPACE::strlen(apc_string) < overrideSendLength && !utf16) ? CNAMESPACE::strlen(apc_string) : overrideSendLength;
#endif

  /// Use BUFFER - NOT MultiSendStreamer!
  vec_uploadBuffer.clear();
  vec_uploadBuffer.reserve (((5+strLen) < 8) ? 8 : (5+strLen)); // DO NOT USED an UploadBuffer < 8 as ECU->VT ALWAYS has 8 BYTES!

  vec_uploadBuffer.push_back (179);
  vec_uploadBuffer.push_back (aui16_objId & 0xFF);
  vec_uploadBuffer.push_back (aui16_objId >> 8);
  vec_uploadBuffer.push_back (strLen & 0xFF);
  vec_uploadBuffer.push_back (strLen >> 8);
  int i=0;
  for (; i < strLen; i++) {
    vec_uploadBuffer.push_back (*apc_string);
    apc_string++;
  }
  for (; i < 3; i++) {
    // at least 3 bytes from the string have to be written, if not, fill with 0xFF, so the pkg-len is 8!
    vec_uploadBuffer.push_back (0xFF);
  }

  mssObjectString = NULL;  /// Use BUFFER - NOT MultiSendStreamer!
  ppc_vtObjects = NULL;
}


void
SendUpload_c::set (uint8_t* apui8_buffer, uint32_t bufferSize)
{
  SendUploadBase_c::set (apui8_buffer, bufferSize);
  mssObjectString = NULL;  /// Use BUFFER - NOT MultiSendStreamer!
  ppc_vtObjects = NULL;
}


} // __IsoAgLib
