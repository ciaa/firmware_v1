/*
  eeprom_target_extensions.h: header for PC specific extensions for
    the HAL for EEPROM

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef _PC_EEPROM_TARGET_EXTENSIONS_H_
#define _PC_EEPROM_TARGET_EXTENSIONS_H_

#include <IsoAgLib/isoaglib_config.h>

namespace __HAL {

uint32_t getEepromSize(void);          /* get the size of the eeprom */
int16_t getEepromSegmentSize(void);    /* get the segment size of the eeprom for page write access*/
int16_t eepromReady(void);             /* get the status of eeprom*/
int16_t eepromWp(boolean bitMode);     /* enable or disabel write protection*/
int16_t eepromWrite(uint16_t wAddress,uint16_t wNumber,const uint8_t *pbData); /* write one or more bytes into the eeprom*/
int16_t eepromWriteByte(uint16_t wAddress,uint8_t bByte);           /* write one uint8_t into the eeprom */
int16_t eepromRead(uint16_t wAddress,uint16_t wNumber,uint8_t *pbByte);  /* read one or more uint8_t from the eeprom*/

}

#endif
