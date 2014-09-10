/*
  isofiltermanager_c.cpp: module for managing IsoFilters

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "isofiltermanager_c.h"

#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/util/iassert.h>

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif

namespace __IsoAgLib {


IsoFilterManager_c::IsoFilterManager_c () :
  mvec_isoFilterBox(),
  mt_handler(*this)
{
}


void
IsoFilterManager_c::init()
{
  isoaglib_assert (!initialized());

  __IsoAgLib::getIsoMonitorInstance4Comm().registerControlFunctionStateHandler( mt_handler );

  setInitialized();
}


void
IsoFilterManager_c::close()
{
  isoaglib_assert (initialized());

  // for now, clear all the registered filters.
  for (IsoFilterBox_it it_isoFilterBox = mvec_isoFilterBox.begin();
       it_isoFilterBox != mvec_isoFilterBox.end();
        ++it_isoFilterBox)
  { // Search for existing IsoFilterBox
    it_isoFilterBox->updateOnRemove (NULL);
  }
  mvec_isoFilterBox.clear();
  // for later, all modules should remove their filters!

  getIsoMonitorInstance4Comm().deregisterControlFunctionStateHandler( mt_handler );

  setClosed();
}


bool
IsoFilterManager_c::existIsoFilter (const IsoFilter_s& arcs_isoFilter)
{
  for (IsoFilterBox_it it_isoFilterBox = mvec_isoFilterBox.begin();
       it_isoFilterBox != mvec_isoFilterBox.end();
       ++it_isoFilterBox)
  { // Search for existing IsoFilterBox
    if (it_isoFilterBox->hasIsoFilter (arcs_isoFilter))
    { // This IsoFilterBox already has such a filter
      return true;
    }
  }
  return false;
}


void
IsoFilterManager_c::insertIsoFilter (const IsoFilter_s& arcs_isoFilter)
{
  // Check if IsoFilter does yet exist in some IsoFilterBox
  if (!existIsoFilter (arcs_isoFilter))
  { // insert an empty IsoFilterBox. initialized then in list right after
    mvec_isoFilterBox.push_back (IsoFilterBox_c (arcs_isoFilter MULTITON_INST_WITH_COMMA));

    // now get the inserted IsoFilterBox and retrigger update of real hardware filters
    mvec_isoFilterBox.back().updateOnAdd();
  }
}


bool
IsoFilterManager_c::removeIsoFilter (const IsoFilter_s& arcs_isoFilter)
{
  for (IsoFilterBox_it it_isoFilterBox = mvec_isoFilterBox.begin();
       it_isoFilterBox != mvec_isoFilterBox.end();
       ++it_isoFilterBox)
  { // Search for existing IsoFilterBox
    if (it_isoFilterBox->hasIsoFilter (arcs_isoFilter))
    {
      it_isoFilterBox->updateOnRemove (NULL);
      mvec_isoFilterBox.erase (it_isoFilterBox);
      return true;
    }
  }

  // IsoFilterBox not existing, can't remove!
  return false;
}



void
IsoFilterManager_c::reactOnIsoItemModification (ControlFunctionStateHandler_c::iIsoItemAction_e at_action, IsoItem_c const& acrc_isoItem)
{
  if ((at_action == ControlFunctionStateHandler_c::AddToMonitorList) || (at_action == ControlFunctionStateHandler_c::ReclaimedAddress))
  {
    for (IsoFilterBox_it it_isoFilterBox = mvec_isoFilterBox.begin();
        it_isoFilterBox != mvec_isoFilterBox.end();
        ++it_isoFilterBox)
    { // the ISOFilterBoxes will take care if they have to do anything at all or not...
      it_isoFilterBox->updateOnAdd();
    }
  }
  else if (at_action == ControlFunctionStateHandler_c::ChangedAddress)
  {
    for (IsoFilterBox_it it_isoFilterBox = mvec_isoFilterBox.begin();
        it_isoFilterBox != mvec_isoFilterBox.end();
        ++it_isoFilterBox)
    { // the ISOFilterBoxes will take care if they have to do anything at all or not...
      it_isoFilterBox->updateOnRemove(&acrc_isoItem.isoName());
      it_isoFilterBox->updateOnAdd();
    }
  }
  else
  { // ((at_action == RemoveFromMonitorList) || (at_action == LostAddress))
    for (IsoFilterBox_it it_isoFilterBox = mvec_isoFilterBox.begin();
         it_isoFilterBox != mvec_isoFilterBox.end();
         ++it_isoFilterBox)
    { // the ISOFilterBoxes will take care if they have to do anything at all or not...
      it_isoFilterBox->updateOnRemove (&acrc_isoItem.isoName());
    }
  }
}


IsoFilterManager_c &
getIsoFilterManagerInstance( unsigned int instance )
{
  MACRO_MULTITON_GET_INSTANCE_BODY(IsoFilterManager_c, PRT_INSTANCE_CNT, instance);
}

} // __IsoAgLib
