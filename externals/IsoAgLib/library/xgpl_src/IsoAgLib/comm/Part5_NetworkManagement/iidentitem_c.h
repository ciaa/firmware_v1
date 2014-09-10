/*
  iidentitem_c.h: object for managing identity/ies of the ECU

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IIDENT_ITEM_H
#define IIDENT_ITEM_H

#include "impl/identitem_c.h"
#include "iisoitem_c.h"
#include "iisoname_c.h"

#include <IsoAgLib/comm/Part12_DiagnosticsServices/idiagnosticsservices_c.h>

namespace __IsoAgLib {
  class VtClient_c;
  class DevPropertyHandler_c;
  class ProprietaryMessageHandler_c;
  class ProprietaryMessageClient_c;
  class TcClientConnection_c;
}

namespace IsoAgLib {
class iVtClient_c;
class iProprietaryMessageHandler_c;
class iProprietaryMessageClient_c;
class iTimePosGps_c;
class iTracCert_c;
class iTracFacilities_c;
class iTracGeneral_c;
class iTracGuidanceCommand_c;
class iTracGuidance_c;
class iTracLight_c;
class iTracMoveSetPoint_c;
class iTracMove_c;
class iTracPTOSetPoint_c;
class iTracPTO_c;
class iTracAux_c;


typedef __IsoAgLib::DtcContainer_c iDtcContainer_c;

/**
  class to define an interface class for the storage a retrieved SA. Users can derive from
  and implement the load and store functions to their needs.
 */
class iIdentDataStorage_c {
  public:
    /** retrieve SA from user application
        @return SA to use - either from eeprom or smth else
     */
    virtual uint8_t loadSa() = 0;

    /** store SA in non volatile memory
        @param a_sa SA to store for next power cycle
      */
    virtual void storeSa( const uint8_t a_sa ) = 0;

    /** Application needs to fill in the stored DTCs. Note that all entries are disabled at
        default, so only fields with registered DTCs need to be filled in.
        @param arr_dtc
      */
    virtual void loadDtcs( IsoAgLib::iDtcContainer_c &arc_dtcContainer ) = 0;

    /** Application needs to store all registered DTCs from the list.
        All entries with "ui32_spn == spiNone" can be omitted.
        @param arr_dtc
      */
    virtual void storeDtcs( const IsoAgLib::iDtcContainer_c &arc_dtcContainer ) = 0;
};

/**
  class for a Control Function.
  new instances start in prepare address claim state and stay there for a random and serialNo dependent time;
  then they are inserted as announcing member in the monitoring list and start announcing;

  @short member ident item of this Control Function 
  @author Martin Wodok
  @author Dipl.-Inform. Achim Spangler
*/
class iIdentItem_c : private __IsoAgLib::IdentItem_c  {
public:
  /** init function for later start of address claim of an ISO identity (this can be only called once upon a default-constructed object)
      @param arc_isoname          proper initialized isoname for this identification
      @param arc_dataStorage      ident data storage handler implementation of iIdentDataStorage_c
      @param ai8_slaveCount       amount of attached slave devices; -1 == no master state, >= 0 is master
                                  in case an address claim for the slave devices shall be sent by this ECU, they
                                  must get their own IdentItem_c instance ( then with value -1 for ai8_slaveCount )
      @param apc_slaveIsoNameList pointer to list of IsoName_c values, where the slave devices are defined.
                                  IsoAgLib will then send the needed "master indicates its slaves" messages on BUS
      @param ab_enablediagnosticsServices enable or disable diagnostics services manager
    */
  void init( const iIsoName_c& arc_isoname,
             iIdentDataStorage_c& arc_storageHandler,
             int8_t ai8_slaveCount,
             const iIsoName_c* apc_slaveIsoNameList,
             bool ab_enablediagnosticsServices )
  {
    IdentItem_c::init( arc_isoname,
                       arc_storageHandler,
                       ai8_slaveCount,
                       apc_slaveIsoNameList,
                       ab_enablediagnosticsServices );
  }

  /** deliver pointer to IsoItem_c in IsoMonitor_c
      @return NULL -> either no ISO item or not yet registered in IsoMonitor_c
    */
  iIsoItem_c* getIsoItem( void ) const { return static_cast<iIsoItem_c*>(IdentItem_c::getIsoItem()); }

  /** check if the ident has claimed address */
  bool isClaimedAddress( void ) const { return IdentItem_c::isClaimedAddress(); }

