/*
  filestreamoutput_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef FILESTREAMOUTPUT_C_H
#define FILESTREAMOUTPUT_C_H


#include <IsoAgLib/isoaglib_config.h>
#include "streamoutput_c.h"
#include <supplementary_driver/hal/hal_datastreams.h>
#include <string>

// +X2C includes
// ~X2C

//  +X2C Class 915 : FileStreamOutput_c
class FileStreamOutput_c : public StreamOutput_c
{

public:

  //  Operation: open
  //! open an output stream
  bool open (STD_TSTRING& filename, FileMode_t at_mode);

  //  Operation: open
  //! open an output stream
  bool open (const TCHAR* filename, FileMode_t at_mode);

  //  Operation: close
  //! close an output stream
  bool close (bool b_deleteFile=false, bool b_sync=false);

  //  Operation: operator<<
  //! write to output stream
  virtual StreamOutput_c& operator<<(uint8_t ui8_data);

  //  Operation: eof
  //! check for end of output stream
  virtual bool eof() const { return c_targetHandle.eof(); };

  //  Operation: fail
  //! check for failure of output stream
  virtual bool fail() const { return c_targetHandle.fail(); };

  //  Operation: good
  //! check if output stream is good
  virtual bool good() const { return c_targetHandle.good(); };

private:
  TargetFileStreamOutput_c c_targetHandle;

  STD_TSTRING str_openedFile;
}; // ~X2C

#endif // -X2C
