/*
  eepromio_c.h: header for EEPROM_IO_c object for communication with
    EEPROM

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef EEPROM_IO_H
#define EEPROM_IO_H

#include <IsoAgLib/util/iliberr_c.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <supplementary_driver/hal/hal_eeprom.h>


namespace __IsoAgLib 
{
class IsoName_c;

/**
  object for communication with the EEPROM,
  stream read/write operators for all basic types;
  avoid rewriting same values to EEPROM;
  manages operations cross segment boundaries
  @short Simple data communication with EEPROM.
  @author Dipl.-Inform. Achim Spangler
*/
class EepromIo_c 
{
public:
  ~EepromIo_c() {}

  void init();
  void close() {}

  // ++++++++++++++++++++++++++++++++++++
  // ++++ EEPROM managing operations ++++
  // ++++++++++++++++++++++++++++++++++++

  /**
    deliver the size of the whole EEPROM memory in bytes;
    uses BIOS function
    @return size of EEPROM in BYTE
  */
  static uint32_t eepromSize(){return (HAL::getEepromSize());};

  /**
    set the write position in EEPROM (in Byte); answer if possible
    if aui16_adress exceeds EEPROM memory ignore setting and set range

    possible errors:
        * range aui16_adress outer the limits of EEPROM memory

    @param aui16_adress position of write mark [uint8_t]
    @return true -> setting of write mark without errors
  */
  bool setp(uint16_t aui16_adress);
  /**
    set the read position in EEPROM (in Byte); answer if possible
    if aui16_adress exceeds EEPROM memory ignore setting and set range

    possible errors:
        * range aui16_adress outer the limits of EEPROM memory

    @param aui16_adress position of read mark [uint8_t]
    @return true -> setting of read mark without errors
  */
  bool setg(uint16_t aui16_adress);

  /**
    get the write position in EEPROM (in Byte)
    @return position of write mark [uint8_t]
  */
  inline uint16_t tellp(){return mui16_wPosition;};

  /**
    get the read position in EEPROM (in Byte)
    @return position of read mark [uint8_t]
  */
  inline uint16_t tellg(){return mui16_rPosition;};

  /**
    check if write position is at end of EEPROM
    (parameter specifies lookahead (normally length of operation - 1 is passed to check if a block of data fits into)

    possible errors:
        * range if current read position + lookahead is out of EEPROM range

    @param aui16_lookahead optional uint8_t lookahead offset (default 0 -> only current write mark position tested)
    @return false -> current position + lookahead is a valid EEPROM address. (true -> out of EEPROM range)
  */
  bool eofp( uint16_t aui16_lookahead = 0 );

  /**
    check if read position is at end of EEPROM
    (parameter specifies lookahead (normally length of operation - 1 is passed to check if a block of data fits into)

    possible errors:
        * range if (ab_setState == true) and (current read position + lookahead) is out of EEPROM range

    @param aui16_lookahead optional uint8_t lookahead offset (default 0 -> only current read mark position tested)
    @return false -> (current position + lookahead) is a valid EEPROM address. (true -> out of EEPROM range)
  */
  bool eofg( uint16_t aui16_lookahead = 0 );

  /* *************************************** */
  /* *** EEPROM data operation functions *** */
  /* ************** writing **************** */
  /* *************************************** */

  /**
    write a value to EEPROM from actual write position on (tellp() )
    by use of template mechanism the correct write implementation is generted
    for the given parameter type

    possible errors:
        * range writing position exceeds end of EEPROM
        * busy the EEPROM was busy with another action
        * eepromSegment low level writing caused segment error

    @see EepromIo_c::tellp
    @see EepromIo_c::setp
    @param rTemplateVal value of any type to write into EEPROM from actual write position on
    @return reference to this EepromIo_c instance (for chains like "eeprom << val1 << val2 << ... << val_n;")
  */
  template<class T>
    EepromIo_c& operator<<(const T& rTemplateVal){return writeIntern(((const uint8_t*)(&rTemplateVal)), sizeof(T));}

  /**
    write a uint8_t string value to EEPROM from actual write position on (tellp() )

    possible errors:
        * range writing position exceeds end of EEPROM
        * busy the EEPROM was busy with another action
        * eepromSegment low level writing caused segment error

    @see EepromIo_c::tellp
    @see EepromIo_c::setp
    @param apb_string string to write into EEPROM
    @param aui16_number length of the string to write
    @return reference to this EepromIo_c instance (for chains like "eeprom << val1 << val2 << ... << val_n;")
  */
  EepromIo_c& writeString(const uint8_t *const apb_string, uint16_t aui16_number);

  /* *************************************** */
  /* *** EEPROM data operation functions *** */
  /* ************** reading **************** */
  /* *************************************** */
  /**
    read a value to EEPROM from actual write position on (tellg() )
    by use of template mechanism the correct read implementation is generted
    for the given parameter type

    possible errors:
        * range reading position exceeds end of EEPROM
        * busy the EEPROM was busy with another action
        * eepromSegment low level reading caused segment error

    @see EepromIo_c::tellg
    @see EepromIo_c::setg
    @param rTemplateVal EERPOM data is read into rTemplateVal from actual read position on
    @return reference to this EepromIo_c instance (for chains like "eeprom >> val1 >> val2 >> ... >> val_n;")
  */
  template<class T>
  EepromIo_c& operator>>(T& rTemplateVal){return readIntern(((uint8_t*)(&rTemplateVal)), sizeof(T));}

