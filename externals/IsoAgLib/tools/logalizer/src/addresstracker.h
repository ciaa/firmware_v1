/*
  addresstracker.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ADDRESSTRACKER_H
#define ADDRESSTRACKER_H

#include <logenvirons.h>
#include <dataframe.h>
#include <vector>


class AddressTracker_c {
public:
  AddressTracker_c();
  std::string requestForAddressClaimed(PtrDataFrame_t at_ptrFrame);
  std::string addressClaimed(PtrDataFrame_t at_ptrFrame);
  void checkTimeouts(std::ostream& out, uint64_t aui64_time);

private:
  class IsoName_c {
  public:
    IsoName_c();
    IsoName_c(std::vector< uint8_t > avec_data);
    bool isEmpty() const;
  private:
    std::vector< uint8_t > mvec_data;
    friend bool operator<(IsoName_c const &, IsoName_c const &);
  };
  friend bool operator<(IsoName_c const &, IsoName_c const &);

  enum AddressState_e {
    addressState_idle,
    addressState_preliminary,
    addressState_awaitAddressClaimed,
    addressState_won
  };

  struct AddressAttributes_s {
    AddressState_e e_addressState;
    IsoName_c mc_isoName;
    IsoName_c mc_lastWonIsoName;
    uint64_t mui64_nextTimeout;
    AddressAttributes_s();
  };

  typedef std::vector< AddressAttributes_s > Attributes_t;

  Attributes_t::iterator attributesAt(size_t t_index);

  Attributes_t t_attributes;
};


inline
AddressTracker_c::AddressTracker_c() :
  t_attributes(254)
{
}


inline
AddressTracker_c::IsoName_c::IsoName_c() :
  mvec_data() // empty vector means: no ISO name at all
{
}


inline
bool
AddressTracker_c::IsoName_c::isEmpty() const
{
  return 0 == mvec_data.size();
}


inline
AddressTracker_c::AddressAttributes_s::AddressAttributes_s() :
  e_addressState(addressState_idle),
  mc_isoName(),
  mc_lastWonIsoName(),
  mui64_nextTimeout(0u)
{
}


inline
bool
operator<(
    AddressTracker_c::IsoName_c const &arcs_left,
    AddressTracker_c::IsoName_c const &arcs_right)
{
  return arcs_left.mvec_data < arcs_right.mvec_data;
}


#endif
