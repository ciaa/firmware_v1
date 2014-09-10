/*
  can_device_no_card.cpp: dummy interface when no can device is present

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"

using namespace __HAL;

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
  return 1;
}

bool resetCard(void)
{
  return true;
}


bool openBusOnCard(uint8_t ui8_bus, uint32_t /* wBitrate */, server_c* /* pc_serverData */)
{
  DEBUG_PRINT1("init can bus %d\n", ui8_bus);

  ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = true;
  return true;
}

#if DEBUG_CANSERVER
void closeBusOnCard(uint8_t ui8_bus, server_c* /* pc_serverData */)
#else
void closeBusOnCard(uint8_t /*ui8_bus*/, server_c* /* pc_serverData */)
#endif
{
  DEBUG_PRINT1("close can bus %d\n", ui8_bus);
  //ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen = false;
  // do not call close or CAN_CLOSE because COMMAND_CLOSE is received during initialization!
}


// PURPOSE: To send a msg on the specified CAN BUS
int16_t sendToBus(uint8_t /* ui8_bus */, canMsg_s* /* ps_canMsg */, server_c* /* pc_serverData */)
{
  return 1; // success
}

bool readFromBus(uint8_t /* ui8_bus */, canMsg_s* /* ps_canMsg */, server_c* /* pc_serverData */)
{
  return false;
}