  /**
    read operator for strings with given length; uses BIOS function

    possible errors:
        * range reading position exceeds end of EEPROM
        * busy the EEPROM was busy with another reading action
        * eepromSegment low level reading caused segment error

    @see EepromIo_c::tellg
    @see EepromIo_c::setg
    @param apb_string pointer to uint8_t string, which should be read from actual EEPROM read position on
    @param aui16_number number of uint8_t to read into string
    @return true -> read with success
  */
  bool readString(uint8_t *const apb_string, uint16_t aui16_number);

private:
// Private methods
  friend EepromIo_c& operator<<(EepromIo_c& rc_stream, const IsoName_c& rc_data );
  friend EepromIo_c& operator>>(EepromIo_c& rc_stream, IsoName_c& rc_data );

  /** private constructor which prevents direct instantiation in user application
    * NEVER define instance of EepromIo_c within application
    */
  EepromIo_c()
    : mui16_segmentSize( 0 )
    , mui16_rPosition( 0 )
    , mui16_wPosition( 0 )
  {}

  /**
    set error flags dependent on BIOS return value
    @param ai16_biosReturn BIOS return value which should be translated in error state of EEPROM_IO
  */
  static void setState4BiosReturn(int16_t ai16_biosReturn);

  /**
    init for single write operation, check if write is possible; uses BIOS function

    possible errors:
        * busy other task communicates with EEPROM
        * unspecified low level SPI EEPROM communication doesn't work
        * eepromSegment low level reading caused segment error

    @return true -> EEPROM is ready for write operations
  */
  static bool writeInit();
  /**
    wait with triggering the watchdog, till the EEPROM is ready

    @return HAL_NO_ERR -> EEPROM is ready for next operation
  */
  static int16_t wait_eepromReady( void );
  /**
    deliver the max number of bytes, which can be written in EEPROM from given start position on
    @param aui16_adress wanted write position
    @return amount of bytes between write position and end of EEPROM
  */
  uint16_t maxSize(uint16_t aui16_adress);

  /**
    write the template parameter value at given position to EEPROM,
    return true if successfull

    possible errors:
        * range writing exceeds EEPROM memory
        * busy other task communicates with EEPROM
        * unspecified low level SPI EEPROM communication doesn't work
        * eepromSegment low level reading caused segment error

    @param aui16_adress adress to start writing
    @param rTemplateVal value of any type to write to EEPROM
    @return true -> write successful
  */
  template<class T>
  bool write(uint16_t aui16_adress, T rTemplateVal);

  /**
    write the pointed data string to eeprom; uses BIOS function

    possible errors:
        * range writing exceeds EEPROM memory
        * busy other task communicates with EEPROM
        * unspecified low level SPI EEPROM communication doesn't work
        * eepromSegment low level reading caused segment error

    @param aui16_adress adress to start writing
    @param aui16_number amount of bytes to write to EEPROM
    @param apb_data pointer to string of uint8_t to write to EEPROM
    @return true -> write successful
  */
  bool write(uint16_t aui16_adress, uint16_t aui16_number, const uint8_t* apb_data);
  /**
    write the given data string to eeprom from actual write position
    possible errors:
        * range writing exceeds EEPROM memory
        * busy other task communicates with EEPROM
        * unspecified low level SPI EEPROM communication doesn't work
        * eepromSegment low level reading caused segment error

    @param apb_data pointer to data
    @param aui8_len lenght of data
    @return reference to this object for chains
  */
  EepromIo_c& writeIntern(const uint8_t* apb_data, uint8_t aui8_len);
  /**
    read from actual read position wanted amount of data in to pointer

    possible errors:
        * range writing exceeds EEPROM memory
        * busy other task communicates with EEPROM
        * unspecified low level SPI EEPROM communication doesn't work
        * eepromSegment low level reading caused segment error

    @param apb_data pointer to data
    @param aui8_len lenght of data
    @return reference to this object for chains
  */
  EepromIo_c& readIntern(uint8_t* apb_data, uint8_t aui8_len);

private:
  /** segment size of EEPROM which can't be exceedecd by write actions */
  uint16_t mui16_segmentSize;

  /** actual read position in EEPROM */
  uint16_t mui16_rPosition;

  /** actual write position in EEPROM */
  uint16_t mui16_wPosition;

  friend EepromIo_c &getEepromInstance();
};


/* *********************************************************************** */
/* ****inline implementation of stream input/output template functions**** */
/* *********************************************************************** */

/**
  write the template parameter value at given position to EEPROM,
  return true if successfull

  possible errors:
      * range writing exceeds EEPROM memory
      * busy other task communicates with EEPROM
      * unspecified low level SPI EEPROM communication doesn't work
      * eepromSegment low level reading caused segment error

  @param aui16_adress adress to start writing
  @param rTemplateVal value of any type to write to EEPROM
  @return true -> write successful
*/
template<class T>
bool EepromIo_c::write(uint16_t aui16_adress, T rTemplateVal)
{
  bool b_result = false;
  if ((aui16_adress + sizeof(T)) < eepromSize())
  { // write doesn't exceed EEPROM limits
    // copy to local var -> then call string write
    T tempVal = rTemplateVal;
    b_result =  write(aui16_adress, sizeof(T), static_cast<uint8_t*>(static_cast<void*>(&tempVal)));
  } else {
    isoaglib_header_assert( !"adress + sizeof value exceed eeprom size" );
  }
  return b_result;
};

/** C-style function, to get access to the unique EepromIo_c singleton instance */
EepromIo_c& getEepromInstance();

EepromIo_c& operator<<(EepromIo_c& rc_stream, const IsoName_c& rc_data );
EepromIo_c& operator>>(EepromIo_c& rc_stream, IsoName_c& rc_data );

} // __IsoAgLib

#endif
