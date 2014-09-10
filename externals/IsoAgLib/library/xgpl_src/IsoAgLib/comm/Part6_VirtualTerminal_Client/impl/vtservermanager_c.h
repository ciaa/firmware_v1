/*
  vtservermanager_c.h: store and manage VtServerInstances list 

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTSERVERMANAGER_C_H
#define VTSERVERMANAGER_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/cfstatehandler_c.h>

#include <vector>


namespace IsoAgLib {

class iVtClientObjectPool_c;

}

namespace __IsoAgLib {

class IsoName_c;
class VtServerInstance_c;
class IsoItem_c;
class VtClientConnection_c;
class VtClient_c;

class VtServerManager_c {

public:
    
  VtServerManager_c();
  virtual ~VtServerManager_c() {}

  void close();

  VtServerInstance_c* getActiveVtServer( bool mustBePrimary, const VtServerInstance_c* ap_searchStart ) const;
  VtServerInstance_c* getPreferredVtServer(const IsoName_c& aref_prefferedVTIsoName) const;
  VtServerInstance_c* getSpecificVtServer(const IsoAgLib::iVtClientObjectPool_c& arc_pool) const;
  uint16_t getActiveVtCount() const;

  bool isAnyVtAvailable() const { return !ml_vtServerInst.empty(); }

  void reactOnIsoItemModification (ControlFunctionStateHandler_c::iIsoItemAction_e at_action,
                                   IsoItem_c const& acrc_isoItem,
                                   STL_NAMESPACE::vector<VtClientConnection_c*>& aref_vtConnections,
                                   VtClient_c& aref_vtClient);

  void processVtStatusMsg(const CanPkgExt_c& arc_data,
                          STL_NAMESPACE::vector<VtClientConnection_c*>& aref_vtConnections);
  
  STL_NAMESPACE::vector<VtServerInstance_c*>& getRefServerInstanceVec() { return ml_vtServerInst; }

// the following define should be globally defined in the project settings...
// (currently not supported, due to multi VT enhancements)
#if 0
#ifdef USE_IOP_GENERATOR_FAKE_VT_PROPERTIES
  void fakeVtProperties (uint16_t aui16_dimension, uint16_t aui16_skWidth, uint16_t aui16_skHeight,
                         uint8_t aui16_colorDepth, uint16_t aui16_fontSizes,
                         VtClient_c& aref_vtClient);
#endif
#endif

private:
    
  STL_NAMESPACE::vector<VtServerInstance_c*> ml_vtServerInst;
  
};

}

#endif
