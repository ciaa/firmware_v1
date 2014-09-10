/*
  multireceive_c.cpp: Implementation of ISO 11783-Part 3
    (Extended-)Transport-Protocol and NMEA 2000 Fast-Packet Protocol

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "multireceive_c.h"

// IsoAgLib
#include <IsoAgLib/scheduler/impl/scheduler_c.h>
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#ifdef HAL_USE_SPECIFIC_FILTERS
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isofiltermanager_c.h>
#endif
#include <IsoAgLib/util/iassert.h>

#if DEBUG_MULTIRECEIVE
  #ifdef SYSTEM_PC
    #include <iostream>
  #else
    #include <supplementary_driver/driver/rs232/impl/rs232io_c.h>
  #endif
#endif

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif

// helper macros
#define MACRO_pgnFormatOfPGN(mpPgn)     ((mpPgn>>8) & 0xFF)
#define MACRO_BYTEORDER_toLoMidHi(a)    uint8_t((a) & 0xFF), uint8_t(((a) >> 8) & 0xFF), uint8_t(((a) >> 16) & 0xFF)
#define MACRO_BYTEORDER_toLoMidMidHi(a) uint8_t((a) & 0xFF), uint8_t(((a) >> 8) & 0xFF), uint8_t(((a) >> 16) & 0xFF), uint8_t(((a) >> 24) & 0xFF)
#define MACRO_BYTEORDER_toLoHi(a)       uint8_t((a) & 0xFF), uint8_t(((a) >> 8) & 0xFF)



// Begin Namespace __IsoAgLib
namespace __IsoAgLib {



// helper consts
static const uint8_t scui8_tpPriority=6;


  /** C-style function, to get access to the unique MultiReceive_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  MultiReceive_c &getMultiReceiveInstance( unsigned int instance )
  {
    MACRO_MULTITON_GET_INSTANCE_BODY(MultiReceive_c, PRT_INSTANCE_CNT, instance);
  }



MultiReceiveClientWrapper_s::MultiReceiveClientWrapper_s(
    CanCustomer_c& arc_client,
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
  ) 
  : MULTITON_PARENT_CONSTRUCTOR
    mpc_client(&arc_client)
  , mc_isoName (acrc_isoNameClient)
  , mui32_pgn(aui32_pgn)
  , mui32_pgnMask(aui32_pgnMask)
  , mui8_cachedClientAddress(0xFE)
  , mb_alsoBroadcast (ab_alsoBroadcast)
  , mb_alsoGlobalErrors (ab_alsoGlobalErrors)
  , mc_isoNameSender (acrc_isoNameSender)
  #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  , mb_isFastPacket (ab_isFastPacket) // means the PGN has to be "insertFilter"/"removeFilter"ed
  #endif
{
}


void
MultiReceiveClientWrapper_s::start (CanCustomer_c& apc_fpCustomer)
{
  IsoItem_c *item = __IsoAgLib::getIsoMonitorInstance4Comm().item( mc_isoName, true ); // it needs to have claimed an address
  if( item != NULL )
    mui8_cachedClientAddress = item->nr();
  // else: With the ControlFunctionStateHandler, it's fine if the address is set/changed later

  #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  if (mb_isFastPacket)
  { /// Fast-Packet additions
    if (!getIsoBusInstance4Comm().existFilter (apc_fpCustomer, IsoAgLib::iMaskFilter_c( ( mui32_pgnMask << 8), (mui32_pgn << 8 ))))
      getIsoBusInstance4Comm().insertFilter (apc_fpCustomer, IsoAgLib::iMaskFilter_c( (mui32_pgnMask << 8), (mui32_pgn << 8 ) ), 8 );
  }
  #else
  (void)apc_fpCustomer;
  #endif
}


void
MultiReceiveClientWrapper_s::stop (CanCustomer_c& apc_fpCustomer)
{
  #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  if (mb_isFastPacket)
  { /// Fast-Packet additions
    __IsoAgLib::getIsoBusInstance4Comm().deleteFilter (apc_fpCustomer, IsoAgLib::iMaskFilter_c( (mui32_pgnMask << 8), (mui32_pgn << 8) ) );
  }
  #else
  (void)apc_fpCustomer;
  #endif
}


// c'tor
MultiReceive_c::MultiReceive_c()
  : SchedulerTask_c( 100, true )
  , mlist_streams()
  , mlist_clients()
  , mt_handler(*this)
  , mt_customer(*this)
{
}


//! @todo Optimize that: If noone ever wants notifications, only CPU time is wasted!!!
void
MultiReceive_c::notifyErrorConnAbort(
  const ReceiveStreamIdentifier_c &acrc_streamIdent, 
  MultiReceive_c::TransferError_e a_transferErrorCode,
  bool ab_sendConnAbort)
{
  /// Handle ConnectionAbort
  if (ab_sendConnAbort)
  {
    sendConnAbort (acrc_streamIdent);
  }

  /// Handle Notification
  if (acrc_streamIdent.getDa() == 0xFF)
  { // BAM
    for (STL_NAMESPACE::list<MultiReceiveClientWrapper_s>::iterator i_list_clients = mlist_clients.begin(); i_list_clients != mlist_clients.end(); ++i_list_clients)
    { // // inform all clients that want Broadcast-TP-Messages
      MultiReceiveClientWrapper_s& curClientWrapper = *i_list_clients;
      if (curClientWrapper.mb_alsoBroadcast) {
          notifyCanCustomerOfTransferError(
              *curClientWrapper.mpc_client,
              acrc_streamIdent,
              a_transferErrorCode,
              false);
      }
    }
  }
  else
  { // really destin specific
    if (getClient (acrc_streamIdent))
    {
      notifyCanCustomerOfTransferError(
          *getClient(acrc_streamIdent),
          acrc_streamIdent,
          a_transferErrorCode,
          false);
    }
    else
    {
      // global notify for clients who want notification on global errors (which noone else can take ;-))
      for (STL_NAMESPACE::list<MultiReceiveClientWrapper_s>::iterator i_list_clients = mlist_clients.begin(); i_list_clients != mlist_clients.end(); ++i_list_clients)
      { // // inform all clients that want Broadcast-TP-Messages
        MultiReceiveClientWrapper_s& curClientWrapper = *i_list_clients;
        if (curClientWrapper.mb_alsoGlobalErrors) {
          notifyCanCustomerOfTransferError(
              *curClientWrapper.mpc_client,
              acrc_streamIdent,
              a_transferErrorCode,
              true);
        }
      }
    }
  }
}


void
MultiReceive_c::processMsg( const CanPkg_c& arc_data )
{
  CanPkgExt_c pkg( arc_data, getMultitonInst() );
  if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
    return;

  const uint8_t cui8_pgnFormat = pkg.isoPf();

#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  /// Setup TP/ETP/FP identification
  const StreamType_t ct_streamType = (  (cui8_pgnFormat == MACRO_pgnFormatOfPGN(ETP_CONN_MANAGE_PGN))
                                     || (cui8_pgnFormat == MACRO_pgnFormatOfPGN(ETP_DATA_TRANSFER_PGN)) )
                                     ? StreamETP
                                     : (  (cui8_pgnFormat == MACRO_pgnFormatOfPGN(TP_CONN_MANAGE_PGN))
                                       || (cui8_pgnFormat == MACRO_pgnFormatOfPGN(TP_DATA_TRANSFER_PGN)) )
                                       ? StreamTP
                                       : StreamFastPacket;

  switch (ct_streamType)
  {
    case StreamTP:
    case StreamETP:
      processMsgIso (ct_streamType, pkg ); // bidirectional PGN, so retval is important
      break;
    case StreamFastPacket:
      processMsgNmea( pkg );
      break;
  }
#else
  /// Setup TP/ETP identification
  const StreamType_t ct_streamType = (  (cui8_pgnFormat == MACRO_pgnFormatOfPGN(ETP_CONN_MANAGE_PGN))
                                     || (cui8_pgnFormat == MACRO_pgnFormatOfPGN(ETP_DATA_TRANSFER_PGN)) )
                                     ? StreamETP
                                     : StreamTP;

  processMsgIso (ct_streamType, pkg ); // bidirectional PGN, so retval is important
#endif

  setNextTriggerTime( nextTimeEvent() );
}


bool
MultiReceive_c::processMsgIso (StreamType_t at_streamType, const CanPkgExt_c& arc_pkg )
{
  isoaglib_assert ((at_streamType == StreamETP) || (at_streamType == StreamTP));

  const uint8_t cui8_pgnFormat = arc_pkg.isoPf();
  const uint8_t cui8_dataByte0 = arc_pkg.getUint8Data(0);

  // Note: The RSI's PGN is not valid for DT packets!!
  ReceiveStreamIdentifier_c c_isoRSI(
    at_streamType,
    (uint32_t (arc_pkg.getUint8Data(5)) | (uint32_t (arc_pkg.getUint8Data(6)) << 8) | (uint32_t (arc_pkg.getUint8Data(7)) << 16)),
    arc_pkg.isoPs(),
    arc_pkg.getISONameForDA(),
    arc_pkg.isoSa(),
    arc_pkg.getISONameForSA());

  bool b_eCmd=false;

  switch (cui8_pgnFormat)
  {
    case MACRO_pgnFormatOfPGN (ETP_CONN_MANAGE_PGN):
    case MACRO_pgnFormatOfPGN (TP_CONN_MANAGE_PGN):
    { // to allow local variables
    #if DEBUG_MULTIRECEIVE
      INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "{CM: " << arc_pkg.time() << "} ";
    #endif
      switch (cui8_dataByte0)
      {
        case 0x14: // decimal 20
          b_eCmd=true; // break left out intentionally

        case 0x10: // decimal 16
          { // to allow local variables!
            /////////////////////////
            /// RTS (Request To Send)
            if (b_eCmd != (at_streamType == StreamETP))
            { /* this type is invalid - using Extended commands on Standard-TP PGN (or vice versa) */
              notifyErrorConnAbort (c_isoRSI, TransferErrorWrongCommandByteForThisPgn, true);
              return true; /* no other CAN-Customer should be interested in that one */
            }

            // RTS from an SA that has already a Stream running? // 
            Stream_c* pc_streamFound = getStreamIso (c_isoRSI, /* include PGN in search? */ false);

            if (pc_streamFound != NULL)
            { // abort, already running stream (on any PGN) is interrupted by RTS
              notifyErrorConnAbort (c_isoRSI, TransferErrorAlreadyRunningStream, true);
              // actually questionable: ConnAbort with what PGN?
              // --> The interrupted Stream's, or the interrupting Stream's?
              tellClientRemoveStream (*pc_streamFound);
              #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** ConnectionAbort due to Already-Running-Stream! (RTS in between) ***" << (int)c_isoRSI.getSa() << "->" << (int)c_isoRSI.getDa() << INTERNAL_DEBUG_DEVICE_ENDL;
              #endif
              return true; // all RTSes are not of interest for MultiSend or other CAN-Customers!
            }
            // Maybe close old stream and open a new one nevertheless right here, right now?
            // --> Doesn't seem like a good solution, as we couldn't close the old connection
            // --> because therefore we had to send a ConnAbort, but this would seem like not
            // --> accepting the RTS, so it seems to be the only solution to just ConnAbort
            // --> the current connection. Well, the sender would retry the RTS then anyway...

            // Otherwise it is a new stream, but before check from the client if he can take it (size is okay)

            // Get Message Size
            uint32_t ui32_msgSize = (c_isoRSI.isIsoEtp())
                                      ? arc_pkg.getUint32Data(1)
                                      : arc_pkg.getUint16Data(1);

            // Calculate Number of Packets (only for TP-#Pkg-Check!)
            const uint32_t cui32_numPkg = (ui32_msgSize + 6) / 7;
            if (  (c_isoRSI.isIsoTp() && ((arc_pkg.getUint8Data(3) != cui32_numPkg) || (ui32_msgSize < 9)))
               || (c_isoRSI.isIsoEtp() && (ui32_msgSize < 1786))  )
            { // This handles both
              notifyErrorConnAbort (c_isoRSI, TransferErrorWrongPackageAmountOrMessageSize, true);
              #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** ConnectionAbort due to (Wrong Pkg Amount || msgSize < 9) ***" << INTERNAL_DEBUG_DEVICE_ENDL;
              #endif
              return true; // all RTSes are not of interest for MultiSend or other CAN-Customers!
            }

            const uint8_t cui8_maxPktsPerBurst = (c_isoRSI.isIsoEtp())
                                                   ? 0xFF // not specifyable in ETP
                                                   : arc_pkg.getUint8Data(4);
            if( 0 == cui8_maxPktsPerBurst )
            {
              notifyErrorConnAbort (c_isoRSI, TransferErrorWrongPackageAmountOrMessageSize, true);
              #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** ConnectionAbort due to invalid max pkt range ***" << INTERNAL_DEBUG_DEVICE_ENDL;
              #endif
              return true; // all RTSes are not of interest for MultiSend or other CAN-Customers!
            }

            // First of all, is there a client registered that handles those PGNs via (E)TP-Messages?
            CanCustomer_c* pc_clientFound = getClient (c_isoRSI);
            if (pc_clientFound == NULL)
            { // There's no client registered to take this PGN->thisAddress! */
              notifyErrorConnAbort (c_isoRSI, TransferErrorPgnNotRequestedToReceive, true);
              #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** ConnectionAbort due to PGN requested that the MR-Client has not registered to receive ***" << INTERNAL_DEBUG_DEVICE_ENDL;
              #endif
              return true; // all RTSes are not of interest for MultiSend or other CAN-Customers!
            }

            // Send the Request To Send (RTS) to the client - Does he give us a Clear To Send (CTS) ?
            if (!pc_clientFound->reactOnStreamStart (c_isoRSI, ui32_msgSize)) {
              // Client rejects this stream!
              notifyErrorConnAbort (c_isoRSI, TransferErrorClientRejectingStream, true);
              #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** ConnectionAbort due to Client Rejecting the stream ***" << INTERNAL_DEBUG_DEVICE_ENDL;
              #endif
              return true; // all RTSes are not of interest for MultiSend or other CAN-Customers!
            }
            // else: Client accepts this stream, so create a representation of the stream NOW -
            // - further handling is done in "timeEvent()" now!*/
            Stream_c* newStream = createStream (c_isoRSI, ui32_msgSize, arc_pkg.time() );
            newStream->setTPBurstLimit( cui8_maxPktsPerBurst );

            // the constructor above sets the Stream to "AwaitCtsSend" and "StreamRunning"
            sendCurrentCts ((DEF_Stream_c_IMPL&)*newStream); // send out the initial CTS
            #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << "Send CTS to get initial burst!" << INTERNAL_DEBUG_DEVICE_ENDL;
            #endif
          } // end local variables allowment!
          return true; // all RTSes are not of interest for MultiSend or other CAN-Customers!


        case 0x16: // decimal 22
          { // to allow local variables!
            ////////////////////////////
            /// DPO (Data Packet Offset)
            Stream_c* pc_streamFound = getStreamIso (c_isoRSI, /* include PGN in search? */ true);

            if (pc_streamFound == NULL)
            {
              notifyErrorConnAbort (c_isoRSI, TransferErrorDpoForUnknownOrUnopenedStream, true); // according to Brad: ConnAbort
              #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << INTERNAL_DEBUG_DEVICE_NEWLINE << "DPO for an unknown/unopened stream!!" << INTERNAL_DEBUG_DEVICE_ENDL ;
              #endif
              return true; // all DPOs are not of interest for MultiSend or other CAN-Customers!
            }

            if (!c_isoRSI.isIsoEtp())
            { // DPO although we're not on ETP
              notifyErrorConnAbort (c_isoRSI, TransferErrorDpoForAStandardTpStream, true); // according to Brad: ConnAbort
              tellClientRemoveStream (*pc_streamFound);
              #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << INTERNAL_DEBUG_DEVICE_NEWLINE << "DPO for a Standard-TP stream!!" << INTERNAL_DEBUG_DEVICE_ENDL;
              #endif
              return true; // all DPOs are not of interest for MultiSend or other CAN-Customers!
            }

            // try to set the DPO in this stream - if it's not allowed right now (or timedout), this DPO was not correct
            if (! (pc_streamFound->setDataPageOffset ( uint32_t(arc_pkg.getUint8Data(2)) |
                                                      (uint32_t(arc_pkg.getUint8Data(3)) << 8) |
                                                      (uint32_t(arc_pkg.getUint8Data(4)) << 16))))
            { // DPO not awaited now!
              notifyErrorConnAbort (c_isoRSI, TransferErrorDpoNotAwaitedNow, true);
              tellClientRemoveStream (*pc_streamFound);
              #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** ConnectionAbort due to DPO at wrong 'mt_awaitStep' - was NOT AwaitDpo ***" << INTERNAL_DEBUG_DEVICE_ENDL;
              #endif
              return true; // all DPOs are not of interest for MultiSend or other CAN-Customers!
            }
          } // end local variables allowment!
          return true; // all DPOs are not of interest for MultiSend or other CAN-Customers!




        case 0x20: // decimal 32
          { // to allow local variables!
            ////////////////////////////////////
            /// BAM (Broadcast Announce Message)
            if (at_streamType == StreamETP)
            { /* this type is invalid - using BAM on Extended-PGN */
              notifyErrorConnAbort (c_isoRSI, TransferErrorWrongCommandByteForThisPgn, false);
              // don't send ConnAbort, just ignore
              return true; /* no other CAN-Customer should be interested in that one */
            }

            // Is BAM directed to 0xFF (global) ?
            if (c_isoRSI.getDa() != 0xFF)
            { // we do NOT take BAMs that are NOT directed to the GLOBAL (255) address
              notifyErrorConnAbort (c_isoRSI, TransferErrorBamToNonGlobalAddress, false);
              #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** BAM to NON-GLOBAL address "<< (uint16_t) c_isoRSI.getDa() <<" ***" << INTERNAL_DEBUG_DEVICE_ENDL;
              #endif
              return true; // all BAMs are not of interest for MultiSend or other CAN-Customers!
            }

            // From now on it is assured that BAM is directed to 0xFF (255)

            // BAM from an SA that has already a Stream running?
            Stream_c* pc_streamFound = getStreamIso (c_isoRSI, /* also compare PGN? */ false);

            if (pc_streamFound != NULL) {
              // abort already running stream is interrupted by BAM
              notifyErrorConnAbort (c_isoRSI, TransferErrorBamInBetweenAlreadyRunningStream, false); /* do NOT send ConnAbort msg */
              tellClientRemoveStream (*pc_streamFound);
              #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** ConnectionAbort due to Already-Running-Stream! (BAM in between) ***" << INTERNAL_DEBUG_DEVICE_ENDL;
              #endif
              // ^^^ do NOT return, if the old BAM is "aborted" due to this BAM, try with this BAM now...
              // we can do this here, because we do not send any kind of ConnAbort, so we can simply continue...
            }

            const uint32_t cui32_msgSize = arc_pkg.getUint16Data(1);
            const uint32_t cui32_numPkg = (cui32_msgSize + 6) / 7;
            // check for TP-RTS if pkg-count matches the calculated AND if size > 0
            if ((arc_pkg.getUint8Data(3) != cui32_numPkg) || (cui32_msgSize < 9))
            { // This handles both
              notifyErrorConnAbort (c_isoRSI, TransferErrorBamNotTakenWrongPkgNumberOrMessageSize, false);
              #if DEBUG_MULTIRECEIVE
              INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** BAM not taken due to (Wrong Pkg Number || msgSize < 9) ***" << INTERNAL_DEBUG_DEVICE_ENDL;
              #endif
              return true; // all RTSes are not of interest for MultiSend or other CAN-Customers!
            }

            // NO client checks as in RTS-case above, as it's for ALL clients, they HAVE to take it =)

            // "Stream_c"'s constructor will set awaitStep to "awaitData" and timeOut to 250ms!
            createStream (c_isoRSI, cui32_msgSize, arc_pkg.time() );
          }
          return true; // all BAMs are not of interest for MultiSend or other CAN-Customers!


        case 0xFF: // decimal 255
          { // to allow local variables!
            ////////////////////////////////
            /// ConnAbort (Connection Abort) [same for TP and ETP]

            // also allow a BAM to be aborted by the sender himself...
            Stream_c* pc_streamFound = getStreamIso (c_isoRSI, /* also compare PGN? */ true);

            if (pc_streamFound)
            {
              notifyErrorConnAbort (c_isoRSI, TransferErrorAbortedBySender, false /* do not send connAbort-Msg */);
              tellClientRemoveStream (*pc_streamFound);
            }
            else
            { // For now, ignore an incoming ConnAbort for an unopened/unknown stream!
              // The case may be with a ConnAbort with PGN X while a (E)TP with
              // PGN Y is running. Just ignore to avoid ConnAbort-PingPong!
            }
          } // end local variables allowment!
          return false; // ConnAbort is also of interest for MultiSend!!



        case 0x15: // decimal 21 ETP.CM_CTS  --> this is only sent by receiver of a multi send
        case 0x17: // decimal 23 ETP.CM_EOMA --> this is only sent by receiver of a multi send
        case 0x11: // decimal 17 TP.CM_CTS
        case 0x13: // decimal 19 TP.CM_EndOfMsgACK
          ////////////////////////
          /// CTS (for MultiSend)
          // ignore here and simply pass on to MultiSend (using return false)
          return false;

        default:
          #if DEBUG_MULTIRECEIVE
            INTERNAL_DEBUG_DEVICE << "UNKNOWN/INVALID command with (E)TP-PGN... not passing this on to MultiSend... just discard!!" << INTERNAL_DEBUG_DEVICE_ENDL;
          #endif
          notifyErrorConnAbort (c_isoRSI, TransferErrorUnknownOrInvalidCommandWithTpEtpPgn, false);
          // do not send a ConnAbort, because we don't know if Byte 6-8
          // are the PGN that we need for ConnAbort in Byte 6-8!
          return true;
      } // switch (ui8_dataByte)
    } // end allowment of local variables for ConnManagement PGN
    // no "break;" needed, all "case"s do return!


    case MACRO_pgnFormatOfPGN(ETP_DATA_TRANSFER_PGN):
    case MACRO_pgnFormatOfPGN(TP_DATA_TRANSFER_PGN):
      #if DEBUG_MULTIRECEIVE
        INTERNAL_DEBUG_DEVICE << "{DATA: " << arc_pkg.time() << "} "; INTERNAL_DEBUG_FLUSH
      #endif

      /////////////////////////
      /// Data Transfer (DATA)
      { // to allow local variables!
        // Check if there's already a Stream active from this SA->DA pair (it should ;-)
        // --> Don't compare the PGN, as there's no PGN embedded in DT messages...
        //     The PGN in the RSI is invalid!
        Stream_c* pc_streamFound = getStreamIso (c_isoRSI, /* also compare PGN?: */ false);

        if (pc_streamFound == NULL) {
          // There's no stream running for this multi-packet-DATA!, this [DATA] MAY BE for MultiSend, so simply return false!
          #if DEBUG_MULTIRECEIVE
          INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** (E)TP.DATA, but no open stream! ignoring that... ***" << INTERNAL_DEBUG_DEVICE_ENDL;
          #endif
          notifyErrorConnAbort (c_isoRSI, TransferErrorNoStreamRunningForMultiPacketData, false);
          // currently don't send a ConnAbort when unsolicited DTs come in...
          return false;
        }

        // From this point on the SA/DA pair matches, so that we can return true
        if (!(pc_streamFound->handleDataPacket (arc_pkg)))
		{
          // Stream was not in state of receiving DATA right now, connection abort, inform Client and close Stream!
          const ReceiveStreamIdentifier_c& c_streamRsi = pc_streamFound->getIdent();
          if (c_streamRsi.getDa() == 0xFF)
          {
            notifyErrorConnAbort (c_streamRsi, TransferErrorBamSequenceError, false /* don't send connAbort-Msg */);
            #if DEBUG_MULTIRECEIVE
            INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** BAM sequence error ***" << INTERNAL_DEBUG_DEVICE_ENDL;
            #endif
          } else {
            notifyErrorConnAbort (c_streamRsi, TransferErrorWrongSequenceNumber, true /* send connAbort-Msg */);
            #if DEBUG_MULTIRECEIVE
            INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** ConnectionAbort due to (E)TP.DATA, but wrong sequence number, see msg before! ***" << INTERNAL_DEBUG_DEVICE_ENDL;
            #endif
          }
          tellClientRemoveStream (*pc_streamFound); // won't be told to Broadcast streams anyway...
        }
        else
        {
          if (pc_streamFound->getStreamingState() == StreamFinished)
          {
            const bool keepStream = finishStream ((DEF_Stream_c_IMPL&)*pc_streamFound); // Send EoMAck
            if (!keepStream)
              removeStream (*pc_streamFound);
          }
          // else: Stream not yet finished, set to "AwaitCtsSend" to expect next burst...
        }

        // if code execution comes to here, then the right SA/DA Pair was
        // there so it WAS for MultiReceive, so we can return true safely!
      } // end local variables allowment!
      return true; // DT in this direction is never of interest for MultiSend...

    default:
      isoaglib_assert (!"Shouldn't reach here, must have fallen into the cases above for TP/ETP CM/DT");
      return false;
  } // switch (PF)
}


