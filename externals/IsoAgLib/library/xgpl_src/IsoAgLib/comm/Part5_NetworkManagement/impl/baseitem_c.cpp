/*
  baseitem_c.cpp - base class for member lists; stores access time and
    pointer to root Scheduler_c object

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "baseitem_c.h"
#include <IsoAgLib/scheduler/impl/scheduler_c.h>

namespace __IsoAgLib {

/**
  constructor which takes optional the pointer to the containing Scheduler_c instance
  and the actual time as parameter to initialise all own values
  @param ai32_time optional timestamp to store as last update
  @param ab_status state of this ident (off, claimed address, ...) (default: off)
  @param ai_multitonInst optional key for selection of IsoAgLib instance (default 0)
*/
BaseItem_c::BaseItem_c( int32_t ai32_time, IState_c::itemState_t ab_status, int ai_multitonInst)
  : IState_c(ab_status, ai_multitonInst), mi32_lastTime(ai32_time)
{}

/**
  copy constructor which takes it initial values from another BaseItem_c instance
  @param acrc_baseItem reference to the source BaseItem_c instance
*/
BaseItem_c::BaseItem_c(const BaseItem_c& acrc_baseItem)
: IState_c(acrc_baseItem), mi32_lastTime(acrc_baseItem.mi32_lastTime)
{}

/** destructor which sets the update timestamp to 0 */
BaseItem_c::~BaseItem_c(){
  mi32_lastTime = 0;
}

/**
  set pointer to containing Scheduler_c instance and update timestamp of object
  @param ai32_time optional timestamp to set as update time
  @param ai_multitonInst optional key for selection of IsoAgLib instance (default 0)
*/
void BaseItem_c::set(int32_t ai32_time, int ai_multitonInst)
{
  if (ai32_time >= 0) mi32_lastTime = ai32_time;
  /** ai_multitonInst==-1 is special value to indicate, that the ai_multitonInst should not be changed */
  if (ai_multitonInst != -1) ClientBase::setMultitonInst(ai_multitonInst);
}

/**
  set pointer to containing Scheduler_c instance and update timestamp of object
  @param ai32_time optional timestamp to set as update time
  @param ab_status state of this ident (off, claimed address, ...) (default: off)
  @param ai_multitonInst optional key for selection of IsoAgLib instance (default 0)
*/
void BaseItem_c::set(int32_t ai32_time, IState_c::itemState_t ab_status, int ai_multitonInst)
{
  set(  ai32_time,ai_multitonInst);
  // force clear of old item state
  setItemState( ab_status, true );
}

/** operator= which defines src as const to avoid
compile warnings with the automatic generated version */
BaseItem_c& BaseItem_c::operator=(const BaseItem_c& src){
  mi32_lastTime = src.mi32_lastTime;
  return *this;
}

/**
  check if given time intervall is lasted
  INFO: Interval is only an uint16. That's because of SPEED reasons
        for 16-bit platforms and of the not existing need to check
        for more than a minute and 5 seconds (65535 msec)
  @param aui16_timeInterval time intervall in msec
  @return true -> time last timestamp older than intervall
*/
bool BaseItem_c::checkTime(uint16_t aui16_timeInterval) const  {
  return ( ( System_c::getTime() - mi32_lastTime ) >= aui16_timeInterval );
}

} // end of namespace __IsoAgLib
