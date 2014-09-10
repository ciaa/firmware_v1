/*
  streamoutput_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef STREAMOUTPUT_C_H
#define STREAMOUTPUT_C_H


#include <IsoAgLib/isoaglib_config.h>

// +X2C includes
// ~X2C

//  +X2C Class 915 : StreamOutput_c
class StreamOutput_c
{

public:

  //  Operation: operator<<
  //! Parameter:
  //! @param ui8_data:
  virtual StreamOutput_c& operator<<(uint8_t ui8_data)=0;

  void put(uint8_t aui8_data) { operator<<(aui8_data); };

  //  Operation: eof
  virtual bool eof() const=0;

  // Operation: fail
  virtual bool fail() const=0;

  // Operation: good
  virtual bool good() const=0;

	virtual ~StreamOutput_c() {};
protected:

private:

}; // ~X2C


#endif // -X2C