#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
/// @pre Only to be called with StreamType FP!
void
MultiReceive_c::processMsgNmea( const CanPkgExt_c& pkg )
{
  const uint8_t cui8_dataByte0 = pkg.getUint8Data(0);

  /** Note:
    * FP can also be destination specific!
    * This is not completely handled here!
    * Also registerClient doesn't handle it yet!
    */
  ReceiveStreamIdentifier_c c_fpRSI(
    StreamFastPacket,
    pkg.isoPurePgn(),
    pkg.hasDa() ? pkg.isoPs()           : uint8_t(0xFFu),
    pkg.hasDa() ? pkg.getISONameForDA() : IsoName_c::IsoNameUnspecified(),
    pkg.isoSa(),
    pkg.getISONameForSA());

  Stream_c* pc_streamFound = getStreamNmea (c_fpRSI);
  if (!pc_streamFound)
  { // stream not there. create a new one if it's the first frame
    const uint8_t cui8_counterFrame = cui8_dataByte0 & 0x1F;
    /** @todo SOON-178 check for matching sequence counter!!! add seq-cnt to StreamFastPacket-class. */

    if (cui8_counterFrame == 0)
    { // First Frame => okay, create new Stream!
      // Check if it's registered for fast-packet receive
      for (STL_NAMESPACE::list<MultiReceiveClientWrapper_s>::iterator pc_iter = mlist_clients.begin();
            pc_iter != mlist_clients.end();
            ++pc_iter)
      { // is it fast-packet, and is it this pgn?
        if ( (pc_iter->mb_isFastPacket) &&
             (pc_iter->mui32_pgn == (c_fpRSI.getPgn() & pc_iter->mui32_pgnMask)) /*  &&
             /// right now destin-specific FastPackets are NOT supported...
             ( (pc_iter->mb_alsoBroadcast && c_fpRSI.getDaIsoName().isUnspecified()) || // need to check if it's a destin-spec PGN, only then "alsoBroadcast" is of interest
               (pc_iter->mc_isoName == c_fpRSI.getDaIsoName())
             ) */
             )
        {
          pc_streamFound = createStream (c_fpRSI, pkg.getUint8Data (1), pkg.time() );
          break; // enough if at least one client wants this message...
        }
      }
      if (!pc_streamFound)
      { // no-one was found that's interested in handling this FP-Message
        // so it's fine to just ignore this message.
        return;
      }
    }
    else
    { // else: no stream open and wrong packeted number (non-start "0") comes in.
      notifyErrorConnAbort (c_fpRSI, TransferErrorFastPacketFrameButNoOpenStream, false);
      #if DEBUG_MULTIRECEIVE
      INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** FastPacket-Frame "<<(uint16_t)cui8_counterFrame<<", but no open stream! ignoring that... ***" << INTERNAL_DEBUG_DEVICE_ENDL;
      #endif
      return;
    }
    // stream was maybe created up to now.
  } // else: stream definitely there, so pass on data, just like for first frame

  /// Now we have either
  /// - had the stream already open
  /// - or just created the stream
  isoaglib_assert (pc_streamFound);
  if (!(pc_streamFound->handleDataPacket (pkg)))
  { // Wrong packet sequence number!
    notifyErrorConnAbort (c_fpRSI, TransferErrorFastpacketSequenceError, false);
    /// Do NOT tell client on Abort of something it doesn't know about.
    /// (there's no on-the-fly processing of FastPacket!)
    removeStream (*pc_streamFound);
    #if DEBUG_MULTIRECEIVE
    INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** FastPacket sequence error - removing stream! ***" << INTERNAL_DEBUG_DEVICE_ENDL;
    #endif
  }
  else
  {
    if (pc_streamFound->getStreamingState() == StreamFinished)
    {
      const bool keepStream = finishStream ((DEF_Stream_c_IMPL&)*pc_streamFound);
      if (!keepStream)
        removeStream (*pc_streamFound);
    }
    // else: Stream not yet finished, set wait on for more fast packets...
  }
}
#endif


