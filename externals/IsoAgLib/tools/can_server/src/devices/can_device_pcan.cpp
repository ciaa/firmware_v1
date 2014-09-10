/*
  can_device_pcan.cpp: interface for PEAK can card

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"
#include <string.h>
#include <iostream>
#include <assert.h>

#ifdef WIN32
  #include <windows.h>
  #include <PCANBasic.h>
#else
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <sys/ioctl.h>
  #include <pcan.h>

  // device nodes minor base. Must be the same as defined in the driver
  #ifndef PCAN_MSCAN_MINOR_BASE
  #define PCAN_MSCAN_MINOR_BASE     32
  #endif
#endif


using namespace __HAL;

static struct canDevice_s {
  struct canBus_s {
    bool          mb_canBusIsOpen;
    TPCANHandle   m_channel;
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
  mb_canBusIsOpen(false),
  m_channel(PCAN_NONEBUS)
{
}

bool isBusOpen(uint8_t ui8_bus)
{
  return ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen;
}

uint32_t initCardApi ()
{
  return 1;
}

bool resetCard(void)
{
  return true;
}

#if WIN32
TPCANBaudrate extractPcanBaudrate(uint32_t wBitrate)
{
  if(wBitrate == 250)
    return PCAN_BAUD_250K;
  if(wBitrate == 125)
    return PCAN_BAUD_125K;

  //default we set the baudrate to 250
  return PCAN_BAUD_250K;
}

TPCANHandle extractChannelForUSB(uint8_t ui8_bus)
{
  switch(ui8_bus)
  {
    case 0:
      return PCAN_USBBUS1;
    case 1:
      return PCAN_USBBUS2;
    case 2:
      return PCAN_USBBUS3;
    case 3:
      return PCAN_USBBUS4;
    case 4:
      return PCAN_USBBUS5;
    case 5:
      return PCAN_USBBUS6;
    case 6:
      return PCAN_USBBUS7;
    case 7:
      return PCAN_USBBUS8;
    default:
      return PCAN_USBBUS1;
  }
}

TPCANHandle extractChannelForPCI(uint8_t ui8_bus)
{
  switch(ui8_bus)
  {
    case 0:
      return PCAN_PCIBUS1;
    case 1:
      return PCAN_PCIBUS2;
    case 2:
      return PCAN_PCIBUS3;
    case 3:
      return PCAN_PCIBUS4;
    case 4:
      return PCAN_PCIBUS5;
    case 5:
      return PCAN_PCIBUS6;
    case 6:
      return PCAN_PCIBUS7;
    case 7:
      return PCAN_PCIBUS8;
    default:
      return PCAN_PCIBUS1;
  }
}
#endif

// PURPOSE: To initialize the specified CAN BUS to begin sending/receiving msgs
bool openBusOnCard(uint8_t ui8_bus, uint32_t wBitrate, server_c* pc_serverData)
{
  DEBUG_PRINT1("init can bus %d\n", ui8_bus);

  if( !ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen ) {
    DEBUG_PRINT1("Opening CAN BUS channel=%d\n", ui8_bus);

#if WIN32
  TPCANStatus status;
  TPCANHandle channel = extractChannelForUSB(ui8_bus);
  TPCANBaudrate baudrate = extractPcanBaudrate(wBitrate);
  
  status = CAN_Initialize(channel, baudrate, 0, 0, 0);
  if(status == PCAN_ERROR_OK)
  {
    DEBUG_PRINT1("Connected to USB dongle. %d\n", channel);
    ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = true;
    ss_canDevice.canBus(ui8_bus).m_channel = channel;
    pc_serverData->canBus(ui8_bus).mb_deviceConnected = true;
    return true;
  }
  else
  {
    //Try PEAK PCI CARD
    channel = extractChannelForPCI(ui8_bus);
    status = CAN_Initialize(channel, baudrate, 0, 0, 0);
    if(status == PCAN_ERROR_OK)
    {
      DEBUG_PRINT1("Connected to PCI card. %d\n", channel);
      ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = true;
      ss_canDevice.canBus(ui8_bus).m_channel = channel;
      pc_serverData->canBus(ui8_bus).mb_deviceConnected = true;
      return true;
    }
    else
    {
      std::cerr << "Open PEAK CAN Fault with return-code: " << status << std::endl;
      return false;
    }
  }
#else

    char fname[32];
    sprintf( fname, "/dev/pcan%u", PCAN_MSCAN_MINOR_BASE + ui8_bus );

    pc_serverData->canBus(ui8_bus).mi32_can_device = open(fname, O_RDWR | O_NONBLOCK);
    if (pc_serverData->canBus(ui8_bus).mi32_can_device == -1) {
      DEBUG_PRINT1("Could not open CAN bus %d\n",ui8_bus);
      return false;
    }

    TPBTR0BTR1 ratix;
    TPCANInit init;

    // init wBitrate
    DEBUG_PRINT1("Init Bitrate with PCAN_BTR0BTR1 wBitrate =%d\n",wBitrate*1000);
    ratix.dwBitRate = wBitrate * 1000;
    ratix.wBTR0BTR1 = 0;
    if ((ioctl(pc_serverData->canBus(ui8_bus).mi32_can_device, PCAN_BTR0BTR1, &ratix)) < 0)
      return false;

    // init CanMsgType (if extended Can Msg of not)
    DEBUG_PRINT1("Init CAN Driver with PCAN_INIT wBitrate =%x\n",ratix.wBTR0BTR1);
    //default value = extended
    init.wBTR0BTR1    = ratix.wBTR0BTR1;
    init.ucCANMsgType = MSGTYPE_EXTENDED;  // 11 or 29 bits
    init.ucListenOnly = 0;            // listen only mode when != 0
    if ((ioctl(pc_serverData->canBus(ui8_bus).mi32_can_device, PCAN_INIT, &init)) < 0)
      return false;

    ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = true;
    pc_serverData->canBus(ui8_bus).mb_deviceConnected = true;

    return true;
#endif
  }
  else
    return true; // already initialized and files are already open

}

void closeBusOnCard(uint8_t ui8_bus, server_c* pc_serverData)
{
  (void)ui8_bus;
  DEBUG_PRINT1("close can bus %d\n", ui8_bus);
#ifdef WIN32
  TPCANStatus status = CAN_Uninitialize(ss_canDevice.canBus(ui8_bus).m_channel);
  if(status == PCAN_ERROR_OK)
  {
    DEBUG_PRINT("Disonnected peak driver.\n");
  }
  else
  {
    std::cerr << "Close PEAK CAN Fault with return-code: " << status << std::endl;
  }
  ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = false;
  pc_serverData->canBus(ui8_bus).mb_deviceConnected = false;
#endif
}

// PURPOSE: To send a msg on the specified CAN BUS
// RETURNS: non-zero if msg was sent ok
//          0 on error
int16_t sendToBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
#if WIN32
    TPCANStatus status;
    TPCANMsg msg;

    msg.ID = ps_canMsg->ui32_id;
    msg.MSGTYPE = (ps_canMsg->i32_msgType ? PCAN_MESSAGE_EXTENDED : PCAN_MESSAGE_STANDARD );
    msg.LEN = ps_canMsg->i32_len;

    for( int i=0; i<msg.LEN; i++ )
      msg.DATA[i] = ps_canMsg->ui8_data[i];

    assert((ui8_bus <= HAL_CAN_MAX_BUS_NR) && ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen);
    status = CAN_Write(ss_canDevice.canBus(ui8_bus).m_channel, &msg);
    if (status == PCAN_ERROR_OK)
      return 1;
    else
      return 0;
#else

  TPCANMsg msg;
  msg.ID = ps_canMsg->ui32_id;
  msg.MSGTYPE = ( ps_canMsg->i32_msgType ? MSGTYPE_EXTENDED : MSGTYPE_STANDARD );
  msg.LEN = ps_canMsg->i32_len;

  for( int i=0; i<msg.LEN; i++ )
    msg.DATA[i] = ps_canMsg->ui8_data[i];

  int ret = 0;

  // should have been checked already by calling function isBusOpen:
  assert((ui8_bus <= HAL_CAN_MAX_BUS_NR) && ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen);
  ret = ioctl(pc_serverData->canBus(ui8_bus).mi32_can_device, PCAN_WRITE_MSG, &msg);
  if (ret < 0)
  {
    perror("sendToBus ioctl");
    return 0;
  }

  return 1;
#endif
}

bool readFromBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
#if WIN32
  TPCANMsg msg;
  TPCANStatus status;
  TPCANTimestamp timestamp;

  status = CAN_Read(ss_canDevice.canBus(ui8_bus).m_channel, &msg, &timestamp);
  if (status != PCAN_ERROR_OK) 
  {
    return false;
  }

#else
  TPCANRdMsg msgRd;
  int ret = ioctl(pc_serverData->canBus(ui8_bus).mi32_can_device, PCAN_READ_MSG, &msgRd);
  if (ret < 0)
    return false;

  TPCANMsg& msg = msgRd.Msg;
#endif

  switch (msg.MSGTYPE)
  {
    case PCAN_MESSAGE_STANDARD:
    case PCAN_MESSAGE_EXTENDED:
      break; // process
    default: // don't process status, RTR or other messages
      return false;
  }

  ps_canMsg->ui32_id = msg.ID;
  ps_canMsg->i32_msgType = (msg.MSGTYPE ? 1 : 0);
  ps_canMsg->i32_len = msg.LEN;

  (void) memcpy( ps_canMsg->ui8_data, msg.DATA, msg.LEN );

  return true;
}

int32_t getServerTimeFromClientTime( client_c& r_receiveClient, int32_t ai32_clientTime )
{
  return ai32_clientTime + r_receiveClient.i32_msecStartDeltaClientMinusServer;
}
