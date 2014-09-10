/*
  inputs_target_extensions_hal_simulator.cpp:
    source for HAL simulator for inputs. This file was based on
    inputs_target_extensions.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
/** \file
 * 
 * A module biosExtenders should be used
 * for all methods, which can't be simply
 * mapped from ECU standard BIOS to the needs of
 * IsoAgLib by mostly renaming and reordering of functions, parameters
 * and types in map_bios_namespace.h .
 * ********************************************************** */

#include "inputs_target_extensions.h"

#include <stdlib.h>
#include <cstring>
#include <cstdio>

#include <IsoAgLib/hal/hal_system.h>


namespace __HAL {

int16_t
init_digin(uint8_t bInput,uint8_t bMode,uint8_t bAktivhighlow,void (*pfFunctionName)(...))
{
  if( bInput == 0xFF )
    return HAL_RANGE_ERR;

  halSimulator().init_digin( bInput, bMode, bAktivhighlow, pfFunctionName );
  return HAL_NO_ERR;
}


int16_t
getDiginOnoff(uint8_t bInputNumber)
{
  if( bInputNumber != 0xFF )
    return halSimulator().getDiginOnoff( bInputNumber );
  else
    return HAL_RANGE_ERR;
}


int16_t
getDiginOnoffStatic(uint8_t bInputNumber)
{
  return getDiginOnoff(bInputNumber);
}


int16_t
setDiginPrescaler(uint8_t /*bGroup*/, uint8_t /*bMode*/)
{
  return HAL_NO_ERR;
}


int16_t
getDiginFreq(uint8_t bInput, uint32_t *pwFrequency)
{
  if( bInput == 0xFF )
    return HAL_RANGE_ERR;

  halSimulator().getDiginFreq( bInput, pwFrequency );
  return HAL_NO_ERR;
}


int16_t
getAdc(uint8_t bKanalnummer)
{
  if( bKanalnummer != 0xFF )
    return halSimulator().getAdc( bKanalnummer );
  else
    return HAL_RANGE_ERR;
}


int16_t
init_analogin(uint8_t bNumber, uint8_t bType)
{
  if( bNumber == 0xFF )
    return HAL_RANGE_ERR;

  halSimulator().init_analogin( bNumber, bType );
  return HAL_NO_ERR;
}


void
setFastAnalogin(boolean /*bMode*/)
{
}


int16_t
getAnaloginMean(uint8_t bInput)
{
  if( bInput != 0xFF )
    return getAdc( bInput );
  else
    return HAL_RANGE_ERR;
}


int16_t
init_counter(uint8_t ab_channel, uint16_t aui16_timebase, boolean ab_activHigh, boolean ab_risingEdge)
{
  if( ab_channel == 0xFF )
    return HAL_RANGE_ERR;

  halSimulator().init_counter(ab_channel, aui16_timebase, ab_activHigh, ab_risingEdge);
  return HAL_NO_ERR;
}


uint32_t
getCounter(uint8_t ab_channel)
{
  /* check if ab_channel is allowed and var array is allocated */
  return halSimulator().getCounter( ab_channel );
}


int16_t
resetCounter(uint8_t ab_channel)
{
  if( ab_channel != 0xFF )
    return halSimulator().resetCounter( ab_channel );
  else
    return HAL_RANGE_ERR;
}


uint32_t
getCounterPeriod_us(uint8_t ab_channel)
{
  return halSimulator().getCounterPeriod_us(ab_channel);
}


uint32_t
getCounterFrequency(uint8_t ab_channel)
{
  uint32_t pwFrequency;

  getDiginFreq(ab_channel, &pwFrequency);
  return pwFrequency;
}


uint16_t
getCounterLastSignalAge(uint8_t ab_channel)
{
  return halSimulator().getCounterLastSignalAge( ab_channel );
}


} // __HAL
