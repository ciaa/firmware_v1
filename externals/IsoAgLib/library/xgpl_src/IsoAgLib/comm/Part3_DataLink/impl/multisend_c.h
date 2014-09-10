/*
  multisend_c.h: transfer of multi message data

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef MULTISEND_C_H
#define MULTISEND_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/driver/can/impl/cancustomer_c.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/scheduler/impl/schedulertask_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoname_c.h>

#include "multisendpkg_c.h"
#include "sendstream_c.h"

#include <vector>
#include <list>
#include <functional>


namespace IsoAgLib {
  class iMultiSendStreamer_c;
}


namespace __IsoAgLib {


class SendUploadBase_c
{
public:
  SendUploadBase_c () : vec_uploadBuffer() {}

  SendUploadBase_c (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, uint8_t byte9)
    { set(byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9); }

  SendUploadBase_c (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8)
    {set(byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8); }

  SendUploadBase_c (const SendUploadBase_c& r_source);

  SendUploadBase_c (uint8_t* apui8_buffer, uint32_t bufferSize)
    { set(apui8_buffer, bufferSize); }

  const SendUploadBase_c& operator= (const SendUploadBase_c& r_source);

  void set (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, uint8_t byte9);
  void set (uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8);
  void set (uint8_t* apui8_buffer, uint32_t bufferSize);

  /// Use either a MultiSendStreamer or a direct ui8-Buffer
  STL_NAMESPACE::vector<uint8_t> vec_uploadBuffer;  // don't use malloc_alloc for uint8_t values - here the 8byte overhead per malloc item are VERY big
  // ==> chunk allocation which can be shared among instances is alot better
};


/**
  This class implements the ISO 11783 (Extended) Transport Protocol
  and FastPacket for send of more than 8 bytes of data.

  @author Dipl.-Inform. Achim Spangler
  @author Dipl.-Inf. Martin Wodok
*/
class MultiSend_c : public SchedulerTask_c
{
private:
  MACRO_MULTITON_CONTRIBUTION();
  MultiSend_c();

public:
  void init();
  void close();

  virtual ~MultiSend_c() {}

#ifdef HAL_USE_SPECIFIC_FILTERS
  void reactOnIsoItemModification (ControlFunctionStateHandler_c::iIsoItemAction_e /*at_action*/, IsoItem_c const& /*acrc_isoItem*/);
#endif

  /**
    Send an ISO 11738 (E)TP targeted multipacket message using a given MultiSendStreamer
    @param apc_mss Allow active build of data stream parts for upload by deriving data source class
                   from IsoAgLib::iMultiSendStreamer_c, which defines virtual functions to control the
                   retrieve of data to send. This is especially important for the VT-Client,
                   which assembles the data pool dependent on the terminal capabilities during upload
                   (e.g. bitmap variants, color-depth, positions, etc.)
    @return true -> MultiSend_c was ready -> Transfer was started
  */
  bool sendIsoTarget (const IsoName_c& acrc_isoNameSender, const IsoName_c& acrc_isoNameReceiver, IsoAgLib::iMultiSendStreamer_c* apc_mss, int32_t ai32_pgn, MultiSendEventHandler_c* apc_multiSendEventHandler);

  /**
    Send an ISO 11738 (E)TP targeted multipacket message using a given data-buffer
    @return true -> MultiSend_c was ready -> Transfer was started
  */
  bool sendIsoTarget (const IsoName_c& acrc_isoNameSender, const IsoName_c& acrc_isoNameReceiver, const HUGE_MEM uint8_t* rhpb_data, uint32_t aui32_dataSize, int32_t ai32_pgn, MultiSendEventHandler_c* apc_multiSendEventHandler);

  /**
    Send an ISO 11783 (E)TP broadcast multipacket message using a given data-buffer
    @return true -> MultiSend_c was ready -> Transfer was started
  */
  bool sendIsoBroadcast (const IsoName_c& acrc_isoNameSender, const HUGE_MEM uint8_t* rhpb_data, uint16_t aui16_dataSize, int32_t ai32_pgn, MultiSendEventHandler_c* apc_multiSendEventHandler)
    { return sendIntern (acrc_isoNameSender, IsoName_c(), rhpb_data, aui16_dataSize, ai32_pgn, NULL /* NOT "yet" supported */, SendStream_c::IsoTPbroadcast, apc_multiSendEventHandler); }

