/*
  rs232io_c.cpp:
    source for RS232IO_c object for serial communication (Rs232)

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <IsoAgLib/isoaglib_config.h>
#include <stdio.h>
#include <ctype.h>
#include <IsoAgLib/hal/hal_system.h>
#include <IsoAgLib/driver/system/impl/system_c.h>
#include <IsoAgLib/util/impl/util_funcs.h>

#include "rs232io_c.h"

#if (defined(__TSW_CPP__) && !defined(__TSW_CPP_70__) && !defined(__TSW_CPP_756__))  || defined( __TMS320C2000__ )
  #define isspace(_c) ((_c)&(0x1|0x4|'\r'|'\n'))
#else
  #include <cctype>
#endif

#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif


// Begin Namespace __IsoAgLib
namespace __IsoAgLib {
/** C-style function, to get access to the unique RS232IO_c singleton instance
 * if more than one RS232 channel is used for IsoAgLib, an index must be given to select the wanted channel
 */
RS232IO_c &getRs232Instance( uint8_t aui8_instance )
{
  MACRO_MULTITON_GET_INSTANCE_BODY(RS232IO_c, RS232_INSTANCE_CNT, aui8_instance);
}

/*******************************************/
/** definition of public element functions */
/*******************************************/

RS232IO_c::RS232IO_c( void )
  : mui32_baudrate (BAUDERATE_CONTRUCTOR_DEFAULT_VALUE)
  , men_dataMode (_8_N_1)
  , mb_xon_xoff (false)
  , mui16_sndBuf (0)
  , mui16_recBuf (0)
  , mui8_channel (CHANNEL_CONTRUCTOR_DEFAULT_VALUE)
{};


void
RS232IO_c::close()
{
  if ( mui8_channel != CHANNEL_CONTRUCTOR_DEFAULT_VALUE ) HAL::close_rs232( mui8_channel );
}


bool
RS232IO_c::init(
    uint32_t aui32_baudrate,
    t_dataMode ren_dataMode,
    bool ab_xonXoff,
    uint16_t aui16_sndBuf,
    uint16_t aui16_recBuf,
    uint8_t aui8_channel)
{
  bool b_result;
  // check the configuration informations
  bool b_baudAllowed = false,
       b_dataModeAllowed = false;

  // check if aui32_baudrate is one of the allowed settings
  uint32_t pi32_allowed[] = HAL_RS232_BAUDRATE_LIST;
  for (uint8_t ui8_index = 0; ui8_index < HAL_RS232_BITRATE_CNT; ui8_index++)
  {
    if (pi32_allowed[ui8_index] == aui32_baudrate)
    { // given baudrate is in allowed list at position ui8_index
      b_baudAllowed = true; // store success state
      break; // exit search loop
    }
  }

  uint8_t b_dataParityVal = (ren_dataMode >> 4);
  uint8_t b_stopBit = (ren_dataMode & 0xF);
  if ((b_dataParityVal > 0) && (b_dataParityVal < 6))
  { // setting for data bit and parity is correct
    // now check if stop bis are set to 1 or 2
    b_dataModeAllowed = ((b_stopBit == 1) || (b_stopBit == 2));
  }
  // stop RS232 interface if configured before
  if ( mui32_baudrate != BAUDERATE_CONTRUCTOR_DEFAULT_VALUE )
  { // no more initial value
    if ( mui8_channel != CHANNEL_CONTRUCTOR_DEFAULT_VALUE )
      HAL::close_rs232( mui8_channel );
  }

  // set error state if one of b_baudAllowed and b_dataModeAllowed is false
  // and init hardware if everything is accepted
  if ( ((b_baudAllowed) && (b_dataModeAllowed))
     &&(HAL::init_rs232(aui32_baudrate, b_dataParityVal, b_stopBit, ab_xonXoff, aui8_channel) == HAL_NO_ERR)
      )
  { // o.k.
    // store configuration values
    men_dataMode = ren_dataMode;
    mb_xon_xoff = ab_xonXoff;
    mui16_sndBuf = aui16_sndBuf;
    mui16_recBuf = aui16_recBuf;

    b_result = true;
    // now init buffers
    if (HAL::configRs232TxObj(mui16_sndBuf, NULL, NULL, aui8_channel) != HAL_NO_ERR) b_result = false;
    if (HAL::configRs232RxObj(mui16_recBuf, NULL, aui8_channel) != HAL_NO_ERR) b_result = false;

    if (b_result)
    {
      // Only here do we store the values, as these two fields help us know whether the device is already initialized
      mui32_baudrate = aui32_baudrate;
      mui8_channel = aui8_channel;
    }
  }
  else
  { //wrong values given
    b_result = false;
  }

  return b_result;
}


