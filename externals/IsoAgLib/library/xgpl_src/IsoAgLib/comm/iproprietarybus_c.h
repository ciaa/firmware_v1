/*
  iproprietarybus.h - Interface access class for the proprietary CAN-busses

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IPROPRIETARYBUS_C_H
#define IPROPRIETARYBUS_C_H

#include <IsoAgLib/driver/can/impl/canio_c.h>
#include <IsoAgLib/driver/can/iident_c.h>
#include <IsoAgLib/driver/can/icancustomer_c.h>
#include <IsoAgLib/driver/can/icanpkg_c.h>
#include <IsoAgLib/driver/can/imaskfilter_c.h>
#include <IsoAgLib/util/impl/singleton.h>

#if 0 < PROP_INSTANCE_CNT
namespace IsoAgLib
{

/**
  @short Class managing a Proprietary CAN bus.
  @author Martin Wodok
  @see CanIo_c
*/
class iProprietaryBus_c {
  MACRO_MULTITON_CONTRIBUTION();
 public:
  /**
    Initialize the CAN hardware
    @param aui8_busNumber number of the CAN bus
    @param aui16_bitrate bitrate
    @return true -> correct initialisation without errors
  */
  inline bool init(
    uint8_t aui8_busNumber,
    unsigned int bitrate )
  { return __IsoAgLib::getCanInstance4Prop().init (aui8_busNumber, bitrate ); }

  /** Close the opened Proprietary CAN-bus */
  inline void close() { return __IsoAgLib::getCanInstance4Prop().close(); }

 #ifdef USE_CAN_MEASURE_BUSLOAD
  /**
    deliver actual BUS load in baud
    @return baudrate in [baud] on used CAN BUS
  */
  int16_t getBusLoad() const { return getCanInstance4Prop().getBusLoad(); }
 #endif

  /**
    deliver the numbers which can be placed at the moment in the send buffer
    @param ren_identType type of searched ident: standard 11bit or extended 29bit
    @return number of msgs which fit into send buffer
  */
  int sendCanFreecnt()
  { return __IsoAgLib::getCanInstance4Prop().sendCanFreecnt(); }

  /**
    test if a FilterBox_c definition already exist
    @param ar_customer reference to the processing class ( the same filter setting can be registered by different consuming classes )
    @param acrc_filterMask individual set of filter and mask
    @return true -> same FilterBox_c already exist
  */
  bool existFilter(
    const IsoAgLib::iCanCustomer_c & ar_customer,
    const IsoAgLib::iMaskFilterType_c& acrc_filterMask)
  { return __IsoAgLib::getCanInstance4Prop().existFilter(ar_customer, acrc_filterMask, NULL); }

  /**
    create a Filter Box with specified at_mask/at_filter on ui8_busNr

    @see IsoAgLib::iCANCustomer
    @param ar_customer reference to IsoAgLib::iCanCustomer_c  which needs
           filtered messages (-> on received msg call ar_customer.processMsg())
    @param acrc_filterMask individual filter mask set
    @return true -> inserting and if wanted reconfiguration are
          performed without errors
  */
  bool insertFilter(
    IsoAgLib::iCanCustomer_c& ar_customer,
    IsoAgLib::iMaskFilterType_c acrc_filterMask,
    int ai_dlcForce )
  {
    return ( __IsoAgLib::getCanInstance4Prop().insertFilter(
        ar_customer,
        acrc_filterMask,
        ai_dlcForce ) != NULL);
  }

  /**
    delete a FilerBox definition
    @param ar_customer reference to the processing class ( the same filter setting can be registered by different consuming classes )
    @param acrc_filterMask individual filter mask combination
    @return true -> FilterBox_c found and deleted
  */
  bool deleteFilter(
    const IsoAgLib::iCanCustomer_c& ar_customer,
    IsoAgLib::iMaskFilterType_c acrc_filterMask)
  {
    return __IsoAgLib::getCanInstance4Prop().deleteFilter(
      ar_customer, acrc_filterMask);
  }


  bool deleteAllFiltersForCustomer (const IsoAgLib::iCanCustomer_c& ar_customer)
  { return __IsoAgLib::getCanInstance4Prop().deleteAllFiltersForCustomer (ar_customer); }


  /**
    function for sending data out of iCANPkg
    if send buffer is full a local loop waits till buffer has enough space
    @see iCanPkg_c
    @param acrc_src iCanPkg_c which holds the to be sent data
    @return reference to this iProprietaryBus_c instance
  */
  iProprietaryBus_c& operator<< (iCanPkg_c& acrc_src)
    { (void) __IsoAgLib::getCanInstance4Prop().operator<< (acrc_src);
      return *this; }

#ifdef USE_CAN_MEASURE_BUSLOAD
  uint32_t getProcessedThroughput() const {
    return getCanInstance4Prop().getBusLoad();
  }
#endif

 private:
  /** allow getIproprietaryBusInstance() access to shielded base class.
      otherwise __IsoAgLib::getProprietaryBusInstance() wouldn't be accepted by compiler
    */
  friend iProprietaryBus_c& getIProprietaryBusInstance( uint8_t aui8_instance );
};


/** C-style function, to get access to the unique singleton instance(s) */
iProprietaryBus_c &getIProprietaryBusInstance( uint8_t aui8_instance = 0 );

} // namespace IsoAgLib

#endif // 0 < PROP_INSTANCE_CNT
#endif // IPROPRIETARYBUS_C_H
