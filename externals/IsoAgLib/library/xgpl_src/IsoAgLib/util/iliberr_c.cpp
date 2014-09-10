/*
  iliberr_c.cpp: central object for error management

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include <IsoAgLib/isoaglib_config.h>
#include <cstring>
#include "iliberr_c.h"


namespace IsoAgLib {

iLibErr_c &getILibErrInstance()
{
  MACRO_SINGLETON_GET_INSTANCE_BODY( iLibErr_c );
}


void iLibErr_c::init()
{
  CNAMESPACE::memset( &m_nonFatal, 0, TypeNonFatalSize * sizeof( uint16_t ) );
  CNAMESPACE::memset( &m_fatal, 0, TypeNonFatalSize * sizeof( uint16_t ) );
}


void
iLibErr_c::close()
{
}


iLibErr_c::iLibErr_c() :
  CONTAINER_CLIENT1_CTOR_INITIALIZER_LIST
{
}


void iLibErr_c::registerNonFatal( TypeNonFatal_en at_errType, int instance )
{
  m_nonFatal[ at_errType ] |= uint16_t( 1 << instance );

  #ifdef OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED
  for ( STL_NAMESPACE::vector<iErrorObserver_c*,MALLOC_TEMPLATE(iErrorObserver_c*)>::iterator pc_iter = m_arrClientC1.begin(); ( pc_iter != m_arrClientC1.end() ); ++pc_iter )
  #else
  for ( STL_NAMESPACE::vector<iErrorObserver_c*>::iterator pc_iter = m_arrClientC1.begin(); ( pc_iter != m_arrClientC1.end() ); ++pc_iter )
  #endif
  {
    (*pc_iter)->nonFatalError( at_errType, instance );
  }
}


void iLibErr_c::registerFatal( TypeFatal_en at_errType, int instance )
{
  m_fatal[ at_errType ] |= uint16_t( 1 << instance );

  #ifdef OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED
  for ( STL_NAMESPACE::vector<iErrorObserver_c*,MALLOC_TEMPLATE(iErrorObserver_c*)>::iterator pc_iter = m_arrClientC1.begin(); ( pc_iter != m_arrClientC1.end() ); ++pc_iter )
  #else
  for ( STL_NAMESPACE::vector<iErrorObserver_c*>::iterator pc_iter = m_arrClientC1.begin(); ( pc_iter != m_arrClientC1.end() ); ++pc_iter )
  #endif
  {
    (*pc_iter)->fatalError( at_errType, instance );
  }
}

} // end of namespace IsoAgLib
