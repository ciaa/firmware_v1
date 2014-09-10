/*
  diagnosticsservices_c.cpp: manage Diagnostics Services DM1/DM2/DM3

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "diagnosticsservices_c.h"

#include "../idiagnosticsservices_c.h"

// IsoAgLib
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/identitem_c.h>
#include <IsoAgLib/scheduler/impl/scheduler_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisend_c.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgn_c.h>

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif

namespace __IsoAgLib {

static const int32_t sci32_periodDM1 = 1000;
static const int32_t sci32_periodDM1noDTC = 0x7FFF; // arbitrary never timeevent call
static const int32_t sci32_periodDM1waitForTP = 0x7FFF; // arbitrary never timeevent call

DiagnosticsServices_c::DiagnosticsServices_c( IdentItem_c& arc_identItem ) :
  SchedulerTask_c( 100, true ),
  mrc_identItem ( arc_identItem ),
  mpc_serviceToolVerifier(NULL),
  mc_dtcs(),
  mi32_dm1LastSentTime(-1),
  mb_dm1CurrentNeedsToBeSent(false),
  marr_dm1CurrentSize(0),
  ms_dm1SendingBroadcast(marr_dm1SendingBroadcast),
  ms_dm1SendingDestination(marr_dm1SendingDestination),
  ms_dm2SendingDestination(marr_dm2SendingDestination),
  mt_isoRequestPgnHandler(*this),
  mt_multiSendEventHandler(*this)
{
  // initialize buffers -> not for release since no risk if not initialized
  //std::memset(marr_dm1Current,0,2+4*(CONFIG_MAX_ACTIVE_DTCS));
  //std::memset(marr_dm1SendingBroadcast,0,2+4*(CONFIG_MAX_ACTIVE_DTCS));
  //std::memset(marr_dm1SendingDestination,0,2+4*(CONFIG_MAX_ACTIVE_DTCS));
  //std::memset(marr_dm2SendingDestination,0,2+4*(CONFIG_MAX_PREVIOUSLY_ACTIVE_DTCS));

  // prepare initial DM1 and DM2
  marr_dm1CurrentSize                       = assembleDM1DM2(marr_dm1Current,true, &m_dm1CurrentAtLeastOneDTC);
  ms_dm2SendingDestination.marr_bufferSize  = assembleDM1DM2(ms_dm2SendingDestination.marr_buffer,false, NULL); // not required but nice to be prepared
}

DiagnosticsServices_c::~DiagnosticsServices_c()
{
}

int DiagnosticsServices_c::getMultitonInst() const
{
  return mrc_identItem.getMultitonInst();
}

void
DiagnosticsServices_c::init()
{
  getSchedulerInstance().registerTask( *this, 0 );

  getIsoRequestPgnInstance4Comm().registerPGN ( mt_isoRequestPgnHandler, ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN );
  getIsoRequestPgnInstance4Comm().registerPGN ( mt_isoRequestPgnHandler, PREVIOUSLY_ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN );
  getIsoRequestPgnInstance4Comm().registerPGN ( mt_isoRequestPgnHandler, DIAGNOSTIC_DATA_CLEAR_PGN );
}

void
DiagnosticsServices_c::close()
{
  getMultiSendInstance4Comm().abortSend(mt_multiSendEventHandler);

  getIsoRequestPgnInstance4Comm().unregisterPGN ( mt_isoRequestPgnHandler, DIAGNOSTIC_DATA_CLEAR_PGN );
  getIsoRequestPgnInstance4Comm().unregisterPGN ( mt_isoRequestPgnHandler, PREVIOUSLY_ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN );
  getIsoRequestPgnInstance4Comm().unregisterPGN ( mt_isoRequestPgnHandler, ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN );

  getSchedulerInstance().deregisterTask( *this );
}

void DiagnosticsServices_c::timeEvent()
{
  if ( !mrc_identItem.isClaimedAddress() )
  {
    setPeriod( 50, false ); // check every 50 ms if we can operate again
    return;
  }

  if (!mb_dm1CurrentNeedsToBeSent)
  { // check if we have to send the DM1 out NOW?
    // simple, unoptimzed but secure version
    const uint32_t nextActionTime = calculateNextActionTime();
    if ((0 == nextActionTime) && m_dm1CurrentAtLeastOneDTC)
    {
      mb_dm1CurrentNeedsToBeSent = true;
    }
    else
    { // no need for any action, simply retrigger when needed
      setPeriod( nextActionTime, false );
    }
  }

  // Is a DM1 to be sent out now definitely?
  if (mb_dm1CurrentNeedsToBeSent)
  {
    // check that currently dm1Sending is not being sent right now.
    if (ms_dm1SendingBroadcast.mb_bufferUsedForTP)
    {
      setPeriod( 10, false ); // retrigger in 10ms to check if it could be sent then.
      return;
    }

    // Copy "dm1Current" -> "dm1Sending" ?
    if (!ms_dm1SendingBroadcast.mb_bufferIsValid)
    {
      CNAMESPACE::memcpy(marr_dm1SendingBroadcast,marr_dm1Current,marr_dm1CurrentSize);
      ms_dm1SendingBroadcast.marr_bufferSize = marr_dm1CurrentSize;
      ms_dm1SendingBroadcast.mb_bufferIsValid = true;
    }

    if (ms_dm1SendingBroadcast.marr_bufferSize < 9)
    { // Send out Single Packet
      isoaglib_assert(ms_dm1SendingBroadcast.marr_bufferSize == 8);
      mi32_dm1LastSentTime = HAL::getTime();
      sendSingleDM1DM2(ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN,marr_dm1SendingBroadcast);
 
      setPeriod ( m_dm1CurrentAtLeastOneDTC ? sci32_periodDM1 : sci32_periodDM1noDTC, false );

      mb_dm1CurrentNeedsToBeSent = false;

#if DEBUG_DIAGNOSTICPGN
      INTERNAL_DEBUG_DEVICE << "Send Periodic DM1 single frame " << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
    }
    else
    { // Send out MultiPacket Broadcast
      if (getMultiSendInstance4Comm().sendIsoBroadcast(
            mrc_identItem.isoName(),
            (uint8_t *) marr_dm1SendingBroadcast,
            uint16_t(ms_dm1SendingBroadcast.marr_bufferSize),
            ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN,
            &mt_multiSendEventHandler ) )
      { // Message successfully transmitted to multisend
#if DEBUG_DIAGNOSTICPGN
        INTERNAL_DEBUG_DEVICE << "Send Periodic DM1 multi frame" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
        // indicate that TP (BAM) is sending now.
        ms_dm1SendingBroadcast.mb_bufferUsedForTP = true;

        setPeriod( sci32_periodDM1waitForTP, false ); // wait for multisend to call back reactOnFinished()
        
        mi32_dm1LastSentTime = HAL::getTime();
        mb_dm1CurrentNeedsToBeSent = false;
      }
      else
      {
        setPeriod( 10, false ); // retrigger in 10ms to check if it could be sent then.
      }
    }
  }

  return;
}

void DiagnosticsServices_c::reactOnStateChange(const SendStream_c& sendStream)
{
  switch (sendStream.getSendSuccess())
  {
  case SendStream_c::Running:
    return;
  case SendStream_c::SendAborted:
  case SendStream_c::SendSuccess:
    break;
  }

  switch (sendStream.pgn())
  {
  case ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN:
    if (sendStream.receiver().isSpecified())
    {
#if DEBUG_DIAGNOSTICPGN
      INTERNAL_DEBUG_DEVICE << "DiagnosticsServices_c::reactOnFinished destination ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
      isoaglib_assert (ms_dm1SendingDestination.mb_bufferUsedForTP);
      ms_dm1SendingDestination.mb_bufferUsedForTP = false;
    }
    else
    {
#if DEBUG_DIAGNOSTICPGN
      INTERNAL_DEBUG_DEVICE << "DiagnosticsServices_c::reactOnFinished broadcast ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
      isoaglib_assert (ms_dm1SendingBroadcast.mb_bufferUsedForTP);
      ms_dm1SendingBroadcast.mb_bufferUsedForTP = false;

      // immediately re-check everything in timeEvent
      setNextTriggerTime( HAL::getTime() );
    }
    break;
  case PREVIOUSLY_ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN:
#if DEBUG_DIAGNOSTICPGN
    INTERNAL_DEBUG_DEVICE << "DiagnosticsServices_c::reactOnFinished PREVIOUSLY_ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
    isoaglib_assert (ms_dm2SendingDestination.mb_bufferUsedForTP);
    ms_dm2SendingDestination.mb_bufferUsedForTP = false;
    break;
  default:
    isoaglib_assert (false);
    break;
  }
}

uint32_t
DiagnosticsServices_c::calculateNextActionTime()
{
  if (mb_dm1CurrentNeedsToBeSent)
  { // immediate need of DM1 sending
    return 0;
  }
  else
  { // calculate the minimum
    if (!m_dm1CurrentAtLeastOneDTC)
    { // no DTC -> return with max period
      return sci32_periodDM1noDTC;
    }

    if(mi32_dm1LastSentTime < 0)
    { // immediate need of initial DM1 sending
      return 0;
    }

    // global 1s periodic sending
    int32_t i32_minNextAction = (mi32_dm1LastSentTime + sci32_periodDM1) - HAL::getTime();
  
    // and the need to send changes from the DTCs
    for (uint16_t counter = 0;counter < DtcContainer_c::scui16_sizeDTCList ;++counter) // loop_all_DTCs
    {
      if (mc_dtcs[counter].ui32_spn == DtcContainer_c::Dtc_s::spiNone)   
        continue;

      const int32_t ci32_minNextDtcAction = (mc_dtcs[counter].i32_timeLastStateChangeSent + sci32_periodDM1)
                                              - mc_dtcs[counter].i32_timeLastStateChange;

      if (ci32_minNextDtcAction < i32_minNextAction)
        i32_minNextAction = ci32_minNextDtcAction;
    }
    if (i32_minNextAction < 0)
      i32_minNextAction = 0;

    return static_cast<uint32_t>(i32_minNextAction);
  }
}

void
DiagnosticsServices_c::changeActiveDtcStatusAndRetrigger(DtcContainer_c::Dtc_s& arc_dtcToChange, bool a_active)
{
  arc_dtcToChange.b_active = a_active;
  arc_dtcToChange.i32_timeLastStateChange = HAL::getTime();

  if (arc_dtcToChange.i32_timeLastStateChange >= (arc_dtcToChange.i32_timeLastStateChangeSent + sci32_periodDM1) )
  { // Last State Change Sent is more than 1s,
    // need to send this change out now!
    mb_dm1CurrentNeedsToBeSent = true;
    arc_dtcToChange.i32_timeLastStateChangeSent = arc_dtcToChange.i32_timeLastStateChange;

    // immediately re-check everything in timeEvent
    setNextTriggerTime( HAL::getTime() );
  }
  else
  { // Last State Change Sent is less than 1s,
    // need to see when the next needed send is.
    const uint32_t retriggerDelay = calculateNextActionTime();

    setNextTriggerTime( HAL::getTime() + retriggerDelay );
  }
}

uint16_t DiagnosticsServices_c::dtcActivate(uint32_t SPN, IsoAgLib::FailureModeIndicator_en FMI)
{
  uint16_t dtcId = mc_dtcs.getDTCIndex(SPN,FMI);

  if (dtcId < DtcContainer_c::scui16_sizeDTCList)
  {
    if (!mc_dtcs[dtcId].b_active)
    { // reactivate
      // check if CONFIG_MAX_ACTIVE_DTCS Dtc are already active
      if (mc_dtcs.getNumberOfDtc(true) == CONFIG_MAX_ACTIVE_DTCS)
        return 0;

      changeActiveDtcStatusAndRetrigger(mc_dtcs[dtcId],true);

      // increase OccurrenceCount
      if (mc_dtcs[dtcId].ui16_occurrenceCount < 0xFFFF)
        ++mc_dtcs[dtcId].ui16_occurrenceCount;
    }
    else // already active
    {
      return mc_dtcs[dtcId].ui16_occurrenceCount;
    }
  }
  else // no DTC found -> create a new one
  {
    // check if CONFIG_MAX_ACTIVE_DTCS Dtc are already active
    if (mc_dtcs.getNumberOfDtc(true) == CONFIG_MAX_ACTIVE_DTCS)
      return 0;
    
    // get next free slot
    dtcId = mc_dtcs.getFreeDTCIndex();

    isoaglib_assert(DtcContainer_c::scui16_sizeDTCList != dtcId);

    // insert DTC
    const DtcContainer_c::Dtc_s dtc(SPN,FMI); // explizit call of = operator case of weird issue with HighTec TC 1796 gcc v.3.4.6
    mc_dtcs[dtcId] = dtc;

    // request send out immediately
    mb_dm1CurrentNeedsToBeSent = true;

    // immediately re-check everything in timeEvent
    setNextTriggerTime( HAL::getTime() );
  }

  // update send buffer -> "marr_dm1Current"
  marr_dm1CurrentSize = assembleDM1DM2(marr_dm1Current,true, &m_dm1CurrentAtLeastOneDTC);

  ms_dm1SendingBroadcast.mb_bufferIsValid = false;
  ms_dm1SendingDestination.mb_bufferIsValid = false;
  ms_dm2SendingDestination.mb_bufferIsValid = false;

  return mc_dtcs[dtcId].ui16_occurrenceCount;
}

uint16_t DiagnosticsServices_c::dtcDeactivate(uint32_t SPN, IsoAgLib::FailureModeIndicator_en FMI)
{
  uint16_t dtcId = mc_dtcs.getDTCIndex(SPN,FMI);

  if (dtcId < DtcContainer_c::scui16_sizeDTCList)
  {
    if (mc_dtcs[dtcId].b_active)
    { // deactivate
      // check if CONFIG_MAX_PREVIOUSLY_ACTIVE_DTCS Dtc are already active
      if (mc_dtcs.getNumberOfDtc(false) == CONFIG_MAX_PREVIOUSLY_ACTIVE_DTCS)
        return 0;

      changeActiveDtcStatusAndRetrigger(mc_dtcs[dtcId],false);

      // update send buffer -> "marr_dm1Current"
      marr_dm1CurrentSize = assembleDM1DM2(marr_dm1Current,true, &m_dm1CurrentAtLeastOneDTC);

      ms_dm1SendingBroadcast.mb_bufferIsValid = false;
      ms_dm1SendingDestination.mb_bufferIsValid = false;
      ms_dm2SendingDestination.mb_bufferIsValid = false;

      // no DTC anymore -> final DM1 message with no content
      if (!m_dm1CurrentAtLeastOneDTC)
        mb_dm1CurrentNeedsToBeSent = true;

      return mc_dtcs[dtcId].ui16_occurrenceCount;
    }
    else // already inactive
    {
      return mc_dtcs[dtcId].ui16_occurrenceCount;
    }
  }
  else // DTC was not registered
  { 
    return 0;
  }
}

/** assembleDm1Current()
 * This function will build the raw-data that are to be sent in the DM1 message
 * out of the "marr_dtc" structure-array.
 *
 * Only takes the valid "b_active" ones out of the array (SPI != spiNone)
 *
 * No need for further design, that's plain implementation.
 */
