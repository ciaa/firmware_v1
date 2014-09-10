/*
  target_extensions_rs232_simulating.cpp: source for PC specific extensions for the HAL for RS232

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "rs232_target_extensions.h"
#include <IsoAgLib/hal/pc/system/system.h>
#include <cstdio>
#include <cstring>

#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif

using namespace std; // simple version to avoid problems with using CNAMESPACE


namespace __HAL {

/* **************************************** */
/* ****** RS232 I/O BIOS functions  ******* */
/* **************************************** */
typedef FILE* FilePointer_t;
static FilePointer_t rs232_output[RS232_CHANNEL_CNT];

#ifdef CONFIG_HAL_PC_RS232_WRITE_LOG_FILE
  static FilePointer_t rs232_log[RS232_CHANNEL_CNT];
#endif
static char sendName[RS232_CHANNEL_CNT][200];

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
  if (rs232_output[aui8_channel]) fclose( rs232_output[aui8_channel] );
  #ifdef CONFIG_HAL_PC_RS232_WRITE_LOG_FILE
  if (rs232_log[aui8_channel]) fclose( rs232_log[aui8_channel] );
  #endif
  #ifdef WIN32
  sprintf(sendName[aui8_channel], "rs232_send_%hd", aui8_channel);
  rs232_output[aui8_channel] = fopen(sendName[aui8_channel], "w"); // "a+"
  #else
  sprintf(sendName[aui8_channel], "../../../simulated_io/rs232_send_%hd", aui8_channel);
  rs232_output[aui8_channel] = fopen(sendName[aui8_channel], "w"); // "a+"
  // BEGIN: Added by M.Wodok 6.12.04
  if (rs232_output[aui8_channel] == NULL) {
    // try opening in current directory...
    sprintf(sendName[aui8_channel], "rs232_send_%hd", aui8_channel);
    rs232_output[aui8_channel] = fopen(sendName[aui8_channel], "w"); // "a+"

    printf("init_rs232 aufgerufen mit %d Baud, Modus %hd, %hd Stop-Bits, XON/XOFF %d, Channel %hd, Ausgabedatei %s\n",
          baudrate,bMode, bStoppbits, bitSoftwarehandshake, aui8_channel, sendName[aui8_channel]);
  }
  else
  {
    printf("init_rs232 aufgerufen mit %d Baud, Modus %hd, %hd Stop-Bits, XON/XOFF %d, Channel %hd, Ausgabedatei %s\n",
          baudrate,bMode, bStoppbits, bitSoftwarehandshake, aui8_channel, sendName[aui8_channel]);
  }
  // END: Added by M.Wodok 6.12.04
  #endif

  #ifdef CONFIG_HAL_PC_RS232_WRITE_LOG_FILE
  char name[50];
  #ifdef WIN32
  sprintf(name, "rs232_log_%hd", aui8_channel);
  #else
  sprintf(sendName[aui8_channel], "../../../simulated_io/rs232_log_%hd", aui8_channel);
  #endif
  rs232_log[aui8_channel] = fopen(name, "w"); // "a+"
  // BEGIN: Added by M.Wodok 6.12.04
  if (rs232_log[aui8_channel] == NULL) {
    // try opening in current directory...
    strcpy(name, "rs232_log");
    rs232_log[aui8_channel] = fopen(name, "w"); // "a+"
  }
  printf("Versuch Datei mit Name %s zum schreiben zu oeffnen\n", name);
  #endif
  return HAL_NO_ERR;
}

