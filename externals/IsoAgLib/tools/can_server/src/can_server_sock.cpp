/*
  can_server_sock.cpp: can server communicates with clients through
    sockets message forwarding to other clients

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <cstring>
#include <cstdio>
#include <cctype>

#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"
#include "can_filtering.h"

#ifdef WIN32
  #ifndef WINCE
    #include <ws2tcpip.h>
  #endif
#else
  #include <fcntl.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <sys/un.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <sys/ioctl.h>
  #include <sys/time.h>
  #include <sys/times.h>
  #include <linux/version.h>
#endif

#ifndef WINCE
  #include <sys/stat.h>
#endif

#include <time.h>
#include <errno.h>


#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif


static const uint32_t scui32_selectTimeoutMax = 50000;
static const uint32_t scui32_selectTimeoutMin = 1000;

__HAL::server_c::canBus_s::canBus_s() :
  mui16_globalMask(0),
  mi32_can_device(0),
  mi32_sendDelay(0),
  mb_deviceConnected(false),
  mui16_busRefCnt(0),
  m_logFile(LogFile_c::Null_s()())
{
}

__HAL::server_c::server_c() :
  mb_logMode(false),
  mb_monitorMode(false),
  mb_inputFileMode(false),
  mf_canInput(0),
#ifndef WIN32
  mb_daemon(false),
#endif
  mi16_reducedLoadOnIsoBus(-1),
  mb_interactive(true),
  mi_canReadNiceValue(0),
  mvec_canBus()
{
  memset(marrb_remoteDestinationAddressInUse, 0, sizeof(marrb_remoteDestinationAddressInUse));

  pthread_mutex_init(&mt_protectClientList, NULL);
}

__HAL::client_c::canBus_s::canBus_s() :
  mvec_msgObj(),
  mb_busUsed(false),
  mui16_globalMask(0),
  mui32_globalMask(0),
  mui32_lastMask(0),
  mi32_sendDelay(0),
  mb_initReceived(false)
{
}

__HAL::client_c::client_c() :
  ui16_pid(0),
  i32_msecStartDeltaClientMinusServer(0),
  mvec_canBus()
{
}


namespace __HAL {

#ifndef WIN32
/** linux-begin */
#ifndef LINUX_VERSION_CODE
#error "LINUX_VERSION_CODE is not defined"
#endif
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
/** On Linux 2.4: Define the amount of MSec per Clock_t, in case the project config didn't this before */
#ifndef msecPerClock
#define msecPerClock 10
#define clocksPer100Msec 10
#endif
#endif

clock_t getStartUpTime()
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
  if ( msecPerClock != (1000 / sysconf(_SC_CLK_TCK)) )
  { // BIG PROBLEM -> THE DEFINE DURING COMPILE TIME DOES NOT MATCH THE RUNTIME
    std::cerr << "\n\nVERY BIG PROBLEM!!!\nThis program was compiled with\n#define msecPerClock " << msecPerClock
        << "\nwhile the runtime system has " << (1000 / sysconf(_SC_CLK_TCK))
        << "\n\nSO PLEASE add\n#define msecPerClock " << (1000 / sysconf(_SC_CLK_TCK))
        << "\nto your project configuration header or Makefile, so that a matching binary is built. This program is aborted now, as none of any time calculations will match with this problem.\n\n"
        << std::endl;
    abort();
  }
  static clock_t st_startup4Times = times(NULL);
#else
  static clock_t st_startup4Times = -1;
  if (st_startup4Times < 0)
  {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    st_startup4Times = int32_t(ts.tv_sec)*1000 + int32_t(ts.tv_nsec/1000000);
  }
#endif
  return st_startup4Times;
}
/** linux-end */
#else
int32_t getStartUpTime()
{
  static int32_t st_startup4Times = timeGetTime();
  return st_startup4Times;
}
#endif




#ifdef WIN32
#if defined( _MSC_VER )
  // VC++ with native Win32 API provides very accurate
  // msec timer - use that
  int32_t getTime()
  { // returns time in msec
    return timeGetTime() - getStartUpTime();
  }
#else
  // MinGW has neither simple access to timeGetTime()
  // nor to gettimeofday()
  // - but beware: at least in LINUX clock() measures
  //   only the times of the EXE in CPU core
  int32_t getTime()
  { // returns time in msec
    return (clock()/(CLOCKS_PER_SEC/1000));
  }
