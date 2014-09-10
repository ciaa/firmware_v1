/*
  can_device_socketcan.cpp: nterface for SocketCAN can based on
    kernel version

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/version.h>

#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"
#include <assert.h>

using namespace __HAL;

#ifndef PF_CAN
  #define PF_CAN 29
#endif

#ifndef AF_CAN
  #define AF_CAN PF_CAN
#endif

extern int errno;

/* prior to 2.5.x versions of socket theres a specific ioctrl for 
 * setting the baudrate abailable. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
typedef uint32_t can_baudrate_t;
#define SIOCSCANBAUDRATE	(SIOCDEVPRIVATE+0)
#endif

#if DEBUG_CAN
std::ofstream m_clog;
#endif


static struct canDevice_s {
  struct canBus_s {
    bool          mb_canBusIsOpen;
    canBus_s();
  };
  canBus_s &canBus(size_t n_index);
  size_t nCanBusses();

private:
  std::vector< canBus_s > mvec_canBus;
} ss_canDevice;

inline canDevice_s::canBus_s &canDevice_s::canBus(size_t n_index)
{
  if (mvec_canBus.size() <= n_index)
    mvec_canBus.resize(n_index + 1);
  return mvec_canBus[n_index];
}

inline size_t canDevice_s::nCanBusses()
{
  return mvec_canBus.size();
}

canDevice_s::canBus_s::canBus_s() :
  mb_canBusIsOpen(false)
{
}

bool isBusOpen(uint8_t ui8_bus)
{
    return ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen;
}

uint32_t initCardApi ()
{
#if DEBUG_CAN
    m_clog.open("/tmp/vcan.log", std::ios::out|std::ios::app);
#endif

    return 1;
}

bool resetCard(void)
{
    return true;
}

// PURPOSE: To initialize the specified CAN BUS to begin sending/receiving msgs
bool openBusOnCard(uint8_t ui8_bus, uint32_t wBitrate, server_c* pc_serverData)
{
    (void)wBitrate; // not used for Kernel >= 2.6.

    DEBUG_PRINT1("init can bus %d\n", ui8_bus);

    if( !ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen ) 
    {
        DEBUG_PRINT1("Opening CAN BUS channel=%d\n", ui8_bus);

        char fname[32];
        sprintf(fname, "can%u", ui8_bus);

        // Create a socket for talking to the CAN Bus
        int m_handle = socket(PF_CAN, SOCK_RAW, CAN_RAW);
#if DEBUG_CAN
        m_clog << "Init(): bus: " << (int)ui8_bus << "fname: " << fname << "m_handle: " << m_handle << std::endl;
#endif
        if (m_handle < 0)
        {
            return false;
        }

        // Get the interface
        struct ifreq ifr;
        strncpy(ifr.ifr_name, fname, IFNAMSIZ);
        int ret = ioctl(m_handle, SIOCGIFINDEX, &ifr);
#if DEBUG_CAN
        m_clog << "Init(): ret: " << ret << std::endl;
#endif
        if (ret < 0)
        {
            perror( "SIOCGIFINDEX" );
            pc_serverData->canBus(ui8_bus).mi32_can_device = 0;
            return false;
        }

// Setting of Bitrate not possible in >= 2.6. Kernel version
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
        // Set the baudrate.  Must be set before first use
        struct ifreq ifr_baudrate;   // Use different structure for setting the baud rate as the ioctls
                                    // overwrite the structure
        strcpy(ifr_baudrate.ifr_name, fname);
        can_baudrate_t *baudrate = (can_baudrate_t *)&ifr_baudrate.ifr_ifru;
        *baudrate = wBitrate * 1000; 
        int baud_ret = ioctl(m_handle, SIOCSCANBAUDRATE, &ifr_baudrate);
#if DEBUG_CAN
        m_clog << "Init(): baud_ret: " << baud_ret << std::endl;
#endif
        if (baud_ret < 0)
        {
            perror( "SIOCSCANBAUDRATE" );
            pc_serverData->canBus(ui8_bus).mi32_can_device = 0;
            return false;
        }
#endif
        // Bind to the socket
        struct sockaddr_can addr;
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        int result = bind( m_handle, (struct sockaddr*)&addr, sizeof(addr) );

#if DEBUG_CAN
        m_clog << "Init(): result: " << result << std::endl;
#endif
        if (result < 0)
        {
            perror( "bind" );
            pc_serverData->canBus(ui8_bus).mi32_can_device = 0;
            return false;
        }

        // Enable loopback (ensure, because it may not be default in the future)
        int loopback = 1; /* 0 = disabled, 1 = enabled (default) */
        if( -1 == setsockopt( m_handle, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback)) ) {
            perror( "enabling loopback" );
            return false;
        }

        int recv_own_msgs = 0; /* 0 = disabled (default), 1 = enabled */
        if( -1 == setsockopt( m_handle, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &recv_own_msgs, sizeof(recv_own_msgs)) ) {
            perror( "setting local echo option" );
            return false;
        }

        pc_serverData->canBus(ui8_bus).mi32_can_device = m_handle;
        pc_serverData->canBus(ui8_bus).mb_deviceConnected = true;

        ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen  = true;

        return true;
    }
    else
        return true; // already initialized and files are already open
}


