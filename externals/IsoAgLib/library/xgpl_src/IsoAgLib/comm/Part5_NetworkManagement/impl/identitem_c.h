/*
  identitem_c.h: object for managing identity/ies of the ECU

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IDENT_ITEM_H
#define IDENT_ITEM_H

#include "baseitem_c.h"
#include "isoname_c.h"
#include "isoitem_c.h"

#include <IsoAgLib/comm/Part12_DiagnosticsServices/idiagnosticstypes.h>
#include <IsoAgLib/comm/Part12_DiagnosticsServices/impl/diagnosticpgnhandler_c.h>

#include <IsoAgLib/driver/system/isystem_c.h>

// forward declaration of iIdentDataStorage_c - no internal definition
namespace IsoAgLib {
  class iIdentItem_c;
  class iIdentDataStorage_c;
};

// Begin Namespace __IsoAgLib
namespace __IsoAgLib {

  class DiagnosticsServices_c;
  class DiagnosticFunctionalities_c;

/**
  class for identity/ies (Control Function(s)) which are managed by the actual ECU;
  new instances start in prepare address claim state and stay there for a random and serialNo dependent time;
  then they are inserted as announcing member in the monitoring list and start announcing;

  IsoAgLib starts the address claim procedure after the init function is called and the iIdentItem
  is registered at iIsoMonitor with registerIdentItem().

  The IsoAgLib backend is responsible for answering all requests like
  RequestForClaimedSourceAdress ( ISO 11783 )

  The application just has to call the main trigger function Scheduler_c::timeEvent().

  After the monitor list report a completed address claim the status changes to active/claimed address;

  @short member ident item of this Control Function
  @author Dipl.-Inform. Achim Spangler
  */
class IdentItem_c : public BaseItem_c
{
public:
  IdentItem_c();
  ~IdentItem_c();

  void init( const IsoName_c& arc_isoNameParam,
             IsoAgLib::iIdentDataStorage_c& arc_dataStorage,
             int8_t ai8_slaveCount,
             const IsoName_c* apc_slaveIsoNameList,
             bool ab_enablediagnosticsServices);

  /** IsoMonitor uses this function to activate/start this Item
    @param ai_multitonInst Activate this Item on the given IsoBus
    @return false in case this Item wasn't properly set up before
                  using some init(..)-call.
  */
  bool activate (int ai_multitonInst);

  /** IsoMonitor uses this function to deactivate/stop this Item
      For later (again) use, it needs to be initialized again.
  */
  void deactivate();

  IsoAgLib::iIdentItem_c& toIidentItem_c();

  IsoItem_c* getIsoItem() const { return mpc_isoItem; }

  const IsoName_c& isoName() const { return mc_isoName; }

  /** deliver reference to contained IsoName_c of this IdentItem_c
    * ATTENTION: Use wisely!
    *            Only modify an IdentItem's ISOName if an address is NOT yet claimed!!!!!
    */
  IsoName_c& modifyableIsoName() { return mc_isoName; }

  /** restart SA claim procedure.
      When ISONAME conflict gets detected between creation of corresponding IsoItem_c and sending of SA-Claim,
      a complete restart of IdentItem_c life cycle beginning with PreAddressClaim.
    */
  void restartWithPreAddressClaim( void );

  /** Go Offline by:
    * + reset IdentItem::IState_c to IState_c::Off / OffUnable
    * + remove pointed IsoItem_c node and the respective pointer
    * @param ab_explicitlyOffByUser ("Off" if explicitly called by user (true), "OffUnable" if we are unable to keep a claimed address (false))
    */
  void goOffline (bool ab_explicitlyOffByUser);

  /** NOTE: This timeEvent is not SchedulerTask_c's! */
  void timeEvent();

  /** check if the ident has claimed address */
  bool isClaimedAddress( void ) const { return (getIsoItem() != NULL) ? (getIsoItem()->itemState (IState_c::ClaimedAddress)) : (itemState (IState_c::ClaimedAddress)); }

#ifdef USE_WORKING_SET
  bool isMaster() const { return (mpvec_slaveIsoNames != NULL); }
#endif

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

  bool setSwIdentification( const char *partNbr );

  const DiagnosticPgnHandler_c& getDiagnosticPgnHandler() const { return m_diagnosticPgnHandler; }
  DiagnosticsServices_c* getDiagnosticsServices() { return mpc_diagnosticsServices; }
  DiagnosticFunctionalities_c& getDiagnosticFunctionalities();

  bool setCertificationData(
    uint16_t ui16_year,
    IsoAgLib::CertificationRevision_t a_revision,
    IsoAgLib::CertificationLabType_t a_laboratoryType,
    uint16_t aui16_laboratoryId,
    uint16_t aui16_referenceNumber );

  void updateLastIsoSaRequestForThisItem() { i32_lastIsoSaRequestForThisItem = HAL::getTime(); }
  int32_t getLastIsoSaRequestForThisItem() const { return i32_lastIsoSaRequestForThisItem; }

protected:
  /** periodically called functions do perform
      time dependent actions in prepare address claim state
      -> insert item in appropriate monitor lists and initiate address claim
    */
  void timeEventPreAddressClaim( void );

  /** periodically called functions do perform
      time dependent actions in active (address claim/claimed address) state
      -> call timeEvent for corresponding items in MemberMonitor/ISOMonitor
      -> eventually store SA via IdentDataStorage_c.
    */
  void timeEventActive( void );

private:
  // not copyable!
  IdentItem_c(const IdentItem_c& acrc_src);
  IdentItem_c& operator=(const IdentItem_c& /* acrc_src */);


private:
  IsoItem_c* mpc_isoItem;

  IsoAgLib::iIdentDataStorage_c* mpc_dataStorageHandler; // not used if == NULL

  uint8_t mui8_sa;
  IsoName_c mc_isoName;

  DiagnosticPgnHandler_c m_diagnosticPgnHandler;
  DiagnosticsServices_c* mpc_diagnosticsServices;

  #ifdef USE_WORKING_SET
  /** pointer to a list of all slave nodes represented by their ISO-Name
   * if this pointer is != NULL, this item IS a master and the list's size is the number of associated slaves.
   * if this pointer is == NULL, this item is NOT a master.
   * It may be a slave - this can be determined by searching
   * through all Items' list for this Item's IsoName!
   */
  STL_NAMESPACE::vector<IsoName_c>* mpvec_slaveIsoNames;
  #endif

  int32_t i32_lastIsoSaRequestForThisItem;

  bool mb_readyForActivation; // protect this ident from beeing initialized more than once
};

}

#endif
