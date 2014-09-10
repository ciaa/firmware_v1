/*
  ident_c.cpp: source file for Ident

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "ident_c.h"


namespace __IsoAgLib
{

  Ident_c::Ident_c(uint32_t at_ident, identType_t ren_identType)
  {
    mt_ident = (ren_identType == StandardIdent)?(at_ident&0x7FF):at_ident;
    mt_type = ren_identType;
  }


  Ident_c::Ident_c(const Ident_c& acrc_src)
  {
    mt_ident = acrc_src.mt_ident;
    mt_type = acrc_src.mt_type;
  }

  Ident_c::~Ident_c()
  {
  }


  bool
    Ident_c::operator==(const Ident_c& acrc_src) const
  {
    return ((ident()==acrc_src.ident())
            &&(identType()==acrc_src.identType()));
  }


  void
  Ident_c::set( uint32_t at_ident, identType_t ren_identType )
  {
    mt_ident = (ren_identType == StandardIdent) ? (at_ident&0x7FF) : at_ident;
    mt_type = ren_identType;
  }


  void
  Ident_c::setByte(uint8_t ab_val, uint8_t ab_pos )
  {
    mt_ident &= ~(uint32_t(0xFF) << (ab_pos*8));
    mt_ident |= (uint32_t(ab_val) << (ab_pos*8));
  }


  void
  Ident_c::setWord(uint16_t aui16_val, uint8_t aui8_pos)
  {
    mt_ident &= ~(uint32_t(0xFFFF) << (aui8_pos*16));
    mt_ident |= (uint32_t(aui16_val) << (aui8_pos*16));
  }

}
