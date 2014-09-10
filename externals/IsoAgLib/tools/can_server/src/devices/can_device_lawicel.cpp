/*
  can_device_lawicel.cpp: interface for LAWICEL CANUS

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"

// system/drivers
#include <windows.h>
#include <lawicel_can.h>


using namespace __HAL;


#define LAWICEL_ADAPTER_NAME_LEN 32
#define LAWICEL_ADAPTER_VER_LEN 255
#define LAWICEL_ADAPTER_BITRATE_LEN 5

namespace
{
struct canDevice_s {
  struct canBus_s {
    bool isOpen;
    bool isValid;
    char name[LAWICEL_ADAPTER_NAME_LEN];
    CANHANDLE handle;
    bool mb_canBusIsOpen;
    canBus_s();
  };
  canBus_s &canBus(size_t n_index);
  size_t nCanBusses();

private:
  std::vector< canBus_s > mvec_canBus;
} ss_canDevice;
}

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
  isOpen(false),
  isValid(false),
  handle(0),
  mb_canBusIsOpen(false)
{
  name[0] = 0;
}

bool isBusOpen(uint8_t ui8_bus)
{
	return ss_canDevice.canBus(ui8_bus).isOpen;
}

uint32_t initCardApi ()
{
	char szAdapter[LAWICEL_ADAPTER_NAME_LEN];
	int numOfAdapters = canusb_getFirstAdapter(szAdapter, sizeof(szAdapter));
	int adapterBufferCursor = 0;

	if (0 < numOfAdapters) {

		printf("Driver Configuration:\n  ChannelCount=%u\n", numOfAdapters);

		ss_canDevice.canBus(adapterBufferCursor).isValid = true;
		strcpy_s(ss_canDevice.canBus(adapterBufferCursor).name, sizeof(ss_canDevice.canBus(adapterBufferCursor).name), szAdapter);
		printf("  Channel %u : %s\n",
			adapterBufferCursor,
			ss_canDevice.canBus(adapterBufferCursor).name
		);
		adapterBufferCursor++;

		for (int i = 1; i < numOfAdapters; i++ ) {

			if (0 < canusb_getNextAdapter(szAdapter, sizeof(szAdapter))) {
				ss_canDevice.canBus(adapterBufferCursor).isValid = true;
				strcpy_s(ss_canDevice.canBus(adapterBufferCursor).name, sizeof(ss_canDevice.canBus(adapterBufferCursor).name), szAdapter);
				printf("  Channel %u : %s\n",
					adapterBufferCursor,
					ss_canDevice.canBus(adapterBufferCursor).name
				);
				adapterBufferCursor++;
			}
			else {
				printf("Failed to get adapter.\n");
			}
		}
	}
	else {
		printf("No adapter found, or failed to get adapter.\n");
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
	unsigned long flags = 0;
	char baudRate[LAWICEL_ADAPTER_BITRATE_LEN];

  canDevice_s::canBus_s *info = &ss_canDevice.canBus(ui8_bus);
	
	if (!info->isValid) {
		return false;
	}

	if (info->isOpen) {
		return true;
	}

	sprintf_s(baudRate, LAWICEL_ADAPTER_BITRATE_LEN, "%d", wBitrate);

	CANHANDLE handle = canusb_Open( 
		(LPCSTR)info->name,
		(LPCSTR)baudRate,
		CANUSB_ACCEPTANCE_CODE_ALL,
		CANUSB_ACCEPTANCE_MASK_ALL,
		flags);
	
	if (handle <= 0) {
		printf("Failed to open CANUSB interface.\n" );
		pc_serverData->canBus(ui8_bus).mb_deviceConnected = false;
		return false;
	}
	else {
		info->handle = handle;
		info->isOpen = true;

		char szVersion[LAWICEL_ADAPTER_VER_LEN];

		if (ERROR_CANUSB_OK == canusb_VersionInfo(handle, szVersion)) {
			printf("Version : %s\n", szVersion);
		}
		else {
			printf("Failed to get version information.\n");
		}

		pc_serverData->canBus(ui8_bus).mb_deviceConnected = true;
		return true;
	}
}


//void closeBusOnCard(uint8_t ui8_bus, server_c* /*pc_serverData*/)
void closeBusOnCard(uint8_t ui8_bus)
{
	canDevice_s::canBus_s *info = &ss_canDevice.canBus(ui8_bus);
	
	if (!info->isValid) {
		return;
	}

	if (!info->isOpen) {
		return;
	}

	if (0 < canusb_Flush(info->handle, FLUSH_DONTWAIT)) {
		printf("Output buffer is flushed.\n" );
	}
	else {
		printf("Output buffer can't be flushed.\n" );
	}

	if (canusb_Close(info->handle) == ERROR_CANUSB_OK) {
		printf("CANUSB interface is closed.\n" );
	}
	else {
		printf("CANUSB interface can't be closed.\n" );
	}
	
	info->isOpen = false;
}


void closeBusOnCard(uint8_t ui8_bus, server_c* pc_serverData)
{
	closeBusOnCard(ui8_bus);
	pc_serverData->canBus(ui8_bus).mb_deviceConnected = false;
}


bool readFromBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
	CANMsg msg;

	canDevice_s::canBus_s *info = &ss_canDevice.canBus(ui8_bus);
	
	if (!info->isValid) {
		return false;
	}

	if (!info->isOpen) {
		return false;
	}
	
	if (canusb_Read(info->handle, &msg) == ERROR_CANUSB_OK) {
		ps_canMsg->ui32_id = msg.id;
		ps_canMsg->i32_len = msg.len;
		ps_canMsg->i32_msgType = 1;
		memcpy( ps_canMsg->ui8_data, msg.data, ps_canMsg->i32_len );
		return true;
	}
	else {
		return false;
	}
}


// PURPOSE: To send a msg on the specified CAN BUS
// RETURNS: non-zero if msg was sent ok
//          0 on error
int16_t sendToBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
	CANMsg msg;

	canDevice_s::canBus_s *info = &ss_canDevice.canBus(ui8_bus);
	
	if (!info->isValid) {
		return 0;
	}

	if (!info->isOpen) {
		return 0;
	}

	msg.id = ps_canMsg->ui32_id;
	msg.len = ps_canMsg->i32_len;
	msg.flags = CANMSG_EXTENDED;
	memcpy(msg.data, ps_canMsg->ui8_data, ps_canMsg->i32_len );

	int result;

	if (result = canusb_Write(info->handle, &msg) == ERROR_CANUSB_OK) {
		return 1;
	}
	else {
		printf("error in canSend: %d!\n", result);
		return 0;
	}
}
