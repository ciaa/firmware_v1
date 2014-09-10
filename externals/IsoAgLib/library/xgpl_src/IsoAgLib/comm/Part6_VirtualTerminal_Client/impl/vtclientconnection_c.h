/*
  vtclientconnection_c.h: class for managing the
    connection between vt client and server

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTCLIENTCONNECTION_H
#define VTCLIENTCONNECTION_H

#include <IsoAgLib/driver/can/impl/cancustomer_c.h>

#include "aux2inputs_c.h"
#include "aux2functions_c.h"
#include "uploadpoolstate_c.h"
#include "commandhandler_c.h"
#include "multiplevt_c.h"

namespace IsoAgLib {
  class iVtObjectString_c;
  class iVtClientConnection_c;
  class iVtClientDataStorage_c;
}


namespace __IsoAgLib {

class VtServerInstance_c;
class VtClient_c;


class VtClientConnection_c : public CanCustomer_c
{
private:
  struct AuxAssignment_s
  {
    IsoName_c mc_inputIsoName;
    uint8_t mui8_inputNumber;
    uint16_t mui16_functionUid;
  };

public:
  enum uploadType_t {
    UploadIdle,
    UploadPool,
    UploadCommand
  };

  enum vtClientDisplayState_t {
    VtClientDisplayStateHidden,   // not on the display at all!
  //VtClientDisplayStateInactive, // on the display, but not the Active Working Set (Version 4)
    VtClientDisplayStateActive    // on the display and the Active Working Set
  };

  VtClientConnection_c(
    IdentItem_c& wsMasterIdentItem,
    VtClient_c& isoTerminal,
    IsoAgLib::iVtClientObjectPool_c& pool,
    const char* versionLabel,
    IsoAgLib::iVtClientDataStorage_c& dataStorage,
    uint8_t clientId,
    IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_en mode );

  virtual ~VtClientConnection_c();

  IsoAgLib::iVtClientConnection_c& toInterfaceReference();
  IsoAgLib::iVtClientConnection_c* toInterfacePointer();

  UploadPoolState_c &uploadPoolState() { return m_uploadPoolState; }
  IsoAgLib::iVtClientObjectPool_c& getPool() const { return m_uploadPoolState.getPool(); }
  IsoAgLib::iVtClientDataStorage_c& getVtClientDataStorage() const { return m_dataStorageHandler; }
  CommandHandler_c &commandHandler() { return m_commandHandler; }
  const CommandHandler_c &commandHandler() const { return m_commandHandler; }
  Aux2Inputs_c &aux2Inputs() { return m_aux2Inputs; }

  bool poolSuccessfullyUploaded() const { return m_uploadPoolState.successfullyUploaded(); }

  void processMsgVtToEcu( const CanPkgExt_c& c_data );
  void processMsgAck( const CanPkgExt_c& arc_data );

  void restart();

  void restartWithNextVt();

  void notifyOnVtsLanguagePgn();
  void notifyOnVtStatusMessage();
  void notifyOnAuxInputStatus( const CanPkgExt_c& arc_data );
  void notifyOnAux2InputStatus( const CanPkgExt_c& arc_data );
  void notifyOnAux2InputMaintenance( const CanPkgExt_c& arc_data);

#ifdef USE_VTOBJECT_auxiliaryinput2
  void triggerAux2InputStatusMsg(vtObjectAuxiliaryInput2_c* a_aux2InputObj) { m_aux2Inputs.timeEventInputStateMsg(a_aux2InputObj); }
#endif

  uint16_t getVtObjectPoolDimension() const;
  uint16_t getVtObjectPoolSoftKeyWidth() const;
  uint16_t getVtObjectPoolSoftKeyHeight() const;
  uint8_t  getUserClippedColor (uint8_t colorValue, IsoAgLib::iVtObject_c* obj, IsoAgLib::e_vtColour whichColour);
  uint8_t  getClientId() const { return mui8_clientId; }

  bool connectedToVtServer() const               { return (mpc_vtServerInstance != NULL); }  
  /** ATTENTION: Please assure "connectedToVtServer()/isVtActive()" before getting this reference */
  VtServerInstance_c& getVtServerInst() const    { isoaglib_header_assert(NULL != mpc_vtServerInstance); return *mpc_vtServerInstance; }  
  VtServerInstance_c* getVtServerInstPtr() const { return mpc_vtServerInstance; }

  IdentItem_c& getIdentItem() const              { return mrc_wsMasterIdentItem; }
  int getMultitonInst() { return mrc_wsMasterIdentItem.getMultitonInst(); }

  bool moveToNextVt();

  void notifyOnVtServerInstanceLoss( VtServerInstance_c& r_oldVtServerInst );

  bool isVtActive() const;

  bool isClientActive() const { return getVtDisplayState() == VtClientDisplayStateActive; }
  vtClientDisplayState_t getVtDisplayState() const { return men_displayState; }

  void sendMessage( uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 );

  // ### from CommandHandler_c ###
  void notifyOnCommandQueueFilledFromEmpty();
  void notifyOnFinishedCommand( bool stillCommandsLeft );

  //! @return true for successful assignment, false if SA couldn't be found.
  bool storeAuxAssignment( const CanPkgExt_c& arc_data );
  uint8_t storeAux2Assignment( Stream_c& arc_stream, uint16_t& rui16_functionObjId );

  // ### from UploadPoolState_c ###
  void notifyOnFinishedNonUserPoolUpload( bool initialUpload );

  // Note: This function will clear the stored timed out command after returning it!
  //       Typically call this in eventObjectPoolUploadedSuccessfully!
  // @return 0x00: No command timed out (or already checked and reset)
  //      != 0x00: The command that timed out and was the reason for a reconnect!
  inline uint8_t getAndResetLastTimedOutCommand();

