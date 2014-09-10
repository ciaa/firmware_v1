/*
  inputs_c.h - header for the inputs management object

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef INPUTS_C_H
#define INPUTS_C_H


namespace __IsoAgLib {

/**
  Class for management of Inputs.
  Currently unsused, but should be initialized/closed nevertheless!
  */
class Inputs_c {
public:
  void init() {}
  void close() {}

private:
  // only for singleton
  Inputs_c() {}
  ~Inputs_c() {}

private:
  friend Inputs_c &getInputsInstance();
};


Inputs_c &
getInputsInstance();


} // __IsoAgLib

#endif
