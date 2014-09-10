/*
  iisbclient_c.h: central ISB client management
                  (Stop all implement operations)

  (C) Copyright 2013 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IISBCLIENT_C_H
#define IISBCLIENT_C_H

#include "impl/isbclient_c.h"


namespace IsoAgLib {

class iIdentItem_c;
class iIsbStateHandler_c;


class iIsbClient_c : private __IsoAgLib::IsbClient_c
{
public:
  void registerStateHandler( iIdentItem_c& identItem , IsoAgLib::iIsbStateHandler_c& handler )
  { __IsoAgLib::IsbClient_c::registerStateHandler( static_cast<__IsoAgLib::IdentItem_c&>( identItem ), handler ); }

  void deregisterStateHandler( iIdentItem_c& identItem, IsoAgLib::iIsbStateHandler_c& handler )
  { __IsoAgLib::IsbClient_c::deregisterStateHandler( static_cast<__IsoAgLib::IdentItem_c&>( identItem ), handler ); }

private:
  /** allow getIisbClientInstance() access to shielded base class.
      otherwise __IsoAgLib::getIsbClientInstance() wouldn't be accepted by compiler */
  #if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
  friend iIsbClient_c& getIisbClientInstance( unsigned instance );
  #else
  friend iIsbClient_c& getIisbClientInstance();
  #endif
};

#if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
  inline iIsbClient_c& getIisbClientInstance( unsigned instance = 0 )
  { return static_cast<iIsbClient_c&>( __IsoAgLib::getIsbClientInstance( instance ) ); }
#else
  inline iIsbClient_c& getIisbClientInstance()
  { return static_cast<iIsbClient_c&>( __IsoAgLib::getIsbClientInstance() ); }
#endif

}

#endif
