/*
  can_device_rte.cpp: interface for RTE can

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#define CVS_REV "$Revision: 2865 $"
#define RTE_CLIENT

#include <assert.h>
#include <iomanip>
#include <iostream>
#include <list>

// RTE Includes
#include <rte_client.h>   // be an RTE client


#include "rte_global.h"
#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"


using namespace __HAL;

/////////////////////////////////////////////////////////////////////////
// Globals


static struct canDevice_s {
  struct canBus_s {
    bool          mb_canBusIsOpen;
    can_c        *m_rteCan_c;

    canBus_s();
    ~canBus_s();
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
  m_rteCan_c(new can_c())
{
}

canDevice_s::canBus_s::~canBus_s()
{
  delete m_rteCan_c;
}

static std::list<canMsg_s> l_canMsg;

/*! Handler for identification reply */
static int ident_hdl(rtd_handler_para_t* para, char* ident) {
  if ((para == 0) || (ident == 0))
    return rte_set_error( null_pointer );

  return 0;
}

/*! Handler for acknowledge reply */
static int ackHdl(rtd_handler_para_t*, rte_return_t ) {
  return 0;
}

/*! Handler for incoming CAN messages. */
static int can_receive(rtd_handler_para_t* para, rtd_can_type_t type, uint32_t id, uint8_t size, const void *data,
                       rte_client_t cid, rtd_can_event_flag_t flag, uint8_t /*code*/)
{
  if (0 != flag)
  { /* ERROR has been reported. */
    return -1;
  }

  if ( ss_canDevice.canBus(para->rtd_msg->channel).m_rteCan_c->get_cid() == cid )
  { // echo of this rte client
    return -1;
  }

  canMsg_s s_canMsg;
  s_canMsg.ui32_id = id;
  s_canMsg.i32_len = size;
  s_canMsg.i32_msgType = (type == rtd_can_type_xtd_msg) ? 1 : 0;
  memcpy(s_canMsg.ui8_data, data, size);

  l_canMsg.push_back(s_canMsg);

  return 0;
}
bool isBusOpen(uint8_t ui8_bus)
{
  return ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen;
}


uint32_t initCardApi ()
{
  if ( ! rte_is_init() ) {
    std::string c_rteServer = HAL_PC_RTE_DEFAULT_SERVER;
    if ( getenv( "RTE_HOST" ) != NULL ) c_rteServer = getenv( "RTE_HOST" );

    if (rte_connect( c_rteServer.c_str() ) < 0) {
      cerr << "Unable to connect " << c_rteServer << " server." << endl;
      exit(1);
    } else {
      cout << "Connected to " << c_rteServer << " server." << endl;
    }
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

  if( !ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen ) {
    // Open a CAN messaging interface
    ss_canDevice.canBus(ui8_bus).m_rteCan_c->set_channel( ui8_bus );
    ss_canDevice.canBus(ui8_bus).m_rteCan_c->set_default_timeout( 60*RTE_ONE_SECOND );
    ss_canDevice.canBus(ui8_bus).m_rteCan_c->init();
    // build parameter string with sprintf to handle different baudrates
    char parameterString[50];
    sprintf( parameterString, "C%d,R,B%d,E1,L0", ui8_bus, wBitrate );
    ss_canDevice.canBus(ui8_bus).m_rteCan_c->set_parameters( parameterString );

    // Set handler functions.
    ss_canDevice.canBus(ui8_bus).m_rteCan_c->set_acknowledge_handler( ackHdl, ss_canDevice.canBus(ui8_bus).m_rteCan_c);
    ss_canDevice.canBus(ui8_bus).m_rteCan_c->set_identify_handler( ident_hdl, ss_canDevice.canBus(ui8_bus).m_rteCan_c);
    ss_canDevice.canBus(ui8_bus).m_rteCan_c->set_send_handler( can_receive, ss_canDevice.canBus(ui8_bus).m_rteCan_c );
    //  ss_canDevice.canBus(ui8_bus).m_rteCan_c->set_set_bus_state_handler( setBusState, ss_canDevice.canBus(ui8_bus).m_rteCan_c );

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


// PURPOSE: To send a msg on the specified CAN BUS
// RETURNS: non-zero if msg was sent ok
//          0 on error
int16_t sendToBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
  // should have been checked already by calling function isBusOpen:
  assert((ui8_bus <= HAL_CAN_MAX_BUS_NR) && ss_canDevice.canBus(ui8_bus).mb_canBusIsOpen);
  ss_canDevice.canBus(ui8_bus).m_rteCan_c->set_delta (0); // NOW!
  ss_canDevice.canBus(ui8_bus).m_rteCan_c->set_count (1); // send only once

  if (ps_canMsg->i32_msgType) {
    ss_canDevice.canBus(ui8_bus).m_rteCan_c->send (rtd_can_type_xtd_msg, ps_canMsg->ui32_id, ps_canMsg->i32_len, ps_canMsg->ui8_data);
  } else {
    ss_canDevice.canBus(ui8_bus).m_rteCan_c->send (rtd_can_type_std_msg, ps_canMsg->ui32_id, ps_canMsg->i32_len, ps_canMsg->ui8_data);
  }

  rte_poll();

  return 1;
}


bool readFromBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, server_c* pc_serverData)
{
  rte_poll();

  if (l_canMsg.size())
  {
    *ps_canMsg = l_canMsg.front();
    l_canMsg.pop_front();
    return true;
  }

  return false;
}