#endif
#else
  /** linux-begin */
  int32_t getTime()
  {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
    // use gettimeofday for native LINUX 2.4 system
    // sysconf(_SC_CLK_TCK) provides clock_t ticks per second
    //  static const int64_t ci64_mesecPerClock = 1000 / sysconf(_SC_CLK_TCK);
    static struct timeval now;
    gettimeofday(&now, 0);
    // fetch RAW - non normalized - time in scaling of gettimeofday()
    int64_t i64_time4Timeofday = int64_t(now.tv_sec)*1000LL + int64_t(now.tv_usec/1000);


    // store offset between gettimeofday() and system start
    static int64_t si64_systemStart4Timeofday = i64_time4Timeofday;
    // static store delta between times() normalization and gettimeofday() norm
    static int64_t si64_deltaStartTimes = i64_time4Timeofday - int64_t(getStartUpTime()) * msecPerClock;

    // derive change of the normalization delta
    const int64_t ci64_deltaChange = i64_time4Timeofday - int64_t(times(NULL)) * msecPerClock - si64_deltaStartTimes;
    if ( ( ci64_deltaChange >= 1000 ) || ( ci64_deltaChange <= -1000 ) )
    { // user changed the system clock inbetween
      si64_deltaStartTimes += ci64_deltaChange;
      si64_systemStart4Timeofday += ci64_deltaChange;
    }

    // now calculate the real time in [msec] since startup
    i64_time4Timeofday -= si64_systemStart4Timeofday;
    // now derive the well define overflows
    while ( i64_time4Timeofday > 0x7FFFFFFFLL )
    {
      i64_time4Timeofday         -= 0xFFFFFFFF;
      si64_systemStart4Timeofday += 0xFFFFFFFF;
      si64_deltaStartTimes       += 0xFFFFFFFF;
    }

    return i64_time4Timeofday;
#else
    /** linux-2.6 */
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    const int32_t ci_now = int32_t(ts.tv_sec)*1000 + int32_t(ts.tv_nsec/1000000);
    return ci_now - getStartUpTime();
#endif
  }
  /** linux-end */
#endif


void initClientTime( __HAL::client_c& ref_receiveClient, clock_t rt_startupClock )
{
#ifndef WIN32
  #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
    ref_receiveClient.i32_msecStartDeltaClientMinusServer = (rt_startupClock - getStartUpTime())*msecPerClock;
  #else
    ref_receiveClient.i32_msecStartDeltaClientMinusServer = (rt_startupClock - getStartUpTime()); // we get msec directly with Linux 2.6. upwards!
  #endif
#else
  ref_receiveClient.i32_msecStartDeltaClientMinusServer = (rt_startupClock - getStartUpTime()); // we get msec directly with Linux 2.6. upwards!
#endif
  DEBUG_PRINT1 ("Initialized ref_receiveClient.i32_msecStartDeltaClientMinusServer to %d\n", ref_receiveClient.i32_msecStartDeltaClientMinusServer);
}

int32_t getClientTime( __HAL::client_c& ref_receiveClient )
{
  return getTime() - ref_receiveClient.i32_msecStartDeltaClientMinusServer;
}

//int32_t getServerTimeFromClientTime( __HAL::client_c& ref_receiveClient, int32_t ri32_clientTime )
//{
//  return ri32_clientTime + ref_receiveClient.i32_msecStartDeltaClientMinusServer;
//}

} // end namespace


SOCKET_TYPE get_connection(SOCKET_TYPE listenSocket) {

  SOCKET_TYPE newSocket;                              /* socket of connection */

#ifdef WIN32
  if ((newSocket = accept(listenSocket,NULL,NULL)) == INVALID_SOCKET)  /* accept connection if there is one */
#else
  if ((newSocket = accept(listenSocket,NULL,NULL)) < 0)  /* accept connection if there is one */
#endif
    return(-1);

  return(newSocket);
}

int read_data(SOCKET_TYPE s,     /* connected socket */
              char *buf, /* pointer to the buffer */
              int n      /* number of characters (bytes) we want */
             )
{
  int bcount; /* counts bytes read */
  int br;     /* bytes read this pass */

  bcount= 0;
  br= 0;
  while (bcount < n) {             /* loop until full buffer */
    if ((br= recv(s,buf,n-bcount,0)) > 0) {
      bcount += br;                /* increment byte counter */
      buf += br;                   /* move buffer ptr for next read */
    }
    else if (br <= 0)               /* signal an error or end of communication (0) to the caller */
      return br;
  }
  return(bcount);
}

void dumpCanMsg(__HAL::transferBuf_s *ap_transferBuf, __HAL::server_c *ap_server)
{
  size_t n_bus = ap_transferBuf->s_data.ui8_bus;
  if (!ap_server->canBus(n_bus).m_logFile) {
    bool b_error = newFileLog( ap_server, n_bus );
    if (b_error)
      return;
  }

  dumpCanMsg(
      ap_transferBuf->s_data.ui8_bus,
      ap_transferBuf->s_data.ui8_obj,
      &ap_transferBuf->s_data.s_canMsg,
      ap_server->canBus(n_bus).m_logFile->getRaw());
}

void monitorCanMsg (__HAL::transferBuf_s *ps_transferBuf)
{
  if( !can_filtering::pass(
        ps_transferBuf->s_data.ui8_bus,
        ps_transferBuf->s_data.s_canMsg.ui32_id,
        ps_transferBuf->s_data.s_canMsg.i32_len,
        ps_transferBuf->s_data.s_canMsg.ui8_data) )
   return;

  printf("%10d %-2d %-2d %-2d %-2d %-2d %-8x  ",
         __HAL::getTime(), ps_transferBuf->s_data.ui8_bus, ps_transferBuf->s_data.ui8_obj, ps_transferBuf->s_data.s_canMsg.i32_msgType, ps_transferBuf->s_data.s_canMsg.i32_len,
         (ps_transferBuf->s_data.s_canMsg.ui32_id >> 26) & 7 /* priority */, ps_transferBuf->s_data.s_canMsg.ui32_id);
  for (uint8_t ui8_i = 0; (ui8_i < ps_transferBuf->s_data.s_canMsg.i32_len) && (ui8_i < 8); ui8_i++)
     printf(" %-3hx", ps_transferBuf->s_data.s_canMsg.ui8_data[ui8_i]);
  printf("\n");
  fflush(0);
};


