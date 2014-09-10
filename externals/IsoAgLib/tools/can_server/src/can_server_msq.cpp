/*
  can_server_msq.cpp: can server communicates with clients through
    message queues message forwarding to other clients

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
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <string>
#include <sstream>
#include <iostream>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#include <pthread.h>
#include <linux/version.h>

#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"
#include "can_filtering.h"

#if DEBUG_CANSERVER
  //backtrace
  #include <unistd.h>
  #include <execinfo.h>

  char*  reserve_memory_heap;
#endif

#include <iostream>
#include <fstream>

/////////////////////////////////////////////////////////////////////////
// Globals
/** if the following define is active, the can_server writes important logging info to the given path.
 *  comment this define out to eliminate this */
#ifdef WIN32
  #define CAN_SERVER_LOG_PATH ".\\can_server.log"
#else
  #define CAN_SERVER_LOG_PATH "./can_server.log"
#endif

// set nice level for the threads
#define PRIORITY_CAN_READ    10
#define PRIORITY_CAN_WRITE    0
#define PRIORITY_CAN_COMMAND  0

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
  mb_daemon(false),
  mi32_lastPipeId(0),
  mi16_reducedLoadOnIsoBus(-1),
  mb_interactive(true),
  mi_canReadNiceValue(0),
  mvec_canBus()
{
  memset(marrb_remoteDestinationAddressInUse, 0, sizeof(marrb_remoteDestinationAddressInUse));
  memset(marri32_fileDescrWakeUpPipeForNewBusEvent, 0, sizeof(marri32_fileDescrWakeUpPipeForNewBusEvent));

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
  mvec_canBus(),
  i32_pipeHandle(0)
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
#endif




#ifdef WIN32
#if defined( _MSC_VER )
  // VC++ with native Win32 API provides very accurate
  // msec timer - use that
  int32_t getTime()
  { // returns time in msec
    return timeGetTime();
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


void initClientTime( client_c& r_receiveClient, clock_t at_startupClock )
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
  r_receiveClient.i32_msecStartDeltaClientMinusServer = (at_startupClock - getStartUpTime())*msecPerClock;
#else
  r_receiveClient.i32_msecStartDeltaClientMinusServer = (at_startupClock - getStartUpTime()); // we get msec directly with Linux 2.6. upwards!
#endif
  DEBUG_PRINT1 ("Initialized r_receiveClient.i32_msecStartDeltaClientMinusServer to %d\n", r_receiveClient.i32_msecStartDeltaClientMinusServer);
}

int32_t getClientTime( client_c& r_receiveClient )
{
  return getTime() - r_receiveClient.i32_msecStartDeltaClientMinusServer;
}

//int32_t getServerTimeFromClientTime( client_c& r_receiveClient, int32_t ai32_clientTime )
//{
//  return ai32_clientTime + r_receiveClient.i32_msecStartDeltaClientMinusServer;
//}

} // end namespace


void dumpCanMsg(uint8_t au8_bus, uint8_t au8_msgObj, canMsg_s *ap_canMsg, __HAL::server_c *ap_server)
{
  if (!ap_server->canBus(au8_bus).m_logFile) {
    bool b_error = newFileLog( ap_server, au8_bus );
    if (b_error)
      return;
  }
  dumpCanMsg(
      au8_bus,
      au8_msgObj,
      ap_canMsg,
      ap_server->canBus(au8_bus).m_logFile->getRaw() );
}

void monitorCanMsg (uint8_t bBusNumber, uint8_t bMsgObj, canMsg_s* ps_canMsg)
{
  if( !can_filtering::pass(
        bBusNumber,
        ps_canMsg->ui32_id,
        ps_canMsg->i32_len,
        ps_canMsg->ui8_data) )
    return;

  printf("%10d %-2d %-2d %-2d %-2d %-2d %-8x  ",
         __HAL::getTime(), bBusNumber, bMsgObj, ps_canMsg->i32_msgType, ps_canMsg->i32_len,
         (ps_canMsg->ui32_id >> 26) & 7 /* priority */, ps_canMsg->ui32_id);
  for (uint8_t ui8_i = 0; (ui8_i < ps_canMsg->i32_len) && (ui8_i < 8); ui8_i++)
    printf(" %-3hx", ps_canMsg->ui8_data[ui8_i]);
  printf("\n");
  fflush(0);
};


bool readCanDataFile(__HAL::server_c* pc_serverData, __HAL::can_recv_data* ps_receiveData)
{
  char zeile[100];
  static struct timeval tv_start;
  static bool first_call = true;
  struct timeval tv_current;
  struct timezone tz;
  int32_t mydiff;

  tz.tz_minuteswest=0;
  tz.tz_dsttime=0;

  if (first_call) {
    first_call = false;

    if ((pc_serverData->mf_canInput = fopen(pc_serverData->mstr_inputFile.c_str(), "r")) == NULL ) {
      perror("fopen");
      exit(1);
    }

    if (gettimeofday(&tv_start, &tz)) {
      perror("error in gettimeofday()");
      exit(1);
    }
  }

  for (;;) {

    if (fgets(zeile, 99, pc_serverData->mf_canInput) == NULL) {
      if ( feof(pc_serverData->mf_canInput) )
        // no more data available
        return false;
    }

    if ( strlen(zeile) == 0 )
      continue;

    int obj, xtd, dlc, prio;
    short unsigned int db[8];
    int rc = sscanf(zeile, "%u %d %d %d %d %d %x   %hx %hx %hx %hx %hx %hx %hx %hx \n",
                    &(ps_receiveData->msg.i32_time),
                    &(ps_receiveData->b_bus), &obj,
                    &xtd, &dlc, &prio,
                    &(ps_receiveData->msg.i32_ident),
                    &db[0], &db[1], &db[2], &db[3], &db[4], &db[5], &db[6], &db[7]);

    for (int i=0; i<8; ++i)
      ps_receiveData->msg.pb_data[i] = static_cast<uint8_t>(db[i]);

    if (!rc)
      return false;

    // sscanf uses int by default (overwriting of two adjacent uint8_t with format "%d"!)
    ps_receiveData->msg.b_xtd = xtd;
    ps_receiveData->msg.b_dlc = dlc;

    // is data ready for submission?
    if (gettimeofday(&tv_current, &tz)) {
      perror("error in gettimeofday()");
      exit(1);
    }

    mydiff = ps_receiveData->msg.i32_time - (tv_current.tv_sec-tv_start.tv_sec)*1000 + (tv_current.tv_usec-tv_start.tv_usec)/1000;

    if (mydiff < 0)
      mydiff = 0;

    usleep(mydiff*1000);

    break;
  }

  // more data available
  return true;
}

