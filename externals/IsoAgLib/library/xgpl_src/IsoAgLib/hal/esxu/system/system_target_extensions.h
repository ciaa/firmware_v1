/*
  system_target_extensions.h: header for ESXu specific extensions for
    the HAL for central system

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/** \file IsoAgLib/hal/esxu/system/system_target_extensions.h
 * A module targetExtensions should be used
 * for all methods, which can't be simply 
 * mapped from ECU standard BIOS to the needs of
 * IsoAgLib by mostly renaming and reordering of functions, parameters
 * and types in <i>\<target\>/\<device\>/\<device\>.h</i> .
 * ********************************************************** */

#ifndef _HAL_ESXu_SYSTEM_TARGET_EXTENSIONS_H_
#define _HAL_ESXu_SYSTEM_TARGET_EXTENSIONS_H_

namespace __HAL {
  extern "C" {
    /** include the BIOS specific header into __HAL */
    #include <commercial_BIOS/bios_esxu/mos10osy.h>
  }
}

#include <cstdio>
#include <IsoAgLib/isoaglib_config.h>
#include "../typedef.h"
#include "../errcodes.h"


namespace __HAL {

void openSystem();
void closeSystem();
bool isSystemOpened();
void initWatchdog( void* config );

}

#endif