private:
  void timeEvent();
  void timeEventSearchForNewVt();
  bool timeEventMaster();

  void doStart();
  void doStop();

  void fakeVtOffPeriod( int32_t ai32_fakeOffTimeDurationMs ) { mi32_fakeVtOffUntil = HAL::getTime() + ai32_fakeOffTimeDurationMs; }
  void fakeVtOffStop() { mi32_fakeVtOffUntil = -1; }
  void checkAndHandleVtStateChange();

  /** set display state of vt client (from VT Status Message) */
  void setVtDisplayState( uint8_t ui8_sa );

  bool isPreferredVTTimeOut() const;

  // CanCustomer_c
  virtual void reactOnAbort( Stream_c& ) {}
  virtual bool reactOnStreamStart (const ReceiveStreamIdentifier_c& ac_ident, uint32_t aui32_totalLen);
  virtual bool processPartStreamDataChunk (Stream_c& apc_stream, bool ab_isFirstChunk, bool ab_isLastChunk);

private:
  bool mb_vtAliveCurrent;

  IdentItem_c& mrc_wsMasterIdentItem;
  VtClient_c& mrc_vtClient; // back ref.
  VtServerInstance_c* mpc_vtServerInstance;

  uploadType_t men_uploadType;
#ifdef ENABLE_VTCLIENT_RETRY
  unsigned m_uploadRetry;
#endif

private:
  int32_t mi32_nextWsMaintenanceMsg;
  uint8_t mui8_clientId;
  vtClientDisplayState_t men_displayState;

  STL_NAMESPACE::list<AuxAssignment_s> mlist_auxAssignments;
  Aux2Inputs_c m_aux2Inputs;
  Aux2Functions_c m_aux2Functions;

  UploadPoolState_c m_uploadPoolState;
  CommandHandler_c m_commandHandler;

  MultipleVt_c m_multipleVt;

  int32_t mi32_timeWsAnnounceKey;
  int32_t mi32_fakeVtOffUntil;

  uint8_t m_cmdTimedOut;

  IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_en men_registerPoolMode;

  IsoName_c mc_preferredVt;
  int32_t mi32_bootTime_ms;

  IsoAgLib::iVtClientDataStorage_c& m_dataStorageHandler; 

  CLASS_SCHEDULER_TASK_PROXY(VtClientConnection_c)

  SchedulerTaskProxy_c m_schedulerTaskProxy;
};


inline
void
VtClientConnection_c::notifyOnCommandQueueFilledFromEmpty()
{
  m_schedulerTaskProxy.retriggerNow();
}


inline
void
VtClientConnection_c::notifyOnFinishedCommand( bool stillCommandsLeft )
{
  men_uploadType = UploadIdle;

  // immediately on next timeEvent send out next command
  if( stillCommandsLeft )
    m_schedulerTaskProxy.retriggerNow();
}


inline
void
VtClientConnection_c::notifyOnAux2InputStatus( const CanPkgExt_c& pkg )
{
  m_aux2Functions.notifyOnAux2InputStatus( pkg, getPool() );
}


inline
void
VtClientConnection_c::notifyOnAux2InputMaintenance( const CanPkgExt_c& pkg )
{
  m_aux2Functions.notifyOnAux2InputMaintenance( pkg );
}


inline
uint8_t
VtClientConnection_c::getAndResetLastTimedOutCommand()
{
  const uint8_t timedOutCmd = m_cmdTimedOut;

  m_cmdTimedOut = 0x00;

  return timedOutCmd;
}


} // __IsoAgLib

#endif
