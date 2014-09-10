/*
  isorequestpgnhandler_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISO_REQUEST_PGN_HANDLER_C_H
#define ISO_REQUEST_PGN_HANDLER_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/driver/can/impl/cancustomer_c.h>
/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */

namespace __IsoAgLib {


class IsoItem_c;
/**
  This class implements the handling of Request_For_PGN

  @author Dipl.-Inf.(FH) Martina Winkler
*/
class IsoRequestPgnHandler_c
{

public:
  virtual ~IsoRequestPgnHandler_c() {};
  // IsoItem_c is intentionally not const, because the internal IsoAgLib classes are allowed to e.g. use sendSaClaim().
  virtual bool processMsgRequestPGN(
      uint32_t aui32_pgn,
      IsoItem_c *apc_isoItemSender,
      IsoItem_c *apc_isoItemReceiver,
      int32_t ai_requestTimestamp ) = 0;

};

/** Proxy for IsoRequestPgnHandler_c.
  * Having such a proxy as component is an alternative to subclassing
  * IsoRequestPgnHandler_c directly.
  */
template < typename OWNER_T >
class IsoRequestPgnHandlerProxy_c : public IsoRequestPgnHandler_c {
public:
  typedef OWNER_T Owner_t;

  IsoRequestPgnHandlerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}

  virtual ~IsoRequestPgnHandlerProxy_c() {}

private:
  virtual bool processMsgRequestPGN(
      uint32_t aui32_pgn,
      IsoItem_c *apc_isoItemSender,
      IsoItem_c *apc_isoItemReceiver,
      int32_t ai_time )
  {
    return mrt_owner.processMsgRequestPGN(
        aui32_pgn,
        apc_isoItemSender,
        apc_isoItemReceiver,
        ai_time );
  }

  // IsoRequestPgnHandlerProxy_c shall not be copyable. Otherwise
  // the reference to the containing object would become invalid.
  IsoRequestPgnHandlerProxy_c(IsoRequestPgnHandlerProxy_c const &);

  IsoRequestPgnHandlerProxy_c &operator=(IsoRequestPgnHandlerProxy_c const &);

  Owner_t &mrt_owner;
};

}
#endif
