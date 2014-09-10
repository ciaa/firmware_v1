/*
  hal_can_interface.cpp: CAN interface between BIOS/OS and IsoAgLib to
    concentrate CAN handling abstraction within one module

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#define NUMBER_CANMESSAGES 24

#include "hal_can_interface.h"
#include "../typedef.h"
#include "../config.h"
#include "../errcodes.h"
#include "../system/system_target_extensions.h"

#include <IsoAgLib/driver/can/impl/ident_c.h>
#include <IsoAgLib/hal/generic_utils/can/canfifo_c.h>
#include <IsoAgLib/hal/generic_utils/can/canutils.h>

extern "C" {
#include "datacontainer.h"
}
#include "_caninterface_public.h"
#include "_CanDefs_public.h"

// project include
#include "userdata.h"

extern "C" {
#include "_canfifo_public.h"
#include "canfifo.h"
}

// The FIFO Handler is declared in the kernel
extern LinkedFIFOHandler_T _FIFOHandler[NUMBER_CANCONTROLLERS];

//  You can only have one FIFO per CAN port, but you can have
//  multiple PostPools per FIFO.  We don't have access to the ones
//  used in the kernel, so declare one (per CAN port) for the
//  application.  Note that each PostPool has an internal pool
//  of 50 message buffers.  This size is fixed.
LinkedTxFIFOMessagePostPool_T arr_PostPool_CAN[NUMBER_CANCONTROLLERS];

namespace __HAL {

/** Message Objekt Datenstruktur fuer CAN Botschaft *
 * Struktur Sende/Empfang Daten allgemein fuer Anwender (14 Byte) */
typedef struct
{
  uint32_t dwId;                          /** Identifier */
  uint8_t bXtd;                           /** Laenge Bit Identifier */
  uint8_t bDlc;                           /** Anzahl der empfangenen Daten */
  uint8_t abData[8];                      /** Datenpuffer */
} tSend;

static tSend t_cinterfMsgobjSend;

/* ******************************************************* */
/* ***************** Status Checking ********************* */
/* ******************************************************* */

/* ******************** */
/* ***Global Per BUS*** */
/* ******************** */

/**
  update the CAN BUS state parameters for
  WARN, OFF and Bit1Err
  @param rui8_busNr number of bus to check
*/
void updateCanStateTimestamps(uint8_t rui8_busNr)
{
}

/**
  test if the CAN BUS is in WARN state
  @param rui8_busNr number of the BUS to check (default 0)
  @return true == CAN BUS is in WARN state, else normal operation
*/
bool can_stateGlobalWarn(uint8_t rui8_busNr)
{
  return false;
}

/**
  test if the CAN BUS is in OFF state
  @param rui8_busNr number of the BUS to check (default 0)
  @return true == CAN BUS is in OFF state, else normal operation
*/
bool can_stateGlobalOff(uint8_t rui8_busNr)
{
  return false;
}
/**
  update the timestamp of last succ receive by
  comparing the cnt of msgs in send buffers
  @param rui8_busNr number of bus to check
*/
void updateSuccSendTimestamp(uint8_t rui8_busNr)
{
}

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
bool can_stateGlobalBlocked(uint8_t rui8_busNr)
{
  return false;
}

/**
  deliver the baudrate of the CAN BUS in [kbaud]
  @param rui8_busNr number of the BUS to check (default 0)
  @return BUS load of the last second [kbaud]
*/
int32_t can_stateGlobalBusload(uint8_t rui8_busNr)
{
  return 0;
}

/**
  check if send try of this MsgObj caused an Bit1Error
  (occurs if >1 controller try to send msg with same ident)
  ==> ISO 11783 forces stop of retries in this case for
      adress claim
      @param rui8_busNr number of the BUS to check
      @return true -> Bit1Err occured
*/
bool can_stateGlobalBit1err(uint8_t rui8_busNr)
{
  return false;
}

/* ***************************** */
/* ***Specific for one MsgObj*** */
/* ***************************** */

