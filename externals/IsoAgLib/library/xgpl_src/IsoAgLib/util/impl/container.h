/*
  container.h: C-style Macros for added container(s) to classes.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>

#define CONTAINER_CLIENTx_CTOR_INITIALIZER_LIST(m_arrClientCx) \
  m_arrClientCx() \

#define CONTAINER_CLIENT1_CTOR_INITIALIZER_LIST CONTAINER_CLIENTx_CTOR_INITIALIZER_LIST (m_arrClientC1)
#define CONTAINER_CLIENT2_CTOR_INITIALIZER_LIST CONTAINER_CLIENTx_CTOR_INITIALIZER_LIST (m_arrClientC2)
#define CONTAINER_CLIENT3_CTOR_INITIALIZER_LIST CONTAINER_CLIENTx_CTOR_INITIALIZER_LIST (m_arrClientC3)

#ifdef OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED
  #define CONTAINER_CLIENTx_MEMBERS(Cx, iterCx_t, const_iterCx_t, m_arrClientCx) \
  private: \
    typedef STL_NAMESPACE::vector<Cx*,MALLOC_TEMPLATE(Cx*)>::iterator iterCx_t; \
    typedef STL_NAMESPACE::vector<Cx*,MALLOC_TEMPLATE(Cx*)>::const_iterator const_iterCx_t; \
    STL_NAMESPACE::vector<Cx*,MALLOC_TEMPLATE(Cx*)> m_arrClientCx;
#else
  #define CONTAINER_CLIENTx_MEMBERS(Cx, iterCx_t, const_iterCx_t, m_arrClientCx) \
  private: \
    typedef STL_NAMESPACE::vector<Cx*>::iterator iterCx_t; \
    typedef STL_NAMESPACE::vector<Cx*>::const_iterator const_iterCx_t; \
    STL_NAMESPACE::vector<Cx*> m_arrClientCx;
#endif


#define CONTAINER_CLIENTx_MEMBER_FUNCTIONS_MAIN(registerCx,unregisterCx,Cx,iterCx_t,const_iterCx_t,m_arrClientCx) \
public: \
  void registerCx( Cx* pc_client ) \
  { \
    m_arrClientCx.push_back( pc_client ); \
  } \
\
  void unregisterCx( Cx* pc_client ) \
  { \
    for( STL_NAMESPACE::vector<Cx*>::iterator iter = m_arrClientCx.begin(); \
         iter != m_arrClientCx.end(); \
         ++iter ) \
    { \
      if ( *iter == pc_client ) \
      { \
        m_arrClientCx.erase( iter ); \
        break; \
      } \
    } \
  } \
\
CONTAINER_CLIENTx_MEMBERS(Cx,iterCx_t,const_iterCx_t,m_arrClientCx)


#define CONTAINER_CLIENT1_MEMBER_FUNCTIONS_MAIN(C1) CONTAINER_CLIENTx_MEMBER_FUNCTIONS_MAIN(registerC1,unregisterC1,C1,iterC1_t,const_iterC1_t,m_arrClientC1)
#define CONTAINER_CLIENT2_MEMBER_FUNCTIONS_MAIN(C2) CONTAINER_CLIENTx_MEMBER_FUNCTIONS_MAIN(registerC2,unregisterC2,C2,iterC2_t,const_iterC2_t,m_arrClientC2)
#define CONTAINER_CLIENT3_MEMBER_FUNCTIONS_MAIN(C3) CONTAINER_CLIENTx_MEMBER_FUNCTIONS_MAIN(registerC3,unregisterC3,C3,iterC3_t,const_iterC3_t,m_arrClientC3)

#endif // CONTAINER_H
