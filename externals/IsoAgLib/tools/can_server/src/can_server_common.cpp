/*
  can_server_common.cpp: Code common for both can_servers, socket
    and msg-queue.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "wrapper_for_can_server_interface.h"
#include "can_server_common.h"
#include "can_filtering.h"

#include <iostream>
#include <sstream>
#include <functional>
#include <algorithm>
#include <string.h>

#ifdef WIN32
#include <conio.h>
#include <time.h>
#else
#include <unistd.h>
#include <sys/times.h>
#endif


struct PrintSetting_s : public std::unary_function< yasper::ptr< AOption_c >, void >{
  PrintSetting_s( __HAL::server_c &ar_server ) : mr_server(ar_server) {}
  void operator()(yasper::ptr< AOption_c > ap_option) {
    std::cerr << ap_option->getSetting(mr_server);
  }
  __HAL::server_c &mr_server;
};


void printSettings(__HAL::server_c &ar_server)
{
  std::for_each(gp_optionsBegin, gp_optionsEnd, PrintSetting_s(ar_server));
}


void checkAndHandleOptionsAndStartup(int argc, char *argv[], __HAL::server_c &ar_server)
{
  for (int i = 1, step = 0; i < argc; i += step) {
    for (yasper::ptr< AOption_c > const *p_opt = gp_optionsBegin; p_opt != gp_optionsEnd; ++p_opt) {
      step = (*p_opt)->checkAndHandle(argc, argv, i, ar_server);
      if (step)
        goto NextStep;
    }
    std::cerr << "Error: unknown option. ";
    usage();
    exit(1);
  NextStep:;
  }

#ifndef WIN32
  if (ar_server.mb_daemon) {
    // turn off interactive in daemon mode:
    ar_server.mb_interactive = false;

    // daemonize
    if (daemon( 0, 0 ) < 0) {
      std::cerr << "error during daemonization\n" << std::endl;
      exit(1);
    }
  }
#endif

  if (ar_server.mb_interactive) {
    std::cerr << "IsoAgLib CAN-Server" << std::endl;
    std::cerr << "(Run with '--help' to get help)" << std::endl << std::endl;
    printSettings (ar_server);
  }

  // explicitly call getTime to initialize the time to 0.
  (void) __HAL::getTime();

  const uint32_t apiversion = initCardApi();
  if ( apiversion == 0 ) { // failure - nothing found
    DEBUG_PRINT("FAILURE - No CAN card was found with automatic search\n");
    exit(1);
  }

  // do the reset
  if (!resetCard()) {
    DEBUG_PRINT("Reset not ok\n");
    exit(1);
  }
}

static std::string readInputLine()
{
#ifdef WIN32
  static DWORD const millisecondsToWaitForNextKeypress = 100u;
  std::ostringstream ostr_accumulator;
  for(;;) {
    while (!_kbhit())
      Sleep(millisecondsToWaitForNextKeypress);  
    int i_char = _getche();
    if ('\r' == i_char)
      break;
    ostr_accumulator << char(i_char);
  }
  return ostr_accumulator.str();
#else
  char c_buf;
  std::ostringstream ostr_accumulator;
  bool b_eof = false;
  do {
    ssize_t len = read(fileno(stdin), &c_buf, sizeof(c_buf));
    if (len == -1) // error condition 
      break;
    b_eof = (0 < len) && ('\n' == c_buf);
    ostr_accumulator << std::string(&c_buf, len - ssize_t(b_eof));
  } while (!b_eof);
  return ostr_accumulator.str();
#endif
}
static void enableLog( __HAL::server_c *p_server )
{
  for (size_t n_bus = 0; n_bus < p_server->nCanBusses(); ++n_bus) {
    if (0 < p_server->canBus(n_bus).mui16_busRefCnt) {
      (void)newFileLog( p_server, n_bus );
    }
  }
  p_server->mb_logMode = true;
}

static void disableLog( __HAL::server_c *p_server )
{
  p_server->mb_logMode = false;

  for (size_t n_canBusses = p_server->nCanBusses(); 0 < n_canBusses; ) {
    --n_canBusses;
    p_server->canBus(n_canBusses).m_logFile = __HAL::LogFile_c::Null_s()();
  }
}

void *readUserInput( void *ap_arg )
{
  static char const s_on[] = "on";
  static char const s_off[] = "off";
  static char const s_enable[] = "enable";
  static char const s_disable[] = "disable";
  static char const s_monitor[] = "monitor";
  static char const s_log[] = "log";
  static char const s_help[] = "help";
  static char const s_send[] = "send";
  static char const s_send_short[] = "s";
  static char const s_filter[] = "filter";
  static char const s_filter_short[] = "f";
  __HAL::server_c *pc_serverData = static_cast< __HAL::server_c * >(ap_arg);
  for (;;) {
    std::string inputline = readInputLine();
    std::istringstream istr_inputLine( inputline );
    std::string s_rawCommand;
    istr_inputLine >> s_rawCommand;
    size_t size_ignore = ( s_rawCommand.substr(0,2) == "--" ) ? 2 : 0;
    std::string s_command = s_rawCommand.substr( size_ignore );
    bool b_needHelp = false;
    if (!s_command.compare( s_on ) || !s_command.compare( s_enable )) {
      std::string s_toEnable;
      istr_inputLine >> s_toEnable;
      if (!s_toEnable.compare( s_monitor )) {
        std::cerr << "Enabling monitoring." << std::endl;
        pc_serverData->mb_monitorMode = true;
      } else if (!s_toEnable.compare( s_log )) {
        istr_inputLine >> pc_serverData->mstr_logFileBase;
        enableLog( pc_serverData );
      } else {
        std::cerr << "Don't know how to enable " << s_toEnable << "." << std::endl;
        b_needHelp = true;
      }
    } else if (!s_command.compare( s_off ) || !s_command.compare( s_disable )) {
      std::string s_toDisable;
      istr_inputLine >> s_toDisable;
      if (!s_toDisable.compare( s_monitor )) {
        std::cerr << "Disabling monitoring." << std::endl;
        pc_serverData->mb_monitorMode = false;
      } else if (!s_toDisable.compare( s_log )) {
        disableLog( pc_serverData );
      } else {
        std::cerr << "Don't know how to disable " << s_toDisable << "." << std::endl;
        b_needHelp = true;
      }
    }
    else if (s_command.compare(0, strlen(s_filter), s_filter ) == 0) {
      int startPos = size_ignore+strlen(s_filter);
      while( inputline[ startPos ] == ' ' )
        ++startPos;
      std::cout << can_filtering::config( inputline.substr( startPos ) );
    }
    else if (s_command.compare(0, strlen(s_filter_short), s_filter_short ) == 0) {
      int startPos = size_ignore+strlen(s_filter_short);
      while( inputline[ startPos ] == ' ' )
        ++startPos;
      std::cout << can_filtering::config( inputline.substr( startPos ) );
    }
    else if ( (s_command.compare(0, strlen(s_send), s_send ) == 0) || (s_command.compare(0, strlen(s_send_short), s_send_short ) == 0) ) {

      int repeatCount = 1;
      std::string s_repeat;

      // extract repeat count
      if (s_command.compare(0, strlen(s_send), s_send ) == 0)
      {
        s_repeat = s_command.substr(strlen(s_send));
      }
      else if (s_command.compare(0, strlen(s_send_short), s_send_short ) == 0)
      {
        s_repeat = s_command.substr(strlen(s_send_short));
      }
      std::istringstream istr_repeatCount( s_repeat );
      istr_repeatCount >> repeatCount;

      std::string s_type;
      istr_inputLine >> s_type;
      bool b_ext = false;
      if (!s_type.compare( "x" ) || !s_type.compare( "ext" ) || !s_type.compare( "extended" )) {
        b_ext = true;
      } else if (!s_type.compare( "s" ) || !s_type.compare( "std" ) || !s_type.compare( "standard" )) {
        b_ext = false; // though already set initially
      } else {
        b_needHelp = true;
      }
      if (!b_needHelp)
      { // parse and send the data
        int i_id=0;
        int i_db [8]={-1,-1,-1,-1,-1,-1,-1,-1}; // used for getting user input
        uint8_t ui8_db [8]={0,0,0,0,0,0,0,0}; // passed to sendUserMsg
        int i_bus=-1;

        istr_inputLine >> std::dec >> i_bus >> std::hex >> i_id  >> i_db[0] >> i_db[1] >> i_db[2] >> i_db[3] >> i_db[4] >> i_db[5] >> i_db[6] >> i_db[7];
        int i_len = 0;
        for (int i=0; i < 8; ++i)
        { // count databytes
          if (i_db[i] == -1)
            break; // no databyte at this position
          if ((i_db[i] < 0) || (i_db[i] > 255))
          {
            std::cout << "ERROR: Databyte out of range. Valid range is 0..FF" << std::endl;
            b_needHelp = true;
            break;
          }

          ui8_db[i] = i_db[i]; // copy to uint8 array
          ++i_len;
        }

        if (i_bus < 0 || i_bus > 255)
        {
          std::cout << "ERROR: invalid bus. Valid range is 0..FF" << std::endl;
          b_needHelp = true;
        }

        if (!b_needHelp)
        { // still everything fine
          std::cout << "Sending ID 0x" << std::hex << i_id << std::dec << " on bus " << i_bus << " with " << i_len << " databytes";
          for (int i=0; i<i_len; ++i) std::cout << " " << std::hex << i_db[i] << std::dec;
          std::cout << std::endl;

          for (int i=0; i<repeatCount; ++i)
            sendUserMsg(i_len, i_id, i_bus, b_ext, ui8_db, pc_serverData);

        }
      }
    } else if (!s_command.compare( s_help )) {
      b_needHelp = true; // set to wrongCommand to get help shown!
    } else {
      if (!s_command.empty())
        std::cerr << "Don't know command '" << s_command << "'. Use 'help' to get a list of commands." << std::endl;
    }

    if (b_needHelp) {
      std::cerr << "Interactive usage:" << std::endl <<
        "  " << s_enable << " " << s_monitor << std::endl <<
        "  " << s_disable << " " << s_monitor << std::endl <<
        "  " << s_enable << " " << s_log << " FILENAMEPREFIX" << std::endl <<
        "  " << s_disable << " " << s_log << " FILENAMEPREFIX" << std::endl <<
        "  " << s_on << " ... (see " << s_enable << " ...)" << std::endl <<
        "  " << s_off << " ... (see " << s_disable << " ...)" << std::endl <<
        "  " << s_filter << "|"<< s_filter_short << " ... (see \"" << s_filter << " help\")" << std::endl <<
        "  " << "send|s[<reapeat count>] s|std|standard|x|ext|extended <bus(dec)> <ID(hex)> DB1 DB2 .. DB8" << std::endl <<
        "  " << s_help << std::endl;
    }
  }
  // shouldn't reach here as thread runs in an endless loop.
  return NULL; // dummy return
}

void usage() {
  std::cout
    << "IsoAgLib CAN-Server - Virtual Host CAN-Bus." << std::endl << std::endl
    << "Usage: can_server [OPTIONS]" << std::endl << std::endl
    << "Note: When running in Interactive mode (default)" << std::endl
    << "      type 'help' for a list of interactive commands." << std::endl << std::endl
    << "Options:" << std::endl;
  for (yasper::ptr< AOption_c > const *p_opt = gp_optionsBegin; p_opt < gp_optionsEnd; ++p_opt) {
    std::cout << (*p_opt)->getUsage();
  }
}

template <>
int Option_c< OPTION_MONITOR >::doCheckAndHandle(int /*argc*/, char *argv[], int ai_pos, __HAL::server_c &ar_server) const
{
  if (!strcmp(argv[ai_pos], "--monitor")) {
    ar_server.mb_monitorMode=true;
    return 1;
  }
  return 0;  
}

