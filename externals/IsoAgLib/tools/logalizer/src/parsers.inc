#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif


std::pair< int, PtrDataFrame_t >
parseLogLineCANMon( std::string const &acr_line ) //RX        4     1   CFE5182x| 98  2B  97  6F  FD  00  FF  EB
{
  unsigned arru_d[8];
  int iA, iB;
  int parsed_count = sscanf(
      acr_line.c_str(),
      "%*s %i %*i %x%*s%x %x %x %x %x %x %x %x",
      &iA, &iB, arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7);
  int i_result = (parsed_count >= 2) ? 0 : -1;
  if (i_result < 0)
    return std::make_pair( i_result, PtrDataFrame_t(0) );

  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + size_t(parsed_count-2), back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(iA, iB, vec_data);
  return std::make_pair( i_result, t_ptrFrame );
}


std::pair< int, PtrDataFrame_t >
parseLogLineJohnDeere( std::string const &acr_line ) // "r Xtd 2 1CAAF883 8 20 03 03 02 00 5C 5C FF 0   0 0060846488      17920  ....\\. "
{
  int iA, iB;
  unsigned arru_d[8];
  int parsed_count = sscanf(
      acr_line.c_str(),
      "r Xtd %*i %x %*i %x %x %x %x %x %x %x %x %*i   %*i %i      17920  %*s ",
      &iB, arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7, &iA);
  int i_result = parsed_count >= 2 ? 0 : -1;
  if (i_result < 0)
    return std::make_pair( i_result, PtrDataFrame_t(0) );
  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + size_t(parsed_count-2), back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(iA, iB, vec_data);
  return std::make_pair( i_result, t_ptrFrame );
}


std::pair< int, PtrDataFrame_t >
parseLogLineCanServer( std::string const &acr_line ) // "104916846 0 1 1 3 6 18eafffe   0   ee  0   0   0   0   0   0"
{
  long long iA;
  int iB;
  unsigned arru_d[8];
  int parsed_count = sscanf(
      acr_line.c_str(),
      "%lli %*i %*i %*i %*i %*i %x %x %x %x %x %x %x %x %x",
      &iA, &iB, arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7);
  int i_result = parsed_count >= 2 ? 0 : -1;
  if (i_result < 0)
    return std::make_pair( i_result, PtrDataFrame_t(0) );

  size_t t_sizeData = size_t(parsed_count-2);
  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + t_sizeData, back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(iA, iB, vec_data);
  return std::make_pair( i_result, t_ptrFrame );
}


std::pair< int, PtrDataFrame_t >
parseLogLineCANoe( std::string const &acr_line ) // "  18.9530 1  0CFE4980x        Rx   d 8 00 00 FF FF FF FF FF FF "
{
  float f1;
  int iB, iDb;
  unsigned arru_d[8];
  int parsed_count = sscanf(
      acr_line.c_str(),
      " %f %*i %xx %*s %*s %u %x %x %x %x %x %x %x %x",      
      &f1, &iB, &iDb, arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7);
  int i_result = parsed_count == (3+iDb) ? 0 : -1;
  if (i_result < 0)
    return std::make_pair( i_result, PtrDataFrame_t(0) );
  uint64_t const cui64_time = uint64_t(f1*1000);
  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + size_t(iDb), back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(cui64_time, iB, vec_data);
  return std::make_pair( i_result, t_ptrFrame );
}


std::pair< int, PtrDataFrame_t >
parseLogLineA1ASCII( std::string const &acr_line ) // "m e 0x0cf00203 8  0xff 0x00 0x00 0xfa 0xff 0xf0 0x18 0xff       446270"
{
  char ch_baseOrExtended = '.';
  int iA, iB, iDb;
  unsigned arru_d[8];
  int parsed_count = sscanf(
      acr_line.c_str(),
      "m %c 0x%x %u 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x %i",
      &ch_baseOrExtended, &iA, &iDb, arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7, &iB);
  int i_result = parsed_count == (4+iDb) ? 0 : -1;
  if (i_result < 0)
    return std::make_pair( i_result, PtrDataFrame_t(0) );
  bool const cb_extendedFrameFormat = 'e' == ch_baseOrExtended;
  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + size_t(iDb), back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(iB, iA, vec_data, cb_extendedFrameFormat);
  return std::make_pair( i_result, t_ptrFrame );
}