/**
  deliver the timestamp of last successfull CAN send action
  @param rui8_busNr number of the BUS to check  [0..1]
  @param rui8_msgobjNr number of the MsgObj to check [0..13]
  @return timestamp of last successful send
          OR -1 if rui8_msgObjNr corresponds to no valid send obj
*/
int32_t can_stateMsgobjTxok(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
{
  return true;
}

/**
  test if buffer of a MsgObj is full (e.g. no more
  msg can be put into buffer (important for TX objects))
  @param rui8_busNr number of the BUS to check
  @param rui8_msgobjNr number of the MsgObj to check [0..14]
  @return true -> buffer is full -> no further can_send allowed
*/
bool can_stateMsgobjOverflow(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
{
  return false;
}

/**
  deliver amount of messages in buffer
  (interesting for RX objects)
  @param rui8_busNr number of the BUS to check
  @param rui8_msgobjNr number of the MsgObj to check
  @return number of messages in buffer or negative error code:
    HAL_CONFIG_ERR == BUS not initialised
    HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_stateMsgobjBuffercnt(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
{
  return 1;
}

/**
  deliver amount of messages which can be placed in buffer
  (interesting for TX objects)
  @param rui8_busNr number of the BUS to check
  @param rui8_msgobjNr number of the MsgObj to check
  @return number of messages which can be placed in buffer or negative error code:
    HAL_CONFIG_ERR == BUS not initialised
    HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_stateMsgobjFreecnt(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
{
  return 1;
}
/**
  check if MsgObj is currently locked
  @param rui8_busNr number of the BUS to check
  @param rui8_msgobjNr number of the MsgObj to check
  @return true -> MsgObj is currently locked
*/
bool can_stateMsgobjLocked( uint8_t rui8_busNr, uint8_t rui8_msgobjNr )
{
  return false;
}
/* ***************************************************** */
/* ***************** Configuration ********************* */
/* ***************************************************** */


/* ******************** */
/* ***Global Per BUS*** */
/* ******************** */

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
int16_t can_configGlobalInit(uint8_t rui8_busNr, uint16_t rb_baudrate, uint16_t rb_maskStd, uint32_t rui32_maskExt, uint32_t rui32_maskLastmsg)
{
  CAN[rui8_busNr].Baudrate = rb_baudrate * 1000;
  LinkedTxFIFOInitPostPool(&arr_PostPool_CAN[rui8_busNr]);
  return HAL_NO_ERR;
}

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
int16_t can_configGlobalMask(uint8_t rui8_busNr, uint16_t rb_maskStd, uint32_t rui32_maskExt, uint32_t rui32_maskLastmsg)
{
  return HAL_NO_ERR;
}

/**
  close CAN BUS
  (important to close BUS before re-init with other baudrate)
  @param rui8_busNr number of the BUS to close (default 0)
  @return HAL_NO_ERR == no error;
          HAL_RANGE_ERR == wrong BUS nr;
          HAL_CONFIG_ERR == BUS previously not initialised
*/
int16_t can_configGlobalClose(uint8_t rui8_busNr)
{
  return HAL_NO_ERR;
}

/** wait until specified timeout or until next CAN message receive
 *  @return true -> there are CAN messages waiting for process. else: return due to timeout
 */
bool can_waitUntilCanReceiveOrTimeout( int32_t timeoutInterval )
{
    return false;
}


/* ***************************** */
/* ***Specific for one MsgObj*** */
/* ***************************** */

/**
  config a MsgObj
  (buffer size is defined locally in implementation of this interface;
  all received msgs should get a time stamp;
  class __IsoAgLib::Ident_c has ident and type 11/29bit)
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @param rrefc_ident filter ident of this MsgObj
  @param rb_rxtx 0==RX receive; 1==TX transmit
  @return HAL_NO_ERR == no error;
          C_BUSY == this MsgObj is already used
          HAL_CONFIG_ERR == BUS not initialised or error during buffer allocation
          HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_configMsgobjInit(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::Ident_c& rrefc_ident, uint8_t rb_rxtx)
{
  return HAL_NO_ERR;
}

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
int16_t can_configMsgobjChgid(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::Ident_c& rrefc_ident)
{
  return HAL_NO_ERR;
}

/**
  lock a MsgObj to avoid further placement of messages into buffer.
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @param rb_doLock true==lock(default); false==unlock
  @return HAL_NO_ERR == no error;
          HAL_CONFIG_ERR == BUS not initialised or ident can't be changed
          HAL_RANGE_ERR == wrong BUS or MsgObj number
  */
int16_t can_configMsgobjLock( uint8_t rui8_busNr, uint8_t rui8_msgobjNr, bool rb_doLock )
{
  return HAL_NO_ERR;
}

/**
  change the the send rate for one MsgObj by setting the minimum
  pause time between two messages [msec.]
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @param rui16_minSendPause minimum send pause between two sent messages [msec.]
  @return HAL_NO_ERR == no error;
          HAL_CONFIG_ERR == BUS not initialised or ident can't be changed
          HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_configMsgobjSendpause(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, uint16_t rui16_minSend)
{
  return HAL_NO_ERR;
}

/**
  close a MsgObj
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @return HAL_NO_ERR == no error;
          HAL_CONFIG_ERR == BUS not initialised, MsgObj previously not used or buffer memory not freed
          HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_configMsgobjClose(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
{
  return HAL_NO_ERR;
}

/* ***************************************************** */
/* ***************** Use of MsgObj ********************* */
/* ***************************************************** */

/**
  update the CAN BUS load statistic
  @param rui8_busNr BUS number to update
  @param rb_dlc length ot the detected (send or receive) message
*/
void updateCanBusLoad(uint8_t rui8_busNr, uint8_t rb_dlc)
{
}

/**
  send a message via a MsgObj;
  CanPkg_c (or derived object) must provide (virtual)
  functions:
  * MASK_TYPE ident() -> deliver ident value
  * __IsoAgLib::Ident_c::identType_t identType() -> deliver type of ident
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
int16_t can_useMsgobjSend(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::CanPkg_c* rpc_data)
{
  LinkedTxFIFOMessagePost_T *post = LinkedTxFIFOGetNextFreePoolPost(&arr_PostPool_CAN[rui8_busNr]);

  if (NULL != post)
  {
    tSend* pt_send = &t_cinterfMsgobjSend;
    rpc_data->getData(pt_send->dwId, pt_send->bXtd, pt_send->bDlc, pt_send->abData);

    post->Message.canID.ext.ID            = uint32_t(pt_send->dwId & 0x1FFFFFFF);
    post->Message.canID.info.isExtendedID = pt_send->bXtd;
    post->Message.dataLength              = pt_send->bDlc;

    post->Message.data.U8Field.byte0      = uint8_t((pt_send->abData[0] ) & 0xFF);
    post->Message.data.U8Field.byte1      = uint8_t((pt_send->abData[1] ) & 0xFF);
    post->Message.data.U8Field.byte2      = uint8_t((pt_send->abData[2] ) & 0xFF);
    post->Message.data.U8Field.byte3      = uint8_t((pt_send->abData[3] ) & 0xFF);
    post->Message.data.U8Field.byte4      = uint8_t((pt_send->abData[4] ) & 0xFF);
    post->Message.data.U8Field.byte5      = uint8_t((pt_send->abData[5] ) & 0xFF);
    post->Message.data.U8Field.byte6      = uint8_t((pt_send->abData[6] ) & 0xFF);
    post->Message.data.U8Field.byte7      = uint8_t((pt_send->abData[7] ) & 0xFF);

    LinkedTxFIFOIn(&(_FIFOHandler[rui8_busNr].TxFIFOList), post);

    return HAL_NO_ERR;
  }

  return HAL_OVERFLOW_ERR;
}

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
int32_t can_useMsgobjReceivedIdent(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, int32_t &reflIdent)
{
  return HAL_NO_ERR;
}

/**
  transfer front element in buffer into the pointed CanPkg_c;
  DON'T clear this item from buffer.
  @see can_useMsgobjPopFront for explicit clear of this front item
  functions:
  * void setIdent(MASK_TYPE rt_ident, Ident_c::identType_t rt_type)
    -> set ident rrefc_ident of received msg in CanPkg_c
  * uint8_t setDataFromString(uint8_t* rpb_data, uint8_t rb_dlc)
    -> set DLC in CanPkg_c from rb_dlc and insert data from uint8_t string rpb_data
  * void setTime(int32_t ri32_time) -> set receive time
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @param rpc_data pointer to CanPkg_c instance with data to send
  @return HAL_NO_ERR == no error;
          HAL_CONFIG_ERR == BUS not initialised, MsgObj is no RX object
          HAL_NOACT_ERR == BUS OFF
          HAL_OVERFLOW_ERR == send buffer overflowed
          HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_useMsgobjGet(uint8_t rui8_busNr, uint8_t rui8_msgobjNr, __IsoAgLib::CanPkg_c* rpc_data)
{
  return HAL_NO_ERR;;
}

/**
  Either register the currenct front item of buffer as not relevant,
  or just pop the front item, as it was processed.
  This explicit pop is needed, as one CAN message shall be served to
  several CANCustomer_c instances, as long as one of them indicates a
  succesfull process of the received message.
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
*/
void can_useMsgobjPopFront(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
{
}

/**
  clear the buffer of a MsgObj (e.g. to stop sending retries)
  @param rui8_busNr number of the BUS to config
  @param rui8_msgobjNr number of the MsgObj to config
  @return HAL_NO_ERR == no error;
          HAL_CONFIG_ERR == BUS not initialised
          HAL_RANGE_ERR == wrong BUS or MsgObj number
*/
int16_t can_useMsgobjClear(uint8_t rui8_busNr, uint8_t rui8_msgobjNr)
{
  return HAL_NO_ERR;
}


void getIrqData(void* inputData,_near HAL::fifoData_s* destination,uint8_t aui8_bXtd)
{
  CanMessage_T* input = (CanMessage_T*)inputData;

  destination->bXtd = (input->canID.info.isExtendedID? 1 : 0);
  destination->bDlc = input->dataLength;

  destination->abData[0] = input->data.U8Field.byte0;
  destination->abData[1] = input->data.U8Field.byte1;
  destination->abData[2] = input->data.U8Field.byte2;
  destination->abData[3] = input->data.U8Field.byte3;
  destination->abData[4] = input->data.U8Field.byte4;
  destination->abData[5] = input->data.U8Field.byte5;
  destination->abData[6] = input->data.U8Field.byte6;
  destination->abData[7] = input->data.U8Field.byte7;
}

int iWriteCentralCanFifo(const char bBus, const CanMessage_T* ap_msg) {
  HAL::iFifoWrite(bBus, -1, (ap_msg->canID.ext.ID & 0x1FFFFFFF), (void*)ap_msg);
  return 0;
}

} // end of namespace

