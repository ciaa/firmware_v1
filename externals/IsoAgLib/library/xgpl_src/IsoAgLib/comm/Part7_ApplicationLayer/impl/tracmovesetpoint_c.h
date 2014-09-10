/*
  tracmovesetpoint_c.h: working on Base Data moving set point; stores,
    updates and delivers all base data informations from CanCustomer_c
    derived for CAN sending and receiving interaction; from
    SchedulerTask_c derived for interaction with other IsoAgLib
    objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TRACMOVESETPOINT_C_H
#define TRACMOVESETPOINT_C_H

#include "basecommon_c.h"


// Begin Namespace __IsoAgLib
namespace __IsoAgLib {
 
 /** working on Base Data Msg;
      stores, updates  and delivers all base data informations;
      Derive from BaseCommon_c some fundamental funktionality for all base data
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */

  class TracMoveSetPoint_c : public BaseCommon_c {
    MACRO_MULTITON_CONTRIBUTION();
  public:// Public methods
    /* ********************************************* */

    /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
        @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
        @return true -> configuration was successfull
      */
    virtual bool config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask = 0 );

    virtual ~TracMoveSetPoint_c() {}
    /*@}*/

    /** set commanded direction of the machine
        @param t_val  commanded direction of travel
      */
    void setSelectedDirectionCmd(const IsoAgLib::IsoDirectionFlag_t t_val) {mt_selectedDirectionCmd = t_val;}

    /** set commanded set point value of the machine speed as measured by the selected source
        @param ui16_val  set point value of the machine speed
      */
    void setSelectedSpeedSetPointCmd(const uint16_t ui16_val) {mi16_selectedSpeedSetPointCmd = ui16_val;}

    /** communicate maximum allowed speed to the tractor
        @param ui16_val  maximum allowed speed
      */
    void setSelectedSpeedSetPointLimit(const uint16_t ui16_val) {mi16_selectedSpeedSetPointLimit = ui16_val;}

    /** get commanded direction of the machine
        @return  commanded direction of travel
      */
    IsoAgLib::IsoDirectionFlag_t selectedDirectionCmd() const {return mt_selectedDirectionCmd;}

    /** get commanded set point value of the machine speed as measured by the selected source
        @return  set point value of the machine speed
      */
    uint16_t selectedSpeedSetPointCmd() const {return mi16_selectedSpeedSetPointCmd;}

    /** get communicated maximum allowed speed to the tractor
        @return  maximum allowed speed
      */
    uint16_t selectedSpeedSetPointLimit() const {return mi16_selectedSpeedSetPointLimit;}

  virtual bool processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t );

  private:
    // Private methods
    /** HIDDEN constructor for a TracMoveSetPoint_c object instance which can optional
        set the configuration for send/receive for base msg type 1
        NEVER instantiate a variable of type TracMoveSetPoint_c within application
        only access TracMoveSetPoint_c via getTracMoveSetPointInstance() or getTracMoveSetPointInstance( int riLbsBusNr ) in case more than one ISO11783 BUS is used for IsoAgLib
      */
    TracMoveSetPoint_c() {};

    /** check if filter boxes shall be created - create only filters based
        on active local idents which has already claimed an address
        --> avoid to much Filter Boxes
      */
    virtual void checkCreateReceiveFilter( );

    /** send a ISO11783 base information PGN.
      * this is only called when sending ident is configured and it has already claimed an address
        @pre  function is only called in implement mode
        @see  BaseCommon_c::timeEvent()
        @see  CanIo_c::operator<<
      */
    virtual void timeEventImplMode( );

    /** process a ISO11783 base information PGN
        @pre  sender of message is existent in monitor list
        @see  CanPkgExt_c::resolveSendingInformation()
      */
    virtual void processMsg( const CanPkg_c& arc_data );

  private:
    // private attributes

    /** commanded set point value of the machine speed as measured by the selected source */
    uint16_t mi16_selectedSpeedSetPointCmd;

    /** parameter is used by a machine to communicate it's maximum allowed speed to the tractor */
    uint16_t mi16_selectedSpeedSetPointLimit;

    /** commanded direction of the machine */
    IsoAgLib::IsoDirectionFlag_t mt_selectedDirectionCmd;
    friend TracMoveSetPoint_c &getTracMoveSetPointInstance(uint8_t aui8_instance);
  };

  /** C-style function, to get access to the unique TracMoveSetPoint_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an m_index must be given to select the wanted BUS
    */
  TracMoveSetPoint_c &getTracMoveSetPointInstance(uint8_t aui8_instance = 0);
}
#endif
