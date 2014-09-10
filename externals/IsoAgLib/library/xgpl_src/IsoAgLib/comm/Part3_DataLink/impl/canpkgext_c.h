/*
  canpkgext_c.h: class for an ISO-supported CanPkg_c object

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef CAN_PKG_EXT_H
#define CAN_PKG_EXT_H

#include <IsoAgLib/driver/can/impl/canpkg_c.h>
#include <IsoAgLib/util/impl/util_funcs.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoname_c.h>


namespace __IsoAgLib {

class IsoItem_c;


/** structure which will be filled when an address is resolved */
class AddressResolveResults_c
{
private:
  /** not copyable (due to Ident_c& and no need to be copyable! */
  AddressResolveResults_c& operator=(const AddressResolveResults_c&); 
  AddressResolveResults_c(const AddressResolveResults_c&);

public:
  AddressResolveResults_c(Ident_c& arc_ident, uint8_t aui8_position);
  ~AddressResolveResults_c();

  uint8_t getAddress() const {return mrc_ident.ident(mui8_position);}
  void setAddress(uint8_t aui8_newAddress) { mrc_ident.setByte(aui8_newAddress,mui8_position); }

  IsoName_c mc_isoName;
  IsoItem_c* mpc_monitorItem;

  Ident_c& mrc_ident;
  uint8_t mui8_position; //can be source or destination address
};


typedef enum Scope_en
{
  local  = 0,
  remote = 1
} Scope;


/** values which indicate the state of the received can-message
   Note: For a valid resolving/packet
     - DA may be FF
     - SA may be FE
   --> The ISO-modules need to check if DE==FF is wanted and SA==FE is allowed!
 */
typedef enum MessageState_en {
  MessageValid        = 0,

  AdrResolveMask      = (0x3<<0), // AdrResolve wraps SA and DA into one!
  AdrValid            = (0x0<<0),
  AdrInvalid          = (0x3<<0), // any of the SaInvalid/DaInvalid set.

  // from here on it's just additional information -
  // decisions can be purely taken on the flags above!
  SaValidationMask    = (0x3<<4),
  SaValid             = (0x0<<4),
  SaInvalidUnknown    = (0x1<<4),
  SaInvalidGlobal     = (0x2<<4),
  SaInvalidLocal      = (0x3<<4),

  DaValidationMask    = (0x3<<6),
  DaValid             = (0x0<<6),
  DaInvalidUnknown    = (0x1<<6),
  DaInvalidAnonymous  = (0x2<<6),
  DaInvalidRemote     = (0x3<<6),
} MessageState_t;


/** class for an ISO-supported CanPkg_c object */
class CanPkgExt_c : public CanPkg_c
{
private:
  /** not copyable (due to AddressResolveResults_c and no need to be copyable! */
  CanPkgExt_c(const CanPkgExt_c&);
  CanPkgExt_c& operator=(const CanPkgExt_c&); 

public:
  CanPkgExt_c();
  CanPkgExt_c( const CanPkg_c&, int ai_multitonInst );
  virtual ~CanPkgExt_c();

  // Note: FE is considered here a VALID SA!
  bool isValid() const { return (mt_msgState == MessageValid); }

  /**
    get the value of the ISO11783 ident field SA
    @return source adress
  */
  uint8_t isoSa() const;

  /**
    get the value of the ISO11783 ident field PGN
    @return parameter group number
  */
  uint32_t isoPgn() const {return ( ( ( (uint32_t)ident() >> 8) & 0x3FF00LU ) | isoPs() );}

  /**
    get the value of the ISO11783 ident field EDP+DP
    @return data page
  */
  uint8_t isoDp() const {return (ident(3) & 0x3);}

  /**
    get the value of the ISO11783 ident field PF
    @return PDU Format
  */
  uint8_t isoPf() const {return ident(2);}

  /**
    get the value of the ISO11783 ident field PS
    @return PDU Specific
  */
  uint8_t isoPs() const;

  /**
    get the value of the ISO11783 ident field PRI
    @return priority
  */
  uint8_t isoPri() const {return uint8_t(ident(3) >> 2);}

  bool isPdu1() const { return (isoPf() <= 0xEF); }
  bool isPdu2() const { return (isoPf() >= 0xF0); }
  bool hasDa() const { return isPdu1(); }
  uint32_t isoPurePgn() const { return ( ((ident() >> 8) & 0x3FF00LU) | (hasDa() ? 0 : isoPs()) ); }

  /**
    set the value of the ISO11783 ident field SA
    @return source adress
  */
  void setIsoSa(uint8_t aui8_val);

  /**
    set the value of the ISO11783 ident field PGN
    @return parameter group number
  */
  void setIsoPgn(uint32_t aui32_val);

  /**
    set the value of the ISO11783 ident field DP
    @return data page
  */
  void setIsoDp(uint8_t aui8_val) {setIdentByte( uint8_t((ident(3)& 0x1C) | (aui8_val & 0x03)), 3 );}

  /**
    set the value of the ISO11783 ident field PF
    @return PDU Format
  */
  void setIsoPf(uint8_t aui8_val) {setIdentByte(aui8_val, 2 ); }