void releaseClient(__HAL::server_c* pc_serverData, std::list< __HAL::client_c >::iterator& iter_delete) {

  for (uint8_t i=0; i < iter_delete->nCanBusses(); i++)
  {
    for (uint8_t j=0; j<iter_delete->canBus(i).mvec_msgObj.size(); j++) {
      __HAL::clearReadQueue (i, j, pc_serverData->ms_msqDataServer.i32_rdHandle, iter_delete->ui16_pid);
    }

    if (iter_delete->canBus(i).mb_initReceived && (pc_serverData->canBus(i).mui16_busRefCnt > 0))
      pc_serverData->canBus(i).mui16_busRefCnt--; // decrement bus ref count when client dropped off
  }

  if (iter_delete->i32_pipeHandle)
    close(iter_delete->i32_pipeHandle);

  for (uint8_t k=0; k < iter_delete->nCanBusses(); k++)
    iter_delete->canBus(k).mvec_msgObj.clear();

  // erase sets iterator to next client
  iter_delete = pc_serverData->mlist_clients.erase(iter_delete);

}


static void enqueue_msg(uint32_t DLC, uint32_t ui32_id, uint32_t b_bus, uint8_t b_xtd, uint8_t* pui8_data, uint16_t ui16_pid, __HAL::server_c* pc_serverData)
{

  __HAL::can_data *pc_data;
  std::list<__HAL::client_c>::iterator iter, iter_delete = pc_serverData->mlist_clients.end();

  // mutex to prevent client list modification already got in calling function

  // prepare msqRead_s for each potential broadcast send
  __HAL::msqRead_s msqReadBuf;
  pc_data = &(msqReadBuf.s_canData);
  // preset constant fields of msqReadBuf
  pc_data->i32_ident = ui32_id;
  pc_data->b_dlc = DLC;
  pc_data->b_xtd = b_xtd;
  memcpy( pc_data->pb_data, pui8_data, DLC );

  for (iter = pc_serverData->mlist_clients.begin(); iter != pc_serverData->mlist_clients.end(); iter++) {

    if (!iter->canBus(b_bus).mb_busUsed)
      continue;

    // send signal 0 (no signal is send, but error handling is done) to check is process is alive
    if (kill(iter->ui16_pid, 0) == -1) {
      // client dead!
      DEBUG_PRINT1("client with ID %d no longer alive!\n", iter->ui16_pid);
      // remove this client from list after iterator loop
      iter_delete = iter;
      continue;
    }

    // i32_clientID != 0 in forwarding mode during send, do not enqueue this message for sending client
    if (ui16_pid && (iter->ui16_pid == ui16_pid))
      continue;

    // now search for MsgObj queue on this b_bus, where new message from b_bus maps
    // start with 1 since MsgObj with id 0 is anyway planned for sending
    const int32_t i32_maxObj = iter->canBus(b_bus).mvec_msgObj.size();
    for (int32_t i32_obj = 1; i32_obj < i32_maxObj; i32_obj++) {

      if ( (iter->canBus(b_bus).mvec_msgObj[i32_obj].ui8_bMsgType != RX )
        || (iter->canBus(b_bus).mvec_msgObj[i32_obj].ui16_size == 0     ) )
      { // this MsgObj is no candidate for message receive
        continue;
      }

      if (!iter->canBus(b_bus).mvec_msgObj[i32_obj].b_canObjConfigured) {
        continue;
      }

      if ( iter->canBus(b_bus).mvec_msgObj[i32_obj].b_canBufferLock ) {
        // don't even check this MsgObj as it shall not receive messages
        DEBUG_PRINT2("lock bus %d, obj %d\n", b_bus, i32_obj);
        continue;
      }

      // compare received msg with filter
         if (
             ( (iter->canBus(b_bus).mvec_msgObj[i32_obj].ui8_bufXtd == 1)
                   && (b_xtd == 1)
                   && ( (ui32_id & iter->canBus(b_bus).mvec_msgObj[i32_obj].ui32_mask_xtd) == (iter->canBus(b_bus).mvec_msgObj[i32_obj].ui32_filter & iter->canBus(b_bus).mvec_msgObj[i32_obj].ui32_mask_xtd) )
                 )
                 ||
                 ( (iter->canBus(b_bus).mvec_msgObj[i32_obj].ui8_bufXtd == 0)
                   && (b_xtd == 0)
                   && ( (ui32_id & iter->canBus(b_bus).mvec_msgObj[i32_obj].ui16_mask_std) == (iter->canBus(b_bus).mvec_msgObj[i32_obj].ui32_filter & iter->canBus(b_bus).mvec_msgObj[i32_obj].ui16_mask_std) )
                 )
            )
        { // received msg fits actual filter
          DEBUG_PRINT("queueing message\n");

          pc_data->i32_time = getClientTime(*iter);

          msqReadBuf.i32_mtypePidBusObj = __HAL::assembleRead_mtype(iter->ui16_pid, b_bus, COMMON_MSGOBJ_IN_QUEUE);
          msqReadBuf.s_canData.bMsgObj = i32_obj;

          int i_rcSnd=msgsnd(pc_serverData->ms_msqDataServer.i32_rdHandle, &msqReadBuf, sizeof(__HAL::msqRead_s) - sizeof(long), IPC_NOWAIT);
          if (i_rcSnd == -1)
          {
            int error = errno;
            DEBUG_PRINT2("error in msgsnd (errno: %d) %s\n", error, strerror(error));
            #ifdef CAN_SERVER_LOG_PATH
            std::ofstream logging( CAN_SERVER_LOG_PATH );
            logging << "error in msgsnd (errno: " << errno << ")" << std::endl;
            #endif
            if (error == EAGAIN)
            { // queue is full => remove oldest msg and try again
              __HAL::msqWrite_s msqWriteBuf;
              DEBUG_PRINT4("message queue for CAN Ident: %x with Filter: %x, Mask: %x for MsgObj: %d is full => try to remove oldest msg and send again!!\n",
                           ui32_id, iter->canBus(b_bus).mvec_msgObj[i32_obj].ui32_filter, iter->canBus(b_bus).mvec_msgObj[i32_obj].ui32_mask_xtd, i32_obj );
              #ifdef CAN_SERVER_LOG_PATH
              logging << "message queue for CAN Ident: " << ui32_id << " with Filter: "
                << iter->canBus(b_bus).mvec_msgObj[i32_obj].ui32_filter << ", global Mask: " << iter->canBus(b_bus).mui32_globalMask
                << " for MsgObj NR: " << i32_obj
                << " is full => try to remove oldest msg and send again!!" << std::endl;
              #endif
              int i_rcRcv = msgrcv(pc_serverData->ms_msqDataServer.i32_rdHandle, &msqWriteBuf, sizeof(__HAL::msqWrite_s) - sizeof(long), 0,IPC_NOWAIT);
              if ( i_rcRcv > 0 )
              { // number of received bytes > 0 => msgrcv successfull => try again
                DEBUG_PRINT("oldest msg from queue removed!!\n");
                #ifdef CAN_SERVER_LOG_PATH
                logging << "oldest msg from queue removed!!" << std::endl;
                #endif
                i_rcSnd=msgsnd(pc_serverData->ms_msqDataServer.i32_rdHandle, &msqReadBuf, sizeof(__HAL::msqRead_s) - sizeof(long), IPC_NOWAIT);
                if (i_rcSnd == 0)
                {
                  DEBUG_PRINT("message sent again after queue full!!\n");
                  #ifdef CAN_SERVER_LOG_PATH
                  logging << "message sent again after queue full!!" << std::endl;
                  #endif
                }
              }
            }
          }

#ifndef CAN_SERVER_SKIP_CLIENT_WAKEUP_VIA_PIPE
          if (iter->i32_pipeHandle) {
            static uint8_t ui8_buf;

#ifndef CAN_SERVER_SKIP_PIPE_CLEARING
            // clear pipe (is done also in client)
            ssize_t dontcareRead = read(iter->i32_pipeHandle, &ui8_buf, 1); // just read one byte into static variable
            (void)dontcareRead;
#endif
            // trigger wakeup
            ssize_t dontcareWrite = write(iter->i32_pipeHandle, &ui8_buf, 1);
            (void)dontcareWrite;
          }
#endif

          // don't check following objects if message is already enqueued for this client
          break;

        } // if fit

    } // for objNr
  }// for iter

  if (iter_delete != pc_serverData->mlist_clients.end())
    // clear read/write queue for this client, close pipe, remove from client list
    releaseClient(pc_serverData, iter_delete);

}


