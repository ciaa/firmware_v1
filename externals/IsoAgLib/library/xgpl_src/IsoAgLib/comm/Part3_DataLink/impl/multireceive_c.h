/*
  multireceive_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef MULTIRECEIVE_C_H
#define MULTIRECEIVE_C_H

namespace IsoAgLib {
  class ReceiveStreamIdentifier_c;
}


#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/scheduler/impl/schedulertask_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoname_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/cfstatehandler_c.h>
#include <IsoAgLib/driver/can/impl/cancustomer_c.h>

#include <list>

#define STREAM_IMPLEMENTATION_HEADER <IsoAgLib/comm/Part3_DataLink/impl/DEF_Stream_h_IMPL>
#include STREAM_IMPLEMENTATION_HEADER


namespace __IsoAgLib {


/** struct for client definition */
class MultiReceiveClientWrapper_s : public ClientBase
{
public:
  /// @param acrc_isoNameSender ".isUnspecified()" to receive Multipacket from anybody
  ///                           ".isSpecified()" to receive Multipacket only from that sender
  MultiReceiveClientWrapper_s( CanCustomer_c& apc_client,
                               const IsoName_c& acrc_isoNameClient,
                               uint32_t aui32_pgn,
                               uint32_t aui32_pgnMask,
                               bool ab_alsoBroadcast,
                               bool ab_alsoGlobalErrors,
                               const IsoName_c& acrc_isoNameSender
                              #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
                              ,bool ab_isFastPacket
                              #endif
                              MULTITON_INST_PARAMETER_DEF_WITH_COMMA
                             );

  //! Check if this MRCW should handle the given stream
  //! @param arcc_streamIdent The to-be-investigated stream's identifier
  //! @return true -> this MRCW wants/has to handle this Stream
  bool doesAcceptStream (const ReceiveStreamIdentifier_c &arcc_streamIdent);

  void start (CanCustomer_c& apc_fpCustomer);
  void stop (CanCustomer_c& apc_fpCustomer);

public: // attributes
  CanCustomer_c* mpc_client;
  IsoName_c mc_isoName;
  uint32_t mui32_pgn;
  uint32_t mui32_pgnMask;
  uint8_t mui8_cachedClientAddress; // kinda "cached" (normally clients register for receiving multi-packages to their own SA)
  bool mb_alsoBroadcast;
  bool mb_alsoGlobalErrors;
  IsoName_c mc_isoNameSender;
#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  bool mb_isFastPacket;
#endif
};


inline
bool
MultiReceiveClientWrapper_s::doesAcceptStream (const ReceiveStreamIdentifier_c &arcc_streamIdent)
{
  return ( ((arcc_streamIdent.getPgn() & mui32_pgnMask) == mui32_pgn) // PGN matches
        && (!(arcc_streamIdent.getDa() == 0xFF) || (mb_alsoBroadcast)) // broadcast => alsoBroadcast
        && (!(arcc_streamIdent.getDa() != 0xFF) || ((arcc_streamIdent.getDa() == mui8_cachedClientAddress))) // destin-spec => addresses-match
        && (!mc_isoNameSender.isSpecified() || (mc_isoNameSender == arcc_streamIdent.getSaIsoName())) // filter for senders? => sender has to match the filter
      #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
        && ((arcc_streamIdent.getStreamType() == StreamFastPacket) == mb_isFastPacket)
      #endif
          );
}



class MultiReceive_c : public SchedulerTask_c
{
  MACRO_MULTITON_CONTRIBUTION();
public:
  enum TransferError_e {
    TransferErrorWrongCommandByteForThisPgn = 100,
    TransferErrorAlreadyRunningStream = 101,
    TransferErrorWrongPackageAmountOrMessageSize = 102,
    TransferErrorClientRejectingStream = 103,
    TransferErrorDpoForUnknownOrUnopenedStream = 104,
    TransferErrorDpoNotAwaitedNow = 105,
    TransferErrorAbortedBySender = 107,
    TransferErrorUnknownOrInvalidCommandWithTpEtpPgn = 108,
    TransferErrorWrongSequenceNumber = 109,
    TransferErrorStreamTimedOut = 110,
    TransferErrorNoStreamRunningForMultiPacketData = 111,
    TransferErrorBamToNonGlobalAddress = 112,
    TransferErrorBamNotTakenWrongPkgNumberOrMessageSize = 113,
    TransferErrorBamSequenceError = 114,
    TransferErrorPgnNotRequestedToReceive = 115,
    TransferErrorDpoForAStandardTpStream = 116,
    TransferErrorBamInBetweenAlreadyRunningStream = 117,
    TransferErrorFastPacketFrameButNoOpenStream = 118,
    TransferErrorFastpacketSequenceError = 119
  };

  ~MultiReceive_c() {}

  virtual void processMsg( const CanPkg_c& arc_data );

  /// @pre Only to be called with StreamType TP/ETP!
  bool processMsgIso (StreamType_t at_streamType, const CanPkgExt_c& arc_pkg );
#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  void processMsgNmea( const CanPkgExt_c& pkg );
#endif

