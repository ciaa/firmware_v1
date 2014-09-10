/*
  sendstream_c.cpp: transfer of multi message data

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "sendstream_c.h"

#include <IsoAgLib/scheduler/impl/scheduler_c.h>
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/util/iassert.h>

#include "multisend_c.h"
#include "multisendpkg_c.h"

#if DEBUG_MULTISEND
  #ifdef SYSTEM_PC
    #include <iostream>
  #else
    #include <supplementary_driver/driver/rs232/impl/rs232io_c.h>
  #endif
#endif

static const uint8_t scui8_CM_RTS = 16;
static const uint8_t scui8_CM_CTS = 17;
static const uint8_t scui8_CM_EndofMsgACK = 19;
static const uint8_t scui8_CM_BAM = 32;
static const uint8_t scui8_eCM_RTS = 20;
static const uint8_t scui8_eCM_CTS = 21;
static const uint8_t scui8_eCM_DPO = 22;
static const uint8_t scui8_eCM_EndofMsgACK = 23;
static const uint8_t scui8_CM_ConnAbort = 255;


namespace __IsoAgLib {

void
SendStream_c::init (const IsoName_c& acrc_isoNameSender, const IsoName_c& acrc_isoNameReceiver, const HUGE_MEM uint8_t* rhpb_data, uint32_t aui32_dataSize, uint32_t aui32_pgn, IsoAgLib::iMultiSendStreamer_c* apc_mss, msgType_t ren_msgType, MultiSendEventHandler_c* apc_multiSendEventHandler)
{
  mui32_pgn = aui32_pgn;
  if ((mui32_pgn & 0x0FF00LU) < 0x0F000LU) mui32_pgn &= 0x3FF00LU;
  mc_isoNameSender = acrc_isoNameSender;     // copy the 8byte IsoName
  mc_isoNameReceiver = acrc_isoNameReceiver; // copy the 8byte IsoName
  mpc_multiSendEventHandler = apc_multiSendEventHandler;
  mhpbui8_data = rhpb_data;
  mui32_dataSize = aui32_dataSize;   // initialise data for begin
  mpc_mss = apc_mss;
  men_msgType = ren_msgType;

  mui32_dataBufferOffset = 0;
  mui8_packetsSentInThisBurst = 0;

  /// Do send out the first packet right now, right here!
  MultiSendPkg_c r_multiSendPkg;

#if defined (ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
  if (men_msgType == NmeaFastPacket)
  { // send Fast Packet First Frame
    mui8_FpSequenceCounter = mrc_multiSend.allocFpSequenceCounter();
    r_multiSendPkg.setUint8Data(0, static_cast<uint8_t>((mui8_FpSequenceCounter << 5) & 0xE0));
    r_multiSendPkg.setUint8Data(1, static_cast<uint8_t>(getDataSize() & 0xFF));

    uint8_t ui8_nettoCnt = 6;
    if (ui8_nettoCnt > getDataSize()) ui8_nettoCnt = uint8_t(getDataSize());
    if (mhpbui8_data != NULL) {
      r_multiSendPkg.setFastPacketDataPart(mhpbui8_data, 0, ui8_nettoCnt, 2);
    } else {
      mpc_mss->setDataNextFastPacketStreamPart (&r_multiSendPkg, ui8_nettoCnt, 2);
    }
    mui32_dataBufferOffset += ui8_nettoCnt; // already sent out the first 6 bytes along with the first FP message.
    switchToState (SendData, 0);
    sendPacketFp( r_multiSendPkg );
  }
  else
#endif
  { // NOT NmeaFastPacket ==> some ISO protocol
    r_multiSendPkg.setUint16Data(1, getDataSize() & 0xFFFF);                             // Byte 2+3
    r_multiSendPkg.setUint8Data (5, static_cast<uint8_t>(mui32_pgn & 0xFF));              // Byte 6
    r_multiSendPkg.setUint16Data(6, static_cast<uint16_t>((mui32_pgn >> 8) & 0xFFFFU));   // Byte 7+8
    if (men_msgType == IsoETP)
    {
      r_multiSendPkg.setUint8Data (0, static_cast<uint8_t>(scui8_eCM_RTS));              // Byte 1
      r_multiSendPkg.setUint16Data(3, static_cast<uint16_t>(getDataSize() >> 16));       // Byte 4+5
      switchToState (AwaitCts, 1250);
    }
    else if (men_msgType == IsoTP)
    {
      r_multiSendPkg.setUint8Data (0, static_cast<uint8_t>(scui8_CM_RTS));               // Byte 1
      r_multiSendPkg.setUint8Data (3, static_cast<uint8_t>((mui32_dataSize + 6) / 7));    // Byte 4
      r_multiSendPkg.setUint8Data (4, static_cast<uint8_t>(0xFF));                       // Byte 5
      switchToState (AwaitCts, 1250);
    }
    else if (men_msgType == IsoTPbroadcast)
    {
      mui8_packetsLeftToSendInBurst = static_cast<uint8_t>((mui32_dataSize + 6) / 7);
      r_multiSendPkg.setUint8Data (0, static_cast<uint8_t>(scui8_CM_BAM));               // Byte 1
      r_multiSendPkg.setUint8Data (3, mui8_packetsLeftToSendInBurst);                    // Byte 4
      r_multiSendPkg.setUint8Data (4, static_cast<uint8_t>(0xFF));                       // Byte 5
      switchToState (AwaitCts, 50); // on broadcast, we'll have to interspace with 50ms (minimum!)
    }

    mui32_packetNrRequestedInLastCts = 1;
    if (mpc_mss) mpc_mss->resetDataNextStreamPart();
    sendPacketIso( false, r_multiSendPkg ); // ISO never starts with data. BAM/RTS
  }
  if (men_msgType == IsoTPbroadcast)
  { // now we can switch the state to SendData
    switchToState (SendData, 50); // on broadcast, we'll have to interspace with 50ms (minimum!)
  }
  notifySender(Running);
}


void
SendStream_c::notifySender(sendSuccess_t ae_newStatus)
{
  men_sendSuccess = ae_newStatus;
  if (mpc_multiSendEventHandler)
    mpc_multiSendEventHandler->reactOnStateChange(*this);
}


bool
SendStream_c::timeEvent ( unsigned pkgCnt )
{
  isoaglib_assert( pkgCnt > 0 );
  MultiSendPkg_c c_multiSendPkg;
  uint8_t ui8_nettoDataCnt;

  switch (men_sendState)
  {
    case AwaitCts:
    case AwaitEndofmsgack:
    case SendPauseTillCts:
      switch (men_msgType)
      {
        case IsoTPbroadcast: // we shouldn't reach this state!
        case NmeaFastPacket: // we shouldn't reach this state!
          break;
        case IsoTP: // break left out intentionally
        case IsoETP:
          abortSend( ConnectionAbortReasonTimeOut );
          return true; // FINISHED SendStream, remove it from list please!
      }
      break;

    case SendData:
    {
      const int32_t before = System_c::getTime();
#if defined (ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
      if (men_msgType == NmeaFastPacket)
      {
        for ( unsigned pkgInd = 0; pkgInd < pkgCnt; pkgInd++)
        {
          prepareSendMsg (ui8_nettoDataCnt);
          c_multiSendPkg.setUint8Data(0, static_cast<uint8_t>(((mui8_FpSequenceCounter << 5) |(mui8_packetsSentInThisBurst & 0x1F)) ) );

          if (mhpbui8_data != NULL) {
            c_multiSendPkg.setFastPacketDataPart(mhpbui8_data, mui32_dataBufferOffset, ui8_nettoDataCnt, 1);
          } else {
            mpc_mss->setDataNextFastPacketStreamPart (&c_multiSendPkg, ui8_nettoDataCnt, 1);
          }
          sendPacketFp( c_multiSendPkg );
          mui32_dataBufferOffset += ui8_nettoDataCnt;
          // break if this message part is finished
          if (isCompleteData())
          {
            notifySender(SendSuccess);
            return true; // FINISHED SendStream, remove it from list please!
          }
        } // for

        const int32_t timeToNextTrigger = ( pkgCnt / 2 ) - ( System_c::getTime() - before );
        // sending frames takes us at least 1 ms per 2 frames
        // retrigger first possible time we can continue sending.
        retriggerIn ( timeToNextTrigger < 0 ? 0 : timeToNextTrigger ); 
      }
      else
#endif
      { // NOT NmeaFastPacket - some ISO protocol!
        if (men_msgType == IsoTPbroadcast)
        { // IsoTPbroadcast forces 50ms between all packets!!
          pkgCnt = 1; // only send 1 packet at a time, then wait 50ms.
        }

        for ( unsigned pkgInd = 0; pkgInd < pkgCnt; pkgInd++)
        {
          prepareSendMsg (ui8_nettoDataCnt);
          const uint8_t cui8_pkgNumberToSend = uint8_t(mui8_packetsSentInThisBurst + ((men_msgType == IsoETP) ? 0 : uint8_t(mui32_packetNrRequestedInLastCts-1)));
          c_multiSendPkg.setUint8Data (0, cui8_pkgNumberToSend);
          if (mhpbui8_data != NULL) {
            c_multiSendPkg.setDataPart (mhpbui8_data, mui32_dataBufferOffset, ui8_nettoDataCnt);
          } else {
            mpc_mss->setDataNextStreamPart (&c_multiSendPkg, ui8_nettoDataCnt);
          }
          sendPacketIso (true, c_multiSendPkg );
          mui32_dataBufferOffset += ui8_nettoDataCnt;
          // break if this message part is finished
          if (isCompleteData())
          {
            if (men_msgType == IsoTPbroadcast)
            { // ISO Broadcast
              notifySender(SendSuccess);
              return true; // FINISHED SendStream, remove it from list please!
            }
            else
            { // ISO Target
              #if DEBUG_MULTISEND
              INTERNAL_DEBUG_DEVICE << "MultiSend_c::timeEvent --- after complete Sending now awaiting EOMACK!" << INTERNAL_DEBUG_DEVICE_ENDL;
              #endif
              switchToState (AwaitEndofmsgack, 1250);
              return false; // stream not yet finished!
            }
          }
          // not completely finished, but maybe this burst has finished?
          if (isCompleteBurst())
          { // wait for CTS for next part of transfer
            switchToState (AwaitCts, 1250);
            #if DEBUG_MULTISEND
            INTERNAL_DEBUG_DEVICE << "MultiSend_c::timeEvent --- after Sending now awaiting CTS!" << INTERNAL_DEBUG_DEVICE_ENDL;
            #endif
            return false; // stream not yet finished!
          }
          // nothing special, keep on sending...
        } // for - sent all pkgs for now, yet some more to go, send them in a later timeEvent()

        if (men_msgType == IsoTPbroadcast)
        { // IsoTPbroadcast forces 50ms between all packets!!
          retriggerIn (50); // same state - but the time stamp gets updated, so we'll wait on for 50ms...
        }
        else
        { // IsoTP || IsoETP can send on immediately
          const int32_t timeToNextTrigger = ( pkgCnt / 3 ) - ( System_c::getTime() - before );
          // sending frames takes us at least 1 ms per 3 frames
          // retrigger first possible time we can continue sending.
          retriggerIn ( timeToNextTrigger <= 0 ? 1 : timeToNextTrigger ); 
        }
      } // Not Nmea- some ISO protocol
    } break; // case SendData
  }
  return false; // stream not yet finished!
}


void 
SendStream_c::processMsg( const CanPkgExt_c& arc_data )
{
  if (isFinished() || (men_msgType == NmeaFastPacket) || (men_msgType == IsoTPbroadcast))
  { // nothing to come in for us when we're broadcasting or already done (succeeded or aborted)
    return; // anyway, return with false here for "safety"
  }

  switch (arc_data.getUint8Data (0))
  {
    case scui8_eCM_CTS:
    case scui8_CM_CTS:
      if (   (men_sendState == AwaitCts)
          || (men_sendState == SendPauseTillCts)
          || (men_sendState == AwaitEndofmsgack)
          || (men_sendState == SendData)
         )
      { // awaited (or resent-) CTS received -- only possible in ISO protocol case
        mui8_packetsLeftToSendInBurst = arc_data.getUint8Data (1);

        const uint32_t cui32_packetNrRequested =
          uint32_t(arc_data.getUint8Data(2)) +
          ((men_msgType == IsoTP) ? 0 : (uint32_t(arc_data.getUint16Data(3)) << 8));

        mui32_dataBufferOffset = (cui32_packetNrRequested - 1UL) * 7L; // take sequence nr with -1 because prepareSendMsg increment by 1 before first send

        if (mui8_packetsLeftToSendInBurst == 0)
        { /// Send pause commanded from receiver (CTS with 0 packets requested)
          switchToState (SendPauseTillCts, 1050);
        }
        else
        {
          
          MultiSendPkg_c c_sendData;
          
          /// Really some packets requested
          // Can we deliver all the requested packets?
          if ((mui32_dataBufferOffset + (uint32_t(mui8_packetsLeftToSendInBurst-1)*7)) >= mui32_dataSize)
          { // request will be out of buffer, either already at the start or later during sending
            abortSend( ConnectionAbortReasonUnexpectedDataPacket );
            break;
          }

          if (mpc_mss != NULL)
          { // For the MSS, it's only allowed to go back to an arbitrary point in the last burst
            // Is the receiver asking for the next available packet (that'd be next if we were continuing to stream out)?
            if (cui32_packetNrRequested == (mui32_packetNrRequestedInLastCts + mui8_packetsSentInThisBurst))
            { // then nothing to do, we're at this position already, we can just send on...
              // --> This should be the normal case!
            }
            else
            { // some retransmission is requested
              // (needn't be the beginning, but needs to be in the last burst!)
              if ( (cui32_packetNrRequested >= mui32_packetNrRequestedInLastCts)
                && (cui32_packetNrRequested < (mui32_packetNrRequestedInLastCts + mui8_packetsSentInThisBurst)) )
              { // go back to requested point in last burst.
                // first, go back to start of last burst
                mpc_mss->restoreDataNextStreamPart();
                // second, advance to the requested position
                while (mui32_packetNrRequestedInLastCts < cui32_packetNrRequested)
                { // step one packet forward.
                  const uint8_t cui8_nettoDataCnt = 7; // here, the netto data count is always 7
                  // let the packet be streamed to c_multiSendPkg, although we're just ignoring them, just need to fast forward...
                  mpc_mss->setDataNextStreamPart(&c_sendData, cui8_nettoDataCnt);
                  ++mui32_packetNrRequestedInLastCts; // we can alter this variable. It's being set below anyway..
                }
              }
              else
              { // requesting some other packets (earlier than the last burst or after the current position (jump forward))
                abortSend( ConnectionAbortReasonBadSequenceNumber );
                break; // break out of big switch to reach "return true" for deletion of this stream.
              }
            }
            mpc_mss->saveDataNextStreamPart();
          }
          // else: it's okay if we have the complete buffer, the sender can get what he wants. his problem.

          // send out Extended Connection Mode Data Packet Offset
          if (men_msgType == IsoETP)
          {
            c_sendData.setUint8Data (0, static_cast<uint8_t>(scui8_eCM_DPO));
            c_sendData.setUint8Data (1, static_cast<uint8_t>(mui8_packetsLeftToSendInBurst));
            c_sendData.setUint16Data(2, static_cast<uint16_t>((cui32_packetNrRequested-1) & 0xFFFF));
            c_sendData.setUint8Data (4, static_cast<uint8_t>((cui32_packetNrRequested-1) >> 16));
            // change order of setUint8Data(), setUint16Data() so that
            // setUint16Data() with equal position, so that a faster assignment is enabled
            c_sendData.setUint8Data (5, static_cast<uint8_t>(mui32_pgn & 0xFF));
            c_sendData.setUint16Data(6, static_cast<uint16_t>(mui32_pgn >> 8));
            sendPacketIso( false, c_sendData ); // DPO is not Data, we need the connection management pgn here...
          }
          // else if (men_msgType == IsoTP) //-> nothing special to do here...

          mui32_packetNrRequestedInLastCts = cui32_packetNrRequested;
          mui8_packetsSentInThisBurst = 0;

          // now receiver wants to receive new data
          #if DEBUG_MULTISEND
          INTERNAL_DEBUG_DEVICE << "Start To Send Next Data Block" << INTERNAL_DEBUG_DEVICE_ENDL;
          #endif
          switchToState (SendData, 0);
        } // end request to send
      } // awaited (or resent-) CTS received
      // else: nothing to do on "SendRts"
      break;

    case scui8_CM_EndofMsgACK:
    case scui8_eCM_EndofMsgACK:
      if (men_sendState == AwaitEndofmsgack)
      {
        #if DEBUG_MULTISEND
        INTERNAL_DEBUG_DEVICE << "MultiSend_c::processMsg --- EOMACK received!" << INTERNAL_DEBUG_DEVICE_ENDL;
        #endif
        // Notify sender that it finished!
        notifySender(SendSuccess);
        // so trigger timeEvent so it gets actually deleted - do it asap so that there's
        // no confusion about the finished stream and a possible new stream!
        retriggerIn (0); // changed from 1500 to 0 by JVB 20090916
        /** @todo SOON-178 Should we remove the finished send-stream here now immediately
            even though we're NOT iterating through the list now? */
        break;
      }
      // else: handle the same as ConnAbort (EoMACK was sent unsolicited)
      // break left out intentionally
    case scui8_CM_ConnAbort:
      #if DEBUG_MULTISEND
      INTERNAL_DEBUG_DEVICE << "MultiSend_c::processMsg --- ConnAbort received!" << INTERNAL_DEBUG_DEVICE_ENDL;
      #endif
      notifySender(SendAborted);  // will be kicked out after next timeEvent!
      retriggerIn (1500);
    default:
      break;
  }
}


