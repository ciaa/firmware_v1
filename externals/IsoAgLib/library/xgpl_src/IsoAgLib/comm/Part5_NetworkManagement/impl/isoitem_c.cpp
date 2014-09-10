/*
  isoitem_c.cpp: object which represents an item in a service monitor
    list

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <IsoAgLib/isoaglib_config.h>
#include <cstdio>

#include "isoitem_c.h"
// necessary for convert operators
#include "../iisoitem_c.h"
#include "isomonitor_c.h"
#include <IsoAgLib/scheduler/impl/scheduler_c.h>
#include <IsoAgLib/comm/impl/isobus_c.h>

#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif


namespace __IsoAgLib {

/** default constructor - only used in creation of IsoMonitor_c's temp member variable.
 * all data has to be initialized with a call to "set(..)"
 * afterwards the IsoItem is being inserted to the list using the copy constructor!
 * --> so these constructors are very dumb now!
 */
IsoItem_c::IsoItem_c()
  : BaseItem_c(0, IState_c::IstateNull, 0)
  #ifdef USE_WORKING_SET
  , m_wsSlavesAnnounced (NULL)
  , m_wsSlavesAnnouncing (NULL)
  , m_wsLocalSlavesToClaimAddress (0) // idle around
  , m_wsLocalLastCompletedAnnounceStartedTime (-1)
  , m_wsLocalCurrentAnnounceStartedTime (-1)
  , m_wsLocalRepeatAnnounce (false)
  , m_wsRemoteAnnounceTime (-1)
  #endif
  , mui8_nr(0xFE)
  , mpc_identItem (NULL) // per default not a local item which has a back-reference to an IdentItem
  , mc_isoName()
{
}


/** copy constructor for IsoItem
  All members are simply copied.
  This constructor is only used at construction-time.
  No IsoItems are copied later on as they're in (s)lists and not vectors..
  @param acrc_src source IsoItem_c instance
*/
IsoItem_c::IsoItem_c(const IsoItem_c& acrc_src)
  : BaseItem_c (acrc_src)
#ifdef USE_WORKING_SET
  , m_wsSlavesAnnounced (NULL)
  , m_wsSlavesAnnouncing (NULL)
  , m_wsLocalSlavesToClaimAddress (acrc_src.m_wsLocalSlavesToClaimAddress)
  , m_wsLocalLastCompletedAnnounceStartedTime (acrc_src.m_wsLocalLastCompletedAnnounceStartedTime)
  , m_wsLocalCurrentAnnounceStartedTime (acrc_src.m_wsLocalCurrentAnnounceStartedTime)
  , m_wsLocalRepeatAnnounce (acrc_src.m_wsLocalRepeatAnnounce)
  , m_wsRemoteAnnounceTime (acrc_src.m_wsRemoteAnnounceTime)
#endif
  , mui8_nr (acrc_src.mui8_nr)
  , mpc_identItem (acrc_src.mpc_identItem)
  , mc_isoName (acrc_src.mc_isoName)

{
  #ifdef USE_WORKING_SET
  isoaglib_assert( acrc_src.m_wsSlavesAnnounced == NULL );
  isoaglib_assert( acrc_src.m_wsSlavesAnnouncing == NULL );
  #endif
}


IsoItem_c::~IsoItem_c()
{
#ifdef USE_WORKING_SET
  delete m_wsSlavesAnnounced;
  delete m_wsSlavesAnnouncing;
#endif
}


/**
 * General Change Address function. Will set the Address and then call the necessary broadcast.
 */
