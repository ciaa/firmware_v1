/*
  system_target_extensions.h: header for PC specific extensions for
    the HAL for central system

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _HAL_PC_SYSTEM_TARGET_EXTENSIONS_H_
#define _HAL_PC_SYSTEM_TARGET_EXTENSIONS_H_

#include <IsoAgLib/isoaglib_config.h>
#include "../typedef.h"
#include "../errcodes.h"

#if !defined( WIN32 ) && !defined( __IAR_SYSTEMS_ICC__ )
#include <sys/time.h>
#include <time.h>
#include <cstdlib>
#endif


class HALSimulator_c
{
public:
  HALSimulator_c() {}
  virtual ~HALSimulator_c() {} 

  // System
  virtual bool getOn_offSwitch();
  virtual void powerHold( bool /* ab_on */ ) {}

  // Outputs
  virtual void eventMainRelais( bool /*on*/ ) {}
  virtual void eventSetPwmFreq(uint8_t /*bOutputGroup*/, uint32_t /*dwFrequency*/) {}
  virtual void eventSetDigout(uint8_t /*bOutputNo*/, uint16_t /*wPWMValue*/) {}

  // Inputs
  virtual void init_digin( uint8_t /*bInput*/, uint8_t /*bMode*/, uint8_t /*bAktivhighlow*/, void (*pfFunctionName)(...) ) { (void)pfFunctionName; }
  virtual void init_analogin( uint8_t /*bNumber*/, uint8_t /*bType*/ ) {}
  virtual void init_counter(uint8_t /*bInput*/, uint16_t /*aui16_timebase*/, boolean /*ab_activHigh*/, boolean /*ab_risingEdge*/) {}
  virtual int16_t getDiginOnoff( uint8_t /*bInputNumber*/ ) { return false; }
  virtual uint32_t getCounterPeriod_us( uint8_t /*bInput*/ ) { return 0xFFFFFFFFUL; }
  virtual void getDiginFreq( uint8_t /*bInput*/, uint32_t * /*pwFrequency*/ ) {}
  virtual int16_t getAdc( uint8_t /*bKanalnummer*/ ) { return 0; }
  virtual uint32_t getCounter( uint8_t /*bInput*/ ) { return 0; }
  virtual int16_t resetCounter( uint8_t /*bInput*/ ) { return 0; }
  virtual uint32_t getCounterLastSignalAge( uint8_t /*bInput*/ ) { return 0; }

  // RS-232
  virtual void init_rs232( uint32_t /*baudrate*/, uint8_t /*bMode*/, uint8_t /*bStoppbits*/, bool /*bitSoftwarehandshake*/, uint8_t /*comport*/ ) {}
  virtual void write_rs232( uint8_t /*aui8_channel*/, const uint8_t * /*lpBuf*/, uint16_t /*dwToWrite*/ )	{}
  virtual int16_t getRs232RxBufCount( uint8_t /*comport*/ )	{ return 0; }
  virtual int16_t getRs232Char( uint8_t /*aui8_channel*/, uint8_t * /*pbRead*/ ) { return 0; }

  // CAN
  virtual void SendCanMsg( uint8_t /*bBusNumber*/, uint32_t /*dwTimeStamp*/, uint8_t /*bXtd*/, uint32_t /*dwId*/, uint8_t /*bDlc*/, const uint8_t /*abData*/[8] ) {}
  virtual void ReceiveCanMsg( uint8_t /*bBusNumber*/, uint32_t /*dwTimeStamp*/, uint8_t /*bXtd*/, uint32_t /*dwId*/, uint8_t /*bDlc*/, uint8_t /*abData*/[8] ) {}
  virtual bool InsertReceiveCanMsg( uint8_t /*bBusNumber*/, uint8_t* /*bXtd*/, uint32_t* /*dwId*/, uint8_t* /*bDlc*/, uint8_t* /*abData*/ ) { return false; }

  // EEPROM
  virtual const char* GetEEPROM_FileName()  { return "eeprom.dat"; }
};



namespace __HAL {


HALSimulator_c &halSimulator();
void setHalSimulator( HALSimulator_c *sim );


#define  GET_U_C               35        /* UC (Boardnnung)   */
#define  GET_U_EXT_8_5_V       15        /* U 8,r = sim; */


/** \name General BIOS Extenders */
/*@{*/

void openSystem();
void closeSystem();
bool isSystemOpened();

int32_t getTime();
int32_t getStartupTime();
int16_t getSnr(uint8_t *snrDat);               /* serial number of target */

int16_t  getCpuFreq(void);                 /* get the cpu frequency*/

void powerHold( bool ab_on );              /* turn on and off self power holding */

bool getOn_offSwitch(void);               /* the evaluation of the on/off-switch (D+)*/

int16_t KeyGetByte(uint8_t *p);

void sleep_max_ms( uint32_t ms );

/*@}*/

} //end namespace __HAL
#endif