// This thread handles the client's writes to the server (us).
/////////////////////////////////////////////////////////////////////////
static void* can_write_thread_func(void* ptr)
{
  int16_t i16_rc = 1; // default means OK (no error)
  int32_t i32_error;
  __HAL::msqWrite_s msqWriteBuf;

  __HAL::server_c* pc_serverData = static_cast< __HAL::server_c * >(ptr);

  for (;;) {

    i32_error = 0;

    if (msgrcv(pc_serverData->ms_msqDataServer.i32_wrHandle, &msqWriteBuf, sizeof(__HAL::msqWrite_s) - sizeof(long), MTYPE_ANY, 0) == -1)
    {
      if (pc_serverData->mb_interactive) {
        perror("msgrcv-all");
      }
      usleep(1000);
      continue;
    }


    // acquire mutex (prevents concurrent read/write access to can driver and modification of client list during execution of enqueue_msg
    pthread_mutex_lock( &(pc_serverData->mt_protectClientList) );

    if ((pc_serverData->mi16_reducedLoadOnIsoBus == msqWriteBuf.ui8_bus) && (msqWriteBuf.s_canMsg.i32_msgType != 0))
    { // We're on ISOBUS with EXTENDED Identifier, so mark this SA as LOCAL (i.e. NOT REMOTE)
      #if DEBUG_CANSERVER
      if (pc_serverData->marrb_remoteDestinationAddressInUse[msqWriteBuf.s_canMsg.ui32_id & 0xFF])
      {
        DEBUG_PRINT1("Reduced ISO bus load: source address 0x%x is now LOCAL (has been REMOTE until now)\n", msqWriteBuf.s_canMsg.ui32_id & 0xFF);
      }
      #endif
      pc_serverData->marrb_remoteDestinationAddressInUse[msqWriteBuf.s_canMsg.ui32_id & 0xFF] = false;
    }

    __HAL::client_c* ps_client = NULL;
    // search client...
    for (std::list<__HAL::client_c>::iterator iter = pc_serverData->mlist_clients.begin(); iter != pc_serverData->mlist_clients.end(); iter++)
    {
      if (iter->ui16_pid == msqWriteBuf.ui16_pid) // not used any more in write thread!!! directly having the variables now! disassemble_client_id(msqWriteBuf.i32_mtype))
      {
        ps_client = &(*iter);
        break;
      }
    }
    if (ps_client != NULL)
    {
      bool b_sendOnBus = true;
      // destination address check based on already collected source addresses from CAN bus messages
      if ( (pc_serverData->mi16_reducedLoadOnIsoBus == msqWriteBuf.ui8_bus) && (msqWriteBuf.s_canMsg.i32_msgType != 0) &&
           (((msqWriteBuf.s_canMsg.ui32_id >> 16) & 0xFF) < 0xF0) && // PDU1 check
           (((msqWriteBuf.s_canMsg.ui32_id >> 8) & 0xFF) < 0xFE) ) // no broadcast (0xFF) or invalid (0xFE) message
      {
        if (!pc_serverData->marrb_remoteDestinationAddressInUse[(msqWriteBuf.s_canMsg.ui32_id >> 8) & 0xFF])
        { // destination address matches LOCAL source address
          b_sendOnBus = false; // ==> So DON'T send out on real ISOBUS.
        }
      }

      if (b_sendOnBus)
      {
        if (isBusOpen(msqWriteBuf.ui8_bus)) {
          DEBUG_PRINT("+");
          i16_rc = sendToBus(msqWriteBuf.ui8_bus, &(msqWriteBuf.s_canMsg), pc_serverData);
        } else {
          i16_rc = 0; // can't longer deny that it's an error
        }
      }

      if (pc_serverData->mb_logMode)
      {
        dumpCanMsg(
            msqWriteBuf.ui8_bus,
            msqWriteBuf.ui8_obj,
            &msqWriteBuf.s_canMsg,
            pc_serverData);
      }
      if (pc_serverData->mb_monitorMode)
      {
        monitorCanMsg (msqWriteBuf.ui8_bus, msqWriteBuf.ui8_obj, &(msqWriteBuf.s_canMsg));
      }

      if (!i16_rc)
        i32_error = HAL_OVERFLOW_ERR;
      else
      { // i16_rc != 0: send was ok
        // get clientID from msqWriteBuf.i32_mtype
        enqueue_msg(msqWriteBuf.s_canMsg.i32_len, msqWriteBuf.s_canMsg.ui32_id, msqWriteBuf.ui8_bus, msqWriteBuf.s_canMsg.i32_msgType, &msqWriteBuf.s_canMsg.ui8_data[0], msqWriteBuf.ui16_pid, pc_serverData); // not done any more: disassemble_client_id(msqWriteBuf.i32_mtype)
      }

      send_command_ack (msqWriteBuf.ui16_pid, &(pc_serverData->ms_msqDataServer), ACKNOWLEDGE_DATA_CONTENT_ERROR_VALUE, i32_error); // not used any more, we have now direct fields in: disassemble_client_id(msqWriteBuf.i32_mtype)
    }

    pthread_mutex_unlock( &(pc_serverData->mt_protectClientList) );

  }

  return NULL;
}


