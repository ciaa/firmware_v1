/*
  vtclient_c.cpp: central VT Client management

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "vtclient_c.h"

#include <IsoAgLib/scheduler/impl/scheduler_c.h>
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/impl/vtclientconnection_c.h>
#include <IsoAgLib/util/iassert.h>
#include "../ivtclientobjectpool_c.h"

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif


namespace __IsoAgLib {

/** C-style function, to get access to the unique multiton instance */
VtClient_c &getVtClientInstance( uint8_t instance )
{ // if > 1 singleton instance is used, no static reference can be used
  MACRO_MULTITON_GET_INSTANCE_BODY(VtClient_c, PRT_INSTANCE_CNT, instance);
}



VtClient_c::VtClient_c()
  : m_vtConnections()
  , mt_handler( *this )
  , m_serverManager()
  , mt_customer( *this )
{
}


void
VtClient_c::init()
{
  isoaglib_assert (!initialized());

  getIsoMonitorInstance4Comm().registerControlFunctionStateHandler(mt_handler);

  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, LANGUAGE_PGN<<8, Ident_c::ExtendedIdent ), 8 );
#ifdef HAL_USE_SPECIFIC_FILTERS
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, VT_TO_GLOBAL_PGN << 8, Ident_c::ExtendedIdent ), 8 );
#else
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, VT_TO_ECU_PGN << 8, Ident_c::ExtendedIdent ), 8 );
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, ACKNOWLEDGEMENT_PGN << 8, Ident_c::ExtendedIdent ), 8 );
#endif
#ifdef USE_VTOBJECT_auxiliaryfunction2
  getIsoBusInstance4Comm().insertFilter( mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, ECU_TO_GLOBAL_PGN << 8, Ident_c::ExtendedIdent ), 8 );
#endif

  setInitialized();
}


void
VtClient_c::close()
{
  isoaglib_assert (initialized());
  isoaglib_assert (getClientCount() == 0);

  m_serverManager.close();

  getIsoBusInstance4Comm().deleteFilter(mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, LANGUAGE_PGN << 8, Ident_c::ExtendedIdent ) );
#ifdef HAL_USE_SPECIFIC_FILTERS
  getIsoBusInstance4Comm().deleteFilter(mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, VT_TO_GLOBAL_PGN << 8, Ident_c::ExtendedIdent  ) );
#else
  getIsoBusInstance4Comm().deleteFilter(mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, VT_TO_ECU_PGN << 8, Ident_c::ExtendedIdent  ) );
  getIsoBusInstance4Comm().deleteFilter(mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FF0000UL, ACKNOWLEDGEMENT_PGN << 8, Ident_c::ExtendedIdent ) );
#endif
#ifdef USE_VTOBJECT_auxiliaryfunction2
  getIsoBusInstance4Comm().deleteFilter(mt_customer, IsoAgLib::iMaskFilterType_c( 0x3FFFF00UL, ECU_TO_GLOBAL_PGN << 8, Ident_c::ExtendedIdent ) );
#endif

  getIsoMonitorInstance4Comm().deregisterControlFunctionStateHandler(mt_handler);

  setClosed();
}


VtClientConnection_c*
VtClient_c::initAndRegisterObjectPool(
  IdentItem_c& arc_identItem,
  IsoAgLib::iVtClientObjectPool_c& arc_pool, 
  const char* apc_versionLabel, 
  IsoAgLib::iVtClientDataStorage_c& apc_claimDataStorage, 
  IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_en aen_mode )
{
  switch (aen_mode)
  {
    case IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_MasterToPrimaryVt:
    case IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_MasterToAnyVt:
    case IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_MasterToSpecificVt:
      isoaglib_assert( arc_identItem.isMaster() );
      break;  

    case IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_Slave:
      break;
  }

  return initAndRegisterObjectPoolCommon(arc_identItem, arc_pool, apc_versionLabel, apc_claimDataStorage, aen_mode);
}


