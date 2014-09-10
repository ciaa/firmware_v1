/*
  target_extensions_rs232_rte.cpp: source for PC specific extensions
    for the HAL for RS232

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#define  STRICT

#define RTE_CLIENT
#include <rte_client.h>   // be an RTE client
#include <rte_serial.h>

#include "rs232_target_extensions.h"
#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/pc/system/system.h>
#include <cstdio>
#include <cstring>
#include <deque>
#include <cstdlib>


using namespace std; // simple version to avoid problems with using CNAMESPACE


namespace __HAL {

/* **************************************** */
/* ****** RS232 I/O BIOS functions  ******* */
/* **************************************** */
typedef serial_c* PointerSerial_t;
#if RS232_CHANNEL_CNT == 1
#define DEF_SerialPointer(x) pc_serial
PointerSerial_t pc_serial = NULL;
#elif RS232_CHANNEL_CNT == 2
#define DEF_SerialPointer(x) pc_serial[x]
PointerSerial_t pc_serial[RS232_CHANNEL_CNT] = {NULL,NULL};
#elif RS232_CHANNEL_CNT == 3
#define DEF_SerialPointer(x) pc_serial[x]
PointerSerial_t pc_serial[RS232_CHANNEL_CNT] = {NULL,NULL,NULL};
#else
#define DEF_SerialPointer(x) pc_serial[x]
PointerSerial_t pc_serial[RS232_CHANNEL_CNT];
#endif

STL_NAMESPACE::deque<uint8_t> c_buffer[RS232_CHANNEL_CNT];

/** send handler which is called by RTE on each new received data -> store current fertilizer amount */
int rs232_send_handler(rtd_handler_para_t* para, uint8_t size, const uint8_t *data ) {
  // ignore events from other channel than arr_serialChannels[cui32_fertilizerSerialIndex]
  for ( int testInd = 0; testInd < RS232_CHANNEL_CNT; testInd++ )
  {
    if ( para->rtd_msg->channel != testInd ) continue;
    // append received string in buffer
    #if 0
    INTERNAL_DEBUG_DEVICE << hex << "RS232 Receive Handler: #";
    #endif
//    INTERNAL_DEBUG_DEVICE << "\nRS232 Empfang:";
    for ( uint16_t ind = 0; ind < size; ind++ ) {
      c_buffer[testInd].push_back( data[ind] );
//      INTERNAL_DEBUG_DEVICE << data[ind];
//      INTERNAL_DEBUG_DEVICE << "0x" << int ( data[ind] ) << ", ";
    }
//    INTERNAL_DEBUG_DEVICE << INTERNAL_DEBUG_DEVICE_ENDL;
//    #endif
    #if 0
    INTERNAL_DEBUG_DEVICE << "#" << dec << INTERNAL_DEBUG_DEVICE_ENDL;
    #endif
    return 0;
  }
  // if reaching here -> no channel matched
  return -1;
}


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
int16_t init_rs232(uint32_t baudrate,uint8_t bMode,uint8_t bStoppbits,bool bitSoftwarehandshake, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  char param[30];
  uint16_t ui16_dataBit = 8;
  if ( ( bMode == 1 ) || ( bMode == 3 ) ) ui16_dataBit = 7;
  char par = 'n';
  if      ( ( bMode == 1 ) || ( bMode == 2 ) ) par = 'e';
  else if ( ( bMode == 3 ) || ( bMode == 4 ) ) par = 'o';
  sprintf( param, "C%hd,R,E0,B%d,L%d%c%hd", aui8_channel,baudrate, ui16_dataBit, par, bStoppbits );

  if ( ! rte_is_init() ) {
		std::string c_rteServer = CONFIG_HAL_PC_RTE_DEFAULT_SERVER;
		if ( getenv( "RTE_HOST" ) != NULL ) c_rteServer = getenv( "RTE_HOST" );

    if (rte_connect( c_rteServer.c_str() ) < 0) {
      cerr << "Unable to connect " << c_rteServer << " server." << endl;
      exit(1);
    } else {
      cout << "Connected to " << c_rteServer << " server." << endl;
    }
  }
  if ( DEF_SerialPointer(aui8_channel) == NULL ) DEF_SerialPointer(aui8_channel) = new serial_c;
  DEF_SerialPointer(aui8_channel)->set_channel( 0 );
  DEF_SerialPointer(aui8_channel)->set_send_handler( rs232_send_handler );
  DEF_SerialPointer(aui8_channel)->set_line_parameters( param );
  DEF_SerialPointer(aui8_channel)->set_echo( false );
  return HAL_NO_ERR;
} // soll "C0,R,E0,B4800,L8n1"

/** close the RS232 interface. */
int16_t close_rs232(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
	if ( DEF_SerialPointer(aui8_channel) == NULL ) return HAL_NOACT_ERR;
	delete DEF_SerialPointer(aui8_channel);
	DEF_SerialPointer(aui8_channel) = NULL;
	c_buffer[aui8_channel].clear();
	return HAL_NO_ERR;
}


