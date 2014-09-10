/*
  tracptosetpoint_c.h: working on pto set point data; stores, updates
    and delivers all pto set point data informations from
    CanCustomer_c derived for CAN sending and receiving interaction;
    from SchedulerTask_c derived for interaction with other IsoAgLib
    objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TRACPTOSETPOINT_C_H
#define TRACPTOSETPOINT_C_H

#include <IsoAgLib/comm/Part7_ApplicationLayer/impl/basecommon_c.h>


namespace __IsoAgLib { // Begin Namespace __IsoAgLib

  /** working on pto set point data Type;
      stores, updates and delivers all base data informations;
      Derive from BaseCommon_c some fundamental funktionality for all base data
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */
  class TracPTOSetPoint_c : public BaseCommon_c {
    MACRO_MULTITON_CONTRIBUTION();
  public:// Public methods
    /* ********************************************* */
    /** \name Management Functions for class TracPTO_c  */
    /*@{*/

    /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
        @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
        @return true -> configuration was successfull
      */
    virtual bool config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask = 0 );

    virtual ~TracPTOSetPoint_c() {}
    /*@}*/

    /* ******************************************* */
    /** \name Set Values for periodic send on BUS  */
    /*@{*/

    /** allowing the position of the front three-point hitch to be set
        @param ui8_val  command value
      */
    void setFrontHitchPosCmd(const uint8_t ui8_val) {mui8_frontHitchPosCmd = ( ( ui8_val * 5 ) / 2 );}

    /** allowing the position of the rear three-point hitch to be set
        @param ui8_val  command value
      */
    void setRearPtoSetPointCmd(const uint8_t ui8_val) {mui8_rearHitchPosCmd = ( ( ui8_val * 5 ) / 2 );}

    /** set rotational speed of the front power take-off (PTO) output shaft
        @param ui16_val  command value
      */
    void setFrontPtoSetPointCmd(const uint16_t ui16_val) {mui16_frontPtoSetPointCmd = ( ui16_val / 8 );}
    /** set rotational speed of the rear power take-off (PTO) output shaft
        @param ui16_val  command value
      */
    void setRearPtoSetPointCmd(const uint16_t ui16_val) {mui16_rearPtoSetPointCmd = ( ui16_val / 8 );}

    /** set front power take-off (PTO)
        @param t_val  IsoActive -> power take-off is engaged
      */
    void setFrontPtoEngagement(const IsoAgLib::IsoActiveFlag_t t_val) {mt_frontPtoEngagement = t_val;}

    /** set rear power take-off (PTO)
        @param t_val  IsoActive -> power take-off is engaged
      */
    void setRearPtoEngagement(const IsoAgLib::IsoActiveFlag_t t_val) {mt_rearPtoEngagement = t_val;}

    /** set mode of the front power take-off (PTO)
        @param t_val  IsoActive -> 1000 r/min; IsoInactive -> 540 r/min
      */
    void setFrontPtoMode(const IsoAgLib::IsoActiveFlag_t t_val) {mt_frontPtoMode = t_val;}

    /** set mode of the rear power take-off (PTO)
        @param t_val  IsoActive -> 1000 r/min; IsoInactive -> 540 r/min
      */
    void setRearPtoMode(const IsoAgLib::IsoActiveFlag_t t_val) {mt_rearPtoMode = t_val;}

    /** set front power take-off (PTO) economy mode
        @param t_val  IsoActive -> pto economy mode is engaged
      */
    void setFrontPtoEconomyMode(const IsoAgLib::IsoActiveFlag_t t_val) {mt_frontPtoEconomyMode = t_val;}

    /** set rear power take-off (PTO) economy mode
        @param t_val  IsoActive -> pto economy mode is engaged
      */
    void setRearPtoEconomyMode(const IsoAgLib::IsoActiveFlag_t t_val){mt_rearPtoEconomyMode = t_val;}
    /*@}*/

    /* ****************************************************** */
    /** \name Retrieve Values which are sent from other ECUs  */
    /*@{*/

    /** get position of the front three-point hitch
        @return  current position of the three-point hitch
      */
    uint8_t frontHitchPosCmd() const {return ( ( mui8_frontHitchPosCmd * 2 ) / 5 );}

    /** get position of the rear three-point hitch
        @return  current position of the three-point hitch
      */
    uint8_t rearHitchPosCmd() const {return ( ( mui8_rearHitchPosCmd * 2 ) / 5 );}

    /** get rotational speed of the front power take-off (PTO) output shaft
        @return current rotational speed of power take-off
      */
    uint16_t frontPtoSetPointCmd() const {return ( mui16_frontPtoSetPointCmd * 8 );}

    /** get rotational speed of the rear power take-off (PTO) output shaft
        @return  current rotational speed of power take-off
      */
    uint16_t rearPtoSetPointCmd() const {return ( mui16_rearPtoSetPointCmd * 8 );}

    /** get front power take-off (PTO)
        @return  IsoActive -> power take-off is engaged
      */
    IsoAgLib::IsoActiveFlag_t frontPtoEngagement() const {return mt_frontPtoEngagement;}

    /** get rear power take-off (PTO)
        @return  IsoActive -> power take-off is engaged
      */
    IsoAgLib::IsoActiveFlag_t rearPtoEngagement() const {return mt_rearPtoEngagement;}

    /** get mode of the front power take-off (PTO)
        @return  IsoActive -> 1000 r/min; IsoInactive -> 540 r/min
      */
    IsoAgLib::IsoActiveFlag_t frontPtoMode() const {return mt_frontPtoMode;}

    /** get mode of the rear power take-off (PTO)
        @return  IsoActive -> 1000 r/min; IsoInactive -> 540 r/min
      */
    IsoAgLib::IsoActiveFlag_t rearPtoMode() const {return mt_rearPtoMode;}

    /** get front power take-off (PTO) economy mode
        @return  IsoActive -> pto economy mode is engaged
      */
    IsoAgLib::IsoActiveFlag_t frontPtoEconomyMode() const {return mt_frontPtoEconomyMode;}

    /** get rear power take-off (PTO) economy mode
        @return  IsoActive -> pto economy mode is engaged
      */

    virtual bool processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t );

    IsoAgLib::IsoActiveFlag_t rearPtoEconomyMode() const {return mt_rearPtoEconomyMode;}
    /*@}*/

  private:
    // Private methods
    /** HIDDEN constructor for a TracPTOSetPoint_c object instance which can optional
        set the configuration for send/receive for base msg type
        NEVER instantiate a variable of type TracPTOSetPoint_c within application
        only access TracPTOSetPoint_c via getTracPtoSetPointInstance() or getTracPtoSetPointInstance( int riLbsBusNr ) in case more than one ISO11783 BUS is used for IsoAgLib
      */
    TracPTOSetPoint_c() {};

    /** check if filter boxes shall be created - create only ISO filters based
        on active local idents which has already claimed an address
        --> avoid to much Filter Boxes
      */
    virtual void checkCreateReceiveFilter();

    /** send a ISO11783 pto information PGN.
      * this is only called when sending ident is configured and it has already claimed an address
        @pre  function is only called in implement mode
        @see  BaseCommon_c::timeEvent()
      */
    virtual void timeEventImplMode();

    /** send Base2 Group data with hitch and PTO state */
    void isoSendBase2Update( void );

    virtual void processMsg( const CanPkg_c& arc_data );

    /** send hitch and pto command
        @see  CanIo_c::operator<<
      */
    void sendPtoMsg();

  private:
    // Private attributes

    /** general */
    /** position of the front three-point hitch*/
    uint8_t mui8_frontHitchPosCmd;

    /** position of the rear three-point hitch*/
    uint8_t mui8_rearHitchPosCmd;

    /** rotational speed of the front power take-off (PTO) output shaft*/
    uint16_t mui16_frontPtoSetPointCmd;

    /** rotational speed of the rear power take-off (PTO) output shaft*/
    uint16_t mui16_rearPtoSetPointCmd;

    /** tate of the front power take-off (PTO)*/
    IsoAgLib::IsoActiveFlag_t mt_frontPtoEngagement;

    /** state of the rear power take-off (PTO)*/
    IsoAgLib::IsoActiveFlag_t mt_rearPtoEngagement;

    /** mode of the front power take-off (PTO): IsoInactive == 1000 r/min; IsoActive == 540 r/min*/
    IsoAgLib::IsoActiveFlag_t mt_frontPtoMode;

    /** mode of the rear power take-off (PTO): IsoInactive == 1000 r/min; IsoActive == 540 r/min*/
    IsoAgLib::IsoActiveFlag_t mt_rearPtoMode;

    /** state of the front power take-off (PTO) economy mode*/
    IsoAgLib::IsoActiveFlag_t mt_frontPtoEconomyMode;

    /** state of the rear power take-off (PTO) economy mode*/
    IsoAgLib::IsoActiveFlag_t mt_rearPtoEconomyMode;

    friend TracPTOSetPoint_c &getTracPtoSetPointInstance(uint8_t aui8_instance);
  };

  /** C-style function, to get access to the unique TracPTOSetPoint_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an m_index must be given to select the wanted BUS
    */
  TracPTOSetPoint_c &getTracPtoSetPointInstance(uint8_t aui8_instance = 0);
}
#endif
