/*
  vtserverinstance_c.cpp: class for wrapping one vtserver instance

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtserverinstance_c.h"
// necessary for interface convert operators ("up"-cast)
#include "../ivtserverinstance_c.h"

#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/hal/hal_system.h>
#include <IsoAgLib/driver/can/impl/canio_c.h>
#include "vtclient_c.h"


namespace __IsoAgLib {

VtServerInstance_c::VtServerInstance_c( const IsoItem_c& isoItem, VtClient_c& client )
  : m_isoItem( isoItem )
  , mrc_isoTerminal( client )
  , ms_vtStateA()
  , ms_vtCapabilitiesA()
  , ms_localSettingsA()
{
  /// init all variables to an initial upload state (Upload will not start before ws-announcing is due
  ms_vtCapabilitiesA.lastReceivedSoftkeys = 0; // not yet (queried and) got answer about vt's capabilities yet
  ms_vtCapabilitiesA.lastRequestedSoftkeys = 0; // not yet requested vt's capabilities yet
  ms_vtCapabilitiesA.lastReceivedHardware = 0; // not yet (queried and) got answer about vt's capabilities yet
  ms_vtCapabilitiesA.lastRequestedHardware = 0; // not yet requested vt's capabilities yet
  ms_vtCapabilitiesA.lastReceivedFont = 0; // not yet (queried and) got answer about vt's capabilities yet
  ms_vtCapabilitiesA.lastRequestedFont = 0; // not yet requested vt's capabilities yet
  ms_vtCapabilitiesA.lastReceivedVersion = 0; // interesting for NACK handling, that's why it's reset here!
  ms_localSettingsA.lastRequested = 0; // no language info requested yet
  ms_localSettingsA.lastReceived = 0; // no language info received yet
  ms_vtStateA.lastReceived = 0; // no vt_statusMessage received yet
}


VtServerInstance_c::~VtServerInstance_c()
{
}


bool
VtServerInstance_c::isVtActive() const
{
  if (ms_vtStateA.lastReceived)
  {
    // Using the current time for comparison checks currently, because we don't have the information
    // about the time when the last can-msg was processed or checked for (in case there's none).
    if ((System_c::getTime() - (int32_t)ms_vtStateA.lastReceived) <= 3000)
    { // comparing as int, so that in case "NOW-time > CAN-time" NO client-reload happens
      return true;
    }
  }
  return false;
}


void
VtServerInstance_c::setLatestVtStatusData( const CanPkgExt_c& arc_data )
{
  ms_vtStateA.lastReceived =               arc_data.time();
  ms_vtStateA.saOfActiveWorkingSetMaster = arc_data.getUint8Data (1);
  ms_vtStateA.dataAlarmMask =              arc_data.getUint8Data (2) | (arc_data.getUint8Data (3) << 8);
  ms_vtStateA.softKeyMask =                arc_data.getUint8Data (4) | (arc_data.getUint8Data (5) << 8);
  ms_vtStateA.busyCodes =                  arc_data.getUint8Data (6);
  ms_vtStateA.functionBusy =               arc_data.getUint8Data (7);
}


void
VtServerInstance_c::setLocalSettings( const CanPkgExt_c& arc_data )
{
  ms_localSettingsA.lastReceived =  arc_data.time();
  ms_localSettingsA.languageCode = (arc_data.getUint8Data (0) << 8) | arc_data.getUint8Data (1);
  ms_localSettingsA.nDecimalPoint = arc_data.getUint8Data (2) >> 6;
  ms_localSettingsA.nTimeFormat =  (arc_data.getUint8Data (2) >> 4) & 0x03;
  ms_localSettingsA.dFormat =       arc_data.getUint8Data (3);
  ms_localSettingsA.uDistance =     arc_data.getUint8Data (4) >> 6;
  ms_localSettingsA.uArea =        (arc_data.getUint8Data (4) >> 4) & 0x03;
  ms_localSettingsA.uVolume =      (arc_data.getUint8Data (4) >> 2) & 0x03;
  ms_localSettingsA.uMass =         arc_data.getUint8Data (4)       & 0x03;
  ms_localSettingsA.uTemperature =  arc_data.getUint8Data (5) >> 6;
  ms_localSettingsA.uPressure =    (arc_data.getUint8Data (5) >> 4) & 0x03;
  ms_localSettingsA.uForce =       (arc_data.getUint8Data (5) >> 2) & 0x03;
  ms_localSettingsA.uUnitsSystem =  arc_data.getUint8Data (5)       & 0x03;
  // The other fields are reserved. (yet ;-)
}


void
VtServerInstance_c::setVersion( const CanPkgExt_c& arc_data )
{
  ms_vtCapabilitiesA.lastReceivedVersion =  arc_data.time();
  ms_vtCapabilitiesA.iso11783version =      arc_data.getUint8Data (1);
}


void
VtServerInstance_c::setSoftKeyData( const CanPkgExt_c& arc_data )
{
  ms_vtCapabilitiesA.lastReceivedSoftkeys = arc_data.time();
  ms_vtCapabilitiesA.skWidth =              arc_data.getUint8Data (4);
  ms_vtCapabilitiesA.skHeight =             arc_data.getUint8Data (5);
  ms_vtCapabilitiesA.skVirtual =            arc_data.getUint8Data (6);
  ms_vtCapabilitiesA.skPhysical =           arc_data.getUint8Data (7);
}


void
VtServerInstance_c::setTextFontData( const CanPkgExt_c& arc_data )
{
  ms_vtCapabilitiesA.lastReceivedFont = arc_data.time();
  ms_vtCapabilitiesA.fontSizes = (static_cast<uint16_t>(arc_data.getUint8Data (5) & 0x7F) << 1) | 0x01;
  ms_vtCapabilitiesA.fontSizes += static_cast<uint16_t>(arc_data.getUint8Data (6) & 0x7F) << 8;
  ms_vtCapabilitiesA.fontTypes = arc_data.getUint8Data (7);
}


void
VtServerInstance_c::setHardwareData( const CanPkgExt_c& arc_data )
{
  ms_vtCapabilitiesA.lastReceivedHardware = arc_data.time();
  ms_vtCapabilitiesA.bootTime =             arc_data.getUint8Data (1);
  ms_vtCapabilitiesA.hwGraphicType =        arc_data.getUint8Data (2);
  ms_vtCapabilitiesA.hwHardware =           arc_data.getUint8Data (3);
  ms_vtCapabilitiesA.hwWidth =              arc_data.getUint8Data (4) + (static_cast<uint16_t>(arc_data.getUint8Data (5)) << 8);
  ms_vtCapabilitiesA.hwHeight =             arc_data.getUint8Data (6) + (static_cast<uint16_t>(arc_data.getUint8Data (7)) << 8);
}


uint32_t
VtServerInstance_c::getVtHardwareDimension() const
{
  return (uint32_t) (ms_vtCapabilitiesA.hwWidth);
}


uint16_t
VtServerInstance_c::getVtFontSizes() const
{
  return ms_vtCapabilitiesA.fontSizes;
}


uint8_t
VtServerInstance_c::getVtIsoVersion() const
{
  return ms_vtCapabilitiesA.iso11783version;
}


IsoAgLib::iVtServerInstance_c*
VtServerInstance_c::toIvtServerInstancePtr_c()
{
  return static_cast<IsoAgLib::iVtServerInstance_c*>(this);
}


IsoAgLib::iVtServerInstance_c&
VtServerInstance_c::toIvtServerInstance_c()
{
  return static_cast<IsoAgLib::iVtServerInstance_c&>(*this);
}


void
VtServerInstance_c::requestLocalSettings( IdentItem_c& identItem )
{
  // If our IsoItem has claimed address, immediately try to get the LANGUAGE_PGN from VT/anyone ;-) (regardless of pool-upload!)
  if ( (getLocalSettings()->lastRequested == 0) ||
       ((HAL::getTime()-getLocalSettings()->lastRequested) > 2000) 
     )
  { // Try every 2 seconds to get the LANGUAGE_PGN, be polite to not bombard the VT...
    /** @todo SOON-258 Give up somewhen?? Or retry really every 2 seconds? Don't care too much for now, shouldn't happen in real systems... */
    // Get Local Settings (may not be reached, when terminal is switched on after ECU, as VT sends LNAGUAGE Info on startup!
    CanPkgExt_c mc_sendData;
    mc_sendData.setIsoPri( 6 );
    mc_sendData.setIsoPgn( REQUEST_PGN_MSG_PGN );
    mc_sendData.setIsoPs( getIsoItem().nr() ); // due to const-ness problem use SA here!
    mc_sendData.setMonitorItemForSA( identItem.getIsoItem() );
    mc_sendData.setUint8Data( 0,  (LANGUAGE_PGN        & 0xFF) );
    mc_sendData.setUint8Data( 1, ((LANGUAGE_PGN >> 8)  & 0xFF) );
    mc_sendData.setUint8Data( 2, ((LANGUAGE_PGN >> 16) & 0xFF) );
    mc_sendData.setLen( 3 );
    getIsoBusInstance( mrc_isoTerminal.getMultitonInst() ) << mc_sendData;

    getLocalSettings()->lastRequested = HAL::getTime();
  }
}


} // __IsoAgLib
