/*
  pc_eeditor_rs232.cpp: source for the PSEUDO BIOS RS232 EEPROM Editor
    module for development and test on a PC

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "eeprom.h"
#include <cstdio>

namespace __HAL {

int16_t iCallRs232EEMonitor(void)
{
  printf("iCallRs232EEMonitor())\n");
  return HAL_NO_ERR;
}
} // end namespace __HAL
