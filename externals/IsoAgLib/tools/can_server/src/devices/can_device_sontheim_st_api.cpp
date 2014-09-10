/*
  can_device_sontheim_st_api.cpp: interface for SONTHEIM can card
    (single thread API)

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

#define USE_THREAD

#include <stdio.h>
#ifdef WIN32
#include <conio.h>
#endif
#include <time.h>

#define HWTYPE_AUTO 1000


#define c_ICAN     1
#define c_PowerCAN 2
#define c_CANAS    3
#define c_EICAN    6
#define c_ECAN_PCI 7
#define c_CANLpt   8
#define c_PowerCANPCI       10
#define c_CANUSB_Std_Api    11


#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"
#include <assert.h>

using namespace __HAL;

/////////////////////////////////////////////////////////////////////////
// Globals
HINSTANCE hCAPIDLL;


// CAN Globals
int AllCanChannelCount = 0;
int apiversion;
int DLL_loaded = false;

// IO address for LPT and ISA ( PCI and PCMCIA use automatic adr )
const int32_t LptIsaIoAdr = 0x378;

#ifdef USE_CAN_CARD_TYPE
int32_t gHwType = USE_CAN_CARD_TYPE;
#else
// select the Vector CAN card type to use
int32_t gHwType = HWTYPE_AUTO;
#endif

#ifdef WIN32
/////////////////////////////////////////////////////////////////////////////
// definitions of DLL functions
/////////////////////////////////////////////////////////////////////////////
int (far __stdcall *ca_InitApi_1) (int typ, int IoAdr);

int (far __stdcall *ca_ResetCanCard_1) (void);

int (far __stdcall *ca_InitCanCard_1)
    (int channel, int msgnr, int accode, int accmask, int fullcanmask[5],
     int btr0, int btr1, int octrl, int typ, int extended);
int  (far __stdcall *ca_TransmitCanCard_1)
    (int channel, int extended, int *data);

int  (far __stdcall *ca_GetData_1) (int *data);

int  (far __stdcall *ca_GetDataChannel_1) (int channel, int *data);

// alle Identifier sperren
int  (far __stdcall *ca_ResetSoftwareFilterMask_1) (int channel);

// alle Identifier zulassen
int  (far __stdcall *ca_SetAllId_1 ) (int channel);


int  (far __stdcall *ca_SetFilterId_1) (int channel, int identifier);

int  (far __stdcall *ca_Instruction_1) (int *data);
#else
int (*ca_InitApi_1) (int typ, int IoAdr);
int (*ca_ResetCanCard_1) (void);
int (*ca_InitCanCard_1)
    (int channel, int msgnr, int accode, int accmask, int fullcanmask[5],
     int btr0, int btr1, int octrl, int typ, int extended);
int  (*ca_TransmitCanCard_1)
    (int channel, int extended, int *data);
int  (*ca_GetData_1) (int *data);
int  (*ca_GetDataChannel_1) (int channel, int *data);
// alle Identifier sperren
int  (*ca_ResetSoftwareFilterMask_1) (int channel);
// alle Identifier zulassen
int  (*ca_SetAllId_1 ) (int channel);
int  (*ca_SetFilterId_1) (int channel, int identifier);
int  (*ca_Instruction_1) (int *data);
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

int16_t loadDllFunctionAddresses(void)
{
#ifdef WIN32
  DLL_loaded = false;

  // if ((int) (hCAPIDLL = LoadLibrary ("C:\\Hör\\AKTUELL\\Canapi_CanLPT\\Software\\PC\\api\\Release\\CANAPI.DLL")) <=  HINSTANCE_ERROR) {
  if ((int) (hCAPIDLL = LoadLibrary ("CANAPI.DLL")) <=  HINSTANCE_ERROR) {
       printf ("ERROR - CAN Library could NOT be opened!!!\n");
    return HAL_CONFIG_ERR;
  }
  ca_InitApi_1 =
    (int (far __stdcall*) (int typ, int IoAdr)) GetProcAddress (hCAPIDLL,"ca_InitApi_1");

  ca_ResetCanCard_1 =
    (int (far __stdcall*) (void))               GetProcAddress (hCAPIDLL,"ca_ResetCanCard_1");

  ca_InitCanCard_1 =
    (int (far __stdcall*) (int channel, int msgnr, int accode, int accmask, int fullcanmask[5], int btr0, int btr1, int octrl,
      int typ, int extended))                   GetProcAddress (hCAPIDLL,"ca_InitCanCard_1");

  ca_TransmitCanCard_1 =
    (int (far __stdcall*) (int channel, int extended, int *data)) GetProcAddress (hCAPIDLL,"ca_TransmitCanCard_1");

  ca_GetData_1 =
    (int (far __stdcall*) (int *data))          GetProcAddress (hCAPIDLL,"ca_GetData_1");

  ca_ResetSoftwareFilterMask_1 =
    (int (far __stdcall*) (int channel))        GetProcAddress (hCAPIDLL,"ca_ResetSoftwareFilterMask_1");

  ca_SetAllId_1 =
    (int (far __stdcall*) (int channel))        GetProcAddress (hCAPIDLL,"ca_SetAllId_1");


  ca_SetFilterId_1 =
    (int (far __stdcall*) (int channel,int identifier)) GetProcAddress (hCAPIDLL,"ca_SetFilterId_1");

  ca_Instruction_1 =
    (int (far __stdcall*) (int *data))          GetProcAddress (hCAPIDLL,"ca_Instruction_1");

  DLL_loaded = true;
#endif
  return HAL_NO_ERR;
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
//  gHwType = c_CANLpt; //noch zu machen: HWTYPE_AUTO;

  printf("load driver Dll\n");
  if ( loadDllFunctionAddresses() != HAL_NO_ERR) return 0;
  printf("driver DLL loaded\n");

  if ( HWTYPE_AUTO == gHwType )
  {
    const int32_t ci_tryCardTypes[] = { c_CANLpt, c_PowerCANPCI, c_CANUSB_Std_Api, c_PowerCAN, c_CANAS, c_ICAN, c_EICAN, c_ECAN_PCI };
    const int32_t ci_cardTypeCnt = 7;
    for ( int32_t ind = 0; ind < ci_cardTypeCnt; ind++ )
    {
#ifdef WIN32
      Sleep( 100 );
#endif

      if ( ( ci_tryCardTypes[ind] != c_CANAS ) && ( ci_tryCardTypes[ind] != c_ECAN_PCI ) )
      {
        if (ci_tryCardTypes[ind]== c_CANUSB_Std_Api)
                    apiversion = ca_InitApi_1(ci_tryCardTypes[ind], 0x300);  //I/O Address is 0x300 for CANUSB!
                else
                    apiversion = ca_InitApi_1(ci_tryCardTypes[ind], LptIsaIoAdr);
      }
      else
      {
        apiversion = ca_InitApi_1(ci_tryCardTypes[ind], 0);
      }
      // break search loop if card found
      if ( apiversion != 0 ) {
        gHwType = ci_tryCardTypes[ind]; // store used card type
        break;
      }
      else
      {
#ifdef WIN32
        FreeLibrary(hCAPIDLL);
#endif
        DLL_loaded = false;
#ifdef WIN32
        Sleep( 10 );
#endif
        loadDllFunctionAddresses();
      }
    }
    if ( apiversion == 0 )
    { // failure - nothing found
      printf( "FAILURE - No CAN card was found with automatic search with IO address %04X for manually configured cards\r\n", LptIsaIoAdr );
      return 0;
    }
  }
  else
  {
    if ( ( gHwType != c_CANAS ) && ( gHwType != c_ECAN_PCI ) )
    {
      apiversion = ca_InitApi_1(gHwType, LptIsaIoAdr);
    }
    else
    {
      apiversion = ca_InitApi_1(gHwType, 0);
    }
    if ( apiversion == 0 )
    { // failure - nothing found
      printf( "FAILURE - No CAN card was found with automatic search with IO address %04X for manually configured cards\r\n", LptIsaIoAdr );
      return 0;
    }
  }
  printf("InitApi done\n");
  printf("API-Version......: %4x\n",(apiversion & 0xFF00) >> 8);
  printf("API-Ausfuehrung..: %4x\n",(apiversion & 0xFF));
  // ----------------------------------------------------------------------------
  // do the reset
  int i = ca_ResetCanCard_1();
  if (i) 
  {
    printf("Reset CANLPT ok\n");
  }
  else {
    printf("Reset CANLPT not ok\n");
    // exit(0);
  }

  // Added this section of code to try to get the version from the CANUSB adapter.
  if (gHwType == c_CANUSB_Std_Api)
  {
      int sdata[16];
      int k=0;
      int erg = 0;
      char buf[256];
      for (k=0 ; k<16 ; k++) sdata[k]=0;
      sdata[0]=0x25;
      erg = ca_Instruction_1(sdata);
      if(erg)
      {
          buf[0]=0;
          for(k=3;k<16;k++)
          {
              buf[k-3] = (char)sdata[k];
          }
          buf[12]=0;
          printf("\nVersion of CANUSB: %s", buf);
      }
      else
      {
          printf("\nReading of Version failed !!!");
          // getch();
      }
  }
  // wait to be shure that CAN card is clean reset
#ifdef WIN32
  Sleep(100);
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
  DEBUG_PRINT1("init can bus %d\n", ui8_bus);

  if( !ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen ) {
    DEBUG_PRINT1("Opening CAN BUS channel=%d\n", ui8_bus);

    if( !DLL_loaded )
      return false;

    int fdata[16];
    int channel = 0;
    int btr0, btr1;
    int errcode;
    int accMask = 0x0;

    if ( ( gHwType == c_ICAN ) || ( gHwType == c_CANLpt ) )
    {
      if ( ui8_bus > 0 )
      {
        printf( "ERROR - the selected CAN card has only ONE channel, so that Bus-Nr: %hd is not defined\r\n", ui8_bus );
        return false;
      }
    }
    else
    { // channel for sontheim multi BUS cards starts counting with 1
      channel = ui8_bus + 1;
    }

    for ( int fdata_ind = 0; fdata_ind < 15; fdata_ind++ )
      fdata[fdata_ind] = 0;

    // set the data array for control of filters
    // -> let here everything IN
    switch ( gHwType )
    {
      case c_ICAN: // not used -> let 0
        break;
      case c_CANLpt:
          case c_CANUSB_Std_Api:
        // receive everything for CAN-LPT style
        fdata[0]  = 0x00;
        fdata[1]  = 0x00;
        fdata[2]  = 0x00;
        fdata[3]  = 0x00;
        fdata[4]  = 0xFF;
        fdata[5]  = 0xFF;
        fdata[6]  = 0xFF;
        fdata[7]  = 0xFF;

        fdata[8]  = 0x00;
        fdata[9]  = 0x00;
        fdata[10] = 0x00;
        fdata[11] = 0x00;
        fdata[12] = 0xFF;
        fdata[13] = 0xFF;
        fdata[14] = 0xFF;
        fdata[15] = 0xFF;
        break;
      case c_PowerCAN:
      case c_CANAS:
      case c_EICAN:
        // receive everything the other style
        fdata[0]  = 0x00;
        fdata[1]  = 0x00;
        fdata[2]  = 0xFFE0;
        fdata[3]  = 0x00;
        fdata[4]  = 0xFF;
        break;
    }

    // derive appropriate the bitrate setting
    switch ( gHwType )
    {
      case c_CANLpt:
              accMask = 0xFF;
        switch ( wBitrate )
        {
          case 10: { btr0 = 0x3F; btr1 = 0x7F;} break;
          case 20: { btr0 = 0x31; btr1 = 0x1c;} break;
          case 50: { btr0 = 0x13; btr1 = 0x1C;} break;
          case 100: { btr0 = 0x09; btr1 = 0x1C;} break;
          case 125: { btr0 = 0x07; btr1 = 0x1C;} break;
          case 250: { btr0 = 0x03; btr1 = 0x1C;} break;
          case 500: { btr0 = 0x01; btr1 = 0x1C;} break;
          case 800: { btr0 = 0x01; btr1 = 0x16;} break;
          case 1000: { btr0 = 0x00; btr1 = 0x1C;} break;
        }
        break;
          case c_CANUSB_Std_Api:
        switch ( wBitrate )
        {
          case 10: { btr0 = 0xF1; btr1 = 0x3E;} break;
          case 20: { btr0 = 0xF1; btr1 = 0x3E;} break;
          case 50: { btr0 = 0xF1; btr1 = 0x3E;} break;
          case 100: { btr0 = 0xD8; btr1 = 0x3E;} break;
          case 125: { btr0 = 0xD3; btr1 = 0x3E;} break;
          case 250: { btr0 = 0xC9; btr1 = 0x3E;} break;
          case 500: { btr0 = 0xC3; btr1 = 0x7F;} break;
          case 800: { btr0 = 0xC1; btr1 = 0x1C;} break;
          case 1000: { btr0 = 0xC1; btr1 = 0x1C;} break;
        }
        break;
      default:
        switch ( wBitrate )
        {
          case 10: { btr0 = 0x31; btr1 = 0x1c;} break;
          case 20: { btr0 = 0x18; btr1 = 0x1c;} break;
          case 50: { btr0 = 0x09; btr1 = 0x1c;} break;
          case 100: { btr0 = 0x04; btr1 = 0x1c;} break;
          case 125: { btr0 = 0x03; btr1 = 0x1c;} break;
          case 250: { btr0 = 0x01; btr1 = 0x1C;} break;
          case 500: { btr0 = 0x00; btr1 = 0x1C;} break;
          case 800: { btr0 = 0x00; btr1 = 0x16;} break;
          case 1000: { btr0 = 0x00; btr1 = 0x14;} break;
        }
        break;
    }
    errcode = ca_InitCanCard_1(
              channel,  // 0 for CANLPT/ICAN, else 1 for first BUS
              0x00,  // msg-nr / 0 for CANLPT/ICAN
              0x00,  // Acceptance Code to receive everything for ICAN
              accMask,  // Acceptance Mask to receive everything for ICAN
              fdata, // filter array of int[16];
              btr0,  // BTR0
              btr1,  // BTR1
              0x00,  // reserved
              0x00,  // typ 0 = 2 x 32 Bit, 1 = 4 x 16 Bit,
                    // 2 = 8 x 8 Bit, 3 = kein durchlass
              0x00); // reserved

    if ( errcode && ( gHwType != c_ICAN ) )
    { // as the examples are not very clarifying in relation to the DOC
      // we configure also msg obj 0xF to get a safe receive of Std AND Ext
      if (errcode)
      {
        printf ("CAN-LPT initialized for first MsgObj - Now configure for MsgObj 0xF to get safe receive of Std AND Ext\n");
      }
      else
      {
        printf ("can't initialize CANLPT");
        return false;
      }
      errcode = ca_InitCanCard_1(
                channel,  // 0 for CANLPT/ICAN, else 1 for first BUS
                0x0F,  // msg-nr
                0x00,  // Acceptance Code to receive everything for ICAN
                accMask,  // Acceptance Mask to receive everything for ICAN
                fdata, // filter array of int[16];
                btr0,  // BTR0
                btr1,  // BTR1
                0x00,  // reserved
                0x00,  // typ 0 = 2 x 32 Bit, 1 = 4 x 16 Bit,
                      // 2 = 8 x 8 Bit, 3 = kein durchlass
                0x00); // reserved
    }
    if (errcode)
    {
      printf ("CAN-LPT initialized\n");
    }
    else
    {
      printf ("can't initialize CAN");
      return false;
    }

    ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = true;
    pc_serverData->canBus(ui8_bus).mb_deviceConnected = true;
	return true;
  }
  else
    return true; // already initialized and files are already open
}


void closeBusOnCard(uint8_t ui8_bus, server_c* /*pc_serverData*/)
{
  DEBUG_PRINT1("close can bus %d\n", ui8_bus);
  //ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = false;
  // do not call close or CAN_CLOSE because COMMAND_CLOSE is received during initialization!
}


