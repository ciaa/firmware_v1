/*
  tracguidance_c.cpp: stores, updates and delivers all guidance data
    information from CanCustomer_c derived for CAN sending and
    receiving interaction; from BaseCommon_c derived for interaction
    with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#include "tracguidancecommand_c.h"
#include "tracguidance_c.h"


namespace __IsoAgLib { // Begin Namespace __IsoAglib

/** C-style function, to get access to the unique TracGuidanceCommand_c singleton instance
  * if more than one CAN BUS is used for IsoAgLib, an m_index must be given to select the wanted BUS
  */
TracGuidanceCommand_c &getTracGuidanceCommandInstance( uint8_t aui8_instance )
{ // if > 1 singleton instance is used, no static reference can be used
  MACRO_MULTITON_GET_INSTANCE_BODY(TracGuidanceCommand_c, PRT_INSTANCE_CNT, aui8_instance);
}


bool TracGuidanceCommand_c::config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask)
{
  //call config for handling which is base data independent
  //if something went wrong leave function before something is configured
  if ( !BaseCommon_c::config_base ( apc_ident, at_identMode, aui16_suppressMask) ) return false;

  ///Set time Period for Scheduler_c --> periodic action only if in IMPLEMENT MODE!!
  if (at_identMode == IsoAgLib::IdentModeImplement) mt_task.setPeriod( 100, false );
  else  mt_task.setPeriod( 1000, false );

  mui8_commanderSa = 0xFF;

  return true;
}

/** check if filter boxes shall be created - create only ISO filters based
    on active local idents which has already claimed an address
    --> avoid to much Filter Boxes
  */
void TracGuidanceCommand_c::checkCreateReceiveFilter( )
{
  if( !checkFilterCreated() )
  {
    setFilterCreated();

    IsoBus_c &c_can = getIsoBusInstance4Comm();
    c_can.insertFilter(*this, IsoAgLib::iMaskFilter_c( 0x3FF00LU << 8, GUIDANCE_SYSTEM_CMD << 8), 8 );
  }
}

/** send a ISO11783 guidance command information PGN.
  * this is only called when sending ident is configured and it has already claimed an address
    @pre  function is only called in implement mode
    @see  BaseCommon_c::timeEvent()
  */
void TracGuidanceCommand_c::timeEventImplMode( )
{ ///Timeperiod of 100ms is set in ::config
  isoSendMessageImplMode();
}

/** HIDDEN constructor for a TracGuidance_c object instance which can optional
    set the configuration for send/receive for a guidance msg
    NEVER instantiate a variable of type TracGuidance_c within application
    only access TracGuidance_c via getTracGuidanceInstance() or getTracGuidanceInstance( int riLbsBusNr ) in case more than one ISO11783 BUS is used for IsoAgLib
  */
TracGuidanceCommand_c::TracGuidanceCommand_c() {}


void TracGuidanceCommand_c::processMsg( const CanPkg_c& arc_data )
{
  CanPkgExt_c pkg( arc_data, getMultitonInst() );
  if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
    return;

  switch (pkg.isoPgn() & 0x3FF00LU)
  {
    case GUIDANCE_SYSTEM_CMD:
      if ( checkMode(IsoAgLib::IdentModeTractor) && ( getCommander() == pkg.isoSa() ) )
      {
        mmap_commanders[mui8_commanderSa].ui16_curvatureCmd = pkg.getUint16Data(0);
        mmap_commanders[mui8_commanderSa].t_curvatureCmdStatus = IsoAgLib::IsoSteerFlag_t( pkg.getUint8Data(2) & 0x3 );

        setUpdateTime( pkg.time() );
      }
      break;
  }
}

/** send guidance command data
    @see  CanIo_c::operator<<
  */
void TracGuidanceCommand_c::isoSendMessageImplMode( )
{
  if( ( ! getIdentItem() ) || ( ! getIdentItem()->isClaimedAddress() ) )
    return;


  IsoBus_c& c_can = getIsoBusInstance4Comm();

  CanPkgExt_c pkg;
  // retrieve the actual dynamic sender no of the member with the registered isoName
  pkg.setIsoPri(3);
  pkg.setLen(8);

  pkg.setISONameForDA( getTracGuidanceInstance4Comm().getSelectedDataSourceISONameConst() );
  pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
  pkg.setIsoPgn(GUIDANCE_SYSTEM_CMD);
  pkg.setUint16Data(0, mmap_commanders[mui8_commanderSa].ui16_curvatureCmd);
  uint8_t ui8_temp = 0;
  ui8_temp |= (mmap_commanders[mui8_commanderSa].t_curvatureCmdStatus);
  pkg.setUint8Data(2, ui8_temp);
  //reserved fields
  pkg.setUint32Data(3, 0xFFFFFFFFUL);
  pkg.setUint8Data(7, 0xFF);

  c_can << pkg;
}

/** get currently selected curvature commanders source address
    @param  ui8_sourceAddress  source address of curvature commander
    */
void TracGuidanceCommand_c::setCommander(uint8_t ui8_sourceAddress)
{
  // create new entry with key ui8_sourceAddress if it does not yet exist
  mmap_commanders[ui8_sourceAddress];
  mui8_commanderSa = ui8_sourceAddress;
}

/** get data of a curvature commander by source address
    @param  ui8_sourceAddress  source address
    @return                    curvature commander data or NULL if not exist
  */
CurvatureCommander_s* TracGuidanceCommand_c::getCommanderDataBySA( uint8_t ui8_sourceAddress )
{
  STL_NAMESPACE::map<uint8_t, CurvatureCommander_s>::iterator itr = mmap_commanders.find(ui8_sourceAddress);
  CurvatureCommander_s*  ps_commander = NULL;

  if ( itr != mmap_commanders.end() )
  {
    ps_commander = &itr->second;
  }
  else
  { // there exist no commander data for this source address
    IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracError, getMultitonInst() );
  }

  return ps_commander;
}

/** get data of a curvature commander by m_index
    @param ui8_index  source address
    @return curvature commander data or NULL if not exist
  */
CurvatureCommander_s* TracGuidanceCommand_c::getCommanderDataByIndex( uint8_t ui8_index)
{
  STL_NAMESPACE::map<uint8_t, CurvatureCommander_s>::iterator itr;
  CurvatureCommander_s*  ps_commander = NULL;
  unsigned int count = 0;

  if ( ui8_index > mmap_commanders.size() )
  {
    IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracError, getMultitonInst() );
    return ps_commander;
  }

  for(itr = mmap_commanders.begin(); (itr != mmap_commanders.end() && count <= ui8_index); ++itr)
  {
    if (count == ui8_index)
      ps_commander = &itr->second;

    count++;
  }

  return ps_commander;
}

/** dummy implementation
    @todo SOON-824: add answering of requestPGN in case this object is configured for sending of these information
           - verify this also for the other TracFoo classes
  */
bool TracGuidanceCommand_c::processMsgRequestPGN (uint32_t /*aui32_pgn*/, IsoItem_c* /*apc_isoItemSender*/, IsoItem_c* /*apc_isoItemReceiver*/, int32_t )
{
  return false;
}

}// End Namespace __IsoAglib
