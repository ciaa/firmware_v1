/*
  volatilememorywithsize_c.h


  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef VOLATILEMEMORYWITHSIZE_C_H
#define VOLATILEMEMORYWITHSIZE_C_H



#include <supplementary_driver/driver/datastreams/streaminput_c.h>



class VolatileMemoryWithSize_c : public StreamInput_c
{

public:

	VolatileMemoryWithSize_c(const uint8_t* apcui8_data, int32_t ai32_available) :
  			mpcui8_data ( apcui8_data ),
  			mi32_available(ai32_available)
  			{};
  
  VolatileMemoryWithSize_c(const VolatileMemoryWithSize_c& rrefc_src) : 
  			StreamInput_c (), 
  			mpcui8_data ( rrefc_src.mpcui8_data ), 
  			mi32_available(rrefc_src.mi32_available)
  			{};

	

  //  Operation: operator >>
  virtual StreamInput_c& operator >> (uint8_t& ui8_data);

  //  Operation: eof
  virtual bool eof() const;

  const VolatileMemoryWithSize_c& operator = (const VolatileMemoryWithSize_c& rrefc_src);

protected:
  static const uint8_t scui8_valueWhenEOF = 0xFF;

  //  Attribute: p_volatileMemory
  const uint8_t* mpcui8_data;
  int32_t 	mi32_available;

};


	  

#endif
