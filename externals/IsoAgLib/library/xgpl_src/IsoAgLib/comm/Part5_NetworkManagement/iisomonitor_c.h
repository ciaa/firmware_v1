/*
  iisomonitor_c.h: object for monitoring members (list of iIsoItem_c)

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IISO_MONITOR_H
#define IISO_MONITOR_H

#include "impl/isomonitor_c.h"
#include "iidentitem_c.h"
#include <IsoAgLib/comm/Part5_NetworkManagement/icontrolfunctionstatehandler_c.h>


namespace IsoAgLib {


/** this object manages a monitor list of all ISO members
    including inserting and administration of local own members.
  @author Martin Wodok
  @author Dipl.-Inform. Achim Spangler
*/
class iIsoMonitor_c : private __IsoAgLib::IsoMonitor_c
{
public:
  /** register a properly set-up IdentItem.
      @return false in case the IdentItem wasn't properly activated */
  bool registerIdentItem( iIdentItem_c& arc_item ) { return IsoMonitor_c::registerIdentItem( arc_item ); }

  /** deregister a registered IdentItem
      @param arc_item The (registered) IdentItem */
  void deregisterIdentItem( iIdentItem_c& arc_item ) { IsoMonitor_c::deregisterIdentItem( arc_item ); }

  /** deliver amount of ISO members in monitor list which optionally
    match the condition of address claim state
    @param ab_forceClaimedAddress true -> only members with claimed address are used
    @return amount of ISO members with claimed address
  */
  uint8_t isoMemberCnt( bool forceClaimedAddress = false )
  { return IsoMonitor_c::isoMemberCnt( forceClaimedAddress ); }

  /** deliver the n'th ISO member in monitor list which optionally match the condition of
    address claim state check first with isoMemberCnt if enough members are registered in Monitor-List
    @see isoMemberCnt
    @param aui8_ind index of the wanted member in the sublist of members
                    (first index == 0) with the wanted property
    @param ab_forceClaimedAddress true -> only members with claimed address are used
    @return reference to searched element
  */
  iIsoItem_c& isoMemberInd( uint8_t index, bool forceClaimedAddress = false )
  { return static_cast<iIsoItem_c&>( IsoMonitor_c::isoMemberInd( index, forceClaimedAddress ) ); }

  iIsoItem_c* item( const iIsoName_c& name, bool forceClaimedAddress = false )
  { return static_cast<iIsoItem_c*>( IsoMonitor_c::item( name, forceClaimedAddress ) ); }

  void registerControlFunctionStateHandler(iControlFunctionStateHandler_c &arc_handler) {
    IsoMonitor_c::registerControlFunctionStateHandler( arc_handler );
  }

  void deregisterControlFunctionStateHandler(iControlFunctionStateHandler_c &arc_handler) {
    IsoMonitor_c::deregisterControlFunctionStateHandler( arc_handler );
  }

#ifdef USE_WORKING_SET
  iIsoItem_c* getMaster( iIsoItem_c& isoitem )
  { return static_cast<iIsoItem_c *>( IsoMonitor_c::getMaster( isoitem ) ); }
#endif

private:
  /** allow getIisoMonitorInstance() access to shielded base class.
      otherwise __IsoAgLib::getIsoMonitorInstance() wouldn't be accepted by compiler
    */
  #if defined( PRT_INSTANCE_CNT ) && ( PRT_INSTANCE_CNT > 1 )
  friend iIsoMonitor_c& getIisoMonitorInstance( uint8_t aui8_instance );
  #else
  friend iIsoMonitor_c& getIisoMonitorInstance( void );
  #endif

  /** HIDDEN constructor for a iIsoMonitor_c object instance
    NEVER instantiate a variable of type iIsoMonitor_c within application
    only access iIsoMonitor_c via getIisoInstance() or getIisoInstance( int riLbsBusNr ) in case more than one ISO11783 BUS is used for IsoAgLib
    */
  iIsoMonitor_c() : IsoMonitor_c() {}
};

#if defined( PRT_INSTANCE_CNT ) && ( PRT_INSTANCE_CNT > 1 )
  /** C-style function, to get access to the unique IsoMonitor_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  inline iIsoMonitor_c& getIisoMonitorInstance( uint8_t aui8_instance = 0 )
  { return static_cast<iIsoMonitor_c&>(__IsoAgLib::getIsoMonitorInstance(aui8_instance));}
#else
  /** C-style function, to get access to the unique IsoMonitor_c singleton instance */
  inline iIsoMonitor_c& getIisoMonitorInstance( void )
  { return static_cast<iIsoMonitor_c&>(__IsoAgLib::getIsoMonitorInstance());}
#endif


}
#endif
