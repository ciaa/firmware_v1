/*
  multisendpkg_c.h: data object for multi message data transfer

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef MULTI_SEND_PKG_H
#define MULTI_SEND_PKG_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/canpkgext_c.h>

#ifdef USE_ISO_11783
  #include <IsoAgLib/driver/can/impl/ident_c.h>
#endif

namespace __IsoAgLib {


/**
  data object for multi message transfer
  transforms flag formated information to/from
  CAN uint8_t string
  @author Dipl.-Inform. Achim Spangler
*/
class MultiSendPkg_c : public CanPkgExt_c  {
private:
  // private typedef alias names
public:
  MultiSendPkg_c() : CanPkgExt_c() {}
  ~MultiSendPkg_c() {}

  /* ************************** */
  /* ***retreiving of values*** */
  /* ************************** */

  /**
    set the 7 uint8_t data part of transfer message
    @param apb_source source data pointer
    @param ai32_pos uint8_t position in data string to start
    @param ab_partSize optional amount of bytes of data stream for actual pkg (default 7)
  */
  void setDataPart(const HUGE_MEM uint8_t* apb_source, int32_t ai32_pos, uint8_t ab_partSize = 7);

  /**
    set the 7 uint8_t data part of transfer message
    @param rc_vecSource source data pointer
    @param ai32_pos uint8_t position in data string to start
    @param ab_partSize optional amount of bytes of data stream for actual pkg (default 7)
   */
  void setDataPart(const STL_NAMESPACE::vector<uint8_t>& rc_vecSource, int32_t ai32_pos, uint8_t ab_partSize = 7);

  #if defined(ENABLE_MULTIPACKET_VARIANT_FAST_PACKET)
  /**
    set the 7 uint8_t data part of transfer message
    @param apb_source source data pointer
    @param ai32_pos uint8_t position in data string to start
    @param ab_partSize optional amount of bytes of data stream for actual pkg (default 7)
   */
  void setFastPacketDataPart(const HUGE_MEM uint8_t* apb_source, int32_t ai32_pos, uint8_t ab_partSize = 7, uint8_t aui8_offset = 0);

  /**
    set the 7 uint8_t data part of transfer message
    @param apb_source source data pointer
    @param ai32_pos uint8_t position in data string to start
    @param ab_partSize optional amount of bytes of data stream for actual pkg (default 7)
   */
  void setFastPacketDataPart(const STL_NAMESPACE::vector<uint8_t>& rc_vecSource, int32_t ai32_pos, uint8_t ab_partSize = 7, uint8_t aui8_offset = 0);
  #endif
};

}
#endif
