/*
  stream_c.h - Implementation of the generic (E)TP/FP-multi-packet
    reception functionality.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef STREAM_C_H
#define STREAM_C_H

#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/driver/can/impl/canpkg_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoname_c.h>
#include <supplementary_driver/driver/datastreams/streaminput_c.h>


namespace __IsoAgLib {
  

// forward declarations (for friend declaration)
class MultiReceive_c;


/** enum for type of stream */
enum StreamType_t
{
  StreamTP              = 0, // Standard TP with Standard CommandSet
  StreamETP             = 1  // Extended TP with Extended CommandSet
#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  ,StreamFastPacket     = 2 /// Fast-Packet addition: Check stream for Fast-Packet before proceeding anything!
#endif
};


class ReceiveStreamIdentifier_c
{
public:
  ReceiveStreamIdentifier_c (StreamType_t at_streamType, uint32_t aui32_pgn,
                             uint8_t aui8_da, const IsoName_c& acrc_daIsoName,
                             uint8_t aui8_sa, const IsoName_c& acrc_saIsoName)
    : mt_streamType (at_streamType)
    , ui32_pgn (aui32_pgn)
    , ui8_da (aui8_da)
    , ui8_sa (aui8_sa)
    , c_daIsoName (acrc_daIsoName)
    , c_saIsoName (acrc_saIsoName)
  {
  }

  ReceiveStreamIdentifier_c (const ReceiveStreamIdentifier_c& ac_rsi)
    : mt_streamType (ac_rsi.mt_streamType)
    , ui32_pgn (ac_rsi.ui32_pgn)
    , ui8_da (ac_rsi.ui8_da)
    , ui8_sa (ac_rsi.ui8_sa)
    , c_daIsoName (ac_rsi.c_daIsoName)
    , c_saIsoName (ac_rsi.c_saIsoName)
  {
  }

  bool match (const ReceiveStreamIdentifier_c& ac_rsi, bool ab_alsoComparePgn) const
  {
    return (ac_rsi.mt_streamType == mt_streamType)
        && ( !ab_alsoComparePgn || (ac_rsi.ui32_pgn == ui32_pgn) )
        && (ac_rsi.ui8_sa == ui8_sa)
        && (ac_rsi.ui8_da == ui8_da);
  }

  bool operator== (const ReceiveStreamIdentifier_c& ac_rsi) const
  {
    return match (ac_rsi, true);
  }

  bool operator< (const ReceiveStreamIdentifier_c& ac_rsi) const
  {
    if      (mt_streamType != ac_rsi.mt_streamType)
      return mt_streamType <  ac_rsi.mt_streamType;
    else if   (ui32_pgn    != ac_rsi.ui32_pgn)
      return   ui32_pgn    <  ac_rsi.ui32_pgn;
    else if   (c_daIsoName != ac_rsi.c_daIsoName)
      return   c_daIsoName <  ac_rsi.c_daIsoName;
    else if   (c_saIsoName != ac_rsi.c_saIsoName)
      return   c_saIsoName <  ac_rsi.c_saIsoName;
    else
      return false; // all fields equal
  }

  StreamType_t    getStreamType() const { return mt_streamType; }
  uint32_t               getPgn() const { return ui32_pgn; }
  const IsoName_c& getDaIsoName() const { return c_daIsoName; }
  const IsoName_c& getSaIsoName() const { return c_saIsoName; }

  // helper functions
  bool isIsoTp()  { return getStreamType() == StreamTP; }
  bool isIsoEtp() { return getStreamType() == StreamETP; }
#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  bool isNmeaFp() { return getStreamType() == StreamFastPacket; }
#endif

  // public for direct access to the CACHED addresses.
  //! Be aware that SA/DA can CHANGE while the system is running, so
  //! better use the IsoName normally instead. Only use the SA if you
  //! keep your own SA list also up-to-date with the SA-Claim-Handlers!
  uint8_t getDa() const { return ui8_da; }
  uint8_t getSa() const { return ui8_sa; }

// declaring/defining the following methods mutable as
// A) they're only updating the cached SA/DA and
// B) only MultiReceive is friend and can do this!
private:
  void setDa (uint8_t aui8_da) const { ui8_da = aui8_da; }
  void setSa (uint8_t aui8_sa) const { ui8_sa = aui8_sa; }

