/*
  traclight_c.h: working on lighting data; stores, updates and
    delivers all base data informations from CanCustomer_c derived for
    CAN sending and receiving interaction; from BaseCommon_c derived
    for interaction with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TRACLIGHT_C_H
#define TRACLIGHT_C_H

#include "basecommon_c.h"
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgnhandler_c.h>

#include <map>


namespace __IsoAgLib { // Begin Namespace __IsoAgLib

/** bit data for the parameter group number lighting command and lighting data*/
typedef struct {
//Byte 1
  /** daytime running lights state */
  unsigned int daytimeRunning : 2;
  /** alternate head lights state */
  unsigned int alternateHead : 2;
  /** low-beam head lights state */
  unsigned int lowBeamHead : 2;
  /** high-beam head lights state */
  unsigned int highBeamHead : 2;
//Byte 2
  /** front fog lights state */
  unsigned int frontFog : 2;
  /** beacon lights state */
  unsigned int beacon : 2;
  /** right-turn lights state */
  unsigned int rightTurn : 2;
  /** left-turn lights state */
  unsigned int leftTurn : 2;
//Byte 3
  /** back up light and alarm horn */
  unsigned int backUpLightAlarmHorn : 2;
  /** center stop lights state */
  unsigned int centerStop : 2;
  /** right-stop lights state */
  unsigned int rightStop : 2;
  /** left-stop lights state */
  unsigned int leftStop : 2;
//Byte 4
  /** implement clearance lights state */
  unsigned int implClearance : 2;
  /** tractor clearance lights state */
  unsigned int tracClearance : 2;
  /** implement marker lights state */
  unsigned int implMarker : 2;
  /** tractor marker lights state */
  unsigned int tracMarker : 2;
//Byte 5
  /** rear fog lights state */
  unsigned int rearFog : 2;
  /** tractor underside work lights state */
  unsigned int undersideWork : 2;
  /** tractor rear low mounted work lights state */
  unsigned int rearLowWork : 2;
  /** tractor rear high mounted work lights state */
  unsigned int rearHighWork : 2;
//Byte 6
  /** tractor side low mounted work lights state */
  unsigned int sideLowWork : 2;
  /** tractor side high mounted work lights state */
  unsigned int sideHighWork : 2;
  /** tractor front low mounted work lights state */
  unsigned int frontLowWork : 2;
  /** tractor front high mounted work lights state */
  unsigned int frontHighWork : 2;
//Byte 7
  /** implement OEM option 2 light state */
  unsigned int implOEMOpt2 : 2;
  /** implement OEM option 1 light state */
  unsigned int implOEMOpt1 : 2;
  /** implement right forward work lights state */
  unsigned int implRightForwardWork : 2;
  /** implement left forward work lights state */
  unsigned int implLeftForwardWork : 2;
//Byte 8
  /** lighting data message request state */
  unsigned int dataMsgReq : 2;    //reserved in Lighting data
  /** implement right-facing work lights state */
  unsigned int implRightFacingWork : 2;
  /** implement left-facing work lights state */
  unsigned int implLeftFacingWork : 2;
  /** implement rear work lights state */
  unsigned int implRearWork : 2;

  } lightBitData_t;

  /** stores, updates  and delivers lighting data information;
      Derive from BaseCommon_c some fundamental funktionality for all base data
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */
  class TracLight_c : public BaseCommon_c
  {
    MACRO_MULTITON_CONTRIBUTION();
  public: // Public methods

    // from BaseCommon_c
    virtual void init_specialized();
    virtual void close_specialized();

    /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
        @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
        @return true -> configuration was successfull
    */
    virtual bool config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask = 0 );

    bool processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t );

    virtual ~TracLight_c() {}

    /** set a lighting command in tractor mode or set a response on a lighting command in implement mode
        @param t_command  command to set
        @param t_val      command value
      */
    void setCommand(const IsoAgLib::IsoCommandFlag_t t_command, const IsoAgLib::IsoActiveFlag_t t_val);

    /** get the number of responses from implements to a command from the tractor
        @return number of responding implements to a command
      */
    int getCount() const {return mmap_data.size(); }

    /** get the status of a light of a special implement
        possible errors:
          * iLibErr_c::Range m_index exceeds map size
        @param acui_index    acui_index of implement
        @param at_command  which light
        @return           status of the light of the implement
      */
    IsoAgLib::IsoActiveFlag_t getByIndex(const unsigned int acui_index, const IsoAgLib::IsoCommandFlag_t at_command);

    /** get the status of a light of a special implement
        possible errors:
          * iLibErr_c::Range the implement with sa has not responded to command from the tractor
        @param sa         source address of implement
        @param t_command  which light
        @return           status of the light of the implement
      */
    IsoAgLib::IsoActiveFlag_t getBySa(const uint8_t sa, const IsoAgLib::IsoCommandFlag_t t_command) const;

    /** get command which the tractor send
        @param t_command  command from the tractor
        @return           value of the command which the tractor send
      */
    IsoAgLib::IsoActiveFlag_t getCommand(const IsoAgLib::IsoCommandFlag_t t_command) const;


    /** find out if a particular implement has responded to a command from the tractor
        @param sa   source address of the sought implement
        @return     has responded to a command (true), else (false)
      */
    bool find(const uint8_t sa) const;

  private:
    // Private methods
    /** HIDDEN constructor for a TracLight_c object instance which can optional
        set the configuration for send/receive for a lighting msg type
        NEVER instantiate a variable of type TracLight_c within application
        only access TracLight_c via getTracLightInstance() or getTracLightInstance( int riLbsBusNr ) in case more than one ISO11783 BUS is used for IsoAgLib
      */
    TracLight_c();

    /** check if filter boxes shall be created - create only ISO filters based
        on active local idents which has already claimed an address
        --> avoid to much Filter Boxes
      */
    virtual void checkCreateReceiveFilter( );

    /** helper funktion to get the mode of a light of a specified implement
        @param at_data       bit field where the lighting information is stored
        @param at_command  light to find
        @return           status of the light
      */
    IsoAgLib::IsoActiveFlag_t getInfo(const lightBitData_t& at_data, const IsoAgLib::IsoCommandFlag_t at_command) const;

    /** functions with actions, which must be performed periodically
        -> called periodically by Scheduler_c
        ==> sends msg if configured in the needed rates
        possible errors:
          * dependant error in CanIo_c on CAN send problems
        @pre  function is only called in tractor mode
        @see  BaseCommon_c::timeEvent()
      */
    virtual void timeEventTracMode();

    /** functions with actions, which must be performed periodically
        -> called periodically by Scheduler_c
        ==> sends msg if configured in the needed rates
        possible errors:
          * dependant error in CanIo_c on CAN send problems
        @pre  function is only called in implement mode
        @see  BaseCommon_c::timeEvent()
      */
    virtual void timeEventImplMode();

    virtual void processMsg( const CanPkg_c& arc_data );

    /** send light update; there is a difference between implement and tractor mode
        @see  TracLight_c::processMsgRequestPGN
        @see  CanIo_c::operator<<
      */
    void sendMessage();

    enum SendMessage_e { MessageSent, MessageNotSent };

    /** Help method sendMessage.
     */
    SendMessage_e helpSendMessage( CanPkgExt_c& pkg );

    /** Send lighting data message.
     */
    SendMessage_e sendLightingData();

    /** Send lighting command message.
     */
    SendMessage_e sendLightingCommand();

  private:
    // Private attributes
    /** remember if we received a command, which have to be responded */
    bool mb_cmdWait4Response;

    /** send lighting command if change of lamp between OF/ON state */
    bool mb_changeNeedBeSend;


    /// General
    /** bit field with all lighting information for tractor*/
    lightBitData_t mt_cmd;

    /** stores for each requesting implement (sourceAddress) a bit field with all lighting information*/
    STL_NAMESPACE::map<uint8_t, lightBitData_t> mmap_data; // access mmap_data[sourceAdr].

    /** the last 10 times when a command was send.*/
    int32_t marr_timeStamp[10];

    /** m_index which stores the position where the eldest marr_timeStamp is stored*/
    unsigned int m_index;
    friend TracLight_c &getTracLightInstance( uint8_t aui8_instance );
  };

  /** C-style function, to get access to the unique TracLight_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an m_index must be given to select the wanted BUS
    */
  TracLight_c &getTracLightInstance( uint8_t aui8_instance = 0 );
}
#endif
