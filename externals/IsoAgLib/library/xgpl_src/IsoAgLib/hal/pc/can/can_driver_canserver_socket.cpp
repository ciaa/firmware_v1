/*
  target_extension_can_client_sock.cpp:
    CAN-Server client based on Sockets

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <cstring>
#include <cctype>

#include "can_server_interface.h"

#include <IsoAgLib/driver/can/impl/canpkg_c.h>
#include <IsoAgLib/hal/generic_utils/can/canfifo_c.h>
#include <IsoAgLib/hal/hal_can.h>
#include <IsoAgLib/hal/pc/system/system.h>
#include <IsoAgLib/isoaglib_config.h>


#ifdef WIN32
#define ISOAGLIB_CLOSESOCKET closesocket
#include <stdio.h>
#else
#define ISOAGLIB_CLOSESOCKET close
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/times.h>
#include <linux/version.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#ifndef WIN32
#include <fcntl.h>
#include <signal.h>
#endif
#include <list>
#include <set>

#ifndef ENTRY_POINT_FOR_INSERT_RECEIVE_CAN_MSG
#define ENTRY_POINT_FOR_INSERT_RECEIVE_CAN_MSG
#endif

#ifndef ENTRY_POINT_FOR_SEND_CAN_MSG
#define ENTRY_POINT_FOR_SEND_CAN_MSG
#endif

#ifndef ENTRY_POINT_FOR_RECEIVE_CAN_MSG
#define ENTRY_POINT_FOR_RECEIVE_CAN_MSG
#endif


#define TRACE(); printf("%s (%d)\n", __FUNCTION__, __LINE__ );

namespace __IsoAgLib {
  class CanPkg_c;
}

namespace __HAL {

  static SOCKET_TYPE i32_commandSocket = INVALID_SOCKET;
  static SOCKET_TYPE i32_dataSocket = INVALID_SOCKET;

#ifdef USE_MUTUAL_EXCLUSION
  static SOCKET_TYPE breakWaitSocket_read = INVALID_SOCKET;
  static SOCKET_TYPE breakWaitSocket_write = INVALID_SOCKET;
#endif

  struct idFilter_s {
    bool operator<(const struct idFilter_s& r ) const {
      return idx < r.idx;
    }
    unsigned idx;
    struct {
      bool xtd;
      uint32_t mask;
      uint32_t filter;
    } filter;
  };
  static std::set<idFilter_s> filterIdx[HAL_CAN_MAX_BUS_NR];


  int readData( SOCKET_TYPE s, char *buf, int n ) {

    int bcount = 0; /* counts bytes read */
    int br = 0;     /* bytes read this pass */

    while ( bcount < n ) {           /* loop until full buffer */
      if ( ( br= recv( s,buf,n-bcount,0 ) ) > 0 ) {
        bcount += br;                /* increment byte counter */
        buf += br;                   /* move buffer ptr for next read */
      } else if ( br <= 0 )           /* signal an error or end of communication (0) to the caller */
        return br;
    }
    return bcount;
  }


  bool sendCommand( transferBuf_s* p_writeBuf, SOCKET_TYPE ri32_commandSocket ) {

    if ( send( ri32_commandSocket, ( char* )p_writeBuf, sizeof( transferBuf_s ),
#ifdef WIN32
               0
#else
               MSG_DONTWAIT
#endif
             ) == SOCKET_ERROR ) {
      MACRO_ISOAGLIB_PERROR( "send" );
      return false;
    }

    // wait for ACK
    if ( readData( ri32_commandSocket, ( char* )p_writeBuf, sizeof( transferBuf_s ) ) == -1 ) {
      MACRO_ISOAGLIB_PERROR( "read_data" );
      return false;
    }


    if ( p_writeBuf->ui16_command == COMMAND_ACKNOWLEDGE ) {
      if ( ( p_writeBuf->s_acknowledge.i32_dataContent == ACKNOWLEDGE_DATA_CONTENT_ERROR_VALUE )
           && ( p_writeBuf->s_acknowledge.i32_data != 0 ) ) {
        // error in p_writeBuf->s_acknowledge.i32_data;
        return false;
      } else {
        return true;
      }
    }
    return false;
  }


  SOCKET_TYPE call_socket( unsigned short portnum ) {
    SOCKET_TYPE connectSocket = INVALID_SOCKET;
    bool printedRetryMsg = false;

#ifdef WIN32
    // Create a SOCKET for listening for
    // incoming connection requests
    connectSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( connectSocket == INVALID_SOCKET ) {
      WSACleanup();
      return INVALID_SOCKET;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr( CAN_SERVER_HOST );
    service.sin_port = htons( portnum );

    //----------------------
    // Connect to server.
    while ( connect( connectSocket, ( SOCKADDR* ) &service, sizeof( service ) ) == SOCKET_ERROR ) { /* connect */
      // couldn't connect for some reason.
      // Assume CAN-Server not yet started!
      if ( !printedRetryMsg ) {
        // notify user that can_server needs to be started! (only once)
        fprintf ( stderr,"ISOAgLib CAN-Init: Can't connect to CAN-Server (socket-variant).\n"
                  "ISOAgLib CAN-Init: Waiting for CAN-Server to be started - Retrying every second...\n" );
        printedRetryMsg = true;
      }
      // wait until can_server may be ready...
      Sleep ( 1000 ); // 1 second
    }

#else

    uint32_t ui32_len;
#ifdef USE_UNIX_SOCKET
    struct sockaddr_un sa;
    memset( &sa, 0, sizeof( struct sockaddr_un ) ); /* clear our address */
    sa.sun_family = SOCKET_TYPE_INET_OR_UNIX;
    sprintf( sa.sun_path, "%s.%d", SOCKET_PATH, portnum );
    ui32_len = strlen( sa.sun_path ) + sizeof( sa.sun_family );
#else
    struct sockaddr_in sa;
    memset( &sa, 0, sizeof( struct sockaddr_in ) ); /* clear our address */
    sa.sin_addr.s_addr = inet_addr( CAN_SERVER_HOST );
    sa.sin_family = AF_INET;
    sa.sin_port= htons( portnum );
    ui32_len = sizeof( struct sockaddr_in );
#endif

    if ( ( connectSocket = socket( SOCKET_TYPE_INET_OR_UNIX, SOCK_STREAM, 0 ) ) < 0 ) /* get socket */
      return INVALID_SOCKET;

    while ( connect( connectSocket, ( struct sockaddr * )&sa, ui32_len ) < 0 ) { /* connect */
      // couldn't connect for some reason. why?
      if ( errno == ECONNREFUSED ) {
        // CAN-Server not yet started!
        if ( !printedRetryMsg ) {
          // notify user that can_server needs to be started! (only once)
          fprintf ( stderr,"ISOAgLib CAN-Init: Can't connect to CAN-Server (socket-variant).\n"
                    "ISOAgLib CAN-Init: Waiting for CAN-Server to be started - Retrying every second...\n" );
          printedRetryMsg = true;
        }
        // wait until can_server may be ready...
        sleep ( 1 ); // 1 second
      } else {
        // any other error.
        fprintf ( stderr, "ISOAgLib CAN-Init: Can't connect to CAN-Server (socket-variant).\n" );
        fprintf ( stderr, "ISOAgLib CAN-Init: connect: %s\n", strerror( errno ) );

        ( void )close( connectSocket );
        return INVALID_SOCKET;
      }
    }
#endif

    if ( printedRetryMsg ) {
      // if retry was printed, print success, too.
      printf ( "ISOAgLib CAN-Init: Finally connected to CAN-Server. Continuing with application...\n\n" );
    }

    return connectSocket;
  }

  void closeAllSockets()
  {
      if(INVALID_SOCKET != i32_commandSocket)     (void)ISOAGLIB_CLOSESOCKET ( i32_commandSocket );
      i32_commandSocket = INVALID_SOCKET;

      if(INVALID_SOCKET != i32_dataSocket)        (void)ISOAGLIB_CLOSESOCKET ( i32_dataSocket );
      i32_dataSocket = INVALID_SOCKET;

#ifdef USE_MUTUAL_EXCLUSION
      if(INVALID_SOCKET != breakWaitSocket_read)  (void)ISOAGLIB_CLOSESOCKET ( breakWaitSocket_read );
      breakWaitSocket_read = INVALID_SOCKET;

      if(INVALID_SOCKET != breakWaitSocket_write) (void)ISOAGLIB_CLOSESOCKET ( breakWaitSocket_write ) ;
      breakWaitSocket_write = INVALID_SOCKET;
#endif
  }

#ifdef USE_MUTUAL_EXCLUSION
  SOCKET_TYPE establish(unsigned short portnum);
#endif

  bool canStartDriver() {
#ifdef WIN32
    // Initialize Winsock
    WSADATA wsaData;
    (void)WSAStartup( MAKEWORD( 2,2 ), &wsaData );
#endif

    i32_commandSocket = call_socket( COMMAND_TRANSFER_PORT );
    if ( i32_commandSocket == INVALID_SOCKET ) {
      return false;
    }

    i32_dataSocket = call_socket( DATA_TRANSFER_PORT );
    if ( i32_dataSocket == INVALID_SOCKET ) {
      closeAllSockets();
      return false;
    }
    
#ifdef USE_MUTUAL_EXCLUSION
    SOCKET_TYPE server_socket = INVALID_SOCKET;
    int try_break_wait_port = BREAK_WAIT_PORT;
    while (( server_socket = establish(try_break_wait_port)) == INVALID_SOCKET) {
      try_break_wait_port--;
      if(try_break_wait_port <= 6000) {
        MACRO_ISOAGLIB_PERROR("establish");
        closeAllSockets();        
        return false;
      }
    }
    
    // call_socket will not block (client-role)
    breakWaitSocket_read = call_socket(try_break_wait_port);
    if ( breakWaitSocket_read == INVALID_SOCKET ) {
      closeAllSockets();
      return false;
    }

    // accept would not block because the connection is requested above via call_socket already! (server-role)
    breakWaitSocket_write = accept(server_socket,NULL,NULL);
    if ( breakWaitSocket_write == INVALID_SOCKET ) {
      MACRO_ISOAGLIB_PERROR("accept");
      closeAllSockets();
      return false;
    }    
#endif

#ifdef WIN32
    // Set the socket I/O mode: In this case FIONBIO
    // enables or disables the blocking mode for the
    // socket based on the numerical value of iMode.
    // If iMode = 0, blocking is enabled;
    // If iMode != 0, non-blocking mode is enabled.
    int iMode = 1;

    // set data socket to nonblocking, because we need MSG_PEEK, but there is no MSG_DONTWAIT in WIN32
    ioctlsocket( i32_dataSocket, FIONBIO, ( unsigned long FAR* ) &iMode );
#ifdef USE_MUTUAL_EXCLUSION
    ioctlsocket( breakWaitSocket_read, FIONBIO, ( unsigned long FAR* ) &iMode );
#endif
#endif

    transferBuf_s s_transferBuf;
    s_transferBuf.ui16_command = COMMAND_REGISTER;
    s_transferBuf.s_startTimeClock.t_clock = getStartupTime();

    return sendCommand( &s_transferBuf, i32_commandSocket );
  }


  bool canStopDriver() {
    transferBuf_s s_transferBuf;
    s_transferBuf.ui16_command = COMMAND_DEREGISTER;
    // can_server_sock does not acknowledge COMMAND_DEREGISTER, socket read returns 0 => sendCommand() returns false
    (void)sendCommand( &s_transferBuf, i32_commandSocket );

    closeAllSockets();

    return true;
  }


}



