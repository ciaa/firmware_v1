/*
  isomonitor_c.cpp - object for monitoring members (list of IsoItem_c)

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "isomonitor_c.h"
#include "isorequestpgn_c.h"
#include <IsoAgLib/scheduler/impl/scheduler_c.h>
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/driver/system/impl/system_c.h>
#include <IsoAgLib/util/iliberr_c.h>
#include <IsoAgLib/util/iassert.h>

#if DEBUG_ISOMONITOR
  #include <IsoAgLib/util/impl/util_funcs.h>
  #ifdef SYSTEM_PC
    #include <iostream>
    #include <iomanip>
  #else
    #include <supplementary_driver/driver/rs232/impl/rs232io_c.h>
  #endif
  #include <IsoAgLib/util/impl/util_funcs.h>
#endif

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif


namespace __IsoAgLib {

IsoMonitor_c &
getIsoMonitorInstance( unsigned int instance )
{
  MACRO_MULTITON_GET_INSTANCE_BODY(IsoMonitor_c, PRT_INSTANCE_CNT, instance);
}


IsoMonitor_c::IsoMonitor_c() :
  SchedulerTask_c( 125, true ),
  mvec_isoMember(),
  mt_handler(*this),
  mt_customer(*this),
  CONTAINER_CLIENT1_CTOR_INITIALIZER_LIST
{
}


void
IsoMonitor_c::init()
{
  isoaglib_assert (!initialized());
  isoaglib_assert (mvec_isoMember.empty());

  mi32_lastSaRequest = -1; // not yet requested. Do NOT use 0, as the first "setLastRequest()" could (and does randomly) occur at time0 as it's called at init() time.
  mc_tempIsoMemberItem.set( 0, IsoName_c::IsoNameUnspecified(), 0xFE, IState_c::Active, getMultitonInst() );

  setPeriod( 125, false );
  getSchedulerInstance().registerTask( *this, 0 );

  CNAMESPACE::memset( &m_isoItems, 0x0, sizeof( m_isoItems ) );

  // add filter REQUEST_PGN_MSG_PGN via IsoRequestPgn_c
  getIsoRequestPgnInstance4Comm().registerPGN (mt_handler, ADDRESS_CLAIM_PGN);
#ifdef USE_WORKING_SET
  getIsoRequestPgnInstance4Comm().registerPGN (mt_handler, WORKING_SET_MASTER_PGN);
  getIsoRequestPgnInstance4Comm().registerPGN (mt_handler, WORKING_SET_MEMBER_PGN);
#endif

  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, ((ADDRESS_CLAIM_PGN)+0xFF)<<8 ), 8 );
#ifdef USE_WORKING_SET
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (WORKING_SET_MASTER_PGN<<8) ), 8 );
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (WORKING_SET_MEMBER_PGN<<8) ), 8 );
#endif

  setInitialized();
}


void
IsoMonitor_c::close()
{
  isoaglib_assert (initialized());

  isoaglib_assert( m_arrClientC1.empty() );
  isoaglib_assert( mvec_saClaimHandler.empty() );

  getSchedulerInstance().deregisterTask( *this );

  // We can clear the list of remote nodes.
  /// NOTE: We do currently NOT call "internalIsoItemErase",
  ///       because the list of SaClaimHandlers is empty anyway.
  ///       But if the erase does some more stuff, it may be needed
  ///       to call "internalIsoItemErase" for each item instead
  ///       of just clearing the container of isoMembers.
  mvec_isoMember.clear();

  getIsoRequestPgnInstance4Comm().unregisterPGN (mt_handler, ADDRESS_CLAIM_PGN);
#ifdef USE_WORKING_SET
  getIsoRequestPgnInstance4Comm().unregisterPGN (mt_handler, WORKING_SET_MASTER_PGN);
  getIsoRequestPgnInstance4Comm().unregisterPGN (mt_handler, WORKING_SET_MEMBER_PGN);
#endif

  getIsoBusInstance4Comm().deleteFilter( mt_customer, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, ((ADDRESS_CLAIM_PGN+0xFF) << 8) ) );
#ifdef USE_WORKING_SET
  getIsoBusInstance4Comm().deleteFilter( mt_customer, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, ((WORKING_SET_MASTER_PGN) << 8) ) );
  getIsoBusInstance4Comm().deleteFilter( mt_customer, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, ((WORKING_SET_MEMBER_PGN) << 8) ) );
#endif

  setClosed();
}


bool
IsoMonitor_c::registerIdentItem( IdentItem_c& arc_item )
{
  const bool cb_activationSuccess
    = arc_item.activate( getMultitonInst() );

  if (cb_activationSuccess)
  { // Could activate it, so register it!
    /// IsoMonitor_c.timeEvent() should be called from Scheduler_c in 50 ms
    setNextTriggerTime( System_c::getTime() + 50 );
    (void) registerC1 (&arc_item);
    return true;
  }
  else
  { // Couldn't activate it, so we don't register it
    return false;
  }
}


void
IsoMonitor_c::deregisterIdentItem( IdentItem_c& arc_item )
{
  arc_item.deactivate();
  unregisterC1 (&arc_item);
}


void 
IsoMonitor_c::timeEvent()
{
  int32_t i32_checkPeriod = 3000;
  #ifdef OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED
  for ( STL_NAMESPACE::vector<__IsoAgLib::IdentItem_c*,MALLOC_TEMPLATE(__IsoAgLib::IdentItem_c*)>::iterator pc_iter = m_arrClientC1.begin(); ( pc_iter != m_arrClientC1.end() ); ++pc_iter )
  #else
  for ( STL_NAMESPACE::vector<__IsoAgLib::IdentItem_c*>::iterator pc_iter = m_arrClientC1.begin(); ( pc_iter != m_arrClientC1.end() ); ++pc_iter )
  #endif
  { // call timeEvent for each registered client -> if timeEvent of item returns false
    // it had to return BEFORE its planned activities were performed (because of the registered end time)
    (*pc_iter)->timeEvent();
    /// @todo SOON-240 Adapt the check on itemState. Check if 0x7C is correct...
    switch( (*pc_iter)->itemState() & 0x7C )
    {
    case IState_c::AddressClaim | IState_c::Active:
      if (i32_checkPeriod > 10) i32_checkPeriod = 10;
      break;

    case IState_c::ClaimedAddress | IState_c::Active:
      #ifdef USE_WORKING_SET
      if ((*pc_iter)->getIsoItem()->isWsAnnouncing())
      { // we need 100ms for WS-Announce Sequence!
        if (i32_checkPeriod > 100) i32_checkPeriod = 100;
        break;
      }
      #endif
      if (i32_checkPeriod > 1000) i32_checkPeriod = 1000;
      break;

    // do not change period
    case IState_c::PreAddressClaim | IState_c::Active: // shouldn't happen. after timeEvent we cannot be any longer PreAddressClaim
    case IState_c::OffExplicitly:
    case IState_c::OffUnable:
    default:
      // nothing to to do stay on 3000 ms timePeriod
      break;
    }

  }
  // new TimePeriod is necessary - change it
  if (i32_checkPeriod != getPeriod())
    setPeriod(i32_checkPeriod, false);

  /// We have now: (HT=HardTiming, ST=SoftTiming)
  /// At least one IdentItem
  /// - that has state AddressClaim: 150ms HT
  /// - ClaimedAddress, WS claiming: 100ms HT
  /// - ClaimedAddress, ws claimed: 1000ms ST
  /// otherwise idling around with: 3000ms ST
  /// @todo ON REQUEST: improve later to have the IdentItems give back fix timestamps. If you, use Hard-Timing and wait for exactly this timestamp
  ///       if not so, use soft-timing and idle around...

#if CONFIG_ISO_ITEM_MAX_AGE > 0
  if ( lastIsoSaRequest() != -1)
  {  
    IsoItem_c *someActiveLocalMember = anyActiveLocalItem();
#if SA_REQUEST_PERIOD_MSEC > 0
    if( ( ( HAL::getTime() - lastIsoSaRequest() ) > SA_REQUEST_PERIOD_MSEC ) && someActiveLocalMember )
      ( void ) sendRequestForClaimedAddress( true, someActiveLocalMember );
#endif

    const int32_t ci32_timeSinceLastAdrClaimRequest = ( System_c::getTime() - lastIsoSaRequest());
    bool b_requestAdrClaim = false;
    if ( ci32_timeSinceLastAdrClaimRequest > CONFIG_ISO_ITEM_MAX_AGE )
    { // the last request is more than CONFIG_ISO_ITEM_MAX_AGE ago
      // --> each client MUST have answered until now if it's still alive
      for(Vec_ISOIterator pc_iter = mvec_isoMember.begin(); pc_iter != mvec_isoMember.end();)
      { // delete item, if it didn't answer longer than CONFIG_ISO_ITEM_MAX_AGE since last adress claim request
        if ( ( (pc_iter->lastTime()+CONFIG_ISO_ITEM_MAX_AGE) < lastIsoSaRequest() )
          && ( !(pc_iter->itemState(IState_c::Local))   ) )
        { // its last AdrClaim is too old - it didn't react on the last request
          // was it too late for the first time??
          // special case: when the rate of ReqForAdrClaimed is at about CONFIG_ISO_ITEM_MAX_AGE
          // a node migth answer just in time to the _previous_ request, which is in turn _before_ the last
          // detected request on BUS
          // -->> regard the client only as stale, when the last AdrClaim was longer than CONFIG_ISO_ITEM_MAX_AGE
          //      before the newest ReqForAdrClaimed on BUS
          if ( pc_iter->itemState( IState_c::PossiblyOffline) )
          { // it's too late the second time -> remove it
            Vec_ISOIterator pc_iterDelete = pc_iter;
            pc_iter = internalIsoItemErase (pc_iterDelete); // erase returns iterator to next element after the erased one
          }
          else
          { // give it another chance
            pc_iter->setItemState( IState_c::PossiblyOffline );
            ++pc_iter;
            b_requestAdrClaim = true;
          }
        } else {
          ++pc_iter;
        }
      } // for
      if ( b_requestAdrClaim )
      { // at least one node needs an additional adr claim
        ( void )sendRequestForClaimedAddress( true, someActiveLocalMember );
        // we're forcing, so no need for the return value
      }
    } // if
  }
  #endif
}

#ifdef USE_WORKING_SET
IsoItem_c*
IsoMonitor_c::getMaster( IsoItem_c &member )
{
  // quick check
  if ( member.isMaster() )
    return &member;

  for( Vec_ISOIterator iter = mvec_isoMember.begin();
       iter != mvec_isoMember.end();
       ++iter )
  {
    if( (*iter).isMaster() )
    {
      STL_NAMESPACE::vector<IsoName_c>* wsSlaves = (*iter).getVectorOfClients();
      if (wsSlaves == NULL)
        continue;

      for( STL_NAMESPACE::vector<IsoName_c>::iterator memberIter = wsSlaves->begin();
           memberIter != wsSlaves->end();
           ++memberIter )
      {
        if ( *memberIter == member.isoName() )
        { // found master to this
          return &( *iter );
        }
      }
    }
  }
  // master of client cannot be found -> standalone client
  return NULL;
}
#endif


uint8_t
IsoMonitor_c::isoMemberCnt( bool forceClaimedAddress )
{
  uint8_t result = 0;
  for( Vec_ISOIterator iter = mvec_isoMember.begin();
       iter != mvec_isoMember.end();
       ++iter)
  {
    if ( !forceClaimedAddress || iter->itemState( IState_c::ClaimedAddress ) )
      ++result;
  }
  return result;
}


IsoItem_c&
IsoMonitor_c::isoMemberInd( uint8_t index, bool forceClaimedAddress )
{
  int8_t c_cnt = -1;
  for( Vec_ISOIterator iter = mvec_isoMember.begin();
       iter != mvec_isoMember.end();
       ++iter )
  {
    if( !forceClaimedAddress || iter->itemState( IState_c::ClaimedAddress ) )
    {
      ++c_cnt;
      if( c_cnt == index )
        return *iter;
    }
  }

  isoaglib_assert( !"IsoMonitor_c::isoMemberInd called with out of bound index!" );
  return mvec_isoMember.front();
}


IsoItem_c &
IsoMonitor_c::insertIsoMember(
  const IsoName_c& acrc_isoName,
  uint8_t aui8_nr,
  IState_c::itemState_t ren_state,
  IdentItem_c* apc_identItemForLocalItems,
  bool ab_announceAddition )
{
  isoaglib_assert( item( acrc_isoName ) == NULL );

  // prepare temp item with wanted data
  mc_tempIsoMemberItem.set (System_c::getTime(), // Actually this value/time can be anything. The time is NOT used in PreAddressClaim and when entering AddressClaim it is being set correctly!
    acrc_isoName, aui8_nr, IState_c::itemState_t(ren_state | IState_c::Active), getMultitonInst() );
  // if it's a local item, we need to set the back-reference.
  if (apc_identItemForLocalItems)
    mc_tempIsoMemberItem.setIdentItem(*apc_identItemForLocalItems);

  // now insert element
  mvec_isoMember.push_front(mc_tempIsoMemberItem);
  IsoItem_c &insertedItem = *mvec_isoMember.begin();

  if( ren_state & ( IState_c::AddressClaim | IState_c::ClaimedAddress ) ) {
    // update lookup
    updateSaItemTable( insertedItem, true );
  }

  if (ab_announceAddition)
  { // immediately announce addition.
    // only not do this if you insert a local isoitem that is in state "AddressClaim" - it will be done there if it changes its state to "ClaimedAddress".
    broadcastIsoItemModification2Clients( ControlFunctionStateHandler_c::AddToMonitorList, insertedItem );
  }

  return insertedItem;
}


IsoItem_c *
IsoMonitor_c::anyActiveLocalItem() const
{
  for( const_iterC1_t iter = m_arrClientC1.begin();
       iter != m_arrClientC1.end();
       ++iter )
  {
    if ((*iter)->itemState(IState_c::ClaimedAddress))
    {
      IsoItem_c* pc_monitorItem = (*iter)->getIsoItem();
      if( pc_monitorItem )
        return pc_monitorItem;
    }
  }
  return NULL;
}


void
IsoMonitor_c::registerControlFunctionStateHandler( ControlFunctionStateHandler_c & arc_client )
{
  for ( ControlFunctionStateHandlerVectorConstIterator_t iter = mvec_saClaimHandler.begin(); iter != mvec_saClaimHandler.end(); ++iter )
  { // check if it points to the same client
    if ( *iter == &arc_client )
      return; // already in multimap -> don't insert again
  }

  mvec_saClaimHandler.push_back( &arc_client );

  // now: trigger suitable ControlFunctionStateHandler_c calls for all already known IsoNames in the list
  for ( Vec_ISOIteratorConst iter = mvec_isoMember.begin(); iter != mvec_isoMember.end(); ++iter)
  { // inform this ControlFunctionStateHandler_c on existance of the ISONAME node at iter
    arc_client.reactOnIsoItemModification (ControlFunctionStateHandler_c::AddToMonitorList, *iter);
  }
}


void
IsoMonitor_c::deregisterControlFunctionStateHandler (ControlFunctionStateHandler_c & arc_client)
{
  for ( ControlFunctionStateHandlerVectorIterator_t iter = mvec_saClaimHandler.begin(); iter != mvec_saClaimHandler.end(); ++iter )
  { // check if it points to the same client
    if ( *iter == &arc_client )
    {
      mvec_saClaimHandler.erase (iter); // in multimap -> so delete it
      break;
    }
  }
}


void
IsoMonitor_c::broadcastIsoItemModification2Clients( ControlFunctionStateHandler_c::iIsoItemAction_e at_isoItemModification, IsoItem_c const& acrc_isoItem ) const
{
  for ( ControlFunctionStateHandlerVectorConstIterator_t iter = mvec_saClaimHandler.begin(); iter != mvec_saClaimHandler.end(); ++iter )
  { // call the handler function of the client
    (*iter)->reactOnIsoItemModification (at_isoItemModification, acrc_isoItem);
  }
}

void
IsoMonitor_c::broadcastWsAnnounceFinished2Clients( IsoItem_c const& acrc_isoItem ) const
{
  for ( ControlFunctionStateHandlerVectorConstIterator_t iter = mvec_saClaimHandler.begin(); iter != mvec_saClaimHandler.end(); iter++ )
  {
    (*iter)->reactOnWsAnnounceFinished (acrc_isoItem);
  }    
}


IsoItem_c *
IsoMonitor_c::item( const IsoName_c& acrc_isoName, bool ab_forceClaimedAddress ) const
{
  for( Vec_ISOIteratorConst iter = mvec_isoMember.begin();
       iter != mvec_isoMember.end();
       ++iter )
  {
    if( ( iter->isoName() == acrc_isoName )
     && ( !ab_forceClaimedAddress || iter->itemState( IState_c::ClaimedAddress ) )
      )
      return const_cast<IsoItem_c*>( &( *iter ) );
  }
  return NULL;
}


IsoItem_c*
IsoMonitor_c::item( uint8_t sa ) const
{
  for( Vec_ISOIteratorConst iter = mvec_isoMember.begin();
       iter != mvec_isoMember.end();
       ++iter )
  {
    if( iter->nr() == sa )
      return const_cast<IsoItem_c*>( &( *iter ) );
  }
  return NULL;
}


void
IsoMonitor_c::deleteItem( const IsoItem_c& isoitem )
{
  for( Vec_ISOIterator iter = mvec_isoMember.begin();
       iter != mvec_isoMember.end();
       ++iter )
  {
    if( &(*iter) == &isoitem )
    {
      ( void )internalIsoItemErase( iter );
      return;
    }
  }
}


bool
isAddressFree( const IsoItem_c* apc_isoItem, const STL_NAMESPACE::list<IsoItem_c>& vec_isoMember, uint8_t address, bool ab_resolveConflict )
{
  for (STL_NAMESPACE::list<IsoItem_c>::const_iterator pc_iterItem = vec_isoMember.begin();
        pc_iterItem != vec_isoMember.end(); ++pc_iterItem)
  {
    if ((pc_iterItem->nr() == address)
         && (ab_resolveConflict || (&(*pc_iterItem) != apc_isoItem))
       )
    { // the tried SA is already used by this item
      return false;
    }
  }
  return true;
}

uint8_t
IsoMonitor_c::unifyIsoSa(const IsoItem_c* apc_isoItem, bool ab_resolveConflict)
{
  const uint8_t scui8_addressRangeLower = 0x80; // 128
  const uint8_t scui8_addressRangeHigher = 0xEE; // 238
  const uint8_t scui8_noAddressFound = 0xFE;

  uint8_t ui8_wishSa = apc_isoItem->nr();
  if ( (ui8_wishSa < scui8_noAddressFound) && (isAddressFree(apc_isoItem, mvec_isoMember, ui8_wishSa, ab_resolveConflict)) )
  { // address is FREE: use it
    return ui8_wishSa;
  }
  //else: address not available
  const bool cb_selfConf = apc_isoItem->isoName().selfConf();
  if (!cb_selfConf)
  { // not configurable -> no need to loop free address
    return scui8_noAddressFound;
  }
  else
  { // self-configurable
    if ( (ui8_wishSa >= scui8_addressRangeLower) &&
         (ui8_wishSa <= scui8_addressRangeHigher) )
    { // address already in dynamic address range,
      // search on from the wished address
    }
    else
    { // moved to dynamic address range
      ui8_wishSa = scui8_addressRangeLower;
    }

    const uint8_t stopSa = ui8_wishSa;
    do
    { // try the current ui8_wishSa
      if (isAddressFree(apc_isoItem, mvec_isoMember, ui8_wishSa, ab_resolveConflict))
      { // address is FREE: use it
        return ui8_wishSa;
      }
      else
      {
        ++ui8_wishSa;
        if (ui8_wishSa >= scui8_addressRangeHigher)
          ui8_wishSa = scui8_addressRangeLower;
      }
    } while (ui8_wishSa != stopSa);
    // completely looped one time, but no free address found
    return scui8_noAddressFound;
  }
}


bool
IsoMonitor_c::sendRequestForClaimedAddress( bool ab_force, IsoItem_c *sender )
{ // trigger an initial request for claimed address
  // ( only if no request was detected )
  if ( ( lastIsoSaRequest() != -1 ) && ( ! ab_force ) )
  { // at least one request was already detected
    return false;
  }
  CanPkgExt_c c_data;

  c_data.setIsoPri(6);
  c_data.setIsoPgn(REQUEST_PGN_MSG_PGN);
  c_data.setIsoPs(255); // global request
  if ( sender )
    c_data.setMonitorItemForSA( sender );
  else
    c_data.setIsoSa( 0xFE );
  c_data.setLen(3);
  c_data.setUint32Data( 0, ADDRESS_CLAIM_PGN );
  getIsoBusInstance4Comm() << c_data;

  setLastIsoSaRequest( HAL::getTime() );

  // let local items answer, too!
  for( const_iterC1_t iter = m_arrClientC1.begin(); 
       iter != m_arrClientC1.end();
       ++iter )
  {
    IsoItem_c *localItem = (*iter)->getIsoItem();
    if( localItem )
      localItem->sendSaClaim();
  }
  return true;
}


void
IsoMonitor_c::processMsg( const CanPkg_c& arc_data )
{
#if DEBUG_ISOMONITOR
  INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_ENDL << "IsoMonitor_c::processMsg()-BEGIN" << INTERNAL_DEBUG_DEVICE_ENDL;
  debugPrintNameTable();
#endif

  CanPkgExt_c pkg( arc_data, getMultitonInst() );

  const IsoName_c cc_dataIsoName (pkg.getDataUnionConst());

  // Special NETWORK-MANAGEMENT Handling of ADDRESS_CLAIM_PGN
  // don't do the generic "valid-resolving" check here!
  if( (pkg.isoPgn() & 0x3FF00LU) == ADDRESS_CLAIM_PGN )
  {
    const int32_t ci32_time = pkg.time();
    const uint8_t cui8_sa = pkg.isoSa();

    IsoItem_c *pc_itemSameISOName = item( cc_dataIsoName );
    if( pc_itemSameISOName != NULL )
    {
      if (pc_itemSameISOName->itemState(IState_c::PreAddressClaim))
        // no need to check here for LostAddress, as it's only about the ISOName,
        // and that's correct in all other cases!
      { // this item is still in PreAddressClaim, so don't consider its
        // ISOName as final, it may be able to adapt it when switching to AddressClaim
        // Note: Only LOCAL Items can be in state PreAddressClaim
        pc_itemSameISOName = NULL;
      }
    }

    IsoItem_c *pc_itemSameSa = item( cui8_sa );
    if( pc_itemSameSa != NULL )
    {
      if (pc_itemSameSa->itemState(IState_c::PreAddressClaim)
       || pc_itemSameSa->itemState(IState_c::AddressLost) )
      { // this item has no valid address, as it's not (anymore) active.
        // so don't consider it as item with the same SA as the received one.
        pc_itemSameSa = NULL;
      }
    }

    /// Receiving REMOTE Address-Claim
    /// ##############################

    if (NULL == pc_itemSameISOName)
    { // We have NO item with this IsoName
      /// Insert this new remote node (new isoname). Just check before if it steals a SA from someone
      if (NULL == pc_itemSameSa)
      { // New remote node took a fresh SA. The way it should be. Insert it to the list.
        insertIsoMember (cc_dataIsoName, cui8_sa, IState_c::ClaimedAddress, NULL, true);
      }
      else
      { // New remote node stole a SA. Check if it stole from local or remote.
        if (pc_itemSameSa->itemState(IState_c::Local))
        { /// New remote node steals SA from Local node!
          // --> change address if it has lower PRIO
          if (pc_itemSameSa->isoName() < cc_dataIsoName)
          { // the LOCAL item has lower PRIO
            if (pc_itemSameSa->itemState(IState_c::AddressClaim))
            { // the LOCAL item was still in AddressClaim (250ms) phase
              pc_itemSameSa->setNr (unifyIsoSa (pc_itemSameSa, true));
              // No need to broadcast anything, we didn't yet even call AddToMonitorList...
              pc_itemSameSa->sendAddressClaim (false); // false: Address-Claim due to SA-change on conflict **while 250ms-phase** , so we can skip the "AddressClaim"-phase!
            }
            else
            { // the LOCAL item is already up and running, so simply change the SA, claim again and go on
              pc_itemSameSa->changeAddressAndBroadcast (unifyIsoSa (pc_itemSameSa, true));
              pc_itemSameSa->sendAddressClaim (true); // true: Address-Claim due to SA-change on conflict **after 250ms-phase**, so we can skip the "AddressClaim"-phase!
            }

            if (pc_itemSameSa->nr() == 254)
            { // Couldn't get a new address -> remove the item and let IdentItem go to OffUnable!
              if (pc_itemSameSa->getIdentItem())
              { // as it should be! as it's local!
                pc_itemSameSa->getIdentItem()->goOffline(false); // false: we couldn't get a new address for this item!
              }
            } else {
              updateSaItemTable( *pc_itemSameSa, true );
            }
            insertIsoMember (cc_dataIsoName, cui8_sa, IState_c::ClaimedAddress, NULL, true);
          }
          else
          { // let local IsoItem_c process the conflicting adr claim
            // --> the IsoItem_c::processMsg() will send an ADR CLAIM to indicate the higher prio
            pc_itemSameSa->processAddressClaimed (ci32_time, cui8_sa);
            insertIsoMember (cc_dataIsoName, 0xFE, IState_c::AddressLost, NULL, true);
            /// ATTENTION: We insert the IsoName WITHOUT a valid Address. (and even notify the registered clients about it!)
            /// But this may also happen anyway if you register your handler at a later time -
            /// then your handler will be called with "AddToMonitorList" for all yet known IsoItems -
            /// and those don't need to have a valid SA at this moment!!
          }
        }
        else
        { /// New remote node steals SA from Remote node!
          pc_itemSameSa->giveUpAddressAndBroadcast();
          insertIsoMember (cc_dataIsoName, cui8_sa, IState_c::ClaimedAddress, NULL, true);
        }
      }
    }
    else
    { // We already have an item with this IsoName
      if (pc_itemSameISOName->itemState(IState_c::Local))
      { // We have a local item with this IsoName
        isoaglib_assert( pc_itemSameISOName->getIdentItem() );
        pc_itemSameISOName->getIdentItem()->goOffline( false ); // false: we couldn't get a new address for this item!
        IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::MonitorNameRxConflict, getMultitonInst() );
        // now create a new node for the remote SA claim
        insertIsoMember (cc_dataIsoName, cui8_sa, IState_c::ClaimedAddress, NULL, true);
      }
      else
      { // We have a remote item with this IsoName
        /// Change SA of existing remote node. Just check before if it steals a SA from someone
        if (NULL == pc_itemSameSa)
        { // (A9) Existing remote node took a fresh SA. The way it should be. Just change its address.
          updateSaItemTable( *pc_itemSameISOName, false );
          pc_itemSameISOName->processAddressClaimed (ci32_time, cui8_sa);
          updateSaItemTable( *pc_itemSameISOName, true );
        }
        else
        { // Existing remote node took an already existing SA.
          if (pc_itemSameSa == pc_itemSameISOName)
          { // (A1) Existing remote node reclaimed its SA, so it's just a repeated address-claim.
            pc_itemSameISOName->processAddressClaimed (ci32_time, cui8_sa); // only call to update the timestamp basically
          }
          else if (pc_itemSameSa->itemState(IState_c::Local))
          { // (A5) Existing remote node steals SA from Local node!
            // --> change address if it has lower PRIO
            if (pc_itemSameSa->isoName() < cc_dataIsoName)
            { // the LOCAL item has lower PRIO
              if (pc_itemSameSa->itemState(IState_c::AddressClaim))
              { // the LOCAL item was still in AddressClaim (250ms) phase
                pc_itemSameSa->setNr (unifyIsoSa (pc_itemSameSa, true));
                // No need to broadcast anything, we didn't yet even call AddToMonitorList...
                pc_itemSameSa->sendAddressClaim (false); // false: Address-Claim due to SA-change on conflict **while 250ms-phase** , so we can skip the "AddressClaim"-phase!
              }
              else
              { // the LOCAL item is already up and running, so simply change the SA, claim again and go on
                pc_itemSameSa->changeAddressAndBroadcast (unifyIsoSa (pc_itemSameSa, true));
                pc_itemSameSa->sendAddressClaim (true); // true: Address-Claim due to SA-change on conflict **after 250ms-phase**, so we can skip the "AddressClaim"-phase!
              }

              if (pc_itemSameSa->nr() == 254)
              { // Couldn't get a new address -> remove the item and let IdentItem go to OffUnable!
                if (pc_itemSameSa->getIdentItem())
                { // as it should be! as it's local!
                  pc_itemSameSa->getIdentItem()->goOffline(false); // false: we couldn't get a new address for this item!
                }
              } else {
                /* we could change our SA -> update */
                updateSaItemTable( *pc_itemSameSa, true );
              }
              pc_itemSameISOName->processAddressClaimed (ci32_time, cui8_sa);
              updateSaItemTable( *pc_itemSameISOName, true );
            }
            else
            { // let local IsoItem_c process the conflicting adr claim
              // --> the IsoItem_c::processMsg() will send an ADR CLAIM to indicate the higher prio
              pc_itemSameSa->processAddressClaimed (ci32_time, cui8_sa);
              updateSaItemTable( *pc_itemSameISOName, false ); // update table before item get FE
              pc_itemSameISOName->giveUpAddressAndBroadcast();
            }
          }
          else
          { // (A3) Existing remote node steals other remote node's SA
            pc_itemSameSa->giveUpAddressAndBroadcast();
            pc_itemSameISOName->processAddressClaimed (ci32_time, cui8_sa); // will set the new SA and do broadcasting
            updateSaItemTable( *pc_itemSameISOName, true );
          }
        }
      }
    }
  }
  else
  {
    // for all following modules, we do the "typical" "valid-resolving"-check!
    if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
      return;

#ifdef USE_WORKING_SET
    // Handle NON-DESTINATION PGNs
    switch ((pkg.isoPgn() /* & 0x3FFFF */ )) // isoPgn is already "& 0x3FFFF" !
    {
      case WORKING_SET_MASTER_PGN:
        pkg.getMonitorItemForSA()->processMsgWsMaster (uint8_t(pkg.getUint8Data(1-1) - 1), pkg.time() );
      break;

      case WORKING_SET_MEMBER_PGN:
        pkg.getMonitorItemForSA()->processMsgWsMember (cc_dataIsoName, pkg.time());
      break;

      default:
        break;
    } // end switch for NON-DESTINATION pgn
#endif
  }