void
MultiReceive_c::registerClientIso(
  CanCustomer_c& arc_client, const IsoName_c& acrc_isoName,
  uint32_t aui32_pgn, uint32_t aui32_pgnMask,
  bool ab_alsoBroadcast, bool ab_alsoGlobalErrors,
  const IsoName_c* apcc_isoNameSender)
{
  IsoName_c isoNameSender;
  if (apcc_isoNameSender) isoNameSender = *apcc_isoNameSender;
  else /* no name ---- */ isoNameSender.setUnspecified();

  mlist_clients.push_back(
    MultiReceiveClientWrapper_s(
      arc_client,
      acrc_isoName,
      aui32_pgn,
      aui32_pgnMask,
      ab_alsoBroadcast,
      ab_alsoGlobalErrors,
      isoNameSender
      #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
      , false
      #endif
      MULTITON_INST_WITH_COMMA));

  mlist_clients.back().start (mt_customer);
}


#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
void
MultiReceive_c::registerClientNmea (CanCustomer_c& arc_client, const IsoName_c& acrc_isoName,
                                    uint32_t aui32_pgn, uint32_t aui32_pgnMask,
                                    bool ab_alsoBroadcast, bool ab_alsoGlobalErrors)
{
  IsoName_c isoNameSender;
#if 0
  if (apcc_isoNameSender) isoNameSender = *apcc_isoNameSender;
  else /* no name ---- */ isoNameSender.setUnspecified();
#endif
  // currently filtering on the sender is not supported for NMEA FP!
  isoNameSender.setUnspecified();

  mlist_clients.push_back(
    MultiReceiveClientWrapper_s(
      arc_client,
      acrc_isoName,
      aui32_pgn,
      aui32_pgnMask,
      ab_alsoBroadcast,
      ab_alsoGlobalErrors,
      isoNameSender,
      true
      MULTITON_INST_WITH_COMMA));

  mlist_clients.back().start (mt_customer);
}
#endif