bool RS232IO_c::setBaudrate(uint32_t aui32_baudrate)
{
  // check the configuration informations
  bool b_baudAllowed = false;

  // check if aui32_baudrate is one of the allowed settings
  uint32_t pi32_allowed[] = HAL_RS232_BAUDRATE_LIST;
  for (uint8_t ui8_index = 0; ui8_index < HAL_RS232_BITRATE_CNT; ui8_index++)
  {
    if (pi32_allowed[ui8_index] == aui32_baudrate)
    { // given baudrate is in allowed list at position ui8_index
      b_baudAllowed = true; // store success state
      break; // exit search loop
    }
  }

  // now set the baudrate if allowed
  if ((b_baudAllowed) && (HAL::setRs232Baudrate(aui32_baudrate, mui8_channel) == HAL_NO_ERR))
  { // everything o.k.
    mui32_baudrate = aui32_baudrate;
  }
  else
  { // wrong setting
    b_baudAllowed = false;
  }
  return b_baudAllowed;
}


bool RS232IO_c::setSndBufferSize(uint16_t aui16_bufferSize)
{
  if (HAL::configRs232TxObj(aui16_bufferSize, NULL, NULL, mui8_channel) == HAL_NO_ERR)
  { // send buffer successful created
    mui16_sndBuf = aui16_bufferSize;
    return true;
  }
  else
  {
    return false;
  }
}


bool RS232IO_c::setRecBufferSize(uint16_t aui16_bufferSize)
{
  if (HAL::configRs232RxObj(aui16_bufferSize, NULL, mui8_channel) == HAL_NO_ERR)
  { // receive buffer successful created
    mui16_recBuf = aui16_bufferSize;
    return true;
  }
  else
  {
    return false;
  }

}


  /* ******************************** */
  /* iostream related output operator */
  /* ********    sending     ******** */
  /* ******************************** */

  void RS232IO_c::send(const uint8_t* apb_data, uint8_t aui8_len)
  {
    isoaglib_assert( isInitialized() );

    uint16_t ui16_maxSendItemSize;
    uint16_t ui16_startSendPos = 0,
            ui16_restLen = aui8_len;  

    while ( ui16_restLen > 0 )
    { // send max item

      ui16_maxSendItemSize = mui16_sndBuf - HAL::getRs232TxBufCount(mui8_channel);    

      // restrict actual max item size to waiting chars to send
      if ( ui16_maxSendItemSize > ui16_restLen ) ui16_maxSendItemSize = ui16_restLen;
      // send actual item
      if( HAL::put_rs232NChar((apb_data + ui16_startSendPos), ui16_maxSendItemSize, mui8_channel) != HAL_NO_ERR) {
        isoaglib_assert( !"Impossible RS232 Overflow. Gratulation!" );
        return;
      }
      // update ui8_restLen
      ui16_restLen -= ui16_maxSendItemSize;
      // update start send pos
      ui16_startSendPos += ui16_maxSendItemSize;
    }
  }


  RS232IO_c& RS232IO_c::operator<<(const char *const apc_data)
  {
    send( (uint8_t*)(apc_data), (uint8_t)CNAMESPACE::strlen( apc_data ) );
    return *this;
  }


  RS232IO_c& RS232IO_c::operator<<(uint8_t ab_data)
  {
    isoaglib_assert( isInitialized() );
    (void)HAL::put_rs232Char(ab_data, mui8_channel);
    return *this;
  }


  RS232IO_c& RS232IO_c::operator<<(int8_t ac_data)
  {
    int16_t i16_val (ac_data);
    char pc_data[5];
    // sprintf print value as text to uint8_t string and terminate it with '\0'
    CNAMESPACE::sprintf(pc_data, "%hi", i16_val);

    return RS232IO_c::operator<<(pc_data);
  }


  RS232IO_c& RS232IO_c::operator<<(uint16_t aui16_data)
  {
    char pc_data[10];
    // sprintf print value as text to uint8_t string and terminate it with '\0'
    CNAMESPACE::sprintf(pc_data, "%hu", aui16_data);

    return operator<<(pc_data);
  }


  RS232IO_c& RS232IO_c::operator<<(int16_t ai16_data)
  {
    char pc_data[10];
    // sprintf print value as text to uint8_t string and terminate it with '\0'
    CNAMESPACE::sprintf(pc_data, "%hi", ai16_data);

    return operator<<(pc_data);
  }


  RS232IO_c& RS232IO_c::operator<<(uint32_t aui32_data)
  {
    char pc_data[20];
    const long unsigned int t = aui32_data;
    // sprintf print value as text to uint8_t string and terminate it with '\0'
    CNAMESPACE::sprintf(pc_data, "%lu", t );

    return operator<<(pc_data);
  }


  RS232IO_c& RS232IO_c::operator<<(int32_t ai32_data)
  {
    char pc_data[20];
    #ifdef SYSTEM_PC
    // sprintf print value as text to uint8_t string and terminate it with '\0'
    // variant for 32Bit CPU
    CNAMESPACE::sprintf(pc_data, "%d", ai32_data);
    #else
    // sprintf print value as text to uint8_t string and terminate it with '\0'
    // variant for 16Bit CPU where int has size of int16_t --> only long int has 32 Bit
    CNAMESPACE::sprintf(pc_data, "%ld", ai32_data);
    #endif

    return operator<<(pc_data);
  }