namespace HAL {


  bool canInit( unsigned channel, unsigned baudrate ) {

    bool r = true;

    __HAL::transferBuf_s s_transferBuf[4];

    s_transferBuf[0].ui16_command = COMMAND_INIT;
    s_transferBuf[0].s_init.ui8_bus = uint8_t(channel);
    s_transferBuf[0].s_init.ui16_wGlobMask = 0x0;
    s_transferBuf[0].s_init.ui32_dwGlobMask = 0x0;
    s_transferBuf[0].s_init.ui32_dwGlobMaskLastmsg = 0x0;
    s_transferBuf[0].s_init.ui16_wBitrate = uint16_t(baudrate);
    r = __HAL::sendCommand( &s_transferBuf[0], __HAL::i32_commandSocket );
    isoaglib_assert( r ); (void)r;

    s_transferBuf[1].ui16_command = COMMAND_CONFIG;
    s_transferBuf[1].s_config.ui8_bus = uint8_t(channel);
    s_transferBuf[1].s_config.ui8_obj = 0;
    s_transferBuf[1].s_config.ui8_bXtd = 0;
    s_transferBuf[1].s_config.ui32_mask = 0;
    s_transferBuf[1].s_config.ui8_bMsgType = TX;
    s_transferBuf[1].s_config.ui32_dwId = 0x0;
    s_transferBuf[1].s_config.ui16_wNumberMsgs = 20;
    r &= sendCommand( &s_transferBuf[1], __HAL::i32_commandSocket );
    isoaglib_assert( r ); (void)r;
    return r;
  }


