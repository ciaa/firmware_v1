/*
  icontrolfunctionstatehandler_c.h: notify on control function state
  changes.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ICONTROLFUNCTIONSTATEHANDLER_C_H
#define ICONTROLFUNCTIONSTATEHANDLER_C_H

#include <IsoAgLib/comm/Part5_NetworkManagement/iisoitem_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/cfstatehandler_c.h>

namespace IsoAgLib
{

class iControlFunctionStateHandler_c : private __IsoAgLib::ControlFunctionStateHandler_c {

public:
  using __IsoAgLib::ControlFunctionStateHandler_c::iIsoItemAction_e;
  using __IsoAgLib::ControlFunctionStateHandler_c::AddToMonitorList;
  using __IsoAgLib::ControlFunctionStateHandler_c::ChangedAddress;
  using __IsoAgLib::ControlFunctionStateHandler_c::LostAddress;
  using __IsoAgLib::ControlFunctionStateHandler_c::ReclaimedAddress;
  using __IsoAgLib::ControlFunctionStateHandler_c::RemoveFromMonitorList;

  virtual ~iControlFunctionStateHandler_c() {}

private:
  // Enable iIsoMonitor_c to recognize that iControlFunctionStateHandler_c is
  // derived from ControlFunctionStateHandler_c:
  friend class iIsoMonitor_c;

  /** Notice about control function state change
    */
  virtual void onIControlFunctionStateChange(
      iIsoItemAction_e ae_action, //< What happened?
      iIsoItem_c const &acrc_iIsoItem //< Who is it happened?
    ) = 0;

  // Don't override; for IsoAgLib internal use only:
  virtual void reactOnIsoItemModification(
      iIsoItemAction_e ae_action,
      __IsoAgLib::IsoItem_c const &acrc_isoItem)
  {
    onIControlFunctionStateChange(ae_action, static_cast< IsoAgLib::iIsoItem_c const & >(acrc_isoItem));
  }
};

} //namespace IsoAgLib

#endif
