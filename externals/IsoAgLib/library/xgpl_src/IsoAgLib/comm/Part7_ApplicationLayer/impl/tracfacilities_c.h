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
#ifndef TRACFACILITIES_C_H
#define TRACFACILITIES_C_H

#include <IsoAgLib/comm/Part7_ApplicationLayer/impl/basecommon_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgnhandler_c.h>

namespace __IsoAgLib {


  /** stores, updates  and delivers all Facilities data information;
      Derive from BaseCommon_c some fundamental funktionality for all base data
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */
class TracFacilities_c : public BaseCommon_c
{
  MACRO_MULTITON_CONTRIBUTION();
public:// Public methods

  /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
      @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      @return true -> configuration was successfull
   */
  virtual bool config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask = 0);

  virtual ~TracFacilities_c() {}

  bool processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t );

  /* ******************************************* */
  /** \name Set Values for periodic send on BUS  */
  /*@{*/

  /** set tecu class with class t_val
      @param t_val  type of tecu class
    */
  void setTractorClass(const IsoAgLib::IsoTecuClassFlag_t at_val);

  /** set t_facility facility with value t_val
      @param t_facility  type of facility to be set
      @param t_val       type of facility flag
    */
  void setFacitity(const IsoAgLib::IsoFacility_t at_facility, const IsoAgLib::IsoFacilityFlag_t at_val);

  /* ****************************************************** */
  /** \name Retrieve Values which are sent from other ECUs  */
  /*@{*/
  /** get tecu class
      @return  type of tecu class
    */
  IsoAgLib::IsoTecuClassFlag_t getTractorClass() const {return IsoAgLib::IsoTecuClassFlag_t(arrui8_facilitiesBitData[0] >> 6);}

  /** get facility flag for t_facility
      @param at_facility  type of facility to be set
      @return  type of facility flag
    */
  IsoAgLib::IsoFacilityFlag_t getFacitity(const IsoAgLib::IsoFacility_t at_facility) const;

 private:
  // Private methods
  /** HIDDEN constructor for a TracFacilities_c object instance which can optional
      set the configuration for send/receive for a moving msg
      NEVER instantiate a variable of type TracFacilities_c within application
      only access TracFacilities_c via getTracFacilitiesInstance() or getTracFacilitiesInstance( int riLbsBusNr ) in case more than one ISO11783 BUS is used for IsoAgLib
    */
  TracFacilities_c();

  /** check if filter boxes shall be created - create only ISO filters based
      on active local idents which has already claimed an address
      --> avoid to much Filter Boxes
    */
  virtual void checkCreateReceiveFilter( );

  /** this is only called when sending ident is configured and it has already claimed an address
  @pre  function is only called in tractor mode
  @see  BaseCommon_c::timeEvent()
     */
  virtual void timeEventTracMode();

  virtual void processMsg( const CanPkg_c& arc_data );

  /** send guidance data
  @see  TracFacilities_c::processMsgRequestPGN
  @see  CanIo_c::operator<<
   */
  void sendFacilities();

 private:
    // Private attributes
    /** struct of facilities data */
    uint8_t arrui8_facilitiesBitData[8];

    /** check if facilities already sent after address claim */
    bool mb_facilitiesOnInitSent;
    friend TracFacilities_c &getTracFacilitiesInstance(uint8_t aui8_instance);
};

  TracFacilities_c &getTracFacilitiesInstance(uint8_t aui8_instance = 0);
}

#endif
