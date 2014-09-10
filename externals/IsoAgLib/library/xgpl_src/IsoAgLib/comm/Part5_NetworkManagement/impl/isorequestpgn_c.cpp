/*
  isomonitor_c.cpp: object for monitoring members (list of IsoItem_c)

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "isorequestpgn_c.h"
#include "isorequestpgnhandler_c.h"
#include "isomonitor_c.h"

#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/util/iassert.h>
#include <algorithm>


namespace __IsoAgLib {


void
IsoRequestPgn_c::init()
{
  isoaglib_assert (!mc_subsystemState.initialized());

  getIsoBusInstance4Comm().insertFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FF0000UL, REQUEST_PGN_MSG_PGN  << 8 ), 3 );

  mc_subsystemState.setInitialized();
}


void
IsoRequestPgn_c::close()
{
  isoaglib_assert (mc_subsystemState.initialized());

  getIsoBusInstance4Comm().deleteFilter( *this, IsoAgLib::iMaskFilter_c( 0x3FF0000UL, REQUEST_PGN_MSG_PGN << 8 ) );

  mc_subsystemState.setClosed();
}


bool
IsoRequestPgn_c::registerPGN(
    IsoRequestPgnHandler_c &r_PGNHandler,
    const uint32_t cui32_pgnToRegister,
    uint32_t aui32_pgnMask)
{
  if (checkIfAlreadyRegistered (r_PGNHandler, cui32_pgnToRegister, aui32_pgnMask))
    return false; // false could also mean that the PGN - client pair is already inserted in list

  PGN_s s_pgnToRegister(&r_PGNHandler, cui32_pgnToRegister, aui32_pgnMask);
  m_registeredClientsWithPGN.push_back (s_pgnToRegister);

  return true; // PGN - client pair didn't exist, so it was added
}


/** adds n PGN for the client to the list */
bool
IsoRequestPgn_c::registerPGN (IsoRequestPgnHandler_c &r_PGNHandler, const uint8_t cui8_pgnCount, const uint32_t *pcui32_pgnToRegister)
{
  uint8_t ui8_index = 0;
  bool b_msgProcessed = false;
  for (;ui8_index < cui8_pgnCount; ui8_index++)
  {
    bool const cb_set = !checkIfAlreadyRegistered (r_PGNHandler, *pcui32_pgnToRegister) &&
      registerPGN(r_PGNHandler, *pcui32_pgnToRegister);
    if (cb_set)
      b_msgProcessed = true;
    pcui32_pgnToRegister++;
  }
  return b_msgProcessed;
}


/** remove PGN from the list */
void
IsoRequestPgn_c::unregisterPGN(
    IsoRequestPgnHandler_c &r_PGNHandler,
    const uint32_t cui32_pgnToRegister,
    uint32_t aui32_pgnMask)
{
  PGN_s const cs_lookFor(&r_PGNHandler, cui32_pgnToRegister, aui32_pgnMask);
  for (STL_NAMESPACE::vector<PGN_s>::iterator regPGN_it = m_registeredClientsWithPGN.begin();
       regPGN_it != m_registeredClientsWithPGN.end();
      )
  {
    if (cui32_pgnToRegister == 0)
    { // every registered PGN will be deleted
      if (regPGN_it->p_handler == &r_PGNHandler)
        regPGN_it = m_registeredClientsWithPGN.erase (regPGN_it);
      else
        ++regPGN_it;
    }
    else
    { // only the cui32_pgnToRegister will be deleted
      if (cs_lookFor == *regPGN_it)
      {
        m_registeredClientsWithPGN.erase (regPGN_it);
        break; // the PGN is unique for the RequestPGNHandler, so we can leave the loop
      }
      else
        ++regPGN_it;
    }
  }
}


/** removes n PGN for the client from the list */
void
IsoRequestPgn_c::unregisterPGN (IsoRequestPgnHandler_c &r_PGNHandler, const uint8_t cui8_pgnCount, const uint32_t *pcui32_pgnToUnregister)
{
  if (cui8_pgnCount == 0) return;

  uint8_t ui8_index = 0;
  for (; ui8_index < cui8_pgnCount; ui8_index++, pcui32_pgnToUnregister++)
    unregisterPGN (r_PGNHandler, *pcui32_pgnToUnregister);
}


/** before adding any further PGN - RequestPGNHandler pair, check if not already existing */
bool
IsoRequestPgn_c::checkIfAlreadyRegistered(
    IsoRequestPgnHandler_c &r_PGNHandler,
    const uint32_t cui32_pgn,
    uint32_t aui32_pgnMask)
{
  PGN_s const cs_lookFor(&r_PGNHandler, cui32_pgn, aui32_pgnMask);
  bool const bc_found = m_registeredClientsWithPGN.end() != STL_NAMESPACE::find(
      m_registeredClientsWithPGN.begin(),
      m_registeredClientsWithPGN.end(),
      cs_lookFor);
  return bc_found;
}


