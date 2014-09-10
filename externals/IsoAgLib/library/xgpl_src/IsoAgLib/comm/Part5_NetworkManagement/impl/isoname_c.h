/*
  isoname_c.h: handles the ISO 64bit NAME field

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISO_NAME_H
#define ISO_NAME_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/flexiblebytestrings.h>
#include <IsoAgLib/util/iassert.h>


namespace IsoAgLib {
  class iIsoName_c;
}

namespace __IsoAgLib {

/** handle the 64bit ISO11783 NAME field with input/output from/to 8byte string
  and specific read/write access to single flags. Additionally it can be set
  to "unspecified", which has a special meaning in the different modules.
  When set to unspecified, the NAME will still be kept as is and can be
  "reactivated" by setting it to specified again.
  @author Martin Wodok (originally Achim Spangler)
*/
class IsoName_c {
public:
  /** constant for default parameters and initialization, where the device type is not yet spcified.
    */
  static const IsoName_c& IsoNameUnspecified();

  /** constructor which can read in initial data from uint8_t string
    @param apb_src 64bit input data string, mustn't be NULL.
                   (if NULL, currently all fields are set to 0, but future behaviour is not defined.)
  */
  inline IsoName_c(const uint8_t* apb_src);

  /** (default) constructor which will set the IsoName to Unspecified */
  inline IsoName_c();

  /** constructor which can read in initial data from uint8_t string
    @param apu_src 64bit input data string
  */
  inline IsoName_c(const Flexible8ByteString_c* apu_src);

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
  inline IsoName_c(bool ab_selfConf, uint8_t aui8_indGroup, uint8_t aui8_devClass, uint8_t aui8_devClassInst,
                   uint8_t ab_func, uint16_t aui16_manufCode, uint32_t aui32_serNo, uint8_t ab_funcInst = 0, uint8_t ab_ecuInst = 0);

  /** copy constructor for ISOName
    @param acrc_src source IsoName_c instance
  */
  inline IsoName_c(const IsoName_c& acrc_src);

  /** default destructor */
  inline ~IsoName_c();

  /** set data string with all flags with one call - and set to specified!
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
        uint8_t ab_func, uint16_t aui16_manufCode, uint32_t aui32_serNo, uint8_t ab_funcInst = 0, uint8_t ab_ecuInst = 0);

  /** set this instance to indicator for specified value - with the NAME prior set
   * Attention: Make sure that the NAME was properly set before or will be properly
   *            set at latest before using this instance! */
  void setSpecified( void ) { m_specified = true; }

  /** set this instance to indicator for unspecified value - and keep the NAME */
  void setUnspecified( void ) { m_specified = false; }

  /** check if this instance has specified value */
  bool isSpecified( void ) const { return m_specified; }

  /** check if this instance has unspecified value (no NAME fields set up) */
  bool isUnspecified( void ) const { return !isSpecified(); }

  /** IsoAgLib-specific enum for often used types of "functions" of IsoNames */
  enum ecuType_t {
    ecuTypeANYOTHER, // not one of the following
    ecuTypeTractorECU,
    ecuTypeTaskControl,
    ecuTypeVirtualTerminal,
    ecuTypeFileServerOrPrinter,
    ecuTypeNavigation,
    ecuTypeDataLogger,
  };

  /** deliver type of ECU: Tractor, VT, Navigation, etc. */
  ecuType_t getEcuType() const;


  /** deliver the data NAME string as pointer to 8byte string
    NOTE: Make sure that this instance is Specified or has all NAME fields properly set up
    @return const pointer to 8 uint8_t string with NAME
  */
  const uint8_t* outputString() const{return mu_data.getUint8DataConstPointer();}

  /** deliver the data NAME string as pointer to internal Flexible8ByteString_c
    NOTE: Make sure that this instance is Specified or has all NAME fields properly set up
    @return const pointer to 8 uint8_t string with NAME
  */
  const Flexible8ByteString_c* outputUnion() const{return &mu_data;}

  /** get self config mode
    @return self configuration adress state
  */
  inline bool selfConf() const;

  /** get industry group code
    @return industry group of device
  */
  inline uint8_t indGroup() const;

  /** get device class instance number
    @return:device class instance number
  */
  inline uint8_t devClassInst() const;

  /** get device class code
    @return:device class
  */
  inline uint8_t devClass() const;

  /** get function code
    @return function code
  */
  inline uint8_t func() const;

  /** get function instance code
    @return function instance code
  */
  inline uint8_t funcInst() const;

  /** get ECU instance code
    @return ECU instance code
  */
  inline uint8_t ecuInst() const;

