/*
  system_target_extensions.h: header for P1MC specific extensions for
    the HAL for central system

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/** \file IsoAgLib/hal/p1mc/system/system_system_target_extensions.h
 * A module targetExtensions should be used
 * for all methods, which can't be simply
 * mapped from ECU standard BIOS to the needs of
 * IsoAgLib by mostly renaming and reordering of functions, parameters
 * and types in <i>\<target\>/\<device\>/\<device\>.h</i> .   */
/* ********************************************************** */

#ifndef _HAL_P1MC_SYSTEM_TARGET_EXTENSIONS_H_
#define _HAL_P1MC_SYSTEM_TARGET_EXTENSIONS_H_

#include <IsoAgLib/isoaglib_config.h>
#include "../typedef.h"
#include "../errcodes.h"

#define C_NO_ERR   0
#define ON   1
#define OFF  0

namespace __HAL {

void openSystem();

void closeSystem();

int32_t getTime(void);

int16_t  getSnr(uint8_t *snrDat);

void  start_task_timer ( void );

bool getOn_offSwitch();

void powerHold( bool ab_on );

int16_t  __getAdcUbat();

int16_t  getAdc_u85();

}

#endif
