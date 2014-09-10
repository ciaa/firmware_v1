/*
  traclight_c: working on lighting data; stores, updates and delivers
    all base data informations from CanCustomer_c derived for CAN
    sending and receiving interaction; from BaseCommon_c derived for
    interaction with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "traclight_c.h"
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgn_c.h>
#include <IsoAgLib/util/iassert.h>


namespace __IsoAgLib {

  /** C-style function, to get access to the unique TracLight_c singleton instance
   * if more than one CAN BUS is used for IsoAgLib, an m_index must be given to select the wanted BUS
   */
  TracLight_c& getTracLightInstance( uint8_t aui8_instance )
  { // if > 1 singleton instance is used, no static reference can be used
    MACRO_MULTITON_GET_INSTANCE_BODY(TracLight_c, PRT_INSTANCE_CNT, aui8_instance);
  }

  void
  TracLight_c::init_specialized()
  {
    isoaglib_assert (IsoAgLib::IdentModeImplement == getMode());
    getRegisterPgn()(LIGHTING_DATA_PGN);
  }

  void
  TracLight_c::close_specialized()
  {
    if (checkMode (IsoAgLib::IdentModeTractor))
    { // Tractor to OFF
      getUnregisterPgn()(LIGHTING_COMMAND_PGN);
    }
    else
    { // Implement to OFF
      getUnregisterPgn()(LIGHTING_DATA_PGN);
    }
  }


  bool TracLight_c::config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask )
  {
    //store old mode to decide to register or unregister to request for pgn
    IsoAgLib::IdentMode_t t_oldMode = getMode();
    //call config for handling which is base data independent
    //if something went wrong leave function before something is configured
    if ( ! BaseCommon_c::config_base ( apc_ident, at_identMode, aui16_suppressMask) ) return false;

    if ( checkMode(IsoAgLib::IdentModeTractor))
    {
      // set Time Period in ms for Scheduler_c
      mt_task.setPeriod( 1000, false );
    }

    // un-/register to PGN
    if (t_oldMode == at_identMode)
      ; // no change, still the same mode
    else if (at_identMode == IsoAgLib::IdentModeTractor) {
      // a change from Implement mode to Tractor mode occured
      getUnregisterPgn()(LIGHTING_DATA_PGN);
      getRegisterPgn()(LIGHTING_COMMAND_PGN);
    } else {
      // a change from Tractor mode to Implement mode occured
      getUnregisterPgn()(LIGHTING_COMMAND_PGN);
      getRegisterPgn()(LIGHTING_DATA_PGN);
    }

    // set configure values
    mb_cmdWait4Response = false;
    mb_changeNeedBeSend = false;

    for (int i = 0; i < 10; i++)
      marr_timeStamp[i] = 0;

    m_index = 0;

    return true;
  };

  /** check if filter boxes shall be created - create only ISO filters based
      on active local idents which has already claimed an address
      --> avoid to much Filter Boxes
    */
  void TracLight_c::checkCreateReceiveFilter()
  {
    if( !checkFilterCreated() )
    {
      setFilterCreated();

      IsoBus_c &c_can = getIsoBusInstance4Comm();
      c_can.insertFilter(*this, IsoAgLib::iMaskFilter_c( 0x3FFFFLU << 8, LIGHTING_DATA_PGN << 8), 8 );
      c_can.insertFilter(*this,IsoAgLib::iMaskFilter_c( 0x3FFFFLU << 8, LIGHTING_COMMAND_PGN << 8), 8 );
    }
  }

  bool TracLight_c::processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t )
  {
    // check if we are allowed to send a request for pgn
    if ( ! BaseCommon_c::check4ReqForPgn(aui32_pgn, apc_isoItemSender, apc_isoItemReceiver) ) return false;
    bool b_processed = true;
    // call TracLight_c function to send lighting information
    // sendMessage checks if this item (identified by ISOName)
    // is configured to send lighting information
    switch (aui32_pgn) {
    default:
      b_processed = false;
      break;
    case LIGHTING_DATA_PGN:
      mb_cmdWait4Response = true;
      b_processed = MessageSent == sendLightingData();
      break;
    case LIGHTING_COMMAND_PGN:
      mb_changeNeedBeSend = true;
      b_processed = MessageSent == sendLightingCommand();
      break;
    }
    return b_processed;
  }

  /** find out if a particular implement has responded to a command from the tractor
      @param sa   source address of the sought implement
      @return     has responded to a command (true), else (false)
    */
  bool TracLight_c::find(const uint8_t sa) const
   {
      STL_NAMESPACE::map<uint8_t, lightBitData_t>::const_iterator ci = mmap_data.find(sa);
      if (ci != mmap_data.end())
        return true;
      else
        return false;
  }

  /** get the status of a light of a special implement
      possible errors:
        * iLibErr_c::Range m_index exceeds map size
      @param acui_index index of implement
      @param at_command which light
      @return status of the light of the implement
    */
  IsoAgLib::IsoActiveFlag_t TracLight_c::getByIndex(const unsigned int acui_index, const IsoAgLib::IsoCommandFlag_t at_command)
  {
    STL_NAMESPACE::map<uint8_t, lightBitData_t>::iterator itr;
    const lightBitData_t* pt_data = NULL;
    unsigned int count = 0;

    for(itr = mmap_data.begin(); (itr != mmap_data.end() && count <= acui_index); ++itr)
    {
      if (count == acui_index)
        pt_data = &itr->second;
      count++;
    }
    if (pt_data != NULL)
      return getInfo(*pt_data, at_command);
    else {
      return IsoAgLib::IsoError;
    }
  }

  /** get the status of a light of a special implement
      possible errors:
        * iLibErr_c::Range the implement with sa has not responded to command from the tractor
      @param sa         source address of implement
      @param t_command  which light
      @return           status of the light of the implement
    */
  IsoAgLib::IsoActiveFlag_t TracLight_c::getBySa(const uint8_t sa, const IsoAgLib::IsoCommandFlag_t t_command) const
  {
    const lightBitData_t* pt_data = NULL;
    STL_NAMESPACE::map<uint8_t, lightBitData_t>::const_iterator itr = mmap_data.find(sa);

    if (itr != mmap_data.end())
      pt_data = &itr->second;
    if (pt_data != NULL)
      return getInfo(*pt_data, t_command);
    else {
      return IsoAgLib::IsoError;
    }
  }

  /** set a lighting command in tractor mode or set a response on a lighting command in implement mode
      @param t_command command to set
      @param t_val command value
    */
  void TracLight_c::setCommand(const IsoAgLib::IsoCommandFlag_t t_command, const IsoAgLib::IsoActiveFlag_t t_val)
  {
    switch (t_command) {
      case IsoAgLib::daytimeRunning:
        if (  (mt_cmd.daytimeRunning == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.daytimeRunning == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.daytimeRunning = t_val;
        break;
      case IsoAgLib::alternateHead:
        if (  (mt_cmd.alternateHead == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.alternateHead == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.alternateHead = t_val;
        break;
      case IsoAgLib::lowBeamHead:
        if (  (mt_cmd.lowBeamHead == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.lowBeamHead == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.lowBeamHead = t_val;
        break;
      case IsoAgLib::highBeamHead:
        if (  (mt_cmd.highBeamHead == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.highBeamHead == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.highBeamHead = t_val;
        break;
      case IsoAgLib::frontFog:
        if (  (mt_cmd.frontFog == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.frontFog == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.frontFog = t_val;
        break;
      case IsoAgLib::beacon:
        if (  (mt_cmd.beacon == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.beacon == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.beacon = t_val;
        break;
      case IsoAgLib::rightTurn:
        if (  (mt_cmd.rightTurn == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.rightTurn == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.rightTurn = t_val;
        break;
      case IsoAgLib::leftTurn:
        if (  (mt_cmd.leftTurn == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.leftTurn == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.leftTurn = t_val;
        break;
      case IsoAgLib::backUpLightAlarmHorn:
        if (  (mt_cmd.backUpLightAlarmHorn == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.backUpLightAlarmHorn == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.backUpLightAlarmHorn = t_val;
        break;
      case IsoAgLib::centerStop:
        if (  (mt_cmd.centerStop == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.centerStop == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.centerStop = t_val;
        break;
      case IsoAgLib::rightStop:
        if (  (mt_cmd.rightStop == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.rightStop == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.rightStop = t_val;
        break;
      case IsoAgLib::leftStop:
        if (  (mt_cmd.leftStop == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.leftStop == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.leftStop = t_val;
        break;
      case IsoAgLib::implClearance:
        if (  (mt_cmd.implClearance == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.implClearance == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.implClearance = t_val;
        break;
      case IsoAgLib::tracClearance:
        if (  (mt_cmd.tracClearance == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.tracClearance == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.tracClearance = t_val;
        break;
      case IsoAgLib::implMarker:
        if (  (mt_cmd.implMarker == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.implMarker == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.implMarker = t_val;
        break;
      case IsoAgLib::tracMarker:
        if (  (mt_cmd.tracMarker == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.tracMarker == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.tracMarker = t_val;
        break;
      case IsoAgLib::rearFog:
        if (  (mt_cmd.rearFog == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.rearFog == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.rearFog = t_val;
        break;
      case IsoAgLib::undersideWork:
        if (  (mt_cmd.undersideWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.undersideWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.undersideWork = t_val;
        break;
      case IsoAgLib::rearLowWork:
        if (  (mt_cmd.rearLowWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.rearLowWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.rearLowWork = t_val;
        break;
      case IsoAgLib::rearHighWork:
        if (  (mt_cmd.rearHighWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.rearHighWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.rearHighWork = t_val;
        break;
      case IsoAgLib::sideLowWork:
        if (  (mt_cmd.sideLowWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.sideLowWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.sideLowWork = t_val;
        break;
      case IsoAgLib::sideHighWork:
        if (  (mt_cmd.sideHighWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.sideHighWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.sideHighWork = t_val;
        break;
      case IsoAgLib::frontLowWork:
        if (  (mt_cmd.frontLowWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.frontLowWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.frontLowWork = t_val;
        break;
      case IsoAgLib::frontHighWork:
        if (  (mt_cmd.frontHighWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.frontHighWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.frontHighWork = t_val;
        break;
      case IsoAgLib::implOEMOpt2:
        if (  (mt_cmd.implOEMOpt2 == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.implOEMOpt2 == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.implOEMOpt2 = t_val;
        break;
      case IsoAgLib::implOEMOpt1:
        if (  (mt_cmd.implOEMOpt1 == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.implOEMOpt1 == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.implOEMOpt1 = t_val;
        break;
      case IsoAgLib::implRightForwardWork:
        if (  (mt_cmd.implRightForwardWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.implRightForwardWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.implRightForwardWork = t_val;
        break;
      case IsoAgLib::implLeftForwardWork:
        if (  (mt_cmd.implLeftForwardWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.implLeftForwardWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.implLeftForwardWork = t_val;
        break;
      case IsoAgLib::dataMsgReq:
        // this is the data message request flag which indicates if the lighting data message from all
        // implements should be send and NOT a flag for a light
        mt_cmd.dataMsgReq = t_val;
        break;
      case IsoAgLib::implRightFacingWork:
        if (  (mt_cmd.implRightFacingWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.implRightFacingWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.implRightFacingWork = t_val;
        break;
      case IsoAgLib::implLeftFacingWork:
        if (  (mt_cmd.implLeftFacingWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.implLeftFacingWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.implLeftFacingWork = t_val;
        break;
      case IsoAgLib::implRearWork:
        if (  (mt_cmd.implRearWork == IsoAgLib::IsoActive && t_val == IsoAgLib::IsoInactive)
            ||(mt_cmd.implRearWork == IsoAgLib::IsoInactive && t_val == IsoAgLib::IsoActive)
           )
          mb_changeNeedBeSend = true;
        mt_cmd.implRearWork = t_val;
        break;
    }
    if ( mb_changeNeedBeSend )
      sendMessage();
  }

  /** get command which the tractor send
      @param t_command  command from the tractor
      @return           value of the command which the tractor send
    */
  IsoAgLib::IsoActiveFlag_t TracLight_c::getCommand(const IsoAgLib::IsoCommandFlag_t t_command) const
  {
    return getInfo(mt_cmd, t_command);;
  }

  /** helper funktion to get the mode of a light of a specified implement
      @param at_data bit field where the lighting information is stored
      @param at_command light to find
      @return status of the light
    */
  IsoAgLib::IsoActiveFlag_t TracLight_c::getInfo(const lightBitData_t& at_data, const IsoAgLib::IsoCommandFlag_t at_command) const
  {
    switch (at_command)
    {
      case IsoAgLib::daytimeRunning:        return IsoAgLib::IsoActiveFlag_t(at_data.daytimeRunning);
      case IsoAgLib::alternateHead:         return IsoAgLib::IsoActiveFlag_t(at_data.alternateHead);
      case IsoAgLib::lowBeamHead:           return IsoAgLib::IsoActiveFlag_t(at_data.lowBeamHead);
      case IsoAgLib::highBeamHead:          return IsoAgLib::IsoActiveFlag_t(at_data.highBeamHead);
      case IsoAgLib::frontFog:              return IsoAgLib::IsoActiveFlag_t(at_data.frontFog);
      case IsoAgLib::beacon:                return IsoAgLib::IsoActiveFlag_t(at_data.beacon);
      case IsoAgLib::rightTurn:             return IsoAgLib::IsoActiveFlag_t(at_data.rightTurn);
      case IsoAgLib::leftTurn:              return IsoAgLib::IsoActiveFlag_t(at_data.leftTurn);
      case IsoAgLib::backUpLightAlarmHorn:  return IsoAgLib::IsoActiveFlag_t(at_data.backUpLightAlarmHorn);
      case IsoAgLib::centerStop:            return IsoAgLib::IsoActiveFlag_t(at_data.centerStop);
      case IsoAgLib::rightStop:             return IsoAgLib::IsoActiveFlag_t(at_data.rightStop);
      case IsoAgLib::leftStop:              return IsoAgLib::IsoActiveFlag_t(at_data.leftStop);
      case IsoAgLib::implClearance:         return IsoAgLib::IsoActiveFlag_t(at_data.implClearance);
      case IsoAgLib::tracClearance:         return IsoAgLib::IsoActiveFlag_t(at_data.tracClearance);
      case IsoAgLib::implMarker:            return IsoAgLib::IsoActiveFlag_t(at_data.implMarker);
      case IsoAgLib::tracMarker:            return IsoAgLib::IsoActiveFlag_t(at_data.tracMarker);
      case IsoAgLib::rearFog:               return IsoAgLib::IsoActiveFlag_t(at_data.rearFog);
      case IsoAgLib::undersideWork:         return IsoAgLib::IsoActiveFlag_t(at_data.undersideWork);
      case IsoAgLib::rearLowWork:           return IsoAgLib::IsoActiveFlag_t(at_data.rearLowWork);
      case IsoAgLib::rearHighWork:          return IsoAgLib::IsoActiveFlag_t(at_data.rearHighWork);
      case IsoAgLib::sideLowWork:           return IsoAgLib::IsoActiveFlag_t(at_data.sideLowWork);
      case IsoAgLib::sideHighWork:          return IsoAgLib::IsoActiveFlag_t(at_data.sideHighWork);
      case IsoAgLib::frontLowWork:          return IsoAgLib::IsoActiveFlag_t(at_data.frontLowWork);
      case IsoAgLib::frontHighWork:         return IsoAgLib::IsoActiveFlag_t(at_data.frontHighWork);
      case IsoAgLib::implOEMOpt2:           return IsoAgLib::IsoActiveFlag_t(at_data.implOEMOpt2);
      case IsoAgLib::implOEMOpt1:           return IsoAgLib::IsoActiveFlag_t(at_data.implOEMOpt1);
      case IsoAgLib::implRightForwardWork:  return IsoAgLib::IsoActiveFlag_t(at_data.implRightForwardWork);
      case IsoAgLib::implLeftForwardWork:   return IsoAgLib::IsoActiveFlag_t(at_data.implLeftForwardWork);
      case IsoAgLib::dataMsgReq:            return IsoAgLib::IsoActiveFlag_t(at_data.dataMsgReq);
      case IsoAgLib::implRightFacingWork:   return IsoAgLib::IsoActiveFlag_t(at_data.implRightFacingWork);
      case IsoAgLib::implLeftFacingWork:    return IsoAgLib::IsoActiveFlag_t(at_data.implLeftFacingWork);
      case IsoAgLib::implRearWork:          return IsoAgLib::IsoActiveFlag_t(at_data.implRearWork);
      //not necessarily essential, but it prevents the compiler from a warning
      default:                              return IsoAgLib::IsoNotAvailable;
    }
  }

  /** HIDDEN constructor for a TracLight_c object instance which can optional
      set the configuration for send/receive for a lighting msg type
      NEVER instantiate a variable of type TracLight_c within application
      only access TracLight_c via getTracLightInstance() or getTracLightInstance( int riLbsBusNr ) in case more than one ISO11783 BUS is used for IsoAgLib
    */
  TracLight_c::TracLight_c() {}


  void TracLight_c::processMsg( const CanPkg_c& arc_data )
  {    
    CanPkgExt_c pkg( arc_data, getMultitonInst() );
    if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
      return;

    IsoName_c const& rcc_tempISOName = pkg.getISONameForSA();

    lightBitData_t* pt_data = NULL;

    switch (pkg.isoPgn() /*& 0x3FFFF*/) // don't need to &, we're interested in the whole PGN!
    {
      case LIGHTING_DATA_PGN:
        // lighting state information is sent by more than one sender -> store ALL messages with SA as key in STL_NAMESPACE::map
        pt_data = &(mmap_data[pkg.isoSa()]);
        if (pt_data != NULL)
          pt_data->dataMsgReq = IsoAgLib::IsoDontCare; //reserved field in lighting data
        break;
      case LIGHTING_COMMAND_PGN:
        // CMD is EXCLUSIVELY SENT BY ONE TRACTOR ECU!!! --> CHECK
        if ( checkParseReceived (rcc_tempISOName) )
        { // sender is allowed to send
          pt_data = &mt_cmd;
          mt_cmd.dataMsgReq = IsoAgLib::IsoDataReq_t( pkg.getUint8Data(7) & 3 );

          if (mt_cmd.dataMsgReq == IsoAgLib::IsoDataRequested)
            mb_cmdWait4Response = true;

          // set last time - use the array of send time stamps which is needed in tractor mode
          // in implement mode, the first item can be used to trace received tractor commands
          marr_timeStamp[0] = pkg.time();

          setSelectedDataSourceISOName (rcc_tempISOName);
          setUpdateTime( pkg.time() );
        } else
        { // there is a sender conflict
          IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracMultipleSender, getMultitonInst() );
        }
        break;
    }

    if ( pt_data == NULL )
    { // preconditions for parsing of this message are NOT fullfilled --> exit function with false
      return;
    }
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // from here on, we can safely process the message as all preconditions are fullfilled
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    uint16_t ui16_temp = pkg.getUint16Data( 0 );
    pt_data->daytimeRunning =       IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  0) & 3 ) ;
    pt_data->alternateHead  =       IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  2) & 3 ) ;
    pt_data->lowBeamHead  =         IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  4) & 3 ) ;
    pt_data->highBeamHead  =        IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  6) & 3 ) ;
    pt_data->frontFog =             IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  8) & 3 ) ;
    pt_data->beacon  =              IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 10) & 3 ) ;
    pt_data->rightTurn  =           IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 12) & 3 ) ;
    pt_data->leftTurn  =            IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 14) & 3 ) ;
    ui16_temp = pkg.getUint16Data( 2 );
    pt_data->backUpLightAlarmHorn = IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  0) & 3 ) ;
    pt_data->centerStop  =          IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  2) & 3 ) ;
    pt_data->rightStop  =           IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  4) & 3 ) ;
    pt_data->leftStop  =            IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  6) & 3 ) ;
    pt_data->implClearance =        IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  8) & 3 ) ;
    pt_data->tracClearance  =       IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 10) & 3 ) ;
    pt_data->implMarker  =          IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 12) & 3 ) ;
    pt_data->tracMarker  =          IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 14) & 3 ) ;
    ui16_temp = pkg.getUint16Data( 4 );
    pt_data->rearFog =              IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  0) & 3 ) ;
    pt_data->undersideWork  =       IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  2) & 3 ) ;
    pt_data->rearLowWork  =         IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  4) & 3 ) ;
    pt_data->rearHighWork  =        IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  6) & 3 ) ;
    pt_data->sideLowWork =          IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  8) & 3 ) ;
    pt_data->sideHighWork  =        IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 10) & 3 ) ;
    pt_data->frontLowWork  =        IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 12) & 3 ) ;
    pt_data->frontHighWork  =       IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 14) & 3 ) ;
    ui16_temp = pkg.getUint16Data( 6 );
    pt_data->implOEMOpt2 =          IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  0) & 3 ) ;
    pt_data->implOEMOpt1  =         IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  2) & 3 ) ;
    pt_data->implRightForwardWork  =IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  4) & 3 ) ;
    pt_data->implLeftForwardWork  = IsoAgLib::IsoActiveFlag_t( (ui16_temp >>  6) & 3 ) ;
    // pt_data->dataMsgReq is treated separately
    pt_data->implRightFacingWork  = IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 10) & 3 ) ;
    pt_data->implLeftFacingWork  =  IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 12) & 3 ) ;
    pt_data->implRearWork  =        IsoAgLib::IsoActiveFlag_t( (ui16_temp >> 14) & 3 ) ;

    if ( mb_cmdWait4Response )
      sendMessage();
  }

  /** functions with actions, which must be performed periodically
      -> called periodically by Scheduler_c
      ==> sends msg if configured in the needed rates
      possible errors:
        * dependant error in CanIo_c on CAN send problems
      @pre  function is only called in tractor mode
      @see  BaseCommon_c::timeEvent()
    */
  void TracLight_c::timeEventTracMode( )
  {
    // directly call sendMessage() as this function performs the needed pre checks
    sendMessage();
  }

  /** functions with actions, which must be performed periodically
      -> called periodically by Scheduler_c
      ==> sends msg if configured in the needed rates
      possible errors:
        * dependant error in CanIo_c on CAN send problems
      @pre  function is only called in implement mode
      @see  BaseCommon_c::timeEvent()
    */
  void TracLight_c::timeEventImplMode( )
  {
    // directly call sendMessage() as this function performs the needed pre checks
    sendMessage();
  }

  TracLight_c::SendMessage_e TracLight_c::helpSendMessage( CanPkgExt_c& pkg )
  {
    // there is no need to check for address claim in tractor mode because this is already done in the timeEvent
    // function of base class BaseCommon_c

    pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
    pkg.setLen(8);

    uint16_t ui16_temp = 0;
    ui16_temp =
      (mt_cmd.daytimeRunning <<  0) +
      (mt_cmd.alternateHead  <<  2) +
      (mt_cmd.lowBeamHead    <<  4) +
      (mt_cmd.highBeamHead   <<  6) +
      (mt_cmd.frontFog       <<  8) +
      (mt_cmd.beacon         << 10) +
      (mt_cmd.rightTurn      << 12) +
      (mt_cmd.leftTurn       << 14);
    pkg.setUint16Data(0, ui16_temp);
    ui16_temp = 0;
    ui16_temp =
      (mt_cmd.backUpLightAlarmHorn <<  0) +
      (mt_cmd.centerStop           <<  2) +
      (mt_cmd.rightStop            <<  4) +
      (mt_cmd.leftStop             <<  6) +
      (mt_cmd.implClearance        <<  8) +
      (mt_cmd.tracClearance        << 10) +
      (mt_cmd.implMarker           << 12) +
      (mt_cmd.tracMarker           << 14);
    pkg.setUint16Data(2, ui16_temp);
    ui16_temp = 0;
    ui16_temp =
      (mt_cmd.rearFog       <<  0) +
      (mt_cmd.undersideWork <<  2) +
      (mt_cmd.rearLowWork   <<  4) +
      (mt_cmd.rearHighWork  <<  6) +
      (mt_cmd.sideLowWork   <<  8) +
      (mt_cmd.sideHighWork  << 10) +
      (mt_cmd.frontLowWork  << 12) +
      (mt_cmd.frontHighWork << 14);
    pkg.setUint16Data(4, ui16_temp);

    // CanIo_c::operator<< retreives the information with the help of CanPkg_c::getData
    // then it sends the data
    getIsoBusInstance4Comm() << pkg;
    return MessageSent;
  }

  TracLight_c::SendMessage_e TracLight_c::sendLightingData()
  { // there is no need to check for address claim in tractor mode because this is already done in the timeEvent
    // function of base class BaseCommon_c
    CanPkgExt_c pkg;

    // precondition checks for sending as implement
    if (!mb_cmdWait4Response) return MessageNotSent;

    if( ( ! getIdentItem() ) || ( ! getIdentItem()->isClaimedAddress() ) ) {
      return MessageNotSent;
    }

    pkg.setIsoPgn(LIGHTING_DATA_PGN);
    //reset flag because msg will now be send
    mb_cmdWait4Response = false;

    pkg.setIsoPri(6);

    uint16_t ui16_temp = 0;
    ui16_temp =
      (mt_cmd.implOEMOpt2          <<  0) +
      (mt_cmd.implOEMOpt1          <<  2) +
      (mt_cmd.implRightForwardWork <<  4) +
      (mt_cmd.implLeftForwardWork  <<  6) +
      (0                          <<  8) +    //reserved field in lighting data
      (mt_cmd.implRightFacingWork  << 10) +
      (mt_cmd.implLeftFacingWork   << 12) +
      (mt_cmd.implRearWork         << 14);
    pkg.setUint16Data(6, ui16_temp);

    return helpSendMessage( pkg );
  }

  TracLight_c::SendMessage_e TracLight_c::sendLightingCommand()
  {
    int32_t const ci32_now = System_c::getTime();

    CanPkgExt_c pkg;

    // tractor mode
    if ( ( ci32_now - marr_timeStamp[m_index] ) <= 1000 )
    { // WE ARE NOT ALLOWED TO SEND - EVEN IF REQUESTED
      return MessageNotSent;
    }
    else if ( (!mb_changeNeedBeSend) && ( (ci32_now - marr_timeStamp[(m_index+9)%10]) < 900 ) ) // (m_index+9)%10 -> youngest entry in array marr_timeStamp[];
    { // to send requested (i.e. no change occured) or not yet time to repeat last command
      return MessageNotSent;
    }
    mb_changeNeedBeSend = false;
    // now it's evident, that we have to send a command
    pkg.setIsoPgn(LIGHTING_COMMAND_PGN);
    setSelectedDataSourceISOName( getIdentItem()->isoName() );

    pkg.setIsoPri(3);
    uint16_t ui16_temp = 0;
    ui16_temp =
      (mt_cmd.implOEMOpt2          <<  0) +
      (mt_cmd.implOEMOpt1          <<  2) +
      (mt_cmd.implRightForwardWork <<  4) +
      (mt_cmd.implLeftForwardWork  <<  6) +
      (mt_cmd.dataMsgReq           <<  8) +
      (mt_cmd.implRightFacingWork  << 10) +
      (mt_cmd.implLeftFacingWork   << 12) +
      (mt_cmd.implRearWork         << 14);
    pkg.setUint16Data(6, ui16_temp);
    //overwrite the eldest time event with latest time event
    marr_timeStamp[m_index] = ci32_now;

    //set m_index to the eldest time event
    m_index = (m_index + 1) % 10;

    return helpSendMessage( pkg );
  }

  /** send light update; there is a difference between implement and tractor mode
      @see  TracLight_c::processMsgRequestPGN
      @see  CanIo_c::operator<<
    */
  void TracLight_c::sendMessage()
  {
    bool const b_isTractor = checkMode(IsoAgLib::IdentModeTractor);
    return (void)(b_isTractor ? sendLightingCommand() : sendLightingData());
  }


} // end of namespace __IsoAgLib
