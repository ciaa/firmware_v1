/*
  canio_c.h: master object for CAN communication; owns array of
    FilterBox_c and MsgObj_c instances

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef CAN_IO_H
#define CAN_IO_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/iliberr_c.h>
#include <IsoAgLib/util/impl/util_funcs.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/hal/hal_can.h>
#include <IsoAgLib/hal/hal_system.h>
#include "ident_c.h"
#include "filterbox_c.h"

#include <list>

namespace __IsoAgLib {

  class CanIo_c : public Subsystem_c {
      MACRO_MULTITON_CONTRIBUTION();
    protected:
      typedef STL_NAMESPACE::list<FilterBox_c*> ArrFilterBox;

    public:

      bool init( uint8_t bus, unsigned int bitrate );

      /** every subsystem of IsoAgLib has explicit function for controlled shutdown */
      void close( void );

      ~CanIo_c() {}

      /** provide BUS number */
      uint8_t getBusNumber( void ) const {
        return mui8_busNumber;
      }

#ifdef USE_CAN_MEASURE_BUSLOAD
      /** deliver actual BUS load in baud */
      uint32_t getProcessedThroughput() const;
#endif

      /** wait until specified timeout or until next CAN message receive
       *  @return true -> there are CAN messages waiting for process. else: return due to timeout
       */
      static bool waitUntilCanReceiveOrTimeout( int32_t ai32_timeoutInterval ) {
        return HAL::canRxWait( ai32_timeoutInterval );
      }

      /** deliver the numbers which can be placed at the moment in the send buffer
        @return number of msgs which fit into send buffer
      */
      int sendCanFreecnt() {
        return HAL::canTxQueueFree( mui8_busNumber );
      }

      /** test if a FilterBox_c definition already exist
        (version expecial for extended ident, chosen at compile time)
        @param ar_customer reference to the processing class ( the same filter setting can be registered by different consuming classes )
        @param arc_filterpair filter mask type set
        @param apc_iter optional pointer Iterator to result FilterBox
        @return true -> same FilterBox_c already exist
      */
      bool existFilter( const __IsoAgLib::CanCustomer_c& ar_customer,
                        const IsoAgLib::iMaskFilterType_c& arc_filterpair,
                        ArrFilterBox::iterator* apc_iter = NULL );

      FilterBox_c* insertFilter( __IsoAgLib::CanCustomer_c& ar_customer,
                                 const IsoAgLib::iMaskFilterType_c& arc_filterpair,
                                 int ai_dlcForce );

      /** helper function to search all FilterBoxes for matching
        instance which maps to received CAN messages
        @param aui32_ident Ident of received CAN message
        @return is another filterbox found?
      */
      bool canMsg2FilterBox( uint32_t aui32_ident, Ident_c::identType_t at_type, ArrFilterBox::iterator& ar_arrFilterBoxIter );

      /** delete a FilerBox definition
        @param ar_customer reference to the processing class ( the same filter setting can be registered by different consuming classes )
        @param arc_filterpair filter mask combination
        @return true -> FilterBox_c found and deleted
      */
      bool deleteFilter( const __IsoAgLib::CanCustomer_c& ar_customer, const IsoAgLib::iMaskFilterType_c& arc_filterpair );

      bool deleteAllFiltersForCustomer ( const __IsoAgLib::CanCustomer_c& ar_customer );


      /** initiate processing of all received msg */
      void processMsg( bool b_break );

      /** function for sending data out of CanPkg_c */
      CanIo_c& operator<<( CanPkg_c& acrc_src );

      /** return time stamp of the last can package that has been received and processed successfully */
      int32_t getLastProcessedCanPkgTime() const {
        return mi32_lastProcessedCanPkgTime;
      }

    protected:
      /** evaluate common bits of all defined filterBox
         instances and set it in mask -> build up global mask
       */
      void getCommonFilterMask();

    private:
      CanIo_c( void );

      /** search for a FilterBox with given mask and filter
          @param arc_filterpair filter mask combination
          @return located FilterBox or NULL if not exist
        */
      FilterBox_c* getFilterBox( const IsoAgLib::iMaskFilterType_c& arc_maskFilter ) const;

      /** Vector of configured filter boxes */
      ArrFilterBox m_arrFilterBox;

      /** maximum send delay - value of < 0 indicates that no send-delay check is requested*/
      int32_t mi32_maxSendDelay;

      /**  timestamp of last received and and processed CAN package  */
      int32_t mi32_lastProcessedCanPkgTime;

      /** configured bitrate */
      unsigned int mui_bitrate;

      /** global mask with standard 11bit type */
      Ident_c mc_maskStd;
      /** global mask with extended 29bit type */
      Ident_c mc_maskExt;

      uint8_t mui8_busNumber;

      /** flag to avoid loop of CAN message processing, when timeEvent() is called during previous
       *  timeEvent triggered CAN processing -> when this flag is true, no further processing is performed
       */
      bool mb_runningCanProcess;

      friend CanIo_c &getCanInstance( unsigned int aui_instance );
  };


  /** C-style function, to get access to the unique CanIo_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  CanIo_c &getCanInstance( unsigned int aui_instance = 0 );

}

#endif
