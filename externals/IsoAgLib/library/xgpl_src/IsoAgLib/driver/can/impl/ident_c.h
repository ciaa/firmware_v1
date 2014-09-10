/*
  ident_c.h: header file for Ident

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IDENT_H
#define IDENT_H

#include <IsoAgLib/isoaglib_config.h>


namespace __IsoAgLib {

/** class for management of CAN ident with
  ident type dpendent compare and setting operations
  @author Martin Wodok
  @author Dipl.-Inform. Achim Spangler
  */
class Ident_c {
public:
  enum identType_t {StandardIdent = 0, ExtendedIdent = 1};

  Ident_c( uint32_t at_ident, identType_t ren_identType );
  Ident_c(const Ident_c& acrc_src);
  ~Ident_c();

  /** compare this ident setting with another
    (use memory operation for max speed)
    @param acrc_src compared ident
    @return true -> referenced ident has same setting and type
  */
  bool operator==(const Ident_c& acrc_src) const;

  /** compare this ident setting with another
    (use memory operation for max speed)
    @param acrc_src compared ident
    @return true -> referenced ident has different setting and type
  */
  bool operator!=(const Ident_c& acrc_src) const
    {return (!(operator==(acrc_src)));}

  /** compare this ident value and ident type
    @param at_ident compared ident setting
    @param ren_identType type of compared ident
    @return true -> given setting and type are equal
  */
  bool equal(const uint32_t at_ident, identType_t ren_identType) const
    { return( (mt_ident == at_ident) && (mt_type == ren_identType) ); }

  /** deliver ident value masked by given ident
    @param at_mask mask value
    @return ident setting masked by at_mask (only '1' where mask and ident has '1')
  */
  uint32_t masked( uint32_t at_mask = ~0 ) const
    { return (mt_type == StandardIdent) ? (at_mask & mt_ident & 0x7FF) : (at_mask & mt_ident); }

  /** deliver ident value masked by given ident
    @param acrc_mask mask value
    @return ident setting masked by at_mask (only '1' where mask and ident has '1')
  */
  uint32_t masked( const Ident_c& acrc_mask ) const
    { return (mt_type == StandardIdent) ? (acrc_mask.mt_ident & mt_ident & 0x7FF) : (acrc_mask.mt_ident & mt_ident); }

  identType_t identType() const {return mt_type;}

  uint32_t ident() const {return mt_ident;}

  uint8_t ident(uint8_t ab_pos) const {return static_cast<uint8_t>((mt_ident >> (ab_pos*8)) & 0xFF);}

  void set( uint32_t at_ident, identType_t ren_identType );

  /** set this ident with access to single unsigned char
    (position 0 is least significant position -> nearest to DLC field of
    CAN frame)
    @param ab_val new val for ident at wanted position
    @param ab_pos position in ident, where ident should be placed in
  */
  void setByte(uint8_t ab_val, uint8_t ab_pos);

  /**
    set specific uint16_t of this ident
    (position 0 is least significant position -> nearest to DLC field of
    CAN frame)
    @param aui16_val value for ident at wanted position for the telegram
    @param aui8_pos position [0..1] for wanted value for ident for the telegram (pos0==byte0, pos1==byte2)
    @param at_type type of Ident_c: 11bit Ident_c::S or 29bit Ident_c::E
  */
  void setWord(uint16_t aui16_val, uint8_t aui8_pos);

  /** set type of ident
    @param at_type type of Ident_c: 11bit Ident_c::S or 29bit Ident_c::E
  */
  void setIdentType(Ident_c::identType_t at_type){mt_type = at_type;}

  /** set this ident according to other Ident
    @param acrc_src source Ident_c instance
  */
  const Ident_c& operator=(const Ident_c& acrc_src) {
    mt_ident = acrc_src.mt_ident;
    mt_type = acrc_src.mt_type;
    return acrc_src;
  }

private:
  uint32_t    mt_ident;
  identType_t mt_type;
};
}

#endif