/**
  set the RS232 Baudrate
  @param baudrate wanted baudrate
  @return HAL_NO_ERR -> o.k. else baudrate setting incorrect
*/
int16_t setRs232Baudrate(uint32_t baudrate, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  DEF_SerialPointer(aui8_channel)->set_baud( baudrate );
  return HAL_NO_ERR;
}
/**
  get the amount of data [uint8_t] in receive buffer
  @return receive buffer data byte
*/
int16_t getRs232RxBufCount(uint8_t aui8_channel)
{
  DEF_SerialPointer(aui8_channel)->poll();
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  return c_buffer[aui8_channel].size();
}
/**
  get the amount of data [uint8_t] in send buffer
  @return send buffer data byte
*/
int16_t getRs232TxBufCount(uint8_t aui8_channel)
{
  DEF_SerialPointer(aui8_channel)->poll();
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  return 0;
}
/**
  configure a receive buffer and set optional irq function pointer for receive
  @param wBuffersize wanted buffer size
  @param pFunction pointer to irq function or NULL if not wanted
*/
int16_t configRs232RxObj(uint16_t wBuffersize,void (*pFunction)(uint8_t *bByte), uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  return HAL_NO_ERR;
}
/**
  configure a send buffer and set optional irq function pointer for send
  @param wBuffersize wanted buffer size
  @param funktionAfterTransmit pointer to irq function or NULL if not wanted
  @param funktionBeforTransmit pointer to irq function or NULL if not wanted
*/
int16_t configRs232TxObj(uint16_t wBuffersize,void (*funktionAfterTransmit)(uint8_t *bByte),
                                void (*funktionBeforTransmit)(uint8_t *bByte), uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  return HAL_NO_ERR;
}
/**
  get errr code of BIOS
  @return 0=parity, 1=stopbit framing error, 2=overflow
*/
int16_t getRs232Error(uint8_t *Errorcode, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  *Errorcode = 0;
  return HAL_NO_ERR;
}

/**
  read single int8_t from receive buffer
  @param pbRead pointer to target data
  @return HAL_NO_ERR -> o.k. else buffer underflow
*/
int16_t getRs232Char(uint8_t *pbRead, uint8_t aui8_channel)
{  // retrieve first char
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  if ( c_buffer[aui8_channel].empty() ) {
    // no char in receive buffer
    *pbRead = '\0';
    return HAL_RANGE_ERR;
  }
  pbRead[0] = c_buffer[aui8_channel][0];
  c_buffer[aui8_channel].pop_front();
  return HAL_NO_ERR;
}
/**
  read bLastChar terminated string from receive buffer
  @param pbRead pointer to target data
  @param bLastChar terminating char
  @return HAL_NO_ERR -> o.k. else buffer underflow
*/
int16_t getRs232String(uint8_t *pbRead,uint8_t bLastChar, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  uint8_t ui8_test;
  if (! c_buffer[aui8_channel].empty())
  {
    for ( STL_NAMESPACE::deque<uint8_t>::iterator iter = c_buffer[aui8_channel].begin(); iter != c_buffer[aui8_channel].end(); ++iter )
    { // check if terminating char is found
      ui8_test = *iter;
      if ( ui8_test == bLastChar )
      { // found -> copy area from begin to iterator
        uint16_t ind = 0;
        for ( ; ( c_buffer[aui8_channel].front() != bLastChar); ind++ )
        {
          pbRead[ind] = c_buffer[aui8_channel].front();
          c_buffer[aui8_channel].pop_front();
        }
        // lastly pop the termination char and terminate the result string
        c_buffer[aui8_channel].pop_front();
        pbRead[ind] = '\0';
        return HAL_NO_ERR;
      }
    }
  }
  return HAL_NOACT_ERR;
}

/**
  send single uint8_t on RS232
  @param bByte data uint8_t to send
  @return HAL_NO_ERR -> o.k. else send buffer overflow
*/
int16_t put_rs232Char(uint8_t bByte, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  DEF_SerialPointer(aui8_channel)->send( 1, &bByte );
  return HAL_NO_ERR;
}
/**
  send string of n uint8_t on RS232
  @param bpWrite pointer to source data string
  @param wNumber number of data uint8_t to send
  @return HAL_NO_ERR -> o.k. else send buffer overflow
*/
int16_t put_rs232NChar(const uint8_t *bpWrite,uint16_t wNumber, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  DEF_SerialPointer(aui8_channel)->send( wNumber, bpWrite );
  return HAL_NO_ERR;
}
/**
  send '\0' terminated string on RS232
  @param pbString pointer to '\0' terminated (!) source data string
  @return HAL_NO_ERR -> o.k. else send buffer overflow
*/
int16_t put_rs232String(const uint8_t *pbString, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  DEF_SerialPointer(aui8_channel)->send( (const char*)pbString );
  return HAL_NO_ERR;
}

/**
  clear receive buffer
*/
void clearRs232RxBuffer(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return;
  c_buffer[aui8_channel].clear();
};

/**
  clear send buffer
*/
void clearRs232TxBuffer(uint8_t aui8_channel)
{
}

} // End of name space __HAL
