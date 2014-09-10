/*
  basecommon_c.h: base class for basa data types

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef BASE_COMMON_H
#define BASE_COMMON_H

#include <IsoAgLib/scheduler/impl/schedulertask_c.h>
#include <IsoAgLib/scheduler/impl/scheduler_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoname_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgnhandler_c.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/driver/system/impl/system_c.h>
#include <IsoAgLib/comm/Part7_ApplicationLayer/ibasetypes.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgn_c.h>

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif

// Begin Namespace __IsoAgLib
namespace __IsoAgLib
{

  class BaseCommon_c : public CanCustomer_c
  {
  public:

    /// Note: The following comment is obsolete, but kept here for reference because
    ///       the Trac-classes aren't completely rewritten yet!!
    /// General init() so that EVERY subsystem of IsoAgLib has its init() call.
    /// nothing done so far, but can be overwritten in the derived classes if needed
    /// do not mistake this init() with the virtual (!) init_base() function
    /// It's okay that this init() normally does NOTHING, as init_base() is being
    /// called from singleonInit()!
    void init();

    // specialized init/close for the concrete derived specializations.
    // will be called at the end of "init()" and the beginning of "close()", resp.
    virtual void init_specialized() {}
    virtual void close_specialized() {}

    /** constructor */
    BaseCommon_c() :
      mt_task(*this, 100, true),
      mt_handler(*this),
      mui16_suppressMask(0),
      mt_identMode(IsoAgLib::IdentModeImplement),
      mb_filterCreated(false),
      mi32_lastMsgReceived(0),
      mpc_ident(NULL),
      mc_selectedDataSourceISOName(),
      mui16_timeOut(TIMEOUT_SENDING_NODE_NMEA)
    {}

    /** destructor */
    ~BaseCommon_c() {}

    /** every subsystem of IsoAgLib has explicit function for controlled shutdown */
    void close();

    /** tractor object after init --> store isoName and mode
        @param apc_ident pointer to the ident  instance (pointer enables automatic value update if var val is changed)
        @param at_identMode either IsoAgLib::IdentModeImplement or IsoAgLib::IdentModeTractor
      */
    virtual bool config_base ( const IdentItem_c* apc_ident, IsoAgLib::IdentMode_t at_identMode = IsoAgLib::IdentModeImplement, uint16_t aui16_suppressMask = 0);


    /** functions with actions, which must be performed periodically
        -> called periodically by Scheduler_c
        ==> sends base msg if configured in the needed rates
        possible errors:
          * dependant error in CanIo_c on CAN send problems
        @see CanPkg_c::getData
        @see CanPkgExt_c::getData
        @see CanIo_c::operator<<
      */
    virtual void timeEvent();

    /** send a PGN request */
    bool sendPgnRequest(uint32_t ui32_requestedPGN);

    /** check if preconditions for request for pgn are fullfilled
        @return  true -> the request for pgn can be send
      */
    virtual bool check4ReqForPgn(uint32_t aui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* apc_isoItemReceiver);

    /** send a ISO11783 base information PGN.
      * this is only called when sending ident is configured and it has already claimed an address
      */
    virtual void timeEventTracMode();

    /** send a ISO11783 base information PGN.
      * this is only called when sending ident is configured and it has already claimed an address
      */
    virtual void timeEventImplMode();

    /** Retrieve the last update time of the specified information type*/
    int32_t lastedTimeSinceUpdate() const { return (System_c::getTime() - mi32_lastMsgReceived);}

    /** Retrieve the time of last update */
    int32_t lastUpdateTime() const {return mi32_lastMsgReceived;}

    /** set last time of data msg [msec]*/
    void setUpdateTime(int32_t updateTime) {mi32_lastMsgReceived = updateTime;}

    /** check if a received message should be parsed */
    bool checkParseReceived(const IsoName_c& acrc_currentSender) const;

    /** return if you currently are in implement mode or tractor mode*/
    bool checkMode(IsoAgLib::IdentMode_t at_identMode) const {return (mt_identMode == at_identMode);}

    /** check if iso filters have alread been created*/
    bool checkFilterCreated() const {return mb_filterCreated;}

    /** set mb_filterCreated*/
    void setFilterCreated() {mb_filterCreated = true;}

    /** clear mb_filterCreated*/
    void clearFilterCreated() {mb_filterCreated = false;}

    const IdentItem_c* getIdentItem() const { return mpc_ident; }

    /** get IsoName of data source (e.g. tractor, terminal) from which commands are send exclusively */
    IsoName_c& getSelectedDataSourceISOName() {return mc_selectedDataSourceISOName;}

    /** get const IsoName of data source (e.g. tractor, terminal) from which commands are send exclusively */
    const IsoName_c& getSelectedDataSourceISONameConst() const {return mc_selectedDataSourceISOName;}

    /** get actual mode */
    IsoAgLib::IdentMode_t getMode() const {return mt_identMode;}

    /** set mode to implement or tractor*/
    void setMode(IsoAgLib::IdentMode_t at_identMode) {mt_identMode = at_identMode;}

    /** set IsoName of data source (e.g. tractor, terminal) which sends commands exclusively */
    void setSelectedDataSourceISOName(const IsoName_c& ac_dataSourceISOName){mc_selectedDataSourceISOName = ac_dataSourceISOName;}

    /** if a message is not sent after 3 or 5 seconds it is expected that the sending node stopped sending */
    static const uint16_t TIMEOUT_SENDING_NODE_NMEA = 3000;
    static const uint16_t TIMEOUT_SENDING_NODE_J1939 = 5000;

  protected:
    void setTimeOut( uint16_t aui16_timeout) { mui16_timeOut = aui16_timeout; }
    uint16_t getTimeOut( ) { return mui16_timeOut; }

    RegisterPgn_s getRegisterPgn() {
      return RegisterPgn_s(&mt_handler, 0 ); // XXX TODO
    }

    UnregisterPgn_s getUnregisterPgn(){
      return UnregisterPgn_s(&mt_handler, 0 ); // XXX TODO
    }

    bool initialized() const {
      return mt_task.initialized();
    }

    void setInitialized() {
      mt_task.setInitialized();
    }

    void setClosed() {
      mt_task.setClosed();
    }

  private:
    // BaseCommon_c shall not be copyable.
    BaseCommon_c(BaseCommon_c const &arc_from);
    BaseCommon_c &operator=(BaseCommon_c const &arc_from);

    CLASS_SCHEDULER_TASK_PROXY(BaseCommon_c)
    typedef SchedulerTaskProxy_c Task_t;

    class IsoRequestPgnHandlerProxy_c : public IsoRequestPgnHandler_c {
    public:
      typedef BaseCommon_c Owner_t;

      IsoRequestPgnHandlerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}

      virtual ~IsoRequestPgnHandlerProxy_c() {}

    private:
      virtual bool processMsgRequestPGN(
          uint32_t aui32_pgn,
          IsoItem_c *apc_isoItemSender,
          IsoItem_c *apc_isoItemReceiver,
          int32_t ai_time )
      {
        return mrt_owner.processMsgRequestPGN(
            aui32_pgn,
            apc_isoItemSender,
            apc_isoItemReceiver,
            ai_time );
      }

      // IsoRequestPgnHandlerProxy_c shall not be copyable. Otherwise
      // the reference to the containing object would become invalid.
      IsoRequestPgnHandlerProxy_c(IsoRequestPgnHandlerProxy_c const &);

      IsoRequestPgnHandlerProxy_c &operator=(IsoRequestPgnHandlerProxy_c const &);

      Owner_t &mrt_owner;
    };
    typedef IsoRequestPgnHandlerProxy_c Handler_t;

    // private methods
    /** check if filter boxes shall be created - create only ISO filters based
      on active local idents which has already claimed an address
      --> avoid to much Filter Boxes
    */
    virtual void checkCreateReceiveFilter() = 0;

    virtual bool processMsgRequestPGN(
        uint32_t aui32_pgn,
        IsoItem_c *apc_isoItemSender,
        IsoItem_c *apc_isoItemReceiver,
        int32_t ai_time ) = 0;

  protected:
    Task_t mt_task;
    Handler_t mt_handler;

    /** flags that disable PGNs individually */
    uint16_t mui16_suppressMask;

  private:
    /** can be implement mode or tractor mode */
    IsoAgLib::IdentMode_t mt_identMode;

    /** flag to detect, if receive filters for ISO are created */
    bool mb_filterCreated;

    /** last time of data msg [msec] */
    int32_t mi32_lastMsgReceived;

    /** identItem which act as sender of a msg: either responses on behalf of implement or commands as tractor. */
    const IdentItem_c* mpc_ident;

    /** IsoName of data source (e.g. tractor, terminal) from which commands are send exclusively */
    IsoName_c mc_selectedDataSourceISOName;

    /**
      * There are two timeout times for GPS-Positions and Speed
      * NMEA is 3 seconds, J1939 is 5 seconds.
      * So make timeout configurable.
      */
    uint16_t mui16_timeOut;
  };

}// end namespace __IsoAgLib

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#endif
