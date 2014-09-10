/*
  rs232.cpp: definition of HAL namesapce layer for Hardware Abstraction Layer

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "rs232.h"


uint8_t StoreInBigBuf = true;


namespace HAL
{
  /* ****************************** */
  /** \name RS232 I/O BIOS functions */
/*@{*/

  /**
    init the RS232 interface
    @param baudrate wnated Baudrate {75, 600, 1200, 2400, 4800, 9600, 19200}
          as configured in <IsoAgLib/isoaglib_config.h>
    @param bMode one of (DATA_7_BITS_EVENPARITY = 1, DATA_8_BITS_EVENPARITY = 2,
            DATA_7_BITS_ODDPARITY = 3, DATA_8_BITS_ODDPARITY = 4, DATA_8_BITS_NOPARITY = 5)
    @param bStoppbits amount of stop bits (1,2)
    @param bitSoftwarehandshake true -> use xon/xoff software handshake
    @return HAL_NO_ERR -> o.k. else one of settings incorrect
  */

uint8_t RS232_over_can_initialized = false;
uint8_t RS232_over_can_busnum = 1;

uint8_t BigBuf[1024];
uint16_t BigBufLen = 0;

uint16_t RS232_over_can_CANID = 0x7F0;

int16_t init_rs232 (uint32_t baudrate, uint8_t bMode, uint8_t bStoppbits, bool bitSoftwarehandshake, uint8_t aui8_channel)
{
  (void) baudrate;
  (void) bMode;
  (void) bStoppbits;
  (void) bitSoftwarehandshake;
  (void) aui8_channel;

  RS232_over_can_initialized = true;
  return HAL_NO_ERR;
};

/**
send single uint8_t on RS232
@param bByte data uint8_t to send
@return HAL_NO_ERR -> o.k. else send buffer overflow
*/
int16_t put_rs232Char(uint8_t bByte, uint8_t aui8_channel)
{
	return put_rs232NChar(&bByte, 1, aui8_channel);
};
/**
send string of n uint8_t on RS232
@param bpWrite pointer to source data string
@param wNumber number of data uint8_t to send
@return HAL_NO_ERR -> o.k. else send buffer overflow
*/
int16_t put_rs232NChar(const uint8_t *bpWrite,uint16_t wNumber, uint8_t aui8_channel)
{
	uint16_t numLeft;

	if( StoreInBigBuf )
		{
		if( !BigBufLen )
			{
			BigBuf[0] = BigBuf[1] = '\r';
			BigBufLen += 2;
			}
		if( BigBufLen + wNumber < sizeof( BigBuf ) )
			numLeft = wNumber;
		else
			numLeft = sizeof( BigBuf ) - BigBufLen;

		if( numLeft )
			{
			CNAMESPACE::memcpy( &BigBuf[BigBufLen], bpWrite, numLeft );
			BigBufLen += numLeft;
			}

		return HAL_NO_ERR;
		}
	else
		{
		if( BigBufLen )
			{
			numLeft = BigBufLen;
			BigBufLen = 0;
			put_rs232NChar( BigBuf, numLeft, aui8_channel );
			}

		numLeft = wNumber;
		while( numLeft )
			{
	#if ( CAN_INSTANCE_CNT > 1 )
			__IsoAgLib::CanIo_c& c_can = IsoAgLib::getCanInstance(RS232_over_can_busnum);
	#else
			__IsoAgLib::CanIo_c& c_can = IsoAgLib::getCanInstance();
	#endif
			const uint8_t cui8_len = (numLeft < 8 ? numLeft : 8);

			__IsoAgLib::CanPkg_c c_sendData;
			c_sendData.setIdent( RS232_over_can_CANID+aui8_channel, __IsoAgLib::Ident_c::StandardIdent );
			c_sendData.setDataFromString( &bpWrite[wNumber-numLeft], cui8_len );
			c_can << c_sendData;

			numLeft -= cui8_len;
			}

   return IsoAgLib::getILibErrInstance().good( IsoAgLib::iLibErr_c::Can );
		}

};
/**
send '\0' terminated string on RS232
@param pbString pointer to '\0' terminated (!) source data string
@return HAL_NO_ERR -> o.k. else send buffer overflow
*/
int16_t put_rs232String(const uint8_t *pbString, uint8_t aui8_channel)
{
	return put_rs232NChar (pbString, strlen( (char*)pbString ), aui8_channel);
};
}