  /**
    retreive the actual ISOName value of this ident item
    @return ISOName code of this ident item instance
  */
  const iIsoName_c& isoName() const {return IdentItem_c::isoName().toConstIisoName_c(); }

  // get the ISOBUS instance
  // NOTE: Calling this is only valid if this IdentItem is already
  // registered to an iIsoBus_c instance! Will throw an assertion otherwise!
  int getIsoBusInstance() const;
  // OBSOLETE: For backwards compatibility only, don't use in future implementations!
  int getMultitonInst() const { return getIsoBusInstance(); }

  /**
    Set ECU Identification fields, needed during the diagnostic procedure
    @return true if the fields were okay
            false if one field was too long (> 200 chars) or had '*' in it.
  */
  bool setEcuIdentification(
      const char *partNbr,
      const char *serialNbr,
      const char *location,
      const char *type,
      const char *manufacturerName,
      const char *hardwareVer)
  {
    return IdentItem_c::setEcuIdentification( partNbr, serialNbr, location, type, manufacturerName, hardwareVer );
  }

  /** Set SW Identification fields, needed during the diagnostic procedure
      @param astr_swId SwIdentification according to ISO 11783-12.
                       End fields with *, even the last one (and even if it is only one)
      @return true if the SW Identification was okay
              false if it was too long (> 200 chars) or empty
   */
  bool setSwIdentification( const char *swId )
  {
    return IdentItem_c::setSwIdentification( swId );
  }

  bool setProductIdentification(
      const char *code,
      const char *brand,
      const char *model )
  {
    return IdentItem_c::setProductIdentification( code, brand, model );
  }

  //! Setter for the different certification message fields
  //! Parameter:
  //! @param ui16_year Certification year as in ISO 11783-7 A.29.1, must be between 2000 and 2061
  //! @param a_revision Certification revision as in ISO 11783-7 A.29.2
  //! @param a_laboratoryType Certification laboratory type as in ISO 11783-7 A.29.3
  //! @param aui16_laboratoryId Certification laboratory ID (11 bits wide) as in ISO 11783-7 A.29.4
  //! @param aui16_referenceNumber Compliance certification reference number as in ISO 11783-7 A.29.18
  //! @return true all parameters were okay in range
  //!         false at least one parameter was wrong
  bool setCertificationData(
      uint16_t ui16_year,
      IsoAgLib::CertificationRevision_t a_revision,
      IsoAgLib::CertificationLabType_t a_laboratoryType,
      uint16_t aui16_laboratoryId,
      uint16_t aui16_referenceNumber)
  {
    return IdentItem_c::setCertificationData(
        ui16_year,
        a_revision,
        a_laboratoryType,
        aui16_laboratoryId,
        aui16_referenceNumber);
  }

  //! Get Diagnostics Services manager
  //! @return a valid pointer if the Diagnostics Services is defined 
  //!         a NULL pointer otherwise
  iDiagnosticsServices_c* getIdiagnosticsServices()
  {
    return static_cast<iDiagnosticsServices_c*>(IdentItem_c::getDiagnosticsServices());
  }

private:
  friend class __IsoAgLib::ProprietaryMessageHandler_c;
  friend class __IsoAgLib::VtClient_c;
  friend class __IsoAgLib::TcClientConnection_c;
  friend class iDevPropertyHandler_c;
  friend class iFsManager_c;
  friend class iIsoMonitor_c;
  friend class iProcData_c;
  friend class iProprietaryMessageA_c;
  friend class iProprietaryMessageB_c;
  friend class iProprietaryMessageClient_c;
  friend class iProprietaryMessageHandler_c;
  friend class iTimePosGps_c;
  friend class iTracAux_c;
  friend class iTracCert_c;
  friend class iTracFacilities_c;
  friend class iTracGeneral_c;
  friend class iTracGuidanceCommand_c;
  friend class iTracGuidance_c;
  friend class iTracLight_c;
  friend class iTracMoveSetPoint_c;
  friend class iTracMove_c;
  friend class iTracPTOSetPoint_c;
  friend class iTracPTO_c;
  friend class iIsbClient_c;
  friend class iVtClientConnection_c;
  friend class iVtClient_c;
  friend class iTcClientConnection_c;
  friend class iTcClient_c;
};


inline int
iIdentItem_c::getIsoBusInstance() const
{
  int inst = __IsoAgLib::IdentItem_c::getMultitonInst();
  isoaglib_assert( inst >= 0 );
  return inst;
}

} // IsoAgLib

#endif