void
IsoItem_c::changeAddressAndBroadcast (uint8_t aui8_newAddress)
{
  uint8_t ui8_adrBefore = nr();
  bool b_adrBefore = (nr() < 0xFE);
  setNr (aui8_newAddress);
  bool b_adrAfter = (nr() < 0xFE);
  if (b_adrBefore && !b_adrAfter)
  { // address lost
    setItemState (IState_c::AddressLost);
    getIsoMonitorInstance4Comm().broadcastIsoItemModification2Clients (ControlFunctionStateHandler_c::LostAddress, *this);
  }
  else if (!b_adrBefore && b_adrAfter)
  { // reclaimed address
    setItemState (IState_c::ClaimedAddress);
    getIsoMonitorInstance4Comm().broadcastIsoItemModification2Clients (ControlFunctionStateHandler_c::ReclaimedAddress, *this);
  }
  else if (b_adrBefore && b_adrAfter)
  { // changed address
    if (ui8_adrBefore != nr())
    { // only act on a real Change
      getIsoMonitorInstance4Comm().broadcastIsoItemModification2Clients (ControlFunctionStateHandler_c::ChangedAddress, *this);
    }
  }
  // else /*if*/ (!b_adrBefore && !b_adrAfter)
  // {
  //   // nothing to do in this case
  // }
}


void 
IsoItem_c::set(
    int32_t ai32_time, const IsoName_c& acrc_isoName, uint8_t aui8_nr,
    itemState_t aren_status, int ai_multitonInst )
{
  BaseItem_c::set( ai32_time, aren_status, ai_multitonInst );
  mc_isoName = acrc_isoName;
  mui8_nr = aui8_nr;
}


/// @param ab_fromConflict false => Initial Address-Claim, so we need to go to "AddressClaim"-phase!
///                        true => go to "ClaimedAddress" state, no 250ms wait (for now) as we changed SA!
/// @todo SOON-240 Do we really need this parameter? Should we wait 250ms on change of SA also??? (clarification says: NO!)
/// @todo SOON-240 Merge with sendSaClaim - create an enum for the three cases!
void IsoItem_c::sendAddressClaim (bool ab_fromConflict)
{
  if (nr() == 254)
  { // no success -> send NACK and switch to off | error state
    setItemState(IState_c::itemState_t(IState_c::OffUnable | IState_c::Error));
    clearItemState(IState_c::PreAddressClaim);
    /** @todo SOON-240 Do we need to notify the IdentItem of the OFF state? Or will it check that regularly?
              Somehow this IsoItem needs to be thrown out of the list! (New: We don't remove IsoItems anymore ;-) */
  }
  else
  { // success -> start address claim mode
    if (ab_fromConflict)
    { // we had an SA conflict and changed our SA, so let's just get another and keep "ClaimedAddress"
      setItemState(IState_c::ClaimedAddress);
    }
    else
    { // we're initially claiming an address, so we need to wait 250ms...
      setItemState(IState_c::AddressClaim);
    }
  }

  CanPkgExt_c c_pkg;
  c_pkg.setIsoPri(6);
  c_pkg.setIsoPgn(ADDRESS_CLAIM_PGN);
  c_pkg.setIsoPs( 0xFF );
  c_pkg.setIsoSa( nr() );
  c_pkg.setDataUnion(  mc_isoName.outputUnion() );
  getIsoBusInstance4Comm() << c_pkg;

  updateTime();
}



