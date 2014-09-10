/*
  can_server_common.h: Code common for both can_servers, socket
    and msg-queue.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef _CAN_SERVER_COMMON_H_
#define _CAN_SERVER_COMMON_H_

#include <pthread.h>
#include <stdio.h>
#include <string>
#include <list>

#include <../../tools/libs/misc/yasper.h>
#include <IsoAgLib/hal/pc/errcodes.h>

#define HAL_CAN_MAX_BUS_NR 16

namespace __HAL {

class server_c;

}

void *readUserInput(void *ap_arg);

void checkAndHandleOptionsAndStartup(int argc, char *argv[], __HAL::server_c &ar_server);
void usage();

bool newFileLog( __HAL::server_c *p_server, size_t n_bus );
void closeFileLog(__HAL::server_c *ap_server, size_t an_bus );
void dumpCanMsg (uint8_t bBusNumber, uint8_t bMsgObj, canMsg_s* ps_canMsg, FILE *f_handle);

namespace __HAL {


#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

class LogFile_c {
public:
  LogFile_c ( std::string const &arstr_filename )
    : mp_file( fopen( arstr_filename.c_str(), "a+" ) ) {}
  ~LogFile_c() {
    if (mp_file)
      fclose( mp_file );
  }
  FILE *getRaw() {
    return mp_file;
  }
  struct Null_s {
    yasper::ptr< LogFile_c > operator()() {
      return 0;
    }
  };
private:
  FILE *mp_file;
  // intentionally not implemented (prevent use):
  LogFile_c( LogFile_c const & );
  LogFile_c &operator= ( LogFile_c const & );
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif


// server specific data
class server_c {
public:
  server_c();

#ifdef CAN_DRIVER_MESSAGE_QUEUE
  msqData_s ms_msqDataServer;
#endif

  std::list<client_c> mlist_clients;
  std::string mstr_logFileBase;
  std::string mstr_inputFile;
  // logging
  bool     mb_logMode;
  // monitor
  bool     mb_monitorMode;
  // replay
  bool     mb_inputFileMode;
  FILE*    mf_canInput;
#ifndef WIN32
  // daemon option
  bool     mb_daemon;
#endif

  bool     marrb_remoteDestinationAddressInUse[0x100];

#ifdef CAN_DRIVER_MESSAGE_QUEUE
  int32_t  mi32_lastPipeId;
  int32_t  marri32_fileDescrWakeUpPipeForNewBusEvent[2];
#endif

  // if >0 => do not send messages with local destination address on the bus
  int16_t  mi16_reducedLoadOnIsoBus;

  pthread_mutex_t mt_protectClientList;
  bool     mb_interactive;
  int      mi_canReadNiceValue;

  struct canBus_s {
    uint16_t                 mui16_globalMask;
    int32_t                  mi32_can_device;
    int32_t                  mi32_sendDelay;
    bool                     mb_deviceConnected;
    uint16_t                 mui16_busRefCnt;
    yasper::ptr< LogFile_c > m_logFile;
    canBus_s();
  };
  canBus_s &canBus(size_t n_index);
  size_t nCanBusses();

private:
  std::vector< canBus_s > mvec_canBus;
};

inline server_c::canBus_s &server_c::canBus(size_t n_index)
{
  if (mvec_canBus.size() <= n_index)
    mvec_canBus.resize(n_index + 1);
  return mvec_canBus[n_index];
}

inline size_t server_c::nCanBusses()
{
  return mvec_canBus.size();
}

int32_t getTime();

} //namespace __HAL

class AOption_c {
public:
  int checkAndHandle(int argc, char *argv[], int ai_pos, __HAL::server_c &ar_server) const {
    return doCheckAndHandle(argc, argv, ai_pos, ar_server);
  }
  std::string getSetting(__HAL::server_c &ar_server) const { return doGetSetting(ar_server); }
  std::string getUsage() const { return doGetUsage(); }
  virtual ~AOption_c() {}
private:
  virtual int doCheckAndHandle(int argc, char *argv[], int ai_pos, __HAL::server_c &ar_server) const = 0;
  virtual std::string doGetSetting(__HAL::server_c &ar_server) const = 0;
  virtual std::string doGetUsage() const = 0;
};

/* The following identifiers OPTION_... serve as template parameter
 * values for Option_c (which is defined below). */
enum OPTION_MONITOR {};
enum OPTION_LOG {};
enum OPTION_FILE_INPUT {};
enum OPTION_REDUCED_LOAD_ISO_BUS_NO {};
enum OPTION_NICE_CAN_READ {};
enum OPTION_INTERACTIVE {};
enum OPTION_PRODUCTIVE {};
#ifndef WIN32
enum OPTION_DAEMON {};
#endif
enum OPTION_HELP {};

template < typename OPTION >
class Option_c : public AOption_c {
public:
  static yasper::ptr< AOption_c > create() {
    return yasper::ptr< AOption_c >( new Option_c );
  }
private:
  virtual int doCheckAndHandle(int argc, char *argv[], int ai_pos, __HAL::server_c &ar_server) const;
  virtual std::string doGetSetting(__HAL::server_c &ar_server) const;
  virtual std::string doGetUsage() const;
};

extern yasper::ptr< AOption_c > const *const gp_optionsBegin;
extern yasper::ptr< AOption_c > const *const gp_optionsEnd;

/////////////////////////////////////////////////////////////////////////
// Driver Function Declarations

uint32_t initCardApi();
bool     resetCard(void);

bool     openBusOnCard(uint8_t ui8_bus, uint32_t wBitrate, __HAL::server_c* pc_serverData);
void     closeBusOnCard(uint8_t ui8_bus, __HAL::server_c* pc_serverData);

int16_t  sendToBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, __HAL::server_c* pc_serverData);
bool     readFromBus(uint8_t ui8_bus, canMsg_s* ps_canMsg, __HAL::server_c* pc_serverData);

bool     isBusOpen(uint8_t ui8_bus);

void sendUserMsg(uint32_t DLC, uint32_t ui32_id, uint32_t ui32_bus, uint8_t ui8_xtd, uint8_t* pui8_data, __HAL::server_c* pc_serverData);

#endif //ndef _CAN_SERVER_COMMON_H_
