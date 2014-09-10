/*
  icanpkg_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef iCAN_Pkg_H
#define iCAN_Pkg_H



#include "impl/canpkg_c.h"

#include <IsoAgLib/driver/can/iident_c.h>

// Begin Namespace IsoAgLib
namespace IsoAgLib {


/** @todo SOON-129: Add Interface class for FlexibleByteString stuff in iutil_funcs.h, so that this iCanPkg_c gets consistent ... */

/**
  Storing the informations of one CAN
  message package. Implements some methods
  for input and output of informations into
  other objects and data structures.
  @author Dipl.-Inform. Achim Spangler
*/
class iCanPkg_c : public __IsoAgLib::CanPkg_c
{
public:

  void setIdent( uint32_t at_ident, iIdent_c::identType_t at_type )
    { CanPkg_c::setIdent (at_ident, at_type); }

  /**
    set specific uint8_t of ident for the telegram
    (position 0 is least significant position -> nearest to DLC field of
    CAN frame)
    @param ab_val value for ident at wanted position for the telegram
    @param ab_pos position for wanted value for ident for the telegram
  */
  void setIdentByte (uint8_t ab_val, uint8_t ab_pos )
    { CanPkg_c::setIdentByte (ab_val, ab_pos); }

  /**
    set type of ident
    @param at_type type of Ident_c: 11bit Ident_c::S or 29bit Ident_c::E
  */
  void setIdentType(iIdent_c::identType_t at_type) { CanPkg_c::setIdentType (at_type); }

  iIdent_c::identType_t identType() const { return CanPkg_c::identType(); }

  uint32_t ident() const { return CanPkg_c::ident(); }

  uint32_t ident( uint8_t ab_pos ) const { return CanPkg_c::ident(ab_pos); }

  /** retrieve CAN data bytes represented by pointer to Union8ByteString_u */
  __IsoAgLib::Flexible8ByteString_c* getDataUnion() { return CanPkg_c::getDataUnion(); }

  /** retrieve CAN data bytes represented by CONST pointer to Union8ByteString_u */
  const __IsoAgLib::Flexible8ByteString_c* getDataUnionConst() const { return CanPkg_c::getDataUnionConst(); }

  /** retrieve a raw const pointer to uint8_t data string from given offset position onwards */
  const uint8_t* getUint8DataConstPointer( uint8_t aui8_positionOffset ) const
    { return CanPkg_c::getUint8DataConstPointer (aui8_positionOffset); }

  /** retrieve a raw const pointer to uint8_t data string */
  const uint8_t* getUint8DataConstPointer() const { return CanPkg_c::getUint8DataConstPointer();}

  /** retrieve a raw const pointer to uint8_t data string from given offset position onwards */
  uint8_t* getUint8DataPointer (uint8_t aui8_positionOffset)
    { return CanPkg_c::getUint8DataPointer (aui8_positionOffset); }

  /** retrieve a raw const pointer to uint8_t data string */
  uint8_t* getUint8DataPointer() { return CanPkg_c::getUint8DataPointer(); }

  /**
    set data with size bytes from source array;
    if aui8_len is greater than 8 (max data length of CAN) than max 8 bytes are transfered
    @param apb_data pointer to source data uint8_t array
    @param aui8_len amount/len of the source data string
  */
  void setDataFromString(const uint8_t* apb_data, uint8_t aui8_len)
    { CanPkg_c::setDataFromString(apb_data, aui8_len); }

  /**
    set data with size bytes from source array;
    if aui8_len is greater than 8 (max data length of CAN) than max 8 bytes are transfered.
    this variant of the setDataFromString allows to set the CAN data from specified offset position onwards.
    @param aui8_targetPositionOffset
    @param apb_data pointer to source data uint8_t array
    @param aui8_len amount/len of the source data string
  */
  void setDataFromString(uint8_t aui8_targetPositionOffset, const uint8_t* apb_data, uint8_t aui8_len)
    { CanPkg_c::setDataFromString(aui8_targetPositionOffset, apb_data, aui8_len); }

  /**
    set a complete 8-Byte data string from source type Union8ByteString_u.
    The length of this package is automatically set to length 8.
    @param apc_data pointer to source data Flexible8ByteString_c
  */
  void setDataUnion(const __IsoAgLib::Flexible8ByteString_c* apc_data)
    { CanPkg_c::setDataUnion(apc_data); }

  /**
    set a complete 8-Byte data string from source type Union8ByteString_u.
    The length of this package is automatically set to length 8.
    @param apc_data pointer to source data Flexible8ByteString_c
  */
  void setDataUnion(uint8_t aui8_ind, const __IsoAgLib::Flexible4ByteString_c* apc_data)
    { CanPkg_c::setDataUnion(aui8_ind, apc_data); }

  /** retrieve len of last received CAN message */
  uint8_t getLen( void ) const { return CanPkg_c::getLen(); }

  /**
    set the data len of CAN pkg (if ab_val is greater than 8, 8 is stored)
    @param ab_val length/amount of uint8_t in data string
  */
  void setLen(uint8_t ab_val) { CanPkg_c::setLen (ab_val); }

  /**
    set receive time
    @param ai32_time time stamp of CAN telegram in [msec.] from system start
  */
  void setTime(int32_t ai32_time) { CanPkg_c::setTime (ai32_time); }

  /**
    deliver time
    @return timestamp of the CAN telegram in [msec.] since system start
  */
  int32_t time() const { return CanPkg_c::time(); }