  /** get manufactor code
    @return manufactor code
  */
  inline uint16_t manufCode() const;

  /** get serial number
    @return serial number
  */
  inline uint32_t serNo() const;

  /** set the NAME data from 8 uint8_t string - sets this instance to Specified.
    @param apb_src pointer to 8byte source string, mustn't be NULL!
  */
  inline void inputString(const uint8_t* apb_src);

  /** set the NAME data from Flexible8ByteString_c - sets this instance to Specified.
    @param apu_src pointer to 8byte source string, mustn't be NULL!
  */
  inline void inputUnion(const Flexible8ByteString_c* apu_src);

  /** set self config mode
    @param ab_selfConf true -> indicate sefl configuring ECU
  */
  inline void setSelfConf(bool ab_selfConf);

  /** set industry group code
    @param aui8_indGroup industry group of device (2 for agriculture)
  */
  inline void setIndGroup(uint8_t aui8_indGroup);

  /** set device class instance number
    @param aui8_devClassInst instance number of ECU with same devClass in the network
  */
  inline void setDevClassInst(uint8_t aui8_devClassInst);

  /** set device class code
    @param aui8_devClass device class of ECU
  */
  inline void setDevClass(uint8_t aui8_devClass);

  /** set function code
    @param ab_func function of the ECU (usual 25 for network interconnect)
  */
  inline void setFunc(uint8_t ab_func);

  /** set function instance code
    @param ab_funcInst instance number of ECU with same function and device class
        (default 0 - normally)
  */
  inline void setFuncInst(uint8_t ab_funcInst);

  /** set ECU instance code
    @param ab_ecuInst instance number of ECU with same function, device class and function instance
        (default 0 - normally)
  */
  inline void setEcuInst(uint8_t ab_ecuInst);

  /** set manufactor code
    @param aui16_manufCode code of manufactor (11bit)
  */
  inline void setManufCode(uint16_t aui16_manufCode);

  /** set serial number (Identity Number)
    @param aui32_serNo serial no of specific device (21bit)
  */
  inline void setSerNo(uint32_t aui32_serNo);

  /** Check if all Non-Instance fields of both ISONames match
   * Attention: Ignores if the NAMEs are specified or not!
   * @return true if equal, false if one non-inst field differs!
  */
  bool isEqualRegardingNonInstFields (const IsoName_c& acrc_isoName) const;

  /** assignment operator */
  inline const IsoName_c& operator=(const IsoName_c& src)
    { mu_data = src.mu_data; m_specified = src.m_specified; return src; }

  /** compare two IsoName_c values with operator== */
  inline bool operator==( const IsoName_c& right ) const
    { return( (m_specified == true) && (right.m_specified == true) )
        ? (mu_data == right.mu_data)
        : (m_specified == right.m_specified); }

  /** compare two IsoName_c values with operator!= */
  inline bool operator!=( const IsoName_c& right ) const
    { return( (m_specified == true) && (right.m_specified == true) )
        ? operator!= (right.mu_data)
        : (m_specified != right.m_specified); }

  /** compare IsoName_c-NAME value (regardless of "Specified")
      and Flexible8ByteString_c with operator!= */
  inline bool operator!=( const Flexible8ByteString_c& acrc_right ) const
    { return (mu_data != acrc_right); }

  /** compare two IsoName_c values with operator<
      NOTE: The NAMEs are being compared based on the PRIORITY
            and not on the numeric value.
      NOTE: A NAME has a lower priority if it has a higher numeric value!
      NOTE: If either NAME is unspecified, false will be returned!
  */
  inline bool operator<( const IsoName_c& right ) const
    { return( (m_specified == true) && (right.m_specified == true) )
        ? (mu_data > right.mu_data)
        : false; }

  /** convert function */
  IsoAgLib::iIsoName_c& toIisoName_c();

  /** convert function */
  const IsoAgLib::iIsoName_c& toConstIisoName_c() const;

private:
  /** ISO 8-uint8_t NAME field */
  Flexible8ByteString_c mu_data;

  bool m_specified;
};

inline
bool
IsoName_c::selfConf() const
{
  return ( mu_data[7] >> 7) != 0;
}


inline
uint8_t
IsoName_c::indGroup() const
{
  return ((mu_data[7] >> 4) & 0x7) ;
}


inline
uint8_t
IsoName_c::devClassInst() const
{
  return (mu_data[7] & 0xF);
}


inline
uint8_t
IsoName_c::devClass() const
{
  return uint8_t(mu_data[6] >> 1);
}


inline
uint8_t
IsoName_c::func() const
{
  return mu_data[5];
}


