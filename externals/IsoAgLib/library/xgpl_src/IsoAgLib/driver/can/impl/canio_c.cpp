/*
  canio_c.cpp: master object for CAN communication; owns array of
    FilterBox_c and MsgObj_c instances

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "canio_c.h"
#include <functional>
#include <algorithm>
#include <IsoAgLib/scheduler/impl/scheduler_c.h>
#include <IsoAgLib/driver/system/impl/system_c.h>
#include <IsoAgLib/driver/can/impl/canpkg_c.h>
#include <IsoAgLib/util/iassert.h>
#include <IsoAgLib/hal/hal_system.h>
#include <IsoAgLib/hal/generic_utils/can/canfifo_c.h>
#include <IsoAgLib/hal/generic_utils/can/canutils.h>

namespace __IsoAgLib {

  static const uint32_t scui8_filter_box_list_update_rate = 100;

  CanIo_c &getCanInstance( unsigned int aui_instance ) {
    MACRO_MULTITON_GET_INSTANCE_BODY( CanIo_c, CAN_INSTANCE_CNT, aui_instance );
  }


  CanIo_c::CanIo_c( void )
    : Subsystem_c(),
      m_arrFilterBox(),
      mi32_maxSendDelay( -1 ),
      mi32_lastProcessedCanPkgTime( 0 ),
      mui_bitrate( 0 ),
      mc_maskStd( 0, Ident_c::StandardIdent ),
      mc_maskExt( 0, Ident_c::ExtendedIdent ),
      mui8_busNumber( 0xFF )
  {}


  bool
  CanIo_c::init( uint8_t aui8_busNumber, unsigned int bitrate ) {
    isoaglib_assert( ! initialized() );
    mui8_busNumber = aui8_busNumber;
    mui_bitrate = bitrate;
    mi32_lastProcessedCanPkgTime = 0;

    const bool r = HAL::canInit( mui8_busNumber, mui_bitrate );
    isoaglib_assert( r );

    if( r ) {
      setInitialized();
    }
    return r;
  }


  void
  CanIo_c::close() {
    isoaglib_assert( initialized() );

    const bool r = HAL::canClose( mui8_busNumber );
    isoaglib_assert( r );
    ( void )r;

    m_arrFilterBox.clear();

    setClosed();
  }


#ifdef USE_CAN_MEASURE_BUSLOAD
  /** deliver actual BUS load in baud */
  uint32_t CanIo_c::getProcessedThroughput() const {
    isoaglib_assert( initialized() );
    return HAL::canBusLoads[ mui8_busNumber ].getBusLoad();
  }