//  Operation: deregisterClient
//! Will kick all the MR-Clients registered by a CanCustomer_c.
//! Mainly to be used when shutting down a class-instance
//! that just wants to notify MR that it's gone!
void
MultiReceive_c::deregisterClient (CanCustomer_c& arc_client)
{
  // first of all remove all streams that are for this client!
  for (STL_NAMESPACE::list<DEF_Stream_c_IMPL>::iterator pc_iter = mlist_streams.begin(); pc_iter != mlist_streams.end(); )
  {
    // do also erase "kept" streams!!
    if (getClient (pc_iter->getIdent()) == &arc_client)
    { // remove stream (do not call any callbacks, as deregister is likely called in the client's destructor
      pc_iter = mlist_streams.erase (pc_iter);
    } else {
      ++pc_iter;
    }
  }

  // then remove all MultiReceiveClientWrappers for this client
  for (STL_NAMESPACE::list<MultiReceiveClientWrapper_s>::iterator pc_iter = mlist_clients.begin(); pc_iter != mlist_clients.end(); )
  {
    if (pc_iter->mpc_client == &arc_client)
    { // remove MultiReceiveClientWrapper_s
      pc_iter->stop (mt_customer);
      pc_iter = mlist_clients.erase (pc_iter);
    } else {
      ++pc_iter;
    }
  }
}


