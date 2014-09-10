/*
  multiplevt_c.h: handle "next VT"

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef MULTIPLEVT_C_H
#define MULTIPLEVT_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/iisoname_c.h>
#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivttypes.h>

#include <queue>

namespace __IsoAgLib {

  class UploadPoolState_c;
  class VtClient_c;
  class VtServerInstance_c;

  class MultipleVt_c
  {
    public:

      enum TimeEventResult
      {
        TIME_EVENT_RESULT_NO_ACTION,
        TIME_EVENT_RESULT_TRY_NEXT_VT
      };

      MultipleVt_c(const UploadPoolState_c& aref_uploadPoolState,
                   const VtClient_c& aref_vtClient);

      /*
       * - trigger multipleActiveVts() callback if "move to next VT" will be possible or not
       * - detect pool upload failure
       */
      TimeEventResult timeEvent();

      void resetInfoShown()
      {
        m_moveToNextVtInfoShown = false;
      }

      void trySwitchingState();

      bool isSwitching() const
      {
        return (STATE_SWITCH_REQUESTED == m_state);
      }

      void resetSwitchingData();

      void setLastVtServerInstance(const VtServerInstance_c* ap_lastVtServerInstance);
      const VtServerInstance_c* getLastVtServerInstance() const
      {
        return mpc_lastVtServerInstance;
      }

    private:
      MultipleVt_c( const MultipleVt_c& ); // non-copyable
      MultipleVt_c& operator=( const MultipleVt_c& );

      enum State
      {
        STATE_INACTIVE,
        STATE_SWITCH_REQUESTED
      };

      const UploadPoolState_c& mref_uploadPoolState;
      const VtClient_c&        mref_vtClient;

      bool m_moveToNextVtInfoShown;

      State m_state;

      const VtServerInstance_c* mpc_lastVtServerInstance;
  };
}

#endif
