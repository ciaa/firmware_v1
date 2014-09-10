/*
  can_server.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef _PC_HAL_CAN_CAN_SERVER_H_
#define _PC_HAL_CAN_CAN_SERVER_H_

#include <cstring>
#include <string.h>
#include <vector>
#include <IsoAgLib/isoaglib_config.h>

//#define DEBUG_CAN 1

#if DEBUG_CAN
//lint –emacro( {717}, DEBUG_PRINT_ )
#  define DEBUG_PRINT_(args) do { printf args; fflush(0); } while (0)
#else
#  define DEBUG_PRINT_(args)
#endif
// NOTE: Usage of DEBUG_PRINT **ONLY** in CAN-Server!
#define DEBUG_PRINT(str) DEBUG_PRINT_((str))
#define DEBUG_PRINT1(str,a) DEBUG_PRINT_((str,a))
#define DEBUG_PRINT2(str,a,b) DEBUG_PRINT_((str,a,b))
#define DEBUG_PRINT3(str,a,b,c) DEBUG_PRINT_((str,a,b,c))
#define DEBUG_PRINT4(str,a,b,c,d) DEBUG_PRINT_((str,a,b,c,d))
// NOTE: Usage of DEBUG_PRINT **ONLY** in CAN-Server!

#define COMMAND_ACKNOWLEDGE     1
#define COMMAND_REGISTER        10
#define COMMAND_DEREGISTER      11
#define COMMAND_INIT            20
#define COMMAND_CLOSE           21
#define COMMAND_CHG_GLOBAL_MASK 22
#define COMMAND_CONFIG          30
#define COMMAND_CHG_CONFIG      31
#define COMMAND_LOCK            40
#define COMMAND_UNLOCK          41
#define COMMAND_QUERYLOCK       42
#define COMMAND_CLOSEOBJ        50
#define COMMAND_SEND_DELAY      60
#define COMMAND_DATA            70

#define ACKNOWLEDGE_DATA_CONTENT_ERROR_VALUE 0
#define ACKNOWLEDGE_DATA_CONTENT_PIPE_ID     1
#define ACKNOWLEDGE_DATA_CONTENT_SEND_DELAY  2
#define ACKNOWLEDGE_DATA_CONTENT_QUERY_LOCK  3

// msq specific defines
#define MTYPE_ANY               0x0
#define MTYPE_WRITE_PRIO_NORMAL 0x1
#define MTYPE_WRITE_PRIO_HIGH   0x3

#define MSQ_COMMAND      'm'
#define MSQ_COMMAND_ACK  'n'
#define MSQ_CLIENT_READ  'o'
#define MSQ_CLIENT_WRITE 'p'

#define MSQ_KEY_PATH           "/root"
// this directory will be created if MSQ_KEY_PATH doesn't exist
#define MSQ_KEY_PATH_FALLBACK  "/tmp/can_server_key_dir"

#define PIPE_PATH        "/tmp/can_server_"


// socket specific defines
#ifndef CAN_SERVER_CHANNEL
	#define CAN_SERVER_CHANNEL 0
#endif

#define BREAK_WAIT_PORT       (36797)
#define COMMAND_TRANSFER_PORT (36798 + (CAN_SERVER_CHANNEL)*2)
#define DATA_TRANSFER_PORT    (36799 + (CAN_SERVER_CHANNEL)*2)



// USE_UNIX_SOCKET to use /tmp/can_server.sock.<command_port> and /tmp/can_server.sock.<data_port> instead of real sockets 
// => no loopback network configuration necessary
//#define USE_UNIX_SOCKET

#ifdef WIN32
  // Note: winsock2.h is included in compiler_adaptations.h for WinCE.
  #ifndef WINCE
    #include <winsock2.h> //needed for canserver
    #include <windows.h>  //also needed!
  #endif
  #include <ctime>
#else
  #include <sys/time.h>
  #include <sys/types.h>
  #include <sys/time.h>
  #include <sys/socket.h>
  #include <unistd.h>
#endif

#ifdef USE_UNIX_SOCKET
  #define SOCKET_TYPE_INET_OR_UNIX AF_UNIX
  #define SOCKET_PATH "/tmp/can_server.sock"
#else
  #define SOCKET_TYPE_INET_OR_UNIX AF_INET
#endif

#ifdef CAN_DRIVER_SOCKET
  #define CAN_SERVER_HOST "127.0.0.1"
#endif

#ifdef WIN32
  typedef SOCKET SOCKET_TYPE;
#else
  typedef int    SOCKET_TYPE;
  #define INVALID_SOCKET -1
  #define SOCKET_ERROR   -1
#endif

#define  RX 0
#define  TX 1

#define COMMON_MSGOBJ_IN_QUEUE  0xFF

struct canMsg_s {
  uint32_t        ui32_id;
  int32_t         i32_msgType;
  int32_t         i32_len;
  uint8_t         ui8_data[8];
};

namespace __HAL {

struct tMsgObj {
  bool     b_canBufferLock;
  bool     b_canObjConfigured;
  uint8_t  ui8_bufXtd;
  uint8_t  ui8_bMsgType;
  uint32_t ui32_filter;
  uint32_t ui32_mask_xtd;
  uint16_t ui16_mask_std;
  uint16_t ui16_size;
};

#ifdef CAN_DRIVER_MESSAGE_QUEUE

struct can_data {
  int32_t i32_time;
  int32_t i32_ident;
  uint8_t b_dlc;
  uint8_t b_xtd;
  uint8_t pb_data[8];
  uint8_t bMsgObj;
};

struct can_recv_data {
  uint32_t b_bus;
  can_data msg;
};

struct msqRead_s {
  long   i32_mtypePidBusObj;
  struct can_data s_canData;
};

struct msqWrite_s {
  long     i32_mtypePrioAnd1; // has now priority and Pid (PID is needed for clearing the queue :-()
  int16_t  ui16_pid;
  uint8_t  ui8_bus;
  uint8_t  ui8_obj;
  canMsg_s s_canMsg;
  int32_t  i32_sendTimeStamp;
};


// message queues / process id
struct msqData_s {
  long    i32_cmdHandle;
  int32_t i32_cmdAckHandle;
  // client read queue
  int32_t i32_rdHandle;
  // client write queue
  int32_t i32_wrHandle;
  int32_t i32_pid;
  int32_t i32_pipeHandle;
};
#endif

struct transferBuf_s {
#ifdef CAN_DRIVER_MESSAGE_QUEUE
  long    i32_mtypePid;
#endif
  uint16_t ui16_command;
  union {
    struct {
      clock_t t_clock;
      int32_t i32_fill1;
      int32_t i32_fill2;
      int32_t i32_fill3;
    } s_startTimeClock;
    struct {
      int32_t i32_dataContent; // set to DATA_CONTENT_xxx
      int32_t i32_data; // depends on dataContent
      int32_t i32_fill2;
      int32_t i32_fill3;
    } s_acknowledge;
    struct {
      // byte 0-3
      uint8_t  ui8_bus;
      uint8_t  ui8_obj;
      uint8_t  ui8_bXtd;
      uint8_t  ui8_bMsgType;
      // byte 4-7
      uint32_t ui32_dwId;
      // byte 8-11
      uint16_t ui16_wNumberMsgs;
      uint16_t ui16_queryLockResult;
      // byte 12-15
      uint32_t ui32_mask;
    } s_config;
    struct {
      // byte 0-3
      uint8_t  ui8_bus;
      uint8_t  ui8_fill1;
      uint16_t ui16_wGlobMask;
      // byte 4-7
      uint32_t ui32_dwGlobMask;
      // byte 8-11
      uint32_t ui32_dwGlobMaskLastmsg;
      // byte 12-15
      uint16_t ui16_wBitrate;
      uint16_t ui16_fill2;
    } s_init;
#ifdef CAN_DRIVER_SOCKET
    struct {
      struct canMsg_s s_canMsg;
      uint8_t  ui8_bus;
      uint8_t  ui8_obj;
      int32_t  i32_sendTimeStamp;
    } s_data;
#endif
  };
  transferBuf_s() {
    memset(this, 0, sizeof *this);
  }
};

// client specific data
struct client_c
{
public:
  client_c();

#ifdef CAN_DRIVER_SOCKET
  SOCKET_TYPE  i32_commandSocket;
  SOCKET_TYPE  i32_dataSocket;
#endif

  uint16_t ui16_pid;
  int32_t  i32_msecStartDeltaClientMinusServer;

  //typedef STL_NAMESPACE::vector<tMsgObj> ArrMsgObj;
  //ArrMsgObj arrMsgObj[cui32_maxCanBusCnt];

  struct canBus_s {
    std::vector<tMsgObj>    mvec_msgObj;
    bool                    mb_busUsed;
    uint16_t                mui16_globalMask;
    uint32_t                mui32_globalMask;
    uint32_t                mui32_lastMask;
    int32_t                 mi32_sendDelay;
    bool                    mb_initReceived;
    canBus_s();
  };
  canBus_s &canBus(size_t n_index);
  size_t nCanBusses();

private:
  std::vector< canBus_s > mvec_canBus;

public:
#ifdef CAN_DRIVER_MESSAGE_QUEUE
  int32_t  i32_pipeHandle;
#endif
};

inline client_c::canBus_s &client_c::canBus(size_t n_index)
{
  if (mvec_canBus.size() <= n_index)
    mvec_canBus.resize(n_index + 1);
  return mvec_canBus[n_index];
}

inline size_t client_c::nCanBusses()
{
  return mvec_canBus.size();
}

#ifdef CAN_DRIVER_MESSAGE_QUEUE
void send_command_ack(int32_t ai32_mtype, msqData_s* p_msqDataServer, int32_t ai32_dataContent, int32_t ai32_data);

int32_t send_command(transferBuf_s* ps_transferBuf, msqData_s* p_msqDataClient);

int16_t createMsqs(msqData_s& msqData);

int32_t assembleRead_mtype (uint16_t ui16_pid, uint8_t bus, uint8_t obj);
int32_t assembleWrite_mtype(bool ab_prio);

uint16_t disassembleRead_client_id (int32_t i32_mtype);
uint16_t disassembleWrite_client_id(int32_t i32_mtype);

void clearReadQueue (uint8_t bBusNumber, uint8_t bMsgObj, int32_t i32_msqHandle, uint16_t ui16_pID);
void clearWriteQueue(bool ab_prio, int32_t i32_msqHandle, uint16_t ui16_pID);
#endif

} // end namespace

#endif