/** process REQUEST_PGN_MSG_PGN message
    Since we only insertFilter for REQUEST_PGN_MSG_PGN we don't need further checking
    a la "if ((data().isoPgn() & 0x3FF00) == REQUEST_PGN_MSG_PGN)" */
void
IsoRequestPgn_c::processMsg ( const CanPkg_c& arc_data )
{
  CanPkgExt_c pkg( arc_data, getMultitonInst() );
  if( !pkg.isValid() ) // allow packets with "(pkg.getMonitorItemForSA() == NULL)"
    return;

  /// Store incoming information for possible later user-triggered "sendAcknowledgePGN()"
  mpc_isoItemSA = pkg.getMonitorItemForSA();
  mpc_isoItemDA = pkg.getMonitorItemForDA();

  if( ( mpc_isoItemDA != NULL ) && ( ! mpc_isoItemDA->itemState( IState_c::Local ) ) ) // omit DA specific for non local receivers
    return;

  mui32_requestedPGN = ( (static_cast<uint32_t>(pkg.operator[](0)))
                      | (static_cast<uint32_t>(pkg.operator[](1)) << 8)
                      | (static_cast<uint32_t>(pkg.operator[](2)) << 16) );

  /// 1. Distribute to all clients
  bool b_processedByAnyClient = false;
  for (STL_NAMESPACE::vector<PGN_s>::iterator it_pgn = m_registeredClientsWithPGN.begin();;) {

    it_pgn = STL_NAMESPACE::find_if(
        it_pgn,
        m_registeredClientsWithPGN.end(),
        DoesMatchPgn_s(mui32_requestedPGN));
    if (it_pgn == m_registeredClientsWithPGN.end())
      break;
    bool const cb_set = it_pgn->p_handler->processMsgRequestPGN(mui32_requestedPGN, mpc_isoItemSA, mpc_isoItemDA, pkg.time() );
    if (cb_set)
      b_processedByAnyClient = true;
    ++it_pgn;
  }

  /// 2. Check if we have to send a NACK as nobody could answer it
  if (!b_processedByAnyClient)
  { // no client could answer the Request PGN
    if ( (mpc_isoItemDA != NULL) && (mpc_isoItemSA != NULL) )
    { // and it was Destination specific
      answerRequestPGNwithACK (*mpc_isoItemDA, 0x01);  // so "NOT Acknowledge (NACK)" it (Control Byte 0x01)
    }
    // else: Don't NACK if it was requested to global!
  }
}


void
IsoRequestPgn_c::sendAcknowledgePGN (IsoItem_c& arc_isoItemSender, uint8_t aui8_ackType)
{
  uint32_t ui32_purePgn = mui32_requestedPGN;
  if (((ui32_purePgn >> 8) & 0xFF) < 0xF0)
  { // destination specific, so clear the destSA field as we want the PURE PGN!
    ui32_purePgn &= 0x3FF00LU;
  }

  CanPkgExt_c isoPkg;
  isoPkg.setIsoPri(6);
  isoPkg.setIsoDp(0);
  isoPkg.setIsoPf(ACKNOWLEDGEMENT_PGN >> 8);
  isoPkg.setMonitorItemForDA (mpc_isoItemSA);
  isoPkg.setMonitorItemForSA (&arc_isoItemSender);
  // set the first four bytes as uint32_t value, where lowest byte equals to ControlByte
  isoPkg.setUint32Data ((1-1), (0xFFFFFF00UL | uint32_t (aui8_ackType)));
  // set at lowest byte of second uint32_t value the reserved 0xFF
  // and place at the higher bytes of this second uint32_t
  // the ui32_purePgn
  isoPkg.setUint32Data ((5-1), ((ui32_purePgn << 8)|0xFFUL) );
  isoPkg.setLen (8);

  __IsoAgLib::getIsoBusInstance4Comm() << isoPkg;
}


IsoRequestPgn_c::IsoRequestPgn_c ()
  : mc_subsystemState()
  , m_registeredClientsWithPGN ()
  , mpc_isoItemSA( NULL ) // dummy value, is always properly set when used
  , mpc_isoItemDA( NULL ) // dummy value, is always properly set when used
  , mui32_requestedPGN( 0xFFFFFFFFLU ) // dummy value, is always properly set when used
  , mb_alreadyClosed( true )
{
}


/** C-style function, to get access to the unique IsoRequestPgn_c singleton instance
 * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS */
IsoRequestPgn_c &getIsoRequestPgnInstance(unsigned int instance)
{
  MACRO_MULTITON_GET_INSTANCE_BODY(IsoRequestPgn_c, PRT_INSTANCE_CNT, instance);
}

} // end of namespace __IsoAgLib
