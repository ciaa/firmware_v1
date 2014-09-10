/*
  cfstatehandler_c.h - ControlFunctionStateHandler_c

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef CF_STATE_HANDLER_C_H
#define CF_STATE_HANDLER_C_H

#include <IsoAgLib/isoaglib_config.h>
#include "isorequestpgnhandler_c.h"

/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */

namespace IsoAgLib
{
class iControlFunctionStateHandler_c;
}

namespace __IsoAgLib
{

/** Handler class which can be used to react on all actions depending on address-claims.
 *  Actions that can take place are listed in the class-embdedded enum.
 */
class ControlFunctionStateHandler_c
{
public:

  enum iIsoItemAction_e { /// ATTENTION: If a node is added and "AddToMonitorList" is broadcast,
                          /// ~~~~~~~~~  the node doesn't need to have a valid Address at this moment!!
    AddToMonitorList,     /// Initial Address-Claim --> Node added to IsoMonitor
    ChangedAddress,       /// Changed Address
    LostAddress,          /// Address stolen due to Address-Claim of other node with Same SA (New Address is 254)
    ReclaimedAddress,     /// Reclaimed an Address after Address was stolen (Old Address was 254)
    RemoveFromMonitorList /// No more Address-Claims (after requests) --> Node removed from IsoMonitor
  };

  ControlFunctionStateHandler_c() {}
  virtual ~ControlFunctionStateHandler_c() {}

  /** this function is called by IsoMonitor_c on addition, state-change and removal of an IsoItem. */
  virtual void reactOnIsoItemModification(
      iIsoItemAction_e ae_action,
      IsoItem_c const &acrc_isoItem) = 0;

  /** this function is called when WS announce is finished (initial and updates) */
  virtual void reactOnWsAnnounceFinished(const IsoItem_c& /* acrc_isoItem */) {}
      
};

/** Proxy for ControlFunctionStateHandler_c.
  * Having such a proxy as component is an alternative to subclassing
  * ControlFunctionStateHandler_c directly.
  */
template < typename OWNER_T >
class ControlFunctionStateHandlerProxy_c : public ControlFunctionStateHandler_c {
public:
  typedef OWNER_T Owner_t;

  ControlFunctionStateHandlerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}

  virtual ~ControlFunctionStateHandlerProxy_c() {}

private:
  virtual void reactOnIsoItemModification(
      iIsoItemAction_e at_action,
      IsoItem_c const &acrc_isoItem)
  {
    mrt_owner.reactOnIsoItemModification(at_action, acrc_isoItem);
  }

  // ControlFunctionStateHandlerProxy_c shall not be copyable. Otherwise the
  // reference to the containing object would become invalid.
  ControlFunctionStateHandlerProxy_c(ControlFunctionStateHandlerProxy_c const &);

  ControlFunctionStateHandlerProxy_c &operator=(ControlFunctionStateHandlerProxy_c const &);

  Owner_t &mrt_owner;
};

}
#endif