  /** combined setting of DP and PF field in identifier as 10Bit part of a 16-Bit parameter
      @param aui16_dpPf DP and PF for the PGN
    */
  void setIsoDpPf( uint16_t aui16_dpPf ) { setIdentWord( uint16_t(((ident(3)& 0x1C)<<8) | (aui16_dpPf & 0x03FF)), 1 );}

  /**
    set the value of the ISO11783 ident field PS
    @return PDU Specific
  */
  void setIsoPs(uint8_t aui8_val);

  /**
    set the value of the ISO11783 ident field PRI
    @return priority
  */
  void setIsoPri(uint8_t aui8_val){setIdentByte( uint8_t((ident(3)&3) | (aui8_val << 2)), 3 );}

  // This function sets the DLC to 8 and fills up
  // the databytes AFTER lenActualData with 0xFF
  // Example: WS-Master has only first databyte used, rest is filled with 0xFFs
  //          call setLen8FillUpWithReserved(1)
  void setLen8FillUpWithReserved (uint8_t lenActualData)
  {
    for (; lenActualData < 8; lenActualData++)
      setUint8Data(lenActualData, 0xFF);
    setLen(8);
  }

  void setExtCanPkg8(uint8_t pri, uint8_t dp, uint8_t pf, uint8_t ps, uint8_t sa, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) {
    setIsoPri(pri);
    setIsoDp(dp);
    setIsoPf(pf);
    setIsoPs(ps);
    setIsoSa(sa);
    setUint8Data (0, d0);
    setUint8Data (1, d1);
    setUint8Data (2, d2);
    setUint8Data (3, d3);
    setUint8Data (4, d4);
    setUint8Data (5, d5);
    setUint8Data (6, d6);
    setUint8Data (7, d7);
    setLen (8);
  }

  /** check if source and destination address are valid
      @see     CanPkgExt_c::operator<<
      @pre     we want to send a message
      @return  Valid -> both addresses are valid
               Invalid -> one or both addresses are invalid
               OnlyNetworkMgmt -> one or both addresses are only useable for network management
    */
  bool resolveSendingInformation( int ai_multitonInst );

  /** set the monitoritem for resolve SA
      @param apc_monitorItem  needed monitoritem
    */
  void setMonitorItemForSA( IsoItem_c* apc_monitorItem );

  /** set the isoName for resolve SA
      @param acrc_isoName        needed isoName
    */
  void setISONameForSA( const IsoName_c& acrc_isoName );

  /** set the monitoritem for resolve SA
      @param apc_monitorItem  needed monitoritem
    */
  void setMonitorItemForDA( IsoItem_c* apc_monitorItem );

  /** set the isoName for resolve SA
      @param acrc_isoName        needed isoName
    */
  void setISONameForDA( const IsoName_c& acrc_isoName );

  /** short inline function for setting the Destination address (PS) to global (0xFF)
    */
  void setGlobalDA() { setIsoPs (0xFF); }

  /** get the monitoritem for resolved SA
    */
  IsoItem_c* getMonitorItemForSA() const { return mc_addrResolveResSA.mpc_monitorItem; }

  /** get the isoName for resolved SA
    */
  const IsoName_c& getISONameForSA() const { return mc_addrResolveResSA.mc_isoName; }

  /** set the monitoritem for resolved SA
    */
  IsoItem_c* getMonitorItemForDA() const { return mc_addrResolveResDA.mpc_monitorItem; }

  /** set the isoName for resolved DA
    */
  const IsoName_c& getISONameForDA() const { return mc_addrResolveResDA.mc_isoName; }

private:
  /** check if source and destination address are valid - called from the c'tor */
  MessageState_t resolveReceivingInformation( int ai_multitonInstance );

  bool resolveAddress(AddressResolveResults_c& arc_addressResolveResults, int ai_multitonInstance );

  /** report if the combination of address and scope is valid in context of message processing
      @return  true -> address, scope combination is valid
    */
  MessageState_t address2IdentRemoteSa( int ai_multitonInstance );

  /** report if the combination of address and scope is valid in context of message processing
      @return  true -> address, scope combination is valid
    */
  MessageState_t address2IdentLocalDa( int ai_multitonInstance );

  /** set address in context of sending a message
      @param  arc_addressResolveResults  source or destination address
      @param  scope                  local or remote
      @return  true -> monitoritem_c, isoName_c is a valid combination
    */
  MessageState_t setAddress(AddressResolveResults_c& arc_addressResolveResults, Scope scope);

  /** resolve a given monitoritem and get address if possible
      @param arc_addressResolveResults  address to resolve
      @return true -> monitoritem could be resolved
              false -> nothing more to be done
    */
  bool resolveMonitorItem( AddressResolveResults_c& arc_addressResolveResults, int ai_multitonInstance  );

private:
  AddressResolveResults_c mc_addrResolveResSA;
  AddressResolveResults_c mc_addrResolveResDA;

  MessageState_t mt_msgState;
};

} // __IsoAgLib

#endif
