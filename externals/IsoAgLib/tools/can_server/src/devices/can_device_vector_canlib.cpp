/*
  can_device_vector_canlib.cpp: interface for Vector-Informatik
    can card (canlib)

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

extern "C" {
  #include <VCanD.h>
}

#endif

#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"
#include <assert.h>

#define HWTYPE_AUTO 1000

using namespace __HAL;

// CAN Globals
int AllCanChannelCount = 0;

static struct canDevice_s {
  struct canBus_s {
    VportHandle m_portHandle;
    Vaccess     m_channelMask;
    Vaccess     m_permissionMask;
    Vaccess     m_initMask;
    bool        mb_canBusIsOpen;
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
  m_portHandle(INVALID_PORTHANDLE),
  m_channelMask(0),
  m_permissionMask(0),
  m_initMask(0),
  mb_canBusIsOpen(false)
{
}

Vevent  gEvent;
Vevent*  gpEvent;
VDriverConfig *gDriverConfig = 0;

#ifdef USE_CAN_CARD_TYPE
uint32_t gHwType = USE_CAN_CARD_TYPE;
#else
// select the Vector CAN card type to use
uint32_t gHwType = HWTYPE_AUTO;
#endif


static void printDriverConfig( void ) {
  int i;
  printf("Driver Configuration:\n  ChannelCount=%u\n", gDriverConfig->channelCount);
  for (i=0; i<gDriverConfig->channelCount; i++) {
    printf("  Channel %u (%04X): %s, (%u,%u,%u), isOnBus=%u\n",
      gDriverConfig->channel[i].channelIndex,
      gDriverConfig->channel[i].channelMask,
      gDriverConfig->channel[i].name,
      gDriverConfig->channel[i].hwIndex,
      gDriverConfig->channel[i].hwType,
      gDriverConfig->channel[i].hwChannel,
      gDriverConfig->channel[i].isOnBus
    );
  }
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
  gHwType = HWTYPE_AUTO;
  #endif
  // open the driver
  Vstatus vErr;
  vErr = ncdOpenDriver();
  if (vErr) goto error;

  printf("ncdDriverConfig()\n");
  vErr = ncdGetDriverConfig(&AllCanChannelCount, NULL); // get the number of channels
  if (vErr) goto error;

  printf(" %u channels found\n",AllCanChannelCount);
  gDriverConfig = (VDriverConfig*)malloc(AllCanChannelCount*sizeof(VDriverConfig));
  if (!gDriverConfig) return 0;

  vErr = ncdGetDriverConfig(&AllCanChannelCount, gDriverConfig); // get information about all channels
  if (vErr) goto error;

  printDriverConfig();

  return 1;

  error:
    printf("ERROR: %s!\n", ncdGetErrorString(vErr));
    return 0;

}

bool resetCard(void)
{
  ncdCloseDriver();
  return true;
}


bool openBusOnCard(uint8_t ui8_bus, uint32_t wBitrate, server_c* pc_serverData)
{
  DEBUG_PRINT1("init can bus %d\n", ui8_bus);

  Vstatus vErr;

  if( !ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen ) {
    DEBUG_PRINT1("Opening CAN BUS channel=%d\n", ui8_bus);

    int32_t i32_busInd = -1, i32_virtualBusInd = -1;
    Vaccess virtualChannelMask = 0;

    // select the wanted channels
    ss_canDevice.canBus(ui8_bus).m_channelMask = ss_canDevice.canBus(ui8_bus).m_initMask = 0;
    i32_busInd = -1;
    for (int32_t i=0; i<AllCanChannelCount; i++) {
      if ( ( gDriverConfig->channel[i].hwType==gHwType                                           )
        || ( ( gHwType == HWTYPE_AUTO ) && ( gDriverConfig->channel[i].hwType > HWTYPE_VIRTUAL ) ) )
      {
        i32_busInd++;
        printf( "Detect Real Channel %d\n", i32_busInd );
        if ( ui8_bus == i32_busInd )
        { // BUS found
          ss_canDevice.canBus(ui8_bus).m_channelMask |= gDriverConfig->channel[i].channelMask;
        }
      }
      else if ( gDriverConfig->channel[i].hwType == HWTYPE_VIRTUAL )
      {
        i32_virtualBusInd++;
        printf( "Detect Virtual Channel %d\n", i32_virtualBusInd );
        if ( ui8_bus == i32_virtualBusInd )
        { // BUS found
          virtualChannelMask |= gDriverConfig->channel[i].channelMask;
        }
      }
    }
 
    // if AUTO HW detection is wanted, and only virtual channels are found
    // use virtualChannelMask
    if ( ( gHwType == HWTYPE_AUTO ) && ( i32_busInd == -1 ) )
    { // no real CAN channels found
      ss_canDevice.canBus(ui8_bus).m_channelMask = virtualChannelMask;
    }

    ss_canDevice.canBus(ui8_bus).m_initMask = ss_canDevice.canBus(ui8_bus).m_channelMask;

    // open a port
    printf("ncdOpenPort(channelMask=%04X, initMask=%04X)\n", ss_canDevice.canBus(ui8_bus).m_channelMask, ss_canDevice.canBus(ui8_bus).m_initMask);
    vErr = ncdOpenPort(&ss_canDevice.canBus(ui8_bus).m_portHandle, "IsoAgLib", ss_canDevice.canBus(ui8_bus).m_channelMask, ss_canDevice.canBus(ui8_bus).m_initMask, &ss_canDevice.canBus(ui8_bus).m_permissionMask, 1024);
    if (vErr) goto error;

    printf(" portHandle=%u\n", ss_canDevice.canBus(ui8_bus).m_portHandle);
    if (ss_canDevice.canBus(ui8_bus).m_portHandle==INVALID_PORTHANDLE) goto error;

    printf(" permissionMask=%04X\n", ss_canDevice.canBus(ui8_bus).m_permissionMask);

    // if permision to init
    if (ss_canDevice.canBus(ui8_bus).m_permissionMask) {
      // set BUS timing
      vErr = ncdSetChannelBitrate(ss_canDevice.canBus(ui8_bus).m_portHandle, ss_canDevice.canBus(ui8_bus).m_permissionMask, (wBitrate * 1000));
      if (vErr) goto error;
    }
    else if (wBitrate) {
      printf("WARNING: No init access, bitrate ignored!\n");
    }

    // Disable the TX and TXRQ notifications
    vErr = ncdSetChannelMode(ss_canDevice.canBus(ui8_bus).m_portHandle,ss_canDevice.canBus(ui8_bus).m_channelMask,0,0);
    if (vErr) goto error;

    if (ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen == false)
    { // set global mask
      VsetAcceptance acc;
      acc.mask = 0x000;
      acc.code = 0x000;
      vErr = ncdSetChannelAcceptance(ss_canDevice.canBus(ui8_bus).m_portHandle, ss_canDevice.canBus(ui8_bus).m_channelMask, &acc);
      if (vErr) goto error;
      acc.mask = 0x80000000;
      acc.code = 0x80000000;
      vErr = ncdSetChannelAcceptance(ss_canDevice.canBus(ui8_bus).m_portHandle, ss_canDevice.canBus(ui8_bus).m_channelMask, &acc);
      if (vErr) goto error;

      // reset clock
      vErr = ncdResetClock(ss_canDevice.canBus(ui8_bus).m_portHandle);
      if (vErr) goto error;
      // Go on bus
      vErr = ncdActivateChannel(ss_canDevice.canBus(ui8_bus).m_portHandle,ss_canDevice.canBus(ui8_bus).m_channelMask);
      if (vErr) goto error;
    }
    ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = true;
    pc_serverData->canBus(ui8_bus).mb_deviceConnected = true;
  }
  
  // either fresh init without error or the bus was already initialized
  return true; // already initialized and files are already open

  error:
    printf("ERROR: %s!\n", ncdGetErrorString(vErr));

    if (ss_canDevice.canBus(ui8_bus).m_portHandle != INVALID_PORTHANDLE) {
      ncdClosePort(ss_canDevice.canBus(ui8_bus).m_portHandle);
      ss_canDevice.canBus(ui8_bus).m_portHandle = INVALID_PORTHANDLE;
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
  // should have been checked already by calling function isBusOpen:
  assert((ui8_bus <= HAL_CAN_MAX_BUS_NR) && ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen);
  VportHandle lPortHandle = ss_canDevice.canBus(ui8_bus).m_portHandle;
  Vaccess lChannelMask = ss_canDevice.canBus(ui8_bus).m_channelMask;
  Vaccess lPermissionMask = ss_canDevice.canBus(ui8_bus).m_permissionMask;

  Vevent  lEvent = gEvent;
  Vstatus vErr;

  lEvent.tag = V_TRANSMIT_MSG;
  lEvent.tagData.msg.id = ps_canMsg->ui32_id;

  // set extended type if needed by setting MSB bit
  if (ps_canMsg->i32_msgType > 0)
    lEvent.tagData.msg.id |= 0x80000000UL;
  lEvent.tagData.msg.flags = 0;
  lEvent.tagData.msg.dlc = ps_canMsg->i32_len;
  memcpy(lEvent.tagData.msg.data, ps_canMsg->ui8_data, ps_canMsg->i32_len);

  vErr = ncdTransmit(lPortHandle, lChannelMask, &lEvent);

  if (vErr == VERR_QUEUE_IS_FULL)
    return 0;

  return 1; // success
}

bool readFromBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
  Vstatus vErr;

  vErr = ncdReceive1(ss_canDevice.canBus(ui8_bus).m_portHandle, &gpEvent);
  // msg from CANcardX buffer
  // this functions retrurns not only received messages
  // ACK for SENT messages is also returned!!!
  if( vErr || ( gpEvent->tag != V_RECEIVE_MSG ) || ( gpEvent->tagData.msg.flags != 0 ) )
  { // don't further process this message as it is NO received message
    return false;
  }

  ps_canMsg->ui32_id = (gpEvent->tagData.msg.id & 0x1FFFFFFF);
  if (ps_canMsg->ui32_id >= 0x7FFFFFFF)
  {
    printf("!!Received of malformed message with undefined CAN ident: %x\n", ps_canMsg->ui32_id);
    return false;
  }

  ps_canMsg->i32_len = gpEvent->tagData.msg.dlc;
  ps_canMsg->i32_msgType = (gpEvent->tagData.msg.id > 0x7FFFFFFF) ? 1 : 0;

  memcpy( ps_canMsg->ui8_data, gpEvent->tagData.msg.data, ps_canMsg->i32_len );

  return true;
}
