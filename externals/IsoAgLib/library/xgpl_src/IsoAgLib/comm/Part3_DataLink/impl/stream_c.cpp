/*
  stream_c.cpp - Implementation of the generic (E)TP/FP-multi-packet
    reception functionality.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "stream_c.h"
#include "multireceive_c.h"
#include <IsoAgLib/hal/hal_system.h>
#include <algorithm>

// Debugging
#if DEBUG_MULTIRECEIVE
  #ifdef SYSTEM_PC
    #include <iostream>
  #else
    #include <supplementary_driver/driver/rs232/impl/rs232io_c.h>
  #endif
#endif


// Begin Namespace __IsoAgLib
namespace __IsoAgLib {


Stream_c::Stream_c(
  const ReceiveStreamIdentifier_c& ac_rsi, 
  uint32_t aui32_msgSize,
  int32_t ai_time
  MULTITON_INST_PARAMETER_DEF_WITH_COMMA ,
  bool ab_skipCtsAwait
) : StreamInput_c()
  , MULTITON_MEMBER_CONSTRUCTOR
    mc_ident (ac_rsi)
  , mt_streamState (StreamRunning)
  , mt_awaitStep (AwaitCtsSend) // so next timeEvent will send out the CTS!
  , mi32_delayCtsUntil (msci32_timeNever) // means send out IMMEDIATELY (the initial CTS, afterwards delay some time!)
  , mui32_byteTotalSize (aui32_msgSize)
  , mb_streamInvalid (false)
  , mui32_byteAlreadyReceived (0)
  , mui32_pkgNextToWrite (1)                     // DEFAULT FOR ISO-STREAMS
  , mui32_pkgTotalSize ((aui32_msgSize + 6) / 7) // DEFAULT FOR ISO-STREAMS
 // mui8_pkgRemainingInBurst     // will be set in "expectBurst(wishingPkgs)", don't care here as mt_awaitStep == awaitCtsSend!!
#ifdef ENABLE_MULTIPACKET_RETRY
  , mui8_pkgsReceivedInBurst( 0 )
  , mui32_burstCurrent (1) // so we know that it's the first burst when calling the processBurst from the client
#else
  , mui32_burstCurrent (0) // so we know that it's the first burst when calling the processBurst from the client
#endif
  , mui8_streamFirstByte (0) // meaning: not yet identified!! (when you check it, it's already set!)
  , mui32_dataPageOffset (0) // will be set when needed
  , mui8_maxPacketInTPBurst( 255 ) //default to 255, set if important later.
  , mi32_timeoutLimit (msci32_timeNever)
  , mi_startTime(ai_time)
  , mi_finishTime(-1)
#ifdef ENABLE_MULTIPACKET_RETRY
  , mui32_isoErrorBurstWaitForPkgThenRetry(0)
  , mui8_isoPkgRetryCountInBurst(0)
#endif
{
  #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  if (getStreamType() == StreamFastPacket)
  { // other calculation for FastPacket, as the first package only has 6 netto data bytes AND first package begins with frame count 0
    mui32_pkgNextToWrite = 0;
    mui32_pkgTotalSize = (aui32_msgSize + 7) / 7;
  }
  // else: for ISO-Streams, these values are set in the constructors.
  #endif

  if ((ac_rsi.getDa() == 0xFF) || ab_skipCtsAwait)
  { // if it's Broadcast (FastPacket-Broadcast or BAM), then directly expect data to be sent!
    // --- or if we directly wanna expect data (for fake streams..)
    expectBurst (255); // We're expecting one big burst directly now without CTS/DPO stuff!
  }
}


Stream_c::Stream_c (const Stream_c &rhs)
  : StreamInput_c (rhs)
  , MULTITON_MEMBER_COPY_CONSTRUCTOR(rhs)
    mc_ident (rhs.mc_ident)
  , mt_streamState (rhs.mt_streamState)
  , mt_awaitStep (rhs.mt_awaitStep)
  , mi32_delayCtsUntil (rhs.mi32_delayCtsUntil)
  , mui32_byteTotalSize (rhs.mui32_byteTotalSize)
  , mb_streamInvalid (rhs.mb_streamInvalid)
  , mui32_byteAlreadyReceived (rhs.mui32_byteAlreadyReceived)
  , mui32_pkgNextToWrite (rhs.mui32_pkgNextToWrite)
  , mui32_pkgTotalSize (rhs.mui32_pkgTotalSize)
  , mui8_pkgRemainingInBurst (rhs.mui8_pkgRemainingInBurst)
#ifdef ENABLE_MULTIPACKET_RETRY
  , mui8_pkgsReceivedInBurst (rhs.mui8_pkgsReceivedInBurst)
#endif
  , mui32_burstCurrent (rhs.mui32_burstCurrent)
  , mui8_streamFirstByte (rhs.mui8_streamFirstByte)
  , mui32_dataPageOffset (rhs.mui32_dataPageOffset)
  , mui8_maxPacketInTPBurst(rhs.mui8_maxPacketInTPBurst)
  , mi32_timeoutLimit (rhs.mi32_timeoutLimit)
  , mi_startTime(rhs.mi_startTime)
  , mi_finishTime(rhs.mi_finishTime)
#ifdef ENABLE_MULTIPACKET_RETRY
  , mui32_isoErrorBurstWaitForPkgThenRetry(rhs.mui32_isoErrorBurstWaitForPkgThenRetry)
  , mui8_isoPkgRetryCountInBurst(rhs.mui8_isoPkgRetryCountInBurst)
#endif
{
}


Stream_c::~Stream_c()
{
}


Stream_c&
Stream_c::operator= (const Stream_c& ref)
{
  MULTITON_MEMBER_ASSIGN(ref)

  mc_ident = ref.mc_ident;
  mt_streamState = ref.mt_streamState;
  mt_awaitStep = ref.mt_awaitStep;
  mi32_delayCtsUntil = ref.mi32_delayCtsUntil;
  mui32_byteTotalSize = ref.mui32_byteTotalSize;
  mb_streamInvalid = ref.mb_streamInvalid;
  mui32_byteAlreadyReceived = ref.mui32_byteAlreadyReceived;

  mui32_pkgNextToWrite = ref.mui32_pkgNextToWrite;
  mui32_pkgTotalSize = ref.mui32_pkgTotalSize;
  mui8_pkgRemainingInBurst = ref.mui8_pkgRemainingInBurst;
#ifdef ENABLE_MULTIPACKET_RETRY
  mui8_pkgsReceivedInBurst = ref.mui8_pkgsReceivedInBurst;
#endif
  mui32_burstCurrent = ref.mui32_burstCurrent;
  mui8_streamFirstByte = ref.mui8_streamFirstByte;
  mui32_dataPageOffset = ref.mui32_dataPageOffset;
  mui8_maxPacketInTPBurst = ref.mui8_maxPacketInTPBurst;

  mi32_timeoutLimit = ref.mi32_timeoutLimit;
  mi_startTime = ref.mi_startTime;
  mi_finishTime = ref.mi_finishTime;

#ifdef ENABLE_MULTIPACKET_RETRY
  mui32_isoErrorBurstWaitForPkgThenRetry = ref.mui32_isoErrorBurstWaitForPkgThenRetry;
  mui8_isoPkgRetryCountInBurst = ref.mui8_isoPkgRetryCountInBurst;
#endif
  return *this;
}


void
Stream_c::awaitNextStep (NextComing_t at_awaitStep, int32_t ai32_timeOut)
{
  mt_awaitStep = at_awaitStep;
  if (at_awaitStep == AwaitCtsSend) {
#ifdef ENABLE_MULTIPACKET_RETRY
    mui32_isoErrorBurstWaitForPkgThenRetry = 0;
#endif
    mi32_delayCtsUntil = HAL::getTime() + ai32_timeOut; // use the timeOut parameter here for the delay!!!!
    mi32_timeoutLimit = msci32_timeNever; // no timeOut on own sending...
  } else {
    mi32_timeoutLimit = (ai32_timeOut==msci32_timeNever) ? (msci32_timeNever) : (HAL::getTime()+ai32_timeOut);
  }
}


bool
Stream_c::readyToSendCts()
{
  return (mi32_delayCtsUntil == msci32_timeNever) || (HAL::getTime() >= mi32_delayCtsUntil);
}



/**
  will be directly called in sendCurrentCts
  (anyway, be sure to only call if in the right state)
*/
uint8_t
Stream_c::expectBurst(uint8_t wishingPkgs)
{
  switch (getStreamType())
  {
    case StreamETP:
      // Await after is CTS has timeout value of "msci32_timeOutT2=1250; // cts -> data(TP)/dpo(ETP)"
      awaitNextStep (AwaitDpo,  msci32_timeOutT2);
      // how many pkgs are missing at all? is it more than wished?
      mui8_pkgRemainingInBurst = uint8_t(STL_NAMESPACE::min<uint32_t>( (mui32_pkgTotalSize - (mui32_pkgNextToWrite - 1)), wishingPkgs));
      break;

    case StreamTP:
      // Await after is CTS has timeout value of "msci32_timeOutT2=1250; // cts -> data(TP)/dpo(ETP)"
      awaitNextStep (AwaitData,
                     (getIdent().getDa() == 0xFF)
                       ? msci32_timeOutT1 /* BAM */
                       : msci32_timeOutT2 /* dest-adr. */);
      // how many pkgs are missing at all? is it more than wished?
      mui8_pkgRemainingInBurst = uint8_t(STL_NAMESPACE::min<uint32_t>( (mui32_pkgTotalSize - (mui32_pkgNextToWrite - 1)), wishingPkgs));
      mui8_pkgRemainingInBurst = STL_NAMESPACE::min<uint8_t>( mui8_maxPacketInTPBurst, mui8_pkgRemainingInBurst );
      break;

#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
    case StreamFastPacket:
      awaitNextStep (AwaitData, msci32_timeOutFP);
      mui8_pkgRemainingInBurst = uint8_t(mui32_pkgTotalSize); // for Fast-Packet there's only one burst!
      break;
#endif
  }

#ifdef ENABLE_MULTIPACKET_RETRY
  // is the expected Burst a next (new) one or is it a complete retry?
  if( mui8_pkgsReceivedInBurst > 0 )
    ++mui32_burstCurrent;

  mui8_pkgsReceivedInBurst = 0;
#else
  // increase mui32_burstCurrent, the expected Burst is a next new one (of course)...
  ++mui32_burstCurrent;
#endif

  return mui8_pkgRemainingInBurst;
}