  bool canClose( unsigned channel ) {
    __HAL::transferBuf_s s_transferBuf;
    s_transferBuf.ui16_command = COMMAND_CLOSE;
    s_transferBuf.s_init.ui8_bus = uint8_t(channel);

    return sendCommand( &s_transferBuf, __HAL::i32_commandSocket );
  }


  bool canState( unsigned, canState_t& state ) {
    state = e_canNoError;
    return true;
  }


  bool canTxSend( unsigned channel, const __IsoAgLib::CanPkg_c& msg ) {
    __HAL::transferBuf_s s_transferBuf;

    memset( &s_transferBuf, 0, sizeof( __HAL::transferBuf_s ) );

    s_transferBuf.ui16_command = COMMAND_DATA;
    s_transferBuf.s_data.ui8_bus = uint8_t(channel);
    s_transferBuf.s_data.ui8_obj = 0;
    s_transferBuf.s_data.s_canMsg.ui32_id = msg.ident();
    s_transferBuf.s_data.s_canMsg.i32_msgType = ( msg.identType() == __IsoAgLib::Ident_c::ExtendedIdent ) ? 1 : 0;// ==0 and !=0 will be checked from can-server, but legacy ISOAgLib implementations expect 0 and 1 only.
    s_transferBuf.s_data.s_canMsg.i32_len = msg.getLen();
    memcpy( s_transferBuf.s_data.s_canMsg.ui8_data, msg.getUint8DataConstPointer(), msg.getLen() );

    s_transferBuf.s_data.i32_sendTimeStamp = getTime();

    ENTRY_POINT_FOR_SEND_CAN_MSG

    if ( send( __HAL::i32_dataSocket, ( char* )&s_transferBuf, sizeof( __HAL::transferBuf_s ),
#ifdef WIN32
               0
#else
               MSG_DONTWAIT
#endif
             ) == SOCKET_ERROR ) {
      MACRO_ISOAGLIB_PERROR("send");
      return false;
    }

    return true;
  }