template <>
std::string Option_c< OPTION_MONITOR >::doGetUsage() const
{
  return "  --monitor                  Display can traffic in console\n";
}

template <>
std::string Option_c< OPTION_MONITOR >::doGetSetting(__HAL::server_c &ar_server) const
{
  return ar_server.mb_monitorMode ? "Monitoring.\n" : "";
}

template <>
int Option_c< OPTION_LOG >::doCheckAndHandle(int argc, char *argv[], int ai_pos, __HAL::server_c &ar_server) const
{
  if (!strcmp(argv[ai_pos], "--log")) {
    if (ai_pos+1>=argc) {
      std::cerr << "error: option needs second parameter" << std::endl;
      exit(1);
    }
    ar_server.mstr_logFileBase = argv[ai_pos+1];
    ar_server.mb_logMode=true;
    return 2;
  }
  return 0;
}

template <>
std::string Option_c< OPTION_LOG >::doGetSetting(__HAL::server_c &ar_server) const
{
  std::ostringstream ostr_setting;
  if (ar_server.mb_logMode) {
    ostr_setting << "Logging to file(s), filename prefix set to " <<
      ar_server.mstr_logFileBase << std::endl;
  }
  return ostr_setting.str();
}

template <>
std::string Option_c< OPTION_LOG >::doGetUsage() const
{
  return "  --log LOG_FILE_NAME_BASE   Log can traffic into <LOG_FILE_NAME_BASE>_<bus_id>\n";
}