  friend class __IsoAgLib::MultiReceive_c;

private:

  /** Type of this Stream: TP/ETP/FP */
  StreamType_t mt_streamType;

  /** PGN of the stream */
  uint32_t ui32_pgn;

  /** Destination Address - mutable as it's only a CACHE for the c_daIsoName! */
  mutable uint8_t ui8_da;

  /** Source Address - mutable as it's only a CACHE for the c_daIsoName! */
  mutable uint8_t ui8_sa;

  /** Destination ISOName */
  IsoName_c c_daIsoName;

  /** Source ISOName */
  IsoName_c c_saIsoName;
};



// T1/T2 here are the same as in "multireceive_c.h"
// T1/T2 here are the same as in "multireceive_c.h"
static const int32_t msci32_timeOutT1=250;  // data -> data
static const int32_t msci32_timeOutT2=1250; // cts -> data(TP)/dpo(ETP)
static const int32_t msci32_timeOutT3=1250; // data/rts -> cts (not needed for checking here)
static const int32_t msci32_timeOutT4=550;  // cts(0)<->cts
static const int32_t msci32_timeOutT5=250;  // dpo -> data (ETP only, naming "T5" done by Martin)
static const int32_t msci32_timeOutFP=1000;  // FPdata <-> FPdata
static const int32_t msci32_timeNever=-1;
/** @todo SOON-178 DataLinkLayerRevision The above should be adapted to the new revision of Part 3 - Data Link Layer, or should we use the old IS timings to be correct??
  * also limit number of retries to 2 (if retries are implemented, if not, do so ;-) */


/** enum for Streaming state */
typedef enum {
  StreamRunning  = 2,
  StreamFinished = 3,
  StreamFinishedJustKept = 4,
} StreamingState_t;


/** enum for next awaited information */
typedef enum {
  AwaitNothing = 0,
  AwaitDpo     = 1,
  AwaitData    = 2,
  AwaitCtsSend = 3
} NextComing_t;



class Stream_c : public StreamInput_c //, public ClientBase --> for single inheritance this is now a member variable
{
public:
  Stream_c (const ReceiveStreamIdentifier_c& ac_rsi,
            uint32_t aui32_msgSize, int32_t ai_time MULTITON_INST_PARAMETER_DEF_WITH_COMMA,
            bool ab_skipCtsAwait=false);

  Stream_c (const Stream_c &);

  Stream_c& operator= (const Stream_c &);

  virtual ~Stream_c();


  /// Former iStream_c functions!
  virtual uint32_t getNotParsedSize()=0;

  virtual uint8_t getNextNotParsed()=0;

  virtual uint8_t getNotParsed(uint32_t ui32_notParsedRelativeOffset)=0;

  void setStreamInvalid()       { mb_streamInvalid = true; };
  bool getStreamInvalid() const { return mb_streamInvalid; };


  bool timedOut();

  // Tell the stream what we wish to CTS, get as result how many can really be CTS'd.
  // Sets the internal state to expect (DPO and afterwards) the calculated amount of DATA commands next
  uint8_t expectBurst(uint8_t wishingPkgs);

  //! @return false -> Stream was either not in the state of awaiting Data
  //!                  or the sequence number didn't match
  //!         true -> Data was added to the stream.
  bool handleDataPacket (const CanPkg_c& arc_pkg );

/// Begin Additional Abstract methods handled by StreamLinear_c/StreamChunk_c
  //  Operation: insert
  virtual void insert7Bytes(const uint8_t* pui8_data)=0;
#ifdef ENABLE_MULTIPACKET_VARIANT_FAST_PACKET
  virtual void insertFirst6Bytes(const uint8_t* pui8_data)=0;
#endif
/// End Abstract methods handled by StreamLinear_c/StreamChunk_c


/// Begin StreamInput_c methods
  StreamInput_c& operator>>(uint8_t& ui8_data) { ui8_data = getNextNotParsed(); return *this; };

//  virtual bool eof() const=0; // pass on abstract method
/// End StreamInput_c methods