bool readFromBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
  bool b_dataReceived = false;
  static int pi_receivedata[15];

  if ( gHwType == c_ECAN_PCI )
  {
    if ( ca_GetDataChannel_1( ui8_bus + 1, pi_receivedata) )
      b_dataReceived = true;
  }
  else
  { // specify no channel on data request
    if ( ca_GetData_1( pi_receivedata) )
      b_dataReceived = true;
  }

  if (!b_dataReceived)
    return false;

  ps_canMsg->ui32_id = pi_receivedata[1];

  if (ps_canMsg->ui32_id >= 0x7FFFFFFF)
  {
    printf("!!Received of malformed message with undefined CAN ident: %x\n", ps_canMsg->ui32_id);
    return false;
  }

  ps_canMsg->i32_len = ( pi_receivedata[2] & 0xF );
  if ( ps_canMsg->i32_len > 8 )
    ps_canMsg->i32_len = 8;

  // detect channel
  switch ( pi_receivedata[0] )
  {
    case 0x7001: // $7001 = CAN 1 standard data received
      ps_canMsg->i32_msgType = 0;
      break;
    case 0x7002: // $7002 = CAN 2 standard data received
      ps_canMsg->i32_msgType = 0;
      break;
    case 0x7003: // ICAN $7003 = CAN 1 data received
      ps_canMsg->i32_msgType = 0;
      break;
    case 0x7103: // ICAN $7103 = CAN 2 data received
      ps_canMsg->i32_msgType = 0;
      break;
    case 0xC001: // $C001 = CAN 1 extended data received
      ps_canMsg->i32_msgType = 1;
      break;
    case 0xC002: // $C002 = CAN 2 extended data received
      ps_canMsg->i32_msgType = 1;
      break;
    default:
      ps_canMsg->i32_msgType = 0;
      break;
  }

  for (uint8_t ui8_cnt = 0; ui8_cnt < ps_canMsg->i32_len; ui8_cnt++)
	ps_canMsg->ui8_data[ui8_cnt] = pi_receivedata[3 + ui8_cnt];

  return true;

}