inline
uint8_t
IsoName_c::funcInst() const
{
  return uint8_t(mu_data[4] >> 3);
}


inline
uint8_t
IsoName_c::ecuInst() const
{
  return (mu_data[4] & 0x7);
}


inline
uint16_t
IsoName_c::manufCode() const
{
  return uint16_t((mu_data[3] << 3) | (mu_data[2] >> 5));
}


inline
uint32_t
IsoName_c::serNo() const
{
  return ((static_cast<uint32_t>(mu_data[2] & 0x1F) << 16) | (static_cast<uint32_t>(mu_data[1]) << 8) | mu_data[0]);
}


inline
void
IsoName_c::inputString (const uint8_t* apb_src)
{
  isoaglib_header_assert (NULL != apb_src);
  mu_data.setDataFromString( apb_src );
}


inline
void
IsoName_c::inputUnion (const Flexible8ByteString_c* apu_src)
{
  isoaglib_header_assert (NULL != apu_src);
  mu_data = *apu_src;
}


inline
void
IsoName_c::setSelfConf (bool ab_selfConf)
{
  mu_data.setUint8Data( 7, uint8_t((mu_data[7] & 0x7F) | (ab_selfConf << 7)) );
}


inline
void
IsoName_c::setIndGroup (uint8_t aui8_indGroup)
{
  mu_data.setUint8Data( 7, uint8_t((mu_data[7] & 0x8F) | ((aui8_indGroup & 0x7) << 4)) );
}


inline
void
IsoName_c::setDevClassInst (uint8_t aui8_devClassInst)
{
  mu_data.setUint8Data( 7, uint8_t((mu_data[7] & 0xF0) | (aui8_devClassInst)) );
}


inline
void
IsoName_c::setDevClass (uint8_t aui8_devClass)
{
  mu_data.setUint8Data( 6, uint8_t((0 /* reserved bit set to zero!*/) | (aui8_devClass << 1)) );
/* old version, which would be right if the reserved bit would have been set somewhere else.
  pb_data[6] = ((pb_data[6] & 0x1) | (aui8_devClass << 1));
*/
}


inline
void
IsoName_c::setFunc (uint8_t ab_func)
{
  mu_data.setUint8Data( 5, ab_func );
}


inline
void
IsoName_c::setFuncInst (uint8_t ab_funcInst)
{
  mu_data.setUint8Data( 4, uint8_t((mu_data[4] & 0x7) | (ab_funcInst << 3)) );
}


inline
void
IsoName_c::setEcuInst (uint8_t ab_ecuInst)
{
  mu_data.setUint8Data( 4, uint8_t((mu_data[4] & 0xF8) | (ab_ecuInst & 0x7)) );
}


inline
void
IsoName_c::setManufCode (uint16_t aui16_manufCode)
{
  mu_data.setUint8Data( 3, uint8_t(aui16_manufCode >> 3) );
  mu_data.setUint8Data( 2, uint8_t((mu_data[2] & 0x1F) | ((aui16_manufCode & 0x7) << 5)) );
}


inline
void
IsoName_c::setSerNo (uint32_t aui32_serNo)
{
  mu_data.setUint16Data( 0, uint16_t(aui32_serNo & 0xFFFFU) );
  mu_data.setUint8Data( 2, uint8_t( (mu_data[2] & 0xE0) | ((aui32_serNo >> 16) & 0x1F) ) );
}


inline
IsoName_c::IsoName_c (const uint8_t* apb_src)
  : mu_data (apb_src)
  , m_specified( true )
{
}


inline
IsoName_c::IsoName_c()
  : mu_data()
  , m_specified( false )
{
}


inline
IsoName_c::IsoName_c (const Flexible8ByteString_c* apu_src)
  : mu_data (*apu_src)
  , m_specified( true )
{
}


inline
IsoName_c::IsoName_c (bool ab_selfConf, uint8_t aui8_indGroup, uint8_t aui8_devClass, uint8_t aui8_devClassInst,
                      uint8_t ab_func, uint16_t aui16_manufCode, uint32_t aui32_serNo, uint8_t ab_funcInst, uint8_t ab_ecuInst)
  : mu_data()
  , m_specified( true )
{
  set (ab_selfConf, aui8_indGroup, aui8_devClass, aui8_devClassInst,
       ab_func, aui16_manufCode, aui32_serNo, ab_funcInst, ab_ecuInst);
}


inline
IsoName_c::IsoName_c (const IsoName_c& src)
  : mu_data( src.mu_data )
  , m_specified( src.m_specified )
{ // simply copy data string / specified
}


inline
IsoName_c::~IsoName_c()
{
}


}
#endif