#if DEBUG_ISOMONITOR
  INTERNAL_DEBUG_DEVICE << "IsoMonitor_c::processMsg()-END" << INTERNAL_DEBUG_DEVICE_ENDL;
  debugPrintNameTable();
#endif
}


bool
IsoMonitor_c::processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t ai_requestTimestamp )
{
  // ADDRESS_CLAIM_PGN is NETWORK-MANAGEMENT, so SA=0xFE is allowed
  if( aui32_pgn == ADDRESS_CLAIM_PGN )
  {
    if (apc_isoItemReceiver == NULL)
    { // No specific destination so it's broadcast: Let all local item answer!
      // update time of last GLOBAL adress claim request to detect dead nodes
      setLastIsoSaRequest (ai_requestTimestamp); // Now using CAN-Pkg-Times, see header for "setLastIsoSaRequest" for more information!

      bool b_processedRequestPGN = false;
      for (Vec_ISOIterator pc_iterItem = mvec_isoMember.begin();
        pc_iterItem != mvec_isoMember.end(); ++pc_iterItem)
      { // let all local pc_iterItem process this request
        bool const cb_set = pc_iterItem->itemState (IState_c::Local) &&
          pc_iterItem->sendSaClaim();
        if (cb_set)
          b_processedRequestPGN = true;
      }
      return b_processedRequestPGN; //return value doesn't matter, because the request was for GLOBAL (255), so it isn't NACKed anyway
    }
    else
    { // ISORequestPGN ensured that the Item exists and is local: Let it process!
      return apc_isoItemReceiver->sendSaClaim();
    }
  }
  
  // for all other PGNs, SA=0xFE is not allowed, enforce a valid sender!
  
  // we're not Network Management, so don't answer requests from 0xFE
  if( apc_isoItemSender == NULL )
    return false;

#ifdef USE_WORKING_SET
  if( ( aui32_pgn == WORKING_SET_MASTER_PGN ) || ( aui32_pgn == WORKING_SET_MEMBER_PGN ) )
  {
      if (apc_isoItemReceiver == NULL)
      { // No specific destination so it's broadcast: Let all local item answer!
        for (Vec_ISOIterator pc_iterItem = mvec_isoMember.begin();
              pc_iterItem != mvec_isoMember.end(); ++pc_iterItem)
        { // let all local pc_iterItem process process this request
          if (pc_iterItem->itemState (IState_c::Local))
          {
            if (pc_iterItem->isMaster())
            { // is Master, so send out the ws-announce. If it's busy now, it will enqueue the request
              (void) pc_iterItem->startWsAnnounce(); // so it's definitely being sent out.
            }
            else
            { // no master, can't send out these PGNs
              // ==> NACK, but not when request went to GLOBAL
            }
          }
        }
        return true; //return value doesn't matter, because the request was for GLOBAL (255), so it isn't NACKed anyway
      }
      else
      { // Let the given local item answer
        if (apc_isoItemReceiver->isMaster())
        { // is Master, so can it send out the ws-announce?
          (void) apc_isoItemReceiver->startWsAnnounce();
          return true;
        }
        else
        { // no master, can't send out these PGNs
          return false; // let it get NACKed
        }
      }
	 
  }
#endif
  // shouldn't happen as we only registered for the above handled PGNs
  return false;
}


