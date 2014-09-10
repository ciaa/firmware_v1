/*
  vtservermanager_c.cpp: central VT Client management

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "vtservermanager_c.h"

#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtclientobjectpool_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoname_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoitem_c.h>
#include "vtserverinstance_c.h"
#include "vtclientconnection_c.h"
#include "vtclient_c.h"

// @todo: needed?
#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif


namespace __IsoAgLib {


VtServerManager_c::VtServerManager_c()
  : ml_vtServerInst()
{
}

void
VtServerManager_c::close()
{
  while (!ml_vtServerInst.empty())
  {
    delete ml_vtServerInst.back();
    ml_vtServerInst.pop_back();
  }  
}


VtServerInstance_c* VtServerManager_c::getActiveVtServer( bool mustBePrimary, const VtServerInstance_c* ap_searchStart ) const
{
  STL_NAMESPACE::vector<VtServerInstance_c*>::const_iterator lit_vtServerInst = ml_vtServerInst.begin();

  if(NULL != ap_searchStart)
  {
    for ( ; lit_vtServerInst != ml_vtServerInst.end(); ++lit_vtServerInst)
    {
      if(ap_searchStart == *lit_vtServerInst)
      break;
    }
    
    if(lit_vtServerInst == ml_vtServerInst.end())
    {
       lit_vtServerInst = ml_vtServerInst.begin();
    }
    else
    {
       ++lit_vtServerInst;
    }
  }

  STL_NAMESPACE::vector<VtServerInstance_c*>::const_iterator lit_searchStart = lit_vtServerInst;
  
  // search rest of list
  for ( ; lit_vtServerInst != ml_vtServerInst.end(); ++lit_vtServerInst)
    if ( (*lit_vtServerInst)->isVtActive() && (!mustBePrimary || (*lit_vtServerInst)->isPrimaryVt()) )
      return (*lit_vtServerInst);

    
  // not found => wrap around and start at beginning until ap_searchStart reached again
  lit_vtServerInst = ml_vtServerInst.begin();
  for ( ; lit_vtServerInst != lit_searchStart; ++lit_vtServerInst)
    if ( (*lit_vtServerInst)->isVtActive() && (!mustBePrimary || (*lit_vtServerInst)->isPrimaryVt()) )
      return (*lit_vtServerInst);

  return NULL;
}


VtServerInstance_c* VtServerManager_c::getPreferredVtServer(const IsoName_c& aref_prefferedVTIsoName) const
{
  STL_NAMESPACE::vector<VtServerInstance_c*>::const_iterator lit_vtServerInst;
  for (lit_vtServerInst = ml_vtServerInst.begin(); lit_vtServerInst != ml_vtServerInst.end(); ++lit_vtServerInst)
  {
    if ((*lit_vtServerInst)->isVtActive() && ((*lit_vtServerInst)->getIsoName() == aref_prefferedVTIsoName))
      return (*lit_vtServerInst);
  }
  return NULL;
}


VtServerInstance_c* VtServerManager_c::getSpecificVtServer(const IsoAgLib::iVtClientObjectPool_c& arc_pool) const
{
  STL_NAMESPACE::vector<VtServerInstance_c*>::const_iterator lit_vtServerInst;
  for (lit_vtServerInst = ml_vtServerInst.begin(); lit_vtServerInst != ml_vtServerInst.end(); ++lit_vtServerInst)
  {
    if ((*lit_vtServerInst)->isVtActive() && arc_pool.selectVtServer((*lit_vtServerInst)->getIsoName().toConstIisoName_c()))
      return (*lit_vtServerInst);
  }
  return NULL;
}

uint16_t VtServerManager_c::getActiveVtCount() const
{
  uint16_t cnt = 0;
  
  STL_NAMESPACE::vector<VtServerInstance_c*>::const_iterator lit_vtServerInst;
  for (lit_vtServerInst = ml_vtServerInst.begin(); lit_vtServerInst != ml_vtServerInst.end(); ++lit_vtServerInst)
  {
    if ((*lit_vtServerInst)->isVtActive())
      cnt++;
  }
  return cnt;    
}



void
VtServerManager_c::reactOnIsoItemModification(
  ControlFunctionStateHandler_c::iIsoItemAction_e action,
  IsoItem_c const& isoItem,
  STL_NAMESPACE::vector<VtClientConnection_c*>& aref_vtConnections,
  VtClient_c& aref_vtClient )
{
  // we only care for the VTs
  if( isoItem.isoName().getEcuType() != IsoName_c::ecuTypeVirtualTerminal )
    return;

  switch( action )
  {
    case ControlFunctionStateHandler_c::AddToMonitorList:
    { ///! Attention: This function is also called from "init()", not only from ISOMonitor!
      for( STL_NAMESPACE::vector<VtServerInstance_c*>::iterator iter = ml_vtServerInst.begin();
           iter != ml_vtServerInst.end(); ++iter )
      { // check if newly added VtServerInstance is already in our list
        if (&isoItem == &(*iter)->getIsoItem())
          return;
      }

      ml_vtServerInst.push_back(
        new VtServerInstance_c( isoItem, aref_vtClient ) );
    } break;

    case ControlFunctionStateHandler_c::RemoveFromMonitorList:
      for( STL_NAMESPACE::vector<VtServerInstance_c*>::iterator iter = ml_vtServerInst.begin();
           iter != ml_vtServerInst.end(); ++iter )
      { // check if lost VtServerInstance is in our list
        if (&isoItem == &(*iter)->getIsoItem())
        { // the VtServerInstance is already known and in our list,
          // delete it and notify all clients on early loss of that VtServerInstance
          for (unsigned index = 0; index < aref_vtConnections.size(); ++index)
          {
            if( aref_vtConnections[ index ] )
              aref_vtConnections[ index ]->notifyOnVtServerInstanceLoss( *(*iter) );
          }

          delete *iter;
          ( void )ml_vtServerInst.erase( iter );
          break;
        }
      }
      break;

    default:
      break;
  }
}


void
VtServerManager_c::processVtStatusMsg(const CanPkgExt_c& arc_data,
                                      STL_NAMESPACE::vector<VtClientConnection_c*>& aref_vtConnections)
{
  STL_NAMESPACE::vector<VtServerInstance_c*>::iterator lit_vtServerInst;

  for (lit_vtServerInst = ml_vtServerInst.begin();
      lit_vtServerInst != ml_vtServerInst.end();
      ++lit_vtServerInst)
  {
    if (&(*lit_vtServerInst)->getIsoItem() == arc_data.getMonitorItemForSA())
    {
      (*lit_vtServerInst)->setLatestVtStatusData( arc_data );

      // iterate through all registered VtClientServerCommunication and notify their pools with "eventVtStatusMsg"
      for (unsigned index = 0; index < aref_vtConnections.size(); index++)
      {
        if (aref_vtConnections[index])
        {
          if (aref_vtConnections[index]->getVtServerInstPtr() == (*lit_vtServerInst))
          { // this vtClientServerComm is connected to this VT, so notify the objectpool!!
            aref_vtConnections[index]->notifyOnVtStatusMessage();
          }
        }
      }
      break;
    }
  }
}

// (currently not supported, due to multi VT enhancements)
#if 0
#ifdef USE_IOP_GENERATOR_FAKE_VT_PROPERTIES
void
VtServerManager_c::fakeVtProperties (uint16_t aui16_dimension, uint16_t aui16_skWidth, uint16_t aui16_skHeight,
                                     uint8_t aui16_colorDepth, uint16_t aui16_fontSizes,
                                     VtClient_c& aref_vtClient)
{
  const IsoItem_c c_dummyIsoItem;
  // casting NULL to a reference is okay here, as the reference isn't used for any FAKE_VT case (iop_generator, etc.)
  ml_vtServerInst.push_back ( new VtServerInstance_c (c_dummyIsoItem, aref_vtClient) );
  VtServerInstance_c* p_vtServerInst = ml_vtServerInst.back();
  p_vtServerInst->fakeVtProperties (aui16_dimension, aui16_skWidth, aui16_skHeight, aui16_colorDepth, aui16_fontSizes);
  // ... and notify all vtClientServerComm instances
}
#endif
#endif

} // __IsoAgLib