std::pair< int, PtrDataFrame_t >
parseLogLineRte( std::string const &acr_line ) // "[0] HW             97.41  X   9f80182 8 67 34 b0 1c 54 01 e6 06"
{
  int iB, iDb = 0; // "%i* %x
  unsigned arru_d[8];
  uint64_t iA;
  static int const sci_minimal_expected_parse_count = 4;
  int parsed_count = 0;
  static int remembered_mode = 0;

  for (int current_mode = remembered_mode; ; ++current_mode) {
    switch (current_mode) {
    case 0:
      break;
    case 1:
      parsed_count = sscanf(
          acr_line.c_str(),
          "%*s %*s %Li.%*s %*i.%*s X %x %u %x %x %x %x %x %x %x %x",
          &iA, &iB, &iDb, arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7);
      if (parsed_count <= -1 || sci_minimal_expected_parse_count <= parsed_count)
        remembered_mode = current_mode;
      break;
    case 2:
      parsed_count = sscanf(
          acr_line.c_str(),
          "%*s %Li.%*s %*i.%*s X %x %u %x %x %x %x %x %x %x %x",
          &iA, &iB, &iDb, arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7);
      if (remembered_mode)
        ;
      else if (-1 < parsed_count && parsed_count < sci_minimal_expected_parse_count)
        exit_with_error("wrong rte-log format. use -a for absolute timestamps!");
      else
        remembered_mode = current_mode;
      break;
    }
    if (remembered_mode)
      break; // repeat until remembered_mode is set
  }
  int i_result = parsed_count == (3+iDb) ? 0 : -1;
  if (i_result < 0)
    return std::make_pair( i_result, PtrDataFrame_t(0) );

  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + size_t(iDb), back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(iA, iB, vec_data);
  return std::make_pair( i_result, t_ptrFrame );
}


std::pair< int, PtrDataFrame_t >
parseLogLineRte2( std::string const &acr_line )
{
    static uint64_t sui64_rteTime = 0;
    int big;
    char *cursor;
    size_t byte;

    /* skip over any initial HW */
    for (cursor = const_cast<char *>(acr_line.c_str()); *cursor != 0 && !isspace (*cursor); cursor++)
        /* empty */;

    /*
     * must specify base 10 because of leading 0's
     */
    big = strtol (cursor, &cursor, 10);
    if (*cursor != '.') {
        fprintf (stderr, "unexpected terminator %c (%d): %s\n", *cursor, *cursor, acr_line.c_str());
        return std::make_pair( -4, PtrDataFrame_t(0) );
    }
    long ret = strtol (&cursor[1], &cursor, 10);
    (void)ret;
    while (isspace (*cursor))
        cursor++;
    if (*cursor == 'X')
        cursor++;
    else {
        fprintf (stderr, "unexpected field %c\n", *cursor);
        return std::make_pair( -5, PtrDataFrame_t(0) );
    }

    sui64_rteTime += big;
    uint64_t const cui64_time = sui64_rteTime;
    uint32_t const cui32_identifier = strtol (cursor, &cursor, 16);
    size_t t_sizeData = uint8_t (strtol (cursor, &cursor, 0));


    if (t_sizeData > 8) {
        fprintf (stderr, "protocol violation at time %lld: dlc of %d too long! (forcing to 8)\n", cui64_time, t_sizeData);
        t_sizeData = 8;
    }

    std::vector< uint8_t > vec_data(t_sizeData);

    for (byte = 0; byte < 8; byte++) {
        if (byte >= t_sizeData)
            break;

        vec_data[byte] = uint8_t (strtol (cursor, &cursor, 16));
    }

    return std::make_pair( 0, new DataFrame_c( cui64_time, cui32_identifier, vec_data ) );
}


std::pair< int, PtrDataFrame_t >
parseLogLineTrc( std::string const &acr_line ) // "    13)       116.6  Rx     18EF808B  8  12 15 15 15 15 15 15 15"
{
  unsigned arru_d[8];
  int iA, iB, iDb;
  int parsed_count = sscanf(
      acr_line.c_str(),
      " %*s %u.%*u %*s %x %u %x %x %x %x %x %x %x %x",
      &iA, &iB, &iDb, arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7);
  int i_result = parsed_count == (3+iDb) ? 0 : -1;
  if (i_result < 0)
    return std::make_pair( i_result, PtrDataFrame_t(0) );

  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + size_t(iDb), back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(iA, iB, vec_data);
  return std::make_pair( i_result, t_ptrFrame );
}


std::pair< int, PtrDataFrame_t >
parseLogLineTrc2( std::string const &acr_line ) // "    13)       116.6 1 Rx     18EF808B 80 8  12 15 15 15 15 15 15 15"
{
  unsigned arru_d[8];
  int iA, iB, iDb;
  int parsed_count = sscanf(
      acr_line.c_str(),
      " %*s %u.%*u %*u %*s %x %*x %u %x %x %x %x %x %x %x %x",
      &iA, &iB, &iDb, arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7);
  int i_result = parsed_count == (3+iDb) ? 0 : -1;
  if (i_result < 0)
    return std::make_pair( i_result, PtrDataFrame_t(0) );

  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + size_t(iDb), back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(iA, iB, vec_data);
  return std::make_pair( i_result, t_ptrFrame );
}


std::pair< int, PtrDataFrame_t >
parseLogLineJrf( std::string const &acr_line ) // "41.19,0CFFFF2A,77,04,00,00,7D,00,64,FF"
{
  float fA;
  int iB;
  unsigned arru_d[8];
  int parsed_count = sscanf(
      acr_line.c_str(),
      "%f,%x,%x,%x,%x,%x,%x,%x,%x,%x",
      &fA, &iB, arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7);
  int i_result = parsed_count >= 2 ? 0 : -1;
  if (i_result < 0)
    return std::make_pair( i_result, PtrDataFrame_t(0) );

  uint64_t const cui64_time = uint64_t(fA*1000);
  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + size_t(parsed_count-2), back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(cui64_time, iB, vec_data);
  return std::make_pair( i_result, t_ptrFrame );
}