/////////////////////////////////////////////////////////////////////////
static void can_read(__HAL::server_c* pc_serverData)
{
  __HAL::can_recv_data receiveData;
  uint32_t DLC;
  bool b_moreToRead = true;
  bool b_processMsg = false;

  static canMsg_s s_canMsg;

  fd_set rfds;
  int retval;
  uint32_t channel;

  uint32_t channel_with_change = 0;
  uint8_t ui8_cntOpenDevice = 0;
  uint32_t ui32_sleepTime = 10000;

  for (;;) {

    b_processMsg = false;

    FD_ZERO(&rfds);

    ui8_cntOpenDevice = 0;

    for (channel=0; channel < pc_serverData->nCanBusses(); channel++ )
    {
      if (isBusOpen(channel) && (pc_serverData->canBus(channel).mi32_can_device > 0))
      { // valid file handle => use it for select
        FD_SET(pc_serverData->canBus(channel).mi32_can_device, &rfds);
        ui8_cntOpenDevice++;
      }
    }

    // COMMAND_INIT will write one wake byte to this pipe => select returns and file descriptor for new bus is used in next select call
    FD_SET(pc_serverData->marri32_fileDescrWakeUpPipeForNewBusEvent[0], &rfds);

    // we have open devices
    if (ui8_cntOpenDevice > 0) {

      retval = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);

      if(retval == -1) {
        DEBUG_PRINT("Error Occured in select\n");
        break;
      } else if (retval == 0) {
        // timeout should not happen
        continue;
      } else {

        if (FD_ISSET(pc_serverData->marri32_fileDescrWakeUpPipeForNewBusEvent[0], &rfds) == 1) {
          // wake up for new bus => just read pipe for clearing
          char readbuffer[1];
          ssize_t dontcareRead = read(pc_serverData->marri32_fileDescrWakeUpPipeForNewBusEvent[0], readbuffer, sizeof(readbuffer));
          (void)dontcareRead;
        }

        // get device with changes
        for (channel_with_change=0; channel_with_change < pc_serverData->nCanBusses(); channel_with_change++ ) {
          if (isBusOpen(channel_with_change)) {
            if (FD_ISSET(pc_serverData->canBus(channel_with_change).mi32_can_device, &rfds) == 1) {
              DEBUG_PRINT2("change on channel %d, FD_ISSET %d\n", channel_with_change, FD_ISSET(pc_serverData->canBus(channel_with_change).mi32_can_device, &rfds));
              b_processMsg = true;
              break;
            }
          }
        }
      }
    }
    else
    {
      usleep(ui32_sleepTime); // no device => nothing to read
      // check devices which have no device handle (RTE)
      b_processMsg = true;
    }

    if (b_processMsg) {
      // acquire mutex to prevent concurrent read/write to can driver
      pthread_mutex_lock( &(pc_serverData->mt_protectClientList) );

      bool b_rc = readFromBus(channel_with_change, &s_canMsg, pc_serverData);

      if (b_rc)
        ui32_sleepTime = 5000; // CAN message received => reduce sleep time
      else
      { // invalid message or no CAN message received => increase sleep time
        ui32_sleepTime = 10000; 
        pthread_mutex_unlock( &(pc_serverData->mt_protectClientList) );
        continue;
      }

    }

    if (pc_serverData->mb_inputFileMode && b_moreToRead) {
      b_moreToRead = readCanDataFile(pc_serverData, &receiveData);
      b_processMsg = true;

      // acquire mutex to prevent modification of client list during execution of enqueue_msg
      pthread_mutex_lock( &(pc_serverData->mt_protectClientList) );

    }

    if (!b_processMsg) {
      continue;
    }

    DLC = ( s_canMsg.i32_len & 0xF );
    if ( DLC > 8 ) DLC = 8;

    const uint8_t b_xtd = s_canMsg.i32_msgType;

    DEBUG_PRINT4("DLC %d, ui32_id 0x%08x, b_bus %d, b_xtd %d\n", DLC, s_canMsg.ui32_id, channel_with_change, b_xtd);

    if (s_canMsg.ui32_id >= 0x7FFFFFFF) {
      DEBUG_PRINT1("!!Received of malformed message with undefined CAN ident: %x\n", s_canMsg.ui32_id);
    }
    else
    { // check for new source address
      if ((pc_serverData->mi16_reducedLoadOnIsoBus == (int16_t)channel_with_change) && (s_canMsg.i32_msgType != 0))
      { // On ISOBUS, mark this SA as REMOTE
        #if DEBUG_CANSERVER
        if (!pc_serverData->marrb_remoteDestinationAddressInUse[s_canMsg.ui32_id & 0xFF] && ((s_canMsg.ui32_id & 0xFF) != 0xFE)) // skip 0xFE source address
        { // new, unknown source address
          DEBUG_PRINT1("Reduced ISO bus load: new source address 0x%x is now marked as REMOTE (was LOCAL before).\n", s_canMsg.ui32_id & 0xFF);
        }
        #endif
        pc_serverData->marrb_remoteDestinationAddressInUse[s_canMsg.ui32_id & 0xFF] = true;
      }

      if (pc_serverData->mb_logMode)
      {
        dumpCanMsg(
            channel_with_change,
            0/* we don't have no msgobj when receiving .. msqWriteBuf.ui8_obj*/,
            &s_canMsg,
            pc_serverData);
      }
      if (pc_serverData->mb_monitorMode)
      {
        monitorCanMsg (channel_with_change, 0/* we don't have no msgobj when receiving .. msqWriteBuf.ui8_obj*/, &s_canMsg);
      }

      enqueue_msg(DLC, s_canMsg.ui32_id, channel_with_change, b_xtd, &s_canMsg.ui8_data[0], 0, pc_serverData);
    }

    // release mutex
    pthread_mutex_unlock( &(pc_serverData->mt_protectClientList) );

  }
}



