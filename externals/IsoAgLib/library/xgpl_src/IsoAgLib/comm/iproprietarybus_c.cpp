/*
  iproprietarybus.cpp - Interface access class for the proprietary CAN-busses

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <IsoAgLib/comm/iproprietarybus_c.h>

#if 0 < PROP_INSTANCE_CNT
namespace IsoAgLib
{

iProprietaryBus_c &getIProprietaryBusInstance( uint8_t aui8_instance )
{
  MACRO_MULTITON_GET_INSTANCE_BODY(iProprietaryBus_c, CAN_INSTANCE_CNT, aui8_instance);
}

}
#endif
