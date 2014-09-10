/*
  multisendpkg_c.cpp: data object for Multipacket messages

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */
#include "multisendpkg_c.h"
#include <IsoAgLib/driver/can/impl/ident_c.h>

namespace __IsoAgLib {


static const uint8_t paddingDataArr[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

/**
  set the 7 uint8_t data part of transfer message
  @param apb_source source data pointer
  @param ai32_pos uint8_t position in data string to start
  @param ab_partSize optional amount of bytes of data stream for actual pkg (default 7) {Anyway MAX 7 bytes!!}
*/
void MultiSendPkg_c::setDataPart(const HUGE_MEM uint8_t* apb_source, int32_t ai32_pos, uint8_t ab_partSize)
{
  #ifdef USE_HUGE_MEM
  const HUGE_MEM uint8_t* pb_source = apb_source + ai32_pos;
  // fist assign is performed with index:=1, so that last assign should be performed with
  // ind == ab_partSize (e.g. ab_partSize:=7 ==> last written index:=7 == Byt8 of CanPkg_c)
  for ( uint8_t ind = 1; ind <= ab_partSize; ind++ )
  { // the ind matches already to the appropriate CanPkg_c data string position,
    // while the pb_source pointer is also set to the start of the source data
    setUint8Data( ind, *pb_source );
    // now increment the source pointer for next access
    pb_source++;
  }
  #else
  setDataFromString (1, apb_source + ai32_pos, ab_partSize);
  #endif
  if ( ab_partSize < 7 )
  { // only pad when less than 7 data byte
    setDataFromString( uint8_t(1+ab_partSize), paddingDataArr, uint8_t(7-ab_partSize) );
  }
}



/**
  set the 7 uint8_t data part of transfer message
  @param rc_vecSource source data pointer
  @param ai32_pos uint8_t position in data string to start
  @param ab_partSize optional amount of bytes of data stream for actual pkg (default 7)
  */
void MultiSendPkg_c::setDataPart(const STL_NAMESPACE::vector<uint8_t>& rc_vecSource, int32_t ai32_pos, uint8_t ab_partSize)
{
  STL_NAMESPACE::vector<uint8_t>::const_iterator iter = rc_vecSource.begin()+ai32_pos;
  // fist assign is performed with index:=1, so that last assign should be performed with
  // ind == ab_partSize (e.g. ab_partSize:=7 ==> last written index:=7 == Byt8 of CanPkg_c)
  for ( uint8_t ind = 1; ind <= ab_partSize; ind++ )
  { // the ind matches already to the appropriate CanPkg_c data string position,
    // while the iter iterator is also set to the start of the source data
    setUint8Data( ind, *iter );
    // now increment the iterator for next access
    ++iter;
  }
  if ( ab_partSize < 7 )
  { // only pad when less than 7 data byte
    setDataFromString( uint8_t(1+ab_partSize), paddingDataArr, uint8_t(7-ab_partSize) );
  }
}



#if defined(ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
/**
  set the 7 uint8_t data part of transfer message
  @param apb_source source data pointer
  @param ai32_pos uint8_t position in data string to start
  @param ab_partSize optional amount of bytes of data stream for actual pkg (default 7) {Anyway MAX 7 bytes!!}
 */
void MultiSendPkg_c::setFastPacketDataPart(const HUGE_MEM uint8_t* apb_source, int32_t ai32_pos, uint8_t ab_partSize, uint8_t aui8_offset)
{
#ifdef USE_HUGE_MEM
  const HUGE_MEM uint8_t* pb_source = apb_source + ai32_pos;
  // examples:
  // ab_partSize:=7, aui8_offset:=1 ==> cui8_endCondition:=8
  // ==> first assign should match target byte index aui8_offset:=1
  //     and last index should be <8 -> i.e. index:=7
  const uint8_t cui8_endCondition = ab_partSize + aui8_offset;
  for ( uint8_t ind = aui8_offset; ind < cui8_endCondition; ind++ )
  { // the ind matches already to the appropriate CanPkg_c data string position,
    // while the pb_source pointer is also set to the start of the source data
    setUint8Data( ind, *pb_source );
    // now increment the source pointer for next access
    pb_source++;
  }
#else
  setDataFromString (aui8_offset, apb_source + ai32_pos, ab_partSize);
#endif
  setDataFromString (uint8_t(aui8_offset+ab_partSize), paddingDataArr, uint8_t(8-aui8_offset-ab_partSize));
}



/**
  set the 7 uint8_t data part of transfer message
  @param rc_vecSource source data pointer
  @param ai32_pos uint8_t position in data string to start
  @param ab_partSize optional amount of bytes of data stream for actual pkg (default 7)
  */
void MultiSendPkg_c::setFastPacketDataPart(const STL_NAMESPACE::vector<uint8_t>& rc_vecSource, int32_t ai32_pos, uint8_t ab_partSize, uint8_t aui8_offset )
{
  STL_NAMESPACE::vector<uint8_t>::const_iterator iter = rc_vecSource.begin()+ai32_pos;
  // examples:
  // ab_partSize:=7, aui8_offset:=1 ==> cui8_endCondition:=8
  // ==> first assign should match target byte index aui8_offset:=1
  //     and last index should be <8 -> i.e. index:=7
  const uint8_t cui8_endCondition = uint8_t(ab_partSize + aui8_offset);
  for ( uint8_t ind = aui8_offset; ind < cui8_endCondition; ind++ )
  { // the ind matches already to the appropriate CanPkg_c data string position,
    // while the iter iterator is also set to the start of the source data
    setUint8Data( ind, *iter );
    // now increment the iterator for next access
    ++iter;
  }
  setDataFromString (uint8_t(aui8_offset+ab_partSize), paddingDataArr, uint8_t(8-aui8_offset-ab_partSize));
}
#endif

} // end of namespace __IsoAgLib
