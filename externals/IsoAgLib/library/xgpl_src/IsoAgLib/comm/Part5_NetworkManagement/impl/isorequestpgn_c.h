/*
  isorequestpgn_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISO_REQUESTPGN_C_H
#define ISO_REQUESTPGN_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/driver/can/impl/cancustomer_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/canpkgext_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>


namespace __IsoAgLib {

// forward declaration
class IsoRequestPgnHandler_c;


/** this object manages a list of all ISO members to react on Request for PGN
    @short Manager for handling of Requests for PGN
    @see
    @author Dipl.-Inf. (FH) Martina Winkler */

class IsoRequestPgn_c : CanCustomer_c
{
  MACRO_MULTITON_CONTRIBUTION();
private:
  struct PGN_s
  {
    IsoRequestPgnHandler_c *p_handler;
    uint32_t ui32_pgn;
    uint32_t mui32_pgnMask;
    PGN_s(
        IsoRequestPgnHandler_c *ap_handler,
        uint32_t aui32_pgn,
        uint32_t aui32_pgnMask = mscui32_pgnDefaultMask);
    bool operator==(PGN_s const &arc_pgn) const;
  };

  struct DoesMatchPgn_s {
    bool operator()(PGN_s const &rcs_pgn) {
      return doMatchPgns(mui32_pgn, rcs_pgn.ui32_pgn, rcs_pgn.mui32_pgnMask);
    }
    DoesMatchPgn_s(uint32_t aui32_pgn) : mui32_pgn(aui32_pgn) {}
  private:
    uint32_t mui32_pgn;
  };

public:
  static uint32_t const mscui32_pgnDefaultMask = 0x3FFFFuL;

  /** Do two PGNs match for a given PGN mask?
   */
  static inline bool doMatchPgns(
      uint32_t aui32_pgnA,
      uint32_t aui32_pgnB,
      uint32_t aui32_pgnMask = mscui32_pgnDefaultMask);

  void init (void);
  void close (void);

  virtual ~IsoRequestPgn_c () {}


  /** adds the PGN to the list
      @param r_PGNHandler: reference to a IsoRequestPgnHandler_c from IsoMonitor_c, IsoItem_c, TimePosGps_c, TracGeneral_c or IsoTerminalServer
      @param cui32_pgnToRegister: PGN for which the r_PGNHandler's processMsgRequestPGN should be called
      @param aui32_pgnMask: mask to register multiple PGNs at once.
      @return true, if PGN for that r_PGNHandler wasn't registered yet and is now added and registered successfully
              false, if PGN for that r_PGNHandler was already registered and needn't be added again */
  bool registerPGN (IsoRequestPgnHandler_c &r_PGNHandler, const uint32_t cui32_pgnToRegister, uint32_t aui32_pgnMask = mscui32_pgnDefaultMask);

  /** adds n PGN for the client to the list
      @param r_PGNHandler: reference to a IsoRequestPgnHandler_c from IsoMonitor_c, IsoItem_c, TimePosGps_c, TracGeneral_c or IsoTerminalServer
      @param cui8_pgnCount: number of PGNs to be added in the list
      @param pcui32_pgnToRegister: array of PGNs for which the r_PGNHandler's processMsgRequestPGN should be called
      @return true, if at least one of the PGNs for that r_PGNHandler wasn't registered yet and is now added and registered successfully
              false, if all PGNs for that r_PGNHandler were already registered and needn't be added again*/
  bool registerPGN (IsoRequestPgnHandler_c &r_PGNHandler, const uint8_t cui8_pgnCount, const uint32_t *pcui32_pgnToRegister);

  /** removes a PGN from the list
      @param r_PGNHandler: reference to a IsoRequestPgnHandler_c from IsoMonitor_c, IsoItem_c, TimePosGps_c, TracGeneral_c or IsoTerminalServer
      @param cui32_pgnToRegister if not given, all PGNs of the r_PGNHandler will be removed
                                 if given, only that single PGN will be removed
      @param aui32_pgnMask: PGN mask given by the corresponding call of registerPGN.
    */
  void unregisterPGN (IsoRequestPgnHandler_c &r_PGNHandler, const uint32_t cui32_pgnToRegister = 0, uint32_t aui32_pgnMask = mscui32_pgnDefaultMask);

  /** removes n PGN for the client from the list
      @param r_PGNHandler: reference to a IsoRequestPgnHandler_c from IsoMonitor_c, IsoItem_c, TimePosGps_c, TracGeneral_c or IsoTerminalServer
      @param cui8_pgnCount: number of PGNs to be removed from the list
      @param pcui32_pgnToUnregister: array of PGNs to be removed */
  void unregisterPGN (IsoRequestPgnHandler_c &r_PGNHandler, const uint8_t cui8_pgnCount, const uint32_t *pcui32_pgnToUnregister);