void releaseClient(__HAL::server_c* pc_serverData, std::list<__HAL::client_c>::iterator& iter_delete)
{
#if DEBUG_CANSERVER
  printf("Client disconnected.\n");
#endif

  for (uint8_t ui8_cnt=0; ui8_cnt < iter_delete->nCanBusses(); ui8_cnt++)
  {
    if (iter_delete->canBus(ui8_cnt).mb_initReceived && (pc_serverData->canBus(ui8_cnt).mui16_busRefCnt > 0))
    {
      pc_serverData->canBus(ui8_cnt).mui16_busRefCnt--; // decrement bus ref count when client dropped off

      if (!pc_serverData->canBus(ui8_cnt).mui16_busRefCnt)
        closeBusOnCard(ui8_cnt, pc_serverData);
    }
  }

  for (uint8_t k=0; k < iter_delete->nCanBusses(); k++)
    iter_delete->canBus(k).mvec_msgObj.clear();

  if (
#ifdef WIN32
      closesocket
#else
	  close
#endif
	  (iter_delete->i32_commandSocket))
  {
    #if DEBUG_CANSERVER
    printf("releaseClient: close i32_commandSocket=%d Error: %d (%s)\n", iter_delete->i32_commandSocket, errno, strerror(errno));
    #endif
  }
  if (
#ifdef WIN32
      closesocket
#else
	  close
#endif
	  (iter_delete->i32_dataSocket))
  {
    #if DEBUG_CANSERVER
    printf("releaseClient: close i32_dataSocket=%d Error: %d (%s)\n", iter_delete->i32_dataSocket, errno, strerror(errno));
    #endif
  }

  // erase sets iterator to next client
  iter_delete = pc_serverData->mlist_clients.erase(iter_delete);
}

static void enqueue_msg(__HAL::transferBuf_s* p_sockBuf, SOCKET_TYPE i32_socketSender, __HAL::server_c* pc_serverData)
{
  const uint8_t ui8_bus = p_sockBuf->s_data.ui8_bus;

  // mutex to prevent client list modification already got in calling function

  for (std::list<__HAL::client_c>::iterator iter = pc_serverData->mlist_clients.begin(); iter != pc_serverData->mlist_clients.end(); iter++) {

    if (!iter->canBus(ui8_bus).mb_busUsed)
      continue;

    // i32_clientID != 0 in forwarding mode during send, do not enqueue this message for sending client
    if (i32_socketSender && (iter->i32_dataSocket == i32_socketSender))
      continue;

    // now search for MsgObj queue on this ui8_bus, where new message from ui8_bus maps
    // start with 1 since MsgObj with id 0 is anyway planned for sending
    const int32_t i32_maxObj = iter->canBus(ui8_bus).mvec_msgObj.size();
    for (int32_t i32_obj = 1; i32_obj < i32_maxObj; i32_obj++) {

      if ( (iter->canBus(ui8_bus).mvec_msgObj[i32_obj].ui8_bMsgType != RX )
        || (iter->canBus(ui8_bus).mvec_msgObj[i32_obj].ui16_size == 0     ) )
      { // this MsgObj is no candidate for message receive
        continue;
      }

      if (!iter->canBus(ui8_bus).mvec_msgObj[i32_obj].b_canObjConfigured) {
        continue;
      }

      if ( iter->canBus(ui8_bus).mvec_msgObj[i32_obj].b_canBufferLock ) {
        // don't even check this MsgObj as it shall not receive messages
        DEBUG_PRINT2("lock bus %d, obj %d\n", ui8_bus, i32_obj);
        continue;
      }

      // compare received msg with filter
      if (
          ( (iter->canBus(ui8_bus).mvec_msgObj[i32_obj].ui8_bufXtd > 0)
                && (p_sockBuf->s_data.s_canMsg.i32_msgType > 0)
                && ( (p_sockBuf->s_data.s_canMsg.ui32_id & iter->canBus(ui8_bus).mvec_msgObj[i32_obj].ui32_mask_xtd) == (iter->canBus(ui8_bus).mvec_msgObj[i32_obj].ui32_filter & iter->canBus(ui8_bus).mvec_msgObj[i32_obj].ui32_mask_xtd) )
              )
              ||
              ( (iter->canBus(ui8_bus).mvec_msgObj[i32_obj].ui8_bufXtd == 0)
                && (p_sockBuf->s_data.s_canMsg.i32_msgType == 0)
                && ( (p_sockBuf->s_data.s_canMsg.ui32_id & iter->canBus(ui8_bus).mvec_msgObj[i32_obj].ui16_mask_std) == (iter->canBus(ui8_bus).mvec_msgObj[i32_obj].ui32_filter & iter->canBus(ui8_bus).mvec_msgObj[i32_obj].ui16_mask_std) )
              )
        )
        { // received msg fits actual filter
          //DEBUG_PRINT("queueing message\n");
          // update send time stamp in paket
          p_sockBuf->s_data.i32_sendTimeStamp = getClientTime(*iter);

          p_sockBuf->s_data.ui8_obj = i32_obj;

          if (send(iter->i32_dataSocket, (char*)p_sockBuf, sizeof(__HAL::transferBuf_s),
#ifdef WIN32
              0
#else
              MSG_DONTWAIT | MSG_NOSIGNAL
#endif
             ) < 0)
          {
            perror("send");
            if(EPIPE == errno)
            {
                DEBUG_PRINT("pipe error\n");
                // connection will be closed in next read from socket
            }                
          }

          // don't check following objects if message is already enqueued for this client
          break;

        } // if fit
    } // for objNr
  }// for iter

}