// Operation: deregisterClient
void
MultiReceive_c::deregisterClient(CanCustomer_c& arc_client, const IsoName_c& acrc_isoName,
                                 uint32_t aui32_pgn, uint32_t aui32_pgnMask, const IsoName_c* apcc_isoNameSender)
{
  IsoName_c isoNameSender;
  if (apcc_isoNameSender) isoNameSender = *apcc_isoNameSender;
  else /* no name ---- */ isoNameSender.setUnspecified();

  // first of all remove all streams that are for this client with this filter/mask/isoname tuple
  for (STL_NAMESPACE::list<DEF_Stream_c_IMPL>::iterator pc_iter = mlist_streams.begin(); pc_iter != mlist_streams.end(); )
  {
    // do also erase "kept" streams!!
    STL_NAMESPACE::list<MultiReceiveClientWrapper_s>::iterator i_list_clients = mlist_clients.begin();
    while (i_list_clients != mlist_clients.end())
    {
      if (i_list_clients->doesAcceptStream (pc_iter->getIdent()))
        break;
      ++i_list_clients;
    }

    if (i_list_clients != mlist_clients.end())
    {
      if ( (i_list_clients->mpc_client == &arc_client)
        && (i_list_clients->mc_isoName == acrc_isoName)
        && (i_list_clients->mui32_pgn == aui32_pgn)
        && (i_list_clients->mui32_pgnMask == aui32_pgnMask)
        && (i_list_clients->mc_isoNameSender == isoNameSender)
         )
      { // remove stream (do not call any callbacks, as deregister is likely called in the client's destructor
        // @todo 178 maybe call connection abort, maybe also do abort?
        pc_iter = mlist_streams.erase (pc_iter);
      } else {
        ++pc_iter;
      }
    }
  }

  // then remove all MultiReceiveClientWrappers for this client
  for (STL_NAMESPACE::list<MultiReceiveClientWrapper_s>::iterator pc_iter = mlist_clients.begin(); pc_iter != mlist_clients.end(); )
  {
    if ( (pc_iter->mpc_client == &arc_client)
      && (pc_iter->mc_isoName == acrc_isoName)
      && (pc_iter->mui32_pgn == aui32_pgn)
      && (pc_iter->mui32_pgnMask == aui32_pgnMask)
       )
    { // remove MultiReceiveClientWrapper_s
      pc_iter->stop (mt_customer);
      pc_iter = mlist_clients.erase (pc_iter);
    } else {
      ++pc_iter;
    }
  }
}


Stream_c*
MultiReceive_c::createStream (const ReceiveStreamIdentifier_c &arcc_streamIdent, uint32_t aui32_msgSize, int32_t ai_time )
{
  // Assumption/Precondition: Stream not there, so create and add it without checking!
  mlist_streams.push_back (DEF_Stream_c_IMPL (arcc_streamIdent, aui32_msgSize, ai_time MULTITON_INST_WITH_COMMA, false));
  mlist_streams.back().immediateInitAfterConstruction();

  return &mlist_streams.back();
}


Stream_c*
MultiReceive_c::getStreamInternal(
  const ReceiveStreamIdentifier_c &arcc_streamIdent,
  bool ab_includePgnInSearch)
{
  STL_NAMESPACE::list<DEF_Stream_c_IMPL>::iterator i_list_streams = mlist_streams.begin();
  while (i_list_streams != mlist_streams.end())
  {
    DEF_Stream_c_IMPL& curStream = *i_list_streams;
    if (curStream.getIdent().match (arcc_streamIdent, ab_includePgnInSearch))
    {
      if (curStream.getStreamingState() != StreamFinishedJustKept)
      { // only return streams that are not "kept". ignore kept streams here for further processing!
        return &curStream;
      }
    }
    ++i_list_streams;
  }
  return NULL;
}


void
MultiReceive_c::removeStream (Stream_c &arc_stream)
{
  for (STL_NAMESPACE::list<DEF_Stream_c_IMPL>::iterator i_list_streams = mlist_streams.begin();
       i_list_streams != mlist_streams.end();
       ++i_list_streams) {
    if (&arc_stream == (&*i_list_streams))
    { // also let "kept" streams be erased!
      mlist_streams.erase (i_list_streams);
      return;
    }
  }
}


bool
MultiReceive_c::processStreamDataChunk_ofMatchingClient(
  Stream_c& arc_stream,
  bool b_lastChunk)
{
  bool b_firstChunk = false;
  if (arc_stream.getBurstNumber() == 1) {
    // For first chunk processing, fill in the "mui8_streamFirstByte" field into the stream, so that the Client will now and later know what type of stream it is and how to handle the later Chunks...
    uint8_t ui8_firstByte;
    arc_stream >> ui8_firstByte;
    arc_stream.setFirstByte (ui8_firstByte);
    b_firstChunk = true;
  }

  // There must be a registered client, we can assume that here!
  // else we wouldn't have created that stream...
  CanCustomer_c* pc_client = getClient (arc_stream.getIdent());
  isoaglib_assert (pc_client);
  bool b_keepIt = pc_client->processPartStreamDataChunk (arc_stream, b_firstChunk, b_lastChunk);
  if (!b_lastChunk) return false; // result doesn't care if it's not the last chunk
  return b_keepIt;
}


bool
MultiReceive_c::finishStream (DEF_Stream_c_IMPL& arc_stream)
{
  if (arc_stream.getIdent().getDa() == 0xFF)
  { // BAM or FP-Broadcast
    for (STL_NAMESPACE::list<MultiReceiveClientWrapper_s>::iterator i_list_clients = mlist_clients.begin();
         i_list_clients != mlist_clients.end();
         ++i_list_clients)
    { // inform all clients that want Broadcast-Messages
      MultiReceiveClientWrapper_s& curClientWrapper = *i_list_clients;
      if ( (curClientWrapper.mui32_pgn == (arc_stream.getIdent().getPgn() & curClientWrapper.mui32_pgnMask))
       #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
        && (curClientWrapper.mb_isFastPacket == (arc_stream.getStreamType() == StreamFastPacket))
       #endif
        && (curClientWrapper.mb_alsoBroadcast) )
      {
        // don't care about result, as BAMs will NOT be kept anyway!
        (void) curClientWrapper.mpc_client->processPartStreamDataChunk(
          arc_stream,
          true, /* firstChunk. it's only one, don't care */
          true); /* lastChunk */
      }
    }
    // Don't keep stream, so the stream will be removed, as BAM-streams can NOT be kept!
    return false;
  }
  else
  { // destination specific
    /// handle destination specific FP different here, if they're handled!!
    #if DEBUG_MULTIRECEIVE
      INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "Sending End of Message Acknowledge out!" << INTERNAL_DEBUG_DEVICE_ENDL;
    #endif
    sendEndOfMessageAck (arc_stream);

    const bool cb_keepStream = processStreamDataChunk_ofMatchingClient (arc_stream, true);
    if (cb_keepStream)
    { // Keep stream (in "FinishedJustKept" kinda state)
      arc_stream.setStreamFinishedJustKept();
      return true;
    }
    else
    { // Don't keep stream, it was processed properly already.
      return false;
    }
  }
}


