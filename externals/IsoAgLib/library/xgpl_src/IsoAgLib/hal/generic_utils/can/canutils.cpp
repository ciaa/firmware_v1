/*
  canutils.cpp:

  (C) Copyright 2012 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "canutils.h"

namespace HAL {

#ifdef USE_CAN_MEASURE_BUSLOAD
  canBusLoad_c canBusLoads[ HAL_CAN_MAX_BUS_NR + 1 ];

  void canBusLoad_c::init() {
    m_busLoadCurrentSlice = 0;
    for( int i = 0; i < mc_numSlices; ++i ) {
      m_busLoadBytes[ i ] = 0;
    }
  }
#endif

}
