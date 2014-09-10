/*
  dataframe.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <logenvirons.h>
#include <vector>
#include <yasper.h>


class DataFrame_c {
public:
  DataFrame_c(
      uint64_t aui64_time,
      uint32_t aui64_identifier,
      std::vector< uint8_t > const &acrvec_data,
      bool ab_canExt = true
    ) :
    mui64_time(aui64_time),
    mui64_identifier(aui64_identifier),
    mvec_data(acrvec_data),
    mb_canExt(ab_canExt)
  {}

  DataFrame_c( // artificial frame for frame data and addresses only!
      std::vector< uint8_t > const &acrvec_data,
      uint8_t aui8_sourceAddress,
      uint8_t aui8_destinationAddress,
      uint32_t pgn
    ) :
    mui64_time(0),
    mui64_identifier( uint32_t( ( pgn | aui8_destinationAddress) ) << 8 | aui8_sourceAddress),
    mvec_data(acrvec_data),
    mb_canExt(true)
  {}

  uint8_t prio() const {
    return uint8_t(mui64_identifier >> 26);
  }

  uint8_t dp() const {
    return (mui64_identifier >> 24) & 0x03;
  }

  uint8_t sourceAddress() const {
     return mui64_identifier & 0xFF;
  }

  uint8_t ps() const {
    return (mui64_identifier >>  8) & 0xFF;
  }

  uint8_t destinationAddress() const {
    return (mui64_identifier >> 8) & 0xFF;
  }

  uint8_t pf() const {
    return (mui64_identifier >> 16) & 0xFF;
  }

  uint32_t pgn() const {
    return
      (dp() << 16) |
      (pf() << 8) |
      ((pf() >= 0xF0) ? ps() : 0);
  }

  uint64_t time() const {
    return mui64_time;
  }

  uint8_t dataOctet(size_t at_index) const {
    return mvec_data[at_index];
  }

  char asciiDataOctet(size_t at_index) const {
    return getAscii(mvec_data[at_index]);
  }

  std::vector< uint8_t > const &data() const {
    return mvec_data;
  }

  size_t dataSize() const {
    return mvec_data.size();
  }

  bool isPdu1() const {
    return pf() < 0xF0;
  }

  uint64_t identifier() const {
    return mui64_identifier;
  }

  bool isExtendedFrameFormat() const {
    return mb_canExt;
  }

private:
  uint64_t mui64_time;
  uint32_t mui64_identifier;
  std::vector< uint8_t > mvec_data;
  bool mb_canExt;
};


typedef yasper::ptr< DataFrame_c > PtrDataFrame_t;


#endif
