/*
  eeprom_target_extensions.cpp: source for PC specific extensions for
    the HAL for EEPROM

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/** \file hal/pc/system/system_target_extensions.cpp
 * A module targetExtensions should be used
 * for all methods, which can't be simply
 * mapped from ECU standard BIOS to the needs of
 * IsoAgLib by mostly renaming and reordering of functions, parameters
 * and types in <i>\<target\>/\<device\>/\<device\>.h</i> .
 * ********************************************************** */

#include "eeprom.h"
#include <IsoAgLib/hal/hal_system.h>
#include <cstdio>
#include <cctype>

#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif

namespace __HAL {


// These are the default names for the pc's simulated eeprom
// Add a #define in your config file if you wish to name it something else.
#ifndef EEPROM_DAT_FILE
#  define EEPROM_DAT_FILE		halSimulator().GetEEPROM_FileName()
#endif

#ifndef HAL_SIMULATOR_EEPROM_SIZE
#  define HAL_SIMULATOR_EEPROM_SIZE   (8*1024)
#endif

#ifndef HAL_SIMULATOR_EEPROM_SEGMENT_SIZE
#  define HAL_SIMULATOR_EEPROM_SIZE   (32)
#endif


/* ***************************************** */
/* ****** EEPROM I/O BIOS functions  ******* */
/* ***************************************** */
static FILE* eepromDat;

/* get the size of the eeprom */
uint32_t getEepromSize(void)
{
  return HAL_SIMULATOR_EEPROM_SIZE;
};

/* get the segment size of the eeprom for page write access */
int16_t getEepromSegmentSize(void){
  return HAL_SIMULATOR_EEPROM_SIZE;
};

/* get the status of eeprom */
int16_t eepromReady(void)
{
  return HAL_NO_ERR; // PC EEPROM is always ready.
}

/* enable or disable write protection */
int16_t eepromWp(boolean /* bitMode */ )
{
  // printf("eepromWp mit %i aufgerufen\n", bitMode);
  // @todo ON REQUEST Implement Write Protection for PC EEPROM?
  return HAL_NO_ERR;
}

int16_t openDatFileAndSeek(long al_position)
{
  eepromDat = fopen(EEPROM_DAT_FILE, "r+b");
  if ( NULL == eepromDat )
  { // try to create file with creation mode
    eepromDat = fopen(EEPROM_DAT_FILE, "w+b");
    // Note: Using "w+b" instead of "a+b" for the following reason:
    // (although we're only using this command for creation of the file,
    //  it's being closed and reopened with "r+b" anyway!)
    // [Mike: Appending a non-existent file in certain versions of Windows, does not fill
    // the file with 00's up to the seek position.  So, the file ends up containing
    // whatever garbage happened to exist on the disk previously.  Telling it w+b
    // instead, creates the file and fills with 00's.]
    if ( NULL != eepromDat )
    { // we have to close the file, as the normal operation works only with a "r+b" opened FILE
      // - at least on some targets
      fclose( eepromDat );
      // reopen now with "r+b"
      eepromDat = fopen(EEPROM_DAT_FILE, "r+b");
    }
  }
  if ( NULL != eepromDat )
  { // seek on success to the wanted position
    fseek(eepromDat, al_position, 0);
    return HAL_NO_ERR;
  }
  else return HAL_CONFIG_ERR;
}

/* write one or more bytes into the eeprom*/
int16_t eepromWrite(uint16_t wAddress,uint16_t wNumber,const uint8_t *pbData)
{
  int16_t errCode = openDatFileAndSeek(wAddress);
  if ( errCode  != HAL_NO_ERR ) return errCode;

#if DEBUG_EEPROM
  INTERNAL_DEBUG_DEVICE << "Writing data to EEPROM address " << wAddress << " - Text: ";
#endif

  for (int i=0; i < wNumber; ++i)
  {
    fputc (pbData[i], eepromDat);
#if DEBUG_EEPROM
    INTERNAL_DEBUG_DEVICE << (isprint(pbData[i]) ? pbData[i] : '.');
#endif
  }

#if DEBUG_EEPROM
  switch (wNumber)
  {
    case 4:
      INTERNAL_DEBUG_DEVICE << " - Number (4 byte signed): " << *(int32_t*)pbData;
      break;
    case 2:
      INTERNAL_DEBUG_DEVICE << " - Number (2 byte signed): " << *(int16_t*)pbData;
      break;
    case 1:
      INTERNAL_DEBUG_DEVICE << " - Number (1 byte signed): " << *(int8_t*)pbData;
      break;
    default:
      break;
  }
  INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_ENDL;
#endif

  fclose(eepromDat);
  return HAL_NO_ERR;
}

/* write one uint8_t into the eeprom */
int16_t eepromWriteByte(uint16_t wAddress,uint8_t bByte)
{
#if DEBUG_EEPROM
  INTERNAL_DEBUG_DEVICE << "Writing byte '" << uint16_t (bByte) << "' to EEPROM address " << wAddress << " - Text: ";
  INTERNAL_DEBUG_DEVICE << (isprint(bByte) ? bByte : '.');
  INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_ENDL;
#endif
  int16_t errCode = openDatFileAndSeek(wAddress);
  if ( errCode  != HAL_NO_ERR ) return errCode;

  fputc(bByte, eepromDat);
  fclose(eepromDat);
  return HAL_NO_ERR;
}

/* read one or more uint8_t from the eeprom */
int16_t eepromRead(uint16_t wAddress,uint16_t wNumber,uint8_t *pbByte)
{
  const int16_t errCode = openDatFileAndSeek(wAddress);
  if ( errCode  != HAL_NO_ERR ) return errCode;

#if DEBUG_EEPROM
  INTERNAL_DEBUG_DEVICE << "Reading EEPROM data from address " << wAddress << " - Text: ";
#endif

  for (int i=0; i < wNumber; ++i)
  {
    const int16_t c = fgetc(eepromDat);
    pbByte[i] = ( c == EOF ) ? 0xFF : (const uint8_t)c;   // pad when at EOF
#if DEBUG_EEPROM
    INTERNAL_DEBUG_DEVICE << ((pbByte[i] >= 0x20) ? pbByte[i] : '.');
#endif
  }

#if DEBUG_EEPROM
  switch (wNumber)
  {
    case 4:
      INTERNAL_DEBUG_DEVICE << " - Number (4 byte signed): " << *(int32_t*)pbByte;
      break;
    case 2:
      INTERNAL_DEBUG_DEVICE << " - Number (2 byte signed): " << *(int16_t*)pbByte;
      break;
    case 1:
      INTERNAL_DEBUG_DEVICE << " - Number (1 byte signed): " << *(int8_t*)pbByte;
      break;
    default:
      break;
  }
  INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_ENDL;
#endif

  fclose(eepromDat);
  return HAL_NO_ERR;
}

} // End of namespace __HAL

