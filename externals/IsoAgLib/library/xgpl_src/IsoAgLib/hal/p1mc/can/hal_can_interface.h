/*
  hal_can_interface.h: CAN interface between BIOS/OS and IsoAgLib to
    concentrate CAN handling abstraction within one module

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/** \file hal_can_interface.h
 * The module halCanInterface implements a standard
 * interface for the CAN interactions of the IsoAgLib
 * This way the the adaptation of the IsoAgLib can be
 * better restricted to the various BIOS specific
 * directories xgpl_src/IsoAgLib/hal/xx/can/ .
 */
/* ********************************************************** */

#ifndef _HAL_P1MC_CAN_INTERFACE_H_
#define _HAL_P1MC_CAN_INTERFACE_H_

#include <IsoAgLib/isoaglib_config.h>
#include "../errcodes.h"
#include "../typedef.h"

#define NUMBER_CANMESSAGES 24

// application interface include
#include "datatypes.h"
#include "idapp.h"
#include "datacontainer.h"
#include "_caninterface_public.h"
#include "_CanDefs_public.h"


namespace __IsoAgLib { class Ident_c; class CanPkg_c;}

namespace __HAL {

/* ************************************************** */
/** \name Global Status Per BUS
 *  Functions for status check of global CAN BUS      */
/* ************************************************** */
/*@{*/

/**
  test if the CAN BUS is in WARN state
  @param rui8_busNr number of the BUS to check (default 0)
  @return true == CAN BUS is in WARN state, else normal operation
*/
bool can_stateGlobalWarn(uint8_t rui8_busNr = 0);

/**
  test if the CAN BUS is in OFF state
  @param rui8_busNr number of the BUS to check (default 0)
  @return true == CAN BUS is in OFF state, else normal operation
*/
bool can_stateGlobalOff(uint8_t rui8_busNr = 0);

/**
  test if the CAN BUS is in Blocked state, which can be a sign
  for CAN controllers which are configured with different baudrates;
  this is the case if neither succesfull sent nor received msg
  is detcted AND CAN controller is in WARN or OFF state
  (the time since last succ. send/rec and the time of WARN/OFF
   can be defined with CONFIG_CAN_MAX_CAN_ERR_TIME_BEFORE_SLOWERING
   in the application specific config file isoaglib_config
   -> should not be to short to avoid false alarm)
  @param rui8_busNr number of the BUS to check (default 0)
  @return true == CAN BUS is in blocked state, else normal operation
*/
bool can_stateGlobalBlocked(uint8_t rui8_busNr = 0);

/**
  deliver the baudrate of the CAN BUS in [kbaud]
  @param rui8_busNr number of the BUS to check (default 0)
  @return BUS load of the last second [kbaud]
*/
int32_t can_stateGlobalBusload(uint8_t rui8_busNr = 0);

/**
  check if a send try on this BUS caused an Bit1Error
  (occurs if >1 controller try to send msg with same ident)
  ==> ISO 11783 forces stop of retries in this case for
      adress claim
      @param rui8_busNr number of the BUS to check
      @return true -> Bit1Err occured
*/
bool can_stateGlobalBit1err(uint8_t rui8_busNr = 0);
/*@}*/

/* ************************************************** */
/** \name Specific for one MsgObj
 *  Functions for status check of a single MsgObj     */
/* ************************************************** */
/*@{*/

/**
  deliver the timestamp of last successfull CAN send action
  @param rui8_busNr number of the BUS to check
  @param rui8_msgobjNr number of the MsgObj to check
  @return timestamp of last successful send
          OR -1 if rui8_msgObjNr corresponds to no valid send obj
*/
int32_t can_stateMsgobjTxok(uint8_t rui8_busNr, uint8_t rui8_msgobjNr);

/**
  test if buffer of a MsgObj is full (e.g. no more
  msg can be put into buffer (important for TX objects))
  @param rui8_busNr number of the BUS to check
  @param rui8_msgobjNr number of the MsgObj to check
  @return true -> buffer is full -> no further can_send allowed
*/
bool can_stateMsgobjOverflow(uint8_t rui8_busNr, uint8_t rui8_msgobjNr);

/**
  deliver amount of messages in buffer
  (interesting for RX objects)
  @param rui8_busNr number of the BUS to check
  @param rui8_msgobjNr number of the MsgObj to check
  @return number of messages in buffer or negative error code:
    HAL_CONFIG_ERR == BUS not initialised
    HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_stateMsgobjBuffercnt(uint8_t rui8_busNr, uint8_t rui8_msgobjNr);

/**
  deliver amount of messages which can be placed in buffer
  (interesting for TX objects)
  @param rui8_busNr number of the BUS to check
  @param rui8_msgobjNr number of the MsgObj to check
  @return number of messages which can be placed in buffer or negative error code:
    HAL_CONFIG_ERR == BUS not initialised
    HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_stateMsgobjFreecnt(uint8_t rui8_busNr, uint8_t rui8_msgobjNr);

/**
  check if MsgObj is currently locked
  @param rui8_busNr number of the BUS to check
  @param rui8_msgobjNr number of the MsgObj to check
  @return true -> MsgObj is currently locked
*/
bool can_stateMsgobjLocked( uint8_t rui8_busNr, uint8_t rui8_msgobjNr );
/*@}*/

/* ************************************************** */
/** \name Global Configuration Per BUS
 *  Functions to configure the global CAN BUS         */
/* ************************************************** */
/*@{*/

/**
  init CAN BUS with given baudrate and masks
  (11bit, 29bit and last msg mask)
  @param rui8_busNr number of the BUS to init (default 0)
  @param rb_baudrate baudrate in [kbaud]
  @param rui16_maskStd 11bit global mask
  @param rui32_maskExt 29bit global mask
  @param rui32_maskLastmsg mask of last CAN msg
  @return HAL_NO_ERR == no error;
          HAL_RANGE_ERR == wrong BUS nr or wrong baudrate;
*/
int16_t can_configGlobalInit(uint8_t rui8_busNr, uint16_t rb_baudrate, uint16_t rb_maskStd, uint32_t rui32_maskExt, uint32_t rui32_maskLastmsg);

/**
  set the global masks
  (11bit, 29bit and last msg mask)
  @param rui8_busNr number of the BUS to config (default 0)
  @param rui16_maskStd 11bit global mask
  @param rui32_maskExt 29bit global mask
  @param rui32_maskLastmsg mask of last CAN msg
  @return HAL_NO_ERR == no error;
          HAL_RANGE_ERR == wrong BUS nr
*/
int16_t can_configGlobalMask(uint8_t rui8_busNr, uint16_t rb_maskStd, uint32_t rui32_maskExt, uint32_t rui32_maskLastmsg);

int16_t can_configMsgobjSendpause(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, uint16_t rui16_minSend);

/**
  close CAN BUS
  (important to close BUS before re-init with other baudrate)
  @param rui8_busNr number of the BUS to close (default 0)
  @return HAL_NO_ERR == no error;
          HAL_RANGE_ERR == wrong BUS nr;
          HAL_CONFIG_ERR == BUS previously not initialised
*/
int16_t can_configGlobalClose(uint8_t rui8_busNr);

/** wait until specified timeout or until next CAN message receive
 *  @return true -> there are CAN messages waiting for process. else: return due to timeout
 */
bool can_waitUntilCanReceiveOrTimeout( int32_t timeoutInterval );
/*@}*/


/* ************************************************** */
/** \name Configuration specific for one MsgObj
 *  Functions to configure a specific MsgObj          */
/* ************************************************** */
/*@{*/

/**
  config a MsgObj
  (buffer size is defined locally in implementation of this interface;
  all received msgs should get a time stamp;
  class Ident_c has ident and type 11/29bit)
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @param rrefc_ident filter ident of this MsgObj
  @param rb_rxtx 0==RX receive; 1==TX transmit
  @return HAL_NO_ERR == no error;
          C_BUSY == this MsgObj is already used
          HAL_CONFIG_ERR == BUS not initialised or error during buffer allocation
          HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_configMsgobjInit(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::Ident_c& rrefc_ident, uint8_t rb_rxtx);

/**
  change the Ident_c of an already initialised MsgObj
  (class __IsoAgLib::Ident_c has ident and type 11/29bit)
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @param rrefc_ident filter ident of this MsgObj
  @return HAL_NO_ERR == no error;
          HAL_CONFIG_ERR == BUS not initialised or ident can't be changed
          HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_configMsgobjChgid(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::Ident_c& rrefc_ident);

/**
  lock a MsgObj to avoid further placement of messages into buffer.
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @param rb_doLock true==lock(default); false==unlock
  @return HAL_NO_ERR == no error;
          HAL_CONFIG_ERR == BUS not initialised or ident can't be changed
          HAL_RANGE_ERR == wrong BUS or MsgObj number
  */
int16_t can_configMsgobjLock( uint8_t rui8_busNr, uint8_t rui8_msgobjNr, bool rb_doLock = true );

/**
  close a MsgObj
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @return HAL_NO_ERR == no error;
          HAL_CONFIG_ERR == BUS not initialised, MsgObj previously not used or buffer memory not freed
          HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_configMsgobjClose(uint8_t rui8_busNr, uint8_t rui8_msgobjNr);
/*@}*/

/* ******************************************** */
/** \name Use of MsgObj
 *  Functions to use a specific MsgObj          */
/* ******************************************** */
/*@{*/

/**
  send a message via a MsgObj;
  CanPkg_c (or derived object) must provide (virtual)
  functions:
  * Ident_c& getIdent() -> deliver ident of msg to send
  * void getData(MASK_TYPE& reft_ident, uint8_t& refui8_identType,
                 uint8_t& refb_dlcTarget, uint8_t* pb_dataTarget)
    -> put DLC in referenced ref_dlc and insert data in uint8_t string pb_data
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @param rpc_data pointer to CanPkg_c instance with data to send
  @return HAL_NO_ERR == no error;
          HAL_CONFIG_ERR == BUS not initialised, MsgObj is no send object
          HAL_NOACT_ERR == BUS OFF
          HAL_OVERFLOW_ERR == send buffer overflowed
          HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_useMsgobjSend(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::CanPkg_c* rpc_data);

/**
  get the ident of a received message to decide about the further
  processing before the whole data string is retreived
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @param reflIdent reference to the var, where the ident should be inserted
  @return error code
  HAL_NO_ERR == No problem
  HAL_CONFIG_ERR == BUS not initialised, MsgObj is no RX object
  HAL_NOACT_ERR == BUS OFF
  HAL_OVERFLOW_ERR == send buffer overflowed
  HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int32_t can_useMsgobjReceivedIdent(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, int32_t &reflIdent);

/**
  transfer front element in buffer into the pointed CanPkg_c;
  DON'T clear this item from buffer.
  @see can_useMsgobjPopFront for explicit clear of this front item
  functions:
  * setIdent(Ident_c& rrefc_ident)
    -> set ident rrefc_ident of received msg in CANPkg
  * uint8_t setDataFromString(uint8_t* rpb_data, uint8_t rb_dlc)
    -> set DLC in CanPkg_c from rb_dlc and insert data from uint8_t string rpb_data
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @param rpc_data pointer to CanPkg_c instance with data to send
  @return HAL_NO_ERR == no error;
          HAL_CONFIG_ERR == BUS not initialised, MsgObj is no RX object
          HAL_NOACT_ERR == BUS OFF
          HAL_OVERFLOW_ERR == send buffer overflowed
          HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_useMsgobjGet(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::CanPkg_c* rpc_data);
/**
  Either register the currenct front item of buffer as not relevant,
  or just pop the front item, as it was processed.
  This explicit pop is needed, as one CAN message shall be served to
  several CANCustomer_c instances, as long as one of them indicates a
  succesfull process of the received message.
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
*/
void can_useMsgobjPopFront(uint8_t rui8_busNr, uint8_t rui8_msgobjNr);

/**
  clear th buffer of a MsgObj (e.g. to stop sending retries)
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @return HAL_NO_ERR == no error;
          HAL_CONFIG_ERR == BUS not initialised
          HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_useMsgobjClear(uint8_t rui8_busNr, uint8_t rui8_msgobjNr);

int iWriteCentralCanFifo(const char bBus, const CanMessage_T* ap_msg);
}

#endif