uint16_t DiagnosticsServices_c::assembleDM1DM2(uint8_t* arr_send8bytes, bool ab_searchForActiveDtc, bool* atleastoneDTC)
{
  // control if more than CONFIG_MAX_ACTIVE_DTCS element are active. Should not happened here, tested earlier in application
  isoaglib_assert(mc_dtcs.getNumberOfDtc(ab_searchForActiveDtc) <= (ab_searchForActiveDtc?CONFIG_MAX_ACTIVE_DTCS:CONFIG_MAX_PREVIOUSLY_ACTIVE_DTCS));

  // Byte 1 and 2 are Unspecified
  arr_send8bytes[0] = 0xFF;
  arr_send8bytes[1] = 0xFF;

  uint16_t temp_size = 2;

  for (uint16_t counter = 0;counter < DtcContainer_c::scui16_sizeDTCList ;++counter) // loop_all_DTCs
  {
    const DtcContainer_c::Dtc_s& c_dtc = mc_dtcs[counter]; 
    if ( (c_dtc.ui32_spn != DtcContainer_c::Dtc_s::spiNone)
      && (c_dtc.b_active == ab_searchForActiveDtc) )
    {
      arr_send8bytes[temp_size++] = static_cast<uint8_t>(c_dtc.ui32_spn);
      arr_send8bytes[temp_size++] = static_cast<uint8_t>(c_dtc.ui32_spn >> 8);
      arr_send8bytes[temp_size++] = static_cast<uint8_t>(((c_dtc.ui32_spn >> 11) & 0xE0) // 3 MSB in bits 8-6
                                                         | (c_dtc.en_fmi));
      arr_send8bytes[temp_size++] = static_cast<uint8_t>((c_dtc.ui16_occurrenceCount < 0x7F)?c_dtc.ui16_occurrenceCount:0x7F);
    }
  }

  bool noDTC = false;
  if (2 == temp_size)
  {
    arr_send8bytes[temp_size++] = 0;
    arr_send8bytes[temp_size++] = 0;
    arr_send8bytes[temp_size++] = 0;
    arr_send8bytes[temp_size++] = 0;
    noDTC = true;
  }
  if (6 == temp_size)
  {
    arr_send8bytes[temp_size++] = 0xFF;
    arr_send8bytes[temp_size++] = 0xFF;
  }

  if (atleastoneDTC)
    *atleastoneDTC = !noDTC;

  return temp_size;
}

