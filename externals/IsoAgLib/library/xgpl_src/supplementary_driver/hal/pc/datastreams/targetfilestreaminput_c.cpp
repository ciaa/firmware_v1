/*
  targetfilestreaminput_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "targetfilestreaminput_c.h"
#ifndef USE_BUFFERED_READ
  #include <cstdio>
  #include <fstream>
#else
  #include <fcntl.h>
#endif

#if __GNUC__ < 3
  #define MACRO_IOS ios
#else
  #define MACRO_IOS STL_NAMESPACE::ios_base
#endif

using namespace std;

//! open a input stream
bool TargetFileStreamInput_c::open( const TCHAR* filename, FileMode_t at_mode )
{
#ifndef USE_BUFFERED_READ

  b_eofReached = false;

  if ( ( at_mode & StreamOut ) != 0 ) return false;

  MACRO_IOS::openmode mode = MACRO_IOS::in;

  if ( ( at_mode & StreamAte   )  != 0 ) mode = MACRO_IOS::openmode( mode | MACRO_IOS::ate    );
  if ( ( at_mode & StreamApp   )  != 0 ) mode = MACRO_IOS::openmode( mode | MACRO_IOS::app    );
  if ( ( at_mode & StreamTrunc )  != 0 ) mode = MACRO_IOS::openmode( mode | MACRO_IOS::trunc  );
  if ( ( at_mode & StreamBinary ) != 0 ) mode = MACRO_IOS::openmode( mode | MACRO_IOS::binary );


  static_cast<ifstream*>(this)->open( filename, mode );

  if ( ( static_cast<ifstream*>(this)->good()    )
    && ( static_cast<ifstream*>(this)->is_open() ) ) return true;
  else
    return false;

#else

  if ( ( at_mode & StreamOut ) != 0 ) return false;

  if ( ( at_mode & StreamTrunc ) != 0 )
  {
    fileDescr = fopen(filename, "w"); // Truncate  file  to  zero length
    if (NULL == fileDescr)
      return false;

    fclose(fileDescr);
    fileDescr = NULL;
  }

  if ( ( at_mode & StreamApp ) != 0 )
    fileDescr = fopen(filename, "a+"); // open for reading and appending, file position for reading at the begin
  else
    fileDescr = fopen(filename, "r"); // open for reading

  if (NULL == fileDescr)
    return false;

  if ( ( at_mode & StreamAte ) != 0 )
    fseek(fileDescr, 0, SEEK_END); // file position for reading at end

  b_eofReached = feof(fileDescr);

  return true;
#endif
};


//  Operation: operator>>
//! Parameter:
//! @param ui8_data:
TargetFileStreamInput_c& TargetFileStreamInput_c::operator>>(uint8_t &ui8_data)
{
#ifndef USE_BUFFERED_READ

  ifstream* isp_tmp = static_cast<ifstream*>(this);

  ui8_data = isp_tmp->get();

  // check if next get returns EOF: nothing more to read
  if ((isp_tmp->peek() == EOF) || (isp_tmp->eof()))
    b_eofReached = true;

#else

  if (ui16_currentReadIndexInBuffer >= ui16_bytesInBuffer)
  {
    ui16_bytesInBuffer = fread(ch_buf, 1, cui16_bufSize, fileDescr);
    ui16_currentReadIndexInBuffer = 0;
    b_eofReached = feof(fileDescr);
    if (!b_eofReached)
    {
      ungetc(fgetc(fileDescr), fileDescr); // do peek
      // stream state is still EOF after fgetc/ungetc in case fgetc yields EOF
      b_eofReached = feof(fileDescr);
    }
  }

  if (ui16_currentReadIndexInBuffer < ui16_bytesInBuffer)
  {
    ui8_data = ch_buf[ui16_currentReadIndexInBuffer];
    ui16_currentReadIndexInBuffer++;
  }
#endif

  return *this;
}

bool
TargetFileStreamInput_c::eof() const
{
#ifndef USE_BUFFERED_READ
  return b_eofReached | static_cast<const ifstream*>(this)->eof();
#else
  if (!fileDescr)
    return true;

  return (b_eofReached && (ui16_currentReadIndexInBuffer >= ui16_bytesInBuffer));
#endif
}

void
TargetFileStreamInput_c::close()
{
#ifndef USE_BUFFERED_READ
  static_cast<ifstream*>(this)->close();
#else
  if (fileDescr)
    fclose(fileDescr);

  fileDescr = NULL;
#endif
}

