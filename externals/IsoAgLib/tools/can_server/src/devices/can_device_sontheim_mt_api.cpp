/*
  can_device_sontheim_mt_api.cpp: interface for SONTHEIM can card
    (multi thread API)

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#if !defined(WIN32)
typedef int HINSTANCE;
#endif

#define  STRICT
#include <stdlib.h>
#include <ctype.h>

#include <stdio.h>
#include <conio.h>
#include <time.h>

#include <SIECA132.h>


#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"
#include <assert.h>

using namespace __HAL;

/////////////////////////////////////////////////////////////////////////
// Globals

// CAN Globals
static struct canDevice_s {
  struct canBus_s {
    HANDLE  mh_tEventR;
    HANDLE  mh_tEventE;
    HANDLE  m_handle;
    bool    mb_isHandleAvailable;
    bool    mb_canBusIsOpen;
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
  mh_tEventR(CreateEvent(NULL,TRUE,FALSE,"R1")),
  mh_tEventE(CreateEvent(NULL,TRUE,FALSE,"E1")),
  m_handle(0),
  mb_isHandleAvailable(false),
  mb_canBusIsOpen(false)
{
}

bool isBusOpen(uint8_t ui8_bus)
{
  return ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen;
}

uint32_t initCardApi ()
{
  long l_netnumber_max = 26;                  // highest net number
  long l_txtimeout = 50;                      // msec
  long l_rxtimeout = 1;                       // msec
  long l_retval;

  // create events
 
  printf ( "HW detection ...\n");
  
  uint32_t busnr=0;
  long l_netnumber = 0;
  for (; l_netnumber < l_netnumber_max; l_netnumber++)
  {
    // open new handle
    l_retval = canOpen (l_netnumber,
                        0,
                        0,
                        l_txtimeout,
                        l_rxtimeout,
                        "c_MyFirstApplication",
                        "R1",
                        "E1",
                        &ss_canDevice.canBus(busnr).m_handle);
    // successful?
    if ( l_retval == NTCAN_SUCCESS ) {
      ss_canDevice.canBus(busnr).mb_isHandleAvailable = true;
      // success => HW found
      printf ( "canOpen: CAN%u using netnumber %lu\n", unsigned(busnr), l_netnumber);
      busnr++;
    }
  }
  
  if (busnr == 0) 
  {
     printf("HW detection failed.\n");
     exit(1);
  }
  return 1;
}

bool resetCard(void)
{
  return true;
}

// PURPOSE: To initialize the specified CAN BUS to begin sending/receiving msgs
bool openBusOnCard(uint8_t ui8_bus, uint32_t wBitrate, server_c* pc_serverData)
{
  DEBUG_PRINT1("init can bus %d\n", ui8_bus);

  if ( ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen )
    return true; // already initialized and files are already open

  if ( !ss_canDevice.canBus(ui8_bus).mb_isHandleAvailable )
    return false; // not available

  DEBUG_PRINT1("Opening CAN BUS channel=%u\n", unsigned(ui8_bus));

  long                    l_retval;
  long                    l_baud;

  switch (wBitrate) {
  case 1000: l_baud = 0; break;
  case 800:  l_baud = 1; break;
  case 500:  l_baud = 2; break;
  case 250:  l_baud = 3; break;
  case 125:  l_baud = 4; break;
  case 100:  l_baud = 5; break;
  case  50:  l_baud = 6; break;
  case  20:  l_baud = 7; break;
  default: 
    printf ( "invalid baud rate %dk\n(user defined baud rate not yet supported)\n", wBitrate);
    return false;
  }

  l_retval = canSetBaudrate ( ss_canDevice.canBus(ui8_bus).m_handle, l_baud );
  // successful ?
  if ( l_retval != NTCAN_SUCCESS ) {
    // error
    printf ( "could not initialize CAN!\n");
    return false;
  }

  // enable all CAN identifiers
  l_retval = canEnableAllIds(ss_canDevice.canBus(ui8_bus).m_handle, true);
  if (l_retval != NTCAN_SUCCESS) {
    // Error handling...
    printf ( "error in  canEnableAllIds!\n");
    return false;       
  }

  l_retval = canSetFilterMode(ss_canDevice.canBus(ui8_bus).m_handle, filterMode_nofilter);
  if (l_retval != NTCAN_SUCCESS) {
    // Error handling...
    printf ( "error in  canEnableAllIds!\n");
    return false;       
  }

  CAN_IF_STATUS t_CANStatus;
  l_retval = canStatus ( ss_canDevice.canBus(ui8_bus).m_handle, &t_CANStatus );
  if ( l_retval == NTCAN_SUCCESS ) {
    printf ( "status from card:\n");
    //  printf ( "HW Rev. : %04X\n", t_CANStatus.w_hw_recv );
    printf ( "FW Rev. : %04X\n", t_CANStatus.w_fw_rev );
    printf ( "DLL Rev. : %04X\n", t_CANStatus.w_dll_rev);
    printf ( "Drv Rev. : %04X\n", t_CANStatus.w_drv_rev);
    printf ( "exist. Nets: %02x\n", t_CANStatus.w_netctr);
  }

  ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = true;
  pc_serverData->canBus(ui8_bus).mb_deviceConnected = true;
  return true;
}


void closeBusOnCard(uint8_t ui8_bus, server_c* /*pc_serverData*/)
{
  DEBUG_PRINT1("close can bus %d\n", ui8_bus);
  //ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = false;
  // do not call close or CAN_CLOSE because COMMAND_CLOSE is received during initialization!
}


