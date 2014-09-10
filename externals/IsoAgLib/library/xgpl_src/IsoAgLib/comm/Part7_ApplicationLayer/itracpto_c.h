/*
  itracpto_c.h: working on Base Data Msg Type NMEA 200 GPS and
    Calendar; stores, updates and delivers all base data informations
    from CanCustomer_c derived for CAN sending and receiving
    interaction; from SchedulerTask_c derived for interaction with
    other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ITRACPTO_C_H
#define ITRACPTO_C_H

/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */
#include <IsoAgLib/comm/Part5_NetworkManagement/iidentitem_c.h>
#include "ibasetypes.h"
#include "impl/tracpto_c.h"

// Begin Namespace IsoAgLib
namespace IsoAgLib {

  /** working on Base Data Msg Type 1;
      stores, updates  and delivers all base data informations;
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */
  class iTracPTO_c : private __IsoAgLib::TracPTO_c {
  public:
  /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
      @param at_identMode set mode to either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      @return true -> configuration was successfull
    */
  bool config ( const iIdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode = IsoAgLib::IdentModeImplement, uint16_t aui16_suppressMask = 0 )
  { return TracPTO_c::config_base ( apc_ident, at_identMode, aui16_suppressMask ); }

/* ******************************************* */
    /** \name Set Values for periodic send on BUS  */
    /*@{*/

    /** set rear PTO
        @param aui16_val value to store as the speed of the rear PTO
      */
    void setPtoRear(const uint16_t aui16_val) { TracPTO_c::setPtoRear(aui16_val);}

    /** set front PTO
        @param aui16_val value to store as the speed of the front PTO
      */
    void setPtoFront(const uint16_t aui16_val) { TracPTO_c::setPtoFront(aui16_val);}

    /** set explicit information whether front PTO is engaged
      * @param at_val IsoActive -> PTO is engaged
      */
    void setPtoFrontEngaged(const IsoAgLib::IsoActiveFlag_t at_val) { TracPTO_c::setPtoFrontEngaged(at_val);}

    /** set explicit information whether rear PTO is engaged
      * @param at_val IsoActive -> PTO is engaged
      */
    void setPtoRearEngaged(const IsoAgLib::IsoActiveFlag_t at_val) { TracPTO_c::setPtoRearEngaged(at_val);}

    /** set std RPM of front PTO (1000 n/min; 540 n/min)
      * @param at_val IsoActive == 1000 n/min; false == 540 n/min
      */
    void setPtoFront1000(const IsoAgLib::IsoActiveFlag_t at_val) {  TracPTO_c::setPtoFront1000(at_val);}

    /** set std RPM of rear PTO (1000 n/min; 540 n/min)
      * @param at_val IsoActive == 1000 n/min; false == 540 n/min
      */
    void setPtoRear1000(const IsoAgLib::IsoActiveFlag_t at_val) { TracPTO_c::setPtoRear1000(at_val);}

    /** set economy mode of front PTO
      * @param at_val IsoActive -> front PTO is in economy mode
      */
    void setPtoFrontEconomy(const IsoAgLib::IsoActiveFlag_t at_val) { TracPTO_c::setPtoFrontEconomy(at_val);}

    /** set economy mode of rear PTO
      * @param at_val IsoActive -> rear PTO is in economy mode
      */
    void setPtoRearEconomy(const IsoAgLib::IsoActiveFlag_t at_val) { TracPTO_c::setPtoRearEconomy(at_val);}

    /** set measured value of the set point of the rotational speed of the front power take-off (PTO) output shaft
      * @param ai16_val
      */
    void setFrontPtoSetPoint(const uint16_t ai16_val)  { TracPTO_c::setFrontPtoSetPoint(ai16_val);}

    /** set measured value of the set point of the rotational speed of the rear power take-off (PTO) output shaft
      *  @param ai16_val
      */
    void setRearPtoSetPoint(const uint16_t ai16_val)  { TracPTO_c::setRearPtoSetPoint(ai16_val);}