/////////////////////////////////////////////////////////////////////////
static void* command_thread_func(void* ptr)
{

  int16_t i16_rc;
  int32_t i32_error;
  int32_t i32_dataContent;
  int32_t i32_data = 0;
  __HAL::transferBuf_s s_transferBuf;

  __HAL::server_c* pc_serverData = static_cast< __HAL::server_c * >(ptr);

  for (;;) {

    // check command queue
    /* The length is essentially the size of the structure minus sizeof(mtype) */
    if((i16_rc = msgrcv(pc_serverData->ms_msqDataServer.i32_cmdHandle, &s_transferBuf, sizeof(__HAL::transferBuf_s) - sizeof(long), 0, 0)) == -1) {
      if (pc_serverData->mb_interactive) {
        perror("msgrcv");
      }
      usleep(1000);
      continue;
    }

    DEBUG_PRINT1("cmd %d\n", s_transferBuf.ui16_command);

    i32_error = 0;

    // acquire mutex
    pthread_mutex_lock( &(pc_serverData->mt_protectClientList) );

    // get client
    std::list<__HAL::client_c>::iterator iter_client = pc_serverData->mlist_clients.end(), tmp_iter;
    for (tmp_iter = pc_serverData->mlist_clients.begin(); tmp_iter != pc_serverData->mlist_clients.end(); tmp_iter++)
      if (tmp_iter->ui16_pid == s_transferBuf.i32_mtypePid) // here the mtype is the PID without any disassembling needed!
      {
        iter_client = tmp_iter;
        break;
      }

    // default to simple ACK which returns the error.
    // Overwrite if special answers needed like PIPE_ID or SEND_DELAY
    i32_dataContent = ACKNOWLEDGE_DATA_CONTENT_ERROR_VALUE;
    // in DATA_CONTENT_ERROR_VALUE case i32_error will be used as i32_data!!!
    i32_error = 0; // default to no error
    if (s_transferBuf.ui16_command != COMMAND_REGISTER)
    { /// iter_client check needed, iter_client WILL be used and MUST be set!
      if (iter_client == pc_serverData->mlist_clients.end())
      { // client not found, but was required. return HAL_CONFIG_ERR
        i32_error = HAL_CONFIG_ERR;
      }
    }

    // if still no error, evaluate command
    if (!i32_error)
    {
      switch (s_transferBuf.ui16_command)
      {

        case COMMAND_REGISTER:
        {
          __HAL::client_c s_tmpClient; // constructor initialize values to zero
          DEBUG_PRINT("command start driver\n");

          // do check for dead clients before queueing any new message
          for (std::list<__HAL::client_c>::iterator iter_deadClient = pc_serverData->mlist_clients.begin(); iter_deadClient != pc_serverData->mlist_clients.end();) {

            // send signal 0 (no signal is send, but error handling is done) to check is process is alive
            if (kill(iter_deadClient->ui16_pid, 0) == -1) {
              // client dead!
              DEBUG_PRINT1("client with ID %d no longer alive!\n", iter_deadClient->ui16_pid);
              // clear read/write queue for this client, close pipe, remove from client list, iter_deadClient is incremented
              releaseClient(pc_serverData, iter_deadClient);
            } else
              // increment iter_deadClient manually (not in for statement)
              iter_deadClient++;
          }

          // no need to set to 0, as everything got set to zero before!
          // s_tmpClient.canBus(all-buses).mi32_sendDelay = 0;

          // client process id is used as clientID
          s_tmpClient.ui16_pid = s_transferBuf.i32_mtypePid;

          DEBUG_PRINT1 ("Client registering with startTimeClock_t from his REGISTER message as %d\n", s_transferBuf.s_startTimeClock.t_clock);
          initClientTime( s_tmpClient, s_transferBuf.s_startTimeClock.t_clock );

          //DEBUG_PRINT1("client start up time (absolute value in clocks): %d\n", s_tmpClient.t_startTimeClock);

          char pipe_name[255];
          sprintf(pipe_name, "%s%d", PIPE_PATH, ++(pc_serverData->mi32_lastPipeId));

          umask(0);
          // if not existing already
          mknod(pipe_name, S_IFIFO|0666, 0);

          // open pipe in read/write mode to allow read access to clean the pipe and to avoid SIGPIPE when client dies and this client was the only reader to the pipe
          if ((s_tmpClient.i32_pipeHandle = open(pipe_name, O_NONBLOCK | O_RDWR, 0)) == -1)
            i32_error = HAL_UNKNOWN_ERR;

          if (!i32_error)
          { // no error
            pc_serverData->mlist_clients.push_back(s_tmpClient);
            // transmit current pipeId to client (for composition of pipe name)
            i32_dataContent = ACKNOWLEDGE_DATA_CONTENT_PIPE_ID;
            i32_data = pc_serverData->mi32_lastPipeId;
          }
        } break;


        case COMMAND_DEREGISTER: {
          DEBUG_PRINT("command stop driver\n");

          for (uint8_t j=0; j < iter_client->nCanBusses(); j++)
          {
            iter_client->canBus(j).mvec_msgObj.clear();
            if (iter_client->canBus(j).mb_initReceived && (pc_serverData->canBus(j).mui16_busRefCnt > 0))
              pc_serverData->canBus(j).mui16_busRefCnt--; // decrement ref count only when we received the INIT command before
          }

          if (iter_client->i32_pipeHandle)
            close(iter_client->i32_pipeHandle);

          pc_serverData->mlist_clients.erase(iter_client);
          // i32_error will stay at 0 for "no error"
        } break;


        case COMMAND_INIT:

          if (s_transferBuf.s_config.ui8_bus > HAL_CAN_MAX_BUS_NR)
            i32_error = HAL_RANGE_ERR;
          else if (!pc_serverData->canBus(s_transferBuf.s_init.ui8_bus).mui16_busRefCnt)
          { // first init command for current bus
            // open log file only once per bus
            if (pc_serverData->mb_logMode) {
              (void)newFileLog( pc_serverData, s_transferBuf.s_init.ui8_bus );
            }

            int16_t i16_init_rc = openBusOnCard(s_transferBuf.s_init.ui8_bus,  // 0 for CANLPT/ICAN, else 1 for first BUS
                                                s_transferBuf.s_init.ui16_wBitrate,  // BTR0BTR1
                                                pc_serverData);

            if (!i16_init_rc) {
              std::cerr << "Can't initialize CAN-BUS." << std::endl;
              std::cerr << "CAN device/driver not ready.\n" << std::endl;
              i32_error = HAL_CONFIG_ERR;
              abort();
            }

            // wake up can_read thread to use file descriptor of new bus in next select call
            char writebuffer[1];
            ssize_t dontcareWrite = write(pc_serverData->marri32_fileDescrWakeUpPipeForNewBusEvent[1], writebuffer, sizeof(writebuffer));
            (void)dontcareWrite;
          }

          if (!i32_error) {
            pc_serverData->canBus(s_transferBuf.s_init.ui8_bus).mui16_busRefCnt++;
            iter_client->canBus(s_transferBuf.s_init.ui8_bus).mb_initReceived = true; // when the CLOSE command is received => allow decrement of ref count
            iter_client->canBus(s_transferBuf.s_init.ui8_bus).mb_busUsed = true; // when the CLOSE command is received => allow decrement of ref count
          }
          // do rest of init handling in next case statement (no break!)

        case COMMAND_CHG_GLOBAL_MASK:
          if (!i32_error) {
            iter_client->canBus(s_transferBuf.s_init.ui8_bus).mui16_globalMask = s_transferBuf.s_init.ui16_wGlobMask;
            iter_client->canBus(s_transferBuf.s_init.ui8_bus).mui32_globalMask = s_transferBuf.s_init.ui32_dwGlobMask;
            iter_client->canBus(s_transferBuf.s_init.ui8_bus).mui32_lastMask = s_transferBuf.s_init.ui32_dwGlobMaskLastmsg;
          }
          break;


        case COMMAND_CLOSE:
        {
          if (s_transferBuf.s_config.ui8_bus > HAL_CAN_MAX_BUS_NR )
            i32_error = HAL_RANGE_ERR;
          else
          {
            if (iter_client->canBus(s_transferBuf.s_init.ui8_bus).mb_initReceived && (pc_serverData->canBus(s_transferBuf.s_init.ui8_bus).mui16_busRefCnt > 0))
            {
              pc_serverData->canBus(s_transferBuf.s_init.ui8_bus).mui16_busRefCnt--; // decrement ref count only when we received the INIT command before
            }

            iter_client->canBus(s_transferBuf.s_init.ui8_bus).mb_initReceived = false; // reset flag


            __HAL::clearReadQueue (s_transferBuf.s_init.ui8_bus, COMMON_MSGOBJ_IN_QUEUE, pc_serverData->ms_msqDataServer.i32_rdHandle, iter_client->ui16_pid);
//          clearWriteQueue(s_transferBuf.s_init.ui8_bus, COMMON_MSGOBJ_IN_QUEUE, pc_serverData->ms_msqDataServer.i32_wrHandle, iter_client->ui16_pid);

            if (!pc_serverData->canBus(s_transferBuf.s_init.ui8_bus).mui16_busRefCnt && pc_serverData->mb_logMode) {
              closeFileLog(pc_serverData, s_transferBuf.s_init.ui8_bus);
            }

            iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj.clear();
            // i32_error will stay at 0 for "no error"
          }
        } break;


        case COMMAND_CONFIG:
          if (s_transferBuf.s_config.ui8_obj >= iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj.size()) {
            // add new elements in the vector with resize
            iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj.resize(s_transferBuf.s_config.ui8_obj+1);
          } else {
            // reconfigure element
          }
          // break left out intentionally

        case COMMAND_CHG_CONFIG:

          if ((s_transferBuf.s_config.ui8_bus > HAL_CAN_MAX_BUS_NR ) || ( s_transferBuf.s_config.ui8_obj > iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj.size()-1 ))
            i32_error = HAL_RANGE_ERR;
          else
          {
            iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].b_canObjConfigured = true;

            iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].ui8_bufXtd = s_transferBuf.s_config.ui8_bXtd;
            iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].ui32_filter = s_transferBuf.s_config.ui32_dwId;

            if (s_transferBuf.s_config.ui8_bXtd)
                iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].ui32_mask_xtd = s_transferBuf.s_config.ui32_mask;
            else
                iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].ui16_mask_std = s_transferBuf.s_config.ui32_mask;

            if (s_transferBuf.ui16_command == COMMAND_CONFIG) {
              __HAL::clearReadQueue (s_transferBuf.s_config.ui8_bus, s_transferBuf.s_config.ui8_obj, pc_serverData->ms_msqDataServer.i32_rdHandle, iter_client->ui16_pid);
//            clearWriteQueue(s_transferBuf.s_config.ui8_bus, s_transferBuf.s_config.ui8_obj, pc_serverData->ms_msqDataServer.i32_wrHandle, iter_client->ui16_pid);

              iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].ui8_bMsgType = s_transferBuf.s_config.ui8_bMsgType;
              iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].ui16_size = s_transferBuf.s_config.ui16_wNumberMsgs;
              iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].b_canBufferLock = false;
            }
          }
          break;


        case COMMAND_LOCK:
        case COMMAND_UNLOCK:

          if ((s_transferBuf.s_config.ui8_bus > HAL_CAN_MAX_BUS_NR ) || ( s_transferBuf.s_config.ui8_obj > iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj.size()-1 ))
            i32_error = HAL_RANGE_ERR;
          else {
            if (s_transferBuf.ui16_command == COMMAND_LOCK) {
              iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].b_canBufferLock = true;
              DEBUG_PRINT2("locked buf %d, obj %d\n", s_transferBuf.s_config.ui8_bus, s_transferBuf.s_config.ui8_obj);
            } else {
              iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].b_canBufferLock = false;
              DEBUG_PRINT2("unlocked buf %d, obj %d\n", s_transferBuf.s_config.ui8_bus, s_transferBuf.s_config.ui8_obj);
            }
          }
          break;


        case COMMAND_QUERYLOCK:

          if ((s_transferBuf.s_config.ui8_bus > HAL_CAN_MAX_BUS_NR ) || ( s_transferBuf.s_config.ui8_obj > iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj.size()-1 ))
            i32_error = HAL_RANGE_ERR;
          else {
            i32_dataContent = ACKNOWLEDGE_DATA_CONTENT_QUERY_LOCK;
            i32_data = iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].b_canBufferLock;
          }
          break;


        case COMMAND_CLOSEOBJ:

          if ((s_transferBuf.s_config.ui8_bus > HAL_CAN_MAX_BUS_NR ) || ( s_transferBuf.s_config.ui8_obj > iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj.size()-1 ))
            i32_error = HAL_RANGE_ERR;
          else {
            iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].b_canObjConfigured = false;

            iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].b_canBufferLock = false;
            iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj[s_transferBuf.s_config.ui8_obj].ui16_size = 0;
            __HAL::clearReadQueue (s_transferBuf.s_config.ui8_bus, s_transferBuf.s_config.ui8_obj, pc_serverData->ms_msqDataServer.i32_rdHandle, iter_client->ui16_pid);
