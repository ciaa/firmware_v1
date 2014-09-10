/*
  aux2functions_c.h - 

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef AUX2FUNCTIONS_C_H
#define AUX2FUNCTIONS_C_H

#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtclientobjectpool_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/iisoname_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/canpkgext_c.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/scheduler/impl/schedulertask_c.h>

#include "../ivtobjectauxiliaryfunction2_c.h"

#include <map>


namespace __IsoAgLib {

class VtClientConnection_c;

class Aux2Functions_c : public SchedulerTask_c
{
public:
  enum Aux2FunctionsState_en
  {
    State_WaitForPoolUploadSuccessfully,
    State_WaitForFirstInputMaintenanceMessage,
    State_CollectInputMaintenanceMessage,
    State_Ready
  };

  Aux2Functions_c( VtClientConnection_c& vtConnection );

  virtual ~Aux2Functions_c(void);

  void loadAssignment();

#ifdef USE_VTOBJECT_auxiliaryfunction2
  STL_NAMESPACE::map<uint16_t, vtObjectAuxiliaryFunction2_c*>& getObjects() { return m_aux2Function; }
#endif

  void notifyOnAux2InputStatus( const CanPkgExt_c& arc_data, IsoAgLib::iVtClientObjectPool_c& arc_pool);

  /** update maintenance time stamp in mmap_receivedInputMaintenanceData and decide when to send the preferred assignment message
   *
   * sending preferred assingment:
   * 1. wait for own object pool upload successfully
   * 2. wait for first input maintenance message with ready state
   *    - first receive: send empty preferred assignments if we do not have any preferred assignments
   *    - following receives until timeout: check if preferred assignment match with the input devices
   *    - after timeout: send preferred assignments for detected inputs (maybe empty too)
   */
  void notifyOnAux2InputMaintenance( const CanPkgExt_c& arc_data);

  // @return error-code as in J.7.6
  uint8_t storeAux2Assignment(Stream_c& arc_stream, uint16_t& rui16_functionObjId, IsoAgLib::iVtClientObjectPool_c& arc_pool);

  void timeEvent();


  bool sendPreferredAux2Assignments();

  void setState(Aux2FunctionsState_en a_state) { m_state = a_state; }

  /** set internal state and send empty preferred AUX2 assignment message (if we don't have any preferred assignments) */
  void objectPoolUploadedSuccessfully();

  void setLearnMode(bool a_learnMode) { mb_learnMode = a_learnMode; }

private:

  // use this structure to store received data from input maintenance message in map with isoname as key
  struct InputMaintenanceDataForIsoName_s
  {
    InputMaintenanceDataForIsoName_s()
      : mui16_inputModelIdentificationCode(0xFFFF),
        mi32_timeLastAux2Maintenance(0)
    {}

    InputMaintenanceDataForIsoName_s(uint16_t a_inputModelIdentificationCode, int32_t a_timeLastAux2Maintenance )
      : mui16_inputModelIdentificationCode(a_inputModelIdentificationCode),
        mi32_timeLastAux2Maintenance(a_timeLastAux2Maintenance)
    {}

    uint16_t mui16_inputModelIdentificationCode;
    int32_t  mi32_timeLastAux2Maintenance;
  };

  // use this map:
  // 1. to detect input maintenance message timeout => unassign functions
  // 2. when storing preferred assignment (during processing of assign command from VT), get the model identificaton code from here (via matching isoname)
  STL_NAMESPACE::map<IsoName_c,InputMaintenanceDataForIsoName_s> mmap_receivedInputMaintenanceData;

#ifdef USE_VTOBJECT_auxiliaryfunction2
  STL_NAMESPACE::map<uint16_t, vtObjectAuxiliaryFunction2_c*> m_aux2Function;
#endif

  // back reference for accessing functions in parent
  VtClientConnection_c& m_vtConnection;

  Aux2FunctionsState_en m_state;

  bool mb_learnMode;

private:
  /** not copyable : copy constructor is only declared, never defined */
  Aux2Functions_c(const Aux2Functions_c&);
  /** not copyable : copy operator is only declared, never defined */
  Aux2Functions_c& operator=(const Aux2Functions_c&);
};

}

#endif