void
MultiReceive_c::timeEvent()
{
  STL_NAMESPACE::list<DEF_Stream_c_IMPL>::iterator i_list_streams = mlist_streams.begin();
  while (i_list_streams != mlist_streams.end())
  {
    DEF_Stream_c_IMPL& rc_stream = *i_list_streams;
    // BEGIN timeEvent every Stream_c

    // Check how to proceed with this Stream
    if (rc_stream.getStreamingState() == StreamFinishedJustKept)
    { // those streams are only stored for later processing, do NOTHING with them!
      ++i_list_streams;
      continue;
    }
    else if ((rc_stream.getNextComing() == AwaitCtsSend) && (rc_stream.readyToSendCts()))
    { // this case shouldn't happen for BAM / FastPacket
      #if DEBUG_MULTIRECEIVE
        INTERNAL_DEBUG_DEVICE << "Processing Burst" << INTERNAL_DEBUG_DEVICE_ENDL;
      #endif
      // CTS after Burst? -> process last Burst!
#ifdef ENABLE_MULTIPACKET_RETRY
      if( rc_stream.getPkgsReceivedInBurst() > 0 )
#endif
      processStreamDataChunk_ofMatchingClient(rc_stream, false);
      #if DEBUG_MULTIRECEIVE
        INTERNAL_DEBUG_DEVICE << "Send CTS to get next burst!" << INTERNAL_DEBUG_DEVICE_ENDL;
      #endif
      sendCurrentCts (rc_stream); // will increase the burstCurrent (in ENABLE_MULTIPACKET_RETRY only of ANY packets were in this burst)
    }

    /// TimeOut-Checks
    if (rc_stream.timedOut())
    {
      #if DEBUG_MULTIRECEIVE
        #ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
        if (rc_stream.getStreamType() == StreamFastPacket)
          INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** Fast-Packet-";
        else
        #endif
          INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_NEWLINE << "*** (E)TP-";
        INTERNAL_DEBUG_DEVICE << "Stream with SA " << (uint16_t) rc_stream.getIdent().getSa() << " timedOut, so sending out 'connAbort'. AwaitStep was " << (uint16_t) rc_stream.getNextComing() << " ***" << INTERNAL_DEBUG_DEVICE_ENDL;
      #endif
      notifyErrorConnAbort (rc_stream.getIdent(), TransferErrorStreamTimedOut, /* send Out ConnAbort Msg*/ true);
      tellClient (rc_stream);
      // remove Stream
      i_list_streams = mlist_streams.erase (i_list_streams);
      continue;
    }

    // END timeEvent every Stream_c
    ++i_list_streams;
  }

  int32_t newPeriod = nextTimeEvent() - System_c::getTime();
  if( newPeriod < 1 )
    newPeriod = 1;
  
  setPeriod( newPeriod, false );
}


Stream_c*
MultiReceive_c::getFinishedJustKeptStream (Stream_c* apc_lastKeptStream)
{
  // If "last==NULL", take the first to get, else wait for last to occur and take next!
  bool b_takeIt = (apc_lastKeptStream == NULL);

  for (STL_NAMESPACE::list<DEF_Stream_c_IMPL>::iterator i_list_streams = mlist_streams.begin(); i_list_streams != mlist_streams.end(); ++i_list_streams)
  {
    DEF_Stream_c_IMPL* pc_stream = &*i_list_streams;
    if (pc_stream->getStreamingState() == StreamFinishedJustKept)
    { // let's see if we take this kept one..
      if (b_takeIt)
        return pc_stream;
      if (apc_lastKeptStream == pc_stream)
        b_takeIt=true; // take the next kept one following this...
    }
  }
  return NULL;
}

/// Use to remove a "kept"-stream after it is gotten by "getFinishedJustKeptStream" and processed.
void
MultiReceive_c::removeKeptStream (Stream_c* apc_keptStream)
{
  for (STL_NAMESPACE::list<DEF_Stream_c_IMPL>::iterator i_list_streams = mlist_streams.begin();
       i_list_streams != mlist_streams.end();
       ++i_list_streams)
  {
    if ((&*i_list_streams) == apc_keptStream)
    { // delete it. it's a justKept one, as we checked that before!
      if (i_list_streams->getStreamingState() != StreamFinishedJustKept)
        return; // do NOT allow any other streams to be deleted

      mlist_streams.erase (i_list_streams);
      return;
    }
  }
}


uint32_t
MultiReceive_c::getStreamCount() const
{
  uint32_t streamCount = 0;

  for( STL_NAMESPACE::list<DEF_Stream_c_IMPL>::const_iterator iter = mlist_streams.begin();
    iter != mlist_streams.end(); ++iter )
  {
    if( iter->getStreamingState() != StreamFinishedJustKept )
      ++streamCount;
  }

  return streamCount;
}


void
MultiReceive_c::sendCurrentCts (DEF_Stream_c_IMPL &arc_stream)
{
  isoaglib_assert( arc_stream.getNextComing() == AwaitCtsSend );

  /* may also be 0, meaning HOLD CONNECTION OPEN, but we can handle multiple streams... ;-)
     and we don't want to hold connections open that are very short, so well........... */

  // the following "> 0" check shouldn't be needed because if we reach here, we shouldn't
  uint8_t ui8_allowPackets = (getStreamCount() > 0)
    ? uint8_t((CONFIG_MULTI_RECEIVE_MAX_OVERALL_PACKETS_ADDED_FROM_ALL_BURSTS) / getStreamCount())
    : uint8_t(1);

  if (ui8_allowPackets == 0)
  { // Don't allow 0 packets here as this would mean HOLD-CONNECTION OPEN and
    // we'd have to take action and cannot wait for the sender sending...
    ui8_allowPackets = 1;
  }
  if (ui8_allowPackets > CONFIG_MULTI_RECEIVE_MAX_PER_CLIENT_BURST_IN_PACKETS)
  { // limit the number of packets a single sender can send even if we could handle all those packets!
    ui8_allowPackets = CONFIG_MULTI_RECEIVE_MAX_PER_CLIENT_BURST_IN_PACKETS;
  }

  uint8_t ui8_pkgsToExpect = arc_stream.expectBurst (ui8_allowPackets); // we wish e.g. 20 pkgs (as always), but there're only 6 more missing to complete the stream!

  CanPkgExt_c pkg;
  switch (arc_stream.getStreamType())
  {
    case StreamETP:
      pkg.setExtCanPkg8 (scui8_tpPriority, 0, (ETP_CONN_MANAGE_PGN >> 8),
                             arc_stream.getIdent().getSa(), /* dest */
                             arc_stream.getIdent().getDa(), /* src */
                             0x15 /* decimal: 21 */,
                             ui8_pkgsToExpect,
                             MACRO_BYTEORDER_toLoMidHi (arc_stream.getPkgNextToWrite()),
                             MACRO_BYTEORDER_toLoMidHi (arc_stream.getIdent().getPgn()));
      break;

    case StreamTP:
      pkg.setExtCanPkg8 (scui8_tpPriority, 0, (TP_CONN_MANAGE_PGN >> 8),
                             arc_stream.getIdent().getSa(), /* dest */
                             arc_stream.getIdent().getDa(), /* src */
                             0x11 /* decimal: 17 */,
                             ui8_pkgsToExpect,
                             ((arc_stream.getPkgNextToWrite()) & 0xFF),
                             0xFF,
                             0xFF,
                             MACRO_BYTEORDER_toLoMidHi (arc_stream.getIdent().getPgn()));
      break;

#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
    case StreamFastPacket:
      isoaglib_assert (!"Sending a CTS shouldn't have been called for a FastPacket-stream.");
      break;
#endif

    default:
      isoaglib_assert (!"sendCurrentCts called with unknown StreamType-enum.");
      break;
  }

  // send message
  __IsoAgLib::getIsoBusInstance4Comm() << pkg;
}


void
MultiReceive_c::sendConnAbort (const ReceiveStreamIdentifier_c &arcc_rsi)
{
#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  if (arcc_rsi.getStreamType() == StreamFastPacket)
  { // NEVER answer to FP-Streams
    return;
  }
#endif

  if (arcc_rsi.getDa() == 0xFF)
  { // NEVER answer to a packet that was sent to GLOBAL ADDRESS 0xFF
    return;
  }

  uint8_t pgn = (arcc_rsi.getStreamType() == StreamETP) ? (ETP_CONN_MANAGE_PGN >> 8) : (TP_CONN_MANAGE_PGN >> 8);

  CanPkgExt_c pkg;
  pkg.setExtCanPkg8 (scui8_tpPriority, 0, pgn, /* dest: */ arcc_rsi.getSa(), /* src: */ arcc_rsi.getDa(),
                         0xFF /* decimal: 255 */, 0xFF,0xFF,0xFF,0xFF, MACRO_BYTEORDER_toLoMidHi(arcc_rsi.getPgn()));
  #if DEBUG_MULTIRECEIVE
  INTERNAL_DEBUG_DEVICE << "Sending out ConnAbort!" << INTERNAL_DEBUG_DEVICE_ENDL;
  #endif
  __IsoAgLib::getIsoBusInstance4Comm() << pkg;
}