//          clearWriteQueue(s_transferBuf.s_config.ui8_bus, s_transferBuf.s_config.ui8_obj, pc_serverData->ms_msqDataServer.i32_wrHandle, iter_client->ui16_pid);

            // erase element if it is the last in the vector, otherwise it can stay there
            while (iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj.back().b_canObjConfigured == false)
                iter_client->canBus(s_transferBuf.s_config.ui8_bus).mvec_msgObj.pop_back();
          }
          break;


        case COMMAND_SEND_DELAY:
          if ( s_transferBuf.s_config.ui8_bus > HAL_CAN_MAX_BUS_NR )
          {
            i32_error = HAL_RANGE_ERR;
          } else {
            i32_dataContent = ACKNOWLEDGE_DATA_CONTENT_SEND_DELAY;
            i32_data = iter_client->canBus(s_transferBuf.s_config.ui8_bus).mi32_sendDelay;
            if (i32_data < 0) i32_data = -i32_data; // so we always return a positive send delay!
            else iter_client->canBus(s_transferBuf.s_config.ui8_bus).mi32_sendDelay  = -i32_data; // stamp a positive stored delay as READ by negating it!
          }
          break;


        default:
          i32_error = HAL_UNKNOWN_ERR;
          break;
      } // end switch
    } // end error check


    // do centralized error-answering here
    if (i32_dataContent == ACKNOWLEDGE_DATA_CONTENT_ERROR_VALUE) i32_data = i32_error;
    send_command_ack(s_transferBuf.i32_mtypePid, &(pc_serverData->ms_msqDataServer), i32_dataContent, i32_data);


    // release mutex
    pthread_mutex_unlock( &(pc_serverData->mt_protectClientList) );

  } // end for

}



