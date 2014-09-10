/*
  iisoname.h: handles the ISO 64bit NAME field

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IISO_NAME_H
#define IISO_NAME_H

#include "impl/isoname_c.h"

namespace __IsoAgLib
{ // forward declarations
  class ProprietaryMessageHandler_c;
  class ProprietaryMessageClient_c;
  class TimePosGps_c;
  class Aux2Functions_c;
}


namespace IsoAgLib {

class iEepromIo_c;

/** handle the 64bit ISO11783 NAME field with
  input/output from/to 8byte string and specific
  read/write access to single flags
  @author Dipl.-Inform. Achim Spangler
*/
#ifdef __IAR_SYSTEMS_ICC__
// Using IAR it is not allowed to static_cast from private or protected base.
class iIsoName_c : public __IsoAgLib::IsoName_c
#else
class iIsoName_c : private __IsoAgLib::IsoName_c
#endif
{
public:
  /** constant for default parameters and initialization, where the device type is not yet spcified.
      the instantiation of this constant variable is located in the module cancustomer_c.cpp
    */
  static const iIsoName_c& iIsoNameUnspecified() { return IsoNameUnspecified().toConstIisoName_c(); }

  /** constructor which format data string from series of input flags
    @param ab_selfConf true -> indicate sefl configuring ECU
    @param aui8_indGroup industry group of device (2 for agriculture)
    @param aui8_devClass device class of ECU
    @param aui8_devClassInst instance number of ECU with same devClass in the network
    @param ab_func function of the ECU (usual 25 for network interconnect)
    @param aui16_manufCode code of manufactor (11bit)
    @param aui32_serNo serial no of specific device (21bit)
    @param ab_funcInst instance number of ECU with same function and device class
        (default 0 - normally)
    @param ab_funcInst instance number of ECU with same function, device class and function instance
        (default 0 - normally)
  */
  iIsoName_c(bool ab_selfConf, uint8_t aui8_indGroup, uint8_t aui8_devClass, uint8_t aui8_devClassInst,
        uint8_t ab_func, uint16_t aui16_manufCode, uint32_t aui32_serNo, uint8_t ab_funcInst = 0, uint8_t ab_ecuInst = 0)
        : IsoName_c( ab_selfConf, aui8_indGroup, aui8_devClass, aui8_devClassInst, ab_func, aui16_manufCode, aui32_serNo, ab_funcInst, ab_ecuInst ) {}

  /** constructor which can read in initial data from uint8_t string
    @param apb_src 64bit input data string, mustn't be NULL.
                   (if NULL, currently all fields are set to 0, but future behaviour is not defined.)
  */
  iIsoName_c(const uint8_t* apb_src) : IsoName_c(apb_src) {}

  /** (default) constructor which will set the IsoName to Unspecified */
  iIsoName_c() : IsoName_c() {}

  /** copy constructor for ISOName
    @param acrc_src source IsoName_c instance
  */
  iIsoName_c(const iIsoName_c& acrc_src) : IsoName_c( acrc_src ) {}

  /** assign constructor for ISOName
    @param acrc_src source IsoName_c object
  */
  const iIsoName_c& operator=(const iIsoName_c& acrc_src)
    { return IsoName_c::operator=( acrc_src ).toConstIisoName_c();}

  /** compare two iIsoName_c values with operator<
      NOTE: The NAMEs are being compared based on the PRIORITY
            and not on the numeric value.
      NOTE: A NAME has a lower priority if it has a higher numeric value!
  */
  bool operator<( const iIsoName_c& rc_right ) const
    { return IsoName_c::operator < (rc_right);}

  /** compare two IsoName_c values with operator== */
  bool operator==( const iIsoName_c& rc_right ) const
    { return IsoName_c::operator==(rc_right);}

  /** compare two IsoName_c values with operator!= */
  bool operator!=( const iIsoName_c& rc_right ) const
    { return IsoName_c::operator!=(rc_right);}

  /** set data string with all flags with one call
    @param ab_selfConf true -> indicate sefl configuring ECU
    @param aui8_indGroup industry group of device (2 for agriculture)
    @param aui8_devClass device class of ECU
    @param aui8_devClassInst instance number of ECU with same devClass in the network
    @param ab_func function of the ECU (usual 25 for network interconnect)
    @param aui16_manufCode code of manufactor (11bit)
    @param aui32_serNo serial no of specific device (21bit)
    @param ab_funcInst instance number of ECU with same function and device class
        (default 0 - normally)
    @param ab_funcInst instance number of ECU with same function, device class and function instance
        (default 0 - normally)
  */
  void set(bool ab_selfConf, uint8_t aui8_indGroup, uint8_t aui8_devClass, uint8_t aui8_devClassInst,
        uint8_t ab_func, uint16_t aui16_manufCode, uint32_t aui32_serNo, uint8_t ab_funcInst = 0, uint8_t ab_ecuInst = 0)
    { IsoName_c::set( ab_selfConf, aui8_indGroup, aui8_devClass, aui8_devClassInst, ab_func, aui16_manufCode, aui32_serNo, ab_funcInst, ab_ecuInst );}


  /** deliver the data NAME string as pointer to 8byte string
    NOTE: make sure that the NAME is/was "specified"!
    @return const pointer to 8 uint8_t string with NAME
  */
  const uint8_t* outputString() const {return IsoName_c::outputString();}

  /** get self config mode
    @return self configuration adress state
  */
  bool selfConf() const { return IsoName_c::selfConf();}

  /** get industry group code
    @return industry group of device
  */
  uint8_t indGroup() const { return IsoName_c::indGroup();}

  /** get device class instance number
    @return:device class instance number
  */
  uint8_t devClassInst() const { return IsoName_c::devClassInst();}

  /** get device class code
    @return:device class
  */
  uint8_t devClass() const { return IsoName_c::devClass();}

  /** get function code
    @return function code
  */
  uint8_t func() const { return IsoName_c::func();}

  /** get function instance code
    @return function instance code
  */
  uint8_t funcInst() const { return IsoName_c::funcInst();}

  /** get ECU instance code
    @return ECU instance code
  */
  uint8_t ecuInst() const { return IsoName_c::ecuInst();}

  /** get manufactor code
    @return manufactor code
  */
  uint16_t manufCode() const { return IsoName_c::manufCode();}

  /** get serial number
    @return serial number
  */
  uint32_t serNo() const { return IsoName_c::serNo();}

  /** set the NAME data from 8 uint8_t string
    @param apb_src pointer to 8byte source string
  */
  void inputString(const uint8_t* apb_src) { IsoName_c::inputString( apb_src );}

  /** set self config mode
    @param ab_selfConf true -> indicate sefl configuring ECU
  */
  void setSelfConf(bool ab_selfConf) { IsoName_c::setSelfConf( ab_selfConf );}

  /** set industry group code
    @param aui8_indGroup industry group of device (2 for agriculture)
  */
  void setIndGroup(uint8_t aui8_indGroup) { IsoName_c::setIndGroup( aui8_indGroup );}

  /** set device class instance number
    @param aui8_devClassInst instance number of ECU with same devClass
          in the network
  */
  void setDevClassInst(uint8_t aui8_devClassInst) { IsoName_c::setDevClassInst( aui8_devClassInst );}

  /** set device class code
    @param aui8_devClass device class of ECU
  */
  void setDevClass(uint8_t aui8_devClass) { IsoName_c::setDevClass( aui8_devClass );}

  /** set function code
    @param ab_func function of the ECU (usual 25 for network interconnect)
  */
  void setFunc(uint8_t ab_func) { IsoName_c::setFunc( ab_func );}

  /** set function instance code
    @param ab_funcInst instance number of ECU with same function and device class
        (default 0 - normally)
  */
  void setFuncInst(uint8_t ab_funcInst) { IsoName_c::setFuncInst( ab_funcInst );}

  /** set ECU instance code
    @param ab_ecuInst instance number of ECU with same function, device class and function instance
        (default 0 - normally)
  */
  void setEcuInst(uint8_t ab_ecuInst) { IsoName_c::setEcuInst( ab_ecuInst );}

  /** set manufactor code
    @param aui16_manufCode code of manufactor (11bit)
  */
  void setManufCode(uint16_t aui16_manufCode) { IsoName_c::setManufCode( aui16_manufCode );}

  /** set serial number (Identity Number)
    @param aui32_serNo serial no of specific device (21bit)
  */
  void setSerNo(uint32_t aui32_serNo) { IsoName_c::setSerNo( aui32_serNo );}

  /** set this instance to indicator for specified value - with the NAME prior set
   * Attention: Make sure that the NAME was properly set before or will be properly
   *            set at latest before using this instance! */
  void setSpecified( void ) { IsoName_c::setSpecified( );}

  /** set this instance to indicator for unspecified value */
  void setUnspecified( void ) { IsoName_c::setUnspecified( );}

  /** check if this instance has specified value */
  bool isSpecified( void ) const { return IsoName_c::isSpecified( );}

  /** check if this instance has unspecified value (no NAME fields set up) */
  bool isUnspecified( void ) const { return IsoName_c::isUnspecified( );}

private:
  friend class iIsoItem_c;
  friend class iIdentItem_c;
  friend class iCanPkgExt_c;
  friend class iIsoMonitor_c;
  friend class iTimePosGps_c;
  friend class iTracMove_c;
  friend class iTracMoveSetPoint_c;
  friend class iTracGeneral_c;
  friend class iTracPTO_c;
  friend class iTracPTOSetPoint_c;
  friend class iTracLight_c;
  friend class iTracFacilities_c;
  friend class iTracAux_c;
  friend class iTracCert_c;
  friend class iTracGuidance_c;
  friend class iTracGuidanceCommand_c;
  friend class IsoName_c;
  friend class iTcClient_c;
  friend class iProcData_c;
  friend class iVtObjectAuxiliaryFunction2_c;
  friend iEepromIo_c& operator<<(iEepromIo_c& rc_stream, const iIsoName_c& rc_data );
  friend iEepromIo_c& operator>>(iEepromIo_c& rc_stream, iIsoName_c& rc_data );
  friend struct iIsoFilter_s;
  friend class iProprietaryMessageHandler_c;
  friend class iProprietaryMessageA_c;
  friend class iProprietaryMessageB_c;
  friend class __IsoAgLib::ProprietaryMessageClient_c;
  friend class __IsoAgLib::ProprietaryMessageHandler_c;
  friend class __IsoAgLib::TimePosGps_c;
  friend class __IsoAgLib::Aux2Functions_c;
};


}
#endif
