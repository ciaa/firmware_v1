/*
  can.h: definition of Hardware Abstraction Layer for CAN functions
    for P1MC

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/** \file IsoAgLib/hal/p1mc/can/can.h
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

#ifndef _HAL_P1MC_CAN_H_
#define _HAL_P1MC_CAN_H_

#include "hal_can_interface.h"

namespace HAL
{
  inline bool can_stateGlobalWarn(uint8_t rui8_busNr)
    {return __HAL::can_stateGlobalWarn(rui8_busNr);}

  inline bool can_stateGlobalOff(uint8_t rui8_busNr)
    {return __HAL::can_stateGlobalOff(rui8_busNr);}

  inline bool can_stateGlobalBlocked(uint8_t rui8_busNr)
    {return __HAL::can_stateGlobalBlocked(rui8_busNr);}

  inline int32_t can_stateGlobalBusload(uint8_t rui8_busNr)
    {return __HAL::can_stateGlobalBusload(rui8_busNr);}

  inline bool can_stateGlobalBit1err(uint8_t rui8_busNr)
    {return __HAL::can_stateGlobalBit1err(rui8_busNr);}

  inline int32_t can_stateMsgobjTxok(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
    {return __HAL::can_stateMsgobjTxok(rui8_busNr, rui8_msgobjNr);}

  inline bool can_stateMsgobjOverflow(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
    {return __HAL::can_stateMsgobjOverflow(rui8_busNr, rui8_msgobjNr);}

  inline uint16_t can_stateMsgobjBuffercnt(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
    {return __HAL::can_stateMsgobjBuffercnt(rui8_busNr, rui8_msgobjNr);}

  inline int16_t can_stateMsgobjFreecnt(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
    {return __HAL::can_stateMsgobjFreecnt(rui8_busNr, rui8_msgobjNr);}

  inline bool can_stateMsgobjLocked( uint8_t rui8_busNr, uint8_t rui8_msgobjNr )
    {return __HAL::can_stateMsgobjLocked( rui8_busNr, rui8_msgobjNr );}

  inline int16_t can_configGlobalInit(uint8_t rui8_busNr, uint16_t rb_baudrate, uint16_t rb_maskStd,
        uint32_t rui32_maskExt, uint32_t rui32_maskLastmsg)
    {return __HAL::can_configGlobalInit(rui8_busNr, rb_baudrate, rb_maskStd,
        rui32_maskExt, rui32_maskLastmsg);}

  inline int16_t can_configGlobalMask(uint8_t rui8_busNr, uint16_t rb_maskStd, uint32_t rui32_maskExt, uint32_t rui32_maskLastmsg)
    {return __HAL::can_configGlobalMask(rui8_busNr, rb_maskStd, rui32_maskExt, rui32_maskLastmsg);}

  inline int16_t can_configGlobalClose(uint8_t rui8_busNr)
    {return __HAL::can_configGlobalClose(rui8_busNr);}

  inline bool can_waitUntilCanReceiveOrTimeout( int32_t timeoutInterval )
    { return __HAL::can_waitUntilCanReceiveOrTimeout( timeoutInterval );}

  inline int16_t can_configMsgobjInit(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::Ident_c& rrefc_ident, uint8_t rb_rxtx)
    {return __HAL::can_configMsgobjInit(rui8_busNr, rui8_msgobjNr, rrefc_ident, rb_rxtx);}

  inline int16_t can_configMsgobjChgid(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::Ident_c& rrefc_ident)
    {return __HAL::can_configMsgobjChgid(rui8_busNr, rui8_msgobjNr, rrefc_ident);}

  inline int16_t can_configMsgobjSendpause(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, uint16_t rui16_minSend)
    {return __HAL::can_configMsgobjSendpause(rui8_busNr, rui8_msgobjNr, rui16_minSend);}

  inline int16_t can_configMsgobjClose(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
    {return __HAL::can_configMsgobjClose(rui8_busNr, rui8_msgobjNr);}

  inline int16_t can_useMsgobjSend(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::CanPkg_c* rpc_data)
    {return __HAL::can_useMsgobjSend(rui8_busNr, rui8_msgobjNr, rpc_data);}

  inline int32_t can_useMsgobjReceivedIdent(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, int32_t &reflIdent)
    {return __HAL::can_useMsgobjReceivedIdent(rui8_busNr, rui8_msgobjNr, reflIdent);}

  inline int16_t can_useMsgobjGet(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::CanPkg_c* rpc_data)
    {return __HAL::can_useMsgobjGet(rui8_busNr, rui8_msgobjNr, rpc_data);}

  inline void can_useMsgobjPopFront(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
    {__HAL::can_useMsgobjPopFront(rui8_busNr, rui8_msgobjNr);}

  inline int16_t can_useMsgobjClear(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
    {return __HAL::can_useMsgobjClear(rui8_busNr, rui8_msgobjNr);}
}
#endif
