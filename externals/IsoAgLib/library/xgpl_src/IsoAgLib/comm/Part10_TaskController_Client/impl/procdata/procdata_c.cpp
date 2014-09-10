/*
  procdata_c.cpp: Class for handling Process Data

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "procdata_c.h"
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/devicepool_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/identitem_c.h>

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif


namespace __IsoAgLib {

  ProcData_c::ProcData_c( )
    : m_ident( NULL )
    , m_dpd( NULL )
    , m_det( NULL )
    , m_setpoint()
    , m_measurement()
  {}


  void ProcData_c::init( IdentItem_c& ident, const DeviceObjectDpd_c& dpd, const DeviceObjectDet_c& det, SetpointHandler_c* setpointhandler ) {
    m_ident = &ident;
    m_dpd = &dpd;
    m_det = &det;

    m_setpoint.init( setpointhandler );

    isoaglib_assert(
      ( DDI() != IsoAgLib::ProcData::DefaultDataLoggingDDI ) ? true :
      isMethodSet( IsoAgLib::ProcData::TimeInterval ) &
      isMethodSet( IsoAgLib::ProcData::DistInterval ) &
      isMethodSet( IsoAgLib::ProcData::ThresholdLimit ) &
      isMethodSet( IsoAgLib::ProcData::OnChange ) &
      isMethodSet( IsoAgLib::ProcData::Total ) );
  }


  void ProcData_c::close() {}


  uint16_t ProcData_c::DDI() const {
    return m_dpd->ddi();
  }


  uint16_t ProcData_c::element() const {
    return m_det->elementNumber();
  }


  uint8_t ProcData_c::triggerMethod() const {
    return m_dpd->method();
  }

}
