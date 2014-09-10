/*
  hal_can.h: include dependent on used target (defined in
    IsoAgLib/isoaglib_config.h) the suitable HAL specific header for
    CAN communication

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _HAL_CAN_H_
#define _HAL_CAN_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/hal_config.h>

namespace __IsoAgLib {
  class CanPkg_c;
}

namespace HAL {

  bool canInit( unsigned channel, unsigned baudrate );
  bool canClose( unsigned channel );

  typedef enum {
    e_canNoError,
    e_canBusWarn,
    e_canBusOff
    // TODO
  } canState_t;

  bool canState( unsigned channel, canState_t& state );

  bool canTxSend( unsigned channel, const __IsoAgLib::CanPkg_c& msg );
  void canRxPoll( unsigned channel );
  bool canRxWait( unsigned timeout_ms );
#ifdef USE_MUTUAL_EXCLUSION
  void canRxWaitBreak();
#endif

  //! Returning -1 means that the queue can't be queried,
  //! but it definitely has enough space to put messages in!
  int canTxQueueFree( unsigned channel );

  void defineRxFilter( unsigned channel, bool xtd, uint32_t filter, uint32_t mask );
  void deleteRxFilter( unsigned channel, bool xtd, uint32_t filter, uint32_t mask );


} // HAL

#endif