void
SendStream_c::abortSend( ConnectionAbortReason_t reason )
{
  switch (men_msgType)
  {
  case IsoTP:
  case IsoETP:
  {
    MultiSendPkg_c c_multiSendPkg;

    c_multiSendPkg.setUint8Data (0, static_cast<uint8_t>(scui8_CM_ConnAbort));
    c_multiSendPkg.setUint8Data (1, static_cast<uint8_t>(reason));
    c_multiSendPkg.setUint8Data (2, uint8_t(0xFF));
    c_multiSendPkg.setUint16Data(3, uint16_t(0xFFFF));
    c_multiSendPkg.setUint8Data (5, static_cast<uint8_t>(mui32_pgn & 0xFF));
    c_multiSendPkg.setUint16Data(6, static_cast<uint16_t>(mui32_pgn >> 8));

    sendPacketIso( false, c_multiSendPkg ); // ConnAbort is not Data, we need the connection management pgn here...
  } break;

  case IsoTPbroadcast:
  case NmeaFastPacket:
    break;
  }

  notifySender(SendAborted); // will cause isFinished() to report true!
}


void
SendStream_c::sendPacketIso (bool ab_data, MultiSendPkg_c& arc_data )
{
  // set identifier
  arc_data.setIsoPri (6);
  arc_data.setISONameForSA (mc_isoNameSender);
  arc_data.setLen (8);

  switch (men_msgType)
  {
    case IsoTP:          arc_data.setIsoPgn (ab_data ?  TP_DATA_TRANSFER_PGN :  TP_CONN_MANAGE_PGN);
                         arc_data.setISONameForDA (mc_isoNameReceiver);
                         break;
    case IsoETP:         arc_data.setIsoPgn (ab_data ? ETP_DATA_TRANSFER_PGN : ETP_CONN_MANAGE_PGN);
                         arc_data.setISONameForDA (mc_isoNameReceiver);
                         break;
    case IsoTPbroadcast: arc_data.setIsoPgn (ab_data ?  TP_DATA_TRANSFER_PGN :  TP_CONN_MANAGE_PGN);
                         arc_data.setGlobalDA();
                         break;

    case NmeaFastPacket: // all other cases are wrong here
    default:
                         isoaglib_assert (men_msgType != NmeaFastPacket);
                         return;
  } // switch
  getIsoBusInstance4Comm() << arc_data;
}


