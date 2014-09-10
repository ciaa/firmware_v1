/*
  proprietarymessagehandler_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef PROPRIETARYMESSAGEHANDLER_H
#define PROPRIETARYMESSAGEHANDLER_H

#include "proprietarymessageclient_c.h"

#include <IsoAgLib/isoaglib_config.h>

#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisend_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multireceive_c.h>

#include <cstring>
#include <list>


namespace __IsoAgLib
{


  class ProprietaryMessageHandler_c : public Subsystem_c
  {
    MACRO_MULTITON_CONTRIBUTION();
  public:
    ProprietaryMessageHandler_c();

    virtual ~ProprietaryMessageHandler_c() {}

    void init();
    void close();

    void registerProprietaryMessage( ProprietaryMessageA_c& msg );
    void registerProprietaryMessage( ProprietaryMessageB_c& msg, uint8_t ps );

    void deregisterProprietaryMessage( ProprietaryMessageA_c& msg );
    void deregisterProprietaryMessage( ProprietaryMessageB_c& msg, uint8_t ps );

  private:


    class CanCustomerA_c : public CanCustomer_c {
      public:
        CanCustomerA_c( ProprietaryMessageHandler_c& handler ) : m_filter( 0x00FF0000, PROPRIETARY_A_PGN << 8, IsoAgLib::iIdent_c::ExtendedIdent ), m_handler( handler )  {}
        virtual ~CanCustomerA_c() {}


        typedef STL_NAMESPACE::list<ProprietaryMessageA_c*> ProprietaryMessageAVector_t;
        typedef STL_NAMESPACE::list<ProprietaryMessageA_c*>::iterator ProprietaryMessageAVectorIterator_t;
        typedef STL_NAMESPACE::list<ProprietaryMessageA_c*>::const_iterator ProprietaryMessageAVectorConstIterator_t;

        ProprietaryMessageAVector_t m_msgs;

        const IsoAgLib::iMaskFilterType_c m_filter; // A1 and A2

      private:
        virtual void processMsg( const CanPkg_c& arc_data );

        virtual bool reactOnStreamStart( const ReceiveStreamIdentifier_c& ident, uint32_t len );
        virtual void reactOnAbort( Stream_c & ) { }
        virtual bool processPartStreamDataChunk( Stream_c &apc_stream, bool first, bool last );
        virtual void notificationOnMultiReceiveError( ReceiveStreamIdentifier_c const &, uint8_t, bool ) { }

        ProprietaryMessageHandler_c& m_handler;

      private:
        /** not copyable : copy constructor is only declared, never defined */
        CanCustomerA_c(const CanCustomerA_c&);
        /** not copyable : copy operator is only declared, never defined */
        CanCustomerA_c& operator=(const CanCustomerA_c&);
    };


    class CanCustomerB_c : public CanCustomer_c {
      public:
        CanCustomerB_c( ProprietaryMessageHandler_c& handler ) : m_filter( 0x00FF0000, PROPRIETARY_B_PGN << 8, IsoAgLib::iIdent_c::ExtendedIdent ), m_handler( handler )
        { CNAMESPACE::memset( m_msgs, 0x00, sizeof( m_msgs ) ); }

        virtual ~CanCustomerB_c() {}


        typedef STL_NAMESPACE::list<ProprietaryMessageB_c*> ProprietaryMessageBVector_t;
        typedef STL_NAMESPACE::list<ProprietaryMessageB_c*>::iterator ProprietaryMessageBVectorIterator_t;
        typedef STL_NAMESPACE::list<ProprietaryMessageB_c*>::const_iterator ProprietaryMessageBVectorConstIterator_t;

        ProprietaryMessageBVector_t* m_msgs[256];

        const IsoAgLib::iMaskFilterType_c m_filter; // B1 and B2

      private:
        virtual void processMsg( const CanPkg_c& arc_data );

        virtual bool reactOnStreamStart( const ReceiveStreamIdentifier_c& ident, uint32_t len );
        virtual void reactOnAbort( Stream_c & ) { }
        virtual bool processPartStreamDataChunk( Stream_c &apc_stream, bool first, bool last );
        virtual void notificationOnMultiReceiveError( ReceiveStreamIdentifier_c const &, uint8_t, bool ) { }

        ProprietaryMessageHandler_c& m_handler;

      private:
        /** not copyable : copy constructor is only declared, never defined */
        CanCustomerB_c(const CanCustomerB_c&);
        /** not copyable : copy operator is only declared, never defined */
        CanCustomerB_c& operator=(const CanCustomerB_c&);
    };


  private:
    CanCustomerA_c m_customerA;
    CanCustomerB_c m_customerB;

    friend ProprietaryMessageHandler_c &getProprietaryMessageHandlerInstance(uint8_t aui8_instance);
  };

  /** C-style function, to get access to the unique ProprietaryMesageHandler_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  ProprietaryMessageHandler_c &getProprietaryMessageHandlerInstance(uint8_t aui8_instance);

}
#endif