bool
Stream_c::handleDataPacket (const CanPkg_c& pkg)
{
  const Flexible8ByteString_c* apu_data = pkg.getDataUnionConst();
  // expecting data at all?
  if (mt_awaitStep != AwaitData) {
    #if DEBUG_MULTIRECEIVE
      INTERNAL_DEBUG_DEVICE << "mt_awaitStep != AwaitData! --- mt_awaitStep ==" << mt_awaitStep << INTERNAL_DEBUG_DEVICE_ENDL;
    #endif
    return false; // abort stream
  }

  const uint8_t sequenceNr = apu_data->getUint8Data( 0 );

  uint32_t offset = 0;
  switch (getStreamType())
  {
    case StreamETP:
      offset = mui32_dataPageOffset;
      // break left out intentionally
    case StreamTP:
      if( sequenceNr == 0 )
        return false; // abort stream

#ifdef ENABLE_MULTIPACKET_RETRY
      bool b_isoFirstWrongPktInBurst=false;
      if ((sequenceNr + offset) != mui32_pkgNextToWrite)
      { // wrong packet!
        if( mui32_isoErrorBurstWaitForPkgThenRetry == 0 )
        {
            mui32_isoErrorBurstWaitForPkgThenRetry = mui32_pkgNextToWrite + mui8_pkgRemainingInBurst - 1;
            b_isoFirstWrongPktInBurst=true;
        }
      }

      if ((sequenceNr + offset) == mui32_isoErrorBurstWaitForPkgThenRetry)
      {
        const int32_t ci32_ctsSendDelay = __IsoAgLib::getMultiReceiveInstance4Comm().getCtsDelay();
        awaitNextStep (AwaitCtsSend, ci32_ctsSendDelay);
      }
      else if( b_isoFirstWrongPktInBurst )
      {
        ++mui8_isoPkgRetryCountInBurst;
        if( mui8_isoPkgRetryCountInBurst == 3 )
          return false; // abort stream
      }
#else
      if ((sequenceNr + offset) != mui32_pkgNextToWrite)
      {
        #if DEBUG_MULTIRECEIVE
          INTERNAL_DEBUG_DEVICE << "wrong tp/etp pkg-number! ";
        #endif
        return false; // abort stream
      }
#endif
      break;

    #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
    case StreamFastPacket:
      if ((apu_data->getUint8Data( 0 ) & (0x1FU)) != mui32_pkgNextToWrite)
      {
        #if DEBUG_MULTIRECEIVE
          INTERNAL_DEBUG_DEVICE << "wrong fp pkg-number! ";
        #endif
        return false; // No retry on FP, ignore wrong frame. // abort stream
      }
      break;
    #endif
  }


  #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  if ((getStreamType() == StreamFastPacket) && (mui32_pkgNextToWrite == 0))
  { // special FastPacket first-frame handling
    insertFirst6Bytes (apu_data->getUint8DataConstPointer(2));
    mui32_byteAlreadyReceived += 6;
  }
  else
  #endif
  {
    insert7Bytes (apu_data->getUint8DataConstPointer(1));
    mui32_byteAlreadyReceived += 7;
  }

  if (mui32_byteAlreadyReceived > mui32_byteTotalSize)
    mui32_byteAlreadyReceived = mui32_byteTotalSize; // cut any padded (0xFF) bytes...

  #if DEBUG_MULTIRECEIVE
    INTERNAL_DEBUG_DEVICE << "#" << mui32_pkgNextToWrite << " ";
  #endif

/// <<UPDATE_ALL>> Pkg counting stuff
  mui32_pkgNextToWrite++;
  mui8_pkgRemainingInBurst--;
#ifdef ENABLE_MULTIPACKET_RETRY
  mui8_pkgsReceivedInBurst++;
#endif

  if (mui8_pkgRemainingInBurst == 0) {
#ifdef ENABLE_MULTIPACKET_RETRY
    mui8_isoPkgRetryCountInBurst = 0;
#endif
    // End? or CTS for more?
    if (
        #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
        (getStreamType() == StreamFastPacket) ||
        #endif
        (mui32_pkgNextToWrite > mui32_pkgTotalSize))
    {
      // ---END--- was last packet! So
      awaitNextStep (AwaitNothing, msci32_timeNever); // no timeOut on own Send-Awaits
      mt_streamState = StreamFinished;
      mi_finishTime = pkg.time();
    } else {
      // ---CTS--- go for more!
      // Calculate the send delay
      const int32_t ci32_ctsSendDelay = __IsoAgLib::getMultiReceiveInstance4Comm().getCtsDelay();
      awaitNextStep (AwaitCtsSend, ci32_ctsSendDelay); // no timeOut on own Send-Awaits (this is automatically done in awaitNextStep) - parameter is the send-delay!
    }
    // (A complete / The last) chunk is received, handling will be done after this function returns
  } else {
    awaitNextStep (AwaitData, msci32_timeOutT1); // state "AwaitData" was already set until now, but it's about the timeOut!
  }

  return true;
}