void send_command_ack(SOCKET_TYPE ri32_commandSocket, int32_t ri32_dataContent, int32_t ri32_data, __HAL::server_c &ar_server)
{
  __HAL::transferBuf_s s_transferBuf;

  s_transferBuf.ui16_command = COMMAND_ACKNOWLEDGE;
  s_transferBuf.s_acknowledge.i32_dataContent = ri32_dataContent;
  s_transferBuf.s_acknowledge.i32_data = ri32_data;

  if (send(ri32_commandSocket, (char*)&s_transferBuf, sizeof(__HAL::transferBuf_s),
#ifdef WIN32
           0
#else
           MSG_DONTWAIT | MSG_NOSIGNAL
#endif
          ) < 0)
  {
    perror("send");
    if(EPIPE == errno)
    {
      DEBUG_PRINT("pipe error\n");
      // connection will be closed in next read from socket
    }
  }
}


/////////////////////////////////////////////////////////////////////////
bool handleCommand(__HAL::server_c* pc_serverData, std::list<__HAL::client_c>::iterator& iter_client, __HAL::transferBuf_s* p_writeBuf)
{
  int32_t i32_error;
  int32_t i32_dataContent;
  int32_t i32_data=0;

  DEBUG_PRINT1("cmd %d\n", p_writeBuf->ui16_command);

  i32_error = 0;

  // default to simple ACK which returns the error.
  // Overwrite if special answers needed like PIPE_ID or SEND_DELAY
  i32_dataContent = ACKNOWLEDGE_DATA_CONTENT_ERROR_VALUE;
  // in DATA_CONTENT_ERROR_VALUE case i32_error will be used as i32_data!!!

    switch (p_writeBuf->ui16_command)
    {

      case COMMAND_REGISTER:
      {
        DEBUG_PRINT("COMMAND_REGISTER\n");

        // no need to set to 0, as everything got set to zero before!
        // s_tmpClient.canBus(all-buses).mi32_sendDelay = 0;

        DEBUG_PRINT1 ("Client registering with startTimeClock_t from his REGISTER message as %d\n", p_writeBuf->s_startTimeClock.t_clock);
        initClientTime(*iter_client, p_writeBuf->s_startTimeClock.t_clock );

        DEBUG_PRINT1("client start up time (absolute value in clocks): %d\n", p_writeBuf->s_startTimeClock.t_clock);

        if (!i32_error)
        { // no error
          // transmit current pipeId to client (for composition of pipe name)
          i32_dataContent = ACKNOWLEDGE_DATA_CONTENT_PIPE_ID;
        }
      }
      break;

      case COMMAND_DEREGISTER: {
        DEBUG_PRINT("COMMAND_DEREGISTER\n");

        for (uint8_t j=0; j < iter_client->nCanBusses(); j++)
        {
          iter_client->canBus(j).mvec_msgObj.clear();
          if (iter_client->canBus(j).mb_initReceived && (pc_serverData->canBus(j).mui16_busRefCnt > 0))
            pc_serverData->canBus(j).mui16_busRefCnt--; // decrement ref count only when we received the INIT command before
        }

        // i32_error will stay at 0 for "no error"
        releaseClient(pc_serverData, iter_client);
      } return true; // don't send answer and report "clientReleased" as return value!


      case COMMAND_INIT:
        if (p_writeBuf->s_config.ui8_bus > HAL_CAN_MAX_BUS_NR)
          i32_error = HAL_RANGE_ERR;
        else if (!pc_serverData->canBus(p_writeBuf->s_init.ui8_bus).mui16_busRefCnt)
        { // first init command for current bus
          // open log file only once per bus
          if (pc_serverData->mb_logMode) {
            newFileLog( pc_serverData, p_writeBuf->s_init.ui8_bus);
          }

          if (!openBusOnCard(p_writeBuf->s_init.ui8_bus,  // 0 for CANLPT/ICAN, else 1 for first BUS
                             p_writeBuf->s_init.ui16_wBitrate,  // BTR0BTR1
                             pc_serverData))
          {
            std::cerr << "Can't initialize CAN-BUS." << std::endl;
            std::cerr << "CAN device/driver not ready.\n" << std::endl;
            i32_error = HAL_CONFIG_ERR;
            abort();
          }
        }

        if (!i32_error) {
          pc_serverData->canBus(p_writeBuf->s_init.ui8_bus).mui16_busRefCnt++;
          iter_client->canBus(p_writeBuf->s_init.ui8_bus).mb_initReceived = true; // when the CLOSE command is received => allow decrement of ref count
          iter_client->canBus(p_writeBuf->s_init.ui8_bus).mb_busUsed = true; // when the CLOSE command is received => allow decrement of ref count
        }
        // do rest of init handling in next case statement (no break!)

      case COMMAND_CHG_GLOBAL_MASK:
        if (!i32_error) {
          iter_client->canBus(p_writeBuf->s_init.ui8_bus).mui16_globalMask = p_writeBuf->s_init.ui16_wGlobMask;
          iter_client->canBus(p_writeBuf->s_init.ui8_bus).mui32_globalMask = p_writeBuf->s_init.ui32_dwGlobMask;
          iter_client->canBus(p_writeBuf->s_init.ui8_bus).mui32_lastMask = p_writeBuf->s_init.ui32_dwGlobMaskLastmsg;
        }
        break;


      case COMMAND_CLOSE:
      {
        if (p_writeBuf->s_config.ui8_bus > HAL_CAN_MAX_BUS_NR )
          i32_error = HAL_RANGE_ERR;
        else
        {
          if (iter_client->canBus(p_writeBuf->s_init.ui8_bus).mb_initReceived && (pc_serverData->canBus(p_writeBuf->s_init.ui8_bus).mui16_busRefCnt > 0))
          {
            pc_serverData->canBus(p_writeBuf->s_init.ui8_bus).mui16_busRefCnt--; // decrement ref count only when we received the INIT command before
          }
          iter_client->canBus(p_writeBuf->s_init.ui8_bus).mb_initReceived = false; // reset flag

          if (pc_serverData->canBus(p_writeBuf->s_init.ui8_bus).mui16_busRefCnt == 0)
          {
            // close can device
            closeBusOnCard(p_writeBuf->s_init.ui8_bus, pc_serverData);
          }

          if (!pc_serverData->canBus(p_writeBuf->s_init.ui8_bus).mui16_busRefCnt && pc_serverData->mb_logMode) {
            closeFileLog(pc_serverData, p_writeBuf->s_init.ui8_bus);
          }

          iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj.clear();
          // i32_error will stay at 0 for "no error"
        }
      }
      break;


      case COMMAND_CONFIG:
        if ( p_writeBuf->s_config.ui8_bus > HAL_CAN_MAX_BUS_NR )
        {
          i32_error = HAL_RANGE_ERR;
          break;
        }

        if (p_writeBuf->s_config.ui8_obj >= iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj.size()) {
          // add new elements in the vector with resize
          iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj.resize(p_writeBuf->s_config.ui8_obj+1);
        } else {
          // reconfigure element
        }
        // break left out intentionally

      case COMMAND_CHG_CONFIG:

        if ((p_writeBuf->s_config.ui8_bus > HAL_CAN_MAX_BUS_NR ) || ( p_writeBuf->s_config.ui8_obj > iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj.size()-1 ))
          i32_error = HAL_RANGE_ERR;
        else
        {
          iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].b_canObjConfigured = true;

          iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].ui8_bufXtd = p_writeBuf->s_config.ui8_bXtd;
          iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].ui32_filter = p_writeBuf->s_config.ui32_dwId;

          if (p_writeBuf->s_config.ui8_bXtd)
              iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].ui32_mask_xtd = p_writeBuf->s_config.ui32_mask;
          else
              iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].ui16_mask_std = p_writeBuf->s_config.ui32_mask;

          if (p_writeBuf->ui16_command == COMMAND_CONFIG) {
//              clearReadQueue (p_writeBuf->s_config.ui8_bus, p_writeBuf->s_config.ui8_obj, pc_serverData->ms_msqDataServer.i32_rdHandle, iter_client->ui16_pid);
//            clearWriteQueue(p_writeBuf->s_config.ui8_bus, p_writeBuf->s_config.ui8_obj, pc_serverData->ms_msqDataServer.i32_wrHandle, iter_client->ui16_pid);

            iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].ui8_bMsgType = p_writeBuf->s_config.ui8_bMsgType;
            iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].ui16_size = p_writeBuf->s_config.ui16_wNumberMsgs;
            iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].b_canBufferLock = false;
          }
        }
        break;


      case COMMAND_LOCK:
      case COMMAND_UNLOCK:

        if ((p_writeBuf->s_config.ui8_bus > HAL_CAN_MAX_BUS_NR ) || ( p_writeBuf->s_config.ui8_obj > iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj.size()-1 ))
          i32_error = HAL_RANGE_ERR;
        else {
          if (p_writeBuf->ui16_command == COMMAND_LOCK) {
            iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].b_canBufferLock = true;
            DEBUG_PRINT2("locked buf %d, obj %d\n", p_writeBuf->s_config.ui8_bus, p_writeBuf->s_config.ui8_obj);
          } else {
            iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].b_canBufferLock = false;
            DEBUG_PRINT2("unlocked buf %d, obj %d\n", p_writeBuf->s_config.ui8_bus, p_writeBuf->s_config.ui8_obj);
          }
        }
        break;


      case COMMAND_QUERYLOCK:

        if ((p_writeBuf->s_config.ui8_bus > HAL_CAN_MAX_BUS_NR ) || ( p_writeBuf->s_config.ui8_obj > iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj.size()-1 ))
          i32_error = HAL_RANGE_ERR;
        else {
          i32_dataContent = ACKNOWLEDGE_DATA_CONTENT_QUERY_LOCK;
          i32_data = iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].b_canBufferLock;
        }
        break;


      case COMMAND_CLOSEOBJ:

        if ((p_writeBuf->s_config.ui8_bus > HAL_CAN_MAX_BUS_NR ) || ( p_writeBuf->s_config.ui8_obj > iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj.size()-1 ))
          i32_error = HAL_RANGE_ERR;
        else {
          iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].b_canObjConfigured = false;

          iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].b_canBufferLock = false;
          iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj[p_writeBuf->s_config.ui8_obj].ui16_size = 0;
 //           clearReadQueue (p_writeBuf->s_config.ui8_bus, p_writeBuf->s_config.ui8_obj, pc_serverData->ms_msqDataServer.i32_rdHandle, iter_client->ui16_pid);
