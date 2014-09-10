/*
  iisobus_c.h: Interface access class for the managed ISO11783-CAN-instances

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IISOBUS_C_H
#define IISOBUS_C_H

#include "impl/isobus_c.h"
#include <IsoAgLib/driver/can/icancustomer_c.h>


namespace IsoAgLib {

/**
  Interface-class managing one ISOBUS CAN instance.
  @author Martin Wodok
  @see CanIo_c
*/
class iIsoBus_c : private __IsoAgLib::IsoBus_c {

 public:
  /** Initialize the CAN hardware and all related ISO-protocol modules.
      For possible errors refer to @see CanIo_c
      @param aui8_busNumber number of the physical CAN bus (starting at 0)
    */
  bool init (uint8_t aui8_busNumber) { return IsoBus_c::init (aui8_busNumber); }

  /** Close the CAN hardware and all related ISO-protocol modules.
      For possible errors refer to @see CanIo_c
      @return true if successful, false if already closed
    */
  bool close() { return IsoBus_c::close(); }

  /** @return CAN-Bus Number - 0xFF in case the bus is not initialized yet */
  uint8_t getBusNumber() const { return IsoBus_c::getBusNumber(); }

  iIsoBus_c& operator<< (iCanPkg_c& acrc_src) {
    return static_cast<iIsoBus_c&>(IsoBus_c::operator<<( acrc_src ));
  }

  bool insertStdFilter( iCanCustomer_c& ar_customer,
                        const IsoAgLib::iMaskFilter_c& arc_maskFilter,
                        int ai_dlcForce )
  {
    return IsoBus_c::insertStdFilter(
        ar_customer,
        arc_maskFilter,
        ai_dlcForce );
  }


  bool deleteStdFilter( const iCanCustomer_c& ar_customer, const iMaskFilter_c& arc_maskFilter )
  { return IsoBus_c::deleteStdFilter (ar_customer, arc_maskFilter ); }

  bool existStdFilter(const iCanCustomer_c& ar_customer, const iMaskFilter_c& arc_maskFilter ) {
    return IsoBus_c::existStdFilter (ar_customer, arc_maskFilter);
  }

  #ifdef USE_CAN_MEASURE_BUSLOAD
  /** deliver actual BUS load in baud */
  uint32_t getProcessedThroughput() const {
    return IsoBus_c::getProcessedThroughput();
  }
  #endif

 private:
  /** allow getIisoBusInstance() access to shielded base class.
      otherwise __IsoAgLib::getIsoBusInstance() wouldn't be accepted by compiler
    */
  #if (PRT_INSTANCE_CNT > 1)
  friend iIsoBus_c& getIIsoBusInstance( unsigned int instance );
  #else
  friend iIsoBus_c& getIIsoBusInstance( void );
  #endif
};


/** C-style function, to get access to the unique singleton instance(s) */
#if (PRT_INSTANCE_CNT > 1)
  inline iIsoBus_c& getIIsoBusInstance( unsigned int instance = 0 )
  { return static_cast<iIsoBus_c&>(__IsoAgLib::getIsoBusInstance(instance)); }
#else
  inline iIsoBus_c& getIIsoBusInstance( void )
  { return static_cast<iIsoBus_c&>(__IsoAgLib::getIsoBusInstance()); }
#endif


} // IsoAgLib

#endif // IISOBUS_C_H