RS232IO_c& RS232IO_c::operator<<(float af_data)
{
  char pc_data[20];
  // sprintf print value as text to uint8_t string and terminate it with '\0'
  CNAMESPACE::sprintf(pc_data, "%f", af_data);
  // change use float format to german
  *(CNAMESPACE::strstr((char*)pc_data, ".")) = ',';

  return operator<<(pc_data);
}


/* ******************************** */
/* iostream related output operator */
/* ********   receiving    ******** */
/* ******************************** */

void RS232IO_c::receive(uint8_t* pb_data, uint16_t aui16_len)
{
  isoaglib_assert( isInitialized() );

  uint8_t* pb_writer = pb_data;
  uint16_t ui16_ind = 0;
  for (; ui16_ind < aui16_len; ui16_ind++)
  {
    if (eof())
    { // error - buffer empty before reading wanted count of data
      break;
    }
    HAL::getRs232Char(pb_writer, mui8_channel);
    pb_writer++;
  }
}


int16_t RS232IO_c::getLine( uint8_t* pui8_data, uint8_t ui8_lastChar )
{
  return HAL::getRs232String( pui8_data, ui8_lastChar, mui8_channel);
}


RS232IO_c& RS232IO_c::operator>>(uint8_t* pb_data)
{
  HAL::getRs232String(pb_data, '\n', mui8_channel);
  return *this;
}


RS232IO_c& RS232IO_c::operator>>(uint8_t& b_data)
{
  readToken(); // it set rs232_underflow error if no data is read
  uint16_t ui16_val;
  CNAMESPACE::sscanf(mpc_token, "%hu", &ui16_val);
  b_data = static_cast<uint8_t>(ui16_val);
  return *this;
}


RS232IO_c& RS232IO_c::operator>>(int8_t& c_data)
{
  readToken(); // it set rs232_underflow error if no data is read
  int16_t i16_val;
  CNAMESPACE::sscanf(mpc_token, "%hi", &i16_val);
  c_data = static_cast<int8_t>(i16_val);

  return *this;
}


RS232IO_c& RS232IO_c::operator>>(uint16_t& ui16_data)
{
  readToken(); // it set rs232_underflow error if no data is read
  CNAMESPACE::sscanf(mpc_token, "%hu", &ui16_data);
  return *this;
}


RS232IO_c& RS232IO_c::operator>>(int16_t& i16_data)
{
  readToken(); // it set rs232_underflow error if no data is read
  CNAMESPACE::sscanf(mpc_token, "%hi", &i16_data);
  return *this;
}


RS232IO_c& RS232IO_c::operator>>(uint32_t& ui32_data)
{
  readToken(); // it set rs232_underflow error if no data is read
  #ifdef SYSTEM_PC
  // variant for 32Bit CPU
  CNAMESPACE::sscanf(mpc_token, "%u", &ui32_data);
  #else
  // variant for 16Bit CPU where int has size of int16_t --> only long int has 32 Bit
  CNAMESPACE::sscanf(mpc_token, "%lu", &ui32_data);
  #endif
  return *this;
}


RS232IO_c& RS232IO_c::operator>>(int32_t& i32_data)
{
  readToken(); // it set rs232_underflow error if no data is read
  #ifdef SYSTEM_PC
  // variant for 32Bit CPU
  CNAMESPACE::sscanf(mpc_token, "%i", &i32_data);
  #else
  // variant for 16Bit CPU where int has size of int16_t --> only long int has 32 Bit
  CNAMESPACE::sscanf(mpc_token, "%li", &i32_data);
  #endif
  return *this;
}


RS232IO_c& RS232IO_c::operator>>(float& f_data)
{
  readToken(); // it set rs232_underflow error if no data is read
  CNAMESPACE::sscanf(mpc_token, "%f", &f_data);
  return *this;
}


void RS232IO_c::readToken()
{
  uint8_t b_data = 0;
  uint8_t ui8_ind = 0;

  // first eat white space (including \t,\n) - stop if buffer is empty
  for (HAL::getRs232Char(&b_data, mui8_channel); (isspace(b_data) && (eof()));
       HAL::getRs232Char(&b_data, mui8_channel));


  // if buffer is empty, overwrite old data, set rs232_underflow error and exit
  if (eof())
  { // overwrite old token data with '\0'
    for (ui8_ind = 0; ui8_ind < 12; ui8_ind++)
      mpc_token[ui8_ind] = '\0';
    return;
  }

  // now b_data is a not whitespace byte
  mpc_token[0] = b_data; // store it
  for (ui8_ind = 1; ((ui8_ind < 12) && (eof())); ui8_ind++)
  {
    HAL::getRs232Char(&b_data, mui8_channel);
    if (isspace(b_data))break; // break loop on whitespace
    mpc_token[ui8_ind] = b_data;
  }

  // set lasting positions in mpc_token to '\0'
  for (;(ui8_ind < 12); ui8_ind++) mpc_token[ui8_ind] = '\0';
}

} // end of namespace __IsoAgLib