//          clearWriteQueue(p_writeBuf->s_config.ui8_bus, p_writeBuf->s_config.ui8_obj, pc_serverData->ms_msqDataServer.i32_wrHandle, iter_client->ui16_pid);

          // erase element if it is the last in the vector, otherwise it can stay there
          while (iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj.back().b_canObjConfigured == false)
              iter_client->canBus(p_writeBuf->s_config.ui8_bus).mvec_msgObj.pop_back();
        }
        break;


      case COMMAND_SEND_DELAY:
        if ( p_writeBuf->s_config.ui8_bus > HAL_CAN_MAX_BUS_NR )
        {
          i32_error = HAL_RANGE_ERR;
        } else {
          i32_dataContent = ACKNOWLEDGE_DATA_CONTENT_SEND_DELAY;
          i32_data = iter_client->canBus(p_writeBuf->s_config.ui8_bus).mi32_sendDelay;
          if (i32_data < 0) i32_data = -i32_data; // so we always return a positive send delay!
          else iter_client->canBus(p_writeBuf->s_config.ui8_bus).mi32_sendDelay  = -i32_data; // stamp a positive stored delay as READ by negating it!
        }
        break;

      default:
        i32_error = HAL_UNKNOWN_ERR;
        break;
    } // end switch

    // do centralized error-answering here
    if (i32_dataContent == ACKNOWLEDGE_DATA_CONTENT_ERROR_VALUE) i32_data = i32_error;

    send_command_ack(iter_client->i32_commandSocket, i32_dataContent, i32_data, *pc_serverData);
    return false; // client not released, we did even send the ACK :)
}