    /** set reported tractor ECU's status of front engagement
      *  @param at_val  status to set
      */
    void setFrontPtoEngagementReqStatus(IsoAgLib::IsoReqFlag_t at_val) {TracPTO_c::setFrontPtoEngagementReqStatus(at_val);}

    /** set reported tractor ECU's status of rear engagement
      *  @param at_val  status to set
      */
    void setRearPtoEngagementReqStatus(IsoAgLib::IsoReqFlag_t at_val) {TracPTO_c::setRearPtoEngagementReqStatus(at_val);}

    /** set reported tractor ECU's status of front mode
      *  @param at_val  status to set
      */
    void setFrontPtoModeReqStatus(IsoAgLib::IsoReqFlag_t at_val) {TracPTO_c::setFrontPtoModeReqStatus(at_val);}

    /** set reported tractor ECU's status of rear mode
      *  @param at_val  status to set
      */
    void setRearPtoModeReqStatus(IsoAgLib::IsoReqFlag_t at_val) {TracPTO_c::setRearPtoModeReqStatus(at_val);}

    /** set reported tractor ECU's status of front economy mode
      *  @param at_val  status to set
      */
    void setFrontPtoEconomyModeReqStatus(IsoAgLib::IsoReqFlag_t at_val) {TracPTO_c::setFrontPtoEconomyModeReqStatus(at_val);}

    /** set reported tractor ECU's status of rear economy mode
      * @param at_val  status to set
      */
    void setRearPtoEconomyModeReqStatus(IsoAgLib::IsoReqFlag_t at_val) {TracPTO_c::setRearPtoEconomyModeReqStatus(at_val);}

    /** set present limit status of front pto shaft speed
      *  @param at_val  status to set
      */
    void setFrontPtoShaftSpeedLimitStatus(IsoAgLib::IsoLimitFlag_t at_val) {TracPTO_c::setFrontPtoShaftSpeedLimitStatus(at_val);}

    /** set present limit status of rear pto shaft speed
      *  @param at_val  status to set
      */
    void setRearPtoShaftSpeedLimitStatus(IsoAgLib::IsoLimitFlag_t at_val) {TracPTO_c::setRearPtoShaftSpeedLimitStatus(at_val);}
    /*@}*/

    /* ****************************************************** */
    /** \name Retrieve Values which are sent from other ECUs  */
    /*@{*/

    /** get rear pto
      *  @return actual rpm speed of rear PTO
      */
    uint16_t ptoRear() const { return TracPTO_c::ptoRear();}

    /** get front pto
      *  @return actual rpm speed of front PTO
      */
    uint16_t ptoFront() const { return TracPTO_c::ptoFront();}

    /** deliver explicit information whether front PTO is engaged
      * @return IsoActive -> PTO is engaged
      */
    IsoAgLib::IsoActiveFlag_t ptoFrontEngaged() const { return TracPTO_c::ptoFrontEngaged();}

    /** deliver explicit information whether rear PTO is engaged
      * @return IsoActive -> PTO is engaged
      */
    IsoAgLib::IsoActiveFlag_t ptoRearEngaged() const { return TracPTO_c::ptoRearEngaged();}

    /** deliver std RPM of front PTO (1000 n/min; 540 n/min)
      * @return IsoActive == 1000 n/min; false == 540 n/min
      */
    IsoAgLib::IsoActiveFlag_t ptoFront1000() const { return TracPTO_c::ptoFront1000();}

    /** deliver std RPM of rear PTO (1000 n/min; 540 n/min)
      * @return IsoActive == 1000 n/min; false == 540 n/min
      */
    IsoAgLib::IsoActiveFlag_t ptoRear1000() const { return TracPTO_c::ptoRear1000();}

    /** deliver economy mode of front PTO
      * @return IsoActive -> front PTO is in economy mode
      */
    IsoAgLib::IsoActiveFlag_t ptoFrontEconomy() const { return TracPTO_c::ptoFrontEconomy();}

