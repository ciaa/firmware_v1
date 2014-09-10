/*
  inputstream.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include <logenvirons.h>
#include <string>
#include <fstream>
#include <yasper.h>
#include <inputstream.h>


class InputStream_c {
// RAII Programming Idiom for file being the resource
public:
  InputStream_c(std::string str_filename);
  ~InputStream_c();
  std::istream &raw();
  bool isOpen();

private:
  // don't let the public copy it:
  InputStream_c(InputStream_c const &);
  bool isFile();
  std::ifstream mifs_in;
  std::istream *mpis_in;
};


typedef yasper::ptr< InputStream_c > PtrInputStream_t;


inline
bool
InputStream_c::isFile()
{
  return mpis_in != &std::cin;
}


inline
InputStream_c::InputStream_c( std::string str_filename ) :
  mifs_in(),
  mpis_in( (str_filename == "-")? &std::cin : &mifs_in )
{
  if (isFile()) {
    mifs_in.open(str_filename.c_str());
  }
}


inline
InputStream_c::~InputStream_c()
{
  if (isFile())
    mifs_in.close();
}


inline
std::istream &
InputStream_c::raw()
{
  return *mpis_in;
}


inline
bool
InputStream_c::isOpen()
{
  return !isFile() || mifs_in.is_open();
}


#endif
