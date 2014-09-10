/*
  hal_rs232.h: include dependent on used target (defined in
    IsoAgLib/isoaglib_config.h) the suitable HAL specific header for
    RS232 communication

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/* ************************************************************ */
/** \file
 * 
  * include dependent on used target (defined in
	  IsoAgLib/isoaglib_config.h) the suitable HAL
		specific header for RS232 communication.
*/
/* ************************************************************ */
#ifndef _HAL_INDEPENDENT_RS232_H_
#define _HAL_INDEPENDENT_RS232_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/hal_config.h>


namespace HAL
{
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
  int16_t init_rs232(uint32_t baudrate,uint8_t bMode,uint8_t bStoppbits,bool bitSoftwarehandshake, uint8_t aui8_channel);

  /** close the RS232 interface. */
  int16_t close_rs232(uint8_t aui8_channel);

  /**
    set the RS232 Baudrate
    @param baudrate wanted baudrate
    @return HAL_NO_ERR -> o.k. else baudrate setting incorrect
  */
  int16_t setRs232Baudrate(uint32_t baudrate, uint8_t aui8_channel);

  /**
    get the amount of data [uint8_t] in receive buffer
    @return receive buffer data byte
  */
  int16_t getRs232RxBufCount(uint8_t aui8_channel);

  /**
    get the amount of data [uint8_t] in send buffer
    @return send buffer data byte
  */
  int16_t getRs232TxBufCount(uint8_t aui8_channel);

// put the following out of the HAL-declaration, because
// of the "_huge" which is different... Don't want to care right now...
#if 0
  /**
    configure a receive buffer and set optional irq function pointer for receive
    @param wBuffersize wanted buffer size
    @param pFunction pointer to irq function or NULL if not wanted
  */
  int16_t configRs232RxObj(uint16_t wBuffersize,void (*pFunction)(uint8_t _huge *bByte), uint8_t aui8_channel);

  /**
    configure a send buffer and set optional irq function pointer for send
    @param wBuffersize wanted buffer size
    @param funktionAfterTransmit pointer to irq function or NULL if not wanted
    @param funktionBeforTransmit pointer to irq function or NULL if not wanted
  */
  int16_t configRs232TxObj(uint16_t wBuffersize,void (*funktionAfterTransmit)(uint8_t _huge *bByte),
                           void (*funktionBeforTransmit)(uint8_t _huge *bByte), uint8_t aui8_channel);
#endif

  /**
    get errr code of BIOS
    @return 0=parity, 1=stopbit framing error, 2=overflow
  */
  int16_t getRs232Error(uint8_t *Errorcode, uint8_t aui8_channel);

  /**
    read single int8_t from receive buffer
    @param pbRead pointer to target data
    @return HAL_NO_ERR -> o.k. else buffer underflow
  */
  int16_t getRs232Char(uint8_t *pbRead, uint8_t aui8_channel);

  /**
    read bLastChar terminated string from receive buffer
    @param pbRead pointer to target data
    @param bLastChar terminating char
    @return HAL_NO_ERR -> o.k. else buffer underflow
  */
  int16_t getRs232String(uint8_t *pbRead,uint8_t bLastChar, uint8_t aui8_channel);

  /**
    send single uint8_t on RS232
    @param bByte data uint8_t to send
    @return HAL_NO_ERR -> o.k. else send buffer overflow
  */
  int16_t put_rs232Char(uint8_t bByte, uint8_t aui8_channel);

  /**
    send string of n uint8_t on RS232
    @param bpWrite pointer to source data string
    @param wNumber number of data uint8_t to send
    @return HAL_NO_ERR -> o.k. else send buffer overflow
  */
  int16_t put_rs232NChar(const uint8_t *bpWrite,uint16_t wNumber, uint8_t aui8_channel);

  /**
    send '\\0' terminated string on RS232
    @param pbString pointer to '\\0' terminated (!) source data string
    @return HAL_NO_ERR -> o.k. else send buffer overflow
  */
  int16_t put_rs232String(const uint8_t *pbString, uint8_t aui8_channel);

  /**
    clear receive buffer
  */
  void clearRs232RxBuffer(uint8_t aui8_channel);

  /**
    clear send buffer
  */
  void clearRs232TxBuffer(uint8_t aui8_channel);
} // HAL


/// INCLUDE CONCRETE SYSTEM-HAL-IMPLEMENTATION
/// ==========================================

#ifdef HAL_PATH_SUPPLEMENTARY_RS232
#  define _hal_rs232_header_ <HAL_PATH_SUPPLEMENTARY_RS232/rs232.h>
#  include _hal_rs232_header_
#endif


#endif
