/*
  can_messenger.cpp: Testtool to inject CAN packages in a running
    can_server_bus.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/* include headers for the needed drivers */
#include <IsoAgLib/driver/system/isystem_c.h>
#include <IsoAgLib/driver/can/impl/canio_c.h>
#include <IsoAgLib/driver/can/impl/canpkg_c.h>
#include <IsoAgLib/scheduler/ischeduler_c.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

class cmdline_c
{
public:
  cmdline_c ()
  : i_repeat (1)
  , i_databytes (0)
  , i_id (0)
  , i_channel (0)
  , i_period (0)
  , b_ext (false)
  {}

  int i_repeat;
  int i_databytes;
  int i_id;
  int i_channel;
  int i_period;
  int b_ext;
  uint8_t pui8_databytes [8];

  void parse (int argc, char *argv[]);

private:

  int ahextoi (char* str);
  int databytes (char* src, uint8_t* dst);

  void usage_and_exit(int ai_errorCode) const;
};


int cmdline_c::ahextoi (char* str)
{
  int value=0;
  char x;
  while ((x = *str++) != 0x00)
  {
    if (x == 'x') value=0; /// entering HEX mode here
    else if ((x >= '0') && (x <= '9')) value = (value << 4) | (x - '0');
    else if ((x >= 'A') && (x <= 'F')) value = (value << 4) | (x - 'A' + 10);
    else if ((x >= 'a') && (x <= 'f')) value = (value << 4) | (x - 'a' + 10);
    else { printf ("Hex number error!\n"); usage_and_exit(1); }
  }
  return value;
}


int cmdline_c::databytes (char* src, uint8_t* dst)
{
  int bytes=0;
  bool first=true;
  char x;
  while ((x = *src++) != 0x00)
  {
    int value;
    /**/ if ((x >= '0') && (x <= '9')) value = (x - '0');
    else if ((x >= 'A') && (x <= 'F')) value = (x - 'A' + 10);
    else if ((x >= 'a') && (x <= 'f')) value = (x - 'a' + 10);
    else { value=0; printf ("Hex number error!\n"); usage_and_exit(1); }
    if (first)
    {
      *dst = value << 4;
      first = false;
    }
    else
    {
      *dst++ |= value;
      bytes++;
      first = true;
    }
  }

  if (!first)
  {
    printf ("last data byte incomplete!!\n");
    usage_and_exit(1);
  }
  return bytes;
}


void cmdline_c::parse (int argc, char *argv[])
{
  char nextup [] = "  ";
  if (argc < 2) usage_and_exit(1);
  for (int i=1; i<argc; i++)
  {
    if (nextup[2-1] == ' ')
    {
      if (argv[i][1-1] != '-')
      { // error
        printf ("Specify parameter type please!\n");
        usage_and_exit(1);
      }
      nextup[2-1] = argv[i][2-1];
      continue;
    }
    else
    {
      switch (nextup[2-1])
      {
        case 'i': i_id = ahextoi(argv[i]); break;
        case 'n': i_repeat = atoi(argv[i]); break;
        case 'c': i_channel = atoi (argv[i]); break;
        case 's':
        case 'd': i_databytes = databytes(argv[i], pui8_databytes); break;
        case 'p': i_period = atoi (argv[i]); break;
        case 'x': b_ext = true; i--; break;
        default: printf ("Unsupported parameter!\n"); usage_and_exit(1); break;
      }
      nextup[2-1] = ' '; // free to await next parameter-pair
      continue;
    }
  }
  if (nextup[2-1] != ' ')
  { // didn't finish parameter-pair ==> error
    printf ("Incomplete parameter-type\n");
    usage_and_exit (1);
  }
}


void cmdline_c::usage_and_exit (int ai_errorCode) const
{
  printf ("\nCommandline-parameters are:\n   -i <CAN-Identifier as HEX>\n");
  printf ("   -n <Repeat-Count>\n");
  printf ("   -c <CAN-Channel>\n");
  printf ("   -d <Databytes (up to 8) as HEX> (for compatibility also -s can be used)\n");
  printf ("   -p <Period in ms>\n");
  printf ("   -x     (use eXtended Identifier)\n");
  printf ("\n Example: can_messenger -x -n 1 -c 0 -i 1ceafffe -d a1b2c3d4e5f6affe\n\n");

  exit (ai_errorCode);
}


using namespace IsoAgLib;
using namespace __IsoAgLib;

int main( int argc, char *argv[] )
{
  cmdline_c params;

  params.parse (argc, argv);

  printf ("Sending on CAN-Bus %d %d times with a period of %d: ID=%x [%d databytes:] ", params.i_channel, params.i_repeat, 
                                                                                        params.i_period,  params.i_id,
                                                                                        params.i_databytes);
  for (int i=0; i<params.i_databytes; i++) printf (" %02x", params.pui8_databytes[i]);
  printf ("\n");

  // Initialize ISOAgLib
  getISchedulerInstance().init();

  // Initialize CAN-Bus
  getCanInstance().init (params.i_channel, 250 ); // CAN-Bus (with defaulting 250 kbit)

  CanPkg_c pkg;

  pkg.setIdent(params.i_id, (params.b_ext ? iIdent_c::ExtendedIdent : iIdent_c::StandardIdent));
  pkg.setDataFromString(0, params.pui8_databytes, params.i_databytes);

  for( int i = 0; i < params.i_repeat; ++i ) {
    getCanInstance() << pkg;
    if( params.i_period && ( 1 != params.i_repeat ) ) {
#ifdef WIN32
      Sleep ( params.i_period ); // won't be too accurate though due to bad Windows Sleep-capability.
#else
      usleep( params.i_period * 1000 );
#endif
    }
  }

  return 0;
}
