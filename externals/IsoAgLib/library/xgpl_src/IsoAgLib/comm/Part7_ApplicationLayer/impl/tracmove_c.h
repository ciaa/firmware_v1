/*
  tracmove_c.h: stores, updates and delivers all moving data
    information from CanCustomer_c derived for CAN sending and
    receiving interaction; from BaseCommon_c derived for interaction
    with other IsoAgLib objects

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TRACMOVE_C_H
#define TRACMOVE_C_H

#include <IsoAgLib/comm/Part7_ApplicationLayer/impl/basecommon_c.h>



// Begin Namespace __IsoAgLib
namespace __IsoAgLib {

#define GROUND_BASED_SPEED_DIST_PGN_DISABLE_MASK         0x0001LU
#define WHEEL_BASED_SPEED_DIST_PGN_DISABLE_MASK          0x0002LU
#define SELECTED_SPEED_PGN_DISABLE_MASK                  0x0004LU
#define ELECTRONIC_ENGINE_CONTROLLER_1_PGN_DISABLE_MASK  0x0008LU 

  /** stores, updates  and delivers all moving data information;
      Derive from BaseCommon_c some fundamental funktionality for all base data
      Derive from SchedulerTask_c to register in Scheduler_c for timeEvent trigger
      Derive from CANCustomer to register FilterBox'es in CanIo_c to receive CAN messages
      Derive from SINGLETON to create a Singleton which manages one global accessible singleton
      per IsoAgLib instance (if only one IsoAgLib instance is defined in application config, no overhead is produced).
    */

  class TracMove_c : public BaseCommon_c
  {
    MACRO_MULTITON_CONTRIBUTION();
  public:// Public methods

    /** @param apc_ident pointer to the variable of the responsible member instance. If NULL the module cannot send requests!
        @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
        @return true -> configuration was successfull
    */
    virtual bool config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode, uint16_t aui16_suppressMask = 0);

    /** update selected speed with actually best available speed
        @param t_speedSrc  from which source is the speed available
      */
    void updateSpeed(IsoAgLib::SpeedSource_t t_speedSrc, int32_t ai_time );

    /** update distance and direction with the actually best available distance and direction
        @param t_distanceSrc  from which source is the distance and direction available
      */
    void updateDistanceDirection(IsoAgLib::DistanceDirectionSource_t t_distanceSrc);

    virtual ~TracMove_c() {}

    /* ******************************************* */
    /** \name Set Values for periodic send on BUS  */
    /*@{*/
    /** set the theoretical (gear calculated) driven distance
        @param rreflVal value to store as theoretical (gear calculated) driven distance
      */
    void setDistTheor(const uint32_t& rreflVal) { mui32_distTheor = rreflVal; }

    /** set the real (radar measured) driven distance
        @param rreflVal value to store as real radar measured distance
      */
    void setDistReal(const uint32_t& rreflVal) { mui32_distReal = rreflVal; }

    /** set the value of real speed (measured by radar)
        @param ai32_val value to store as real radar measured speed
      */
    void setSpeedReal(const int32_t& ai32_val)
      {mi32_speedReal = ai32_val; if(mt_selectedSpeedSource == IsoAgLib::IsoGroundBasedSpeed) mi32_selectedSpeed = ai32_val;}

    /** set the value of theoretical speed (calculated from gear)
        @param ai32_val value to store as theoretical gear calculated speed
      */
    void setSpeedTheor(const int32_t& ai32_val)
      {mi32_speedTheor = ai32_val; if(mt_selectedSpeedSource == IsoAgLib::IsoWheelBasedSpeed) mi32_selectedSpeed = ai32_val;}

    /** set measured signal indicating either forward or reverse as the real (radar measured) direction of travel
        @return  direction of travel
      */
    void setDirectionReal(IsoAgLib::IsoDirectionFlag_t t_val)
      {mt_directionReal = t_val; if(mt_selectedSpeedSource == IsoAgLib::IsoGroundBasedSpeed) mt_selectedDirection = t_val;}

    /** set measured signal indicating either forward or reverse as the theoretical (gear calculated) direction of travel
        @return  direction of travel
      */
    void setDirectionTheor(IsoAgLib::IsoDirectionFlag_t t_val)
      {mt_directionTheor = t_val; if(mt_selectedSpeedSource == IsoAgLib::IsoWheelBasedSpeed) mt_selectedDirection = t_val;}

    /** set parameter which indicates whetcher the reported direction is reversed from the perspective of the operator
        @param at_val  indicates direction (IsoInactive = not reversed; IsoActive = reversed)
      */
    void setOperatorDirectionReversed(const IsoAgLib::IsoOperatorDirectionFlag_t at_val) { mt_operatorDirectionReversed = at_val;}

    /** start/stop state BE AWARE THIS IS A DUMMY BECAUSE DESCRIPTION IS NOT TO FIND IN AMENDMENT 1*/
    void setStartStopState(const IsoAgLib::IsoActiveFlag_t at_val) {mt_startStopState = at_val;}

    /** set actual distance traveled by the machine based on the value of selected machine speed
        @param i32_val  actual distance
      */
    void setSelectedDistance(const uint32_t& i32_val) {mui32_selectedDistance = i32_val;}

    /** set current direction of travel of the machine
        @param t_val  current direction of travel
      */
    void setSelectedDirection(IsoAgLib::IsoDirectionFlag_t t_val) {mt_selectedDirection = t_val;}

    /** get current value of the speed as determined from a number of sources by the machine
        @param i32_val  current value of speed
      */
    void setSelectedSpeed(const int32_t& i32_val)  {mi32_selectedSpeed = i32_val;}

    /** set speed source that is currently being reported in the machine speed parameter
        @param t_val  actual speed source
      */
    void setSelectedSpeedSource(IsoAgLib::IsoSpeedSourceFlag_t t_val) {mt_selectedSpeedSource = t_val;}

    /** present limit status of selected speed
        @param t_val  limit status
      */
    void setSelectedSpeedLimitStatus(const IsoAgLib::IsoLimitFlag_t t_val) {mt_selectedSpeedLimitStatus = t_val;}

    void setEngineSpeed(const uint16_t aui16_engineSpeed) { mui16_engineSpeed = aui16_engineSpeed; }
    /*@}*/


    /* ****************************************************** */
    /** \name Retrieve Values which are sent from other ECUs  */
    /*@{*/
    /** get the real driven distance with int16_t val
        @return actual radar measured driven distance value
      */
    uint32_t distReal() const { return mui32_distReal;}

    /** get the real driven distance with int16_t val
        @return actual gear calculated driven distance value
      */
    uint32_t distTheor() const { return mui32_distTheor;}

    /** get the value of real speed (measured by radar)
        @return actual radar measured speed value
      */
    int32_t speedReal() const { return mi32_speedReal;}

    /** is looking for a valid speed value
        @return true if speed is valid otherwise false
      */
    bool isRealSpeedUsable() const;

    /** is looking for a missing speed value
        @return true if speed is missing otherwise false
      */
    bool isRealSpeedMissing() const { return (mi32_speedReal == NO_VAL_32S);}

    /** is looking for a erroneous speed value
        @return true if speed is erroneous otherwise false
      */
    bool isRealSpeedErroneous() const { return (mi32_speedReal == ERROR_VAL_32S);}

    /** get the value of theoretical speed (calculated from gear)
        @return theoretical gear calculated speed value
      */
    int32_t speedTheor() const { return mi32_speedTheor;}

    /** is looking for a valid speed value
        @return true if speed is valid otherwise false
      */
    bool isTheorSpeedUsable() const;

    /** is looking for a missing speed value
        @return true if speed is missing otherwise false
      */
    bool isTheorSpeedMissing() const { return (mi32_speedTheor == NO_VAL_32S);}

    /** is looking for a erroneous speed value
        @return true if speed is erroneous otherwise false
      */
    bool isTheorSpeedErroneous() const { return (mi32_speedTheor == ERROR_VAL_32S);}

    /** get measured signal indicating either forward or reverse as the theoretical (gear calculated) direction of travel
        @return  direction of travel
      */
    IsoAgLib::IsoDirectionFlag_t directionTheor() {return mt_directionTheor;}

    /** get measured signal indicating either forward or reverse as the real (radar measured) direction of travel
        @return  direction of travel
      */
    IsoAgLib::IsoDirectionFlag_t directionReal() {return mt_directionReal;}

    /** get parameter which indicates whetcher the reported direction is reversed from the perspective of the operator
        @return indicates direction (IsoInactive = not reversed; IsoActive = reversed)
      */
    IsoAgLib::IsoOperatorDirectionFlag_t operatorDirectionReversed()const { return mt_operatorDirectionReversed;}

    /** start/stop state BE AWARE THIS IS A DUMMY BECAUSE DESCRIPTION IS NOT TO FIND IN AMENDMENT 1*/
    IsoAgLib::IsoActiveFlag_t startStopState() const {return mt_startStopState;}

    /** get actual distance traveled by the machine based on the value of selected machine speed
        @return  actual distance traveled
      */
    uint32_t selectedDistance() const {return mui32_selectedDistance;}

    /** get current direction of travel of the machine
        @return  current direction of travel
      */
    IsoAgLib::IsoDirectionFlag_t selectedDirection() const {return mt_selectedDirection;}

    /** get current value of the speed as determined from a number of sources by the machine
        @return  current value of speed
      */
    int32_t selectedSpeed() const {return mi32_selectedSpeed;}

    /** is looking for a valid speed value
        @return true if speed is valid otherwise false
      */
    bool isSelectedSpeedUsable() const;

    /** is looking for a missing speed value
        @return true if speed is missing otherwise false
      */
    bool isSelectedSpeedMissing() const { return (mi32_selectedSpeed == NO_VAL_32S); }

    /** is looking for a erroneous speed value
        @return true if speed is erroneous otherwise false
      */
    bool isSelectedSpeedErroneous() const { return (mi32_selectedSpeed == ERROR_VAL_32S); }

    /** present limit status of selected speed
        @return  limit status
      */
    IsoAgLib::IsoLimitFlag_t selectedSpeedLimitStatus() const {return mt_selectedSpeedLimitStatus;}

    /** get speed source that is currently being reported in the machine speed parameter
        @return  speed source that is currently being reported
      */
    IsoAgLib::IsoSpeedSourceFlag_t selectedSpeedSource() const {return mt_selectedSpeedSource;}

    /** get the value of engine speed
        @return engine speed -> 1 bit is 0.125 rpm
      */
    uint16_t engineSpeed() const { return mui16_engineSpeed;}


  virtual bool processMsgRequestPGN (uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver, int32_t );

  private:
    // Private methods
    /** HIDDEN constructor for a TracMove_c object instance which can optional
        set the configuration for send/receive for a moving msg
        NEVER instantiate a variable of type TracMove_c within application
        only access TracMove_c via getTracMoveInstance() or getTracMoveInstance( int riLbsBusNr ) in case more than one BUS is used for IsoAgLib
      */
    TracMove_c() : mui16_engineSpeed(0) {}

    /** check if filter boxes shall be created - create only filters based
        on active local idents which has already claimed an address
        --> avoid to much Filter Boxes
      */
    virtual void checkCreateReceiveFilter( );

    /** send a ISO11783 moving information PGN.
      * this is only called when sending ident is configured and it has already claimed an address
        @pre  function is only called in tractor mode
        @see  BaseCommon_c::timeEvent()
      */
    virtual void timeEventTracMode();

    /** send a ISO11783 moving information PGN.
      * this is only called when sending ident is configured and it has already claimed an address
        @pre  function is only called in implement mode
        @see  BaseCommon_c::timeEvent()
      */
    virtual void timeEventImplMode();

    /** process a ISO11783 base information PGN
        @pre  sender of message is existent in monitor list
        @see  CanPkgExt_c::resolveSendingInformation()
      */
    virtual void processMsg( const CanPkg_c& arc_data );

    /** send moving data with ground&theor speed&dist
        @see  CanIo_c::operator<<
      */
    void sendMovingTracMode( );

    /** Can ground based speed/distance be sent?
     */
    bool canSendGroundBasedSpeedDist();

    enum SendMessage_e { MessageSent, MessageNotSent };

    /** Send ground based speed/distance.
     */
    SendMessage_e sendGroundBasedSpeedDist();

    /** Can wheel based speed/distance be sent?
     */
    bool canSendWheelBasedSpeedDist();

    /** Send wheel based speed/distance.
     */
    SendMessage_e sendWheelBasedSpeedDist();

    /** Can selected speed distance be sent?
     */
    bool canSendSelectedSpeed();

    /** Can selected engine speed be sent?
     */
    bool canSendEngineSpeed();
    
    /** Send selected speed distance.
     */
    SendMessage_e sendSelectedSpeed();
    
    /** Send engine speed distance.
     */
    SendMessage_e sendEngineSpeed();

    /** Prepare sending any message.
     */
    void prepareSending( CanPkgExt_c& pkg );

  private:
    // Private attributes
    /** actually selected distance and direction source */
    IsoAgLib::DistanceDirectionSource_t mt_distDirecSource;

    uint32_t mui32_lastUpdateTimeSpeedSelected;
    uint32_t mui32_lastUpdateTimeSpeedReal;
    uint32_t mui32_lastUpdateTimeSpeedTheor;

    /** last time when direction and distance data was processed */
    uint32_t mui32_lastUpdateTimeDistDirec;


    /************ DISTANCE **************/

    /** real distance as int32_t value (cumulates 16bit overflows) */
    uint32_t mui32_distReal;

    /** theoretical distance as int32_t value (cumulates 16bit overflows)*/
    uint32_t mui32_distTheor;

    /** last 16bit real distance to cope with 16bit overflows */
    //uint16_t ui32_lastDistReal;

    /** last 16bit theoretical distance to cope with 16bit overflows */
    //uint16_t ui32_lastDistTheor;

    /** actual distance traveled by the machine based on the value of selected machine speed */
    uint32_t mui32_selectedDistance;

    /** start/stop state BE AWARE THIS IS A DUMMY BECAUSE DESCRIPTION IS NOT TO FIND IN AMENDMENT 1*/
    IsoAgLib::IsoActiveFlag_t mt_startStopState;


    /************ DIRECTION *************/
    /** parameter indicates whetcher the reported direction is reversed from the perspective of the operator */
    IsoAgLib::IsoOperatorDirectionFlag_t mt_operatorDirectionReversed;

    /** measured signal indicating either forward or reverse as the direction of travel */
    IsoAgLib::IsoDirectionFlag_t mt_directionTheor;

    /** measured signal indicating either forward or reverse as the direction of travel */
    IsoAgLib::IsoDirectionFlag_t mt_directionReal;

    /** indicates the current direction of travel of the machine */
    IsoAgLib::IsoDirectionFlag_t mt_selectedDirection;


    /************ SPEED ***************/
    /** actually selected speed source */
    IsoAgLib::SpeedSource_t mt_speedSource;

    /** real speed */
    int32_t mi32_speedReal;

    /** theoretical speed */
    int32_t mi32_speedTheor;

    /** current value of the speed as determined from a number of sources by the machine */
    int32_t mi32_selectedSpeed;

    /** present limit status of selected speed */
    IsoAgLib::IsoLimitFlag_t mt_selectedSpeedLimitStatus;

    /** indicates the speed source that is currently being reported in the machine speed parameter */
    IsoAgLib::IsoSpeedSourceFlag_t mt_selectedSpeedSource;

    uint16_t mui16_engineSpeed;
    friend TracMove_c &getTracMoveInstance(uint8_t aui8_instance);
  };

  inline bool TracMove_c::canSendGroundBasedSpeedDist() {
    return 0 == (GROUND_BASED_SPEED_DIST_PGN_DISABLE_MASK & mui16_suppressMask);
  }

  inline bool TracMove_c::canSendWheelBasedSpeedDist() {
    return 0 == (WHEEL_BASED_SPEED_DIST_PGN_DISABLE_MASK & mui16_suppressMask);
  }

  inline bool TracMove_c::canSendSelectedSpeed() {
    return 0 == (SELECTED_SPEED_PGN_DISABLE_MASK & mui16_suppressMask);
  }

  inline bool TracMove_c::canSendEngineSpeed() {
    return 0 == (ELECTRONIC_ENGINE_CONTROLLER_1_PGN_DISABLE_MASK & mui16_suppressMask);
  }

  /** C-style function, to get access to the unique Base_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  TracMove_c &getTracMoveInstance(uint8_t aui8_instance = 0);
}
#endif
