/*
  iident.h: header file for iIdent

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IIDENT_H
#define IIDENT_H

#include "impl/ident_c.h"

// Begin Namespace IsoAgLib
namespace IsoAgLib {

/** class for management of CAN ident with 
  ident type dpendent compare and setting operations
  @see iCAN_IO
  @see FilterBox
  @see MSgObj
  *@author Dipl.-Inform. Achim Spangler
  */
class iIdent_c : public __IsoAgLib::Ident_c {
public: 
  /**
    @brief enum type for defining the CAN ident length to S (11Bit)
    or extended (29Bit)
  */
  typedef Ident_c::identType_t identType_t;
    
  /** 
    @brief constructor which can set the values
    @param at_ident new ident setting
    @param ren_identType new ident type 
        (Ident_c::S for 11bit ident or Ident_c::E for 29bit)
  */
  iIdent_c( uint32_t at_ident, identType_t ren_identType )
    : Ident_c(at_ident, ren_identType) {}

  /**
    @brief constructor which gets its values from other instance
    @param acrc_src source Ident_c instance
  */
  iIdent_c(const Ident_c& acrc_src) : Ident_c(acrc_src) {}
  
  /**
    @brief compare this ident setting with another
    (use memory operation for max speed)
    @param acrc_src compared ident
    @return true -> referenced ident has same setting and type
  */
  bool operator==(const Ident_c& acrc_src) const
    { return Ident_c::operator==(acrc_src); }

  /**
    @brief compare this ident setting with another
    (use memory operation for max speed)
    @param acrc_src compared ident
    @return true -> referenced ident has different setting and type
  */
  bool operator!=(const Ident_c& acrc_src) const
    { return Ident_c::operator!=(acrc_src); }

  /**
    @brief compare this ident value and ident type
    @param at_ident compared ident setting
    @param ren_identType type of compared ident
    @return true -> given setting and type are equal
  */
  bool equal( const uint32_t at_ident, identType_t ren_identType ) const
    { return Ident_c::equal( at_ident, ren_identType ); }

  /**
    @brief:deliver ident value masked by given ident
    @param at_mask mask value
    @return ident setting masked by at_mask (only '1' where mask and ident has '1')
  */
  uint32_t masked( uint32_t at_mask = ~0 ) const
    { return Ident_c::masked( at_mask ); }

  /**
    @brief:deliver ident value masked by given ident
    @param acrc_mask mask value
    @return ident setting masked by at_mask (only '1' where mask and ident has '1')
  */
  uint32_t masked( const Ident_c& acrc_mask ) const
    { return Ident_c::masked( acrc_mask ); }

  identType_t identType() const { return Ident_c::identType(); }

  uint32_t ident() const { return Ident_c::ident(); }

  uint32_t ident( uint8_t ab_pos ) const { return Ident_c::ident(ab_pos); }

  void set( uint32_t at_ident, identType_t ren_identType )
    { Ident_c::set(at_ident, ren_identType); }

  /**
    @brief set this ident with access to single unsigned char
    (position 0 is least significant position -> nearest to DLC field of
    CAN frame)
    @param ab_val new val for ident at wanted position
    @param ab_pos position in ident, where ident should be placed in
  */
  void setByte(uint8_t ab_val, uint8_t ab_pos )
    { Ident_c::setByte(ab_val, ab_pos); }

  /**
    @brief set type of ident
    @param at_type type of Ident_c: 11bit Ident_c::S or 29bit Ident_c::E
  */
  void setIdentType(iIdent_c::identType_t at_type)
    { Ident_c::setIdentType(at_type); }

  /**
    @brief set this ident according to other Ident
    @param acrc_src source Ident_c instance
  */
  const iIdent_c& operator=(const iIdent_c& acrc_src)
    { return( (const iIdent_c&)Ident_c::operator=(((const Ident_c&)acrc_src)) ); }
};

}

#endif
