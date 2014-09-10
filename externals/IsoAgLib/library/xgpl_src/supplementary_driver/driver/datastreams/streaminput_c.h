/*
  streaminput_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef STREAMINPUT_C_H
#define STREAMINPUT_C_H

#include <IsoAgLib/util/convert.h>

/** Abstract base class for a streaming input source.
  * Realized by FileStreamInput_c for file readout,
  * and VolatileMemory_c for memory buffer readout.
  * iStream_c is defining a superset of StreamInput_c.
  */
class StreamInput_c
{
public:
  // Abstract interface definition

  //! Stream input operation.
  //! @param ui8_data Referenze to store read byte to.
  virtual StreamInput_c& operator>>(uint8_t& ui8_data)=0;

  //! End of file
  virtual bool eof() const=0;

  //! Virtual destructor to prevent from warning.
  virtual ~StreamInput_c() {};

  // For convenience some transparent interface extensions.

  //! Read one byte
  uint8_t get( void ) { uint8_t byte; *this >> byte; return byte; };

  //! Stream input of int8_t
  StreamInput_c& operator>>(int8_t& i8_data) {
    i8_data = convert_n::castI( get() ); return *this;
  }

  //! Stream input of uint16_t
  StreamInput_c& operator>>(uint16_t& ui16_data) {
    uint8_t l,h; *this >> l >> h;
    ui16_data = uint16_t((uint16_t(h) << 8) | uint16_t(l));
    return *this;
  }

  //! Stream input of int16_t
  StreamInput_c& operator>>(int16_t& i16_data) {
    uint16_t ui16; *this >> ui16;
    i16_data = convert_n::castI( ui16 ); return *this;
  }

#if defined(_BASIC_STRING_H) || defined(__BASTRING__)
  //! Read string up to given size or up to eof() if less.
  void get( STL_NAMESPACE::string& c_str, int i_size ) {
    for (;(i_size > 0) && (!eof());--i_size) c_str.push_back( get() );
  }

  //! Read string up to eof
  void get( STL_NAMESPACE::string& c_str ) {
    while (!eof()) c_str.push_back( get() );
  }
#endif
};

#endif // STREAMINPUT_C_H

