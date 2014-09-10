/*
  sendstream_c.h: transfer of a stream of multi message data

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef SENDSTREAM_C_H
#define SENDSTREAM_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoname_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/canpkgext_c.h>
#include <IsoAgLib/driver/system/impl/system_c.h>

#include "multisendeventhandler_c.h"
#include "../imultisendstreamer_c.h"

#include <list>
#include <functional>

// Begin Namespace __IsoAgLib
namespace __IsoAgLib {


enum ConnectionAbortReason_t
{
  ConnectionAbortReasonCannotSupportAnother = 1,
  ConnectionAbortReasonSystemBusy = 2,
  ConnectionAbortReasonTimeOut = 3,
  ConnectionAbortReasonUnexpectedCts = 4,
  ConnectionAbortReasonMaximumRetransmitRequestLimitReached = 5,
  ConnectionAbortReasonUnexpectedDataPacket = 6,
  ConnectionAbortReasonBadSequenceNumber= 7,
  ConnectionAbortReasonDuplicateSequenceNumber= 8,
  ConnectionAbortReasonUnexpectedEdpo = 9,
  ConnectionAbortReasonUnexpectedEdpoPgn = 10,
  ConnectionAbortReasonEdpoNumberOfPacketsIsGreaterThanCts = 11,
  ConnectionAbortReasonBadEdpoOffset = 12,
  ConnectionAbortReasonAnyOtherError = 13
};

// forward declarations
class MultiSend_c;

class SendStream_c : public ClientBase
{
public:
  enum sendState_t   { AwaitCts, SendData, SendPauseTillCts, AwaitEndofmsgack };
  enum sendSuccess_t { SendSuccess, SendAborted, Running };
  enum msgType_t     { IsoTP, IsoETP, IsoTPbroadcast, NmeaFastPacket };

public:
  // This c'tor does NOT REALLY "initialize "anything, use "init(...)" directly after construction!!!!
  SendStream_c (MultiSend_c& arc_multiSend MULTITON_INST_PARAMETER_DEF_WITH_COMMA )
    : MULTITON_PARENT_CONSTRUCTOR 
      mui32_pgn (0)
    , mc_isoNameReceiver()
    , mc_isoNameSender()
    , mpc_multiSendEventHandler(NULL)
    #if defined (ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
    , mui8_FpSequenceCounter (0)
    #endif
    , mi32_timestampToWaitTo (0)
    , mui32_dataBufferOffset (0)
    , mui32_dataSize (0)
    , mhpbui8_data (NULL)
    , men_sendState (AwaitCts) // dummy init state
    , men_sendSuccess (Running) // dummy init state
    , men_msgType (IsoTP) // dummy init state
    , mpc_mss (NULL)
    , mui32_packetNrRequestedInLastCts (0)
    , mui8_packetsLeftToSendInBurst (0)
    , mui8_packetsSentInThisBurst (0)
    , mrc_multiSend (arc_multiSend)
  {}

  /**
    call "init" directly after construction!
  */
  void init (const IsoName_c& acrc_isoNameSender,
             const IsoName_c& acrc_isoNameReceiver,
             const HUGE_MEM uint8_t* rhpb_data,
             uint32_t aui32_dataSize,
             uint32_t aui32_pgn,
             IsoAgLib::iMultiSendStreamer_c* apc_mss,
             msgType_t ren_msgType,
             MultiSendEventHandler_c* apc_multiSendEventHandler);

  /**
    @return true: stream finished, it'll be erased then!
  */
  bool timeEvent (unsigned pkgCnt);

  void processMsg( const CanPkgExt_c& arc_data );

  int32_t getNextTriggerTime() const { return mi32_timestampToWaitTo; }

  /**
    check if time delay is reached
    @return true -> time delay is over
  */
  bool timeHasCome() const {return (System_c::getTime() - mi32_timestampToWaitTo) >= 0; }

  /**
    check if current message is complete
  */
  bool isCompleteBurst() const { return (mui8_packetsLeftToSendInBurst == 0); }

  /**
    check if send of all data is complete
  */
  bool isCompleteData() const { return (mui32_dataBufferOffset >= mui32_dataSize); }

  /**
    deliver the data size in byte
    @return data size in uint32_t for RTS
  */
  uint32_t getDataSize() const { return mui32_dataSize; }

  bool isFinished() const { return (men_sendSuccess == SendAborted) || (men_sendSuccess == SendSuccess); }

  sendSuccess_t getSendSuccess() const { return men_sendSuccess; }

  void abortSend( ConnectionAbortReason_t reason );

  bool matchSaDa (const IsoName_c& acrc_sa, const IsoName_c& acrc_da) const { return (acrc_sa == mc_isoNameSender) && (acrc_da == mc_isoNameReceiver); }

  MultiSendEventHandler_c* getMultiSendEventHandler() const { return mpc_multiSendEventHandler; }

  uint32_t pgn() const { return mui32_pgn;}
  const IsoName_c& receiver() const { return mc_isoNameReceiver; }
  const IsoName_c& sender() const { return mc_isoNameSender; }
  
  bool isBurstStream() const {
    return ( men_msgType != IsoTPbroadcast );
  }


private:
  void sendPacketIso( bool ab_data, MultiSendPkg_c& arc_data );

#if defined (ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
  /**
    send a FP message -> set the ident and initiate sending to CAN
  */
  void sendPacketFp( MultiSendPkg_c& );
#endif

  /**
    calculate the actual sequence number and
    calculate the amount of data bytes which must be placed in new CAN pkg
    @param ui8_nettoDataCnt amount of data which should be sent within this msg
  */
  void prepareSendMsg (uint8_t &ui8_nettoDataCnt);

  void retriggerIn (int32_t i32_timeDelta) { mi32_timestampToWaitTo = System_c::getTime() + i32_timeDelta;}

  void switchToState (sendState_t ren_sendState, int32_t i32_timeDelta) { men_sendState = ren_sendState; retriggerIn (i32_timeDelta); }

  const SendStream_c& operator= (const SendStream_c&); // Only private declaration, no implementation: Forbid assignment of SendStream_c instances.

  void notifySender( sendSuccess_t );

private:
  uint32_t mui32_pgn;
  IsoName_c mc_isoNameReceiver;
  IsoName_c mc_isoNameSender;

  MultiSendEventHandler_c* mpc_multiSendEventHandler;

  #if defined (ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
  uint8_t mui8_FpSequenceCounter;
  #endif

  /** timestamp for time control */
  int32_t mi32_timestampToWaitTo;

  /** data counter for data to send
      will be set on CTS and incremented after each sent packet. */
  uint32_t mui32_dataBufferOffset;

  /** size of the data complete */
  uint32_t mui32_dataSize;

  /** pointer to the data */
  const HUGE_MEM uint8_t* mhpbui8_data;

  sendState_t men_sendState;

  sendSuccess_t men_sendSuccess;

  /** are we broadcast, iso, extended, fast packet? */
  msgType_t men_msgType;

  /** pointer to an IsoAgLib::MultiSendStreamer_c class which streams out parts of the stream step by step */
  IsoAgLib::iMultiSendStreamer_c* mpc_mss;

  /** save ... from last CTS so we can see if the CTS was resent... */
  uint32_t mui32_packetNrRequestedInLastCts;

  /** cnt of pkg left to send in this burst */
  uint8_t mui8_packetsLeftToSendInBurst;

  /** cnt of pkg sent since the last DPO (ETP) - now also used to TP */
  uint8_t mui8_packetsSentInThisBurst;

  MultiSend_c& mrc_multiSend;
};

} // __IsoAgLib

#endif