void readWrite(__HAL::server_c* pc_serverData)
{
  fd_set rfds;
  __HAL::transferBuf_s s_transferBuf;
  int i_selectResult;
  struct timeval t_timeout;
  bool b_deviceHandleFound;
  bool b_deviceConnected;
  bool b_handlesAvailableForSelect;

  for (;;) {

    pthread_mutex_lock( &(pc_serverData->mt_protectClientList) );

    FD_ZERO(&rfds);

    std::list<__HAL::client_c>::iterator iter_client;

    b_handlesAvailableForSelect = false;

    for (iter_client = pc_serverData->mlist_clients.begin(); iter_client != pc_serverData->mlist_clients.end(); iter_client++)
    {
      FD_SET(iter_client->i32_commandSocket, &rfds);
      FD_SET(iter_client->i32_dataSocket, &rfds);
      b_handlesAvailableForSelect = true; // calling select() makes sense because we have a socket from client
    }

    b_deviceHandleFound=false;
    b_deviceConnected = false;

    for (uint32_t ui32=0; ui32 < pc_serverData->nCanBusses(); ui32++ )
    {
      if (pc_serverData->canBus(ui32).mi32_can_device > 0)
      {
        FD_SET(pc_serverData->canBus(ui32).mi32_can_device, &rfds);
        b_deviceHandleFound=true;
        b_handlesAvailableForSelect = true; // calling select() makes sense because we have a file handle from CAN device
      }

      if (pc_serverData->canBus(ui32).mb_deviceConnected)
        b_deviceConnected = true;
    }

    // from now on: no access to client list => unlock mutex
    pthread_mutex_unlock( &(pc_serverData->mt_protectClientList) );

    if (b_handlesAvailableForSelect)
    {

		t_timeout.tv_sec = 0;
		// do long timeout if we have a device handle or if we do not have hardware device at all
		if (b_deviceHandleFound || !b_deviceConnected)
		  t_timeout.tv_usec = scui32_selectTimeoutMax;
		else
		  t_timeout.tv_usec = scui32_selectTimeoutMin;

		// timeout to check for
		// 1. modified client list => new sockets to wait for
		// new incoming can messages when can device has no file handle (WIN32 PEAK can card and RTE)
		i_selectResult = select(FD_SETSIZE, &rfds, NULL, NULL, &t_timeout);

	   if (i_selectResult < 0)
		  // error
		  continue;
    }
    else
    { // no sockets or device handle available for select => just sleep for a while
#ifdef WIN32
      Sleep( 10 );
#else
      usleep( 10000 );
#endif
    }

    // new message from can device ?
    for (uint32_t ui32_cnt = 0; ui32_cnt < pc_serverData->nCanBusses(); ui32_cnt++ )
    {
      if( !isBusOpen(ui32_cnt) )
        continue; // this bus number was not yet used => do not try to read

      if ( !b_deviceHandleFound ||
           (b_deviceHandleFound && pc_serverData->canBus(ui32_cnt).mi32_can_device && FD_ISSET(pc_serverData->canBus(ui32_cnt).mi32_can_device, &rfds)) 
         )
      { // do card read
        if (readFromBus(ui32_cnt, &(s_transferBuf.s_data.s_canMsg), pc_serverData))
        {
          pthread_mutex_lock( &(pc_serverData->mt_protectClientList) );
          s_transferBuf.s_data.ui8_bus = ui32_cnt;
          enqueue_msg(&s_transferBuf, 0, pc_serverData);
          pthread_mutex_unlock( &(pc_serverData->mt_protectClientList) );
          if (pc_serverData->mb_logMode) {
            dumpCanMsg(
                &s_transferBuf,
                pc_serverData);
          }
          if (pc_serverData->mb_monitorMode)
            monitorCanMsg (&s_transferBuf);
        }
      }
    }

    pthread_mutex_lock( &(pc_serverData->mt_protectClientList) );

    // new message from socket ?
    for (iter_client = pc_serverData->mlist_clients.begin(); iter_client != pc_serverData->mlist_clients.end(); )
    {
      if (FD_ISSET(iter_client->i32_commandSocket, &rfds))
      {
        // socket still alive? (returns 0 (peer shutdown) or -1 (error))
#ifdef WINCE
        // @todo WINCE-176 Windows CE has a bug with MSG_PEEK (bytes are actually read)
        //int bytesRecv = select( 0, &rfds, NULL, NULL, 0 ); // this was a try for a workaround
        #error "This place has to be done correctly for Windows CE"
#else
        int bytesRecv = recv(iter_client->i32_commandSocket, (char*)&s_transferBuf, sizeof(__HAL::transferBuf_s),
        #ifndef WIN32
          MSG_DONTWAIT|
        #endif
          MSG_PEEK);
#endif
        if (bytesRecv == 0 || bytesRecv == -1)
        {
          if (pc_serverData->mb_interactive) {
#ifdef WIN32
            if( bytesRecv == -1 && WSAGetLastError() == WSAEOPNOTSUPP )
              printf("The attempted operation is not supported for the type of object referenced.\n");
#endif
            printf( "connection closed.\n");
          }
          releaseClient(pc_serverData, iter_client);
          continue;
        }

        read_data(iter_client->i32_commandSocket, (char*)&s_transferBuf, sizeof(s_transferBuf));

        if (s_transferBuf.ui16_command != COMMAND_DATA)
        {
          const bool clientReleased =
            handleCommand(pc_serverData, iter_client, &s_transferBuf);

          if( clientReleased )
            continue; // no "++iter" then, iter_client was "moved on" with the erase inside of the releaseClient()-call!
        }
      }
      if (FD_ISSET(iter_client->i32_dataSocket, &rfds))
      {
        // socket still alive? (returns 0 (peer shutdown) or -1 (error))
#ifdef WINCE
        // @todo WINCE-176 Windows CE has a bug with MSG_PEEK (bytes are actually read)
        //int bytesRecv = select( 0, &rfds, NULL, NULL, 0 ); // this was a try for a workaround
        #error "This place has to be done correctly for Windows CE"
#else
        int bytesRecv = recv(iter_client->i32_dataSocket, (char*)&s_transferBuf, sizeof(__HAL::transferBuf_s),
        #ifndef WIN32
          MSG_DONTWAIT|
        #endif
          MSG_PEEK);
#endif

        if (bytesRecv == 0 || bytesRecv == -1)
        {
          if (pc_serverData->mb_interactive) {
#ifdef WIN32
            if( bytesRecv == -1 && WSAGetLastError() == WSAEOPNOTSUPP )
              printf("The attempted operation is not supported for the type of object referenced.\n");
#endif
            printf( "connection closed.\n");
          }
          releaseClient(pc_serverData, iter_client);
          continue;
        }

        read_data(iter_client->i32_dataSocket, (char*)&s_transferBuf, sizeof(s_transferBuf));

        if (s_transferBuf.ui16_command == COMMAND_DATA)
        {
          // process data message
          enqueue_msg(&s_transferBuf, iter_client->i32_dataSocket, pc_serverData); // not done any more: disassemble_client_id(msqWriteBuf.i32_mtype)
          
          if (isBusOpen(s_transferBuf.s_data.ui8_bus))
          {
            (void)sendToBus(s_transferBuf.s_data.ui8_bus, &(s_transferBuf.s_data.s_canMsg), pc_serverData);
          }

          if (pc_serverData->mb_logMode) {
            dumpCanMsg(
                &s_transferBuf,
                pc_serverData);
          }
          if (pc_serverData->mb_monitorMode)
          {
            monitorCanMsg (&s_transferBuf);
          }
        }
      }

      // if the client wasn't released (and hence "iter = list.erase(iter);" was called),
      // do step on to the next item.
      ++iter_client;
    } // for

    pthread_mutex_unlock( &(pc_serverData->mt_protectClientList) );

  }
}