#endif


  bool
  CanIo_c::existFilter(
    const __IsoAgLib::CanCustomer_c& ar_customer,
    const IsoAgLib::iMaskFilterType_c& arc_filterpair,
    ArrFilterBox::iterator* apc_iter ) {
    // check if filter/t_mask are already inserted
    // return false if this setting isn't unique
    bool b_identDefFound = false;

    // iterator for quick access to all array elements
    ArrFilterBox::iterator pc_iter = m_arrFilterBox.begin();

    // check if given FilterBox_c definition is not yet in array
    for ( ; pc_iter != m_arrFilterBox.end(); ++pc_iter ) {
      if ( ( (*pc_iter)->equalFilterMask( arc_filterpair ) )
           && ( (*pc_iter)->equalCustomer( ar_customer ) ) ) {
        // FilterBox_c with equal def found
        //-> don't insert complete ident def two times
        b_identDefFound = true;
        // don't search rest of array
        break;
      }
    }
    if ( b_identDefFound && ( apc_iter != NULL ) ) *apc_iter = pc_iter;
    return b_identDefFound;
  }


  FilterBox_c *
  CanIo_c::insertFilter(
    __IsoAgLib::CanCustomer_c & ar_customer,
    const IsoAgLib::iMaskFilterType_c& arc_filterpair,
    int ai_dlcForce ) {

    isoaglib_assert( initialized() );

    if( existFilter( ar_customer, arc_filterpair, NULL ) )
      return NULL;

    //look for existing mask/filter definition
    FilterBox_c* tempFilterBox_c = getFilterBox( arc_filterpair );

    if ( tempFilterBox_c != NULL ) {
      // filterbox with mask/filter found
      tempFilterBox_c->insertCustomer( &ar_customer, ai_dlcForce );
      //do not insert new filterbox because it already exists
      return tempFilterBox_c;
    }

    FilterBox_c* newFb = new FilterBox_c();
    newFb->set( arc_filterpair, &ar_customer, ai_dlcForce );
    HAL::defineRxFilter( getBusNumber(), arc_filterpair.getType() == IsoAgLib::iIdent_c::ExtendedIdent,
        arc_filterpair.getFilter(), arc_filterpair.getMask() );
    m_arrFilterBox.push_back( newFb );
    return m_arrFilterBox.back();
  }


  bool
  CanIo_c::deleteFilter(
    const __IsoAgLib::CanCustomer_c& ar_customer,
    const IsoAgLib::iMaskFilterType_c& arc_filterpair ) {
    bool b_result = false;
 

    // iterator for quick access to all array elements
    ArrFilterBox::iterator pc_iter;

    if( existFilter( ar_customer, arc_filterpair, &pc_iter ) ) {
      // filter found -> delete element where pc_iter points to
      if ( (*pc_iter)->deleteFilter( ar_customer ) ) {
        //no more cancustomer exist for the filterbox -> delete
        delete *pc_iter;
        m_arrFilterBox.erase( pc_iter );
        HAL::deleteRxFilter( getBusNumber(), arc_filterpair.getType() == IsoAgLib::iIdent_c::ExtendedIdent,
            arc_filterpair.getFilter(), arc_filterpair.getMask() );
       }
       b_result = true;
    }

    return b_result;
  }


  bool
  CanIo_c::deleteAllFiltersForCustomer( const __IsoAgLib::CanCustomer_c & ar_customer ) {
    bool b_result = false;

    for ( ArrFilterBox::iterator pc_iter = m_arrFilterBox.begin(); pc_iter != m_arrFilterBox.end(); ) {
      // make copy of filter pair because deleteFilter may clear FilterBox_c::mc_maskFilterPair
      const IsoAgLib::iMaskFilterType_c filterpair = (*pc_iter)->maskFilterPair();
      if( (*pc_iter)->deleteFilter( ar_customer ) ) {
        //no more cancustomer exist for the filterbox -> delete
        delete *pc_iter;
        pc_iter = m_arrFilterBox.erase( pc_iter );
        HAL::deleteRxFilter( getBusNumber(), filterpair.getType() == IsoAgLib::iIdent_c::ExtendedIdent,
            filterpair.getFilter(), filterpair.getMask() );
        b_result = true;
      } else {
        ++pc_iter;
      }
    }

    return b_result;
  }


  void 
  CanIo_c::processMsg( bool ) {

    if( initialized() ) { 

      HAL::canRxPoll( mui8_busNumber );

      while( ! HAL::CanFifos_c::get(mui8_busNumber).empty() ) {
        CanPkg_c& pkg = HAL::CanFifos_c::get( mui8_busNumber).front();

        CanIo_c::ArrFilterBox::iterator pc_iFilterBox;
        if( canMsg2FilterBox( pkg.ident(), pkg.identType(), pc_iFilterBox ) ) {
#ifndef NO_FILTERBOX_LIST_ORDER_SWAP
          if( (*pc_iFilterBox)->getMatchCount() > scui8_filter_box_list_update_rate ) {
            // reorder Filter box
            (*pc_iFilterBox)->resetMatchCount();

            if( pc_iFilterBox != m_arrFilterBox.begin() ) {
              CanIo_c::ArrFilterBox::iterator pc_iFilterBoxPrev = pc_iFilterBox;
              --pc_iFilterBox;
              STL_NAMESPACE::iter_swap( pc_iFilterBoxPrev, pc_iFilterBox );
            }
          }
#endif
          (*pc_iFilterBox)->processMsg( pkg );
        }

        HAL::CanFifos_c::get( mui8_busNumber).pop();
      }
    }
  }


  CanIo_c&
  CanIo_c::operator<<( CanPkg_c& acrc_src ) {

    isoaglib_assert ( initialized() );

    int fc = sendCanFreecnt();

    /*  -1 indicates that the used CAN HAL implementation does not support 
     *  a send queue and no information about the possible about of frames 
     *  is available - so we just try to send and check the result */
    if( -1 != fc ) {
      const int32_t now = System_c::getTime();
      while( fc < 1 ) {
        HAL::sleep_max_ms( 1 );
        /** we wait for CONFIG_CAN_BLOCK_TIME ms if the send-queue is opening for this one package.
          * if we cannot send out the message within CONFIG_CAN_BLOCK_TIME ms we assume that the
          * bus is in an error situation (bus-off or bus-passive) and no messages can be sent out.
          * In that case we clear the send buffer and try to insert the most recent message.
          **/
        if ( ( System_c::getTime() - now ) > CONFIG_CAN_BLOCK_TIME ) {
          IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::HalCanBusOverflow, getMultitonInst() );
          break;
        }
        fc = sendCanFreecnt();
      }
    }

    if( ! HAL::canTxSend( mui8_busNumber, acrc_src ) ) {
      IsoAgLib::getILibErrInstance().registerNonFatal( IsoAgLib::iLibErr_c::HalCanBusOverflow, getMultitonInst() );
    }

    return *this;
  }


  bool
  CanIo_c::canMsg2FilterBox(
    uint32_t aui32_ident,
    Ident_c::identType_t at_type,
    ArrFilterBox::iterator& ar_arrFilterBoxIter ) {

    ar_arrFilterBoxIter = m_arrFilterBox.begin();
    
    const IsoAgLib::iIdent_c ident(aui32_ident, at_type);
    while ( ar_arrFilterBoxIter != m_arrFilterBox.end() ) {
      if ( (*ar_arrFilterBoxIter)->maskFilterPair().matchMsgId( ident ) ) {
        // matching FilterBox_c found
        return true;
      }
      ++ar_arrFilterBoxIter;
    }
    // if execution reaches this point, no matching FilterBox_c found
    return false;
  }


  FilterBox_c*
  CanIo_c::getFilterBox( const IsoAgLib::iMaskFilterType_c& arc_maskFilter ) const {
    for( ArrFilterBox::const_iterator i = m_arrFilterBox.begin(); i != m_arrFilterBox.end(); ++i ) {
      if( (*i)->equalFilterMask( arc_maskFilter ) )
        return *i;
    }
    return NULL;
  }

} // __IsoAgLib
