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
#include "objectpoolstreamer_c.h"

#include <IsoAgLib/comm/Part3_DataLink/impl/multisendpkg_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/impl/vtclientconnection_c.h>
#include "vtobject_c.h"


namespace __IsoAgLib {


void
ObjectPoolStreamer_c::setDataNextStreamPart (MultiSendPkg_c* mspData, uint8_t bytes)
{
  while ((m_uploadBufferFilled-m_uploadBufferPosition) < bytes)
  {
    // copy down the rest of the buffer (we have no ring buffer here!)
    int j = m_uploadBufferFilled - m_uploadBufferPosition;
    for (int i=0; i<j; i++)
      marr_uploadBuffer [i] = marr_uploadBuffer [i+m_uploadBufferPosition];
    // adjust pointers
    m_uploadBufferPosition = 0;
    m_uploadBufferFilled = j;

    // stream some more bytes into internal ISO_VT_UPLOAD_BUFFER_SIZE byte buffer...
    while( true )
    {
      vtObject_c &object = *((vtObject_c*)(*mpc_iterObjects));
      
      const uint16_t bytes2Buffer = m_uploadPoolState.dontUpload( object )
        ? 0
        : object.stream( marr_uploadBuffer+m_uploadBufferFilled, ISO_VT_UPLOAD_BUFFER_SIZE-m_uploadBufferFilled, mui32_objectStreamPosition );

      if( bytes2Buffer == 0 )
      { // no data for this object, try next one!
        mpc_iterObjects++;
        mui32_objectStreamPosition = 0;
        continue;
      }
      else
      { // could stream bytes for this object.
        m_uploadBufferFilled += bytes2Buffer;
        mui32_objectStreamPosition += bytes2Buffer;
        break; // fine. let's get to the other objects..
      }
    }
  }
  mspData->setDataPart (marr_uploadBuffer, m_uploadBufferPosition, bytes);
  m_uploadBufferPosition += bytes;
}


void
ObjectPoolStreamer_c::resetDataNextStreamPart()
{
  mpc_iterObjects = mpc_objectsToUpload;
  mui32_objectStreamPosition = 0;
  m_uploadBufferPosition = 0;
  m_uploadBufferFilled = 1;
  marr_uploadBuffer [0] = 0x11; // Upload Object Pool!
}


void
ObjectPoolStreamer_c::saveDataNextStreamPart()
{
  mpc_iterObjectsStored = mpc_iterObjects;
  mui32_objectStreamPositionStored = mui32_objectStreamPosition;
  m_uploadBufferPositionStored = m_uploadBufferPosition;
  m_uploadBufferFilledStored = m_uploadBufferFilled;
  for (int i=0; i<ISO_VT_UPLOAD_BUFFER_SIZE; i++)
    marr_uploadBufferStored [i] = marr_uploadBuffer [i];
}


void
ObjectPoolStreamer_c::restoreDataNextStreamPart()
{
  mpc_iterObjects = mpc_iterObjectsStored;
  mui32_objectStreamPosition = mui32_objectStreamPositionStored;
  m_uploadBufferPosition = m_uploadBufferPositionStored;
  m_uploadBufferFilled = m_uploadBufferFilledStored;
  for (int i=0; i<ISO_VT_UPLOAD_BUFFER_SIZE; i++)
    marr_uploadBuffer [i] = marr_uploadBufferStored [i];
}


} // __IsoAgLib
