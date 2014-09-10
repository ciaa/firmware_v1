/*
  target_extension_rs232_w32_sys.cpp

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


/** close the RS232 interface. */
int16_t close_rs232(uint8_t comport)
{
  if ( comport >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  if ( arr_usedPort[comport] )
  {
    SetCommState(hCom[comport],&(oldConfig[comport]));
    CloseHandle(hCom[comport]);
    arr_usedPort[comport] = false;
    return HAL_NO_ERR;
  }
  else
  {
    return HAL_NOACT_ERR;
  }
}

void close_rs232() {
  for ( int comport = 0; comport < RS232_CHANNEL_CNT; comport++)
  {
    if ( arr_usedPort[comport] )
    {
      SetCommState(hCom[comport],&(oldConfig[comport]));
      CloseHandle(hCom[comport]);
      arr_usedPort[comport] = false;
    }
  }
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
int16_t init_rs232(uint32_t baudrate,uint8_t bMode,uint8_t bStoppbits,bool bitSoftwarehandshake, uint8_t comport)
{
  if ( comport >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;

  DCB dcb;
  COMMTIMEOUTS ct;
  char com[] = "COMx";
  com[3] = comport + '1'; // comport starts counting at 0, while the device starts at COM1
  // first close if already configured
  close_rs232(comport);

  hCom[comport] = CreateFile(com,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
  if (hCom[comport]==INVALID_HANDLE_VALUE) return HAL_CONFIG_ERR;

  // read old config
  if (!GetCommState(hCom[comport],&(oldConfig[comport]))) return HAL_CONFIG_ERR;
  arr_usedPort[comport] = true;

  atexit(close_rs232);

  if (!SetupComm(hCom[comport],1024,1024)) return HAL_CONFIG_ERR;
  if (!GetCommState(hCom[comport],&dcb)) return HAL_CONFIG_ERR;

  dcb.BaudRate    = baudrate;
  dcb.fDtrControl = DTR_CONTROL_DISABLE;

  switch ( bMode )
  {
    case __HAL::DATA_8_BITS_NOPARITY: // no parity
      dcb.Parity      = NOPARITY;
      break;
    case __HAL::DATA_7_BITS_EVENPARITY:
    case __HAL::DATA_8_BITS_EVENPARITY:
      // even parity
      dcb.Parity      = EVENPARITY;
      break;
    default: // odd parity
      dcb.Parity      = ODDPARITY;
      break;
  }
  switch ( bMode )
  {
    case __HAL::DATA_7_BITS_EVENPARITY:
    case __HAL::DATA_7_BITS_ODDPARITY:
      dcb.ByteSize    = 7;
      break;
    default:
      dcb.ByteSize    = 8;
      break;
  }


  if ( bStoppbits == 2 )
	  dcb.StopBits    = TWOSTOPBITS;
  else
	  dcb.StopBits    = ONESTOPBIT;

  if ( bitSoftwarehandshake )
	  dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
  else
	  dcb.fRtsControl = RTS_CONTROL_ENABLE;

  if (!SetCommState(hCom[comport],&dcb)) return HAL_CONFIG_ERR;
  if (!GetCommTimeouts(hCom[comport],&ct)) return HAL_CONFIG_ERR;

  ct.ReadIntervalTimeout = MAXDWORD;
  ct.ReadTotalTimeoutMultiplier = 0;
  ct.ReadTotalTimeoutConstant = 0;
  ct.WriteTotalTimeoutMultiplier = 0;
  ct.WriteTotalTimeoutConstant = 0;
  if (!SetCommTimeouts(hCom[comport],&ct)) return HAL_CONFIG_ERR;

  // wait some time to avoid buffer error
  Sleep(10);

  // reset read buffer
  deq_readBuff[comport].clear();

  return HAL_NO_ERR;
}

/**
  set the RS232 Baudrate
  @param baudrate wanted baudrate
  @return HAL_NO_ERR -> o.k. else baudrate setting incorrect
 */
int16_t setRs232Baudrate(uint32_t baudrate, uint8_t comport)
{
  if ( comport >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  DCB dcb;
  char com[] = "COMx";
  com[3] = comport + '1'; // comport starts counting at 0, while the device starts at COM1
  hCom[comport] = CreateFile(com,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
  if (!hCom[comport]) return HAL_CONFIG_ERR;

  atexit(close_rs232);

  if (!SetupComm(hCom[comport],1024,1024)) return HAL_CONFIG_ERR;
  if (!GetCommState(hCom[comport],&dcb)) return HAL_CONFIG_ERR;
  dcb.BaudRate    = baudrate;
  if (!SetCommState(hCom[comport],&dcb)) return HAL_CONFIG_ERR;

  return HAL_NO_ERR;
}

BOOL WriteABuffer(HANDLE &r_hCom, const uint8_t * lpBuf, DWORD dwToWrite)
{
     OVERLAPPED osWrite = {0};
     DWORD dwWritten;
     DWORD dwRes;
     BOOL fRes;
     // Create this write operation's OVERLAPPED structure's hEvent.
     osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
     if (osWrite.hEvent == NULL) // error creating overlapped event handle
       return FALSE;
     // Issue write.
     if (!WriteFile(r_hCom, lpBuf, dwToWrite, &dwWritten, &osWrite))
     {
       if (GetLastError() != ERROR_IO_PENDING)
       { // WriteFile failed, but isn't delayed. Report error and abort.
         fRes = FALSE;
       }
       else // Write is pending.
         dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
       switch(dwRes) { // OVERLAPPED structure's event has been signaled.
         case WAIT_OBJECT_0:
           if (!GetOverlappedResult(r_hCom, &osWrite, &dwWritten, FALSE)) fRes = FALSE;
           else // Write operation completed successfully.
             fRes = TRUE;
           break;
         default: // An error has occurred in WaitForSingleObject.
           // This usually indicates a problem with the
           // OVERLAPPED structure's event handle.
           fRes = FALSE;
           break;
       }
     }
     else // WriteFile completed immediately.
       fRes = TRUE;
     CloseHandle(osWrite.hEvent);
     return fRes;
}

/**
  send single uint8_t on RS232
  @param bByte data uint8_t to send
  @return HAL_NO_ERR -> o.k. else send buffer overflow
 */
int16_t put_rs232Char(uint8_t bByte, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  uint8_t b_data = bByte;

  return WriteABuffer(hCom[aui8_channel], &b_data, 1)?HAL_NO_ERR:HAL_NOACT_ERR;
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

  return WriteABuffer(hCom[aui8_channel], bpWrite, wNumber)?HAL_NO_ERR:HAL_NOACT_ERR;
}

/**
  send '\0' terminated string on RS232
  @param pbString pointer to '\0' terminated (!) source data string
  @return HAL_NO_ERR -> o.k. else send buffer overflow
 */
int16_t put_rs232String(const uint8_t *pbString, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;

  return WriteABuffer(hCom[aui8_channel], pbString, strlen((const char*)pbString))?HAL_NO_ERR:HAL_NOACT_ERR;
}

/**
  get the amount of data [uint8_t] in receive buffer
  @return receive buffer data byte
 */
int16_t getRs232RxBufCount(uint8_t comport)
{
  if ( comport >= RS232_CHANNEL_CNT ) return 0;
  int8_t c_temp[300];
  DWORD tempLen;
  ReadFile(hCom[comport],c_temp,299,&tempLen,NULL);

  for ( uint16_t ind = 0; ind < tempLen; ind++ ) deq_readBuff[comport].push_back( c_temp[ind] );

  return deq_readBuff[comport].size();
}

/**
  read bLastChar terminated string from receive buffer
  @param pbRead pointer to target data
  @param bLastChar terminating char
  @return HAL_NO_ERR -> o.k. else buffer underflow
 */
int16_t getRs232String(uint8_t *pbRead,uint8_t ui8_terminateChar, uint8_t comport)
{
  if ( comport >= RS232_CHANNEL_CNT ) return 0;
  getRs232RxBufCount(comport);
  if (! deq_readBuff[comport].empty())
  {
    for ( STL_NAMESPACE::deque<int8_t>::iterator iter = deq_readBuff[comport].begin(); iter != deq_readBuff[comport].end(); ++iter )
    { // check if terminating char is found
      if ( *iter == ui8_terminateChar )
      { // found -> copy area from begin to iterator
        uint16_t ind = 0;
        for ( ; (deq_readBuff[comport].front() != ui8_terminateChar); ind++ )
        {
          pbRead[ind] = deq_readBuff[comport].front();
          deq_readBuff[comport].pop_front();
        }
        // lastly pop the termination char and terminate the result string
        deq_readBuff[comport].pop_front();
        pbRead[ind] = '\0';
        return HAL_NO_ERR;
      }
    }
  }
  return HAL_NOACT_ERR;
}

/**
  read single int8_t from receive buffer
  @param pbRead pointer to target data
  @return HAL_NO_ERR -> o.k. else buffer underflow
 */
int16_t getRs232Char(uint8_t *pbRead, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  getRs232RxBufCount(aui8_channel);
  if (! deq_readBuff[aui8_channel].empty())
  {
    pbRead[0] = deq_readBuff[aui8_channel].front();
    deq_readBuff[aui8_channel].pop_front();
    return HAL_NO_ERR;
  }
  else
  {
    return HAL_NOACT_ERR;
  }
}

/**
  get the amount of data [uint8_t] in send buffer
  @return send buffer data byte
 */
int16_t getRs232TxBufCount(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  /** @todo ON REQUEST: decide if RS232 TX buffer from OS should be asked */
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
  /** @todo ON REQUEST: should this be implemented? */
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
  /** @todo ON REQUEST: should this be implemented? */
  return HAL_NO_ERR;
}

/**
  get errr code of BIOS
  @return 0=parity, 1=stopbit framing error, 2=overflow
 */
int16_t getRs232Error(uint8_t *Errorcode, uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return HAL_RANGE_ERR;
  /** @todo ON REQUEST: should this be implemented? */
  return HAL_NO_ERR;
}

/**
  clear receive buffer
 */
void clearRs232RxBuffer(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return;
  /** @todo ON REQUEST: should this be implemented? */
};

/**
  clear send buffer
 */
void clearRs232TxBuffer(uint8_t aui8_channel)
{
  if ( aui8_channel >= RS232_CHANNEL_CNT ) return;
  /** @todo ON REQUEST: should this be implemented? */
}

} // end of namespace __HAL
