#include "multiplevt_c.h"

#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtclient_c.h>

namespace __IsoAgLib {

  MultipleVt_c::MultipleVt_c(const UploadPoolState_c& aref_uploadPoolState,
                             const VtClient_c& aref_vtClient)
      : mref_uploadPoolState(aref_uploadPoolState),
        mref_vtClient(aref_vtClient),
        m_moveToNextVtInfoShown(false),
        m_state(STATE_INACTIVE),
        mpc_lastVtServerInstance(NULL)
  {
  }

  MultipleVt_c::TimeEventResult MultipleVt_c::timeEvent()
  {
    if(mref_uploadPoolState.successfullyUploaded())
    {
      if(mref_vtClient.getActiveVtCount() > 1)
      {
        if(!m_moveToNextVtInfoShown)
        {
          mref_uploadPoolState.getPool().multipleActiveVts(true /* a_state */);
          m_moveToNextVtInfoShown = true;
        }
      }
      else
      {
        if(m_moveToNextVtInfoShown)
        {
          mref_uploadPoolState.getPool().multipleActiveVts(false /* a_state */);
          m_moveToNextVtInfoShown = false;
        }
      }
    }

    if(mref_uploadPoolState.unsuccessfullyUploaded())
    {
        // change to switching mode after pool upload failed
      trySwitchingState();

      switch(m_state)
      {
        case STATE_SWITCH_REQUESTED:
          return TIME_EVENT_RESULT_TRY_NEXT_VT;
        case STATE_INACTIVE:
          break;
      }

      // After VtClientConnection_c::restartWithNextVt() is called once,
      // the state in mref_uploadPoolState is set to initial.
      // => we do not return "TIME_EVENT_RESULT_TRY_NEXT_VT" in the next timeEvent()
      //    and client can try to connect with VT until mref_uploadPoolState.unsuccessfullyUploaded() is true again
    }

    return TIME_EVENT_RESULT_NO_ACTION;
  }

  void MultipleVt_c::trySwitchingState()
  {
    if(mref_vtClient.getActiveVtCount() > 1)
    {
      m_state = STATE_SWITCH_REQUESTED;
    }
    else
    {
       m_state = STATE_INACTIVE;
    }
  }

  void MultipleVt_c::resetSwitchingData()
  {
    m_state = STATE_INACTIVE;
    mpc_lastVtServerInstance = NULL;
  }

  void MultipleVt_c::setLastVtServerInstance(const VtServerInstance_c* ap_lastVtServerInstance)
  {
    mpc_lastVtServerInstance = ap_lastVtServerInstance;
  }
}
