/*
  basecommon_c.h:  base class for basa data types

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <IsoAgLib/driver/can/impl/cancustomer_c.h>
#include <IsoAgLib/driver/can/impl/canpkg_c.h>
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#include <IsoAgLib/util/iassert.h>
#include "basecommon_c.h"


// Begin Namespace __IsoAgLib
namespace __IsoAgLib {


/// NOTE: Currently all BaseCommon_c-based singletons are NOT working
///       with Multiple ISOBUS Instances
///       because getMultitonInst is needed here but declared/defined
///       in the derived classes, which are the singletons.


void
BaseCommon_c::init()
{
  isoaglib_assert (!initialized());

  getSchedulerInstance().registerTask(mt_task, 0);
  // set configure values with call for config
  config_base (NULL, IsoAgLib::IdentModeImplement, 0 /* No individual PGN disabling */);
  // now let concrete specialized classes init their part...
  init_specialized();

  setInitialized();
}

void
BaseCommon_c::close( )
{
  isoaglib_assert (initialized());

  close_specialized();
  getSchedulerInstance().deregisterTask(mt_task);

  // actually the filters aren't getting removed, but this is just a quick workaround until the Trac-classes-redesign!
  clearFilterCreated();

  setClosed();
};


bool BaseCommon_c::config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask)
{
  mui16_suppressMask = aui16_suppressMask;
  isoaglib_assert( ( at_identMode != IsoAgLib::IdentModeTractor ) || ( NULL != apc_ident ));

  // set configure values
  mpc_ident = apc_ident;
  setMode(at_identMode);

  // set the timestamps to 0
  mi32_lastMsgReceived = 0;

  //setSelectedDataSourceISOName is only used in tractor mode
  if (at_identMode == IsoAgLib::IdentModeTractor)
  {
    mc_selectedDataSourceISOName = apc_ident->isoName();
  }
  else
  { //implement mode
    mc_selectedDataSourceISOName.setUnspecified();
  }
  return true;
}

/** check if a received message should be parsed */
bool BaseCommon_c::checkParseReceived(const IsoName_c& acrc_currentSender) const
{
  return ( checkMode(IsoAgLib::IdentModeImplement) // I'm not the sender
            && ( // one of the following conditions must be true
                   (mc_selectedDataSourceISOName == acrc_currentSender  ) // actual sender equivalent to last
                || (mc_selectedDataSourceISOName.isUnspecified()         ) // last sender has not correctly claimed address member
               )
          );
}


/** functions with actions, which must be performed periodically
    -> called periodically by Scheduler_c
    ==> sends base data msg if configured in the needed rates
    possible errors:
      * dependant error in CanIo_c on CAN send problems
    <!--@param ai32_demandedExecEnd optional timestamp, where timeEvent shall return execution to calling function-->
    @see CanPkg_c::getData
    @see CanPkgExt_c::getData
    @see CanIo_c::operator<<
  */
void BaseCommon_c::timeEvent()
{
  checkCreateReceiveFilter();

  // check for different base data types whether the previously
  // sending node stopped sending -> other nodes can now step in
  if (  checkMode(IsoAgLib::IdentModeImplement)
        &&(lastedTimeSinceUpdate() >= getTimeOut( ) )
        && (mc_selectedDataSourceISOName.isSpecified())    )
  { // the previously sending node didn't send the information for 3 seconds -> give other items a chance
    mc_selectedDataSourceISOName.setUnspecified();
  }


  if ( mpc_ident && ( ! mpc_ident->isClaimedAddress() ) )
  { // local isoname for sending is registrated, but it is not yet fully claimed
    // --> nothing to do
    return;
  }

  // check if we are in tractor mode and have a pointer to the sending isoname
  if ( checkMode(IsoAgLib::IdentModeTractor) )
  { // all tractor mode timeEvent() functions have the only target to send messages,
    // now:
    // 1) getISOName() != NULL
    // 2) getSystemMgmtInstance4Comm().existLocalMemberISOName(*getISOName(), true) indicates, that a corresponding
    //    item has already performed its address claim
    // ==> we can directly call sending time event in this case
    timeEventTracMode();
    return;
  }
  else
  { // we are in implement mode
    // call this function also if isoName == NULL, because some functions do settings which are independent from isoName
    timeEventImplMode();
    return;
  }
}


/** send a ISO11783 base information PGN.
  * this is only called when sending ident is configured and it has already claimed an address
  */
void BaseCommon_c::timeEventTracMode() {}


/** send a ISO11783 base information PGN.
  * this is only called when sending ident is configured and it has already claimed an address
  */
void BaseCommon_c::timeEventImplMode() {}


/** send a PGN request */
bool BaseCommon_c::sendPgnRequest(uint32_t ui32_requestedPGN)
{
  if( ( ! getIdentItem() ) || ( ! getIdentItem()->getIsoItem() ) )
    return false;

  IsoItem_c *dest = getSelectedDataSourceISONameConst().isSpecified()
    ? getIsoMonitorInstance( getIdentItem()->getMultitonInst() ).item( getSelectedDataSourceISONameConst(), true )
    : NULL; // --> ask to global

  CanPkgExt_c pkg;
  pkg.setIsoPri(6);
  pkg.setIsoPgn(REQUEST_PGN_MSG_PGN);
  pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
  pkg.setMonitorItemForDA( dest );
  pkg.setLen( 3 );
  pkg.setUint32Data( 0 , ui32_requestedPGN );
  getIsoBusInstance( getIdentItem()->getMultitonInst() ) << pkg;
  return true;
}


/** check if preconditions for request for pgn are fullfilled
    @return  true -> the request for pgn can be send
  */
bool BaseCommon_c::check4ReqForPgn(uint32_t /* aui32_pgn */, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver)
{
  // we're not Network Management, so don't answer requests from 0xFE
  if( apc_isoItemSender == NULL )
    return false;

  if ( ( ! getIdentItem() ) || ( ! getIdentItem()->isClaimedAddress() ) )
    return false; // not claimed

  // request to global
  if( ! apc_isoItemReceiver )
    return true;

  // we're the receiver
  if( apc_isoItemReceiver == getIdentItem()->getIsoItem() )
    return true;

  return false;
}

}// end namespace __IsoAgLib
