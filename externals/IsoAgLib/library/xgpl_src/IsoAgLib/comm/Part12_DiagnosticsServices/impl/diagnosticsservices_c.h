/*
  diagnosticsservices_c.h: manage Diagnostics Services DM1/DM2/DM3

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef DIAGNOSTICS_SERVICES_H
#define DIAGNOSTICS_SERVICES_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/scheduler/impl/schedulertask_c.h>
#include <IsoAgLib/driver/system/impl/system_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/canpkgext_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisendeventhandler_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgnhandler_c.h>

#include <IsoAgLib/comm/Part5_NetworkManagement/iisoname_c.h>

#include "../idiagnosticstypes.h"
#include "dtccontainer_c.h"

namespace IsoAgLib {
class iServiceToolVerifier_c;
}

// Begin Namespace __IsoAgLib
namespace __IsoAgLib {

// forward declarations
class IdentItem_c;

/**
  This class implements the ISO 11783 Diagnostics Services DM1/DM2/DM3
  @author Antoine Kandera, reviewed by Martin Wodok
*/
class DiagnosticsServices_c : public SchedulerTask_c
{
private:
  // helper structure for describing the status of a TP buffer
  struct BufferDescription_s
  {
    BufferDescription_s(uint8_t* parr_buffer) : marr_buffer(parr_buffer), marr_bufferSize(0), mb_bufferUsedForTP(false), mb_bufferIsValid(false) {}

    uint8_t* marr_buffer;
    uint32_t marr_bufferSize;
    bool mb_bufferUsedForTP;
    bool mb_bufferIsValid;
  };
public:
  DiagnosticsServices_c(IdentItem_c&);
  ~DiagnosticsServices_c();

  /**
    Registering of the "iServiceToolVerifier_c"
    If no handler is registered (default behavior), the request is accepted from any node.
    Note that only one Verifier can be registered.
    @return true -> iServiceToolVerifier_c registered correctly
            false -> an iServiceToolVerifier_c is already registered
  */
  bool registerServiceToolVerifier (IsoAgLib::iServiceToolVerifier_c& stVerifier);

  /**
    Deregistering of the "iServiceToolVerifier_c"
    @return true -> iServiceToolVerifier_c deregistered correctly
            false -> wrong iServiceToolVerifier_c
  */
  bool deregisterServiceToolVerifier (IsoAgLib::iServiceToolVerifier_c& stVerifier);

  /**
    Activate DTC
    @return 0 : Activation Failure (Too many DTCs registered, etc.)
            1..65535 : Internal OccurrenceCount clipped to 65535.
            Note: This is clipped in the DM1 message to 0x7E.
  */
  uint16_t dtcActivate (uint32_t SPN, IsoAgLib::FailureModeIndicator_en FMI);

  /**
    Deactivate DTC
    @return 0 : Deactivation Failure (DTC wasn't registered, etc.)
            1..65535 : Internal OccurrenceCount clipped to 65535.
            Note: This is clipped in the DM1 message to 0x7E.
  */
  uint16_t dtcDeactivate (uint32_t SPN, IsoAgLib::FailureModeIndicator_en FMI);

  /** Clear the Previous Active Trouble Codes */
  void serviceTool_dtcClearPrevious();

  /** return reference to DtcContainer */
  DtcContainer_c& getDtcContainer() {return mc_dtcs;}
  const DtcContainer_c& getDtcContainer() const {return mc_dtcs;}

  /** perform periodical actions */
  void timeEvent();

  /** initialisation for DiagnosticsServices_c */
  void init();
  /** every subsystem of IsoAgLib has explicit function for controlled shutdown */
  void close();

  /** call back function called when the TP is finished */
  void reactOnStateChange(const SendStream_c& sendStream);

  bool processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t ai_time );

#if DEBUG_DIAGNOSTICPGN
  bool isAtLeastOneDTCInCurrent() const { return m_dm1CurrentAtLeastOneDTC; }
#endif

