/*
  addresstracker.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <logenvirons.h>
#include <addresstracker.h>
#include <iomanip>


AddressTracker_c::IsoName_c::IsoName_c(std::vector< uint8_t > avec_data)
{
  mvec_data.swap(avec_data);
  mvec_data.resize(8, 0xFF);
}


AddressTracker_c::Attributes_t::iterator
AddressTracker_c::attributesAt(size_t t_index)
{
  Attributes_t::iterator it = t_attributes.begin();
  std::advance(it, t_index);
  return it;
}


std::string
AddressTracker_c::requestForAddressClaimed(PtrDataFrame_t at_ptrFrame)
{
  size_t ct_dest = at_ptrFrame->destinationAddress();
  std::pair< Attributes_t::iterator, Attributes_t::iterator > range =
    (0xFF == ct_dest) ?
    std::make_pair( t_attributes.begin(), t_attributes.end() ) :
    std::make_pair( attributesAt(ct_dest), attributesAt(ct_dest + 1) );

  for (Attributes_t::iterator it = range.first; it < range.second; ++it) {
    it->mui64_nextTimeout = at_ptrFrame->time() + 1250u;
    if (addressState_won == it->e_addressState) {
      // rembember last won ISO name:
      it->mc_lastWonIsoName = it->mc_isoName;
    }
    it->mc_isoName = IsoName_c();
    it->e_addressState = addressState_awaitAddressClaimed;
  }

  return "";
}


std::string
AddressTracker_c::addressClaimed(PtrDataFrame_t at_ptrFrame)
{
  size_t ct_source = at_ptrFrame->sourceAddress();
  Attributes_t::iterator it = attributesAt(ct_source);
  IsoName_c c_name = at_ptrFrame->data();
  bool b_setOrReplace = false;
  switch (it->e_addressState) {    
  case addressState_idle:
  case addressState_awaitAddressClaimed:
    b_setOrReplace = true;
    break;
  case addressState_preliminary:
  case addressState_won:
    b_setOrReplace = c_name < it->mc_isoName;
    break;
  }

  if (b_setOrReplace) {
    it->mc_isoName = c_name;
    it->mui64_nextTimeout = at_ptrFrame->time() + 250u;
    it->e_addressState = addressState_preliminary;
  }

  return "";
}


void
AddressTracker_c::checkTimeouts( std::ostream& out, uint64_t aui64_time)
{
  for (Attributes_t::iterator it = t_attributes.begin(); it < t_attributes.end(); ++it)
    switch (it->e_addressState) {
    case addressState_awaitAddressClaimed:
      if (it->mui64_nextTimeout <= aui64_time) {
        // timeout happened
        if (!it->mc_isoName.isEmpty()) {
          // non-empty name is becoming empty
          out << " Address " << std::hex << std::distance(t_attributes.begin(), it) << " no longer claimed ";
        }
        it->mui64_nextTimeout = 0u;
        it->mc_isoName = IsoName_c();
        it->e_addressState = addressState_idle;
      }
      break;
    case addressState_preliminary:
      if (it->mui64_nextTimeout <= aui64_time) {
        // timeout happened, won address
        it->mui64_nextTimeout = 0u;
        out << " Address " << std::hex << std::distance(t_attributes.begin(), it) << " claimed successfully ";
        // TODO: more detailed report by compararision of mc_isoName with mc_lastWonIsoName;
        //   if equal: reclaimed,
        //   if no longer empty:  claimed
        //   otherwise: moved
        it->e_addressState = addressState_won;
      }
      break;
    default:
      break;
    }
}
