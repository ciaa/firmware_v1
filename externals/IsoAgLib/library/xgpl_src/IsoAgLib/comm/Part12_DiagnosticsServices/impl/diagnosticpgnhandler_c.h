/*
  diagnosticpgnhandler_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef DIAGNOSTIC_PGN_HANDLER_C_H
#define DIAGNOSTIC_PGN_HANDLER_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisendeventhandler_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgnhandler_c.h>
#include <IsoAgLib/comm/Part12_DiagnosticsServices/impl/diagnosticfunctionalities_c.h>
#include <IsoAgLib/util/impl/bitfieldwrapper_c.h>

#include "../idiagnosticstypes.h"


namespace __IsoAgLib {

class IdentItem_c;

/**
  This class implements the handling of Diagnostic
  requests for a given IdentItem_c instance

  @author Nizar Souissi
  @author Martin Wodok
*/
class DiagnosticPgnHandler_c : public IsoRequestPgnHandler_c
{
public:
  DiagnosticPgnHandler_c (IdentItem_c&);
  virtual ~DiagnosticPgnHandler_c();

  void init();
  void close();

  bool setEcuIdentification(
    const char *partNr,
    const char *serialNr,
    const char *location,
    const char *type,
    const char *manufacturerName,
    const char *hardwareVer );

  bool setProductIdentification(
    const char *code,
    const char *brand,
    const char *model );

  bool setSwIdentification(
    const char *swIdentification );

  // only for internal usage when querying from the local application
  const char* getEcuIdentification() const { return mcstr_ecuIdentification; }
  const char* getSwIdentification() const { return mcstr_swIdentification; }

  //! Setter for the different certification message fields
  //! Parameter:
  //! @param ui16_year Certification year as in ISO 11783-7 A.29.1, must be between 2000 and 2061 
  //! @param a_revision Certification revision as in ISO 11783-7 A.29.2
  //! @param a_laboratoryType Certification laboratory type as in ISO 11783-7 A.29.3
  //! @param aui16_laboratoryId Certification laboratory ID (11 bits wide) as in ISO 11783-7 A.29.4
  //! @param aui16_referenceNumber Compliance certification reference number ( as in ISO 11783-7 A.29.18 )
  bool setCertificationData(
    uint16_t ui16_year,
    IsoAgLib::CertificationRevision_t,
    IsoAgLib::CertificationLabType_t,
    uint16_t aui16_laboratoryId,
    uint16_t aui16_referenceNumber );

  DiagnosticFunctionalities_c& getDiagnosticFunctionalities();

  virtual bool processMsgRequestPGN (uint32_t, IsoItem_c*, IsoItem_c*, int32_t );

private:
  void sendSinglePacket (const HUGE_MEM uint8_t* data, int32_t pgn);

private:
  class MultiSendEventHandlerProxy_c : public MultiSendEventHandler_c
  {
  private:
    virtual void reactOnStateChange(const SendStream_c& sendStream);
  };

  MultiSendEventHandlerProxy_c m_mrEventProxy;
  IdentItem_c& mrc_identItem;
  DiagnosticFunctionalities_c m_diagnosticFunctionalities;

  char *mcstr_ecuIdentification;
  char *mcstr_productIdentification;
  char *mcstr_swIdentification;
  bool mb_certificationIsSet;
  uint8_t m_certification[8];

  uint8_t m_protocol[8];

private:
  // not copyable
  DiagnosticPgnHandler_c(const DiagnosticPgnHandler_c&);
  DiagnosticPgnHandler_c& operator=(const DiagnosticPgnHandler_c&); 
};

} // __IsoAgLib

#endif
