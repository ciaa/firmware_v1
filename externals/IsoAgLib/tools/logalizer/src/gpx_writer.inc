static uint64_t s_vehiclePosTime = -1;
static uint64_t s_vehiclePosTimeStart = -1;
static double s_vehicleLat = 0;
static double s_vehicleLon = 0;


FILE *
gpxInit( const char *filename )
{
  FILE *f = fopen( filename, "w" );
  
  if( f != NULL )
  {
    fputs(
      "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n" 
      "<gpx version=\"1.0\" creator=\"ISOAgLib's Logalizer\">\n"
      " <trk>\n"
      "  <name>Replay of CAN-log</name>\n"
      "  <trkseg>\n"
      , f );
  }

  return f;
}

void
gpxClose()
{
  fputs(
    "  </trkseg>\n"
    " </trk>\n"
    "</gpx>\n",
    gs_main.m_gpxFile );
}

void handleVehiclePosition( uint64_t logTime, double lat, double lon )
{
  if( gs_main.m_gpxFile == NULL )
    return;

  if( s_vehiclePosTimeStart < 0 )
    s_vehiclePosTimeStart = logTime;

  s_vehiclePosTime = logTime;
  s_vehicleLat = lat;
  s_vehicleLon = lon;
}

void handleVehicleSpeed( uint64_t, double bearing, double speedms, double pitch, double altitude )
{
  if( gs_main.m_gpxFile == NULL )
    return;

  if( s_vehiclePosTime < 0 )
    return; // no position yet

  int msFrom0 = int( s_vehiclePosTime - s_vehiclePosTimeStart );
  const int ms = msFrom0 % 1000;
  msFrom0 /= 1000;
  const int s = msFrom0 % 60;
  msFrom0 /= 60;
  const int m = msFrom0 % 60;
  msFrom0 /= 60;
  const int h = msFrom0 % 24;
  msFrom0 /= 24;
  const int d = msFrom0 + 9;

  fprintf( gs_main.m_gpxFile,
    "   <trkpt lat=\"%.7f\" lon=\"%.7f\">\n"
    "    <ele>%.3f</ele>\n"
    "    <time>2013-11-%02dT%02d:%02d:%02d.%03dZ</time>\n"
	"    <speed>%.3f</speed>\n"
	"    <course>%.4f</course>\n"
    "   </trkpt>\n",
    s_vehicleLat, s_vehicleLon, altitude, d, h, m, s, ms, speedms, bearing );

  s_vehiclePosTime = -1; // wait for next Position
}