void sendUserMsg(uint32_t DLC, uint32_t ui32_id, uint32_t ui32_bus, uint8_t ui8_xtd, uint8_t* pui8_data, __HAL::server_c* pc_serverData)
{
  static canMsg_s s_canMsg;

  // set the necessary data for monitorCanMsg
  s_canMsg.i32_msgType = ui8_xtd;
  s_canMsg.i32_len = DLC;
  s_canMsg.ui32_id = ui32_id;
  memcpy(s_canMsg.ui8_data, pui8_data, DLC);

  // acquire mutex (prevents concurrent read/write access to can driver and modification of client list during execution of enqueue_msg
  pthread_mutex_lock( &(pc_serverData->mt_protectClientList) );

  enqueue_msg(DLC, ui32_id, ui32_bus, ui8_xtd, pui8_data, 0, pc_serverData);

  if (isBusOpen(ui32_bus)) {
    sendToBus(ui32_bus, &s_canMsg, pc_serverData);
  }

  pthread_mutex_unlock( &(pc_serverData->mt_protectClientList) );

  if (pc_serverData->mb_logMode)
  {
    dumpCanMsg(ui32_bus, 0, &s_canMsg, pc_serverData);
  }

  if (pc_serverData->mb_monitorMode)
  { 
    monitorCanMsg (ui32_bus, 0, &s_canMsg);
  }
}



