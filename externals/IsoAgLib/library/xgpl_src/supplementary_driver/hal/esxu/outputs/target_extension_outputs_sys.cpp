/*
  outputs_target_extensions.cpp: implement diagnose state

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <IsoAgLib/isoaglib_config.h>
#include "outputs.h"
#include <IsoAgLib/util/impl/util_funcs.h>


namespace HAL {

static const int16_t cui16_openLow  = ( 170000L / 4000L ); // 1700mV * 100 / 40mV * 100

  int16_t getDigoutDiagnose(uint8_t aui8_channel, uint16_t aui16_minCurrent, uint16_t aui16_maxCurrent)
  {
    int16_t i16_adc = getDigoutCurrent(aui8_channel);

    if( i16_adc < 0 )
      return HAL_NO_ERR;  // Pin doesn't support current checking - should really do something else here!!!

    bool b_hiSet = ( aui16_maxCurrent > 0 );
    uint16_t cui16_useLowCompare = cui16_openLow;

    if ( b_hiSet )
    { // if active PWM ( setting > 0 ) has lower voltage than
      // the open gain low level, something is wrong
      // ( if the PWM setting is really low, this evaluation could go wring )
      if ( i16_adc < cui16_useLowCompare )
        return HAL_DIGOUT_SHORTCUT;
      else
        return HAL_NO_ERR;
    }
    else
    {
      if ( i16_adc >= cui16_useLowCompare )
        return HAL_DIGOUT_OPEN;
      else
        return HAL_NO_ERR;
    }
  }

  int16_t getDigoutAdc( uint8_t aui8_channel )
  {
    if( aui8_channel > OUT4 )
      return -1;

    int16_t ci16_result = __HAL::get_adc(__HAL::getPwmoutAdcCheckNr(aui8_channel));

    if ( ci16_result != HAL_NO_ERR ) return ci16_result;
      return __IsoAgLib::mul1Div1Mul2Div2(ci16_result, 3743, 1, 100);
  }

};