bool readFromBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
  long            l_retval;
  CMSG            t_CANMsg[1];
  long      l_len;

  l_len = 1;
  l_retval = canRead (ss_canDevice.canBus(ui8_bus).m_handle,
                      &t_CANMsg[0],
                      &l_len);
  // successful ?
  if ( l_retval == NTCAN_SUCCESS ) {
    ps_canMsg->ui32_id = t_CANMsg[0].l_id;
    ps_canMsg->i32_len = t_CANMsg[0].by_len & 0x0F;
    ps_canMsg->i32_msgType = (t_CANMsg[0].by_extended ? 1 : 0);

    for (uint8_t ui8_cnt = 0; ui8_cnt < ps_canMsg->i32_len; ui8_cnt++)
      ps_canMsg->ui8_data[ui8_cnt] = t_CANMsg[0].aby_data[ui8_cnt];

    return true;
  }
  return false;
}


// PURPOSE: To send a msg on the specified CAN BUS
// RETURNS: non-zero if msg was sent ok
//          0 on error
int16_t sendToBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
  CMSG       t_CANMsg[1];
  long l_len;
  long l_retval;

  l_len = 1; // one frame
  // fill with CAN data
  t_CANMsg[0].l_id = ps_canMsg->ui32_id;
  t_CANMsg[0].by_len = ps_canMsg->i32_len;
  t_CANMsg[0].by_extended = ( ps_canMsg->i32_msgType ? 1 : 0 );
  t_CANMsg[0].by_remote = 0;
  for ( int32_t ind = 0; ind < ps_canMsg->i32_len; ind++ )
    t_CANMsg[0].aby_data[ind] = ps_canMsg->ui8_data[ind];

  // should have been checked already by calling function isBusOpen:
  assert((ui8_bus <= HAL_CAN_MAX_BUS_NR) && ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen);
  // blocked transmit
  l_retval = canSend ( ss_canDevice.canBus(ui8_bus).m_handle,
                       &t_CANMsg[0],
                       &l_len );
  if ( l_retval == NTCAN_SUCCESS ) {
    return 1;
  }
  else
  {
    printf("error in canSend: %d!\n", l_retval);
    return 0;
  }
}