std::pair< int, PtrDataFrame_t >
defaultParseLogLine( std::string const &acr_line )
{
  (void)acr_line;
  std::pair< int, PtrDataFrame_t > result = std::make_pair(
      -1,
      PtrDataFrame_t(0) );
  exit_with_error("Unknown Log-Type!");
  return std::make_pair(1, result.second); // return just to satisfy compiler. exit_with_error will exit anyway ;)
}


std::pair< int, PtrDataFrame_t >
parseLogLineSocketCAN( std::string const &acr_line ) // (1321953173.064559) can1 10844D4C#FF7F737F73FFFFFF ( candump -l format )
{
#if DEBUG
  std::cout << acr_line << std::endl;
#endif

  unsigned int d[8];
  unsigned int t_s, t_us;
  unsigned int id;

  const int cnt = sscanf(
                    acr_line.c_str(),
                    "(%d.%d) can%*d %x#%02x%02x%02x%02x%02x%02x%02x%02x",
                    &t_s, &t_us, &id, d, d+1, d+2, d+3, d+4, d+5, d+6, d+7 );

  const int r = (cnt >= 3) ? 0 : -1;
  if (r < 0)
    return std::make_pair(r, PtrDataFrame_t(0));

  const uint64_t t_ms = uint64_t((t_s*1000)+(t_us/1000));
  std::vector<uint8_t> vec_data;
  copy(d, d + size_t(cnt-3), back_inserter(vec_data));

  PtrDataFrame_t t_ptrFrame = new DataFrame_c(t_ms, id, vec_data);
  return std::make_pair(r, t_ptrFrame);
}


std::pair< int, PtrDataFrame_t >
parseLogLineWTK( std::string const &acr_line ) // "0000.376 can r 18E6FFF1  8  21 00 FF FF 00 00 00 FF  0"
{
  float f1;
  int iID, inumDb, iX;
  unsigned arru_d[8];
  int parsed_count = sscanf(
      acr_line.c_str(),
      "%f %*s %*s %x  %u  %x %x %x %x %x %x %x %x  %u",      
      &f1,
      &iID, 
      &inumDb, 
      arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7,
      &iX);
  int i_result = parsed_count == (4+inumDb) ? 0 : -1;
  if (i_result < 0)
    return std::make_pair( i_result, PtrDataFrame_t(0) );
  uint64_t const cui64_time = static_cast<uint64_t>(f1*1000);
  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + size_t(inumDb), back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(cui64_time, iID, vec_data);
  return std::make_pair( i_result, t_ptrFrame );
}

std::pair< int, PtrDataFrame_t >
parseLogLineKvaserMemorator( std::string const &acr_line ) // "0.33198,1,cfffff0,4,8,55,7d,7d,7d,7d,ff,ff,ff,1,2014-05-05 15:01:08"
{
  float f1;
  int iID, inumDb;
  unsigned arru_d[8];
  int parsed_count = sscanf(
      acr_line.c_str(),
      "%f,%*d,%x,%*d,%d,%x,%x,%x,%x,%x,%x,%x,%x%*s",      
      &f1,
      &iID,
      &inumDb,
      arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7);
  int i_result = parsed_count == (3+inumDb) ? 0 : -1;
  if (i_result < 0)
    return std::make_pair( i_result, PtrDataFrame_t(0) );
  uint64_t const cui64_time = static_cast<uint64_t>(f1*1000);
  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + size_t(inumDb), back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(cui64_time, iID, vec_data);
  return std::make_pair( i_result, t_ptrFrame );
}


std::pair< int, PtrDataFrame_t >
parseLogLineCsv( std::string const &acr_line ) // "0xCFE46F0*,54.6857,FF,FF,FF,FF,00,FF,FF,FF"
{
  float f1;
  int iID;
  unsigned arru_d[8];
  int parsed_count = sscanf(
      acr_line.c_str(),
      "%x*,%f,%x,%x,%x,%x,%x,%x,%x,%x",
      &iID,
      &f1,
      arru_d, arru_d+1, arru_d+2, arru_d+3, arru_d+4, arru_d+5, arru_d+6, arru_d+7);
  const int inumDb = parsed_count - 2;
  if( inumDb < 0 )
    return std::make_pair( -1, PtrDataFrame_t(0) );

  uint64_t const cui64_time = static_cast<uint64_t>(f1*1000);
  std::vector< uint8_t > vec_data;
  copy(arru_d, arru_d + size_t(inumDb), back_inserter(vec_data));
  PtrDataFrame_t t_ptrFrame = new DataFrame_c(cui64_time, iID, vec_data);
  return std::make_pair( 0, t_ptrFrame );
}