void
MultiReceive_c::tellClient (Stream_c &arc_stream)
{
  if (arc_stream.getIdent().getDa() != 0xFF)
  {
    // search Client and tell about connAbort
    CanCustomer_c* pc_clientFound = getClient (arc_stream.getIdent());
    if (pc_clientFound) {
      pc_clientFound->reactOnAbort (arc_stream);
    }
  } // else: // NEVER promote aborts from BROADCASTS, because there's no partial parsing possible, so no need to inform the user...
}


void
MultiReceive_c::tellClientRemoveStream (Stream_c &arc_stream)
{
  tellClient (arc_stream);
  removeStream (arc_stream);
}


void
MultiReceive_c::sendEndOfMessageAck (DEF_Stream_c_IMPL &arc_stream)
{
  CanPkgExt_c pkg;
  // NO Check here, this function IS called on purpose and WILL send EoMAck and CLOSE the Stream!
  switch (arc_stream.getStreamType())
  {
    case StreamETP:
      pkg.setExtCanPkg8 (scui8_tpPriority, 0, (ETP_CONN_MANAGE_PGN >> 8),
                             arc_stream.getIdent().getSa(), /* dest */
                             arc_stream.getIdent().getDa(), /* src */
                             0x17 /* decimal: 23 */,
                             MACRO_BYTEORDER_toLoMidMidHi(arc_stream.getByteTotalSize()),
                             MACRO_BYTEORDER_toLoMidHi(arc_stream.getIdent().getPgn()));
      break;

    case StreamTP:
      pkg.setExtCanPkg8 (scui8_tpPriority, 0, (TP_CONN_MANAGE_PGN >> 8),
                             arc_stream.getIdent().getSa(), /* dest */
                             arc_stream.getIdent().getDa(), /* src */
                             0x13 /* decimal: 19 */,
                             MACRO_BYTEORDER_toLoHi(arc_stream.getByteTotalSize()),
                             uint8_t(arc_stream.getPkgTotalSize()),
                             0xFF /* reserved */,
                             MACRO_BYTEORDER_toLoMidHi(arc_stream.getIdent().getPgn()));
      break;

#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
    case StreamFastPacket:
      isoaglib_assert (!"Sending an EoMAck shouldn't have been called for a FastPacket-stream.");
      break;
#endif

    default:
      isoaglib_assert (!"sendCurrentCts called with unknown StreamType-enum.");
      break;
  }

  // send message
  __IsoAgLib::getIsoBusInstance4Comm() << pkg;
}


void
MultiReceive_c::init()
{
  isoaglib_assert (!initialized());

  getSchedulerInstance().registerTask( *this, 0 );
  getIsoMonitorInstance4Comm().registerControlFunctionStateHandler( mt_handler );

#ifdef HAL_USE_SPECIFIC_FILTERS
  // insert receive filters for broadcasted TP
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, ( TP_CONN_MANAGE_PGN  |0xFF)<<8, Ident_c::ExtendedIdent ), 8);
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, ( TP_DATA_TRANSFER_PGN|0xFF)<<8, Ident_c::ExtendedIdent ), 8);
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, (ETP_CONN_MANAGE_PGN  |0xFF)<<8, Ident_c::ExtendedIdent ), 8);
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, (ETP_DATA_TRANSFER_PGN|0xFF)<<8, Ident_c::ExtendedIdent ), 8);
#else
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, TP_CONN_MANAGE_PGN << 8, Ident_c::ExtendedIdent ), 8 );
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, TP_DATA_TRANSFER_PGN << 8, Ident_c::ExtendedIdent ), 8 );
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, ETP_CONN_MANAGE_PGN << 8, Ident_c::ExtendedIdent ), 8 );
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, ETP_DATA_TRANSFER_PGN << 8, Ident_c::ExtendedIdent ), 8 );
#endif

  setPeriod( 5000, false ); // nothing to do per default!

  setInitialized();
}


void
MultiReceive_c::close( void )
{
  isoaglib_assert (initialized());

  getSchedulerInstance().deregisterTask( *this );
  getIsoMonitorInstance4Comm().deregisterControlFunctionStateHandler( mt_handler );


#ifdef HAL_USE_SPECIFIC_FILTERS
  // remove receive filters for broadcasted TP
  getIsoBusInstance4Comm().deleteFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, ( TP_CONN_MANAGE_PGN  |0xFF)<<8, Ident_c::ExtendedIdent ) );
  getIsoBusInstance4Comm().deleteFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, ( TP_DATA_TRANSFER_PGN|0xFF)<<8, Ident_c::ExtendedIdent ) );
  getIsoBusInstance4Comm().deleteFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, (ETP_CONN_MANAGE_PGN  |0xFF)<<8, Ident_c::ExtendedIdent ) );
  getIsoBusInstance4Comm().deleteFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, (ETP_DATA_TRANSFER_PGN|0xFF)<<8, Ident_c::ExtendedIdent ) );
#else
  getIsoBusInstance4Comm().deleteFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, TP_CONN_MANAGE_PGN << 8, Ident_c::ExtendedIdent ) );
  getIsoBusInstance4Comm().deleteFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, TP_DATA_TRANSFER_PGN << 8, Ident_c::ExtendedIdent ) );
  getIsoBusInstance4Comm().deleteFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, ETP_CONN_MANAGE_PGN << 8, Ident_c::ExtendedIdent ) );
  getIsoBusInstance4Comm().deleteFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, ETP_DATA_TRANSFER_PGN << 8, Ident_c::ExtendedIdent ) );
#endif

  mlist_streams.clear();
  mlist_clients.clear();

  setClosed();
}


CanCustomer_c*
MultiReceive_c::getClient (ReceiveStreamIdentifier_c ac_streamIdent)
{
  for (STL_NAMESPACE::list<MultiReceiveClientWrapper_s>::iterator i_list_clients = mlist_clients.begin();
       i_list_clients != mlist_clients.end();
       ++i_list_clients)
  {
    if (i_list_clients->doesAcceptStream (ac_streamIdent))
      return i_list_clients->mpc_client;
  }
  return NULL;
}


//! return 0x00-0xFF: first byte of the stream!
//! return 0x100: first byte not yet known!
//! return 0x101: not a valid index!
uint16_t
MultiReceive_c::getStreamFirstByte (uint32_t ui32_index) const
{
  uint32_t ui32_curIndex=0;
  if (ui32_index < mlist_streams.size()) {
    for (STL_NAMESPACE::list<DEF_Stream_c_IMPL>::const_iterator pc_iter = mlist_streams.begin(); pc_iter != mlist_streams.end(); ++pc_iter)
    {
      if (ui32_curIndex == ui32_index) {
        if ((*pc_iter).getByteAlreadyReceived() > 0)
          return (*pc_iter).getFirstByte();
        else
          return 0x100; // first byte not yet known!
      }
      ui32_curIndex++;
    }
  }
  return 0x101; // wrong index
}


bool
MultiReceive_c::isAtLeastOneWithFirstByte(uint8_t firstByte)
{
  for (STL_NAMESPACE::list<DEF_Stream_c_IMPL>::const_iterator pc_iter = mlist_streams.begin(); pc_iter != mlist_streams.end(); ++pc_iter) {
    if ((*pc_iter).getByteAlreadyReceived() > 0)  {
      if ((*pc_iter).getFirstByte() == firstByte) {
        return true;
      }
    }
  }
  return false;
}