    /** deliver economy mode of rear PTO
      * @return IsoActive -> rear PTO is in economy mode
      */
    IsoAgLib::IsoActiveFlag_t ptoRearEconomy() const { return TracPTO_c::ptoRearEconomy();}

    /** set measured value of the set point of the rotational speed of the front power take-off (PTO) output shaft
      * @return
      */
    uint16_t frontPtoSetPoint()const { return TracPTO_c::frontPtoSetPoint();}

    /** set measured value of the set point of the rotational speed of the rear power take-off (PTO) output shaft
      * @return
      */
    uint16_t rearPtoSetPoint()const { return TracPTO_c::rearPtoSetPoint();}

    /** get reported tractor ECU's status of front engagement
      * @return  reported status
      */
    IsoAgLib::IsoReqFlag_t frontPtoEngagementReqStatus() const {return TracPTO_c::frontPtoEngagementReqStatus();}

    /** get reported tractor ECU's status of rear engagement
      * @return  reported status
      */
    IsoAgLib::IsoReqFlag_t rearPtoEngagementReqStatus() const {return TracPTO_c::rearPtoEngagementReqStatus();}

    /** get reported tractor ECU's status of front mode
      *  @return  reported status
      */
    IsoAgLib::IsoReqFlag_t frontPtoModeReqStatus() const {return TracPTO_c::frontPtoModeReqStatus();}

    /** get reported tractor ECU's status of rear mode
      * @return  reported status
      */
    IsoAgLib::IsoReqFlag_t rearPtoModeReqStatus() const {return TracPTO_c::rearPtoModeReqStatus();}

    /** get reported tractor ECU's status of front economy mode
      * @return  reported status
      */
    IsoAgLib::IsoReqFlag_t frontPtoEconomyModeReqStatus() const {return TracPTO_c::frontPtoEconomyModeReqStatus();}

    /** get reported tractor ECU's status of rear economy mode
      * @return  reported status
      */
    IsoAgLib::IsoReqFlag_t rearPtoEconomyModeReqStatus()const {return TracPTO_c::rearPtoEconomyModeReqStatus();}

    /** get present limit status of front pto shaft speed
      * @return  present limit status
      */
    IsoAgLib::IsoLimitFlag_t frontPtoShaftSpeedLimitStatus() const {return TracPTO_c::frontPtoShaftSpeedLimitStatus();}

    /** get present limit status of rear pto shaft speed
      * @return  present limit status
      */
    IsoAgLib::IsoLimitFlag_t rearPtoShaftSpeedLimitStatus() const {return TracPTO_c::rearPtoShaftSpeedLimitStatus();}

    /** force a request for pgn for front pto state */
    bool sendRequestUpdateFront() { return TracPTO_c::sendRequestUpdateFront(); }

    /** force a request for pgn for rear pto state */
    bool sendRequestUpdateRear() { return TracPTO_c::sendRequestUpdateRear(); }
    /*@}*/

  private:
    /** allow getITracMoveInstance() access to shielded tracgeneral class.
    otherwise __IsoAgLib::getTracMoveInstance4Comm() wouldn't be accepted by compiler
    */
    #if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
    friend iTracPTO_c& getITracPtoInstance(uint8_t aui8_instance);
    #else
    friend iTracPTO_c& getITracPtoInstance(void);
    #endif

  };

  #if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
  /** C-style function, to get access to the unique TracMove_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  inline iTracPTO_c& getITracPtoInstance(uint8_t aui8_instance = 0)
  { return static_cast<iTracPTO_c&>(__IsoAgLib::getTracPtoInstance(aui8_instance));}
  #else
  /** C-style function, to get access to the unique TracMove_c singleton instance */
  inline iTracPTO_c& getITracPtoInstance(void)
  { return static_cast<iTracPTO_c&>(__IsoAgLib::getTracPtoInstance());}
  #endif
}
#endif
