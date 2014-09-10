/*
  errcodes.h: header for the P1MC error codes

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef __P1MC_ERRCODES_H__
#define __P1MC_ERRCODES_H__

/* include BIOS errcode definition */
/* --- map BIOS error codes to target independent HAL codes --- */
#define HAL_NO_ERR            1
#define HAL_BUSY_ERR          2
#define HAL_CONFIG_ERR        3
#define HAL_RANGE_ERR         4
#define HAL_NOACT_ERR         5
#define HAL_OVERFLOW_ERR      7
#define HAL_UNKNOWN_ERR       9

#endif
