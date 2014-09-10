/*
  processpkg_c.h: data object for Process-Data messages

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef PROCESSPKG_C_H
#define PROCESSPKG_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/canpkgext_c.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/iprocdata.h>

namespace __IsoAgLib {

class ProcessPkg_c : public CanPkgExt_c
{
public:
  ProcessPkg_c( const CanPkg_c& arc_src, int ai_multitonInst = 0 );
  ProcessPkg_c( IsoAgLib::ProcData::CommandType_t, uint16_t element, uint16_t ddi, int32_t value );
  ProcessPkg_c( uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t );

  ~ProcessPkg_c() {}

public:
  int32_t mi32_pdValue;

  IsoAgLib::ProcData::CommandType_t men_command;
  uint16_t mui16_element;
  uint16_t mui16_DDI;

private:
  /** not copyable : copy constructor/operator only declared, not defined */
  ProcessPkg_c( const ProcessPkg_c& );
  ProcessPkg_c& operator=( const ProcessPkg_c& );
};

}

#endif