private: // typedef
  class IsoRequestPgnHandlerProxy_c : public IsoRequestPgnHandler_c {
  public:
    typedef DiagnosticsServices_c Owner_t;

    IsoRequestPgnHandlerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}

    ~IsoRequestPgnHandlerProxy_c() {}

  private:
    bool processMsgRequestPGN(
        uint32_t aui32_pgn,
        IsoItem_c *apc_isoItemSender,
        IsoItem_c *apc_isoItemReceiver,
        int32_t ai_time )
    {
      return mrt_owner.processMsgRequestPGN(
          aui32_pgn,
          apc_isoItemSender,
          apc_isoItemReceiver,
          ai_time );
    }

    // IsoRequestPgnHandlerProxy_c shall not be copyable. Otherwise
    // the reference to the containing object would become invalid.
    IsoRequestPgnHandlerProxy_c(IsoRequestPgnHandlerProxy_c const &);

    IsoRequestPgnHandlerProxy_c &operator=(IsoRequestPgnHandlerProxy_c const &);

    Owner_t &mrt_owner;
  }; // IsoRequestPgnHandlerProxy_c

  class MultiSendEventHandlerProxy_c : public MultiSendEventHandler_c {
  public:
    typedef DiagnosticsServices_c Owner_t;

    MultiSendEventHandlerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}

    ~MultiSendEventHandlerProxy_c() {}

  private:
    void reactOnStateChange(const SendStream_c& sendStream)
    {
       mrt_owner.reactOnStateChange(sendStream);
    }

    // MultiSendEventHandlerProxy_c shall not be copyable. Otherwise
    // the reference to the containing object would become invalid.
    MultiSendEventHandlerProxy_c(MultiSendEventHandlerProxy_c const &);

    MultiSendEventHandlerProxy_c &operator=(MultiSendEventHandlerProxy_c const &);

    Owner_t &mrt_owner;
  }; // MultiSendEventHandlerProxy_c

private: // method
  /**
    Function to calculate when the next action (performed in timeEvent) is needed.
    Use this function at several places to set the Period or Retrigger-time.
    return: return a positive value, since return value is used for retrigger in the future
  */
  uint32_t calculateNextActionTime();

  uint16_t assembleDM1DM2(uint8_t* arr_send8bytes, bool ab_searchForActiveDtc, bool* atleastoneDTC);

  // do not call from this->timeEvent
  void changeActiveDtcStatusAndRetrigger(DtcContainer_c::Dtc_s& dtcToChange, bool active);

  void sendSingleDM1DM2(uint32_t ui32_pgn, uint8_t* arr_send8bytes);

  int getMultitonInst() const;

private: // attributes
  IdentItem_c& mrc_identItem;
  IsoAgLib::iServiceToolVerifier_c* mpc_serviceToolVerifier;

  // Wrap list of DTC active/inactive
  DtcContainer_c mc_dtcs;

  int32_t mi32_dm1LastSentTime;
  bool mb_dm1CurrentNeedsToBeSent; // indicates if sending needs to be triggered.

  /// The current buffer as created/changed by timeEvent/add/remove/clear
  uint8_t marr_dm1Current [2+4*(CONFIG_MAX_ACTIVE_DTCS)];
  uint32_t marr_dm1CurrentSize;
  bool m_dm1CurrentAtLeastOneDTC;

  uint8_t marr_dm1SendingBroadcast [2+4*(CONFIG_MAX_ACTIVE_DTCS)]; // the buffer currently being broadcast'
  BufferDescription_s ms_dm1SendingBroadcast;

  uint8_t marr_dm1SendingDestination [2+4*(CONFIG_MAX_ACTIVE_DTCS)]; // the buffer currently being broadcast'
  BufferDescription_s ms_dm1SendingDestination;

  uint8_t marr_dm2SendingDestination [2+4*(CONFIG_MAX_PREVIOUSLY_ACTIVE_DTCS)];
  BufferDescription_s ms_dm2SendingDestination;

  IsoRequestPgnHandlerProxy_c mt_isoRequestPgnHandler;
  MultiSendEventHandlerProxy_c mt_multiSendEventHandler;
};

}  // __IsoAgLib
#endif
