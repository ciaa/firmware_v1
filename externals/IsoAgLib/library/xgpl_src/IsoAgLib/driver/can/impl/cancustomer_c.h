/*
  cancustomer_c.h: header file for basic object for customer
    relationship to one or more FilterBox_c (-> CAN IO) instances

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef CAN_CUSTOMER_H
#define CAN_CUSTOMER_H

#include <IsoAgLib/driver/can/impl/ident_c.h>
#include <IsoAgLib/driver/can/impl/canpkg_c.h>
#include <IsoAgLib/driver/can/icanpkg_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/canpkgext_c.h>


// Begin Namespace __IsoAgLib
namespace __IsoAgLib
{
  // forward declarations
  class ReceiveStreamIdentifier_c;
  class Stream_c;


/** Base class for CAN customer classes
  -> FilterBox_c calls member function
    processMsg() to initiate processing of new received CAN telegram,
    which suits the configured filter/mask combination
  ==> all CAN handling object, which uses the services of FilterBox_c, CAN_IO
      should be derived from this class
  @see __IsoAgLib::CAN_IO
  @see __IsoAgLib::FilterBox
  *@author Dipl.-Inform. Achim Spangler
  */
class CanCustomer_c  {
public:

  /** virtual destructor */
  virtual ~CanCustomer_c() {}


/// SINGLE-PACKET METHODS
/// /////////////////////

  virtual void processMsg( const CanPkg_c& arc_data ) {
    process( static_cast<const IsoAgLib::iCanPkg_c&>( arc_data ) );
  }

  virtual void process( const IsoAgLib::iCanPkg_c& /* arc_data */ ) {}

/// MULTI-PACKET (TP/ETP) METHODS
/// /////////////////////////////

  /// Operation: reactOnStreamStart
  /// <!--@param ac_ident
  /// @param aui32_totalLen-->
  virtual bool reactOnStreamStart(
      ReceiveStreamIdentifier_c const &ac_ident,
      uint32_t aui32_totalLen)
  {
    return reactOnStreamStartDefault(ac_ident, aui32_totalLen);
  }
  bool reactOnStreamStartDefault (const ReceiveStreamIdentifier_c& /*ac_ident*/, uint32_t /*aui32_totalLen*/) { return false; }

  //  Operation: reactOnAbort
  virtual void reactOnAbort (Stream_c& arc_stream) { reactOnAbortDefault(arc_stream); }
  void reactOnAbortDefault (Stream_c& /*arc_stream*/) {}

  /// Operation: processPartStreamDataChunk
  ///
  //! <!--@param arc_stream: stream that is to be processed
  //! @param ab_isFirstChunk: is it the first chunk? do we have to perform some decision action?
  //! @param ab_isLastChunk: this also indicates that the MultiPacket-Message has been acknowledged via "End Of Message Acknowledge"!-->
  //! @return only of interest when "ab_isLastChunk==true":
  //!           true -> keep the stream (for later processing)
  //!           false -> don't keep the stream, let it be finished/removed right away.
  virtual bool processPartStreamDataChunk(
      Stream_c &apc_stream,
      bool ab_isFirstChunk,
      bool ab_isLastChunk)
  {
    return processPartStreamDataChunkDefault(
        apc_stream,
        ab_isFirstChunk,
        ab_isLastChunk);
  }
  bool processPartStreamDataChunkDefault(
      Stream_c &/*apc_stream*/, bool /*ab_isFirstChunk*/, bool /*ab_isLastChunk*/)
  {
    return false;
  }

  virtual void notificationOnMultiReceiveError(
      ReceiveStreamIdentifier_c const &ac_streamIdent,
      uint8_t aui8_multiReceiveError,
      bool ab_isGlobal) // needs not to be overwritten
  {
    notificationOnMultiReceiveErrorDefault(
        ac_streamIdent,
        aui8_multiReceiveError,
        ab_isGlobal);
  }
  void notificationOnMultiReceiveErrorDefault(
      ReceiveStreamIdentifier_c const &/*ac_streamIdent*/,
      uint8_t /*aui8_multiReceiveError*/,
      bool /*ab_isGlobal*/) {}
};

/** Proxy for CanCustomer_c.
  * Having such a proxy as component is an alternative to subclassing
  * CanCustomer_c directly.
  */
template < typename OWNER_T >
class CanCustomerProxy_c : public CanCustomer_c {
public:
  typedef OWNER_T Owner_t;

  CanCustomerProxy_c(Owner_t &art_owner) : mrt_owner(art_owner) {}

  virtual ~CanCustomerProxy_c() {}

private:
  virtual void processMsg( const CanPkg_c& arc_data ) {
    return mrt_owner.processMsg( arc_data );
  }

  virtual bool reactOnStreamStart(
      ReceiveStreamIdentifier_c const &ac_ident,
      uint32_t aui32_totalLen)
  {
    return mrt_owner.reactOnStreamStart(ac_ident, aui32_totalLen);
  }

  virtual void reactOnAbort(Stream_c &arc_stream)
  {
    mrt_owner.reactOnAbort(arc_stream);
  }

  virtual bool processPartStreamDataChunk(
      Stream_c &apc_stream,
      bool ab_isFirstChunk,
      bool ab_isLastChunk)
  {
    return mrt_owner.processPartStreamDataChunk(
        apc_stream,
        ab_isFirstChunk,
        ab_isLastChunk);
  }

  virtual void notificationOnMultiReceiveError(
      ReceiveStreamIdentifier_c const &ac_streamIdent,
      uint8_t aui8_multiReceiveError,
      bool ab_isGlobal)
  {
    mrt_owner.notificationOnMultiReceiveError(
        ac_streamIdent,
        aui8_multiReceiveError,
        ab_isGlobal);
  }

  // CanCustomerProxy_c shall not be copyable. Otherwise the
  // reference to the containing object would become invalid.
  CanCustomerProxy_c(CanCustomerProxy_c const &);

  CanCustomerProxy_c &operator=(CanCustomerProxy_c const &);

  Owner_t &mrt_owner;
};


} // end of namespace __IsoAgLib
#endif //CAN_CUSTOMER_H