void
IsoItem_c::timeEvent()
{
  isoaglib_assert( itemState( IState_c::Local ) );
  isoaglib_assert( mpc_identItem != NULL );

  if (itemState(IState_c::PreAddressClaim))
  { // this item is in prepare address claim state -> wait for sending first adress claim
    const int32_t lastAdrRequestTime = mpc_identItem->getLastIsoSaRequestForThisItem();

    if (lastAdrRequestTime != -1)
    {
      int32_t i32_wait = 1250 + calc_randomWait();
      if ((System_c::getTime() - lastAdrRequestTime) > i32_wait)
      { // last iso adress claim request is still valid and should have been answered till now
        // check if this item is self conf
        // unifyIsoSa delivers actual SA of this item if free
        // if actual SA isn't free and item is NOT-self-conf -> NACK flag 254 is answered
        // if actual SA is not free for self-conf item
        // -> a free SA is searched and answered
        // - if no free SA is found, 254 is answered for NACK
        setNr(getIsoMonitorInstance4Comm().unifyIsoSa (this, false)); // false: We're NOT resolving a conflict here.

        sendAddressClaim (false); // false: Initial Address-Claim, so we need to go to "AddressClaim"-phase!
        // we may be "Off" here now but can't delete ourself,
        // so we check IsoItems for Off after we called their timeEvent.
        // => This way we can remove it immediately after detecting that we couldn't exist (anymore)
        getIsoMonitorInstance4Comm().updateSaItemTable( *this, true );
      }
    }
    else
    { // no adress claim request sent till now
      ( void )getIsoMonitorInstance4Comm().sendRequestForClaimedAddress( true, NULL );
      // we're forcing, so always update, no need for the return value then!
      mpc_identItem->updateLastIsoSaRequestForThisItem();
    }
  }
  else if (itemState(IState_c::AddressClaim))
  { // item in address claim mode (time between send of claim and
    // check if there is a CAN send conflict during send of adress claim
    // final acceptance of adr claim (wait for possible contention)
    if ( false )
    { // item was in address claim state and send of claim caused error
      setItemState(IState_c::PreAddressClaim);
    }
    else
    {
      if (checkTime(250))
      {
        // no conflict since sent of adress claim since 250ms
        setItemState(IState_c::ClaimedAddress);
        // now inform the ISO monitor list change clients on NEW client use
        getIsoMonitorInstance4Comm().broadcastIsoItemModification2Clients (ControlFunctionStateHandler_c::AddToMonitorList, *this);
      }
    }
  }
  #ifdef USE_WORKING_SET
  else if ( itemState(IState_c::ClaimedAddress) )
  { // do workingset-stuff if completely announced only
    if (isMaster())
    { // We're master, so check if something has to be done..
      if ( m_wsLocalSlavesToClaimAddress == 0 )
      { // 0 means successfully sent out the ws-announce.
        // So Wait. Nothing to be done here...
      }
      else
      { // <0 or >0
        bool b_sendOutWsMessage=true;

        CanPkgExt_c c_pkg;

        if ( m_wsLocalSlavesToClaimAddress < 0 ) // should be -1, but simply catch all <0 for ws-master sending
        { // Announce WS-Master
#ifdef ISOAGLIB_WORKAROUND_MUELLER_TC
/// HACK FOR MUELLER TC THAT DOES NOT REQUEST THE ADDRESS CLAIMS WHEN SENDING ITS OWN!
sendAddressClaim( true );
#endif
          m_wsLocalSlavesToClaimAddress = int8_t(m_wsSlavesAnnounced->size());

          c_pkg.setIsoPgn (WORKING_SET_MASTER_PGN);
          c_pkg.setUint8Data (0, uint8_t(m_wsLocalSlavesToClaimAddress+1));
          c_pkg.setLen8FillUpWithReserved (1);
        }
        else // it must be > 0 here now, so omit: if (m_wsLocalSlavesToClaimAddress > 0)
        { // Slave announcing needs 100ms interspace!
          if (!checkTime(100))
            b_sendOutWsMessage = false;
          else
          { // Announce WS-Slave(s)
            c_pkg.setIsoPgn (WORKING_SET_MEMBER_PGN);
            c_pkg.setDataUnion ((*m_wsSlavesAnnounced) [m_wsSlavesAnnounced->size()-m_wsLocalSlavesToClaimAddress].outputUnion());
            m_wsLocalSlavesToClaimAddress--; // claimed address for one...
          }
        }
        if (b_sendOutWsMessage)
        { // Really send it out on the bus now!
          c_pkg.setIsoPri (7);
          c_pkg.setMonitorItemForSA (this);
          getIsoBusInstance4Comm() << c_pkg;
          updateTime();

          // did we send the last message of the announce sequence?
          if (m_wsLocalSlavesToClaimAddress == 0)
          { // yes, announcing finished!
            m_wsLocalLastCompletedAnnounceStartedTime = m_wsLocalCurrentAnnounceStartedTime;
            m_wsLocalCurrentAnnounceStartedTime = -1; // no announce running right now.
            // now see if we have to repeat the sequence because it was requested while it was sent...
            if (m_wsLocalRepeatAnnounce)
            { // repeat the announce-sequence
              m_wsLocalRepeatAnnounce = false;
              (void) startWsAnnounce();
            }
          }
        }
      }
    }
  }
  #endif
}


