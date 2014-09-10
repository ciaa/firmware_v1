/*
  target_extension_rs232_hal_simulator.cpp: source for HAL simulator
    for RS-232. This file was based on target_extension_rs232_w32_sys.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "rs232_target_extensions.h"

#include <IsoAgLib/hal/pc/errcodes.h>

#include <stdio.h>      /* for fgets()  */
#include <stdlib.h>     /* for atexit() */
#include <conio.h>      /* for KeyXXX() */
#include <windows.h>    /* for SioXXX() */
#include <deque>

#include <IsoAgLib/hal/pc/system/system_target_extensions.h>


namespace __HAL {
HANDLE hCom[RS232_CHANNEL_CNT];
DCB oldConfig[RS232_CHANNEL_CNT];
STL_NAMESPACE::deque<int8_t> deq_readBuff[RS232_CHANNEL_CNT];

int8_t c_read;

static int arr_usedPort[RS232_CHANNEL_CNT] = {
#if RS232_CHANNEL_CNT > 0
false
#endif
#if RS232_CHANNEL_CNT > 1
, false
#endif
#if RS232_CHANNEL_CNT > 2
, false
#endif
#if RS232_CHANNEL_CNT > 3
, false
#endif
#if RS232_CHANNEL_CNT > 4
, false
#endif
#if RS232_CHANNEL_CNT > 5
, false
#endif
#if RS232_CHANNEL_CNT > 6
, false
#endif
#if RS232_CHANNEL_CNT > 7
, false
#endif
#if RS232_CHANNEL_CNT > 8
, false
#endif
};


int16_t
close_rs232(uint8_t comport)
{
	if ( comport >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
	
	return HAL_NO_ERR;
}

void
close_rs232()
{
  for ( int comport = 0; comport < RS232_CHANNEL_CNT; comport++)
  {
    if ( arr_usedPort[comport] )
    {
      arr_usedPort[comport] = false;
    }
  }
}


int16_t
init_rs232(uint32_t baudrate,uint8_t bMode,uint8_t bStoppbits,bool bitSoftwarehandshake, uint8_t comport)
{
	if ( comport >= RS232_CHANNEL_CNT )
		return HAL_RANGE_ERR;

	halSimulator().init_rs232( baudrate, bMode, bStoppbits, bitSoftwarehandshake, comport );

	return HAL_NO_ERR;
}


int16_t
setRs232Baudrate(uint32_t baudrate, uint8_t comport)
{
	if ( comport >= RS232_CHANNEL_CNT )
		return HAL_RANGE_ERR;

	return HAL_NO_ERR;
}


bool
WriteABuffer( uint8_t aui8_channel, const uint8_t * lpBuf, uint16_t dwToWrite)
{
	halSimulator().write_rs232( aui8_channel, lpBuf, dwToWrite );
	
	return true;
}


int16_t
put_rs232Char(uint8_t bByte, uint8_t aui8_channel)
{
	if ( aui8_channel >= RS232_CHANNEL_CNT )
		return HAL_RANGE_ERR;

	return WriteABuffer( aui8_channel, &bByte, 1) ? HAL_NO_ERR : HAL_NOACT_ERR;
}


int16_t
put_rs232NChar(const uint8_t *bpWrite,uint16_t wNumber, uint8_t aui8_channel)
{
	if ( aui8_channel >= RS232_CHANNEL_CNT )
		return HAL_RANGE_ERR;

	return WriteABuffer( aui8_channel, bpWrite, wNumber) ? HAL_NO_ERR : HAL_NOACT_ERR;
}


int16_t
put_rs232String(const uint8_t *pbString, uint8_t aui8_channel)
{
	if ( aui8_channel >= RS232_CHANNEL_CNT )
		return HAL_RANGE_ERR;

	return WriteABuffer( aui8_channel, pbString, strlen((const char*)pbString)) ? HAL_NO_ERR : HAL_NOACT_ERR;
}


int16_t
getRs232RxBufCount(uint8_t comport)
{
	return halSimulator().getRs232RxBufCount( comport );
}


int16_t
getRs232String(uint8_t *pbRead, uint8_t ui8_terminateChar, uint8_t comport)
{
	int16_t ret;

	do
	{
		ret = getRs232Char( pbRead, comport );
	} while( ret == HAL_NO_ERR && (*(pbRead++) != ui8_terminateChar ) );

	return ret;
}


int16_t
getRs232Char(uint8_t *pbRead, uint8_t aui8_channel)
{
	return halSimulator().getRs232Char( aui8_channel, pbRead );
}


int16_t
getRs232TxBufCount(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  /** @todo ON REQUEST: decide if RS232 TX buffer from OS should be asked */
  return 0;
}


int16_t
configRs232RxObj(uint16_t wBuffersize,void (*pFunction)(uint8_t *bByte), uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  /** @todo ON REQUEST: should this be implemented? */
  return HAL_NO_ERR;
}


int16_t
configRs232TxObj(uint16_t wBuffersize,void (*funktionAfterTransmit)(uint8_t *bByte),
                         void (*funktionBeforTransmit)(uint8_t *bByte), uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  /** @todo ON REQUEST: should this be implemented? */
  return HAL_NO_ERR;
}


int16_t
getRs232Error(uint8_t *Errorcode, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  /** @todo ON REQUEST: should this be implemented? */
  return HAL_NO_ERR;
}


void
clearRs232RxBuffer(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return;
  /** @todo ON REQUEST: should this be implemented? */
};


void
clearRs232TxBuffer(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return;
  /** @todo ON REQUEST: should this be implemented? */
}

} // end of namespace __HAL