/** close the RS232 interface. */
int16_t close_rs232(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  if (rs232_output[aui8_channel]) fclose( rs232_output[aui8_channel] );
  #ifdef CONFIG_HAL_PC_RS232_WRITE_LOG_FILE
  if (rs232_log[aui8_channel]) fclose( rs232_log[aui8_channel] );
  #endif
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
  printf("RS232 Baudrate auf %d gestellt\n", baudrate);
  return HAL_NO_ERR;
}
/**
  get the amount of data [uint8_t] in receive buffer
  @return receive buffer data byte
*/
int16_t getRs232RxBufCount(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  return 2;
}
/**
  get the amount of data [uint8_t] in send buffer
  @return send buffer data byte
*/
int16_t getRs232TxBufCount(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  fclose( rs232_output[aui8_channel] );
  rs232_output[aui8_channel] = fopen(sendName[aui8_channel], "a+"); // "a+"
  return 0;
}
/**
  configure a receive buffer and set optional irq function pointer for receive
  @param wBuffersize wanted buffer size
  @param pFunction pointer to irq function or NULL if not wanted
*/
int16_t configRs232RxObj(uint16_t wBuffersize,void (*pFunction)(uint8_t *bByte), uint8_t aui8_channel)
{
  (void)pFunction;

  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  printf("configRs232RxObj aufgerufen mit empfang buffersize %d \n", wBuffersize);
//  return (pFunction!=NULL)?HAL_NO_ERR:HAL_CONFIG_ERR;
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
  (void)funktionBeforTransmit;
  (void)funktionAfterTransmit;

  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  printf("configRs232TxObj aufgerufen mit sende buffersize %d \n", wBuffersize);
//  return ((funktionAfterTransmit!=NULL)&&(funktionBeforTransmit!=NULL))?HAL_NO_ERR:HAL_CONFIG_ERR;
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
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  int32_t i32_time = getTime();
  *pbRead = ((uint8_t*)&i32_time)[3];
  #ifdef CONFIG_HAL_PC_RS232_WRITE_LOG_FILE
  fprintf(rs232_log[aui8_channel], "%d read %c\n", getTime(), *pbRead);
  #endif
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
  (void)bLastChar;
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  int32_t i32_time = getTime();
  memcpy(pbRead, ((uint8_t*)&i32_time), 4);
  pbRead[4] = '\0';
  return HAL_NO_ERR;
}

/**
  send single uint8_t on RS232
  @param bByte data uint8_t to send
  @return HAL_NO_ERR -> o.k. else send buffer overflow
*/
int16_t put_rs232Char(uint8_t bByte, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  // printf("RS232:\n %c\n", bByte);
  fprintf(rs232_output[aui8_channel], "%c", bByte);
  fflush( rs232_output[aui8_channel] );

  #ifdef CONFIG_HAL_PC_RS232_WRITE_LOG_FILE
  fprintf(rs232_log[aui8_channel], "%d write %c\n", getTime(), bByte);
  #endif
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
  uint8_t ui8_ind = 0;
  // printf("RS232: ");
//  for (; ui8_ind < wNumber; ui8_ind++)
//  {
//    putchar(bpWrite[ui8_ind]);
//    putc(bpWrite[ui8_ind], rs232_output[aui8_channel]);
//  }
//  printf("\n");
  #ifdef CONFIG_HAL_PC_RS232_WRITE_LOG_FILE
  fprintf(rs232_log[aui8_channel], "%d write ", getTime());
  #endif
  for (ui8_ind = 0; ui8_ind < wNumber; ui8_ind++)
  {
    fprintf(rs232_output[aui8_channel], "%c", bpWrite[ui8_ind]);
    fflush( rs232_output[aui8_channel] );
    #ifdef CONFIG_HAL_PC_RS232_WRITE_LOG_FILE
    fprintf(rs232_log[aui8_channel], "%c", bpWrite[ui8_ind]);
    #endif
  }
  #ifdef CONFIG_HAL_PC_RS232_WRITE_LOG_FILE
  fprintf(rs232_log[aui8_channel], "\n");
  #endif
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
  // printf("RS232:: %s", pbString);
  fprintf(rs232_output[aui8_channel], "%s", pbString);
  fflush( rs232_output[aui8_channel] );
  return HAL_NO_ERR;
}

/**
  clear receive buffer
*/
void clearRs232RxBuffer(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return;
  printf("empfangsbuffer gecleared\n");
};

/**
  clear send buffer
*/
void clearRs232TxBuffer(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return;
  printf("sendebuffer gecleared\n");
}

} // End of name space __HAL
