/*
  targetfilestreaminput_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef TARGETFILESTREAMINPUT_C_H
#define TARGETFILESTREAMINPUT_C_H

#if defined(USE_BUFFERED_READ) && defined(WIN32)
  #undef USE_BUFFERED_READ
#endif


#include <IsoAgLib/isoaglib_config.h>
#include <fstream>
#include <cstdio>
#include <supplementary_driver/hal/hal_datastreams.h>
#include <string>

// +X2C includes
// ~X2C

//  +X2C Class 915 : FileTargetFileStreamInput_c
#ifndef USE_BUFFERED_READ
class TargetFileStreamInput_c :public STL_NAMESPACE::ifstream
#else
class TargetFileStreamInput_c // :public STL_NAMESPACE::ifstream
#endif
{

public:

	//! open a input stream
	bool open( STD_TSTRING& filename, FileMode_t at_mode )
		{ return open( filename.c_str(), at_mode );};
	//! open a input stream
	bool open( const TCHAR* filename, FileMode_t at_mode );
	//! close a input stream
	void close();

	//  Operation: operator>>
  //! Parameter:
  //! @param ui8_data:
  virtual TargetFileStreamInput_c& operator>>(uint8_t &ui8_data);

  //  Operation: eof
  //  b_eofReached is set to true when peek() returns EOF in operator>>: nothing more to read
	//  b_eofReached is initialized to false in open()
	virtual bool eof() const;

  TargetFileStreamInput_c() : fileDescr(NULL), b_eofReached(false), ui16_bytesInBuffer(0), ui16_currentReadIndexInBuffer(0)
  {}

  virtual ~TargetFileStreamInput_c() { close(); }

private:

  FILE *fileDescr;

  #ifndef USE_BUFFERED_READ
  #define cui16_bufSize 256
  #else
  static const uint16_t cui16_bufSize = 256;
  #endif

  char ch_buf[cui16_bufSize];

  //  does next get() fail?
  bool b_eofReached;

  uint16_t ui16_bytesInBuffer;
  uint16_t ui16_currentReadIndexInBuffer;
}; // ~X2C

// TargetFileStreamInput_c & operator>> (TargetFileStreamInput_c &, uint8_t &ui8_data);


#endif // -X2C
