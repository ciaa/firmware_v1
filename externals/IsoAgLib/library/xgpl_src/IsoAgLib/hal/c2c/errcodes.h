/*
  errcodes.h: header for the C2C error codes

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef __C2C_ERRCODES_H__
#define __C2C_ERRCODES_H__

namespace HAL {
  extern "C" {
    #include <commercial_BIOS/bios_c2c/c2c10err.h>
  }
} // __HAL

/* --- map BIOS error codes to target independend HAL codes --- */
#define HAL_NO_ERR            C_NO_ERR
#define HAL_BUSY_ERR          C_BUSY
#define HAL_CONFIG_ERR        C_CONFIG
#define HAL_RANGE_ERR         C_RANGE
#define HAL_NOACT_ERR         C_NOACT
#define HAL_OVERFLOW_ERR      C_OVERFLOW
#define HAL_UNKNOWN_ERR       C_UNKNOWN_ERR

#endif
