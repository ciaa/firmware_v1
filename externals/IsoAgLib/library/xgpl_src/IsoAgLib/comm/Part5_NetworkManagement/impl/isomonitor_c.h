/*
  isomonitor_c.h: object for monitoring members (list of IsoItem_c)

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISO_MONITOR_H
#define ISO_MONITOR_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/scheduler/impl/schedulertask_c.h>
#include <IsoAgLib/driver/can/impl/cancustomer_c.h>
#include <IsoAgLib/driver/system/impl/system_c.h>
#include "isoitem_c.h"
#include "cfstatehandler_c.h"
#include "isorequestpgnhandler_c.h"
#include "identitem_c.h"

#include <map>

#include <list>


namespace IsoAgLib { class iIsoMonitor_c; }


namespace __IsoAgLib {

/** type of map which is used to store ControlFunctionStateHandler_c clients corresponding to a IsoName_c reference */
typedef STL_NAMESPACE::vector<ControlFunctionStateHandler_c*> ControlFunctionStateHandlerVector_t;
typedef STL_NAMESPACE::vector<ControlFunctionStateHandler_c*>::iterator ControlFunctionStateHandlerVectorIterator_t;
typedef STL_NAMESPACE::vector<ControlFunctionStateHandler_c*>::const_iterator ControlFunctionStateHandlerVectorConstIterator_t;

/** this object manages a monitor list of all
  ISO members including inserting and administration of local own members.
  @author Martin Wodok
  @author Dipl.-Inform. Achim Spangler
*/
class IsoMonitor_c : public SchedulerTask_c
{
  MACRO_MULTITON_CONTRIBUTION();
private:
  // private typedef alias names
  #ifdef OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED
  typedef STL_NAMESPACE::list<IsoItem_c,MALLOC_TEMPLATE(IsoItem_c) > Vec_ISO;
  typedef STL_NAMESPACE::list<IsoItem_c,MALLOC_TEMPLATE(IsoItem_c) >::iterator Vec_ISOIterator;
  typedef STL_NAMESPACE::list<IsoItem_c,MALLOC_TEMPLATE(IsoItem_c) >::const_iterator Vec_ISOIteratorConst;
  #else
  typedef STL_NAMESPACE::list<IsoItem_c> Vec_ISO;
  typedef STL_NAMESPACE::list<IsoItem_c>::iterator Vec_ISOIterator;
  typedef STL_NAMESPACE::list<IsoItem_c>::const_iterator Vec_ISOIteratorConst;
  #endif

public:
  void init();
  void close();

  /** register a properly set-up IdentItem.
      @return false in case the IdentItem couldn't be properly activated */
  bool registerIdentItem( IdentItem_c& arc_item );

  /** deregister a registered IdentItem */
  void deregisterIdentItem( IdentItem_c& arc_item );

  /** deliver amount of ISO members in monitor list which optionally
    match the condition of address claim state
    @param ab_forceClaimedAddress true -> only members with claimed address are used
    @return amount of ISO members with claimed address
  */
  uint8_t isoMemberCnt( bool forceClaimedAddress = false );

  /** deliver the n'th ISO member in monitor list which optionally
    match the condition of address claim state
    check first with isoMemberCnt if enough members are registered in Monitor-List
    @see isoMemberCnt
    @param index index of the wanted member in the sublist of
                 members (first index == 0) with the wanted property
    @param ab_forceClaimedAddress true -> only members with claimed address are used
    @return reference to searched element
  */
  IsoItem_c& isoMemberInd( uint8_t index, bool forceClaimedAddress = false );

  IsoItem_c& insertIsoMember(const IsoName_c& acrc_isoName, uint8_t aui8_nr,
                             IState_c::itemState_t ren_state,
                             IdentItem_c* apc_identItemForLocalItems,
                             bool ab_announceAddition);

  void registerControlFunctionStateHandler (ControlFunctionStateHandler_c & arc_client);
  void deregisterControlFunctionStateHandler (ControlFunctionStateHandler_c & arc_client);

  void broadcastIsoItemModification2Clients( ControlFunctionStateHandler_c::iIsoItemAction_e action, IsoItem_c const& acrc_isoItem ) const;
  void broadcastWsAnnounceFinished2Clients( IsoItem_c const& acrc_isoItem ) const;
  
  IsoItem_c* item( uint8_t sa ) const;
  IsoItem_c* item( const IsoName_c& name, bool forceClaimedAddress = false ) const;
  void deleteItem( const IsoItem_c& );

  IsoItem_c* isoMemberNrFast( uint8_t aui8_nr ) { return m_isoItems[ aui8_nr ]; }