void
IsoItem_c::processAddressClaimed(
  int32_t ai32_pkgTime,
  uint8_t aui8_pkgSa)
{
  updateTime(ai32_pkgTime);

  // check if this item is local
  if ( itemState(IState_c::Local ) )
  { // IsoItem_c::processMsg() is only called for local item, when
    // this item has higher PRIO, so that we shall reject the SA steal
    // by resending OUR SA CLAIM
    CanPkgExt_c c_pkg;
    c_pkg.setIsoSa( nr() );
    c_pkg.setIsoPri(6);
    c_pkg.setIsoPgn(ADDRESS_CLAIM_PGN);
    c_pkg.setIsoPs(255); // global information
      // set NAME to CANPkg
    c_pkg.setDataUnion( mc_isoName.outputUnion() );
      // now IsoSystemPkg_c has right data -> send
    getIsoBusInstance4Comm() << c_pkg;
  }
  else
  { // remote item
    changeAddressAndBroadcast (aui8_pkgSa);
  }
}


bool
IsoItem_c::sendSaClaim()
{
  isoaglib_assert( itemState( IState_c::Local ) );

  // See ISO 11783-5 4.5.2d): Also respond in 250ms Address-claim phase
  // even if not yet actively sending any other packets...
  if ( itemState(IState_c::PreAddressClaim) )
    return false;

  CanPkgExt_c c_pkg;
  c_pkg.setIsoPri(6);
  c_pkg.setIsoPgn(ADDRESS_CLAIM_PGN);
  c_pkg.setIsoPs(255);
  c_pkg.setIsoSa( nr() );
  c_pkg.setDataUnion( mc_isoName.outputUnion() );
  getIsoBusInstance4Comm() << c_pkg;

  return true;
}


/** calculate random wait time between 0 and 153msec. from NAME and time
  @return wait offset in msec. [0..153]
*/
uint8_t IsoItem_c::calc_randomWait()
{ // perform some calculation from NAME
  uint16_t ui16_result = uint16_t(uint16_t(mc_isoName.outputUnion()->getUint8Data(0)) * uint16_t(mc_isoName.outputUnion()->getUint8Data(1)));
  const uint16_t data2part = mc_isoName.outputUnion()->getUint8Data(2) +1;
  if( data2part != 0 )
  {
    const uint16_t divisor = uint16_t(System_c::getTime() / data2part);
    if( divisor != 0 )
      ui16_result = ui16_result / divisor;
  }
  ui16_result = uint16_t( ui16_result + uint16_t(mc_isoName.outputUnion()->getUint8Data(3)) );
  ui16_result = uint16_t( ui16_result % uint16_t(mc_isoName.outputUnion()->getUint8Data(4) + 1) );
  ui16_result = uint16_t( ui16_result - mc_isoName.outputUnion()->getUint8Data(5) );
  ui16_result = uint16_t( ui16_result * uint16_t((mc_isoName.outputUnion()->getUint8Data(6) + 1) / (mc_isoName.outputUnion()->getUint8Data(7) + 1)) );

  // divide by last uint8_t of name till offset in limit
  uint8_t ui8_divisor = uint8_t(mc_isoName.outputUnion()->getUint8Data(7) + 1);

  for (; ui16_result > 153; ui16_result = uint16_t(ui16_result/ui8_divisor) )
  {
    // Get around potential div0 errors
    ui8_divisor = uint8_t(mc_isoName.outputUnion()->getUint8Data(7) + 1);

    // Get around potential div1 eternal loop
    if (ui8_divisor == 1)
    {
      ui8_divisor++;
    }
  }

  return static_cast<uint8_t>(ui16_result); // we can cast, as it's assured above that the ui16_result is <= 153.
}


#ifdef USE_WORKING_SET
/// This is for IdentItem's setting of WS-master/slave
void
IsoItem_c::setLocalMasterSlaves (STL_NAMESPACE::vector<IsoName_c>* apvec_slaveIsoNames)
{
  // ISOAgLib doesn't allow changing the WS-definition during runtime!
  isoaglib_assert( m_wsSlavesAnnounced == NULL );

  if (apvec_slaveIsoNames)
    m_wsSlavesAnnounced = new STL_NAMESPACE::vector<IsoName_c> (*apvec_slaveIsoNames);
  else
    m_wsSlavesAnnounced = NULL;
}