  /** before adding any further PGN - RequestPGNHandler, check if not already existing
      @param r_PGNHandler: reference to a IsoRequestPgnHandler_c from IsoMonitor_c, IsoItem_c, TimePosGps_c, TracGeneral_c or IsoTerminalServer
      @param cui32_pgn: check that PGN, please!
      @param aui32_pgnMask: mask to register multiple PGNs at once
      @return true, if the r_PGNHandler is already registered with that PGN */
  bool checkIfAlreadyRegistered (IsoRequestPgnHandler_c &r_PGNHandler, const uint32_t cui32_pgn, uint32_t aui32_pgnMask = mscui32_pgnDefaultMask);

  /** Only call sendCannotRespondNow(..) when you're about to respond to a requested PGN */
  void answerRequestPGNwithACK (IsoItem_c& arc_isoItemSender, uint8_t aui8_ackCode) { sendAcknowledgePGN (arc_isoItemSender, aui8_ackCode); }

protected: // Protected methods
  /** process system msg
    * @return true -> message (Request for PGN) processed by IsoRequestPgn_c; false -> let others process */
  void processMsg ( const CanPkg_c& arc_data );

private: // Private methods
  /** constructor for IsoRequestPgn_c */
  IsoRequestPgn_c();

  void sendAcknowledgePGN (IsoItem_c& arc_isoItemSender, uint8_t aui8_ackType);

private: // Private attributes
  Subsystem_c mc_subsystemState;

  /// holds all registered clients with PGN(s)
  STL_NAMESPACE::vector<PGN_s> m_registeredClientsWithPGN;

  /// The following variables are just kept here as cache in case the user
  /// calls "sendAcknowledgePGN" out and the CAN-Pkg was changed in between
  /// by another ReqPGNHandler...
  IsoItem_c* mpc_isoItemSA;
  IsoItem_c* mpc_isoItemDA;
  uint32_t mui32_requestedPGN;

  bool mb_alreadyClosed;
  friend IsoRequestPgn_c &getIsoRequestPgnInstance(unsigned int instance);
};

/** C-style function, to get access to the unique IsoRequestPgn_c singleton instance
    if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS */
IsoRequestPgn_c& getIsoRequestPgnInstance (unsigned int instance = 0);

/** Convenience functor to register PGN. */
struct RegisterPgn_s : public STL_NAMESPACE::binary_function< uint32_t, uint32_t, void > {
  void operator()(
      uint32_t aui32_pgn,
      uint32_t aui32_pgnMask = IsoRequestPgn_c::mscui32_pgnDefaultMask) {
    getIsoRequestPgnInstance4Comm().registerPGN(*mcp_handler, aui32_pgn, aui32_pgnMask);
  }
  RegisterPgn_s(IsoRequestPgnHandler_c *apc_handler MULTITON_INST_PARAMETER_DEF_WITH_COMMA) :
    MULTITON_MEMBER_CONSTRUCTOR
    mcp_handler(apc_handler) {}
private:
  MULTITON_MEMBER_DEF
  IsoRequestPgnHandler_c *const mcp_handler;
};

/** Convenience functor to unregister PGN. */
struct UnregisterPgn_s : public STL_NAMESPACE::binary_function< uint32_t, uint32_t, void > {
  void operator()(
      uint32_t aui32_pgn,
      uint32_t aui32_pgnMask = IsoRequestPgn_c::mscui32_pgnDefaultMask) {
    getIsoRequestPgnInstance4Comm().unregisterPGN(*mcp_handler, aui32_pgn, aui32_pgnMask);
  }
  UnregisterPgn_s(IsoRequestPgnHandler_c *apc_handler MULTITON_INST_PARAMETER_DEF_WITH_COMMA) :
    MULTITON_MEMBER_CONSTRUCTOR
    mcp_handler(apc_handler) {}
private:
  MULTITON_MEMBER_DEF
  IsoRequestPgnHandler_c *const mcp_handler;
};

inline IsoRequestPgn_c::PGN_s::PGN_s(
    IsoRequestPgnHandler_c *ap_handler,
    uint32_t aui32_pgn,
    uint32_t aui32_pgnMask) :
  p_handler(ap_handler),
  ui32_pgn(aui32_pgn),
  mui32_pgnMask(aui32_pgnMask) {}

inline bool IsoRequestPgn_c::PGN_s::operator==(PGN_s const &arc_pgn) const {
  return (p_handler == arc_pgn.p_handler) &&
    (mui32_pgnMask == arc_pgn.mui32_pgnMask) &&
    DoesMatchPgn_s(ui32_pgn)(arc_pgn);
}

inline bool IsoRequestPgn_c::doMatchPgns(
    uint32_t aui32_pgnA,
    uint32_t aui32_pgnB,
    uint32_t aui32_pgnMask)
{
  return 0 == (aui32_pgnMask & (aui32_pgnA ^ aui32_pgnB));
}

}
#endif
