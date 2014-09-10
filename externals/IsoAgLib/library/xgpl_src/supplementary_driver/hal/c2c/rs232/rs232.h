/*
  rs232.h: definition of HAL namesapce layer for C2C

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/** \file
 * 
 * The header <i>\<target\>/\<device\>/\<device\>.h</i> performs a name
   mapping between platform specific BIOS / OS function names
   and the function names, the IsoAgLib uses for hardware access.
   In this header only the groups for error codes, global system
   functions and CAN must be adapted to a new platform.
   If a simple name mapping is not possible for a specific
   function, the module targetExtensions can be used to implement
   the needed activity. The implementation of the PC version
   can be used to derive the meaning of the specific function.
*/
/* ************************************************************ */

#ifndef _HAL_C2C_RS232_H_
#define _HAL_C2C_RS232_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/c2c/typedef.h>
#include <IsoAgLib/hal/c2c/errcodes.h>

namespace __HAL {
  extern "C" {
    /** include the BIOS specific header into __HAL */
    #include <commercial_BIOS/bios_c2c/c2c10osy.h>
  }
}
/**
   namespace with layer of inline (cost NO overhead -> compiler replaces
   inline function with call to orig BIOS function)
   functions between all IsoAgLib calls for BIOS and the corresponding BIOS functions
   --> simply replace the call to the corresponding BIOS function in this header
       for adaptation to new platform
 */
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
  inline int16_t init_rs232(uint32_t baudrate,uint8_t bMode,uint8_t bStoppbits,bool bitSoftwarehandshake, uint8_t aui8_channel)
    {return __HAL::init_rs232(aui8_channel,baudrate,bMode,bStoppbits,bitSoftwarehandshake) ;};
	/** close the RS232 interface. */
	inline int16_t close_rs232(uint8_t aui8_channel)
		{__HAL::config_rs232_rx_obj(aui8_channel,0,NULL); return __HAL::config_rs232_tx_obj(aui8_channel,0,NULL,NULL);};
  /**
    set the RS232 Baudrate
    @param baudrate wanted baudrate
    @return HAL_NO_ERR -> o.k. else baudrate setting incorrect
  */
  inline int16_t setRs232Baudrate(uint32_t baudrate, uint8_t aui8_channel)
    {return __HAL::set_rs232_baudrate(aui8_channel,baudrate) ;};
  /**
    get the amount of data [uint8_t] in receive buffer
    @return receive buffer data byte
  */
  inline int16_t getRs232RxBufCount(uint8_t aui8_channel)
    {return __HAL::get_rs232_rx_buf_count(aui8_channel);};
  /**
    get the amount of data [uint8_t] in send buffer
    @return send buffer data byte
  */
  inline int16_t getRs232TxBufCount(uint8_t aui8_channel)
    {return __HAL::get_rs232_tx_buf_count(aui8_channel);};
  /**
    configure a receive buffer and set optional irq function pointer for receive
    @param wBuffersize wanted buffer size
    @param pFunction pointer to irq function or NULL if not wanted
  */
  inline int16_t configRs232RxObj(uint16_t wBuffersize,void (*pFunction)(byte *bByte), uint8_t aui8_channel)
    {return __HAL::config_rs232_rx_obj(aui8_channel,wBuffersize,pFunction) ;};
  /**
    configure a send buffer and set optional irq function pointer for send
    @param wBuffersize wanted buffer size
    @param funktionAfterTransmit pointer to irq function or NULL if not wanted
    @param funktionBeforTransmit pointer to irq function or NULL if not wanted
  */
  inline int16_t configRs232TxObj(uint16_t wBuffersize,void (*funktionAfterTransmit)(byte *bByte),
                                  void (*funktionBeforTransmit)(byte *bByte), uint8_t aui8_channel)
    {return __HAL::config_rs232_tx_obj(aui8_channel,wBuffersize,funktionAfterTransmit,funktionBeforTransmit);};
  /**
    get errr code of BIOS
    @return 0=parity, 1=stopbit framing error, 2=overflow
  */
  inline int16_t getRs232Error(uint8_t *Errorcode, uint8_t aui8_channel)
    {return __HAL::get_rs232_error(aui8_channel,Errorcode);};

  /**
    read single int8_t from receive buffer
    @param pbRead pointer to target data
    @return HAL_NO_ERR -> o.k. else buffer underflow
  */
  inline int16_t getRs232Char(uint8_t *pbRead, uint8_t aui8_channel)
    {return __HAL::get_rs232_char(aui8_channel,pbRead);};
  /**
    read bLastChar terminated string from receive buffer
    @param pbRead pointer to target data
    @param bLastChar terminating char
    @return HAL_NO_ERR -> o.k. else buffer underflow
  */
  inline int16_t getRs232String(uint8_t *pbRead,uint8_t bLastChar, uint8_t aui8_channel)
    {return __HAL::get_rs232_string(aui8_channel,pbRead,bLastChar);};

  /**
    send single uint8_t on RS232
    @param bByte data uint8_t to send
    @return HAL_NO_ERR -> o.k. else send buffer overflow
  */
  inline int16_t put_rs232Char(uint8_t bByte, uint8_t aui8_channel)
    {return __HAL::put_rs232_char(aui8_channel,bByte);};
  /**
    send string of n uint8_t on RS232
    @param bpWrite pointer to source data string
    @param wNumber number of data uint8_t to send
    @return HAL_NO_ERR -> o.k. else send buffer overflow
  */
  inline int16_t put_rs232NChar(const uint8_t *bpWrite,uint16_t wNumber, uint8_t aui8_channel)
    {return __HAL::put_rs232_n_char(aui8_channel,(uint8_t*)bpWrite,wNumber);};
  /**
    send '\\0' terminated string on RS232
    @param pbString pointer to '\\0' terminated (!) source data string
    @return HAL_NO_ERR -> o.k. else send buffer overflow
  */
  inline int16_t put_rs232String(const uint8_t *pbString, uint8_t aui8_channel)
    {return __HAL::put_rs232_string(aui8_channel,(uint8_t*)pbString);};

  /**
    clear receive buffer
  */
  inline void clearRs232RxBuffer(uint8_t aui8_channel)
    {__HAL::clear_rs232_rx_buffer(aui8_channel);};
  /**
    clear send buffer
  */
  inline void clearRs232TxBuffer(uint8_t aui8_channel)
    {__HAL::clear_rs232_tx_buffer(aui8_channel);};
  /*@}*/
}
#endif
