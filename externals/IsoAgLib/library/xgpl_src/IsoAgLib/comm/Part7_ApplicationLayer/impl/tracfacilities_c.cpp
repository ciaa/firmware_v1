/*
  tracfacilities_c: working on facilities data; data information from
    CanCustomer_c derived for CAN sending and receiving interaction;
    from BaseCommon_c derived for interaction with other IsoAgLib
    objects. This class handles Part 7 - B.25.3 Tractor facilities
    response message 0xFE09 in both directions (configurable)

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>
#include "tracfacilities_c.h"
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgn_c.h>


namespace __IsoAgLib { // Begin Namespace __IsoAglib

  /** C-style function, to get access to the unique TracFacilities_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an m_index must be given to select the wanted BUS
    */
  TracFacilities_c &getTracFacilitiesInstance( uint8_t aui8_instance )
  { // if > 1 singleton instance is used, no static reference can be used
    MACRO_MULTITON_GET_INSTANCE_BODY(TracFacilities_c, PRT_INSTANCE_CNT, aui8_instance);
  }

  /** HIDDEN constructor for a TracFacilities_c object instance which can optional
      set the configuration for send/receive for a moving msg
      NEVER instantiate a variable of type TracFacilities_c within application
      only access TracFacilities_c via getTracFacilitiesInstance() or getTracFacilitiesInstance( int riLbsBusNr ) in case more than one ISO11783 BUS is used for IsoAgLib
    */
  TracFacilities_c::TracFacilities_c()
  : mb_facilitiesOnInitSent(false)
  {
    CNAMESPACE::memset(arrui8_facilitiesBitData,0,sizeof(arrui8_facilitiesBitData));
  }


  bool TracFacilities_c::config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask )
  {
    //store old mode to decide to register or unregister from request for pgn
    IsoAgLib::IdentMode_t t_oldMode = getMode();

    //call config for handling which is base data independent
    //if something went wrong leave function before something is configured
    if ( ! BaseCommon_c::config_base ( apc_ident, at_identMode, aui16_suppressMask) ) return false;

    ///Set time Period for Scheduler_c
    if (at_identMode == IsoAgLib::IdentModeTractor)
    { // SEND facilities at start
      mt_task.setPeriod( 100, false );
    }

    if (t_oldMode == IsoAgLib::IdentModeImplement && at_identMode == IsoAgLib::IdentModeTractor)
    {  // a change from Implement mode to Tractor mode occured
      // create FilterBox_c for REQUEST_PGN_MSG_PGN, REQUIRED_TRACTOR_FACILITIES_PGN
      getRegisterPgn()(TRACTOR_FACILITIES_PGN); // request for certificate
    }

    if (t_oldMode == IsoAgLib::IdentModeTractor && at_identMode == IsoAgLib::IdentModeImplement)
    {  // a change from Tractor mode to Implement mode occured
      // unregister from request for pgn, because in implement mode no request should be answered
      getUnregisterPgn()(TRACTOR_FACILITIES_PGN);
    }

    /// init all value in facilitiesBitData_t with default Not Available
    CNAMESPACE::memset(arrui8_facilitiesBitData,0,sizeof(arrui8_facilitiesBitData));

    return true;
  }

  /** check if filter boxes shall be created - create only ISO filters based
      on active local idents which has already claimed an address
      --> avoid to much Filter Boxes
    */
  void TracFacilities_c::checkCreateReceiveFilter( )
  {
    if( !checkFilterCreated() )
    {
      setFilterCreated();

      IsoBus_c &c_can = getIsoBusInstance4Comm();
      c_can.insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FFFF00UL, (TRACTOR_FACILITIES_PGN<<8) ), 8 );
    }
  }

  bool TracFacilities_c::processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t )
  {
    // check if we are allowed to send a request for pgn
    if ( ! BaseCommon_c::check4ReqForPgn(aui32_pgn, apc_isoItemSender, apc_isoItemReceiver) ) return false;

    // call TracFacilities_c function to send facilities status of a TECU
    // isoSendMessage checks if this item (identified by ISOName)
    // is configured to send facilities status
    sendFacilities();
    return true;
  };

  void TracFacilities_c::processMsg( const CanPkg_c& arc_data )
  {
    CanPkgExt_c pkg( arc_data, getMultitonInst() );
    if( !pkg.isValid() || (pkg.getMonitorItemForSA() == NULL) )
      return;

    IsoName_c const& rcc_tempISOName = pkg.getISONameForSA();

    switch (pkg.isoPgn() & 0x3FF00LU)
    {
      case TRACTOR_FACILITIES_PGN:
        if ( checkParseReceived (rcc_tempISOName) )
        { // sender is allowed to send

          for (uint8_t ui8_counter = 0;ui8_counter<sizeof(arrui8_facilitiesBitData)/sizeof(uint8_t);++ui8_counter)
            arrui8_facilitiesBitData[ui8_counter] = pkg.getUint8Data(ui8_counter);

          setSelectedDataSourceISOName (rcc_tempISOName);
          setUpdateTime( pkg.time() );
        } else
        { // there is a sender conflict
          IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::TracMultipleSender, getMultitonInst() );
        }
      break;
    }
  }

  /** send facilities data
      @see  TracFacilities_c::processMsgRequestPGN
      @see  CanIo_c::operator<<
    */
  void TracFacilities_c::sendFacilities( )
  {// there is no need to check for address claim because this is already done in the processMsgRequestPGN;
   // this function is only be called on request!!
    isoaglib_assert( getIdentItem() );

    IsoBus_c& c_can = getIsoBusInstance4Comm();

    CanPkgExt_c pkg;
    pkg.setMonitorItemForSA( getIdentItem()->getIsoItem() );
    setSelectedDataSourceISOName( getIdentItem()->isoName() );
    pkg.setIsoPri(3);
    pkg.setLen(8);

    pkg.setIsoPgn(TRACTOR_FACILITIES_PGN);

    for (uint8_t ui8_counter = 0;ui8_counter<sizeof(arrui8_facilitiesBitData)/sizeof(uint8_t);++ui8_counter)
      pkg.setUint8Data(ui8_counter,arrui8_facilitiesBitData[ui8_counter]);

    // CanIo_c::operator<< retreives the information with the help of CanPkg_c::getData
    // then it sends the data
    c_can << pkg;
  }

  void TracFacilities_c::setTractorClass(const IsoAgLib::IsoTecuClassFlag_t at_val)
  {
    arrui8_facilitiesBitData[ 0 ] &= ~(0x3    << 6);
    arrui8_facilitiesBitData[ 0 ] |=  (at_val << 6);
  }

  void TracFacilities_c::setFacitity(const IsoAgLib::IsoFacility_t at_facility, const IsoAgLib::IsoFacilityFlag_t at_val)
  {
    if (uint32_t(at_facility/8) < sizeof(arrui8_facilitiesBitData))
    {
      arrui8_facilitiesBitData[ at_facility/8 ] &= ~(0x1    << (7 - (at_facility % 8)));
      arrui8_facilitiesBitData[ at_facility/8 ] |=  (at_val << (7 - (at_facility % 8)));
    }
  }

  IsoAgLib::IsoFacilityFlag_t TracFacilities_c::getFacitity(const IsoAgLib::IsoFacility_t at_facility) const
  {
    if (uint32_t(at_facility/8) < sizeof(arrui8_facilitiesBitData))
    {
      return IsoAgLib::IsoFacilityFlag_t((arrui8_facilitiesBitData[at_facility/8] >> (7 - (at_facility % 8))) & 0x1);
    }
    return IsoAgLib::IsoFacilityNotAvailable;
  }

  /** this is only called when sending ident is configured and it has already claimed an address
  @pre  function is only called in tractor mode
  @see  BaseCommon_c::timeEvent()
   */
  void TracFacilities_c::timeEventTracMode( )
  {
    if (!mb_facilitiesOnInitSent)
    {
      // send Tractor Facility PGN once at start
      sendFacilities();
      // scheduler not needed anymore -> set period to big value
      mt_task.setPeriod ( 3600000, false );

      mb_facilitiesOnInitSent = true;
    }
  }

}// End Namespace __IsoAglib
