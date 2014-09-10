/*
  rs232_target_extensions.h: header for PC specific extensions for the
    HAL for RS232

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef _HAL_PC_RS232_TARGET_EXTENSIONS_H_
#define _HAL_PC_RS232_TARGET_EXTENSIONS_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/pc/config.h>
#include <IsoAgLib/hal/pc/typedef.h>

namespace __HAL {
typedef enum {
  DATA_7_BITS_EVENPARITY   = 1,
  DATA_8_BITS_EVENPARITY   = 2,
  DATA_7_BITS_ODDPARITY    = 3,
  DATA_8_BITS_ODDPARITY    = 4,
  DATA_8_BITS_NOPARITY     = 5
} Rs232PortMode_t;

/* ******************************************************** */
/**
 * \name RS232 I/O BIOS functions
 */
/* ******************************************************** */
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
int16_t init_rs232(uint32_t baudrate,uint8_t bMode,uint8_t bStoppbits,bool bitSoftwarehandshake, uint8_t aui8_channel = 0);
/** close the RS232 interface. */
int16_t close_rs232(uint8_t aui8_channel);
/**
  set the RS232 Baudrate
  @param baudrate wanted baudrate
  @return HAL_NO_ERR -> o.k. else baudrate setting incorrect
*/
int16_t setRs232Baudrate(uint32_t baudrate, uint8_t aui8_channel = 0);
/**
  get the amount of data [uint8_t] in receive buffer
  @return receive buffer data uint8_t
*/
int16_t getRs232RxBufCount(uint8_t aui8_channel = 0);
/**
  get the amount of data [uint8_t] in send buffer
  @return send buffer data uint8_t
*/
int16_t getRs232TxBufCount(uint8_t aui8_channel = 0);
/**
  configure a receive buffer and set optional irq function pointer for receive
  @param wBuffersize wanted buffer size
  @param pFunction pointer to irq function or NULL if not wanted
*/
int16_t configRs232RxObj(uint16_t wBuffersize,void (*pFunction)(uint8_t *bByte), uint8_t aui8_channel = 0);
/**
  configure a send buffer and set optional irq function pointer for send
  @param wBuffersize wanted buffer size
  @param funktionAfterTransmit pointer to irq function or NULL if not wanted
  @param funktionBeforTransmit pointer to irq function or NULL if not wanted
*/
int16_t configRs232TxObj(uint16_t wBuffersize,void (*funktionAfterTransmit)(uint8_t *bByte),
                                void (*funktionBeforTransmit)(uint8_t *bByte), uint8_t aui8_channel = 0);
/**
  get errr code of BIOS
  @return 0=parity, 1=stopbit framing error, 2=overflow
*/
int16_t getRs232Error(uint8_t *Errorcode, uint8_t aui8_channel = 0);

/**
  read single int8_t from receive buffer
  @param pbRead pointer to target data
  @return HAL_NO_ERR -> o.k. else buffer underflow
*/
int16_t getRs232Char(uint8_t *pbRead, uint8_t aui8_channel = 0);
/**
  read bLastChar terminated string from receive buffer
  @param pbRead pointer to target data
  @param bLastChar terminating int8_t
  @return HAL_NO_ERR -> o.k. else buffer underflow
*/
int16_t getRs232String(uint8_t *pbRead,uint8_t bLastChar, uint8_t aui8_channel = 0);

/**
  send single uint8_t on RS232
  @param bByte data uint8_t to send
  @return HAL_NO_ERR -> o.k. else send buffer overflow
*/
int16_t put_rs232Char(uint8_t bByte, uint8_t aui8_channel = 0);
/**
  send string of n uint8_t on RS232
  @param bpWrite pointer to source data string
  @param wNumber number of data uint8_t to send
  @return HAL_NO_ERR -> o.k. else send buffer overflow
*/
int16_t put_rs232NChar(const uint8_t *bpWrite,uint16_t wNumber, uint8_t aui8_channel = 0);
/**
  send '\0' terminated string on RS232
  @param pbString pointer to '\0' terminated (!) source data string
  @return HAL_NO_ERR -> o.k. else send buffer overflow
*/
int16_t put_rs232String(const uint8_t *pbString, uint8_t aui8_channel = 0);

/**
  clear receive buffer
*/
void clearRs232RxBuffer(uint8_t aui8_channel = 0);
/**
  clear send buffer
*/
void clearRs232TxBuffer(uint8_t aui8_channel = 0);
/*@}*/

} // End of name space __HAL
#endif
