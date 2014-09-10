/*
  pc_eeditor_can.cpp: source for the PSEUDO BIOS CAN EEPROM Editor
    module for development and test on a PC

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "eeprom.h"
#include <string.h>
#include <cstdio>
//typedef char int8_t;

#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif

namespace __HAL {

int16_t iInitEEEditor(  uint8_t bBus,
                       int16_t iObjNrReceiveCan, int16_t iObjNrTransmitCan,
                       uint32_t dwReceiveCanId, uint8_t bCanType,
                       int16_t iNumberMsgsReceive, int16_t iNumberMsgsTransmit)
{
 FILE *eeditor;
 int16_t wAddress, wLen;
 int32_t lVal;
 short iVal;
 int8_t cVal;
 float fVal;
 int8_t pcType;
 char pcZeile[50], pcVal[50];

  printf("iInitEEEditor mit CAN-BUS %hu, Empfangs Objekt %hd, Sende Objekt %hd, \
    Empfangs CAN Ident_c %x, Empfangspuffer %d, Sendepuffer %d und CanType %hd aufgerufen.\n",
    bBus, iObjNrReceiveCan, iObjNrTransmitCan, dwReceiveCanId, iNumberMsgsReceive,
    iNumberMsgsTransmit, bCanType);

 printf("Funktion zum Start des EEPROM Editors aufgerufen - lese Daten in EEPROM Datei\n");
#ifdef WIN32
 eeditor = fopen("..\\..\\..\\simulated_io\\eeditor.dat", "r+");
#else
 eeditor = fopen("../../../simulated_io/eeditor.dat", "r+");
#endif
 if (eeditor == NULL) {
   // try again with current path...
   eeditor = fopen("eeditor.dat", "r+");
 }

 if (eeditor == NULL)
 { // no eeditor.dat, better return;
   return HAL_NO_ERR;
 }

 while (fgets(pcZeile, 49, eeditor) != NULL)
 {
  /* erase '\n' at end of line */
  if (strchr(pcZeile, '\n') != 0)
    *(strchr(pcZeile, '\n')) = '\0';

  sscanf(pcZeile, "%hd %hd %c %s", &wAddress, &wLen, &pcType, pcVal);
  switch (pcType)
  {
    case 'l':
      sscanf(pcVal, "%d", &lVal);
      printf("EEprom Editor Long %d and Addresse %d\n", lVal, wAddress);
      eepromWrite(wAddress, 4, (const uint8_t*)&lVal);
      break;
    case 'i':
      sscanf(pcVal, "%hd", &iVal);
      printf("EEprom Editor Int16 %hd and Addresse %d\n", iVal, wAddress);
      eepromWrite(wAddress, 2, (const uint8_t*)&iVal);
      break;
    case 'c':
      sscanf(pcVal, "%hd", &iVal);
      cVal = static_cast<int8_t>(iVal);
      printf("EEprom Editor int8_t %c and Addresse %d\n", cVal, wAddress);
      eepromWrite(wAddress, 1, (const uint8_t*)&cVal);
      break;
    case 'f':
      sscanf(pcVal, "%f", &fVal);
      printf("EEprom Editor Float %f and Addresse %d\n", fVal, wAddress);
      eepromWrite(wAddress, 4, (const uint8_t*)&fVal);
      break;
    case 's':
      printf("EEprom Editor Text %s and Addresse %d\n", pcVal, wAddress);
      eepromWrite(wAddress, wLen, (const uint8_t*)pcVal);
      break;
  }
 }
 (void)fclose (eeditor);

  return HAL_NO_ERR;
}

int16_t iCallCanEEMonitor(void)
{
  //printf("iCallCanEEMonitor())\n");
  return HAL_NO_ERR;
}

} //end namespace __HAL
