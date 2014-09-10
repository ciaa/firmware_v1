/*
  filterbox_c.cpp: FilterBox_c permits free definition of mask/filter
    for receiving CAN telegrams by a CANCustomer (or derived) object

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "filterbox_c.h"
#include "canio_c.h"
#include <IsoAgLib/driver/can/impl/cancustomer_c.h>
#include <IsoAgLib/util/iassert.h>

namespace __IsoAgLib {


FilterBox_c* FilterBox_c::mspc_currentlyProcessedFilterBox = NULL;
int FilterBox_c::msi_processMsgLoopIndex = -1; // not used if "mspc_currentlyProcessedFilterBox==NULL" though.
int FilterBox_c::msi_processMsgLoopSize = -1; // not used if "mspc_currentlyProcessedFilterBox==NULL" though.


FilterBox_c::FilterBox_c()
  : mc_maskFilterPair()
  , mvec_customer()
#ifndef NO_FILTERBOX_LIST_ORDER_SWAP
  , m_matchCnt( 0 )
#endif
{}


void
FilterBox_c::clearData()
{
  mvec_customer.clear();
  mc_maskFilterPair.setEmpty();
}


void
FilterBox_c::set (const IsoAgLib::iMaskFilterType_c& arc_maskFilterPair,
                  CanCustomer_c* apc_customer,
                  int a_dlcForce)
{
  // actually "apc_customer" should've rather been a reference!
  isoaglib_assert (apc_customer);

  mc_maskFilterPair = arc_maskFilterPair;

  STL_NAMESPACE::vector<CustomerLen_s>::iterator pc_iter = mvec_customer.begin();
  for (; pc_iter != mvec_customer.end(); ++pc_iter)
  {
    if (apc_customer == pc_iter->pc_customer)
    { // overwrite the DLC of the one found!
      pc_iter->dlcForce = a_dlcForce;
      break;
    }
  }
  if (pc_iter == mvec_customer.end())
  { // push back new
    mvec_customer.push_back (CustomerLen_s (apc_customer, a_dlcForce));
    /// Currently "msi_processMsgLoopIndex" is not being adapted,
    /// because if a Message triggers Customer A and B, and Customer A
    /// inserts a filter for that same message for Customer C,
    /// we do NOT want Customer C to be called immediately.
  }
};


bool
FilterBox_c::equalCustomer( const __IsoAgLib::CanCustomer_c& ar_customer ) const

{
  STL_NAMESPACE::vector<CustomerLen_s>::const_iterator pc_iter;
  for(pc_iter = mvec_customer.begin(); pc_iter != mvec_customer.end(); ++pc_iter)
    if( &ar_customer == pc_iter->pc_customer)
      return true;

  return false;
}


bool
FilterBox_c::deleteFilter( const __IsoAgLib::CanCustomer_c& ar_customer)
{
  int deleteIndex = 0;
  for (STL_NAMESPACE::vector<CustomerLen_s>::iterator pc_iter = mvec_customer.begin();
       pc_iter != mvec_customer.end();)
  {
    if (&ar_customer == pc_iter->pc_customer)
    { // the to-be-deleted customer is found and now pointed by pc_iter
      mvec_customer.erase(pc_iter);
      // in case we're currently in the processing loop for this FilterBox
      if (mspc_currentlyProcessedFilterBox == this)
      { // we need to adapt the LoopSize and maybe LoopIndex
        if (deleteIndex <= msi_processMsgLoopIndex)
          --msi_processMsgLoopIndex;
        --msi_processMsgLoopSize;
      }
      break;
    }
    ++deleteIndex;
    ++pc_iter;
  }

  if ( mvec_customer.empty() )
  { // the last customer has been removed
    clearData();
    return true;
  }
  else
  { // there is at least one other can customer -> this FilterBox_c is still in use
    return false;
  }
}


void
FilterBox_c::processMsg( CanPkg_c& pkg )
{
  //! We need to FIRST get the number of entries and then DON'T USE iterators,
  //! because the number may increase and the iterators may get invalid in case
  //! a Filter is inserted IN THIS filterbox (pushed back!)
  mspc_currentlyProcessedFilterBox = this;
  msi_processMsgLoopSize = mvec_customer.size();
  for ( msi_processMsgLoopIndex = 0;
        msi_processMsgLoopIndex < msi_processMsgLoopSize;
        ++msi_processMsgLoopIndex )
  {
    const int vecCustomerDlcForce = mvec_customer[msi_processMsgLoopIndex].dlcForce;
    CanCustomer_c* pc_customer = mvec_customer[msi_processMsgLoopIndex].pc_customer;
    isoaglib_assert( pc_customer );

    if (( vecCustomerDlcForce < 0 ) || ( vecCustomerDlcForce == pkg.getLen() ) )
      pc_customer->processMsg( pkg );
  }

  mspc_currentlyProcessedFilterBox = NULL; // indicate that we're not anymore in the loop!

#ifndef NO_FILTERBOX_LIST_ORDER_SWAP
  ++m_matchCnt;
#endif
}

} // __IsoAgLib
