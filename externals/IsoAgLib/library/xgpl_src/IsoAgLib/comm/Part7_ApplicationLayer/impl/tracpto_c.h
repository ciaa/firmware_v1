/*
  tracpto_c.h: working on pto data; stores, updates and delivers all
    base data informations from CanCustomer_c derived for CAN sending
    and receiving interaction; from SchedulerTask_c derived for
    interaction with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TRACPTO_C_H
#define TRACPTO_C_H

#include <IsoAgLib/comm/Part7_ApplicationLayer/impl/basecommon_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgnhandler_c.h>

namespace __IsoAgLib { // Begin Namespace __IsoAgLib

/** decide which pto information to send (front/rear) */
typedef enum
{
  sendFrontPto = 0,
  sendRearPto  = 1
} SendPtoData_t;

/** pto information */
typedef struct
{
  /** last time of pto data msg [msec] */
  int32_t i32_lastPto;
  /** pto [1/8RPM] */
  uint16_t ui16_pto8DigitPerRpm;
  /** measured value of the set point of the rotational speed of the power take-off (PTO) output shaft [1/8RPM] */
  uint16_t ui16_ptoSetPoint8DigitPerRpm;
  /** PTO engaged */
  IsoAgLib::IsoActiveFlag_t t_ptoEngaged : 2;
  /** mode of PTO: true == 1000 n/min; false == 540 n/min */
  IsoAgLib::IsoActiveFlag_t t_pto1000 : 2;
  /** economy mode of PTO */
  IsoAgLib::IsoActiveFlag_t t_ptoEconomy : 2;
  /** report tractor ECU's status of engagement */
  IsoAgLib::IsoReqFlag_t t_ptoEngagementReqStatus : 2;
  /** report tractor ECU's status of mode */
  IsoAgLib::IsoReqFlag_t t_ptoModeReqStatus : 2;
  /** report tractor ECU's status of economy mode */
  IsoAgLib::IsoReqFlag_t t_ptoEconomyModeReqStatus : 2;
  /** present limit status of pto shaft speed */
  IsoAgLib::IsoLimitFlag_t t_ptoShaftSpeedLimitStatus : 3;
} PtoData_t;

  /** working on Base Data Msg;
      stores, updates and delivers all base data informations;
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */
  class TracPTO_c : public BaseCommon_c
  {
    MACRO_MULTITON_CONTRIBUTION();
  public:// Public methods
    /* ********************************************* */
    /** \name Management Functions for class TracPTO_c  */
    /*@{*/

    /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
        @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
        @return true -> configuration was successfull
      */
    virtual bool config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_IdentMode, uint16_t aui16_suppressMask = 0);

    virtual ~TracPTO_c() {}
    /*@}*/

    /* ******************************************* */
    /** \name Set Values for periodic send on BUS  */
    /*@{*/

    /** set rear PTO
      @param aui16_val value to store as the speed of the rear PTO [1RPM]
      */
    void setPtoRear(const uint16_t aui16_val)
    {
      mt_ptoRear.ui16_pto8DigitPerRpm = ( aui16_val * 8 );

      if ( aui16_val == 0 ) setPtoRearEngaged( IsoAgLib::IsoInactive );
      else                  setPtoRearEngaged( IsoAgLib::IsoActive );
    }

    /** set front PTO
        @param aui16_val value to store as the speed of the front PTO [1RPM]
      */
    void setPtoFront(const uint16_t aui16_val)
    {
      mt_ptoFront.ui16_pto8DigitPerRpm = ( aui16_val * 8 );

      if ( aui16_val == 0 ) setPtoFrontEngaged( IsoAgLib::IsoInactive );
      else                  setPtoFrontEngaged( IsoAgLib::IsoActive );
    }

    /** set rear PTO with unit [1/8RPM] so that the full resolution of ISOBUS messages can be used with integer arithmetic
    @param aui16_val8DigitPerRpm value to store as the speed of the rear PTO [1/8RPM]
     */
    void setPtoRear8DigitPerRpm(const uint16_t aui16_val8DigitPerRpm)
    {
      mt_ptoRear.ui16_pto8DigitPerRpm = aui16_val8DigitPerRpm;

      if ( aui16_val8DigitPerRpm == 0 ) setPtoRearEngaged( IsoAgLib::IsoInactive );
      else                              setPtoRearEngaged( IsoAgLib::IsoActive );
    }

    /** set front PTO with unit [1/8RPM] so that the full resolution of ISOBUS messages can be used with integer arithmetic
    @param aui16_val8DigitPerRpm value to store as the speed of the front PTO [1/8RPM]
     */
    void setPtoFront8DigitPerRpm(const uint16_t aui16_val8DigitPerRpm)
    {
      mt_ptoFront.ui16_pto8DigitPerRpm = aui16_val8DigitPerRpm;

      if ( aui16_val8DigitPerRpm == 0 ) setPtoFrontEngaged( IsoAgLib::IsoInactive );
      else                              setPtoFrontEngaged( IsoAgLib::IsoActive );
    }

    bool processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t );

    /** force a request for pgn for front pto state */
    bool sendRequestUpdateFront();

    /** force a request for pgn for rear pto state */
    bool sendRequestUpdateRear();

    /** send pto data message
        @param t_sendptodata  send pto front or pto rear
        @see  TracCert_c::processMsgRequestPGN
        @see  CanIo_c::operator<<
      */
    void sendMessage(SendPtoData_t t_sendptodata);

    /** set explicit information whether front PTO is engaged
      * @param at_val IsoActive -> PTO is engaged
      */
    void setPtoFrontEngaged(const IsoAgLib::IsoActiveFlag_t at_val) { mt_ptoFront.t_ptoEngaged = at_val; }

    /** set explicit information whether rear PTO is engaged
      * @param at_val IsoActive -> PTO is engaged
      */
    void setPtoRearEngaged(const IsoAgLib::IsoActiveFlag_t at_val) { mt_ptoRear.t_ptoEngaged = at_val;}

    /** set std RPM of front PTO (1000 n/min; 540 n/min)
      * @param at_val IsoActive == 1000 n/min; false == 540 n/min
      */
    void setPtoFront1000(const IsoAgLib::IsoActiveFlag_t at_val) { mt_ptoFront.t_pto1000 = at_val;}

    /** set std RPM of rear PTO (1000 n/min; 540 n/min)
      * @param at_val IsoActive == 1000 n/min; false == 540 n/min
      */
    void setPtoRear1000(const IsoAgLib::IsoActiveFlag_t at_val) { mt_ptoRear.t_pto1000 = at_val;}

    /** set economy mode of front PTO
      * @param at_val IsoActive -> front PTO is in economy mode
      */
    void setPtoFrontEconomy(const IsoAgLib::IsoActiveFlag_t at_val) { mt_ptoFront.t_ptoEconomy = at_val;}

    /** set economy mode of rear PTO
      * @param at_val IsoActive -> rear PTO is in economy mode
      */
    void setPtoRearEconomy(const IsoAgLib::IsoActiveFlag_t at_val) { mt_ptoRear.t_ptoEconomy = at_val;}

    /** set measured value of the set point of the rotational speed of the front power take-off (PTO) output shaft
        @param ai16_val requested RPM [1RPM]
      */
    void setFrontPtoSetPoint(const uint16_t ai16_val)  { mt_ptoFront.ui16_ptoSetPoint8DigitPerRpm = ( ai16_val * 8 );}

    /** set measured value of the set point of the rotational speed of the rear power take-off (PTO) output shaft
        @param ai16_val requested RPM [1RPM]
      */
    void setRearPtoSetPoint(const uint16_t ai16_val)  { mt_ptoRear.ui16_ptoSetPoint8DigitPerRpm = ( ai16_val * 8 );}

    /** set measured value of the set point of the rotational speed of the front power take-off (PTO) output shaft
        @param ai16_val8DigitPerRpm requested RPM [1/8RPM]
     */
    void setFrontPtoSetPoint8DigitPerRpm(const uint16_t ai16_val8DigitPerRpm)
    { mt_ptoFront.ui16_ptoSetPoint8DigitPerRpm = ai16_val8DigitPerRpm;}

    /** set measured value of the set point of the rotational speed of the rear power take-off (PTO) output shaft
        @param ai16_val8DigitPerRpm requested RPM [1/8RPM]
     */
    void setRearPtoSetPoint8DigitPerRpm(const uint16_t ai16_val8DigitPerRpm)
    { mt_ptoRear.ui16_ptoSetPoint8DigitPerRpm = ai16_val8DigitPerRpm;}

    /** set reported tractor ECU's status of front engagement
        @param at_val  status to set
      */
    void setFrontPtoEngagementReqStatus(IsoAgLib::IsoReqFlag_t at_val) {mt_ptoFront.t_ptoEngagementReqStatus = at_val;}

    /** set reported tractor ECU's status of rear engagement
        @param at_val  status to set
      */
    void setRearPtoEngagementReqStatus(IsoAgLib::IsoReqFlag_t at_val) {mt_ptoRear.t_ptoEngagementReqStatus = at_val;}

    /** set reported tractor ECU's status of front mode
        @param at_val  status to set
      */
    void setFrontPtoModeReqStatus(IsoAgLib::IsoReqFlag_t at_val) {mt_ptoFront.t_ptoModeReqStatus = at_val;}

    /** set reported tractor ECU's status of rear mode
        @param at_val  status to set
      */
    void setRearPtoModeReqStatus(IsoAgLib::IsoReqFlag_t at_val) {mt_ptoRear.t_ptoModeReqStatus = at_val;}

    /** set reported tractor ECU's status of front economy mode
        @param at_val  status to set
      */
    void setFrontPtoEconomyModeReqStatus(IsoAgLib::IsoReqFlag_t at_val) {mt_ptoFront.t_ptoEconomyModeReqStatus = at_val;}

    /** set reported tractor ECU's status of rear economy mode
        @param at_val  status to set
      */
    void setRearPtoEconomyModeReqStatus(IsoAgLib::IsoReqFlag_t at_val) {mt_ptoRear.t_ptoEconomyModeReqStatus = at_val;}

    /** set present limit status of front pto shaft speed
        @param at_val  status to set
      */
    void setFrontPtoShaftSpeedLimitStatus(IsoAgLib::IsoLimitFlag_t at_val) {mt_ptoFront.t_ptoShaftSpeedLimitStatus = at_val;}

    /** set present limit status of rear pto shaft speed
        @param at_val  status to set
      */
    void setRearPtoShaftSpeedLimitStatus(IsoAgLib::IsoLimitFlag_t at_val) {mt_ptoRear.t_ptoShaftSpeedLimitStatus = at_val;}
    /*@}*/

    /* ****************************************************** */
    /** \name Retrieve Values which are sent from other ECUs  */
    /*@{*/

    /** get rear pto
        @return actual rpm speed of rear PTO [1RPM]
      */
    uint16_t ptoRear() const { return ( mt_ptoRear.ui16_pto8DigitPerRpm / 8 );}

    /** get front pto
        @return actual rpm speed of front PTO [1RPM]
      */
    uint16_t ptoFront() const { return ( mt_ptoFront.ui16_pto8DigitPerRpm / 8 );}

    /** get rear pto
      @return actual rpm speed of rear PTO [1/8RPM]
     */
    uint16_t ptoRear8DigitPerRpm() const { return ( mt_ptoRear.ui16_pto8DigitPerRpm);}

    /** get front pto
      @return actual rpm speed of front PTO [1/8RPM]
     */
    uint16_t ptoFront8DigitPerRpm() const { return ( mt_ptoFront.ui16_pto8DigitPerRpm);}

    /** deliver explicit information whether front PTO is engaged
      * @return IsoActive -> PTO is engaged
      */
    IsoAgLib::IsoActiveFlag_t ptoFrontEngaged() const { return static_cast<IsoAgLib::IsoActiveFlag_t>(mt_ptoFront.t_ptoEngaged);}

    /** deliver explicit information whether rear PTO is engaged
      * @return IsoActive -> PTO is engaged
      */
    IsoAgLib::IsoActiveFlag_t ptoRearEngaged() const { return static_cast<IsoAgLib::IsoActiveFlag_t>(mt_ptoRear.t_ptoEngaged);}

    /** deliver std RPM of front PTO (1000 n/min; 540 n/min)
      * @return IsoActive == 1000 n/min; false == 540 n/min
      */
    IsoAgLib::IsoActiveFlag_t ptoFront1000() const { return static_cast<IsoAgLib::IsoActiveFlag_t>(mt_ptoFront.t_pto1000);}

    /** deliver std RPM of rear PTO (1000 n/min; 540 n/min)
      * @return IsoActive == 1000 n/min; false == 540 n/min
      */
    IsoAgLib::IsoActiveFlag_t ptoRear1000() const { return static_cast<IsoAgLib::IsoActiveFlag_t>(mt_ptoRear.t_pto1000);}

    /** deliver economy mode of front PTO
      * @return IsoActive -> front PTO is in economy mode
      */
    IsoAgLib::IsoActiveFlag_t ptoFrontEconomy() const { return static_cast<IsoAgLib::IsoActiveFlag_t>(mt_ptoFront.t_ptoEconomy);}

    /** deliver economy mode of rear PTO
      * @return IsoActive -> rear PTO is in economy mode
      */
    IsoAgLib::IsoActiveFlag_t ptoRearEconomy() const { return static_cast<IsoAgLib::IsoActiveFlag_t>(mt_ptoRear.t_ptoEconomy);}

    /** get measured value of the set point of the rotational speed of the front power take-off (PTO) output shaft
        @return measured value of the set point [1RPM]
      */
    uint16_t frontPtoSetPoint() const { return ( mt_ptoFront.ui16_ptoSetPoint8DigitPerRpm / 8 );}

    /** get measured value of the set point of the rotational speed of the rear power take-off (PTO) output shaft
        @return measured value of the set point [1RPM]
      */
    uint16_t rearPtoSetPoint() const { return ( mt_ptoRear.ui16_ptoSetPoint8DigitPerRpm / 8 );}

    /** get measured value of the set point of the rotational speed of the front power take-off (PTO) output shaft
        @return measured value of the set point [1/8RPM]
     */
    uint16_t frontPtoSetPoint8DigitPerRpm() const { return mt_ptoFront.ui16_ptoSetPoint8DigitPerRpm;}

    /** get measured value of the set point of the rotational speed of the rear power take-off (PTO) output shaft
        @return measured value of the set point [1/8RPM]
     */
    uint16_t rearPtoSetPoint8DigitPerRpm() const { return mt_ptoRear.ui16_ptoSetPoint8DigitPerRpm;}

    /** get reported tractor ECU's status of front engagement
        @return  reported status
      */
    IsoAgLib::IsoReqFlag_t frontPtoEngagementReqStatus() const {return static_cast<IsoAgLib::IsoReqFlag_t>(mt_ptoFront.t_ptoEngagementReqStatus);}

    /** get reported tractor ECU's status of rear engagement
        @return  reported status
      */
    IsoAgLib::IsoReqFlag_t rearPtoEngagementReqStatus() const {return static_cast<IsoAgLib::IsoReqFlag_t>(mt_ptoRear.t_ptoEngagementReqStatus);}

    /** get reported tractor ECU's status of front mode
         @return  reported status
      */
    IsoAgLib::IsoReqFlag_t frontPtoModeReqStatus() const {return static_cast<IsoAgLib::IsoReqFlag_t>(mt_ptoFront.t_ptoModeReqStatus);}

    /** get reported tractor ECU's status of rear mode
        @return  reported status
      */
    IsoAgLib::IsoReqFlag_t rearPtoModeReqStatus() const {return static_cast<IsoAgLib::IsoReqFlag_t>(mt_ptoRear.t_ptoModeReqStatus);}

    /** get reported tractor ECU's status of front economy mode
        @return  reported status
      */
    IsoAgLib::IsoReqFlag_t frontPtoEconomyModeReqStatus() const {return static_cast<IsoAgLib::IsoReqFlag_t>(mt_ptoFront.t_ptoEconomyModeReqStatus);}

    /** get reported tractor ECU's status of rear economy mode
        @return  reported status
      */
    IsoAgLib::IsoReqFlag_t rearPtoEconomyModeReqStatus()const {return static_cast<IsoAgLib::IsoReqFlag_t>(mt_ptoRear.t_ptoEconomyModeReqStatus);}

    /** get present limit status of front pto shaft speed
        @return  present limit status
      */
    IsoAgLib::IsoLimitFlag_t frontPtoShaftSpeedLimitStatus() const {return static_cast<IsoAgLib::IsoLimitFlag_t>(mt_ptoFront.t_ptoShaftSpeedLimitStatus);}

    /** get present limit status of rear pto shaft speed
        @return  present limit status
      */
    IsoAgLib::IsoLimitFlag_t rearPtoShaftSpeedLimitStatus() const {return static_cast<IsoAgLib::IsoLimitFlag_t>(mt_ptoRear.t_ptoShaftSpeedLimitStatus);}

  private:
    // Private methods
    /**
      HIDDEN constructor for a TracPTO_c object instance which can optional
      set the configuration for send/receive for base msg type 1,2
      and calendar
      NEVER instantiate a variable of type TracPTO_c within application
      only access TracPTO_c via getTracPTOInstance() or getTracPTOInstance( int riLbsBusNr ) in case more than one BUS is used for IsoAgLib
    */
    TracPTO_c() {}

    /** check if filter boxes shall be created - create only filters based
        on active local idents which has already claimed an address
        --> avoid to much Filter Boxes
      */
    virtual void checkCreateReceiveFilter();

    /** send a ISO11783 base information PGN.
      * this is only called when sending ident is configured and it has already claimed an address
        @pre  function is only called in tractor mode
        @see  BaseCommon_c::timeEvent()
      */
    virtual void timeEventTracMode( );

    /** Detect stop of PTO update from tractor -> indication for stopped PTO */
    virtual void timeEventImplMode();

    /** process a ISO11783 base information PGN
        @pre  sender of message is existent in monitor list
        @see  CanPkgExt_c::resolveSendingInformation()
      */
    virtual void processMsg( const CanPkg_c& arc_data );

  private:
    // Private attributes
    /** pto front information */
    PtoData_t mt_ptoFront;

    /** pto rear information */
    PtoData_t mt_ptoRear;
    friend TracPTO_c &getTracPtoInstance(uint8_t aui8_instance);
  };

  /** C-style function, to get access to the unique TracPTO_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  TracPTO_c &getTracPtoInstance(uint8_t aui8_instance = 0);
}
#endif
