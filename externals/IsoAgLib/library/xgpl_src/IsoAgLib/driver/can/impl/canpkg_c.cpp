/*
  canpkg_c.cpp: source for CanPkg_c, the encapsulation of CAN
     telegrams

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <IsoAgLib/driver/can/impl/canpkg_c.h>

namespace __IsoAgLib {


CanPkg_c::CanPkg_c() 
  : mc_data()
  , mi32_time( 0 )
  , mc_ident( 0, Ident_c::ExtendedIdent )
  , mui8_len( 0 )
{
}


CanPkg_c::~CanPkg_c()
{
}


CanPkg_c::CanPkg_c( const CanPkg_c& arc_src )
  : mc_data( arc_src.mc_data ),
    mi32_time( arc_src.mi32_time ),
    mc_ident( arc_src.mc_ident ),
    mui8_len( arc_src.mui8_len )
{
}


CanPkg_c::CanPkg_c( uint32_t id, bool ext, uint8_t len, int32_t tm )
  : mc_data(),
    mi32_time( tm ),
    mc_ident( id, ext ? __IsoAgLib::Ident_c::ExtendedIdent : __IsoAgLib::Ident_c::StandardIdent ),
    mui8_len( len )
{
}


CanPkg_c &CanPkg_c::operator=(const CanPkg_c& acrc_right)
{
  mc_data = acrc_right.mc_data;
  mi32_time = acrc_right.mi32_time;
  mc_ident = acrc_right.mc_ident;
  mui8_len = acrc_right.mui8_len;

  return *this;
}


void
CanPkg_c::getData(
  uint32_t& rt_ident,
  uint8_t& rui8_identType,
  uint8_t& rb_dlcTarget,
  uint8_t* pb_dataTarget) const
{
  rt_ident = ident();
  rui8_identType = identType();
  rb_dlcTarget = mui8_len;
  mc_data.getDataToString( pb_dataTarget, mui8_len );
}


void
CanPkg_c::set(
  uint32_t at_ident, 
  const uint8_t* apb_data, 
  uint8_t aui8_len, 
  int32_t ai32_time,
  Ident_c::identType_t at_type)
{
  isoaglib_assert( aui8_len <= 8 );

  mui8_len = aui8_len;
  mc_data.setDataFromString( apb_data, mui8_len );
  mi32_time = ai32_time;
  mc_ident.set( at_ident, at_type );
}


void
CanPkg_c::setDataFromString(
  const uint8_t* apb_data, 
  uint8_t aui8_len)
{
  isoaglib_assert( aui8_len <= 8 );

  mui8_len = aui8_len;
  mc_data.setDataFromString( apb_data, mui8_len );
}


void
CanPkg_c::setDataFromString(
  uint8_t aui8_targetPositionOffset, 
  const uint8_t* apb_data, 
  uint8_t aui8_len)
{
  isoaglib_assert( (aui8_targetPositionOffset+aui8_len) <= 8 );

  mui8_len = uint8_t( aui8_targetPositionOffset + aui8_len );
  mc_data.setDataFromString( aui8_targetPositionOffset, apb_data, aui8_len);
}


} // __IsoAgLib
