/*
  can_device_vector_xl.cpp: interface for Vector-Informatik can
    card (xl)

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#define  STRICT

#include <stdlib.h>
#include <ctype.h>

#ifdef WIN32
#include <dos.h>

#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif

/* Typedef taken from windows.h to avoid complete windows inclusion: */
typedef void *HANDLE;
extern "C" {
  #include <vxlapi.h>
}

#endif

#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"
#include <assert.h>
#include <vector>

#define XL_HWTYPE_AUTO 1000

using namespace __HAL;

// CAN Globals
int AllCanChannelCount = 0;

char            g_AppName[XL_MAX_LENGTH+1]  = "IsoAgLib";   //!< Application name which is displayed in VHWconf
XLdriverConfig  g_xlDrvConfig;                              //!< Contains the actual hardware configuration
int16_t gChannel = 0;

static struct canDevice_s {
  struct canBus_s {
    XLportHandle  m_xlPortHandle;     //!< Global porthandle (we use only one!)
    XLaccess      m_xlChannelMask;    //!< Global channelmask (includes all founded channels)
    XLaccess      m_xlPermissionMask; //!< Global permissionmask (includes all founded channels)
    XLaccess      m_xlInitMask;
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
  m_xlPortHandle(XL_INVALID_PORTHANDLE),
  m_xlChannelMask(0),
  m_xlPermissionMask(0),
  m_xlInitMask(0),
  mb_canBusIsOpen(false)
{
}

#ifdef USE_CAN_CARD_TYPE
uint16_t gHwType = USE_CAN_CARD_TYPE;
#else
// select the Vector CAN card type to use
//
uint16_t gHwType = XL_HWTYPE_AUTO;
#endif

static void printDriverConfig( void )
{
  unsigned int i;
  char         str[XL_MAX_LENGTH + 1]="";

  printf("----------------------------------------------------------\n");
  printf("- %02d channels       Hardware Configuration               -\n", g_xlDrvConfig.channelCount);
  printf("----------------------------------------------------------\n");

  for (i=0; i < g_xlDrvConfig.channelCount; i++) {

    printf("- Ch.: %02d, CM:0x%3I64x,",
      g_xlDrvConfig.channel[i].channelIndex, g_xlDrvConfig.channel[i].channelMask);
    printf(" %20s ", g_xlDrvConfig.channel[i].name);
    if (g_xlDrvConfig.channel[i].transceiverType != XL_TRANSCEIVER_TYPE_NONE) {
      strncpy( str, g_xlDrvConfig.channel[i].transceiverName, 13 );
      printf(" %3s -\n", str);
    }
    else printf(" no Cab!       -\n", str);

  }
  //0x%I64x

  printf("----------------------------------------------------------\n\n");

}


bool isBusOpen(uint8_t ui8_bus)
{
  return ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen;
}

uint32_t initCardApi ()
{
  #ifdef USE_CAN_CARD_TYPE
  gHwType = USE_CAN_CARD_TYPE;
  #else
  // select the Vector CAN card type to use
  gHwType = XL_HWTYPE_AUTO;
  #endif

  // open the driver
  XLstatus xlStatus;
  xlStatus = xlOpenDriver();
  if (xlStatus)
    goto error;

  printf("xlDriverConfig()\n");
  xlStatus = xlGetDriverConfig(&g_xlDrvConfig);
  if (xlStatus) return 0;

  printf(" %u channels found\n",g_xlDrvConfig.channelCount);
  printDriverConfig();

  return 1;

  error:
    printf("ERROR: %s!\n", xlGetErrorString(xlStatus));
    return 0;
}

bool resetCard(void)
{
  xlCloseDriver();
  return true;
}


bool openBusOnCard(uint8_t ui8_bus, uint32_t wBitrate, server_c* pc_serverData)
{
  DEBUG_PRINT1("init can bus %d\n", ui8_bus);
  XLstatus xlStatus;

  if( !ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen ) {
    DEBUG_PRINT1("Opening CAN BUS channel=%d\n", ui8_bus);

    int32_t i32_busInd = -1, i32_virtualBusInd = -1;
    XLaccess virtualChannelMask = 0;

    // select the wanted channels
    ss_canDevice.canBus(ui8_bus).m_xlChannelMask = ss_canDevice.canBus(ui8_bus).m_xlInitMask = 0;
    i32_busInd = -1;
    for (uint32_t i=0; i<g_xlDrvConfig.channelCount; i++)
    {
      if ( ( g_xlDrvConfig.channel[i].hwType==gHwType                                           )
        || ( ( gHwType == XL_HWTYPE_AUTO ) && ( g_xlDrvConfig.channel[i].hwType > XL_HWTYPE_VIRTUAL ) ) )
      {
        i32_busInd++;
        printf( "Detect Real Channel %d\n", i32_busInd );
        if ( ui8_bus == i32_busInd )
        { // BUS found
          ss_canDevice.canBus(ui8_bus).m_xlChannelMask |= g_xlDrvConfig.channel[i].channelMask;
        }
      }
      else if ( g_xlDrvConfig.channel[i].hwType == XL_HWTYPE_VIRTUAL )
      {
        i32_virtualBusInd++;
        printf( "Detect Virtual Channel %d\n", i32_virtualBusInd );
        if ( ui8_bus == i32_virtualBusInd )
        { // BUS found
          virtualChannelMask |= g_xlDrvConfig.channel[i].channelMask;
        }
      }
    }

    // if AUTO HW detection is wanted, and only virtual channels are found
    // use virtualChannelMask
    if ( ( gHwType == XL_HWTYPE_AUTO ) && ( i32_busInd == -1 ) )
    { // no real CAN channels found
      ss_canDevice.canBus(ui8_bus).m_xlChannelMask = virtualChannelMask;
    }

    ss_canDevice.canBus(ui8_bus).m_xlInitMask = ss_canDevice.canBus(ui8_bus).m_xlPermissionMask = ss_canDevice.canBus(ui8_bus).m_xlChannelMask;

    // ------------------------------------
    // open ONE port including all channels
    // ------------------------------------
    xlStatus = xlOpenPort(
        &ss_canDevice.canBus(ui8_bus).m_xlPortHandle,
        g_AppName,
        ss_canDevice.canBus(ui8_bus).m_xlChannelMask,
        &ss_canDevice.canBus(ui8_bus).m_xlPermissionMask,
        256,
        XL_INTERFACE_VERSION,
        XL_BUS_TYPE_CAN);
    printf(
        "- OpenPort         : CM=0x%I64x, PH=0x%02X, PM=0x%I64x, %s\n",
        ss_canDevice.canBus(ui8_bus).m_xlChannelMask,
        ss_canDevice.canBus(ui8_bus).m_xlPortHandle,
        ss_canDevice.canBus(ui8_bus).m_xlPermissionMask,
        xlGetErrorString(xlStatus) );

    if ( (xlStatus) || (ss_canDevice.canBus(ui8_bus).m_xlPortHandle == XL_INVALID_PORTHANDLE) )
        goto error;

    // ------------------------------------
    // if we have permission we set the
    // bus parameters (baudrate)
    // ------------------------------------
    if (ss_canDevice.canBus(ui8_bus).m_xlPermissionMask) {
      xlStatus = xlCanSetChannelBitrate(
          ss_canDevice.canBus(ui8_bus).m_xlPortHandle,
          ss_canDevice.canBus(ui8_bus).m_xlChannelMask,
          wBitrate * 1000);
      printf("- SetChannelBitrate: baudr.=%u, %s\n",wBitrate, xlGetErrorString(xlStatus));
      if (xlStatus)
        goto error;
    }
    else if (wBitrate) {
      printf("WARNING: No init access, bitrate ignored!\n");
    }

    // Disable the TX and TXRQ notifications
    xlStatus = xlCanSetChannelMode(
        ss_canDevice.canBus(ui8_bus).m_xlPortHandle,
        ss_canDevice.canBus(ui8_bus).m_xlChannelMask,
        0,
        0);
    if (xlStatus)
      goto error;

    if (ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen == false)
    { // set global mask
      xlStatus = xlCanSetChannelAcceptance(
          ss_canDevice.canBus(ui8_bus).m_xlPortHandle,
          ss_canDevice.canBus(ui8_bus).m_xlChannelMask,
          0x000,
          0x000,
          XL_CAN_STD );
      if (xlStatus)
        goto error;
      xlStatus = xlCanSetChannelAcceptance(
          ss_canDevice.canBus(ui8_bus).m_xlPortHandle,
          ss_canDevice.canBus(ui8_bus).m_xlChannelMask,
          0x000,
          0x000,
          XL_CAN_EXT );
      if (xlStatus)
        goto error;

      // Go on bus and reset clock
      xlStatus = xlActivateChannel(
          ss_canDevice.canBus(ui8_bus).m_xlPortHandle,
          ss_canDevice.canBus(ui8_bus).m_xlChannelMask,
          XL_BUS_TYPE_CAN,
          XL_ACTIVATE_RESET_CLOCK);
      if (xlStatus)
        goto error;

    }
    ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = true;
    pc_serverData->canBus(ui8_bus).mb_deviceConnected = true;
  }
  
  // either fresh init without error or the bus was already initialized
  return true; // already initialized and files are already open

  error:
      printf("ERROR: %s!\n", xlGetErrorString(xlStatus));

      if (ss_canDevice.canBus(ui8_bus).m_xlPortHandle != XL_INVALID_PORTHANDLE) {
        xlClosePort(ss_canDevice.canBus(ui8_bus).m_xlPortHandle);
        ss_canDevice.canBus(ui8_bus).m_xlPortHandle = XL_INVALID_PORTHANDLE;
      }

      ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = false;
      return false;
}

void closeBusOnCard(uint8_t ui8_bus, server_c* pc_serverData)
{
  DEBUG_PRINT1("close can bus %d\n", ui8_bus);
  //ui8_bus[ui8_bus] = false;
  // do not call close or CAN_CLOSE because COMMAND_CLOSE is received during initialization!
}


// PURPOSE: To send a msg on the specified CAN BUS
int16_t sendToBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
  XLevent       xlEvent;
  XLstatus      xlStatus;
  unsigned int  messageCount = 1;