  bool isWsmMember( IsoItem_c &isoItem ) { return (getMaster( isoItem ) != NULL); }
  IsoItem_c* getMaster( IsoItem_c &member );

public:
  /** check if SA of an announcing IsoItem_c is unique and deliver
    another free SA if not yet unique (else deliver its actual SA if unique yet)
    @param apc_isoItem pointer to announcing IsoItem_c
    @param ab_resolveConflict true => don't use current SA because someone else
                                      claimed it with a higher prior isoname,
                                      so we have to change our SA. This is needed
                                      as the new member is not yet in the list,
                                      so the algorithm would still take the current SA.
                                      We can't insert the new item, as we don't want
                                      a state where two items have the same SA.
                              false => no conflict to resolve, so we can take the current
                                       source address if it's available!
    @return free unique SA (if possible the actual SA of the pointed item)
      (if wanted SA is not free for NOT-self-conf item or if no free SA is available
       254 is answered -> special flag for NACK)
  */
  uint8_t unifyIsoSa(const IsoItem_c* apc_isoItem, bool ab_resolveConflict);

  /** deliver timestamp of last ISO request for SA claim msg */
  int32_t lastIsoSaRequest() const { return mi32_lastSaRequest; }

  /** set timestamp of last ISO request for SA claim msg
    @param ai32_time set timestamp to ai32_time or use actual time on default

    changed by M.Wodok so that NO system time is used,
    because REQUEST_FOR_CLAIMED_ADDRESS may be processed AFTER
    ADDRESS_CLAIM, so it's NOT chronologically. So CAN-Pkg-Times
    should be used here instead!!
  */
  void setLastIsoSaRequest (int32_t ai32_time/* = -1*/) {mi32_lastSaRequest = /*(ai32_time != -1)?*/ai32_time/*:System_c::getTime()*/;}

  /** trigger a request for claimed addreses
    @param ab_force false -> send request only if no request was detected until now
    @return true -> request was sent
   */
  bool sendRequestForClaimedAddress( bool ab_force, IsoItem_c *sender );

  void updateSaItemTable( IsoItem_c& item, bool add );

#if DEBUG_ISOMONITOR
  void debugPrintNameTable();
#endif

private:
  IsoMonitor_c();
  virtual ~IsoMonitor_c() {}

  virtual void timeEvent();
  virtual void processMsg( const CanPkg_c& arc_data );
  virtual bool processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* /*apc_isoItemSender*/, IsoItem_c* apc_isoItemReceiver, int32_t ai_requestTimestamp );

  IsoItem_c* anyActiveLocalItem() const;
  Vec_ISOIterator internalIsoItemErase( Vec_ISOIterator aiter_toErase);

private:
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

  virtual void reactOnAbort(Stream_c &arc_stream)
  {
    mt_customer.reactOnAbortDefault(arc_stream);
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

  virtual bool reactOnStreamStart(
      ReceiveStreamIdentifier_c const &ac_ident,
      uint32_t aui32_totalLen)
  {
    return mt_customer.reactOnStreamStartDefault(ac_ident, aui32_totalLen);
  }

  class CanCustomerProxy_c : public CanCustomer_c {
  public:
    typedef IsoMonitor_c Owner_t;

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
  class IsoRequestPgnHandlerProxy_c : public IsoRequestPgnHandler_c {
  public:
    typedef IsoMonitor_c Owner_t;

    IsoRequestPgnHandlerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}

    virtual ~IsoRequestPgnHandlerProxy_c() {}

  private:
    virtual bool processMsgRequestPGN(
        uint32_t aui32_pgn,
        IsoItem_c *apc_isoItemSender,
        IsoItem_c *apc_isoItemReceiver,
        int32_t ai_requestTimestamp )
    {
      return mrt_owner.processMsgRequestPGN(
          aui32_pgn,
          apc_isoItemSender,
          apc_isoItemReceiver,
          ai_requestTimestamp );
    }

    // IsoRequestPgnHandlerProxy_c shall not be copyable. Otherwise
    // the reference to the containing object would become invalid.
    IsoRequestPgnHandlerProxy_c(IsoRequestPgnHandlerProxy_c const &);

    IsoRequestPgnHandlerProxy_c &operator=(IsoRequestPgnHandlerProxy_c const &);

    Owner_t &mrt_owner;
  };
  typedef IsoRequestPgnHandlerProxy_c Handler_t;


  Vec_ISO mvec_isoMember;

  // SA IsoItem resolving
  IsoItem_c* m_isoItems[256];

  /** last time of request for adress claim */
  int32_t mi32_lastSaRequest;

  /** temporary memberItem instance for better inserting of new elements */
  IsoItem_c mc_tempIsoMemberItem;

  ControlFunctionStateHandlerVector_t mvec_saClaimHandler;

  Handler_t mt_handler;
  Customer_t mt_customer;

  CONTAINER_CLIENT1_MEMBER_FUNCTIONS_MAIN(IdentItem_c)
  friend IsoMonitor_c &getIsoMonitorInstance( unsigned int instance );
  friend class IsoAgLib::iIsoMonitor_c;
};


IsoMonitor_c &
getIsoMonitorInstance( unsigned int instance = 0 );


}

#endif
