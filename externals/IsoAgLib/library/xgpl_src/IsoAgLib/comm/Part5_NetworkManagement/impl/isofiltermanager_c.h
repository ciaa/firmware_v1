/*
  isofiltermanager_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISO_FILTER_MANAGER_C_H
#define ISO_FILTER_MANAGER_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>

#include "isofilterbox_c.h"

#include <cstdlib>	// Include before vector or else CNAMESPACE stuff is screwed up for Tasking
#include <vector>


namespace __IsoAgLib {


class IsoFilterManager_c : public Subsystem_c
{
  MACRO_MULTITON_CONTRIBUTION();
public:
  typedef STL_NAMESPACE::vector<IsoFilterBox_c> IsoFilterBox_vec;
  typedef STL_NAMESPACE::vector<IsoFilterBox_c>::iterator IsoFilterBox_it;

  ~IsoFilterManager_c () {}

  void init();
  void close();

  bool existIsoFilter (const IsoFilter_s& rrefcs_isoFilter);
  void insertIsoFilter (const IsoFilter_s& rrefcs_isoFilter);
  bool removeIsoFilter (const IsoFilter_s& rrefcs_isoFilter);

private:
  class ControlFunctionStateHandlerProxy_c : public ControlFunctionStateHandler_c {
  public:
    typedef IsoFilterManager_c Owner_t;

    ControlFunctionStateHandlerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}

    virtual ~ControlFunctionStateHandlerProxy_c() {}

  private:
    virtual void reactOnIsoItemModification(
        iIsoItemAction_e at_action,
        IsoItem_c const &acrc_isoItem)
    {
      mrt_owner.reactOnIsoItemModification(at_action, acrc_isoItem);
    }

    // not copyable
    ControlFunctionStateHandlerProxy_c(ControlFunctionStateHandlerProxy_c const &);
    ControlFunctionStateHandlerProxy_c &operator=(ControlFunctionStateHandlerProxy_c const &);

    Owner_t &mrt_owner;
  };
  typedef ControlFunctionStateHandlerProxy_c Handler_t;

  // Private methods

  void reactOnIsoItemModification (ControlFunctionStateHandler_c::iIsoItemAction_e /*at_action*/, IsoItem_c const& /*acrc_isoItem*/);

private:
  IsoFilterManager_c();

  IsoFilterBox_vec mvec_isoFilterBox;
  Handler_t mt_handler;

  friend IsoFilterManager_c &getIsoFilterManagerInstance( unsigned int instance );
};


IsoFilterManager_c &getIsoFilterManagerInstance( unsigned int instance = 0 );

}
#endif