template <>
int Option_c< OPTION_REDUCED_LOAD_ISO_BUS_NO >::doCheckAndHandle(int argc, char *argv[], int ai_pos, __HAL::server_c &ar_server) const
{
  if (!strcmp(argv[ai_pos], "--reduced-load-iso-bus-no")) {
    ar_server.mi16_reducedLoadOnIsoBus = atoi(argv[ai_pos+1]);
    if (ai_pos+1>=argc) {
      std::cerr << "error: option needs second parameter" << std::endl;
      exit(1);
    }
    return 2;
  }
  return 0;
}

template <>
std::string Option_c< OPTION_REDUCED_LOAD_ISO_BUS_NO >::doGetSetting(__HAL::server_c &ar_server) const
{
  std::ostringstream ostr_setting;
  if (ar_server.mi16_reducedLoadOnIsoBus >= 0) {
    ostr_setting << "Reducing load on Bus no. " <<
      ar_server.mi16_reducedLoadOnIsoBus << std::endl;
  }
  return ostr_setting.str();
}

template <>
std::string Option_c< OPTION_REDUCED_LOAD_ISO_BUS_NO >::doGetUsage() const
{
  return
    "  --reduced-load-iso-bus-no BUS_NUMBER\n"
    "                             Don't send internal traffic to the physical\n"
    "                             CAN-Bus and thus reduce the load on the specified\n"
    "                             (ISOBUS) bus number. This is detected by source\n"
    "                             and destination addresses in the identifier\n";
}

