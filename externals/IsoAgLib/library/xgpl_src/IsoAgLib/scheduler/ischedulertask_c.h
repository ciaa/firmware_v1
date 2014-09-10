/*
  ischedulertask_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISCHEDULERTASK_H
#define ISCHEDULERTASK_H

#include "impl/schedulertask_c.h"

namespace IsoAgLib {

  class iSchedulerTask_c : private __IsoAgLib::SchedulerTask_c {
    public:
      iSchedulerTask_c( int32_t period, bool hardTiming ) : SchedulerTask_c( period, hardTiming ) {}
      virtual ~iSchedulerTask_c() {}

      virtual void timeEvent() = 0;

      int32_t getPeriod() const {
        return SchedulerTask_c::getPeriod();
      }

      void setPeriod ( int32_t a_period, bool retrigger ) {
        SchedulerTask_c::setPeriod( a_period, retrigger );
      }

      int32_t getTimeToNextTrigger() const {
        return SchedulerTask_c::getTimeToNextTrigger();
      }

      int32_t getNextTriggerTime() const {
        return SchedulerTask_c::getNextTriggerTime();
      }

      void setNextTriggerTime( int32_t time ) {
        SchedulerTask_c::setNextTriggerTime( time );
      }

      void retriggerNow() {
        SchedulerTask_c::retriggerNow();
      }

      bool isRegistered() const {
        return SchedulerTask_c::isRegistered();
      }

    private:
      friend class iScheduler_c;
  };

}

#endif