// PURPOSE: To send a msg on the specified CAN BUS
// RETURNS: non-zero if msg was sent ok
//          0 on error
int16_t sendToBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
  static int pi_transmitdata[15];

  if ( ( gHwType == c_ICAN ) || ( gHwType == c_CANLpt ) )
  {
    if ( ui8_bus > 0 )
    {
      printf( "ERROR - the selected CAN card has only ONE channel, so that Bus-Nr: %hd is not defined\r\n", ui8_bus );
      return 0;
    }
  }


  pi_transmitdata[0] = ps_canMsg->ui32_id;
  pi_transmitdata[1] = ps_canMsg->i32_len;
  for ( int32_t ind = 0; ind < ps_canMsg->i32_len; ind++ )
    pi_transmitdata[2+ind] = ps_canMsg->ui8_data[ind];

  pi_transmitdata[11] = 0; // no remote

  // should have been checked already by calling function isBusOpen:
  assert((ui8_bus <= HAL_CAN_MAX_BUS_NR) && ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen);
  int result = ca_TransmitCanCard_1(ui8_bus + 1 /* channel (starts from 1!) */, ps_canMsg->i32_msgType, pi_transmitdata);
  if ( result )
    return 1;
  else
  {
    printf("HAL_OVERFLOW_ERROR!\n");
    return 0;
  }
}

