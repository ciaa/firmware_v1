/*
  vtclient_c.h: central ISO terminal management

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTCLIENT_C_H
#define VTCLIENT_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/driver/can/impl/cancustomer_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/canpkgext_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtclientobjectpool_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/identitem_c.h>
#include "vtserverinstance_c.h"
#include "vtservermanager_c.h"
#include "vtclientconnection_c.h"

#include <list>


namespace __IsoAgLib {

class VtClientConnection_c;
class iIdentItem_c;

/** central IsoAgLib Virtual Terminal-Client management object */
class VtClient_c : private Subsystem_c {
  MACRO_MULTITON_CONTRIBUTION();
public:
  virtual ~VtClient_c() {}

  void init();
  void close();

  VtClientConnection_c* initAndRegisterObjectPool(
    IdentItem_c& apc_wsMasterIdentItem, 
    IsoAgLib::iVtClientObjectPool_c& arc_pool, 
    const char* apc_versionLabel, 
    IsoAgLib::iVtClientDataStorage_c& apc_claimDataStorage, 
    IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_en aen_mode );

  bool deregisterObjectPool (IdentItem_c& apc_wsMasterIdentItem);

  /** function that handles incoming can messages */
  virtual void processMsg( const CanPkg_c& arc_data );
  void processMsgNonGlobal( const CanPkgExt_c& arc_data );
  void processMsgGlobal( const CanPkgExt_c& arc_data );

  bool sendCommandForDEBUG(IsoAgLib::iIdentItem_c& apc_wsMasterIdentItem, uint8_t* apui8_buffer, uint32_t ui32_size);

  VtClientConnection_c& getClientByID (uint8_t ui8_clientIndex) { return *m_vtConnections[ui8_clientIndex]; }
  VtClientConnection_c* getClientPtrByID (uint8_t ui8_clientIndex) { return (!m_vtConnections.empty()) ? m_vtConnections[ui8_clientIndex] : NULL; }

  bool isAnyVtAvailable() const { return m_serverManager.isAnyVtAvailable(); }
  // is any claimed VT sending VT status
  bool isAnyVtActive( bool mustBePrimary ) const { return (getActiveVtServer( mustBePrimary, NULL ) != NULL); }
  uint16_t getActiveVtCount() const { return m_serverManager.getActiveVtCount(); }

  void notifyAllConnectionsOnAux1InputStatus( const CanPkgExt_c& refc_data ) const;
  void notifyAllConnectionsOnAux2InputStatus( const CanPkgExt_c& refc_data ) const;
  void notifyAllConnectionsOnAux2InputMaintenance( const CanPkgExt_c& refc_data ) const;

  VtServerInstance_c* getActiveVtServer( bool mustBePrimary, const VtServerInstance_c* ap_searchStart ) const
  { return m_serverManager.getActiveVtServer(mustBePrimary, ap_searchStart); }

  VtServerInstance_c* getPreferredVtServer(const IsoName_c& aref_prefferedVTIsoName) const
  { return m_serverManager.getPreferredVtServer(aref_prefferedVTIsoName); }
  
  VtServerInstance_c* getSpecificVtServer(const IsoAgLib::iVtClientObjectPool_c& arc_pool) const
  { return m_serverManager.getSpecificVtServer(arc_pool); }

  ////////////////////////
  // INTERFACE FUNTIONS //
  ////////////////////////
// the following define should be globally defined in the project settings...
// (currently not supported, due to multi VT enhancements)
#if 0
#ifdef USE_IOP_GENERATOR_FAKE_VT_PROPERTIES
  void fakeVtProperties (uint16_t aui16_dimension, uint16_t aui16_skWidth, uint16_t aui16_skHeight, uint8_t aui16_colorDepth, uint16_t aui16_fontSizes)
  { m_serverManager.fakeVtProperties(aui16_dimension, aui16_skWidth, aui16_skHeight, aui16_colorDepth, aui16_fontSizes, *this); }
#endif
#endif

private:
  uint16_t getClientCount() const;

  class CanCustomerProxy_c : public CanCustomer_c {
  public:
    typedef VtClient_c Owner_t;

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
    typedef VtClient_c Owner_t;

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

  /** private constructor which prevents direct instantiation in user application
    * NEVER define instance of VtClient_c within application
    */
  VtClient_c();

  void reactOnIsoItemModification (ControlFunctionStateHandler_c::iIsoItemAction_e at_action, IsoItem_c const& acrc_isoItem)
  {
    m_serverManager.reactOnIsoItemModification(at_action, acrc_isoItem, m_vtConnections, *this);
  }

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


  VtClientConnection_c* initAndRegisterObjectPoolCommon(
    IdentItem_c& rc_identItem, 
    IsoAgLib::iVtClientObjectPool_c& arc_pool, 
    const char* apc_versionLabel, 
    IsoAgLib::iVtClientDataStorage_c& apc_claimDataStorage, 
    IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_en aen_mode );

private:
  STL_NAMESPACE::vector<VtClientConnection_c*> m_vtConnections;

  Handler_t mt_handler;

  VtServerManager_c m_serverManager;

public:
  // so that VtClientConnection can insert filters to us.
  Customer_t mt_customer;

  friend VtClient_c &getVtClientInstance(uint8_t aui8_instance);
};


/** C-style function, to get access to the unique multiton instance */
VtClient_c &getVtClientInstance( uint8_t instance = 0 );

}

#endif
