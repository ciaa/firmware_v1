/*
  eeprom.h: definition of HAL namesapce layer for P1MC

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/** \file IsoAgLib/hal/p1mc/eeprom/eeprom.h
 * The header <i>\<target\>/\<device\>/\<device\>.h</i> performs a name
   mapping between platform specific BIOS / OS function names
   and the function names, the IsoAgLib uses for hardware access.
   In this header only the groups for error codes, global system
   functions and CAN must be adapted to a new platform.
   If a simple name mapping is not possible for a specific
   function, the module targetExtensions can be used to implement
   the needed activity. The implementation of the PC version
   can be used to derive the meaning of the specific function.
*/
/* ************************************************************ */

#ifndef _HAL_P1MC_EEPROM_H_
#define _HAL_P1MC_EEPROM_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/p1mc/typedef.h>
#include <IsoAgLib/hal/p1mc/errcodes.h>

namespace __HAL {
  extern "C" {
  }
}
/**
   namespace with layer of inline (cost NO overhead -> compiler replaces
   inline function with call to orig BIOS function)
   functions between all IsoAgLib calls for BIOS and the corresponding BIOS functions
   --> simply replace the call to the corresponding BIOS function in this header
       for adaptation to new platform
 */
namespace HAL {
  /* *************************** */
  /** \name EEPROM BIOS functions */
  /*@{*/

  /**
   deliver the EEPROM size in uint8_t -> mult P1MC BIOS value with 1024
   @return EEPROM size in byte
  */
  inline uint32_t getEepromSize( void ) {
    /* DWS */ // return ( 1024 * __HAL::getEepromSize() );
    return 0;
  };

  /**
   deliver the EEPROM segment size in kbyte
   @return EEPROM segment size in kbyte
  */
  inline uint16_t getEepromSegmentSize( void )
  /* DWS */ { //    {return __HAL::getEepromSegmentSize();};
    return 0;
  }

  /**
    read amount of uint8_t in uint8_t string variable
    @param wAdress start adress to read
    @param wNumber amount of bytes to read
    @param pbByte pointer to uint8_t string, where data is inserted
    @return error state (C_NO_ERR == o.k.)
  */
  inline int16_t eepromRead( uint16_t wAddress,uint16_t wNumber,uint8_t *pbByte )
  /* DWS */ { //    {return __HAL::eepromRead(wAddress, wNumber, pbByte);};
    return 0;
  }

  /**
    write amount of uint8_t from string into EEPROM from given start adress
    @param wAdress adress of first written uint8_t in EEPROM
    @param wNumber amount of written data
    @param pbData pointer to uint8_t string which should be written into EEPROM
    @return error state (C_NO_ERR == o.k.)
  */
  inline int16_t eepromWrite( uint16_t wAddress,uint16_t wNumber,const uint8_t *pbData )
  /* DWS */ { //    {return __HAL::eepromWrite(wAddress, wNumber, pbData);};
    return 0;
  }

  /**
    set or unset set write protection of EEPROM, should be called before
    every write access
    @param bitMode sets write protection OFF or ON
    @return error state (C_NO_ERR == o.k.)
  */
  inline int16_t eepromWp( bool bitMode )
  /* DWS */ { //    {return __HAL::eepromWp(bitMode);};
    return 0;
  }

  /**
    check if EEPROM is ready for actions
    @return HAL_NO_ERR -> ready
  */
  inline int16_t eepromReady( void )
    return HAL_NO_ERR;
  }
}
#endif
