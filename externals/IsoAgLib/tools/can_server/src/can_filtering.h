/*
  can_filtering.h: Module to handle filtering of CAN-Packets.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef CAN_FILTERING_H
#define CAN_FILTERING_H

#include <string>
#include "wrapper_for_can_server_interface.h"

// Author: Martin Wodok
namespace can_filtering
{
  std::string config( const std::string &cmd );
  bool pass( unsigned bus, uint32_t id, unsigned dlc, uint8_t *databytes );
}

#endif