  //! register a TP/ETP receiver
  //! @pre This combination (Client/IsoName/Pgn) is not yet registered
  void registerClientIso (CanCustomer_c& arc_client, const IsoName_c& acrc_isoName,
                          uint32_t aui32_pgn, uint32_t aui32_pgnMask=0x3FFFFLU,
                          bool ab_alsoBroadcast=false, bool ab_alsoGlobalErrors=false, const IsoName_c* apcc_isoNameSender=NULL);

  #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  //! register a broadcast FastPacket receiver
  //! NOTE: Destination-Specific NMEA reception is NOT YET supported!
  //! @pre This combination (Client/IsoName/Pgn) is not yet registered
  void registerClientNmea (CanCustomer_c& arc_client, const IsoName_c& acrc_isoName,
                           uint32_t aui32_pgn, uint32_t aui32_pgnMask=0x3FFFFLU,
                           bool ab_alsoBroadcast=false, bool ab_alsoGlobalErrors=false);
  #endif

  void deregisterClient (CanCustomer_c& apc_client);
  void deregisterClient (CanCustomer_c& arc_client, const IsoName_c& acrc_isoName, uint32_t aui32_pgn, uint32_t aui32_pgnMask, const IsoName_c* apcc_isoNameSender=NULL);

  //! @pre ONLY CALL THIS IF YOU KNOW THAT THERE'S NOT SUCH A STREAM ALREADY IN LIST!
  Stream_c* createStream (const ReceiveStreamIdentifier_c &arcc_streamIdent, uint32_t aui32_msgSize, int32_t ai_time );

  int32_t nextTimeEvent() const;
  int32_t getCtsDelay() const { return (getStreamCount() == 1) ? CONFIG_MULTI_RECEIVE_CTS_DELAY_AT_SINGLE_STREAM : CONFIG_MULTI_RECEIVE_CTS_DELAY_AT_MULTI_STREAMS; }

  void reactOnIsoItemModification (ControlFunctionStateHandler_c::iIsoItemAction_e /*at_action*/, IsoItem_c const& /*acrc_isoItem*/);

  /// Use to remove a "kept"-stream after it is gotten by "getFinishedJustKeptStream" and processed.
  void removeKeptStream (Stream_c* apc_keptStream);

  //  Operation: timeEvent
  void timeEvent( void );

  //  Operation: init
  void init();

  /** every subsystem of IsoAgLib has explicit function for controlled shutdown */
  void close( void );

  //! return 0x00-0xFF: first byte of the stream!
  //! return 0x100: first byte not yet known!
  //! return 0x101: not a valid index!
  uint16_t getStreamFirstByte (uint32_t ui32_index) const;
  //! ATTENTION: The meaning changed! Now only the non-kept (i.e. active) streams are counted!
  uint32_t getStreamCount() const;

  uint32_t getStreamCompletion1000 (uint32_t ui32_index, bool b_checkFirstByte=false, uint8_t ui8_returnNullIfThisIsFirstByte=0x00 /*don't care if check=false*/) const;
  uint32_t getMaxStreamCompletion1000 (bool b_checkFirstByte=false, uint8_t ui8_returnNullIfThisIsFirstByte=0x00 /*don't care if check=false*/) const;

  bool isAtLeastOneWithFirstByte(uint8_t firstByte);

  Stream_c* getFinishedJustKeptStream (Stream_c* apc_lastKeptStream);



private:
  class CanCustomerProxy_c : public CanCustomer_c {
  public:
    typedef MultiReceive_c Owner_t;

    CanCustomerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}

    virtual ~CanCustomerProxy_c() {}

  private:
    virtual void processMsg( const CanPkg_c& arc_data ) {
      mrt_owner.processMsg( arc_data );
    }

    virtual bool reactOnStreamStart(
        ReceiveStreamIdentifier_c const &ac_ident,
        uint32_t aui32_totalLen)
    {
      return mrt_owner.reactOnStreamStart(ac_ident, aui32_totalLen);
    }

    virtual void reactOnAbort(Stream_c &arc_stream)
    {
      mrt_owner.reactOnAbort(arc_stream);
    }

    virtual bool processPartStreamDataChunk(
        Stream_c &apc_stream,
        bool ab_isFirstChunk,
        bool ab_isLastChunk)
    {
      return mrt_owner.processPartStreamDataChunk(
          apc_stream,
          ab_isFirstChunk,
          ab_isLastChunk);
    }

    virtual void notificationOnMultiReceiveError(
        ReceiveStreamIdentifier_c const &ac_streamIdent,
        uint8_t aui8_multiReceiveError,
        bool ab_isGlobal)
    {
      mrt_owner.notificationOnMultiReceiveError(
          ac_streamIdent,
          aui8_multiReceiveError,
          ab_isGlobal);
    }

    // CanCustomerProxy_c shall not be copyable. Otherwise the
    // reference to the containing object would become invalid.
    CanCustomerProxy_c(CanCustomerProxy_c const &);

