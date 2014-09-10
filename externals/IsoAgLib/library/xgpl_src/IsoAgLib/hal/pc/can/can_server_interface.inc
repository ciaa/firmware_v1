/*
  msq_helper: helper functions for command messaging

  (C) Copyright 2009 - 2013 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifdef CAN_DRIVER_MESSAGE_QUEUE
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <cstdio>

#include <IsoAgLib/hal/pc/errcodes.h>

namespace __HAL {

void send_command_ack(int32_t ai32_mtype, msqData_s* p_msqDataServer, int32_t ai32_dataContent, int32_t ai32_data)
{
  transferBuf_s s_transferBuf;

  s_transferBuf.i32_mtypePid = ai32_mtype;
  s_transferBuf.ui16_command = COMMAND_ACKNOWLEDGE;
  s_transferBuf.s_acknowledge.i32_dataContent = ai32_dataContent;
  s_transferBuf.s_acknowledge.i32_data = ai32_data;

  msgsnd(p_msqDataServer->i32_cmdAckHandle, &s_transferBuf, sizeof(transferBuf_s) - sizeof(long), IPC_NOWAIT);
}


int32_t send_command(transferBuf_s* p_s_transferBuf, msqData_s* p_msqDataClient)
{
  if (msgsnd(p_msqDataClient->i32_cmdHandle, p_s_transferBuf, sizeof(transferBuf_s) - sizeof(long), 0) == -1)
    return HAL_UNKNOWN_ERR;

  // wait for ACK
  if(msgrcv(p_msqDataClient->i32_cmdAckHandle, p_s_transferBuf, sizeof(transferBuf_s) - sizeof(long), p_msqDataClient->i32_pid, 0) == -1)
    return HAL_UNKNOWN_ERR;

  if (p_s_transferBuf->ui16_command == COMMAND_ACKNOWLEDGE)
  {
    if ( (p_s_transferBuf->s_acknowledge.i32_dataContent == ACKNOWLEDGE_DATA_CONTENT_ERROR_VALUE)
      && (p_s_transferBuf->s_acknowledge.i32_data != 0) )
    { // error!
      printf("nack received in send_command\n");
      return p_s_transferBuf->s_acknowledge.i32_data;
    }
    else
    { // either ACK with NO error OR ACK with PIPE_ID, SEND_DELAY, etc.etc. which is okay!
      return HAL_NO_ERR;
    }
  }
  else
  { // wrong answer to a COMMAND. Must be COMMAND_ACKNOWLEDGE!
    return HAL_UNKNOWN_ERR;
  }
}


/////////////////////////////////////////////////////////////////////////
int16_t createMsqs(msqData_s& msqData)
{
  key_t  msgkey;

  msqData.i32_pid = getpid();
  bool b_useFallback = false;

  struct stat s_stat;
  if (stat(MSQ_KEY_PATH, &s_stat) != 0)
  { // directory does not exist => create default directory
    mkdir(MSQ_KEY_PATH_FALLBACK, 00755);
    b_useFallback = true;
  }

  /* Generate our IPC key value */
  msgkey = ftok((b_useFallback ? MSQ_KEY_PATH_FALLBACK : MSQ_KEY_PATH), MSQ_COMMAND);
  if ((msqData.i32_cmdHandle = msgget(msgkey, IPC_CREAT | 0660 )) == -1)
    return HAL_UNKNOWN_ERR;
  
  msgkey = ftok((b_useFallback ? MSQ_KEY_PATH_FALLBACK : MSQ_KEY_PATH), MSQ_COMMAND_ACK);
  if ((msqData.i32_cmdAckHandle = msgget(msgkey, IPC_CREAT | 0660 )) == -1)
    return HAL_UNKNOWN_ERR;
  
  msgkey = ftok((b_useFallback ? MSQ_KEY_PATH_FALLBACK : MSQ_KEY_PATH), MSQ_CLIENT_READ);
  if ((msqData.i32_rdHandle = msgget(msgkey, IPC_CREAT | 0660 )) == -1)
    return HAL_UNKNOWN_ERR;
  
  msgkey = ftok((b_useFallback ? MSQ_KEY_PATH_FALLBACK : MSQ_KEY_PATH), MSQ_CLIENT_WRITE);
  if ((msqData.i32_wrHandle = msgget(msgkey, IPC_CREAT | 0660 )) == -1)
    return HAL_UNKNOWN_ERR;
  
  return 0;
}

