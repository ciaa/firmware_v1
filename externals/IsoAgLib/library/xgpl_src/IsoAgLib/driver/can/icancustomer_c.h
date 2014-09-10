/*
  icancustomer_c.h: header file for basic object for customer
    relationship to one or more FilterBox_c (-> CAN IO) instances

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ICAN_CUSTOMER_H
#define ICAN_CUSTOMER_H

#include "impl/cancustomer_c.h"
#include <IsoAgLib/driver/can/icanpkg_c.h>

namespace __IsoAgLib
{ // forward declarations
  class ProprietaryMessageHandler_c;
}

// Begin Namespace IsoAgLib
namespace IsoAgLib {

/** Base class for CAN customer classes
  -> FilterBox_c calls member function
    processMsg() to initiate processing of new received CAN telegram,
    which suits the configured filter/mask combination
  ==> all CAN handling object, which uses the services of FilterBox_c, CAN_IO
      should be derived from this class
  @see __IsoAgLib::CAN_IO
  @see __IsoAgLib::FilterBox
  *@author Dipl.-Inform. Achim Spangler
  */
class iCanCustomer_c : private __IsoAgLib::CanCustomer_c
{
public:
  /**
    process a message -> the specialized/derived version of this virtual
    function is called during processing of received CAN telegrams in CanIo_c::processMsg
    @see __IsoAgLib::CanIo_c::processMsg
  */
  virtual void process( const iCanPkg_c& arc_data ) = 0;

private:
  friend class iIsoBus_c;
  friend class iProprietaryBus_c;
  friend struct iIsoFilter_s;
  friend class __IsoAgLib::ProprietaryMessageHandler_c;
};

} // namespace
#endif
