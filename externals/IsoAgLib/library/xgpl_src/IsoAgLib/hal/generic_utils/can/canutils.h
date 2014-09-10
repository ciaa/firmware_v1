/*
  canutils.h:

  (C) Copyright 2012 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/



#ifndef __CANUTILS_H
#define __CANUTILS_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/hal_system.h>

namespace HAL {

#ifdef USE_CAN_MEASURE_BUSLOAD

  /** store bytes in 10 slots per seccond */
  class canBusLoad_c {
    public:
      void init();
      /** return bits in the last second */
      uint32_t getBusLoad() const;
      void updateCanBusLoad( int bytes );

    private:
      static const int mc_numSlices = 11; // 10 slots per second counting for baudrate and one current slot
      unsigned m_busLoadBytes[ mc_numSlices ];
      int m_busLoadCurrentSlice;
  };


  inline void canBusLoad_c::updateCanBusLoad( int bytes ) {
    const int newSlice = ( ( getTime() / 100 ) % mc_numSlices );
    if ( m_busLoadCurrentSlice != newSlice ) {
      m_busLoadBytes[ newSlice ] = bytes;
      m_busLoadCurrentSlice = newSlice;
    } else {
      m_busLoadBytes[ newSlice ] += bytes;
    }
  }


  inline uint32_t canBusLoad_c::getBusLoad() const {
    uint32_t rate = 0;
    for ( int i = 0; i < mc_numSlices; ++i ) {
      if( i != m_busLoadCurrentSlice ) {
        rate += m_busLoadBytes[ i ] * 8;
      }
    }
    return rate;
  }

  extern canBusLoad_c canBusLoads[ HAL_CAN_MAX_BUS_NR + 1 ];
#endif

}

#endif