/// Only used in SERVER -> CLIENT direction
int32_t assembleRead_mtype(uint16_t ui16_pid, uint8_t bus, uint8_t obj)
{
  return (ui16_pid << 12 | bus << 8 | obj);
}

uint16_t disassembleRead_client_id(int32_t i32_mtype)
{
  return (i32_mtype >> 12);
}

/// Only used in CLIENT -> SERVER direction
int32_t assembleWrite_mtype(bool ab_prio)
{
  return (ab_prio) ? (MTYPE_WRITE_PRIO_HIGH) : (MTYPE_WRITE_PRIO_NORMAL);
}

// NO MORE CLIENT ID IN <<WRITE>>'s MTYPE
//uint16_t disassembleWrite_client_id(int32_t i32_mtype)
//{ return (i32_mtype >> 1) & 0xFFFF; }


void clearReadQueue(uint8_t bBusNumber, uint8_t bMsgObj, int32_t i32_msqHandle, uint16_t ui16_pid)
{
  msqRead_s msqReadBuf;

  while (msgrcv(i32_msqHandle, &msqReadBuf, sizeof(msqRead_s) - sizeof(long), assembleRead_mtype(ui16_pid, bBusNumber, bMsgObj), IPC_NOWAIT) > 0)
    ;
}

/** we don't clear the write queue anymore now with the new MTYPE format, which only has the prio anymore
void clearWriteQueue(bool ab_prio, int32_t i32_msqHandle, uint16_t ui16_pid)
{
  msqWrite_s msqWriteBuf;

  while (msgrcv(i32_msqHandle, &msqWriteBuf, sizeof(msqWrite_s) - sizeof(long), assembleWrite_mtype(ui16_pid, ab_prio), IPC_NOWAIT) > 0)
    ;
}
*/

} // end namespace __HAL

#endif

#ifdef CAN_DRIVER_SOCKET

#ifndef WIN32
  #include <sys/socket.h>
  #include <sys/un.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <linux/version.h>
#endif


namespace __HAL {
  SOCKET_TYPE establish(unsigned short portnum)
  {
    SOCKET_TYPE listenSocket;

    #ifdef WIN32
    // Create a SOCKET for listening for
    // incoming connection requests
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(CAN_SERVER_HOST);
    service.sin_port = htons(portnum);

    // Bind the socket.
    if (bind( listenSocket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR) {
        closesocket(listenSocket);
        return -1;
    }

    // Listen for incoming connection requests.
    // on the created socket
    if (listen( listenSocket, 1 ) == SOCKET_ERROR) {
        printf("Error listening on socket.\n");
        closesocket(listenSocket);
        WSACleanup();
        return -1;
    }

    #else

    uint32_t ui32_len;

    #ifdef USE_UNIX_SOCKET
    struct sockaddr_un sa;
    memset(&sa, 0, sizeof(struct sockaddr_un));   /* clear our address */
    sa.sun_family = SOCKET_TYPE_INET_OR_UNIX;
    sprintf(sa.sun_path, "%s.%d", SOCKET_PATH, portnum);
    unlink(sa.sun_path);
    ui32_len = strlen(sa.sun_path) + sizeof(sa.sun_family);
    #else
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(struct sockaddr_in));   /* clear our address */
    sa.sin_family = SOCKET_TYPE_INET_OR_UNIX;
    sa.sin_addr.s_addr = inet_addr(CAN_SERVER_HOST);
    sa.sin_port = htons(portnum);                  /* this is our port number */
    ui32_len = sizeof(struct sockaddr_in);
    #endif

    if ((listenSocket= socket(SOCKET_TYPE_INET_OR_UNIX, SOCK_STREAM, 0)) < 0) /* create socket */
    {
        printf("socket error");
        return(-1);
    }

    if (bind(listenSocket, (struct sockaddr *)&sa, ui32_len) < 0) {
        printf("bind error");
        printf("\nmaybe socket is in TIME_WAIT state, check this with the netstat command\n");
        printf("=> please wait one minute (or stop all clients before stopping the can_server)\n\n");
        close(listenSocket);
        return(-1);                                  /* bind address to socket */
    }

    listen(listenSocket, 10);                                  /* max # of queued connects */

    #endif

    return listenSocket;
  }
} // end namespace __HAL
#endif