void
IsoItem_c::processMsgWsMaster (uint8_t slaveCount, int32_t time )
{
  if (m_wsSlavesAnnouncing)
  {
    m_wsSlavesAnnouncing->clear();
    m_wsSlavesAnnouncing->resize( slaveCount, IsoName_c::IsoNameUnspecified() );
  }
  else
  {
    m_wsSlavesAnnouncing = new STL_NAMESPACE::vector<IsoName_c>( slaveCount, IsoName_c::IsoNameUnspecified() );
  }

  checkWsRemoteAnnouncingFinished( time );
}


void
IsoItem_c::processMsgWsMember( IsoName_c const& slaveName, int32_t time )
{
  // @todo Should we check the timestamps on receive? --> Clarify via ISO!

  // ignore WsMembers if no WsMaster sent before
  if( m_wsSlavesAnnouncing == NULL )
    return;

  for( STL_NAMESPACE::vector<IsoName_c>::iterator iter = m_wsSlavesAnnouncing->begin();
        iter != m_wsSlavesAnnouncing->end(); ++iter )
  {
    if( iter->isUnspecified() )
    { // found the next free slave-entry!
      (*iter) = slaveName;
      checkWsRemoteAnnouncingFinished( time );
      return;
    }
  }

  isoaglib_assert( !"Can't reach here. If the list was full, it should've been copied over already!" );
}


void
IsoItem_c::checkWsRemoteAnnouncingFinished( int32_t time )
{
  isoaglib_assert( m_wsSlavesAnnouncing );

  int index = m_wsSlavesAnnouncing->size()-1;
  if( ( index < 0 ) || ( m_wsSlavesAnnouncing->operator[]( index ).isSpecified() ) )
  {
    delete m_wsSlavesAnnounced;
    m_wsSlavesAnnounced = m_wsSlavesAnnouncing;
    m_wsSlavesAnnouncing = NULL;
    m_wsRemoteAnnounceTime = time;

    getIsoMonitorInstance4Comm().broadcastWsAnnounceFinished2Clients(*this);
  }
}


int32_t
IsoItem_c::startWsAnnounce()
{
  int32_t const ci32_timeNow = HAL::getTime();

  if (m_wsLocalCurrentAnnounceStartedTime < 0)
  { // no announce currently running, so start it!
    m_wsLocalCurrentAnnounceStartedTime = ci32_timeNow;
    m_wsLocalSlavesToClaimAddress = -1; // start with announcing the master!
  }
  else
  { // we're right in announcing, so delay a second one till after this one ran through
    m_wsLocalRepeatAnnounce = true;
  }

  // return the Announce-Key
  return ci32_timeNow; // which is the NOW-time!
}



/// For checking if the WS-Announce is completed use the "announce key" returned from "startWsAnnounce()".
/// Only check for valid announce keys (i.e. ai32_timeAnnounceStarted).
/// Init your announce key to -1, then you're always fine with calling this function!
bool
IsoItem_c::isWsAnnounced (int32_t ai32_timeAnnounceStarted) const
{
  if (ai32_timeAnnounceStarted < 0)
    return false;
  else
    return (m_wsLocalLastCompletedAnnounceStartedTime >= ai32_timeAnnounceStarted);
}
#endif


IsoAgLib::iIsoItem_c& IsoItem_c::toIisoItem_c()
{
  // typically would be: static_cast<IsoAgLib::iIsoItem_c&>(*this);
  // but avoiding that due to problems with IAR compiler
  return (IsoAgLib::iIsoItem_c&)(*this);
}


const IsoAgLib::iIsoItem_c& IsoItem_c::toConstIisoItem_c() const
{
  // typically would be: static_cast<const IsoAgLib::iIsoItem_c&>(*this);
  // but avoiding that due to problems with IAR compiler
  return (const IsoAgLib::iIsoItem_c&)(*this);
}


} // __IsoAgLib
