/*
  can_driver_sys.cpp: socketcan driver implementation

  (C) Copyright 2011 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#ifdef WIN32
#include <windows.h>
#endif

#include <vector>

#include "IsoAgLib/hal/generic_utils/can/canfifo_c.h"
#include <IsoAgLib/hal/pc/system/system.h>
#include <IsoAgLib/util/iassert.h>
#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/hal_can.h>


namespace __HAL {
  bool canStartDriver() {
    return true;
  }

  bool canStopDriver() {
    return true;
  }
}


namespace HAL {

  bool canInit( unsigned channel, unsigned baudrate ) {
    return true;
  }

  bool canClose( unsigned channel ) {
    return true;
  }


  bool canState( unsigned channel, canState_t& state ) {
    state = e_canNoError;
    return true;
  }


  bool canTxSend( unsigned channel, const __IsoAgLib::CanPkg_c& msg ) {
    return true;
  }

  void canRxPoll( unsigned channel ) {
  }


  bool canRxWait( unsigned timeout_ms ) {
    return false;
  }


  unsigned canTxQueueFree( unsigned channel ) {
    return -1;
  }


  bool defineRxFilter( uint32_t id, uint32_t mask ) {}
  bool deleteRxFilter( uint32_t id, uint32_t mask ) {}

} // end namespace __HAL

// eof
