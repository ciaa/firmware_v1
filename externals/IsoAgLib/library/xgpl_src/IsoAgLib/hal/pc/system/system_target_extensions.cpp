/*
  system_target_extensions.cpp: source for PC specific extensions for
    the HAL for central system

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/** \file hal/pc/system/system_target_extensions.cpp
 * A module targetExtensions should be used
 * for all methods, which can't be simply
 * mapped from ECU standard BIOS to the needs of
 * IsoAgLib by mostly renaming and reordering of functions, parameters
 * and types in <i>\<target\>/\<device\>/\<device\>.h</i> .
 * ********************************************************** */

#include "system_target_extensions.h"
#include <IsoAgLib/util/iassert.h>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <time.h>
#include <iostream>
#include <cstdlib>

#ifdef WIN32
  #ifndef WINCE
    #include <conio.h>
  #endif
  #include <windows.h>
  #include <MMSYSTEM.H>
#else
  #include <fcntl.h>
  #include <sys/time.h>
  #include <sys/times.h>
  #include <unistd.h>
  #include <linux/version.h>
#endif


bool
HALSimulator_c::getOn_offSwitch()
{
  #if 0
    // old style execution stop detection when
    // application shoul stop if all CAN messages of
    // FILE based CAN simulation were processed
    return (getTime() - can_lastReceiveTime() < 1000);
  #elif 0
    // exit function if key typed
    uint8_t b_temp;
    return ( KeyGetByte( &b_temp ) !=1 );
  #else
    // use std C++ cin function to check for unprocessed input
    // -> as soon as RETURN is hit, the programm stops
    return ( STL_NAMESPACE::cin.rdbuf()->in_avail() <= 0 );
  #endif
}


namespace __HAL {


extern bool canStartDriver();
extern bool canStopDriver();


static bool s_systemStarted = false;

static HALSimulator_c* g_halSimulator = NULL;

HALSimulator_c &halSimulator() { isoaglib_assert( g_halSimulator ); return *g_halSimulator; }
void setHalSimulator( HALSimulator_c* sim ) { g_halSimulator = sim; }



#ifndef WIN32
/** define the amount of MSec per Clock_t, in case the project config didn't this before */
#ifndef msecPerClock
#define msecPerClock 10LL
#endif

#ifndef LINUX_VERSION_CODE
#error "LINUX_VERSION_CODE is not defined"
#endif

int32_t getStartupTime()
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
  if ( msecPerClock != (1000 / sysconf(_SC_CLK_TCK)) )
  { // BIG PROBLEM -> THE DEFINE DURING COMPILE TIME DOES NOT MATCH THE RUNTIME
    INTERNAL_DEBUG_DEVICE << "\n\nVERY BIG PROBLEM!!!\nThis program was compiled with\n#define msecPerClock " << msecPerClock
        << "\nwhile the runtime system has " << (1000 / sysconf(_SC_CLK_TCK))
        << "\n\nSO PLEASE add\n#define msecPerClock " << (1000 / sysconf(_SC_CLK_TCK))
        << "\nto your project configuration header or Makefile, so that a matching binary is built. This program is aborted now, as none of any time calculations will match with this problem.\n\n"
        << INTERNAL_DEBUG_DEVICE_ENDL;
    MACRO_ISOAGLIB_ABORT();
  }
  static int32_t st_startup4Times = int32_t (times(NULL));
#else
  static bool sb_init_done = false;
  static int32_t st_startup4Times = int32_t (-1);
  if (!sb_init_done)
  {
    sb_init_done = true;
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    st_startup4Times = int32_t(ts.tv_sec)*1000 + int32_t(ts.tv_nsec/1000000);
  }
#endif
  return st_startup4Times;
}
#else // WIN32
  int32_t getStartupTime()
  { // returns time in msec
    // VC++ and mingw with native Win32 API provides very accurate
    // msec timer - use that
    // in case of mingw compiler error link winmm.lib (add -lwinmm).
    static int32_t st_startup4Times = MACRO_ISOAGLIB_TIMEGETTIME();
    return st_startup4Times;
  }
#endif


void 
openSystem()
{
  // check if system is opened before user set the halSimulator?
  if (g_halSimulator == NULL)
  { // use default halSimulator then!
    static HALSimulator_c dummyDefaultHalSimulator;
    setHalSimulator( &dummyDefaultHalSimulator );
  }

  // init system start time (first call sets the start-time-base)
  (void)getTime();

  s_systemStarted = canStartDriver();
  if( !s_systemStarted )
    MACRO_ISOAGLIB_ABORT();
}


void 
closeSystem()
{
  s_systemStarted = !canStopDriver();
  if( s_systemStarted )
    MACRO_ISOAGLIB_ABORT();
}


bool
isSystemOpened()
{
  return s_systemStarted;
}


#ifdef WIN32
  // VC++ and mingw with native Win32 API provides very accurate
  // msec timer - use that
  int32_t getTime()
  { // returns time in msec
    // in case of mingw compiler error link winmm.lib (add -lwinmm).
    return MACRO_ISOAGLIB_TIMEGETTIME() - getStartupTime();
  }
#else
 // use gettimeofday for native LINUX system
int32_t getTime()
{
  /** linux-2.6 */
  static timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  const int32_t ci_now = int32_t(ts.tv_sec)*1000 + int32_t(ts.tv_nsec/1000000);
  return ci_now - getStartupTime();
}
#endif


int16_t
getSnr(uint8_t *snrDat)
{
  snrDat[0] = 0x99;
  snrDat[1] = 0x12;
  snrDat[2] = 0x34;
  snrDat[3] = 0x56;
  snrDat[4] = 0x00;
  snrDat[5] = 0x01;

  return HAL_NO_ERR;
}


int16_t
getCpuFreq()
{
  return 20;
}


void
powerHold( bool ab_on )
{
  halSimulator().powerHold( ab_on );
}


bool
getOn_offSwitch()
{
	return halSimulator().getOn_offSwitch();
}


int16_t
KeyGetByte(uint8_t *p)
{
  #ifndef WIN32
    // fcntl( 0,
    return read(0, p, sizeof *p) == sizeof *p;
  #else
    #ifdef WINCE
      return 0;  /// @todo WINCE-176 add key handling for WINCE
    #else
      if (!_kbhit()) return 0;
      *p = _getch();
    #endif
    return 1;
  #endif
}


void
sleep_max_ms( uint32_t ms )
{
#ifdef WIN32
  Sleep( ms );
#else
  usleep ( ms * 1000 );
#endif
}


} // __HAL
