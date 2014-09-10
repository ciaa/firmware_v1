/*
  counteri_c.h - header file for CounterI_c

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef COUNTERI_C_H
#define COUNTERI_C_H

#include "inputbase_c.h"
#include <IsoAgLib/util/iassert.h>


namespace __IsoAgLib {
/**
  Input object for counting of digital impulses
  @author Dipl.-Inform. Achim Spangler
  */
class CounterI_c : public InputBase_c
{
public:
  /**
    constructor for a new digital input channel which performs configuration of hardware
    @param ab_channel hardware channel for this input
    @param aui16_timebase default-argument for setting the timebase which should be
            greater than max time distance between signals and should be small
            enough to avoid overflow of signals in one timebase
    @param ab_activHigh true -> counter input is configured fo ACTIV_HIGH; else ACTIV_LOW
    @param ab_risingEdge true -> counter triggers on rising edge; else on falling edge
  */
  inline CounterI_c(
    uint8_t ab_channel = 0xFF,
    uint16_t aui16_timebase = 0,
    bool ab_activHigh = true,
    bool ab_risingEdge = true );

  virtual ~CounterI_c() {}

  /**
    configuration of hardware
    @param ab_channel hardware channel for this input
    @param aui16_timebase default-argument for setting the timebase which should be
            greater than max time distance between signals and should be small
            enough to avoid overflow of signals in one timebase
    @param ab_activHigh true -> counter input is configured fo ACTIV_HIGH; else ACTIV_LOW
    @param ab_risingEdge true -> counter triggers on rising edge; else on falling edge
  */
  inline void init(
    uint8_t ab_channel,
    uint16_t aui16_timebase = 0,
    bool ab_activHigh = true,
    bool ab_risingEdge = true );

  /**
    check for the input value
    @return amount of signals since initialisation or last reset
  */
  inline uint32_t val() const;

  /**
    check for the input value to be > 0
  */
  inline bool active() const;

  /**
    reset the given counter
  */
  inline void reset();

  /**
    get period of counter channel
    @return time between last two signals in microseconds
            or 0xFFFFFFFF if time is longer than initially given timebase
  */
  inline uint32_t period_us();

  /**
    get frequency of counter channel
    @return frequency calculated from time between last two signals
            or 0 if time is longer than initially given timebase
  */
  inline uint32_t frequency();

  /**
   get time since last signal
   @return time since last signal [msec.]
  */
  inline uint32_t lastSignalAge();

private:
  // unimplemented, not copyable
  CounterI_c(const CounterI_c&);
  CounterI_c& operator=(const CounterI_c&);
};


inline
CounterI_c::CounterI_c(
    uint8_t ab_channel,
    uint16_t aui16_timebase,
    bool ab_activHigh,
    bool ab_risingEdge)
  : InputBase_c( ab_channel, IsoAgLib::iInput_c::counter )
{
  if ( ab_channel != 0xFF )
    init( ab_channel, aui16_timebase, ab_activHigh, ab_risingEdge );
}


inline
bool
CounterI_c::active() const
{
  return (val() > 0);
}


inline
void
CounterI_c::init(uint8_t ab_channel, uint16_t aui16_timebase, bool ab_activHigh, bool ab_risingEdge )
{
  InputBase_c::init(ab_channel, IsoAgLib::iInput_c::counter);
  const bool r = ( HAL::init_counter(channelNr(), aui16_timebase, ab_activHigh, ab_risingEdge) != HAL_RANGE_ERR);
  isoaglib_header_assert( r ); ( void )r;
}


inline
uint32_t
CounterI_c::val() const
{
  return HAL::getCounter(channelNr());
}


inline
void
CounterI_c::reset()
{
  HAL::resetCounter(channelNr());
}


inline
uint32_t
CounterI_c::period_us()
{
  return HAL::getCounterPeriod_us(channelNr());
}


inline
uint32_t
CounterI_c::frequency()
{
  return HAL::getCounterFrequency(channelNr());
}


inline
uint32_t
CounterI_c::lastSignalAge()
{
  return HAL::getCounterLastSignalAge(channelNr());
}


} // __IsoAgLib

#endif
