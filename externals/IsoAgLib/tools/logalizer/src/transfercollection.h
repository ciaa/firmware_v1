/*
  transfercollection.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef TRANSFERCOLLECTION_H
#define TRANSFERCOLLECTION_H

#include <logenvirons.h>
#include <yasper.h>
#include <vector>
#include <map>


class TransferCollection_c {
public:
  enum Variant_e {
    variant_tp,
    variant_etp,
  };

  struct Connection_s {
    Connection_s(size_t at_sizeTransferData);
    std::vector< uint8_t > mvec_data;
    uint32_t mui32_embeddedPgn;
    uint32_t mui32_packetOffSet;
  };

  typedef yasper::ptr< Connection_s > PtrConnection_t;

  PtrConnection_t newConnection(
      Variant_e ae_variant,
      uint8_t aui8_transferSourceAddress,
      uint8_t aui8_transferDestinationAddress,
      size_t at_sizeTransferData);

  PtrConnection_t getConnection(
      Variant_e ae_variant,
      uint8_t aui8_transferSourceAddress,
      uint8_t aui8_transferDestinationAddress);

  void deleteConnection(
      Variant_e ae_variant,
      uint8_t aui8_transferSourceAddress,
      uint8_t aui8_transferDestinationAddress);

private:
  struct Key_s {
    Key_s(
        Variant_e ae_variant,
        uint8_t aui8_transferSourceAddress,
        uint8_t aui8_transferDestinationAddress);
    Variant_e me_variant;
    uint8_t mui8_transferSourceAddress;
    uint8_t mui8_transferDestinationAddress;
  };

  friend bool operator<(
      Key_s const &arcs_left,
      Key_s const &arcs_right);

  typedef std::map< Key_s, PtrConnection_t > Transfers_t;
  Transfers_t t_transfers;
};



inline
TransferCollection_c::Connection_s::Connection_s( size_t at_sizeTransferData ) :
  mvec_data(at_sizeTransferData, 0),
  mui32_embeddedPgn(0),
  mui32_packetOffSet(0)
{
}


inline
TransferCollection_c::Key_s::Key_s(
    Variant_e ae_variant,
    uint8_t aui8_transferSourceAddress,
    uint8_t aui8_transferDestinationAddress) :
  me_variant(ae_variant),
  mui8_transferSourceAddress(aui8_transferSourceAddress),
  mui8_transferDestinationAddress(aui8_transferDestinationAddress)
{}


#endif
