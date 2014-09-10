/*
  volatilememory_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef VOLATILEMEMORY_C_H
#define VOLATILEMEMORY_C_H


#include <supplementary_driver/driver/datastreams/streaminput_c.h>



class VolatileMemory_c : public StreamInput_c
{

public:

  //  Operation: VolatileMemory_c
  VolatileMemory_c(const uint8_t* ap_volatileMemory) : p_volatileMemory ( ap_volatileMemory ) {};

  VolatileMemory_c(const VolatileMemory_c& acrc_src) : StreamInput_c (), p_volatileMemory ( acrc_src.p_volatileMemory ) {};

  //  Operation: operator >>
  virtual StreamInput_c& operator >> (uint8_t& ui8_data);

  //  Operation: eof
  virtual bool eof() const;

  const VolatileMemory_c& operator = (const VolatileMemory_c& acrc_src);

protected:

private:

  //  Attribute: p_volatileMemory
  const uint8_t* p_volatileMemory;

};


#endif
