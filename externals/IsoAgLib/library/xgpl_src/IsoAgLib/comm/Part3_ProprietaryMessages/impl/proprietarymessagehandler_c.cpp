/*
  proprietarymessagehandler_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "proprietarymessagehandler_c.h"
#include "proprietarymessageclient_c.h"
#include <IsoAgLib/util/iassert.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/iisoitem_c.h>

#if defined(_MSC_VER)
#pragma warning( disable : 4355 )
#endif

namespace __IsoAgLib
{
  ProprietaryMessageHandler_c::ProprietaryMessageHandler_c()
    : m_customerA( *this )
    , m_customerB( *this )
  {
  }


  void
  ProprietaryMessageHandler_c::init()
  {
    isoaglib_assert (!initialized());
    setInitialized();
  }


  void
  ProprietaryMessageHandler_c::close()
  {
    isoaglib_assert (initialized());
    setClosed();
  }


  void ProprietaryMessageHandler_c::registerProprietaryMessage( ProprietaryMessageA_c& msg ) {
    isoaglib_assert(msg.ident());
    if( m_customerA.m_msgs.empty() ) {
      getCanInstance4Comm().insertFilter( m_customerA, m_customerA.m_filter, -1 );
      getMultiReceiveInstance4Comm().registerClientIso ( m_customerA,
                                                         msg.ident()->isoName(),
                                                         m_customerA.m_filter.getFilter() >> 8,
                                                         m_customerA.m_filter.getMask() >> 8,
                                                         true /* also Broadcast */ );
    }
    m_customerA.m_msgs.push_front( &msg );
  }


  void ProprietaryMessageHandler_c::deregisterProprietaryMessage( ProprietaryMessageA_c& msg ) {

    for ( CanCustomerA_c::ProprietaryMessageAVectorIterator_t it = m_customerA.m_msgs.begin(); it != m_customerA.m_msgs.end(); ++it ) {
      if( *it == &msg ) {
        m_customerA.m_msgs.erase( it );
        break;
      }
    }

    if( m_customerA.m_msgs.empty() ) {
      getCanInstance4Comm().deleteFilter( m_customerA, m_customerA.m_filter );
      isoaglib_assert(msg.ident());
      getMultiReceiveInstance4Comm().deregisterClient ( m_customerA,
                                                        msg.ident()->isoName(),
                                                        m_customerA.m_filter.getFilter() >> 8,
                                                        m_customerA.m_filter.getMask() >> 8 );
    }
  }


  void ProprietaryMessageHandler_c::registerProprietaryMessage( ProprietaryMessageB_c& msg, uint8_t ps ) {

    if( m_customerB.m_msgs[ ps ] == NULL ) {
      m_customerB.m_msgs[ ps ] = new CanCustomerB_c::ProprietaryMessageBVector_t();

      getCanInstance4Comm().insertFilter( m_customerB, m_customerB.m_filter, -1 );
      isoaglib_assert(msg.ident());
      getMultiReceiveInstance4Comm().registerClientIso ( m_customerB,
                                                         msg.ident()->isoName(),
                                                         m_customerB.m_filter.getFilter() >> 8,
                                                         m_customerB.m_filter.getMask() >> 8,
                                                         true /* also Broadcast */ );
    }
    m_customerB.m_msgs[ ps ]->push_front( &msg );
  }


  void ProprietaryMessageHandler_c::deregisterProprietaryMessage( ProprietaryMessageB_c& msg, uint8_t ps )
  {
    isoaglib_assert( m_customerB.m_msgs[ ps ] != NULL );

    for( CanCustomerB_c::ProprietaryMessageBVectorIterator_t it = m_customerB.m_msgs[ ps ]->begin(); it != m_customerB.m_msgs[ ps ]->end(); ++it )
    {
      if( *it == &msg )
      {
        m_customerB.m_msgs[ ps ]->erase( it );
        if( m_customerB.m_msgs[ ps ]->empty() )
        {
          delete m_customerB.m_msgs[ ps ];
          m_customerB.m_msgs[ ps ] = NULL;

          getCanInstance4Comm().deleteFilter( m_customerB, m_customerB.m_filter );
          isoaglib_assert(msg.ident());
          getMultiReceiveInstance4Comm().deregisterClient ( m_customerB,
                                                            msg.ident()->isoName(),
                                                            m_customerB.m_filter.getFilter() >> 8,
                                                            m_customerB.m_filter.getMask() >> 8 );
        }
        break;
      }
    }
  }


  void 
  ProprietaryMessageHandler_c::CanCustomerA_c::processMsg( const CanPkg_c& data )
  {
    CanPkgExt_c pkg( data, m_handler.getMultitonInst() );
    if( ! pkg.isValid() || ( pkg.getMonitorItemForSA() == NULL ) )
      return;

    for ( ProprietaryMessageAVectorIterator_t it = m_msgs.begin(); it != m_msgs.end(); ++it )
    {
      if ( ( pkg.getMonitorItemForDA() != NULL ) && ( pkg.getMonitorItemForDA() != (*it)->ident()->getIsoItem() ) )
        continue;

      if ( pkg.isoDp() != (*it)->dp() )
        continue;

      if ( (*it)->remote().isSpecified() && ( pkg.getISONameForSA() != (*it)->remote() ) )
        continue;

      (*it)->getDataReceive().clearVector();
      (*it)->getDataReceive().setDataStream( 0, pkg.getUint8DataConstPointer(), pkg.getLen() );
      // the static cast is prettier that a old C cast, but the Hightec GCC 3.4.6 is quite picky
      //(*it)->processA( *static_cast<IsoAgLib::iIsoItem_c*>( pkg.getMonitorItemForSA() ) );
      (*it)->processA( *( (IsoAgLib::iIsoItem_c*)( pkg.getMonitorItemForSA() ) ) );
    }
  }


  bool
  ProprietaryMessageHandler_c::CanCustomerA_c::reactOnStreamStart( const ReceiveStreamIdentifier_c &ident, uint32_t )
  {

    for ( ProprietaryMessageAVectorIterator_t it = m_msgs.begin(); it != m_msgs.end(); ++it )
    {

      if ( ( ident.getDaIsoName().isSpecified() ) && ( ident.getDaIsoName() != (*it)->ident()->isoName() ) )
        continue;

      if ( ( ident.getPgn() >> 16 ) != (*it)->dp() )
        continue;

      if ( (*it)->remote().isSpecified() && ( ident.getSaIsoName() != (*it)->remote() ) )
        continue;

      return true;
    }

    return false;
  }


  bool
  ProprietaryMessageHandler_c::CanCustomerA_c::processPartStreamDataChunk( Stream_c &apc_stream, bool, bool last )
  {
    if( ! last ) {
      return false;
    }

    // get the ident from the stream
    const ReceiveStreamIdentifier_c& ident = apc_stream.getIdent();


    ProprietaryMessageAVector_t msgs;

    for ( ProprietaryMessageAVectorIterator_t it = m_msgs.begin(); it != m_msgs.end(); ++it )
    {
      if ( ( ident.getDaIsoName().isSpecified() ) && ( ident.getDaIsoName() != (*it)->ident()->isoName() ) )
        continue;

      if ( ( ident.getPgn() >> 16 ) != (*it)->dp() )
        continue;

      if ( (*it)->remote().isSpecified() && ( ident.getSaIsoName() != (*it)->remote() ) )
        continue;

      (*it)->getDataReceive().clearVector();
      msgs.push_front( *it );
    }


    if( ! msgs.empty() ) {

      uint8_t db = apc_stream.getFirstByte();
      for ( unsigned cnt = 0; cnt < apc_stream.getByteTotalSize(); cnt++)
      {

        for( ProprietaryMessageAVectorIterator_t it = msgs.begin(); it != msgs.end(); ++it ) {
          (*it)->getDataReceive().setDataUi8( cnt, db );
        }
        db = apc_stream.getNextNotParsed();
      }

      for( ProprietaryMessageAVectorIterator_t it = msgs.begin(); it != msgs.end(); ++it ) {
        // the static cast is prettier that a old C cast, but the Hightec GCC 3.4.6 is quite picky
        //(*it)->processA( *static_cast<IsoAgLib::iIsoItem_c*>( getIsoMonitorInstance( m_handler.getMultitonInst() ).isoMemberNrFast( ident.getSa() ) ) );
        (*it)->processA( * ( IsoAgLib::iIsoItem_c*)( getIsoMonitorInstance( m_handler.getMultitonInst() ).isoMemberNrFast( ident.getSa() ) ) );
      }
    }
    // don't keep the stream - we processed it right now!
    return false;

  }


  void 
  ProprietaryMessageHandler_c::CanCustomerB_c::processMsg( const CanPkg_c& data )
  {
    CanPkgExt_c pkg( data, m_handler.getMultitonInst() );
    if( ! pkg.isValid() || ( pkg.getMonitorItemForSA() == NULL ) )
      return;

    const unsigned ps = pkg.isoPs();
    if( m_msgs[ ps ] == NULL )
      return;

    for ( ProprietaryMessageBVectorIterator_t it = m_msgs[ ps ]->begin(); it != m_msgs[ ps ]->end(); ++it )
    {
      if ( pkg.isoDp() != (*it)->dp() )
        continue;

      if ( (*it)->remote().isSpecified() && ( pkg.getISONameForSA() != (*it)->remote() ) )
        continue;

      (*it)->getDataReceive().clearVector();
      (*it)->getDataReceive().setDataStream( 0, pkg.getUint8DataConstPointer(), pkg.getLen() );

      // the static cast is prettier that a old C cast, but the Hightec GCC 3.4.6 is quite picky
      //(*it)->processB( *static_cast<IsoAgLib::iIsoItem_c*>( pkg.getMonitorItemForSA() ) );
      (*it)->processB( *( ( IsoAgLib::iIsoItem_c*)( pkg.getMonitorItemForSA() ) ), ps );
    }
  }


  bool
  ProprietaryMessageHandler_c::CanCustomerB_c::reactOnStreamStart( const ReceiveStreamIdentifier_c &ident, uint32_t )
  {
    const unsigned ps = ident.getPgn() & 0xFF;
    const unsigned dp = ident.getPgn() >> 16;

    if( m_msgs[ ps ] == NULL )
      return false;

    for ( ProprietaryMessageBVectorIterator_t it = m_msgs[ ps ]->begin(); it != m_msgs[ ps ]->end(); ++it )
    {
      if ( dp != (*it)->dp() )
        continue;

      if ( (*it)->remote().isSpecified() && ( ident.getSaIsoName() != (*it)->remote() ) )
        continue;

      return true;
    }

    return false;
  }


  bool
  ProprietaryMessageHandler_c::CanCustomerB_c::processPartStreamDataChunk( Stream_c &apc_stream, bool, bool last )
  {
    if( ! last )
      return false;

    const ReceiveStreamIdentifier_c& ident = apc_stream.getIdent();
    const unsigned ps = ident.getPgn() & 0xFF;
    const unsigned dp = ident.getPgn() >> 16;

    if( m_msgs[ ps ] == NULL )
      return false;

    ProprietaryMessageBVector_t msgs;

    for ( ProprietaryMessageBVectorIterator_t it = m_msgs[ ps ]->begin(); it != m_msgs[ ps ]->end(); ++it )
    {
      if ( dp != (*it)->dp() )
        continue;

      if ( (*it)->remote().isSpecified() && ( ident.getSaIsoName() != (*it)->remote() ) )
        continue;

      (*it)->getDataReceive().clearVector();
      msgs.push_front( *it );
    }


    if( ! msgs.empty() )
    {
      uint8_t db = apc_stream.getFirstByte();
      for( unsigned cnt = 0; cnt < apc_stream.getByteTotalSize(); cnt++)
      {
        for( ProprietaryMessageBVectorIterator_t it = msgs.begin(); it != msgs.end(); ++it ) {
          (*it)->getDataReceive().setDataUi8( cnt, db );
        }
        db = apc_stream.getNextNotParsed();
      }

      for( ProprietaryMessageBVectorIterator_t it = msgs.begin(); it != msgs.end(); ++it ) {
        // the static cast is prettier that a old C cast, but the Hightec GCC 3.4.6 is quite picky
        //(*it)->processB( *static_cast<IsoAgLib::iIsoItem_c*>( getIsoMonitorInstance( m_handler.getMultitonInst() ).isoMemberNrFast( ident.getSa() ) ) );
        (*it)->processB( *( IsoAgLib::iIsoItem_c*)( getIsoMonitorInstance( m_handler.getMultitonInst() ).isoMemberNrFast( ident.getSa() ) ), ps );
      }
    }
    // don't keep the stream - we processed it right now!
    return false;

  }


  ProprietaryMessageHandler_c &getProprietaryMessageHandlerInstance(uint8_t aui8_instance)
  {
    MACRO_MULTITON_GET_INSTANCE_BODY(ProprietaryMessageHandler_c, PRT_INSTANCE_CNT, aui8_instance);
  }

} // namespace __IsoAgLib
