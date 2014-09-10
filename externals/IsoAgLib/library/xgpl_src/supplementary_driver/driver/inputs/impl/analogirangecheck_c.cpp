/*
  analogirangecheck_c.cpp - implementation file for AnalogIRangeCheck_c

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "analogirangecheck_c.h"


namespace __IsoAgLib {

AnalogIRangeCheck_c::AnalogIRangeCheck_c(uint8_t ab_channel, IsoAgLib::iInput_c::analogType_t ren_analogType, bool ab_useMean, bool ab_fastAdc,
  uint16_t aui16_minValid, uint16_t aui16_maxValid )
  : AnalogI_c( ab_channel, ren_analogType, ab_useMean, ab_fastAdc )
{
  setRange( aui16_minValid, aui16_maxValid );
}


void
AnalogIRangeCheck_c::init(uint8_t ab_channel, IsoAgLib::iInput_c::analogType_t ren_analogType, bool ab_useMean, bool ab_fastAdc,
  uint16_t aui16_minValid, uint16_t aui16_maxValid )
{
  AnalogI_c::init( ab_channel, ren_analogType, ab_useMean, ab_fastAdc );
  setRange( aui16_minValid, aui16_maxValid );
}


AnalogIRangeCheck_c::~AnalogIRangeCheck_c()
{
}


void
AnalogIRangeCheck_c::setRange( uint16_t aui16_minValid, uint16_t aui16_maxValid )
{
  ui16_minValid = aui16_minValid;
  ui16_maxValid = aui16_maxValid;
}


int16_t
AnalogIRangeCheck_c::validatedVal( bool &rb_tooLow, bool &rb_tooHigh ) const
{
  const int16_t ci16_tempVal = val();
  rb_tooLow  = ( ci16_tempVal < ui16_minValid );
  rb_tooHigh = ( ci16_tempVal > ui16_maxValid );
  return ci16_tempVal;
}


bool
AnalogIRangeCheck_c::good( void ) const
{
 const uint16_t ui16_tempVal = val();
 if ( ( ui16_tempVal >= ui16_minValid )
   && ( ui16_tempVal <= ui16_maxValid ) ) {
   return true;
 }
 else {
   return false;
  }
}


bool
AnalogIRangeCheck_c::error( void ) const
{
  return checkRangeError();
}


bool
AnalogIRangeCheck_c::checkRangeError( void ) const
{
 const uint16_t ui16_tempVal = val();
 if ( ( ui16_tempVal < ui16_minValid )
   || ( ui16_tempVal > ui16_maxValid ) ) {
   return true;
 }
 else {
   return false;
  }
}


bool
AnalogIRangeCheck_c::checkTooHigh( void ) const
{
  const uint16_t ui16_tempVal = val();
  if ( ui16_tempVal > ui16_maxValid ) {
    return true;
  }
  else {
    return false;
  }
}


bool
AnalogIRangeCheck_c::checkTooLow( void ) const
{
  const uint16_t ui16_tempVal = val();
  if ( ui16_tempVal < ui16_minValid ) {
    return true;
  }
  else {
    return false;
  }
}

} // __IsoAgLib
