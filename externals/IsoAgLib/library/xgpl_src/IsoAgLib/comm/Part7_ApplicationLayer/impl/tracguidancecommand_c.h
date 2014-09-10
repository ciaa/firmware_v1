/*
  tracguidance_c.cpp: stores, updates and delivers all guidance
    command data information from CanCustomer_c derived for CAN
    sending and receiving interaction; from BaseCommon_c derived for
    interaction with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TRAC_GUIDANCE_COMMAND_C_H
#define TRAC_GUIDANCE_COMMAND_C_H

#include "basecommon_c.h"


namespace __IsoAgLib {

typedef struct
{
  /** desired course curvature over ground that a machine's steering system is required to achieve */
  uint16_t ui16_curvatureCmd;
  /** indicates whether the guidance system is attempting to control steering with this command */
  IsoAgLib::IsoSteerFlag_t t_curvatureCmdStatus;

} CurvatureCommander_s;


  /** stores, updates  and delivers all guidance command data information;
      Derive from BaseCommon_c some fundamental funktionality for all base data
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */
class TracGuidanceCommand_c : public BaseCommon_c
{
  MACRO_MULTITON_CONTRIBUTION();
public:

   /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
       @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
       @return true -> configuration was successfull
    */
  virtual bool config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask = 0);

  virtual ~TracGuidanceCommand_c() {}

  /* ******************************************* */
  /** \name Set Values for periodic send on BUS  */
  /*@{*/
  /** set desired course curvature over ground that a machine's steering system is required to achieve
      @param  i16_val  desired course curvature over ground
    */
  void setCurvatureCmd(int16_t i16_val) {mmap_commanders[mui8_commanderSa].ui16_curvatureCmd = ( i16_val + 8032 ) * 4;}

  /** set whether the guidance system is attempting to control steering with this command
      @param  t_val  is guidance system attempting to control steering
    */
  void setCurvatureCmdStatus(IsoAgLib::IsoSteerFlag_t t_val) {mmap_commanders[mui8_commanderSa].t_curvatureCmdStatus = t_val;}

  /** get currently selected curvature commander source address
      @param  ui8_sourceAddress  source address of curvature commander
      */
  void setCommander(uint8_t ui8_sourceAddress);
  /*@}*/

  /* ****************************************************** */
  /** \name Retrieve Values which are sent from other ECUs  */
  /*@{*/
  /** set desired course curvature over ground that a machine's steering system is required to achieve
      @return  desired course curvature over ground
    */
  int16_t curvatureCmd() {return ( mmap_commanders[mui8_commanderSa].ui16_curvatureCmd / 4 ) - 8032;}

  /** set whether the guidance system is attempting to control steering with this command
      @return  is guidance system attempting to control steering
    */
  IsoAgLib::IsoSteerFlag_t curvatureCmdStatus() {return mmap_commanders[mui8_commanderSa].t_curvatureCmdStatus;}

  /** get currently selected curvature commander source address
      @return  source address of curvature commander
      */
  uint8_t getCommander() {return mui8_commanderSa;}

  /** get data of a curvature commander by source address
      @param  ui8_sourceAddress  source address
      @return                    curvature commander data or NULL if not exist
    */
  CurvatureCommander_s* getCommanderDataBySA( uint8_t ui8_sourceAddress );

  /** get data of a curvature commander by m_index
      @param ui8_index  source address
      @return curvature commander data or NULL if not exist
    */
  CurvatureCommander_s* getCommanderDataByIndex( uint8_t ui8_index);
  /*@}*/


  virtual bool processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t );

 private:
  // Private methods
  /** HIDDEN constructor for a TracGuidance_c object instance which can optional
      set the configuration for send/receive for a guidance msg
      NEVER instantiate a variable of type TracGuidance_c within application
      only access TracGuidance_c via getTracGuidanceInstance() or getTracGuidanceInstance( int riLbsBusNr ) in case more than one ISO11783 BUS is used for IsoAgLib
    */
  TracGuidanceCommand_c();

  /** check if filter boxes shall be created - create only ISO filters based
      on active local idents which has already claimed an address
      --> avoid to much Filter Boxes
    */
  virtual void checkCreateReceiveFilter( );

  /** send a ISO11783 guidance information PGN.
    * this is only called when sending ident is configured and it has already claimed an address
      @pre  function is only called in tractor mode
      @see  BaseCommon_c::timeEvent()
    */
  virtual void timeEventImplMode( );

  /** process a ISO11783 base information PGN
      @pre  sender of message is existent in monitor list
      @see  CanPkgExt_c::resolveSendingInformation()
    */
  virtual void processMsg( const CanPkg_c& arc_data );

  /** send guidance data
      @see  CanIo_c::operator<<
    */
  void isoSendMessageImplMode( );

 private:
  // Private attributes

  /** source address of the currently selected curvature commander */
  uint8_t mui8_commanderSa;

  /** list of all available clients which sends curvature commands */
  STL_NAMESPACE::map<uint8_t, CurvatureCommander_s> mmap_commanders; // mmap_commanders[sourceAddress]
  friend TracGuidanceCommand_c &getTracGuidanceCommandInstance(uint8_t aui8_instance);
};

/** C-style function, to get access to the unique TracGuidanceCommand_c singleton instance
  * if more than one CAN BUS is used for IsoAgLib, an m_index must be given to select the wanted BUS
  */
TracGuidanceCommand_c &getTracGuidanceCommandInstance(uint8_t aui8_instance = 0);

}//end namespache __IsoAgLib

#endif
