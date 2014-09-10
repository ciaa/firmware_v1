
#include <IsoAgLib/isoaglib_config.h>
#include "canfifo_c.h"


namespace HAL {

  CanFifo_c::CanFifo_c() : m_rIdx( 0 ), m_wIdx( 0 ) {}


  void CanFifo_c::push( __IsoAgLib::CanPkg_c& pkg  ) {
    const unsigned w = m_wIdx;
    if( w == ( m_rIdx + ( m_bufferSize * 2 ) ) ) {
      isoaglib_assert( !"buffer full" );
    }

    m_wIdx++;
    m_data[ ( m_wIdx / 2 ) % m_bufferSize ] = pkg;
    m_wIdx++;
  }


  void CanFifo_c::pop() {
    m_rIdx += 2;
  }


  __IsoAgLib::CanPkg_c& CanFifo_c::front() {
    isoaglib_assert( ! empty() );
    return m_data[ ( m_rIdx / 2 ) % m_bufferSize ];
  }


  bool CanFifo_c::empty() const {
    const unsigned w = m_wIdx;
    return( ( w == m_rIdx ) || ( ( w - m_rIdx ) == 1 ) );
  }


  CanFifo_c CanFifos_c::m_fifos[ HAL_CAN_MAX_BUS_NR + 1 ];
}