void sendUserMsg(uint32_t DLC, uint32_t ui32_id, uint32_t ui32_bus, uint8_t ui8_xtd, uint8_t* pui8_data, __HAL::server_c* pc_serverData)
{
  __HAL::transferBuf_s s_transferBuf;

  // set the necessary data for monitorCanMsg
  s_transferBuf.s_data.s_canMsg.i32_msgType = ui8_xtd;
  s_transferBuf.s_data.s_canMsg.i32_len = DLC;
  s_transferBuf.s_data.s_canMsg.ui32_id = ui32_id;
  memcpy(s_transferBuf.s_data.s_canMsg.ui8_data, pui8_data, DLC);

  s_transferBuf.s_data.ui8_bus = ui32_bus;

  // acquire mutex (prevents concurrent read/write access to can driver and modification of client list during execution of enqueue_msg
  pthread_mutex_lock( &(pc_serverData->mt_protectClientList) );

  enqueue_msg(&s_transferBuf, 0, pc_serverData);

  if (isBusOpen(s_transferBuf.s_data.ui8_bus))
  {
    (void)sendToBus(s_transferBuf.s_data.ui8_bus, &(s_transferBuf.s_data.s_canMsg), pc_serverData);
  }

  pthread_mutex_unlock( &(pc_serverData->mt_protectClientList) );
  
  if (pc_serverData->mb_logMode)
  {
    dumpCanMsg( &s_transferBuf, pc_serverData);
  }
  if (pc_serverData->mb_monitorMode)
  {
    monitorCanMsg (&s_transferBuf);
  }
}