//! giving the data in 1/1000. This implementation works for pools up to 4 MB, which should be enough ;)
uint32_t
MultiReceive_c::getStreamCompletion1000 (uint32_t ui32_index, bool b_checkFirstByte, uint8_t ui8_returnNullIfThisIsFirstByte) const
{
  uint32_t ui32_curIndex=0;
  if (ui32_index < mlist_streams.size()) {
    // retrieve completion in 1/10..100%
    for (STL_NAMESPACE::list<DEF_Stream_c_IMPL>::const_iterator pc_iter = mlist_streams.begin(); pc_iter != mlist_streams.end(); ++pc_iter)
    {
      if (ui32_curIndex == ui32_index) {
        if ((b_checkFirstByte) && ((*pc_iter).getFirstByte() != ui8_returnNullIfThisIsFirstByte))
          return 0;
        // over 4 MB clipping ;) shouldn't occur anyway...
        if ((*pc_iter).getByteAlreadyReceived() > 0x400000 /*(4*1024*1024)*/)
          return 1000;
        else
          return (pc_iter->getByteAlreadyReceived() * 1000) / pc_iter->getByteTotalSize();
      }
      ui32_curIndex++;
    }
  }
  return 0; // wrong index or (unlikely) not "found" in list...
}


//! giving the data in 1/1000. This implementation works for pools up to 4 MB, which should be enough ;)
uint32_t
MultiReceive_c::getMaxStreamCompletion1000 (bool b_checkFirstByte, uint8_t ui8_returnNullIfThisIsFirstByte) const
{
  uint32_t ui32_maxStreamCompletion1000=0;
  uint32_t ui32_currentCompletion1000;
  // retrieve completion in 1/10..100%
  for (STL_NAMESPACE::list<DEF_Stream_c_IMPL>::const_iterator pc_iter = mlist_streams.begin(); pc_iter != mlist_streams.end(); ++pc_iter)
  {
    if ((b_checkFirstByte) && ((*pc_iter).getFirstByte() != ui8_returnNullIfThisIsFirstByte))
      ui32_currentCompletion1000=0; // don't care for this stream
    else if ((*pc_iter).getByteAlreadyReceived() > 0x400000 /*(4*1024*1024)*/) // over 4 MB clipping ;) shouldn't occur anyway...
      return 1000; // is already max ;)
    else
      ui32_currentCompletion1000 = (pc_iter->getByteAlreadyReceived() * 1000) / pc_iter->getByteTotalSize();

    if (ui32_currentCompletion1000 > ui32_maxStreamCompletion1000)
      ui32_maxStreamCompletion1000 = ui32_currentCompletion1000;
  }
  return ui32_maxStreamCompletion1000;
}


void
MultiReceive_c::reactOnIsoItemModification (ControlFunctionStateHandler_c::iIsoItemAction_e at_action, IsoItem_c const& acrc_isoItem)
{
#ifdef HAL_USE_SPECIFIC_FILTERS
  switch (at_action)
  {
    case ControlFunctionStateHandler_c::AddToMonitorList:
      if (acrc_isoItem.itemState (IState_c::Local))
      { // local IsoItem_c has finished adr claim
        getIsoFilterManagerInstance4Comm().insertIsoFilter (IsoFilter_s(mt_customer, IsoAgLib::iMaskFilter_c( (0x3FFFF00UL), ( TP_CONN_MANAGE_PGN   << 8) ), &acrc_isoItem.isoName(), NULL, 8) );
        getIsoFilterManagerInstance4Comm().insertIsoFilter (IsoFilter_s(mt_customer, IsoAgLib::iMaskFilter_c( (0x3FFFF00UL), ( TP_DATA_TRANSFER_PGN << 8) ), &acrc_isoItem.isoName(), NULL, 8) );
        getIsoFilterManagerInstance4Comm().insertIsoFilter (IsoFilter_s(mt_customer, IsoAgLib::iMaskFilter_c( (0x3FFFF00UL), (ETP_CONN_MANAGE_PGN   << 8) ), &acrc_isoItem.isoName(), NULL, 8) );
        getIsoFilterManagerInstance4Comm().insertIsoFilter (IsoFilter_s(mt_customer, IsoAgLib::iMaskFilter_c( (0x3FFFF00UL), (ETP_DATA_TRANSFER_PGN << 8) ), &acrc_isoItem.isoName(), NULL, 8) );
      }
      break;

    case ControlFunctionStateHandler_c::RemoveFromMonitorList:
      if (acrc_isoItem.itemState (IState_c::Local))
      { // local IsoItem_c has gone (i.e. IdentItem has gone, too.)
        /// @todo SOON-178 activate the reconfiguration when the second parameter in removeIsoFilter is there finally...
        getIsoFilterManagerInstance4Comm().removeIsoFilter (IsoFilter_s(mt_customer, IsoAgLib::iMaskFilter_c( (0x3FFFF00UL), ( TP_CONN_MANAGE_PGN   << 8) ), &acrc_isoItem.isoName(), NULL, 8));
        getIsoFilterManagerInstance4Comm().removeIsoFilter (IsoFilter_s(mt_customer, IsoAgLib::iMaskFilter_c( (0x3FFFF00UL), ( TP_DATA_TRANSFER_PGN << 8) ), &acrc_isoItem.isoName(), NULL, 8));
        getIsoFilterManagerInstance4Comm().removeIsoFilter (IsoFilter_s(mt_customer, IsoAgLib::iMaskFilter_c( (0x3FFFF00UL), (ETP_CONN_MANAGE_PGN   << 8) ), &acrc_isoItem.isoName(), NULL, 8));
        getIsoFilterManagerInstance4Comm().removeIsoFilter (IsoFilter_s(mt_customer, IsoAgLib::iMaskFilter_c( (0x3FFFF00UL), (ETP_DATA_TRANSFER_PGN << 8) ), &acrc_isoItem.isoName(), NULL, 8));
        /// @todo SOON-178 Maybe clean up some streams and clients?
        /// Shouldn't appear normally anyway, so don't care for right now...
      }
      break;

    default:
      break;
  }
#endif

  if ((at_action == ControlFunctionStateHandler_c::AddToMonitorList) || (at_action == ControlFunctionStateHandler_c::ChangedAddress) || (at_action == ControlFunctionStateHandler_c::LostAddress) || (at_action == ControlFunctionStateHandler_c::ReclaimedAddress))
  {
    /// If we're LostAddress, then we automatically have 0xFE now as SA...
    const uint8_t cui8_nr = acrc_isoItem.nr();
    for (STL_NAMESPACE::list<MultiReceiveClientWrapper_s>::iterator i_list_clients = mlist_clients.begin();
        i_list_clients != mlist_clients.end();
        ++i_list_clients)
    {
      if (i_list_clients->mc_isoName == acrc_isoItem.isoName())
      { // yes, it's that ISOName that A) (locally) lost its SA before or B) (remotely) just changed it.
        // note: we can receive (E)TPs for remote nodes, too. Needed for sniffing the WS-slave stuff!
        // conclusion: just update it, regardless if it "lost (SA == 0xFF)" its SA before or not...
        i_list_clients->mui8_cachedClientAddress = cui8_nr;
      }
    }
    // Notify all running streams
    for (STL_NAMESPACE::list<DEF_Stream_c_IMPL>::iterator i_list_streams = mlist_streams.begin();
         i_list_streams != mlist_streams.end(); ++i_list_streams)
    { // Adapt the SA also for kept streams - the application should only use the isoname anyway!
      const ReceiveStreamIdentifier_c& rc_rsi = i_list_streams->getIdent();
    // re-vitalize the Addresses, so that following packets using this address will get processed again...
      if (rc_rsi.getDaIsoName() == acrc_isoItem.isoName()) rc_rsi.setDa (cui8_nr);
      if (rc_rsi.getSaIsoName() == acrc_isoItem.isoName()) rc_rsi.setSa (cui8_nr);
    }
  }
}


int32_t
MultiReceive_c::nextTimeEvent() const
{
  int32_t nextTrigger = System_c::getTime() + 5000; // default: idle around...

  for( STL_NAMESPACE::list<DEF_Stream_c_IMPL>::const_iterator i_list_streams = mlist_streams.begin();
    i_list_streams != mlist_streams.end();
    ++i_list_streams )
  {
    DEF_Stream_c_IMPL const& rc_stream = *i_list_streams;

    const int32_t streamNextTrigger = rc_stream.nextTimeEvent();
    if( (streamNextTrigger != msci32_timeNever) && (streamNextTrigger < nextTrigger) )
      nextTrigger = streamNextTrigger;
  }
  return nextTrigger;
}


} // end namespace __IsoAgLib
