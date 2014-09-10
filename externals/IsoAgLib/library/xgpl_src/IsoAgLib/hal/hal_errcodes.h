/*
  hal_errcodes.h: target independent(!) error-codes only

  (C) Copyright 2013 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef _HAL_INDEPENDENT_ERRCODES_H_
#define _HAL_INDEPENDENT_ERRCODES_H_

#define HAL_DIGOUT_OPEN        (int16_t)(-(20 * 0x0001))
#define HAL_DIGOUT_SHORTCUT    (int16_t)(-(21 * 0x0001))
// the following 3 defines are currently not handled by ISOAgLib driver code!
#define HAL_DIGOUT_OVERTEMP    (int16_t)(-(22 * 0x0001))
#define HAL_DIGOUT_UNDERVOLT   (int16_t)(-(23 * 0x0001))
#define HAL_DIGOUT_OVERVOLT    (int16_t)(-(24 * 0x0001))

#endif