  /**
    send an ISO 11783 (E)TP broadcast multipacket message using a given MultiSendStreamer
    @param apc_mss Allow active build of data stream parts for upload by deriving data source class
                   from IsoAgLib::iMultiSendStreamer_c, which defines virtual functions to control the
                   retrieve of data to send.
    @return true -> MultiSend_c was ready -> Transfer was started
  */
  bool sendIsoBroadcast(const IsoName_c& acrc_isoNameSender, IsoAgLib::iMultiSendStreamer_c* apc_mss, int32_t ai32_pgn, MultiSendEventHandler_c* apc_multiSendEventHandler)
    { return sendIntern(acrc_isoNameSender, IsoName_c(), NULL, apc_mss->getStreamSize(), ai32_pgn, apc_mss, SendStream_c::IsoTPbroadcast, apc_multiSendEventHandler);}


#if defined(ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
  /**
    send a FastPacket targeted multipacket message using a given uint8_t* buffer
    @param apc_mss Allow active build of data stream parts for upload by deriving data source class
                   from IsoAgLib::iMultiSendStreamer_c, which defines virtual functions to control the
                   retrieve of data to send.
    @return true -> MultiSend_c was ready -> Transfer was started
  */
  bool sendIsoFastPacket (const IsoName_c& acrc_isoNameSender, const IsoName_c& acrc_isoNameReceiver, HUGE_MEM uint8_t* rhpb_data, uint16_t aui32_dataSize, int32_t ai32_pgn, MultiSendEventHandler_c* apc_multiSendEventHandler)
    { return sendIntern (acrc_isoNameSender, acrc_isoNameReceiver, rhpb_data, aui32_dataSize, ai32_pgn, NULL, SendStream_c::NmeaFastPacket, apc_multiSendEventHandler); }

  /**
    Send a FastPacket targeted multipacket message using a given MultiSendStreamer
    @return true -> MultiSend_c was ready -> Transfer was started
  */
  bool sendIsoFastPacket (const IsoName_c& acrc_isoNameSender, const IsoName_c& acrc_isoNameReceiver, IsoAgLib::iMultiSendStreamer_c* apc_mss, int32_t ai32_pgn, MultiSendEventHandler_c* apc_multiSendEventHandler)
    { return sendIntern (acrc_isoNameSender, acrc_isoNameReceiver, NULL, apc_mss->getStreamSize(), ai32_pgn, apc_mss, SendStream_c::NmeaFastPacket, apc_multiSendEventHandler); }

  /**
    Send a FastPacket broadcast multipacket message using a given data-buffer
    @return true -> MultiSend_c was ready -> Transfer was started
  */
  bool sendIsoFastPacketBroadcast (const IsoName_c& acrc_isoNameSender, HUGE_MEM uint8_t* rhpb_data, uint16_t aui16_dataSize, int32_t ai32_pgn, MultiSendEventHandler_c* apc_multiSendEventHandler)
    { return sendIntern (acrc_isoNameSender, IsoName_c(), rhpb_data, aui16_dataSize, ai32_pgn, NULL, SendStream_c::NmeaFastPacket, apc_multiSendEventHandler); }

  /**
    Send a FastPacket broadcast multipacket message using a given MultiSendStreamer
    @param apc_mss Allow active build of data stream parts for upload by deriving data source class
                   from IsoAgLib::iMultiSendStreamer_c, which defines virtual functions to control the
                   retrieve of data to send.
    @return true -> MultiSend_c was ready -> Transfer was started
  */
  bool sendIsoFastPacketBroadcast (const IsoName_c& acrc_isoNameSender, IsoAgLib::iMultiSendStreamer_c* apc_mss, int32_t ai32_pgn, MultiSendEventHandler_c* apc_multiSendEventHandler)
    { return sendIntern (acrc_isoNameSender, IsoName_c(), NULL, apc_mss->getStreamSize(), ai32_pgn, apc_mss, SendStream_c::NmeaFastPacket, apc_multiSendEventHandler); }
#endif

  /**
    set the delay between two sent messages, called by SendStream_c's timeEvent() when neccessary
    @param aui16_delay wanted delay
  */
  void setSendStreamDelay(uint16_t aui16_delay);


  /**
    start processing of a process msg: delegate to specific SendStream_c instance
  */
  void processMsg( const CanPkg_c& arc_data );

  /**
    perform periodical actions
    @return true -> all planned activities performed in allowed time
  */
  void timeEvent();

  /** user function for explicit abort of any running matching stream. */
  void abortSend(
    const IsoName_c& acrc_isoNameSender,
    const IsoName_c& acrc_isoNameReceiver,
    ConnectionAbortReason_t reason = ConnectionAbortReasonAnyOtherError );

  void abortSend(
    const MultiSendEventHandler_c& apc_multiSendEventHandler,
    ConnectionAbortReason_t reason = ConnectionAbortReasonAnyOtherError );

  /** check if at least one multisend stream is running */
  bool isMultiSendRunning() const { return (!mlist_sendStream.empty()); }

protected:


private:
  class CanCustomerProxy_c : public CanCustomer_c {
  public:
    typedef MultiSend_c Owner_t;
    CanCustomerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}
    virtual ~CanCustomerProxy_c() {}

  private:
    virtual void processMsg( const CanPkg_c& data )
    { mrt_owner.processMsg( data ); }

