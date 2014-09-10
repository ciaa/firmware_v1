/*
  schedulertask_c.h: header for base class for all IsoAgLib member
    objects which stores the pointer to the IsoAgLib instance, and
    delivers some access features to other components of the system

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef SCHEDULERTASK_C_H
#define SCHEDULERTASK_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/util_funcs.h>
#include <IsoAgLib/driver/system/impl/system_c.h>


namespace __IsoAgLib {

  // All Scheduler-Tasks are subsystems, because of
  // the not-possible multiple-inheritance right now
  class SchedulerTask_c : public Subsystem_c {
    public:
      SchedulerTask_c( int32_t period, bool hardTiming );
      virtual ~SchedulerTask_c() {}

      virtual void timeEvent() = 0;

      int32_t getPeriod() const {
        return m_period;
      }

      void setPeriod( int32_t period, bool retrigger );

      int32_t getTimeToNextTrigger() const {
        return getNextTriggerTime() - System_c::getTime();
      }

      int32_t getNextTriggerTime() const {
        return m_nextTriggerTime;
      }

      void setNextTriggerTime( int32_t time );

      void retriggerNow() {
        setNextTriggerTime( System_c::getTime() );
      }

      bool isRegistered() const {
        return m_registered;
      }

    private:
      friend class Scheduler_c;
      void setRegistered( bool r );
      void timeEventPre();
      void timeEventPost();

      bool m_hardTiming;
      bool m_nextTriggerTimeSet;
      bool m_registered;
      int32_t m_nextTriggerTime;
      int32_t m_period;

#if defined( ISOAGLIB_DEBUG_TIMEEVENT ) || defined( ISOAGLIB_TASK_MAX_TIMEEVENT )
      int32_t m_startTime;
      int32_t m_thisTimeEvent;
      int32_t m_maxTimeEvent;
#endif
  };

} // __IsoAgLib


  /** Proxy for SchedulerTask_c.
    * Having such a proxy as component is an alternative to subclassing
    * SchedulerTask_c directly. OWNER_T simply needs timeEvent implemented
    * NOTE: This is not done as template because of the Tasking compiler
    *       had problems with the template.
    */
#define CLASS_SCHEDULER_TASK_PROXY(OWNER_T)                            \
  class SchedulerTaskProxy_c : public SchedulerTask_c {                \
    public:                                                            \
      SchedulerTaskProxy_c(                                            \
          OWNER_T &art_owner,                                          \
          int32_t period,                                              \
          bool hardTiminig )                                           \
        : SchedulerTask_c( period, hardTiminig )                       \
        , mrt_owner( art_owner )                                       \
      {}                                                               \
                                                                       \
      virtual ~SchedulerTaskProxy_c() {}                               \
                                                                       \
    private:                                                           \
      virtual void timeEvent() {                                       \
        mrt_owner.timeEvent();                                         \
      }                                                                \
                                                                       \
      /* SchedulerTaskProxy_c shall not be copyable. Otherwise the */  \
      /* reference to the containing object would become invalid. */   \
      SchedulerTaskProxy_c( SchedulerTaskProxy_c const & );            \
      SchedulerTaskProxy_c &operator=( SchedulerTaskProxy_c const & ); \
                                                                       \
      OWNER_T &mrt_owner;                                              \
  };                                                                   \


#endif