    CanCustomerProxy_c &operator=(CanCustomerProxy_c const &);

    Owner_t &mrt_owner;
  };
  typedef CanCustomerProxy_c Customer_t;
  class ControlFunctionStateHandlerProxy_c : public ControlFunctionStateHandler_c {
  public:
    typedef MultiReceive_c Owner_t;

    ControlFunctionStateHandlerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}

    virtual ~ControlFunctionStateHandlerProxy_c() {}

  private:
    virtual void reactOnIsoItemModification(
        iIsoItemAction_e at_action,
        IsoItem_c const &acrc_isoItem)
    {
      mrt_owner.reactOnIsoItemModification(at_action, acrc_isoItem);
    }

    // ControlFunctionStateHandlerProxy_c shall not be copyable. Otherwise the
    // reference to the containing object would become invalid.
    ControlFunctionStateHandlerProxy_c(ControlFunctionStateHandlerProxy_c const &);

    ControlFunctionStateHandlerProxy_c &operator=(ControlFunctionStateHandlerProxy_c const &);

    Owner_t &mrt_owner;
  };
  typedef ControlFunctionStateHandlerProxy_c Handler_t;

  Stream_c* getStreamIso (const ReceiveStreamIdentifier_c &arcc_streamIdent,
                          bool ab_includePgnInSearch)
  { return getStreamInternal (arcc_streamIdent, ab_includePgnInSearch); }

  Stream_c* getStreamNmea (const ReceiveStreamIdentifier_c &arcc_streamIdent)
  { return getStreamInternal (arcc_streamIdent, true); }

  Stream_c* getStreamInternal  (const ReceiveStreamIdentifier_c &arcc_streamIdent,
                                bool ab_includePgnInSearch);

  //! @return NULL for "doesn't exist", otherwise valid "CanCustomer_c*"
  CanCustomer_c* getClient (ReceiveStreamIdentifier_c ac_streamIdent);

  void sendCurrentCts(DEF_Stream_c_IMPL &arc_stream);

  bool finishStream (DEF_Stream_c_IMPL& rc_stream);

  void sendEndOfMessageAck(DEF_Stream_c_IMPL &arc_stream);


  /** @return keep it? (true=yes,keep, false=removeWhenLastChunk) */
  bool processStreamDataChunk_ofMatchingClient (Stream_c& arc_stream, bool b_lastChunk);


  void notifyErrorConnAbort (const ReceiveStreamIdentifier_c &ac_streamIdent, TransferError_e a_error, bool ab_sendConnAbort);
  void sendConnAbort (const ReceiveStreamIdentifier_c &arcc_rsi);

  //! Will inform the client via "reactOnAbort" 
  void tellClient (Stream_c &arc_stream);

  //! Will inform the client via "reactOnAbort" and close/remove the stream from list
  void tellClientRemoveStream (Stream_c &arc_stream);
  
  //! Only removes the stream from list, won't call any clients or alike
  //! Will also remove kept-streams.
  void removeStream (Stream_c &arc_stream);

  virtual bool reactOnStreamStart(
      ReceiveStreamIdentifier_c const &ac_ident,
      uint32_t aui32_totalLen)
  {
    return mt_customer.reactOnStreamStartDefault(ac_ident, aui32_totalLen);
  }

  virtual void reactOnAbort(Stream_c &arc_stream)
  {
    mt_customer.reactOnAbortDefault(arc_stream);
  }

  virtual bool processPartStreamDataChunk(
      Stream_c &apc_stream,
      bool ab_isFirstChunk,
      bool ab_isLastChunk)
  {
    return mt_customer.processPartStreamDataChunkDefault(
        apc_stream,
        ab_isFirstChunk,
        ab_isLastChunk);
  }

  virtual void notificationOnMultiReceiveError(
      ReceiveStreamIdentifier_c const &ac_streamIdent,
      uint8_t aui8_multiReceiveError,
      bool ab_isGlobal)
  {
    mt_customer.notificationOnMultiReceiveErrorDefault(
        ac_streamIdent,
        aui8_multiReceiveError,
        ab_isGlobal);
  }


private:
  static void notifyCanCustomerOfTransferError(
      CanCustomer_c &ar_canCustomer,
      const ReceiveStreamIdentifier_c &ac_streamIdent,
      TransferError_e a_transferError,
      bool ab_isGlobal) {
    ar_canCustomer.notificationOnMultiReceiveError(
        ac_streamIdent,
        uint8_t(a_transferError),
        ab_isGlobal);
  }

  // attributes
private:
  STL_NAMESPACE::list<DEF_Stream_c_IMPL> mlist_streams;
  STL_NAMESPACE::list<MultiReceiveClientWrapper_s> mlist_clients;

  Handler_t mt_handler;
  Customer_t mt_customer;
private:
  MultiReceive_c();
  friend MultiReceive_c &getMultiReceiveInstance( unsigned int instance );
};

  /** C-style function, to get access to the unique MultiReceive_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  MultiReceive_c &getMultiReceiveInstance( unsigned int instance = 0 );

} // __IsoAgLib

#endif