bool
Stream_c::setDataPageOffset (uint32_t aui32_dataPageOffset)
{
  if (mt_awaitStep == AwaitDpo) {
    mui32_dataPageOffset = aui32_dataPageOffset;
    awaitNextStep (AwaitData, msci32_timeOutT5);
    #if DEBUG_MULTIRECEIVE
      INTERNAL_DEBUG_DEVICE << "DPO ";
    #endif
    return true; // was awaited!
  } else {
    #if DEBUG_MULTIRECEIVE
      INTERNAL_DEBUG_DEVICE << "\n\n DPO was not awaited at this state (" << (uint16_t) mt_awaitStep << "), please handle as error! \n ";
    #endif
    return false; // DPO was not awaited at this state, please handle as error!
  }
}


bool
Stream_c::timedOut()
{
  bool b_result=false;

  if (mi32_timeoutLimit != msci32_timeNever) {
    if (HAL::getTime() > mi32_timeoutLimit) b_result = true;
  }

  return b_result;
}


int32_t
Stream_c::nextTimeEvent() const
{
  switch( getNextComing() )
  {
    case AwaitNothing:
      return msci32_timeNever; /* stream finished, no timeEvent request! */

    case AwaitDpo:
    case AwaitData:
      isoaglib_assert( mi32_timeoutLimit != msci32_timeNever );
      return mi32_timeoutLimit;

    case AwaitCtsSend:
      isoaglib_assert( mi32_delayCtsUntil != msci32_timeNever ); // should never be set to c'tor's default value!
      return mi32_delayCtsUntil;
  }
  
  return msci32_timeNever; // make (at least Tasking) compiler happy.
}

} //__IsoAgLib
