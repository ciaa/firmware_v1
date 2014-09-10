/*
  idiagnosticsservices_c.h - Interface access class for the diagnostic services DM1/DM2/DM3 manager

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IDIAGNOSTICS_SERVICES_H
#define IDIAGNOSTICS_SERVICES_H

#include "impl/diagnosticsservices_c.h"

#include "idiagnosticstypes.h"

// Begin Namespace IsoAgLib
namespace IsoAgLib {

class iIsoName_c;

/**
  To decide if a "Clear Previous Active Trouble Codes" request should be accepted,
  the Application can register an implementation of "iServiceToolVerifier_c" where
  the implemented "acceptAsServiceTool(..)" function decides if the requesting node
  represented by the parameter NAME is allowed to perform the clear operation.
  @short Base class for ServiceTool verifier feature
  @author Antoine Kandera, reviewed by Martin Wodok
*/
class iServiceToolVerifier_c
{
public:
  virtual bool acceptAsServiceTool (const iIsoName_c &name) = 0;
};

/**
  Provides interface for accessing the diagnostics services DM1/DM2/DM3 manager
  NOTE: this class is not Application-constructable
  @short Interface access class managing diagnostics services DM1/DM2/DM3
  @author Antoine Kandera, reviewed by Martin Wodok
*/
class iDiagnosticsServices_c : private __IsoAgLib::DiagnosticsServices_c
{
public:
  /**
    Registering of the "iServiceToolVerifier_c"
    If no handler is registered (default behavior), the request is accepted from any node.
    Note that only one Verifier can be registered.
    @return true -> iServiceToolVerifier_c registered correctly
            false -> an iServiceToolVerifier_c is already registered
  */
  bool registerServiceToolVerifier (iServiceToolVerifier_c& stVerifier)
  {
    return __IsoAgLib::DiagnosticsServices_c::registerServiceToolVerifier(stVerifier);
  }

  /**
    Deregistering of the "iServiceToolVerifier_c"
    @return true -> iServiceToolVerifier_c deregistered correctly
            false -> wrong iServiceToolVerifier_c
  */
  bool deregisterServiceToolVerifier (iServiceToolVerifier_c& stVerifier)
  {
    return __IsoAgLib::DiagnosticsServices_c::deregisterServiceToolVerifier(stVerifier);
  }

  /**
    Activate DTC
    In case the DTC isn't yet registered, it will be added to the list of
    active DTCs with an OccurrenceCount of 1 and the activation will be
    reflected immediately through the next timeEvent in a DM1 message.
    If it was previously active, it will be reactivated and the OccurrenceCount
    incremented by 1. In case the DTC (SPN/FMI-pair) is already active, OccurrenceCount
    is not incremented and no DM1 message is sent. This is NOT a fault and
    OccurrenceCount is returned
    @return 0 : Activation Failure, too many DTCs registered
            1..65535 : Internal OccurrenceCount clipped to 65535.
            Note: This is clipped in the DM1 message to 0x7E.
  */
  uint16_t dtcActivate (uint32_t SPN, FailureModeIndicator_en FMI)
  {
    return __IsoAgLib::DiagnosticsServices_c::dtcActivate(SPN,FMI);
  }

  /**
    Deactivate DTC
    If it was active, it will be deactivated and the OccurrenceCount is returned.
    If it was already inactive, this is NOT a fault and OccurrenceCount is returned
    @return 0 : Deactivation Failure (DTC wasn't registered)
            1..65535 : Internal OccurrenceCount clipped to 65535.
            Note: This is clipped in the DM1 message to 0x7E.
  */
  uint16_t dtcDeactivate (uint32_t SPN, FailureModeIndicator_en FMI)
  {
    return __IsoAgLib::DiagnosticsServices_c::dtcDeactivate(SPN,FMI);
  }

  /**
    Clear the Previous Active Trouble Codes
    NOTE: This operation is normally controlled and executed by a Service Tool
          per remote can command (DM3 message), so it should only be called from
          the Application if it acts like a Service Tool itself.
          "Normal" applications don't need to call this function.
  */
  void serviceTool_dtcClearPrevious()
  {
    __IsoAgLib::DiagnosticsServices_c::serviceTool_dtcClearPrevious();
  }

#if DEBUG_DIAGNOSTICPGN
  bool isAtLeastOneDTCInCurrent() const { return __IsoAgLib::DiagnosticsServices_c::isAtLeastOneDTCInCurrent(); }
#endif

private:
  // this constructor is only needed to be compilable on some compilers.
  iDiagnosticsServices_c(__IsoAgLib::IdentItem_c &x) : DiagnosticsServices_c(x) {}
  ~iDiagnosticsServices_c() {}

private:
  friend class iIdentItem_c;
};

} // IsoAgLib
#endif
