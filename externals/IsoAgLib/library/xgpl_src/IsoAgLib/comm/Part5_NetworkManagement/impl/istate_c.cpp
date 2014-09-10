/*
  istate_c.cpp: object which manages state of item in monitor list

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "istate_c.h"


/**
  constructor of IState_c which can set the state to given initial value
  @param ren_itemState optional wanted state information (default \<empty\> value)
*/
__IsoAgLib::IState_c::IState_c(itemState_t ren_itemState, int ai_multitonInst)
	: ClientBase( ai_multitonInst )
{
  en_itemState = ren_itemState;
}

/**
  constructor of IState_c which can set the state to given initial value
  @param aui8_state optional wanted state information (default \<empty\> value)
*/
__IsoAgLib::IState_c::IState_c(uint8_t aui8_state, int ai_multitonInst )
	: ClientBase( ai_multitonInst )
{
  en_itemState = itemState_t(aui8_state);
}

/**
  copy constructor for IState_c
  @param acrc_src source for this new instance
*/
__IsoAgLib::IState_c::IState_c(const __IsoAgLib::IState_c& acrc_src)
	: ClientBase( acrc_src )
{
  en_itemState = acrc_src.en_itemState;
}



/**
  set the state of an monitor list item and
  return the resulting state value

  set state to Active, and reset OffExplicitly and OffUnable on
  Active, PreAddressClaim, AddressClaim, ClaimedAddress;

  set: PreAddressClaim, AddressClaim and ClaimedAddress exclusive

  set: OffExplicitly, OffUnable, Active exclusive
  @param ren_itemState state information
  @param ab_clearOld optional clear old value for complete new set (default no clear)
  @return resulting state information
*/
__IsoAgLib::IState_c::itemState_t __IsoAgLib::IState_c::setItemState(itemState_t ren_itemState, bool ab_clearOld)
{
  if (ab_clearOld) en_itemState = ren_itemState;
  // set state to Active, and reset OffExplicitly and OffUnable on
  // Active, PreAddressClaim, AddressClaim, ClaimedAddress
  if ((ren_itemState & (Active|PreAddressClaim|AddressClaim|ClaimedAddress|AddressLost)) > 0)
  { // one of these states set -> clear OffExplicitly and OffUnable
    clearItemState(itemState_t(OffExplicitly|OffUnable));
    // set PreAddressClaim, AddressClaim, ClaimedAddress and AddressLost exclusive
    if ((ren_itemState & (PreAddressClaim|AddressClaim|ClaimedAddress|AddressLost)) > 0)
    { // one of PreAddressClaim, AddressClaim, ClaimedAddress and AddressLost exclusive
      // clear before set
      clearItemState(itemState_t(PreAddressClaim|AddressClaim|ClaimedAddress|AddressLost|PossiblyOffline));
    }
    // now set always additionally the Active flag
    en_itemState = itemState_t(en_itemState | Active);
  }

  // set: OffExplicitly, OffUnable, Active exclusive
  if ((ren_itemState & (OffExplicitly|OffUnable)) > 0)
  { // one of OffExplicitly, OffUnable, Active (handled above)
    // clear: OffExplicitly, OffUnable, Active
    clearItemState(itemState_t(OffExplicitly|OffUnable|Active));
  }

  // now simple set the new value
  en_itemState = itemState_t(en_itemState | ren_itemState);

  return en_itemState;
};