   void set( uint32_t at_ident, const uint8_t* apb_data, uint8_t aui8_len, int32_t ai32_time,
    iIdent_c::identType_t at_type )
    { CanPkg_c::set (at_ident, apb_data, aui8_len, ai32_time, at_type); }

  void set( uint32_t at_ident, const __IsoAgLib::Flexible8ByteString_c* apc_data, int32_t ai32_time,
    iIdent_c::identType_t at_type )
    { CanPkg_c::set(at_ident, apc_data, ai32_time, at_type); }


  /**
    put data into given reference to BIOS related data structure with data, len
    @param rt_ident     reference where the ident is placed for send
    @param rui8_identType reference to the ident type val: 0==std, 1==ext
    @param rb_dlcTarget reference to the DLC field of the target
    @param pb_dataTarget pointer to the data string of the target
  */
  virtual void getData(uint32_t& rt_ident, uint8_t& rui8_identType,
                       uint8_t& rb_dlcTarget, uint8_t* pb_dataTarget) const
    { CanPkg_c::getData (rt_ident, rui8_identType, rb_dlcTarget, pb_dataTarget); }

  /** copy the data bytes from the CanPkg_c to the given uint8_t* pointer.
      the pointed array must be at least 8 byte in size.
      This function copies as many byte as are defined by msui8_len.
    */
  void getDataToString( uint8_t* pui8_targetData ) const
    { CanPkg_c::getDataToString (pui8_targetData); }

  /** copy the data bytes from the CanPkg_c to the given uint8_t* pointer.
      the amount of copied data can be restricted by the last parameter.
      The first parameter defines the index of the first copied data byte.
    */
  void getDataToString( uint8_t aui8_positionOffset, uint8_t* pui8_targetData, uint8_t aui8_maxSize ) const
    { CanPkg_c::getDataToString (aui8_positionOffset, pui8_targetData, aui8_maxSize); }

  /**
    simply deliver a uint8_t from a specific position with operator[]
    @param ab_pos position of dellivered uint8_t [0..7]
    @return uint8_t balue in CAN data string at pos ab_pos
  */
  uint8_t operator[](uint8_t ab_pos) const {return CanPkg_c::operator[](ab_pos); }

  /**
    set an uint8_t value at specified position in string
    @param aui8_pos position [0..7]
    @param aui8_val uint8_t value to set
  */
  void setUint8Data( uint8_t aui8_pos, uint8_t aui8_val) { CanPkg_c::setUint8Data( aui8_pos, aui8_val ); }
  /**
    set an uint16_t value at specified position in string
    @param aui8_pos position [0..6]
    @param aui16_val uint16_t value to set
  */
  void setUint16Data( uint8_t aui8_pos, uint16_t aui16_val) { CanPkg_c::setUint16Data( aui8_pos, aui16_val ); }
  /**
    set an int16_t value at specified position in string
    @param aui8_pos position [0..6]
    @param ai16_val int16_t value to set
  */
  void setInt16Data( uint8_t aui8_pos, int16_t ai16_val) { CanPkg_c::setInt16Data( aui8_pos, ai16_val ); }
  /**
    set an uint32_t value at specified position in string
    @param aui8_pos position [0..4]
    @param aui32_val uint32_t value to set
  */
  void setUint32Data( uint8_t aui8_pos, uint32_t aui32_val) { CanPkg_c::setUint32Data( aui8_pos, aui32_val ); }
  /**
    set an int32_t value at specified position in string
    @param aui8_pos position [0..4]
    @param ai32_val int32_t value to set
  */
  void setInt32Data( uint8_t aui8_pos, int32_t ai32_val) { CanPkg_c::setInt32Data( aui8_pos, ai32_val ); }

  /**
    simply deliver a uint8_t from a specific position with
    @param ab_pos position of dellivered uint8_t [0..7]
    @return uint8_t balue in CAN data string at pos ab_pos
  */
  uint8_t getUint8Data(uint8_t ab_pos) const {return CanPkg_c::getUint8Data( ab_pos ); }
  /**
    simply deliver a uint16_t from a specific starting position with
    @param ab_pos position of dellivered uint16_t [0..6]
    @return uint16_t balue in CAN data string at pos (ab_pos, ab_pos+1) read Low/High order
  */
  uint16_t getUint16Data(uint8_t ab_pos) const {return CanPkg_c::getUint16Data( ab_pos ); }
  /**
    simply deliver a int16_t from a specific starting position with
    @param ab_pos position of dellivered int16_t [0..6]
    @return int16_t balue in CAN data string at pos (ab_pos, ab_pos+1) read Low/High order
  */
  int16_t getInt16Data(uint8_t ab_pos) const {return CanPkg_c::getInt16Data( ab_pos ); }
  /**
    simply deliver a uint32_t from a specific starting position with
    @param ab_pos position of dellivered uint32_t [0..4]
    @return uint32_t balue in CAN data string at pos (ab_pos, ab_pos+1) read Low/High order
  */
  uint32_t getUint32Data(uint8_t ab_pos) const {return CanPkg_c::getUint32Data( ab_pos ); }
  /**
    simply deliver a int32_t from a specific starting position with
    @param ab_pos position of dellivered int32_t [0..4]
    @return int32_t balue in CAN data string at pos (ab_pos, ab_pos+1) read Low/High order
  */
  int32_t getInt32Data(uint8_t ab_pos) const {return CanPkg_c::getInt32Data( ab_pos ); }

private:
  friend class iIsoBus_c;
  friend class iProprietaryBus_c;
};

}
#endif