void closeBusOnCard(uint8_t ui8_bus, server_c* pc_serverData )
{
  if (!ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen)
    return; // not yet initialized => return

  ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = false;

  if( 0 > close( pc_serverData->canBus(ui8_bus).mi32_can_device ) ) {
      perror( "close" );
  }

  // signal that no device handle is available and device is closed
  pc_serverData->canBus(ui8_bus).mi32_can_device = 0;
  pc_serverData->canBus(ui8_bus).mb_deviceConnected = false;

}


// PURPOSE: To send a msg on the specified CAN BUS
// RETURNS: non-zero if msg was sent ok
//          0 on error
int16_t sendToBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
#if DEBUG_CAN
  m_clog << "sendToBus(): bus: " << (int)ui8_bus << " isOpen: " << ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen << std::endl;
#endif
  assert((ui8_bus <= HAL_CAN_MAX_BUS_NR) && ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen);
  // Create the frame
  struct can_frame frame;

  frame.can_id = ps_canMsg->ui32_id;
  if (ps_canMsg->i32_msgType > 0)
    frame.can_id |= CAN_EFF_FLAG;   // set extended frame
  frame.can_dlc = ps_canMsg->i32_len;

  for( int i=0; i<frame.can_dlc; i++ )
    frame.data[i] = ps_canMsg->ui8_data[i];

  // ...and send the packet
  int bytes_sent = send(pc_serverData->canBus(ui8_bus).mi32_can_device, &frame, sizeof(frame), 0 );
  if (bytes_sent < 0)
  {
    perror( "send" );
#if DEBUG_CAN
    m_clog << "sendToBus(): error, bytes_sent: " << bytes_sent << std::endl;
#endif
    return 0;
  }

#if DEBUG_CAN
  char buf[200];
  m_clog << "sendToBus(): bytes_sent: " << bytes_sent << std::endl;
  sprintf(buf,"sendToBus(): %X,%X,%d,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X",
          frame.can_id, ps_canMsg->i32_msgType, frame.can_dlc, frame.data[0], frame.data[1], frame.data[2], frame.data[3], frame.data[4], frame.data[5], frame.data[6], frame.data[7]);
  m_clog << buf << std::endl;
#endif
  return 1;
}


bool readFromBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
#if DEBUG_CAN
        m_clog << "readFromBus(): bus: " << (int)ui8_bus << " driverHandle: " << pc_serverData->canBus(ui8_bus).mi32_can_device << std::endl;
#endif
    if ((ui8_bus <= HAL_CAN_MAX_BUS_NR) && ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen)
    {
        struct can_frame frame;
        fd_set rfds;       // read file descriptor
        struct timeval tv;
        int retval;

        FD_ZERO(&rfds);
        FD_SET(pc_serverData->canBus(ui8_bus).mi32_can_device, &rfds);

        tv.tv_sec  = 0;
        tv.tv_usec = 0;

        retval = select(pc_serverData->canBus(ui8_bus).mi32_can_device+1, &rfds, NULL, NULL, &tv);
#if DEBUG_CAN
        m_clog << "readFromBus(timeout): retval: " << retval << std::endl;
#endif
        if (retval <= 0)     // -1 on error, 0 on no data within timeout
        {
            if ( -1 == retval ) {
                perror( "select" );
            }
            return false;
        }

        int status = recv(pc_serverData->canBus(ui8_bus).mi32_can_device, &frame, sizeof(frame), 0);
#if DEBUG_CAN
        m_clog << "readFromBus(): status: " << status << std::endl;
#endif
        if (status <= 0) {
            if ( -1 == status ) {
                perror( "read" );
            }
            return false;
        }

        ps_canMsg->ui32_id = frame.can_id & CAN_ERR_MASK;
        for(int i = 0; i < 8; ++i)
            ps_canMsg->ui8_data[i] = frame.data[i];
        ps_canMsg->i32_len = frame.can_dlc;
        ps_canMsg->i32_msgType = 0;
        if ((frame.can_id & CAN_EFF_FLAG) == CAN_EFF_FLAG)
            ps_canMsg->i32_msgType = 1;

#if DEBUG_CAN
        char buf[200];
        m_clog << "readFromBus(): bytes recv: " << status << std::endl;
        sprintf(buf,"readFromBus(): %X,%X,%d,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X",
                ps_canMsg->ui32_id, ps_canMsg->i32_msgType, ps_canMsg->i32_len, frame.data[0], frame.data[1], frame.data[2], frame.data[3], frame.data[4], frame.data[5], frame.data[6], frame.data[7]);
        m_clog << buf << std::endl;
#endif

        return true;
    }
    return false;
}

