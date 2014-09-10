/*
  baseitem_c.h: base class for member lists; stores access time and
    pointer to root Scheduler_c object

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef BASE_ITEM_H
#define BASE_ITEM_H

/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */
#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/scheduler/impl/scheduler_c.h>
#include "istate_c.h"

// Begin Namespace __IsoAgLib
namespace __IsoAgLib {
/**
  base class for member lists; stores access time
  and pointer to root Scheduler_c object
  Derived indirectly from ClientBase to get management of IsoAgLib instance,
  in case more than one CAN BUS is used for IsoAgLib.
  @author Dipl.-Inform. Achim Spangler
*/
class BaseItem_c  : public IState_c {
public:
  /**
    constructor which takes optional the pointer to the containing Scheduler_c instance
    and the actual time as parameter to initialise all own values
    @param ai32_time optional timestamp to store as last update
    @param ab_status state of this ident (off, claimed address, ...) (default: off)
    @param ai_multitonInst optional key for selection of IsoAgLib instance (default 0)
  */
  BaseItem_c( int32_t ai32_time = 0, IState_c::itemState_t ab_status = IState_c::IstateNull, int ai_multitonInst = 0);

  /**
    copy constructor which takes it initial values from another BaseItem_c instance
    @param acrc_baseItem reference to the source BaseItem_c instance
  */
  BaseItem_c(const BaseItem_c& acrc_baseItem);

  /** destructor which sets the update timestamp to 0 */
  ~BaseItem_c();

  /**
    operator= which defines src as const to avoid
    compile warnings with the automatic generated version
    @param acrc_src reference to source BaseItem_c to copy values from
    @return reference to the source BaseItem_c instance for assign chains like "base1 = base2 = base3;"
  */
   BaseItem_c& operator=(const BaseItem_c& acrc_src);

  /**
    set pointer to containing Scheduler_c instance and update timestamp of object
    @param ai32_time optional timestamp to set as update time
    @param ai_multitonInst optional key for selection of IsoAgLib instance (default 0)
  */
  void set(int32_t ai32_time = -1, int ai_multitonInst = 0);

  /**
    set pointer to containing Scheduler_c instance and update timestamp of object
    @param ai32_time optional timestamp to set as update time
    @param ab_status state of this ident (off, claimed address, ...) (default: off)
    @param ai_multitonInst optional key for selection of IsoAgLib instance (default 0)
  */
  void set(int32_t ai32_time, IState_c::itemState_t ab_status, int ai_multitonInst = 0);

  /**
    calculates time between now and last set of mi32_lastTime;
    if called with time parameter, the difference to this is calculated,
    otherwise the system time is retreived and used
    @return lasted time between last update and the compare time [msec.]
  */
  int32_t lastedTime( void ) const;

  /**
    deliver the timestamp of the last update as int32_t [msec]
    @return last update [msec]
  */
  int32_t lastTime() const {return mi32_lastTime;}

  /**
    updates mi32_lastTime to ai32_time or actual
    system time if no time is given
    @param ai32_time optional time to store as last update time
  */
  void updateTime( int32_t ai32_time = -1 )
    {if ( ai32_time < 0 ) mi32_lastTime = System_c::getTime();
     else mi32_lastTime = ai32_time;
    }

  /**
    check if given time intervall is lasted
    @param aui16_timeInterval time intervall in msec
    <!--@param ai32_time optional timestamp in [msec]-->
    @return true -> time last timestamp older than intervall
  */
  bool checkTime(uint16_t aui16_timeInterval) const;

private:
  /** last system time of access or alive or received message in [250ms]*/
  int32_t mi32_lastTime;
};


}
#endif