VtClientConnection_c*
VtClient_c::initAndRegisterObjectPoolCommon (IdentItem_c& rc_identItem, IsoAgLib::iVtClientObjectPool_c& arc_pool, const char* apc_versionLabel, IsoAgLib::iVtClientDataStorage_c& apc_claimDataStorage, IsoAgLib::iVtClientObjectPool_c::RegisterPoolMode_en aen_mode)
{
  uint8_t ui8_index = 0;
  // add new instance of VtClientServerCommunication
  for (; ui8_index < m_vtConnections.size(); ui8_index++)
  {
    if (m_vtConnections[ui8_index] == NULL)
      break; // found one emtpy entry

    isoaglib_assert( &m_vtConnections[ui8_index]->getIdentItem() != &rc_identItem );
  }

  VtClientConnection_c* vtCSC = new VtClientConnection_c(
    rc_identItem, *this, arc_pool, apc_versionLabel, apc_claimDataStorage, ui8_index, aen_mode );

  // add new instance to vector
  if (ui8_index < m_vtConnections.size())
    m_vtConnections[ ui8_index ] = vtCSC;
  else
    m_vtConnections.push_back( vtCSC );

  return vtCSC;
}


bool
VtClient_c::deregisterObjectPool (IdentItem_c& r_identItem)
{
  /* what states the IdentItem could have we have to interrupt???
  * - IState_c::ClaimedAddress -> that item is Active and Member on ISOBUS
  * - !UploadType::UploadIdle -> interrupt any upload
  */
  uint8_t ui8_index = 0;
  for (; ui8_index < m_vtConnections.size(); ui8_index++)
  {
    if (m_vtConnections[ui8_index])
    {
      if (&r_identItem == &m_vtConnections[ui8_index]->getIdentItem())
      {
        delete m_vtConnections[ui8_index];
        m_vtConnections[ui8_index] = NULL;
        break;
      }
    }
  }

  if (ui8_index == m_vtConnections.size())
    return false; // appropriate IdentItem could not be found, so nothing was deleted
  else
    return true; // IdentItem was found and deleted
}



uint16_t
VtClient_c::getClientCount() const
{
  uint16_t ui16_count = 0;
  for (uint8_t ui8_index = 0; ui8_index < m_vtConnections.size(); ++ui8_index)
  {
    if (m_vtConnections[ui8_index])
      ++ui16_count;
  }
  return ui16_count;
}


void 
VtClient_c::processMsg( const CanPkg_c& arc_data )
{
  CanPkgExt_c c_data( arc_data, getMultitonInst() );
  if( ( ! c_data.isValid() ) || ( c_data.getMonitorItemForSA() == NULL ) )
    return;

  if( c_data.getMonitorItemForDA() != NULL ) {
    processMsgNonGlobal( c_data );
  } else {
    processMsgGlobal( c_data );
  }
}

void VtClient_c::processMsgNonGlobal( const CanPkgExt_c& pkg ) {

  isoaglib_assert( ( pkg.isoPurePgn() == VT_TO_ECU_PGN ) || ( pkg.isoPurePgn() == ACKNOWLEDGEMENT_PGN ) );

  for( STL_NAMESPACE::vector<VtClientConnection_c*>::iterator it = m_vtConnections.begin();
       it != m_vtConnections.end(); ++it )
  {
    if( (*it)->connectedToVtServer() &&
        ( pkg.getMonitorItemForDA() == (*it)->getIdentItem().getIsoItem() ) &&
        ( pkg.getMonitorItemForSA() == &(*it)->getVtServerInst().getIsoItem() ) )
    {
      switch( pkg.isoPurePgn() )
      {
        case ACKNOWLEDGEMENT_PGN:
          (*it)->processMsgAck( pkg );
          break;
        case VT_TO_ECU_PGN:
          (*it)->processMsgVtToEcu( pkg );
          break;
      }
    }
  }
}