/////////////////////////////////////////////////////////////////////////

namespace __HAL {
  SOCKET_TYPE establish(unsigned short portnum);
}


static void* collectClient(void* ptr) {

  SOCKET_TYPE new_socket, base_commandSocket, base_dataSocket;
  __HAL::client_c s_tmpClient; // constructor initialize values to zero

  __HAL::server_c* pc_serverData = static_cast<__HAL::server_c*>(ptr);

  if ((base_commandSocket = __HAL::establish(COMMAND_TRANSFER_PORT)) < 0) {
    perror("establish");
    exit(1);
  }
  if (pc_serverData->mb_interactive) {
    printf("Command socket for port %d established\n", COMMAND_TRANSFER_PORT);
  }

  if ((base_dataSocket = __HAL::establish(DATA_TRANSFER_PORT)) < 0) {
    perror("establish");
    exit(1);
  }
  if (pc_serverData->mb_interactive) {
    printf("Data socket for port %d established\n", DATA_TRANSFER_PORT);
  }

  while (1) {

#ifdef WIN32
    if ((new_socket=get_connection(base_commandSocket)) == INVALID_SOCKET) {
      perror("socket connect failed");
      exit(1);
    }
#else
    if ((new_socket=get_connection(base_commandSocket)) == -1) {
      perror("Socket connect failed!");
      exit(1);
    }
#endif

    s_tmpClient.i32_commandSocket = new_socket;

#ifdef WIN32
    if ((new_socket=get_connection(base_dataSocket)) == INVALID_SOCKET) {
      perror("socket connect failed");
      exit(1);
    }
#else
    if ((new_socket=get_connection(base_dataSocket)) == -1) {
      perror("socket connect failed");
      exit(1);
    }
#endif

    s_tmpClient.i32_dataSocket = new_socket;

    pthread_mutex_lock( &(pc_serverData->mt_protectClientList) );

    pc_serverData->mlist_clients.push_back(s_tmpClient);

    pthread_mutex_unlock( &(pc_serverData->mt_protectClientList) );

    if (pc_serverData->mb_interactive) {
      printf("Command and data socket connected.\n");
    }
  }
}

yasper::ptr< AOption_c > const ga_options[] = {
  Option_c< OPTION_MONITOR >::create(),
  Option_c< OPTION_LOG >::create(),
  Option_c< OPTION_REDUCED_LOAD_ISO_BUS_NO >::create(),
  Option_c< OPTION_INTERACTIVE >::create(),
  Option_c< OPTION_PRODUCTIVE >::create(),
#ifndef WIN32
  Option_c< OPTION_DAEMON>::create(),
#endif
  Option_c< OPTION_HELP >::create()
};

yasper::ptr< AOption_c > const *const gp_optionsBegin = ga_options;
yasper::ptr< AOption_c > const *const gp_optionsEnd = ga_options +
  (sizeof ga_options) / (sizeof ga_options[0]);

int main(int argc, char *argv[])
{
  pthread_t threadCollectClient;
  __HAL::server_c c_serverData;

  checkAndHandleOptionsAndStartup( argc, argv, c_serverData );

#ifdef WIN32
  WSADATA wsaData;
  // Initialize Winsock
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
     printf("WSAStartup failed: %d\n", iResult);
     exit(1);
  }
#endif

  (void)pthread_create( &threadCollectClient, NULL, &collectClient, &c_serverData);

  if (c_serverData.mb_interactive) {
    pthread_t thread_readUserInput;
    int i_status = pthread_create( &thread_readUserInput, NULL, &readUserInput, &c_serverData );
    if (i_status)
    {
      printf("Could not create collect-client-thread!\n");
      exit( i_status ); // thread could not be created
    }
  }

  readWrite(&c_serverData);
}

#if defined(WINCE) || (defined(WIN32) && defined(UNICODE))
// CE is unicode, so we need to convert the commandline...
// However, it will be only called when using "mainWCRTStartup" as entry point
// (using "mainCRTStartup" atm).
int _tmain( int argc, _TCHAR* argv[] )
{
	char** argv_char = new char*[argc];

	// convert command line	
	for( int i = 0; i < argc; i++ )
	{
		size_t arglen = wcslen(argv[i]);	
		char* arg = new char[arglen+1];
		WideCharToMultiByte( CP_ACP, 0, argv[i], arglen, arg, arglen, NULL, NULL );
		argv_char[i] = arg;
	}

	// call main
	int ret = main( argc, argv_char );

	// cleanup
	for( int i = 0; i < argc; i++ )
	{
		delete[] argv_char[i];
	}
	delete[] argv_char;

	return ret;
}
#endif
