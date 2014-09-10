/*
  filestreamoutput_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "filestreamoutput_c.h"
#include "stdio.h"
#include <string>


bool
FileStreamOutput_c::open (STD_TSTRING& filename, FileMode_t at_mode)
{
  bool b_result = c_targetHandle.open( filename, at_mode );
  if (b_result) str_openedFile = filename;
  return b_result;
}


bool
FileStreamOutput_c::open (const TCHAR* filename, FileMode_t at_mode)
{
  bool b_result = c_targetHandle.open( filename, at_mode);
  if (b_result) str_openedFile = filename;
  return b_result;
}


bool
FileStreamOutput_c::close (bool b_deleteFile, bool b_sync)
{
  if (b_sync)
  { // get path name from file name
    const STD_TSTRING::size_type size = str_openedFile.rfind('/');
	STD_TSTRING str_path; 
    if (STD_TSTRING::npos == size)
      // no path in file name
      str_path = TEXT(".");
    else
      str_path = str_openedFile.substr(0, size);

    // sync file and directory entry
    c_targetHandle.close(str_path.c_str());
  }
  else
    c_targetHandle.close(NULL);
  
  if (b_deleteFile) {
#if (defined(WIN32) && defined(UNICODE)) || defined(WINCE)
    WCHAR tmpFileName[MAX_PATH];
    mb2wc( (LPSTR)str_openedFile.c_str(), tmpFileName, MAX_PATH );
    return DeleteFileW( tmpFileName );
#else
    return (remove (str_openedFile.c_str()) == 0);
#endif
  }
  return true; // success
}

StreamOutput_c&
FileStreamOutput_c::operator<<(uint8_t ui8_data)
{
  c_targetHandle.operator<<( ui8_data );
  return *this;
}