void VtClient_c::processMsgGlobal( const CanPkgExt_c& arc_data ) {
  // VT_TO_GLOBAL is the only PGN we accept without VT being active, because it marks the VT active!!

  uint8_t const cui8_cmdByte = arc_data.getUint8Data (1-1);
  
  switch (arc_data.isoPgn())
  {
    case VT_TO_GLOBAL_PGN:
    {
      switch(cui8_cmdByte)
      {
        case 0xFE:  // Command: "Status", Parameter: "VT Status Message"
          m_serverManager.processVtStatusMsg(arc_data, m_vtConnections);
          break;

        case 0x21: // Command: "Auxiliary Control", Parameter: "Auxiliary Input Status"
          notifyAllConnectionsOnAux1InputStatus( arc_data );
          break;

        case 0x26: // Command: "Auxiliary Control Type 2", Parameter: "Auxiliary Input Status"
          notifyAllConnectionsOnAux2InputStatus( arc_data );
          break;
      }
      break;
    }
    
    case ECU_TO_GLOBAL_PGN:
    {
      if (cui8_cmdByte == 0x23) // Command: "Auxiliary Control", Parameter: "input maintenance message"
        notifyAllConnectionsOnAux2InputMaintenance( arc_data );

      break;
    }


    case LANGUAGE_PGN:
    {
      STL_NAMESPACE::vector<VtServerInstance_c*>::iterator lit_vtServerInst;
      VtServerInstance_c* pc_server = NULL;
      uint8_t ui8_index;

      // first process LANGUAGE_PGN for all VtServerInstances BEFORE processing for the VtClientServerCommunications
      for (lit_vtServerInst = m_serverManager.getRefServerInstanceVec().begin();
           lit_vtServerInst != m_serverManager.getRefServerInstanceVec().end();
           ++lit_vtServerInst)
      {
        if (&(*lit_vtServerInst)->getIsoItem() == arc_data.getMonitorItemForSA())
        {
          pc_server = *lit_vtServerInst;
          break;
        }
      }

      if (pc_server != NULL)
      {
        pc_server->setLocalSettings( arc_data );

        // notify all connected vtCSCs
        for (ui8_index = 0; ui8_index < m_vtConnections.size(); ui8_index++)
        {
          if ( m_vtConnections[ui8_index] &&
              (m_vtConnections[ui8_index]->getVtServerInstPtr() == pc_server) )
          m_vtConnections[ui8_index]->notifyOnVtsLanguagePgn();
        }
      }
      break;
    }

    default:
        ; // else: Language PGN from non-VtServerInstance - ignore
  }
}


bool
VtClient_c::sendCommandForDEBUG(IsoAgLib::iIdentItem_c& mrc_wsMasterIdentItem, uint8_t* apui8_buffer, uint32_t ui32_size)
{
  for (uint8_t ui8_index = 0; ui8_index < m_vtConnections.size(); ui8_index++)
  {
    if (&static_cast<__IsoAgLib::IdentItem_c&>(mrc_wsMasterIdentItem) == &m_vtConnections[ui8_index]->getIdentItem())
      return m_vtConnections[ui8_index]->commandHandler().sendCommand(apui8_buffer, ui32_size);
  }
  return false;
}


void
VtClient_c::notifyAllConnectionsOnAux1InputStatus( const CanPkgExt_c& refc_data ) const
{
  for (unsigned index = 0; index < m_vtConnections.size(); index++)
  {
    if (m_vtConnections[index])
      m_vtConnections[index]->notifyOnAuxInputStatus( refc_data );
  }
}

void
VtClient_c::notifyAllConnectionsOnAux2InputStatus( const CanPkgExt_c& refc_data ) const
{
  for( unsigned index = 0; index < m_vtConnections.size(); ++index )
  {
    if( m_vtConnections[ index ] )
      m_vtConnections[ index ]->notifyOnAux2InputStatus( refc_data );
  }
}

void
VtClient_c::notifyAllConnectionsOnAux2InputMaintenance( const CanPkgExt_c& refc_data ) const
{
  for( unsigned index = 0; index < m_vtConnections.size(); ++index )
  {
    if( m_vtConnections[ index ] )
      m_vtConnections[ index ]->notifyOnAux2InputMaintenance( refc_data );
  }
}

} // __IsoAgLib