void
DiagnosticsServices_c::serviceTool_dtcClearPrevious()
{
  mc_dtcs.dtcClearPrevious();
  ms_dm2SendingDestination.mb_bufferIsValid = false;
}

// helper method for sending DM1 single package
void
DiagnosticsServices_c::sendSingleDM1DM2(uint32_t ui32_pgn, uint8_t* arr_send8bytes)
{
  IsoBus_c& c_isobus = getIsoBusInstance4Comm();
  CanPkgExt_c c_pkg;

  // now nr() has now suitable value
  c_pkg.setIsoPri(6);
  c_pkg.setIsoPgn(ui32_pgn);
  c_pkg.setMonitorItemForSA( mrc_identItem.getIsoItem() );

  c_pkg.setDataFromString(arr_send8bytes, 8);

  c_isobus << c_pkg;
}

bool
DiagnosticsServices_c::processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* isoItemSender, IsoItem_c* isoItemReceiver, int32_t )
{
  if ( !mrc_identItem.isClaimedAddress() )
    return false;

  // we're not Network Management, so don't answer requests from 0xFE
  if( isoItemSender == NULL )
    return false;

  if ( ( isoItemReceiver != NULL ) && ( mrc_identItem.getIsoItem() != isoItemReceiver ) )
    return false; // request not adressed to us!

  switch (aui32_pgn)
  {
    case ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN:
#if DEBUG_DIAGNOSTICPGN
      INTERNAL_DEBUG_DEVICE << "processMsgRequestPGN ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
      if (!ms_dm1SendingDestination.mb_bufferUsedForTP)
      {
        // Copy "dm1Current" -> "dm1SendingDestination"
        if (!ms_dm1SendingDestination.mb_bufferIsValid)
        {
          CNAMESPACE::memcpy(marr_dm1SendingDestination,marr_dm1Current,marr_dm1CurrentSize);
          ms_dm1SendingDestination.marr_bufferSize = marr_dm1CurrentSize;

          ms_dm1SendingDestination.mb_bufferIsValid = true;
        }
        if (ms_dm1SendingDestination.marr_bufferSize < 9)
        { // Send out Single Packet
    #if DEBUG_DIAGNOSTICPGN
          INTERNAL_DEBUG_DEVICE << "Send response DM1 single frame" << INTERNAL_DEBUG_DEVICE_ENDL;
    #endif
          isoaglib_assert(ms_dm1SendingDestination.marr_bufferSize == 8);
          sendSingleDM1DM2(ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN,marr_dm1SendingDestination);

          return true;
        }
        else
        { // Send out MultiPacket Destination specific
          if (getMultiSendInstance4Comm().sendIsoTarget (
                mrc_identItem.isoName(),
                isoItemSender->isoName(),
                (uint8_t *) marr_dm1SendingDestination,
                ms_dm1SendingDestination.marr_bufferSize,
                ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN,
                &mt_multiSendEventHandler ) )
          { // Message successfully transmitted to multisend
            // indicate that TP (BAM) is sending now.
            ms_dm1SendingDestination.mb_bufferUsedForTP = true;
    #if DEBUG_DIAGNOSTICPGN
            INTERNAL_DEBUG_DEVICE << "Send response destination specific DM1 multi frame" << INTERNAL_DEBUG_DEVICE_ENDL;
    #endif
            return true;
          }
          // else cannot respond because MultiSend busy
        }
      }
      // else cannot respond because we are already sending right now
      getIsoRequestPgnInstance4Comm().answerRequestPGNwithACK ( *mrc_identItem.getIsoItem(), 0x03 ); // CannotRespondNow ACKNOWLEDGE
      return true;

    case PREVIOUSLY_ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN:
      if (!ms_dm2SendingDestination.mb_bufferUsedForTP)
      {
  #if DEBUG_DIAGNOSTICPGN
        INTERNAL_DEBUG_DEVICE << "processMsgRequestPGN PREVIOUSLY_ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN" << INTERNAL_DEBUG_DEVICE_ENDL;
  #endif
        //* - Build buffer if necessary
        if (!ms_dm2SendingDestination.mb_bufferIsValid)
        {
          ms_dm2SendingDestination.marr_bufferSize = assembleDM1DM2(marr_dm2SendingDestination,false, NULL);
          ms_dm2SendingDestination.mb_bufferIsValid = true;
        }

        //* - TP-Send out the request or Broadcast SinglePacket
        if (ms_dm2SendingDestination.marr_bufferSize < 9)
        {
          isoaglib_assert(ms_dm2SendingDestination.marr_bufferSize == 8);
          sendSingleDM1DM2(PREVIOUSLY_ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN,marr_dm2SendingDestination);
          return true;
        }
        else
        {
          if (getMultiSendInstance4Comm().sendIsoTarget (
                mrc_identItem.isoName(),
                isoItemSender->isoName(),
                (uint8_t *) marr_dm2SendingDestination,
                ms_dm2SendingDestination.marr_bufferSize,
                PREVIOUSLY_ACTIVE_DIAGNOSTIC_TROUBLE_CODES_PGN,
                &mt_multiSendEventHandler ) )
          { // Message successfully transmitted to multisend -> return true
            ms_dm2SendingDestination.mb_bufferUsedForTP = true;
            return true;
          }
          // else cannot respond because MultiSend busy
        }
      }
      // else cannot respond because we are already sending right now
      getIsoRequestPgnInstance4Comm().answerRequestPGNwithACK ( *mrc_identItem.getIsoItem(), 0x03 ); // CannotRespondNow ACKNOWLEDGE
      return true; 

    case DIAGNOSTIC_DATA_CLEAR_PGN:
      {
#if DEBUG_DIAGNOSTICPGN
      INTERNAL_DEBUG_DEVICE << "processMsgRequestPGN DIAGNOSTIC_DATA_CLEAR_PGN" << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
        uint8_t responsevalue = 0x00; // Positive ACKNOWLEDGE

        // check if diagnostic tool is accepted
        if (mpc_serviceToolVerifier && !mpc_serviceToolVerifier->acceptAsServiceTool(isoItemSender->isoName().toConstIisoName_c()))
          responsevalue = 0x01; // so "NOT Acknowledge (NACK)" it (Control Byte 0x01)
        else
          serviceTool_dtcClearPrevious(); // ServiceTool accepted (or no ServiceToolVerifier defined) 

        if(isoItemReceiver)
          getIsoRequestPgnInstance4Comm().answerRequestPGNwithACK ( *mrc_identItem.getIsoItem(), responsevalue );
        // else: suppress both ACK and NACK when requested to global
        return true;
      }

    default:
      isoaglib_assert(false);
      return false;
  }
}

bool DiagnosticsServices_c::registerServiceToolVerifier (IsoAgLib::iServiceToolVerifier_c& stVerifier)
{
  if (mpc_serviceToolVerifier) return false; // a ServiceToolVerifier is already registered

  mpc_serviceToolVerifier = &stVerifier;
  return true;
}
bool DiagnosticsServices_c::deregisterServiceToolVerifier (IsoAgLib::iServiceToolVerifier_c& stVerifier)
{
  if (mpc_serviceToolVerifier != &stVerifier) return false; // not the ServiceTool registered or none registered

  mpc_serviceToolVerifier = NULL;
  return true;
}

} // end of namespace __IsoAgLib
