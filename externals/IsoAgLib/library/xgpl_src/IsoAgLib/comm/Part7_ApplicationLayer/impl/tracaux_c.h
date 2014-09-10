/*
  tracaux_c.h: working on Base Data Msg Type 1; stores, updates and
    delivers all base data informations from CanCustomer_c derived for
    CAN sending and receiving interaction; from SchedulerTask_c
    derived for interaction with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TRACAUX_C_H
#define TRACAUX_C_H

#include "basecommon_c.h"


// Begin Namespace __IsoAgLib
namespace __IsoAgLib {

  typedef struct
  {
    /** measured nominal pressure at the extend port of auxiliary marr_valve of a tractor */
    uint16_t ui16_extendPortPressure : 16;
    /** measured nominal pressure at the retract port of auxiliary marr_valve of a tractor */
    uint16_t ui16_retractPortPressure : 16;
    /** value reported by the controller of flow through the extended port of auxiliary marr_valve of a tractor
        which could be based on the commandes of the marr_valve */
    uint8_t ui8_extendPortEstFlow : 8;
    /** value reported by the controller of flow through the retract port of auxiliary marr_valve of a tractor
        which could be based on the commandes of the marr_valve */
    uint8_t ui8_retractPortEstFlow : 8;
    /** measured flow through the extend port auxiliary marr_valve of a tractor, expressed as a percentage of full flow */
    uint8_t ui8_extendPortMeasuredFlow : 8;
    /** measured flow through the retract port auxiliary marr_valve of a tractor, expressed as a percentage of full flow */
    uint8_t ui8_retractPortMeasuredFlow : 8;
    /** measured nominal pressure at the return port of auxiliary marr_valve of a tractor */
    uint8_t ui8_returnPortPressure : 8;
    /** command for setting the flow through the extend or retract port of auxiliary marr_valve of a tractor, expressed as a
        percentage of full flow */
    uint8_t ui8_cmdPortFlow : 8;
    /** command for setting the auxiliary marr_valve state */
    uint8_t ui8_cmdValveState : 4;
    /** measured state of the auxiliary marr_valve */
    uint8_t ui8_estValveState : 4;
    /** estimated present limit status of auxiliary marr_valve */
    uint8_t ui8_estValveLimitStatus : 3;
    /** measured present limit status of auxiliary marr_valve */
    uint8_t ui8_measuredValveLimitStatus : 3;
    /** measured state of the fail save mode of auxiliary marr_valve */
    uint8_t ui8_estFailSaveMode : 2;
    /** command for setting the fail save mode of auxiliary marr_valve */
    uint8_t ui8_cmdFailSaveMode : 2;

  } IsoAuxValveData_t;

  typedef struct
  {
    /** flag which shows if estimated part is actually used (0: not used, 1: used) */
    uint8_t ui8_estimatedActivated : 1;
    /** flag which shows if measured part is actually used (0: not used, 1: used) */
    uint8_t ui8_measuredActivated : 1;
    /** flag which shows if command part is actually used (0: not used, 1: used) */
    uint8_t ui8_commandActivated : 1;
  } IsoAuxActivated_t;

  //constants from ISO11783 Chapter 7 Annex A
  enum
  {
    //A.21.1 number of valves
    nrOfValves = 16,
    //A.21.2 - A.21.5 offset
    a21_2to5_Offset = 125,
    //A.21.2 - A.21.5 minimum range
    a21_2to5_MinRange = -125,
    //A.21.2 - A.21.5 maximum range
    a21_2to5_MaxRange = 125,
    //A.21.7 - A.21.8 resolution
    a21_7to8_Resolution = 5,
    //A.21.7 - A.21.8 maximum range
    a21_7to8_MaxRange = 321275,
    //A.21.9 resolution
    a21_9_Resolution = 16,
    //A.21.9 maximum range
    a21_9_MaxRange = 4000,
    //A.21.10 maximum range
    a21_10_MaxRange = 100
  };

  /** stores, updates  and delivers all auxiliary data information;
      Derive from BaseCommon_c some fundamental funktionality for all base data
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */

  class TracAux_c : public BaseCommon_c {
    MACRO_MULTITON_CONTRIBUTION();
  public:// Public methods
    /* ********************************************* */
    /** \name Management Functions for class TracAux_c  */
    /*@{*/

    /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
        @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
        @return true -> configuration was successfull
      */
    virtual bool config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask = 0 );

    virtual ~TracAux_c() {}

    /*@}*/

    /* ******************************************* */
    /** \name Set Values for periodic send on BUS  */
    /*@{*/
    /** set value reported by the controller of flow through the extended port of auxiliary marr_valve of a tractor
        which could be based on the commandes of the marr_valve
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setExtendPortEstFlow(const uint8_t valveNumber, const int8_t t_val);

    /** set value reported by the controller of flow through the retract port of auxiliary marr_valve of a tractor
        which could be based on the commandes of the marr_valve
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setRetractPortEstFlow(const uint8_t valveNumber, const int8_t t_val);

    /** set measured state of the fail save mode of auxiliary marr_valve
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setEstFailSaveMode(const uint8_t valveNumber, const IsoAgLib::IsoAuxFlag_t t_val);

    /** set measured state of the auxiliary marr_valve
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setEstValveState(const uint8_t valveNumber, const IsoAgLib::IsoAuxFlagExtended_t t_val);

    /** set estimated present limit status of auxiliary marr_valve
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param rt_val       limit status to store
        @return             true -> data was set; false -> data range error
      */
    bool setEstValveLimitStatus(const uint8_t valveNumber, const IsoAgLib::IsoLimitFlag_t rt_val);

    /** set measured flow through the extend port auxiliary marr_valve of a tractor, expressed as a percentage of full flow
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setExtendPortMeasuredFlow(const uint8_t valveNumber, const int8_t t_val);

    /** set measured flow through the retract port auxiliary marr_valve of a tractor, expressed as a percentage of full flow
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setRetractPortMeasuredFlow(const uint8_t valveNumber, const int8_t t_val);

    /** set measured nominal pressure at the extend port of auxiliary marr_valve of a tractor
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setExtendPortPressure(const uint8_t valveNumber, const uint32_t t_val);

    /** set measured nominal pressure at the retract port of auxiliary marr_valve of a tractor
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setRetractPortPressure(const uint8_t valveNumber, const uint32_t t_val);

    /** set measured nominal pressure at the return port of auxiliary marr_valve of a tractor
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setReturnPortPressure(const uint8_t valveNumber, const uint16_t t_val);

    /** set measured present limit status of auxiliary marr_valve
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param rt_val       limit status to store
        @return             true -> data was set; false -> data range error
      */
    bool setMeasuredValveLimitStatus(const uint8_t valveNumber, const IsoAgLib::IsoLimitFlag_t rt_val);

    /** set command for setting the flow through the extend or retract port of auxiliary marr_valve or a tractor, expressed as a
        percentage of full flow
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setCmdPortFlow(const uint8_t valveNumber, const uint8_t t_val);

    /** set command for setting the fail save mode of auxiliary marr_valve
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setCmdFailSaveMode(const uint8_t valveNumber, const IsoAgLib::IsoAuxFlag_t t_val);

    /** set command for setting the auxiliary marr_valve state
        possible errors:
          * data range error
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @param t_val        value to store
        @return             true -> data was set; false -> data range error
      */
    bool setCmdValveState(const uint8_t valveNumber, const IsoAgLib::IsoAuxFlagExtended_t t_val);
    /*@{*/

    /* ****************************************************** */
    /** \name Retrieve Values which are sent from other ECUs  */
    /*@{*/
    /** get value reported by the controller of flow through the extended port of auxiliary marr_valve of a tractor
        which could be based on the commandes of the marr_valve
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
      */
    int8_t extendPortEstFlow(const uint8_t valveNumber) const
    {return (marr_valve[valveNumber].ui8_extendPortEstFlow - a21_2to5_Offset);}

    /** get value reported by the controller of flow through the retract port of auxiliary marr_valve of a tractor
        which could be based on the commandes of the marr_valve
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
      */
    int8_t retractPortEstFlow(const uint8_t valveNumber) const
    {return (marr_valve[valveNumber].ui8_retractPortEstFlow -a21_2to5_Offset);}

    /** get measured state of the fail save mode of auxiliary marr_valve
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
      */
    IsoAgLib::IsoAuxFlag_t estFailSaveMode(const uint8_t valveNumber) const
    {return IsoAgLib::IsoAuxFlag_t (marr_valve[valveNumber].ui8_estFailSaveMode);}

    /** get measured state of the auxiliary marr_valve
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
      */
    IsoAgLib::IsoAuxFlagExtended_t estValveState(const uint8_t valveNumber) const
    {return IsoAgLib::IsoAuxFlagExtended_t (marr_valve[valveNumber].ui8_estValveState);}

    /** get measured flow through the extend port auxiliary marr_valve of a tractor, expressed as a percentage of full flow
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
    */
    int8_t extendPortMeasuredFlow(const uint8_t valveNumber) const
    {return (marr_valve[valveNumber].ui8_extendPortMeasuredFlow - a21_2to5_Offset);}

    /** get measured flow through the retract port auxiliary marr_valve of a tractor, expressed as a percentage of full flow
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
      */
    int8_t retractPortMeasuredFlow(const uint8_t valveNumber) const
    {return (marr_valve[valveNumber].ui8_retractPortMeasuredFlow - a21_2to5_Offset);}

    /** get measured nominal pressure at the extend port of auxiliary marr_valve of a tractor
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
      */
    uint32_t extendPortPressure(const uint8_t valveNumber) const
    {return (marr_valve[valveNumber].ui16_extendPortPressure * a21_7to8_Resolution);}

    /** get measured nominal pressure at the retract port of auxiliary marr_valve of a tractor
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
      */
    uint32_t retractPortPressure(const uint8_t valveNumber) const
    {return (marr_valve[valveNumber].ui16_retractPortPressure * a21_7to8_Resolution);}

    /** get measured nominal pressure at the return port of auxiliary marr_valve of a tractor
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
      */
    uint16_t returnPortPressure(const uint8_t valveNumber) const
    {return (marr_valve[valveNumber].ui8_returnPortPressure * a21_9_Resolution);}

    /** get command for setting the flow through the extend or retract port of auxiliary marr_valve or a tractor, expressed as a
        percentage of full flow
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
      */
    uint8_t cmdPortFlow(const uint8_t valveNumber) const
    {return ( (marr_valve[valveNumber].ui8_cmdPortFlow * 2) / 5);}

    /** get command for setting the fail save mode of auxiliary marr_valve
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
      */
    IsoAgLib::IsoAuxFlag_t cmdFailSaveMode(const uint8_t valveNumber) const
    {return IsoAgLib::IsoAuxFlag_t (marr_valve[valveNumber].ui8_cmdFailSaveMode);}

    /** get command for setting the auxiliary marr_valve state
        @param valveNumber  auxiliary marr_valve number (range 0 to 15)
        @return             value of extended port of auxiliary marr_valve number valveNumber
      */
    IsoAgLib::IsoAuxFlagExtended_t cmdValveState(const uint8_t valveNumber) const
    {return IsoAgLib::IsoAuxFlagExtended_t (marr_valve[valveNumber].ui8_cmdValveState);}

    /** get estimated present limit status of auxiliary marr_valve
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @return             present limit status
      */
    IsoAgLib::IsoLimitFlag_t estValveLimitStatus(const uint8_t valveNumber) const
    {return IsoAgLib::IsoLimitFlag_t (marr_valve[valveNumber].ui8_estValveLimitStatus);}

    /** get measured present limit status of auxiliary marr_valve
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
        @return             present limit status
      */
    IsoAgLib::IsoLimitFlag_t measuredValveLimitStatus(const uint8_t valveNumber) const
    {return IsoAgLib::IsoLimitFlag_t (marr_valve[valveNumber].ui8_measuredValveLimitStatus);}
    /*@}*/

  virtual bool processMsgRequestPGN (uint32_t rui32_pgn, IsoItem_c* rpc_isoItemSender, IsoItem_c* rpc_isoItemReceiver, int32_t );

  private:
    // Private methods
    /** HIDDEN constructor for a TracAux_c object instance which can optional
        set the configuration for send/receive for base msg type 1
        NEVER instantiate a variable of type TracAux_c within application
        only access TracAux_c via getTracAuxInstance() or getTracAuxInstance( int riLbsBusNr ) in case more than one ISO11783 BUS is used for IsoAgLib
      */
    TracAux_c() {};

    /** check if filter boxes shall be created - create only ISO based
        on active local idents which has already claimed an address
        --> avoid to much Filter Boxes
      */
    virtual void checkCreateReceiveFilter();

    /** send a ISO11783 marr_valve information PGN in tractor mode
      * this is only called when sending ident is configured and it has already claimed an address
        @pre  function is only called in tractor mode
        @see  BaseCommon_c::timeEvent()
      */
    virtual void timeEventTracMode( );

    /** send a ISO11783 marr_valve information PGN in implement mode
      * this is only called when sending ident is configured and it has already claimed an address
        @pre  function is only called in implement mode
        @see  BaseCommon_c::timeEvent()
      */
    virtual void timeEventImplMode( );

    /** process a ISO11783 base information PGN
        @pre  sender of message is existent in monitor list
        @see  CanPkgExt_c::resolveSendingInformation()
      */
    virtual void processMsg( const CanPkg_c& arc_data );

    /** send estimated and measured messages (only tractor mode)
        @pre client has already claimed an address
        @see CanIo_c::operator<<
      */
    void isoSendEstimatedMeasured();

    enum SendCommand_e { CommandSent, CommandNotSent };

    /** send command messages (only tractor mode)
        @pre client has already claimed an address
        @see CanIo_c::operator<<
      */
    void isoSendCommand();

    /** send one command message (only tractor mode)
        @pre client has already claimed an address
        @see CanIo_c::operator<<
      */
    SendCommand_e isoSendCommand(uint32_t aui32_pgn);

    /** set flag for the command part
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
      */
    void activateCmd(const uint8_t valveNumber) {marr_auxFlag[valveNumber].ui8_commandActivated = true;}

    /** set flag for the measured part
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
      */
    void activateMeasured(const uint8_t valveNumber) {marr_auxFlag[valveNumber].ui8_measuredActivated = true;}

    /** set flag for the estimated part
        @param valveNumber  auxiliary marr_valve number (range from 0 to 15)
      */
    void activateEstimated(const uint8_t valveNumber) {marr_auxFlag[valveNumber].ui8_estimatedActivated = true;}

    /** Send one estimated flow.
     */
    void isoSendEstimated(uint32_t aui32_pgn);

    /** Send one measured flow.
     */
    void isoSendMeasured(uint32_t aui32_pgn);

    /** Prepare sending estimated or measured flow.
     */
    void prepareSendingEstimatedMeasured( CanPkgExt_c& pkg );

  private:
    /** auxiliary valves are numbered beginning with 0 and increase in sequence to the maximum number of auxiliary
        hydraulic valves in the device (maximum 16). All auxiliary valves have the same data. In systems using
        "power beyond" technology, marr_valve number 0 shall be used to identify the power beyond control marr_valve.
      */
    IsoAuxValveData_t marr_valve[nrOfValves];

    /** stores which data (estimated, measured and command) of which marr_valve is actually used */
    IsoAuxActivated_t marr_auxFlag[nrOfValves];

    /** IsoName_c for implement sender */
    IsoName_c mc_sendImplISOName;

    /** last time of command msg [msec] */
    uint32_t mi32_lastMsgReceivedCmd;
    friend TracAux_c &getTracAuxInstance(uint8_t rui8_instance);
  };

  /** C-style function, to get access to the unique TracAux_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an m_index must be given to select the wanted BUS
    */
  TracAux_c &getTracAuxInstance(uint8_t rui8_instance = 0);
}
#endif