  void canRxPoll( unsigned channel ) {

    ENTRY_POINT_FOR_INSERT_RECEIVE_CAN_MSG
    ( void )channel; // in case not used in the Macro above!

    __HAL::transferBuf_s s_transferBuf;

    for( ;; ) {
      memset( &s_transferBuf, 0, sizeof( __HAL::transferBuf_s ) );

#ifdef WIN32
#ifdef WINCE
      if( true ) {
#else
      if( recv( __HAL::i32_dataSocket, ( char* )&s_transferBuf, sizeof( __HAL::transferBuf_s ), MSG_PEEK ) > 0 ) {
#endif
#else
      if( recv( __HAL::i32_dataSocket, ( char* )&s_transferBuf, sizeof( __HAL::transferBuf_s ), MSG_DONTWAIT| MSG_PEEK ) > 0 ) {
#endif

        __HAL::readData( __HAL::i32_dataSocket, ( char* )&s_transferBuf, sizeof( __HAL::transferBuf_s ) );

        const int32_t now = getTime();
        const __IsoAgLib::Ident_c::identType_t type =
          (s_transferBuf.s_data.s_canMsg.i32_msgType != 0)
            ? __IsoAgLib::Ident_c::ExtendedIdent
            : __IsoAgLib::Ident_c::StandardIdent;

        static __IsoAgLib::CanPkg_c msg;
        msg.setIdent( s_transferBuf.s_data.s_canMsg.ui32_id, type );
        msg.setLen( uint8_t(s_transferBuf.s_data.s_canMsg.i32_len) );
        msg.setTime( ( now > s_transferBuf.s_data.i32_sendTimeStamp ) ? s_transferBuf.s_data.i32_sendTimeStamp : now );
        memcpy( msg.getUint8DataPointer(), s_transferBuf.s_data.s_canMsg.ui8_data, s_transferBuf.s_data.s_canMsg.i32_len );

        ENTRY_POINT_FOR_RECEIVE_CAN_MSG

        HAL::CanFifos_c::get( s_transferBuf.s_data.ui8_bus).push( msg );
      } else {
        return;
      }
    }
  }


  bool canRxWait( unsigned timeout_ms ) {
    fd_set rfds;
    struct timeval s_timeout;

    FD_ZERO( &rfds );
    FD_SET( __HAL::i32_dataSocket, &rfds );
#ifdef USE_MUTUAL_EXCLUSION
    FD_SET( __HAL::breakWaitSocket_read, &rfds );
#endif

    s_timeout.tv_sec = 0;
    s_timeout.tv_usec = timeout_ms * 1000;

    int rc = select( FD_SETSIZE, &rfds, NULL, NULL, &s_timeout );

#ifdef USE_MUTUAL_EXCLUSION
    if( FD_ISSET( __HAL::breakWaitSocket_read, &rfds ) ) {
      static char buf[256];
#ifdef WIN32
#ifdef WINCE
      // @todo: nothing to do here? --> probably also read all bytes (non-blocking)!
#else
      while( recv( __HAL::breakWaitSocket_read, ( char* )&buf, sizeof(buf), 0) > 0 ) { }
#endif
#else
      while( recv( __HAL::breakWaitSocket_read, ( char* )&buf, sizeof(buf), MSG_DONTWAIT ) > 0 ) { }
#endif
    }
#endif

    // return true, when the timeout was NOT the trigger for coming back from select
    return ( rc > 0 );
  }


#ifdef USE_MUTUAL_EXCLUSION
  void canRxWaitBreak()
  {
     static char buf = { 0 };
     if ( send( __HAL::breakWaitSocket_write, &buf, 1, 0) == SOCKET_ERROR ) {
        MACRO_ISOAGLIB_PERROR("send");
     }
  }
#endif


  int canTxQueueFree( unsigned ) {
    return -1;
  }


  void defineRxFilter( unsigned channel, bool xtd, uint32_t filter, uint32_t mask ) {

    unsigned idx = 1;
    for( std::set<__HAL::idFilter_s>::const_iterator i = __HAL::filterIdx[channel].begin(); i != __HAL::filterIdx[channel].end(); ++i ) {
      if( ( i->idx - idx ) > 0 )
        break;
      idx++;
    }

    __HAL::transferBuf_s b;
    b.ui16_command = COMMAND_CONFIG;
    b.s_config.ui8_bus = channel;
    b.s_config.ui8_obj = idx;
    b.s_config.ui8_bXtd = xtd ? 1 : 0;
    b.s_config.ui32_mask = mask;
    b.s_config.ui8_bMsgType = RX;
    b.s_config.ui32_dwId = filter;
    b.s_config.ui16_wNumberMsgs = 0x20;
    const bool r = sendCommand( &b, __HAL::i32_commandSocket );

    isoaglib_assert( r ); (void)r;
    struct __HAL::idFilter_s f;
    f.idx = idx;
    f.filter.xtd = xtd;
    f.filter.mask = mask;
    f.filter.filter = filter;
    __HAL::filterIdx[channel].insert( f );
  }

  void deleteRxFilter( unsigned channel, bool xtd, uint32_t filter, uint32_t mask ) {
    for( std::set<__HAL::idFilter_s>::const_iterator i = __HAL::filterIdx[channel].begin(); i != __HAL::filterIdx[channel].end(); ++i ) {

      if( ( i->filter.xtd == xtd ) &&
          ( i->filter.mask == mask ) &&
          ( i->filter.filter == filter ) ) {
        __HAL::transferBuf_s b;
        b.ui16_command = COMMAND_CLOSEOBJ;
        b.s_config.ui8_bus = channel;
        b.s_config.ui8_obj = i->idx;
        const bool r = sendCommand( &b, __HAL::i32_commandSocket  );
        isoaglib_assert( r ); (void)r;
        __HAL::filterIdx[channel].erase( i );
        break;
      }
    }
  }
}

// eof