    // CanCustomerProxy_c shall not be copyable. Otherwise the
    // reference to the containing object would become invalid.
    CanCustomerProxy_c(CanCustomerProxy_c const &);
    CanCustomerProxy_c &operator=(CanCustomerProxy_c const &);

    Owner_t &mrt_owner;
  };
  typedef CanCustomerProxy_c Customer_t;

#ifdef HAL_USE_SPECIFIC_FILTERS
  class ControlFunctionStateHandlerProxy_c : public ControlFunctionStateHandler_c {
  public:
    typedef MultiSend_c Owner_t;
    ControlFunctionStateHandlerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}
    virtual ~ControlFunctionStateHandlerProxy_c() {}

  private:
    virtual void reactOnIsoItemModification( iIsoItemAction_e action, IsoItem_c const &isoItem )
    { mrt_owner.reactOnIsoItemModification( action, isoItem ); }

    // ControlFunctionStateHandlerProxy_c shall not be copyable. Otherwise the
    // reference to the containing object would become invalid.
    ControlFunctionStateHandlerProxy_c(ControlFunctionStateHandlerProxy_c const &);
    ControlFunctionStateHandlerProxy_c &operator=(ControlFunctionStateHandlerProxy_c const &);

    Owner_t &mrt_owner;
  };
  typedef ControlFunctionStateHandlerProxy_c Handler_t;
#endif

  /**
    use this function to add a new SendStream. (doesn't use existing-finished ones anymore!)
    IMPORTANT: Assure that the added SendStream is initialized right after this call!!
    @return reference to added SendStream ==> HAS TO BE INITIALIZED
  */
  SendStream_c* addSendStream( const IsoName_c& sender, const IsoName_c& receiver );

  /** @return "Running" stream or NULL if none active for this sa/da-key (skips finished streams!) */
  SendStream_c* getRunningStream (const IsoName_c& sender, const IsoName_c& receiver );

  void calcAndSetNextTriggerTime();

  bool sendIntern (const IsoName_c& sender,
                   const IsoName_c& receiver,
                   const HUGE_MEM uint8_t* data,
                   uint32_t dataSize,
                   int32_t pgn,
                   IsoAgLib::iMultiSendStreamer_c* mss,
                   SendStream_c::msgType_t msgType,
                   MultiSendEventHandler_c* multiSendEventHandler);

  #if defined(ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
  uint8_t allocFpSequenceCounter() { const uint8_t cui8_returnVal = mui8_nextFpSequenceCounter;
                                     mui8_nextFpSequenceCounter = (mui8_nextFpSequenceCounter+1) & 0x7;
                                     return cui8_returnVal; }
  #endif


  static SendStream_c::msgType_t protocolTypeByPacketSize(uint32_t ui32_size);

private:
  enum {
    beginSinglePacketSize = 0,
    endSinglePacketSize = 9,
    endNmeaFastPacketSize = 224,
    beginTpPacketSize = endSinglePacketSize,
    endTpPacketSize = 1786,
    beginEtpPacketSize = endTpPacketSize,
  };
  // Private attributes
  #if defined(ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
  uint8_t mui8_nextFpSequenceCounter;
  #endif

  STL_NAMESPACE::list<SendStream_c> mlist_sendStream;
  Customer_t mt_customer;
#ifdef HAL_USE_SPECIFIC_FILTERS
  Handler_t mt_handler;
#endif

  friend MultiSend_c &getMultiSendInstance( unsigned int instance );
  friend class SendStream_c;
};


inline
SendStream_c::msgType_t
MultiSend_c::protocolTypeByPacketSize(uint32_t ui32_size)
{
  return ui32_size >= beginEtpPacketSize ? SendStream_c::IsoETP : SendStream_c::IsoTP;
}


inline
bool
MultiSend_c::sendIsoTarget(const IsoName_c& acrc_isoNameSender, const IsoName_c& acrc_isoNameReceiver, IsoAgLib::iMultiSendStreamer_c* apc_mss, int32_t ai32_pgn, MultiSendEventHandler_c* apc_multiSendEventHandler)
{
  return sendIntern(acrc_isoNameSender, acrc_isoNameReceiver, NULL, apc_mss->getStreamSize(), ai32_pgn, apc_mss, protocolTypeByPacketSize(apc_mss->getStreamSize()),apc_multiSendEventHandler);
}


inline
bool 
MultiSend_c::sendIsoTarget (const IsoName_c& acrc_isoNameSender, const IsoName_c& acrc_isoNameReceiver, const HUGE_MEM uint8_t* rhpb_data, uint32_t aui32_dataSize, int32_t ai32_pgn, MultiSendEventHandler_c* apc_multiSendEventHandler)
{
  return sendIntern(acrc_isoNameSender, acrc_isoNameReceiver, rhpb_data, aui32_dataSize, ai32_pgn, NULL, protocolTypeByPacketSize(aui32_dataSize),apc_multiSendEventHandler);
}


MultiSend_c &
getMultiSendInstance( unsigned int instance = 0 );


} // __IsoAgLib

#endif
