/*
  diagnosticpgnhandler_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "diagnosticpgnhandler_c.h"

#include <IsoAgLib/comm/Part5_NetworkManagement/impl/identitem_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgn_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisend_c.h>
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/util/iassert.h>
#include <IsoAgLib/util/impl/util_funcs.h>
#include <stdlib.h>


namespace __IsoAgLib
{

DiagnosticPgnHandler_c::DiagnosticPgnHandler_c ( IdentItem_c& identItem ) :
    m_mrEventProxy(),
    mrc_identItem ( identItem ),
    m_diagnosticFunctionalities( identItem ),
    mcstr_ecuIdentification ( NULL ),
    mcstr_productIdentification( NULL ),
    mcstr_swIdentification ( NULL),
    mb_certificationIsSet ( false )
{
  // m_certification is not set as mb_certificationIsSet indicates it not being used!

  // protocol is currently hard-coded.
  m_protocol[ 0 ] = 0x00;
  for( uint8_t ui = 1 ; ui < 8; ++ui )
    m_protocol[ ui ] = 0xFF;
}


DiagnosticPgnHandler_c::~DiagnosticPgnHandler_c()
{
  if (mcstr_ecuIdentification)
    CNAMESPACE::free (mcstr_ecuIdentification);

  if (mcstr_productIdentification)
    CNAMESPACE::free (mcstr_productIdentification);

  if (mcstr_swIdentification)
    CNAMESPACE::free (mcstr_swIdentification);
}


void
DiagnosticPgnHandler_c::init()
{
  getIsoRequestPgnInstance( mrc_identItem.getMultitonInst() ).registerPGN ( *this, SOFTWARE_IDENTIFICATION_PGN );
  getIsoRequestPgnInstance( mrc_identItem.getMultitonInst() ).registerPGN ( *this, ECU_IDENTIFICATION_INFORMATION_PGN );
  getIsoRequestPgnInstance( mrc_identItem.getMultitonInst() ).registerPGN ( *this, ISOBUS_CERTIFICATION_PGN );
  getIsoRequestPgnInstance( mrc_identItem.getMultitonInst() ).registerPGN ( *this, ECU_DIAGNOSTIC_PROTOCOL_PGN );
  getIsoRequestPgnInstance( mrc_identItem.getMultitonInst() ).registerPGN ( *this, PRODUCT_IDENTIFICATION_PGN );

  m_diagnosticFunctionalities.init();
}


void
DiagnosticPgnHandler_c::close()
{
  m_diagnosticFunctionalities.close();

  getMultiSendInstance( mrc_identItem.getMultitonInst() ).abortSend( m_mrEventProxy );

  getIsoRequestPgnInstance( mrc_identItem.getMultitonInst() ).unregisterPGN ( *this, PRODUCT_IDENTIFICATION_PGN );
  getIsoRequestPgnInstance( mrc_identItem.getMultitonInst() ).unregisterPGN ( *this, ECU_DIAGNOSTIC_PROTOCOL_PGN );
  getIsoRequestPgnInstance( mrc_identItem.getMultitonInst() ).unregisterPGN ( *this, ISOBUS_CERTIFICATION_PGN );
  getIsoRequestPgnInstance( mrc_identItem.getMultitonInst() ).unregisterPGN ( *this, ECU_IDENTIFICATION_INFORMATION_PGN );
  getIsoRequestPgnInstance( mrc_identItem.getMultitonInst() ).unregisterPGN ( *this, SOFTWARE_IDENTIFICATION_PGN );
}


bool
DiagnosticPgnHandler_c::processMsgRequestPGN ( uint32_t pgn, IsoItem_c* isoItemSender, IsoItem_c* isoItemReceiver, int32_t )
{
  if ( !mrc_identItem.isClaimedAddress() )
    return false;

  // we're not Network Management, so don't answer requests from 0xFE
  if( isoItemSender == NULL )
    return false;

  if ( ( isoItemReceiver != NULL ) && ( mrc_identItem.getIsoItem() != isoItemReceiver ) )
    return false; // request not adressed to us!

  switch ( pgn )
  {
    case ISOBUS_CERTIFICATION_PGN:
      if (mb_certificationIsSet)
      {
        sendSinglePacket(m_certification,ISOBUS_CERTIFICATION_PGN);
        return true;
      }
      break;

    case SOFTWARE_IDENTIFICATION_PGN:
      if (mcstr_swIdentification != NULL)
      {
        if (getCStringLength (mcstr_swIdentification) < 9)
        {
          sendSinglePacket((uint8_t *) mcstr_swIdentification, SOFTWARE_IDENTIFICATION_PGN);
          return true;
        }
        else
        {
          if( isoItemReceiver != NULL )
          { // dest-spec. request -> answer TP
            if ( getMultiSendInstance( mrc_identItem.getMultitonInst() ).sendIsoTarget(
                  mrc_identItem.isoName(),
                  isoItemSender->isoName(),
                  (uint8_t *) mcstr_swIdentification,
                  getCStringLength (mcstr_swIdentification),
                  SOFTWARE_IDENTIFICATION_PGN,
                  &m_mrEventProxy) )
            { // Message successfully started with multisend
              return true;
            }
          }
          else
          { // broadcast request -> answer broadcast
            if ( getMultiSendInstance( mrc_identItem.getMultitonInst() ).sendIsoBroadcast(
                  mrc_identItem.isoName(),
                  (uint8_t *) mcstr_swIdentification,
                  uint16_t(getCStringLength (mcstr_swIdentification)),
                  SOFTWARE_IDENTIFICATION_PGN,
                  &m_mrEventProxy) )
            { // Message successfully started with multisend
              return true;
            }
          }
        }
      } // else
      break;

    case ECU_IDENTIFICATION_INFORMATION_PGN:
      if (mcstr_ecuIdentification != NULL)
      {
        if (getCStringLength (mcstr_ecuIdentification) < 9)
        {
          sendSinglePacket((uint8_t *) mcstr_ecuIdentification, ECU_IDENTIFICATION_INFORMATION_PGN);
          return true;
        }
        else
        {
          if( isoItemReceiver != NULL )
          { // dest-spec. request -> answer TP
            if( getMultiSendInstance( mrc_identItem.getMultitonInst() ).sendIsoTarget(
                  mrc_identItem.isoName(),
                  isoItemSender->isoName(),
                  (uint8_t *) mcstr_ecuIdentification,
                  getCStringLength (mcstr_ecuIdentification),
                  ECU_IDENTIFICATION_INFORMATION_PGN,
                  &m_mrEventProxy) )
            { // Message successfully started with multisend
              return true;
            }
          }
          else
          { // broadcast request -> answer broadcast
            if ( getMultiSendInstance( mrc_identItem.getMultitonInst() ).sendIsoBroadcast(
                  mrc_identItem.isoName(),
                  (uint8_t *) mcstr_ecuIdentification,
                  uint16_t(getCStringLength (mcstr_ecuIdentification)),
                  ECU_IDENTIFICATION_INFORMATION_PGN,
                  &m_mrEventProxy) )
            { // Message successfully started with multisend
              return true;
            }
          }
        }
      }
      break;

    case PRODUCT_IDENTIFICATION_PGN:
      if (mcstr_productIdentification != NULL)
      {
        if (getCStringLength (mcstr_productIdentification) < 9)
        {
          sendSinglePacket((uint8_t *) mcstr_productIdentification, PRODUCT_IDENTIFICATION_PGN);
          return true;
        }
        else
        {
          if( isoItemReceiver != NULL )
          { // dest-spec. request -> answer TP
            if( getMultiSendInstance( mrc_identItem.getMultitonInst() ).sendIsoTarget(
                  mrc_identItem.isoName(),
                  isoItemSender->isoName(),
                  (uint8_t *) mcstr_productIdentification,
                  getCStringLength (mcstr_productIdentification),
                  PRODUCT_IDENTIFICATION_PGN,
                  &m_mrEventProxy) )
            { // Message successfully started with multisend
              return true;
            }
          }
          else
          { // broadcast request -> answer broadcast
            if ( getMultiSendInstance( mrc_identItem.getMultitonInst() ).sendIsoBroadcast(
                  mrc_identItem.isoName(),
                  (uint8_t *) mcstr_productIdentification,
                  uint16_t(getCStringLength (mcstr_productIdentification)),
                  PRODUCT_IDENTIFICATION_PGN,
                  &m_mrEventProxy) )
            { // Message successfully started with multisend
              return true;
            }
          }
        }
      }
      break;

    case ECU_DIAGNOSTIC_PROTOCOL_PGN:
      sendSinglePacket(m_protocol, ECU_DIAGNOSTIC_PROTOCOL_PGN);
      return true;
 
    default:
      isoaglib_assert(!"Not registered for this PGN.");
      break;
  }

  // something wrong happend - answer with CannotRespondNow
  //  (couldn't multisend or Identification not yet ready)
  getIsoRequestPgnInstance( mrc_identItem.getMultitonInst() ).answerRequestPGNwithACK ( *mrc_identItem.getIsoItem(), 0x03 ); // CannotRespondNow ACKNOWLEDGE
  return true;
}


void
DiagnosticPgnHandler_c::MultiSendEventHandlerProxy_c::reactOnStateChange(const SendStream_c& sendStream)
{
  // Nothing to be done. The event-handler is
  // only needed as handle for abortSend() in close().
  ( void )sendStream;
}


/// Internal helper functions:
/// (currently only for diagnostics)
/// /////////////////////////

/// The given strings should not have more than 200 characters and
/// can't contain '*' characters as these are used as field delimiters
/// @return length of the string (0..x) if valid, <0 if not valid!
int getLengthIfValid (const char *text)
{
  isoaglib_assert (text);

  int len = 0;
  while (text[len] != 0x00)
  {
    // check current...
    if (text[len] == '*')
    { // string contains forbidden '*'
      return -1;
    }
    // advance...
    ++len;
    if (len == 200)
    { // string to long - not valid
      return -2;
    }
  }

  return len;
}


bool
DiagnosticPgnHandler_c::setEcuIdentification(
  const char *partNr,
  const char *serialNr,
  const char *location,
  const char *type,
  const char *manufacturerName,
  const char *hardwareVer)
{
  isoaglib_assert (partNr);
  isoaglib_assert (serialNr);
  isoaglib_assert (location);
  isoaglib_assert (type);
  isoaglib_assert (manufacturerName);
  isoaglib_assert (hardwareVer);

  // currently a once set identification can't be changed.
  // this is due to not having separate send-buffers!
  if (mcstr_ecuIdentification)
    return false;

  int len1 = getLengthIfValid (partNr);
  int len2 = getLengthIfValid (serialNr);
  int len3 = getLengthIfValid (location);
  int len4 = getLengthIfValid (type);
  int len5 = getLengthIfValid (manufacturerName);
  int len6 = getLengthIfValid (hardwareVer);

  if ( (len1 < 0) || (len2 < 0) || (len3 < 0) || (len4 < 0) || (len5 < 0) || (len6 < 0) )
    return false; // wrong sublength(s) or * in substring(s)

  // string separated by * and terminated by 0x00
  int newLen = len1 + 1 + len2 + 1 + len3 + 1 + len4 + 1 + len5 + 1 + len6 + 1 + 1;

  mcstr_ecuIdentification = (char *) CNAMESPACE::malloc (sizeof (char) * newLen);

  char *destPtr = mcstr_ecuIdentification;
  addCStringWithoutTermination (&destPtr, partNr);
  addCStringWithoutTermination (&destPtr, "*");
  addCStringWithoutTermination (&destPtr, serialNr);
  addCStringWithoutTermination (&destPtr, "*");
  addCStringWithoutTermination (&destPtr, location);
  addCStringWithoutTermination (&destPtr, "*");
  addCStringWithoutTermination (&destPtr, type);
  addCStringWithoutTermination (&destPtr, "*");
  addCStringWithoutTermination (&destPtr, manufacturerName);
  addCStringWithoutTermination (&destPtr, "*");
  addCStringWithoutTermination (&destPtr, hardwareVer);
  addCStringWithoutTermination (&destPtr, "*");
  *destPtr++ = 0x00;

  // detect buffer-overruns!
  isoaglib_assert (newLen == (destPtr - mcstr_ecuIdentification));

  return true;
}


bool
DiagnosticPgnHandler_c::setProductIdentification(
  const char *code,
  const char *brand,
  const char *model)
{
  isoaglib_assert (code);
  isoaglib_assert (brand);
  isoaglib_assert (model);

  // currently a once set identification can't be changed.
  // this is due to not having separate send-buffers!
  if (mcstr_productIdentification)
    return false;

  int len1 = getLengthIfValid (code);
  int len2 = getLengthIfValid (brand);
  int len3 = getLengthIfValid (model);

  if ( (len1 < 0) || (len2 < 0) || (len3 < 0) )
    return false; // wrong sublength(s) or * in substring(s)

  // string separated by * and terminated by 0x00
  int newLen = len1 + 1 + len2 + 1 + len3 + 1 + 1;

  mcstr_productIdentification = (char *) CNAMESPACE::malloc (sizeof (char) * newLen);

  char *destPtr = mcstr_productIdentification;
  addCStringWithoutTermination (&destPtr, code);
  addCStringWithoutTermination (&destPtr, "*");
  addCStringWithoutTermination (&destPtr, brand);
  addCStringWithoutTermination (&destPtr, "*");
  addCStringWithoutTermination (&destPtr, model);
  addCStringWithoutTermination (&destPtr, "*");
  *destPtr++ = 0x00;

  // detect buffer-overruns!
  isoaglib_assert (newLen == (destPtr - mcstr_productIdentification));

  return true;
}


bool
DiagnosticPgnHandler_c::setSwIdentification ( const char *swId )
{
  isoaglib_assert( swId );

  // currently a once set identification can't be changed.
  // this is due to not having separate send-buffers!
  if (mcstr_swIdentification)
    return false;

  int swIdLen = getCStringLength (swId);

  if ( (swIdLen > 200 ) || (swIdLen < 1 ) )
    return false;

  if (swId [swIdLen-1] != '*')
    return false; // last character must be '*'

  int numStars = getCStringCount (swId, '*');

  int newLen = 1+swIdLen+1; // // uint8,swid,0x00
  mcstr_swIdentification = (char *) CNAMESPACE::malloc (sizeof (char) * newLen);

  char *destPtr = mcstr_swIdentification;
  *destPtr++ = uint8_t (numStars); // number of fields
  addCStringWithoutTermination (&destPtr, swId);
  *destPtr++ = 0x00;

  // detect buffer-overruns!
  isoaglib_assert (newLen == (destPtr - mcstr_swIdentification));

  return true;
}


bool
DiagnosticPgnHandler_c::setCertificationData(
  uint16_t ui16_year,
  IsoAgLib::CertificationRevision_t a_revision,
  IsoAgLib::CertificationLabType_t a_laboratoryType,
  uint16_t aui16_laboratoryId,
  uint16_t aui16_referenceNumber)
{
  // currently a once set identification can't be changed.
  // this is to be consistent with SW/ECU-identification
  if (mb_certificationIsSet)
    return false;

  if ( ( ui16_year < 2000 ) || ( ui16_year > 2061 ) )
    return false;

  if ( aui16_laboratoryId > 2047 ) // Lab ID is only 11 bits wide
    return false;

  m_certification[0] = uint8_t( ( ( a_revision & 0x03 ) << 6 ) | ( ( ui16_year - 2000 ) & 0x3F ) );
  m_certification[1] = uint8_t( ( ( aui16_laboratoryId & 0x07 ) << 5 ) | ( ( a_laboratoryType & 0x03 ) << 1 ) | ( ( ( a_revision & 0x18 ) >> 3 ) << 3 ) | ( ( a_revision & 0x04 ) >> 2 ) ); // see AEF exception
  m_certification[2] = uint8_t( ( aui16_laboratoryId >> 3 ) & 0xFF );
  m_certification[3] = 0; // reserved for future use
  m_certification[4] = 0; // reserved for future use
  m_certification[5] = 0x80; // ISOBUS Compliance Certification message revision: Now "Post-3rd ed", see AEF exception
  m_certification[6] = ( aui16_referenceNumber & 0xFFu );
  m_certification[7] = uint8_t(( aui16_referenceNumber >> 8 ) & 0xFF);

  mb_certificationIsSet = true;
  return true;
}

void
DiagnosticPgnHandler_c::sendSinglePacket (const HUGE_MEM uint8_t* data, int32_t pgn)
{
  CanPkgExt_c pkg;
  pkg.setIsoPri( 6 );
  pkg.setMonitorItemForSA( mrc_identItem.getIsoItem() );
  pkg.setLen( 8 );
  pkg.setIsoPgn( pgn );
  for( uint8_t ui = 0 ; ui < 8; ++ui )
    pkg.setUint8Data( ui, data[ui] );

  getIsoBusInstance( mrc_identItem.getMultitonInst() ) << pkg;
}

DiagnosticFunctionalities_c &
DiagnosticPgnHandler_c::getDiagnosticFunctionalities()
{
  return m_diagnosticFunctionalities;
}

}
