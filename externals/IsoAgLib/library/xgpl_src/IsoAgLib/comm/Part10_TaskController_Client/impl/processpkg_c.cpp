/*
  processpkg_c.cpp: data object for Process-Data messages

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "processpkg_c.h"


namespace __IsoAgLib {

ProcessPkg_c::ProcessPkg_c( const CanPkg_c& arc_src, int ai_multitonInst )
  : CanPkgExt_c( arc_src, ai_multitonInst )
  , mi32_pdValue( mc_data.getInt32Data(4) )
  , men_command( IsoAgLib::ProcData::CommandType_t(mc_data[0] & 0xf) )
  , mui16_element( 0 )
  , mui16_DDI( 0 )
{
  mui16_element = static_cast<uint16_t>(mc_data[1]) << 4;
  mui16_element |= (mc_data[0] & 0xF0) >> 4;

  mui16_DDI = static_cast<uint16_t>(mc_data[3]) << 8;
  mui16_DDI |= mc_data[2];
}


ProcessPkg_c::ProcessPkg_c( IsoAgLib::ProcData::CommandType_t cmd, uint16_t element, uint16_t ddi, int32_t value )
  : CanPkgExt_c()
  , mi32_pdValue( value )
  , men_command( cmd )
  , mui16_element( element )
  , mui16_DDI( ddi )
{
  isoaglib_assert( men_command != IsoAgLib::ProcData::commandReserved1 );
  isoaglib_assert( men_command != IsoAgLib::ProcData::commandReserved2 );
  isoaglib_assert( men_command != IsoAgLib::ProcData::commandReserved3 );
  isoaglib_assert( men_command != IsoAgLib::ProcData::commandReserved4 );
  isoaglib_assert( men_command != IsoAgLib::ProcData::CommandUndefined );

  setIsoPgn( PROCESS_DATA_PGN );
  setIsoPri( 3 );
  setLen( 8 );

  setUint8Data( 0, static_cast<uint8_t>( men_command | ( ( mui16_element & 0x0f ) << 4 ) ) );
  setUint8Data( 1, static_cast<uint8_t>( mui16_element >> 4 ) );
  setUint8Data( 2, static_cast<uint8_t>( mui16_DDI ) );
  setUint8Data( 3, static_cast<uint8_t>( mui16_DDI >> 8 ) );
  setInt32Data( 4, mi32_pdValue );
}


ProcessPkg_c::ProcessPkg_c( uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 )
  : CanPkgExt_c()
  , mi32_pdValue( 0 )
  , men_command( IsoAgLib::ProcData::CommandUndefined )
  , mui16_element( 0 )
  , mui16_DDI( 0 )
{
  setIsoPgn( PROCESS_DATA_PGN );
  setIsoPri( 3 );
  setLen( 8 );

  setUint8Data( 0, d0 );
  setUint8Data( 1, d1 );
  setUint8Data( 2, d2 );
  setUint8Data( 3, d3 );
  setUint8Data( 4, d4 );
  setUint8Data( 5, d5 );
  setUint8Data( 6, d6 );
  setUint8Data( 7, d7 );
}

} // __IsoAgLib
