/*
  pc_eeditor_can.h: header for the PSEUDO BIOS CAN EEPROM Editor
    module for development and test on a PC

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef PC_EEDITOR_CAN_H
#define PC_EEDITOR_CAN_H

  /* PROTOTYPES */
namespace __HAL {
  int16_t iInitEEEditor(  uint8_t bBus,
                       int16_t iObjNrReceiveCan, int16_t iObjNrTransmitCan, 
                       uint32_t dwReceiveCanId, uint8_t bCanType,
                       int16_t iNumberMsgsReceive, int16_t iNumberMsgsTransmit);
  int16_t iCallCanEEMonitor(void);
} // end namespace __HAL
#endif