  int32_t nextTimeEvent() const;

  bool setDataPageOffset(uint32_t aui32_dataPageOffset);

  const ReceiveStreamIdentifier_c& getIdent() const { return mc_ident; }
  StreamType_t     getStreamType()        const { return mc_ident.getStreamType(); };
  StreamingState_t getStreamingState ()   const { return mt_streamState; };
  NextComing_t     getNextComing ()       const { return mt_awaitStep; };
  uint32_t getPkgNextToWrite ()           const { return mui32_pkgNextToWrite; };
  uint32_t getPkgTotalSize ()             const { return mui32_pkgTotalSize; };
  uint32_t getByteTotalSize ()            const { return mui32_byteTotalSize; };
  uint32_t getByteAlreadyReceived()       const { return mui32_byteAlreadyReceived; };
  uint32_t getBurstNumber()               const { return mui32_burstCurrent; };
#ifdef ENABLE_MULTIPACKET_RETRY
  uint8_t  getPkgsReceivedInBurst()       const { return mui8_pkgsReceivedInBurst; }
#endif
  int32_t getStartTime()                  const { return mi_startTime; }
  int32_t getFinishTime()                 const { return mi_finishTime; }
  //! Provide first byte set by first call of processDataChunk... First byte containes command.
  uint8_t  getFirstByte()                 const { return mui8_streamFirstByte; };
  //! Store first byte of stream. First byte containes command.
  void     setFirstByte(uint8_t aui8_firstByte) { mui8_streamFirstByte = aui8_firstByte; };

  void setStreamFinishedJustKept() { awaitNextStep (AwaitNothing, msci32_timeNever); // no timeOut on own Send-Awaits
                                     mt_streamState = StreamFinishedJustKept; }; // from now on NOTHING more should be done with this stream!

  bool readyToSendCts();
  void setTPBurstLimit( uint8_t limit ) { mui8_maxPacketInTPBurst = limit; };

private:
  void awaitNextStep (NextComing_t at_awaitStep, int32_t ai32_timeOut);


protected:
  MULTITON_MEMBER_DEF


private:
  ReceiveStreamIdentifier_c mc_ident;

  StreamingState_t mt_streamState;

  NextComing_t mt_awaitStep;

  int32_t mi32_delayCtsUntil;


/// Byte counting stuff
protected:
  uint32_t mui32_byteTotalSize;

  // Has this stream been marked invalid while parsing?
  bool mb_streamInvalid;

private:
  uint32_t mui32_byteAlreadyReceived;

/// Pkg counting stuff
  uint32_t mui32_pkgNextToWrite;      // should be initialized to 1
  uint32_t mui32_pkgTotalSize;        // calculated amount of pkgs to arrive for the given byteTotalSize!
  uint8_t   mui8_pkgRemainingInBurst; // the value requested by CTS
#ifdef ENABLE_MULTIPACKET_RETRY
  uint8_t   mui8_pkgsReceivedInBurst;
#endif
  uint32_t mui32_burstCurrent;        // counting the bursts, so we know if it's the first or a following!
  uint8_t   mui8_streamFirstByte;     // will be the command that it's containing. set at the first call to processDataChunk...
  uint32_t mui32_dataPageOffset;      //  Attribute: mui32_dataPageOffset: gets set when a DPO arrives...
  uint8_t mui8_maxPacketInTPBurst;      // Burst Size limit the RTS controller specified.

  int32_t mi32_timeoutLimit;

  int32_t mi_startTime;
  int32_t mi_finishTime;

#ifdef ENABLE_MULTIPACKET_RETRY
  uint32_t mui32_isoErrorBurstWaitForPkgThenRetry; // == 0 ==> normal operation. > 0 ==> missing packet in burst, wait for last packet, then re-CTS
  uint8_t mui8_isoPkgRetryCountInBurst;
#endif
};


} // end namespace __IsoAgLib

#endif