template <>
int Option_c< OPTION_INTERACTIVE >::doCheckAndHandle(int /*argc*/, char *argv[], int ai_pos, __HAL::server_c &ar_server) const
{
  if (!strcmp(argv[ai_pos], "--interactive")) {
    ar_server.mb_interactive = true;
    return 1;
  }
  return 0;
}

template <>
std::string Option_c< OPTION_INTERACTIVE >::doGetSetting(__HAL::server_c &ar_server) const
{
  return ar_server.mb_interactive ? "Interactive mode set\n(Type 'help' for a list of interactive commands)\n" : "";
}

template <>
std::string Option_c< OPTION_INTERACTIVE >::doGetUsage() const
{
  return
    "  --interactive              Set interactive mode (contrarily to --productive)\n"
    "                             explicitly. (This is the default!)\n";
}

template <>
int Option_c< OPTION_PRODUCTIVE >::doCheckAndHandle(int /*argc*/, char *argv[], int ai_pos, __HAL::server_c &ar_server) const
{
  if (!strcmp(argv[ai_pos], "--productive")) {
    ar_server.mb_interactive = false;
    return 1;
  }
  return 0;
}

template <>
std::string Option_c< OPTION_PRODUCTIVE >::doGetSetting(__HAL::server_c &ar_server) const
{
  return ar_server.mb_interactive ? "" : "Productive mode set\n";
}