#if defined (ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
void
SendStream_c::sendPacketFp( MultiSendPkg_c& pkg )
{
  // no-one should call this when the Stream is not set to FP-mode.
  isoaglib_assert (men_msgType == NmeaFastPacket);

  // For fast packet, the PGN itself tells us that the message is a fast packet message.
  /// Since we currently only need/support broadcast FP-PGNs, we do NOT need to "setIsoPs(...)" !
  pkg.setIsoPgn (mui32_pgn);
  /// @todo ON REQUEST-690 The FP-Messages can have different priorities,
  /// so the calling code must be able to specify the priority.
  /// Now setting the Priority to 3 as it's the default priority
  /// for the currently only NMEA message being sent out, so regard
  /// this is hard-coded workaround for the current situation.
  /// (This PGN is NMEA_GPS_POSITION_DATA_PGN)
  pkg.setIsoPri (3);
  pkg.setISONameForSA (mc_isoNameSender);
  pkg.setLen (8);

  getIsoBusInstance4Comm() << pkg;
}
#endif


void
SendStream_c::prepareSendMsg (uint8_t &ui8_nettoDataCnt)
{
  ui8_nettoDataCnt = 7;
  --mui8_packetsLeftToSendInBurst;
  ++mui8_packetsSentInThisBurst;

  if (mui32_dataBufferOffset + ui8_nettoDataCnt > mui32_dataSize)
  {
    ui8_nettoDataCnt = uint8_t(mui32_dataSize - mui32_dataBufferOffset);
  }
}


} // __IsoAgLib
