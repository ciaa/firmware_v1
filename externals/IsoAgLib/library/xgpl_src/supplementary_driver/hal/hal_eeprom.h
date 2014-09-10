/*
  hal_eeprom.h: include dependent on used target (defined in
    IsoAgLib/isoaglib_config.h) the suitable HAL specific header for
    EEPROM data storage

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/* ************************************************************ */
/** \file IsoAgLib/hal/hal_eeprom.h
  * include dependent on used target (defined in
    IsoAgLib/isoaglib_config.h) the suitable HAL
    specific header for EEPROM data storage.
*/
/* ************************************************************ */
#ifndef _HAL_INDEPENDENT_EEPROM_H_
#define _HAL_INDEPENDENT_EEPROM_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/hal_config.h>


namespace HAL
{
  /**
   @return EEPROM size in byte
  */
  uint32_t getEepromSize();

  /**
   @return EEPROM segment size in kbyte OR byte - this needs to be verified!!
  */
  uint16_t getEepromSegmentSize();

  /**
    read amount of uint8_t in uint8_t string variable
    @param wAddress start adress to read
    @param wNumber amount of bytes to read
    @param pbByte pointer to uint8_t string, where data is inserted
    @return error state (C_NO_ERR == o.k.)
  */
  int16_t eepromRead(uint16_t wAddress,uint16_t wNumber,uint8_t *pbByte);

  /**
    write amount of uint8_t from string into EEPROM from given start adress
    @param wAddress adress of first written uint8_t in EEPROM
    @param wNumber amount of written data
    @param pbData pointer to uint8_t string which should be written into EEPROM
    @return error state (C_NO_ERR == o.k.)
  */
  int16_t eepromWrite(uint16_t wAddress,uint16_t wNumber,const uint8_t *pbData);

  /**
    set or unset set write protection of EEPROM, should be called before
    every write access
    @param bitMode sets write protection OFF or ON
    @return error state (C_NO_ERR == o.k.)
  */
  int16_t eepromWp(bool bitMode);

  /**
    check if EEPROM is ready for actions
    @return HAL_NO_ERR -> ready
  */
  int16_t eepromReady();
} // HAL


/// INCLUDE CONCRETE SYSTEM-HAL-IMPLEMENTATION
/// ==========================================

#ifdef HAL_PATH_SUPPLEMENTARY_EEPROM
#  define _hal_eeprom_header_ <HAL_PATH_SUPPLEMENTARY_EEPROM/eeprom.h>
#  include _hal_eeprom_header_
#endif


#endif