template <>
std::string Option_c< OPTION_PRODUCTIVE >::doGetUsage() const
{
  return "  --productive               Set productive mode (contrarily to --interactive)\n";
}

#ifndef WIN32
/*  WIN32 Platforms can't handle the daemonize syscall. The service aequivalent is not supported by can_server */
template <>
int Option_c< OPTION_DAEMON >::doCheckAndHandle(int /*argc*/, char *argv[], int ai_pos, __HAL::server_c &ar_server) const
{
  if (!strcmp(argv[ai_pos], "--daemon")) {
    ar_server.mb_daemon = true;
    return 1;
  }
  return 0;
}

template <>
std::string Option_c< OPTION_DAEMON>::doGetUsage() const
{
  return "  --daemon                   Daemonize after startup.\n";
}

template <>
std::string Option_c< OPTION_DAEMON>::doGetSetting(__HAL::server_c &ar_server) const
{
  return ar_server.mb_daemon? "Daemonized.\n" : "";
}
#endif

template <>
int Option_c< OPTION_HELP >::doCheckAndHandle(int /*argc*/, char *argv[], int ai_pos, __HAL::server_c &/*ar_server*/) const
{
  if (!strcmp(argv[ai_pos], "--help")) {
    usage();
    exit(0);
  }
  return 0;
}

template <>
std::string Option_c< OPTION_HELP >::doGetSetting(__HAL::server_c &/*ar_server*/) const
{
  return "";
}

template <>
std::string Option_c< OPTION_HELP >::doGetUsage() const
{
  return "  --help                     Print this help.\n";
}

/** Start a new file log.
 *  \return error.
 *  \retval true if an error occured.
 *  \retval false if successfull.
 */
bool newFileLog(
    __HAL::server_c *ap_server, /// server data
    size_t an_bus ) /// bus number
{
  std::ostringstream ostr_filename;
  ostr_filename << ap_server->mstr_logFileBase << "_" << std::hex << an_bus;
  yasper::ptr< __HAL::LogFile_c > p_logFile = new __HAL::LogFile_c( ostr_filename.str() );
  bool b_error = !p_logFile->getRaw();
  if (b_error) {
    if (ap_server->mb_interactive) {
      std::cerr << "Error: can not open log file " << ostr_filename.str() << "." << std::endl;
    }
  } else {
    if (ap_server->mb_interactive) {
      std::cerr << "New log file " << ostr_filename.str() << "." << std::endl;
    }
    ap_server->canBus(an_bus).m_logFile = p_logFile;
  }
  return b_error;
}

/** Close a file log.
 */
void closeFileLog(
    __HAL::server_c *ap_server, /// server data
    size_t an_bus ) /// bus number
{
  ap_server->canBus(an_bus).m_logFile = __HAL::LogFile_c::Null_s()();
}


void dumpCanMsg (uint8_t bBusNumber, uint8_t bMsgObj, canMsg_s* ps_canMsg, FILE *f_handle)
{
  if( !can_filtering::pass(
        bBusNumber,
        ps_canMsg->ui32_id,
        ps_canMsg->i32_len,
        ps_canMsg->ui8_data) )
    return;

    fprintf(f_handle, "%10d %-2d %-2d %-2d %-2d %-2d %-8x  ",
            __HAL::getTime(), bBusNumber, bMsgObj, ps_canMsg->i32_msgType, ps_canMsg->i32_len,
            (ps_canMsg->ui32_id >> 26) & 7 /* priority */, ps_canMsg->ui32_id);
    for (uint8_t ui8_i = 0; (ui8_i < ps_canMsg->i32_len) && (ui8_i < 8); ui8_i++)
      fprintf(f_handle, " %-3hx", ps_canMsg->ui8_data[ui8_i]);
    fprintf(f_handle, "\n");
    fflush(f_handle);
}