/////////////////////////////////////////////////////////////////////////

#if DEBUG_CANSERVER
void print_trace (void)
{
    void * array[100];
    int size;
    char ** strings;
    int i;

    int pid = getpid();

    size = backtrace (array, 100);
    strings = backtrace_symbols (array, size);

    fprintf (stderr,"Obtained %i stack frames.\n", size);

    for (i = 0; i < size; i++)
    {
        fprintf (stderr,"(%d) %s\n", pid, strings[i]);
    }
}


void segfaulthand(int /* i */ )
{
    delete[] reserve_memory_heap;

    print_trace();
    killpg( 0, 9 ); // pgid 0 means process group of current process.
    exit(-1);
}
#endif

yasper::ptr< AOption_c > const ga_options[] = {
  Option_c< OPTION_MONITOR >::create(),
  Option_c< OPTION_LOG >::create(),
  Option_c< OPTION_FILE_INPUT >::create(),
  Option_c< OPTION_REDUCED_LOAD_ISO_BUS_NO >::create(),
  Option_c< OPTION_NICE_CAN_READ >::create(),
  Option_c< OPTION_INTERACTIVE >::create(),
  Option_c< OPTION_PRODUCTIVE >::create(),
#ifndef WIN32
  Option_c< OPTION_DAEMON > ::create(),
#endif
  Option_c< OPTION_HELP >::create(),
};

yasper::ptr< AOption_c > const *const gp_optionsBegin = ga_options;
yasper::ptr< AOption_c > const *const gp_optionsEnd = ga_options +
  (sizeof ga_options) / (sizeof ga_options[0]);

int main(int argc, char *argv[])
{
  __HAL::server_c c_serverData;

  checkAndHandleOptionsAndStartup( argc, argv, c_serverData );

  // wait to be shure that CAN card is clean reset
  //  usleep(100);

  int16_t i16_rc = createMsqs(c_serverData.ms_msqDataServer);

  if (i16_rc) {
    std::cerr << "error creating queues\n" << std::endl;
    exit(1);
  }
#if DEBUG_CANSERVER
  reserve_memory_heap = new char[4096];
  signal(11,segfaulthand);
#endif

  if (pipe(c_serverData.marri32_fileDescrWakeUpPipeForNewBusEvent) != 0) {
    std::cerr << "error creating wakeup pipe for new bus event\n" << std::endl;
    exit(1);
  }

  pthread_t thread_registerClient, thread_canWrite;
  int threadError = 0;

  threadError |= pthread_create( &thread_registerClient, NULL, &command_thread_func, &c_serverData);
  threadError |= pthread_create( &thread_canWrite, NULL, &can_write_thread_func, &c_serverData);
  
  if (c_serverData.mb_interactive) {
    pthread_t thread_readUserInput;
    threadError |= pthread_create( &thread_readUserInput, NULL, &readUserInput, &c_serverData );
  }

  if( threadError != 0 )
  {
    std::cerr << "Error creating thread(s).\n" << std::endl;
    exit( threadError );
  }

#ifndef WIN32
  if (c_serverData.mi_canReadNiceValue)
    setpriority(PRIO_PROCESS, 0, c_serverData.mi_canReadNiceValue);
#endif

  can_read(&c_serverData);
}


template <>
int Option_c< OPTION_FILE_INPUT >::doCheckAndHandle(int argc, char *argv[], int ai_pos, __HAL::server_c &ar_server) const
{
  if (!strcmp(argv[ai_pos], "--file-input")) {
    if (ai_pos+1>=argc) {
      std::cerr << "error: option needs second parameter" << std::endl;
      exit(1);
    }
    ar_server.mstr_inputFile = argv[ai_pos+1];
    ar_server.mb_inputFileMode=true;
    return 2;
  }
  return 0;
}

template <>
std::string Option_c< OPTION_FILE_INPUT >::doGetSetting(__HAL::server_c &ar_server) const
{
  std::ostringstream ostr_setting;
  if (ar_server.mb_inputFileMode) {
    ostr_setting << "Reading can data from file " <<
      ar_server.mstr_inputFile << std::endl;
  }
  return ostr_setting.str();
}

template <>
std::string Option_c< OPTION_FILE_INPUT >::doGetUsage() const
{
  return "  --file-input LOG_FILE_NAME   read can data from file <LOG_FILE_NAME>\n";
}

template <>
int Option_c< OPTION_NICE_CAN_READ >::doCheckAndHandle(int argc, char *argv[], int ai_pos, __HAL::server_c &ar_server) const
{
  if (!strcmp(argv[ai_pos], "--nice-can-read")) {
    if (ai_pos+1>=argc) {
      std::cerr << "error: option needs second parameter" << std::endl;
      exit(1);
    }
    ar_server.mi_canReadNiceValue = atoi (argv[ai_pos+1]); // 0 for no prio mode!
    std::cout << "nice value for can read thread: " << ar_server.mi_canReadNiceValue << std::endl;
    return 2;
  }
  return 0;
}

template <>
std::string Option_c< OPTION_NICE_CAN_READ >::doGetSetting(__HAL::server_c &ar_server) const
{
  std::ostringstream ostr_setting;
  if (ar_server.mi_canReadNiceValue > 0) {
    ostr_setting << "Nice value set to " <<
      ar_server.mi_canReadNiceValue << std::endl;
  }
  return ostr_setting.str();
}

template <>
std::string Option_c< OPTION_NICE_CAN_READ >::doGetUsage() const
{
  return 
    "  --nice-can-read VALUE      set a nice value for can read thread to reduce slowing down effect on other applications\n"
    "                             due to heavy OS task switches, 0 < VALUE < 20 for reducing the priority\n\n";
}
