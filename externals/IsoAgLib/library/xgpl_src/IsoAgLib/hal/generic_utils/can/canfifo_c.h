#ifndef __CANFIFO_C_H
#define __CANFIFO_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/driver/can/impl/canpkg_c.h>

namespace HAL {

  class CanFifo_c {
    public:
      CanFifo_c();
      void push( __IsoAgLib::CanPkg_c& pkg  );
      __IsoAgLib::CanPkg_c& front();
      void pop();
      bool empty() const;
    private:
      static const unsigned m_bufferSize = 1 << CAN_FIFO_EXPONENT_BUFFER_SIZE; // see isoaglib_config.h

      volatile unsigned m_rIdx;
      volatile unsigned m_wIdx;

      __IsoAgLib::CanPkg_c m_data[m_bufferSize];
  };


  class CanFifos_c {
    public:
      static CanFifo_c& get( unsigned channel ) {
        return m_fifos[ channel ];
      }

    private:
      static CanFifo_c m_fifos[];
  };

}

#endif