  xlEvent.tag                 = XL_TRANSMIT_MSG;
  xlEvent.tagData.msg.id      = ps_canMsg->ui32_id;

  // set extended type if needed by setting MSB bit
  if (ps_canMsg->i32_msgType > 0)
    xlEvent.tagData.msg.id |= 0x80000000UL;

  xlEvent.tagData.msg.dlc     = ps_canMsg->i32_len;
  xlEvent.tagData.msg.flags   = 0;
  memcpy(xlEvent.tagData.msg.data, ps_canMsg->ui8_data, ps_canMsg->i32_len);

  // should have been checked already by calling function isBusOpen:
  assert((ui8_bus <= HAL_CAN_MAX_BUS_NR) && ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen);
  xlStatus = xlCanTransmit(ss_canDevice.canBus(ui8_bus).m_xlPortHandle, ss_canDevice.canBus(ui8_bus).m_xlChannelMask, &messageCount, &xlEvent);

  if (xlStatus == XL_ERR_QUEUE_IS_FULL)
    return 0;

  return 1; // success
}

bool readFromBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
  XLstatus xlStatus;
  XLevent  gpEvent;

  unsigned int    msgsrx = 1;

  // try to receive a message
  msgsrx = 1; // we want to receive always only one message
  xlStatus = xlReceive(ss_canDevice.canBus(ui8_bus).m_xlPortHandle, &msgsrx, &gpEvent);
  // msg from CANcardX buffer
  // this functions retrurns not only received messages
  // ACK for SENT messages is also returned!!!
  if( ( gpEvent.tag != XL_RECEIVE_MSG ) || ( gpEvent.tagData.msg.flags != 0 ) )
  { // don't further process this message as it is NO received message
    return false;
  }

  ps_canMsg->ui32_id = (gpEvent.tagData.msg.id & 0x1FFFFFFF);
  if (ps_canMsg->ui32_id >= 0x7FFFFFFF)
  {
    printf("!!Received of malformed message with undefined CAN ident: %x\n", ps_canMsg->ui32_id);
    return false;
  }

  ps_canMsg->i32_len = gpEvent.tagData.msg.dlc;
  ps_canMsg->i32_msgType = (gpEvent.tagData.msg.id > 0x7FFFFFFF) ? 1 : 0;

  memcpy( ps_canMsg->ui8_data, gpEvent.tagData.msg.data, ps_canMsg->i32_len );

  return true;
}

