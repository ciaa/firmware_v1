/*
  filestreaminput_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef FILESTREAMINPUT_C_H
#define FILESTREAMINPUT_C_H


#include <IsoAgLib/isoaglib_config.h>
#include "streaminput_c.h"
#include <supplementary_driver/hal/hal_datastreams.h>
#include <stdio.h>



//  +X2C Class 915 : FileStreamInput_c
class FileStreamInput_c : public StreamInput_c
{

public:

  //  Operation: open
  //! open an input stream
  bool open (STD_TSTRING& filename, FileMode_t at_mode);

  //  Operation: open
  //! open an input stream
  bool open (const TCHAR* filename, FileMode_t at_mode);

  //  Operation: close
  //! close an input stream
  bool close (bool b_deleteFile=false);

  //  Operation: operator>>
  //! read from input stream
  virtual StreamInput_c& operator>>(uint8_t& ui8_data);

  //  Operation: eof
  //! check for end of input stream
  virtual bool eof() const { return c_targetHandle.eof(); };

  //  Operation: getFileName
  //! get file name
  const STD_TSTRING& getFileName() const { return str_openedFile; };

private:
  TargetFileStreamInput_c c_targetHandle;

  STD_TSTRING str_openedFile;
}; // ~X2C


#endif // -X2C