IsoMonitor_c::Vec_ISOIterator
IsoMonitor_c::internalIsoItemErase( Vec_ISOIterator aiter_toErase )
{
  // first inform SA-Claim handlers on SA-Loss
  /// @todo SOON-240 We need to get sure that the IdentItem doesn't have a dangling reference to this IsoItem!
  broadcastIsoItemModification2Clients (ControlFunctionStateHandler_c::RemoveFromMonitorList, *aiter_toErase);

  m_isoItems[ aiter_toErase->nr() ] = 0x0;
  return mvec_isoMember.erase( aiter_toErase );
}


void
IsoMonitor_c::updateSaItemTable( IsoItem_c& isoItem, bool add ) {
  if( isoItem.nr() < 0xFE ) {
    m_isoItems[ isoItem.nr() ] = add ? &isoItem : 0x0;
  }
}


#if DEBUG_ISOMONITOR
void
IsoMonitor_c::debugPrintNameTable()
{
  INTERNAL_DEBUG_DEVICE << "IsoMonitor-NAME/SA-Table - Time:" << HAL::getTime() << INTERNAL_DEBUG_DEVICE_ENDL;
  for (Vec_ISOIterator iter = mvec_isoMember.begin();
       iter != mvec_isoMember.end();
       ++iter)
  {
    INTERNAL_DEBUG_DEVICE << "   NAME (LE as on CAN): " 
#ifdef SYSTEM_PC
        << std::hex << std::setfill('0')
        << std::setw(2) << int(iter->isoName().outputString()[0]) << " "
        << std::setw(2) << int(iter->isoName().outputString()[1]) << " "
        << std::setw(2) << int(iter->isoName().outputString()[2]) << " "
        << std::setw(2) << int(iter->isoName().outputString()[3]) << " "
        << std::setw(2) << int(iter->isoName().outputString()[4]) << " "
        << std::setw(2) << int(iter->isoName().outputString()[5]) << " "
        << std::setw(2) << int(iter->isoName().outputString()[6]) << " "
        << std::setw(2) << int(iter->isoName().outputString()[7]) << " "
        << " --> SA: "
        << std::setw(2) << int (iter->nr())
        << std::dec << std::endl;
#else
        << int(iter->isoName().outputString()[0]) << " "
        << int(iter->isoName().outputString()[1]) << " "
        << int(iter->isoName().outputString()[2]) << " "
        << int(iter->isoName().outputString()[3]) << " "
        << int(iter->isoName().outputString()[4]) << " "
        << int(iter->isoName().outputString()[5]) << " "
        << int(iter->isoName().outputString()[6]) << " "
        << int(iter->isoName().outputString()[7]) << " "
        << " --> SA: "
        << int (iter->nr())
        << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
  }
}
#endif

} // __IsoAgLib
